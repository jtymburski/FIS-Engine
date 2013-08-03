
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
Party::Party(Person* p_main, Inventory* inventory, EnumDb::PartyType party)
    : pouch(inventory),
      party_type(party)

{
  members.push_back(p_main);

  switch (party_type)
  {
    case(EnumDb::SLEUTH):
      setMaxSize(getMaxSleuthSize());
      break;

    case(EnumDb::REGULAR_FOE):
    case(EnumDb::MINI_BOSS):
    case(EnumDb::BOSS):
    case(EnumDb::FINAL_BOSS):
      setMaxSize(getMaxFoeSize());
      break;

  case(EnumDb::BEARACKS):
      setMaxSize(getMaxBearacksSize());
      break;
  };
}

/*
 * Description: Annihilates a party object
 */
Party::~Party() {}

/*============================================================================
 * PUBLIC FUNCTIONS
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
    if (person != NULL)
      members.push_back(person);
      return true;
  }
  return false;
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
    return false;
  if (members.at(index))
  {
    delete members.at(index);
    members[index] = 0;
    members.remove(index);
    return true;
  }
  return false;
}

/*
 * Description: Removes a person from the party of a given name
 *
 * Inputs: QString - name of the person to be removed
 * Output: bool - true if person was removed succesfully
 */
bool Party::removeMember(QString value)
{
  if (members.size() < 2)
    return false;
  for (ushort i = 1; i < members.size(); i++)
  {
    if (members.at(i)->getName() == value)
    {
      delete members.at(i);
      members[i] = 0;
      members.remove(i);
      return true;
    }
  }
  return false;
}

/*
 * Description: Uses an item given a primary target.
 *
 * Inputs: Item* used_item - the item to be used.
 *         ushort target - the primary target for the item (it may be multihit)
 * Output: bool - true if the item was used succesfully
 */
bool Party::useItem(Item *used_item, ushort target)
{
  /* Assert the person can use items and the item is menu-usable */
  if (members[target]->getPersonFlag(Person::CANUSEITEM));
    return false;
  if (!used_item->getItemFlag(Item::MENUREADY))
    return false;
  if (target > getMaxSize())
    return false;
  /*
  QVector<Skill*> to_do = used_item->getActionSet()->getSkills();

  for (int i = 0; i < to_do.size(); i++)
  {
    QVector<Action*> to_be_done = to_do.at(i)->getEffects();

    for (int j = 0; j < to_be_done.size(); j++)
    {
      Action* cur_action = to_be_done.at(j);
      cur_action->getMinimum()

      if (cur_action->getActionFlag(Action::OFFENSIVE))
      {
        if (cur_action->getActionFlag(Action::PHYSICAL))
        {
          if (cur_action->getActionFlag(Action::RAISE))
          {
            if (to_do.at(i)->getFlag(Skill::MULTIHIT))
              for (int k = 0; k < getPartySize(); k++)
              {
                  members.at(k)->tempStats()->changeStat("PHAG",)
              }

          }

          }
          if (cur_action->getActionFlag(Action::LOWER))
          {

          }
        }

        if (cur_action->getActionFlag(Action::THERMAL))
        {

        }

        if (cur_action->getActionFlag(Action::POLAR))
        {

        }

        if (cur_action->getActionFlag(Action::PRIMAL))
        {

        }

        if (to_be_done.at(j)->getActionFlag(Action::CHARGED))
        {

        }

        if (to_be_done.at(j)->getActionFlag(Action::CYBERNETIC))
        {

        }

        if (to_be_done.at(j)->getActionFlag(Action::NIHIL))
        {

        }

        if (cur_action->getActionFlag(Action::DEFENSIVE))
        {

        }
      }
    }
    */
}

/*
 * Description: Clears the party (except for the main character (index 0))
 *
 * Inputs: none
 * Output: none
 */
void Party::clearParty()
{
  for (uint i = 1; i < members.size(); i++)
    members.remove(i);
}

/*
 * Description: Prints out all the information of the state of the party by
 *              calling sub-print classes.
 *
 * Inputs: none
 * Output: none
 */
void Party::printAll()
{
  qDebug() << "---  Party --- ";
  printInfo();
  printFlags();
  qDebug() << "--- / Party --- ";
}

/*
 * Description: Prints out all the basic info of the state of the party
 *              including its members.
 *
 * Inputs: none
 * Output: none
 */
void Party::printInfo()
{
    // qDebug() << "Inventory Name: " << pouch->getName();
    qDebug() << "Party Members: " << members.size();
    qDebug() << "Max Size: " << max_size;

    for (int i = 0; i < members.size(); i++)
      qDebug() << " Member #" << i << ": " << members.at(i)->getName();
}

/*
 * Description: Prints out the state of the PartyFlags.
 *
 * Inputs: none
 * Output: none
 */
void Party::printFlags()
{

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
  return 0;
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
ushort Party::getPartySize()
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
ushort Party::getMaxSize()
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
 * Output: bool - true if party size was successfully changed
 */
bool Party::setMaxSize(ushort value)
{
  int party_type_max = 0;

  switch (party_type)
  {
    case(EnumDb::SLEUTH):
      party_type_max = getMaxSleuthSize();
      break;
    case (EnumDb::REGULAR_FOE):
    case(EnumDb::MINI_BOSS):
    case(EnumDb::BOSS):
    case(EnumDb::FINAL_BOSS):
      party_type_max = getMaxFoeSize();
      break;
    case(EnumDb::BEARACKS):
      party_type_max = getMaxBearacksSize();
      break;
  }

  if (value > 0 && value <= party_type_max)
  {
    max_size = value;
    return true;
  }
  return false;
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
  (set_value) ? (pflag_set |= flag) : (pflag_set &= flag);
}

/*============================================================================
 * STATIC PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the maximum number of members a Bearacks can contain
 *
 * Inputs: none
 * Output: const ushort - the maximum number of Bearacks members.
 */
const ushort Party::getMaxBearacksSize()
{
  return kMAX_MEMBERS_BEARACKS;
}

/*
 * Description: Returns the maximum number of members for a party of foes.
 *
 * Inputs: none
 * Output: const ushort - the maximum number of Bearacks members.
 */
const ushort Party::getMaxFoeSize()
{
  return kMAX_MEMBERS_FOE;
}

/*
 * Description: Returns the maximum number of members for a sleuth.
 *
 * Inputs: none
 * Output: const ushort - the maximum number of Bearacks members.
 */
const ushort Party::getMaxSleuthSize()
{
  return kMAX_MEMBERS_SLEUTH;
}
