/******************************************************************************
* Class Name: EventHandler
* Date Created: September 12, 2013
* Inheritance: QObject
* Description: This event handler contains all events between the multiple 
*              classes and manages data transfer between from calls internally.
*              It is merely meant as a facilitator of actions that need to
*              happen deep inside of the nested structure and then need to 
*              fire from the top of the chain.
******************************************************************************/
#include "Game/EventHandler.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventHandler::EventHandler()
{
}

/* Destructor function */
EventHandler::~EventHandler()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the initial event template, clearing it */
Event EventHandler::createEventTemplate()
{
  Event blank_event;
  blank_event.handler = this;
  blank_event.classification = EnumDb::NOEVENT;
  blank_event.integer_stack.clear();

  return blank_event;
}

/* Execute the teleport event */
void EventHandler::executeTeleportEvent(Event event, int target_id)
{
  int x = -1;
  int y = -1;

  if(event.integer_stack.size() == 2)
  {
    x = event.integer_stack[0];
    y = event.integer_stack[1];
  }

  if(x >= 0 && y >= 0)
    emit teleportThing(target_id, x, y);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Creates a disabled blank event */
Event EventHandler::createBlankEvent()
{
  return createEventTemplate();
}

/* Creates a teleport event */
Event EventHandler::createTeleportEvent(int tile_x, int tile_y)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EnumDb::TELEPORTPLAYER;

  /* Fill in the event specific information */
  new_event.integer_stack.append(tile_x);
  new_event.integer_stack.append(tile_y);

  return new_event;
}

/* Execute the given event - done through signal emits */
void EventHandler::executeEvent(Event event, int target_id)
{
  if(event.classification == EnumDb::TELEPORTPLAYER)
    executeTeleportEvent(event, target_id);
}
