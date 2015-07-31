/*******************************************************************************
 * Class Name: TestBattle
 * Date Created: July 31, 2015
 * Inheritance: none
 * Description: This class is the testing interface for battle scenarios.
 ******************************************************************************/
#include "TestBattle.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
TestBattle::TestBattle(Options* running_config)
{

}

/* Destructor function */
TestBattle::~TestBattle()
{

}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
  
/* Builds the ailment frames */
void TestBattle::buildBattleDisplayFrames(SDL_Renderer* renderer)
{

}

/* Set up the battle */
void TestBattle::setupBattle()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* The key up and down events to be handled by the class */
bool TestBattle::keyDownEvent(SDL_KeyboardEvent event)
{
  /* Exit the test battle: finished processing */
  if(event.keysym.sym == SDLK_ESCAPE)
  {
    return true;
  } 
  return false;
}

/* The key up and down events to be handled by the class */
void TestBattle::keyUpEvent(SDL_KeyboardEvent event)
{

}

/* Renders the title screen */
bool TestBattle::render(SDL_Renderer* renderer)
{
  return true;
}
  
/* Set the running configuration, from the options class */
bool TestBattle::setConfiguration(Options* running_config)
{
  return true;
}

/* Updates the game state */
bool TestBattle::update(int cycle_time)
{
  return false;
}
