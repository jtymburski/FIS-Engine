/******************************************************************************
* Class Name: Game
* Date Created: Dec 2 2012
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
#include "Game/Game.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Game::Game()
{
  game_config = 0;
  game_map = 0;
  game_mode = DISABLED;
}

/* Constructor function */
Game::Game(Options* running_config)
{
  game_config = running_config;
  game_map = 0;
  game_mode = DISABLED;
}

/* Destructor function */
Game::~Game()
{
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/


/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Evaluates a given BattleOption flag
 *
 * Inputs: BattleOption - flag to be evaluated
 * Output: bool   - evaluation of the flag
 */
bool Game::getBattleFlag(BattleOption flags)
{
  return bo_flag_set.testFlag(flags);
}

/* Enables or disables the GAME_ENABLED flag */
void Game::setEnabled (bool enable)
{
  GAME_ENABLED = enable;
}

/*
 * Description: Sets ta given BattleOption flag
 *
 * Inputs: BattleOption - flag to be set
 *         bool   - value to set flag to
 * Output: none
 */
void Game::setBattleFlag(BattleOption flags, bool set_value)
{
  (set_value) ? (bo_flag_set |= flags) : (bo_flag_set ^= flags);
}

void Game::switchGameMode(GameMode mode)
{
  game_mode = mode;
}

/* Updates the game state */
void Game::updateGame()
{
}


