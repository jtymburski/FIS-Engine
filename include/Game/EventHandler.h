/******************************************************************************
* Class Name: EventHandler
* Date Created: September 12, 2013
* Inheritance: QObject
* Description: This event handler contains all events between the multiple 
*              classes and manages data transfer between from calls internally.
*              It is merely meant as a facilitator of actions that need to
*              happen deep inside of the nested structure and then need to 
*              fire from the top of the chain.
*
* Notes: The 'Event' struct is located at the bottom of this class which is
*        used for executing actions according the handler specification
******************************************************************************/
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

struct Conversation;
class MapItem;
class MapPerson;
class MapThing;

#include <QDebug>
#include <QObject>

#include "EnumDb.h"
#include "XmlData.h"
  
/* Test struct */
struct Event
{
  EnumDb::EventClassifier classification;
  Conversation* convo;
  QList<int> integer_stack;
};

/* Struct to handle the conversation throughout the map */
// TODO: tile image?
struct Conversation
{
  QString text;
  int thing_id;
  EnumDb::DialogCategory category;
  Event action_event;
  QList<Conversation> next;
};

class EventHandler : public QObject
{
  Q_OBJECT

public:
  /* Constructor function */
  EventHandler();

  /* Destructor function */
  ~EventHandler();
  
  /* Test enumerator */
  //enum EventTestEnum { TEST1, TEST2, TEST3 };

private:
  /* Private variables - needed?? */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the initial event template, clearing it */
  Event createEventTemplate();

  /* Execute a conversation event */
  void executeConversationEvent(Conversation* convo, MapPerson* initiator, 
                                                     MapThing* source);

  /* Execute a start battle event */
  void executeStartBattleEvent(Event event, MapPerson* target);
  
  /* Execute the teleport event */
  void executeTeleportEvent(Event event, MapPerson* target);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Signals for the various events */
  void giveItem(int id, int count);
  void initConversation(Conversation* convo, 
                        MapPerson* initiator, MapThing* source);
  void pickupItem(MapItem* item, bool walkover);
  void startBattle();
  void teleportThing(MapPerson* target, int x, int y, int section_id);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Creates a disabled blank event */
  Event createBlankEvent();

  /* Creates the conversation initiation event */
  Event createConversationEvent(Conversation* new_conversation);

  /* Creates a start battle event */
  Event createStartBattleEvent();
  
  /* Creates a teleport event */
  Event createTeleportEvent(int tile_x, int tile_y, int section_id = -1);

  /* Deletes the given event. Just clears the relevant memory */
  Event deleteEvent(Event event);
  
  /* Execute the given event - done through signal emits */
  void executeEvent(Event event, MapPerson* initiator, MapThing* source = 0);
  
  /* Executes a pickup item event */
  void executePickup(MapItem* item, bool walkover = false);

  /* Updates the event from the data in the file */
  Event updateEvent(Event event, XmlData data, int file_index, 
                                               int section_index);
};

#endif // EVENTHANDLER_H
