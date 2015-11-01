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

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

// bool Battle::calcIgnoreState()
// {
//   auto success = false;

//   if(curr_action != nullptr && curr_skill != nullptr)
//   {
//     /* Offensive Action */
//     if(curr_skill->getFlag(SkillFlags::OFFENSIVE) ||
//        curr_skill->getFlag(SkillFlags::DEFENSIVE))
//     {
//       auto IG_PHYS_ATK = IgnoreState::IGNORE_PHYS_ATK;
//       auto IG_PHYS_DEF = IgnoreState::IGNORE_PHYS_DEF;
//       auto IG_PRIM_ATK = IgnoreState::IGNORE_PRIM_ATK;
//       auto IG_PRIM_DEF = IgnoreState::IGNORE_PRIM_DEF;
//       auto IG_SECD_ATK = IgnoreState::IGNORE_SECD_ATK;
//       auto IG_SECD_DEF = IgnoreState::IGNORE_SECD_DEF;

//       if(curr_skill->getFlag(SkillFlags::DEFENSIVE))
//       {
//         IG_PHYS_ATK = IgnoreState::IGNORE_PHYS_DEF;
//         IG_PHYS_DEF = IgnoreState::IGNORE_PHYS_ATK;
//         IG_PRIM_ATK = IgnoreState::IGNORE_PRIM_DEF;
//         IG_PRIM_DEF = IgnoreState::IGNORE_PRIM_ATK;
//         IG_SECD_ATK = IgnoreState::IGNORE_SECD_DEF;
//         IG_SECD_DEF = IgnoreState::IGNORE_SECD_ATK;
//       }

//       if(curr_action->atkFlag(IgnoreFlags::PHYSICAL))
//         setIgnoreFlag(IG_PHYS_ATK);
//       if(curr_action->defFlag(IgnoreFlags::PHYSICAL))
//         setIgnoreFlag(IG_PHYS_DEF);

//       if(curr_action->atkFlag(IgnoreFlags::LUCK))
//         setIgnoreFlag(IgnoreState::IGNORE_LUCK_ATK);
//       if(curr_action->defFlag(IgnoreFlags::LUCK))
//         setIgnoreFlag(IgnoreState::IGNORE_LUCK_DEF);

//       if(prim_off == Attribute::THAG)
//         setIgnoreFlag(IG_PRIM_ATK,
//         curr_action->atkFlag(IgnoreFlags::THERMAL));
//       else if(prim_off == Attribute::POAG)
//         setIgnoreFlag(IG_PRIM_ATK,
//         curr_action->atkFlag(IgnoreFlags::POLAR));
//       else if(prim_off == Attribute::PRAG)
//         setIgnoreFlag(IG_PRIM_ATK,
//         curr_action->atkFlag(IgnoreFlags::PRIMAL));
//       else if(prim_off == Attribute::CHAG)
//         setIgnoreFlag(IG_PRIM_ATK,
//         curr_action->atkFlag(IgnoreFlags::CHARGED));
//       else if(prim_off == Attribute::CYAG)
//         setIgnoreFlag(IG_PRIM_ATK,
//                       curr_action->atkFlag(IgnoreFlags::CYBERNETIC));
//       else if(prim_off == Attribute::NIAG)
//         setIgnoreFlag(IG_PRIM_ATK,
//         curr_action->atkFlag(IgnoreFlags::NIHIL));

//       if(prim_def == Attribute::THFD)
//         setIgnoreFlag(IG_PRIM_DEF,
//         curr_action->defFlag(IgnoreFlags::THERMAL));
//       else if(prim_def == Attribute::POFD)
//         setIgnoreFlag(IG_PRIM_DEF,
//         curr_action->defFlag(IgnoreFlags::POLAR));
//       else if(prim_def == Attribute::PRFD)
//         setIgnoreFlag(IG_PRIM_DEF,
//         curr_action->defFlag(IgnoreFlags::PRIMAL));
//       else if(prim_def == Attribute::CHFD)
//         setIgnoreFlag(IG_PRIM_DEF,
//         curr_action->defFlag(IgnoreFlags::CHARGED));
//       else if(prim_def == Attribute::CYFD)
//         setIgnoreFlag(IG_PRIM_DEF,
//                       curr_action->defFlag(IgnoreFlags::CYBERNETIC));
//       else if(prim_def == Attribute::NIFD)
//         setIgnoreFlag(IG_PRIM_DEF,
//         curr_action->defFlag(IgnoreFlags::NIHIL));

//       if(secd_off == Attribute::THAG)
//         setIgnoreFlag(IG_SECD_ATK,
//         curr_action->atkFlag(IgnoreFlags::THERMAL));
//       else if(secd_off == Attribute::POAG)
//         setIgnoreFlag(IG_SECD_ATK,
//         curr_action->atkFlag(IgnoreFlags::POLAR));
//       else if(secd_def == Attribute::PRAG)
//         setIgnoreFlag(IG_SECD_ATK,
//         curr_action->atkFlag(IgnoreFlags::PRIMAL));
//       else if(secd_def == Attribute::CHAG)
//         setIgnoreFlag(IG_SECD_ATK,
//         curr_action->atkFlag(IgnoreFlags::CHARGED));
//       else if(secd_off == Attribute::CYAG)
//         setIgnoreFlag(IG_SECD_ATK,
//                       curr_action->atkFlag(IgnoreFlags::CYBERNETIC));
//       else if(secd_off == Attribute::NIAG)
//         setIgnoreFlag(IG_SECD_ATK,
//         curr_action->atkFlag(IgnoreFlags::NIHIL));

//       if(secd_def == Attribute::THFD)
//         setIgnoreFlag(IG_SECD_DEF,
//         curr_action->defFlag(IgnoreFlags::THERMAL));
//       else if(secd_def == Attribute::POFD)
//         setIgnoreFlag(IG_SECD_DEF,
//         curr_action->defFlag(IgnoreFlags::POLAR));
//       else if(secd_def == Attribute::PRFD)
//         setIgnoreFlag(IG_SECD_DEF,
//         curr_action->defFlag(IgnoreFlags::PRIMAL));
//       else if(secd_def == Attribute::CHFD)
//         setIgnoreFlag(IG_SECD_DEF,
//         curr_action->defFlag(IgnoreFlags::CHARGED));
//       else if(secd_def == Attribute::CYFD)
//         setIgnoreFlag(IG_SECD_DEF,
//                       curr_action->defFlag(IgnoreFlags::CYBERNETIC));
//       else if(secd_def == Attribute::NIAG)
//         setIgnoreFlag(IG_SECD_DEF,
//         curr_action->defFlag(IgnoreFlags::NIHIL));
//     }

//     success = true;
//   }

//   return success;
// }

// void Battle::calcElementalMods()
// {
//   /* Grab the temp attribute set for the curent processing target index */
//   auto targ_attrs = temp_target_stats.at(pro_index);

//   /* Determine the correct stats to apply the modifier to */
//   auto prim_user_stat = Attribute::NONE;
//   auto secd_user_stat = Attribute::NONE;
//   auto prim_targ_stat = Attribute::NONE;
//   auto secd_targ_stat = Attribute::NONE;

//   if(curr_skill->getFlag(SkillFlags::OFFENSIVE))
//   {
//     prim_user_stat = prim_off;
//     secd_user_stat = secd_off;
//     prim_targ_stat = prim_def;
//     secd_targ_stat = secd_def;
//   }

//   auto prim_user_mod = temp_user_stats.getStat(prim_user_stat);
//   auto secd_user_mod = temp_user_stats.getStat(secd_user_stat);

//   auto prim_targ_mod = targ_attrs.getStat(prim_targ_stat);
//   auto secd_targ_mod = targ_attrs.getStat(secd_targ_stat);

//   auto prim_strength = false; /* If the opponent's prim element is str */
//   auto secd_strength = false; /* If the opponent's secd element is str */
//   auto prim_weakness = false; /* If the opponent's prim element is weak */
//   auto secd_weakness = false; /* If the opponent's secd element is weak */

//   if(curr_skill->getPrimary() != Element::NONE)
//   {
//     /* If the user's prim element is weak against the target's */
//     if(curr_skill->getPrimary() ==
//        Helpers::getStrength(curr_target->getPrimary()))
//       prim_strength = true;

//     /* If the user's prim element is strong against the target's */
//     if(curr_skill->getPrimary() ==
//        Helpers::getWeakness(curr_target->getPrimary()))
//       prim_weakness = true;
//   }

//   if(curr_skill->getSecondary() != Element::NONE)
//   {
//     /* If the user's secd element is weak against the target's */
//     if(curr_skill->getSecondary() ==
//        Helpers::getStrength(curr_target->getSecondary()))
//       secd_strength = true;

//     /* If the user's secd element is strong against the target's */
//     if(curr_skill->getSecondary() ==
//        Helpers::getWeakness(curr_target->getSecondary()))
//       secd_weakness = true;
//   }

//   if(curr_skill->getFlag(SkillFlags::OFFENSIVE))
//   {
//     /* User is strong in primary elemental case */
//     if(prim_weakness && !secd_weakness)
//     {
//       prim_user_mod *= kPRIM_ELM_ADV_MODIFIER;
//     }
//     /* User is strong in secondary elemental case */
//     else if(!prim_weakness && secd_weakness)
//     {
//       secd_user_mod *= kSECD_ELM_ADV_MODIFIER;
//     }
//     /* User is strong in both elemental cases */
//     else if(prim_weakness && secd_weakness)
//     {
//       prim_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
//       secd_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
//     }
//     /* Opponent is strong in primary elemental case */
//     else if(prim_strength && !secd_strength)
//     {
//       prim_targ_mod *= kPRIM_ELM_ADV_MODIFIER;
//     }
//     /* Opponent is strong in secondary elemental case */
//     else if(!prim_strength && secd_strength)
//     {
//       secd_targ_mod *= kSECD_ELM_ADV_MODIFIER;
//     }
//     /* Opponent is strong in both elemental cases */
//     else if(prim_strength && secd_strength)
//     {
//       prim_targ_mod *= kDOUBLE_ELM_ADV_MODIFIER;
//       secd_targ_mod *= kDOUBLE_ELM_ADV_MODIFIER;
//     }
//   }

//   temp_user_stats.setStat(prim_user_stat, prim_user_mod);
//   temp_user_stats.setStat(secd_user_stat, secd_user_mod);
//   targ_attrs.setStat(prim_targ_stat, prim_targ_mod);
//   targ_attrs.setStat(secd_targ_stat, secd_targ_mod);
// }

 // void Battle::buildActionVariables(ActionType action_type,
//                                   std::vector<Person*> targets)
// {
//   auto prim_stats = Helpers::elementToStats(Element::PHYSICAL);
//   auto secd_stats = Helpers::elementToStats(Element::PHYSICAL);

//   if(action_type == ActionType::SKILL)
//   {
//     /* Grab the enumerated attribute types related to elements of the Skill
//     */
//     prim_stats = Helpers::elementToStats(curr_skill->getPrimary());
//     secd_stats = Helpers::elementToStats(curr_skill->getSecondary());
//   }
//   else if(action_type == ActionType::IMPLODE)
//   {
//     prim_stats = Helpers::elementToStats(curr_user->getPrimary());
//     secd_stats = Helpers::elementToStats(curr_user->getSecondary());
//   }

//   /* Update the temporary copy of the User's current stats */
//   temp_user_stats = AttributeSet(curr_user->getCurr());
//   temp_user_max_stats = AttributeSet(curr_user->getTemp());

//   /* Build vectors of curr and curr_max stas for each target */
//   for(auto jt = begin(targets); jt != end(targets); ++jt)
//   {
//     temp_target_stats.push_back(AttributeSet((*jt)->getCurr()));
//     temp_target_max_stats.push_back(AttributeSet((*jt)->getTemp()));
//   }

//   /* User ref. vars related to prim/secd skill attributes, -1 if Attr:NONE
//   */
//   auto prim_user_off = temp_user_stats.getStat(prim_off);
//   auto prim_user_def = temp_user_stats.getStat(prim_def);
//   auto secd_user_off = temp_user_stats.getStat(secd_off);
//   auto secd_user_def = temp_user_stats.getStat(secd_def);

//   if(action_type == ActionType::SKILL)
//   {
//     if(curr_user->getPrimary() == curr_skill->getPrimary())
//     {
//       prim_user_off *= kOFF_PRIM_ELM_MODIFIER;
//       prim_user_def *= kDEF_PRIM_ELM_MODIFIER;
//     }
//     else if(curr_user->getSecondary() == curr_skill->getSecondary())
//     {
//       secd_user_off *= kOFF_SECD_ELM_MODIFIER;
//       secd_user_def *= kDEF_SECD_ELM_MODIFIER;
//     }
//   }
// }
