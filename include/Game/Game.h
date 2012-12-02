/******************************************************************************
* Class Name: Game
* Date Created: Nov 04 2012
* Inheritance: QWidget
* Description: The Game class
******************************************************************************/
#ifndef GAME_H
#define GAME_H

#include <QtGui/QWidget>

#include "Game/Battle/Battle.h"
#include "Game/Map/Map.h"
#include "Game/Player/Player.h"
#include "Game/VictoryScreen.h"

/* All the modes in game */
enum GameMode {MAP, BATTLE, VICTORY_SCREEN };

class Game : public QWidget
{
public:
  /* Constructor function */
  Game();

  /* Destructor function */
  ~Game();

private:
  /* A current battle pointer */
  Battle* current_battle;

  /* The current game mode */
  GameMode current_gamemode;

  /* The current loaded map */
  Map* current_level;

  /* A current victory screen pointer */
  VictoryScreen* current_victoryscreen;

  /* The maps in the game */
  QVector<Map*> levels;

  /* The level number currently on */
  int level_num;

  /* The player */
  Player main_player;

  /* Timer for updateGame() function */
  QTimer tick;

  /* Flag for if the tick is enabled */
  bool GAME_ENABLED;

public:
  /* Enables or disables the GAME_ENABLED flag */
  void setEnabled (bool enable);

  /* Updates the game state */
  void updateGame();
};

#endif // GAME_H
