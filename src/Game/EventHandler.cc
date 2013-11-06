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
  
/* Creates an empty conversation */
Conversation EventHandler::createEmptyConversation()
{
  Conversation convo;
  convo.text = "";
  convo.thing_id = -1;
  convo.category = EnumDb::TEXT;
  convo.action_event = createBlankEvent();
  convo.next.clear();

  return convo;
}

/* Creates the initial event template, clearing it */
Event EventHandler::createEventTemplate()
{
  Event blank_event;
  blank_event.classification = EnumDb::NOEVENT;
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
void EventHandler::executeTeleportPlayerEvent(Event event, MapPerson* target)
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
  
/* Returns the conversation at the given index */
Conversation* EventHandler::getConversation(Conversation* reference, 
                                            QStringList index_list)
{
  Conversation* convo = reference;

  for(int i = 1; i < index_list.size(); i++)
  {
    while(convo->next.size() < index_list[i].toInt())
      convo->next.append(createEmptyConversation());

    convo = &convo->next[index_list[i].toInt()-1];
  }
  
  return convo;
}

/* Sets the conversation values of the pointed object, based on the XML
 * file data */
void EventHandler::setConversationValues(Conversation* reference, XmlData data, 
                                         int index, int section_index)
{
  /* Only proceed if the reference convo is not NULL */
  if(reference != 0)
  {
    QString element = data.getElement(index);

    /* Determine what part of the conversation to edit */
    if(element == "text")
      reference->text = data.getDataString();
    else if(element == "id")
      reference->thing_id = data.getDataInteger();
    else if(element == "event")
      reference->action_event = updateEvent(reference->action_event, data, 
                                            index + 1, section_index);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Creates a disabled blank event */
Event EventHandler::createBlankEvent()
{
  return createEventTemplate();
}
  
/* Creates the conversation initiation event */
Event EventHandler::createConversationEvent(Conversation* new_conversation)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EnumDb::STARTCONVO;

  /* Use the existing conversation if it exists. Otherwise create new one */
  if(new_conversation != 0)
    new_event.convo = new_conversation;
  else
  {
    new_event.convo = new Conversation;
    new_event.convo->text = "";
    new_event.convo->thing_id = -1;
    new_event.convo->category = EnumDb::TEXT;
    new_event.convo->action_event = createBlankEvent();
  }

  return new_event;
}

/* Creates a start battle event */
/* TODO: Add parameters. Battle not ready */
Event EventHandler::createStartBattleEvent()
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EnumDb::RUNBATTLE;

  return new_event;
}

/* Creates a teleport event */
Event EventHandler::createTeleportPlayerEvent(int tile_x, int tile_y, 
                                                          int section_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EnumDb::TELEPORTPLAYER;

  /* Fill in the event specific information */
  new_event.integer_stack.append(tile_x);
  new_event.integer_stack.append(tile_y);
  new_event.integer_stack.append(section_id);

  return new_event;
}

/* Deletes the given event. Just clears the relevant memory 
 * Returns a blank event */
Event EventHandler::deleteEvent(Event event)
{
  /* Delet the existing event, if relevant */
  if(event.convo != 0)
    delete event.convo;
  event.convo = 0;

  return createBlankEvent();
}

/* Execute the given event - done through signal emits */
void EventHandler::executeEvent(Event event, MapPerson* initiator, 
                                             MapThing* source)
{
  // TODO: Fix? Let map decide player??
  if(event.classification == EnumDb::TELEPORTPLAYER)
    executeTeleportPlayerEvent(event, initiator);
  else if(event.classification == EnumDb::RUNBATTLE)
    executeStartBattleEvent(event, initiator);
  else if(event.classification == EnumDb::STARTCONVO)
    executeConversationEvent(event.convo, initiator, source);
}

/* Executes a pickup item event */
void EventHandler::executePickup(MapItem* item, bool walkover)
{
  emit pickupItem(item, walkover);
}

Event EventHandler::updateEvent(Event event, XmlData data, int file_index, 
                                                          int section_index)
{
  EnumDb::EventClassifier category = EnumDb::NOEVENT;
  QString category_str = data.getElement(file_index);

  /* Determine the category of the event that is being updated */
  if(category_str == "giveitem")
    category = EnumDb::GIVEITEM;
  else if(category_str == "startbattle")
    category = EnumDb::RUNBATTLE;
  else if(category_str == "startmap")
    category = EnumDb::RUNMAP;
  else if(category_str == "teleportplayer")
    category = EnumDb::TELEPORTPLAYER;
  else if(category_str == "teleportthing")
    category = EnumDb::TELEPORTTHING;
  else if(category_str == "conversation")
    category = EnumDb::STARTCONVO;

  /* If the category doesn't match, create a new event */
  if(category != event.classification)
  {
    event = deleteEvent(event);
    if(category == EnumDb::STARTCONVO)
      event = createConversationEvent();
  }

  /* Proceed to set up the event with the marked changes */
  if(category == EnumDb::GIVEITEM)
  {
    // TODO
  }
  else if(category == EnumDb::RUNBATTLE)
  {
    event = createStartBattleEvent();
  }
  else if(category == EnumDb::RUNMAP)
  {
    // TODO
  }
  else if(category == EnumDb::TELEPORTPLAYER)
  {
    QStringList list = data.getDataString().split(",");
    if(list.size() == 3)
      event = createTeleportPlayerEvent(list[0].toInt(),
                                        list[1].toInt(), list[2].toInt());
  }
  else if(category == EnumDb::TELEPORTTHING)
  {
    // TODO
  }
  else if(category == EnumDb::STARTCONVO)
  {
    QStringList index_list = data.getKeyValue(file_index).split(".");
    if(index_list.size() == 1)
    {
      setConversationValues(event.convo, data, file_index + 1, section_index);
    }
    else if(index_list.size() > 1)
    {
      Conversation* edited_convo = getConversation(event.convo, index_list);
      setConversationValues(edited_convo, data, file_index + 1, section_index);
    }
  }

  return event;
}
