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
    setEquipLocations();
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
   // return NULL;
}

bool Equipment::canEquip(QString location)
{
   if (location == "head")
     return equip_locations[0];
  /*if (location == "left arm")
    return equip_locations[1];
  if (location == "right arm")
    return equip_locations[2];
  if (location == "torso")
    return equip_locations[3];
  if (location == "legs")
    return equip_locations[4]; */
  return 0;
}

void Equipment::setEquipLocations(bool hd, bool la, bool ra, bool to, bool lg)
{
  equip_locations[0] = hd;
  equip_locations[1] = la;
  equip_locations[2] = ra;
  equip_locations[3] = to;
  equip_locations[4] = lg;

}
