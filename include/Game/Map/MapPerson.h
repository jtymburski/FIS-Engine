/******************************************************************************
* Class Name: MapPerson
* Date Created: Oct 28 2012
* Inheritance: MapThing
* Description: The MapPerson class. An addition on top of MapThing that 
*              expands the Thing into possible states to allow for walking
*              in multiple directions and on multiple surfaces. At present, 
*              it allows for the 4 directions (N,S,E,W) all on one surface
*              (Ground). Future expansion is available for other surfaces
*              such as water, flying, etc.
******************************************************************************/
#ifndef MAPPERSON_H
#define MAPPERSON_H

//#include <QDebug>
#include <QKeyEvent>

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
   *  NORTH - player facing up
   *  EAST  - player facing right
   *  SOUTH - player facing down
   *  WEST  - player facing left */
  enum MovementDirection{NORTH, EAST, SOUTH, WEST, NONE};

  /* The surface classification, first layer of states 2D vector
   *  GROUND - is on a dirt like substance (where walking is possible) */
  enum SurfaceClassifier{GROUND};

private:
  /* The direction of the player and the current movement direction */
  MovementDirection direction;
  MovementDirection movement;
  QList<MovementDirection> movement_stack;

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
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears all information from the class (including deleting necessary
   * pointers) */
  void clear();

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

  /* Updates the thing, based on the tick */
  void updateThing();

  /* Unsets a state, if it exists, to remove from the stack of states */
  void unsetState(SurfaceClassifier surface, MovementDirection direction);
};

#endif // MAPPERSON_H
