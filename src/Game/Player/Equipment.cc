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
  // INITIALIZE SIGNATURE

  for (int i = 0; i < kMAX_X; i++)
      for (int j = 0; j < kMAX_Y; j++)
          bubby_signature[i][j] = NULL;
}

/*
 * Description: Equipment class destructor
 */
Equipment::~Equipment()
{
  for (int i = 0; i < kMAX_X; i++)
    for (int j = 0; j < kMAX_Y; j++)
      if (bubby_signature[i][j] != NULL)
        qDebug() << "WARNING: DELETING EQUIPMENT WITHOUT REMOVING BUBBIES";
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Checks if the bubby will fit into the bubby signature
 *
 * Inputs: Bubby* - pointer to bubby to be checked for attachment capability
 *         int x - the left most coordinate of the proposed location
 *         int y - the top most coordinate of the proposed location
 * Output: bool - boolean if the bubby can be attached
 */
const bool Equipment::canAttach(Bubby* new_bubby, ushort x, ushort y)
{
  if (new_bubby->getTier() == 0)
    return FALSE;
  if (signature[x][y] == 'O')
  {
    if (new_bubby->getTier() == 2 && signature[x][y - 1] == 'O')
      return TRUE;
    if (new_bubby->getTier() == 3)
      if (signature[x + 1][y] == 'O' && signature[x + 1][y - 1] == 'O')
        return TRUE;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Attempts to attach the bubby into the signature. WARNING:
 *              Only give it the left most and top most coordinate!
 *
 * Inputs: Bubby* - pointer of the bubby to be attached
 *         int x - the left most coordinate
 *         int y - the top most coordinate
 * Output: bool - true if the attachment was successful
 */
const bool Equipment::attachBubby(Bubby* new_bubby, ushort x, ushort y)
{
  if (new_bubby->getTier() == 0)
    return FALSE;

  bubby_signature[x][y] = new_bubby;
  signature[x][y] = 'B';

  if (new_bubby->getTier() > 1)
      signature[x][y - 1] = 'C';
  if (new_bubby->getTier() == 3)
  {
      signature[x + 1][y] = 'C';
      signature[x + 1][y - 1] = 'C';
  }
  return TRUE;
}

/*
 * Description: Checks if a Bubby at a given index can be unattached
 *
 * Inputs: ushort x - x coordinate of the Bubby
 *         ushort y - y coordinate of the Bubby
 * Output: bool - TRUE if the Bubby can be unattached
 */
const bool Equipment::canUnattach(ushort x, ushort y)
{
  int left_x = getLeftX(x, y, FALSE);
  int left_y = getLeftY(y, y, FALSE);

  if (left_x == -1 || left_y == -1)
    return FALSE;

  if (signature[left_x][left_y] == 'B')
    return TRUE;
  return FALSE;
}

/*
 * Description: Checks if a Bubby of a given ID can be unattached
 *
 * Inputs: ushort x - x coordinate of the Bubby
 *         ushort y - y coordinate of the Bubby
 * Output: bool - TRUE if the Bubby can be unattached
 */
const bool Equipment::canUnattach(uint id)
{
  // TODO: FIND X AND Y
  // int left_x = getLeftX(x, y, TRUE);
  // int left_y = getLeftY(y, y TRUE);

  //if (left_x == -1 || left_y == -1)
    return FALSE;

  //if (signature[left_x][left_y] == 'B')
    return TRUE;
  return FALSE;
}

/*
 * Description: Unattaches a Bubby at a given (leftmost x, leftmost y) coord
 *
 * Inputs: ushort x - leftmost x coordinate of the Bubby
 *         ushort y - leftmost y coordinate of the Bubby
 * Output: Bubby - the (now) unattached Bubby
 */
Bubby Equipment::unattach(ushort x, ushort y)
{
  int left_x = getLeftX(x, y, FALSE);
  int left_y = getLeftY(y, y, FALSE);

  if (bubby_signature[left_x][left_y]->getTier() > 0)
      signature[left_x][left_y] = 'O';
  if (bubby_signature[left_x][left_y]->getTier() > 1)
    signature[left_x][left_y - 1] = 'O';
  if (bubby_signature[left_x][left_y]->getTier() > 2)
  {
    signature[left_x + 1][left_y] = 'O';
    signature[left_x + 1][left_y - 1] = 'O';
  }
  return *bubby_signature[left_x][left_y];
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
 * Description: Gets the X-coordinate on the signature of a Bubby given its
 *              unique ID. Returns -1 if not found.
 *
 * Inputs: uint - unique ID or x-value of Bubby to be checked
 *         bool - TRUE if passing an ID rather than an x-coordinate
 * Output: int - the leftmost x-coordinate of the Bubby if found, -1 otherwise
 // TODO: CORNER CASES
 */
int Equipment::getLeftX(uint x, uint y, bool passingID)
{
  if (signature[x][y] == 'B')
    return x;
  if (signature[x][y - 1] == 'B')
    return x;
  if (signature[x - 1][y] == 'B')
    return x - 1;
  if (signature[x - 1][y - 1] == 'B')
    return x - 1;
  return -1;
}

/*
 * Description: Gets the X-coordinate on the signature of a Bubby given its
 *              unique ID. Returns -1 if not found.
 *
 * Inputs: uint - unique ID or y-value of Bubby to be checked
 *         bool - TRUE if passing an ID rather than a y-coordinate
 * Output: int - the leftmost y-coordinate of the Bubby if found, -1 otherwise
 // TODO: CORNER CASES
 */
int Equipment::getLeftY(uint x, uint y, bool passingID)
{
    if (signature[x][y] == 'B')
      return y;
    if (signature[x - 1][y] == 'B')
        return y;
    if (signature[x][y + 1] == 'B')
        return y + 1;
    if (signature[x -1][y + 1] == 'B')
        return y + 1;
    return -1;
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

