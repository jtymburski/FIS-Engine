/*******************************************************************************
* Class Name: Item Buffer [Implementation]
* Date Created: October 27th, 2013
* Inheritance: None
* Description: Item Buffer stores the stack of items which have been chosen by
*              players or enemies to be used. In addition it contains the user
*              of these items, and the targets thesee items are aimed at hitting.
*******************************************************************************/

#include "Game/Battle/ItemBuffer.h"

/*==============================================================================
 * CONSTANTS
 *============================================================================*/

const ushort ItemBuffer::kMAXIMUM_ELEMENTS   = 20;
const ushort ItemBuffer::kMAXIMUM_TARGETS    = 10;
const ushort ItemBuffer::kSTARTING_ELEMENT   =  0;

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */

ItemBuffer::ItemBuffer()
{
  curr_index = kSTARTING_ELEMENT;
}

/*
 * Description: Annihilates an ItemBuffer object.
 *
 */
ItemBuffer::~ItemBuffer() {}

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Checks the validity of an ItemUseAction. Will check that the user
 *              of the item is alive, in battle, and has item use enabled. Will
 *              also check that the item is battle ready, the target vector is
 *              not empty, and that the targets are all in battle.
 *
 * Inputs: ItemUseAction - struct describing to element of the ItemBuffer
 * Outputs: bool - the validity of the ItemUseAction struct
 */
bool ItemBuffer::checkValid(ItemUseAction item_use_action)
{
  bool is_valid = true;

  /* If the user of the item is both alive, in battle, and can use items */
  is_valid &= item_use_action.user->getPersonFlag(Person::ALIVE);
  is_valid &= item_use_action.user->getPersonFlag(Person::IN_BATTLE);
  is_valid &= item_use_action.user->getPersonFlag(Person::ITEM_USE_ENABLED);

  /* If the item to be used is BattleReady */
  is_valid &= item_use_action.item_used->getItemFlag(Item::BATTLEREADY);

  /* If the targets of the item are empty */
  is_valid &= !item_use_action.targets.empty();

  /* Iterate through the targets and assert they are in battle */
  QVector<Person*>::Iterator it = item_use_action.targets.begin();

  for (; it < item_use_action.targets.end(); ++it)
    is_valid &= (*it)->getPersonFlag(Person::IN_BATTLE);

  /* Assign the validity of the item_use_action */
  item_use_action.valid_item_use = is_valid;

  return is_valid;
}

/*
 * Description: Clears the list of ItemUseActions. This does not delete the
 *              data of the person or items contained in the struct.
 *
 * Inputs: none
 * Outputs: none
 */
void ItemBuffer::clearAll()
{
  item_buffer.clear();
  curr_index = kSTARTING_ELEMENT;
}

/*
 * Description: Returns a single element of the Item Buffer given an index.
 *
 * Inputs: int index - the index of the item buffer to be checked.
 * Outputs: The ItemUseAction struct at that index, if one exists
 */
ItemBuffer::ItemUseAction ItemBuffer::getIndex(int index)
{
  if (index < item_buffer.size())
    return item_buffer.at(index);

  return item_buffer.at(kSTARTING_ELEMENT);
}

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add a use of an Item to the Item Use Buffer given
 *              a user, an item to be used, and targets. This may fail if the
 *              person is not in battle or dead, the item is invalid, or the
 *              target vector is empty.
 *
 * Inputs: Person* user - the potential user of the item
 *         Item* item_used - the item to be used
 *         QVector<Person*> targets - the targets the item is to target
 * Outputs: bool - the validity of the new ItemUseAction
 */
bool ItemBuffer::addItemUse(Person* user, Item* item_used,
                            QVector<Person*> targets)
{
  bool add_to_buffer = true;

  /* Assert the user and item to be used exist */
  if (user == 0 || item_used == 0)
    add_to_buffer = false;

  /* If the new size of the ItemBuffer will be within within bounds */
  if (item_buffer.size() >= kMAXIMUM_ELEMENTS)
    add_to_buffer = false;

  if (add_to_buffer)
  {
    struct ItemUseAction item_use;

    item_use.user = user;
    item_use.item_used = item_used;
    item_use.targets = targets;

    add_to_buffer &= checkValid(item_use);

    if (add_to_buffer)
      item_buffer.push_back(item_use);
  }

  return add_to_buffer;
}

/*
 * Description: Clears only the invalid elements in the item buffer by creating
 *              a temporary buffer and only adding in the valid elements.
 *
 * Inputs: none
 * Outputs: none
 */
void ItemBuffer::clearInvalid()
{
  /* Create an empty temp buffer of ItemUseAction structs */
  QList<ItemUseAction> temp;

  /* Iterate through the original item buffer */
  QList<ItemUseAction>::Iterator it = item_buffer.begin();

  for (; it < item_buffer.end(); ++it)
  {
    /* Add the valid elements to the temp buffer */
    if ((*it).valid_item_use)
      temp.push_back(*it);
  }

  /* Re-assign the actual item buffer to the temp, now containing valid elems. */
  item_buffer = temp;
  curr_index = kSTARTING_ELEMENT;
}

/*
 * Description: Re-Evaluates whether the next element (if one exists) in the item
 *              buffer and return the state of that validity.
 *
 * Inputs: none
 * Outputs: bool - evaluation of the validity of the next ItemUseAction
 */
bool ItemBuffer::isNextValid()
{
  if (curr_index < item_buffer.size())
    return checkValid(getIndex(curr_index + 1));
  return false;
}

/*
 * Description: Method for calling all print functions for the ItemBuffer.
 *
 * Inputs: none
 * Outputs: none
 */
void ItemBuffer::printAll()
{
  qDebug() << "--- Item Buffer --- ";
  printInfo();

  for (int i = 0; i < item_buffer.size(); i++)
    printElement(i);

  qDebug() << "--- / Skill Buffer --";
}

/*
 * Description: Method for printing out the basic info describing the item buffer
 *
 * Inputs: none
 * Outputs: none
 */
void ItemBuffer::printInfo()
{
  qDebug() << "Current Item Buffer Index: " << curr_index;
  qDebug() << "Current Item Buffer Size: " << item_buffer.size();
}

/*
 * Description: Prints out the data describing an element in the ItemBuffer
 *              including the index, the user's name, the item's name, the
 *              size of the target vector, and the boolean valid item use.
 *
 * Inputs: int index - the index of the item buffer to print info for
 * Outputs: bool - true if the element exists
 */
bool ItemBuffer::printElement(int index)
{
  if (index >= 0 && index < item_buffer.size())
  {
    qDebug() << "Element Index: " << index;
    qDebug() << "User Name: " << item_buffer.at(index).user->getName();
    qDebug() << "Item Name: " << item_buffer.at(index).item_used->getName();
    qDebug() << "Target Size: " << item_buffer.at(index).targets.size();
    qDebug() << "Valid Item Use: " << item_buffer.at(index).valid_item_use;
    return true;
  }
  qDebug() << "INVALID ELEMENT INDEX";
  return false;
}

/*
 * Description: Attempts to remove an ItemUseAction from the item buffer being
 *              given a user. In the case of more than one item use, this
 *              function will remove the first occurence of an ItemUseAction by
 *              the user.
 *
 * Inputs: Person* user - the user of the ItemUseAction to be checked for
 * Outputs: bool - true if an ItemUseAction element was found and removed
 */
bool ItemBuffer::removeItemUse(Person* user)
{
  /* Iterate through the ItemBuffer and check for user equivalencies */
  QList<ItemUseAction>::Iterator it = item_buffer.begin();

  for (; it < item_buffer.end(); ++it)
  {
    /* If the user is found, delete the ItemBuffer at that point */
    if ((*it).user == user)
    {
      item_buffer.erase(it);
      return true;
    }
  }

  return true;
}

/*
 * Description: Attempts to remove an ItemUseAction element from the item buffer
 *              given an index of an element to be removed.
 *
 * Inputs: int index - index of element to be removed
 * Outputs: bool - true if the element exists and was removed
 */
bool ItemBuffer::removeItemUse(int index)
{
  if (index >= 0 && index < item_buffer.size())
  {
    item_buffer.removeAt(index);
    return true;
  }

  return false;
}

/*
 * Description: Returns a pointer to the user of the current element if the
 *              current index on the item Buffer is valid.
 *
 * Inputs: none
 * Outputs: Person* - pointer to the person on the curr_index of the ItemBuffer
 */
Person* ItemBuffer::getUser()
{
  if (checkValid(getIndex(curr_index)))
    return item_buffer.at(curr_index).user;

  return 0;
}

/*
 * Description: Returns a pointer to the item of the current element if the next
 *              index on the ItemBuffer is valid.
 *
 * Inputs: none
 * Outputs: Item* - pointer to the item on the curr_index of the ItemBuffer
 */
Item* ItemBuffer::getItemUse()
{
  if (checkValid(getIndex(curr_index)))
    return item_buffer.at(curr_index).item_used;

  return 0;
}

/*
 * Description: Returns the vector of targets on the current element if the
 *              current index of the ItemBuffer is valid
 *
 * Inputs: none
 * Outputs: QVector<Person*> - the vector of targets on the curr_index
 */
QVector<Person*> ItemBuffer::getTargets()
{
  if (checkValid(getIndex(curr_index)))
    return item_buffer.at(curr_index).targets;

  QVector<Person*> empty_targets;
  return empty_targets;
}

/*
 * Description: Sets the curr_index of the ItemBuffer to the next value, unless
 *              the index is at the end, in which case the ItemBuffer is cleared.
 *
 * Inputs: none
 * Outputs: bool - true if the next index was set, false if the buffer is cleared
 */
bool ItemBuffer::setNextIndex()
{
  /* If the next current index is not the last element of the item buffer */
  if (curr_index + 1 < item_buffer.size())
  {
    curr_index++;
    return true;
  }

  /* Otherwise the next index does not exist, clear the Item Buffer */
  clearAll();
  return false;
}
