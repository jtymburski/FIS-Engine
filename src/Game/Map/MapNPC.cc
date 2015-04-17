/*******************************************************************************
 * Class Name: MapNPC
 * Date Created: Dec 2 2012
 * Inheritance: MapPerson
 * Description: The MapNPC class, this covers all AI found on the map in game
 *
 * TODO: 1. Reimplement the movement functions based on NPC scripts
 ******************************************************************************/
#include "Game/Map/MapNPC.h"

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
  /* Clear the path pointers */
  current = NULL;
  head = NULL;
  forced_interaction = false;
  moving_forward = true;
  node_state = LOOPED;
  nodes_delete = true;
  npc_delay = 0;
  tracking_state = NOTRACK;
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
  /* Clear the path pointers */
  current = NULL;
  head = NULL;
  forced_interaction = false;
  moving_forward = true;
  node_state = LOOPED;
  nodes_delete = true;
  npc_delay = 0;
  tracking_state = NOTRACK;
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
 * Description: Returns the node at the given index. NULL if out of range.
 *
 * Inputs: uint16_t index - the node index, offset from HEAD
 * Output: Path* - the path structure at the node index
 */
Path* MapNPC::getNode(uint16_t index)
{
  bool failed = false;
  Path* returned_node = NULL;
 
  if(base != NULL && base_category == ThingBase::NPC)
  {
    returned_node = static_cast<MapNPC*>(base)->getNode(index);
  }
  else
  {
    if(head != NULL)
    {
      /* Loop through to find the node */
      returned_node = head;
      for(uint16_t i = 0; i < index; i++)
      {
        returned_node = returned_node->next;
        if(returned_node == head)
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
 
  /* Initial checks to see if nodes are set by base class */
  if(!nodes_delete)
  {
    head = NULL;
    current = NULL;
    nodes_delete = true;
  }

  /* Only proceed if node is non-null */
  if(node != NULL)
  {
    /* If the head is 0 and the index is 0, drop it in the first slot */
    if(head == NULL && index == 0)
    {
      node->previous = node;
      node->next = node;
      head = node;
      success = true;
    }
    /* If it's at the end of the 
     * Otherwise, drop it behind the first, if the index is in range */
    else if(index <= getPathLength())
    {
      Path* temp_node = head;
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
        head = node;
    }
    
    /* If successful, reset the current node ptr of the class */
    if(success)
    {
      current = head;
      resetPosition();
    }
  }

  return success;
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
          node->x = x;
      }
      else if(elements[1] == "y")
      {
        uint16_t y = data.getDataInteger(&success);
        if(success)
          node->y = y;
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
      success = false;
    }
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
  head = NULL;
  current = NULL;

  /* Clear out other variables */
  npc_delay = 0;

  /* Clear out parent */
  MapPerson::clear();
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
  Path* temp_node = head;
 
  /* Check if base is used or not */
  if(base != NULL && base_category == ThingBase::NPC)
  {
    size = static_cast<MapNPC*>(base)->getPathLength();
  }
  else
  {
    if(head == NULL)
      size = 0;
    else
    {
      while(temp_node->next != head)
      {
        temp_node = temp_node->next;
        size++;
      }
    }
  }
  
  return size;
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
  if(isTilesSet() && current != NULL)
  {
    int delta_x = current->x - tile_main.front().front()->getX();
    int delta_y = current->y - tile_main.front().front()->getY();
    Direction direction = Direction::DIRECTIONLESS;
    
    /* If the npc needs to move on the X plane */
    if(delta_x != 0)
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
  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
    return static_cast<MapNPC*>(base)->tracking_state;
  return tracking_state;
}

/*
 * Description: Returns if the NPC will force interaction upon the player if it
 *              moves within the vicinity of the NPC.
 *
 * Inputs: none
 * Output: bool - true if the NPC will force interaction
 */
bool MapNPC::isForcedInteraction()
{
  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
    return static_cast<MapNPC*>(base)->forced_interaction;
  return forced_interaction;
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
    head = NULL;
    current = NULL;
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
      temp_node = head;
      head = NULL;
      current = NULL;
      success = true;
    }
    else if(index < length)
    {
      /* Loop through to find the node to remove */
      temp_node = head;
      for(uint16_t i = 0; i < index; i++)
        temp_node = temp_node->next;

      /* Reset the pointers around the path to delete */
      temp_node->previous->next = temp_node->next;
      temp_node->next->previous = temp_node->previous;

      /* Fix the pointers if they need to be changed */
      if(index == 0)
        head = temp_node->next;
      if(current == temp_node)
        resetPosition();

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
  if(head != NULL)
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
    if(base != NULL && base->classDescriptor() == "MapNPC")
    {
      this->base = base;
      base_category = ThingBase::NPC;
      setMatrix(getState(getSurface(), getDirection()));
      if(head == NULL)
      {
        head = static_cast<MapNPC*>(base)->head;
        current = head;
        nodes_delete = false;
      }
      success = true;
    }
    else if(base == NULL)
    {
      this->base = NULL;
      base_category = ThingBase::ISBASE;
      success = true;
    }
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
  
  if(state == BACKANDFORTH)
    moving_forward = true;
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
  tracking_state = state;
}

/*
 * Description: Updates the frames of the NPC. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the ms time to update the movement/animation
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 * Output: none 
 */
// TODO: Add forced interaction, tracking state, and additional movement nodes
void MapNPC::update(int cycle_time, std::vector<std::vector<Tile*>> tile_set)
{
  /* Some initial parameters */
  Direction direction = Direction::DIRECTIONLESS;

  /* Begin the check to handle each time the NPC is on a tile */
  if(isTilesSet() && current != NULL)
  {
    int delta_x = current->x - tile_main.front().front()->getX();
    int delta_y = current->y - tile_main.front().front()->getY();
    
    /* If the npc needs to move on the X plane */
    if(delta_x != 0)
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
    /* Otherwise, on tile if not moving so handle pauses or shifts */
    else if(!isMoving())
    {
      if(current->delay > npc_delay)
        npc_delay += cycle_time;
      else
      {
        if(node_state == LOOPED)
        {
          current = current->next;
          npc_delay = 0;
        }
        else if(node_state == BACKANDFORTH)
        {
          /* Check to see if the ends are reached */
          if(moving_forward && current->next == head)
            moving_forward = false;
          else if(!moving_forward && current == head)
            moving_forward = true;
          
          /* Move in the new / old direction */
          if(moving_forward)
            current = current->next;
          else
            current = current->previous;
          npc_delay = 0;
        }
      }
    }

    /* Update the new direction */
    if(getMoveRequest() != direction)
    {
      clearAllMovement();
      addDirection(direction);
    }
  }

  MapPerson::update(cycle_time, tile_set);
}
