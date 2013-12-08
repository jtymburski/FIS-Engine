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
  RUNBATTLE      = 2, 
  RUNMAP         = 3,
  TELEPORTTHING  = 4,
  STARTCONVO     = 5,
  PICKUPITEM     = 6
};

/* 
 * Description: Event structure, to hold all pertinent event information
 */
struct Event
{
  EventClassifier classification;
  Conversation* convo;
  std::vector<int> ints;
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

  /* Creates a start battle event */
  Event createStartBattleEvent();
  
  /* Creates a teleport event */
  Event createTeleportEvent(int thing_id, int tile_x, int tile_y, 
                                                      int section_id = -1);

  /* Deletes the given event. Just clears the relevant memory */
  Event deleteEvent(Event event);
  
  /* Execute the given event - done through signal emits */
  void executeEvent(Event event, MapPerson* initiator, MapThing* source = NULL);
  
  /* Executes a pickup item event */
  void executePickup(MapItem* item, bool walkover = false);

  /* Clears the event handling poll queue */
  void pollClear(); // NEW
  
  /* Poll a conversation event. Only true if this event is next on queue */
  bool pollConversation(Conversation** convo, MapPerson** initiator, 
                                              MapThing** source); // NEW

  /* Poll a give item event */
  bool pollGiveItem(int* id, int* count); // NEW
  
  /* It goes to the next event in the poll sequence. Returns true if there is
   * one */
  bool pollEvent(); // NEW
  
  /* Returns the current event type, to be polled by the management class */
  EventClassifier pollEventType(); // NEW
  
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
