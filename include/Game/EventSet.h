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
  const static int32_t kUNSET_ID; /* The unset ID - for sound ref */

private:
  /* Event sets */
  Event events_locked;
  std::vector<Event> events_unlocked;

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

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears all event data within the class */
  void clear();

  /* Returns the event when accessed - depending on locked unlocked status */
  Event* getEvent();

  /* Load data from file */
  bool loadData(XmlData data, int file_index, int section_index);

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

  /* Creates the conversation initiation event */
  static Event createConversationEvent(Conversation* new_conversation = NULL,
                                       int sound_id = kUNSET_ID);

  /* Creates a give item event, with the appropriate parameters */
  static Event createGiveItemEvent(int id = 0, int count = 0, 
                                   int sound_id = kUNSET_ID);

  /* Creates a notification event, that can fire and result in visible text */
  static Event createNotificationEvent(std::string notification = "",
                                       int sound_id = kUNSET_ID);

  /* Creates a sound event */
  static Event createSoundEvent(int sound_id = kUNSET_ID);

  /* Creates a start battle event */
  static Event createStartBattleEvent(int sound_id = kUNSET_ID);

  /* Create a start map event */
  static Event createStartMapEvent(int id = 0, int sound_id = kUNSET_ID);

  /* Creates a teleport event */
  static Event createTeleportEvent(int thing_id = 0, uint16_t tile_x = 0,
                                   uint16_t tile_y = 0, uint16_t section_id = 0,
                                   int sound_id = kUNSET_ID);

  /* Deletes the given event. Just clears the relevant memory */
  static Event deleteEvent(Event event);

  /* Updates the event from the data in the file */
  static Event updateEvent(Event event, XmlData data, int file_index,
                           int section_index);
};

#endif // EVENTSET_H
