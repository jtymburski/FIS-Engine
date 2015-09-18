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

struct Conversation;
class MapItem;
class MapPerson;
class MapThing;

#include <iostream>
#include <string>
#include <vector>

#include "Game/KeyHandler.h"
#include "EnumDb.h"
#include "Helpers.h"
#include "SoundHandler.h"
#include "XmlData.h"

/*
 * Description: The event classifier identification. Fully inclusive to events
 */
enum class EventClassifier
{
  NOEVENT        = 0,
  GIVEITEM       = 1,
  NOTIFICATION   = 2,
  RUNBATTLE      = 3,
  RUNMAP         = 4,
  TELEPORTTHING  = 5,
  JUSTSOUND      = 6,
  STARTCONVO     = 7, /* Needs to be the last before the non-editor points */
  PICKUPITEM     = 8  /* All categories here and lower are not from Editor. */
};

/*
 * Description: Event structure, to hold all pertinent event information
 */
struct Event
{
  EventClassifier classification;
  Conversation* convo;
  std::vector<int> ints;
  int32_t sound_id;
  std::vector<std::string> strings;
};

/*
 * Description: Event queue storage structure. Stores all occurred events since
 *              the last clean up call.
 */
struct EventExecution
{
  Event event;
  MapItem* item;
  MapPerson* initiator;
  MapThing* source;
};

/*
 * Description: Conversation structure, for what occurs during talking sequences
 * TODO: Tile image? inside display box
 */
struct Conversation
{
  std::string text;
  int thing_id;
  DialogCategory category;
  Event action_event;
  std::vector<Conversation> next;
};

class EventHandler
{
public:
  /* Constructor function */
  EventHandler();

  /* Destructor function */
  ~EventHandler();

  /* Test enumerator */
  //enum EventTestEnum { TEST1, TEST2, TEST3 };

  /* -------------------------- Constants ------------------------- */
  const static uint8_t kGIVE_ITEM_COUNT; /* give item count index */
  const static uint8_t kGIVE_ITEM_ID; /* Give item ID index */
  const static uint8_t kMAP_ID; /* The map ID location for the run event */
  const static uint8_t kTELEPORT_ID; /* Teleport thing ID index */
  const static uint8_t kTELEPORT_SECTION; /* Teleport thing section index */
  const static uint8_t kTELEPORT_X; /* Teleport thing X index */
  const static uint8_t kTELEPORT_Y; /* Teleport thing Y index */
  const static int32_t kUNSET_ID; /* The unset ID - for sound ref */

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
  /* Returns the conversation at the given index */
  Conversation* getConversation(Conversation* reference,
                                std::vector<std::string> index_list);

  /* Sets the conversation values of the pointed object, based on the XML
   * file data */
  void setConversationValues(Conversation* reference, XmlData data,
                             int index, int section_index);

  /* Trigger queue sound */
  void triggerQueueSound();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Creates a disabled blank event */
  Event createBlankEvent();

  /* Creates the conversation initiation event */
  Event createConversationEvent(Conversation* new_conversation = NULL,
                                int sound_id = kUNSET_ID);

  /* Creates a give item event, with the appropriate parameters */
  Event createGiveItemEvent(int id = 0, int count = 0, 
                            int sound_id = kUNSET_ID);

  /* Creates a notification event, that can fire and result in visible text */
  Event createNotificationEvent(std::string notification,
                                int sound_id = kUNSET_ID);

  /* Creates a sound event */
  Event createSoundEvent(int sound_id = kUNSET_ID);

  /* Creates a start battle event */
  Event createStartBattleEvent(int sound_id = kUNSET_ID);

  /* Create a start map event */
  Event createStartMapEvent(int id = 0, int sound_id = kUNSET_ID);

  /* Creates a teleport event */
  Event createTeleportEvent(int thing_id = 0, uint16_t tile_x = 0,
                            uint16_t tile_y = 0, uint16_t section_id = 0,
                            int sound_id = kUNSET_ID);

  /* Execute the given event */
  void executeEvent(Event event, MapPerson* initiator, MapThing* source = NULL);

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

  /* Returns the current event type, to be polled by the management class */
  EventClassifier pollEventType();

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

  /* Poll a teleport thing event */
  bool pollTeleportThing(int* thing_id, int* x, int* y, int* section_id); // NEW

  /* Sets the sound handler used. If unset, no sounds will play */
  void setSoundHandler(SoundHandler* new_handler);

  /* Trigger sound file */
  void triggerMusic(uint32_t id);
  void triggerSound(uint32_t id, SoundChannels channel);

  /* Updates the event from the data in the file */
  Event updateEvent(Event event, XmlData data, int file_index,
                                               int section_index);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Copies a passed in event */
  static Event copyEvent(Event source);

  /* Creates an empty conversation */
  static Conversation createEmptyConversation();

  /* Creates the initial event template, clearing it */
  static Event createEventTemplate();

  /* Deletes the given event. Just clears the relevant memory */
  static Event deleteEvent(Event event);
};

#endif // EVENTHANDLER_H
