/******************************************************************************
 * Class Name: EventSet
 * Date Created: October 20, 2015
 * Inheritance: none
 * Description: The event set handles the set of events that can be stored at
 *              a given trigger location. The event set can be locked, unlocked,
 *              includes multiple events, and handles all associated data.
 *****************************************************************************/
#ifndef EVENTSET_H
#define EVENTSET_H

/* Pre-declarations */
struct Conversation;

/* Includes */
#include <string>
#include <vector>

#include "EnumDb.h"
#include "Helpers.h"
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
 * Description: The locked state - properties in how to unlock
 */
enum class LockedState
{
  NONE = 0,
  TRIGGER = 1,
  ITEM = 2
};

/*
 * Description: The state of the set of unlocked events.
 */
enum class UnlockedState
{
  NONE = 0,
  ORDERED = 1,
  RANDOM = 2
};

/*
 * Description: Locked struct with properties related to locked events or
 *              triggers.
 */
struct Locked
{
  LockedState state;

  bool is_locked;
  bool permanent;

  std::vector<bool> bools;
  std::vector<int> ints;
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

  bool one_shot;
  bool has_exec;
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

/* Event Set class */
class EventSet
{
public:
  /* Constructor function */
  EventSet();

  /* Copy constructor */
  EventSet(const EventSet& source);

  /* Destructor function */
  ~EventSet();

  /* -------------------------- Constants ------------------------- */
  const static uint8_t kGIVE_ITEM_COUNT; /* give item count index */
  const static uint8_t kGIVE_ITEM_ID; /* Give item ID index */
  const static uint8_t kMAP_ID; /* The map ID location for the run event */
  const static uint8_t kTELEPORT_ID; /* Teleport thing ID index */
  const static uint8_t kTELEPORT_SECTION; /* Teleport thing section index */
  const static uint8_t kTELEPORT_X; /* Teleport thing X index */
  const static uint8_t kTELEPORT_Y; /* Teleport thing Y index */
  /* ---- */
  const static uint8_t kHAVE_ITEM_CONSUME; /* Have item consume index */
  const static uint8_t kHAVE_ITEM_COUNT; /* Have item count index */
  const static uint8_t kHAVE_ITEM_ID; /* Have item ID index */
  /* ---- */
  const static int32_t kUNSET_ID; /* The unset ID - for all IDs */

private:
  /* Base event set */
  EventSet* base;

  /* Event sets */
  Event event_locked;
  std::vector<Event> events_unlocked;

  /* Get index - randomly set on getEvent() call */
  int get_index;

  /* Locked status struct */
  Locked locked_status;

  /* State of unlocked events */
  UnlockedState unlocked_state;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EventSet& source);

  /* Returns the pair association of the instance and base of event set. If no
   * base, the pointer pair is identical */
  std::vector<std::pair<Event*,Event*>> getUnlockedPair();

  /* Gets an event reference from the unlocked status. Will create if not
   * there, if the variable has been set */
  Event* getUnlockedRef(int index, bool create = true);

  /* Set-up for base connection */
  void setupForBase();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Add calls, for lists */
  bool addEventUnlocked(Event new_event);

  /* Clears all event data within the class */
  void clear();

  /* Returns the base set reference */
  EventSet* getBase();

  /* Returns the event when accessed - depending on locked unlocked status */
  Event getEvent(bool trigger = true);

  /* Access getters for locked event */
  Event getEventLocked();
  Event* getEventLockedRef(bool force_instance = false);

  /* Access getters for unlocked event(s) */
  std::vector<Event> getEventUnlocked();
  Event getEventUnlocked(int index);
  std::vector<Event*> getEventUnlockedRef(bool force_instance = false);

  /* Returns the locked state struct */
  Locked getLockedState();

  /* Returns the unlocked state enum */
  UnlockedState getUnlockedState();

  /* Returns if the base event set is set */
  bool isBaseSet();

  /* Returns if the class is empty (default state after a clear() call) */
  bool isEmpty();

  /* Returns if the class is currently locked */
  bool isLocked();

  /* Load data from file */
  bool loadData(XmlData data, int file_index, int section_index);

  /* Sets the base set reference */
  void setBase(EventSet* new_base);

  /* Individual setters for events */
  bool setEventLocked(Event new_event);
  bool setEventUnlocked(int index, Event new_event, bool replace = false);

  /* Sets the locked status */
  bool setLocked(Locked new_locked);

  /* Sets the unlocked state */
  bool setUnlockedState(UnlockedState state);

  /* Unset calls */
  bool unsetEventLocked();
  bool unsetEventUnlocked(int index);
  bool unsetEventUnlocked();
  bool unsetLocked();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EventSet& operator=(const EventSet& source);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Copies a passed in event */
  static Event copyEvent(Event source);

  /* Creates a blank conversation */
  static Conversation createBlankConversation();

  /* Create blank event */
  static Event createBlankEvent();

  /* Creates a blank locked struct */
  static Locked createBlankLocked();

  /* Creates the conversation initiation event */
  static Event createEventConversation(Conversation* new_conversation = nullptr,
                                       int sound_id = kUNSET_ID);

  /* Creates a give item event, with the appropriate parameters */
  static Event createEventGiveItem(int id = kUNSET_ID, int count = 0,
                                   int sound_id = kUNSET_ID);

  /* Creates a notification event, that can fire and result in visible text */
  static Event createEventNotification(std::string notification = "",
                                       int sound_id = kUNSET_ID);

  /* Creates a sound event */
  static Event createEventSound(int sound_id = kUNSET_ID);

  /* Creates a start battle event */
  static Event createEventStartBattle(int sound_id = kUNSET_ID);

  /* Create a start map event */
  static Event createEventStartMap(int id = 0, int sound_id = kUNSET_ID);

  /* Creates a teleport event */
  static Event createEventTeleport(int thing_id = 0, uint16_t tile_x = 0, 
                                uint16_t tile_y = 0, int section_id = kUNSET_ID,
                                int sound_id = kUNSET_ID);

  /* Creates a have item check based lock */
  static Locked createLockHaveItem(int id = kUNSET_ID, int count = 1,
                                   bool consume = true, bool permanent = true);

  /* Created a trigger based lock */
  static Locked createLockTriggered(bool permanent = true);

  /* Extract data from events or locks */
  static bool dataEventGiveItem(Event event, int& item_id, int& count);
  static bool dataEventNotification(Event event, std::string& notification);
  static bool dataEventStartMap(Event event, int& map_id);
  static bool dataEventTeleport(Event event, int& thing_id, int& x, int& y,
                                int& section_id);
  static bool dataLockedItem(Locked lock, int& id, int& count, bool& consume);

  /* Deletes the given event. Just clears the relevant memory */
  static Event deleteEvent(Event event);

  /* Returns the conversation at the given index */
  static Conversation* getConversation(Conversation* reference,
                                       std::vector<std::string> index_list);

  /* Unlocks the locked state, if the conditions are met */
  static bool unlockItem(Locked& locked_state, int id, int count);
  static void unlockTrigger(Locked& locked_state);

  /* Unlock state used of locked state - will re-lock if not permanent */
  static void unlockUsed(Locked& locked_state);

  /* Updates the conversation values of the pointed object, based on the XML
   * file data */
  static void updateConversation(Conversation* reference, XmlData data,
                                 int index, int section_index);

  /* Updates the event from the data in the file */
  static Event updateEvent(Event event, XmlData data, int file_index,
                           int section_index);

  /* Updates the event with one shot information */
  static Event updateEventOneShot(Event event, bool one_shot);

  /* Update the locked struct from the data in the file */
  static Locked updateLocked(Locked locked_curr, XmlData data, int file_index);
};

#endif // EVENTSET_H
