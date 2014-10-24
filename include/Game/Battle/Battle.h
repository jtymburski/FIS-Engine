/*******************************************************************************
* Class Name: Battle [Declaration]
* Date Created: February 23rd, 2014
* Inheritance: None
* Description: 
*
* Notes
* -----
*
* [1]: generalUpkeep() adjusts all values based on Weather.  BattleInfoBar
* displays info on these adjustments * after X seconds, personalUpkeep() is
* called.
*
* personalUpkeep() adjusts all values of the Person represented by
* party_index. PersonalUpkeep() then, after X seconds, activates and
* initializes BattleMenu with party_index's relevant options.
*
* 3. Once an option from BattleMenu is selected processAction() is
* called and party_index is incremented, then if it is less
* than 5, personalUpkeep is called (Step 2).  Else, BattleMenu
* deactivates and processEnemyActions() is called (Step 4).
*
* [NOW WRONG]
* 4. processEnemyActions() uses the enemies scripts to add actions to the
* stack, after which orderActions() is called.
*
* [NOW WRONG]
* 5. orderActions() reorders the stack based on speed and status ailments,
* performActions() is then called.
*
* 6. performAction() takes the first Action on the stack and loads it into
* action_animate which then shows the Actions animation.  The Action is then
* removed from the stack.
*
* 7. action_animate emits a finished(Action *) signal which is connected to
* changeStats(Action* battle_action), changeStats(Action* battle_action)
* alters each Person's stats based off its parameter Action.  UpdateScene()
* is then called.
*
* 8. updateScene() emits a finished() signal which is connected to
* actionOutcome(). ActionOutcome() checks for deaths and animates
* appropriately, if a whole party is eliminated, then all the memory
* is cleared, and battleWon() or battleLost() is called.  If the stack is not
* empty, (Step 6) else reset party_index and (Step 1).
*
* Note: Animation has changed since this design and requires some alteration
*     in terms of these steps.
*
* In Battle Action Animations
* ----------------------------
* Action Timer is removed
* For actions that occur during battle, the animations will only be targets
* of the player and centered on them. For example, there will be a sequence
* of sprites where it will animate (based on a timer) and flash on the target
* and possibly flash on the caster itself. These will be attached to the action
* class and will occur during the generation of the action in the step 6? This
* could be tied into the action paint event to draw on the battle screen These
* would utilize the sprite class which already has a linked list to generate
* the sequences of the animation. How would this be programmed in? Attached
* to the database since we generated a sequence within action to program in
* actions and commands that can be used in the battle sequence. An addition
* to that sequence to include frames or generate generic ones that everyone
* has to share.
*
* TODO
* ----
*******************************************************************************/
#ifndef BATTLE_H
#define BATTLE_H

#include "Frame.h"
#include "Options.h"
#include "Game/Battle/Buffer.h"
#include "Game/Battle/InfoBar.h"
#include "Game/Battle/BattleMenu.h"
#include "Game/Player/Ailment.h"
#include "Game/Player/Party.h"

using std::begin;
using std::end;

/* CombatState enumerated flags */
ENUM_FLAGS(CombatState)
enum class CombatState
{
  CONFIGURED       = 1 << 0,
  FLAGS_CONFIGURED = 1 << 1,
  ACTION_DONE      = 1 << 2,
  PHASE_DONE       = 1 << 3,
  VICTORY          = 1 << 4,
  LOSS             = 1 << 5,
  OUTCOME_DONE     = 1 << 6,
  ERROR_STATE      = 1 << 7,
  RANDOM_ENCOUNTER = 1 << 8,
  MINI_BOSS        = 1 << 9,
  BOSS             = 1 << 10,
  FINAL_BOSS       = 1 << 11
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
  IGNORE_LUCK_DEF = 1 << 7
};

/* Enumerated values for turn mode */
enum class TurnMode
{
  FRIENDS_FIRST,
  ENEMIES_FIRST
};

/* Enumerated values for turn state */
enum class TurnState
{
  BEGIN,                /* Setup of the battle */
  GENERAL_UPKEEP,       /* General upkeep phase - weather etc. */
  UPKEEP,               /* Personal upkeep - ailments etc. */
  SELECT_ACTION_ALLY,   /* User choice of action/skill etc. */
  SELECT_ACTION_ENEMY,  /* Enemy choice of skill -> AI */
  ORDER_ACTIONS,        /* Determines order of skills */
  PROCESS_ACTIONS,      /* Determines outcomes of skills */
  CLEAN_UP,             /* Cleanup after turn, turn incr. etc. */
  LOSS,                 /* Loss stage returns to title */
  VICTORY,              /* Victory displays the victory screen */
  RUNNING,              /* Running condition */
  DESTRUCT              /* Battle should be destructed */
};

class Battle
{
public:
  /* Constructs a party given two parties and configured options */
  Battle(Options* const running_config, Party* const friends, 
         Party* const foes);

  /* Annihilates a Battle object */
  ~Battle();

private:
  /* Pairs of unique-ID vs. Ailment pointers for ailments */
  std::vector<std::pair<uint32_t, Ailment*>> ailments;

  /* The current AI Module */
  AIModule* curr_module;

  /* The Battle Info Bar */
  //InfoBar* info_bar;

  /* The Battle Menu Bar */
  BattleMenu* menu;

  /* The Battle Status Bar */
  //BattleStatusBar* status_bar;

  /* Vector of enemy status bars */
  //std::vector<EnemyStatusBar*> enemy_bars;

  /* The Item Buffer of the Battle */
  Buffer* action_buffer;

  /* The Background of the Battle */
  Frame* bg;

  /* The background mage of the Battle Status Bar */
  Frame* status_bar_bg;

  /* Enumerated battle options for ailment updates */
  BattleOptions ailment_update_mode;

  /* Enumerated battle options for hud display mode */
  BattleOptions hud_display_mode;

  /* Enumerated battle mode for Battle output type */
  BattleMode battle_mode;

  /* CombatState flag set */
  CombatState flags;
  IgnoreState ignore_flags;

  /* Pointers to the battling parties */
  Party* friends;
  Party* foes;

  /* Running config */
  Options* config;

  /* Current index for action selection/outcomes */
  int32_t person_index;
  
  /* Dimensions of the screen */
  uint8_t screen_height;
  uint8_t screen_width;

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

  // Currently active weather condition
  // Weather* weather_condition

  /* Action Processing Variables */
  Attribute prim_off;
  Attribute prim_def;
  Attribute secd_off;
  Attribute secd_def;

  AttributeSet temp_user_stats;
  AttributeSet temp_user_max_stats;
  std::vector<AttributeSet> temp_target_stats;
  std::vector<AttributeSet> temp_target_max_stats;

  Person* curr_user;
  Person* curr_target;
  Action* curr_action;
  Skill*  curr_skill;
  Item*   curr_item;

  /* Current pocessing target index */
  uint32_t p_target_index;

  /* ------------ Menu Constants --------------- */
  static const uint16_t kGENERAL_UPKEEP_DELAY;
  static const uint16_t kBATTLE_MENU_DELAY;

  /* ------------ Battle Modifiers (See Implementation) --------------- */
  static const uint16_t kMAX_AILMENTS;
  static const uint16_t kMAX_EACH_AILMENTS;
  static const uint16_t kMAXIMUM_DAMAGE;
  static const uint16_t kMINIMUM_DAMAGE;

  static const float    kOFF_PHYS_MODIFIER;
  static const float    kDEF_PHYS_MODIFIER;
  static const float    kOFF_PRIM_ELM_MATCH_MODIFIER;
  static const float    kDEF_PRIM_ELM_MATCH_MODIFIER;
  static const float    kOFF_SECD_ELM_MATCH_MODIFIER;
  static const float    kDEF_SECD_ELM_MATCH_MODIFIER;

  static const float    kOFF_PRIM_ELM_MODIFIER;
  static const float    kDEF_PRIM_ELM_MODIFIER;
  static const float    kOFF_SECD_ELM_MODIFIER;
  static const float    kDEF_SECD_ELM_MODIFIER;

  static const float    kPRIM_ELM_ADV_MODIFIER;
  static const float    kPRIM_ELM_DIS_MODIFIER;
  static const float    kSECD_ELM_ADV_MODIFIER;
  static const float    kSECD_ELM_DIS_MODIFIER;

  static const float    kDOUBLE_ELM_ADV_MODIFIER;
  static const float    kDOUBLE_ELM_DIS_MODIFIER;

  static const float    kMANNA_POW_MODIFIER;
  static const float    kMANNA_DEF_MODIFIER;
  static const float    kUSER_POW_MODIFIER;
  static const float    kTARG_DEF_MODIFIER;

  static const float    kOFF_CRIT_FACTOR;
  static const float    kBASE_CRIT_CHANCE;
  static const float    kCRIT_MODIFIER;
  static const float    kCRIT_LVL_MODIFIER;
  static const float    kCRIT_DEFENDING_MODIFIER;
  static const float    kCRIT_GUARDED_MODIFIER;
  static const float    kCRIT_SHIELDED_MODIFIER;

  static const float    kDODGE_MODIFIER;
  static const float    kDODGE_PER_LEVEL_MODIFIER;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Attempts to add an ailment to the vector of ailments */
  bool addAilment(Ailment* const new_ailment);

  /* Called when the Battle has been won */
  void battleWon();

  /* Called when the Battle has been lost */
  void battleLost();

  /* Attempt to add the current module settings to the action buffer */
  bool bufferEnemyAction();

  /* Attempt to add the current menu settings to the action buffer */
  bool bufferUserAction();
  
  /* Builds the vector of structs for skills and assosciated targets */
  std::vector<BattleSkill> buildBattleSkills(const int32_t &index, 
      SkillSet* skill_set);

  std::vector<BattleItem> buildBattleItems(const int32_t &index, 
      std::vector<std::pair<Item*, uint16_t>> items);

  /* Calculates the base damage for the current action/target setup */
  int32_t calcBaseDamage(const float &crit_factor);

  /* Calculates the modifiers to be used for curr skill elemental adv setup */
  void calcElementalMods();

  /* Calculates the Crit Factor to be applied to the damage */
  float calcCritFactor();
  
  /* Calculate the current ignore state flags */
  bool calcIgnoreState();

  /* */
  int16_t calcLevelDifference();

  /* Determines whether the current person has selected all actions */
  bool canIncrementIndex(Person* check_person);

  /* Asserts all AI modules are set for the enemy parties */
  bool checkAIModules();

  /* Cleans up the action variables (sets to default upon instantiation too) */
  void clearActionVariables();

  /* Returns enumeration of party death [None, Friends, Foes, Both] */
  bool checkPartyDeath(Party* const check_party);

  /* Cleanup before the end of a Battle turn */
  void cleanUp();

  /* Determines the turn progression of the Battle (based on speed) */
  void determineTurnMode();

  /* Calculates crit chances and determines whether a critical will occur */
  bool doesActionCrit();

  /* Calculates skill missing chance and determines if the skill miss */
  bool doesSkillMiss();

  /* Calculates miss chance and determines whether an action miss will occur */
  bool doesActionMiss();

  /* Deals with general upkeep (i.e. weather) */
  void generalUpkeep();

  /* Sets the flags of BattleState at the beginning of the Battle */
  void loadBattleStateFlags();

  /* Orders the actions on the buffer by speed of the aggressor */
  void orderActions();

  /* Deals with character related upkeep */
  void personalUpkeep(Person* const target);

  /* Processes an individual action from a user against targets */
  void processSkill(std::vector<Person*> targets);

  /* Process the actions (Items & Skills) in the buffer */
  void processActions();

  /* Recalculates the ailments after they have been altered */
  void recalculateAilments(Person* const target);

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

  /* Assigns a new value to the ailment update mode */
  void setAilmentUpdateMode(const BattleOptions &new_value);

  /* Assigns a new value to the battle output mode */
  void setBattleMode(const BattleMode &new_value);

  /* Assigns the friends party of the Battle */
  bool setFriends(Party* const new_friends);

  /* Assigns the foes party of the Battle */
  bool setFoes(Party* const new_foes);

  /* Assigns a new value to the hud display mode */
  void setHudDisplayMode(const BattleOptions &new_value);
  
  /* Updates the person selection index to the next valid one */
  bool setNextPersonIndex();

  /* Updates the Battle to the next state */
  void setNextTurnState();

  /* Assigns a new value for the screen width */
  void setScreenHeight(const uint16_t &new_value);

  /* Assigns a new value for the screen width */
  void setScreenWidth(const uint16_t &new_value);

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
  /* Processing key down events for the Battle */
  bool keyDownEvent(SDL_KeyboardEvent event);

  /* Methods to print information about the Battle */
  void printAll(const bool &simple, const bool &flags, const bool &party);
  void printPartyState();
  void printPersonState(Person* const member, const int32_t &person_index);
  void printInventory(Party* const target_party);
  void printTargetVariables(const bool &print_target_stats = false);
  void printTurnState();

  /* Update the cycle time of Battle */
  bool update(int32_t cycle_time);

  /* Returns the ailment update mode currently set */
  BattleOptions getAilmentUpdateMode();

  /* Returns the assigned Battle display mode */
  BattleMode getBattleMode();

  /* Return the value of a given CombatState flag */
  bool getBattleFlag(const CombatState &test_flag);

  /* Return the value of a given IgnoreState flag */
  bool getIgnoreFlag(const IgnoreState &test_flag);

  /* Returns the friends pointer of the Battle */
  Party* getFriends();

  /* Returns the foes pointer of the Battle */
  Party* getFoes();

  /* Returns the hud display mode currently set */
  BattleOptions getHudDisplayMode();

  /* Returns the value of the screen height */
  uint32_t getScreenHeight();

  /* Returns the value of the screen width */
  uint32_t getScreenWidth();

  /* Evaluates and returns a vector of ailments for a given person */
  std::vector<Ailment*> getPersonAilments(Person* const target);

  /* Returns the value of the turns elapsed */
  uint32_t getTurnsElapsed();

  /* Returns the elapsed time of the Battle */
  uint32_t getTimeElapsed();

  /* Returns the enumerated turn state of the Battle */
  TurnState getTurnState();

  /* Returns the index integer of a given Person ptr */
  int32_t getTarget(Person* battle_member);

  /* Obtains the corresponding Person ptr for a given battle member index */
  Person* getPerson(const int32_t &index);

  /* Calculate and return all BattleMember indexes */
  std::vector<int32_t> getAllTargets();

  /* Obtains the list of friends target indexes */
  std::vector<int32_t> getFriendsTargets(const bool &ko = false);
  
  /* Obtains the list of foes target indexes */
  std::vector<int32_t> getFoesTargets(const bool &ko = false);

  /* Obtains a vector of targets matching the signage */
  std::vector<int32_t> getPartyTargets(int32_t check_index);

  /* Build a vector of person pointers from a vector of person indexes */
  std::vector<Person*> getPersonsFromIndexes(std::vector<int32_t> indexes);

  /* Build a vector of person indexes from a vector of person pointers */
  std::vector<int32_t> getIndexesOfPersons(std::vector<Person*> persons);

  /* Get the index of a person from its pointer */
  int32_t getIndexOfPerson(Person* check_person);

  /* Obtains a vector of battle member indexes for a given user and scope */
  std::vector<int32_t> getValidTargets(int32_t index, ActionScope action_scope);

  /* Assings the running config */
  bool setConfiguration(Options* const config);

  /* Assign a value to a CombatState flag */
  void setBattleFlag(CombatState flags, const bool &set_value = true);

  /* Assign a value to an IgnoreState flag */
  void setIgnoreFlag(IgnoreState flags, const bool &set_value = true);

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

#endif //BATTLE_H