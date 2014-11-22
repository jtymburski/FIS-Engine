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

struct BattleSkill;
struct BattleItem;
class Person;

#include "Game/Player/Item.h"
#include "Game/Player/Skill.h"
#include "EnumFlags.h"
#include "EnumDb.h"
#include "Helpers.h"
#include "Options.h"

#include <vector>

ENUM_FLAGS(AIState)
enum class AIState
{
  ACTION_TYPE_CHOSEN    = 1 << 1,
  ACTION_INDEX_CHOSEN   = 1 << 2,
  ACTION_TARGETS_CHOSEN = 1 << 3,
  SCOPE_ASSIGNED        = 1 << 4,
  TARGETS_ASSIGNED      = 1 << 5,
  SELECTION_COMPLETE    = 1 << 6,
  ADD_TO_RECORD         = 1 << 7
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

  /* The scope of the selected action (found and injected by Battle) */
  ActionScope action_scope;

  /* Valid skills and items */
  std::vector<BattleSkill> valid_skills;
  std::vector<BattleItem>  valid_items;
  
  /* Probability distribution of skill and item possibilities */
  std::vector<std::pair<Skill*, uint32_t>> skill_probabilities;
  std::vector<std::pair<Item*, uint32_t>>  item_probabilities;

  /* Parent of the AI Module */
  Person* parent;

  /* A chosen skill or item */
  int32_t chosen_action_index;
  int32_t qd_cost_paid;

  Skill* chosen_skill;
  Item*  chosen_item;

  /* Possible friend and foe targets to choose from */
  std::vector<Person*> friend_targets;
  std::vector<Person*> foe_targets;

  /* Vector of chosen targets */
  std::vector<Person*> chosen_targets;

  /* A record of the actions this AI module has performed */
  std::vector<ActionRecord> action_record;

  /* Record information */
  uint16_t actions_elapsed_total;
  uint16_t turns_elapsed_total;
  uint16_t actions_elapsed_battle;
  uint16_t turns_elapsed_battle;
  uint16_t battles_elapsed;

  /* Computed chances for using the various types of actions */
  float skill_chance;
  float item_chance;
  float guard_chance;
  float defend_chance;
  float implode_chance;
  float run_chance;
  float pass_chance;

  /* Running configuration of the game */
  Options* running_config;

  /* ------------ Constants --------------- */
  static const AIDifficulty  kDEFAULT_DIFFICULTY;
  static const AIPersonality kDEFAULT_PERSONALITY;
  static const uint32_t kMAXIMUM_RECORD_SIZE;

  /* ------------ General AI Modifiers ------------ */
  static const float kGAI_VARIANCE;
  static const float kGAI_BASE_GUARD_FACTOR;
  static const float kGAI_BASE_IMPLODE_FACTOR;
  static const float kGAI_BASE_DEFEND_FACTOR;
  static const float kGAI_BASE_RUN_FACTOR;
  static const float kGAI_BASE_PASS_FACTOR;

  /* ------------ Random AI Modifiers ------------ */
  static const float kRAI_OFF_FACTOR;
  static const float kRAI_DEF_FACTOR;
  static const float kRAI_BASE_SKILL_FACTOR;
  static const float kRAI_BASE_ITEM_FACTOR;
  static const float kRAI_LEAN_TO_ITEM_FACTOR;
  static const AITarget kRAI_DEFAULT_TARGET;

  /* ------------ Priority AI Modifiers ------------ */
  static const float kPAI_OFF_FACTOR;
  static const float kPAI_DEF_FACTOR;
  static const float kPAI_BASE_SKILL_FACTOR;
  static const float kPAI_BASE_ITEM_FACTOR;
  static const float kPAI_LEAN_TO_ITEM_FACTOR;
  static const AITarget kPAI_DEFAULT_TARGET;

  /* ------------ Tactical AI Modifiers ------------ */
  // TODO

  /* ------------ Deep-Thought AI Modifiers ------------ */
  // TODO

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Adds a random enemy target to the chosen targets */
  Person* addRandomTarget(std::vector<Person*> available_targets);

  /* Constructs a uniform probability distributon for Skills */
  void buildUniformSkills();

  /* Constructs a uniform probability distribution for Items */
  bool buildUniformItems();

  /* Computes the chances of an AI using the various types of actions */
  void calculateActionTypeChances();

  /* Evaluates and returns whether any action may take place */
  bool canSelectAction();
 
  /* Evaluates and returns whether an AI can select Guard */
  bool canSelectGuard();

  /* Evaluates and returns whether an Item action is possible */
  bool canSelectItem();

  /* Evaluates and returns whether a Skill action is possible */
  bool canSelectSkill();

  /* Returns a float value between variance - base, variance + base */
  float calcFloatValVariance(const float &base_value);

  /* Clear invalid Skill and Item choices */
  bool clearInvalid();

  /* Determines whether a given BattleSkill has valid targets */
  bool battleSkillValid(const BattleSkill& battle_skill);

  /* Determines whether a given BattleItem has valid targets */
  bool battleItemValid(const BattleItem &batle_item);

  /* Selects an action for a random-level AI */
  bool selectRandomAction();

  /* Selects action targets for the current selected action for random AI */
  bool selectRandomTargets();

  /* Selects an action for a priority-level AI */
  bool selectPriorityAction();

  /* Selects targets for the current selected action for Priority level AI */
  bool selectPriorityTargets();

  /* Selects an action for a tactical-level AI */
  bool selectTacticalAction();

  /* Selects action targets for current action for tactical level AI */
  bool selectTacticalTargets();
  
  /* Selects an action for a deep-thought level AI */
  bool selectDeepThoughtAction();

  /* Selects action targets for current action for deep-thought level AI */
  bool selectDeepThoughtTargets();

  /* Loads the default parameters of an AI into current object */
  void loadDefaults();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds the current action selection to the record */
  bool addActionToRecord();

  /* Given the current data the module has available, computes an action */
  bool calculateAction();
  bool calculateTargets();

  /* Increment functions */
  void incrementActions();
  void incrementTurns();
  void incrementBattles();

  /* Print out the state of the AI Module */
  void print(const bool &simple = true, const bool &print_flags = false,
             const bool &print_record = false);

  /* Resets the AI for a new turn */
  void resetForNewTurn(Person* const parent = nullptr);

  /* Resets the AI for a new battle */
  void resetForNewBattle();

  /* Returns the scope of the selected action (if assigned) */
  ActionScope getActionScope();

  /* Returns the value of a given AIState flag */
  bool getFlag(const AIState &test_flag);

  /* Returns the selected action type */
  ActionType getActionType();

  /* Returns the index of selected action (along skills of items) */
  int32_t getActionIndex();

  /* Vector of chosen person for the current action */
  std::vector<Person*> getChosenTargets();

  /* Returns the current difficulty of the AI */
  AIDifficulty getDifficulty();

  /* Returns the parent */
  Person* getParent();

  /* Returns the current personality of the AI */
  AIPersonality getPrimPersonality();
  AIPersonality getSecdPersonality();

  /* Returns the # of actions this AI has done */
  uint16_t getActionsElapsed();
  uint16_t getActionsElapsedTotal();

  /* Chosen selected skills and items */
  Skill* getSelectedSkill();
  Item* getSelectedItem();

  /* Returns the # of elapsed turns */
  uint16_t getTurnsElapsed();
  uint16_t getTurnsElapsedTotal();
 
  /* Assign a value to a AIState flag */
  void setFlag(AIState flags, const bool &set_value = true);

  /* Assigns a new action scope */
  bool setActionScope(const ActionScope &new_action_scope);

  /* Assigns the vector of valid action types */
  bool setActionTypes(std::vector<ActionType> new_valid_action_types);

  /* Assigns the vector of valid skills */
  bool setSkills(std::vector<BattleSkill> new_skills);

  /* Assigns the vector of valid items */
  bool setItems(std::vector<BattleItem> new_items);

  /* Assigns the vector of valid friend targets */
  bool setFriendTargets(std::vector<Person*> new_valid_targets);

  /* Assigns the vector of valid foe targets */
  bool setFoeTargets(std::vector<Person*> new_valid_targets);

  /* Assigns a new difficulty */
  bool setDifficulty(const AIDifficulty &new_difficulty);

  /* Assigns a parent person */
  bool setParent(Person* const new_parent);

  /* Assigns the primary personality */
  bool setPrimPersonality(const AIPersonality &new_personality);

  /* Assigns the secondary personality */
  bool setSecdPersonality(const AIPersonality &new_personality);

  /* Assigns a new running config to the AI Module */
  bool setRunningConfig(Options* const new_running_config);
};

#endif //AIMODULE_H