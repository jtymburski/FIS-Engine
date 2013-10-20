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
 * Description: Inventory class constructor: instantiates an inventory object
 *              given a name (describing the current "size" or "type" of the
 *              inventory, a thumbnail Sprite to display a picture of it. The
 *              limits of the inventory must be set afterwards.
 *
 * Inputs: QString name - the name describing the current size or type of inv.
 *         Sprite* thumb - the thumbnail detailing an image of the inventory.
 */
Inventory::Inventory(QString name, Sprite *thumb, QWidget *parent)
    : QWidget(parent),
      bubby_state(EnumDb::NONE),
      equipment_state(EnumDb::NONE),
      item_state(EnumDb::NONE),
      key_item_state(EnumDb::NONE)
{
  setName(name);
  setThumb(thumb);
  bubbies.resize(0);
  equipments.resize(0);
  items.resize(0);
  key_items.resize(0);
  calcMass();

  setLimits(kMIN_BUBBIES, kMIN_EACH_EQUIPMENT, kMIN_EQUIPMENT, kMIN_EACH_ITEM,
            kMIN_ITEMS, kMIN_CARRY_MASS);

  setDescription("????");

  /* Pointer setup */
  setBackdrop(0);

  /* Flag setup */
  setFlag(Inventory::PLAYERSTORAGE, true);
}

/*
 * Description: Annihilates an inventory object
 */
Inventory::~Inventory() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Calculates the current mass stored within the inventory. This
 *              function cannot be called publicly--but will be called
 *              internally by the inventory when the mass of the inventory
 *              needs to be verified or has changed.
 *
 * Inputs: none
 * Output: none
 */
void Inventory::calcMass()
{
  /* Initially resets the class variable to zero */
  carry_mass = 0;
  setFlag(Inventory::OVERWEIGHT, false);

  /* Then, recounts the mass contained in each vector of items. Note that key
     items are not counted as they are assumed to have no mass. */
  for (int i = 0; i < bubbies.size(); i++)
      carry_mass += getBubby(i)->getMass();

  for (int i = 0; i < equipments.size(); i++)
      carry_mass += getEquip(i)->getMass();

  for (int i = 0; i < items.size(); i++)
      carry_mass += getItem(i)->getMass();

  if (carry_mass > max_carry_mass)
      setFlag(Inventory::OVERWEIGHT, false);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: This function attempts to add a Bubby to the vector of Bubbies
 *              in the inventory
 *
 * Inputs: Bubby* new_bubby - pointer to the bubby object to be added
 *         bool bypass - true if containsItem check to be bypassed
 * Output: bool - true if the Bubby was added successfully, false otherwise
 */
bool Inventory::addBubby(Bubby* new_bubby, bool bypass)
{

  if (bubbies.size() >= (int)bubby_limit)
  {
    emit full((QString)"Maximum Bubbies Reached");
    return false;
  }

  /* Assert the Bubbies ID is not already contained */
  if (bypass || !containsItem(new_bubby->getId()))
  {
    bubbies.push_back(new_bubby);

    /* Re-check mass, may emit carry mass exceeded limit */
    calcMass();
    return true;
  }

  return false;
}

/*
 * Description: This function attempts to add an equipment to the vector of
 *              equipments in the inventory.
 *
 * Inputs: Bubby* new_equipment - pointer to the Equipment object to be added
 *         bool bypass - true if containsItem check to be bypassed
 * Output: bool - true if the Equipment was added successfully, false otherwise
 */
bool Inventory::addEquipment(Equipment* new_equipment, bool bypass)
{
  if (equipments.size() >= (int)equip_limit)
  {
    emit full((QString)"Maximum Equipments Reached");
    return false;
  }

  if (getEquipCount(new_equipment->getName()) >= equip_each_limit)
  {
    emit full((QString)"Maximum Equipment of Same Type Reached");
    return false;
  }

  /* Assert the Equipmetns ID is not already contained */
  if (bypass || containsItem(new_equipment->getId()))
  {
    equipments.push_back(new_equipment);

    /* Re-chec mass, may emit carry mass exceeded limit */
    calcMass();
    return true;
  }

  return false;
}

/*
 * Description: This function attempts to add an item to the vector of items
 *              contained in the inventory.
 *
 * Inputs: Item* - pointer to the item object to be added.
 *         bool bypass - true if containsItem check to be bypassed
 * Output: bool - true if the Item was added successfully, false otherwise
 */
bool Inventory::addItem(Item* new_item, bool bypass)
{
  if (items.size() >= (int)item_limit)
  {
    emit full((QString)"Maximum Items Reached");
    return false;
  }

  if (getItemCount(new_item->getName()) >= item_each_limit)
  {
    emit full((QString)"Maximum Item of Same Type Reached");
    return false;
  }

  if (bypass || containsItem(new_item->getId()))
  {
    items.push_back(new_item);

    /* Re-check mass, may emit carry mass exceeded limit */
    calcMass();
    return true;
  }

  return false;
}

/*
 * Description: This function attempts to add a key item to the vector of
 *              key items.
 *
 * Inputs: Item* - the new item to be added to the vector of key items
 *         bool bypass - true if the containsItem check is to be bypassed
 * Output: none
 */
bool Inventory::addKeyItem(Item* new_item, bool bypass)
{
  if (bypass || !containsItem(new_item->getId()))
  {
    key_items.push_back(new_item);
    return true;
  }

  return false;
}

/*
 * Description: Verifies that a given ID is not already contained within the
 *              inventory.
 *
 * Inputs: int id - an ID of an item to be checked
 * Output: bool - true if the item is already contained in the inv, else false
 */
bool Inventory::containsItem(int id)
{
  bool contains_item = false;

  for (int i = 0; i < items.size() && !contains_item; i++)
    if (items.at(i)->getId() == id)
      contains_item = true;

  for (int i = 0; i < bubbies.size() && !contains_item; i++)
    if (bubbies.at(i)->getId() == id)
      contains_item = true;

  for (int i = 0; i < equipments.size() && !contains_item; i++)
    if (equipments.at(i)->getId() == id)
      contains_item = true;

  for (int i = 0; i < key_items.size() && !contains_item; i++)
    if (key_items.at(i)->getId() == id)
      contains_item = true;

  if (contains_item)
      emit full((QString)"ERROR: DUPLICATE ITEM ID");
  return contains_item;
}


/*
 * Description: Attempts to remove a Bubby at a given index from the vector of
 *              Bubbies in the inventory.
 *
 * Inputs: uint index -- the index of the Bubby to be remvoed
 * Output: Bubby* - pointer to the removed Bubby object, if it exists.
 */
Bubby* Inventory::removeBubby(uint index)
{
  if (index > 0 && (int)index < bubbies.size())
  {
    Bubby* removed_bubby = bubbies.value(index);
    bubbies[index] = 0;
    bubbies.remove(index);
    carry_mass -= removed_bubby->getMass();
    return removed_bubby;
  }

  return 0;
}

/*
 * Description: Attempts to remove an Equipment at a given index from the vector
 *              of Equipments in the inventory.
 *
 * Inputs: uint index -- the index of the Equipment to be removed
 * Output: Equipment* - pointer to the removed Equipment object, if it exists.
 */
Equipment* Inventory::removeEquipment(uint index)
{
  if (index > 0 && (int)index < equipments.size())
  {
    Equipment* removed_equipment = equipments.value(index);
    equipments[index] = 0;
    equipments.remove(index);
    carry_mass -= removed_equipment->getMass();
    return removed_equipment;
  }

  return 0;
}

/*
 * Description: Attempts to remove an Equipment of a given name from the vector
 *              of Equipments in the inventory.
 *
 * Inputs: QString name - the name of the equipment to be removed
 * Output: Equipment* - pointer to the removed Equipment object, if it exists.
 */
Equipment* Inventory::removeEquipment(QString name)
{
  for (int i = 0; i < equipments.size(); i++)
    if (equipments.at(i)->getName() == name)
      return removeEquipment(i);
  return 0;
}

/*
 * Description: Attempts to remove an Item at a given index from the vector of
 *              Items in the inventory.
 *
 * Inputs: uint index - the index of the Item to be removed
 * Output: Item* - pointer to the removed Item object, if it exists.
 */
Item* Inventory::removeItem(uint index)
{
  if (index > 0 && (int)index < items.size())
  {
    Item* removed_item = items.value(index);
    items[index] = 0;
    items.remove(index);
    carry_mass -= removed_item->getMass();
    return removed_item;
  }

  return 0;
}

/*
 * Description: Attempts to remove an Item of a given name from the vector of
 *              Items in the invetory.
 *
 * Inputs: QString - name of the item to be removed
 * Output: Item* - pointer to the removed item, it it exist.
 */
Item* Inventory::removeItem(QString name)
{
  for (int i = 0; i < items.size(); i++)
    if (items.at(i)->getName() == name)
        return removeItem(i);
  return 0;
}

/*
 * Description: Attempts to remove a Key Item given an index from the vector of
 *              key items in the inventory.
 *
 * Inputs: uint index - the index of the key item to be removed
 * Output: Item* - pointer to the removed key item, if it exists.
 */
Item* Inventory::removeKeyItem(uint index)
{
  if (index > 0 && (int)index < key_items.size())
  {
    Item* removed_item = key_items.value(index);
    key_items[index] = 0;
    key_items.remove(index);
    return removed_item;
  }

  return 0;
}

/*
 * Description: Attempts to remove a Key Item of a given name from the vector of
 *              key items in the inventory.
 *
 * Inputs: QString name - the name of the key item to be removed
 * Output: Item* - pointer to the removed key item, if it exists.
 */
Item* Inventory::removeKeyItem(QString name)
{
  for (int i = 0; i < key_items.size(); i++)
    if (key_items.at(i)->getName() == name)
      return removeKeyItem(i);
  return 0;
}

/*
 * Description: Prints out all the information describing the inventory by
 *              sub-calling all the other print functions.
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
 * Description: Prints out the basic information of the inventory, its name
 *              and limits etc.
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
 * Description: Prints out a list of the Bubies contained within the Inv.
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
 * Description: Prints out a list of the Equipment contained within the Inv.
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
 * Description: Prints out the state of the flags within the Inv.
 *
 * Inputs: none
 * Output: none
 */
void Inventory::printFlags()
{
  qDebug() << "PlayerStorage: " << getFlag(Inventory::PLAYERSTORAGE);
  qDebug() << "ShipStorage: " << getFlag(Inventory::SHIPSTORAGE);
  qDebug() << "EnemyStorage: " << getFlag(Inventory::ENEMYSTORAGE);
  qDebug() << "CanBeUpgraded: " << getFlag(Inventory::CANBEUPGRADED);
  qDebug() << "Overweight: " << getFlag(Inventory::OVERWEIGHT);
  qDebug() << "Enabled: " << getFlag(Inventory::ENABLED);
}

/*
 * Description: Prints out a list of the items contained within the inv.
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
 * Description: Prints out a list of the key items contained within the inv.
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
 * Description: This function will sort one of the inventory containers
 *              described by the input enumerated object_type first by
 *              the enumerated primary value then by the enumerated secondary
 *              value using std::stable_sort.
 *
 * Inputs: ItemSorts sort_prim - the first method by which to sort
 *         ItemSorts sort_secd - the secondary method by which to sort
 *         SortObjects object_type - the type of object (container) to be sorted
 * Output: bool - true if a sort actually took place
 */
bool Inventory::sort(EnumDb::ItemSorts sort_prim,
                     EnumDb::SortObjects object_type)
{
  /* The sorting is done if a bad ItemSorts enumerator was given */
  if (sort_prim == EnumDb::NONE)
    return false;

  /* Check for certain containers not able to sort certain ways */
  switch (object_type)
  {
    case (EnumDb::BUBBY_VECTOR) :
      /* Bubbies cannot be sorted by value */
      if (sort_prim == EnumDb::VALUE)
        return false;
      break;

    case (EnumDb::EQUIPMENT_VECTOR) :
    case (EnumDb::ITEM_VECTOR) :
    case (EnumDb::KEY_ITEM_VECTOR) :
      /* Equipments cannot be sorted by flavour or level */
      if (sort_prim == EnumDb::FLAVOUR || sort_prim == EnumDb::LEVEL)
        return false;
      break;

    default:
      return false;
  }

  sorter(sort_prim, object_type);
  return false;
}

/*
 * Description: This private sorter function takes a type of an object and will
 *              sort it by the desired item sorts typ using std::stable_sort
 *
 *  Note [1]: This function assumes the object_type can be sorted by
 *            the desired sort state.
 *  Note [2]: All items can be sorted by: ID, NAME, MASS
 *            Bubbies can also be sorted by: FLAVOUR, LEVEL
 *            General (non-key) items can also be sorted by: VALUE, VALUEPERMASS
 *
 * Inputs: ItemSorts sort_by - the desired enumerated sort type
 *         SortObjects object_type - enumeration of container to be sorted
 */
bool Inventory::sorter(EnumDb::ItemSorts sort_by,
                       EnumDb::SortObjects object_type)
{
  bool sort_completed = false;

  if (sort_by == EnumDb::ID)
  {
    if (object_type == EnumDb::BUBBY_VECTOR)
    {
      std::sort(bubbies.begin(), bubbies.end(), ItemPtrSortByID());
      sort_completed = true;
    }
    else if (object_type == EnumDb::EQUIPMENT_VECTOR)
    {
      std::sort(equipments.begin(), equipments.end(), ItemPtrSortByID());
      sort_completed = true;
    }
    else if (object_type == EnumDb::ITEM_VECTOR)
    {
      std::sort(items.begin(), items.end(), ItemPtrSortByID());
      sort_completed = true;
    }
    else if (object_type == EnumDb::KEY_ITEM_VECTOR)
    {
      std::sort(key_items.begin(), key_items.end(), ItemPtrSortByID());
      sort_completed = true;
    }
  }
  else if (sort_by == EnumDb::NAME)
  {
    if (object_type == EnumDb::BUBBY_VECTOR)
    {
      std::sort(bubbies.begin(), bubbies.end(), ItemPtrSortByName());
      sort_completed = true;
    }
    else if (object_type == EnumDb::EQUIPMENT_VECTOR)
    {
      std::sort(equipments.begin(), equipments.end(), ItemPtrSortByName());
      sort_completed = true;
    }
    else if (object_type == EnumDb::ITEM_VECTOR)
    {
      std::sort(items.begin(), items.end(), ItemPtrSortByName());
      sort_completed = true;
    }
    else if (object_type == EnumDb::KEY_ITEM_VECTOR)
    {
      std::sort(key_items.begin(), key_items.end(), ItemPtrSortByName());
      sort_completed = true;
    }
  }
  else if (sort_by == EnumDb::MASS)
  {
    if (object_type == EnumDb::BUBBY_VECTOR)
    {
      std::sort(bubbies.begin(), bubbies.end(), ItemPtrSortByMass());
      sort_completed = true;
    }
    else if (object_type == EnumDb::EQUIPMENT_VECTOR)
    {
      std::sort(equipments.begin(), equipments.end(), ItemPtrSortByMass());
      sort_completed = true;
    }
    else if (object_type == EnumDb::ITEM_VECTOR)
    {
      std::sort(items.begin(), items.end(), ItemPtrSortByMass());
      sort_completed = true;
    }
    else if (object_type == EnumDb::KEY_ITEM_VECTOR)
    {
      std::sort(key_items.begin(), key_items.end(), ItemPtrSortByMass());
      sort_completed = true;
    }
  }
  else if (sort_by == EnumDb::FLAVOUR && object_type == EnumDb::BUBBY_VECTOR)
  {
    std::sort(bubbies.begin(), bubbies.end(), BubbyPtrSortByFlavour());
    sort_completed = true;
  }
  else if (sort_by == EnumDb::LEVEL && object_type == EnumDb::BUBBY_VECTOR)
  {
    std::sort(bubbies.begin(), bubbies.end(), BubbyPtrSortByLevel());
    sort_completed = true;
  }
  else if (sort_by == EnumDb::VALUE)
  {
    if (object_type == EnumDb::EQUIPMENT_VECTOR)
    {
      std::sort(equipments.begin(), equipments.end(), ItemPtrSortByValue());
      sort_completed = true;
    }
    if (object_type == EnumDb::ITEM_VECTOR)
    {
      std::sort(items.begin(), items.end(), ItemPtrSortByValue());
      sort_completed = true;
    }
  }
  else if (sort_by == EnumDb::VALUEPERMASS)
  {
    if (object_type == EnumDb::EQUIPMENT_VECTOR)
    {
      std::sort(equipments.begin(), equipments.end(), ItemPtrSortByVPerM());
      sort_completed = true;
    }
    if (object_type == EnumDb::ITEM_VECTOR)
    {
      std::sort(items.begin(), items.end(), ItemPtrSortByVPerM());
      sort_completed = true;
    }
  }

  if (sort_completed)
  {
    if (object_type == EnumDb::BUBBY_VECTOR)
      bubby_state = sort_by;
    if (object_type == EnumDb::EQUIPMENT_VECTOR)
      equipment_state = sort_by;
    if  (object_type == EnumDb::ITEM_VECTOR)
      item_state = sort_by;
    if (object_type == EnumDb::KEY_ITEM_VECTOR)
      key_item_state = sort_by;
  }

  return sort_completed;
}

/*
 * Description: Returns the backdrop sprite of the Inventory.
 *
 * Inputs: none
 * Output: Sprite* - pointer to the backdrop of the inventory.
 */
Sprite* Inventory::getBackdrop()
{
  return backdrop;
}

/*
 * Description: Returns a pointer to a Bubby at a given index, if one exists.
 *
 * Inputs: none
 * Output: Bubby* - pointer to the Bubby at a given index
 */
Bubby* Inventory::getBubby(uint index)
{
  if (bubbies.size() > (int)index)
    return bubbies.at(index);
  else
    return 0;
}

/*
 * Description: Returns the maximum limit of Bubbies allowed in the inventory.
 *
 * Inputs: none
 * Output: uint - the maximum limit of Bubbies allowed in the inventory.
 */
uint Inventory::getBubbyLimit()
{
  return bubby_limit;
}

/*
 * Description: Returns the vector containing the Bubby pointers
 *
 * Inputs: none
 * Output: QVector<Bubby*> - the vector containing the Bubby pointers.
 */
QVector<Bubby*> Inventory::getBubbyList()
{
  return bubbies;
}

/*
 * Description: Returns the currently known sorted state of the Bubbies.
 *
 * Inputs: none
 * Output: ItemSorts - enumerated state describing the sorting of Bubbies
 */
EnumDb::ItemSorts Inventory::getBubbyState()
{
  return bubby_state;
}

/*
 * Description: Returns the description of the inventory.
 *
 * Inputs: none
 * Output: QString - the description of the inventory.
 */
QString Inventory::getDescription()
{
  return description;
}

/*
 * Description: Returns a pointer to the equipment at a given index.
 *
 * Inputs: uint - index of the equipment to be returned
 * Output: Equipment* - pointer to the equipment contained
 */
Equipment* Inventory::getEquip(uint index)
{
  if (equipments.size() > (int)index)
      return equipments.at(index);
  else
      return 0;
}

/*
 * Description: Returns the # of equipments of a given name
 *
 * Inputs: QString - name of an equipment to be checked
 * Output: uint - the count of the items contained within the inventory
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
 * Description: Returns the equip each limit set value.
 *
 * Inputs: none
 * Output: uint - equip each limit
 */
uint Inventory::getEquipEachLimit()
{
  return equip_each_limit;
}

/*
 * Description: Returns the maximum number of equipment allowed
 *
 * Inputs: none
 * Output: uint - maximum equip limit
 */
uint Inventory::getEquipLimit()
{
  return equip_limit;
}

/*
 * Description: Returns the enumerated known sorted state of equipment
 *
 * Inputs: none
 * Output: ItemSorts - enumerated sorted state of equipment
 */
EnumDb::ItemSorts Inventory::getEquipState()
{
 return equipment_state;
}

/*
 * Description: Returns a list of all the equipment in the inventory
 *
 * Inputs: none
 * Output: QVector<Equipment*> - vector of pointers to the contained equipment
 */
QVector<Equipment*> Inventory::getEquipList()
{
  return equipments;
}

/*
 * Description: Evaluates a given InventoryState flag
 *
 * Inputs: InventoryState flag - the value ofa flag set to be evaluated
 * Output: bool - the value of a given InventoryState flag
 */
bool Inventory::getFlag(InventoryState flags)
{
  return (invflag_set.testFlag(flags));
}

/*
 * Description: Returns a pointer to an item object at a given index.
 *
 * Inputs: uint - the index of the item to be returned
 * Output: Item* - pointer to the item object
 */
Item* Inventory::getItem(uint index)
{
  if (items.size() > (int)index)
    return items.at(index);
  else
    return 0;
}

/*
 * Description: Returns the count of items of a given name in the inventory
 *
 * Inputs: QString - name of the item to be looked for.
 * Output: uint - the number of the item found.
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
 * Description: Returns the get each limit for items.
 *
 * Inputs: none
 * Output: uint - the item each limit
 */
uint Inventory::getItemEachLimit()
{
  return item_each_limit;
}

/*
 * Description: Returns the item limit.
 *
 * Inputs: none
 * Output: uint - the item limit
 */
uint Inventory::getItemLimit()
{
 return item_limit;
}

/*
 * Description: Returns the list of items in the inventory.
 *
 * Inputs: none
 * Output: QVector<Item*> - the vector of items of the inventory
 */
QVector<Item*> Inventory::getItemList()
{
  return items;
}

/*
 * Description: Returns the enumerated state of the items.
 *
 * Inputs: none
 * Output: ItemSorts - the enumerated state of the items.
 */
EnumDb::ItemSorts Inventory::getItemState()
{
  return item_state;
}

/*
 * Description: Returns the pointer to the key item at a given index.
 *
 * Inputs: uint - index of key items to be checked.
 * Output: Item* - pointer to the item at the given index.
 */
Item* Inventory::getKeyItem(uint index)
{
  if ((uint)key_items.size() > index)
    return key_items.at(index);
  else
    return 0;
}

/*
 * Description: Returns a pointer to a key item of a given name.
 *
 * Inputs: QString - name of the item to be looked for.
 * Output: Item* - pointer to the key item (if found).
 */
Item* Inventory::getKeyItem(QString name)
{
  for (int i = 0; i < key_items.size(); i++)
   if (key_items.value(i)->getName() == name)
     return key_items.at(i);

  return 0;
}

/*
 * Description: Returns the list of key items.
 *
 * Inputs: none
 * Output: QVector<Item*> - the vector of key items of the inventory.
 */
QVector<Item*> Inventory::getKeyItemList()
{
  return key_items;
}

/*
 * Description: Returns the enumerated state of the key items vector.
 *
 * Inputs: none
 * Output: ItemSorts - the enumerated state of the key items vector.
 */
EnumDb::ItemSorts Inventory::getKeyItemState()
{
  return key_item_state;
}

/*
 * Description: Returns the max carry mass set value.
 *
 * Inputs: none
 * Output: double - the maximum carry mass of the inventory.
 */
double Inventory::getMaxCarryMass()
{
  return max_carry_mass;
}

/*
 * Description: Returns the QString of the name of the inventory
 *
 * Inputs: none
 * Output: QString - the name of the inventory.
 */
QString Inventory::getName()
{
  return name;
}

/*
 * Description: Returns the poitner to the sprite of the inventory.
 *
 * Inputs: none
 * Output: Sprite* - pointer to the thumbnail
 */
Sprite* Inventory::getThumb()
{
  return thumbnail;
}

/*
 * Description: Returns the mass of the Inventory.
 *
 * Inputs: none
 * Output: double - the mass of the Inventory.
 */
double Inventory::getMass()
{
  calcMass();
  return carry_mass;
}

/*
 * Description: Assigns a new sprite to the backdrop of the inventory.
 *
 * Inputs: Sprite* - a new back drop to be assigned
 * Output: none
 */
void Inventory::setBackdrop(Sprite* new_backdrop)
{
  backdrop = new_backdrop;
}

/*
 * Description: Assigns a new bubby limit to the inventory
 *
 * Inputs: uint - the new limit to be set (within kMIN_BUBBIES)
 * Output: none
 */
void Inventory::setBubbyLimit(uint new_bubby_limit)
{
  if (new_bubby_limit <= kMAX_BUBBIES && new_bubby_limit >= kMIN_BUBBIES)
    bubby_limit = new_bubby_limit;
}

/*
 * Description: Assigns a new enumerated sorted state to the Bubbies.
 *
 * Inputs: ItemSorts - enumerated state of the Bubbies vector.
 * Output: none
 */
void Inventory::setBubbyState(EnumDb::ItemSorts new_state)
{
  bubby_state = new_state;
}

/*
 * Description: Assigns a new description to the inventory.
 *
 * Inputs: QString - the new description of the inventory.
 * Output: none
 */
void Inventory::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Assigns a new equip each limit to the inventory.
 *
 * Inputs: ushort - the new equip each limit of the inventory, within constants.
 * Output: none
 */
void Inventory::setEquipEachLimit(ushort new_equip_each_limit)
{
  if (new_equip_each_limit <= kMAX_EACH_EQUIPMENT &&
      new_equip_each_limit >= kMIN_EACH_EQUIPMENT)
    equip_each_limit = new_equip_each_limit;
}

/*
 * Description: Assigns a new equip limit to the inventory.
 *
 * Inputs: uint - the new equip limit of the inventory, within constants.
 * Output: none
 */
void Inventory::setEquipLimit(uint new_equip_limit)
{
  if (new_equip_limit <= kMAX_EQUIPMENT &&  new_equip_limit >= kMIN_EQUIPMENT)
    equip_limit = new_equip_limit;
}

/*
 * Description: Assigsn a new enumerated equip state of the inventory.
 *
 * Inputs: ItemSorts - enumerated equip state of the Inventory.
 * Output: none
 */
void Inventory::setEquipState(EnumDb::ItemSorts new_state)
{
  equipment_state = new_state;
}

/*
 * Description: Assigns a given InventoryState flag a value.
 *
 * Inputs: InventoryState flags - the flag to be assigned.
 *         set_value - the value to set the flag to.
 * Output: none
 */
void Inventory::setFlag(InventoryState flags, bool set_value)
{
  (set_value) ? (invflag_set |= flags) : (invflag_set &= !flags);
}

/*
 * Description: Assigns a new item each limit to the inventory.
 *
 * Inputs: ushort - the new item each limit.
 * Output: none
 */
void Inventory::setItemEachLimit(ushort new_item_each_limit)
{
  if (new_item_each_limit <= kMAX_EACH_ITEM
      && new_item_each_limit >= kMIN_EACH_ITEM)
    item_each_limit = new_item_each_limit;
}

/*
 * Description: Assigns a new item limit to the inventory.
 *
 * Inputs: uint - the new item limit
 * Output: none
 */
void Inventory::setItemLimit(uint new_item_limit)
{
  if (item_limit <= kMAX_ITEMS && item_limit >= kMIN_ITEMS)
    item_limit = new_item_limit;
}

/*
 * Description: Assign an enumerated item state to the inventory.
 *
 * Inputs: ItemSorts - the enumerated state of the item vector.
 * Output: none
 */
void Inventory::setItemState(EnumDb::ItemSorts new_state)
{
  item_state = new_state;
}

/*
 * Description: Assigns an enumerated key item state to the inventory.
 *
 * Inputs: ItemSorts - the enumerated state of the key item vector.
 * Output: none
 */
void Inventory::setKeyItemState(EnumDb::ItemSorts new_state)
{
  key_item_state = new_state;
}

/*
 * Description: Assigns new limits to each of the limits by calling their
 *              respective functions.
 *
 * Inputs: uint bub_lim - the new value of the bubby limit.
 *         ushort eq_each_lim - the new value of equip each limit.
 *         uint eq_lim - the new value of equip limit.
 *         ushort it_each_lim - the new value of item each limit.
 *         uint it_lim - the new value of item limit.
 *         double mass - the new value of max carry mass.
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
 * Description: Assigsn a new value to max carry mass.
 *
 * Inputs: double new_max_carry_mass - the new maximum carry mass of the inv.
 * Output: none
 */
void Inventory::setMaxCarryMass(double new_max_carry_mass)
{
  if (new_max_carry_mass <= kMAX_CARRY_MASS
      && new_max_carry_mass >= kMIN_CARRY_MASS)
  max_carry_mass = new_max_carry_mass;
}

/*
 * Description: Assigns a new name to the inventory.
 *
 * Inputs: QString - the new name to be assigned.
 * Output: none
 */
void Inventory::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Assigns a new thumbnail to the inventory.
 *
 * Inputs: Sprite* - the new thumbnail of the inventory.
 * Output: none
 */
void Inventory::setThumb(Sprite* new_thumbnail)
{
  thumbnail = new_thumbnail;
}
