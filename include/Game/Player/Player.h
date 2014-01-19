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
*******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "Game/Player/Party.h"

class Player
{
public:
  /* General player construction */
  Player(Party* sleuth, Party* bearacks = nullptr);

private:
  /* The slueth and bearacks parties. The sleuth is the player's main party, 
   * while the bearacks is the reserve party */
  Party* sleuth;
  Party* bearacks;

  /* Current gravity experienced by player */
  double gravity;

  /* Current value of credits */
  uint32_t credits;

  /* ------------ Constants --------------- */
  static const uint32_t kDEFAULT_GRAVITY;
  static const uint32_t kSTARTING_CREDITS;
  static const uint32_t kMAX_CREDITS;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Calculates the carry weight of the player (based off mass & gravity) */
  double calcCarryWeight();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempt to add an amount of credits to the current total */
  bool addCredits(const int32_t &value);

  /* Returns the current credit value */
  uint32_t getCredits();
  
  /* Returns the bearacks Party */
  Party* getBearacks();

  /* Returns the carry weight of the player */
  double getCarryWeight();

  /* Returns the gravity being experiencd by the player */
  double getGravity();
 
 /* Gets the sleuth party */
  Party* getSleuth();

  /* Methods for printing ou the info of the player */
  void print();

  /* Removes a sleuth member by index by calling Party's remove function */
  bool removeSleuthMember(const int32_t &index);

  /* Removes a sleuth member by name by calling Party's remove function */
  bool removeSleuthMember(const std::string &value);

  /* Removes a bearacks member by index by calling Party's remove function */
  bool removeBearacksMember(const uint32_t &index);

  /* Removes a bearacks member by name by calling Party's remove function */
  bool removeBearacksMember(const std::sring &value);

  /* Sets the amount of money the player has */
  void setCredits(const uint32_t &value);

  /* Sets the sleuth party WARNING: DANGEROUS FUNCTION */
  void setSleuth(Party* const new_sleuth);

  /* Sets the bearcks party WARNING: Possibly a dangerous function */
  void setBearacks(Party* const = nullptr);

  /* Assigns the gravity the player is experiencing */
  void setGravity(double new_value);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Return the defaul gravity */
  static uint32_t getDefaultGravity() const;

  /* Return the starting credits of the player */
  static uint32_t getStartingCredits() const;

  /* Return the maximum credits */
  static uint32_t getMaxCredits() const;
};

#endif //PLAYER_H
