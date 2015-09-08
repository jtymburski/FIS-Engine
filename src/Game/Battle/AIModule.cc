// /*******************************************************************************
// * Class Name: AI Module [Implementation]
// * Date Created: June 22, 2014
// * Inheritance: None
// * Description: The AI Module is an object describing the difficulty and
// *              personality for enemy decision making in Battle.
// *
// * Notes
// * -----
// *
// * [1]:
// *
// * See .h file for TODOs
// *******************************************************************************/

// #include "Game/Battle/AIModule.h"
// #include "Game/Player/Person.h"

// /*=============================================================================
//  * CONSTANTS
//  *============================================================================*/

// /* Default Difficulty
//  * Default Personality
//  * Maximum Record Size
//  */
// const AIDifficulty AIModule::kDEFAULT_DIFFICULTY{AIDifficulty::RANDOM};
// const AIPersonality AIModule::kDEFAULT_PERSONALITY{AIPersonality::MODERATOR};
// const uint32_t AIModule::kMAXIMUM_RECORD_SIZE{500};

// /* General AI Variance
//  */
// const float AIModule::kGAI_VARIANCE{0.05};
// const float AIModule::kGAI_BASE_GUARD_FACTOR{0.05};
// const float AIModule::kGAI_BASE_IMPLODE_FACTOR{0.95};
// const float AIModule::kGAI_BASE_DEFEND_FACTOR{0.10};
// const float AIModule::kGAI_BASE_RUN_FACTOR{0.001};
// const float AIModule::kGAI_BASE_PASS_FACTOR{0.01};

// /* Random AI Offensive Factor
//  * Random AI Defensive Factor
//  * Random AI Base Skill Factor
//  * Random AI Base Item Factor
//  * Random AI Lean To Item Factor
//  * Random AI Default Target
//  */
// const float AIModule::kRAI_OFF_FACTOR{1.35};
// const float AIModule::kRAI_DEF_FACTOR{1.50};
// const float AIModule::kRAI_BASE_SKILL_FACTOR{0.600};
// const float AIModule::kRAI_BASE_ITEM_FACTOR{0.200};
// const float AIModule::kRAI_LEAN_TO_ITEM_FACTOR{0.005};
// const AITarget AIModule::kRAI_DEFAULT_TARGET{AITarget::RANDOM};

// /* Priority AI Offensive Factor
//  * Priority AI Defensive Factor
//  * Priority AI Base Skill Factor
//  * Priority AI Base Item Factor
//  * Priority AI Lean To Item Factor
//  * Priority AI Default Target
//  */
// const float AIModule::kPAI_OFF_FACTOR{1.35};
// const float AIModule::kPAI_DEF_FACTOR{1.50};
// const float AIModule::kPAI_BASE_SKILL_FACTOR{0.500};
// const float AIModule::kPAI_BASE_ITEM_FACTOR{0.420};
// const float AIModule::kPAI_LEAN_TO_ITEM_FACTOR{0.008};
// const AITarget AIModule::kPAI_DEFAULT_TARGET{AITarget::LOWEST_HP_FIRST};

// /* Tactical AI //TODO
//  */

// /* Deep-Thought AI //TODO
//  */

// /*=============================================================================
//  * CONSTRUCTORS / DESTRUCTORS
//  *============================================================================*/

// /*
//  * Description: Constructs an AI module object with default parameters.
//  *
//  * Inputs: none
//  */
// AIModule::AIModule()
// {
//   loadDefaults();
// }

// /*
//  * Description: Constructs an AI object given a difficulty and a personality.
//  *
//  * Inputs: diff - the difficulty of the new AI
//  *         prim_personality - the personality to make the AI
//  */
// AIModule::AIModule(const AIDifficulty &diff,
//                    const AIPersonality &prim_personality)
// {
//   loadDefaults();

//   this->difficulty = diff;
//   this->prim_personality = prim_personality;
// }

// /*
//  * Description: Constructs an AI object given a difficulty and a primary and
//  *              secondary personality.
//  *
//  * Inputs: diff - the difficulty of the new AI
//  *         prim_personality - the personality to make the AI
//  *         secd_personality - minor personality effects for secd personality
//  */
// AIModule::AIModule(const AIDifficulty &diff,
//                    const AIPersonality &prim_personality,
//                    const AIPersonality &secd_personality)
//   : AIModule(diff, prim_personality)
// {
//   this->secd_personality = secd_personality;
// }

// /*=============================================================================
//  * PRIVATE FUNCTIONS
//  *============================================================================*/

// /*
//  * Description: Adds a random target from a given vector of available targets
//  *              to the chosen target vector and returns a ptr to the chosen
//  *              target
//  *
//  * Inputs: std::vector<Person*> available_targets
//  * Output: Person* - the chosen target which was added
//  */
// Person* AIModule::addRandomTarget(std::vector<Person*> available_targets)
// {
//   auto rand_value = Helpers::randU(1, available_targets.size());
//   auto target = available_targets.at(rand_value - 1);

//   if (target != nullptr)
//     chosen_targets.push_back(target);

//   return target;
// }

// /*
//  * Description: Determines whether a given BattleItem object is fit for
//  *              selection by the AI module. For example, a BattleItem
//  *              with scope TWO ENEMIES must have at least two enemies
//  *              selectable.
//  *
//  * Inputs: battle_item - constructed BattleSkill object to be tested
//  * Output: bool - true if the given BattleSkill object is fit for selection
//  */
// // bool AIModule::battleItemValid(const BattleItem &battle_item)
// // {
// //   auto target_found = false;

// //   if (battle_item.item_skill->getScope() == ActionScope::TWO_ENEMIES)
// //   {
// //     if (battle_item.foe_targets.size() >= 2)
// //       target_found = true;
// //   }
// //   else if (battle_item.item_skill->getScope() == ActionScope::TWO_ALLIES)
// //   {
// //     if (battle_item.ally_targets.size() >= 2)
// //       target_found = true;
// //   }
// //   else if (!battle_item.all_targets.empty())
// //   {
// //     target_found = true;
// //   }

// //   return target_found;
// // }

// /*
//  * Description: Determines whether a given BattleSkill object is fit for
//  *              selection by the AI module. For example, a BattleSkill
//  *              with scope TWO ENEMIES must have at least two enemies
//  *              selectable.
//  *
//  * Inputs: battle_skill - constructed BattleSkill object to be tested
//  * Output: bool - true if the given BattleSkill object is fit for selection
//  */
// // bool AIModule::battleSkillValid(const BattleSkill &battle_skill)
// // {
// //   auto has_targets = false;

// //   if (battle_skill.skill->getScope() == ActionScope::TWO_ENEMIES)
// //   {
// //     if (battle_skill.foe_targets.size() >= 2)
// //     {
// //       has_targets = true;
// //     }
// //   }
// //   else if (battle_skill.skill->getScope() == ActionScope::TWO_ALLIES)
// //   {
// //     if (battle_skill.ally_targets.size() >= 2)
// //       has_targets = true;
// //   }
// //   else if (!battle_skill.all_targets.empty())
// //   {
// //       has_targets = true;
// //   }

// //   return has_targets;
// // }

// /*
//  * Description: Constructs a uniform distribution between valid skill choices
//  *              and their probabilities based on the arbitrary value level of
//  *              the skill.
//  *
//  * Inputs: none
//  * Output: none
//  */
// void AIModule::buildUniformSkills()
// {
//   for (auto battle_skill : valid_skills)
//   {
//     auto skill_pair = std::make_pair(battle_skill.skill,
//                                      battle_skill.skill->getValue());
//     skill_probabilities.push_back(skill_pair);
//   }

//   Helpers::normalizePair(begin(skill_probabilities), end(skill_probabilities));
// }

// /*
//  * Description: Constructs a unfiorm probability distribution for the list of
//  *              possible items to use based upon each item's skill use's
//  *              arbitrary value.
//  *
//  * Inputs: none
//  * Output: bool - true if there were no problems in creating the distribution
//  */
// /* Constructs a uniform probability distribution from Items */
// bool AIModule::buildUniformItems()
// {
//   auto error_occured = false;
//   item_probabilities.clear();

//   for (auto battle_item : valid_items)
//   {
//     if (battle_item.item != nullptr)
//     {
//       auto item_skill = battle_item.item->getUseSkill();

//       if (item_skill != nullptr)
//       {
//         auto value = item_skill->getValue();
//         item_probabilities.push_back(std::make_pair(battle_item.item, value));
//       }
//       else
//       {
//         std::cerr << "[Error]: Null skill use for item ptr" << std::endl;
//         error_occured = true;
//       }
//     }
//     else
//     {
//       std::cerr << "[Error]: Null item ptr" << std::endl;
//       error_occured = true;
//     }
//   }

//   if (!error_occured)
//     Helpers::normalizePair(begin(item_probabilities), end(item_probabilities));

//   return error_occured;
// }

// /*
//  * Description: Calculates probabilities for an AI to choose between taking
//  *              an action of a certain type (SKILL, ITEM, etc.) for all levels
//  *              of AI. Generally, skill will be among the highest choice.
//  *
//  * Inputs: none
//  * Output: none
//  */
// void AIModule::calculateActionTypeChances()
// {
//   /* First check if Skills or Items can be chosen */
//   auto can_choose_skill = canSelectSkill();
//   auto can_choose_item  = canSelectItem();
//   auto can_select_guard = canSelectGuard();

//   /* Compute the factors for choosing each available action type
//    *
//    * Skill - based on the base skill factor
//    * Item  - the less QD the user has, the more lean to using an Item
//    */
//   auto qd_percent  = parent->getQDPercent();
//   std::vector<std::pair<ActionType, float>> act_typ_chances;

//   if (can_choose_skill)
//   {
//     float skill_lean_factor = 0;

//     if (difficulty == AIDifficulty::RANDOM)
//       skill_lean_factor = kRAI_BASE_SKILL_FACTOR;
//     else if (difficulty == AIDifficulty::PRIORITY)
//       skill_lean_factor = kPAI_BASE_SKILL_FACTOR;

//     /* Adjust the skill lean factor and apply to chance to use skill */

//     skill_chance = calcFloatValVariance(skill_lean_factor);
//     act_typ_chances.push_back(std::make_pair(ActionType::SKILL, skill_chance));
//   }

//   if (can_choose_item)
//   {
//     float item_lean_factor = 0;

//     if (difficulty == AIDifficulty::RANDOM)
//     {
//       item_lean_factor  = AIModule::kRAI_BASE_ITEM_FACTOR;
//       item_lean_factor += (100 - qd_percent) * kRAI_LEAN_TO_ITEM_FACTOR;
//       item_chance       = item_lean_factor;
//     }
//     else if (difficulty == AIDifficulty::PRIORITY)
//     {
//       item_lean_factor  = kPAI_BASE_ITEM_FACTOR;
//       item_lean_factor += (100 - qd_percent) * kPAI_LEAN_TO_ITEM_FACTOR;
//       item_chance       = item_lean_factor;
//     }

//     /* Adjust item lean factor with variance and apply to item chance */
//     item_chance = calcFloatValVariance(item_lean_factor);
//     act_typ_chances.push_back(std::make_pair(ActionType::ITEM, item_chance));
//   }

//   if (can_select_guard)
//   {
//     guard_chance = kGAI_BASE_GUARD_FACTOR;
//     act_typ_chances.push_back(std::make_pair(ActionType::GUARD, guard_chance));
//   }

//   if (Helpers::enumVectorSearch(ActionType::DEFEND, valid_action_types))
//   {
//     defend_chance = kGAI_BASE_DEFEND_FACTOR;
//     act_typ_chances.push_back(std::make_pair(ActionType::DEFEND,
//                                               defend_chance));
//   }

//   if (Helpers::enumVectorSearch(ActionType::IMPLODE, valid_action_types))
//   {
//     implode_chance = kGAI_BASE_IMPLODE_FACTOR;
//     act_typ_chances.push_back(std::make_pair(ActionType::IMPLODE,
//                                               implode_chance));
//   }

//   if (Helpers::enumVectorSearch(ActionType::RUN, valid_action_types))
//   {
//     run_chance = kGAI_BASE_RUN_FACTOR;
//     act_typ_chances.push_back(std::make_pair(ActionType::RUN, run_chance));
//   }

//   if (Helpers::enumVectorSearch(ActionType::PASS, valid_action_types))
//   {
//     pass_chance = kGAI_BASE_PASS_FACTOR;
//     act_typ_chances.push_back(std::make_pair(ActionType::PASS, pass_chance));
//   }

//   if (act_typ_chances.size() > 0)
//   {
//     /* Build a normalized distribution of the calculated action type chances,
//      * and randomly select an action type based on the probability weights */
//     auto it_beg = begin(act_typ_chances);
//     auto it_end = end(act_typ_chances);
//     auto ra_flt = Helpers::randFloat(0, 1);

//     Helpers::normalizePair(it_beg, it_end);
//     auto it = Helpers::selectNormalizedPair(ra_flt, it_beg, it_end);

//     /*  Assign the chosen action type */
// #ifdef UDEBUG
//     std::cout << "AIModule Chosen Action Type: "
//               << Helpers::actionTypeToStr((*it).first) << std::endl;
// #endif
//     chosen_action_type = (*it).first;
//   }
//   else
//   {
// #ifdef UDEBUG
//     std::cout << "[Warning] Enemy has no valid action types available."
//               << std::endl;
// #endif
//   }

//   /* Set flag so Battle knows that the action type has been chosen */
//   setFlag(AIState::ACTION_TYPE_CHOSEN, true);
// }

// /*
//  * Description: Asserts that the AI module is in a state where an appropriate
//  *              action choice can be made.
//  *
//  * Inputs: none
//  * Output: bool - true if an action can be made
//  */
// bool AIModule::canSelectAction()
// {
//   if (getFlag(AIState::SELECTION_COMPLETE))
//     return false;
//   if (parent == nullptr)
//     return false;
//   if (valid_action_types.size() == 0)
//     return false;

//   return true;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::canSelectGuard()
// {
//   auto can_guard = true;

//   can_guard &= (parent->getGuard() == nullptr);
//   can_guard &= Helpers::enumVectorSearch(ActionType::GUARD, valid_action_types);

//   if (can_guard)
//   {
//     auto alive_person = false;

//     for (auto it = begin(friend_targets); it != end(friend_targets); ++it)
//       alive_person &= (*it)->getBFlag(BState::ALIVE);

//     if (!alive_person)
//       can_guard = false;
//   }

//   return can_guard;
// }

// /*
//  * Description: Asserts that SKILL is a valid action type to be chosen
//  *
//  * Inputs: none
//  * Output: bool - true if SKILL is a valid action type
//  */
// bool AIModule::canSelectSkill()
// {
//   auto found_skill = Helpers::enumVectorSearch(ActionType::SKILL,
//                                                valid_action_types);

//   if (found_skill)
//   {
//     auto target_found = false;

//     for (auto it = begin(valid_skills); it != end(valid_skills); ++it)
//       target_found |= battleSkillValid(*it);

//     return target_found;
//   }

//   return false;
// }

// /*
//  * Description: Asserts that ITEM is a valid action type to be chosen
//  *
//  * Inputs: none
//  * Output: bool - true if ITEM is a valid action type
//  */
// bool AIModule::canSelectItem()
// {
//   auto found_item = Helpers::enumVectorSearch(ActionType::ITEM,
//                                               valid_action_types);

//   if (found_item)
//   {
//     auto target_found = false;

//     for (auto it = begin(valid_items); it != end(valid_items); ++it)
//       target_found |= battleItemValid(*it);

//     return target_found;
//   }

//   return false;
// }

// /*
//  * Description: Given a base_value of float, calculates a random value based
//  *              upon the kGAI_VARIANCE for that float value to allow for some
//  *              playful variance in number calculations.
//  *
//  * Inputs: base_value - starting float value
//  * Output: float - the new value with some randomness calculated in
//  */
// float AIModule::calcFloatValVariance(const float &base_value)
// {
//   auto min_var = kGAI_VARIANCE;
//   auto max_var = kGAI_VARIANCE;

//   if (base_value < min_var)
//     min_var = base_value;

//   return Helpers::randFloat(base_value - min_var, base_value + max_var);
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output: bool - true
//  */
// bool AIModule::clearInvalid()
// {
//   valid_items.erase(std::remove_if(begin(valid_items), end(valid_items),
//       [&](const BattleItem &battle_item) -> bool
//       {
//         return !battleItemValid(battle_item);
//       }),
//       end(valid_items));
//   valid_skills.erase(std::remove_if(begin(valid_skills), end(valid_skills),
//       [&](const BattleSkill &battle_skill) -> bool
//       {
//         return !battleSkillValid(battle_skill);
//       }), end(valid_skills));

//   return true;
// }

// /*
//  * Description: Selects an action for a Random level AI based on the already
//  *              chosen type of action.
//  *
//  * Inputs: none
//  * Output: bool - true if an action was able to take place
//  */
// bool AIModule::selectRandomAction()
// {
//   auto action_index_selected = false;

//   /* Randomly select possible actions based on the selected action type */
//   if (chosen_action_type == ActionType::SKILL)
//   {
//     auto skills_size = valid_skills.size();
//     auto rand_value = Helpers::randU(1, skills_size);

//     chosen_skill = valid_skills.at(rand_value - 1).skill;
//     chosen_action_index = rand_value;
//     action_index_selected = true;
//   }
//   else if (chosen_action_type == ActionType::ITEM)
//   {
//     auto items_size = valid_items.size();
//     auto rand_value = Helpers::randU(1, items_size);

//     chosen_item = valid_items[rand_value - 1].item;
//     chosen_action_index = rand_value;
//     action_index_selected = true;
//   }
//   else if (chosen_action_type == ActionType::GUARD ||
//            chosen_action_type == ActionType::DEFEND ||
//            chosen_action_type == ActionType::IMPLODE ||
//            chosen_action_type == ActionType::RUN ||
//            chosen_action_type == ActionType::PASS)
//   {
//     action_index_selected = true;
//   }
//   else
//   {
// #ifdef UDEBUG
//     std::cout << "[Error]: Invalid Action type chosen by enemy." << std::endl;
// #endif
//   }

//   if (action_index_selected)
//     setFlag(AIState::ACTION_INDEX_CHOSEN, true);

//   return action_index_selected;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::selectRandomTargets()
// {
//   auto successful = false;
//   Person* target_ptr  = nullptr;

//   if (action_scope == ActionScope::ONE_ENEMY)
//   {
//     std::cout << "Selecitng Random foe targets of these foes: " << std::endl;

//     for (auto& foe : foe_targets)
//       std::cout << "Foe: " << foe->getName() << std::endl;
//     target_ptr = addRandomTarget(foe_targets);
//   }
//   else if (action_scope == ActionScope::ONE_ALLY ||
//            action_scope == ActionScope::ONE_ALLY_NOT_USER ||
//            action_scope == ActionScope::ONE_ALLY_KO)
//   {
//     target_ptr = addRandomTarget(friend_targets);
//   }
//   else if (action_scope == ActionScope::ONE_TARGET ||
//            action_scope == ActionScope::NOT_USER)
//   {
//     auto temp_vector = foe_targets;
//     temp_vector.insert(end(temp_vector), begin(foe_targets), end(foe_targets));
//     target_ptr = addRandomTarget(temp_vector);
//   }

//   if (target_ptr != nullptr)
//     successful = true;

//   if (!successful)
//   {
//     if (action_scope == ActionScope::USER)
//     {
//       chosen_targets.push_back(parent);
//       successful = true;
//     }
//     else if (action_scope == ActionScope::TWO_ENEMIES)
//     {
//       target_ptr = addRandomTarget(foe_targets);

//       if (target_ptr != nullptr)
//       {
//         for (size_t i = 0; i < foe_targets.size(); i++)
//           if (foe_targets.at(i) == target_ptr)
//             foe_targets.erase(begin(foe_targets) + i);

//         if (foe_targets.size() > 0)
//           target_ptr = addRandomTarget(foe_targets);
//       }
//     }
//     else if (action_scope == ActionScope::TWO_ALLIES)
//     {
//       target_ptr = addRandomTarget(friend_targets);

//       if (target_ptr != nullptr)
//       {
//         for (size_t i = 0; i < friend_targets.size(); i++)
//           if (friend_targets.at(i) == target_ptr)
//             friend_targets.erase(begin(friend_targets) + i);

//         if (friend_targets.size() > 0)
//           target_ptr = addRandomTarget(friend_targets);
//       }
//     }

//     if (target_ptr != nullptr)
//       successful = true;
//   }

//   if (!successful)
//   {
//     if (action_scope == ActionScope::ALL_ALLIES ||
//         action_scope == ActionScope::ALL_ALLIES_KO)
//     {
//       chosen_targets = friend_targets;
//       successful = true;
//     }
//     else if (action_scope == ActionScope::ALL_ENEMIES)
//     {
//       chosen_targets = foe_targets;
//       successful = true;
//     }
//     else if (action_scope == ActionScope::ALL_TARGETS ||
//              action_scope == ActionScope::ALL_NOT_USER)
//     {
//       chosen_targets = friend_targets;
//       chosen_targets.insert(end(chosen_targets), begin(foe_targets),
//                             end(foe_targets));
//       successful = true;
//     }
//     else if (action_scope == ActionScope::ONE_PARTY)
//     {
//       auto rand_bool = Helpers::flipCoin();
//       chosen_targets = foe_targets;

//       if (rand_bool)
//         chosen_targets = friend_targets;

//       successful = true;
//     }
//   }

//   if (successful)
//   {
//     setFlag(AIState::ACTION_TARGETS_CHOSEN, true);
//     setFlag(AIState::SELECTION_COMPLETE, true);
//   }

//   return successful;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::selectPriorityAction()
// {
//   auto action_index_selected = false;

//   /* Randomly select possible actions based on the selected action type */
//   if (chosen_action_type == ActionType::SKILL)
//   {
//     buildUniformSkills();

//     action_index_selected = true;
//   }
//   else if (chosen_action_type == ActionType::ITEM)
//   {
//     buildUniformItems();

//     action_index_selected = true;
//   }
//   else if (chosen_action_type == ActionType::GUARD)
//   {
//     //TODO: Action index selection [07-20-14]
//   }
//   else if (chosen_action_type == ActionType::DEFEND)
//   {
//     //TODO: Action index selection [07-20-14]
//   }
//   else if (chosen_action_type == ActionType::IMPLODE)
//   {
//     action_index_selected = true;
//     //TODO: Action index selection [07-20-14]
//   }
//   else if (chosen_action_type == ActionType::RUN)
//   {
//     //TODO: Action index selection [07-20-14]
//   }
//   else if (chosen_action_type == ActionType::PASS)
//   {
//     //TODO: Action index selection [07-20-14]
//   }
//   else
//   {
// #ifdef UDEBUG
//     std::cout << "[Error]: Invalid Action type chosen by enemy." << std::endl;
// #endif
//   }

//   if (action_index_selected)
//     setFlag(AIState::ACTION_INDEX_CHOSEN, true);

//   return action_index_selected;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::selectPriorityTargets()
// {
//   return true;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::selectTacticalAction()
// {
//   return false;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::selectTacticalTargets()
// {
//   return false;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::selectDeepThoughtAction()
// {
//   return false;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::selectDeepThoughtTargets()
// {
//   return false;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void AIModule::loadDefaults()
// {
//   setFlag(AIState::ACTION_TYPE_CHOSEN, false);
//   setFlag(AIState::ACTION_INDEX_CHOSEN, false);
//   setFlag(AIState::ACTION_TARGETS_CHOSEN, false);
//   setFlag(AIState::SCOPE_ASSIGNED, false);
//   setFlag(AIState::TARGETS_ASSIGNED, false);
//   setFlag(AIState::SELECTION_COMPLETE, false);
//   setFlag(AIState::ADD_TO_RECORD, true);

//   difficulty = kDEFAULT_DIFFICULTY;
//   prim_personality = kDEFAULT_PERSONALITY;
//   secd_personality = kDEFAULT_PERSONALITY;

//   valid_action_types.clear();
//   chosen_action_type = ActionType::NONE;

//   valid_skills.clear();
//   valid_items.clear();

//   chosen_action_index = -1;
//   chosen_skill = nullptr;
//   chosen_item  = nullptr;

//   parent = nullptr;

//   friend_targets.clear();
//   foe_targets.clear();
//   chosen_targets.clear();

//   action_record.clear();

//   actions_elapsed_total  = 0;
//   turns_elapsed_total    = 0;
//   actions_elapsed_battle = 0;
//   turns_elapsed_battle   = 0;
//   battles_elapsed        = 0;

//   skill_chance   = 0;
//   item_chance    = 0;
//   guard_chance   = 0;
//   defend_chance  = 0;
//   implode_chance = 0;
//   run_chance     = 0;
//   pass_chance    = 0;

//   running_config = nullptr;
// }

// /*=============================================================================
//  * PUBLIC FUNCTIONS
//  *============================================================================*/

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::addActionToRecord()
// {
//   if (!getFlag(AIState::ADD_TO_RECORD))
//     return true;

//   if (action_record.size() < static_cast<uint32_t>(kMAXIMUM_RECORD_SIZE))
//   {
//     bool valid_action = true;

//     valid_action &= (chosen_action_type != ActionType::NONE);

//     if (valid_action)
//     {
//       ActionRecord new_action;

//       new_action.action_type  = chosen_action_type;
//       new_action.qd_cost_paid = qd_cost_paid;
//       new_action.targets      = chosen_targets;
//       new_action.skill_used   = chosen_skill;
//       new_action.item_used    = chosen_item;

//       action_record.push_back(new_action);

//       return true;
//     }
//   }

//   return false;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::calculateAction()
// {
//   auto action_selected = false;

//   calculateActionTypeChances();

//   if (canSelectAction())
//   {
//     /* Clear the invalid selections of BattleItem/BattleSkill selections */
//     clearInvalid();

//     if (difficulty == AIDifficulty::RANDOM)
//       action_selected = selectRandomAction();
//     else if (difficulty == AIDifficulty::PRIORITY)
//       action_selected = selectPriorityAction();
//     else if (difficulty == AIDifficulty::TACTICAL)
//       action_selected = selectTacticalAction();
//     else if (difficulty == AIDifficulty::DEEP_THOUGHT)
//       action_selected = selectDeepThoughtAction();
//   }

//   return action_selected;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::calculateTargets()
// {
//   auto targets_selected = false;

//   if (difficulty == AIDifficulty::RANDOM)
//     targets_selected = selectRandomTargets();
//   else if (difficulty == AIDifficulty::PRIORITY)
//     targets_selected = selectPriorityTargets();
//   else if (difficulty == AIDifficulty::TACTICAL)
//     targets_selected = selectTacticalTargets();
//   else if (difficulty == AIDifficulty::DEEP_THOUGHT)
//     targets_selected = selectDeepThoughtTargets();

//   return targets_selected;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void AIModule::incrementActions()
// {
//   actions_elapsed_battle++;
//   actions_elapsed_total++;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void AIModule::incrementTurns()
// {
//   turns_elapsed_battle++;
//   turns_elapsed_total++;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void AIModule::incrementBattles()
// {
//   battles_elapsed++;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void AIModule::print(const bool &simple, const bool &print_flags,
//                      const bool &print_record)
// {
//   std::cout << " === AI Module === \n";

//   if (simple)
//   {
//     std::cout << "Parent: ";

//     if (parent == nullptr)
//       std::cout << "nullptr \n";
//     else
//       std::cout << parent->getName() << "\n";

//     std::cout << "D: " << Helpers::aiDifficultyToStr(difficulty) << " PP: "
//               << Helpers::aiPersonalityToStr(prim_personality) << " SP: "
//               << Helpers::aiPersonalityToStr(secd_personality) << "\n";

//     std::cout << "VAT Size: " << valid_action_types.size() << " Chosen AT: "
//               << Helpers::actionTypeToStr(chosen_action_type) << "\n";

//     // if (valid_skills != nullptr)
//     // {
//     //   std::cout << "Sk Size: " << valid_skills->getSize() << " It Size: "
//     //             << valid_items.size() << "\n";
//     // }
//     std::cout << "QD Paid: " << qd_cost_paid << " Action Index: "
//               << chosen_action_index << "\n";

//     if (chosen_skill != nullptr)
//       std::cout << "C. Skill: " << chosen_skill->getName() << "\n";
//     if (chosen_item != nullptr)
//       std::cout << "C. Item: " << chosen_item->getName() << "\n";

//     std::cout << "A. Targets Size: "  << friend_targets.size()
//               << " E. Targets Size: " << foe_targets.size() << "\n"
//               << "C. Targets Size: " << chosen_targets.size()
//               << " A.R. Size: "       << action_record.size() << "\n";

//     std::cout << "Sk. Ch: " << skill_chance << " It. Ch: " << item_chance
//               << "\n" << "Gr. Ch: " << guard_chance << " Df. Ch: "
//               << defend_chance << "\n" << "Im. Ch: " << implode_chance
//               << " Rn. Ch: " << run_chance << "\n";
//   }
//   else
//   {
//     std::cout << "Skill Chance:  "  << skill_chance << "\n";
//     std::cout << "Item Chance: "    << item_chance << "\n";
//     std::cout << "Guard Chance: "   << guard_chance << "\n";
//     std::cout << "Defend Chance: "  << defend_chance << "\n";
//     std::cout << "Implode Chance: " << implode_chance << "\n";
//     std::cout << "Run Chance: "     << run_chance << "\n";
//     std::cout << "Pass Chance: "    << pass_chance << "\n";

//     std::cout << "Difficulty: " << Helpers::aiDifficultyToStr(difficulty);
//     std::cout << "\nPrimary Personality: "
//               << Helpers::aiPersonalityToStr(prim_personality) << "\n";
//     std::cout << "Secondary Personality: "
//               << Helpers::aiPersonalityToStr(secd_personality) << "\n";

//     std::cout << " --- Valid Action Types --- " << "\n";

//     for (auto valid_action_type : valid_action_types)
//       std::cout << Helpers::actionTypeToStr(valid_action_type ) << "\n";

//     std::cout << "Chosen Action Type: "
//               << Helpers::actionTypeToStr(chosen_action_type) << "\n";
//     std::cout << "Action Scope: " << Helpers::actionScopeToStr(action_scope);

//     std::cout << "\nValid Skills: " << "\n";

//     if (parent != nullptr)
//     {
//       // auto set_elements = valid_skills->getElements(parent->getLevel());
//       // for (auto element : set_elements)
//       //   if (element.skill != nullptr)
//       //     std::cout << element.skill->getName() << "\n";
//     }

//     std::cout << "Valid Items: " <<"\n";

//     for (auto item : valid_items)
//       if (item.item != nullptr)
//         std::cout << item.item->getName() << "\n";

//     std::cout << "Chosen Action Index: " << chosen_action_index << "\n";
//     std::cout << "QD Cost Paid: " << qd_cost_paid << "\n";

//     std::cout << "Chosen Skill: ";

//     if (chosen_skill == nullptr)
//       std::cout << "nullptr" << "\n";
//     else
//       std::cout << chosen_skill->getName() << "\n";

//     std::cout << "Valid Item: ";

//     if (chosen_item == nullptr)
//       std::cout << "nullptr" << "\n";
//     else
//       std::cout << chosen_item->getName() << "\n";

//     std::cout << "Friends Targets: " << "\n";

//     for (auto target : friend_targets)
//       std::cout << target->getName() << "\n";

//     std::cout << "Foe Targets: " << "\n";

//     for (auto target : foe_targets)
//       std::cout << target->getName() << "\n";

//     std::cout << "Chosen Targets: " << "\n";

//     for (auto target : chosen_targets)
//       std::cout << target->getName() << "\n";

//     std::cout << "Actions Elapsed Total: " << actions_elapsed_total << "\n";
//     std::cout << "Turns Elapsed Total: " << turns_elapsed_total << "\n";
//     std::cout << "Actions Elapsed Battle: " << actions_elapsed_battle << "\n";
//     std::cout << "Turns_Elapsed_Battle: " << turns_elapsed_battle << "\n";
//     std::cout << "Battles Elapsed: " << battles_elapsed << "\n";

//     std::cout << "Running Config Set? " << (running_config != nullptr) << "\n";
//   }

//   if (print_flags)
//   {
//     std::cout << "ACTION TYPE CHOSEN" << getFlag(AIState::ACTION_TYPE_CHOSEN);
//     std::cout << "\nACTION INDEX CHOSEN"
//               << getFlag(AIState::ACTION_INDEX_CHOSEN);
//     std::cout << "\nACTION TARGETS CHOSEN"
//               << getFlag(AIState::ACTION_TARGETS_CHOSEN);
//     std::cout << "\nSCOPE ASSIGNED" << getFlag(AIState::SCOPE_ASSIGNED);
//     std::cout << "\nTARGETS ASSIGNED" << getFlag(AIState::TARGETS_ASSIGNED) ;
//     std::cout << "\nSELECTION COMPLETE" << getFlag(AIState::SELECTION_COMPLETE);
//     std::cout << "\nADD TO RECORD: " << getFlag(AIState::ADD_TO_RECORD) << "\n";
//   }

//   if (print_record)
//   {
//     std::cout << "\nAction Record: " << "\n";
//     auto record_index = 0;

//     for (auto record : action_record)
//     {
//       std::cout << "Index: " << record_index++ << "\n";
//       std::cout << "Action Type: "
//                 << Helpers::actionTypeToStr(record.action_type);
//       std::cout << "\nQD Cost Paid: " << record.qd_cost_paid << "\n";
//       std::cout << "Targets: ";

//       for (auto target : record.targets)
//         std::cout << target->getName() << " ";


//       std::cout << "\nSkill Used: ";

//       if (record.skill_used == nullptr)
//         std::cout << "nullptr";
//       else
//         std::cout << record.skill_used->getName() << "\n";

//       std::cout << "\nItem Used: ";

//       if (record.item_used == nullptr)
//         std::cout << "nullptr";
//       else
//         std::cout << record.item_used->getName() << "\n\n";
//     }

//   }

//   std::cout << " === // AI Module ===\n" << std::endl;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void AIModule::resetForNewTurn(Person* const parent)
// {
//   setFlag(AIState::ACTION_TYPE_CHOSEN, false);
//   setFlag(AIState::ACTION_INDEX_CHOSEN, false);
//   setFlag(AIState::ACTION_TARGETS_CHOSEN, false);
//   setFlag(AIState::SCOPE_ASSIGNED, false);
//   setFlag(AIState::TARGETS_ASSIGNED, false);
//   setFlag(AIState::SELECTION_COMPLETE, false);

//   chosen_action_type = ActionType::NONE;
//   action_scope       = ActionScope::NO_SCOPE;

//   valid_skills.clear();
//   valid_items.clear();

//   chosen_action_index = -1;
//   qd_cost_paid        = 0;
//   chosen_skill        = nullptr;
//   chosen_item         = nullptr;

//   skill_probabilities.clear();
//   item_probabilities.clear();
//   friend_targets.clear();
//   foe_targets.clear();
//   chosen_targets.clear();

//   if (parent != nullptr)
//     valid_action_types = parent->getValidActions();
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void AIModule::resetForNewBattle()
// {
//   resetForNewTurn();

//   actions_elapsed_battle = 0;
//   turns_elapsed_battle   = 0;
//   battles_elapsed        = 0;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// ActionScope AIModule::getActionScope()
// {
//   return action_scope;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// bool AIModule::getFlag(const AIState &test_flag)
// {
//   return static_cast<bool>((flags & test_flag) == test_flag);
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// ActionType AIModule::getActionType()
// {
//   return chosen_action_type;
// }

// /*
//  * Description: Returns the currently selected action index, ex. along
//  *              the vector of BattleItem/BattleSkill
//  *
//  * Inputs: none
//  * Output: int32_t - the currently selected action index
//  */
// int32_t AIModule::getActionIndex()
// {
//   return chosen_action_index;
// }

// /*
//  * Description: Returns the vector of chosen targets
//  *
//  * Inputs: none
//  * Output: std::vector<Person*> - vector of ptrs of targets
//  */
// std::vector<Person*> AIModule::getChosenTargets()
// {
//   return chosen_targets;
// }

// /*
//  * Description: Returns the difficulty of the AI
//  *
//  * Inputs: none
//  * Output: AIDifficulty - the enumerated difficulty of the module
//  */
// AIDifficulty AIModule::getDifficulty()
// {
//   return difficulty;
// }

// /*
//  * Description: Returns the parent of the AIModule
//  *
//  * Inputs: none
//  * Output: Person* - ptr to the parent object
//  */
// Person* AIModule::getParent()
// {
//   return parent;
// }

// /*
//  * Description: Returns the primary enumerated personality type
//  *
//  * Inputs: none
//  * Output: AIPersonality - the enumerated personality type
//  */
// AIPersonality AIModule::getPrimPersonality()
// {
//   return prim_personality;
// }

// /*
//  * Description: Returns the enumerated secondary personality type
//  *
//  * Inputs: none
//  * Output: AIPersonality - the enumerated personality type
//  */
// AIPersonality AIModule::getSecdPersonality()
// {
//   return secd_personality;
// }

// /*
//  * Description: Returns the currently selected Skill
//  *
//  * Inputs: none
//  * Output: Skill* - ptr to the Skill the AI module will use
//  */
// Skill* AIModule::getSelectedSkill()
// {
//   return chosen_skill;
// }

// /*
//  * Description: Returns the currently selected Item from the AI Module.
//  *
//  * Inputs: none
//  * Output: Item* - ptr to the Item the AI module will use.
//  */
// Item* AIModule::getSelectedItem()
// {
//   return chosen_item;
// }

// /*
//  * Description: Returns the number of actions this AI module has taken total
//  *
//  * Inputs: none
//  * Output: uint16_t - the number of actions this AI module has ever taken.
//  */
// uint16_t AIModule::getActionsElapsedTotal()
// {
//   return actions_elapsed_total;
// }

// /*
//  * Description: Returns the number of turns that this AI module has taken total
//  *
//  * Inputs: none
//  * Output: uint16_t - the number of turns this AI module has ever taken.
//  */
// uint16_t AIModule::getTurnsElapsedTotal()
// {
//   return turns_elapsed_total;
// }

// /*
//  * Description: Returns the number of actions that this AI module has taken
//  *              (up to 3 per turn) during the current battle.
//  *
//  * Inputs: none
//  * Output: uint16_t - the number of actions AI module has taken this battle
//  */
// uint16_t AIModule::getActionsElapsed()
// {
//   return actions_elapsed_battle;
// }

// /*
//  * Description: Returns the number of turns that this AI module has taken
//  *              during the current Battle.
//  *
//  * Inputs: none
//  * Output: uint16_t - the number of turns AI module has taken this battle
//  */
// uint16_t AIModule::getTurnsElapsed()
// {
//   return turns_elapsed_battle;
// }

// /*
//  * Description: Assigns a given AIState flag a given boolean value.
//  *
//  * Inputs: flag - enumerated flag to be assigned the given value.
//  *         set_value - boolean value to set the flag to.
//  * Output: none
//  */
// void AIModule::setFlag(AIState flag, const bool &set_value)
// {
//   (set_value) ? (flags |= flag) : (flags &= ~flag);
// }

// /*
//  * Description: Assigns the scope of the action for the current selected action
//  *
//  * Inputs: new_action_scope - enumerated ActionScope assigned
//  * Output: bool - true if the ActionScope has a valid scope
//  */
// bool AIModule::setActionScope(const ActionScope &new_action_scope)
// {
//   if (new_action_scope != ActionScope::NO_SCOPE)
//   {
//     action_scope = new_action_scope;
//     setFlag(AIState::SCOPE_ASSIGNED, true);

//     return true;
//   }

//   return false;
// }

// /*
//  * Description: Assigns the vector of valid action types that the AI may
//  *              choose from. This list is generally grabbed from Person and
//  *              this function called by Battle.
//  *
//  * Inputs: new_valid_action_types - the vector of valid action types
//  * Output: bool - true if the action types are possible
//  */
// bool AIModule::setActionTypes(std::vector<ActionType> new_valid_action_types)
// {
//   valid_action_types = new_valid_action_types;

//   return !valid_action_types.empty();
// }

// /*
//  * Description: Assigns the vector of BattleSkills (constructed in Battle
//  *              containing the skill and potential targets) that the
//  *              AI module may choose from.
//  *
//  * Inputs: new_skills - vector of BattleSkill of new items.
//  * Output: bool - true if the assignment of new skills is non-zero size.
//  */
// // bool AIModule::setSkills(std::vector<BattleSkill> new_skills)
// // {
// //   valid_skills = new_skills;

// //   return (valid_skills.size() != 0);
// // }

// /*
//  * Description: Assigns the vector of BattleItems (constructed in Battle
//  *              containing the Item's skill and potential targets) that the
//  *              AI module may choose from.
//  *
//  * Inputs: new_items - vector of BattleItem objects
//  * Output: bool - true if the assignment of new items is non-zero size.
//  */
// // bool AIModule::setItems(std::vector<BattleItem> new_items)
// // {
// //   valid_items = new_items;

// //   return (valid_items.size() != 0);
// // }

// /*
//  * Description: Assigns the vector of friends targets which maybe chosen from.
//  *
//  * Inputs: new_valid_targets - vector of persons which represent allies.
//  * Output: bool - truism
//  */
// bool AIModule::setFriendTargets(std::vector<Person*> new_valid_targets)
// {
//   friend_targets = new_valid_targets;

//   return true;
// }

// /*
//  * Description: Assigns a new vector of foe targets for the AI Module to choose
//  *
//  * Inputs: new_valid_targets - the vector of foe pointers to choose as targets
//  * Output: bool - truism
//  */
// bool AIModule::setFoeTargets(std::vector<Person*> new_valid_targets)
// {
//   foe_targets = new_valid_targets;

//   return true;
// }

// /*
//  * Description: Assigns a new level of difficulty for the AI
//  *
//  * Inputs: new_difficulty - enumerated difficulty to be assigned to the AI
//  * Output: bool - true if difficulty has been changed
//  */
// bool AIModule::setDifficulty(const AIDifficulty &new_difficulty)
// {
//   if (new_difficulty != difficulty)
//   {
//     difficulty = new_difficulty;

//     return true;
//   }

//   return false;
// }

// /*
//  * Description: Assigns a new parent for the AI module
//  *
//  * Inputs: new_parent - pointer to the new parent object
//  * Output: bool - true if the new parent is nullptr (valid parent)
//  */
// bool AIModule::setParent(Person* const new_parent)
// {
//   parent = new_parent;

//   return (new_parent != nullptr);
// }

// /*
//  * Description: Assigns a new primary personality for the AI module
//  *
//  * Inputs: new_personality - new enumerated personality type to be assigned
//  * Output: bool - true if the personality type was changed
//  */
// bool AIModule::setPrimPersonality(const AIPersonality &new_personality)
// {
//   if (new_personality != prim_personality)
//   {
//     prim_personality = new_personality;

//     return true;
//   }

//   return false;
// }

// /*
//  * Description: Assigns a new secondary personality for the AI module
//  *
//  * Inputs: new_personality - new enumerated personality type to be assigned
//  * Output: bool - true if the personality type was changed
//  */
// bool AIModule::setSecdPersonality(const AIPersonality &new_personality)
// {
//   if (new_personality != secd_personality)
//   {
//     secd_personality = new_personality;

//     return true;
//   }

//   return false;
// }

// /*
//  * Description: Assigns a new running config for the AI module
//  *
//  * Inputs: new_running_config - ptr to options for a new running configuration
//  * Output: bool - true if the running config was assigned
//  */
// bool AIModule::setRunningConfig(Options* const new_running_config)
// {
//   if (new_running_config != running_config)
//   {
//     running_config = new_running_config;

//     return true;
//   }

//   return false;
// }
