/*******************************************************************************
* Class Name: AI Module [Implementation]
* Date Created: June 22, 2014
* Inheritance: None
* Description: The AI Module is an object describing the difficulty and 
*              personality for enemy decision making in Battle.
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Battle/AIModule.h"
#include "Game/Player/Person.h"

/* Default Difficulty
 * Default Personality
 * Maximum Record Size
 */
const AIDifficulty AIModule::kDEFAULT_DIFFICULTY{AIDifficulty::RANDOM};
const AIPersonality AIModule::kDEFAULT_PERSONALITY{AIPersonality::MODERATOR};
const uint32_t AIModule::kMAXIMUM_RECORD_SIZE{500};

/* Random AI Offensive Factor
 * Random AI Defensive Factor
 * Random AI Default Target 
 */
const float AIModule::kRAI_OFF_FACTOR{1.35};
const float AIModule::kRAI_DEF_FACTOR{1.50};
const float AIModule::kRAI_BASE_SKILL_FACTOR{0.500};
const float AIModule::kRAI_BASE_ITEM_FACTOR{0.400};
const float AIModule::kRAI_LEAN_TO_ITEM_FACTOR{0.001};
const AITarget AIModule::kRAI_DEFAULT_TARGET{AITarget::RANDOM};

/* Priority AI Offensive Factor
 * Priority AI Defensive Factor
 * Priority AI Default Target 
 */
const float AIModule::kPAI_OFF_FACTOR{1.35};
const float AIModule::kPAI_DEF_FACTOR{1.50};
const float AIModule::kPAI_BASE_SKILL_FACTOR{0.500};
const float AIModule::kPAI_BASE_ITEM_FACTOR{0.400};
const float AIModule::kPAI_LEAN_TO_ITEM_FACTOR{0.007};
const AITarget AIModule::kPAI_DEFAULT_TARGET{AITarget::LOWEST_HP_FIRST};

/* Tactical AI //TODO
 */

/* Deep-Thought AI //TODO
 */

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
AIModule::AIModule()
{
  loadDefaults();
}

AIModule::AIModule(const AIDifficulty &diff,
                   const AIPersonality &prim_personality)
{
  loadDefaults();

  this->difficulty = diff;
  this->prim_personality = prim_personality;
}

AIModule::AIModule(const AIDifficulty &diff, 
                   const AIPersonality &prim_personality,
                   const AIPersonality &secd_personality)
  : AIModule(diff, prim_personality)
{
  this->secd_personality = secd_personality;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void AIModule::calculateActionTypeChances()
{
  /* First check if Skills or Items can be chosen */
  auto can_choose_skill = canSelectSkill();
  auto can_choose_item  = canSelectItem();

  /* Compute the factors for choosing each available action type
   * 
   * Skill - the more skills the user has, the more lean to using a Skill
   * Item - the less QD the user has, the more lean to using an Item
   */

  auto qd_percent = parent->getQDPercent();

  auto total_item_lean_factor = 0;

  if (difficulty == AIDifficulty::RANDOM)
  {
    skill_chance = kRAI_BASE_SKILL_FACTOR;
    item_chance  = kRAI_BASE_ITEM_FACTOR;
    total_item_lean_factor = qd_percent * kRAI_LEAN_TO_ITEM_FACTOR;
  }
  else if (difficulty == AIDifficulty::PRIORITY)
  {
    skill_chance = kPAI_BASE_SKILL_FACTOR;
    item_chance  = kPAI_BASE_ITEM_FACTOR;
    total_item_lean_factor = qd_percent * kPAI_LEAN_TO_ITEM_FACTOR;
  }
  
  if (can_choose_skill)
  {
  }

  if (can_choose_item)
  {

  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::canSelectAction()
{
  if (getFlag(AIState::SELECTION_COMPLETE))
    return false;
  if (parent == nullptr)
    return false;
  if (valid_action_types.size() == 0)
    return false;

  return true;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::canSelectSkill()
{
  return !(valid_skills->getSize() == 0);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::canSelectItem()
{
  return true;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::selectRandomAction()
{

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::selectPriorityAction()
{
  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::selectTacticalAction()
{
  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::selectDeepThoughtAction()
{
  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::loadDefaults()
{
  setFlag(AIState::ACTION_TYPE_CHOSEN, false);
  setFlag(AIState::ACTION_INDEX_CHOSEN, false);
  setFlag(AIState::ACTION_TARGETS_CHOSEN, false);
  setFlag(AIState::SCOPE_ASSIGNED, false);
  setFlag(AIState::TARGETS_ASSIGNED, false);
  setFlag(AIState::SELECTION_COMPLETE, false);
  setFlag(AIState::ADD_TO_RECORD, true);

  difficulty = kDEFAULT_DIFFICULTY;
  prim_personality = kDEFAULT_PERSONALITY;
  secd_personality = kDEFAULT_PERSONALITY;

  valid_action_types.clear();
  chosen_action_type = ActionType::NONE;

  valid_skills = nullptr;
  valid_items.clear();

  chosen_action_index = -1;
  chosen_skill = nullptr;
  chosen_item  = nullptr;
  
  parent = nullptr;

  friend_targets.clear();
  foe_targets.clear();
  chosen_targets.clear();

  action_record.clear();

  actions_elapsed_total  = 0;
  turns_elapsed_total    = 0;
  actions_elapsed_battle = 0;
  turns_elapsed_battle   = 0;

  running_config = nullptr;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool AIModule::addActionToRecord()
{
  if (action_record.size() < static_cast<uint32_t>(kMAXIMUM_RECORD_SIZE))
  {
    bool valid_action = true;

    valid_action &= (chosen_action_type != ActionType::NONE);

    if (chosen_skill != nullptr && chosen_item != nullptr)
      valid_action = false;

    if ((chosen_skill != nullptr || chosen_item != nullptr) &&
        chosen_targets.empty())
      valid_action = false;

    if (valid_action)
    {
      ActionRecord new_action;
 
      new_action.action_type  = chosen_action_type;
      new_action.qd_cost_paid = qd_cost_paid;
      new_action.targets      = chosen_targets;
      new_action.skill_used   = chosen_skill;
      new_action.item_used    = chosen_item;

      action_record.push_back(new_action);

      return true;
    }
  }

  return false;
}

bool AIModule::calculateAction()
{
  auto action_selected = false;

  if (canSelectAction())
  {
    if (difficulty == AIDifficulty::RANDOM)
      action_selected = selectRandomAction();
    else if (difficulty == AIDifficulty::PRIORITY)
      action_selected = selectPriorityAction();
    else if (difficulty == AIDifficulty::TACTICAL)
      action_selected = selectTacticalAction();
    else if (difficulty == AIDifficulty::DEEP_THOUGHT)
      action_selected = selectDeepThoughtAction();
  }

  return action_selected;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::incrementActions()
{
  actions_elapsed_battle++;
  actions_elapsed_total++;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::incrementTurns()
{
  turns_elapsed_battle++;
  turns_elapsed_total++;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::incrementBattles()
{
  battles_elapsed++;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::print(const bool &simple, const bool &print_flags,
                     const bool &print_record)
{
  std::cout << " === AI Module === \n";

  if (simple)
  {
    std::cout << "D: " << Helpers::aiDifficultyToStr(difficulty) << " PP: " 
              << Helpers::aiPersonalityToStr(prim_personality) << " SP: "
              << Helpers::aiPersonalityToStr(secd_personality) << "\n";
   
    std::cout << "VAT Size: " << valid_action_types.size() << "Chosen AT: "
              << Helpers::actionTypeToStr(chosen_action_type) << "\n";
    std::cout << "Sk Size: " << valid_skills->getSize() << " It Size: " 
              << valid_items.size() << "\n";
    std::cout << "QD Paid: " << qd_cost_paid << "Action Index: " 
              << chosen_action_index << "\n";

    if (chosen_skill != nullptr)
      std::cout << "C. Skill: " << chosen_skill->getName() << "\n";
    if (chosen_item != nullptr)
      std::cout << "C. Item: " << chosen_item->getName() << "\n";

    std::cout << "A. Targets Size: " << friend_targets.size()
              << "E. Targets Size: " << foe_targets.size()
              << " C. Targets Size: " << chosen_targets.size()
              << " A.R. Size: " << action_record.size() << "\n";
  }
  else
  {
    std::cout << "Difficulty: " << Helpers::aiDifficultyToStr(difficulty);
    std::cout << "\nPrimary Personality: " 
              << Helpers::aiPersonalityToStr(prim_personality) << "\n";
    std::cout << "Secondary Personality: "
              << Helpers::aiPersonalityToStr(secd_personality) << "\n";

    std::cout << " --- Valid Action Types --- " << "\n";

    for (auto valid_action_type : valid_action_types)
      std::cout << Helpers::actionTypeToStr(valid_action_type ) << "\n";

    std::cout << "Chosen Action Type: " 
              << Helpers::actionTypeToStr(chosen_action_type) << "\n";
    std::cout << "Action Scope: " << Helpers::actionScopeToStr(action_scope);

    std::cout << "\nValid Skills: " << "\n";
 
    if (parent != nullptr)
    {
      auto set_elements = valid_skills->getElements(parent->getLevel());
      for (auto element : set_elements)
        if (element.skill != nullptr)
          std::cout << element.skill->getName() << "\n";
    }

    std::cout << "Valid Items: " <<"\n";

    for (auto item : valid_items)
      if (item.first != nullptr)
        std::cout << item.first->getName() << "\n";

    std::cout << "Chosen Action Index: " << chosen_action_index << "\n";
    std::cout << "QD Cost Paid: " << qd_cost_paid << "\n";

    std::cout << "Chosen Skill: ";

    if (chosen_skill == nullptr)
      std::cout << "nullptr" << "\n";
    else
      std::cout << chosen_skill->getName() << "\n";

    std::cout << "Valid Item: ";

    if (chosen_item == nullptr)
      std::cout << "nullptr" << "\n";
    else
      std::cout << chosen_item->getName() << "\n";

    std::cout << "Friends Targets: " << "\n";

    for (auto target : friend_targets)
      std::cout << target->getName() << "\n";

    std::cout << "Foe Targets: " << "\n";

    for (auto target : foe_targets)
      std::cout << target->getName() << "\n";

    std::cout << "Chosen Targets: " << "\n";

    for (auto target : chosen_targets)
      std::cout << target->getName() << "\n";
   
    std::cout << "Actions Elapsed Total: " << actions_elapsed_total << "\n";
    std::cout << "Turns Elapsed Total: " << turns_elapsed_total << "\n";
    std::cout << "Actions Elapsed Battle: " << actions_elapsed_battle << "\n";
    std::cout << "Turns_Elapsed_Battle: " << turns_elapsed_battle << "\n";
    std::cout << "Battles Elapsed: " << battles_elapsed << "\n";

    std::cout << "Running Config Set? " << (running_config != nullptr) << "\n";
  }

  if (print_flags)
  {
    std::cout << "ACTION TYPE CHOSEN" << getFlag(AIState::ACTION_TYPE_CHOSEN);
    std::cout << "\nACTION INDEX CHOSEN" 
              << getFlag(AIState::ACTION_INDEX_CHOSEN);
    std::cout << "\nACTION TARGETS CHOSEN" 
              << getFlag(AIState::ACTION_TARGETS_CHOSEN);
    std::cout << "\nSCOPE ASSIGNED" << getFlag(AIState::SCOPE_ASSIGNED);
    std::cout << "\nTARGETS ASSIGNED" << getFlag(AIState::TARGETS_ASSIGNED) ;
    std::cout << "\nSELECTION COMPLETE" << getFlag(AIState::SELECTION_COMPLETE);
    std::cout << "\nADD TO RECORD: " << getFlag(AIState::ADD_TO_RECORD) << "\n";
  }

  if (print_record)
  {
    std::cout << "\nAction Record: " << "\n";
    auto record_index = 0;

    for (auto record : action_record)
    {
      std::cout << "Index: " << record_index++ << "\n";
      std::cout << "Action Type: "
                << Helpers::actionTypeToStr(record.action_type);
      std::cout << "\nQD Cost Paid: " << record.qd_cost_paid << "\n";
      std::cout << "Targets: ";

      for (auto target : record.targets)
        std::cout << target->getName() << " ";

      
      std::cout << "\nSkill Used: ";

      if (record.skill_used == nullptr)
        std::cout << "nullptr";
      else
        std::cout << record.skill_used->getName() << "\n";

      std::cout << "\nItem Used: ";

      if (record.item_used == nullptr)
        std::cout << "nullptr";
      else
        std::cout << record.item_used->getName() << "\n\n";
    }

  }
  else
  {
    std::cout << "Action Record Size: " << action_record.size() << std::endl;
  }

  std::cout << " === // AI Module === " << std::endl;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::resetForNewTurn(Person* const parent)
{
  setFlag(AIState::ACTION_TYPE_CHOSEN, false);
  setFlag(AIState::ACTION_INDEX_CHOSEN, false);
  setFlag(AIState::ACTION_TARGETS_CHOSEN, false);
  setFlag(AIState::SCOPE_ASSIGNED, false);
  setFlag(AIState::TARGETS_ASSIGNED, false);
  setFlag(AIState::SELECTION_COMPLETE, false);

  chosen_action_type   = ActionType::NONE;
  action_scope = ActionScope::NO_SCOPE;

  valid_skills = nullptr;

  valid_items.clear();

  chosen_action_index = -1;
  qd_cost_paid        = 0;
  chosen_skill        = nullptr;
  chosen_item         = nullptr;

  friend_targets.clear();
  foe_targets.clear();
  chosen_targets.clear();

  if (parent != nullptr)
  {
    valid_action_types = parent->getValidActions();
    valid_skills       = parent->getUseableSkills();
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::resetForNewBattle()
{
  resetForNewTurn();

  actions_elapsed_battle = 0;
  turns_elapsed_battle   = 0;
  battles_elapsed        = 0;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
ActionScope AIModule::getActionScope()
{
  return action_scope;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::getFlag(const AIState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
ActionType AIModule::getActionType()
{
  return chosen_action_type;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t AIModule::getActionIndex()
{
  return chosen_action_index;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::vector<Person*> AIModule::getChosenTargets()
{
  return chosen_targets;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AIDifficulty AIModule::getDifficulty()
{
  return difficulty;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Person* AIModule::getParent()
{
  return parent;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AIPersonality AIModule::getPrimPersonality()
{
  return prim_personality;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AIPersonality AIModule::getSecdPersonality()
{
  return secd_personality;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Skill* AIModule::getSelectedSkill()
{
  return chosen_skill; 
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Item* AIModule::getSelectedItem()
{
  return chosen_item;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint16_t AIModule::getActionsElapsedTotal()
{
  return actions_elapsed_total;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint16_t AIModule::getTurnsElapsedTotal()
{
  return turns_elapsed_total;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint16_t AIModule::getActionsElapsed()
{
  return actions_elapsed_battle;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint16_t AIModule::getTurnsElapsed()
{
  return turns_elapsed_battle;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::setFlag(AIState flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setActionScope(const ActionScope &new_action_scope)
{
  if (new_action_scope != ActionScope::NO_SCOPE)
  {
    action_scope = new_action_scope;
    setFlag(AIState::SCOPE_ASSIGNED, true);

    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setActionTypes(std::vector<ActionType> new_valid_action_types)
{
  valid_action_types = new_valid_action_types;

  return !valid_action_types.empty();
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setSkills(SkillSet* const new_skills)
{
  if (new_skills != nullptr)
  {
    valid_skills = new_skills;

    return true;
  }

  valid_skills = nullptr;

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setItems(std::vector<std::pair<Item*, uint16_t>> new_items)
{
  if (new_items.size() > 0)
  {
    valid_items = new_items;

    return true;
  }

  valid_items.clear();

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setFriendTargets(std::vector<Person*> new_valid_targets)
{
  friend_targets = new_valid_targets;

  return !(friend_targets.size() == 0);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setFoeTargets(std::vector<Person*> new_valid_targets)
{
  foe_targets = new_valid_targets;

  return !(foe_targets.size() == 0);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setDifficulty(const AIDifficulty &new_difficulty)
{
  if (new_difficulty != difficulty)
  {
    difficulty = new_difficulty;

    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setParent(Person* const new_parent)
{
  parent = new_parent;

  return (new_parent != nullptr);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setPrimPersonality(const AIPersonality &new_personality)
{
  if (new_personality != prim_personality)
  {
    prim_personality = new_personality;

    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setSecdPersonality(const AIPersonality &new_personality)
{
  if (new_personality != secd_personality)
  {
    secd_personality = new_personality;

    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setRunningConfig(Options* const new_running_config)
{
  if (new_running_config != running_config)
  {
    running_config = new_running_config;

    return true;
  }

  return false;
}