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
//const short MapInteractiveObject::kTOTAL_STATES = 3;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapInteractiveObject::MapInteractiveObject() : MapThing()
{
  node_current = 0;
  node_head = 0;
  shifting_forward = true;

  clear();
}

/* Constructor function */
MapInteractiveObject::MapInteractiveObject(int width, int height, QString name, 
                                           QString description, int id)
                    : MapThing(0, width, height, name, description, id)
{
  node_current = 0;
  node_head = 0;
  shifting_forward = true;
}

/* Destructor function */
MapInteractiveObject::~MapInteractiveObject()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/* Appends the node to the tail of the sequence list */
void MapInteractiveObject::appendNode(StateNode* node)
{
  StateNode* tail_node = getTailNode();

  /* Assign the node to the tail of the sequence */
  node->previous = tail_node;
  if(tail_node == 0)
  {
    node_head = node;
    node_current = node_head;
    shifting_forward = true;
    setParentFrames();
  }
  else
    tail_node->next = node;
}

/* Returns the head state */
StateNode* MapInteractiveObject::getHeadNode()
{
  return node_head;
}

/* Returns the tail state */
StateNode* MapInteractiveObject::getTailNode()
{
  StateNode* tail = node_head;

  /* If no nodes have been set, return null */
  if(tail == 0)
    return tail;

  /* Otherwise, find the tail */
  while(tail->next != 0)
    tail = tail->next;

  return tail;
}

/* Initializes the states stack to an empty set */
//void MapInteractiveObject::initializeStates()
//{
//  states.clear();
//
//  for(int i = 0; i < kTOTAL_STATES; i++)
//    states.append(0);
//}
  
/* Sets the current sequence of the node to the parent frames and resets the
 * pointers, where applicable */
void MapInteractiveObject::setParentFrames()
{
  /* Only proceed if the current node is non-NULL */
  if(node_current != 0)
  {
    /* Determine if this is a transition sequence or a state sequence */
    if(node_current->state != 0)
    {
      node_current->state->getSprite()->setAtFirst();
      setFrames(node_current->state->getSprite(), false);
      animate(0, true, false);
    }
    else if(node_current->transition != 0)
    {
      Sprite* transition = node_current->transition;
      MapThing::setFrames(transition, false);
      animate(0, true, false);

      /* Treat the sprite sequence according to the order, if it's in reverse
       * or not */
      if(shifting_forward)
      {
        transition->setDirectionForward();
      }
      else
      {
        transition->setDirectionReverse();
        transition->shiftNext();
      }
    }
  }
}
  
/* Shift the node sequence to next or previous */
// TODO: Event handling
bool MapInteractiveObject::shiftNext()
{
  if(node_current != 0 && node_current->next != 0)
  {
    node_current = node_current->next;
    setParentFrames();

    return true;
  }

  return false;
}

// TODO: Event handling
bool MapInteractiveObject::shiftPrevious()
{
  if(node_current != 0 && node_current->previous != 0)
  {
    node_current = node_current->previous;
    setParentFrames();

    return true;
  }

  return false;
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
  unsetStates();

  /* Parent cleanup */
  MapThing::clear();
}

bool MapInteractiveObject::initializeGl()
{
  StateNode* node = node_head;
  bool status = true;

  while(node != 0)
  {
    if(node->state != 0)
      status &= node->state->initializeGl();
    else if(node->transition != 0)
      status &= node->transition->initializeGl();

    node = node->next;
  }
  
  return status;
}

/* Interact with the thing (use key) */
// TODO: Add events to this sequence since it can cause interaction
// TODO: Fix how the sequence works with the use key at the end of the sequence
// with multiple states.
bool MapInteractiveObject::interact(MapPerson* initiator)
{
  /* Only proceed if the node is available */
  if(node_current != 0 && node_current->state != 0 && 
     node_current->state->getInteraction() == MapState::USE)
  {
    /* Do the interaction based on the direction travelling */
    if(shifting_forward)
    {
      if(node_current->next != 0)
      {
        shiftNext();
      }
      else if(node_current->previous != 0)
      {
        shifting_forward = false;
        shiftPrevious();
      }
      
    }
    else
    {
      if(node_current->previous != 0)
      {
        shiftPrevious();
      }
      else if(node_current->next != 0)
      {
        shifting_forward = true;
        shiftNext();
      }
    }

    return true;
  }
}

/* Reset back to head state */
void MapInteractiveObject::reset()
{
  node_current = node_head;
  shifting_forward = true;
  setParentFrames();
}

/* Sets the state, to be painted and used */
//bool MapInteractiveObject::setState(StateOptions selector, MapState* state)
//{
//  if(state != 0 && state->getSprite() != 0)
//  {
//    StateOptions new_selector = selector;
//
//    /* Reset second transitional element back to other */
//    if(selector == B_TO_A)
//      new_selector = A_TO_B;
//
//    /* Unset and reset the new state */
//    unsetState(new_selector);
//    states[new_selector] = state;
//
//    /* Set the state in the parent class */
//    if(current_state == selector || current_state == new_selector)
//      reset();
//
//    return true;
//  }
//
//  return false;
//}

bool MapInteractiveObject::setState(MapState* state)
{
  if(state != 0 && state->getSprite() != 0)
  {
    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = state;
    node->transition = 0;
    node->previous = 0;
    node->next = 0;

    /* Assign the node to the tail of the sequence */
    appendNode(node);

    return true;
  }

  return false;
}

bool MapInteractiveObject::setState(Sprite* transition)
{
  if(transition != 0)
  {
    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = 0;
    node->transition = transition;
    node->previous = 0;
    node->next = 0;

    /* Assign the node to the tail of the sequence */
    appendNode(node);

    return true;
  }

  return false;
}

/* Updates the thing, based on the tick */
void MapInteractiveObject::updateThing(float cycle_time, Tile* next_tile)
{
  /* Animate the frames and determine if the frame has changed */
  bool frames_changed = animate(cycle_time, false, false);

  /* Only proceed if frames are changed and a transitional sequence  */
  if(frames_changed && node_current != 0 && node_current->transition != 0)
  {
    if(node_current->transition->isDirectionForward() && 
       node_current->transition->isAtFirst())
    {
      /* Try and shift to the next state. If fails, re-animate transition */
      if(!shiftNext())
      {
        node_current->transition->setDirectionReverse();
        shifting_forward = false;
      }
    }
    else if(!node_current->transition->isDirectionForward() && 
            node_current->transition->isAtEnd())
    {
      /* Try and shift to the previous state. If fails, re-animate transition */
      if(!shiftPrevious())
      {
        node_current->transition->setDirectionForward();
        shifting_forward = true;
      }
    }
  }
}

/* Unsets the tied state - this handles deletion */
void MapInteractiveObject::unsetStates()
{
  StateNode* node = node_head;

  /* Proceed to delete all nodes */
  while(node != 0)
  {
    StateNode* temp_node = node;
    node = node->next;
    
    /* Proceed to delete all relevant data */
    if(temp_node->state != 0)
      delete temp_node->state;
    else if(temp_node->transition != 0)
      delete temp_node->transition;
    delete temp_node;
  }

  /* Clear the class data */
  node_current = 0;
  node_head = 0;
  unsetFrames(false);
}
