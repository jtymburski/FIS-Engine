/******************************************************************************
* Class Name: Player
* Date Created: December 2nd, 2012
* Inheritance:
* Description: Person object on map
*  
* TODO: CONSTRUCTORS TO BE FINISHED
* TODO: CHANGE SPEED (CONSTANT) ?
******************************************************************************/

#include "Game/Player/Player.h"

/* Constructs a Player Object */
Player::Player()
{
}

/* Annihilates a Player object */
Player::~Player()
{
}

/* Gets the number of frames in the main persons sprite multiplied by the 
   * kMAPSPEED */
int Player::getSpeed()
{
  return kMAPSPEED;
}

/* Gets the x-position on the map */
int Player::getXPos()
{
  return xpos;
}

/* Gets the y-position on the map */
int Player::getYPos()
{
  return ypos;
}

/* Sets the x-position on the map */
void Player::setXPos(int new_xpos)
{
  xpos = new_xpos;
}

/* Sets the y-position on the map */
void Player::setYPos(int new_ypos)
{
  ypos = new_ypos;
}
