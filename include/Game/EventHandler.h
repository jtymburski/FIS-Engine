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

class MapThing;

#include <QObject>

#include "EnumDb.h"

class EventHandler : public QObject
{
  Q_OBJECT

public:
  /* Constructor function */
  EventHandler();

  /* Destructor function */
  ~EventHandler();

private:
  /* Private variables - needed?? */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the initial event template, clearing it */
  Event createEventTemplate();

  /* Execute a start battle event */
  void executeStartBattleEvent(Event event, MapThing* target);
  
  /* Execute the teleport event */
  void executeTeleportEvent(Event event, MapThing* target);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Signals for the various events */
  void startBattle();
  void teleportThing(MapThing* target, int x, int y, int section_id);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Creates a disabled blank event */
  Event createBlankEvent();

  /* Creates a start battle event */
  Event createStartBattleEvent();
  
  /* Creates a teleport event */
  Event createTeleportEvent(int tile_x, int tile_y, int section_id = -1);

  /* Execute the given event - done through signal emits */
  void executeEvent(Event event, MapThing* target);
};

#endif // EVENTHANDLER_H
