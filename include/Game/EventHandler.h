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

  /* Execute the teleport event */
  void executeTeleportEvent(Event event, int target_id);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Signals for the various events */
  void teleportThing(int id, int x, int y);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Creates a disabled blank event */
  Event createBlankEvent();

  /* Creates a teleport event */
  Event createTeleportEvent(int tile_x, int tile_y);

  /* Execute the given event - done through signal emits */
  void executeEvent(Event event, int target_id);
};

#endif // EVENTHANDLER_H
