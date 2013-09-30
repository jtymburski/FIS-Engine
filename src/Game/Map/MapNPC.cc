/******************************************************************************
* Class Name: MapNPC
* Date Created: Dec 2 2012
* Inheritance: MapPerson
* Description: The MapNPC class, this covers all AI found on the map in game
* TODO: Reimplement the movement functions based on NPC scripts
******************************************************************************/
#include "Game/Map/MapNPC.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapNPC::MapNPC() : MapPerson()
{
  /* Clear the path pointers */
  current = 0;
  head = 0;
  moving_forward = true;
  npc_delay = 0;
  state = LOOPED;
}

/* Another constructor function */
MapNPC::MapNPC(int width, int height, QString name, QString description, int id)
      : MapPerson(width, height, name, description, id)
{
  /* Clear the path pointers */
  current = 0;
  head = 0;
  moving_forward = true;
  npc_delay = 0;
  state = LOOPED;
}

/* Destructor function */
MapNPC::~MapNPC()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the class descriptor, useful for casting */
QString MapNPC::classDescriptor()
{
  return "MapNPC";
}

/* Clears out the NPC construct, void of painting */
void MapNPC::clear()
{
  /* Clear out all the nodes */
  removeAllNodes();
  head = 0;
  current = 0;

  /* Clear out other variables */
  npc_delay = 0;

  /* Clear out parent */
  MapPerson::clear();
}

bool MapNPC::insertNode(int index, Tile* tile, int delay)
{
  bool success = false;
  
  if(index >= 0 && tile != 0 && delay >= 0)
  {
    /* Set up the node structure */
    Path* new_node = new Path();
    new_node->tile = tile;
    new_node->delay = delay;
    
    /* If the head is 0 and the index is 0, drop it in the first slot */
    if(head == 0 && index == 0)
    {
      new_node->previous = new_node;
      new_node->next = new_node;
      head = new_node;
      success = true;
    }
    /* If it's at the end of the 
     * Otherwise, drop it behind the first, if the index is in range */
    else if(index <= getPathLength())
    {
      Path* temp_node = head;
      success = true;

      /* Loop through to find the insertion point */
      for(int i = 0; i < index; i++)
        temp_node = temp_node->next;
      
      /* Re-reference the double linked list pointers */
      temp_node->previous->next = new_node;
      new_node->previous = temp_node->previous;
      temp_node->previous = new_node;
      new_node->next = temp_node;
      
      /* If the index is at 0, reset the head */
      if(index == 0)
        head = new_node;
    }
    
    /* If successful, reset the current node ptr of the class */
    if(success)
      resetMovement();
  }
 
  return success;
}

bool MapNPC::insertNodeAtTail(Tile* tile, int delay)
{
  return insertNode(getPathLength(), tile, delay);
}

/* Gets a pointer to the Persons conversation */
QList<QString> MapNPC::getConversation()
{
  return talking_points;
}

/* Gets a pointer to the gift the NPC has */
MapThing* MapNPC::getGift()
{
  return gift;
}

/* Returns the node movement state - how it traverses */
MapNPC::NodeState MapNPC::getNodeState()
{
  return state;
}

int MapNPC::getPathLength()
{
  int size = 1;
  Path* temp_node = head;
  
  if(head == 0)
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

/* Path nodes removal handling */
bool MapNPC::removeAllNodes()
{
  /* Loop through all nodes and remove them */
  while(removeNodeAtTail());
  return true;
}

bool MapNPC::removeNode(int index)
{
  int length = getPathLength();
  bool success = false;
  Path* temp_node = 0;

  if(index == 0 && length == 1)
  {
    temp_node = head;
    head = 0;
    current = 0;
    success = true;
  }
  else if(index >= 0 && index < length)
  {
    /* Loop through to find the node to remove */
    temp_node = head;
    for(int i = 0; i < index; i++)
      temp_node = temp_node->next;

    /* Reset the pointers around the path to delete */
    temp_node->previous->next = temp_node->next;
    temp_node->next->previous = temp_node->previous;

    /* Fix the pointers if they need to be changed */
    if(index == 0)
      head = temp_node->next;
    if(current == temp_node)
      resetMovement();

    success = true;
  }

  /* Finish by deleting the pointer */
  delete temp_node;

  return success;
}

bool MapNPC::removeNodeAtTail()
{
  return removeNode(getPathLength() - 1);
}

void MapNPC::resetMovement()
{
  current = head;

  if(current != 0)
    setStartingTile(getMapSection(), current->tile);
}

/* Sets the node movement state - how it traverses */
void MapNPC::setNodeState(NodeState state)
{
  this->state = state;
  
  if(state == BACKANDFORTH)
    moving_forward = true;
}

void MapNPC::updateThing(float cycle_time, Tile* next_tile)
{
  /* Some initial parameters */
  EnumDb::Direction direction = EnumDb::DIRECTIONLESS;

  /* Begin the check to handle each time the NPC is on a tile */
  if(current != 0)
  {
    /* If the tile isn't set, try and set the starting point */
    if(getTile() == 0)
      setStartingTile(getMapSection(), current->tile);
    /* Otherwise, it is set so check if it's almost on the tile */
    else
    {
      int delta_x = current->tile->getX() - tile_main->getX();
      int delta_y = current->tile->getY() - tile_main->getY();

      /* If the npc needs to move on the X plane */
      if(delta_x != 0)
      {
        if(delta_x < 0)
          direction = EnumDb::WEST;
        else
          direction = EnumDb::EAST;
      }
      /* Else if the npc needs to move on the Y plane */
      else if(delta_y != 0)
      {
        if(delta_y < 0)
          direction = EnumDb::NORTH;
        else
          direction = EnumDb::SOUTH;
      }
      /* Otherwise, on tile if not moving so handle pauses or shifts */
      else if(!isMoving())
      {
        if(current->delay > npc_delay)
          npc_delay += cycle_time;
        else
        {
          if(state == LOOPED)
          {
            current = current->next;
            npc_delay = 0;
          }
          else if(state == BACKANDFORTH)
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
  }

  MapPerson::updateThing(cycle_time, next_tile);
}
