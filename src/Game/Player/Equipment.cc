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

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Equipment class consructor
 *
 * Inputs: none
 */
Equipment::Equipment()
{

}

/*
 * Description: Equipment class destructor
 */
Equipment::~Equipment() {}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Clears the list of actions
 *
 * Inputs: none
 * Output: none
 */
void Equipment::clearActionList()
{
  // action_list.clear();
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
bool Equipment::isBubbyAttachable(Bubby* bubby, uint x, uint y)
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
bool Equipment::attachBubby(Bubby* bubby, uint x, uint y)
{
  // TODO: Bubby attachments! [02-01-13]
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
 * Description: Toggles a given EquipmentState flag
 *
 * Inputs: EquipmentState flag to be toggled
 * Output: none
 */
void Equipment::toggleEquipmentFlag(EquipmentState flag)
{
    setEquipmentFlag(flag, !getEquipmentFlag(flag));
}


/*
 * Description: Evaluates a given EquipmentState flag
 *
 * Inputs: EquipmentState flag to be evaluated
 * Output: boolean value of the flag
 */
const bool Equipment::getEquipmentFlag(EquipmentState flag)
{
    return (eflag_set.testFlag(flag));
}

/*
 * Description: Given an EquipmentFlags object and a bool, sets the flags
 *              contained in the object to the boolean value.
 *
 * Inputs: EquipmentState flag to be set
 *         set_value - boolean value to set flags to
 * Output: none
 */
void Equipment::setEquipmentFlag(EquipmentState flag, const bool set_value)
{
    (set_value) ? (eflag_set |= flag) : (eflag_set ^= flag);
}

