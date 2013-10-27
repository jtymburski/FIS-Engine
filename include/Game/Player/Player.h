/******************************************************************************
 * Class Name: Player
 * Date Created: December 2nd, 2012
 * Inheritance: none
 * Description: The player class, containing the two parties -- sleuth and
 *              bearacks, and represents any pertinent information to the
 *              working of the game player.
 *****************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include "Game/Player/Party.h"

class Player
{
public:
  /* Player constructor object */
  Player(Party* sleuth, Party* bearacks = 0);

  /* Annihilates a player object */
  ~Player(); 

private:
  /* Current party (Sleuth) object, and base party (Bearacks) */
  Party* sleuth;
  Party* bearacks;

  /* Gravity for weight calculations */
  double gravity;

  /* Money */
  uint32_t credits;

  /* ------------ Constants --------------- */
  static const uint kMAX_CREDITS;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Calculates the carry weight of the player */
  double calcCarryWeight();

/*============================================================================
 * PUBLIC FUNCTIONS
 w*===========================================================================*/
public:
  /* Adds an amount of money to credits */
  void addCredits(unsigned long value);

  /* Returns the amount of credits (money) the player has */
  uint32_t getCredits();

  /* Gets # of frames in the main persons sprite multiplied by kMAPSPEED */
  int getSpeed(); 

  /* Gets the bearacks party */
  Party* getBearacks();

  /* Returns the carry weight */
  double getCarryWeight();

  /* Returns the gravity */
  double getGravity();

  /* Gets the sleuth party */
  Party* getSleuth();

  /* Methods for printing ou the info of the player */
  void printInfo();

  /* Removes a sleuth member by index by calling Party's remove function */
  bool removeSleuthMember(int index);

  /* Removes a sleuth member by name by calling Party's remove function */
  bool removeSleuthMember(QString value);

  /* Removes a bearacks member by index by calling Party's remove function */
  bool removeBearacksMember(int index);

  /* Removes a bearacks member by name by calling Party's remove function */
  bool removeBearacksMember(QString value);

  /* Sets the amount of money the player has */
  void setCredits(unsigned long value);

  /* Sets the sleuth party */
  void setSleuth(Party* p = 0);

  /* Sets the bearcks party */
  void setBearacks(Party* p = 0);

  /* Sets the gravity the player is experiencing */
  void setGravity(double new_value);
};

#endif // PLAYER_H
