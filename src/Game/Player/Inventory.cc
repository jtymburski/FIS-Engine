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
uint32_t Inventory::money_id = 0;

const double   Inventory::kMIN_MASS      =  300.00;
const double   Inventory::kMAX_MASS      = 5000.00;
const uint32_t Inventory::kMIN_ITEM      =      50;
const uint32_t Inventory::kMAX_ITEM      =   25000;
const uint8_t  Inventory::kMIN_EACH_ITEM =      10;
const uint8_t  Inventory::kMAX_EACH_ITEM =      99;

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
    temp_mass += item.first->getMass();

  return temp_mass;
}

/* Sorts the vector of Tier 0 bubbies (the ones that have counts) */
bool Inventory::sortZeroBubbies(Bubby0_It begin, Bubby0_It stop,
                             const ObjectSorts &sort_type, const bool &asc)
{
  auto sorted = false;

  if (sort_type == ObjectSorts::ID)
  {
    std::sort(begin, stop, 
              [&](const std::pair<Bubby*, uint8_t> &a, 
                  const std::pair<Bubby*, uint8_t> &b) -> bool
              {
                if (asc)
                  return a.first->getID() < b.first->getID();

                return b.first->getID() < a.first->getID();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::NAME)
  {
    std::sort(begin, stop,
              [&](const std::pair<Bubby*, uint8_t> &a,
                  const std::pair<Bubby*, uint8_t> &b) -> bool
              {
                if (asc)
                  return a.first->getID() < b.first->getID();

                return b.first->getID() < a.first->getID();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::FLAVOUR)
  {
    std::sort(begin, stop,
              [&](const std::pair<Bubby*, uint8_t> &a,
                  const std::pair<Bubby*, uint8_t> &b) -> bool
              {
                if (asc)
                  return a.first->getType()->getName() < 
                         b.first->getType()->getName();

                return b.first->getType()->getName() < 
                       b.first->getType()->getName();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUE)
  {
    std::sort(begin, stop,
              [&](const std::pair<Bubby*, uint8_t> &a,
                  const std::pair<Bubby*, uint8_t> &b) -> bool
              {
                if (asc)
                  return a.first->getValue() < b.first->getValue();

                return b.first->getValue() < a.first->getValue();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::MASS)
  {
    std::sort(begin, stop,
              [&](const std::pair<Bubby*, uint8_t> &a,
                  const std::pair<Bubby*, uint8_t> &b) -> bool
              {
                if (asc)
                  return a.first->getMass() < b.first->getMass();

                return b.first->getMass() < a.first->getMass();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUEPERMASS)
  {
    std::sort(begin, stop,
              [&](const std::pair<Bubby*, uint8_t> &a,
                  const std::pair<Bubby*, uint8_t> &b) -> bool
              {
                if (asc)
                {
                  return ((static_cast<double>(a.first->getValue()) /
                           static_cast<double>(a.first->getMass())) < 
                          (static_cast<double>(b.first->getValue()) /
                           static_cast<double>(b.first->getMass())));
                }
                
                return ((static_cast<double>(a.first->getValue()) /
                         static_cast<double>(a.first->getMass())) > 
                        (static_cast<double>(b.first->getValue())) /
                         static_cast<double>(b.first->getMass()));
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::COUNT)
  {
    std::sort(begin, stop,
              [&](const std::pair<Bubby*, uint8_t> &a,
                  const std::pair<Bubby*, uint8_t> &b) -> bool
              {
                if (asc)
                  return a.second < b.second;

                return b.second < a.second;
              });
              
    sorted = true;
  }

  return sorted;
}

bool Inventory::sortBubbies(Bubby_It begin, Bubby_It stop, 
	                          const ObjectSorts &sort_type, const bool &asc)
{
  auto sorted = false;

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
              [&](const std::pair<Item*, uint8_t> &a,
                  const std::pair<Item*, uint8_t> &b)
              {
                if (asc)
                  return a.first->getID() < b.first->getID();

                return b.first->getID() < a.first->getID();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::NAME)
  {
    std::sort(begin, stop,
              [&](const std::pair<Item*, uint8_t> &a,
                  const std::pair<Item*, uint8_t> &b)
              {
                if (asc)
                  return a.first->getName() < b.first->getName();

                return b.first->getName() < a.first->getName();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUE)
  {
    std::sort(begin, stop,
              [&](const std::pair<Item*, uint8_t> &a,
                  const std::pair<Item*, uint8_t> &b)
              {
                if (asc)
                  return a.first->getValue() < b.first->getValue();

                return b.first->getValue() < a.first->getValue();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::MASS)
  {
    std::sort(begin, stop,
              [&](const std::pair<Item*, uint8_t> &a,
                  const std::pair<Item*, uint8_t> &b)
              {
                if (asc)
                 return a.first->getMass() < b.first->getMass();

                return b.first->getMass() < a.first->getMass();
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::VALUEPERMASS)
  {
    std::sort(begin, stop,
              [&](const std::pair<Item*, uint8_t> &a,
                  const std::pair<Item*, uint8_t> &b)
              {
                if (asc)
                {
                  return ((static_cast<double>(a.first->getValue()) /
                           static_cast<double>(a.first->getMass())) < 
                          (static_cast<double>(b.first->getValue()) /
                           static_cast<double>(b.first->getMass())));
                }

                return ((static_cast<double>(a.first->getValue()) /
                         static_cast<double>(a.first->getMass())) > 
                        (static_cast<double>(b.first->getValue())) /
                         static_cast<double>(b.first->getMass()));
              });

    sorted = true;
  }
  else if (sort_type == ObjectSorts::COUNT)
  {
    std::sort(begin, stop,
              [&](const std::pair<Item*, uint8_t> &a,
                  const std::pair<Item*, uint8_t> &b) -> bool
              {
                if (asc)
                  return a.second < b.second;

                return b.second < a.second;
              });
              
    sorted = true; 
  }

  return sorted;
}

bool Inventory::increaseBubbyCount(const uint32_t &game_id)
{
  auto bubby_index = getBubbyIndex(game_id);

  if (bubby_index == -1)
    return false;

  auto bubby_count = zero_bubbies.at(bubby_index).second;

  if (bubby_count == 0)
    return false;

  zero_bubbies[bubby_index].second++;

  return false;
}

bool Inventory::increaseItemCount(const uint32_t &game_id)
{
  auto item_index = getItemIndex(game_id);

  if (item_index == -1)
    return false;

  auto item_count = items.at(item_index).second;

  if (item_count == 0 || item_count == item_each_limit)
    return false;

  items[item_index].second++;

  return true;
}

bool Inventory::decreaseBubbyCount(const uint32_t &game_id)
{
  auto bubby_index = getBubbyIndex(game_id);

  if (bubby_index == -1)
    return false;

  auto bubby_count = zero_bubbies.at(bubby_index).second;

  if (bubby_count == 0)
    return false;

  zero_bubbies[bubby_index].second--;

  return false;
}

bool Inventory::decreaseItemCount(const uint32_t &game_id)
{
  auto item_index = getItemIndex(game_id);

  if (item_index == -1)
    return false;

  auto item_count = items.at(item_index).second;

  if (item_count == 0)
    return false;

  items[item_index].second--;

  return true;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Attempts to add a Bubby */
bool Inventory::addBubby(Bubby* new_bubby, bool bypass)
{
  auto can_add = getTotalBubbyCount() < bubby_limit;
  
  auto count = 0;

  if (new_bubby != nullptr)
    count = getBubbyCount(new_bubby->getGameID());
  else 
    can_add = false;

  if (can_add && !bypass)
    can_add &= (new_bubby->getMass() + curr_mass) <= mass_limit;

  if (can_add)
  {
    if (new_bubby->getTier() == 0)
    {
      if (count == 0)
        zero_bubbies.push_back(std::make_pair(new_bubby, 1));
      else
        increaseBubbyCount(new_bubby->getGameID());
    }
    else
    {
      bubbies.push_back(new_bubby);  
    }

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
  auto can_add = getItemTotalCount() < item_limit;
  can_add &= (new_item != nullptr);

  auto count = 0;

  if (new_item != nullptr)
    count = getItemCount(new_item->getGameID());
  else
    can_add = false;

  if (can_add && (!bypass || new_item->getFlag(ItemFlags::KEY_ITEM)))
  {
    can_add &= (new_item->getMass() + curr_mass) <= mass_limit;
    can_add &= getCount(new_item->getGameID()) <= item_each_limit;
  }

  if (can_add)
  {
    if (count == 0 || new_item->isBaseItem())
      items.push_back(std::make_pair(new_item, 1));
    else
      increaseItemCount(new_item->getGameID());

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
    if (item.first != nullptr)
      if (item.first->getID() == id_check)
        return true;

  return false;
}

/* Prints out the state of the inventory */
void Inventory::print(bool simple)
{
  std::cout << "---- Inventory: " << name << " ----\n";

  std::cout << "ID: " << id << " M: " << curr_mass << " N: " << name << "\n";
  std::cout << "# Unique Items: " << items.size() << "\n";
  std::cout << "Total Items: " << getItemTotalCount() << "/" << item_limit << "\n";
  std::cout << "# Unique Zero Bubbies: " << zero_bubbies.size() << "\n";
  std::cout << "# Unique TX Bubbies: " << bubbies.size()  << "\n";
  std::cout << "Total Bubbies: " << getTotalBubbyCount() << "/" << bubby_limit
            << "\n";
  std::cout << "Equipment: " << equipments.size() << "/" << equip_limit << "\n";
  if (!simple)
  {
    std::cout << "Tier 0 Bubbies:\n";

    for (auto bubby_pair : zero_bubbies)
    {
      if (bubby_pair.first != nullptr)
      {
        std::cout << bubby_pair.first->getName() << " " << static_cast<int>(bubby_pair.second) 
        << "\n";
      }
    }

    std::cout << "\nBubbies:\n";

    for (auto bubby : bubbies)
      if (bubby != nullptr)
        std::cout << bubby->getName() << "\n";

    std::cout << "\nEquipments:\n";

    for (auto equipment : equipments)
      if (equipment != nullptr)
        std::cout << equipment->getName() << "\n";

    std::cout << "\nItems:\n";

    for (auto item : items)
      if (item.first != nullptr)
        std::cout << item.first->getName() << " " << static_cast<int>(item.second) << "\n";
  }
  
  std::cout << "--- / Inventory ---\n";
}

/* Removes a zero bubby from the inventory */
bool Inventory::removeZeroBubby(uint32_t index)
{
  if (index < zero_bubbies.size())
  {
    if (zero_bubbies.at(index).first != nullptr)
    {
      if (getBubbyCount(zero_bubbies.at(index).first->getGameID()))
        decreaseBubbyCount(zero_bubbies.at(index).first->getGameID());
      else
        zero_bubbies.erase(begin(zero_bubbies) + index);
    }
    return true;
  }

  return false;
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
    if (items.at(index).first != nullptr)
    {
      if (getItemCount(items.at(index).first->getGameID()) > 1)
        decreaseItemCount(items.at(index).first->getGameID());
      else
        items.erase(begin(items) + index);
    }

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
    case (SortObjects::ZERO_BUBBIES):
    {
      return sortZeroBubbies(begin(zero_bubbies), end(zero_bubbies), sort_type, 
             ascending);
      break;
    }
    case (SortObjects::BUBBIES):
    {
      return sortBubbies(begin(bubbies), end(bubbies), sort_type, ascending);
      break;
    }
    case (SortObjects::EQUIPMENTS):
    {
      return sortEquipments(begin(equipments), end(equipments), sort_type, 
      	                    ascending);
      break;
    }
    case (SortObjects::ITEMS):
    {
      return sortItems(begin(items), end(items), sort_type, ascending);
      break;
    }
    case (SortObjects::KEY_ITEMS):
    {
      //TODO: Fix key items [01-18-14]
      //auto key_items = getKeyItems();
      //return sortItems(begin(key_items), end(key_items), sort_type, ascending);
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
std::vector<std::pair<Item*, uint8_t>> Inventory::getBattleItems()
{
  std::vector<std::pair<Item*, uint8_t>> battle_items;

  for (auto battle_item : items)
    if (battle_item.first != nullptr)
      if (battle_item.first->getOccasion() == ActionOccasion::BATTLE)
        battle_items.push_back(battle_item);

  return battle_items;
}

/* Returns the count of Bubbies of a given game_id - only checks T0 bubbies */
uint32_t Inventory::getBubbyCount(const uint32_t &game_id)
{
  for (auto it = begin(zero_bubbies); it != end(zero_bubbies); ++it)
    if ((*it).first != nullptr)
      if ((*it).first->getGameID() == static_cast<int32_t>(game_id))
        return (*it).second;

  return 0;
}

/* Returns the index on the vector of a given Bubby zero ID */
int32_t Inventory::getBubbyIndex(const uint32_t &game_id)
{
  auto index = -1;

  for (auto it = begin(zero_bubbies); it != end(zero_bubbies); ++it)
  {
    index++;

    if ((*it).first != nullptr)
      if ((*it).first->getGameID() == static_cast<int32_t>(game_id))
        return index;
  }

  return index;
}

/* Returns the currently set bubby limit */
uint32_t Inventory::getBubbyLimit()
{
  return bubby_limit;
}

/* Returns the total count of T0 and TX bubbies */
uint32_t Inventory::getTotalBubbyCount()
{
  auto total = 0;

  /* First, add up values of each T0 Bubby */
  for (auto it = begin(zero_bubbies); it != end(zero_bubbies); ++it)
    if ((*it).first != nullptr)
      total += (*it).second;

  /* Then add the # of TX bubbies */
  total += bubbies.size();

  return total;

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

/* Returns the index of an Item ID in the vector */
int32_t Inventory::getItemIndex(const uint32_t &game_id)
{
  auto index = -1;

  for (auto it = begin(items); it != end(items); ++it)
  {
    index++;
    if ((*it).first != nullptr)
      if ((*it).first->getGameID() == static_cast<int32_t>(game_id))
        return index;
  }

  return index;
}

/* Returns the currently set item limit */
uint32_t Inventory::getItemLimit()
{
  return item_limit;
}

/* Returns the vector of all standard items */
std::vector<std::pair<Item*, uint8_t>> Inventory::getItems()
{
  return items;
}
 
/* Returns the count of a given Item game id */
uint32_t Inventory::getItemCount(const uint32_t &game_id)
{
  for (auto item : items)
    if (item.first != nullptr)
      if (item.first->getGameID() == static_cast<int32_t>(game_id))
        return item.second;

  return 0;
}

/* Returns the currently set item each limit */
uint32_t Inventory::getItemEachLimit()
{
  return item_each_limit;
}

uint32_t Inventory::getItemTotalCount()
{
  uint32_t total = 0;

  for (auto item : items)
    if (item.first != nullptr)
      total += item.second;

  return total;
}

/* Returns the vector of all key items */
std::vector<Item*> Inventory::getKeyItems()
{
  std::vector<Item*> key_items;

  for (auto item : items)
    if (item.first != nullptr)
      if (item.first->getFlag(ItemFlags::KEY_ITEM))
        key_items.push_back(item.first);

  return key_items;
}

/* Returns the total mass stored in the inventory */
double Inventory::getMass()
{
  return calcMass();
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
  if (new_description.size() < StringDb::kMAX_BRIEF_DESC)
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
 	                         const uint32_t item_lim, const uint8_t item_e,
 	                         const double mass_lim)
{
  bubby_limit     = Helpers::setInRange(bubby_lim, kMIN_ITEM, kMAX_ITEM);
  equip_limit     = Helpers::setInRange(equip_lim, kMIN_ITEM, kMAX_ITEM);
  item_limit      = Helpers::setInRange(item_lim, kMIN_ITEM, kMAX_ITEM);
  item_each_limit = Helpers::setInRange(item_e, kMIN_EACH_ITEM, kMAX_EACH_ITEM);
  mass_limit      = Helpers::setInRange(mass_lim, kMIN_MASS, kMAX_MASS);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

void Inventory::setMoneyID(const uint32_t &new_money_id)
{
  money_id = new_money_id;
}