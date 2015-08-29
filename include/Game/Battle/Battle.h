/*******************************************************************************
* Class Name: Battle [Declaration]
* Date Created: February 23rd, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* TODO
* ----
* [03-01-14]: Weather updates
* [08-24-14]: Finish battle run functionality
* [08-24-14]: Finish victory functionality
* [08-24-14]: Finish battle lost functionality
* [08-24-14]: Ailment corner cases
* [11-06-14]: Event handler finish signal (battle)?
* [11-06-14]: Update personal record run from battle count
* [11-23-14]: Documentation
*
* BUGS
* ----
* [06-07-15]: Turn N > 0 not iterating through all ze actions. Why?
* [03-29-15]: Is battleWon() being called twice upon victory?
*******************************************************************************/
#ifndef BATTLE_H
#define BATTLE_H

#include "Game/Battle/Buffer.h"
#include "Game/Battle/BattleMenu.h"
#include "Game/Battle/EventBuffer.h"
#include "Game/Player/Ailment.h"
#include "Game/Player/Party.h"
#include "Game/EventHandler.h"
#include "Options.h"

using std::begin;
using std::end;

/* CombatState enumerated flags */
ENUM_FLAGS(CombatState)
enum class CombatState
{
  PHASE_DONE = 1 << 0,
  OUTCOME_PROCESSED = 1 << 1,
  OUTCOME_PERFORMED = 1 << 2,
  READY_TO_RENDER = 1 << 3,            /* Processing state done */
  RENDERING_COMPLETE = 1 << 4,         /* Current render cycle complete */
  BEGIN_PROCESSING = 1 << 5,           /* This processing loop has begun */
  BEGIN_ACTION_PROCESSING = 1 << 6,    /* Current action processing begun */
  ACTION_PROCESSING_COMPLETE = 1 << 7, /* Curr action processing loop done */
  LAST_INDEX = 1 << 8,
  ALL_PROCESSING_COMPLETE = 1 << 9,   /* Entire processing complete */
  BEGIN_PERSON_UPKEEPS = 1 << 10,     /* The upkeep phase has started */
  PERSON_UPKEEP_COMPLETE = 1 << 11,   /* One person state complete */
  BEGIN_AILMENT_UPKEEPS = 1 << 12,    /* Curr. person has begun ailment up */
  CURRENT_AILMENT_STARTED = 1 << 13,  /* Curr. ailment processing begun */
  CURRENT_AILMENT_COMPLETE = 1 << 14, /* Curr person ailment check stage */
  COMPLETE_AILMENT_UPKEEPS = 1 << 15, /* Curr person ailment check done  */
  ALL_UPKEEPS_COMPLETE = 1 << 16,      /* Upkeep checking stage complete */
  CURR_TARG_DEAD = 1 << 17
};

ENUM_FLAGS(IgnoreState)
enum class IgnoreState
{
  IGNORE_PHYS_ATK = 1 << 0,
  IGNORE_PHYS_DEF = 1 << 1,
  IGNORE_PRIM_ATK = 1 << 2,
  IGNORE_PRIM_DEF = 1 << 3,
  IGNORE_SECD_ATK = 1 << 4,
  IGNORE_SECD_DEF = 1 << 5,
  IGNORE_LUCK_ATK = 1 << 6,
  IGNORE_LUCK_DEF = 1 << 7,
  IGNORE_EQUIPMENT = 1 << 8,
};

/* Description: Enumerated values for the Battle OutStatecomeState
 *
 * VICTORY - The allies have defeated all remaining foes
 * DEFEAT  - The foes have defeated all allies, GAME OVER
 * ALLIES_RUN - The allies have escaped the Battle
 * ENEMIES_RUN - The enemies have escaped the Battle 'pyrric victory'
 * RETURN  - The battle ends without any victory/loss -> return to Map state
 */
enum class OutcomeType
{
  VICTORY,
  DEFEAT,
  ALLIES_RUN,
  ENEMIES_RUN,
  RETURN,
  NONE
};

/* Enumerated values for turn mode */
enum class TurnMode
{
  FRIENDS_FIRST,
  ENEMIES_FIRST,
};

/* Enumerated values for turn state */
enum class TurnState
{
  BEGIN,               /* Setup of the battle */
  GENERAL_UPKEEP,      /* General upkeep phase - weather etc. */
  UPKEEP,              /* Personal upkeep - ailments etc. */
  SELECT_ACTION_ALLY,  /* User choice of action/skill etc. */
  SELECT_ACTION_ENEMY, /* Enemy choice of skill -> AI */
  ORDER_ACTIONS,       /* Determines order of skills */
  PROCESS_ACTIONS,     /* Determines outcomes of skills */
  CLEAN_UP,            /* Cleanup after turn, turn incr. etc. */
  LOSS,                /* LosStates stage returns to title */
  VICTORY,             /* Victory displays the victory screen */
  RUNNING,             /* Running condition */
  DESTRUCT             /* Battle should be destructed */
};

class Battle
{
public:
  /* Constructs a party given two parties and configured options */
  Battle(Party *const friends, Party *const foes,
         SkillSet *const bubbified_skills, EventHandler *event_handler);

  /* Annihilates a Battle object */
  ~Battle();

private:
  /* The Item Buffer of the Battle */
  Buffer *action_buffer;

  /* The normal vector of ailments */
  std::vector<Ailment *> ailments;

  /* Temporary vector of ailments for update processing */
  std::vector<Ailment *> temp_ailments;

  /* The bubbified skill set */
  SkillSet *bubbified_skills;

  /* The current AI Module */
  AIModule *curr_module;

  /* The buffer of events for the battle */
  EventBuffer *event_buffer;

  /* The EventHandler */
  EventHandler *event_handler;

  /* The Battle Menu Bar */
  BattleMenu *menu;

  /* Enumerated battle options for ailment updates */
  BattleOptions ailment_update_mode;

  /* CombatState flag set */
  CombatState flags;
  IgnoreState ignore_flags;

  /* Pointers to the battling parties */
  Party *friends;
  Party *foes;

  /* Enumerated outcome value for type of outcome */
  OutcomeType outcome;

  /* Current index for action selection/outcomes */
  int32_t person_index;

  /* The index of the currently selected target */
  uint32_t target_index;

  /* Elapsed time of the Battle */
  uint32_t time_elapsed;

  /* Elapsed time since last turn */
  uint32_t time_elapsed_this_turn;

  /* Elapsed turns of hte battle */
  uint16_t turns_elapsed;

  /* Enumeration of turn order */
  TurnMode turn_mode;

  /* The turn state of the Battle */
  TurnState turn_state;

  /* Action Processing Variables */
  Attribute prim_off;
  Attribute prim_def;
  Attribute secd_off;
  Attribute secd_def;
  Attribute user_attr;
  Attribute targ_attr;

  AttributeSet temp_user_stats;
  AttributeSet temp_user_max_stats;
  std::vector<AttributeSet> temp_target_stats;
  std::vector<AttributeSet> temp_target_max_stats;

  Person *curr_user;
  Person *curr_target;
  Action *curr_action;
  uint32_t curr_action_index;
  Skill *curr_skill;
  Item *curr_item;

  /* Current pocessing target index */
  uint32_t pro_index;

  /* Vector of persons needing upkeep */
  std::vector<Person *> upkeep_persons;

  /* ------------ Battle Modifiers (See Implementation) --------------- */
  static const size_t kMAX_AILMENTS;
  static const size_t kMAX_EACH_AILMENTS;
  static const uint16_t kMAXIMUM_DAMAGE;
  static const uint16_t kMINIMUM_DAMAGE;

  static const float kOFF_PHYS_MODIFIER;
  static const float kDEF_PHYS_MODIFIER;
  static const float kOFF_PRIM_ELM_MATCH_MODIFIER;
  static const float kDEF_PRIM_ELM_MATCH_MODIFIER;
  static const float kOFF_SECD_ELM_MATCH_MODIFIER;
  static const float kDEF_SECD_ELM_MATCH_MODIFIER;

  static const float kOFF_PRIM_ELM_MODIFIER;
  static const float kDEF_PRIM_ELM_MODIFIER;
  static const float kOFF_SECD_ELM_MODIFIER;
  static const float kDEF_SECD_ELM_MODIFIER;

  static const float kPRIM_ELM_ADV_MODIFIER;
  static const float kPRIM_ELM_DIS_MODIFIER;
  static const float kSECD_ELM_ADV_MODIFIER;
  static const float kSECD_ELM_DIS_MODIFIER;

  static const float kDOUBLE_ELM_ADV_MODIFIER;
  static const float kDOUBLE_ELM_DIS_MODIFIER;

  static const float kMANNA_POW_MODIFIER;
  static const float kMANNA_DEF_MODIFIER;
  static const float kUSER_POW_MODIFIER;
  static const float kTARG_DEF_MODIFIER;

  static const float kOFF_CRIT_FACTOR;
  static const float kBASE_CRIT_CHANCE;
  static const float kCRIT_MODIFIER;
  static const float kCRIT_LVL_MODIFIER;
  static const float kCRIT_DEFENDING_MODIFIER;
  static const float kCRIT_GUARDED_MODIFIER;

  static const float kDODGE_MODIFIER;
  static const float kDODGE_HIGHEST_RATE_PC;
  static const float kDODGE_PER_LEVEL_MODIFIER;

  static const float kBASE_RUN_CHANCE;
  static const float kUSER_RUN_MODIFIER;
  static const float kALLY_RUN_MODIFIER;
  static const float kENEMY_RUN_MODIFIER;
  static const float kRUN_PC_PER_POINT;

  static const float kDEFEND_MODIFIER;
  static const float kGUARD_MODIFIER;
  static const float kSHIELDED_MODIFIER;

  static const int16_t kREGEN_RATE_ZERO_PC;
  static const int16_t kREGEN_RATE_WEAK_PC;
  static const int16_t kREGEN_RATE_NORMAL_PC;
  static const int16_t kREGEN_RATE_STRONG_PC;
  static const int16_t kREGEN_RATE_GRAND_PC;

  /* -------- Battle Outcome Constants (See Implementation) ----------- */
  static const int16_t kALLY_KO_EXP_PC;
  static const int16_t kENEMY_RUN_EXP_PC;
  static const int16_t kRUN_PC_EXP_PENALTY;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Attempts to add an ailment to the vector of ailments */
  bool addAilment(Infliction infliction_type, Person *inflictor,
                  uint16_t min_turns, uint16_t max_turns, int32_t chance);

  /* Determines if there is any member of a party needing selection */
  bool anyUserSelection(bool friends = true);

  /* Returns the most recently created ailmnent */
  Ailment *getLastAilment();

  /* Set the next action index, true if valid */
  bool nextActionIndex();

  /* Attempts to remove an ailment from the vector */
  bool removeAilment(Ailment *remove_ailment);

  /* Called when the Battle has been lost */
  void battleLost();

  /* Called when the Battle is being run from */
  void battleRun();

  /* Called when the Battle has been won */
  void battleWon();

  /* Creates a vector of action variables */
  void buildActionVariables(ActionType action_type,
                            std::vector<Person*> targets);

  /* Attempt to add the current module settings to the action buffer */
  bool bufferEnemyAction();

  /* Attempt to add the current menu settings to the action buffer */
  bool bufferUserAction();

  /* Builds the vector of structs for skills and assosciated targets */
  std::vector<BattleSkill> buildBattleSkills(const int32_t &index,
                                             SkillSet *skill_set);

  std::vector<BattleItem>
  buildBattleItems(const int32_t &index,
                   std::vector<std::pair<Item *, uint16_t>> items);

  /* Calculates the base damage for the current action/target setup */
  int32_t calcBaseDamage(const float &crit_factor);

  /* Calculates the modifiers to be used for curr skill elemental adv setup */
  void calcElementalMods();

  /* Calculates the amount of experience a person will gain for win of Battle */
  int32_t calcExperience(Person *ally);

  /* Calculates the Crit Factor to be applied to the damage */
  float calcCritFactor();

  /* Calculate the current ignore state flags */
  bool calcIgnoreState();

  /* Calculates the amount of damage which implode will do */
  int32_t calcImplodeDamage();

  /* Calculates the level difference between curr_user and curr_target */
  int16_t calcLevelDifference(std::vector<Person *> targets);

  /* Calculates the total regen value of a particular stat for a given person */
  int32_t calcTurnRegen(Person *const target, const Attribute &attr);

  /* Determines whether the current person has selected all actions */
  bool canIncrementIndex(Person *check_person);

  /* Can the current target be inflicted with a new infliction of given type? */
  bool canInflict(Infliction test_infliction);

  /* Tests whether the given infliction type can be removed from target */
  bool canRelieve(Infliction test_infliction);

  /* Determines whether a person has an infliction already */
  bool hasInfliction(Infliction type, Person *check);

  /* Asserts all AI modules are set for the enemy parties */
  bool checkAIModules();

  /* Cleans up the action variables (sets to default upon instantiation too) */
  void clearActionVariables();

  /* Returns enumeration of party death [None, Friends, Foes, Both] */
  bool checkPartyDeath(Party *const check_party, Person *target);

  /* Cleanup before the end of a Battle turn */
  void cleanUp();

  /* Determines the turn progression of the Battle (based on speed) */
  void determineTurnMode();

  /* Calculates crit chances and determines whether a critical will occur */
  bool doesActionCrit();

  /* Calculates miss chance and determines whether an action miss will occur */
  bool doesActionHit();

  /* Determines whether the current person trying to run runs */
  bool doesCurrPersonRun();

  /* Calculates skill missing chance and determines if the skill miss */
  bool doesSkillHit(std::vector<Person *> targets);

  /* Deals with general upkeep (i.e. weather) */
  void generalUpkeep();

  /* Sets the flags of BattleState at the beginning of the Battle */
  void loadBattleStateFlags();

  /* Orders the actions on the buffer by speed of the aggressor */
  void orderActions();

  /* Perform the Battle events */
  void performEvents();

  /* Sub-function to perform damage events */
  void performDamageEvent(BattleEvent *event);

  /* Deals with character related upkeep */
  void personalUpkeep(Person *const target);

  /* Process the events of the Battle */
  void processBuffer();

  /* Determines the Regen % for a given enumerated regeneration rate */
  int16_t getRegenFactor(const RegenRate &regen_rate);

  /* General processing action function */
  bool processAction(BattleEvent *action_event);

  /* Processing function for the current ailment */
  bool processAilment();

  /* Processes the updating portion of the ailment from processAilment() */
  bool processAilmentUpdate(Ailment *ail);

  /* Processes an alteration action */
  bool processAlterAction(BattleEvent *alter_event, Person *action_target,
                          Person *factor_target);

  /* Processes an assigning action */
  bool processAssignAction(BattleEvent *assign_event, Person *action_target,
                           Person *factor_target);

  /* Processes a damaging action */
  bool processDamageAction(BattleEvent *damage_event);

  /* Method for outsourcing an amount of dmg and type of damage to curr targ */
  bool processDamageAmount(int32_t amount);

  /* Processes the death of a recent person checking for party death */
  bool processPersonDeath(bool ally_target);

  /* Processes a relieving action */
  bool processRelieveAction();

  /* Processes a reviving action */
  bool processReviveAction(BattleEvent *revive_event);

  /* Processes an inflicting action */
  bool processInflictAction();

  /* Processes an individual item from a user against targets */
  void processItem(std::vector<Person *> targets);

  /* Processes an individual action from a user against targets */
  void processSkill(std::vector<Person *> targets);

  /* Processes an individual action from a user imploding on a target */
  void processImplode(std::vector<Person*> targets);

  /* Checks to see whether a guard action can occur between user and target */
  bool processGuard();

  /* Actually performs a guard action between cur user and target */
  bool performGuard(BattleEvent *guard_event);

  /* Recalculates the ailments after they have been altered */
  void reCalcAilments(Person *const target);

  /* Recalculates the flags for a target person (based on ailments) */
  bool reCalcAilmentFlags(Person *target, Ailment *ail);

  /* Reset miss/no effect flags for the given user */
  void resetTurnFlags(Person *user);

  /* Calculates enemy actions and add them to the buffer */
  void selectEnemyActions();

  /* Calculates user actions and add them to the buffer */
  void selectUserActions();

  /* Load default configuration of the battle */
  bool setupClass();

  /* Determines whether a potential person index can select an action */
  bool testPersonIndex(const int32_t &test_index);

  /* Method which calls personal upkeeps */
  void upkeep();

  /* Sub-function of update() for ally selections */
  void updateAllySelection();

  /* Sub-function of update() for enemy selections */
  void updateEnemySelection();

  /* Updates the LOSS/VICTORY flags based on party deaths */
  bool updatePartyDeaths(Person *target);

  /* Updates the current targets defense state */
  bool updateTargetDefense();

  /* Assigns a new value to the ailment update mode */
  void setAilmentUpdateMode(const BattleOptions &new_value);

  /* Assigns the friends party of the Battle */
  bool setFriends(Party *const new_friends);

  /* Assigns the foes party of the Battle */
  bool setFoes(Party *const new_foes);

  /* Updates the person selection index to the next valid one */
  bool setNextPersonIndex();

  /* Updates the Battle to the next state */
  void setNextTurnState();

  /* Assigns a new value to the Outcome enumeration */
  bool setOutcome(OutcomeType new_outcome);

  /* Assigns a new value to the elapsed time */
  void setTimeElapsed(const int32_t &new_value);

  /* Assigns thee time elapsed this turn */
  void setTimeElapsedThisTurn(const int32_t &new_value);

  /* Assigns a new value to the turns elapsed */
  void setTurnsElapsed(const uint16_t &new_value);

  /* Assigns a new turn mode to the Battle */
  void setTurnMode(const TurnMode &new_turn_mode);

  /* Updates the turn state of the Battle */
  void setTurnState(const TurnState &new_turn_state);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Evaluates whether a given Person ptr is a member of the allies party */
  bool isAlly(Person *check_person);

  /* Evaluates whether a given Person ptr is a member of the foes party */
  bool isFoe(Person *check_person);

  /* Processing key down events for the Battle */
  bool keyDownEvent(SDL_KeyboardEvent event);

  /* Methods to print information about the Battle */
  void printAll(const bool &flags, const bool &party);
  void printPartyState();
  void printPersonState(Person *const member, const int32_t &person_index);
  void printProcessingState(bool basic = false);
  void printInventory(Party *const target_party);
  void printTurnState();

  /* Update the cycle time of Battle */
  bool update(int32_t cycle_time);

  /* Unsets the attacking flag for all allies & foes members */
  void unsetActorsAttacking();
  void unsetActorsSelecting();

  Skill *getCurrSkill();

  /* Returns the ailment update mode currently set */
  BattleOptions getAilmentUpdateMode();

  /* Returns a pointer to the BattleMenu */
  BattleMenu *getBattleMenu();

  /* Return the value of a given CombatState flag */
  bool getBattleFlag(const CombatState &test_flag);

  /* Return the value of a given IgnoreState flag */
  bool getIgnoreFlag(const IgnoreState &test_flag);

  /* Return the pointer to the event buffer */
  EventBuffer *getEventBuffer();

  /* Returns the friends pointer of the Battle */
  Party *getFriends();

  /* Returns the foes pointer of the Battle */
  Party *getFoes();

  /* Obtains the outcome state enumeration */
  OutcomeType getOutcome();

  /* Evaluates and returns a vector of ailments for a given person */
  std::vector<Ailment *> getPersonAilments(const Person *const target);

  /* Returns the value of the turns elapsed */
  uint32_t getTurnsElapsed();

  /* Returns the elapsed time of the Battle */
  uint32_t getTimeElapsed();

  /* Returns the enumerated turn state of the Battle */
  TurnState getTurnState();

  /* Returns the index integer of a given Person ptr */
  int32_t getTarget(Person *battle_member);

  /* Obtains the corresponding Person ptr for a given battle member index */
  Person *getPerson(const int32_t &index);

  /* Calculate and return all BattleMember indexes */
  std::vector<int32_t> getAllTargets();

  /* Obtains the list of friends target indexes */
  std::vector<int32_t> getFriendsTargets(const bool &ko = false);

  /* Obtains the list of foes target indexes */
  std::vector<int32_t> getFoesTargets(const bool &ko = false);

  /* Obtains a vector of targets matching the signage */
  std::vector<int32_t> getPartyTargets(int32_t check_index);

  /* Build a vector of person pointers from a vector of person indexes */
  std::vector<Person *> getPersonsFromIndexes(std::vector<int32_t> indexes);

  /* Build a vector of person indexes from a vector of person pointers */
  std::vector<int32_t> getIndexesOfPersons(std::vector<Person *> persons);

  /* Get the index of a person from its pointer */
  int32_t getIndexOfPerson(Person *check_person);

  /* Obtains a vector of battle member indexes for a given user and scope */
  std::vector<int32_t> getValidTargets(int32_t index, ActionScope action_scope);

  /* Assign a value to a CombatState flag */
  void setBattleFlag(CombatState flags, const bool &set_value = true);

  /* Assign a value to an IgnoreState flag */
  void setIgnoreFlag(IgnoreState flags, const bool &set_value = true);

  void setUserAttacking(Person *user);

  /*=============================================================================
   * PUBLIC STATIC FUNCTIONS
   *============================================================================*/
public:
  /* Public static gets for menu constant values */
  static uint32_t getGenUpkeepDelay();
  static uint32_t getBattleMenuDelay();

  /* Public static gets for battle modifier values */
  static uint32_t getMaxAilments();
  static uint32_t getMaxEachAilments();
  static uint32_t getMaxDamage();
  static uint32_t getMinDamage();
  static float getOffPrimElmMod();
  static float getDefPrimElmMod();
  static float getOffSecdElmMod();
  static float getDefSecdElmMod();
  static float getDodgeMod();
  static float getDogePerLvlMod();
  static float getPrimElmAdvMod();
  static float getPrimElmDisMod();
  static float getSecdElmAdvMod();
  static float getSecdElmDisMod();
  static float getDoubleElmAdvMod();
  static float getDoubleElmDisMod();
};

#endif // BATTLE_H
