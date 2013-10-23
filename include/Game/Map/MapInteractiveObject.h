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
  //enum StateOptions{STATE_A = 0, 
  //                  A_TO_B  = 1, 
  //                  STATE_B = 2, 
  //                  B_TO_A  = 3};

private:
  /* The nodes that indicate the available states and transitions */
  StateNode* node_current;
  StateNode* node_head;

  /* The current running state */
  //StateOptions current_state;

  /* Shifting the nodes in the forward direction indicator */
  bool shifting_forward;

  /* The list of available states */
  //QList<MapState*> states;

  /* -------------------------- Constants ------------------------- */
  //const static short kTOTAL_STATES; /* The total number of object states */

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

  /* Initializes the states stack to an empty set */
  //void initializeStates();

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

  /* Initializes the GL context with all the state frames */
  bool initializeGl();

  /* Interact with the thing (use key) */
  bool interact(MapPerson* initiator);

  /* Reset back to head state */
  void reset();

  /* Sets the states, to be painted and used */
  bool setState(MapState* state);
  bool setState(Sprite* transition);

  /* Updates the thing, based on the tick */
  virtual void updateThing(float cycle_time, Tile* next_tile);

  /* Unsets all states - this handles deletion */
  void unsetStates();
};

#endif // MAPINTERACTIVEOBJECT_H
