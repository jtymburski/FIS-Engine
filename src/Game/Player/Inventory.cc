/******************************************************************************
* Class Name: Inventory
* Date Created: November 4th, 2012 (Rewritten June 1st, 2013)
* Inheritance: QWidget
* Description: Inventory class which acts as a container storage and sorting
*              mechanism for storing the player's Items, Equipments, and
*              Bubbies.
* Note [1]: The player will have access to two inventories: personal and ship:
*           the personal max capacity will be bound to certain limits and also
*           the gravity the player is experiencing, while the ship inventory
*           will only be bound to higher limits, and regardless of the gravity.
* TODO: Add, Remove, Sorting algorithms [06-02-03]
\*****************************************************************************/
#include "Game/Player/Inventory.h"

/*============================================================================
 * CONSTANTS
 *============================================================================*/
const double Inventory::kMIN_CARRY_MASS     = 100.00;
const double Inventory::kMAX_CARRY_MASS     = 50000.00;    /* 5 * 10 ^ 4 */
const ushort Inventory::kMIN_EACH_ITEM      = 3;
const ushort Inventory::kMAX_EACH_ITEM      = 99;
const ushort Inventory::kMIN_EACH_EQUIPMENT = 1;
const ushort Inventory::kMAX_EACH_EQUIPMENT = 99;
const uint Inventory::kMIN_BUBBIES          = 5;
const uint Inventory::kMAX_BUBBIES          = 1000000;     /* 10 ^ 6 */
const uint Inventory::kMIN_ITEMS            = 30;
const uint Inventory::kMAX_ITEMS            = 1000000000;  /* 10 ^ 9 */
const uint Inventory::kMIN_EQUIPMENT        = 3;
const uint Inventory::kMAX_EQUIPMENT        = 100000;      /* 10 ^ 5 */

/*============================================================================
 * CONSTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Inventory::Inventory(QString name, Sprite *thumb, QWidget *parent)
    : QWidget(parent)
{
  setName(name);
  setThumb(thumb);

  setBubbyState(NONE);
  setEquipState(NONE);
  setItemState(NONE);
  setKeyItemState(NONE);

  bubbies.resize(0);
  equipments.resize(0);
  items.resize(0);
  key_items.resize(0);
  calcMass();

  setLimits(kMIN_BUBBIES, kMIN_EACH_EQUIPMENT, kMIN_EQUIPMENT, kMIN_EACH_ITEM,
            kMIN_ITEMS, kMIN_CARRY_MASS);

  setDescription("????");

  /* Pointer setup */
  setBackdrop(NULL);

  /* Flag setup */
  setFlag(Inventory::PLAYERSTORAGE, false);
  setFlag(Inventory::SHIPSTORAGE, false);
  setFlag(Inventory::ENEMYSTORAGE, false);
  setFlag(Inventory::EMPTY, true);
  setFlag(Inventory::FULL, false);
  setFlag(Inventory::CANBEUPGRADED, true);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Inventory::~Inventory() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::calcMass()
{
  carry_mass = 0;

  for (int i = 0; i < bubbies.size(); i++)
      carry_mass += getBubby(i)->getMass();

  for (int i = 0; i < equipments.size(); i++)
      carry_mass += getEquip(i)->getMass();

  for (int i = 0; i < items.size(); i++)
      carry_mass+= getItem(i)->getMass();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::addBubby(Bubby* new_bubby)
{
  return true; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::addEquipment(Equipment* new_equipment)
{
  return true; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::addItem(Item* new_item)
{
  return true; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::addKeyItem(Item* new_item)
{
  return true; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Bubby* Inventory::removeBubby(uint index)
{
  return NULL; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Equipment* Inventory::removeEquipment(uint index)
{
  return NULL; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Item* Inventory::removeItem(uint index)
{
  return NULL; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Item* Inventory::removeItem(QString name)
{
  return NULL; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Item* Inventory::removeKeyItem(uint index)
{
  return NULL; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Item* Inventory::removeKeyItem(QString name)
{
  return NULL; //warning
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printAll()
{
  qDebug() << "--- Inventory --- ";
  printBasic();
  printBubbies();
  printEquipment();
  printFlags();
  printItems();
  printKeyItems();
  qDebug() << "--- / Inventory --- ";
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printBasic()
{
  qDebug() << "Name: " << name;
  qDebug() << "Desc. " << description;
  qDebug() << "Bubby Limit: " << bubby_limit;
  qDebug() << "Equip Limit: " << equip_limit;
  qDebug() << "Item Limit: " << item_limit;
  qDebug() << "Equip Each Limit: " << equip_each_limit;
  qDebug() << "Item Each L:imit: " << item_each_limit;
  qDebug() << "Carry Mass: " << carry_mass;
  qDebug() << "Max Carry Mass: " << max_carry_mass;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printBubbies()
{
  for (int i = 0; i < bubbies.size(); i++)
    qDebug() << "Bubby #" << i << " " << bubbies.value(i)->getType()->getName();
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printEquipment()
{
  for (int i = 0; i < equipments.size(); i++)
    qDebug() << "Equip #" << i << " " << equipments.value(i)->getName();
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printFlags()
{
  qDebug() << "PlayerStorage: " << getFlag(Inventory::PLAYERSTORAGE);
  qDebug() << "ShipStorage: " << getFlag(Inventory::SHIPSTORAGE);
  qDebug() << "EnemyStorage: " << getFlag(Inventory::ENEMYSTORAGE);
  qDebug() << "Empty: " << getFlag(Inventory::EMPTY);
  qDebug() << "Full: " << getFlag(Inventory::FULL);
  qDebug() << "CanBeUpgraded: " << getFlag(Inventory::CANBEUPGRADED);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printItems()
{
  for (int i = 0; i < items.size(); i++)
    qDebug() << "Item #" << i << " " << items.value(i)->getName();
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printKeyItems()
{
  for (int i = 0; i < key_items.size(); i++)
    qDebug() << "Key Item #" << i << " " << key_items.value(i)->getName();
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::sortBubbies(ItemSorts sort_by)
{

}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::sortEquipments(ItemSorts sort_by)
{

}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::sortItems(ItemSorts sort_by)
{

}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::sortKeyItems(ItemSorts sort_by)
{

}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Sprite* Inventory::getBackdrop()
{
  return backdrop;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Bubby* Inventory::getBubby(uint index)
{
  if (bubbies.size() > index)
    return bubbies.at(index);
  else
    return NULL;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
uint Inventory::getBubbyLimit()
{
  return bubby_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
QVector<Bubby*> Inventory::getBubbyList()
{
  return bubbies;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
ItemSorts Inventory::getBubbyState()
{
  return bubby_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
QString Inventory::getDescription()
{
  return description;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Equipment* Inventory::getEquip(uint index)
{
  if (equipments.size() > index)
      return equipments.at(index);
  else
      return NULL;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
uint Inventory::getEquipCount(QString name)
{
  ushort equip_count = 0;

  for (int i = 0; i < equipments.size(); i++)
      if (equipments.at(i)->getName() == name)
        equip_count++;

  return equip_count;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
uint Inventory::getEquipEachLimit()
{
  return equip_each_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
uint Inventory::getEquipLimit()
{
  return equip_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
ItemSorts Inventory::getEquipState()
{
 return equipment_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
QVector<Equipment*> Inventory::getEquipList()
{
  return equipments;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Inventory::getFlag(InventoryState flags)
{
  return invflag_set.testFlag(flags);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Item* Inventory::getItem(uint index)
{
  if (items.size() > index)
    return items.at(index);
  else
    return NULL;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
uint Inventory::getItemCount(QString name)
{
  ushort item_count = 0;

  for (int i = 0; i < items.size(); i++)
      if (items.at(i)->getName() == name)
        item_count++;

  return item_count;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
uint Inventory::getItemEachLimit()
{
  return item_each_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
uint Inventory::getItemLimit()
{
 return item_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
QVector<Item*> Inventory::getItemList()
{
  return items;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
ItemSorts Inventory::getItemState()
{
  return item_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Item* Inventory::getKeyItem(uint index)
{
  if ((uint)key_items.size() > index)
    return key_items.at(index);
  else
    return NULL;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Item* Inventory::getKeyItem(QString name)
{
  for (int i = 0; i < key_items.size(); i++)
   if (key_items.value(i)->getName() == name)
     return key_items.at(i);

  return NULL;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
QVector<Item*> Inventory::getKeyItemList()
{
  return key_items;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
ItemSorts Inventory::getKeyItemState()
{
  return key_item_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
double Inventory::getMaxCarryMass()
{
  return max_carry_mass;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
QString Inventory::getName()
{
  return name;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
Sprite* Inventory::getThumb()
{
  return thumbnail;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
double Inventory::getMass()
{
  calcMass();
  return carry_mass;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setBackdrop(Sprite* new_backdrop)
{
  backdrop = new_backdrop;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setBubbyLimit(uint new_bubby_limit)
{
  if (new_bubby_limit <= kMAX_BUBBIES && new_bubby_limit >= kMIN_BUBBIES)
    bubby_limit = new_bubby_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setBubbyState(ItemSorts new_state)
{
  bubby_state = new_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setEquipEachLimit(ushort new_equip_each_limit)
{
  if (new_equip_each_limit <= kMAX_EACH_EQUIPMENT &&
      new_equip_each_limit >= kMIN_EACH_EQUIPMENT)
    equip_each_limit = new_equip_each_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setEquipLimit(uint new_equip_limit)
{
  if (new_equip_limit <= kMAX_EQUIPMENT &&  new_equip_limit >= kMIN_EQUIPMENT)
    equip_limit = new_equip_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setEquipState(ItemSorts new_state)
{
  equipment_state = new_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setFlag(InventoryState flags, bool set_value)
{
  (set_value) ? (invflag_set |= flags) : (invflag_set ^= flags);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setItemEachLimit(ushort new_item_each_limit)
{
  if (new_item_each_limit <= kMAX_EACH_ITEM
      && new_item_each_limit >= kMIN_EACH_ITEM)
    item_each_limit = new_item_each_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setItemLimit(uint new_item_limit)
{
  if (item_limit <= kMAX_ITEMS && item_limit >= kMIN_ITEMS)
    item_limit = new_item_limit;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setItemState(ItemSorts new_state)
{
  item_state = new_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setKeyItemState(ItemSorts new_state)
{
  key_item_state = new_state;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setLimits(uint bub_lim, ushort eq_each_lim, uint eq_lim,
                          ushort it_each_lim, uint it_lim, double mass)
{
  setBubbyLimit(bub_lim);
  setEquipEachLimit(eq_each_lim);
  setEquipLimit(eq_lim);
  setItemEachLimit(it_each_lim);
  setItemLimit(it_lim);
  setMaxCarryMass(mass);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setMaxCarryMass(double new_max_carry_mass)
{
  if (new_max_carry_mass <= kMAX_CARRY_MASS
      && new_max_carry_mass >= kMIN_CARRY_MASS)
  max_carry_mass = new_max_carry_mass;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Inventory::setThumb(Sprite* new_thumbnail)
{
  thumbnail = new_thumbnail;
}
