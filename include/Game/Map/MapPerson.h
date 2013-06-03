/******************************************************************************
* Class Name: MapPerson
* Date Created: Oct 28 2012
* Inheritance: MapThing
* Description: The MapPerson class, this covers all Persons in the map
******************************************************************************/
#ifndef MAPPERSON_H
#define MAPPERSON_H

#include "Game/Map/MapThing.h"

class MapPerson : public MapThing
{
public:
  /* Constructor functions */
  MapPerson();
  MapPerson(int width, int height, QString name = "", 
            QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  ~MapPerson();

  /* Handles second layer of states 2D vector
   * NORTH - player facing up
   * EAST  - player facing right
   * SOUTH - player facing down
   * WEST  - player facing left */
  enum MovementDirection{NORTH, EAST, SOUTH, WEST};

  /* The surface classification, first layer of states 2D vector
   * GROUND - is on a dirt like substance (where walking is possible) */
  enum SurfaceClassifier{GROUND};

private:
  /* The current movement direction */
  MovementDirection direction;

  /* Set of all states for person. 1st layer is surface (water, ground, etc)
   * and then 2nd layer is direction facing */
  QVector < QVector <MapState*> > states;

  /* The surface that the person is walking on */
  SurfaceClassifier surface;

  /* -------------------------- Constants ------------------------- */
  const static int kTOTAL_DIRECTIONS; /* The total # of directions to move */
  const static int kTOTAL_SURFACES;   /* The total # of surfaces to walk on */
  
/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Initializes the states stack to an empty set */
  void initializeStates();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the direction that this person is travelling in */
  MovementDirection getDirection();

  /* Returns the surface that this person resides on */
  SurfaceClassifier getSurface();

  /* Sets the direction that the person is travelling in */
  void setDirection(MovementDirection direction);

  /* Sets a new state to add into the states list */
  bool setState(SurfaceClassifier surface, MovementDirection direction, 
                                           MapState* state);

  /* Sets the surface that the person travels on */
  void setSurface(SurfaceClassifier surface);

  /* Unsets a state, if it exists, to remove from the stack of states */
  void unsetState(SurfaceClassifier surface, MovementDirection direction);
};

#endif // MAPPERSON_H
