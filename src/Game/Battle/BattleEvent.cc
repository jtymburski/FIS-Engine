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

/* ------------ Battle Damage Calculation Modifiers ---------------
 * Offensive Physical Modifier
 * Defensive Physical Modifier
 * Offensive Primary Elemental Match Modifier
 * Defensive Primary Elemental Match Modifier
 * Offensive Secondary Elemental Match Modifier
 * Defensive Secondary Elemental Match Modifier
 *
 * Offensive Primary Element Modifier
 * Defensive Primary Element Modifier
 * Offensive Secondary Element Modifier
 * Defensive Secondary Element Modifier
 *
 * Primary Elemental Advantage Modifier
 * Primary Elemental Disadvantage Modifier
 * Secondary Elemental Advantage Modifier
 * Secondary Elemental Disadvantage Modifier
 * Double Elemental Advantage Modifier
 * Double Elemental Disadvantage Modifier
 *
 * Manna Power Modifier
 * Manna Defense Modifier
 * User Power Modifier
 * Target Defense Modifier
 *
 * Base Critical Hit Chance
 * Offensive Critical Factor
 * Critical Modifier (Base)
 * Critical Lvl Modifier (Per Level)
 * Critical Defending Modifier (While User Defending)
 * Crtical Guarded Modifier (While User Being Guarded)
 * Critical Shielded Modifier (While User Being Shielded)
 *
 * Defending Modifier
 * Guarded Modifier
 * Shielded Modifier
 *
 * Dodge Chance [Limbertude] Modifier
 * Dodge Chance [Limbertude] Per Level Modifier
 *
 * Minimum Damage (Possible)
 * Maximum Damage (Possible)
 */

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

const float BattleEvent::kDEFEND_MODIFIER = 0.45;
const float BattleEvent::kGUARD_MODIFIER = 1.10;
const float BattleEvent::kSHIELDED_MODIFIER = 0.00;

const float BattleEvent::kBASE_CRIT_CHANCE = 0.10;
const float BattleEvent::kOFF_CRIT_FACTOR = 1.45;
const float BattleEvent::kCRIT_MODIFIER = 0.0008;
const float BattleEvent::kCRIT_LVL_MODIFIER = 0.012;
const float BattleEvent::kCRIT_DEFENDING_MODIFIER = 0.70;
const float BattleEvent::kCRIT_GUARDED_MODIFIER = 0.65;

const float BattleEvent::kDODGE_MODIFIER = 0.10;
const float BattleEvent::kDODGE_HIGHEST_RATE_PC = 50.0;
const float BattleEvent::kDODGE_PER_LEVEL_MODIFIER = 2.50;

const uint32_t BattleEvent::kMINIMUM_DAMAGE = 1;
const uint32_t BattleEvent::kMAXIMUM_DAMAGE = 29999;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTOR
 *============================================================================*/

BattleEvent::BattleEvent()
    : action_index{0},
      action_state{ActionState::BEGIN},
      action_type{ActionType::NONE},
      actor{nullptr},
      actor_targets{},
      actor_outcomes{},
      event_item{nullptr},
      event_skill{nullptr},
      event_type{BattleEventType::NONE},
      flags_ignore{static_cast<IgnoreState>(0)},
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

BattleEvent::BattleEvent(ActionType type, BattleActor* actor) : BattleEvent()
{
  action_type = type;
  this->actor = actor;
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

  assert(curr_skill);

  /* Grab the temp attribute set for the curent processing target index */
  auto targ_attrs = getStatsOfTarget(curr_target);

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

std::string BattleEvent::getActionName()
{
  if(action_type == ActionType::SKILL && event_skill)
    return event_skill->skill->getName();

  return "";
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
    return event_skill->skill;
  if(action_type == ActionType::ITEM && event_item && event_item->item)
    return event_item->item->getUseSkill();

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
  std::cout << "Setting next action! " << std::endl;
  auto curr_skill = getCurrSkill();

  if(curr_skill && action_index + 1 < curr_skill->getEffects().size())
  {
    std::cout << "Incrementing action index " << std::endl;
    action_index++;
    event_type = BattleEventType::NONE;

    return true;
  }

  return false;
}

// Does the action crit the given BattleActor?
// TODO: Crit level modifier
bool BattleEvent::doesActionCrit(BattleActor* curr_target)
{
  auto curr_action = getCurrAction();

  if(actor && curr_target && curr_action)
  {
    auto crit_mod = temp_user_stats.getValue(Attribute::UNBR) * kCRIT_MODIFIER;
    auto crit_chance = crit_mod + kBASE_CRIT_CHANCE;

    if(curr_target->getGuardingState() == GuardingState::DEFENDING)
      crit_chance *= kCRIT_DEFENDING_MODIFIER;
    else if(curr_target->getGuardingState() == GuardingState::GUARDED)
      crit_chance *= kCRIT_GUARDED_MODIFIER;
    else if(curr_target->getGuardingState() == GuardingState::GUARDED_DEFENDING)
      crit_chance *= kCRIT_DEFENDING_MODIFIER * kCRIT_GUARDED_MODIFIER;

    if(crit_chance > 0)
    {
      uint32_t crit_pc_1000 = floor(crit_chance * 1000);

      if(Helpers::chanceHappens(crit_pc_1000, 1000))
        return true;
    }
  }

  return false;
}

// Does the skill hit the given target vectors or entirely miss?
SkillHitStatus BattleEvent::doesSkillHit()
{
  auto curr_skill = getCurrSkill();
  SkillHitStatus status = SkillHitStatus::HIT;

  if(actor && curr_skill && event_skill)
  {
    if(actor->getFlag(ActorState::MISS_NEXT_TARGET))
      status = SkillHitStatus::DREAMSNARED;

    if(status == SkillHitStatus::HIT)
    {
      auto level_difference = calcLevelDifference();
      (void)level_difference; // TODO

      auto hit_rate = curr_skill->getChance();

      // TODO - Determine other factors - dodge per lvl etc.
      bool hits = Helpers::chanceHappens(static_cast<uint32_t>(hit_rate), 100);

      if(!hits)
        status = SkillHitStatus::MISS;
    }
  }

  return status;
}

bool BattleEvent::doesActionHit(BattleActor* curr_target)
{
  auto curr_action = getCurrAction();

  if(curr_action && curr_target)
  {
    if(curr_action->actionFlag(ActionFlags::DAMAGE))
      if(!curr_target->getFlag(ActorState::ALIVE))
        return false;

    return Helpers::chanceHappens(
        static_cast<uint32_t>(curr_action->getChance()), 100);
  }

  return false;
}

int32_t BattleEvent::calcDamageImplode(BattleActor* curr_target)
{
  auto targ_stats = getStatsOfTarget(curr_target);
  (void)targ_stats;
  // auto p_pow_val = targ_stats.getValue(Attribute::PHAG);
  // auto p_def_val = targ_stats.getValue(Attribute::PHFD);
  // auto base_user_pow = phys_pow_val;
  // auto base_targ_def = phys_def_val;
  return 0;
}

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

// TODO: Test function. Need floats?
int32_t BattleEvent::calcLevelDifference()
{
  int32_t total = 0;

  if(actor)
  {
    for(const auto& target : actor_targets)
      if(target)
        total += target->getBasePerson()->getLevel() * 100;

    if(actor_targets.size() != 0)
    {
      total = actor->getBasePerson()->getLevel() -
              (total / (actor_targets.size() * 100));
    }
  }

  return total;
}

int32_t BattleEvent::calcValPhysPow()
{
  if(getFlagIgnore(IgnoreState::IGNORE_PHYS_ATK))
    return 0;

  return temp_user_stats.getValue(Attribute::PHAG) * kOFF_PHYS_MODIFIER;
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

int32_t BattleEvent::calcValLuckDef(BattleStats target_stats)
{
  if(getFlagIgnore(IgnoreState::IGNORE_LUCK_DEF))
    return 0;

  return target_stats.getValue(Attribute::MANN) * kMANNA_DEF_MODIFIER;
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

BattleStats BattleEvent::getStatsOfTarget(BattleActor* curr_target)
{
  int32_t index = 0;

  for(const auto& target : actor_targets)
  {
    if(target == curr_target && temp_targ_stats.size() > (uint32_t)index)
      return temp_targ_stats.at(index);

    index++;
  }

  return BattleStats();
}

float BattleEvent::calcCritFactor(BattleActor* curr_target)
{
  /* Add the base crit modifier to the user of the action's unbr stat */
  auto unbearability = actor->getStats().getValue(Attribute::UNBR);
  auto crit_factor = kCRIT_MODIFIER * unbearability;
  crit_factor += kOFF_CRIT_FACTOR + calcLevelDifference() * kCRIT_LVL_MODIFIER;

  if(curr_target)
  {
    // TODO [11-20-15]: guarding/defended modifiers + others
    if(curr_target->getGuardingState() == GuardingState::DEFENDING)
      crit_factor *= kCRIT_DEFENDING_MODIFIER;
    else if(curr_target->getGuardingState() == GuardingState::GUARDED)
      crit_factor *= kCRIT_GUARDED_MODIFIER;
  }

  return Helpers::setInRange(crit_factor, 1, 10);
}

int32_t BattleEvent::calcExperience()
{
  assert(actor && actor->getBasePerson());
  auto experience = 0;

  for(auto& enemy : actor_targets)
  {
    if(enemy && enemy->getBasePerson())
      experience += enemy->getBasePerson()->getExpDrop();
  }

  return static_cast<int32_t>(experience * actor->getBasePerson()->getExpMod());
}

// TODO: Guarding damage factor [11-01-15]
// TODO: Guarding for users who are guarding this actor [11-01-15]
int32_t BattleEvent::calcDamage(BattleActor* curr_target)
{
  calcIgnoreState();
  calcElementalMods(curr_target);
  auto crit_factor = calcCritFactor(curr_target);
  auto targ_stats = getStatsOfTarget(curr_target);
  auto curr_skill = getCurrSkill();
  auto curr_action = getCurrAction();

  /* Summation of base power / defense */
  auto base_user_pow = calcValPhysPow() + calcValPrimAtk(curr_skill) +
                       calcValSecdAtk(curr_skill) + calcValLuckAtk();
  auto base_targ_def = calcValPhysDef(targ_stats) + calcValPrimDef(curr_skill) +
                       calcValSecdDef(curr_skill) + calcValLuckDef(targ_stats);

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

  // TODO[11-03-15] Other guarding state factors
  if(curr_target->getGuardingState() == GuardingState::DEFENDING)
    base_damage *= kDEFEND_MODIFIER;
  else if(curr_target->getGuardingState() == GuardingState::GUARDED)
    base_damage *= kGUARD_MODIFIER;

  return base_damage; // * crit_factor;
}
