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

/* Constant Implementation - see header file for descriptions */
const uint8_t EventHandler::kGIVE_ITEM_COUNT = 1;
const uint8_t EventHandler::kGIVE_ITEM_ID = 0;
const uint8_t EventHandler::kMAP_ID = 0;
const uint8_t EventHandler::kTELEPORT_ID = 0;
const uint8_t EventHandler::kTELEPORT_SECTION = 3;
const uint8_t EventHandler::kTELEPORT_X = 1;
const uint8_t EventHandler::kTELEPORT_Y = 2;
const int32_t EventHandler::kUNSET_ID = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventHandler::EventHandler()
{
  event_queue.clear();
  queue_index = 0;
  sound_handler = nullptr;
}

/* Destructor function */
EventHandler::~EventHandler()
{
  event_queue.clear();
  queue_index = 0;
  sound_handler = nullptr;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

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

/* Sets the conversation values of the pointed object, gamd on the XML
 * file data */
void EventHandler::setConversationValues(Conversation* reference, XmlData data,
                                         int index, int section_index)
{
  /* Only proceed if the reference convo is not NULL */
  if(reference != NULL)
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
  
/* Trigger queue sound */
void EventHandler::triggerQueueSound()
{
  if(event_queue[queue_index].event.sound_id >= 0)
    triggerSound(event_queue[queue_index].event.sound_id, 
                 SoundChannels::TRIGGERS);
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
Event EventHandler::createConversationEvent(Conversation* new_conversation,
                                            int sound_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::STARTCONVO;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Use the existing conversation if it exists. Otherwise create new one */
  if(new_conversation != NULL)
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

/* Creates a give item event, with the appropriate parameters */
Event EventHandler::createGiveItemEvent(int id, int count, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::GIVEITEM;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(id);
  new_event.ints.push_back(count);

  return new_event;
}

/* Creates a notification event, that can fire and result in visible text */
Event EventHandler::createNotificationEvent(std::string notification, 
                                            int sound_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::NOTIFICATION;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Set up the rest of the event */
  new_event.strings.push_back(notification);

  return new_event;
}
  
/* Creates a sound event */
Event EventHandler::createSoundEvent(int sound_id)
{
  /* Create the new event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::JUSTSOUND;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  return new_event;
}

/* Creates a start battle event */
Event EventHandler::createStartBattleEvent(int sound_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::RUNBATTLE;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  return new_event;
}

/* Create a start map event */
Event EventHandler::createStartMapEvent(int id, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::RUNMAP;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(id);

  return new_event;
}

/* Creates a teleport event */
Event EventHandler::createTeleportEvent(int thing_id, uint16_t tile_x,
                                        uint16_t tile_y, uint16_t section_id,
                                        int sound_id)
{
  /* Create the event and identify */
  Event new_event = createEventTemplate();
  new_event.classification = EventClassifier::TELEPORTTHING;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(thing_id);
  new_event.ints.push_back(tile_x);
  new_event.ints.push_back(tile_y);
  new_event.ints.push_back(section_id);

  return new_event;
}

/* Execute the given event - done through signal emits */
void EventHandler::executeEvent(Event event, MapPerson* initiator,
                                             MapThing* source)
{
  if(event.classification != EventClassifier::NOEVENT)
  {
    /* Create the executed event queue entry */
    EventExecution executed_event;
    executed_event.event = event;
    executed_event.initiator = initiator;
    executed_event.source = source;

    /* Push the event to the back of the queue */
    event_queue.push_back(executed_event);
  }
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

//TODO: Conventions [04-10-15]
KeyHandler& EventHandler::getKeyHandler()
{
  return key_handler;
}
  
/* Returns a reference to the sound handler */
SoundHandler* EventHandler::getSoundHandler()
{
  return sound_handler;
}

/* Clears the event handling poll queue */
void EventHandler::pollClear()
{
  event_queue.clear();
  queue_index = 0;
}

/* Poll a conversation event. Only true if this event is next on queue */
bool EventHandler::pollConversation(Conversation** convo, MapThing** source)
{
  if(pollEventType() == EventClassifier::STARTCONVO && convo != NULL &&
     source != NULL)
  {
    *convo = event_queue[queue_index].event.convo;
    *source = event_queue[queue_index].source;
    triggerQueueSound();

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
    *id = event_queue[queue_index].event.ints[kGIVE_ITEM_ID];
    *count = event_queue[queue_index].event.ints[kGIVE_ITEM_COUNT];
    triggerQueueSound();

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

/* Poll a notification event */
bool EventHandler::pollNotification(std::string* notification)
{
  if(pollEventType() == EventClassifier::NOTIFICATION && notification != NULL &&
     event_queue[queue_index].event.strings.size() == 1)
  {
    *notification = event_queue[queue_index].event.strings.front();
    triggerQueueSound();

    return true;
  }

  return false;
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
  
/* Poll a sound event */
bool EventHandler::pollSound()
{
  if(pollEventType() == EventClassifier::JUSTSOUND)
  {
    triggerQueueSound();
    return true;
  }
  return false;
}

/* Poll a start battle event */
bool EventHandler::pollStartBattle(MapPerson** person, MapThing** source)
{
  if(pollEventType() == EventClassifier::RUNBATTLE &&
     person != NULL && source != NULL)
  {
    *person = event_queue[queue_index].initiator;
    *source = event_queue[queue_index].source;
    triggerQueueSound();

    return true;
  }
  return false;
}

/* Poll a start map event */
bool EventHandler::pollStartMap(int* id)
{
  if(pollEventType() == EventClassifier::RUNMAP && id != NULL &&
     event_queue[queue_index].event.ints.size() == 1)
  {
    *id = event_queue[queue_index].event.ints[kMAP_ID];
    triggerQueueSound();

    return true;
  }

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
    *thing_id = event_queue[queue_index].event.ints[kTELEPORT_ID];
    *x = event_queue[queue_index].event.ints[kTELEPORT_X];
    *y = event_queue[queue_index].event.ints[kTELEPORT_Y];
    *section_id = event_queue[queue_index].event.ints[kTELEPORT_SECTION];
    triggerQueueSound();

    return true;
  }

  return false;
}

/* Sets the sound handler used. If unset, no sounds will play */
void EventHandler::setSoundHandler(SoundHandler* new_handler)
{
  sound_handler = new_handler;
}

/* Trigger Audio Stop - on channel */
void EventHandler::triggerAudioStop(SoundChannels channel)
{
  if(sound_handler != nullptr)
  {
    sound_handler->addStopToQueue(channel);
  }
}

/* Trigger music file */
void EventHandler::triggerMusic(uint32_t id)
{
  if(sound_handler != nullptr)
  {
    sound_handler->addPlayToQueue(id, SoundChannels::MUSIC1);
  }
}

/* Trigger sound file */
void EventHandler::triggerSound(uint32_t id, SoundChannels channel)
{
  if(sound_handler != nullptr)
  {
    if(channel != SoundChannels::MUSIC1 || channel != SoundChannels::MUSIC2 ||
       channel != SoundChannels::UNASSIGNED)
    {
      sound_handler->addPlayToQueue(id, channel);
    }
  }
}

/* Trigger weather file */
void EventHandler::triggerWeather(uint32_t id)
{
  if(sound_handler != nullptr)
  {
    sound_handler->addPlayToQueue(id, SoundChannels::WEATHER1);
  }
}

/* Update the event, with load data */
Event EventHandler::EventHandler::updateEvent(Event event, XmlData data,
                                                           int file_index,
                                                           int section_index)
{
  EventClassifier category = EventClassifier::NOEVENT;
  std::string category_str = data.getElement(file_index);
  bool read_success;

  /* Determine the category of the event that is being updated */
  if(category_str == "conversation")
    category = EventClassifier::STARTCONVO;
  else if(category_str == "giveitem")
    category = EventClassifier::GIVEITEM;
  else if(category_str == "justsound")
    category = EventClassifier::JUSTSOUND;
  else if(category_str == "notification")
    category = EventClassifier::NOTIFICATION;
  else if(category_str == "startbattle")
    category = EventClassifier::RUNBATTLE;
  else if(category_str == "startmap")
    category = EventClassifier::RUNMAP;
  else if(category_str == "teleportthing")
    category = EventClassifier::TELEPORTTHING;

  /* If the category doesn't match, create a new event */
  if(category != event.classification)
  {
    event = deleteEvent(event);

    if(category == EventClassifier::GIVEITEM)
      event = createGiveItemEvent();
    else if(category == EventClassifier::JUSTSOUND)
      event = createSoundEvent();
    else if(category == EventClassifier::NOTIFICATION)
      event = createNotificationEvent();
    else if(category == EventClassifier::RUNBATTLE)
      event = createStartBattleEvent();
    else if(category == EventClassifier::RUNMAP)
      event = createStartMapEvent();
    else if(category == EventClassifier::STARTCONVO)
      event = createConversationEvent();
    else if(category == EventClassifier::TELEPORTTHING)
      event = createTeleportEvent();
  }

  /* Proceed to set up the event with the marked changes */
  if(data.getElement(file_index + 1) == "sound_id" ||
     category == EventClassifier::JUSTSOUND)
  {
    int32_t sound_id = data.getDataInteger(&read_success);
    if(read_success && sound_id >= 0)
      event.sound_id = sound_id;
  }
  else if(category == EventClassifier::GIVEITEM)
  {
    /* Parse the event classifiers */
    std::string give_item_element = data.getElement(file_index + 1);
    if(give_item_element == "id")
      event.ints.at(kGIVE_ITEM_ID) = data.getDataInteger();
    else if(give_item_element == "count")
      event.ints.at(kGIVE_ITEM_COUNT) = data.getDataInteger();
  }
  else if(category == EventClassifier::NOTIFICATION)
  {
    /* If the classifier is the only element */
    if(data.getTailElements(file_index).size() == 1 ||
       data.getElement(file_index + 1) == "text")
    {
      event.strings.at(0) = data.getDataString();
    }
  }
  //else if(category == EventClassifier::RUNBATTLE)
  //{
  //  event = createStartBattleEvent();
  //}
  else if(category == EventClassifier::RUNMAP)
  {
    /* Parse the event classifiers */
    std::string element = data.getElement(file_index + 1);
    if(element == "id")
      event.ints.at(kMAP_ID) = data.getDataInteger();
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
  else if(category == EventClassifier::TELEPORTTHING)
  {
    /* Parse the event classifiers */
    std::string teleport_element = data.getElement(file_index + 1);
    if(teleport_element == "id")
      event.ints.at(kTELEPORT_ID) = data.getDataInteger();
    if(teleport_element == "x")
      event.ints.at(kTELEPORT_X) = data.getDataInteger();
    else if(teleport_element == "y")
      event.ints.at(kTELEPORT_Y) = data.getDataInteger();
    else if(teleport_element == "section")
      event.ints.at(kTELEPORT_SECTION) = data.getDataInteger();
  }

  return event;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Copies a past in event and returns the copied version as source.
 *              Function that calls it is in charge of deleting conversation,
 *              if that's been generated.
 *
 * Inputs: Event source - the event struct to copy
 * Output: Event - the copied event
 */
Event EventHandler::copyEvent(Event source)
{
  /* Copy the event */
  Event event;
  event.classification = source.classification;
  event.convo = source.convo;
  event.ints = source.ints;
  event.sound_id = source.sound_id;
  event.strings = source.strings;

  /* If convo, do the proper copy */
  if(event.classification == EventClassifier::STARTCONVO &&
     source.convo != NULL)
  {
    event.convo = new Conversation;
    event.convo->action_event = source.convo->action_event;
    event.convo->category = source.convo->category;
    event.convo->next = source.convo->next;
    event.convo->text = source.convo->text;
    event.convo->thing_id = source.convo->thing_id;
  }

  return event;
}

/* Creates an empty conversation */
Conversation EventHandler::createEmptyConversation()
{
  Conversation convo;
  convo.text = "";
  convo.thing_id = -1;
  convo.category = DialogCategory::TEXT;
  convo.action_event = EventHandler::createEventTemplate();
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
  blank_event.sound_id = kUNSET_ID;
  blank_event.strings.clear();

  return blank_event;
}

/* Deletes the given event. Just clears the relevant memory
 * Returns a blank event */
Event EventHandler::deleteEvent(Event event)
{
  /* Delet the existing event, if relevant */
  if(event.convo != NULL)
    delete event.convo;
  event.convo = NULL ;

  return createEventTemplate();
}
