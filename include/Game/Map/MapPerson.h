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

  /* The surface classification, first layer of states 2D vector
   *  GROUND - is on a dirt like substance (where walking is possible) */
  enum SurfaceClassifier{GROUND = 0};

private:
  /* The direction of the player and the current movement direction */
  EnumDb::Direction direction;
  EnumDb::Direction movement;
  QList<EnumDb::Direction> movement_stack;

  /* Set of all states for person. 1st layer is surface (water, ground, etc)
   * and then 2nd layer is direction facing */
  QVector < QVector <MapState*> > states;

  /* The surface that the person is walking on */
  SurfaceClassifier surface;

  /* -------------------------- Constants ------------------------- */
  const static char kDIR_EAST;        /* The EAST direction for moving */
  const static char kDIR_NORTH;       /* The NORTH direction for moving */
  const static char kDIR_SOUTH;       /* The SOUTH direction for moving */
  const static char kDIR_UNKNOWN;     /* The unknown direction value */
  const static char kDIR_WEST;        /* The WEST direction for moving */
  const static char kTOTAL_DIRECTIONS; /* The total # of directions to move */
  const static char kTOTAL_SURFACES;   /* The total # of surfaces to walk on */
  
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
  /* Direction enumerator to/from integer converters */
  int dirToInt(EnumDb::Direction dir);
  EnumDb::Direction intToDir(int dir_index);

  /* Key press event reimplemented */
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);

  /* Sets the direction that the person is travelling in */
  void setDirection(EnumDb::Direction direction);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears all information from the class (including deleting necessary
   * pointers) */
  void clear();

  /* Returns the direction that this person is travelling in */
  EnumDb::Direction getDirection();

  /* Returns the surface that this person resides on */
  SurfaceClassifier getSurface();

  /* Sets a new state to add into the states list */
  bool setState(SurfaceClassifier surface, EnumDb::Direction direction, 
                                           MapState* state);

  /* Sets the surface that the person travels on */
  void setSurface(SurfaceClassifier surface);

  /* Updates the thing, based on the tick */
  void updateThing();

  /* Unsets a state, if it exists, to remove from the stack of states */
  void unsetState(SurfaceClassifier surface, EnumDb::Direction direction);
};

#endif // MAPPERSON_H
