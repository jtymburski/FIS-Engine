/*******************************************************************************
* Class Name: Player [Implementation]
* Date Created: January 18th, 2014
* Inheritance: none
* Description: A Player contains the information of the Parties that are usable
*              by the player and represents pertinent information to the 
*              progression of the game.
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Player.h"

/*=============================================================================
 * CONSTANTS - See header declaration for details
 *============================================================================*/

const uint32_t Player::kDEFAULT_GRAVITY{981};
const uint32_t Player::kSTARTING_CREDITS{100};
const uint32_t Player::kMAX_CREDITS{3141592654};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Player construction! Constructs a player with a Sleuth and
 *              Bearacks party pointers.
 *
 * Inputs: Party* sleuth - pointer to the sleuth party for the player.
 *         Party* bearacks - pointer to the bearacks party for the player.
 */
Player::Player(Party* sleuth, Party* bearacks)
    : sleuth{sleuth}
    , bearacks{bearacks}
    , gravity{kDEFAULT_GRAVITY}
    , credits{kSTARTING_CREDITS}
{}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
/* Attempts to add an amount of credits to the player's total */
bool Player::addCredits(const uint32_t &value)
{
  auto added = false;

  if (value < kMAX_CREDITS / 2 && kMAX_CREDITS - value < kMAX_CREDITS)
  {
    credits += value;
    added = true;
  }

  if (credits > kMAX_CREDITS)
      credits = kMAX_CREDITS;

  return added;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
/* Returns the amount of credits (money) the player has */
uint32_t Player::getCredits()
{
  return credits;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
/* Gets the bearacks party */
Party* Player::getBearacks()
{
  return bearacks;
}

/*
 * Description: REturns the cu
 *
 * Inputs: 
 * Output: 
 */
/* Returns the carry weight */
double Player::getCarryWeight()
{
  if (sleuth->getInventory() != nullptr)
    return gravity * sleuth->getInventory()->getMass();

  return 0.0;
}

/*
 * Description: Returns the gravity the player is currently experiencing.
 *
 * Inputs: none
 * Output: uint32_t - the gravity the player is experiencing
 */
uint32_t Player::getGravity()
{
  return gravity;
}

/*
 * Description: Returns the pointer to the sleuth party.
 *
 * Inputs: none
 * Output: Party* - pointer to the sleuth party.
 */
Party* Player::getSleuth()
{
  return sleuth;
}

/*
 * Description: Prints out the information about the player
 *
 * Inputs:
 * Output: 
 */
void Player::print()
{
  //TODO: Print function [01-18-14]
}

/*
 * Description: Removes a sleuth member by index by calling its party's removal
 *              function.
 *          
 * Inputs: uint32_t index - the index of sleuth member to be removed.
 * Output: bool - true if the member was removed successfully.
 */
bool Player::removeSleuthMember(const uint32_t &index)
{
  return sleuth->removeMember(index);
}

/*
 * Description: Removes a sleuth member by name by calling Party's remove
 *              function.
 *
 * Inputs: const std::string name - the name of the sleuth member to remo.
 * Output: bool - true if the member was removed.
 */
bool Player::removeSleuthMember(const std::string &name)
{
  return sleuth->removeMember(name);
}

/*
 * Description: Removes a bearacks member by index by calling the Paty's
 *              remove function.
 *
 * Inputs: uint32_t index the index of the bearacks member to be removed.
 * Output: bool - true if the removal occurs.
 */
bool Player::removeBearacksMember(const uint32_t &index)
{
  return bearacks->removeMember(index);
}

/*
 * Description: Removes a bearacks member by name by calling Party's remove
 *              function.
 *
 * Inputs: const std::string name - the name of the bearacks member to be rem.
 * Output: bool - true if the member was removed.
 */
bool Player::removeBearacksMember(const std::string &name)
{
  return bearacks->removeMember(name);
}

/*
 * Description: Assigns the amount of money the player has.
 *
 * Inputs: uint32_t value - the amoutn of credtis the player has.
 * Output: bool - true if the credit assignment was kosher.
 */
bool Player::setCredits(const uint32_t &value)
{
  if (value < kMAX_CREDITS)
  {
    credits = value;

    return true;
  }

  return false;
}

/*
 * Description: Assigns the Sleuth party.
 *
 * Inputs: Party* new_slueth - new sleuth party.
 * Output: none
 */
void Player::setSleuth(Party* const new_sleuth)
{
  sleuth = new_sleuth;
}

/*
 * Description: Assigns a pointer to the Bearacks party.
 *
 * Inputs: Party* pointer to the bearacks party.
 * Output: none
 */
void Player::setBearacks(Party* const new_bearacks)
{
  bearacks = new_bearacks;
}

/*
 * Description: Assigns a new value to gravity for the gravity that the player
 *              is experiencing.
 *
 * Inputs: uint32_t new_value - new value for gravity for the player.
 * Output: bool - true if the assignment was successful.
 */
bool Player::setGravity(const uint32_t &new_value)
{
  gravity = new_value;

  return true;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the default value for gravity.
 *
 * Inputs: none
 * Output: uint32_t - the default level of gravity.
 */
uint32_t Player::getDefaultGravity()
{
  return kDEFAULT_GRAVITY;
}

/*
 * Description: Returns the starting credits for the Player.
 *
 * Inputs: none.
 * Output: uint32_t - the starting amount of credits for the player
 */
uint32_t Player::getStartingCredits()
{
  return kSTARTING_CREDITS;
}

/*
 * Description: Returns the maximum possible credits that a Player may have.
 *
 * Inputs: none
 * Output: uint32_t - the maximum possible value for credits
 */
uint32_t Player::getMaxCredits()
{
  return kMAX_CREDITS;
}
