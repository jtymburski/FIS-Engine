/*******************************************************************************
 * Class Name: MapNPC
 * Date Created: Dec 2 2012
 * Inheritance: MapPerson
 * Description: The MapNPC class, this covers all AI found on the map in game
 *
 * TODO: 1. Reimplement the movement functions based on NPC scripts
 *       2. COMMENT
 ******************************************************************************/
#include "Game/Map/MapNPC.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapNPC::MapNPC() : MapPerson()
{
  /* Clear the path pointers */
  current = NULL;
  head = NULL;
  moving_forward = true;
  npc_delay = 0;
  state = LOOPED;
}

/* Another constructor function */
MapNPC::MapNPC(int id, std::string name, std::string description)
      : MapPerson(id, name, description)
{
  /* Clear the path pointers */
  current = NULL;
  head = NULL;
  moving_forward = true;
  npc_delay = 0;
  state = LOOPED;
}

/* Destructor function */
MapNPC::~MapNPC()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Appends an empty node onto the back of the movement stack */
void MapNPC::appendEmptyNode()
{
  /* Set up the node structure */
  Path* new_node = new Path();
  new_node->x = 0;
  new_node->y = 0;
  new_node->delay = 0;
  
  /* Append the node to the end of the list */
  if(!insertNode(getPathLength(), new_node))
    delete new_node;
}

/* Returns the node at the index */
Path* MapNPC::getNode(uint16_t index)
{
  bool failed = false;
  Path* returned_node = NULL;
  
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
  
  /* Execute the return sequence, if applicable */
  if(failed)
    returned_node = NULL;
  return returned_node;
}

/* Inserts a node, at the given index */
bool MapNPC::insertNode(uint16_t index, Path* node)
{
  bool success = false;
  
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
    }
    else
    {
      success = false;
    }
  }
  
  /* Proceed to parent */
  return (success && MapPerson::addThingInformation(data, file_index, 
                                                    section_index, renderer, 
                                                    base_path));
}

/* Returns the class descriptor, useful for casting */
std::string MapNPC::classDescriptor()
{
  return "MapNPC";
}

/* Clears out the NPC construct, void of painting */
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

bool MapNPC::insertNodeAtTail(uint16_t x, uint16_t y, uint16_t delay)
{
  return insertNode(getPathLength(), x, y, delay);
}

/* Returns the node movement state - how it traverses */
MapNPC::NodeState MapNPC::getNodeState()
{
  return state;
}

uint16_t MapNPC::getPathLength()
{
  uint16_t size = 1;
  Path* temp_node = head;
  
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
  
  return size;
}

/* Returns the predicted move request in the class */
Direction MapNPC::getPredictedMoveRequest()
{
  if(current != NULL)
  {
    int delta_x = 0;//current->x - tile_main->getX(); // TODO: Fix
    int delta_y = 0;//current->y - tile_main->getY();
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
  
/* Path nodes removal handling */
bool MapNPC::removeAllNodes()
{
  while(removeNodeAtTail());
  return true;
}

bool MapNPC::removeNode(uint16_t index)
{
  uint16_t length = getPathLength();
  bool success = false;
  Path* temp_node = NULL;

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
  
  /* Finish by deleting the pointer */
  if(success)
    delete temp_node;

  return success;
}

bool MapNPC::removeNodeAtTail()
{
  return removeNode(getPathLength() - 1);
}

/* Sets the node movement state - how it traverses */
void MapNPC::setNodeState(NodeState state)
{
  this->state = state;
  
  if(state == BACKANDFORTH)
    moving_forward = true;
}

void MapNPC::update(int cycle_time, std::vector<std::vector<Tile*>> tile_set)
{
  /* Some initial parameters */
  Direction direction = Direction::DIRECTIONLESS;

// TODO: Fix
//  /* Begin the check to handle each time the NPC is on a tile */
//  if(current != NULL && tile_main != NULL)
//  {
//    int delta_x = current->x - tile_main->getX();
//    int delta_y = current->y - tile_main->getY();
//    
//    /* If the npc needs to move on the X plane */
//    if(delta_x != 0)
//    {
//      if(delta_x < 0)
//        direction = Direction::WEST;
//      else
//        direction = Direction::EAST;
//    }
//    /* Else if the npc needs to move on the Y plane */
//    else if(delta_y != 0)
//    {
//      if(delta_y < 0)
//        direction = Direction::NORTH;
//      else
//        direction = Direction::SOUTH;
//    }
//    /* Otherwise, on tile if not moving so handle pauses or shifts */
//    else if(!isMoving())
//    {
//      if(current->delay > npc_delay)
//        npc_delay += cycle_time;
//      else
//      {
//        if(state == LOOPED)
//        {
//          current = current->next;
//          npc_delay = 0;
//        }
//        else if(state == BACKANDFORTH)
//        {
//          /* Check to see if the ends are reached */
//          if(moving_forward && current->next == head)
//            moving_forward = false;
//          else if(!moving_forward && current == head)
//            moving_forward = true;
//          
//          /* Move in the new / old direction */
//          if(moving_forward)
//            current = current->next;
//          else
//            current = current->previous;
//          npc_delay = 0;
//        }
//      }
//    }
//
//    /* Update the new direction */
//    if(getMoveRequest() != direction)
//    {
//      clearAllMovement();
//      addDirection(direction);
//    }
//  }

  MapPerson::update(cycle_time, tile_set);
}
