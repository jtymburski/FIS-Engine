/******************************************************************************
* Class Name: Game
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Game class
******************************************************************************/
#include "Game/Player/Game.h"

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
