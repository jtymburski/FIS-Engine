/******************************************************************************
* Class Name: MapInteractiveObject
* Date Created: Oct 28 2012
* Inheritance: MapThing
* Description: This is the object is a specialization of the stock map thing
*              that allows interaction from key presses by players in 
*              nearby slots. Pressing the key will toggle the object and allow
*              it to change states. These objects are unmovable and are of the
*              typical interactive objects such as doors, chests, etc.
******************************************************************************/
#ifndef MAPINTERACTIVEOBJECT_H
#define MAPINTERACTIVEOBJECT_H

#include "Game/Map/MapPerson.h"
#include "Game/Map/MapState.h"
#include "Game/Map/MapThing.h"

class MapInteractiveObject : public MapThing
{
public:
  /* Constructor functions */
  MapInteractiveObject();
  MapInteractiveObject(int width, int height, QString name = "", 
                       QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  virtual ~MapInteractiveObject();

private:
  /* The action initiator person pointer - last one called */
  MapPerson* action_initiator;

  /* The nodes that indicate the available states and transitions */
  StateNode* node_current;
  StateNode* node_head;

  /* Shifting the nodes in the forward direction indicator */
  bool shifting_forward;

  /* This defines the time before returning to the previous state after no
   * activity */
  int time_elapsed;
  int time_return;

  /* -------------------------- Constants ------------------------- */
  const static short kRETURN_TIME_UNUSED; /* The unused time inidicator */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Appends the node to the tail of the sequence list */
  void appendNode(StateNode* node);

  /* Returns the head state */
  StateNode* getHeadNode();

  /* Returns the tail state */
  StateNode* getTailNode();

  /* Sets the current sequence of the node to the parent frames and resets the
   * pointers, where applicable */
  void setParentFrames();

  /* Shift the node sequence to next or previous */
  bool shiftNext();
  bool shiftPrevious();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the class descriptor, useful for casting */
  virtual QString classDescriptor();

  /* Clears all information from the class (including deleting necessary
   * pointers) */
  virtual void clear();

  /* Returns the inactive time before returning down the state path */
  int getInactiveTime();

  /* Initializes the GL context with all the state frames */
  bool initializeGl();

  /* Interact with the thing (use key) */
  bool interact(MapPerson* initiator);

  /* Reimplemented thing call - to if the interactive state can be walked on */
  bool isPassable();

  /* Reset back to head state */
  void reset();

  /* Sets the inactive time before returning down the state path (ms) */
  void setInactiveTime(int time);

  /* Sets the states, to be painted and used */
  bool setState(MapState* state);
  bool setState(Sprite* transition);

  /* Updates the thing, based on the tick */
  virtual void updateThing(float cycle_time, Tile* next_tile);

  /* Unsets all states - this handles deletion */
  void unsetStates();
};

#endif // MAPINTERACTIVEOBJECT_H
