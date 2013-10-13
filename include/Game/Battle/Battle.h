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
    CONFIGURED      = 1ul << 0,
    RANDOM_ENCOUTER = 1ul << 1,
    MINI_BOSS       = 1ul << 2,
    BOSS            = 1ul << 3,
    FINAL_BOSS      = 1ul << 4
  };
  Q_DECLARE_FLAGS(BattleFlags, BattleState)
  BattleFlags flag_set;

private:
  /* Enumerated battle options for ailment updates */
  Options::BattleOptions ailment_update_mode;

  /* Enumerated battle options for hud display mode */
  Options::BattleOptions hud_display_mode;

  /* Elapsed time of the Battle */
  uint time_elapsed;

  /* Pointers to the battling parties */
  Party* friends;
  Party* foes;

  /* Dimensions of the screen */
  uint screen_height;
  uint screen_width;

  /* ------------ Constants --------------- */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Load default configuration of the battle */
  bool loadDefaults();

  /* Assigns a new value to the elapsed time */
  void setTimeElapsed(uint new_value);

  /* Assigns the friends party of the Battle */
  void setFriends(Party* new_friends);

  /* Assigns the foes party of the Battle */
  void setFoes(Party* new_foes);

  /* Assign a new value for the screen width */
  void setScreenHeight(uint new_value);

  /* Assign a new value for the screen width */
  void setScreenWidth(uint new_value);

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
  /* Handles all key entries for the Battle */
  void keyPressEvent(QKeyEvent* event);

  /* Returns the elapsed time of the Battle */
  uint getTimeElapsed();

  /* Returns the friends pointer of the Battle */
  Party* getFriends();

  /* Returns the foes pointer of the Battle */
  Party* getFoes();

  /* Returns the value of the screen height */
  uint getScreenHeight();

  /* Returns the value of the screen width */
  uint getScreenWidth();

/*=============================================================================
 * SIGNALS
 *============================================================================*/
signals:
  /* TODO */
  void closeBattle();

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
