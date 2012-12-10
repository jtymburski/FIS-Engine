/******************************************************************************
* Class Name: Equipment
* Date Created: Dec 2 2012
* Inheritance: Item
* Description: Equipment that categorizes under Item in an Inventory unit. Gets
*              called by Person as well for attached Equipment.
******************************************************************************/
#include "Game/Player/Equipment.h"

/*Constructor function*/
Equipment::Equipment()
{
}

/*Destructor function*/
Equipment::~Equipment()
{
}

/* Checks if the bubby will fit into the bubby signature
 * X is the left most coordinate, Y is the top most coordinate
 * Returns if space is available for attachment */
bool Equipment::isBubbyAttachable(Bubby* bubby, int x, int y)
{
    return TRUE;
}

/* Attempt to attach bubby into the signature
 * X is the left most coordinate, Y is the top most coordinate
 * Returns status of attachment */
bool Equipment::attachBubby(Bubby* bubby, int x, int y)
{
    return TRUE;
}

/*Gets the list of equipment actions (Used for total action lists in battle)*/
QVector<Action*> Equipment::getActionList()
{
    //return NULL;
}
