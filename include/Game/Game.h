/******************************************************************************
* Class Name: Game
* Date Created: Nov 04 2012
* Inheritance: QStackedWidget
* Description: This class is the top layer that handles all interclass
*              communication between map, player, and battle as well as all
*              setup and overall insight and control. The painting control
*              won't be handled here and this will just act as an intermediate
*              data highway / event handler.
*
* TODO:
*  1. Add Event Handler. Integrate in to handle all exchanges between class
******************************************************************************/
#ifndef GAME_H
#define GAME_H

#include <QStackedWidget>

#include "Game/Battle/Battle.h"
#include "Game/EventHandler.h"
#include "Game/Map/Map.h"
#include "Game/Player/Player.h"
#include "Game/VictoryScreen.h"
#include "MathHelper.h"
#include "Options.h"

class Game : public QStackedWidget
{
  Q_OBJECT

public:
  /* Constructor functions */
  Game();
  Game(Options* running_config);

  /* Destructor function */
  ~Game();

  // TODO: Why is this here?? Should be in battle
  enum BattleOption
  {
    SHOWLEVELS               = 1ul << 0, /* Show levels of friends/foes       */
    SHOWENEMYINFO            = 1ul << 1, /* Show enemy info on Battle Screen? */
    SHOWALLAILMENTINFO       = 1ul << 2, /* Show all ailment info in Battle?  */
    SHOWESSENTIALAILMENTINFO = 1ul << 3  /* Show only essential ailment info? */
  };
  Q_DECLARE_FLAGS(BattleOptions, BattleOption)

  /* The game mode operator, for controlling the visible widget */
  enum GameMode {DISABLED, MAP, BATTLE, VICTORY_SCREEN};

private:
  /* Battle options variable for flags being set */
  BattleOptions bo_flag_set;

  /* A current battle pointer */
  //Battle* current_battle;
  
  /* A current victory screen pointer */
  //VictoryScreen* current_victoryscreen;

  /* The configuration for the display of the game */
  Options* game_config;

  /* The current loaded map */
  Map* game_map; // TODO: Make non-pointer?

  /* The current game mode */
  GameMode game_mode;

  /* The maps in the game */
  //QVector<Map*> levels;
  //QList<QString> level_list;

  /* The level number currently on */
  //int level_num;

  /* The player */
  //Player main_player;

  /* Timer for updateGame() function */
  //QTimer tick;

  /* Flag for if the tick is enabled */
  bool GAME_ENABLED;

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Retrieve battle option flag and what it's set to */
  bool getBattleFlag(BattleOption flags);

  /* Enables or disables the GAME_ENABLED flag */
  void setEnabled(bool enable);

  /* Set the Battle Option Flag(s) */
  void setBattleFlag(BattleOption flags, bool set_value);

  /* Switches the running game mode */
  void switchGameMode(GameMode mode); // TODO: Make private

  /* Updates the game state */
  void updateGame();
};

/* Qt Declares */
Q_DECLARE_OPERATORS_FOR_FLAGS(Game::BattleOptions)

#endif // GAME_H
