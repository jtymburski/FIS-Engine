/******************************************************************************
* Class Name: MapNPC
* Date Created: Dec 2 2012
* Inheritance: MapPerson
* Description: The MapNPC class, this covers all AI found on the map in game
* TODO: Reimplement the movement functions based on NPC scripts
******************************************************************************/
#include "Game/Map/MapNPC.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapNPC::MapNPC()
{
}

/* Destructor function */
MapNPC::~MapNPC()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Gets a pointer to the Persons conversation */
QVector<QString*>* MapNPC::getConversation()
{
    //return talking_points;
}

/* Gets a pointer to the gift the NPC has */
MapThing* MapNPC::getGift()
{
    return gift;
}
