/******************************************************************************
* Class Name: MapPerson
* Date Created: Dec 2 2012
* Inheritance: MapThing
* Description: The MapPerson class, this covers all Persons in the map
******************************************************************************/
#include "Game/Map/MapPerson.h"

/* Constructor function */
MapPerson::MapPerson()
{
}

/* Destructor function */
MapPerson::~MapPerson()
{
}

/* Sets the person to be facing east */
void MapPerson::faceEast()
{  
}

/* Sets the person to be facing north */
void MapPerson::faceNorth()
{
}

/* Sets the person to be facing south */
void MapPerson::faceSouth()
{
}

/* Sets the person to be facing west */
void MapPerson::faceWest()
{
}

/* Evaluates the East Facing flag */
bool MapPerson::isFacingEast()
{
    return FACING_EAST;
}

/* Evaluates the North Facing flag */
bool MapPerson::isFacingNorth()
{
    return FACING_NORTH;
}

 /* Evaluates the South Facing flag */
bool MapPerson::isFacingSouth()
{
    return FACING_SOUTH;
}

/* Evaluates the West Facing flag */
bool MapPerson::isFacingWest()
{
    return FACING_WEST;
}

/* Evaluates the East Move flag */
bool MapPerson::isMoveEast()
{
    return MOVE_EAST;
}

/* Evaluates the North Move flag */
bool MapPerson::isMoveNorth()
{
    return MOVE_NORTH;
}

 /* Evaluates the South Move flag */
bool MapPerson::isMoveSouth()
{
    return MOVE_SOUTH;
}

/* Evaluates the West Move flag */
bool MapPerson::isMoveWest()
{
    return MOVE_WEST;
}

/* Evaluates the East moving flag */
bool MapPerson::isMovingEast()
{
    return MOVING_EAST;
}

/* Evaluates the North moving flag */
bool MapPerson::isMovingNorth()
{
    return MOVING_NORTH;
}

 /* Evaluates the South moving flag */
bool MapPerson::isMovingSouth()
{
    return MOVING_SOUTH;
}

/* Evaluates the West moving flag */
bool MapPerson::isMovingWest()
{
    return MOVING_WEST;
}

/* Gets the MIRRORED evaluation */
bool MapPerson::isMirrored()
{
    return MIRRORED;
}

/* Returns true if the person is moving */
bool MapPerson::isMoving()
{
    if(MOVING_EAST || MOVING_NORTH || MOVING_SOUTH || MOVING_WEST)
        return TRUE;
    else
        return FALSE;
}

/* Gets the RENDERING evaluation */
bool MapPerson::isRendering()
{
    return RENDERING;
}

/* Returns true if the player is wall moving */
bool MapPerson::isWallMoving()
{
    return TRUE;
}

/* Sets the person to be moving east */
void MapPerson::moveEast()
{
}

/* Sets the person to be moving north */
void MapPerson::moveNorth()
{
}

/* Sets the person to be moving south */
void MapPerson::moveSouth()
{
}

/* Sets the person to be moving west */
void MapPerson::moveWest()
{
}

/* Sets the person to cease moving */
void MapPerson::stopMoving()
{
}

/* Gets the battle version of the person */
Person* MapPerson::getBattlePerson()
{
    //return battle_person;
}

/* Gets the persons direction */
Direction MapPerson::getDirection()
{
    return movement_direction;
}

/* Gets the name */
QString MapPerson::getName()
{
    return name;
}

/* Gets the persons facing sprite */
Sprite* MapPerson::getSprite()
{
    //return;
}

/* Gets the persons step length */
int MapPerson::getStepLength()
{
    return step_length;
}

/* Gets the x position */
int MapPerson::getX()
{
    return xpos;
}

/* Gets the y position */
int MapPerson::getY()
{
    return ypos;
}
