
/******************************************************************************
* Class Name: Party Implementation
* Date Created: December 2nd, 2012
* Inheritance:
* Description: Party is an object holding the five members (Person class) of 
*              a sleuth
******************************************************************************/

#include "Game/Player/Party.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for a party object.
 */
Party::Party(Person* p_main, ushort max, Inventory* inventory, QWidget* parent)
    : QWidget(parent), max_size(kMAX_MEMBERS)
{
  /* Pointer setup */
  setInventory(inventory);

  members.push_back(p_main);
  if (max < kMAX_MEMBERS)
    setMaxSize(max);
}

/*
 * Description: Annihilates a party object
 */
Party::~Party() {}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds a person to a party
 *
 * Inputs: Person* - person to be added to the party
 * Output: bool - true if person was succesfully addedto the party
 */
bool Party::addMember(Person* person)
{
  if ((int)getPartySize() < getMaxSize())
  {
    members.push_back(person);
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Removes a person from the party at given index
 * Note: cleanUp() should be called after this function is called
 *
 * Inputs: int - index of party member needing to be removed
 * Output: bool - TRUE if person was able to be removed
 */
bool Party::removeMember(uint index)
{
  if (members.size() < 2)
    return FALSE;
  if (members.at(index))
  {
    delete members.at(index);
    members[index] = NULL;
    members.remove(index);
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Removes a person from the party from a given name
 *
 * Inputs: QString - name of the person to be removed
 * Output: bool - TRUE if person was removed succesfully
 */
bool Party::removeMember(QString value)
{
  if (members.size() < 2)
    return FALSE;
  for (ushort i = 1; i < kMAX_MEMBERS; i++)
  {
    if (members.at(i)->getName() == value)
    {
      delete members.at(i);
      members[i] = NULL;
      members.remove(i);
      return TRUE;
    }
  }
  return FALSE;
}

/*
 * Description: Clears the party (except for the main character (index 0))
 *
 * Inputs: none
 * Output: none
 */
void Party::clearParty()
{
  for (uint i = 1; i < kMAX_MEMBERS; i++)
    members.remove(i);
}

/*
 * Description: Returns a pointer to the party's inventory
 *
 * Inputs: none
 * Output: Inventory* - pointer to the party's inventory
 */
Inventory* Party::getInventory()
{
  return pouch;
}

/*
 * Description: Returns a pointer to a party member at a given index
 *
 * Inputs: int - index of party member to obtain
 * Output: Person* - pointer to a party member at given index
 */
Person* Party::getMember(uint index)
{
  if ((int)index < members.size())
    return members.at(index);
  return NULL;
}

/*
 * Description: Returns the value of a certain Party flag
 *
 * Inputs: PartyFlag - flag to evaluate
 * Output: bool      - evaluation of the flag
 */
bool Party::getPartyFlag(PartyFlag flag)
{
  return (pflag_set.testFlag(flag));
}

/*
 * Description: Returns the current size of the party
 *
 * Inputs: none
 * Output: int - current size of the party
 */
uint Party::getPartySize()
{
  return members.size();
}

/*
 * Description: Returns the name of a party member at a given index
 *
 * Inputs: int - index of party member to return name of
 * Output: QString - name of party member at index
 */
QString Party::getMemberName(uint index)
{
  if ((int)index < members.size())
    return members.at((int)index)->getName();
  return "";
}

/*
 * Description: Returns the current maximum size of the party
 *
 * Inputs: none
 * Outputs: int - maximum size of the party
 */
int Party::getMaxSize()
{
  return max_size;
}

/*
 * Description: Sets the main member of the party
 *
 * Inputs: Person* - pointer to new main person
 * Output: none
 */
void Party::setInventory(Inventory* i)
{
  pouch = i;
}

/*
 * Description: Sets the temporary maximmum size of the party (sleuth)
 *
 * Inputs: int - new max size of the party
 * Output: bool - TRUE if party size was successfully changed
 */
bool Party::setMaxSize(uint value)
{
  if (value > 0 && value <= kMAX_MEMBERS)
  {
    max_size = value;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Sets the value of a PartyFlag flag
 *
 * Inputs: PartyFlag - flag to be set
 *         bool      - value to set the flag to
 * Output: none
 */
void Party::setPartyFlag(PartyFlag flag, bool set_value)
{
  (set_value) ? (pflag_set |= flag) : (pflag_set ^= flag);
}
