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
* 4. processEnemyActions() uses the enemies scripts to add actions to the
* stack, after which orderActions() is called.
*
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
// #include "Game/Battle/InfoBar.h"
// #include "Game/Battle/BattleMenu.h"
// #include "Game/Battle/StatusBar.h"
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
  PHASE_DONE       = 1 << 2,
  VICTORY          = 1 << 3,
  LOSS             = 1 << 4,
  OUTCOME_DONE     = 1 << 5,
  ERROR_STATE      = 1 << 6,
  RANDOM_ENCOUNTER = 1 << 7,
  MINI_BOSS        = 1 << 8,
  BOSS             = 1 << 9,
  FINAL_BOSS       = 1 << 10
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
  DESTRUCT              /* Battle should be destructed */
};

class Battle
{
public:
  /* Constructs a party given two parties and configured options */
  Battle(Party* const friends, Party* const foes, const Options &config);

  /* Annihilates a Battle object */
  ~Battle();

private:
  /* Pairs of unique-ID vs. Ailment pointers for ailments */
  std::vector<std::pair<uint32_t, Ailment*>> ailments;

  /* The Battle Info Bar */
  //InfoBar* info_bar;

  /* The Battle Menu Bar */
  //BattleMenu* menu;

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

  /* Pointers to the battling parties */
  Party* friends;
  Party* foes;

  /* Running config */
  Options config;

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

  /* ------------ Menu Constants --------------- */
  static const uint16_t kGENERAL_UPKEEP_DELAY;
  static const uint16_t kBATTLE_MENU_DELAY;

  /* ------------ Battle Modifiers (See Implementation) --------------- */
  static const uint16_t kMAX_AILMENTS;
  static const uint16_t kMAX_EACH_AILMENTS;
  static const uint16_t kMAXIMUM_DAMAGE;
  static const uint16_t kMINIMUM_DAMAGE;
  static const float    kOFF_PRIM_ELM_MODIFIER;
  static const float    kDEF_PRIM_ELM_MODIFIER;
  static const float    kOFF_SECD_ELM_MODIFIER;
  static const float    kDEF_SECD_ELM_MODIFIER;
  static const float    kOFF_CRIT_MODIFIER;
  static const float    kDEF_CRIT_MODIFIER;
  static const float    kBASE_CRIT_MODIFIER;
  static const float    kDODGE_MODIFIER;
  static const float    kDODGE_PER_LEVEL_MODIFIER;
  static const float    kPRIM_ELM_ADV_MODIFIER;
  static const float    kPRIM_ELM_DIS_MODIFIER;
  static const float    kSECD_ELM_ADV_MODIFIER;
  static const float    kSECD_ELM_DIS_MODIFIER;
  static const float    kDOUBLE_ELM_ADV_MODIFIER;
  static const float    kDOUBLE_ELM_DIS_MODIFIER;

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

  /* Returns enumeration of party death [None, Friends, Foes, Both] */
  bool checkPartyDeath(Party* const check_party);

  /* Cleanup before the end of a Battle turn */
  void cleanUp();

  /* Determines the turn progression of the Battle (based on speed) */
  void determineTurnMode();

  /* Deals with general upkeep (i.e. weather) */
  void generalUpkeep();

  /* Sets the flags of BattleState at the beginning of the Battle */
  void loadBattleStateFlags();

  /* Orders the actions on the buffer by speed of the aggressor */
  void orderActions();

  /* Actually performs the actions in the buffer */
  void performAction();

  /* Deals with character related upkeep */
  void personalUpkeep(Person* const target);

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

  /* Updates the Battle to the next state */
  void setNextTurnState();

  /* Assings the running config */
  void setRunningConfig(const Options &config);

  /* Assigns a new value for the screen width */
  void setScreenHeight(const uint8_t &new_value);

  /* Assigns a new value for the screen width */
  void setScreenWidth(const uint8_t &new_value);

  /* Assigns a new value to the elapsed time */
  void setTimeElapsed(const uint8_t &new_value);

  /* Assigns thee time elapsed this turn */
  void setTimeElapsedThisTurn(const uint8_t &new_value);

  /* Assigns a new value to the turns elapsed */
  void setTurnsElapsed(const uint8_t &new_value);

  /* Assigns a new turn mode to the Battle */
  void setTurnMode(const TurnMode &new_turn_mode);

  /* Updates the turn state of the Battle */
  void setTurnState(const TurnState &new_turn_state);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns true if all members of a party have died */
  bool isPartyDead();

  /* Methods to print information about the Battle */
  void printAll(const bool &simple, const bool &flags, const bool &party);
  void printPartyState();
  void printTurnState();

  /* Update the cycle time of Battle */
  void updateBattle(int32_t cycle_time);

  /* Returns the ailment update mode currently set */
  BattleOptions getAilmentUpdateMode();

  /* Returns the assigned Battle display mode */
  BattleMode getBattleMode();

  /* Return the value of a given CombatState flag */
  bool getBattleFlag(CombatState test_flag);

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

  /* Assign a value to a CombatState flag */
  void setBattleFlag(CombatState flags, const bool &set_value = true);

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
  static float getOffCritMod();
  static float getDefCritMod();
  static float getBaseCritMod();
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
