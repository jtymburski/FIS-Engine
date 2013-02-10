/******************************************************************************
* Class Name: Player
* Date Created: December 2nd, 2012
* Inheritance:
* Description: Abstract player class, containing the two parties -- sleuth and
*              bearacks, and also represents the player's position and speed
*              on the map
******************************************************************************/

#include "Game/Player/Player.h"

/*
 * Description: Player constructor object
 *
 * Inputs: p_sleuth - pointer to sleuth party
 *         p_bearacks - pointer to bearacks party
 *         x, y - initial position of player on map
 */
Player::Player(Party* p_sleuth, Party* p_racks, int x, int y)
{
    /* Set the sleuth and bearacks parties. */
    setSleuth(p_sleuth);
    setBearacks(p_racks);

    setXPos(x);
    setYPos(y);
}

/*
 * Description: Annihilates a player object
 */
Player::~Player()
{
    /* Set party pointers to NULL */
    setSleuth();
    setBearacks();
}

/*
 * Description: Adds an amount of credits (money) to the player
 *
 * Inputs: unsigned long - value of money to be added
 * Output: none
 */
void Player::addCredits(unsigned long value)
{
    (credits + value < kMAX_CREDITS) ? (credits += value) :
                                       (credits = kMAX_CREDITS);
}

/*
 * Description: Removes a sleuth member by index by
 *              calling party's remove member function
 *
 * Inputs: int - index of sleuth member to be removed
 * Output: bool - true if member was removed successfully
 */
bool Player::removeSleuthMember(int index)
{
    if (sleuth->removeMember(index))
        return true;
    return false;
}

/*
 * Description: Removes a member from the sleuth by name
 *
 * Inputs: QString - name of person to be removed from the bearacks
 * Output: bool - true if member was removed succesfully
 */
bool Player::removeSleuthMember(QString value)
{
    if (sleuth->removeMember(value))
      return true;
    return false;
}

/*
 * Description: Removes a member from the bearacks at a given index
 *
 * Inputs: int - index at which to remove the member
 * Output: bool - true if removal was successful
 */
bool Player::removeRacksMember(int index)
{
    if (bearacks->removeMember(index))
      return true;
    return false;
}

/*
 * Description: Removes a member from the bearacks by name
 *
 * Inputs: QString - name of member to remove
 * Output: bool - true if removal was successful
 */
bool Player::removeRacksMember(QString value)
{
    if (bearacks->removeMember(value))
      return true;
    return false;
}

/*
 * Description: Returns the value of credits
 *
 * Inputs: none
 * Output: unsigned int - value of credits returned
 */
unsigned long Player::getCredits()
{
  return credits;
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
 * Description: Gets the pointer to the sleuth party
 *
 * Inputs: none
 * Output: Party* - pointer to the sleuth party
 */
Party* Player::getSleuth()
{
    return sleuth;
}

/*
 * Description: Gets the pointer to the bearacks party
 * Inputs: none
 * Output: Party* - pointe to the bearacks party
 */
Party* Player::getBearacks()
{
  return bearacks;
}

/*
 * Description: Gets the x-position on the map
 *
 * Inputs: none
 * Output: int - x-position of the player
 */
int Player::getXPos()
{
  return x_pos;
}

/*
 * Description: Gets the y-position on the map
 *
 * Inputs: none
 * Output: int - y-position on the map
 */
int Player::getYPos()
{
  return y_pos;
}

/*
 * Description: Sets an amount of credits
 *
 * Inputs: unsigned long - value of money to be set
 * Output: none
 */
void Player::setCredits(unsigned long value)
{
  (value < kMAX_CREDITS) ? (credits = value) : (credits = kMAX_CREDITS);
}

/*
 * Description: Sets the pointer to the sleuth party
 *
 * Inputs: Party* - pointer to new sleuth party
 * Output: none
 */
void Player::setSleuth(Party* p)
{
   sleuth = p;
}

/*
 * Description: Sets the pointer to the bearacks party
 *
 * Inputs: Party* - pointer to the new bearacks party
 * Output: none
 */
void Player::setBearacks(Party* p)
{
  bearacks = p;
}

/*
 * Description: Sets the x-position of the player
 *
 * Inputs: int - new x-position
 * Output: none
 */
void Player::setXPos(int new_x_pos)
{
  x_pos = new_x_pos;
}
/*
 * Description: Sets the y-position of the player
 *
 * Inputs: int - new y-position
 * Output: none
 */
void Player::setYPos(int new_y_pos)
{
  y_pos = new_y_pos;
}
