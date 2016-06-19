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
 * 1. Possibly optimize the walkon/walkoff in update. Seems inefficient to
 *    constantly check when it could be triggered by a walk on/walk off event
 *    (Possibly built into the thing itself?)
 ******************************************************************************/
#include "Game/Map/MapInteractiveObject.h"

/* Constant Implementation - see header file for descriptions */
const short MapInteractiveObject::kRETURN_TIME_UNUSED = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Base constructor function. No parameters. Initializes all
 *              parameters blank.
 *
 * Inputs: none
 */
MapInteractiveObject::MapInteractiveObject() : MapThing()
{
  action_initiator = nullptr;
  lock_struct = EventSet::createBlankLocked();
  node_current = nullptr;
  node_head = nullptr;
  nodes_delete = true;
  person_on = nullptr;
  shifting_forward = true;
  time_elapsed = 0;
  time_return = kRETURN_TIME_UNUSED;
  time_valid = false;
}

/*
 * Description: ID based constructor function. Sets up an interactive object
 *              with the base introduction parameters.
 *
 * Inputs: int id - the id of the MIO
 *         std::string name - the name of the MIO
 *         std::string description - the description of the MIO
 */
MapInteractiveObject::MapInteractiveObject(int id, std::string name,
                                           std::string description)
                    : MapThing(id, name, description)
{
  action_initiator = nullptr;
  node_current = nullptr;
  node_head = nullptr;
  person_on = nullptr;
  shifting_forward = true;
  time_elapsed = 0;
  time_return = kRETURN_TIME_UNUSED;
}

/*
 * Description: Destructor function
 */
MapInteractiveObject::~MapInteractiveObject()
{
  unsetMatrix();
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Appends an empty node onto the back of the stack of states.
 *
 * Inputs: none
 * Output: none
 */
void MapInteractiveObject::appendEmptyNode()
{
  StateNode* node = new StateNode;

  /* Set the state parameters */
  node->state = nullptr;
  node->transition = nullptr;
  node->previous = nullptr;
  node->next = nullptr;

  /* Assign the node to the tail of the sequence */
  appendNode(node);
}

/*
 * Description: Appends a node passed in to the end of the list of states
 *
 * Inputs: StateNode* node - node pointer to append
 * Output: none
 */
void MapInteractiveObject::appendNode(StateNode* node)
{
  StateNode* tail_node = getTailNode();

  /* Assign the node to the tail of the sequence */
  node->previous = tail_node;
  if(tail_node == nullptr)
  {
    node_head = node;
    node_current = node_head;
    shifting_forward = true;
    setParentFrames();
  }
  else
    tail_node->next = node;
}

/*
 * Description: Removes a node at a given ID and clears the memory for said
 *              node, which represents a state in the stack.
 *
 * Inputs: int id - id corresponding to a state in the stack
 * Output: bool - true if the node was successfully cleared
 */
bool MapInteractiveObject::clearNode(int id)
{
  StateNode* node_to_clear = getNode(id);

  /* Check if the node is valid */
  if(node_to_clear != nullptr)
  {
    /* Delete state pointer, if relevant */
    if(node_to_clear->state != nullptr)
      delete node_to_clear->state;
    node_to_clear->state = nullptr;

    /* Delete transition pointer, if relevant */
    if(node_to_clear->transition != nullptr)
      delete node_to_clear->transition;
    node_to_clear->transition = nullptr;

    return true;
  }

  return false;
}

/*
 * Description: Returns the node based on the ID. NULL if doesn't exist.
 *
 * Inputs: int id - id corresponding to a state in the stack
 * Output: StateNode* - pointer to the node at the ID. NULL if none
 */
StateNode* MapInteractiveObject::getNode(int id)
{
  StateNode* selected = nullptr;

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

/*
 * Description: Returns the length of the stack of states, as part of the MIO.
 *
 * Inputs: none
 * Output: int - integer representing the number of states
 */
int MapInteractiveObject::getNodeLength()
{
  StateNode* parser = node_head;
  int length = 0;

  /* Determine the length of the node sequence by parsing through it */
  while(parser != nullptr)
  {
    length++;
    parser = parser->next;
  }

  return length;
}

/*
 * Description: Returns the state node at the tail of the stack (last ID)
 *
 * Inputs: none
 * Output: StateNode* - the node pointer for the tail state
 */
StateNode* MapInteractiveObject::getTailNode()
{
  StateNode* tail = node_head;

  /* If no nodes have been set, return null */
  if(tail == nullptr)
    return tail;

  /* Otherwise, find the tail */
  while(tail->next != nullptr)
    tail = tail->next;

  return tail;
}

/*
 * Description: Handler interact to shift the states
 *
 * Inputs: none
 * Output: none
 */
void MapInteractiveObject::handlerInteract()
{
  if(!isLocked())
  {
    if(shift())
    {
      EventSet::unlockUsed(lock_struct);
    }
  }
}

/*
 * Description: Handles the connection to the parent class (thing matrix) and
 *              connecting it to the correct state, based on the current ptr.
 *              It will also set the animation back to the main frames.
 *
 * Inputs: none
 * Output: none
 */
void MapInteractiveObject::setParentFrames()
{
  /* Only proceed if the current node is non-NULL */
  if(node_current != nullptr)
  {
    /* Determine if this is a transition sequence or a state sequence */
    if(node_current->state != nullptr)
    {
      setMatrix(node_current->state->getMatrix());
      animate(0, true, false);
    }
    else if(node_current->transition != nullptr)
    {
      setMatrix(node_current->transition);
      if(base == nullptr)
        node_current->transition->setDirectionForward();
      else
        base_control->forward = true;
      animate(0, true, false);

      /* Treat the sprite sequence according to the order, if it's in reverse
       * or not */
      if(!shifting_forward)
      {
        if(base == nullptr)
        {
          node_current->transition->setDirectionReverse();
          node_current->transition->shiftNext();
        }
        else
        {
          base_control->forward = false;
          base_control->curr_frame = base_control->num_frames - 1;
        }
      }
    }
  }
}

/*
 * Description: Shifts the current state node to the next one, based on the
 *              direction designated in the class. Passes the call to
 *              shiftNext() or shiftPrevious(), as applicable.
 *
 * Inputs: none
 * Output: bool - true if the shift was successful
 */
bool MapInteractiveObject::shift()
{
  bool status = false;

  /* Do the interaction based on the direction travelling */
  if(shiftAvailable())
  {
    if(shifting_forward)
      shiftNext();
    else
      shiftPrevious();
    status = true;
  }

  return status;
}

/*
 * Description: Determines if a shift call is available (assuming it is
 *              unlocked). Merely checks if there is a next state available
 *
 * Inputs: none
 * Output: bool - true if the shift call will succeed
 */
bool MapInteractiveObject::shiftAvailable()
{
  bool can_shift = false;

  if(node_current != nullptr)
  {
    /* Determine if directional reverse is required */
    if(node_current->next != nullptr || node_current->previous != nullptr)
    {
      /* Determine direction */
      if(shifting_forward && node_current->next == nullptr)
      {
        shifting_forward = false;
      }
      else if(!shifting_forward && node_current->previous == nullptr)
      {
        shifting_forward = true;
      }

      /* There is another node so a shift is permitted */
      can_shift = true;
    }
  }

  return can_shift;
}

/*
 * Description: Shifts to the next state in the linear linked list. This shifts
 *              to a node with a greater ID (right). This also handles event
 *              triggers and animations.
 *
 * Inputs: none
 * Output: bool - true if the state has been shifted
 */
bool MapInteractiveObject::shiftNext()
{
  if(node_current != nullptr && node_current->next != nullptr)
  {
    /* Fire exit event */
    if(node_current->state != nullptr)
      node_current->state->triggerExitEvent(action_initiator, this);

    /* Shift the pointer and update the frames */
    node_current = node_current->next;
    time_elapsed = 0;
    setParentFrames();

    /* Fire enter event */
    if(node_current->state != nullptr)
      node_current->state->triggerEnterEvent(action_initiator, this);

    return true;
  }

  return false;
}

/*
 * Description: Shifts to the previous state in the linear linked list. This
 *              shifts to a node with a lesser ID (left). This also handles
 *              event triggers and animations.
 *
 * Inputs: none
 * Output: bool - true if the state has been shifted
 */
bool MapInteractiveObject::shiftPrevious()
{
  if(node_current != nullptr && node_current->previous != nullptr)
  {
    /* Fire exit event */
    if(node_current->state != nullptr)
      node_current->state->triggerExitEvent(action_initiator, this);

    node_current = node_current->previous;
    time_elapsed = 0;
    setParentFrames();

    /* Fire enter event */
    if(node_current->state != nullptr)
      node_current->state->triggerEnterEvent(action_initiator, this);

    return true;
  }
  return false;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Can the tile be set with the passed frame. Fails if there is
 *              already a interactive object set on said tile
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the sprite frames pointer to set in the tile
 *         bool avoid_player - true that canSet will return false if 0 render
 *                             player is on location. default false
 * Output: bool - true if the set was successful
 */
bool MapInteractiveObject::canSetTile(Tile* tile, TileSprite* frames,
                                      bool avoid_player)
{
  if(tile != nullptr)
  {
    /* Check if player is there */
    bool player = false;
    if(avoid_player && frames->getRenderDepth())
    {
      MapThing* person = (MapThing*)tile->getPerson(frames->getRenderDepth());
      if(person != nullptr && person->getID() == kPLAYER_ID)
        player = true;
    }

    /* Final return */
    return (!tile->isIOSet(frames->getRenderDepth()) && !player);
  }
  return false;
}

/*
 * Description: Checks if there is data to save for the particular IO. This
 *              is virtualized for all children
 *
 * Inputs: none
 * Output: bool - true if save call will result in text
 */
bool MapInteractiveObject::isDataToSave()
{
  /* Check parent first */
  if(MapThing::isDataToSave())
    return true;

  /* Check current state */
  if(node_head != node_current)
    return true;

  /* Check shifting forward state - only valid if not on head */
  //if(!shifting_forward)
  //  return true;

  /* Check the elapsed return time status (elapsed) */
  if(isInactiveTimeValid() && time_elapsed > 0)
    return true;

  /* Check states */
  StateNode* node_parse = node_head;
  while(node_parse != nullptr)
  {
    if(node_parse->state != nullptr && node_parse->state->isDataToSave())
      return true;
    node_parse = node_parse->next;
  }

  /* Check locked status */
  if(EventSet::isDataToSave(lock_struct))
    return true;

  return false;
}

/*
 * Description: Checks if a move is allowed from the current IO main
 *              tile to the next tile that it is trying to move to. This
 *              handles the individual calculations for a single tile; used
 *              by the isMoveAllowed() function.
 *
 * Inputs: Tile* previous - the tile moving from
 *         Tile* next - the next tile moving to
 *         uint8_t render_depth - the rendering depth, in the stack
 *         Direction move_request - the direction moving
 * Output: bool - returns if the move is allowed.
 */
bool MapInteractiveObject::isTileMoveAllowed(Tile* previous, Tile* next,
                                   uint8_t render_depth, Direction move_request)
{
  bool move_allowed = true;

  /* If the next tile is NULL, move isn't allowed */
  if(next == nullptr)
    move_allowed = false;

  /* Check if the thing can move there */
  if(move_allowed)
  {
    if(render_depth == 0)
    {
      if(!previous->getPassabilityExiting(move_request) ||
         !next->getPassabilityEntering(move_request) ||
         next->isIOSet(render_depth))
      {
        move_allowed = false;
      }
    }
    else if(next->getStatus() == Tile::OFF ||
            next->isIOSet(render_depth))
    {
      move_allowed = false;
    }
  }

  return move_allowed;
}

/*
 * Description: Saves the data for the IO. This does not include the IO
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         const bool &save_event - true to save the base event set (thing)
 * Output: none
 */
bool MapInteractiveObject::saveData(FileHandler* fh, const bool &save_event)
{
  bool success = true;

  /* Parent property saves */
  success &= MapThing::saveData(fh, save_event);

  /* Property change saves */
  if(changed)
  {
    /* Inactive time */
    fh->writeXmlData("inactive", getInactiveTime());
  }

  /* States */
  int node_index = 0;
  int node_save_index = 0;
  StateNode* node_parse = node_head;
  bool overall = false;
  while(node_parse != nullptr)
  {
    /* Node index - find */
    if(node_index > 0 && node_parse == node_current)
      node_save_index = node_index;

    /* Node state event information */
    if(node_parse->state != nullptr)
    {
      if(node_parse->state->isDataToSave())
      {
        /* If overall state wrapper hasn't been written yet, write it first */
        if(!overall)
        {
          fh->writeXmlElement("states");
          overall = true;
        }

        /* Write state differential */
        fh->writeXmlElement("state", "id", std::to_string(node_index));
        node_parse->state->saveData(fh);
        fh->writeXmlElementEnd();
      }
    }

    /* Increment index */
    node_index++;
    node_parse = node_parse->next;
  }
  if(overall)
    fh->writeXmlElementEnd();

  /* State - Current */
  if(node_save_index > 0)
  {
    fh->writeXmlData("stateid", node_save_index);

    /* Reverse shift status */
    if(!shifting_forward)
      fh->writeXmlData("shiftforward", shifting_forward);
  }

  /* Locked status */
  success &= EventSet::saveLocked(fh, lock_struct);

  /* Time elapsed status for return */
  if(isInactiveTimeValid() && time_elapsed > 0)
    fh->writeXmlData("timeelapsed", time_elapsed);

  return success;
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects. If it fails, it resets the pointers.
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the sprite frames pointer to set in the tile
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
bool MapInteractiveObject::setTile(Tile* tile, TileSprite* frames,
                                   bool no_events)
{
  (void)no_events;
  uint8_t render_depth = frames->getRenderDepth();

  /* Attempt and set the tile */
  if(tile != nullptr)
    return tile->setIO(this, render_depth);

  return false;
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects with regards to finishing a tile move.
 *
 * Inputs: Tile* old_tile - the tile the object was on previously
 *         Tile* new_tile - the tile the object is moving to
 *         uint8_t render_depth - the depth the frame is rendered on this tile
 *         bool reverse_last - if the last start should be reversed
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
void MapInteractiveObject::setTileFinish(Tile* old_tile, Tile* new_tile,
                                       uint8_t render_depth, bool reverse_last,
                                       bool no_events)
{
  (void)no_events;

  if(reverse_last)
    new_tile->unsetIO(render_depth);
  else
    old_tile->unsetIO(render_depth);
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects with regards to beginning a tile move. If it
 *              fails, it resets the pointers back to the original tile.
 *
 * Inputs: Tile* old_tile - the tile the object was on previously
 *         Tile* new_tile - the tile the object is moving to
 *         uint8_t render_depth - the depth the frame is rendered on this tile
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
bool MapInteractiveObject::setTileStart(Tile* old_tile, Tile* new_tile,
                                        uint8_t render_depth, bool no_events)
{
  (void)no_events;
  (void)old_tile;

  /* Attempt and set the tile */
  if(new_tile != nullptr)
    return new_tile->setIO(this, render_depth);

  return false;
}

/*
 * Description: Unsets the tile corresponding to the matrix at the x and y
 *              coordinate. However, since this is an private function, it does
 *              not confirm that the X and Y are in the valid range. Must be
 *              checked or results are unknown. This will unset the thing from
 *              the tile corresponding to the frame and the tile from the frame.
 *
 * Inputs: uint32_t x - the x coordinate of the frame (horizontal)
 *         uint32_t y - the y coordinate of the frame (vertical)
 *         bool no_events - should events trigger with the change?
 * Output: none
 */
void MapInteractiveObject::unsetTile(uint32_t x, uint32_t y, bool no_events)
{
  (void)no_events;
  SpriteMatrix* sprite_set = getMatrix();
  uint8_t render_depth = sprite_set->at(x, y)->getRenderDepth();

  /* Remove from main tile, if applicable */
  tile_main[x][y]->unsetIO(render_depth);

  /* Remove from previous tile, if applicable */
  if(tile_prev.size() > 0)
    tile_prev[x][y]->unsetIO(render_depth);
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
 *         bool from_save - true if the load is from a save file
 * Output: bool - status if successful
 */
bool MapInteractiveObject::addThingInformation(XmlData data, int file_index,
                                               int section_index,
                                               SDL_Renderer* renderer,
                                               std::string base_path,
                                               bool from_save)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;

  /* Ensure there is at least one valid node */
  if(node_head == nullptr)
  {
    appendEmptyNode();
    node_head->state = new MapState(event_handler);
    node_head->state->setMatrix(new SpriteMatrix());
    setParentFrames();
  }

  /* Parse the identifier for setting the IO information */
  /*--------------- INACTIVITY TIME -----------------*/
  if(identifier == "inactive" && elements.size() == 1)
  {
    setInactiveTime(data.getDataInteger(&success));
  }
  /*-------------------- LOCK -----------------------*/
  else if(identifier == "lock")
  {
    lock_struct = EventSet::updateLocked(lock_struct, data, file_index + 1);
  }
  /* ------------------- LOCKED --------------------- */
  else if(identifier == "locked")
  {
    lock_struct = EventSet::updateLocked(lock_struct, data, file_index);
  }
  /*----------------- RENDER MATRIX -----------------*/
  else if(elements.size() == 1 && elements.front() == "rendermatrix")
  {
    StateNode* node = node_head;
    while(node != nullptr)
    {
      /* Set the render matrix */
      if(node->state != nullptr && node->state->getMatrix() != nullptr)
        node->state->getMatrix()->setRenderMatrix(data.getDataString(&success));
      else if(node->transition != nullptr)
        node->transition->setRenderMatrix(data.getDataString(&success));

      /* Bump the node */
      node = node->next;
    }
  }
  /* ------------ SHIFT FORWARD ---------------- */
  else if(identifier == "shiftforward")
  {
    bool status = data.getDataBool(&success);
    if(success)
      shifting_forward = status;
  }
  /* -------------- STATE ID ------------------- */
  else if(identifier == "stateid")
  {
    /* Try and get the ID */
    int id = data.getDataInteger(&success);
    if(success && id >= 0)
    {
      StateNode* node_parse = node_head;
      while(node_parse != nullptr && id > 0)
      {
        node_parse = node_parse->next;
        id--;
      }
      if(node_parse != nullptr)
      {
        node_current = node_parse;
        setParentFrames();
      }
      else
      {
        success = false;
      }
    }
  }
  /*--------------- STATES FOR MIO -----------------*/
  else if(identifier == "states" && elements.size() > 2 &&
          (elements[1] == "state" || elements[1] == "transition") &&
          data.getKey(file_index + 1) == "id")
  {
    int index = std::stoi(data.getKeyValue(file_index + 1));
    int length = getNodeLength();

    /* Fill up the node length to suffice for the given index */
    while(length <= index)
    {
      appendEmptyNode();
      length++;
    }

    /* Proceed to determine if it was changed */
    StateNode* modified_node = getNode(index);
    if(modified_node != nullptr)
    {
      /* Get the render matrix of the head node */
      std::string render_matrix = "";
      if(node_head->state != nullptr)
        render_matrix = node_head->state->getMatrix()->getRenderMatrix();
      else
        render_matrix = node_head->transition->getRenderMatrix();

      /* Clear the node if it has the wrong data in it */
      if((elements[1] == "state" && modified_node->transition != nullptr) ||
         (elements[1] == "transition" && modified_node->state != nullptr))
        clearNode(index);

      /* Move forward with updating the given state/transiton sequence */
      if(elements[1] == "state")
      {
        /* Make the state if it doesn't exist */
        if(modified_node->state == nullptr)
        {
          modified_node->state = new MapState(event_handler);
          modified_node->state->getMatrix()->setRenderMatrix(render_matrix);
          if(modified_node == node_current)
            setParentFrames();
        }

        /* Update the state */
        success &= modified_node->state->addFileInformation(data,
                            file_index + 2, section_index, renderer, base_path);
      }
      /*--------------------- TRANSITION FRAMES -----------------*/
      else if(elements[1] == "transition")
      {
        /* Make the transition if it doesn't exist */
        if(modified_node->transition == nullptr)
        {
          modified_node->transition = new SpriteMatrix();
          modified_node->transition->setRenderMatrix(render_matrix);
          if(modified_node == node_current)
            setParentFrames();
        }

        /* Update the transition */
        success &= modified_node->transition->addFileInformation(data,
                                           file_index + 3, renderer, base_path);
      }
    }
  }
  /* ---------------------- TIME ELAPSED -------------------- */
  else if(identifier == "timeelapsed")
  {
    int time = data.getDataInteger(&success);
    if(success && time >= 0)
      time_elapsed = time;
  }
  /* Proceed to parent */
  else
  {
    success &= MapThing::addThingInformation(data, file_index,
                                             section_index, renderer,
                                             base_path, from_save);
  }

  /* If not from save, reset changed back to false */
  if(!from_save)
    changed = false;

  return success;
}

/*
 * Description: This is the class descriptor. Primarily used for encapsulation
 *              to determine which class to cast it to for specific parameters.
 *
 * Inputs: none
 * Output: ThingBase - the thing description enum defining the type to cast
 */
ThingBase MapInteractiveObject::classDescriptor()
{
  return ThingBase::INTERACTIVE;
}

/*
 * Description: Takes the frame matrix, as it's been set up and removes any
 *              rows or columns at the end that have no valid frames set. A
 *              frame is classified as valid if it's not NULL and has renderable
 *              frames stored within it.
 *
 * Inputs: bool first_call - is this the first call? default true
 * Output: bool - true if clean validated frame data
 */
bool MapInteractiveObject::cleanMatrix(bool first_call)
{
  bool equal_size = true;
  (void)first_call;
  uint16_t height = 0;
  uint16_t width = 0;
  StateNode* node = node_head;

  /* Parse through each node and check/clean the data involved */
  while(node != nullptr)
  {
    SpriteMatrix* matrix = nullptr;

    /* Get the matrix corresponding to the node */
    if(node->state != nullptr)
      matrix = node->state->getMatrix();
    else
      matrix = node->transition;

    if(matrix != nullptr)
    {
      /* Clean the matrix */
      matrix->cleanMatrix();

      /* Get the first state width and height */
      if(height == 0 || width == 0)
      {
        height = matrix->height();
        width = matrix->width();
      }
      else if(height != matrix->height() || width != matrix->width())
      {
        equal_size = false;
      }
    }

    /* Shift to the next */
    node = node->next;
  }

  return equal_size;
}

/*
 * Description: Clears all variables in the MIO, back to blank default state.
 *
 * Inputs: none
 * Output: none
 */
void MapInteractiveObject::clear()
{
  /* Clears action initiator pointer */
  action_initiator = nullptr;
  lock_struct = EventSet::createBlankLocked();
  person_on = nullptr;
  shifting_forward = true;
  time_elapsed = 0;
  time_return = kRETURN_TIME_UNUSED;

  /* Clear states */
  unsetFrames();

  /* Parent cleanup */
  MapThing::clear();
}

/*
 * Description: Returns the first valid frame pointer. NULL if none found. This
 *              overrides the default map thing functionality.
 *
 * Inputs: none
 * Output: TileSprite* - the first valid frame reference
 */
TileSprite* MapInteractiveObject::getFrameValid()
{
  StateNode* node = node_head;

  /* Parse through each node and check/clean the data involved */
  while(node != nullptr)
  {
    SpriteMatrix* matrix = nullptr;

    /* Get the matrix corresponding to the node */
    if(node->state != nullptr)
      matrix = node->state->getMatrix();
    else
      matrix = node->transition;

    if(matrix != nullptr)
    {
      TileSprite* found_sprite = matrix->getSpriteValid();
      if(found_sprite != nullptr)
        return found_sprite;
    }

    /* Shift to the next */
    node = node->next;
  }
  return nullptr;
}

/*
 * Description: Returns the inactive time before returning back down the state
 *              path.
 *
 * Inputs: none
 * Output: int - the integer inactive time. -1 if inactive time not used
 */
int MapInteractiveObject::getInactiveTime() const
{
  if(base != nullptr && !time_valid)
    return static_cast<MapInteractiveObject*>(base)->time_return;
  return time_return;
}
  
/* Returns the interaction at the current state */
MapState::InteractionState MapInteractiveObject::getInteraction()
{
  StateNode* node_curr = getStateCurrent();
  if(node_curr != nullptr && node_curr->state != nullptr)
    return node_curr->state->getInteraction();
  return MapState::NOINTERACTION;
}

/*
 * Desciption: Returns the locked state struct. This defines the locked
 *             properties of the MIO states.
 *
 * Inputs: none
 * Output: Locked - the locked state struct. Structure defined in EventSet
 */
Locked MapInteractiveObject::getLock()
{
  return lock_struct;
}

/*
 * Description: Returns the current state node.
 *
 * Inputs: none
 * Output: StateNode* - state node pointer. DO NOT DELETE. NULL if unset
 */
StateNode* MapInteractiveObject::getStateCurrent()
{
  return node_current;
}

/*
 * Description: Returns the head state node.
 *
 * Inputs: none
 * Output: StateNode* - state node pointer. DO NOT DELETE. NULL if unset
 */
StateNode* MapInteractiveObject::getStateHead()
{
  return node_head;
}

/*
 * Description: Handles the MIO trigger call from the event handler. Required
 *              due to the locked struct within the map interactive object.
 *
 * Inputs: int interaction_state - integer representation of InteractionState
 *         MapPerson* initiator - the initiating map person
 */
bool MapInteractiveObject::handlerTrigger(int interaction_state,
                                          MapPerson* initiator)
{
  bool success = false;
  (void)initiator;

  if(node_current != nullptr && node_current->state != nullptr)
  {
    /* Determine state - Event Trigger */
    if(interaction_state == (int)MapState::TRIGGER &&
       node_current->state->getInteraction() == MapState::TRIGGER)
    {
      handlerInteract();
      success = true;
    }
    /* Walk On Trigger */
    else if(interaction_state == (int)MapState::WALKON &&
            node_current->state->getInteraction() == MapState::WALKON)
    {
      handlerInteract();
      success = true;
    }
    /* Walk Off Trigger */
    else if(interaction_state == (int)MapState::WALKOFF &&
            node_current->state->getInteraction() == MapState::WALKOFF)
    {
      handlerInteract();
      success = true;
    }
    /* Use Trigger */
    else if(interaction_state == (int)MapState::USE &&
            node_current->state->getInteraction() == MapState::USE)
    {
      handlerInteract();
      success = true;
    }
  }

  return success;
}

/*
 * Description: The generic interact call. This is what fires when a player
 *              presses a use key and then this searches for if an event is
 *              available and starts it.
 *
 * Inputs: MapPerson* initiator - the pointer to the person that initiated it
 * Output: bool - if the call can occur (Event handler needs to be set)
 */
bool MapInteractiveObject::interact(MapPerson* initiator)
{
  bool status = false;

  /* Only proceed if the node is available */
  if(node_current != nullptr && node_current->state != nullptr)
  {
    /* Set the action initiator for the state */
    action_initiator = initiator;

    /* Fire the use event, if it exists */
    status |= node_current->state->triggerUseEvent(initiator, this);

    /* Proceed to execute a use event, if it exists */
    if(node_current->state->getInteraction() == MapState::USE &&
       shiftAvailable() && event_handler != nullptr)
    {
      event_handler->executeIOShift(this, static_cast<int>(MapState::USE),
                                    initiator);
      status = true;
    }

    /* Reset the time elapsed */
    time_elapsed = 0;
  }

  return status;
}

/*
 * Description: Returns if the inactive time is valid in either the instance
 *              or base.
 *
 * Inputs: none
 * Output: bool - true if there is a valid inactive return time
 */
bool MapInteractiveObject::isInactiveTimeValid()
{
  return (time_valid || (base != nullptr &&
                         static_cast<MapInteractiveObject*>(base)->time_valid));
}

/*
 * Description: Returns if the current state of the MIO is locked. If true,
 *              during an interact, no state change will occur. Otherwise,
 *              a state change is permitted to occur.
 *
 * Inputs: none
 * Output: bool - true if the MIO state change is currently locked.
 */
bool MapInteractiveObject::isLocked()
{
  return EventSet::isLocked(lock_struct);
}

/*
 * Description: Resets the MIO back to the head state.
 *
 * Inputs: none
 * Output: none
 */
void MapInteractiveObject::reset()
{
  node_current = node_head;
  shifting_forward = true;
  setParentFrames();
}

/*
 * Description: Saves the IO data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 * Output: bool - true if the save was successful
 */
bool MapInteractiveObject::save(FileHandler* fh)
{
  if(fh != nullptr)
  {
    bool success = true;

    /* Only proceed if there is data to save */
    if(isDataToSave())
    {
      fh->writeXmlElement("mapio", "id", getID());
      success &= saveData(fh, false);
      fh->writeXmlElementEnd();
    }

    return success;
  }
  return false;
}

/*
 * Description: Sets the base thing class. If set, the primary data will be set
 *              from this with only location and movement handled by this class.
 *
 * Inputs: MapThing* base - the reference base class
 * Output: bool - true if the base was set
 */
bool MapInteractiveObject::setBase(MapThing* base)
{
  bool success = false;

  if(classDescriptor() == ThingBase::INTERACTIVE)
  {
    if(base != nullptr && base->classDescriptor() == ThingBase::INTERACTIVE)
    {
      /* Status */
      this->base = base;
      base_category = ThingBase::INTERACTIVE;
      setVisibility(base->isVisible());

      /* States */
      if(node_head == nullptr)
      {
        /* Starting data */
        MapInteractiveObject* io_ref = static_cast<MapInteractiveObject*>(base);
        StateNode* ref_head = io_ref->node_head;
        StateNode* ref_curr = ref_head;

        /* Loop through all state nodes and create copied version */
        while(ref_curr != nullptr)
        {
          /* Assign node to tail */
          StateNode* node = new StateNode;
          node->state = nullptr;
          node->transition = nullptr;
          node->previous = nullptr;
          node->next = nullptr;
          appendNode(node);

          /* Fill node with relevant info */
          if(ref_curr->state != nullptr)
          {
            node->state = new MapState(event_handler);
            node->state->setBase(ref_curr->state);
          }
          else if(ref_curr->transition != nullptr)
          {
            node->transition = ref_curr->transition;
          }

          /* Next node */
          ref_curr = ref_curr->next;
        }

        //node_head = static_cast<MapInteractiveObject*>(base)->node_head;
        node_current = node_head;
        nodes_delete = false;
      }
      setParentFrames();

      /* Locked struct */
      lock_struct = ((MapInteractiveObject*)base)->lock_struct;

      success = true;
    }
    else if(base == nullptr)
    {
      this->base = nullptr;
      base_category = ThingBase::ISBASE;
      lock_struct = EventSet::createBlankLocked();
      success = true;
    }

    /* Update the event set */
    setEventBase();
  }

  return success;
}

/*
 * Description: Sets the color mode of all sprites within the matrix of the
 *              thing. Virtualized for the purpose of children.
 *
 * Inputs: ColorMode mode - the mode of how to color the thing
 * Output: none
 */
void MapInteractiveObject::setColorMode(ColorMode mode)
{
  StateNode* node = node_head;

  /* Parse through each node and check/clean the data involved */
  while(node != nullptr)
  {
    SpriteMatrix* matrix = nullptr;

    /* Get the matrix corresponding to the node */
    if(node->state != nullptr)
      matrix = node->state->getMatrix();
    else
      matrix = node->transition;

    if(matrix != nullptr)
      matrix->setColorMode(mode);

    /* Shift to the next */
    node = node->next;
  }
}

/*
 * Description: Sets the inactive time before the node sequence begins reverting
 *              to prior states.
 *
 * Inputs: int time - the time in ms before returning down the state stack.
 * Output: none
 */
void MapInteractiveObject::setInactiveTime(int time)
{
  int old_time = getInactiveTime();

  /* Set the inactive time */
  if(time < 0)
    time_return = kRETURN_TIME_UNUSED;
  else
    time_return = time;
  time_valid = true;

  /* Reset the elapsed time */
  time_elapsed = 0;

  /* Check if it was changed */
  if(old_time != time_return)
    changed = true;
}

/*
 * Description: Defines the locked information to use within the MIO. This
 *              replaces any existing locked information with no saving of the
 *              current running conditions (if set while the class is in
 *              operation).
 *
 * Inputs: Locked new_locked - the new locked struct with lock information
 * Output: bool - true if the locked struct was set
 */
bool MapInteractiveObject::setLock(Locked new_locked)
{
  lock_struct = new_locked;
  return true;
}

/*
 * Description: Sets the passed in main state at the tail end of the MIO stack.
 *
 * Inputs: MapState* state - new state to append to the stack
 * Output: bool - true if the node was added
 */
bool MapInteractiveObject::setState(MapState* state)
{
  if(state != nullptr && state->getMatrix() != nullptr)
  {
    /* Check to make sure the base nodes aren't being used */
    if(!nodes_delete)
      unsetFrames(true);

    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = state;
    node->transition = nullptr;
    node->previous = nullptr;
    node->next = nullptr;

    /* Assign the node to the tail of the sequence */
    appendNode(node);

    return true;
  }

  return false;
}

/*
 * Description: Sets the passed in transition state at the tail end of the MIO
 *              stack.
 *
 * Inputs: MapState* state - new transition state to append to the stack
 * Output: bool - true if the node was added
 */
bool MapInteractiveObject::setState(SpriteMatrix* transition)
{
  if(transition != nullptr)
  {
    /* Check to make sure the base nodes aren't being used */
    if(!nodes_delete)
      unsetFrames(true);

    StateNode* node = new StateNode;

    /* Set the state parameters */
    node->state = nullptr;
    node->transition = transition;
    node->previous = nullptr;
    node->next = nullptr;

    /* Assign the node to the tail of the sequence */
    appendNode(node);

    return true;
  }

  return false;
}

/*
 * Description: Triggers the walk off event for the passed of person. Pertinent
 *              for triggering events. Handled by the movement of thing.
 *
 * Inputs: MapPerson* trigger - the triggering person pointer
 * Output: none
 */
void MapInteractiveObject::triggerWalkOff(MapPerson* trigger)
{
  if(trigger != nullptr && person_on == trigger)
  {
    /* Trigger walk off interaction */
    if(node_current != nullptr && node_current->state != nullptr &&
       node_current->state->getInteraction() == MapState::WALKOFF)
    {
      if(event_handler != nullptr)
        event_handler->executeIOShift(this, (int)MapState::WALKOFF, trigger);
    }

    person_on = nullptr;
  }
}

/*
 * Description: Triggers the walk on event for the passed of person. Pertinent
 *              for triggering events. Handled by the movement of thing.
 *
 * Inputs: MapPerson* trigger - the triggering person pointer
 * Output: none
 */
void MapInteractiveObject::triggerWalkOn(MapPerson* trigger)
{
  if(trigger != nullptr && person_on == nullptr)
  {
    person_on = trigger;

    if(node_current != nullptr && node_current->state != nullptr)
    {
      /* Trigger walkover event, if valid */
      node_current->state->triggerWalkoverEvent(person_on, this);

      /* Trigger walk on interaction */
      if(node_current->state->getInteraction() == MapState::WALKON)
      {
        if(event_handler != nullptr)
          event_handler->executeIOShift(this, (int)MapState::WALKON, trigger);
      }
    }
  }
}

/*
 * Description: Attempts an unlock on the MIO taking the mode, relevant state
 *              number (if mode is state based), and the events within the state
 *              to address
 *
 * Inputs: UnlockIOMode mode - the mode(s) of the IO to try and unlock
 *         int state_num - what states to modify (only for EVENTS mode)
 *         UnlockIOEvent mode_events - which events to attempt to unlock
 * Output: bool - true if at least one was locked and now unlocked
 */
bool MapInteractiveObject::unlockTrigger(UnlockIOMode mode, int state_num,
                                         UnlockIOEvent mode_events)
{
  bool unlocked = false;
  bool lock, events;
  EventSet::dataEnumIOMode(mode, lock, events);

  /* If lock mode, parse and execute */
  if(lock)
  {
    if(EventSet::isLocked(lock_struct))
    {
      EventSet::unlockTrigger(lock_struct);
      unlocked |= !lock_struct.is_locked;
    }
  }

  /* If events mode, parse and execute */
  if(events)
  {
    /* All states modified */
    if(state_num < 0)
    {
      StateNode* parser = node_head;

      /* Parse all states */
      while(parser != nullptr)
      {
        if(parser->state != nullptr)
          unlocked |= parser->state->unlockTrigger(mode_events);
        parser = parser->next;
      }
    }
    /* Only designated state is modified */
    else
    {
      StateNode* node = getNode(state_num);
      if(node != nullptr && node->state != nullptr)
        unlocked |= node->state->unlockTrigger(mode_events);
    }
  }

  return unlocked;
}

/*
 * Description: Updates the frames of the MIO. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the ms time to update the movement/animation
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 *         bool active_map - true if this IOs section is the active map
 * Output: Floatinate - the delta x and y of the moved IO
 */
Floatinate MapInteractiveObject::update(int cycle_time,
                                      std::vector<std::vector<Tile*>> tile_set,
                                      bool active_map)
{
  Floatinate delta_move;
  (void)tile_set;

  /* For active and valid tiles, do update sequence */
  if(isActive() && isTilesSet())
  {
    if(active_map)
    {
      /* Animate the frames and determine if the frame has changed */
      bool frames_changed = animate(cycle_time, false, false);

      /* Check if base is set or not */
      if(base != nullptr)
      {
        /* Only proceed if frames are changed and a transitional sequence */
        if(frames_changed && node_current != nullptr &&
           node_current->transition != nullptr)
        {
          if(base_control->forward &&
             base_control->curr_frame == 0)
          {
            /* Try and shift to the next state. If fails, re-animate
             * transition */
            if(!shiftNext())
            {
              base_control->forward = false;
              shifting_forward = false;
              time_elapsed = 0;
            }
          }
          else if(!base_control->forward &&
                  (base_control->curr_frame + 1) == base_control->num_frames)
          {
            /* Try and shift to the previous state. If fails, re-animate
             * transition */
            if(!shiftPrevious())
            {
              base_control->forward = true;
              shifting_forward = true;
              time_elapsed = 0;
            }
          }
        }
      }
      else
      {
        /* Only proceed if frames are changed and a transitional sequence */
        if(frames_changed && node_current != nullptr &&
           node_current->transition != nullptr)
        {
          if(node_current->transition->isDirectionForward() &&
             node_current->transition->isAtFirst())
          {
            /* Try and shift to the next state. If fails, re-animate
             * transition */
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
            /* Try and shift to the previous state. If fails, re-animate
             * transition */
            if(!shiftPrevious())
            {
              node_current->transition->setDirectionForward();
              shifting_forward = true;
              time_elapsed = 0;
            }
          }
        }
      }

      /* Determine if the cycle time has passed on activity response */
      if(getInactiveTime() != kRETURN_TIME_UNUSED && node_current != node_head
                                            && node_current->state != nullptr)
      {
        time_elapsed += cycle_time;
        if(time_elapsed > getInactiveTime())
        {
          shifting_forward = false;
          shiftPrevious();
        }
      }
    }
  }
  /* Base Interactive Object */
  else if(base == nullptr)
  {
    StateNode* node = node_head;

    /* Parse through each node and check/clean the data involved */
    while(node != nullptr)
    {
      SpriteMatrix* matrix = nullptr;

      /* Get the matrix corresponding to the node */
      if(node->state != nullptr)
        matrix = node->state->getMatrix();
      else
        matrix = node->transition;

      if(matrix != nullptr)
        matrix->update(cycle_time, false, true);

      /* Shift to the next */
      node = node->next;
    }
  }
  /* Otherwise, just pass to parent */
  else
  {
    delta_move = MapThing::update(cycle_time, tile_set, active_map);
  }

  return delta_move;
}

/*
 * Description: Unsets the frames that are embedded in all state nodes in the
 *              MIO.
 *
 * Inputs: bool delete_state - should the old frames be deleted?
 * Output: none
 */
void MapInteractiveObject::unsetFrames(bool delete_frames)
{
  StateNode* node = node_head;
  unsetMatrix();

  /* Proceed to delete all nodes */
  while(node != nullptr)
  {
    StateNode* temp_node = node;
    node = node->next;

    /* Proceed to delete all relevant data */
    if(delete_frames)
    {
      if(temp_node->state != nullptr)
        delete temp_node->state;
      else if(temp_node->transition != nullptr && nodes_delete)
        delete temp_node->transition;
      delete temp_node;
    }
  }

  /* Clear the class data */
  node_current = nullptr;
  node_head = nullptr;
  nodes_delete = true;
}
