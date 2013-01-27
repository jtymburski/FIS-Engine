/******************************************************************************
* Class Name: Player
* Date Created: Sunday, November 4th, 2012
* Inheritance: None?
* Description: Person object on map
*  
* TODO: CONSTRUCTORS TO BE FINISHED
* TODO: CHANGE SPEED (CONSTANT) ?
******************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui/QWidget>

#include "Game/Player/Party.h"

class Player
{
public:
  /* Player constructor object */
  Player();

  /* Annihilates a player object */
  ~Player(); 

private:
  /* Tick speed for the maps */
  int kMAPSPEED; //TODO: Change speed ?

  /* Current party (Sleuth) object */
  Party* sleuth;

  /* Party at the ship */
  Party* bearacks;

  /* Your position on the current map */
  int xpos;
  int ypos;

public:
  /* Removes a sleuth member by index by calling Party's remove function */
  bool removeSleuthMember(int index);

  /* Removes a sleuth member by name by calling Party's remove function */
  bool removeSleuthMember(QString value);

  /* Removes a bearacks member by index by calling Party's remove function */
  bool removeRacksMember(int index);

  /* Removes a bearacks member by name by calling Party's remove function */
  bool removeRacksMember(QString value);

  /* Gets # of frames in the main persons sprite multiplied by kMAPSPEED */
  int getSpeed(); 

  /* Gets the x-position on the map */
  int getXPos(); 

  /* Gets the y-position on the map */
  int getYPos();

  /* Sets the x-position on the map */
  void setXPos(int);

  /* Sets the y-position on the map */
  void setYPos(int);
};

#endif // PLAYER_H
