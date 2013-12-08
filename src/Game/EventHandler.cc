/******************************************************************************
* Class Name: EventHandler
* Date Created: September 12, 2013
* Inheritance: none
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
  event_queue.clear();
  queue_index = 0;
}

/* Destructor function */
EventHandler::~EventHandler()
{
  event_queue.clear();
  queue_index = 0;
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
  convo.category = DialogCategory::TEXT;
  convo.action_event = createBlankEvent();
  convo.next.clear();

  return convo;
}

/* Creates the initial event template, clearing it */
Event EventHandler::createEventTemplate()
{
  Event blank_event;
  blank_event.classification = EventClassifier::NOEVENT;
  blank_event.convo = NULL;
  blank_event.ints.clear();

  return blank_event;
}
  
/* Returns the conversation at the given index */
Conversation* EventHandler::getConversation(Conversation* reference, 
                                            std::vector<std::string> index_list)
{
  Conversation* convo = reference;

  for(uint16_t i = 1; i < index_list.size(); i++)
  {
    uint16_t convo_index = std::stoi(index_list[i]);
    
    while(convo->next.size() < convo_index)
      convo->next.push_back(createEmptyConversation());

    convo = &convo->next[convo_index-1];
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
    std::string element = data.getElement(index);

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
  new_event.classification = EventClassifier::STARTCONVO;

  /* Use the existing conversation if it exists. Otherwise create new one */
  if(new_conversation != 0)
    new_event.convo = new_conversation;
  else
  { // TODO: Redundant to createEmptyConvo() function above
    new_event.convo = new Conversation;
    new_event.convo->text = "";
    new_event.convo->thing_id = -1;
    new_event.convo->category = DialogCategory::TEXT;
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
  new_event.classification = EventClassifier::RUNBATTLE;

  return new_event;
}

/* Creates a teleport event */
Event EventHandler::createTeleportEvent(int thing_id, int tile_x, 
                                        int tile_y, int section_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::TELEPORTTHING;

  /* Fill in the event specific information */
  new_event.ints.push_back(thing_id);
  new_event.ints.push_back(tile_x);
  new_event.ints.push_back(tile_y);
  new_event.ints.push_back(section_id);

  return new_event;
}

/* Deletes the given event. Just clears the relevant memory 
 * Returns a blank event */
Event EventHandler::deleteEvent(Event event)
{
  /* Delet the existing event, if relevant */
  if(event.convo != NULL)
    delete event.convo;
  event.convo = NULL ;

  return createBlankEvent();
}

/* Execute the given event - done through signal emits */
void EventHandler::executeEvent(Event event, MapPerson* initiator, 
                                             MapThing* source)
{
  /* Create the executed event queue entry */
  EventExecution executed_event;
  executed_event.event = event;
  executed_event.initiator = initiator;
  executed_event.source = source;
  
  /* Push the event to the back of the queue */
  event_queue.push_back(executed_event);
}

/* Executes a pickup item event */
void EventHandler::executePickup(MapItem* item, bool walkover)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::PICKUPITEM;
  new_event.ints.push_back(walkover);
  
  /* Now execute the pickup event (throw it on the queue) */
  executeEvent(new_event, NULL);
  event_queue.back().item = item;
}

/* Clears the event handling poll queue */
void EventHandler::pollClear()
{
  event_queue.clear();
  queue_index = 0;
}

/* Poll a conversation event. Only true if this event is next on queue */
bool EventHandler::pollConversation(Conversation** convo, MapPerson** initiator, 
                                                          MapThing** source)
{
  if(pollEventType() == EventClassifier::STARTCONVO && convo != NULL && 
     initiator != NULL && source != NULL)
  {
    *convo = event_queue[queue_index].event.convo;
    *initiator = event_queue[queue_index].initiator;
    *source = event_queue[queue_index].source;
    
    return true;
  }
  
  return false;
}

/* Poll a give item event */
bool EventHandler::pollGiveItem(int* id, int* count)
{
  if(pollEventType() == EventClassifier::GIVEITEM && id != NULL && 
     count != NULL && event_queue[queue_index].event.ints.size() == 2)
  {
    *id = event_queue[queue_index].event.ints[0];
    *count = event_queue[queue_index].event.ints[1];
    
    return true;
  }
  
  return false;
}

/* It goes to the next event in the poll sequence. Returns true if there is
 * one */
bool EventHandler::pollEvent()
{
  /* Update the index if it's still under the queue size */
  if(queue_index < event_queue.size())
    queue_index++;
  
  /* Check if the poll is complete */
  if(queue_index >= event_queue.size())
    return false;
  return true;
}

/* Returns the current event type, to be polled by the management class */
EventClassifier EventHandler::pollEventType()
{
  if(queue_index < event_queue.size())
    return event_queue[queue_index].event.classification;
  return EventClassifier::NOEVENT;
}

/* Poll a pickup item event */
bool EventHandler::pollPickupItem(MapItem** item, bool* walkover)
{
  if(pollEventType() == EventClassifier::PICKUPITEM && item != NULL && 
     walkover != NULL && event_queue[queue_index].event.ints.size() == 1)
  {
    *item = event_queue[queue_index].item;
    *walkover = event_queue[queue_index].event.ints[0];
    
    return true;
  }
  
  return false;
}

/* Poll a start battle event */
bool EventHandler::pollStartBattle()
{
  if(pollEventType() == EventClassifier::RUNBATTLE)
    return true;
  return false;
}

/* Poll a teleport thing event */
bool EventHandler::pollTeleportThing(int* thing_id, int* x, int* y, 
                                                    int* section_id)
{
  EventClassifier type = pollEventType();
  if(type == EventClassifier::TELEPORTTHING && thing_id != NULL &&
     x != NULL && y != NULL && section_id != NULL && 
     event_queue[queue_index].event.ints.size() == 4)
  {
    *thing_id = event_queue[queue_index].event.ints[0];
    *x = event_queue[queue_index].event.ints[1];
    *y = event_queue[queue_index].event.ints[2];
    *section_id = event_queue[queue_index].event.ints[3];
    
    return true;
  }
  
  return false;
}

Event EventHandler::EventHandler::updateEvent(Event event, XmlData data, 
                                                           int file_index, 
                                                           int section_index)
{
  EventClassifier category = EventClassifier::NOEVENT;
  std::string category_str = data.getElement(file_index);

  /* Determine the category of the event that is being updated */
  if(category_str == "giveitem")
    category = EventClassifier::GIVEITEM;
  else if(category_str == "startbattle")
    category = EventClassifier::RUNBATTLE;
  else if(category_str == "startmap")
    category = EventClassifier::RUNMAP;
  else if(category_str == "teleportthing")
    category = EventClassifier::TELEPORTTHING;
  else if(category_str == "conversation")
    category = EventClassifier::STARTCONVO;

  /* If the category doesn't match, create a new event */
  if(category != event.classification)
  {
    event = deleteEvent(event);
    if(category == EventClassifier::STARTCONVO)
      event = createConversationEvent();
  }

  /* Proceed to set up the event with the marked changes */
  if(category == EventClassifier::GIVEITEM)
  {
    // TODO
  }
  else if(category == EventClassifier::RUNBATTLE)
  {
    event = createStartBattleEvent();
  }
  else if(category == EventClassifier::RUNMAP)
  {
    // TODO
  }
  else if(category == EventClassifier::TELEPORTTHING)
  {
    /* Split the key value for the coord and id (id,x,y,section_id) */
    std::vector<std::string> list = Helpers::split(data.getDataString(), ',');
    
    /* If the list has enough elements, create the event */
    if(list.size() == 4)
      event = createTeleportEvent(std::stoi(list[0]), std::stoi(list[1]),
                                  std::stoi(list[2]), std::stoi(list[3]));
  }
  else if(category == EventClassifier::STARTCONVO)
  {
    /* Split the key value for conversation (Eg. index="1.2.1.1") */
    std::vector<std::string> index_list = 
                              Helpers::split(data.getKeyValue(file_index), '.');

    /* If size is only one, the conversation is the first item */
    if(index_list.size() == 1)
    {
      setConversationValues(event.convo, data, file_index + 1, section_index);
    }
    /* Otherwise, it's embedded in the conversation stack */
    else if(index_list.size() > 1)
    {
      Conversation* edited_convo = getConversation(event.convo, index_list);
      setConversationValues(edited_convo, data, file_index + 1, section_index);
    }
  }

  return event;
}
