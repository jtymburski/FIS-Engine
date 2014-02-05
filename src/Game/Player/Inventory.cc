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

uint32_t Inventory::id       = 0;
uint32_t Inventory::money_id = 0;

const double   Inventory::kMIN_MASS      =     200.00;
const double   Inventory::kMAX_MASS      =  100000.00;
const uint32_t Inventory::kMIN_ITEM      =         20;
const uint32_t Inventory::kMAX_ITEM      =      25000;
const uint16_t  Inventory::kMIN_EACH_ITEM =        10;
const uint16_t  Inventory::kMAX_EACH_ITEM =       105;

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
  : flags{static_cast<InvState>(0)}
  , curr_mass{0}
  , bubby_limit{0}
  , equip_limit{0}
  , item_limit{0}
  , item_each_limit{0}
  , mass_limit{0}
  , game_id{game_id}
  , my_id{++id}
  , name{name}
  , description{""}
  , backdrop{nullptr}
  , thumbnail{thumb}
{
  setFlag(InvState::UPGRADEABLE, true);
  setFlag(InvState::ENABLED, true);
  setLimits(kMIN_ITEM, kMIN_ITEM, kMIN_ITEM, kMIN_EACH_ITEM, kMIN_MASS);
}

Inventory::~Inventory()
{
  clear(!getFlag(InvState::SHOP_STORAGE));
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Calculates the total mass contained within the Invetory */
void Inventory::calcMass()
{
  double temp_mass{0.0};

  for (auto bubby : bubbies)
    if (bubby != nullptr)
      temp_mass += bubby->getMass();

  for (auto zero_bubby : zero_bubbies)
    if (zero_bubby.first != nullptr)
      temp_mass += (zero_bubby.first->getMass() * zero_bubby.second);

  for (auto equipment : equipments)
    if (equipment != nullptr)
      temp_mass += equipment->getMass();

  for (auto item : items)
    if (item.first != nullptr)
      temp_mass += (item.first->getMass() * item.second);

  curr_mass = temp_mass;
}

bool Inventory::increaseBubbyCount(const uint32_t &game_id, 
                                   const uint16_t &amount)
{
  auto bubby_index = getBubbyZeroIndex(game_id);

  if (bubby_index == -1)
    return false;

  auto bubby_count = zero_bubbies.at(bubby_index).second;

  if (bubby_count == 0)
    return false;

  zero_bubbies[bubby_index].second += amount;

  return false;
}

bool Inventory::increaseItemCount(const uint32_t &game_id, 
                                  const uint16_t &amount)
{
  auto item_index = getItemIndex(game_id);

  if (item_index == -1)
    return false;

  auto item_count = items.at(item_index).second;

  if (item_count == 0 || item_count == item_each_limit)
    return false;

  items[item_index].second += amount;

  return true;
}

bool Inventory::decreaseBubbyCount(const uint32_t &game_id, 
                                   const uint16_t &amount)
{
  auto bubby_index = getBubbyZeroIndex(game_id);

  if (bubby_index == -1)
    return false;

  auto bubby_count = zero_bubbies.at(bubby_index).second;

  if (bubby_count == 0)
    return false;

  if (amount < zero_bubbies[bubby_index].second)
    zero_bubbies[bubby_index].second -= amount;
  else
    return false;

  return false;
}

bool Inventory::decreaseItemCount(const uint32_t &game_id, 
                                  const uint16_t &amount)
{
  auto item_index = getItemIndex(game_id);

  if (item_index == -1)
    return false;

  auto item_count = items.at(item_index).second;

  if (item_count == 0)
    return false;

  if (amount < items[item_index].second)
    items[item_index].second -= amount;
  else
    return false;

  return true;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Attempts to add a Bubby */
AddStatus Inventory::add(Bubby* new_bubby, const uint32_t &amount, 
                         bool bypass)
{
  auto status = AddStatus::FAIL;
  bypass |= getFlag(InvState::SHOP_STORAGE);

  if (amount > 0)
  {
    auto spaces = hasRoom(new_bubby, amount);

    if (new_bubby != nullptr && (spaces >= amount || bypass))
    {
      if (new_bubby->getTier() == 0)
      {
        if (getBubbyZeroCount(new_bubby->getGameID()) == 0)
        {
          zero_bubbies.push_back(std::make_pair(new_bubby, amount));
          calcMass();

          status = AddStatus::GOOD_KEEP;
        }
        else
        {
          increaseBubbyCount(new_bubby->getGameID(), amount);
          calcMass();

          status = AddStatus::GOOD_DELETE;
        }
      }
      else
      {
        if (getFlag(InvState::SHOP_STORAGE))
        {
          // /* Check if the bubby exists already */
          // int32_t index = getBubbyIndex(new_bubby->getGameID());
          // if(index >= 0)
          // {
            // bubbies[index]->
          // }
          // else
          // {
          
          // }
        }
        else
        {
          bubbies.push_back(new_bubby);

          for (uint32_t i = 1; i < amount; i++)
            bubbies.push_back(new Bubby(new_bubby->getType()));

          calcMass();

          status = AddStatus::GOOD_KEEP;
        }
      }
    }
  }

  /* Update the add status, if the shop flag is set since shops delete no
   * items and need to be managed from an outside source */
  if(status == AddStatus::GOOD_KEEP && getFlag(InvState::SHOP_STORAGE))
    status = AddStatus::GOOD_DELETE;
  
  return status;
}

/* Attempts to add an equipment */
AddStatus Inventory::add(Equipment* new_equipment, const uint32_t &amount, 
                         bool bypass)
{
  auto status = AddStatus::FAIL;

  if (amount > 0)
  {
    bypass |= getFlag(InvState::SHOP_STORAGE);

    auto spaces = hasRoom(new_equipment, amount);

    if (new_equipment != nullptr && (spaces >= amount || bypass))
    {
      equipments.push_back(new_equipment);

      for (uint32_t i = 1; i < amount; i++)
        equipments.push_back(new Equipment(new_equipment));

      calcMass();

      if (getFlag(InvState::SHOP_STORAGE))
        status = AddStatus::GOOD_DELETE;
      else
        status = AddStatus::GOOD_KEEP;
    }
  }
  
  /* Update the add status, if the shop flag is set since shops delete no
   * items and need to be managed from an outside source */
  if (status == AddStatus::GOOD_KEEP && getFlag(InvState::SHOP_STORAGE))
    status = AddStatus::GOOD_DELETE;
  
  return status;
}

/* Adds an item to the Inventory */
AddStatus Inventory::add(Item* new_item, const uint32_t &amount, 
                         bool bypass)
{
  if (amount == 0)
    return AddStatus::FAIL;

  bypass |= getFlag(InvState::SHOP_STORAGE);

  auto spaces = hasRoom(new_item, amount);

  if (amount > 0 && new_item != nullptr && (spaces >= amount || bypass))
  {
    if (new_item->isBaseItem())
    {
      for (uint32_t i = 0; i < amount; i++)
        items.push_back(std::make_pair(new_item, 1));

      calcMass();

      return AddStatus::GOOD_KEEP;
    }
    else if (getItemCount(new_item->getGameID()) == 0)
    {
      items.push_back(std::make_pair(new_item, amount));
      calcMass();

      return AddStatus::GOOD_KEEP;
    }
    else
    {
      increaseItemCount(new_item->getGameID(), amount);
      calcMass();

      return AddStatus::GOOD_DELETE;
    }
  }

  return AddStatus::FAIL;
}

/* Clears the memory of the inventory and the vectors of data */
void Inventory::clear(const bool &free)
{
  if (free)
  {
    /* One-each of Tier 0 bubbies are required to be deleted upon removal */
    for (auto zero_bubby : zero_bubbies)
    {
      if (zero_bubby.first != nullptr)
        delete zero_bubby.first;

      zero_bubby.first = nullptr;
    }

    /* Each non-tier-zero Bubby is required to be deleted */
    for (auto bubby: bubbies)
    {
      if (bubby != nullptr)
        delete bubby;

      bubby = nullptr;
    }

    /* Each equipment is required to be deleted */
    for (auto equip : equipments)
    {
      if (equip != nullptr)
        delete equip;

      equip = nullptr;
    }

    /* Each index of the items is required to be deleted */
    for (auto item : items)
    {

      if (item.first != nullptr)
        delete item.first; 

      item.first = nullptr;
    }
  }

  /* Empty the vectors */
  zero_bubbies.clear();
  bubbies.clear();
  equipments.clear();
  items.clear();
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

/* Calcs and returns the number of spaces in the Inv. for a given Equip */
uint32_t Inventory::hasRoom(Equipment* const equip, uint32_t n)
{
  if (equip == nullptr)
    return 0;

  if (getFlag(InvState::SHOP_STORAGE))
    return kMAX_ITEM;

  n = std::min(equip_limit - getEquipCount(equip->getGameID()), n);

  auto lim = std::floor((mass_limit - getMass()) / equip->getMass());

  return std::min(static_cast<uint32_t>(lim), n);
}

/* Calcs and returns the number of spaces in the Inv. for a given Bubby */
uint32_t Inventory::hasRoom(Bubby* const bubby, uint32_t n)
{
  if (bubby == nullptr)
    return 0;

  if (getFlag(InvState::SHOP_STORAGE))
    return kMAX_ITEM;

  n = std::min(bubby_limit - getTotalBubbyCount(), n);

  if (bubby->getTier() == 0)
    n = std::min(item_each_limit - getBubbyZeroCount(bubby->getGameID()), n);
  else
    n = std::min(item_each_limit - getBubbyCount(bubby->getGameID()), n);

  auto lim = std::floor((mass_limit - getMass()) / bubby->getMass());

  return std::min(static_cast<uint32_t>(lim), n);
}

/* Calcs and returns the number of spaces in the Inv. for a given Item */
uint32_t Inventory::hasRoom(Item* const item, uint32_t n)
{
  if (item == nullptr)
    return 0;

  if (getFlag(InvState::SHOP_STORAGE) || item->getFlag(ItemFlags::KEY_ITEM))
    return kMAX_ITEM;

  n = std::min(item_limit - getItemTotalCount(), n);
  n = std::min(item_each_limit - getItemCount(item->getGameID()), n);
  auto lim = std::floor((mass_limit - getMass()) / item->getMass());

  return std::min(static_cast<uint32_t>(lim), n);
}

/* Prints out the state of the inventory */
void Inventory::print(bool simple)
{
  std::cout << "\n---- Inventory: " << name << " ----\n";
  std::cout << "ID: " << id << " M: " << getMass() << " / " << mass_limit;
  std::cout << "\n# Unique Items: " << items.size() << "\n";
  std::cout << "# Unique Zero Bubbies: " << zero_bubbies.size() << "\n";
  std::cout << "# Unique TX Bubbies: " << bubbies.size()  << "\n";
  std::cout << "Bubbies: " << getTotalBubbyCount() << "/" << bubby_limit<< "\n";
  std::cout << "Equips:  " << equipments.size()    << "/" << equip_limit<< "\n";
  std::cout << "Items:   " << getItemTotalCount()  << "/" << item_limit << "\n";

  if (!simple)
  {
    std::cout << "\nTier 0 Bubbies:\n";

    for (auto bubby_pair : zero_bubbies)
    {
      if (bubby_pair.first != nullptr)
      {
        std::cout << bubby_pair.first->getName() << " " 
                  << static_cast<int>(bubby_pair.second) << "\n";
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
    {
      if (item.first != nullptr)
      {
        std::cout << item.first->getName() << " " 
                  << static_cast<int>(item.second) << "\n";
      }
    }
  }
  
  std::cout << "--- / Inventory ---\n\n";
}

/* Removes a zero bubby from the inventory */
bool Inventory::removeZeroBubbyIndex(const uint32_t &index, 
                                     const uint16_t &amount)
{
  if (index < zero_bubbies.size())
  {
    if (zero_bubbies.at(index).first != nullptr)
    {
      auto count = zero_bubbies[index].second;

      if (count > 1 && count > amount)
        decreaseBubbyCount(zero_bubbies.at(index).first->getGameID(), amount);
      else if (count == amount)
      {
        delete zero_bubbies[index].first;
        zero_bubbies[index].first = nullptr;
        zero_bubbies.erase(begin(zero_bubbies) + index);
      }
      else
        return false;

      calcMass();
    }

    return true;
  }

  return false;
}

bool Inventory::removeZeroBubbyID(const uint32_t &game_id,
                                  const uint16_t &amount)
{
  auto bubby_index = getBubbyZeroIndex(game_id);

  if (bubby_index != -1)
    return removeZeroBubbyIndex(bubby_index, amount);

  return false;
}

/* Removes a Bubby from the Inventory at a given index */
bool Inventory::removeBubbyIndex(const uint32_t &index)
{
  if (index < bubbies.size())
  {
    if (bubbies[index] != nullptr)
      delete bubbies[index];

    bubbies[index] = nullptr;
    bubbies.erase(bubbies.begin() + index);
    calcMass();

    return true;
  }

  return false;
}

/* Removes a TX bubby by a given game ID value */
bool Inventory::removeBubbyID(const uint32_t &game_id)
{
  auto bubby_index = getBubbyIndex(game_id);

  if (bubby_index != -1)
    return removeBubbyIndex(bubby_index);

  return false;
}

/* Removes an equipment at a given index */
bool Inventory::removeEquipIndex(const uint32_t &index)
{
  if (index < equipments.size())
  {
    delete equipments[index];
    equipments[index] = nullptr;
    equipments.erase(begin(equipments) + index);
    calcMass();

    return true;
  }

  return false;
}

/* Removes an equipment by a game ID */
bool Inventory::removeEquipID(const uint32_t &game_id)
{
  auto equip_index = getEquipIndex(game_id);

  if (equip_index != -1)
    return removeEquipIndex(equip_index);

  return false;
}
 
/* Removes an item at a given index */
bool Inventory::removeItemIndex(const uint32_t &index,
                                const uint16_t &amount)
{
  if (index < items.size())
  {
    if (items.at(index).first != nullptr)
    {
      auto count = items[index].second;

      if (count > 1 && count > amount)
        decreaseItemCount(items.at(index).first->getGameID(), amount);
      else if (count == amount)
      {
        delete items[index].first;
        items[index].first = nullptr;
        items.erase(begin(items) + index);
      }
      else
        return false;

      calcMass();

      return true;
    }
  }

  return false;
}

bool Inventory::removeItemID(const uint32_t &game_id,
                             const uint16_t &amount)
{
  auto item_index = getItemIndex(game_id);

  if (item_index != -1)
    return removeItemIndex(game_id, amount);

  return false;
}

/* Sorts an object in the inventory a certain way */
bool Inventory::sort(const SortType sort_type, SortObjects object, 
	                   const bool ascending)
{
  auto sort_status = false;

  switch (object)
  {
    case (SortObjects::ZERO_BUBBIES):
    {
      if (sort_type == SortType::COUNT)
      {
        std::sort(begin(zero_bubbies), end(zero_bubbies), 
                  Helpers::CompPairByCount());
        sort_status = true;
      }
      else if (sort_type == SortType::ID)
      {
        std::sort(begin(zero_bubbies), end(zero_bubbies), 
                  Helpers::CompPairByID());
        sort_status = true;
      }
      else if (sort_type == SortType::MASS)
      {
        std::sort(begin(zero_bubbies), end(zero_bubbies), 
                  Helpers::CompPairByMass());
        sort_status = true;
      }
      else if (sort_type == SortType::NAME)
      {
        std::sort(begin(zero_bubbies), end(zero_bubbies), 
                  Helpers::CompPairByName());
        sort_status = true;
      }
      else if (sort_type == SortType::VALUE)
      {
        std::sort(begin(zero_bubbies), end(zero_bubbies), 
                  Helpers::CompPairByValue());
        sort_status = true;
      }
      else if (sort_type == SortType::VALUEPERMASS)
      {
        std::sort(begin(zero_bubbies), end(zero_bubbies), 
                  Helpers::CompPairByValueMass());
        sort_status = true;
      }

      break;
    }
    case (SortObjects::BUBBIES):
    {
      if (sort_type == SortType::ID)
      {
        std::sort(begin(bubbies), end(bubbies), Helpers::CompByID());
        sort_status = true;
      }
      else if (sort_type == SortType::MASS)
      {
        std::sort(begin(bubbies), end(bubbies), Helpers::CompByMass());
        sort_status = true;
      }
      else if (sort_type == SortType::NAME)
      {
        std::sort(begin(bubbies), end(bubbies), Helpers::CompByName());
        sort_status = true;
      }
      else if (sort_type == SortType::VALUE)
      {
        std::sort(begin(bubbies), end(bubbies), Helpers::CompByValue());
        sort_status = true;
      }
      else if (sort_type == SortType::VALUEPERMASS)
      {
        std::sort(begin(bubbies), end(bubbies), Helpers::CompByValueMass());
        sort_status = true;
      }

      break;
    }
    case (SortObjects::EQUIPMENTS):
    {
      if (sort_type == SortType::ID)
      {
        std::sort(begin(equipments), end(equipments), Helpers::CompByID());
        sort_status = true;
      }
      if (sort_type == SortType::MASS)
      {
        std::sort(begin(equipments), end(equipments), Helpers::CompByMass());
        sort_status = true;
      }
      if (sort_type == SortType::NAME)
      {
        std::sort(begin(equipments), end(equipments), Helpers::CompByName());
        sort_status = true;
      }
      if (sort_type == SortType::VALUE)
      {
        std::sort(begin(equipments), end(equipments), Helpers::CompByID());
        sort_status = true;
      }
      if (sort_type == SortType::VALUEPERMASS)
      {
        std::sort(begin(equipments), end(equipments), 
                  Helpers::CompByValueMass());
        sort_status = true;
      }

      break;
    }
    case (SortObjects::ITEMS):
    {
      if (sort_type == SortType::COUNT)
      {
        std::sort(begin(items), end(items), Helpers::CompPairByCount());
        sort_status = true;
      }
      else if (sort_type == SortType::ID)
      {
        std::sort(begin(items), end(items), Helpers::CompPairByID());
        sort_status = true;
      }
      else if (sort_type == SortType::MASS)
      {
        std::sort(begin(items), end(items), Helpers::CompPairByMass());
        sort_status = true;
      }
      else if (sort_type == SortType::NAME)
      {
        std::sort(begin(items), end(items), Helpers::CompPairByName());
        sort_status = true;
      }
      else if (sort_type == SortType::VALUE)
      {
        std::sort(begin(items), end(items), Helpers::CompPairByValue());
        sort_status = true;
      }
      else if (sort_type == SortType::VALUEPERMASS)
      {
        std::sort(begin(items), end(items), Helpers::CompPairByValueMass());
        sort_status = true;
      }

      break;
    }
    default:
      break;
  }

  if (sort_status && !ascending)
  {
    switch (object)
    {
      case (SortObjects::BUBBIES):
        std::reverse(begin(bubbies), end(bubbies));
        break;
      case (SortObjects::ZERO_BUBBIES):
        std::reverse(begin(zero_bubbies), end(zero_bubbies));
        break;
      case (SortObjects::EQUIPMENTS):
        std::reverse(begin(equipments), end(equipments));
        break;
      case (SortObjects::ITEMS):
        std::reverse(begin(items), end(items));
        break;
      default:
        break;
    }
  }

  return sort_status;
}

/* Returns the backdrop of the inventory */
Frame* Inventory::getBackdrop()
{
  return backdrop;
}

/* Returns a vector of all items useable in battle */
std::vector<std::pair<Item*, uint16_t>> Inventory::getBattleItems()
{
  std::vector<std::pair<Item*, uint16_t>> battle_items;

  for (auto battle_item : items)
  {
    if (battle_item.first != nullptr)
    {
      if (battle_item.first->getOccasion() == ActionOccasion::BATTLE ||
          battle_item.first->getOccasion() == ActionOccasion::ALWAYS)
        battle_items.push_back(battle_item);
    }
  }

  return battle_items;
}

/* Returns the count of Bubbies of a given game_id - only checks T0 bubbies */
uint32_t Inventory::getBubbyCount(const uint32_t &game_id)
{
  auto count = 0;

  for (auto it = begin(bubbies); it != end(bubbies); ++it)
    if ((*it) != nullptr && (*it)->getGameID() == static_cast<int32_t>(game_id))
      count++;

  return count;
}

/* Returns the count of T0 Bubbies of a given game_id */
uint32_t Inventory::getBubbyZeroCount(const uint32_t &game_id)
{
  for (auto it = begin(zero_bubbies); it != end(zero_bubbies); ++it)
    if ((*it).first != nullptr)
      if ((*it).first->getGameID() == static_cast<int32_t>(game_id))
        return (*it).second;

  return 0;
}

/* Returns the index on the vector of TX bubbies of a given game ID */
int32_t Inventory::getBubbyIndex(const uint32_t &game_id)
{
  for(uint32_t i = 0; i < bubbies.size(); i++)
  {
    if (bubbies[i] != nullptr)
      if (bubbies[i]->getGameID() == static_cast<int32_t>(game_id))
        return i;
  }
  
  return -1;
}

/* Returns the index of a given equipment game id */
int32_t Inventory::getEquipIndex(const uint32_t &game_id)
{
  auto index = -1;

  for (auto it = begin(equipments); it != end(equipments); ++it)
  {
    index++;

    if ((*it) != nullptr)
      if ((*it)->getGameID() == static_cast<int32_t>(game_id))
        return index;
  }

  return index;
}

/* Returns the index on the vector of a given Bubby zero ID */
int32_t Inventory::getBubbyZeroIndex(const uint32_t &game_id)
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
uint32_t Inventory::getCount(const uint32_t &game_id)
{
  auto count = getBubbyZeroCount(game_id);
  
  if (count == 0)
    count = getBubbyCount(game_id);
  if (count == 0)
    count = getEquipCount(game_id);
  if (count == 0)
    count = getItemCount(game_id);

  return count;
}

/* Counts the # of equipments of a given game ID */
int32_t Inventory::getEquipCount(const uint32_t &game_id)
{
  auto count = 0;

  for (auto it = begin(equipments); it != end(equipments); ++it)
    if ((*it) != nullptr && (*it)->getGameID() == static_cast<int32_t>(game_id))
      count++;

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
std::vector<std::pair<Item*, uint16_t>> Inventory::getItems()
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

uint32_t Inventory::getItemTotalCount(const bool &count_keys)
{
  uint32_t total = 0;

  for (auto item : items)
    if (item.first != nullptr)
      if (!item.first->getFlag(ItemFlags::KEY_ITEM) || count_keys)
        total += item.second;

  return total;
}

/* Returns the vector of all key items */
std::vector<std::pair<Item*, uint16_t>> Inventory::getKeyItems()
{
  std::vector<std::pair<Item*, uint16_t>> key_items;

  for (auto item : items)
    if (item.first != nullptr)
      if (item.first->getFlag(ItemFlags::KEY_ITEM))
        key_items.push_back(std::make_pair(item.first, item.second));

  return key_items;
}

/* Returns the vector of all zero bubbies */
std::vector<std::pair<Bubby*, uint16_t>> Inventory::getZeroBubbies()
{
  return zero_bubbies;
}

/* Returns the total mass stored in the inventory */
double Inventory::getMass()
{
  calcMass();

  return curr_mass;
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

bool Inventory::getFlag(const InvState &test_flag)
{
  return static_cast<bool>((test_flag & flags) == test_flag);
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

void Inventory::setLimits(const uint32_t bubby_lim, const uint32_t equip_lim,
 	                        const uint32_t item_lim, const uint16_t item_e,
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