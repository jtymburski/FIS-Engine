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

/*
 * Description: Player constructor object
 */
Player::Player()
{
}

/*
 * Description: Annihilates a player object
 */
Player::~Player()
{
}

/*
 * Description: Gets the number of frames in the main person's sprite
 *              multiplied by the kMAPSPEED
 * Inputs: none
 * Output: int - speed of the player
 */
int Player::getSpeed()
{
  return kMAPSPEED;
}

/*
 * Description: Gets the x-position on the map
 *
 * Inputs: none
 * Output: int - x-position of the player
 */
int Player::getXPos()
{
  return xpos;
}

/*
 * Description: Gets the y-position on the map
 *
 * Inputs: none
 * Output: int - y-position on the map
 */
int Player::getYPos()
{
  return ypos;
}

/*
 * Description: Sets the x-position of the player
 *
 * Inputs: int - new x-position
 * Output: none
 */
void Player::setXPos(int new_xpos)
{
  xpos = new_xpos;
}
/*
 * Description: Sets the y-position of the player
 *
 * Inputs: int - new y-position
 * Output: none
 */
void Player::setYPos(int new_ypos)
{
  ypos = new_ypos;
}
