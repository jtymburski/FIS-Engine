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
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

//struct Conversation;
class MapInteractiveObject;
class MapItem;
class MapPerson;
class MapThing;

#include <iostream>
#include <string>
#include <vector>

#include "EnumDb.h"
#include "Game/EventSet.h"
#include "Game/KeyHandler.h"
#include "Helpers.h"
#include "SoundHandler.h"
#include "XmlData.h"

/*
 * Description: Event queue storage structure. Stores all occurred events since
 *              the last clean up call.
 */
struct EventExecution
{
  Event event;
  EventSet* event_set;

  MapItem* item;
  MapPerson* initiator;
  MapThing* source;
};

class EventHandler
{
public:
  /* Constructor function */
  EventHandler();

  /* Destructor function */
  ~EventHandler();

  /* -------------------------- Constants ------------------------- */
  //const static uint8_t kGIVE_ITEM_COUNT; /* give item count index */

private:
  /* Event queue of what has been executed thus far, since the last clean
   * up cycle */
  std::vector<EventExecution> event_queue;

  /* Handler for state of the keyboard */
  KeyHandler key_handler;

  /* The event queue index, for parsing the queue */
  uint16_t queue_index;

  /* Sound handling class */
  SoundHandler* sound_handler;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Returns the event in the queue: either from the set or event pointer */
  bool getEvent(Event& event, bool trigger = false);

  /* Trigger queue sound */
  void triggerQueueSound(Event event);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Execute the given event */
  void executeEvent(Event event, MapPerson* initiator, MapThing* source = NULL);

  /* Execute the given event set */
  void executeEventSet(EventSet* set, MapPerson* initiator,
                       MapThing* source = nullptr);

  /* Executes an MIO trigger event */
  void executeIOTrigger(MapInteractiveObject* io, int interaction_state,
                        MapPerson* initiator);

  /* Executes a pickup item event */
  void executePickup(MapItem* item, bool walkover = false);

  /* Returns a reference to the Key Handler */
  KeyHandler& getKeyHandler();

  /* Returns a reference to the sound handler */
  SoundHandler* getSoundHandler();

  /* Clears the event handling poll queue */
  void pollClear();

  /* Poll a conversation event. Only true if this event is next on queue */
  bool pollConversation(Conversation** convo, MapThing** source);

  /* Poll a give item event */
  bool pollGiveItem(int* id, int* count);

  /* It goes to the next event in the poll sequence. Returns true if there is
   * one */
  bool pollEvent();

  /* Checks if there is any valid event available - usually called first */
  bool pollEventAvailable();

  /* Returns the current event type, to be polled by the management class */
  EventClassifier pollEventType();

  /* Polls to see if the current event is locked and can be unlocked (such as
   * with a have item call) */
  bool pollLock(LockedState& state);
  bool pollLockAvail();
  bool pollLockGetData(Locked& lock);
  bool pollLockSetData(Locked lock);

  /* Accesses lock properties */
  bool pollLockItem(int& id, int& count, bool& consume); // TODO: REMOVE?

  /* Poll a notification event */
  bool pollNotification(std::string* notification);

  /* Poll a pickup item event */
  bool pollPickupItem(MapItem** item, bool* walkover);

  /* Poll a sound event */
  bool pollSound();

  /* Poll a start battle event */
  bool pollStartBattle(MapPerson** person, MapThing** source);

  /* Poll a start map event */
  bool pollStartMap(int* id);
  
  /* Poll a take item event */
  bool pollTakeItem(int* id, int* count);
  
  /* Poll a teleport thing event */
  bool pollTeleportThing(int* thing_id, int* x, int* y, int* section_id);

  /* Poll a trigger IO event */
  bool pollTriggerIO(MapInteractiveObject** io, int* state, 
                     MapPerson** initiator);

  /* Unlock triggers, based on if the event set has a lock struct */
  bool pollUnlockItem(int id, int count); // TODO: REMOVE?

  /* Sets the sound handler used. If unset, no sounds will play */
  void setSoundHandler(SoundHandler* new_handler);

  /* Triggers for all sounds */
  void triggerAudioStop(SoundChannels channel);
  void triggerMusic(uint32_t id);
  void triggerSound(uint32_t id, SoundChannels channel);
  void triggerWeather(uint32_t id);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Copies a passed in event */
//  static Event copyEvent(Event source);
};

#endif // EVENTHANDLER_H
