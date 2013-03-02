/******************************************************************************
* Class Name: Game
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Game class
******************************************************************************/
#include "Game/Game.h"

/* Constructor function */
Game::Game()
{
}

/* Destructor function */
Game::~Game()
{
}

/* Enables or disables the GAME_ENABLED flag */
void Game::setEnabled (bool enable)
{
    GAME_ENABLED = enable;
}

/* Updates the game state */
void Game::updateGame()
{
}

/*
 * Description: Evaluates a given BattleOption flag
 *
 * Inputs: BattleOption - flag to be evaluated
 * Output: const bool   - evaluation of the flag
 */
const bool Game::getBattleFlag(BattleOption flags)
{
  return bo_flag_set.testFlag(flags);
}

/*
 * Description: Flips a given BattleOption flag
 *
 * Inputs: BattleOption - flag to be flipped
 * Output: none
 */
void Game::toggleBattleFlag(BattleOption flags)
{
  setBattleFlag(flags, !getBattleFlag(flags));
}

/*
 * Description: Sets ta given BattleOption flag
 *
 * Inputs: BattleOption - flag to be set
 *         const bool   - value to set flag to
 * Output: none
 */
void Game::setBattleFlag(BattleOption flags, const bool set_value)
{
  (set_value) ? (bo_flag_set |= flags) : (bo_flag_set ^= flags);
}
