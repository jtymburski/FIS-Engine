/******************************************************************************
* Class Name: Inventory
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Inventory class
* TODO: CHECK FOR FLAGS (Stackable, etc.)
******************************************************************************/
#include "Game/Player/Inventory.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructs an inventory object with a name, based on given
 *              values or on a basic limit (kBASIC_LIMIT). The inventory
 *              is created empty.
 *
 * Inputs: none
 * Output: uint - limit of bubby items.
 */
Inventory::Inventory(QString name, QImage thumbnail, uint limit, QWidget* parent)
{
  upgrade(name, thumbnail, limit, limit, limit);
  resetLevel(0);
}

/*
 * Description: Destroys an Inventory object
 */
Inventory::~Inventory()
{
    equipments.clear();
    items.clear();
    key_items.clear();
    bubbies.clear();
    equip_count.clear();
    item_count.clear();
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Attempts to add an equipment to the equipments QVector.
 *              Will fail if too many of the same equipment exist in the
 *              inventory or if there would be too many total equipments.
 *
 * Inputs: Equipment - the piece to be added
 * Output: bool - TRUE if the equipment can be added
 */
bool Inventory::addTo(Equipment equipment)
{
  if (getEquipmentCount() >= equip_limit)
    return FALSE;

  /* Increment counter if an equipment with the same name already exists */
  for (uint i = 0; i < equipments.size(); i++)
  {
    if (equipments.value(i).getName() == equipment.getName())
    {
      if (equip_count[i] < kMAX_EQUIP_STACK)
      {
        equip_count[i]++;
        return TRUE;
      }
      else
        return FALSE;
      }
  }

  /* Append the item to the QVector if it was not found */
  equipments.push_back(equipment);
  equip_count.push_back(1);
  return TRUE;
}

/*
 * Description: Attempts to add an item to the items QVector. Will fail if
 *              too many of the same item exist in the inventory or if the
 *              total amount of items would be exceeded.
 *
 * Inputs: none
 * Output: uint - limit of bubby items.
 */
bool Inventory::addTo(Item item)
{
  if (getItemCount() >= item_limit)
    return FALSE;

  /* Increment counter if an item with the same name already exists */
  for (uint i = 0; i < items.size(); i++)
  {
    if (items.value(i).getName() == item.getName())
    {
      if (item_count[i] < kMAX_ITEM_STACK)
      {
        item_count[i]++;
        return TRUE;
      }
      else
      return FALSE;
    }
  }

  /* Append the item to the QVector if it was not found */
  items.push_back(item);
  item_count.push_back(1);
  return TRUE;
}

/*
 * Description: Attempts to add a bubby to the bubbies QVector. Will fail if
 *              the total amount of bubbies permitted would be exceeded.
 *
 * Inputs: Bubby to be added to inventory.
 * Output: bool - TRUE if the addition was successful
 */
bool Inventory::addTo(Bubby bubby)
{
    if (getBubbyCount() >= bubby_limit)
      return FALSE;
    bubbies.push_back(bubby);
    return TRUE;
}

/*
 * Description: Attempts to remove an equipment from the QVector of
 *              equipments. If successful, will decrement the item count and
 *              remove the item and returns true. Otherwise, fails.
 *
 * Inputs: none
 * Output: uint - limit of bubby items.
 */
const bool Inventory::removeFrom(Equipment terminal_equipment)
{
  for (uint i = 0; i < items.size(); i++)
  {
    if (equipments.value(i).getName() == terminal_equipment.getName())
    {
      equip_count[i]--;
      if (equip_count[i] == 0)
      {
        equipments.remove(i);
        equip_count.remove(i);
        return TRUE;
      }
    }
  }
  return FALSE;
}

/*
 * Description: Attempts to remove an item from the QVector of general items
 *              If successful, will decrement the item count and remove the
 *              item and returns true. Otherwise, fails.
 *
 * Inputs: none
 * Output: uint - limit of bubby items.
 */
const bool Inventory::removeFrom(Item terminal_item)
{
  for (int i = 0; i < items.size(); i++)
  {
    if (items.value(i).getName() == terminal_item.getName())
    {
      item_count[i]--;
      if (item_count.at(i) == 0)
      {
        items.remove(i);
        item_count.remove(i);
        return TRUE;
      }
    }
  }
  return FALSE;
}

/*
 * Description: Attempts to remove a bubby from the vector of bubby items
 *
 * Inputs(
 * Output: uint - limit of bubby items.
 */
const bool Inventory::removeFrom(Bubby bubby)
{

}

/*
 * Description: Upgrades the inventory, given a new name and thumbnail.
 *              Attempts to upgrade the item limits and the level of the
 *              inventory, but will stop at the constant max values set
 *              set in Inventory.h
 *
 * Inputs: none
 * Output: none
 */
void Inventory::upgrade(QString name, QImage thumbnail, uint equipment_limit,
                        uint item_limit, uint bubby_limit)
{
  this->name = name;
  thumb = thumbnail;
  (item_limit <= kMAX_ITEM_LIMIT) ? (this->item_limit = item_limit) :
                                    (this->item_limit = kMAX_ITEM_LIMIT);
  (equip_limit <= kMAX_EQUIP_LIMIT) ? (this->equip_limit = equipment_limit) :
                                      (this->equip_limit = kMAX_EQUIP_LIMIT);
  (bubby_limit <= kMAX_BUBBY_LIMIT) ? (this->bubby_limit = bubby_limit) :
                                        (this->equip_limit = kMAX_EQUIP_LIMIT);
  (level + 1 <= kMAX_LEVEL) ? (level++) : (level = kMAX_LEVEL);
}

/*
 * Description:  function sets the level of the inventory. It should only
 *              need to be called when doing something with the inventory when
 *              you do not want upgrade() to increment it.
 *
 * Inputs: new_level = value to change level to
 * Output: none
 */
const bool Inventory::resetLevel(const uint new_level)
{
    if (level >= kMAX_LEVEL)
      return FALSE;
    level = new_level;
    return true;
}

/*
 * Description: Returns the count on the number of a specific equipment
 *              in the inventory, including 0 if the item was not found.
 *
 * Inputs: e - Equipment to be checked for
 * Output: uint - number of the equipment in the inventory
 */
uint Inventory::getCount(Equipment e)
{
  for (uint i = 0; i < equipments.size(); i++)
    if (equipments.value(i).getName() == e.getName())
      return equip_count.at(i);
  return 0;
}

/*
 * Description: Returns the count of the number of a specific item in the
 *              inventory, returns 0 if the item was not found.
 *
 * Inputs: item - Item to be checked for
 * Output: uint - number of the item in the inventory
 */
uint Inventory::getCount(Item item)
{
  for (int i = 0; i < items.size(); i++)
      if (items.value(i).getName() == item.getName())
      return item_count.at(i);
  return 0;
}

/*
 * Description: Returns the current number of bubby items.
 *
 * Inputs: none
 * Output: uint - count of the bubby items.
 */
uint Inventory::getBubbyCount()
{
    return bubbies.size();
}

/*
 * Description: Returns the limit of bubby items.
 *
 * Inputs: none
 * Output: uint - limit of bubby items.
 */
uint Inventory::getBubbyLimit()
{
  return bubby_limit;
}

/*
 * Description: Gets the limit of equipment items.
 *
 * Inputs: none
 * Output: uint - limit of equipment items.
 */
uint Inventory::getEquipmentLimit()
{
  return equip_limit;
}

/*
 * Description: Gets the count of equipment items.
 *
 * Inputs: none
 * Output: uint - limit of equipment item storage
 */
uint Inventory::getEquipmentCount()
{
  uint count = 0;
  foreach (const uint &item, equip_count)
    count += item;
  return count;
}

/*
 * Description: Gets the limit on the number of general items.
 *
 * Inputs: none
 * Output: uint - limit of general item storage
 */
uint Inventory::getItemLimit()
{
  return item_limit;
}

/*
 * Description: Returns the count of the general items in the inventory.
 *
 * Inputs: none
 * Output: uint - count of the general items in the inventory
 */
uint Inventory::getItemCount()
{
  uint count = 0;
  foreach(const uint &item, item_count)
    count += item;
  return count;
}

/*
 * Description: Returns the integer level value of the inventory.
 *
 * Inputs: none
 * Output: uint - level of the inventory.
 */
uint Inventory::getLevel()
{
    return level;
}

/*
 * Description: Returns the name of the inventory.
 *
 * Inputs: none
 * Output: QString of the name
 */
QString Inventory::getName()
{
    return name;
}

/*
 * Description: Returns the thumbnail image of the invetory.
 *
 * Inputs: none
 * Output: QImage of the inventory's thumbnail
 */
QImage Inventory::getThumbnail()
{
    return thumb;
}

