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
const short MapInteractiveObject::kRETURN_TIME_UNUSED = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapInteractiveObject::MapInteractiveObject() : MapThing()
{
  action_initiator = 0;
  node_current = 0;
  node_head = 0;
  person_on = 0;
  shifting_forward = true;
  time_elapsed = 0;
  time_return = kRETURN_TIME_UNUSED;

  clear();
}

/* Constructor function */
MapInteractiveObject::MapInteractiveObject(int width, int height, QString name, 
                                           QString description, int id)
                    : MapThing(0, width, height, name, description, id)
{
  action_initiator = 0;
  node_current = 0;
  node_head = 0;
  person_on = 0;
  shifting_forward = true;
  time_elapsed = 0;
  time_return = kRETURN_TIME_UNUSED;
}

/* Destructor function */
MapInteractiveObject::~MapInteractiveObject()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
   
/* Appends an empty node to the tail of the sequence list */
void MapInteractiveObject::appendEmptyNode()
{
  StateNode* node = new StateNode;

  /* Set the state parameters */
  node->state = 0;
  node->transition = 0;
  node->passable = false;
  node->previous = 0;
  node->next = 0;

  /* Assign the node to the tail of the sequence */
  appendNode(node);
}

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
  
/* Deletes all memory stored within the given node */
bool MapInteractiveObject::clearNode(int id)
{
  StateNode* node_to_clear = getNode(id);

  /* Check if the node is valid */
  if(node_to_clear != 0)
  {
    /* Delete state pointer, if relevant */
    if(node_to_clear->state != 0)
      delete node_to_clear->state;
    node_to_clear->state = 0;

    /* Delete transition pointer, if relevant */
    if(node_to_clear->transition != 0)
      delete node_to_clear->transition;
    node_to_clear->transition = 0;

    return true;
  }

  return false;
}

/* Returns the head state */
StateNode* MapInteractiveObject::getHeadNode()
{
  return node_head;
}

/* Returns the node based on the id, NULL if doeesn't exist */
StateNode* MapInteractiveObject::getNode(int id)
{
  StateNode* selected = 0;

  if(id >= 0 && id < getNodeLength())
  {
    selected = node_head;

    while(id > 0)
    {
      selected = selected->next;
      id--;
    }
  }

  return selected;
}

/* Returns the length of the node sequence */
int MapInteractiveObject::getNodeLength()
{
  StateNode* parser = node_head;
  int length = 0;

  /* Determine the length of the node sequence by parsing through it */
  while(parser != 0)
  {
    length++;
    parser = parser->next;
  }

  return length;
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
      transition->setDirectionForward();
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
 
bool MapInteractiveObject::shift()
{
  bool status = false;

  /* Do the interaction based on the direction travelling */
  if(shifting_forward)
  {
    if(!shiftNext())
    {
      shifting_forward = false;
      status |= shiftPrevious();
    }
    else
      status = true;
  }
  else
  {
    if(!shiftPrevious())
    {
      shifting_forward = true;
      status |= shiftNext();
    }
    else
      status = true;
  }

  return status;
}

/* Shift the node sequence to next or previous */
// TODO: Event handling
bool MapInteractiveObject::shiftNext()
{
  if(node_current != 0 && node_current->next != 0)
  {
    /* Fire exit event */
    if(node_current->state != 0)
      node_current->state->triggerExitEvent(action_initiator);

    /* Shift the pointer and update the frames */
    node_current = node_current->next;
    time_elapsed = 0;
    setParentFrames();

    /* Fire enter event */
    if(node_current->state != 0)
      node_current->state->triggerEnterEvent(action_initiator);

    return true;
  }

  return false;
}

// TODO: Event handling
bool MapInteractiveObject::shiftPrevious()
{
  if(node_current != 0 && node_current->previous != 0)
  {
    /* Fire exit event */
    if(node_current->state != 0)
      node_current->state->triggerExitEvent(action_initiator);

    node_current = node_current->previous;
    time_elapsed = 0;
    setParentFrames();

    /* Fire enter event */
    if(node_current->state != 0)
      node_current->state->triggerEnterEvent(action_initiator);

    return true;
  }

  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
 
/* Adds IO information from the XML file. Will be virtually re-called
 * by all children for proper operation */ // TODO - comment
// TODO: Opacity in state, all events in state
bool MapInteractiveObject::addThingInformation(XmlData data, int file_index, 
                                               int section_index)
{
  QList<QString> elements = data.getTailElements(file_index + 1);
  QString identifier = data.getElement(file_index + 1);
  bool success = true;

  /* Parse the identifier for setting the IO information */
  if(identifier == "inactive" && elements.size() == 1)
  {
    setInactiveTime(data.getDataInteger());
  }
  else if(identifier == "states")
  {
    int index = data.getKeyValue(file_index + 2).toInt();
    int length = getNodeLength();

    /* Only proceed if the index is legitimate */
    if((elements[1] == "state" || elements[1] == "transition") && index >= 0)
    {
      /* Fill up the node length to suffice for the given index */
      while(length <= index)
      {
        appendEmptyNode();
        length++;
      }

      /* Proceed to determine if it was changed */
      StateNode* modified_node = getNode(index);
      if(modified_node != 0)
      {
        if((elements[1] == "state" && modified_node->transition != 0) || 
           (elements[1] == "transition" && modified_node->state != 0))
          clearNode(index);

        /* Move forward with updating the given state/transiton sequence */
        if(elements[1] == "state" && modified_node != 0)
        {
          /* Make the state if it doesn't exist */
          if(modified_node->state == 0)
            modified_node->state = new MapState(0, event_handler);

          /* If frames, parse the string for path sprites */
          if(elements[2] == "frames")
          {
            Sprite* animation = new Sprite(data.getDataString());
            if(modified_node->state->setSprite(animation))
            {
              /* Update the current thing frames if changed */
              if(node_current == modified_node)
                setParentFrames();
            }
            else
            {
              delete animation;
              success = false;
            }
          }
          /* Update the interaction parser for the given thing */
          else if(elements[2] == "interaction")
          {
            success &= modified_node->state->setInteraction(
                                                  data.getDataString(&success));
          }
        }
        else if(elements[1] == "transition" && modified_node != 0)
        {
          /* If transition frames, parse the given sprite data */
          if(elements[2] == "frames")
          {
            Sprite* animation = new Sprite(data.getDataString());
            if(animation->getSize() > 0)
            {
              if(modified_node->transition != 0)
                delete modified_node->transition;
              modified_node->transition = animation;

              /* Update the current thing frames, if changed */
              if(node_current == modified_node)
                setParentFrames();
            }
            else
            {
              delete animation;
              success = false;
            }
          }
        }

        /* If passability setting set, execute */
        if(elements[2] == "passable")
        {
          bool passable = data.getDataBool(&success);
          if(success)
            modified_node->passable = passable;
        }
      }
    }
  }
 
  return (success && 
          MapThing::addThingInformation(data, file_index, section_index));
}

/* Returns the class descriptor, useful for casting */
QString MapInteractiveObject::classDescriptor()
{
  return "MapInteractiveObject";
}

/* Clears all information from the class (including deleting necessary
 * pointers) */
void MapInteractiveObject::clear()
{
  /* Clears action initiator pointer */
  action_initiator = 0;

  /* Clear states */
  unsetStates();

  /* Parent cleanup */
  MapThing::clear();
}
  
/* Returns the inactive time before returning down the state path */
int MapInteractiveObject::getInactiveTime()
{
  return time_return;
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
bool MapInteractiveObject::interact(MapPerson* initiator)
{
  bool status = false;

  /* Only proceed if the node is available */
  if(node_current != 0 && node_current->state != 0)
  {
    /* Set the action initiator for the state */
    action_initiator = initiator;

    /* Fire the use event, if it exists */
    status |= node_current->state->triggerUseEvent(initiator);

    /* Proceed to execute a use event, if it exists */
    if(node_current->state->getInteraction() == MapState::USE)
      status |= shift();
  }

  if(status)
    time_elapsed = 0;

  return status;
}
/* Reimplemented thing call - to if the interactive state can be walked on */
bool MapInteractiveObject::isPassable()
{
  if(node_current != 0)
    return node_current->passable;
  return true;
}

/* Reset back to head state */
void MapInteractiveObject::reset()
{
  node_current = node_head;
  shifting_forward = true;
  setParentFrames();
}
  
/* Sets the inactive time before returning down the state path (ms) */
void MapInteractiveObject::setInactiveTime(int time)
{
  if(time <= 0)
    time_return = kRETURN_TIME_UNUSED;
  else
    time_return = time;

  time_elapsed = 0;
}

bool MapInteractiveObject::setStartingTile(int section_id, Tile* new_tile, 
                                           bool no_events)
{
  if(MapThing::setStartingTile(section_id, new_tile, no_events))
  {
    person_on = 0;
    return true;
  }
  return false;
}

bool MapInteractiveObject::setState(MapState* state, bool passable, int id)
{
  if(state != 0 && state->getSprite() != 0)
  {
    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = state;
    node->transition = 0;
    node->passable = passable;
    node->previous = 0;
    node->next = 0;

    /* Assign the node to the tail of the sequence */
    appendNode(node);

    return true;
  }

  return false;
}

bool MapInteractiveObject::setState(Sprite* transition, bool passable, int id)
{
  if(transition != 0)
  {
    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = 0;
    node->transition = transition;
    node->passable = passable;
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
  (void)next_tile;

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
        time_elapsed = 0;
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
        time_elapsed = 0;
      }
    }
  }
  
  /* Do the walk on / walk off animation for the state */
  Tile* location = getTile();
  if(location != 0)
  {
    if(person_on == 0 && location->isPersonSet() && 
       location->getPerson()->getID() == kPLAYER_ID)
    {
      person_on = location->getPerson();

      if(node_current != 0 && node_current->state != 0)
      {
        /* Trigger walkover event, if valid */
        node_current->state->triggerWalkoverEvent(person_on);

        /* Trigger walk on interaction */
        if(node_current->state->getInteraction() == MapState::WALKON)
          shift();
      }
    }
    else if(person_on != 0 && !location->isPersonSet())
    {
      /* Trigger walk off interaction */
      if(node_current != 0 && node_current->state != 0 && 
         node_current->state->getInteraction() == MapState::WALKOFF)
        shift();

      person_on = 0;
    }
  }
  
  /* Determine if the cycle time has passed on activity response */
  if(time_return != kRETURN_TIME_UNUSED && node_current != node_head)
  {
    time_elapsed += cycle_time;
    if(time_elapsed > time_return)
    {
      shifting_forward = false;
      shiftPrevious();
    }
  }
}

/* Unsets the tied state - this handles deletion */
void MapInteractiveObject::unsetStates()
{
  StateNode* node = node_head;
  unsetFrames(false);
  
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
}

/*
 * Description: Unsets the starting tile location that is stored within the
 *              thing.
 *
 * Inputs: bool no_events - fire no events when unsetting
 * Output: none
 */
void MapInteractiveObject::unsetStartingTile(bool no_events)
{ 
  MapThing::unsetStartingTile(no_events);
  person_on = 0;
}
