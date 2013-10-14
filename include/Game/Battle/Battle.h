/*******************************************************************************
* Class Name: Battle [Header Declaration]
* Date Created: December 2nd, 2012 - Rewritten October 12th, 2013
* Inheritance: QWidget
* Description: Battle is a class which contains all the events handled and
*              happening in a classic RPG battle between two Parties.
*
* Notes:
*
*
* //TODO:
******************************************************************************/
#ifndef BATTLE_H
#define BATTLE_H

#include <QDebug>
#include <QKeyEvent>
#include <QWidget>

#include "Game/Battle/BattleInfoBar.h"
#include "Game/Battle/BattleMenu.h"
#include "Game/Battle/BattleStatusBar.h"
#include "Game/Player/Party.h"
#include "GrammarHelper.h"
#include "MathHelper.h"
#include "Options.h"

class Battle : public QWidget
{
  Q_OBJECT

public:
  /* Creates a default Battle object */
  Battle();

  /* Creates a standard Battle object */
  Battle(Party* friends, Party* foes, Options config, QWidget* parent = 0);

  /* Annihilates a Battle object */
  ~Battle();

  /* Enumerated flags for battle class */
  enum BattleState
  {
    CONFIGURED       = 1ul << 0,
    FLAGS_CONFIGURED = 1ul << 1,
    RANDOM_ENCOUNTER = 1ul << 2,
    MINI_BOSS        = 1ul << 3,
    BOSS             = 1ul << 4,
    FINAL_BOSS       = 1ul << 5
  };
  Q_DECLARE_FLAGS(BattleFlags, BattleState)
  BattleFlags flag_set;

private:
  /* The Battle Info Bar */
  BattleInfoBar* info_bar;

  /* The Battle Menu Bar */
  BattleMenu* menu;

  /* The Battle Status Bar */
  BattleStatusBar* status_bar;

  /* The Background of the Battle */
  Frame* background;

  /* The background mage of the Battle Status Bar */
  Frame* status_bar_image;

  /* Enemy Status Bars */
  QVector<EnemyStatusBar*> enemy_status_bars;

  /* Enumerated battle options for ailment updates */
  Options::BattleOptions ailment_update_mode;

  /* Enumerated battle options for hud display mode */
  Options::BattleOptions hud_display_mode;

  /* Pointers to the battling parties */
  Party* friends;
  Party* foes;

  /* Dimensions of the screen */
  ushort screen_height;
  ushort screen_width;

  /* Elapsed time of the Battle */
  uint time_elapsed;

  /* Elapsed turns of hte battle */
  ushort turns_elapsed;

  /* ------------ Constants --------------- */
  static const ushort kDEFAULT_SCREEN_HEIGHT;
  static const ushort kDEFAULT_SCREEN_WIDTH;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Load default configuration of the battle */
  bool loadDefaults();

  /* Sets the flags of BattleState at the beginning of the Battle */
  bool loadBattleStateFlags();

  /* Assigns a new value to the ailment update mode */
  void setAilmentUpdateMode(Options::BattleOptions new_value);

  /* Assigns the friends party of the Battle */
  void setFriends(Party* new_friends);

  /* Assigns the foes party of the Battle */
  void setFoes(Party* new_foes);

  /* Assigns a new value to the hud display mode */
  void setHudDisplayMode(Options::BattleOptions new_value);

  /* Assign a new value for the screen width */
  void setScreenHeight(ushort new_value);

  /* Assign a new value for the screen width */
  void setScreenWidth(ushort new_value);

  /* Assigns a new value to the elapsed time */
  void setTimeElapsed(uint new_value);

  /* Assigns a new value to the turns elapsed */
  void setTurnsElapsed(ushort new_value);

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
  /* Handles all key entries for the Battle */
  void keyPressEvent(QKeyEvent* event);

  /* Returns the ailment update mode currently set */
  Options::BattleOptions getAilmentUpdateMode();

  /* Returns the friends pointer of the Battle */
  Party* getFriends();

  /* Returns the foes pointer of the Battle */
  Party* getFoes();

  /* Returns the hud display mode currently set */
  Options::BattleOptions getHudDisplayMode();

  /* Returns the value of the screen height */
  ushort getScreenHeight();

  /* Returns the value of the screen width */
  ushort getScreenWidth();

  /* Returns the value of the turns elapsed */
  ushort getTurnsElapsed();

  /* Returns the elapsed time of the Battle */
  uint getTimeElapsed();

/*=============================================================================
 * SIGNALS
 *============================================================================*/
signals:
  /* TODO */
  void closeBattle();

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

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Update the cycle time of Battle */
  void updateBattle(int cycle_time);

  /* BattleState flag functions */
  bool getBattleFlag(BattleState flags);
  void setBattleFlag(BattleState flags, bool set_value = true);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Battle::BattleFlags)

#endif //BATTLE_H
