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
  PICKUPITEM     = 3,
  RUNBATTLE      = 4, 
  RUNMAP         = 5,
  TELEPORTTHING  = 6,
  STARTCONVO     = 7
};

/* 
 * Description: Event structure, to hold all pertinent event information
 */
struct Event
{
  EventClassifier classification;
  Conversation* convo;
  std::vector<int> ints;
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

private:
  /* Event queue of what has been executed thus far, since the last clean
   * up cycle */
  std::vector<EventExecution> event_queue;

  /* The event queue index, for parsing the queue */
  uint16_t queue_index;
  
  /* -------------------------- Constants ------------------------- */
  const static uint8_t kGIVE_ITEM_COUNT; /* give item count index */
  const static uint8_t kGIVE_ITEM_ID; /* Give item ID index */
  const static uint8_t kTELEPORT_ID; /* Teleport thing ID index */
  const static uint8_t kTELEPORT_SECTION; /* Teleport thing section index */
  const static uint8_t kTELEPORT_X; /* Teleport thing X index */
  const static uint8_t kTELEPORT_Y; /* Teleport thing Y index */
  
/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates an empty conversation */
  Conversation createEmptyConversation();

  /* Creates the initial event template, clearing it */
  Event createEventTemplate();

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
  /* Creates a disabled blank event */
  Event createBlankEvent();

  /* Creates the conversation initiation event */
  Event createConversationEvent(Conversation* new_conversation = NULL);

  /* Creates a give item event, with the appropriate parameters */
  Event createGiveItemEvent(int id = 0, int count = 0);
  
  /* Creates a notification event, that can fire and result in visible text */
  Event createNotificationEvent(std::string notification);
  
  /* Creates a start battle event */
  Event createStartBattleEvent();
  
  /* Creates a teleport event */
  Event createTeleportEvent(int thing_id = 0, uint16_t tile_x = 0, 
                            uint16_t tile_y = 0, uint16_t section_id = 0);

  /* Deletes the given event. Just clears the relevant memory */
  Event deleteEvent(Event event);
  
  /* Execute the given event - done through signal emits */
  void executeEvent(Event event, MapPerson* initiator, MapThing* source = NULL);
  
  /* Executes a pickup item event */
  void executePickup(MapItem* item, bool walkover = false);

  /* Clears the event handling poll queue */
  void pollClear(); // NEW
  
  /* Poll a conversation event. Only true if this event is next on queue */
  bool pollConversation(Conversation** convo, MapThing** source); // NEW

  /* Poll a give item event */
  bool pollGiveItem(int* id, int* count); // NEW
  
  /* It goes to the next event in the poll sequence. Returns true if there is
   * one */
  bool pollEvent(); // NEW
  
  /* Returns the current event type, to be polled by the management class */
  EventClassifier pollEventType(); // NEW
  
  /* Poll a notification event */
  bool pollNotification(std::string* notification);
  
  /* Poll a pickup item event */
  bool pollPickupItem(MapItem** item, bool* walkover); // NEW
  
  /* Poll a start battle event */
  bool pollStartBattle(); // NEW
  
  /* Poll a teleport thing event */
  bool pollTeleportThing(int* thing_id, int* x, int* y, int* section_id); // NEW
  
  /* Updates the event from the data in the file */
  Event updateEvent(Event event, XmlData data, int file_index, 
                                               int section_index);
};

#endif // EVENTHANDLER_H
