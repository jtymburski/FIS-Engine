/*******************************************************************************
 * Class Name: MapNPC
 * Date Created: Dec 2 2012
 * Inheritance: MapPerson
 * Description: The MapNPC class, this covers all AI found on the map in game
 *
 * TODO: 1. Reimplement the movement functions based on NPC scripts
 ******************************************************************************/
#include "Game/Map/MapNPC.h"

/* Constant Implementation - see header file for descriptions */
const uint16_t MapNPC::kFORCED_FREEZE = 5000;
const uint16_t MapNPC::kFORCED_NOTRIGGER = 30000;
const uint16_t MapNPC::kMAX_DELAY = 2000;
const uint16_t MapNPC::kMAX_RANGE = 10;
const uint16_t MapNPC::kTRACK_DIST_MIN = 5;
const uint16_t MapNPC::kTRACK_DIST_MAX = 10;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for this class. Sets up an empty NPC with no data.
 *
 * Inputs: none
 */
MapNPC::MapNPC() : MapPerson()
{
  initializeClass();
}

/*
 * Description: Constructor for this class. Takes basic data to start with a
 *              basic version of the NPC.
 *
 * Inputs: int id - the ID of the NPC
 *         std::string name - the name of the NPC
 *         std::string description - the description of the NPC
 */
MapNPC::MapNPC(int id, std::string name, std::string description)
      : MapPerson(id, name, description)
{
  initializeClass();
}

/*
 * Description: Destructor function
 */
MapNPC::~MapNPC()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Appends an empty node onto the back of the movement stack.
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::appendEmptyNode()
{
  /* Set up the node structure */
  Path* new_node = new Path();
  new_node->x = 0;
  new_node->y = 0;
  new_node->delay = 0;
  new_node->xy_flip = false;
  new_node->previous = NULL;
  new_node->next = NULL;

  /* Append the node to the end of the list */
  if(!insertNode(getPathLength(), new_node))
    delete new_node;
}

/*
 * Description: Returns if the xy directional movement between nodes is flipped.
 *              If false, the order is x, then y. Otherwise, the order is
 *              reversed (y then x). This works with the current target node and
 *              the node it was previously at.
 *
 * Inputs: none
 * Output: bool - true if order is y then x. false if order is x then y.
 */
bool MapNPC::getXYFlip()
{
  if(starting)
  {
    return node_start.xy_flip;
  }
  else if(tracking)
  {
    return false;
  }
  else
  {
    if(node_state == LOOPED)
      return node_current->previous->xy_flip;
    else if(node_state == RANDOMRANGE || node_state == RANDOM)
      return node_current->xy_flip;
    else if(node_state == BACKANDFORTH)
    {
      if(moving_forward)
        return node_current->previous->xy_flip;
      else
        return !node_current->xy_flip;
    }
  }

  return false;
}

/*
 * Description: Returns the node at the given index. NULL if out of range.
 *
 * Inputs: uint16_t index - the node index, offset from HEAD
 * Output: Path* - the path structure at the node index
 */
Path* MapNPC::getNode(uint16_t index)
{
  bool failed = false;
  Path* returned_node = NULL;

  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
  {
    returned_node = static_cast<MapNPC*>(base)->getNode(index);
  }
  else
  {
    if(node_head != NULL)
    {
      /* Loop through to find the node */
      returned_node = node_head;
      for(uint16_t i = 0; i < index; i++)
      {
        returned_node = returned_node->next;
        if(returned_node == node_head)
          failed = true;
      }
    }
  }

  /* Execute the return sequence, if applicable */
  if(failed)
    returned_node = NULL;
  return returned_node;
}

/*
 * Description: Initializes the class. Only called on initial construction.
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::initializeClass()
{
  /* Clear the path pointers */
  forced_interaction = false;
  forced_recent = false;
  forced_time = 0;
  moving_forward = true;
  node_current = &node_start;
  node_head = nullptr;
  node_previous = nullptr;
  node_state = LOOPED;
  nodes_delete = true;
  npc_delay = 0;
  player = nullptr;
  starting = true;
  track_dist = kTRACK_DIST_MIN;
  track_dist_max = kTRACK_DIST_MAX;
  track_state = NOTRACK;
  tracking = false;

  /* Set the path player node to blank state */
  node_player.x = 0;
  node_player.y = 0;
  node_player.delay = 0;
  node_player.xy_flip = false;
  node_player.previous = nullptr;
  node_player.next = nullptr;
  
  /* Set the path random node to blank state */
  node_random.x = 0;
  node_random.y = 0;
  node_random.delay = 0;
  node_random.xy_flip = false;
  node_random.previous = NULL;
  node_random.next = NULL;

  /* Bounding rect init to 0 state */
  node_rect = {0, 0, 0, 0};

  /* Blank out start node */
  node_start.x = 0;
  node_start.y = 0;
  node_start.delay = 0;
  node_start.xy_flip = false;
  node_start.previous = NULL;
  node_start.next = NULL;
}

/*
 * Description: Inserts a node, at the given index. If inserted on a node where
 *              one exists already, it pushes it back to index + 1.
 *
 * Inputs: uint16_t index - the node index, offset from HEAD
 *         Path* node - the new path node struct to insert
 * Output: bool - true if successful
 */
bool MapNPC::insertNode(uint16_t index, Path* node)
{
  bool success = false;

  /* Only proceed if node is non-null */
  if(node != NULL)
  {
    /* Initial checks to see if nodes are set by base class */
    if(!nodes_delete)
    {
      node_head = NULL;
      nodes_delete = true;
    }

    /* If the head is 0 and the index is 0, drop it in the first slot */
    if(node_head == NULL && index == 0)
    {
      node->previous = node;
      node->next = node;
      node_head = node;
      success = true;
    }
    /* If it's at the end of the
     * Otherwise, drop it behind the first, if the index is in range */
    else if(index <= getPathLength())
    {
      Path* temp_node = node_head;
      success = true;

      /* Loop through to find the insertion point */
      for(uint16_t i = 0; i < index; i++)
        temp_node = temp_node->next;

      /* Re-reference the double linked list pointers */
      temp_node->previous->next = node;
      node->previous = temp_node->previous;
      temp_node->previous = node;
      node->next = temp_node;

      /* If the index is at 0, reset the head */
      if(index == 0)
        node_head = node;
    }

    /* If successful, reset the current node ptr of the class */
    if(success)
    {
      resetPosition();
      node_current = &node_start;
      npc_delay = 0;
      updateBound();
    }
  }

  return success;
}

/*
 * Description: Takes the current random path node and re-randomizes the x, y,
 *              delay, and flip parameters for random NPC movement. This is
 *              called each time the node is reached or movement is prevented
 *              by permanent object.
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::randomizeNode()
{
  /* X calc */
  int delta_x = Helpers::randU(-kMAX_RANGE, kMAX_RANGE);
  if(delta_x + node_random.x >= 0)
    node_random.x += delta_x;
  else
    node_random.x = 0;

  /* Y calc */
  int delta_y = Helpers::randU(-kMAX_RANGE, kMAX_RANGE);
  if(delta_y + node_random.y >= 0)
    node_random.y += delta_y;
  else
    node_random.y = 0;

  /* Delay calc */
  int delay = Helpers::randU(0, kMAX_DELAY); // -kMAX_DELAY, kMAX_DELAY);
  if(delay > 0)
    node_random.delay = delay;
  else
    node_random.delay = 0;

  /* Flip calc */
  node_random.xy_flip = Helpers::flipCoin();

  /* Validate range if it's a randomrange move */
  if(node_state == RANDOMRANGE)
  {
    /* Determine x and y range first */
    int x1 = node_head->x;
    int y1 = node_head->y;
    int x2 = node_head->next->x;
    int y2 = node_head->next->y;

    /* Order properly */
    if(x1 > x2)
    {
      int temp_x = x1;
      x1 = x2;
      x2 = temp_x;
    }
    if(y1 > y2)
    {
      int temp_y = y1;
      y1 = y2;
      y2 = temp_y;
    }

    /* Ensure within range */
    if(node_random.x < x1)
      node_random.x = x1;
    if(node_random.x > x2)
      node_random.x = x2;
    if(node_random.y < y1)
      node_random.y = y1;
    if(node_random.y > y2)
      node_random.y = y2;
  }
}

/* Update the node bounding rect */
void MapNPC::updateBound()
{
  Path* node_parse = node_head;
  int x_max = -1;
  int x_min = -1;
  int y_max = -1;
  int y_min = -1;
  
  /* Proceed to generate new information */
  if(node_parse != nullptr)
  {
    do
    {
      /* Min */
      if(x_min < 0 || (node_parse->x < x_min))
        x_min = node_parse->x;
      if(y_min < 0 || (node_parse->y < y_min))
        y_min = node_parse->y;
      
      /* Max */
      if(x_max < 0 || (node_parse->x > x_max))
        x_max = node_parse->x;
      if(y_max < 0 || (node_parse->y > y_max))
        y_max = node_parse->y;
      
      /* Go to next */
      node_parse = node_parse->next;

    } while(node_parse != node_head);
  }
  
  /* If data is valid, load it in */
  if(x_min >= 0 && y_min >= 0 && x_max >= 0 && y_max >= 0)
  {
    node_rect = {x_min, y_min, x_max - x_min + 1, y_max - y_min + 1};
  }
  else
  {
    node_rect = {0, 0, 0, 0};
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds npc information from the XML file. This will be
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
bool MapNPC::addThingInformation(XmlData data, int file_index,
                                 int section_index, SDL_Renderer* renderer,
                                 std::string base_path)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;

  /* Parse the identifier for setting the person information */
  /*--------------------- NODESTATE --------------------*/
  if(identifier == "nodestate" && elements.size() == 1)
  {
    std::string state = data.getDataString(&success);
    if(success)
    {
      if(state == "looped")
        setNodeState(MapNPC::LOOPED);
      else if(state == "backandforth")
        setNodeState(MapNPC::BACKANDFORTH);
      else if(state == "locked")
        setNodeState(MapNPC::LOCKED);
      else if(state == "randomrange")
        setNodeState(MapNPC::RANDOMRANGE);
      else if(state == "random")
        setNodeState(MapNPC::RANDOM);
      else
        success = false;
    }
  }
  /*--------------------- NODE MOVEMENT --------------------*/
  else if(identifier == "node" && elements.size() == 2)
  {
    uint16_t index = std::stoul(data.getKeyValue(file_index));
    uint16_t length = getPathLength();

    /* Increase the length if necessary to allow for the new node */
    while(index >= length)
    {
      appendEmptyNode();
      length++;
    }

    /* Get the node and work with it, if relevant */
    Path* node = getNode(index);
    if(node != NULL)
    {
      /* Parse the node identity */
      if(elements[1] == "x")
      {
        uint16_t x = data.getDataInteger(&success);
        if(success)
        {
          node->x = x;
          updateBound();
        }
      }
      else if(elements[1] == "y")
      {
        uint16_t y = data.getDataInteger(&success);
        if(success)
        {
          node->y = y;
          updateBound();
        }
      }
      else if(elements[1] == "delay")
      {
        uint16_t delay = data.getDataInteger(&success);
        if(success)
          node->delay = delay;
      }
      else if(elements[1] == "xyflip")
      {
        bool flip = data.getDataBool(&success);
        if(success)
          node->xy_flip = flip;
      }
    }
    else
    {
      success = false;
    }
  }
  /*--------------------- START NODE --------------------*/
  else if(identifier == "startnode" && elements.size() == 2)
  {
    /* Parse the node identity */
    if(elements.back() == "delay")
    {
      uint16_t delay = data.getDataInteger(&success);
      if(success)
        node_start.delay = delay;
    }
    else if(elements.back() == "xyflip")
    {
      bool flip = data.getDataBool(&success);
      if(success)
        node_start.xy_flip = flip;
    }
  }
  /*--------------------- TRACKING --------------------*/
  else if(identifier == "tracking" && elements.size() == 1)
  {
    std::string state = data.getDataString(&success);
    if(success)
    {
      if(state == "none")
        setTrackingState(MapNPC::NOTRACK);
      else if(state == "toplayer")
        setTrackingState(MapNPC::TOPLAYER);
      else if(state == "avoidplayer")
        setTrackingState(MapNPC::AVOIDPLAYER);
      else
        success = false;
    }
  }
  /*----------------- FORCED INTERACTION --------------------*/
  else if(identifier == "forcedinteraction" && elements.size() == 1)
  {
    bool interact = data.getDataBool(&success);
    if(success)
      setForcedInteraction(interact);
  }
  else
  {
    success &= MapPerson::addThingInformation(data, file_index, section_index,
                                              renderer, base_path);
  }

  return success;
}

/*
 * Description: This is the class descriptor. Primarily used for encapsulation
 *              to determine which class to cast it to for specific parameters.
 *
 * Inputs: none
 * Output: std::string - the string descriptor, it will be the same as the class
 *                       name. For example, "MapThing", "MapPerson", etc.
 */
std::string MapNPC::classDescriptor()
{
  return "MapNPC";
}

/*
 * Description: Clears out all information stored in the class
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::clear()
{
  /* Clear out all the nodes */
  removeAllNodes();
  node_head = NULL;
  node_current = NULL;

  /* Clear out other variables */
  npc_delay = 0;
  player = nullptr;

  /* Clear out parent */
  MapPerson::clear();
}
  
/*
 * Description: Gets if the movement is paused. This calls on parent MapThing
 *              for paused information in addition to forced recent freeze
 *              holds triggered by NPC.
 *
 * Inputs: none
 * Output: bool - true if the movement is paused
 */
bool MapNPC::getMovementPaused()
{
  return MapThing::getMovementPaused() || 
         (forced_recent && forced_time < kFORCED_FREEZE);
}

/*
 * Description: Returns the current node state, which is an Enum.
 *
 * Inputs: none
 * Output: MapNPC::NodeState - a node state enum
 */
MapNPC::NodeState MapNPC::getNodeState()
{
  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
    return static_cast<MapNPC*>(base)->node_state;
  return node_state;
}

/*
 * Description: Returns the number of nodes in the node sequence of the NPC.
 *
 * Inputs: none
 * Output: uint16_t - the number of nodes from HEAD to TAIL
 */
uint16_t MapNPC::getPathLength()
{
  uint16_t size = 1;
  Path* temp_node = node_head;

  /* Check if base is used or not */
  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
  {
    size = static_cast<MapNPC*>(base)->getPathLength();
  }
  else
  {
    if(node_head == NULL)
      size = 0;
    else
    {
      while(temp_node->next != node_head)
      {
        temp_node = temp_node->next;
        size++;
      }
    }
  }

  return size;
}

/*
 * Description: Returns the player reference. NULL if not set
 *
 * Inputs: none
 * Output: MapPerson* - the person reference pointer
 */
MapPerson* MapNPC::getPlayer()
{
  return player;
}

/*
 * Description: Returns the predicted move request for the class, based on the
 *              next node in the sequence.
 *
 * Inputs: none
 * Output: Direction - the direction the NPC will be pointing
 */
Direction MapNPC::getPredictedMoveRequest()
{
  if(isTilesSet() && node_current != NULL)
  {
    int delta_x = node_current->x - tile_main.front().front()->getX();
    int delta_y = node_current->y - tile_main.front().front()->getY();
    Direction direction = Direction::DIRECTIONLESS;
    bool xy_flip = getXYFlip();

    /* If the npc needs to move on the X plane */
    if(delta_x != 0 && (!xy_flip || (xy_flip && delta_y == 0)))
    {
      if(delta_x < 0)
        direction = Direction::WEST;
      else
        direction = Direction::EAST;
    }
    /* Else if the npc needs to move on the Y plane */
    else if(delta_y != 0)
    {
      if(delta_y < 0)
        direction = Direction::NORTH;
      else
        direction = Direction::SOUTH;
    }

    return direction;
  }

  return getMoveRequest();
}

/*
 * Description: Returns the tracking state. This defines how the NPC reacts to
 *              the player as it nears the NPC.
 *
 * Inputs: none
 * Output: MapNPC::TrackingState - the tracking state enum
 */
MapNPC::TrackingState MapNPC::getTrackingState()
{
  //if(base != nullptr)
  //{
  //  MapNPC* base_npc = static_cast<MapNPC*>(base);

  //  if(base_npc->track_state != track_state)
  //    return track_state;
  //  else
  //    return base_npc->track_state;
  //}
  return track_state;
}

/*
 * Description: Inserts a new node that is created with an x and y point and a
 *              delay for pausing at the given index. If a node already exists
 *              at the index, it pushes all other nodes back one.
 *
 * Inputs: uint16_t index - the node index, offset from HEAD
 *         uint16_t x - the x tile location for the node
 *         uint16_t y - the y tile location for the node
 *         uint16_t delay - the time to pause on the node when reached (ms)
 * Output: bool - true if the insertion was successful
 */
bool MapNPC::insertNode(uint16_t index, uint16_t x, uint16_t y, uint16_t delay)
{
  bool success = true;

  /* Set up the node structure */
  Path* new_node = new Path();
  new_node->x = x;
  new_node->y = y;
  new_node->delay = delay;

  /* Attempt to insert the node */
  if(!insertNode(index, new_node))
  {
    delete new_node;
    success = false;
  }

  return success;
}

/*
 * Description: Inserts a new node that is created with an x and y point and a
 *              delay for pausing at the tail of the node stack
 *
 * Inputs: uint16_t x - the x tile location for the node
 *         uint16_t y - the y tile location for the node
 *         uint16_t delay - the time to pause on the node when reached (ms)
 * Output: bool - true if the insertion was successful
 */
bool MapNPC::insertNodeAtTail(uint16_t x, uint16_t y, uint16_t delay)
{
  return insertNode(getPathLength(), x, y, delay);
}

/*
 * Description: This is the call map will make when a forced interaction is
 *              triggered after it is determined the 'initiator' is nearby.
 *              Also sets some hold delays for when the NPC will trigger again
 *
 * Inputs: MapPerson* initiator - the event initiator, typically the player
 * Output: bool - true if an event was triggered
 */
bool MapNPC::interactForced(MapPerson* initiator)
{
  if(!forced_recent && interact(initiator))
  {
    forced_recent = true;
    forced_time = 0;
    return true;
  }
  return false;
}

/*
 * Description: Returns if the NPC will force interaction upon the player if it
 *              moves within the vicinity of the NPC.
 *
 * Inputs: bool false_if_active - true to also include if there has been a
 *                                recent forced interaction. If so, this will
 *                                return false regardless. Default true
 * Output: bool - true if the NPC will force interaction
 */
bool MapNPC::isForcedInteraction(bool false_if_active)
{
  //bool curr = forced_interaction;

  //if(base != nullptr)
  //{
  //  MapNPC* base_npc = static_cast<MapNPC*>(base);

  //  if(base_npc->forced_interaction != forced_interaction)
  //    curr = forced_interaction;
  //  else
  //    curr = base_npc->forced_interaction;
  //}
  //return (curr && !forced_recent);
  if(false_if_active)
    return (forced_interaction && !forced_recent);
  return forced_interaction;
}

/*
 * Description: Returns if there is any tracking within the NPC.
 *
 * Inputs: none
 * Output: bool - true if any tracking is set
 */
bool MapNPC::isTracking()
{
  return (getTrackingState() != MapNPC::NOTRACK);
}

/*
 * Description: Removes all nodes in the NPC sequence.
 *
 * Inputs: none
 * Output: bool - true if successful
 */
bool MapNPC::removeAllNodes()
{
  if(nodes_delete)
    while(removeNodeAtTail());
  else
  {
    node_head = NULL;
    node_current = NULL;
  }

  return true;
}

/*
 * Description: Removes the node at the given index. If this has nodes after it,
 *              those nodes are bumped up (index - 1).
 *
 * Inputs: uint16_t index - the index of the node to remove
 * Output: bool - true if successful
 */
bool MapNPC::removeNode(uint16_t index)
{
  uint16_t length = getPathLength();
  bool success = false;
  Path* temp_node = NULL;

  if(nodes_delete)
  {
    if(index == 0 && length == 1)
    {
      temp_node = node_head;
      node_head = NULL;
      node_current = NULL;
      success = true;
    }
    else if(index < length)
    {
      /* Loop through to find the node to remove */
      temp_node = node_head;
      for(uint16_t i = 0; i < index; i++)
        temp_node = temp_node->next;

      /* Reset the pointers around the path to delete */
      temp_node->previous->next = temp_node->next;
      temp_node->next->previous = temp_node->previous;

      /* Fix the pointers if they need to be changed */
      if(index == 0)
        node_head = temp_node->next;

      updateBound();
      success = true;
    }
  }

  /* Finish by deleting the pointer */
  if(success)
    delete temp_node;

  return success;
}

/*
 * Description: Removes the node at the tail of the node sequence.
 *
 * Inputs: none
 * Output: bool - true if successful
 */
bool MapNPC::removeNodeAtTail()
{
  if(node_head != NULL)
    return removeNode(getPathLength() - 1);
  return false;
}

/*
 * Description: Sets the base thing class. If set, the primary data will be set
 *              from this with only location and movement handled by this class.
 *
 * Inputs: MapThing* base - the reference base class
 * Output: bool - true if the base was set
 */
bool MapNPC::setBase(MapThing* base)
{
  bool success = false;

  if(classDescriptor() == "MapNPC")
  {
    /* Set the base with the new ptr */
    if(base != NULL && base->classDescriptor() == "MapNPC")
    {
      this->base = base;
      base_category = ThingBase::NPC;
      setMatrix(getState(getSurface(), getDirection()));
      setSpeed(base->getSpeed());
      forced_interaction = static_cast<MapNPC*>(base)->forced_interaction;
      track_state = static_cast<MapNPC*>(base)->track_state;
      if(node_head == NULL)
      {
        node_head = static_cast<MapNPC*>(base)->node_head;
        nodes_delete = false;
        node_current = &node_start;
        npc_delay = 0;
        updateBound();
      }
      success = true;
    }
    /* Clear out the old base and make this class a base class */
    else if(base == NULL)
    {
      this->base = NULL;
      base_category = ThingBase::ISBASE;
      success = true;
    }

    /* Update the event set */
    setEventBase();
  }

  return success;
}

/*
 * Description: Sets if the NPC will force interaction on a player if it moves
 *              within the vicinity.
 *
 * Inputs: bool forced - true if the NPC should force interaction
 * Output: none
 */
void MapNPC::setForcedInteraction(bool forced)
{
  forced_interaction = forced;
}

/*
 * Description: Sets the node state and how nodes are parsed with the controller
 *              enum.
 *
 * Inputs: NodeState state - the new node state
 * Output: none
 */
void MapNPC::setNodeState(NodeState state)
{
  node_state = state;

  /* Set starting moving direction */
  if(state == BACKANDFORTH)
    moving_forward = true;

  /* Fix the current node state */
//  if(node_state == NodeState::LOOPED ||
//     node_state == NodeState::BACKANDFORTH)
//  {
//    node_current = node_head;
//  }
//  else if(node_state == NodeState::RANDOM ||
//          node_state == NodeState::RANDOMRANGE)
//  {
//    node_current = &node_random;
//  }
}

/*
 * Description: Sets the reference player. The reference player is used for
 *              tracking and event forced triggering by the NPC object
 *
 * Inputs: MapPerson* player - the new player reference. NULL to unset
 * Output: none
 */
void MapNPC::setPlayer(MapPerson* player)
{
  this->player = player;
}

/*
 * Description: Sets the starting location which needs the map section id and
 *              an x and y coordinate. Virtually called before sending to thing
 *              to set the random starting location.
 *
 * Inputs: uint16_t section_id - the map section id
 *         uint16_t x - the x coordinate of the thing
 *         uint16_t y - the y coordinate of the thing
 * Output: none
 */
void MapNPC::setStartingLocation(uint16_t section_id, uint16_t x, uint16_t y)
{
  node_random.x = x;
  node_random.y = y;

  node_start.x = x;
  node_start.y = y;

  /* Set to parent */
  MapThing::setStartingLocation(section_id, x, y);
}

/*
 * Description: Sets the tracking state of the NPC and how it reacts as a player
 *              comes in the vicinity, based on a set range.
 *
 * Inputs: TrackingState state - the tracking state that the NPC should execute
 * Output: none
 */
void MapNPC::setTrackingState(TrackingState state)
{
  track_state = state;
}

/*
 * Description: Updates the frames of the NPC. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the ms time to update the movement/animation
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 * Output: none
 */
// TODO: Add forced interaction, tracking state - working
void MapNPC::update(int cycle_time, std::vector<std::vector<Tile*>> tile_set)
{
  /* Begin the check to handle each time the NPC is on a tile */
  if(isTilesSet() && node_current != NULL)
  {
    Direction direction = getPredictedMoveRequest();

    /* If starting sequence, operate on different parameters */
    if(starting)
    {
      if(!isMoving())
      {
        /* If reached node and done movement again, kill starting */
        if(node_current != &node_start &&
           node_current->x == tile_main.front().front()->getX() &&
           node_current->y == tile_main.front().front()->getY())
        {
          if(node_state == RANDOMRANGE)
          {
            node_current = &node_random;
            node_random.x = node_head->x;
            node_random.y = node_head->y;
          }
          starting = false;
        }
        else if(node_current == &node_start)
        {
        /* Delay first */
          if(node_start.delay > npc_delay)
          {
            npc_delay += cycle_time;
          }
          /* Then updates current node */
          else
          {
            if(node_state == LOOPED || node_state == BACKANDFORTH ||
               node_state == RANDOMRANGE)
            {
              node_current = node_head;
            }
            else if(node_state == RANDOM)
            {
              node_current = &node_random;
              starting = false;
            }
            else
            {
              node_current = node_head;
              starting = false;
            }

            npc_delay = 0;
          }
        }
      }
    }
    else
    {
      /* Handle tracking */
      if(track_state != NOTRACK)
      {
        /* Delta X/Y distances */
        int delta_x = 0;
        if(player->getTileX() >= getTileX())
          delta_x = player->getTileX() - getTileX();
        else
          delta_x = getTileX() - player->getTileX();
        int delta_y = 0;
        if(player->getTileY() >= getTileY())
          delta_y = player->getTileY() - getTileY();
        else
          delta_y = getTileY() - player->getTileY();

        /* Main delta */
        int delta = 0;
        if(delta_x >= delta_y)
          delta = delta_x + delta_y * 0.4;
        else
          delta = delta_y + delta_x * 0.4;
        //std::cout << "Location: " << player->getTileX() << "," 
        //          << player->getTileY() << "," << getTileX() << ","
        //          << getTileY() << "," << delta << std::endl;

        /* Logic for when NPC is not currently tracking */
        if(!tracking)
        {
          /* Check if tracking should be enabled */
          if(delta < track_dist)
          {
            tracking = true;
            node_previous = node_current;
            node_current = &node_player;
          }
        }
        /* Otherwise it is tracking - handle */
        else
        {
          /* Check if tracking should be disabled */
          if(delta > track_dist_max)
          {
            tracking = false;
            node_current = node_previous;
            node_previous = nullptr;
          }
        }
      }

      /* If tracking, modify how movement is handled */
      if(tracking)
      {
        /* Update location - TODO: needs to be refined */
        node_player.x = player->getTileX();
        node_player.y = player->getTileY();
      }
      /* On tile if not moving so handle pauses or shifts */
      else if(!isMoving() && (direction == Direction::DIRECTIONLESS ||
              node_state == RANDOM || node_state == RANDOMRANGE))
      {
        if(node_current->delay > npc_delay)
        {
          npc_delay += cycle_time;
        }
        else
        {
          if(node_state == LOOPED)
          {
            node_current = node_current->next;
          }
          else if(node_state == BACKANDFORTH)
          {
            /* Check to see if the ends are reached */
            if(moving_forward && node_current->next == node_head)
              moving_forward = false;
            else if(!moving_forward && node_current == node_head)
              moving_forward = true;

            /* Move in the new / old direction */
            if(moving_forward)
              node_current = node_current->next;
            else
              node_current = node_current->previous;
          }
          else if(node_state == RANDOM ||
                  (node_state == RANDOMRANGE && node_head != NULL))
          {
            /* If direction is not directionless, reset random tile location */
            if(direction != Direction::DIRECTIONLESS)
            {
              node_random.x = tile_main.front().front()->getX();
              node_random.y = tile_main.front().front()->getY();
            }

            /* Randomize a new location */
            randomizeNode();
            //node_random.x = player->getTileX();
            //node_random.y = player->getTileY();
          }
          npc_delay = 0;
        }
      }
      else
      {
        npc_delay = 0;
      }
    }

    /* Update the new direction */
    if(getMoveRequest() != direction)
    {
      clearAllMovement();
      addDirection(direction);
    }
  }

  /* Checks on the forced interaction delays and holds */
  if(forced_recent && getTarget() == nullptr)
  {
    forced_time += cycle_time;
    if(forced_time > kFORCED_NOTRIGGER)
      forced_recent = false;
  }

  MapPerson::update(cycle_time, tile_set);

  //std::cout << "BOUND (" << getID() << "): " << node_rect.x << "," // TODO: Working
  //          << node_rect.y << "," << node_rect.w << "," << node_rect.h 
  //          << std::endl;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the node state converted into a string.
 *
 * Inputs: NodeState state - the state of the node to convert
 * Output: std::string - the converted string
 */
std::string MapNPC::getNodeString(NodeState state)
{
  std::string node = "";

  if(state == LOOPED)
    node = "looped";
  else if(state == BACKANDFORTH)
    node = "backandforth";
  else if(state == RANDOMRANGE)
    node = "randomrange";
  else if(state == RANDOM)
    node = "random";
  else if(state == LOCKED)
    node = "locked";

  return node;
}

/*
 * Description: Returns the tracking state converted into a string.
 *
 * Inputs: NodeState state - the tracking state of the npc to convert
 * Output: std::string - the converted string
 */
std::string MapNPC::getTrackingString(TrackingState state)
{
  std::string node = "";

  if(state == NOTRACK)
    node = "none";
  else if(state == TOPLAYER)
    node = "toplayer";
  else if(state == AVOIDPLAYER)
    node = "avoidplayer";

  return node;
}
