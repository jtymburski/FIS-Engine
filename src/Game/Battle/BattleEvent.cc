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

  // calcElementalMods(curr_target);
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
