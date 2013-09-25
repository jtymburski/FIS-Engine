/******************************************************************************
* Class Name: Game
* Date Created: Nov 04 2012
* Inheritance: QStackedWidget
* Description: This class is the top layer that handles all interclass
*              communication between map, player, and battle as well as all
*              setup and overall insight and control. The painting control
*              won't be handled here and this will just act as an intermediate
*              data highway / event handler.
******************************************************************************/
#ifndef GAME_H
#define GAME_H

//#include <QDebug>
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
  Game(QWidget* parent = 0);
  Game(Options running_config, QWidget* parent = 0);

  /* Destructor function */
  ~Game();

  /* The game mode operator, for controlling the visible widget */
  enum GameMode {DISABLED       = 0, 
                 MAP            = 1, 
                 BATTLE         = 2, 
                 VICTORY_SCREEN = 3};

private:
  /* A blank black screened widget for disabled mode */
  QWidget blank_widget;

  /* A current battle pointer */
  Battle* game_battle;
  


  /* The configuration for the display of the game */
  Options game_config;

  /* The current loaded map */
  Map* game_map; // TODO: Make non-pointer?

  /* The maps in the game */
  //QVector<Map*> levels;
  //QList<QString> level_list;

  /* The level number currently on */
  //int level_num;

  /* The player */
  //Player main_player;

  /* A current victory screen pointer */
  //VictoryScreen* victory_screen;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Set up the battle */
  void setupBattle();

  /* Sets up the game - called on first creation */
  void setupGame();

  /* Set up the map */
  void setupMap();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void closeGame();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Set the running configuration, from the options class */
  void setConfiguration(Options running_config);

  /* Switches the running game mode */
  void switchGameMode(GameMode mode); // TODO: Make private

  /* Updates the game state */
  void updateGame();
};

#endif // GAME_H
