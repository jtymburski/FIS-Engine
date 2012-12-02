/******************************************************************************
* Class Name: MapPerson
* Date Created: Oct 28 2012
* Inheritance: MapThing
* Description: The MapPerson class, this covers all Persons in the map
******************************************************************************/
#ifndef MAPPERSON_H
#define MAPPERSON_H

#include <QtGui/QWidget>

//#include "EnumDatabase.h" // TODO: Commented Out? [12-02-12]
#include "Game/Map/MapThing.h"
#include "Game/Player/Person.h"
#include "Game/Sprite.h"

class MapPerson : public MapThing
{
public:
  /* Constructor function */
  MapPerson();

  /* Destructor function */
  ~MapPerson();

private:
  /* The in-battle version of this person */
  Person battle_person;

  /* The current movement direction */
  Direction movement_direction;

  /* This persons name */
  QString name;

  /* The facing sprites at all directions */
  Sprite* north,south,east,west;

  /* The persons step length (used for speed essentially) */
  int step_length;

  /* Position on the map */
  int xpos,ypos;

  /* For if a person is facing certain directions, only one can be true 
     at a time */
  bool FACING_NORTH,FACING_EAST,FACING_SOUTH,FACING_WEST;

  /* For if a person is attempting to move certain directions */
  bool MOVE_NORTH,MOVE_EAST,MOVE_SOUTH,MOVE_WEST;


  /* For if a person is moving (for seamless sprite drawing) */
  bool MOVING_NORTH,MOVING_EAST,MOVING_SOUTH,MOVING_WEST;

  /* For east and west sprites being symmetric */
  bool MIRRORED;

  /* For if a person is actually rendering */
  bool RENDERING;

public:
  /* Sets the person to be facing east */
  void faceEast();

  /* Sets the person to be facing north */
  void faceNorth();

  /* Sets the person to be facing south */
  void faceSouth();

  /* Sets the person to be facing west */
  void faceWest();

  /* Evaluates the East Facing flag */
  bool isFacingEast();

  /* Evaluates the North Facing flag */
  bool isFacingNorth();

   /* Evaluates the South Facing flag */
  bool isFacingSouth();

  /* Evaluates the West Facing flag */
  bool isFacingWest();

  /* Evaluates the East Move flag */
  bool isMoveEast();

  /* Evaluates the North Move flag */
  bool isMoveNorth();

   /* Evaluates the South Move flag */
  bool isMoveSouth();

  /* Evaluates the West Move flag */
  bool isMoveWest();

  /* Evaluates the East moving flag */
  bool isMovingEast();

  /* Evaluates the North moving flag */
  bool isMovingNorth();

   /* Evaluates the South moving flag */
  bool isMovingSouth();

  /* Evaluates the West moving flag */
  bool isMovingWest();

  /* Gets the MIRRORED evaluation */
  bool isMirrored();

  /* Returns true if the person is moving */
  bool isMoving();

  /* Gets the RENDERING evaluation */
  bool isRendering();

  /* Returns true if the player is wall moving */
  bool isWallMoving();

  /* Sets the person to be moving east */
  void moveEast();

  /* Sets the person to be moving north */
  void moveNorth();

  /* Sets the person to be moving south */
  void moveSouth();

  /* Sets the person to be moving west */
  void moveWest();

  /* Sets the person to cease moving */
  void stopMoving();

  /* Gets the battle version of the person */
  Person* getBattlePerson();

  /* Gets the persons direction */
  Direction getDirection();

  /* Gets the name */
  QString getName();

  /* Gets the persons facing sprite */
  Sprite* getSprite();

  /* Gets the persons step length */
  int getStepLength();

  /* Gets the x position */
  int getX();

  /* Gets the y position */
  int getY();
};

#endif // MAPPERSON_H
