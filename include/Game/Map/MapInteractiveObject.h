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

  /* Enumerator for defining the available states 
   *  STATE_A - the initial (closed) state
   *  A_TO_B  - transitional state from A to B
   *  STATE_B - the secondary (open) state -> after interaction
   *  B_TO_A  - transitional state from B to A */
  enum StateOptions{STATE_A = 0, 
                    A_TO_B  = 1, 
                    STATE_B = 2, 
                    B_TO_A  = 3};

private:
  /* The current running state */
  StateOptions current_state;

  /* The list of available states */
  QList<MapState*> states;

  /* -------------------------- Constants ------------------------- */
  const static short kTOTAL_STATES; /* The total number of object states */

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
  /* Returns the class descriptor, useful for casting */
  virtual QString classDescriptor();

  /* Clears all information from the class (including deleting necessary
   * pointers) */
  virtual void clear();

  /* Interact with the thing (use key) */
  void interact();

  /* Reset back to state A */
  void reset();

  /* Sets the state, to be painted and used */
  bool setState(StateOptions selector, MapState* state);

  /* Updates the thing, based on the tick */
  virtual void updateThing(float cycle_time, Tile* next_tile);

  /* Unsets the tied state - this handles deletion */
  void unsetState(StateOptions selector);
};

#endif // MAPINTERACTIVEOBJECT_H
