/*******************************************************************************
* Class Name: AI Module [Declaration]
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
* TODO
* ----
*******************************************************************************/

#ifndef AIMODULE_H
#define AIMODULE_H

class Person;

#include "Game/Player/Item.h"
#include "Game/Player/Skill.h"
#include "EnumFlags.h"
#include "EnumDb.h"
#include "Options.h"

#include <vector>

ENUM_FLAGS(AIState)
enum class AIState
{
  ACTION_TYPE_CHOSEN    = 1 << 0,
  ACTION_INDEX_CHOSEN   = 1 << 1,
  ACTION_TARGETS_CHOSEN = 1 << 2,
  SELECTION_COMPLETE    = 1 << 3,
  ADD_TO_RECORD         = 1 << 4
};

/*
 * Description: 
 *
 */
enum class AIDifficulty
{
  RANDOM,       /* Randomly chooses action types, indexes, targets */
  PRIORITY,     /* Chooses actions based on arbitrary priority values */
  TACTICAL,     /* Tactically chooses combinations of actions */
  DEEP_THOUGHT  /* Computes possible actions to determine the best outcome */
};

/*
 * Description: 
 *
 */
enum class AIPersonality
{
  MODERATOR,   /* Generic personality type */
  AGGRESSOR,   /* Focuses on actions damage the opposing team */
  DEFENDER,    /* Focuses actions on increasing one-self */
  PROTECTOR,   /* Focuses on actions protecting teammates (including self) */
  RETALIATOR,  /* Changes priority targeting if hit by a certain enemy */
  MASOCHISTIC, /* Performs self-destructive behaviour */
  ANNIHILATOR, /* Attempts to perform the most damage possible, at any cost */
  RUNNER,      /* Focuses on running or becoming able to run */
  PASSER,      /* Focuses on doing nothing */
  NONE         /* No personality type - error */
};

/*
 * Description: Enumerated types of selection for AI
 *
 * RANDOM - randomly selects targets until filled for the skill
 * WEAKEST_HP_FIRST - targets the weakest enemy if atking/ally if healing
 * STRONGEST_FIRST  - targets the most powerful target first
 */
enum class AITarget
{
  RANDOM           = 1 << 0,
  LOWEST_HP_FIRST  = 1 << 1,
  STRONGEST_FIRST  = 1 << 2
};

/*
 * Description: A structure detailing an action and related information in the
 *              performing of that action for the AI to decide future actions.
 *
 * action_type  - the type of action performed
 * qd_cost_paid - the actual cost (in QD) paid for a Skill
 * targets      - a vector of all person ptrs who were targets of the action
 * skill_used   - ptr to the skill used (if a Skill was used)
 * item_used    - ptr to the item used  (if an Item was used)
 */
struct ActionRecord
{
  ActionType action_type;

  int16_t qd_cost_paid;

  std::vector<Person*> targets;

  Skill* skill_used;
  Item* item_used;
};

class AIModule
{
public:
  /* Default AI Module constructor */
  AIModule();

  /* Simple AI constructor - constructs with diff and one personality */
  AIModule (const AIDifficulty &diff, const AIPersonality &prim_personality);

  /* General AI constructor - constucts with diff and two personalities */
  AIModule(const AIDifficulty &diff,   const AIPersonality &prim_personality,
           const AIPersonality &secd_personality);

private:
  /* The difficulty and personality of the Enemy */
  AIDifficulty  difficulty;
  AIPersonality prim_personality;
  AIPersonality secd_personality;

  /* Flag set describing the state of the AI */
  AIState flags;

  /* Valid action types an the chosen action type */
  std::vector<ActionType> valid_action_types;
  ActionType chosen_action_type;

  /* Valid skills and items */
  std::vector<Skill*> valid_skills;
  std::vector<Item*>  valid_items;

  /* A chosen skill or item */
  int32_t chosen_action_index;
  int32_t qd_cost_paid;

  Skill* chosen_skill;
  Item* chosen_item;

  /* Remaining and chosen persons */
  std::vector<Person*> remaining_targets;
  std::vector<Person*> chosen_targets;

  /* A record of the actions this AI module has performed */
  std::vector<ActionRecord> action_record;

  /* Record information */
  uint16_t actions_elapsed_total;
  uint16_t turns_elapsed_total;
  uint16_t actions_elapsed_battle;
  uint16_t turns_elapsed_battle;
  uint16_t battles_elapsed;

  /* Running configuration of the game */
  Options* running_config;

  /* ------------ Constants --------------- */
  static const AIDifficulty  kDEFAULT_DIFFICULTY;
  static const AIPersonality kDEFAULT_PERSONALITY;
  static const uint32_t kMAXIMUM_RECORD_SIZE;

  /* ------------ Random AI Modifiers ------------ */
  static const float kRANDOM_AI_OFF_FACTOR;
  static const float kRANDOM_AI_DEF_FACTOR;
  static const AITarget kRANDOM_AI_DEFAULT_TARGET;

  /* ------------ Priority AI Modifiers ------------ */
  static const float kPRIORITY_AI_OFF_FACTOR;
  static const float kPRIORITY_AI_DEF_FACTOR;
  static const AITarget kPRIORITY_AI_DEFAULT_TARGET;

  /* ------------ Tactical AI Modifiers ------------ */
  // TODO

  /* ------------ Deep-Thought AI Modifiers ------------ */
  // TODO

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Loads the default parameters of an AI into current object */
  void loadDefaults();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds the current action selection to the record */
  bool addActionToRecord();

  /* Increment functions */
  void incrementActions();
  void incrementTurns();
  void incrementBattles();

  /* Resets the AI for a new turn */
  void resetForNewTurn();

  /* Resets the AI for a new battle */
  void resetForNewBattle();

  /* Returns the value of a given AIState flag */
  bool getAIFlag(const AIState &test_flag);

  /* Returns the selected action type */
  ActionType getActionType();

  /* Returns the index of selected action (along skills of items) */
  int32_t getActionIndex();

  /* Vector of chosen person for the current action */
  std::vector<Person*> getChosenTargets();

  /* Returns the current difficulty of the AI */
  AIDifficulty getDifficulty();

  /* Returns the current personality of the AI */
  AIPersonality getPrimPersonality();
  AIPersonality getSecdPersonality();

  /* Returns the # of actions this AI has done */
  uint16_t getActionsElapsed();
  uint16_t getActionsElapsedTotal();

  /* Returns the # of elapsed turns */
  uint16_t getTurnsElapsed();
  uint16_t getTurnsElapsedTotal();
 
  /* Assign a value to a AIState flag */
  void setAIFlag(AIState flags, const bool &set_value = true);

  /* Assigns the vector of valid action types */
  bool setActionTypes(std::vector<ActionType> new_valid_action_types);

  /* Assigns the vector of valid skills */
  bool setSkills(std::vector<Skill*> new_skills);

  /* Assigns the vector of valid items */
  bool setItems(std::vector<Item*> new_items);

  /* Assigns the vector of valid targets */
  bool setTargets(std::vector<Person*> new_valid_targets);

  /* Assigns a new difficulty */
  bool setDifficulty(const AIDifficulty &new_difficulty);

  /* Assigns the primary personality */
  bool setPrimPersonality(const AIPersonality &new_personality);

  /* Assigns the secondary personality */
  bool setSecdPersonality(const AIPersonality &new_personality);

  /* Assigns a new running config to the AI Module */
  bool setRunningConfig(Options* const new_running_config);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Public static gets for AI difficulty and personality */
  static AIDifficulty getDefaultDifficulty();
  static AIPersonality getDefaultPersonality();

  /* Public static gets for Random AI modifier values */  
  static float getRandomOffFactor();
  static float getRandomDefFactor();
  static AITarget getRandomDefaultTarget();

  /* Public static gets for Priority AI modifier values */
  static float getPriorityOffFactor();
  static float getPriorityDefFactor();
  static AITarget getPriorityDefaultTarget();
};

#endif //AIMODULE_H
