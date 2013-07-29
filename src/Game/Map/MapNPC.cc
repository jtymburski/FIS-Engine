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
}

/* Another constructor function */
MapNPC::MapNPC(int width, int height, QString name, QString description, int id)
      : MapPerson(width, height, name, description, id)
{
  /* Clear the path pointers */
  current = 0;
  head = 0;
}

/* Destructor function */
MapNPC::~MapNPC()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Clears out the NPC construct, void of painting */
void MapNPC::clear()
{

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
    /* Otherwise, drop it behind the first, if the index is in range */
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
      current = head;
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

int MapNPC::getPathLength()
{
  int size = 1;
  Path* temp_node = head;
  
  if(head == 0)
    size = 0;
  else
  {
    while(temp_node->next != head)
      size++;
  }
  
  return size;
}

/* Path nodes removal handling */
bool MapNPC::removeAllNodes()
{

}

bool MapNPC::removeNode(int index)
{

}

bool MapNPC::removeNodeAtTail()
{

}
