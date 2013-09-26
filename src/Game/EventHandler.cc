/******************************************************************************
* Class Name: EventHandler
* Date Created: September 12, 2013
* Inheritance: QObject
* Description: This event handler contains all events between the multiple 
*              classes and manages data transfer between from calls internally.
*              It is merely meant as a facilitator of actions that need to
*              happen deep inside of the nested structure and then need to 
*              fire from the top of the chain.
******************************************************************************/
#include "Game/EventHandler.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventHandler::EventHandler()
{
}

/* Destructor function */
EventHandler::~EventHandler()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
/* Creates a disabled blank event */
Event EventHandler::createBlankEvent()
{
  Event blank_event;
  blank_event.handler = this;
  blank_event.classification = EnumDb::NOEVENT;
  
  return blank_event;
}