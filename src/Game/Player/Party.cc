/******************************************************************************
* Class Name: Party Implementation
* Date Created: December 2nd, 2012
* Inheritance: None?
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
Party::Party(Person* p_main, uint max, Inventory* inventory, QWidget* parent)
{
    members.push_back(p_main);

    setMaxSize(max);
    setInventory(inventory);
}

/*
 * Description: Annihilates a party object
 */
Party::~Party()
{
    setInventory();
}

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
  if (members.size() < getMaxSize())
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
 * Output: bool - true if person was able to be removed
 */
bool Party::removeMember(uint index)
{
  if (members.size() < 2)
    return FALSE;
  if (members.at(index))
  {
    members.remove(index);
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Removes a person from the party from a given name
 * Note: cleanUp() should be called after this function is called
 *
 * Inputs: QString - name of the person to be removed
 * Output: bool - true if person was removed succesfully
 */
bool Party::removeMember(QString value)
{
    if (members.size() < 2)
        return false;
    for (uint i = 1; i < 5; i++)
    {
        if (members.at(i)->getName() == value)
        {
          members.remove(i);
          return true;
        }
    }
    return false;
}

/*
 * Description: Clears the party (except for the main character (index 0))
 *
 * Inputs: none
 * Output: none
 */
void Party::clearParty()
{
    for (uint i = 1; i < 5; i++)
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
  if (members.at(index))
    return members.at(index);
  return NULL;
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
  if (members.at(index))
    return members.at(index)->getName();
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
 * Description: Sets the main member of the party
 *
 * Inputs: Person* - pointer to new main person
 * Output: none
 */
void Party::setMainMember(Person* p)
{
  main = p;
}

/*
 * Description: Sets the temporary maximmum size of the party (sleuth)
 * Note: Maximum party size is five, minimum party size is 1.
 *
 * Inputs: int - new max size of the party
 * Output: bool - true if party size was successfully changed
 */
bool Party::setMaxSize(uint value)
{
  if (value > 0 && value < 6)
  {
    max_size = value;
    return true;
  }
  return false;
}
