/*******************************************************************************
* Class Name: x [Implementation]
* Date Created: 
* Inheritance: 
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Inventory.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

uint32_t Inventory::id = 0;

const double Inventory::kMIN_CARRY_MASS = 100.00;
const double Inventory::kMAX_CARRY_MASS = 5000.00;
const uint32_t Inventory::kMIN_ITEM = 40;
const uint32_t Inventory::kMAX_ITEM = 25000;
const uint8_t Inventory::kMIN_EACH_ITEM = 10;
const uint8_t Inventory::kMAX_EACH_ITEM = 99;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Inventory::Inventory(const uint32_t game_id, const std::string name, 
	                   Frame* thumb)
  : curr_mass(0)
  , bubby_limit(0)
  , equip_limit(0)
  , item_limit(0)
  , item_each_limit(0)
  , mass_limit(0)
  , game_id(game_id)
  , my_id(++id)
  , name(name)
  , description("")
  , backdrop(nullptr)
  , thumbnail(thumb)
{
  setLimits(kMIN_ITEM, kMIN_ITEM, kMIN_ITEM, kITEM_EACH_LIMIT, kMIN_CARRY_MASS);
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Calculates the total mass contained within the Invetory */
double Inventory::calcMass()
{
  auto temp_mass{0.0};

  for (auto bubby : bubbies)
    temp_mass += bubby->getMass();

  for (auto equipment : equipments)
    temp_mass += equipment->getMass();

  for (auto item : items)
    temp_mass += item->getMass();

  return temp_mass;
}

bool Inventory::sortItems(const ObjectSorts sort_type, std::vector<Item*> item_vec,
	                        const bool ascending)
{
	/* Sort types 
  ID,
  NAME,
  FLAVOUR,
  LEVEL,
  VALUE,
  MASS,
  VALUEPERMASS */
	
  if (sort_type == ObjectSorts::ID)
  {
    std::sort(begin(item_vec), end(item_vec),
              [&](const Item* a, const Item* b) -> bool
              {
                if (ascending)
                  return a->getID() < b->getID();

                return a->getID() > b->getID();
              });
  }
 
  //TODO: Finish sorts [12-18-13]
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Attempts to add a Bubby */
bool Invetory::addBubby(Bubby* new_bubby, bool bypass)
{
  auto can_add = bubbies.size() < bubby_limit;
  can_add &= (new_bubby != nullptr);
  
  if (can_add && !bypass)
    can_add &= (new_bubby->getMass() + curr_mass) <= mass_limit;

  if (can_add)
  {
    bubbies.push_back(new_bubby);
    curr_mass += new_bubby->getMass();
  }

  return can_add;
}

/* Attempts to add an equipment */
bool Inventory::addEquipment(Equipment* new_equipment, const bool bypass)
{
  auto can_add = equipments.size() < equipment_limit;
  can_add &= (new_equipment != nullptr);

  if (can_add && !bypass)
    can_add &= (new_equipment->getMass() + curr_mass) <= mass_limit;

  if (can_add)
  {
    equipments.push_back(new_equipment);
    curr_mass += new_equipment->getMass();
  }

  return can_add;
}

/* Adds an item to the Inventory */
bool Inventory::addItem(Item* new_item, const bool bypass)
{
  auto can_add = items.size() < item_limit;
  can_add &= (new_item != nullptr);

  if (can_add && (!bypass || new_item->getFlag(ItemFlags::KEY_ITEM))
  {
    can_add &= (new_item->getMass() + curr_mass) < mass_limit;
    can_add &= getCount(new_item->getGameID()) <= item_each_limit;
  }

  if (can_add)
  {
    items.push_back(new_item);
    curr_mass += new_item->getMass();
  }
}

/* Prints out the state of the inventory */
void Inventory::print(bool simple)
{
  if (simple)
  {
  	std::cout << "ID: " << id << " M: " << curr_mass << " N: " << name << "\n";
    std::cout << "Items: " << items.size() << "/" << item_limit << "\n";
    std::cout << "Bubbies: " << bubbies.size() << "/" << bubby_limit << "\n";
    std::cout << "Equipment: " << equipments.size() << "/" << equipment_limit
              << "\n";
  }

  std::cout << "Bubbies:\n";

  for (auto bubby : bubbies)
    if (bubby != nullptr)
      std::cout << bubby->getName()

}

/* Removes a Bubby from the Inventory at a given index */
Bubby* Inventory::removeBubby(uint32_t index)
{
  if (index < bubbies.size())
  {
    bubbies.erase(begin(bubbies) + index);

    return true;
  }

  return false;
}

/* Removes an equipment at a given index */
Equipment* Inventory::removeEquipment(uint32_t index)
{
  if (index < equipments.size())
  {
    equipments.erase(begin(equipments) + index);

    return true;
  }

  return false;
}
 
/* Removes an item at a given index */
Item* Inventory::removeItem(uint32_t index)
{
  if (index < items.size())
  {
    items.erase(begin(items) + index);

    return true;
  }
  return false;
}

/* Sorts an object in the inventory a certain way */
bool Inventory::sort(const ObjectSorts sort_by, SortObjects object_to_sort)
{
  auto sort_object = &items;

  switch (object_to_sort)
  {
    case(SortObjects::BUBBIES):
      sort_object = bubbies;
      break;
    case(SortObjects::EQUIPMENTS):
      sort_object = equipments;
      break;
    case(SortObjects::ITEMS):
      sort_object = items;
      break;
    default:
      break;
  }

  if (sort_by != ObjectSorts::UNSORTED)
    return sortItems(sort_by, object_to_sort);
}

/* Returns the backdrop of the inventory */
Frame* getBackdrop()
{
  return backdrop;
}

/* Returns a vector of all items useable in battle */
std::vector<Item*> getBattleItems()
{
  std::vector<Item*> battle_items;

  for (auto item : items)
    if (item != nullptr)
      if (item->getOccasion() == ActionOccasion::BATTLE);
        battle_items.push_back(item);

  return battle_items;
}

/* Returns the currently set bubby limit */
uint32_t getBubbyLimit()
{
  return bubby_limit;
}

/* Returns a vector of all Bubbies */
std::vector<Bubby*> Inventory::getBubbies()
{
  return bubbies;
}

/* Returns the currently set equipment limit */
uint32_t Inventory::getEquipmentLimit()
{
  return equipment_limit;
}

/* Returns a vector of all equipment */
std::vector<Equipment*> Inventory::getEquipments()
{
  return equipments;
}

/* Returns the currently set item limit */
uint32_t Inventory::getItemLimit()
{
  return item_limit;
}

/* Returns the vector of all standard items */
std::vector<Item*> Inventory::getItems()
{
  return items;
}
 
/* Returns the count of a given Item game id */
uint8_t Inventory::getItemCount(uint32_t game_id)
{
  uint8_t count = 0;

  for (auto item : items)
    if (item != nullptr)
      if (item->getGameID() == game_id)
        count++;

  return count;
}

/* Returns the currently set item each limit */
uint32_t Inventory::getItemEachLimit()
{
  return item_each_limit;
}

/* Returns the vector of all key items */
std::vector<Item*> getKeyItems()
{
  std::vector<Item*> key_items;

  for (auto item : items)
    if (item != nullptr)
      if (item->getFlag(ItemFlags::KEY_ITEM))
        key_items.push_back(item);

  return key_items;
}

/* Returns the mass limit */
double Inventory::getMassLimit();
{
  return mass_limit;
}

/* Returns the thumbnail image of the Inventory */
Frame* Inventory::getThumbnail()
{
  return thumbnail;
}

/* Returns the description of the Inventory */
std::string Inventory::getDescription()
{
  return description;
}

/* Returns the name of the Inventory */
std::string Inventory::getName()
{
  return name;
}

bool Inventory::getFlag(const InvState test_flag)
{
  return static_cast<bool>((test_flag & flags) == flags);
}

bool Inventory::setDescription(const std::string new_description)
{
  if (new_description.size() < StringDB::kMAX_BRIEF_DESC)
  {
    description = new_description;

    return true;
  }

  return false;
}

void Inventory::setFlag(const InvState flag, const bool set_value)
{
  (set_value) ? (flags |= flag) : (flags ^= ~flag);
}

bool Inventory::setImages(Frame* const new_backdrop, Frame* const new_thumbnail)
{
  backdrop  = new_backdrop;
  thumbnail = new_thumbnail;

  return (new_backdrop != nullptr && new_thumbnail != nullptr)
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
 void Inventory::setLimits(const uint32_t bubby_lim, const uint32_t equip_lim,
 	                         const uint32_t item_lim, const uint32_t item_ea,
 	                         const double mass_lim)
{
  bubby_limit = Helpers::setWithinRange(bubby_lim, kMIN_ITEM, kMAX_ITEM);
  equip_limit = Helpers::setWithinRange(equip_lim, kMIN_ITEM, kMAX_ITEM);
  item_limit = Helpers::setWithinRange(item_lim, kMIN_ITEM, kMAX_ITEM);
  item_ea = Helpers::setWithinRange(item_ea, kMIN_EACH_ITEM, kMAX_EACH_ITEM);
  mass_limit = Helpers::setWithinRange(mass_lim, kMIN_MASS, kMAX_MASS);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/