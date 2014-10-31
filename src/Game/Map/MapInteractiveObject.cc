/*******************************************************************************
 * Class Name: MapInteractiveObject
 * Date Created: Dec 2 2012
 * Inheritance: MapThing
 * Description: This is the object is a specialization of the stock map thing
 *              that allows interaction from key presses by players in 
 *              nearby slots. Pressing the key will toggle the object and allow
 *              it to change states. These objects are unmovable and are of the
 *              typical interactive objects such as doors, chests, etc.
 *
 * TODO:
 * 1. Locked status of MIO??
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
  action_initiator = NULL;
  node_current = NULL;
  node_head = NULL;
  person_on = NULL;
  shifting_forward = true;
  time_elapsed = 0;
  time_return = kRETURN_TIME_UNUSED;
}

/* Constructor function */
MapInteractiveObject::MapInteractiveObject(uint16_t width, uint16_t height, 
                                           std::string name,
                                           std::string description, int id)
                    : MapThing(NULL, width, height, name, description, id)
{
  action_initiator = NULL;
  node_current = NULL;
  node_head = NULL;
  person_on = NULL;
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
  node->state = NULL;
  node->transition = NULL;
  node->passable = false;
  node->previous = NULL;
  node->next = NULL;

  /* Assign the node to the tail of the sequence */
  appendNode(node);
}

/* Appends the node to the tail of the sequence list */
void MapInteractiveObject::appendNode(StateNode* node)
{
  StateNode* tail_node = getTailNode();

  /* Assign the node to the tail of the sequence */
  node->previous = tail_node;
  if(tail_node == NULL)
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
  if(node_to_clear != NULL)
  {
    /* Delete state pointer, if relevant */
    if(node_to_clear->state != NULL)
      delete node_to_clear->state;
    node_to_clear->state = NULL;

    /* Delete transition pointer, if relevant */
    if(node_to_clear->transition != NULL)
      delete node_to_clear->transition;
    node_to_clear->transition = NULL;

    return true;
  }

  return false;
}

/* Returns the head state */
StateNode* MapInteractiveObject::getHeadNode()
{
  return node_head;
}

/* Returns the node based on the id, NULL if doesn't exist */
StateNode* MapInteractiveObject::getNode(int id)
{
  StateNode* selected = NULL;

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
  while(parser != NULL)
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
  if(tail == NULL)
    return tail;

  /* Otherwise, find the tail */
  while(tail->next != NULL)
    tail = tail->next;

  return tail;
}

/* Sets the current sequence of the node to the parent frames and resets the
 * pointers, where applicable */
void MapInteractiveObject::setParentFrames()
{
  /* Only proceed if the current node is non-NULL */
  if(node_current != NULL)
  {
    /* Determine if this is a transition sequence or a state sequence */
    if(node_current->state != NULL)
    {
      node_current->state->getSprite()->setAtFirst();
      //setFrames(node_current->state->getSprite(), false); // TODO: Repair
      //animate(0, true, false); // TODO: Remove?
    }
    else if(node_current->transition != NULL)
    {
      Sprite* transition = node_current->transition;
      node_current->transition->setAtFirst();
      transition->setDirectionForward();
      //MapThing::setFrames(transition, false); // TODO: Repair
      //animate(0, true, false); // TODO: Remove?

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
bool MapInteractiveObject::shiftNext()
{
  if(node_current != NULL && node_current->next != NULL)
  {
    /* Fire exit event */
    if(node_current->state != NULL)
      node_current->state->triggerExitEvent(action_initiator);

    /* Shift the pointer and update the frames */
    node_current = node_current->next;
    time_elapsed = 0;
    setParentFrames();

    /* Fire enter event */
    if(node_current->state != NULL)
      node_current->state->triggerEnterEvent(action_initiator);

    return true;
  }

  return false;
}

bool MapInteractiveObject::shiftPrevious()
{
  if(node_current != NULL && node_current->previous != NULL)
  {
    /* Fire exit event */
    if(node_current->state != NULL)
      node_current->state->triggerExitEvent(action_initiator);

    node_current = node_current->previous;
    time_elapsed = 0;
    setParentFrames();

    /* Fire enter event */
    if(node_current->state != NULL)
      node_current->state->triggerEnterEvent(action_initiator);

    return true;
  }

  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds IO information from the XML file. This will be
 *              virtually called by children where the child will deal with
 *              additional sets needed and then pass call to this. Works off
 *              reading the XML file in a strict manner. Passes call to parent
 *              after it is complete.
 *
 * Inputs: XmlData data - the read XML data
 *         int file_index - the index in the xml data where this detail begins
 *         int section_index - the map section index of the npc
 *         SDL_Renderer* renderer - the graphical rendering engine pointer
 *         std::string base_path - the base path for resources
 * Output: bool - status if successful
 */
bool MapInteractiveObject::addThingInformation(XmlData data, int file_index, 
                                               int section_index, 
                                               SDL_Renderer* renderer, 
                                               std::string base_path)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;

  /* Parse the identifier for setting the IO information */
  if(identifier == "inactive" && elements.size() == 1)
  {
    setInactiveTime(data.getDataInteger(&success));
  }
  else if(identifier == "states" && elements.size() > 1)
  {
    int index = std::stoi(data.getKeyValue(file_index + 1));
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
      if(modified_node != NULL)
      {
        /* Clear the node if it has the wrong data in it */
        if((elements[1] == "state" && modified_node->transition != NULL) || 
           (elements[1] == "transition" && modified_node->state != NULL))
          clearNode(index);

        /* Move forward with updating the given state/transiton sequence */
        if(elements[1] == "state" && elements.size() > 2)
        {
          /* Make the state if it doesn't exist */
          if(modified_node->state == NULL)
          {
            modified_node->state = new MapState(new Sprite(), event_handler);
            if(modified_node == node_current)
              setParentFrames();
          }

          /*--------------------- SPRITE INFO -----------------*/
          if(elements[2] == "sprite")
          {
            success &= modified_node->state->getSprite()->
                            addFileInformation(data, file_index + 3, 
                                               renderer, base_path);
          }
          /*--------------------- INTERACTION -----------------*/
          else if(elements[2] == "interaction")
          {
            success &= modified_node->state->setInteraction(
                                                  data.getDataString(&success));
          }
          /*------------------ ENTER EVENT -----------------*/
          else if(elements[2] == "enterevent")
          {
            if(event_handler != NULL)
            {
              Event event = event_handler->updateEvent(
                                 modified_node->state->getEnterEvent(), data, 
                                 file_index + 3, section_index);
              modified_node->state->setEnterEvent(event);
            }
          }
          /*------------------ EXIT EVENT -----------------*/
          else if(elements[2] == "exitevent")
          {
            if(event_handler != NULL)
            {
              Event event = event_handler->updateEvent(
                              modified_node->state->getExitEvent(), data, 
                              file_index + 3, section_index);
              modified_node->state->setExitEvent(event);
            }
          }
          /*------------------ USE EVENT -----------------*/
          else if(elements[2] == "useevent")
          {
            if(event_handler != NULL)
            {
              Event event = event_handler->updateEvent(
                              modified_node->state->getUseEvent(), data, 
                              file_index + 3, section_index);
              modified_node->state->setUseEvent(event);
            }
          }
          /*---------------- WALKOVER EVENT -----------------*/
          else if(elements[2] == "walkoverevent")
          {
            if(event_handler != NULL)
            {
              Event event = event_handler->updateEvent(
                              modified_node->state->getWalkoverEvent(), data, 
                              file_index + 3, section_index);
              modified_node->state->setWalkoverEvent(event);
            }
          }
        }
        /*--------------------- TRANSITION FRAMES -----------------*/
        else if(elements[1] == "transition" && elements.size() > 2)
        {
          /* Make the transition if it doesn't exist */
          if(modified_node->transition == NULL)
          {
            modified_node->transition = new Sprite();
            if(modified_node == node_current)
              setParentFrames();
          }
          
          /* If transition frames, parse the given sprite data */
          if(elements[2] == "sprite")
          {
            success &= modified_node->transition->
                            addFileInformation(data, file_index + 3, 
                                               renderer, base_path);
          }
        }

        /*--------------------- PASSABILITY -----------------*/
        if(elements.size() > 2 && elements[2] == "passable")
        {
          bool passable = data.getDataBool(&success);
          if(success)
            modified_node->passable = passable;
        }
      }
    }
  }

  return (success && MapThing::addThingInformation(data, file_index, 
                                                   section_index, renderer, 
                                                   base_path));
}

/* Returns the class descriptor, useful for casting */
std::string MapInteractiveObject::classDescriptor()
{
  return "MapInteractiveObject";
}

/* Clears all information from the class (including deleting necessary
 * pointers) */
void MapInteractiveObject::clear()
{
  /* Clears action initiator pointer */
  action_initiator = NULL;
  person_on = NULL;
  shifting_forward = true;
  time_elapsed = 0;
  time_return = kRETURN_TIME_UNUSED;
  
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

/* Interact with the thing (use key) */
bool MapInteractiveObject::interact(MapPerson* initiator)
{
  bool status = false;

  /* Only proceed if the node is available */
  if(node_current != NULL && node_current->state != NULL)
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
  if(node_current != NULL)
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

bool MapInteractiveObject::setStartingTile(uint16_t section_id, Tile* new_tile, 
                                           bool no_events)
{
  if(MapThing::setStartingTile(section_id, new_tile, no_events))
  {
    person_on = NULL;
    return true;
  }
  return false;
}

bool MapInteractiveObject::setState(MapState* state, bool passable)
{
  if(state != NULL && state->getSprite() != NULL)
  {
    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = state;
    node->transition = NULL;
    node->passable = passable;
    node->previous = NULL;
    node->next = NULL;

    /* Assign the node to the tail of the sequence */
    appendNode(node);

    return true;
  }

  return false;
}

bool MapInteractiveObject::setState(Sprite* transition, bool passable)
{
  if(transition != 0)
  {
    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = NULL;
    node->transition = transition;
    node->passable = passable;
    node->previous = NULL;
    node->next = NULL;

    /* Assign the node to the tail of the sequence */
    appendNode(node);

    return true;
  }

  return false;
}

/* Updates the thing, based on the tick */
void MapInteractiveObject::update(int cycle_time, Tile* next_tile)
{
  (void)next_tile;

  /* Animate the frames and determine if the frame has changed */
  bool frames_changed = animate(cycle_time, false, false);

  /* Only proceed if frames are changed and a transitional sequence  */
  if(frames_changed && node_current != NULL && node_current->transition != NULL)
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
  if(location != NULL)
  {
    if(person_on == NULL && location->isPersonSet() && 
       location->getPerson()->getID() == kPLAYER_ID)
    {
      person_on = location->getPerson();

      if(node_current != NULL && node_current->state != NULL)
      {
        /* Trigger walkover event, if valid */
        node_current->state->triggerWalkoverEvent(person_on);

        /* Trigger walk on interaction */
        if(node_current->state->getInteraction() == MapState::WALKON)
          shift();
      }
    }
    else if(person_on != NULL && !location->isPersonSet())
    {
      /* Trigger walk off interaction */
      if(node_current != NULL && node_current->state != NULL && 
         node_current->state->getInteraction() == MapState::WALKOFF)
        shift();

      person_on = NULL;
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
  while(node != NULL)
  {
    StateNode* temp_node = node;
    node = node->next;
    
    /* Proceed to delete all relevant data */
    if(temp_node->state != NULL)
      delete temp_node->state;
    else if(temp_node->transition != NULL)
      delete temp_node->transition;
    delete temp_node;
  }

  /* Clear the class data */
  node_current = NULL;
  node_head = NULL;
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
  person_on = NULL;
}
