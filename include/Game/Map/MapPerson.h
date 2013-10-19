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
//  Q_OBJECT

public:
  /* Constructor functions */
  MapPerson();
  MapPerson(int width, int height, QString name = "", 
            QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  virtual ~MapPerson();

  /* The surface classification, first layer of states 2D vector
   *  GROUND - is on a dirt like substance (where walking is possible) */
  enum SurfaceClassifier{GROUND = 0};

private:
  /* The animation speed for up and down, extrapolated for left and right */
  short animation_vertical;

  /* The direction of the player and the current movement direction */
  EnumDb::Direction direction;
  QList<EnumDb::Direction> movement_stack;

  /* Set of all states for person. 1st layer is surface (water, ground, etc)
   * and then 2nd layer is direction facing */
  QList< QList<Sprite*> > states;

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
  /* Add movement direction to the stack */
  void addDirection(EnumDb::Direction direction);

  /* Direction enumerator to/from integer converters */
  int dirToInt(EnumDb::Direction dir);
  EnumDb::Direction intToDir(int dir_index);

  /* Remove movement direction from the stack, if it exists */
  void removeDirection(EnumDb::Direction direction);

  /* Sets the direction that the person is travelling in */
  bool setDirection(EnumDb::Direction direction, bool set_movement = true);
  
  /* Starts and stops tile move. Relies on underlying logic for occurance */
  void tileMoveFinish();
  bool tileMoveStart(Tile* next_tile);
  
  /* Updates the animation of the map thing, based on the current state */
  void updateAnimation();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
//  void initiateThingUse(MapPerson* person);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the class descriptor, useful for casting */
  virtual QString classDescriptor();

  /* Clears all information from the class (including deleting necessary
   * pointers) */
  virtual void clear();

  /* Clear all movement from the stack. This is filled from add/remove
   * direction */
  void clearAllMovement();

   /* Gets the animation speed of the person */
  short getAnimationSpeed(); 

  /* Returns the direction that this person is travelling in */
  EnumDb::Direction getDirection();

  /* Returns the move request in the class (virtual reimplemented) */
  EnumDb::Direction getMoveRequest();
  
  /* Returns the surface that this person resides on */
  SurfaceClassifier getSurface();
  
  /* Reimplemented: the GL state initialization for the movement states in this
                    class. */
  bool initializeGl();
  
  /* Determines if there is an active move request (virtual reimplemented) */
  bool isMoveRequested();
  
  /* Key flush/press/release events handled */
  void keyFlush();
  void keyPress(QKeyEvent* event);
  void keyRelease(QKeyEvent* event);
  
  /* Sets the animation time for each frame */
  bool setAnimationSpeed(short frame_time);
  
  /* Set the tile to hook the map person to */
  bool setStartingTile(int section_id, Tile* new_tile, bool no_events = false);

  /* Sets a new state to add into the states list */
  bool setState(SurfaceClassifier surface, EnumDb::Direction direction, 
                                           Sprite* frames);

  /* Sets the surface that the person travels on */
  void setSurface(SurfaceClassifier surface);
 
  /* Updates the thing, based on the tick */
  virtual void updateThing(float cycle_time, Tile* next_tile);

  /* Unsets a state, if it exists, to remove from the stack of states */
  void unsetState(SurfaceClassifier surface, EnumDb::Direction direction);
};

#endif // MAPPERSON_H
