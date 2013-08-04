/******************************************************************************
* Class Name: Game
* Date Created: Nov 04 2012
* Inheritance: QWidget
* Description: The Game class
******************************************************************************/
#ifndef GAME_H
#define GAME_H

#include <QtGui/QWidget>
#include "MathHelper.h"
#include "Game/Battle/Battle.h"
#include "Game/Map/Map.h"
#include "Game/Player/Player.h"
#include "Game/VictoryScreen.h"

class Game : public QWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  Game();

  /* Destructor function */
  ~Game();

  /* Public enumerators */
  enum BattleOption
  {
    SHOWLEVELS    = 1ul << 0, /* Show levels of friends/foes on Battle? */
    SHOWENEMYINFO = 1ul << 1, /* Show enemy info on Battle Screen? */
  };
  Q_DECLARE_FLAGS(BattleOptions, BattleOption)
  enum GameMode {MAP, BATTLE, VICTORY_SCREEN };

private:
  /* A current battle pointer */
  Battle* current_battle;

  /* Battle options variable for flags being set */
  BattleOptions bo_flag_set;
  
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

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Enables or disables the GAME_ENABLED flag */
  void setEnabled (bool enable);

  /* Updates the game state */
  void updateGame();

  /* Battle Option Flags */
  bool getBattleFlag(BattleOption flags);
  void toggleBattleFlag(BattleOption flags);
  void setBattleFlag(BattleOption flags, bool set_value);
};

/* Qt Declares */
Q_DECLARE_OPERATORS_FOR_FLAGS(Game::BattleOptions)

#endif // GAME_H
