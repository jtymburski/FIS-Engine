/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#include "Game/Battle/BattleEvent.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const float BattleEvent::kOFF_PHYS_MODIFIER = 1.00;
const float BattleEvent::kDEF_PHYS_MODIFIER = 1.10;
const float BattleEvent::kOFF_PRIM_ELM_MATCH_MODIFIER = 1.03;
const float BattleEvent::kDEF_PRIM_ELM_MATCH_MODIFIER = 1.03;
const float BattleEvent::kOFF_SECD_ELM_MATCH_MODIFIER = 1.02;
const float BattleEvent::kDEF_SECD_ELM_MATCH_MODIFIER = 1.01;

const float BattleEvent::kOFF_PRIM_ELM_MODIFIER = 1.00;
const float BattleEvent::kDEF_PRIM_ELM_MODIFIER = 1.00;
const float BattleEvent::kOFF_SECD_ELM_MODIFIER = 1.00;
const float BattleEvent::kDEF_SECD_ELM_MODIFIER = 1.00;

const float BattleEvent::kPRIM_ELM_ADV_MODIFIER = 1.10;
const float BattleEvent::kPRIM_ELM_DIS_MODIFIER = 0.90;
const float BattleEvent::kSECD_ELM_ADV_MODIFIER = 1.07;
const float BattleEvent::kSECD_ELM_DIS_MODIFIER = 0.95;
const float BattleEvent::kDOUBLE_ELM_ADV_MODIFIER = 1.20;
const float BattleEvent::kDOUBLE_ELM_DIS_MODIFIER = 0.80;

const float BattleEvent::kMANNA_POW_MODIFIER = 1.20;
const float BattleEvent::kMANNA_DEF_MODIFIER = 1.20;
const float BattleEvent::kUSER_POW_MODIFIER = 1.70;
const float BattleEvent::kTARG_DEF_MODIFIER = 2.90;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTOR
 *============================================================================*/

BattleEvent::BattleEvent()
    : action_index{0},
      action_type{ActionType::NONE},
      actor{nullptr},
      actor_targets{},
      event_skill{nullptr},
      flags_ignore{static_cast<IgnoreState>(0)},
      event_item{nullptr},
      attr_prio{Attribute::NONE},
      attr_prid{Attribute::NONE},
      attr_seco{Attribute::NONE},
      attr_secd{Attribute::NONE},
      attr_user{Attribute::NONE},
      attr_targ{Attribute::NONE}
{
}

BattleEvent::BattleEvent(ActionType type, BattleActor* actor,
                         std::vector<BattleActor*> targets)
    : BattleEvent()
{
  action_type = type;
  this->actor = actor;
  actor_targets = targets;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void BattleEvent::calcActionVariables()
{
  auto curr_skill = getCurrSkill();
  auto prim = Helpers::elementToStats(Element::PHYSICAL);
  auto secd = Helpers::elementToStats(Element::PHYSICAL);

  if(action_type == ActionType::SKILL || action_type == ActionType::ITEM)
  {
    prim = Helpers::elementToStats(curr_skill->getPrimary());
    secd = Helpers::elementToStats(curr_skill->getSecondary());
  }
  else if(action_type == ActionType::IMPLODE)
  {
    prim = Helpers::elementToStats(actor->getBasePerson()->getPrimary());
    secd = Helpers::elementToStats(actor->getBasePerson()->getSecondary());
  }

  attr_prio = prim.first;
  attr_prid = prim.second;
  attr_seco = secd.first;
  attr_secd = secd.second;

  /* User ref. vars rel. to prim/secd skill attributes, -1 if Attr:NONE */
  auto prim_user_off = temp_user_stats.getValue(attr_prio);
  auto prim_user_def = temp_user_stats.getValue(attr_prid);
  auto secd_user_off = temp_user_stats.getValue(attr_seco);
  auto secd_user_def = temp_user_stats.getValue(attr_secd);

  if(action_type == ActionType::SKILL)
  {
    if(actor->getBasePerson()->getPrimary() == curr_skill->getPrimary())
    {
      prim_user_off *= kOFF_PRIM_ELM_MODIFIER;
      prim_user_def *= kDEF_PRIM_ELM_MODIFIER;
    }
    else if(actor->getBasePerson()->getSecondary() ==
            curr_skill->getSecondary())
    {
      secd_user_off *= kOFF_SECD_ELM_MODIFIER;
      secd_user_def *= kDEF_SECD_ELM_MODIFIER;
    }
  }

  temp_user_stats.setBaseValue(attr_prio, prim_user_off);
  temp_user_stats.setBaseValue(attr_prid, prim_user_def);
  temp_user_stats.setBaseValue(attr_seco, secd_user_off);
  temp_user_stats.setBaseValue(attr_secd, secd_user_def);
}

void BattleEvent::calcElementalMods(BattleActor* curr_target)
{
  auto curr_skill = getCurrSkill();

  assert(curr_skill && action_index < temp_targ_stats.size());

  /* Grab the temp attribute set for the curent processing target index */
  auto targ_attrs = temp_targ_stats.at(action_index);

  /* Determine the correct stats to apply the modifier to */
  auto prim_user_stat = Attribute::NONE;
  auto secd_user_stat = Attribute::NONE;
  auto prim_targ_stat = Attribute::NONE;
  auto secd_targ_stat = Attribute::NONE;

  if(curr_skill->getFlag(SkillFlags::OFFENSIVE))
  {
    prim_user_stat = attr_prio;
    secd_user_stat = attr_seco;
    prim_targ_stat = attr_prid;
    secd_targ_stat = attr_secd;
  }

  auto prim_user_mod = temp_user_stats.getValue(prim_user_stat);
  auto secd_user_mod = temp_user_stats.getValue(secd_user_stat);
  auto prim_targ_mod = targ_attrs.getValue(prim_targ_stat);
  auto secd_targ_mod = targ_attrs.getValue(secd_targ_stat);

  auto prim_strength = false; /* If the opponent's prim element is str */
  auto secd_strength = false; /* If the opponent's secd element is str */
  auto prim_weakness = false; /* If the opponent's prim element is weak */
  auto secd_weakness = false; /* If the opponent's secd element is weak */

  if(curr_skill->getPrimary() != Element::NONE)
  {
    /* If the user's prim element is weak against the target's */
    if(curr_skill->getPrimary() ==
       Helpers::getStrength(curr_target->getBasePerson()->getPrimary()))
      prim_strength = true;

    /* If the user's prim element is strong against the target's */
    if(curr_skill->getPrimary() ==
       Helpers::getWeakness(curr_target->getBasePerson()->getPrimary()))
      prim_weakness = true;
  }

  if(curr_skill->getSecondary() != Element::NONE)
  {
    /* If the user's secd element is weak against the target's */
    if(curr_skill->getSecondary() ==
       Helpers::getStrength(curr_target->getBasePerson()->getSecondary()))
      secd_strength = true;

    /* If the user's secd element is strong against the target's */
    if(curr_skill->getSecondary() ==
       Helpers::getWeakness(curr_target->getBasePerson()->getSecondary()))
      secd_weakness = true;
  }

  if(curr_skill->getFlag(SkillFlags::OFFENSIVE))
  {
    /* User is strong in primary elemental case */
    if(prim_weakness && !secd_weakness)
    {
      prim_user_mod *= kPRIM_ELM_ADV_MODIFIER;
    }
    /* User is strong in secondary elemental case */
    else if(!prim_weakness && secd_weakness)
    {
      secd_user_mod *= kSECD_ELM_ADV_MODIFIER;
    }
    /* User is strong in both elemental cases */
    else if(prim_weakness && secd_weakness)
    {
      prim_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in primary elemental case */
    else if(prim_strength && !secd_strength)
    {
      prim_targ_mod *= kPRIM_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in secondary elemental case */
    else if(!prim_strength && secd_strength)
    {
      secd_targ_mod *= kSECD_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in both elemental cases */
    else if(prim_strength && secd_strength)
    {
      prim_targ_mod *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_targ_mod *= kDOUBLE_ELM_ADV_MODIFIER;
    }
  }

  temp_user_stats.setBaseValue(prim_user_stat, prim_user_mod);
  temp_user_stats.setBaseValue(secd_user_stat, secd_user_mod);
  targ_attrs.setBaseValue(prim_targ_stat, prim_targ_mod);
  targ_attrs.setBaseValue(secd_targ_stat, secd_targ_mod);
}

void BattleEvent::calcIgnoreState()
{
  auto curr_action = getCurrAction();
  auto curr_skill = getCurrSkill();

  if(curr_action && curr_skill)
  {
    auto IG_PHYS_ATK = IgnoreState::IGNORE_PHYS_ATK;
    auto IG_PHYS_DEF = IgnoreState::IGNORE_PHYS_DEF;
    auto IG_PRIM_ATK = IgnoreState::IGNORE_PRIM_ATK;
    auto IG_PRIM_DEF = IgnoreState::IGNORE_PRIM_DEF;
    auto IG_SECD_ATK = IgnoreState::IGNORE_SECD_ATK;
    auto IG_SECD_DEF = IgnoreState::IGNORE_SECD_DEF;
    auto IG_LUCK_ATK = IgnoreState::IGNORE_LUCK_ATK;
    auto IG_LUCK_DEF = IgnoreState::IGNORE_LUCK_DEF;

    setFlagIgnore(IG_PHYS_ATK, curr_action->atkFlag(IgnoreFlags::PHYSICAL));
    setFlagIgnore(IG_PHYS_DEF, curr_action->defFlag(IgnoreFlags::PHYSICAL));
    setFlagIgnore(IG_LUCK_ATK, curr_action->atkFlag(IgnoreFlags::LUCK));
    setFlagIgnore(IG_LUCK_DEF, curr_action->defFlag(IgnoreFlags::LUCK));

    if(attr_prio == Attribute::THAG)
      setFlagIgnore(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::THERMAL));
    else if(attr_prio == Attribute::POAG)
      setFlagIgnore(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::POLAR));
    else if(attr_prio == Attribute::PRAG)
      setFlagIgnore(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::PRIMAL));
    else if(attr_prio == Attribute::CHAG)
      setFlagIgnore(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::CHARGED));
    else if(attr_prio == Attribute::CYAG)
      setFlagIgnore(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::CYBERNETIC));
    else if(attr_prio == Attribute::NIAG)
      setFlagIgnore(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::NIHIL));

    if(attr_prid == Attribute::THFD)
      setFlagIgnore(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::THERMAL));
    else if(attr_prid == Attribute::POFD)
      setFlagIgnore(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::POLAR));
    else if(attr_prid == Attribute::PRFD)
      setFlagIgnore(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::PRIMAL));
    else if(attr_prid == Attribute::CHFD)
      setFlagIgnore(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::CHARGED));
    else if(attr_prid == Attribute::CYFD)
      setFlagIgnore(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::CYBERNETIC));
    else if(attr_prid == Attribute::NIFD)
      setFlagIgnore(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::NIHIL));

    if(attr_seco == Attribute::THAG)
      setFlagIgnore(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::THERMAL));
    else if(attr_seco == Attribute::POAG)
      setFlagIgnore(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::POLAR));
    else if(attr_seco == Attribute::PRAG)
      setFlagIgnore(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::PRIMAL));
    else if(attr_seco == Attribute::CHAG)
      setFlagIgnore(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::CHARGED));
    else if(attr_seco == Attribute::CYAG)
      setFlagIgnore(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::CYBERNETIC));
    else if(attr_seco == Attribute::NIAG)
      setFlagIgnore(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::NIHIL));

    if(attr_secd == Attribute::THFD)
      setFlagIgnore(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::THERMAL));
    else if(attr_secd == Attribute::POFD)
      setFlagIgnore(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::POLAR));
    else if(attr_secd == Attribute::PRFD)
      setFlagIgnore(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::PRIMAL));
    else if(attr_secd == Attribute::CHFD)
      setFlagIgnore(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::CHARGED));
    else if(attr_secd == Attribute::CYFD)
      setFlagIgnore(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::CYBERNETIC));
    else if(attr_secd == Attribute::NIAG)
      setFlagIgnore(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::NIHIL));
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void BattleEvent::updateStats()
{
  /* Update the user of the events stats to their current values */
  temp_user_stats = BattleStats(actor->getStats());

  /* Build vectors of curr and curr_max stas for each target */
  for(const auto& target : actor_targets)
    temp_targ_stats.push_back(BattleStats(target->getStats()));

  calcActionVariables();
  calcIgnoreState();
}

Action* BattleEvent::getCurrAction()
{
  auto curr_skill = getCurrSkill();

  if(curr_skill && action_index < curr_skill->getEffects().size())
    return curr_skill->getEffect(action_index);

  return nullptr;
}

Skill* BattleEvent::getCurrSkill()
{
  if(action_type == ActionType::SKILL && event_skill)
    return event_skill;
  if(action_type == ActionType::ITEM && event_item && event_item->getUseSkill())
    return event_item->getUseSkill();

  return nullptr;
}

bool BattleEvent::getFlagIgnore(IgnoreState test_flag)
{
  return static_cast<bool>((flags_ignore & test_flag) == test_flag);
}

void BattleEvent::setFlagIgnore(IgnoreState flag, const bool& set_value)
{
  (set_value) ? (flags_ignore |= flag) : (flags_ignore &= ~flag);
}

bool BattleEvent::setNextAction()
{
  auto curr_skill = getCurrSkill();

  if(curr_skill && action_index + 1 < curr_skill->getEffects().size())
  {
    action_index++;

    return true;
  }

  return false;
}

// bool Battle::doesActionCrit()
// {
//   if(curr_user == nullptr || curr_target == nullptr)
//     return false;

//   auto crit_possible = true;
//   auto crit_happens = false;

//   crit_possible &= curr_user->getBFlag(BState::CAN_CRIT);
//   crit_possible &= curr_target->getBFlag(BState::CAN_BE_CRIT);

//   if(crit_possible)
//   {
//     auto crit_chance = kBASE_CRIT_CHANCE;
//     auto crit_mod = temp_user_stats.getStat(Attribute::UNBR) *
//     kCRIT_MODIFIER;
//     auto crit_lvl_mod =
//         calcLevelDifference(action_buffer->getTargets()) *
//         kCRIT_LVL_MODIFIER;

//     crit_chance += crit_mod + crit_lvl_mod;

//     if(curr_target->getBFlag(BState::DEFENDING))
//       crit_chance *= kCRIT_DEFENDING_MODIFIER;
//     if(curr_target->getBFlag(BState::GUARDED))
//       crit_chance *= kCRIT_GUARDED_MODIFIER;

//     if(crit_chance > 0)
//     {
//       uint32_t crit_pc_1000 = floor(crit_chance * 1000);
//       if(Helpers::chanceHappens(crit_pc_1000, 1000))
//         crit_happens = true;
//     }
//   }

//   return crit_happens;
// }

// bool Battle::doesSkillHit(std::vector<Person*> targets)
// {
//   auto can_process = true;
//   auto hits = true;

//   can_process &= curr_skill != nullptr;
//   can_process &= curr_user != nullptr;

//   if(can_process)
//   {
//     auto can_hit = true;

//     can_hit &= curr_skill->isValid();
//     can_hit &= curr_user->getBFlag(BState::SKL_ENABLED);
//     can_hit &= !curr_user->getAilFlag(PersonAilState::MISS_NEXT_TARGET);

//     if(curr_user->getAilFlag(PersonAilState::MISS_NEXT_TARGET))
//     {
//       event_buffer->createMissEvent(EventType::BLIND_MISS, curr_user,
//       targets);
//       // std::cout << "{BLIND} - Blind miss" << std::endl;
//     }

//     if(can_hit)
//     {
//       if(curr_user->getAilFlag(PersonAilState::NEXT_ATK_NO_EFFECT))
//       {
//         event_buffer->createMissEvent(EventType::DREAMSNARE_MISS,
//         curr_user,
//                                       targets);
//         // std::cout << "{DREAMSNARE} - No effect" << std::endl;
//       }

//       /* Obtain the base hit rate (in XX.X%) */
//       auto hit_rate = curr_skill->getChance();

//       /* Obtain the average level difference in #lvls, a positive value
//       denoting
//        * the user's level is higher -> modify it -> add to base hit chance
//        */
//       auto level_diff = calcLevelDifference(targets);

//       /* Hit rate is not altered for user's who have higher levels */
//       if(level_diff < 0)
//       {
//         /* Add the (negative) mod value to the hit rate */
//         auto mod = static_cast<float>(level_diff *
//         kDODGE_PER_LEVEL_MODIFIER);
//         auto new_hit_rate = hit_rate + mod;
//         auto lowest_hit_rate = hit_rate * (kDODGE_HIGHEST_RATE_PC / 100);

//         /* Assert that the hit rate is above the minimum hit rate */
//         hit_rate = Helpers::setInRange(new_hit_rate, lowest_hit_rate,
//         hit_rate);
//       }

//       hits = Helpers::chanceHappens(static_cast<uint32_t>(hit_rate), 100);
//     }
//   }

//   return hits;
// }

// bool Battle::doesActionHit()
// {
//   auto can_process = true;
//   auto hit = true;

//   can_process &= curr_user != nullptr;
//   can_process &= curr_target != nullptr;
//   can_process &= curr_action != nullptr;

//   if(can_process)
//   {
//     /* Run probabilities of the action occuring */
//     hit = Helpers::chanceHappens(
//         static_cast<uint32_t>(curr_action->getChance()), 100);
//   }
//   else
//   {
//     std::cerr << "[Warning] Cannot process action missing chances" <<
//     std::endl;
//   }

//   return hit;
// }

// int32_t Battle::calcImplodeDamage()
// {
//   auto party_death = false;
//   (void)party_death; // TODO check for party death

//   auto targ_attrs = temp_target_stats.at(pro_index);
//   float base_damage = 0;
//   int32_t base_user_pow = 0;
//   int32_t base_targ_def = 0;
//   int32_t phys_pow_val = temp_user_stats.getStat(Attribute::PHAG);
//   int32_t phys_def_val = targ_attrs.getStat(Attribute::PHFD);
//   int32_t elm1_pow_val = temp_user_stats.getStat(prim_off);
//   int32_t elm1_def_val = targ_attrs.getStat(prim_def);
//   int32_t elm2_pow_val = temp_user_stats.getStat(secd_off);
//   int32_t elm2_def_val = targ_attrs.getStat(secd_def);

//   base_user_pow = phys_pow_val + elm1_pow_val + elm2_pow_val;
//   base_targ_def = phys_def_val + elm1_def_val + elm2_def_val;

//   int32_t action_power = base_user_pow;

//   auto attack_modifier = 1.0 / (1.0 + std::exp(-base_user_pow / 255));
//   auto attack_power = 2.0 * action_power * (1.0 + attack_modifier);

//   auto defense_modifier = 0.0f;

//   if(3 * ((float)action_power + (float)base_targ_def >= 0))
//   {
//     defense_modifier = (float)base_targ_def /
//                        (3 * ((float)action_power + (float)base_targ_def));
//   }

//   base_damage = attack_power * (1 - defense_modifier);

// #ifdef UDEBUG
//   std::cout << "Attack Modifier: " << attack_modifier << std::endl;
//   std::cout << "Action Power: " << action_power << std::endl;
//   std::cout << "Attack Power: " << attack_power << std::endl;
//   std::cout << "Defense Modifier: " << defense_modifier << std::endl;
//   std::cout << "Base Damage: " << base_damage << std::endl;
// #endif

//   int32_t damage_round = std::round(base_damage);
//   damage_round = Helpers::setInRange(damage_round, 0,
//                                      targ_attrs.getStat(Attribute::VITA) -
//                                      1);

// #ifdef UDEBUG
//   std::cout << "Modified Damage: " << damage_round << std::endl;
// #endif

//   return damage_round;
// }

int32_t BattleEvent::calcValPhysPow(BattleStats target_stats)
{
  if(getFlagIgnore(IgnoreState::IGNORE_PHYS_ATK))
    return 0;

  return target_stats.getValue(Attribute::PHAG) * kOFF_PHYS_MODIFIER;
}

int32_t BattleEvent::calcValPhysDef(BattleStats target_stats)
{
  if(getFlagIgnore(IgnoreState::IGNORE_PHYS_DEF))
    return 0;

  return target_stats.getValue(Attribute::PHFD) * kDEF_PHYS_MODIFIER;
}

int32_t BattleEvent::calcValPrimAtk(Skill* curr_skill)
{
  if(!getFlagIgnore(IgnoreState::IGNORE_PRIM_ATK) && doesPrimMatch(curr_skill))
    return temp_user_stats.getValue(attr_prio) * kOFF_PRIM_ELM_MATCH_MODIFIER;

  return 0;
}

int32_t BattleEvent::calcValPrimDef(Skill* curr_skill)
{
  if(!getFlagIgnore(IgnoreState::IGNORE_PRIM_DEF) && doesPrimMatch(curr_skill))
    return temp_user_stats.getValue(attr_prid) * kDEF_PRIM_ELM_MATCH_MODIFIER;

  return 0;
}

int32_t BattleEvent::calcValSecdAtk(Skill* curr_skill)
{
  if(!getFlagIgnore(IgnoreState::IGNORE_SECD_ATK) && doesSecdMatch(curr_skill))
    return temp_user_stats.getValue(attr_seco) * kOFF_SECD_ELM_MATCH_MODIFIER;

  return 0;
}

int32_t BattleEvent::calcValSecdDef(Skill* curr_skill)
{
  if(!getFlagIgnore(IgnoreState::IGNORE_SECD_DEF) && doesSecdMatch(curr_skill))
    return temp_user_stats.getValue(attr_secd) * kDEF_SECD_ELM_MATCH_MODIFIER;

  return 0;
}

int32_t BattleEvent::calcValLuckAtk()
{
  if(getFlagIgnore(IgnoreState::IGNORE_LUCK_ATK))
    return 0;

  return temp_user_stats.getValue(Attribute::MANN) * kMANNA_POW_MODIFIER;
}

int32_t BattleEvent::calcValLuckDef()
{
  if(getFlagIgnore(IgnoreState::IGNORE_LUCK_DEF))
    return 0;

  return temp_user_stats.getValue(Attribute::MANN) * kMANNA_DEF_MODIFIER;
}

bool BattleEvent::doesPrimMatch(Skill* skill)
{
  if(actor && actor->getBasePerson() && skill)
  {
    auto secd = actor->getBasePerson()->getSecondary();

    return (secd == skill->getPrimary() && secd == skill->getSecondary());
  }

  return false;
}

bool BattleEvent::doesSecdMatch(Skill* skill)
{
  if(actor && actor->getBasePerson() && skill)
  {
    auto prim = actor->getBasePerson()->getSecondary();

    return (prim == skill->getPrimary() || prim == skill->getSecondary());
  }

  return false;
}

// TODO: Guarding damage factor [11-01-15]
// TODO: Guarding for users who are guarding this actor [11-01-15]
int32_t BattleEvent::calcDamage(BattleActor* curr_target, float crit_factor)
{
  calcIgnoreState();
  calcElementalMods(curr_target);

  auto targ_stats = curr_target->getStats();
  auto curr_skill = getCurrSkill();
  auto curr_action = getCurrAction();
  auto p_pow_val = calcValPhysPow(targ_stats);
  auto p_def_val = calcValPhysDef(targ_stats);
  auto elm1_pow_val = calcValPrimAtk(curr_skill);
  auto elm1_def_val = calcValPrimDef(curr_skill);
  auto elm2_pow_val = calcValSecdAtk(curr_skill);
  auto elm2_def_val = calcValSecdDef(curr_skill);
  auto luck_pow_val = calcValLuckAtk();
  auto luck_def_val = calcValLuckDef();

  /* Summation of base power / defense */
  auto base_user_pow = p_pow_val + elm1_pow_val + elm2_pow_val + luck_pow_val;
  auto base_targ_def = p_def_val + elm1_def_val + elm2_def_val + luck_def_val;
  base_user_pow *= kUSER_POW_MODIFIER;
  base_targ_def *= kTARG_DEF_MODIFIER;

  /* Addition of the power of the action */
  auto action_power = curr_action->getBase();

  /* If the action power is a percentage, add a pc of total user power */
  if(curr_action->actionFlag(ActionFlags::BASE_PC))
  {
    auto base_pc = static_cast<float>(action_power) / 100;
    action_power = base_pc * base_user_pow;
  }

  /* Addition of the variance of the action */
  auto var_val = 0;
  auto base_var = curr_action->getVariance();

  if(curr_action->actionFlag(ActionFlags::VARI_PC))
    var_val = static_cast<float>(base_var) / 100 * action_power;
  else
    var_val = base_var;

  action_power = Helpers::randU(action_power - var_val, action_power + var_val);

  float base_damage = 0;

  auto attack_modifier = 1.0 / (1.0 + std::exp(-base_user_pow / 255));
  auto attack_power = action_power * (1.0 + attack_modifier);

  auto defense_modifier = 0.0f;

  if(action_power + base_targ_def >= 0)
  {
    defense_modifier =
        (float)base_targ_def / (float)(action_power + base_targ_def);
  }

  base_damage = attack_power * (1 - defense_modifier);

#ifdef UDEBUG
  std::cout << "Attack Modifier: " << attack_modifier << std::endl;
  std::cout << "Action Power: " << action_power << std::endl;
  std::cout << "Attack Power: " << attack_power << std::endl;
  std::cout << "Defense Modifier: " << defense_modifier << std::endl;
  std::cout << "Base Damage: " << base_damage << std::endl;
#endif

  /* If the user is defending, shielded or guarded --->  decrease dmg */
  // if(curr_target->getBFlag(BState::DEFENDING))
  //   base_damage *= kDEFEND_MODIFIER;
  // if(curr_target->getBFlag(BState::SHIELDED))
  //   base_damage *= kSHIELDED_MODIFIER;

  base_damage *= crit_factor;

  return base_damage;
}
