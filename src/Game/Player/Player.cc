/******************************************************************************
 * Class Name: Player
 * Date Created: December 2nd, 2012
 * Inheritance: none
 * Description: The player class, containing the two parties -- sleuth and
 *              bearacks, and represents any pertinent information to the
 *              working of the game player.
 *
 * TODO:
 *   1. Default value of gravity
 *   2. Deletion of party pointers
 *****************************************************************************/
#include "Game/Player/Player.h"

/*============================================================================
 * CONSTANTS
 *===========================================================================*/
const uint Player::kMAX_CREDITS = 1000000000;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Player constructor object
 *
 * Inputs: p_sleuth - pointer to sleuth party
 *         p_bearacks - pointer to bearacks party
 */
Player::Player(Party* sleuth, Party* bearacks)
  : sleuth(0), bearacks(0)
{
  setSleuth(sleuth);
  setBearacks(bearacks);
}

/*
 * Description: Annihilates a player object
 */
Player::~Player() 
{
  setSleuth(0);
  setBearacks(0);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Calculates the carry weight of the player
 *
 * Inputs: none
 * Output: double - the carry weight calculated based on inventory mass
 */
double Player::calcCarryWeight()
{
  if(sleuth != 0 && sleuth->getInventory() != 0)
    return (gravity * sleuth->getInventory()->getMass());
  return 0;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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
 * Description: Gets the pointer to the bearacks party
 * Inputs: none
 * Output: Party* - pointe to the bearacks party
 */
Party* Player::getBearacks()
{
  return bearacks;
}

/*
 * Description: Returns the carry weight of the player
 *
 * Inputs: none
 * Output: double - carry weight of the player
 */
double Player::getCarryWeight()
{
  return calcCarryWeight();
}

/*
 * Description: Returns the value of credits
 *
 * Inputs: none
 * Output: uint32_t - value of credits returned
 */
uint32_t Player::getCredits()
{
  return credits;
}

/*
 * Description: Returns the gravity of the player
 *
 * Inputs: none
 * Output: double - gravity of the player
 */
double Player::getGravity()
{
  return gravity;
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
 * Description: Prints out the information of the Player.
 *
 * Inputs: none
 * Output: none
 */
void Player::printInfo()
{
  if(sleuth != 0)
    qDebug() << "Sleuth Size: " << sleuth->getPartySize();
  if(bearacks != 0)
    qDebug() << "Bearacks Size: " << bearacks->getPartySize();
  qDebug() << "Carry Weight: " << calcCarryWeight();
  qDebug() << "Gravity: " << gravity;
  qDebug() << "Credits: " << credits;
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
  if(sleuth != 0 && sleuth->removeMember(index))
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
  if(sleuth != 0 && sleuth->removeMember(value))
    return true;
  return false;
}

/*
 * Description: Removes a member from the bearacks at a given index
 *
 * Inputs: int - index at which to remove the member
 * Output: bool - true if removal was successful
 */
bool Player::removeBearacksMember(int index)
{
  if(bearacks != 0 && bearacks->removeMember(index))
    return true;
  return false;
}

/*
 * Description: Removes a member from the bearacks by name
 *
 * Inputs: QString - name of member to remove
 * Output: bool - true if removal was successful
 */
bool Player::removeBearacksMember(QString value)
{
  if(bearacks->removeMember(value))
    return true;
  return false;
}

/*
 * Description: Sets the pointer to the bearacks party
 *
 * Inputs: Party* - pointer to the new bearacks party
 * Output: none
 */
void Player::setBearacks(Party* p)
{
  /* Delete the old bearacks */
  if(bearacks != 0)
    delete bearacks;
  bearacks = 0;

  /* Set the new bearacks */
  bearacks = p;
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
 * Description: Sets the gravity of the Player
 *
 * Inputs: double - value of gravity to be set
 * Output: none
 */
void Player::setGravity(double new_value)
{
  gravity = new_value;
}

/*
 * Description: Sets the pointer to the sleuth party
 *
 * Inputs: Party* - pointer to new sleuth party
 * Output: none
 */
void Player::setSleuth(Party* p)
{
  /* Delete the old sleuth */
  if(sleuth != 0)
    delete sleuth;
  sleuth = 0;

  /* Set the new sleuth */
  sleuth = p;
}


