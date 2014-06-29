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
const float AIModule::kRANDOM_AI_OFF_FACTOR{1.35};
const float AIModule::kRANDOM_AI_DEF_FACTOR{1.50};
const AITarget AIModule::kRANDOM_AI_DEFAULT_TARGET{AITarget::RANDOM};

/* Priority AI Offensive Factor
 * Priority AI Defensive Factor
 * Priority AI Default Target 
 */
const float AIModule::kPRIORITY_AI_OFF_FACTOR{1.35};
const float AIModule::kPRIORITY_AI_DEF_FACTOR{1.50};
const AITarget AIModule::kPRIORITY_AI_DEFAULT_TARGET{AITarget::LOWEST_HP_FIRST};

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

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AIModule::loadDefaults()
{
  difficulty = kDEFAULT_DIFFICULTY;
  prim_personality = kDEFAULT_PERSONALITY;
  secd_personality = kDEFAULT_PERSONALITY;

  valid_action_types.clear();
  chosen_action_type = ActionType::NONE;

  valid_skills.clear();
  valid_items.clear();

  chosen_action_index = -1;
  chosen_skill = nullptr;
  chosen_item  = nullptr;

  remaining_targets.clear();
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
void AIModule::resetForNewTurn()
{
  chosen_action_type   = ActionType::NONE;

  valid_skills.clear();
  valid_items.clear();

  chosen_action_index = -1;
  qd_cost_paid        = 0;
  chosen_skill        = nullptr;
  chosen_item         = nullptr;

  remaining_targets.clear();
  chosen_targets.clear();
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
bool AIModule::getAIFlag(const AIState &test_flag)
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
void AIModule::setAIFlag(AIState flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
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
bool AIModule::setSkills(std::vector<Skill*> new_skills)
{
  if (new_skills.size() > 0)
  {
    valid_skills = new_skills;

    return true;
  }

  valid_skills.clear();

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool AIModule::setItems(std::vector<Item*> new_items)
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
bool AIModule::setTargets(std::vector<Person*> new_valid_targets)
{
  if (new_valid_targets.size() > 0)
  {
    remaining_targets = new_valid_targets;
    
    return true;
  }

  remaining_targets.clear();
  
  return false;
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

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AIDifficulty AIModule::getDefaultDifficulty()
{
  return kDEFAULT_DIFFICULTY;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AIPersonality AIModule::getDefaultPersonality()
{
  return kDEFAULT_PERSONALITY;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float AIModule::getRandomOffFactor()
{
  return kRANDOM_AI_OFF_FACTOR;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float AIModule::getRandomDefFactor()
{
  return kRANDOM_AI_DEF_FACTOR;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AITarget AIModule::getRandomDefaultTarget()
{
  return kRANDOM_AI_DEFAULT_TARGET;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float AIModule::getPriorityOffFactor()
{
  return kPRIORITY_AI_OFF_FACTOR;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float AIModule::getPriorityDefFactor()
{
  return kPRIORITY_AI_DEF_FACTOR;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AITarget AIModule::getPriorityDefaultTarget()
{
  return kPRIORITY_AI_DEFAULT_TARGET;
}