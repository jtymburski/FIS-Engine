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
//const uint8_t EventHandler::kGIVE_ITEM_COUNT = 1;

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

/* Execute the given event - done through signal emits */
void EventHandler::executeEvent(Event event, MapPerson* initiator,
                                             MapThing* source)
{
  if(event.classification != EventClassifier::NOEVENT)
  {
    /* Create the executed event queue entry */
    EventExecution executed_event;
    executed_event.event = event;
    executed_event.event_set = nullptr;
    executed_event.item = nullptr;
    executed_event.initiator = initiator;
    executed_event.source = source;

    /* Push the event to the back of the queue */
    event_queue.push_back(executed_event);
  }
}
  
/* Execute the given event set */
void EventHandler::executeEventSet(EventSet* set, MapPerson* initiator, 
                                   MapThing* source)
{
  if(set != nullptr && !set->isEmpty())
  {
    /* Create the executed event queue entry */
    EventExecution executed_event;
    executed_event.event = EventSet::createBlankEvent();
    executed_event.event_set = set;
    executed_event.item = nullptr;
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
  Event new_event = EventSet::createBlankEvent();
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
  if(id != nullptr && count != nullptr && queue_index < event_queue.size())
  {
    if(EventSet::dataEventGiveItem(event_queue[queue_index].event, *id, *count))
    {
      triggerQueueSound();
      return true;
    }
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
  
/* Polls to see if the current event is locked and can be unlocked (such as
 * with a have item call */
bool EventHandler::pollLock()
{
  if(queue_index < event_queue.size() && 
     event_queue[queue_index].event_set != nullptr)
  {
    EventSet* set = event_queue[queue_index].event_set;

    /* Determine if the current set is locked and if its a locked state that
     * needs to be reviewed at call time */
    return (set->isLocked() && 
            (set->getLockedState().state == LockedState::ITEM));
  }
  return false;
}
  
/* Polls the lock item for the related properties associated */
bool EventHandler::pollLockItem(int& id, int& count, bool& consume)
{
  if(queue_index < event_queue.size() &&
     event_queue[queue_index].event_set != nullptr)
  {
    EventSet* set = event_queue[queue_index].event_set;
    if(set->isLocked() && set->getLockedState().state == LockedState::ITEM)
    {
      EventSet::dataLockedItem(set->getLockedState(), id, count, consume);
      return true;
    }
  }
  return false;
}

/* Poll a notification event */
bool EventHandler::pollNotification(std::string* notification)
{
  if(notification != nullptr && queue_index < event_queue.size())
  {
    if(EventSet::dataEventNotification(
                     event_queue[queue_index].event, *notification))
    {
      triggerQueueSound();
      return true;
    }
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
  if(id != nullptr && queue_index < event_queue.size())
  {
    if(EventSet::dataEventStartMap(event_queue[queue_index].event, *id))
    {
      triggerQueueSound();
      return true;
    }
  }
  return false;
}

/* Poll a teleport thing event */
bool EventHandler::pollTeleportThing(int* thing_id, int* x, int* y,
                                                    int* section_id)
{
  if(thing_id != nullptr && x != nullptr && y != nullptr && 
     section_id != nullptr && queue_index < event_queue.size())
  {
    if(EventSet::dataEventTeleport(event_queue[queue_index].event, *thing_id, 
                                   *x, *y, *section_id))
    {
      triggerQueueSound();
      return true;
    }
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
