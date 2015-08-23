/*******************************************************************************
* Class Name: Player [Declaration]
* Date Created: January 18th, 2014
* Inheritance: None
* Description: A Player contains the information of the Parties that are usable
*              by the player and represents pertinent information to the 
*              progression of the game.
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* - Print function [01-18-14]
*******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "Game/Player/Party.h"

class Player
{
public:
  /* General player construction */
  Player(Party* sleuth = nullptr, Party* bearacks = nullptr);

private:
  /* The slueth and bearacks parties. The sleuth is the player's main party, 
   * while the bearacks is the reserve party */
  Party* sleuth;
  Party* bearacks;

  /* Current gravity experienced by player */
  uint32_t gravity;

  /* Current value of credits */
  uint32_t credits;

  /* ------------ Constants --------------- */
  static const uint32_t kDEFAULT_GRAVITY;
  static const uint32_t kSTARTING_CREDITS;
  static const uint32_t kMAX_CREDITS;

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempt to add an amount of credits to the current total */
  bool addCredits(const uint32_t &value);

  /* Returns the current credit value */
  uint32_t getCredits();
  
  /* Returns the bearacks Party */
  Party* getBearacks();

  /* Returns the carry weight of the player */
  double getCarryWeight();

  /* Returns the gravity being experiencd by the player */
  uint32_t getGravity();
 
 /* Gets the sleuth party */
  Party* getSleuth();

  /* Methods for printing ou the info of the player */
  void print();

  /* Removes a sleuth member by index by calling Party's remove function */
  bool removeSleuthMember(const uint32_t &index);

  /* Removes a sleuth member by name by calling Party's remove function */
  bool removeSleuthMember(const std::string &name);

  /* Removes a bearacks member by index by calling Party's remove function */
  bool removeBearacksMember(const uint32_t &index);

  /* Removes a bearacks member by name by calling Party's remove function */
  bool removeBearacksMember(const std::string &name);

  /* Sets the amount of money the player has */
  bool setCredits(const uint32_t &value);

  /* Sets the sleuth party WARNING: DANGEROUS FUNCTION */
  void setSleuth(Party* const new_sleuth);

  /* Sets the bearcks party WARNING: Possibly a dangerous function */
  void setBearacks(Party* const new_bearacks = nullptr);

  /* Assigns the gravity the player is experiencing */
  bool setGravity(const uint32_t &new_value);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Return the defaul gravity */
  static uint32_t getDefaultGravity();

  /* Return the starting credits of the player */
  static uint32_t getStartingCredits();

  /* Return the maximum credits */
  static uint32_t getMaxCredits();
};

#endif //PLAYER_H
