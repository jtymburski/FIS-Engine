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
EventHandler::Event EventHandler::createEventTemplate()
{
  Event blank_event;
  blank_event.classification = NOEVENT;
  blank_event.convo = 0;
  blank_event.integer_stack.clear();

  return blank_event;
}

void EventHandler::executeConversationEvent(Conversation* convo, 
                                            MapPerson* initiator, 
                                            MapThing* source)
{
  if(convo != 0)
    emit initConversation(convo, initiator, source);
}

/* Execute a start battle event */
void EventHandler::executeStartBattleEvent(Event event, MapPerson* target)
{
  (void)event;
  (void)target;

  emit startBattle();
}
  
/* Execute the teleport event */
void EventHandler::executeTeleportEvent(Event event, MapPerson* target)
{
  int x = -1;
  int y = -1;
  int section_id = -1;
  
  if(event.integer_stack.size() == 3)
  {
    x = event.integer_stack[0];
    y = event.integer_stack[1];
    section_id = event.integer_stack[2];
  }

  if(x >= 0 && y >= 0)
    emit teleportThing(target, x, y, section_id);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Creates a disabled blank event */
EventHandler::Event EventHandler::createBlankEvent()
{
  return createEventTemplate();
}
  
/* Creates the conversation initiation event */
EventHandler::Event EventHandler::createConversationEvent(
                                               Conversation* new_conversation)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = STARTCONVO;
  new_event.convo = new_conversation;

  return new_event;
}

/* Creates a start battle event */
/* TODO: Add parameters. Battle not ready */
EventHandler::Event EventHandler::createStartBattleEvent()
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = RUNBATTLE;

  return new_event;
}

/* Creates a teleport event */
EventHandler::Event EventHandler::createTeleportEvent(int tile_x, int tile_y, 
                                                      int section_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = TELEPORTPLAYER;

  /* Fill in the event specific information */
  new_event.integer_stack.append(tile_x);
  new_event.integer_stack.append(tile_y);
  new_event.integer_stack.append(section_id);

  return new_event;
}

/* Execute the given event - done through signal emits */
void EventHandler::executeEvent(Event event, MapPerson* initiator, 
                                             MapThing* source)
{
  if(event.classification == TELEPORTPLAYER)
    executeTeleportEvent(event, initiator); // TODO: Fix? Let map decide player
  else if(event.classification == RUNBATTLE)
    executeStartBattleEvent(event, initiator);
  else if(event.classification == STARTCONVO)
    executeConversationEvent(event.convo, initiator, source);
}

/* Executes a pickup item event */
void EventHandler::executePickup(MapItem* item, bool walkover)
{
  emit pickupItem(item, walkover);
}