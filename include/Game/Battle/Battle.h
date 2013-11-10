/*******************************************************************************
* Class Name: Battle [Header Declaration]
* Date Created: December 2nd, 2012 - Rewritten October 12th, 2013
* Inheritance: QWidget
* Description: Battle is a class which contains all the events handled and
*              happening in a classic RPG battle between two Parties.
*
* Notes:
*
** 1. generalUpkeep() adjusts all values based on Weather.  BattleInfoBar
* displays info on these adjustments * after X seconds, personalUpkeep() is
* called.
*
* 2. personalUpkeep() adjusts all values of the Person represented by
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
* //TODO: Battle Weather effects [11-03-13]
******************************************************************************/
#ifndef BATTLE_H
#define BATTLE_H

#include <QDebug>
#include <QKeyEvent>
#include <QWidget>

#include "Game/Battle/BattleInfoBar.h"
#include "Game/Battle/BattleMenu.h"
#include "Game/Battle/BattleStatusBar.h"
#include "Game/Battle/SkillBuffer.h"
#include "Game/Battle/ItemBuffer.h"
#include "Game/Player/Party.h"
#include "GrammarHelper.h"
#include "MathHelper.h"
#include "Options.h"

class Battle : public QWidget
{
  Q_OBJECT

public:
  /* Creates a standard Battle object */
  Battle(Party* friends, Party* foes, Options config, QWidget* parent = 0);

  /* Annihilates a Battle object */
  ~Battle();

  /* Enumerated flags for battle class */
  enum BattleState
  {
    CONFIGURED              = 1ul << 0,
    FLAGS_CONFIGURED        = 1ul << 1,
    RANDOM_ENCOUNTER        = 1ul << 2,
    MINI_BOSS               = 1ul << 3,
    BOSS                    = 1ul << 4,
    FINAL_BOSS              = 1ul << 5,
    CURRENT_STATE_COMPLETE  = 1ul << 6,
    BATTLE_LOST             = 1ul << 7,
    BATTLE_WON              = 1ul << 8,
    BATTLE_OUTCOME_COMPLETE = 1ul << 9,
    ERROR_STATE             = 1ul << 10
  };

  Q_DECLARE_FLAGS(BattleFlags, BattleState)
  BattleFlags flag_set;

  /* TargetingMode -- current targeting mode selection */
  enum TargetingMode
  {
    DISABLED,
    INACTIVE,
    ACTIVE_OVER_FRIENDS,
    ACTIVE_OVER_FOES
  };

  /* TurnState -- the current turn state of the Battle */
  enum TurnState
  {
    BATTLE_BEGIN,
    GENERAL_UPKEEP,
    UPKEEP,
    SELECT_USER_ACTION,
    SELECT_ENEMY_ACTION,
    ORDER_ACTIONS,
    PROCESS_ACTIONS,
    CLEAN_UP,
    BATTLE_LOSS,
    BATTLE_VICTORY,
    BATTLE_DESTRUCT
  };

private:
  /* The Battle Info Bar */
  BattleInfoBar* info_bar;

  /* The Battle Menu Bar */
  BattleMenu* menu;

  /* The Battle Status Bar */
  BattleStatusBar* status_bar;

  /* Enemy Status Bars */
  QVector<EnemyStatusBar*> enemy_status_bars;

  /* The Item Buffer of the Battle */
  ItemBuffer* item_buffer;

  /* Skill Buffer of the Battle */
  SkillBuffer* skill_buffer;

  /* The Background of the Battle */
  Frame* bg;

  /* The background mage of the Battle Status Bar */
  Frame* status_bar_bg;

  /* Enumerated battle options for ailment updates */
  Options::BattleOptions ailment_update_mode;

  /* Enumerated battle options for hud display mode */
  Options::BattleOptions hud_display_mode;

  /* Enumerated battle mode for Battle output type */
  Options::BattleMode battle_mode;

  /* Pointers to the battling parties */
  Party* friends;
  Party* foes;

  /* Dimensions of the screen */
  ushort screen_height;
  ushort screen_width;

  /* The current targeting mode of the Battle */
  TargetingMode targeting_mode;

  /* The index of the currently selected target */
  uint target_index;

  /* Elapsed time of the Battle */
  uint time_elapsed;

  /* Elapsed turns of hte battle */
  ushort turns_elapsed;

  /* The turn state of the Battle */
  TurnState turn_state;

  // Currently active weather condition
  // Weather* weather_condition

  /* ------------ Menu Constants --------------- */
  static const ushort kDEFAULT_SCREEN_HEIGHT;
  static const ushort kDEFAULT_SCREEN_WIDTH;
  static const ushort kGENERAL_UPKEEP_DELAY;
  static const ushort kBATTLE_MENU_DELAY;

  /* ------------ Battle Modifiers --------------- */
  static const ushort kMAXIMUM_DAMAGE;
  static const ushort kMINIMUM_DAMAGE;
  static const float kOFF_PRIM_ELM_MODIFIER;
  static const float kDEF_PRIM_ELM_MODIFIER;
  static const float kOFF_SECD_ELM_MODIFIER;
  static const float kDEF_SECD_ELM_MODIFIER;
  static const float kOFF_CRIT_MODIFIER;
  static const float kDEF_CRIT_MODIFIER;
  static const float kBASE_CRIT_MODIFIER;
  static const float kDODGE_MODIFIER;
  static const float kDODGE_PER_LEVEL_MODIFIER;
  static const float kPRIM_ELM_ADV_MODIFIER;
  static const float kPRIM_ELM_DIS_MODIFIER;
  static const float kSECD_ELM_ADV_MODIFIER;
  static const float kSECD_ELM_DIS_MODIFIER;
  static const float kDOUBLE_ELM_ADV_MODIFIER;
  static const float kDOUBLE_ELM_DIS_MODIFIER;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Called when the Battle has been won */
  void battleWon();

  /* Called when the Battle has been lost */
  void battleLost();

  /* Returns enumeration of party death [None, Friends, Foes, Both] */
  bool checkPartyDeath(Party* check_party);

  /* Cleanup before the end of a Battle turn */
  void cleanUp();

  /* Deals with general upkeep (i.e. weather) */
  void generalUpkeep();

  /* Load default configuration of the battle */
  bool loadDefaults();

  /* Sets the flags of BattleState at the beginning of the Battle */
  bool loadBattleStateFlags();

  /* Orders the actions on the buffer by speed of the aggressor */
  void orderActions();

  /* Actually performs the actions in the buffer */
  void performAction();

  /* Deals with character related upkeep */
  void personalUpkeep(uint target);

  /* Process the actions (Items & Skills) in the buffer */
  void processActions();

  /* Recalculates the ailments after they have been altered */
  void recalculateAilments();

  /* Calculates enemy actions and add them to the buffer */
  void selectEnemyActions();

  /* Calculates user actions and add them to the buffer */
  void selectUserActions();

  /* Method which calls personal upkeeps */
  void upkeep();

  /* Assigns a new value to the ailment update mode */
  void setAilmentUpdateMode(Options::BattleOptions new_value);

  /* Assigns a new value to the battle output mode */
  void setBattleMode(Options::BattleMode new_value);

  /* Assigns the friends party of the Battle */
  void setFriends(Party* new_friends);

  /* Assigns the foes party of the Battle */
  void setFoes(Party* new_foes);

  /* Assigns a new value to the hud display mode */
  void setHudDisplayMode(Options::BattleOptions new_value);

  /* Updates the Battle to the next state */
  void setNextTurnState();

  /* Assign a new value for the screen width */
  void setScreenHeight(ushort new_value);

  /* Assign a new value for the screen width */
  void setScreenWidth(ushort new_value);

  /* Assigns a new value to the targeting index of the selected party */
  bool setTargetIndex(uint new_value);

  /* Assigns a new targeting mode value */
  void setTargetingMode(TargetingMode new_value);

  /* Assigns a new value to the elapsed time */
  void setTimeElapsed(uint new_value);

  /* Assigns a new value to the turns elapsed */
  void setTurnsElapsed(ushort new_value);

  /* Updates the turn state of the Battle */
  void setTurnState(TurnState new_turn_state);

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
  /* Handles all key entries for the Battle */
  void keyPressEvent(QKeyEvent* event);

  /* Returns the ailment update mode currently set */
  Options::BattleOptions getAilmentUpdateMode();

  /* Returns the assigned Battle display mode */
  Options::BattleMode getBattleMode();

  /* Returns the friends pointer of the Battle */
  Party* getFriends();

  /* Returns the foes pointer of the Battle */
  Party* getFoes();

  /* Returns the hud display mode currently set */
  Options::BattleOptions getHudDisplayMode();

  /* Returns true if a party has died */
  bool getPartyDeath();

  /* Returns the value of the screen height */
  ushort getScreenHeight();

  /* Returns the value of the screen width */
  ushort getScreenWidth();

  /* Returns the value of the target index of the currently selected index */
  uint getTargetingIndex();

    /* Returns the currently assigned targeting mode */
  TargetingMode getTargetingMode();

  /* Returns the value of the turns elapsed */
  ushort getTurnsElapsed();

  /* Returns the elapsed time of the Battle */
  uint getTimeElapsed();

  /* Returns the enumerated turn state of the Battle */
  Battle::TurnState getTurnState();

/*=============================================================================
 * SIGNALS
 *============================================================================*/
signals:
  /* Closes the Battle */
  void closeBattle();

  /* Signal for the finishing of Battle */
  void finished();

  /* Emitted when the Battle reaches an error */
  void battleError(QString error);

  /* Emitted when the time is reset */
  void battleTimerUpdate();

  /* Emitted when the turns occured changes other than when it increments */
  void battleTurnsReset();

/*=============================================================================
 * PUBLIC SLOTS
 *============================================================================*/
public slots:
  /* Checks for outcomes of an action after an action takes place */
  // void actionOutcome();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Methods to print information about the Battle */
  void printAll();
  void printFlags();
  void printInfo();
  void printPartyState();
  void printTurnState();
  void printOther();

  /* Update the cycle time of Battle */
  void updateBattle(int cycle_time);

  /* BattleState flag functions */
  bool getBattleFlag(BattleState flags);
  void setBattleFlag(BattleState flags, bool set_value = true);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Battle::BattleFlags)

#endif //BATTLE_H
