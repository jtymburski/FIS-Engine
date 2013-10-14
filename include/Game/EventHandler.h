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
class MapThing;

//#include <QDebug>
#include <QObject>

//#include "EnumDb.h"

class EventHandler : public QObject
{
  Q_OBJECT

public:
  /* Constructor function */
  EventHandler();

  /* Destructor function */
  ~EventHandler();

  /* 
   * Description: The event classification - for what the event will do 
   */
  enum EventClassifier {NOEVENT        = 0,
                        GIVEITEM       = 1,
                        RUNBATTLE      = 2, 
                        RUNMAP         = 3,
                        TELEPORTPLAYER = 4,
                        TELEPORTTHING  = 5,
                        STARTCONVO     = 6};

  /* Test enumerator */
  //enum EventTestEnum { TEST1, TEST2, TEST3 };

  /* Test struct */
  struct Event
  {
    EventClassifier classification;
    Conversation* convo;
    QList<int> integer_stack;
  };

private:
  /* Private variables - needed?? */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the initial event template, clearing it */
  Event createEventTemplate();

  /* Execute a conversation event */
  void executeConversationEvent(Conversation* convo, MapThing* initiator, 
                                                     MapThing* source);

  /* Execute a start battle event */
  void executeStartBattleEvent(Event event, MapThing* target);
  
  /* Execute the teleport event */
  void executeTeleportEvent(Event event, MapThing* target);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Signals for the various events */
  void initConversation(Conversation* convo, MapThing* initiator, 
                                             MapThing* source);
  void startBattle();
  void teleportThing(MapThing* target, int x, int y, int section_id);

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

  /* Execute the given event - done through signal emits */
  void executeEvent(Event event, MapThing* initiator, MapThing* source = 0);
};

#endif // EVENTHANDLER_H
