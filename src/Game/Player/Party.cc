
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
    : pouch(inventory)
{
  setPartyType(party);
  updateMaxSize();
  members.push_back(p_main);

  setFlag(Party::INVENTORY_ENABLED, true);
  setFlag(Party::MEMBERS_ENABLED, true);
}

/*
 * Description: Annihilates a party object
 */
Party::~Party() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Sets the enumerated party type object. Usually called by the
 *              party constructor.
 *
 * Inputs: EnumDb::PartyType - party type enumeration to set the party to.
 * Output: none
 */
void Party::setPartyType(EnumDb::PartyType new_party_type)
{
  party_type = new_party_type;
}

/*
 * Description: Updates the maximum size of the party based on the enumerated
 *              party type that the party is.
 *
 * Inputs: none
 * Output: none
 */
ushort Party::updateMaxSize()
{
  ushort party_max = 0;

  switch (party_type)
  {
    case (EnumDb::REGULAR_FOE):
    case (EnumDb::MINI_BOSS):
    case (EnumDb::BOSS):
    case (EnumDb::FINAL_BOSS):
        party_max = getMaxFoeSize();
        break;
    case (EnumDb::SLEUTH):
      party_max = getMaxSleuthSize();
      break;
    case(EnumDb::BEARACKS):
      party_max = getMaxBearacksSize();
      break;
  }

  return party_max;
}

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
  bool can_add_member = true;

  if (!getFlag(Party::MEMBERS_ENABLED))
    can_add_member = false;

  if (can_add_member && (int)getPartySize() < getMaxSize())
    if (person != NULL)
      members.push_back(person);

  return true;
}

/*
 * Description: Removes a person from the party at given index
 *
 * Inputs: int - index of party member needing to be removed
 * Output: bool - true if person was able to be removed
 */
bool Party::removeMember(uint i)
{
  bool can_remove = true;

  if (members.size() < 2 || (int)i >= members.size())
    can_remove = false;

  if (!getFlag(Party::MEMBERS_ENABLED))
    can_remove = false;

  if (can_remove)
  {
    members[i] = 0;
    members.removeAt(i);
  }

  return can_remove;
}

/*
 * Description: Removes a person from the party of a given name
 *
 * Inputs: QString - name of the person to be removed
 * Output: bool - true if person was removed succesfully
 */
bool Party::removeMember(QString value)
{
  bool found = false;

  QList<Person*>::iterator it;
  for (it = members.begin(); it < members.end(); ++it)
  {
    if ((*it)->getName() == value)
    {
      members.erase(it);
      found = true;
    }
  }

  return found;
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
  members.erase(members.begin() + 1, members.end());
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
  qDebug() << "Inventory Name: " << pouch->getName();
  qDebug() << "Party Members: " << members.size();
  qDebug() << "Max Size: " << max_size;

  uint index = 0;
  QList<Person*>::iterator it;
  for (it = members.begin(); it < members.end(); ++it)
    qDebug() << " Member #" << index++ << ": " << (*it)->getName();
}

/*
 * Description: Prints out the state of the PartyFlags.
 *
 * Inputs: none
 * Output: none
 */
void Party::printFlags()
{
  qDebug() << "Inventory Enabled: " << getFlag(Party::INVENTORY_ENABLED);
  qDebug() << "Members Locked: " << getFlag(Party::MEMBERS_ENABLED);
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
bool Party::getFlag(PartyFlag flag)
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
QString Party::getMemberName(ushort index)
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
  bool set_new_size = false;
  ushort party_type_max = updateMaxSize();

  if (value > 0 && value <= party_type_max)
  {
    max_size = value;
    set_new_size = true;
  }

  return set_new_size;
}

/*
 * Description: Sets the value of a PartyFlag flag
 *
 * Inputs: PartyFlag - flag to be set
 *         bool      - value to set the flag to
 * Output: none
 */
void Party::setFlag(PartyFlag flag, bool set_value)
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
