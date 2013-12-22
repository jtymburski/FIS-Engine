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

const double Inventory::kMIN_MASS = 100.00;
const double Inventory::kMAX_MASS = 5000.00;
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
  setLimits(kMIN_ITEM, kMIN_ITEM, kMIN_ITEM, kMIN_EACH_ITEM, kMIN_MASS);
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Calculates the total mass contained within the Invetory */
double Inventory::calcMass()
{
  double temp_mass{0.0};

  for (auto bubby : bubbies)
    temp_mass += bubby->getMass();

  for (auto equipment : equipments)
    temp_mass += equipment->getMass();

  for (auto item : items)
    temp_mass += item->getMass();

  return temp_mass;
}

bool Inventory::sortBubbies(Bubby_It begin, Bubby_It stop, 
	                          const ObjectSorts &sort_type, const bool &asc)
{
  bool sorted = false;

  if (sort_type == ObjectSorts::ID)
  {
    std::sort(begin, stop,
              [&](Bubby* const a, Bubby* const b) -> bool
              {
                if (asc)
                  return a->getID() < b->getID();

                return b->getID() < a->getID();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::NAME)
  {
    std::sort(begin, stop,
              [&](Bubby* const a, Bubby* const b) -> bool
              {
                if (asc)
                  return a->getName() < b->getName();

                return b->getName() < a->getName();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::FLAVOUR)
  {
    std::sort(begin, stop,
              [&](Bubby* const a, Bubby* const b) -> bool
              {
                if (asc)
                  return a->getType() < b->getType();

                return b->getType() < a->getType();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::LEVEL)
  {
    std::sort(begin, stop,
              [&](Bubby* const a, Bubby* const b) -> bool
              {
                if (a->getLevel() == b->getLevel())
                {
                  if (asc)
                    return a->getExp() < b->getExp();

                  return b->getExp() < a->getExp();
                }
                if (asc)
                  return a->getLevel() < b->getLevel();

                return b->getLevel() < a->getLevel();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUE)
  {
    std::sort(begin, stop,
              [&](Bubby* const a, Bubby* const b) -> bool
              {
                if (asc)
                  return a->getValue() < b->getValue();

                return a->getValue() > b->getValue();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::MASS)
  {
    std::sort(begin, stop,
              [&](Bubby* const a, Bubby* const b) -> bool
              {
                if (asc)
                  return a->getMass() < b->getMass();

                return a->getMass() > b->getMass();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUEPERMASS)
  {
    std::sort(begin, stop,
              [&](Bubby* const a, Bubby* const b) -> bool
              {
                if (asc)
                {
                  return ((static_cast<double>(a->getValue()) /
                           static_cast<double>(a->getMass())) < 
                          (static_cast<double>(b->getValue()) /
                           static_cast<double>(b->getMass())));
                }
                
                return ((static_cast<double>(a->getValue()) /
                         static_cast<double>(a->getMass())) > 
                        (static_cast<double>(b->getValue())) /
                         static_cast<double>(b->getMass()));
              });

    sorted = true;
  }

  return sorted;
}

bool Inventory::sortEquipments(Equip_It begin, Equip_It stop, 
	                             const ObjectSorts &sort_type, const bool &asc)
{
  bool sorted = false;

  if (sort_type == ObjectSorts::ID)
  {
    std::sort(begin, stop,
              [&](Equipment* const a, Equipment* const b) -> bool
              {
                if (asc)
                  return a->getID() < b->getID();

                return b->getID() < a->getID();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::NAME)
  {
    std::sort(begin, stop,
              [&](Equipment* const a, Equipment* const b) -> bool
              {
                if (asc)
                  return a->getName() < b->getName();

                return b->getName() < a->getName();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUE)
  {
    std::sort(begin, stop,
              [&](Equipment* const a, Equipment* const b) -> bool
              {
                if (asc)
                  return a->getValue() < b->getValue();

                return a->getValue() > b->getValue();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::MASS)
  {
    std::sort(begin, stop,
              [&](Equipment* const a, Equipment* const b) -> bool
              {
                if (asc)
                  return a->getMass() < b->getMass();

                return a->getMass() > b->getMass();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUEPERMASS)
  {
    std::sort(begin, stop,
              [&](Equipment* const a, Equipment* const b) -> bool
              {
                if (asc)
                {
                  return ((static_cast<double>(a->getValue()) /
                           static_cast<double>(a->getMass())) < 
                          (static_cast<double>(b->getValue()) /
                           static_cast<double>(b->getMass())));
                }
                
                return ((static_cast<double>(a->getValue()) /
                         static_cast<double>(a->getMass())) > 
                        (static_cast<double>(b->getValue())) /
                         static_cast<double>(b->getMass()));
              });

    sorted = true;
  }

  return sorted;
}

bool Inventory::sortItems(Item_It begin, Item_It stop, 
	                        const ObjectSorts &sort_type, const bool &asc)
{
  bool sorted = false;

  if (sort_type == ObjectSorts::ID)
  {
    std::sort(begin, stop,
              [&](Item* const a, Item* const b) -> bool
              {
                if (asc)
                  return a->getID() < b->getID();

                return b->getID() < a->getID();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::NAME)
  {
    std::sort(begin, stop,
              [&](Item* const a, Item* const b) -> bool
              {
                if (asc)
                  return a->getName() < b->getName();

                return b->getName() < a->getName();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUE)
  {
    std::sort(begin, stop,
              [&](Item* const a, Item* const b) -> bool
              {
                if (asc)
                  return a->getValue() < b->getValue();

                return a->getValue() > b->getValue();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::MASS)
  {
    std::sort(begin, stop,
              [&](Item* const a, Item* const b) -> bool
              {
                if (asc)
                  return a->getMass() < b->getMass();

                return a->getMass() > b->getMass();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUEPERMASS)
  {
    std::sort(begin, stop,
              [&](Item* const a, Item* const b) -> bool
              {
                if (asc)
                {
                 return ((static_cast<double>(a->getValue()) /
                          static_cast<double>(a->getMass())) < 
                        (static_cast<double>(b->getValue()) /
                         static_cast<double>(b->getMass())));
                }
                
                return ((static_cast<double>(a->getValue()) /
                         static_cast<double>(a->getMass())) > 
                        (static_cast<double>(b->getValue())) /
                         static_cast<double>(b->getMass()));
              });

    sorted = true;
  }

  return sorted;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Attempts to add a Bubby */
bool Inventory::addBubby(Bubby* new_bubby, bool bypass)
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
  auto can_add = equipments.size() < equip_limit;
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

  if (can_add && (!bypass || new_item->getFlag(ItemFlags::KEY_ITEM)))
  {
    can_add &= (new_item->getMass() + curr_mass) < mass_limit;
    can_add &= getCount(new_item->getGameID()) <= item_each_limit;
  }

  if (can_add)
  {
    items.push_back(new_item);
    curr_mass += new_item->getMass();
  }

  return can_add;
}

/* Checks if an Item in the inventory matches a given unique ID */
bool Inventory::contains(const int &id_check)
{
  for (auto bubby : bubbies)
    if (bubby != nullptr)
      if (bubby->getID() == id_check)
        return true;

  for (auto equipment : equipments)
    if (equipment != nullptr)
      if (equipment->getID() == id_check)
        return true;

  for (auto item : items)
    if (item != nullptr)
      if (item->getID() == id_check)
        return true;

  return false;
}

/* Prints out the state of the inventory */
void Inventory::print(bool simple)
{
  if (simple)
  {
  	std::cout << "ID: " << id << " M: " << curr_mass << " N: " << name << "\n";
    std::cout << "Items: " << items.size() << "/" << item_limit << "\n";
    std::cout << "Bubbies: " << bubbies.size() << "/" << bubby_limit << "\n";
    std::cout << "Equipment: " << equipments.size() << "/" << equip_limit
              << "\n";
  }

  std::cout << "Bubbies:\n";

  for (auto bubby : bubbies)
    if (bubby != nullptr)
      std::cout << bubby->getName();
}

/* Removes a Bubby from the Inventory at a given index */
bool Inventory::removeBubby(uint32_t index)
{
  if (index < bubbies.size())
  {
    bubbies.erase(begin(bubbies) + index);

    return true;
  }

  return false;
}

/* Removes an equipment at a given index */
bool Inventory::removeEquipment(uint32_t index)
{
  if (index < equipments.size())
  {
    equipments.erase(begin(equipments) + index);

    return true;
  }

  return false;
}
 
/* Removes an item at a given index */
bool Inventory::removeItem(uint32_t index)
{
  if (index < items.size())
  {
    items.erase(begin(items) + index);

    return true;
  }


  return false;
}

/* Sorts an object in the inventory a certain way */
bool Inventory::sort(const ObjectSorts sort_type, SortObjects object_to_sort, 
	                   const bool ascending)
{
  switch (object_to_sort)
  {
    case(SortObjects::BUBBIES):
    {
      return sortBubbies(begin(bubbies), end(bubbies), sort_type, ascending);
      break;
    }
    case(SortObjects::EQUIPMENTS):
    {
      return sortEquipments(begin(equipments), end(equipments), sort_type, 
      	                    ascending);
      break;
    }
    case(SortObjects::ITEMS):
    {
      return sortItems(begin(items), end(items), sort_type, ascending);
      break;
    }
    case(SortObjects::KEY_ITEMS):
    {
      auto key_items = getKeyItems();
      return sortItems(begin(key_items), end(key_items), sort_type, ascending);
      break;
    }
    default:
      break;
  }

  return false;
}

/* Returns the backdrop of the inventory */
Frame* Inventory::getBackdrop()
{
  return backdrop;
}

/* Returns a vector of all items useable in battle */
std::vector<Item*> Inventory::getBattleItems()
{
  std::vector<Item*> battle_items;

  for (auto battle_item : items)
    if (battle_item != nullptr)
      if (battle_item->getOccasion() == ActionOccasion::BATTLE)
        battle_items.push_back(battle_item);

  return battle_items;
}

/* Returns the currently set bubby limit */
uint32_t Inventory::getBubbyLimit()
{
  return bubby_limit;
}

/* Returns a vector of all Bubbies */
std::vector<Bubby*> Inventory::getBubbies()
{
  return bubbies;
}

/* Counts # of items with a given Game ID in the inventory */
uint32_t Inventory::getCount(const int &game_id)
{
  uint32_t count = 0;

  for (auto bubby : bubbies)
    if (bubby != nullptr)
      if (bubby->getGameID() == game_id)
        count++;
  
  if (count == 0)
    for (auto equipment : equipments)
      if (equipment != nullptr)
        if (equipment->getGameID() == game_id)
          count++;

  if (count == 0)
    return getItemCount(game_id);

  return count;
}

/* Returns the currently set equipment limit */
uint32_t Inventory::getEquipmentLimit()
{
  return equip_limit;
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
uint8_t Inventory::getItemCount(const int &game_id)
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
std::vector<Item*> Inventory::getKeyItems()
{
  std::vector<Item*> key_items;

  for (auto item : items)
    if (item != nullptr)
      if (item->getFlag(ItemFlags::KEY_ITEM))
        key_items.push_back(item);

  return key_items;
}

/* Returns the mass limit */
double Inventory::getMassLimit()
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

  return (new_backdrop != nullptr && new_thumbnail != nullptr);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
 void Inventory::setLimits(const uint32_t bubby_lim, const uint32_t equip_lim,
 	                         const uint32_t item_lim, const uint8_t item_ea,
 	                         const double mass_lim)
{
  bubby_limit = Helpers::setWithinRange(bubby_lim, 0, 4);
  equip_limit = Helpers::setWithinRange(equip_lim, kMIN_ITEM, kMAX_ITEM);
  item_limit = Helpers::setWithinRange(item_lim, kMIN_ITEM, kMAX_ITEM);
 item_each_limit=Helpers::setWithinRange(item_ea,kMIN_EACH_ITEM,kMAX_EACH_ITEM);
  mass_limit = Helpers::setWithinRange(mass_lim, kMIN_MASS, kMAX_MASS);
}