/******************************************************************************
* Class Name: MapInteractiveObject
* Date Created: Dec 2 2012
* Inheritance: MapThing
* Description: This is the object is a specialization of the stock map thing
*              that allows interaction from key presses by players in 
*              nearby slots. Pressing the key will toggle the object and allow
*              it to change states. These objects are unmovable and are of the
*              typical interactive objects such as doors, chests, etc.
******************************************************************************/
#include "Game/Map/MapInteractiveObject.h"

/* Constant Implementation - see header file for descriptions */
const short MapInteractiveObject::kTOTAL_STATES = 3;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapInteractiveObject::MapInteractiveObject()
{
  initializeStates();
  clear();
}

/* Constructor function */
MapInteractiveObject::MapInteractiveObject(int width, int height, QString name, 
                                           QString description, int id)
{
  initializeStates();
  clear();

  setDescription(description);
  setHeight(height);
  setID(id);
  setName(name);
  setWidth(width);
}

/* Destructor function */
MapInteractiveObject::~MapInteractiveObject()
{
  clear();
  states.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/* Initializes the states stack to an empty set */
void MapInteractiveObject::initializeStates()
{
  states.clear();

  for(int i = 0; i < kTOTAL_STATES; i++)
    states.append(0);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* Returns the class descriptor, useful for casting */
QString MapInteractiveObject::classDescriptor()
{
  return "MapInteractiveObject";
}

/* Clears all information from the class (including deleting necessary
 * pointers) */
void MapInteractiveObject::clear()
{
  /* Clear states */
  for(int i = 0; i < kTOTAL_STATES; i++)
    unsetState((StateOptions)i);

  /* Clear class information */
  current_state = STATE_A;

  /* Parent cleanup */
  MapThing::clear();
}

/* Interact with the thing (use key) */
void MapInteractiveObject::interact()
{
  if(current_state == STATE_A && states[A_TO_B] != 0)
  {
    /* Determine if a state shift is possible */
    StateOptions new_state = STATE_A;
    if(states[A_TO_B] != 0)
      new_state = A_TO_B;
    else if(states[STATE_B] != 0)
      new_state = STATE_B;

    /* Execute state shift */
    if(new_state != STATE_A)
    {
      current_state = new_state;
      MapThing::setState(states[current_state], false);
      states[current_state]->getSprite()->setDirectionForward();
      states[current_state]->getSprite()->setAtFirst();
    }
  }
  else if(current_state == STATE_B)
  {
    /* If the return state is available, use it */
    if(states[A_TO_B] != 0)
    {
      current_state = B_TO_A;
      MapThing::setState(states[A_TO_B], false);
      states[A_TO_B]->getSprite()->setDirectionReverse();
      states[A_TO_B]->getSprite()->setAtFirst();
      states[A_TO_B]->getSprite()->shiftNext();
    }
    /* Otherwise, jump to state A again */
    else if(states[STATE_A] != 0)
    {
      current_state = STATE_A;
      MapThing::setState(states[current_state], false);
      states[current_state]->getSprite()->setDirectionForward();
      states[current_state]->getSprite()->setAtFirst();
    }
  }
}

/* Reset back to state A */
void MapInteractiveObject::reset()
{
  MapThing::unsetState(false);
  current_state = STATE_A;
  MapThing::setState(states[current_state], false);
  
  if(states[current_state] != 0)
    states[current_state]->getSprite()->setAtFirst();
}

/* Sets the state, to be painted and used */
bool MapInteractiveObject::setState(StateOptions selector, MapState* state)
{
  if(state != 0 && state->getSprite() != 0)
  {
    StateOptions new_selector = selector;

    /* Reset second transitional element back to other */
    if(selector == B_TO_A)
      new_selector = A_TO_B;

    /* Unset and reset the new state */
    unsetState(new_selector);
    states[new_selector] = state;

    /* Set the state in the parent class */
    if(current_state == selector || current_state == new_selector)
      reset();

    return true;
  }

  return false;
}

/* Updates the thing, based on the tick */
void MapInteractiveObject::updateThing(float cycle_time, Tile* next_tile)
{
  StateOptions access_state = current_state;
  if(access_state == B_TO_A)
    access_state = A_TO_B;
  bool changed = false;
  (void)next_tile;

  /* Get the sprite data */
  Sprite* thing_sprite = 0;
  if(states[access_state] != 0)
    thing_sprite = states[access_state]->getSprite();

  /* Animate the existing tile */
  animate(cycle_time, false, false);

  /* Check the states and if a change should occur */
  if(current_state == A_TO_B && thing_sprite != 0 && thing_sprite->isAtFirst())
  {
    changed = true;
    current_state = STATE_B;
  }
  else if(current_state == B_TO_A && thing_sprite != 0 &&
          (thing_sprite->getPosition() + 1) == thing_sprite->getSize())
  {
    changed = true;
    current_state = STATE_A;
  }

  /* If the state has been changed, update the appropriate sprites */
  if(changed)
  {
    MapThing::setState(states[current_state], false);
    if(states[current_state] != 0)
      states[current_state]->getSprite()->setAtFirst();
  }

}

/* Unsets the tied state - this handles deletion */
void MapInteractiveObject::unsetState(StateOptions selector)
{
  StateOptions new_selector = selector;

  /* Reset second transitional element back to other */
  if(selector == B_TO_A)
    new_selector = A_TO_B;

  /* Delete and reset the state */
  delete states[new_selector];
  states[new_selector] = 0;

  /* Clear the parent state, if it's set */
  if(current_state == selector || current_state == new_selector)
    reset();
}
