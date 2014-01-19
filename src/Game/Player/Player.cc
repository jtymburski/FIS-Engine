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
 * CONSTANTS
 *============================================================================*/

const uint32_t Player::kDEFAULT_GRAVITY{981};
const uint32_t Player::kSTARTING_CREDITS{100};
const uint32_t Player::kMAX_CREDITS{3141592654};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
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

/* Returns the amount of credits (money) the player has */
uint32_t Player::getCredits()
{
  return credits;
}

/* Gets the bearacks party */
Party* Player::getBearacks()
{
  return bearacks;
}

/* Returns the carry weight */
double Player::getCarryWeight()
{
  if (sleuth->getInventory() != nullptr)
    return gravity * sleuth->getInventory()->getMass();

  return 0.0;
}

/* Returns the gravity */
double Player::getGravity()
{
  return gravity;
}

/* Gets the sleuth party */
Party* Player::getSleuth()
{
  return sleuth;
}

/* Methods for printing ou the info of the player */
void Player::print()
{
  //TODO: Print function [01-18-14]
}

/* Removes a sleuth member by index by calling Party's remove function */
bool Player::removeSleuthMember(const uint32_t &index)
{
  return sleuth->removeMember(index);
}

/* Removes a sleuth member by name by calling Party's remove function */
bool Player::removeSleuthMember(const std::string &name)
{
  return sleuth->removeMember(name);
}

/* Removes a bearacks member by index by calling Party's remove function */
bool Player::removeBearacksMember(const uint32_t &index)
{
  return bearacks->removeMember(index);
}

/* Removes a bearacks member by name by calling Party's remove function */
bool Player::removeBearacksMember(const std::string &name)
{
  return bearacks->removeMember(name);
}

/* Sets the amount of money the player has */
bool Player::setCredits(const uint32_t &value)
{
  if (value < kMAX_CREDITS)
  {
    credits = value;

    return true;
  }

  return false;
}

/* Sets the sleuth party */
void Player::setSleuth(Party* const new_sleuth)
{
  sleuth = new_sleuth;
}

/* Sets the bearcks party */
void Player::setBearacks(Party* const new_bearacks)
{
  bearacks = new_bearacks;
}

/* Sets the gravity the player is experiencing */
bool Player::setGravity(const uint32_t &new_value)
{
  //TODO: Maximum gravity? [01-18-14]
  gravity = new_value;

  return true;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/* Return the default gravity */
uint32_t Player::getDefaultGravity()
{
  return kDEFAULT_GRAVITY;
}

/* Return the starting credits of the player */
uint32_t Player::getStartingCredits()
{
  return kSTARTING_CREDITS;
}

/* Return the maximum credits */
uint32_t Player::getMaxCredits()
{
  return kMAX_CREDITS;
}