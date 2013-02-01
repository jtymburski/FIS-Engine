/******************************************************************************
* Class Name: Equipment
* Date Created: Dec 2 2012
* Inheritance: Item
* Description: Equipment that categorizes under Item in an Inventory unit. Gets
*              called by Person as well for attached Equipment.
* TODO: Finish actions [01-31-13]
* TODO: Bubby attachment algorithm [01-31-13]
* TODO: Bubby signature bonus algorithm [01-31-13]
******************************************************************************/
#include "Game/Player/Equipment.h"
#include <QDebug>

/*
 * Description: Equipment class consructor
 *
 * Inputs: none
 */
Equipment::Equipment()
{
    setEquipLocations();
}

/*
 * Description: Equipment class destructor
 */
Equipment::~Equipment()
{
  clearActionList();
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; i++)
    {
      delete bubby_signature[i][j];
      bubby_signature[i][j] = NULL;
      }
  }
}

/*
 * Description: Clears the list of actions
 *
 * Inputs: none
 * Output: none
 */
void Equipment::clearActionList()
{
    action_list.clear();
}

/*
 * Description: Checks if the bubby will fit into the bubby signature
 *
 * Inputs: Bubby* - pointer to bubby to be checked for attachment capability
 *         int x - the left most coordinate of the proposed location
 *         int y - the top most coordinate of the proposed location
 * Output: bool - boolean if the bubby can be attached
 */
// TODO: Bubby attachment algorithm [01-31-13]
bool Equipment::isBubbyAttachable(Bubby* bubby, int x, int y)
{
    return TRUE;
}

/*
 * Description: Attempt to attach the bubby into the signature.
 *
 * Inputs: Bubby* - pointer of the bubby to be attached
 *         int x - the left most coordinate
 *         int y - the top most coordinate
 * Output: bool - true if the attachment was successful
 */
bool Equipment::attachBubby(Bubby* bubby, int x, int y)
{
    return TRUE;
}

/*
 * Description: Obtains the list of actions the equipment allows the person
 *              to perform (used for a total action list calculation in the
 *              battle class
 *
 * Inputs: none
 * Output: QVector of Action pointers - the built action list
 */

QVector<Action*> Equipment::getActionList()
{
   // TODO: Actions! [01-31-13]
    return action_list;
}

/*
 * Description: Returns a bool based if the equipment can be equipped
 *              on a certain slot on a person
 *
 * Inputs: QString - string of location to be checked
 * Output: bool - true if item can be equipped at location
 */
bool Equipment::canEquip(QString location)
{
  if ((flags & BROKEN) == BROKEN)
    return FALSE;
  if ((flags & HEAD) == HEAD)
    return TRUE;
  if ((flags & TORSO) == TORSO)
    return TRUE;
  if ((flags & LEGS) == LEGS)
    return TRUE;
  if ((flags & (TWOHAND | LEFTARM | RIGHTARM)) == (TWOHAND | LEFTARM | RIGHTARM))
     return TRUE;
  if ((flags & LEFTARM) == LEFTARM)
    return TRUE;
  if ((flags & RIGHTARM) == RIGHTARM)
    return TRUE;
  return FALSE;
}

/*
 * Description: Sets the equip locations of the equipment
 *
 * Inputs: 5 * bool - values of can be equipped at each location
 * Output: none
 */
void Equipment::setEquipLocations(bool hd, bool la, bool ra, bool to, bool lg)
{
  flags = HEAD | LEFTARM | RIGHTARM | TORSO | LEGS;
}
