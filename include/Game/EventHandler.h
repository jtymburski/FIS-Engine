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
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Creates a disabled blank event */
  Event createBlankEvent();
};

#endif // EVENTHANDLER_H
