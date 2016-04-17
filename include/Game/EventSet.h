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
 * Description: The flags for the start battle reactions and interactions
 *              and how it is handled by game
 */
enum class BattleFlags
{
  NONE = 0,
  ONWINDISAPPEAR = 1,
  ONLOSEENDGAME = 2,
  RESTOREHEALTH = 4,
  RESTOREQD = 8
};

/*
 * Description: The event classifier identification. Fully inclusive to events
 */
enum class EventClassifier : std::uint32_t
{
  NOEVENT         = 0,
  BATTLESTART     = 1 << 0,
  CONVERSATION    = 1 << 1,
  ITEMGIVE        = 1 << 2,
  ITEMTAKE        = 1 << 3,
  MAPSWITCH       = 1 << 4,
  MULTIPLE        = 1 << 5,
  NOTIFICATION    = 1 << 6,
  PROPERTY        = 1 << 7,
  SOUNDONLY       = 1 << 8,
  TELEPORTTHING   = 1 << 9,
  UNLOCKIO        = 1 << 10,
  UNLOCKTHING     = 1 << 11,
  UNLOCKTILE      = 1 << 12, /* Used in Editor to define last in combo */
  /* Separator: All categories lower are not editable by game designer */
  ITEMPICKUP      = 1 << 13,
  TRIGGERIO       = 1 << 14,
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
 * Description: The thing properties that can be modified by the mod event
 */
enum class ThingProperty
{
  NONE            = 0,
  ACTIVE          = 1 << 0, /* bool */
  FORCED_INTERACT = 1 << 1, /* bool */
  INACTIVE_TIME   = 1 << 2, /* int */
  MOVE_DISABLE    = 1 << 3, /* bool */
  RESET_LOCATION  = 1 << 4, /* bool */
  RESPAWN_TIME    = 1 << 5, /* int */
  SPEED           = 1 << 6, /* int */
  TRACKING        = 1 << 7, /* int */
  VISIBLE         = 1 << 8  /* bool */
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
 * Description: Which event within the state to unlock. Set up to use bit OR
 *              combinations if required
 */
enum class UnlockIOEvent
{
  NONE     = 0, /* No event within the states */
  ENTER    = 1, /* Enter event within the states */
  EXIT     = 2, /* Exit event within the states */
  USE      = 4, /* Use event within the states */
  WALKOVER = 8  /* Walkover event within the states */
};

/*
 * Description: Which IO mode to attempt to unlock - main lock or events
 */
enum class UnlockIOMode
{
  NONE   = 0, /* No lock within the IO */
  LOCK   = 1, /* The main interact lock to attempt unlock */
  EVENTS = 2  /* The events within states to attempt to unlock */
};

/*
 * Description: Which event within the tile to unlock. Set up to use bit OR
 *              combinations if required
 */
enum class UnlockTileMode
{
  NONE  = 0, /* No event within the tile */
  ENTER = 1, /* Enter event on tile */
  EXIT  = 2  /* Exit event on tile */
};

/*
 * Description: Defines the unlock view mode of the object
 */
enum class UnlockView
{
  NONE = 0, /* No view scroll */
  GOTO = 1, /* Should the view goto the unlock location */
  SCROLL = 2 /* Should it scroll there if in the same map */
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
  std::vector<Event> events;
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

/*
 * Description: Pairs for handling within the conversation/event space
 */
struct EventPair
{
  Event* base;
  Event* inst;
};
struct ConvoPair
{
  Conversation* base;
  Conversation* inst;
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
  const static uint8_t kBATTLE_EVENT_WIN; /* The battle win event index */
  const static uint8_t kBATTLE_EVENT_LOSE; /* The battle lose event index */
  const static uint8_t kBATTLE_FLAGS; /* The battle flags index */
  const static uint8_t kGIVE_ITEM_COUNT; /* Give item count index */
  const static uint8_t kGIVE_ITEM_ID; /* Give item ID index */
  const static uint8_t kMAP_ID; /* The map ID location for the run event */
  const static uint8_t kPROP_BOOLS; /* Property bools index */
  const static uint8_t kPROP_ID; /* Property thing id index */
  const static uint8_t kPROP_INACTIVE; /* Property inactive time index */
  const static uint8_t kPROP_MODS; /* Property which to modify index */
  const static uint8_t kPROP_RESPAWN; /* Property respawn time index */
  const static uint8_t kPROP_SPEED; /* Property speed index */
  const static uint8_t kPROP_TRACK; /* Property tracking definition index */
  const static uint8_t kPROP_TYPE; /* Property thing type definition */
  const static uint8_t kTAKE_ITEM_COUNT; /* Take item count index */
  const static uint8_t kTAKE_ITEM_ID; /* Take item ID index */
  const static uint8_t kTELEPORT_ID; /* Teleport thing ID index */
  const static uint8_t kTELEPORT_SECTION; /* Teleport thing section index */
  const static uint8_t kTELEPORT_X; /* Teleport thing X index */
  const static uint8_t kTELEPORT_Y; /* Teleport thing Y index */
  const static uint8_t kUNIO_ID; /* Unlock IO: ID */
  const static uint8_t kUNIO_MODE; /* Unlock IO: Mode */
  const static uint8_t kUNIO_MODE_EVENT; /* Unlock IO: Which events in state */
  const static uint8_t kUNIO_STATE; /* Unlock IO: Which state within stack */
  const static uint8_t kUNIO_VIEW_MODE; /* Unlock IO: View unlock mode */
  const static uint8_t kUNIO_VIEW_TIME; /* Unlock IO: View unlock time */
  const static uint8_t kUNTHING_ID; /* Unlock Thing: ID */
  const static uint8_t kUNTHING_VIEW_MODE; /* Unlock Thing: View unlock mode */
  const static uint8_t kUNTHING_VIEW_TIME; /* Unlock Thing: View unlock time */
  const static uint8_t kUNTILE_MODE; /* Unlock Tile: Mode (enter/exit) */
  const static uint8_t kUNTILE_SECTION; /* Unlock Tile: section ID */
  const static uint8_t kUNTILE_X; /* Unlock Tile: X coord */
  const static uint8_t kUNTILE_Y; /* Unlock Tile: Y coord */
  const static uint8_t kUNTILE_VIEW_MODE; /* Unlock Tile: View unlock mode */
  const static uint8_t kUNTILE_VIEW_TIME; /* Unlock Tile: View unlock time */
  /* ---- */
  const static uint8_t kHAVE_ITEM_CONSUME; /* Have item consume index */
  const static uint8_t kHAVE_ITEM_COUNT; /* Have item count index */
  const static uint8_t kHAVE_ITEM_ID; /* Have item ID index */
  /* ---- */
  const static int32_t kVIEW_TIME; /* The default view time - for unlocks */
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

  /*======================== PRIVATE FUNCTIONS ===============================*/
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

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Add calls, for lists */
  bool addEventUnlocked(Event new_event);

  /* Clears all event data within the class */
  void clear();

  /* Returns the base set reference */
  EventSet* getBase();

  /* Returns the event when accessed - depending on locked unlocked status */
  Event getEvent(bool trigger = true);
  EventPair getEventPair(bool trigger = true);
  Event* getEventRef(bool trigger = true);

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

  /* Returns if there is no interaction if triggering the class */
  bool isNoInteraction();

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

  /* Unlock trigger call */
  bool unlockTrigger();

  /* Unset calls */
  bool unsetEventLocked();
  bool unsetEventUnlocked(int index);
  bool unsetEventUnlocked();
  bool unsetLocked();

  /*========================= OPERATOR FUNCTIONS =============================*/
public:
  /* The copy operator */
  EventSet& operator=(const EventSet& source);

  /*===================== PUBLIC STATIC FUNCTIONS ============================*/
public:
  /* Classification enumerator to and from string */
  static EventClassifier classifierFromStr(const std::string& classifier);
  static std::string classifierToStr(const EventClassifier& classifier);

  /* Copies a passed in event */
  static Event copyEvent(Event source, bool skeleton = false);

  /* Creates a blank conversation */
  static Conversation createBlankConversation();

  /* Create blank event */
  static Event createBlankEvent();

  /* Creates a blank locked struct */
  static Locked createBlankLocked();

  /* Creates enums with given data */
  static BattleFlags createEnumBattleFlags(bool win_disappear = false,
                                           bool lose_gg = false,
                                           bool restore_health = false,
                                           bool restore_qd = false);
  static UnlockIOEvent createEnumIOEvent(bool enter = false, bool exit = false,
                                      bool use = false, bool walkover = false);
  static UnlockIOMode createEnumIOMode(bool lock = false, bool events = false);
  static ThingProperty createEnumProperties(bool active = false,
                bool forced = false, bool inactive = false, bool move = false,
                bool reset = false, bool respawn = false, bool speed = false,
                bool track = false, bool visible = false);
  static UnlockTileMode createEnumTileEvent(bool enter = false,
                                            bool exit = false);
  static UnlockView createEnumView(bool view = false, bool scroll = false);

  /* Creates the conversation initiation event */
  static Event createEventConversation(Conversation* new_conversation = nullptr,
                                       int sound_id = kUNSET_ID);

  /* Creates a give item event, with the appropriate parameters */
  static Event createEventGiveItem(int id = kUNSET_ID, int count = 0,
                                   int sound_id = kUNSET_ID);

  /* Creates a multiple event with a set of events */
  static Event createEventMultiple(
                             std::vector<Event> events = std::vector<Event>(),
                             int sound_id = kUNSET_ID);

  /* Creates a notification event, that can fire and result in visible text */
  static Event createEventNotification(std::string notification = "",
                                       int sound_id = kUNSET_ID);

  /* Creates a property modifier event */
  static Event createEventPropMod(ThingBase type = ThingBase::THING,
                int id = kUNSET_ID, ThingProperty props = ThingProperty::NONE,
                ThingProperty bools = ThingProperty::NONE, int respawn = 0,
                int speed = 0, TrackingState track = TrackingState::NOTRACK,
                int inactive = 0, int sound_id = kUNSET_ID);

  /* Creates a sound event */
  static Event createEventSound(int sound_id = kUNSET_ID);

  /* Creates a start battle event */
  static Event createEventStartBattle(int sound_id = kUNSET_ID);
  static Event createEventStartBattle(BattleFlags flags,
                                      Event event_win,
                                      Event event_lose,
                                      int sound_id = kUNSET_ID);

  /* Create a start map event */
  static Event createEventStartMap(int id = 0, int sound_id = kUNSET_ID);

  /* Creates a take item event */
  static Event createEventTakeItem(int id = kUNSET_ID, int count = 0,
                                   int sound_id = kUNSET_ID);

  /* Creates a teleport event */
  static Event createEventTeleport(int thing_id = 0, uint16_t tile_x = 0,
                                uint16_t tile_y = 0, int section_id = kUNSET_ID,
                                int sound_id = kUNSET_ID);

  /* Create unlock events (thing, tile, or IO) */
  static Event createEventUnlockIO(int io_id = 0,
                UnlockIOMode mode = UnlockIOMode::NONE, int state_num = -1,
                UnlockIOEvent events = UnlockIOEvent::NONE,
                UnlockView view_mode = UnlockView::NONE,
                int view_time = kVIEW_TIME, int sound_id = kUNSET_ID);
  static Event createEventUnlockThing(int thing_id = 0,
                UnlockView view_mode = UnlockView::NONE,
                int view_time = kVIEW_TIME, int sound_id = kUNSET_ID);
  static Event createEventUnlockTile(int section_id = kUNSET_ID,
                uint16_t tile_x = 0, uint16_t tile_y = 0,
                UnlockTileMode mode = UnlockTileMode::NONE,
                UnlockView view_mode = UnlockView::NONE,
                int view_time = kVIEW_TIME, int sound_id = kUNSET_ID);

  /* Creates a have item check based lock */
  static Locked createLockHaveItem(int id = kUNSET_ID, int count = 1,
                                   bool consume = true, bool permanent = true);

  /* Created a trigger based lock */
  static Locked createLockTriggered(bool permanent = true);

  /* Extract data from enum(s) */
  static void dataEnumBattleFlags(BattleFlags flags, bool& win_disappear,
                                  bool& lose_gg, bool& restore_health,
                                  bool& restore_qd);
  static void dataEnumIOEvent(UnlockIOEvent io_enum, bool& enter, bool& exit,
                              bool& use, bool& walkover);
  static void dataEnumIOMode(UnlockIOMode io_enum, bool& lock, bool& events);
  static void dataEnumProperties(ThingProperty props, bool& active,
                      bool& forced, bool& inactive, bool& move, bool& reset,
                      bool& respawn, bool& speed, bool& track, bool& visible);
  static void dataEnumTileEvent(UnlockTileMode tile_enum,
                                bool& enter, bool& exit);
  static void dataEnumView(UnlockView view_enum, bool& view, bool& scroll);

  /* Extract data from event(s) */
  static bool dataEventGiveItem(Event event, int& item_id, int& count);
  static bool dataEventMultiple(Event event, std::vector<Event>& event_list);
  static bool dataEventMultiple(Event* event,
                                std::vector<Event*>& event_list);
  static bool dataEventNotification(Event event, std::string& notification);
  static bool dataEventPropMod(Event event, ThingBase& type, int& id,
                               ThingProperty& props, ThingProperty& bools,
                               int& respawn, int& speed, TrackingState& track,
                               int& inactive);
  static bool dataEventStartBattle(Event* event, BattleFlags& flags,
                                   Event*& event_win, Event*& event_lose);
  static bool dataEventStartMap(Event event, int& map_id);
  static bool dataEventTakeItem(Event event, int& item_id, int& count);
  static bool dataEventTeleport(Event event, int& thing_id, int& x, int& y,
                                int& section_id);
  static bool dataEventUnlockIO(Event event, int& io_id, UnlockIOMode& mode,
                                int& state_num, UnlockIOEvent& mode_events,
                                UnlockView& mode_view, int& view_time);
  static bool dataEventUnlockThing(Event event, int& thing_id,
                                   UnlockView& mode_view, int& view_time);
  static bool dataEventUnlockTile(Event event, int& section_id, int& tile_x,
                                  int& tile_y, UnlockTileMode& mode,
                                  UnlockView& mode_view, int& view_time);

  /* Extract data from lock(s) */
  static bool dataLockedItem(Locked lock, int& id, int& count, bool& consume);

  /* Deletes the given event. Just clears the relevant memory */
  static Event deleteEvent(Event event);

  /* Returns the conversation at the given index */
  static Conversation* getConversation(Conversation* reference,
                                       std::vector<std::string> index_list);

  /* Determines if the locked struct is locked */
  static bool isLocked(Locked lock_struct);

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
