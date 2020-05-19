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
 * CONSTANTS - See implementation for details
 *================================================+============================*/

uint32_t Inventory::id = 0;
uint32_t Inventory::money_id = 0;

const double Inventory::kMIN_MASS = 15.00;
const double Inventory::kMAX_MASS = 99999;
const uint32_t Inventory::kMIN_ITEM = 75;
const uint32_t Inventory::kMAX_ITEM = 25000;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs an Inventory object given an ID for it, a name
 *              and a pointer to a thumbnail.
 *
 * Note [1]: Inventories by default are created empty, upgradeable, with
 *           default limit values and not of non-shop storage type. To change
 *
 * Inputs:
 *           to a stop storage inventory the flag must be set after const.
 */
Inventory::Inventory(const uint32_t game_id, const std::string name,
                     Frame* thumb)
    : flags{static_cast<InvState>(0)},
      curr_mass{0},
      bubby_limit{0},
      equip_limit{0},
      item_limit{0},
      mass_limit{0},
      game_id{game_id},
      my_id{++id},
      name{name},
      description{""},
      backdrop{nullptr},
      thumbnail{thumb}
{
  setFlag(InvState::UPGRADEABLE, true);
  setFlag(InvState::ENABLED, true);
  setLimits(kMIN_ITEM, kMIN_ITEM, kMIN_ITEM, kMIN_MASS);
}

/*
 * Description: Annihilates an Inventory object
 *
 */
Inventory::~Inventory()
{
  clear(!getFlag(InvState::SHOP_STORAGE));
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Calculates the total mass of the Inventory by adding up the
 *              masses of bubbies, equipments, and items.
 *
 * Inputs: none
 * Output: none
 */
void Inventory::calcMass()
{
  double temp_mass{0.0};

  for(auto bubby : bubbies)
    if(bubby.first)
      temp_mass += (bubby.first->getMass() * bubby.second) / 1000.0;

  // for(auto equipment : equipments)
  //   if(equipment.first)
  //     temp_mass += (equipment.first->getMass() * equipment.second) / 1000.0;

  for(auto item : items)
    if(item.first)
      if(!item.first->getFlag(ItemFlags::KEY_ITEM))
        temp_mass += (item.first->getMass() * item.second) / 1000.0;

  curr_mass = temp_mass;
}

/*
 * Description: Increases the count of an item with a given game ID contained
 *              within the inventor by an amount. Will search through each
 *              bubbies, equipments then items and increase the first instance
 *              found by the amount.
 *
 * Note [1]: This function does not check if the inventory has room as this
 *           is a private function and hasRoom should have already been called.
 *
 * Inputs: game_id - the game ID of an item to increase the count for
 * Output: amount - the amount to increase the count by
 */
bool Inventory::increaseCount(const uint32_t& game_id, const uint16_t& amount)
{
  auto increased = false;
  auto index = getBubbyIndex(game_id);

  if(index != -1)
  {
    auto count = bubbies.at(index).second;

    if(count != 0)
    {
      bubbies[index].second += amount;
      increased = true;
    }
  }

  // if(!increased)
  // {
  //   index = getEquipIndex(game_id);

  //   if(index != -1)
  //   {
  //     auto count = equipments.at(index).second;

  //     if(count != 0)
  //     {
  //       equipments[index].second += amount;
  //       increased = true;
  //     }
  //   }
  // }

  if(!increased)
  {
    index = getItemIndex(game_id);

    if(index != -1)
    {
      auto count = items.at(index).second;

      if(count != 0)
      {
        items[index].second += amount;
        increased = true;
      }
    }
  }

  return increased;
}

/*
 * Description: Decreases the count of an item with a given game ID contained
 *              within the inventor by an amount. Will search through each
 *              bubbies, equipments then items and increase the first instance
 *              found by the amount.
 *
 * Inputs: game_id - the game ID of an item to decrease the count for
 * Output: amount - the amount to decrease the count by
 */
bool Inventory::decreaseCount(const uint32_t& game_id, const uint16_t& amount)
{
  auto decreased = false;
  auto index = getBubbyIndex(game_id);

  if(index != -1)
  {
    auto count = bubbies.at(index).second;

    if(count > 0)
    {
      bubbies[index].second -= amount;
      decreased = true;
    }
  }

  // if(!decreased)
  // {
  //   index = getEquipIndex(game_id);

  //   if(index != -1)
  //   {
  //     auto count = equipments.at(index).second;

  //     if(count > 0)
  //     {
  //       equipments[index].second -= amount;
  //       decreased = true;
  //     }
  //   }
  // }

  if(!decreased)
  {
    index = getItemIndex(game_id);

    if(index != -1)
    {
      auto count = items.at(index).second;

      if(count > 0)
      {
        items[index].second -= amount;
        decreased = true;
      }
    }
  }

  return decreased;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add an amount of bubby objects given a Bubby object
 *              pointer to the inventory, if the Inventory has room for them
 *              unless bypass is true.
 *
 * Note [1]: If the Inventory is a shop storage, the value of bypass is
 *           irrelevant.
 *
 * Inputs: Bubby* new_bubby - a Bubby object to be added into the inventory.
 *         amount - the amount of Bubby objects to be added
 *         bypass - true if limit checks are to be bypassed
 * Output: AddStatus - enumerated values of the status of addition
 */
AddStatus Inventory::add(Bubby* new_bubby, const uint32_t& amount, bool bypass)
{
  auto status = AddStatus::FAIL;
  bypass |= getFlag(InvState::SHOP_STORAGE);

  if(amount > 0)
  {
    auto spaces = hasRoom(new_bubby, amount);

    if(new_bubby != nullptr && (spaces >= amount || bypass))
    {
      if(new_bubby->getTier() == 0 || getFlag(InvState::SHOP_STORAGE))
      {
        if(getBubbyCount(new_bubby->getGameID(), new_bubby->getTier()) == 0)
        {
          bubbies.push_back(std::make_pair(new_bubby, amount));
          calcMass();

          status = AddStatus::GOOD_KEEP;
        }
        else
        {
          increaseCount(new_bubby->getGameID(), amount);
          calcMass();

          status = AddStatus::GOOD_DELETE;
        }
      }
      else if(getBubbyCount(new_bubby->getGameID(), new_bubby->getTier()) == 0)
      {
        bubbies.push_back(std::make_pair(new_bubby, amount));
        calcMass();

        status = AddStatus::GOOD_KEEP;
      }
      else
      {
        bubbies.push_back(std::make_pair(new_bubby, 1));

        for(uint32_t i = 1; i < amount; i++)
        {
          auto created_bubby = new Bubby(new_bubby->getType());
          bubbies.push_back(std::make_pair(created_bubby, amount));
        }

        calcMass();

        status = AddStatus::GOOD_KEEP;
      }
    }
  }

  /* Update the add status, if the shop flag is set since shops delete no
   * items and need to be managed from an outside source */
  if(status == AddStatus::GOOD_KEEP && getFlag(InvState::SHOP_STORAGE))
    status = AddStatus::GOOD_DELETE;

  return status;
}

/*
 * Description: Attempts to add an amount of item objects given an Item object
 *              pointer to the inventory, if the Inventory has room for them
 *              unless bypass is true.
 *
 * Note [1]: If the Inventory is a shop storage, the value of bypass is
 *           irrelevant.
 *
 * Inputs: Item* new_item - an Item object to be added into the inventory.
 *         amount - the amount of Bubby objects to be added
 *         bypass - true if limit checks are to be bypassed
 * Output: AddStatus - enumerated values of the status of addition
 */
AddStatus Inventory::add(Item* new_item, const uint32_t& amount, bool bypass)
{
  if(amount == 0)
  {
    return AddStatus::FAIL;
  }

  bypass |= getFlag(InvState::SHOP_STORAGE);
  auto spaces = hasRoom(new_item, amount);

  if(amount > 0 && new_item != nullptr && (spaces >= amount || bypass))
  {
    // TODO: Replace with stackable flag. Base ID should never be used within
    //       inventory; it will not work. Post beta T#355
    if(new_item->isBaseItem()) // TODO: Replace with stackable flag
    {
      for(uint32_t i = 0; i < amount; i++)
        items.push_back(std::make_pair(new_item, 1));

      calcMass();

      return AddStatus::GOOD_KEEP;
    }
    else if(getItemCount(new_item->getGameID()) == 0)
    {
      items.push_back(std::make_pair(new_item, amount));
      calcMass();

      return AddStatus::GOOD_KEEP;
    }
    else
    {
      increaseCount(new_item->getGameID(), amount);
      calcMass();

      return AddStatus::GOOD_DELETE;
    }
  }

  return AddStatus::FAIL;
}

/*
 * Description: Clears the vectors of Inventory objects and potentially
 *              frees the memory of the objects.
 *
 * Inputs: free - boolean whether to free the objects' memory
 * Output: none
 */
void Inventory::clear(const bool& free)
{
  if(free)
  {
    /* Bubby deletion */
    for(auto bubby : bubbies)
    {
      if(bubby.first != nullptr)
      {
        if(getFlag(InvState::SHOP_STORAGE))
          std::cerr << "[ERROR]: Deleting bubby in shop storage inventory\n";

        delete bubby.first;
      }

      bubby.first = nullptr;
    }

    // /* Each equipment is required to be deleted */
    // for(auto equip : equipments)
    // {
    //   if(equip.first != nullptr)
    //   {
    //     if(getFlag(InvState::SHOP_STORAGE))
    //       std::cerr << "[ERROR]: Deleting equip in shop storage inventory\n";

    //     delete equip.first;
    //   }

    //   equip.first = nullptr;
    // }

    /* Each index of the items is required to be deleted */
    for(auto item : items)
    {
      if(item.first != nullptr)
      {
        if(getFlag(InvState::SHOP_STORAGE))
          std::cerr << "[ERROR]: Deleting item in shop storage inventory\n";

        delete item.first;
      }

      item.first = nullptr;
    }
  }

  /* Empty the vectors */
  bubbies.clear();
  //equipments.clear();
  items.clear();
}

/*
 * Description: Checks if an Item in the inventory matches a given unique ID
 *
 * Inputs: id_check - checks if an object is contained within an inventory
 * Output: bool - true if an object matching the ID is contained within
 */
bool Inventory::contains(const int32_t& id_check)
{
  for(auto bubby : bubbies)
    if(bubby.first != nullptr)
      if(bubby.first->getID() == id_check)
        return true;

  // for(auto equipment : equipments)
  //   if(equipment.first != nullptr)
  //     if(equipment.first->getID() == id_check)
  //       return true;

  for(auto item : items)
    if(item.first != nullptr)
      if(item.first->getID() == id_check)
        return true;

  return false;
}

/*
 * Description: Checks if a Bubby in the inventory matches a given unique ID
 *
 * Inputs: b - pointer to a bubby object to be checked room for
 *         n - the maximum number of objects to be checked for
 * Output: int32_t - the # of the bubbies of given type the inventory has room
 */
uint32_t Inventory::hasRoom(Bubby* const b, uint32_t n)
{
  if(b)
  {
    if(!getFlag(InvState::SHOP_STORAGE))
    {
      n = std::min(bubby_limit - getBubbyTotalCount(), n);

      if(b->getMass() > 0)
      {
        auto lim = std::floor((mass_limit - getMass()) / b->getMass());
        n = std::min(static_cast<uint32_t>(lim), n);
      }
    }
    else
    {
      n = kMAX_ITEM;
    }
  }
  else
  {
    n = 0;
  }

  return n;
}

// /*
//  * Description: Checks how many of n equipments may be added to the inventory.
//  *
//  * Inputs: equip - pointer to an equipment object to be checked room for
//  *         n - the maximum number of objects to be checked for
//  * Output: int32_t - the # of the equipment the inventory has room for
//  */
// uint32_t Inventory::hasRoom(Equipment* const equip, uint32_t n)
// {
//   if(equip)
//   {
//     if(!getFlag(InvState::SHOP_STORAGE))
//     {
//       n = std::min(equip_limit - getEquipTotalCount(), n);

//       if(equip->getMass() > 0)
//       {
//         auto lim = std::floor((mass_limit - getMass()) / equip->getMass());
//         n = std::min(static_cast<uint32_t>(lim), n);
//       }
//     }
//     else
//     {
//       n = kMAX_ITEM;
//     }
//   }
//   else
//   {
//     n = 0;
//   }

//   return n;
// }

/*
 * Description:
 *
 * Inputs: item - pointer to an item object to be checked room for
 *         n - the maximum number of objects to be checked for
 * Output: int32_t - the # of the items of given type the inventory has room
 */
uint32_t Inventory::hasRoom(Item* const i, uint32_t n)
{
  if(i && !getFlag(InvState::SHOP_STORAGE) && !i->getFlag(ItemFlags::KEY_ITEM))
  {
    auto count = getItemTotalCount(false);
    n = std::min(item_limit - count, n);

    if(i->getMass() > 0)
    {
      auto mass_lim =
          std::floor((mass_limit - getMass()) * 1000 / i->getMass());
      n = std::min(static_cast<uint32_t>(mass_lim), n);
    }
  }

  return n;
}

/*
 * Description: Loads the data from file associated with the inventory.
 *
 * Inputs: XmlData data - the xml data structure
 *         int index - the element reference index
 *         SDL_Renderer* renderer - the rendering engine
 *         std::string base_path - the base path for file handling
 * Output: bool - true if load was successful
 */
bool Inventory::loadData(XmlData data, int index, SDL_Renderer* renderer,
                         std::string base_path)
{
  (void)renderer;
  (void)base_path;
  bool success = true;

  /* ---- LIMITS ---- */
  if(data.getElement(index) == "limits")
  {
    std::string inside = data.getElement(index + 1);

    /* Total Mass Limit : Float Value */
    if(inside == "mass")
    {
      float val = data.getDataFloat(&success);
      if(success)
        setLimits(bubby_limit, equip_limit, item_limit, val);
    }
    /* Integer limits */
    else
    {
      int val = data.getDataInteger(&success);
      if(success)
      {
        /* Bubby Limit */
        if(inside == "bubbies")
        {
          setLimits(val, equip_limit, item_limit, mass_limit);
        }
        /* Equipment Limit */
        // else if(inside == "equipments")
        // {
        //   setLimits(bubby_limit, val, item_limit, mass_limit);
        // }
        /* Generic Item Limit */
        else if(inside == "items")
        {
          setLimits(bubby_limit, equip_limit, val, mass_limit);
        }
        /* Stack Limit - TODO: remove - no longer need this? */
        // else if(inside == "stack")
        // {
        //   setLimits(bubby_limit, equip_limit, val, mass_limit);
        // }
      }
    }
  }

  return success;
}

/*
 * Description: Prints out the information of the Inventory
 *
 * Inputs: simple - boolean value for printing out a simplified version.
 * Output: none
 */
void Inventory::print(bool simple)
{
  std::cout << "\n---- Inventory: " << name << " ----\n";
  std::cout << "ID: " << id << " M: " << getMass() << " / " << mass_limit;
  std::cout << "\n# Unique Items: " << items.size() << "\n";
  std::cout << "# Unique Bubbies: " << bubbies.size() << "\n";
  std::cout << "# Unique Bubbies: " << getBubbyTotalCount() << "/"
            << bubby_limit << "\n";
  // std::cout << "# Unique Equips:  " << equipments.size() << "/" << equip_limit
  //           << "\n";
  std::cout << "Item Stacks: " << getItemStackCount() << "/" << item_limit
            << "\n";
  std::cout << "Item Stacks (+ keys): " << getItemStackCount(true) << std::endl;
  std::cout << "Items: " << getItemTotalCount() << std::endl;
  std::cout << "Items (+ keys): " << getItemTotalCount(true) << std::endl;

  if(!simple)
  {
    std::cout << "\nBubbies:\n";

    for(auto bubby : bubbies)
    {
      if(bubby.first != nullptr && bubby.first->getType())
      {
        std::cout << bubby.first->getType()->getName() << " T: " << bubby.first->getTier()
                  << " " << static_cast<int>(bubby.second) << "\n";
      }
    }

    // std::cout << "\nEquipments:\n";

    // for(auto equipment : equipments)
    // {
    //   if(equipment.first != nullptr)
    //   {
    //     std::cout << equipment.first->getName() << " "
    //               << static_cast<int>(equipment.second) << "\n";
    //   }
    // }

    std::cout << "\nItems:\n";

    for(auto item : items)
    {
      if(item.first != nullptr)
      {
        if(item.first->getFlag(ItemFlags::KEY_ITEM))
          std::cout << "[K] ";
        std::cout << item.first->getGameID() << ": " << item.first->getName()
                  << " " << static_cast<int>(item.second) << "\n";
      }
    }
  }

  std::cout << "--- / Inventory ---" << std::endl
            << std::endl;
}

/*
 * Description: Removes an item by a given game ID. First attempts to remove
 *              a bubby, then equipment, then an item. Returns the success of
 *              the attempted removal.
 *
 * Inputs: game_id - the game ID wanting to be removed
 *         amount - the amount wanting to be removed
 * Output: bool - true if the removal was successful
 */
bool Inventory::removeID(const uint32_t& game_id, const uint16_t& amount)
{
  bool removed = false;

  removed |= removeBubbyID(game_id, amount);

  // if(!removed)
  //   removed |= removeEquipID(game_id, amount);

  if(!removed)
    removed |= removeItemID(game_id, amount);

  return removed;
}

/*
 * Description: Removes an item by a given unique ID. First attempts to remove
 *              a bubby, then equipment, then an item. Returns the success of
 *              the attempted removal.
 *
 * Inputs: unique_id - the uniquie ID wanting to be removed
 *         amount    - the amount wanting to be removed
 * Output: bool - true if the removal was successful
 */
bool Inventory::removeUID(const uint32_t& unique_id, const uint16_t& amount)
{
  bool removed = false;

  removed &= removeBubbyUID(unique_id, amount);

  // if(!removed)
  //   removed &= removeEquipUID(unique_id, amount);

  if(!removed)
    removed &= removeItemUID(unique_id, amount);

  return removed;
}

/*
 * Description: Attempts to remove an amount of Bubbies at a given index.
 *
 * Inputs: index - the index of Bubbies to be removed
 *         amount - the amount of Bubbies to be removed
 * Output: bool - true if the removal was successful
 */
bool Inventory::removeBubbyIndex(const uint32_t& index, const uint16_t& amount)
{
  if(index < bubbies.size())
  {
    if(bubbies.at(index).first != nullptr)
    {
      auto count = bubbies[index].second;

      if(count > 1 && count > amount)
        decreaseCount(bubbies.at(index).first->getGameID(), amount);
      else if(count == amount)
      {
        if(!getFlag(InvState::SHOP_STORAGE))
          delete bubbies[index].first;

        bubbies[index].first = nullptr;
        bubbies.erase(begin(bubbies) + index);
      }
      else
        return false;

      calcMass();
    }

    return true;
  }

  return false;
}

/*
 * Description: Attempts to remove Bubbies by a given game ID
 *
 * Inputs: game_id - the game ID of Bubbies to be removed
 *         amount - the amount of Bubbies to be removed
 * Output: bool - true if the removal was successful
 */
bool Inventory::removeBubbyID(const uint32_t& game_id, const uint16_t& amount)
{
  auto bubby_index = getBubbyIndex(game_id);

  if(bubby_index != -1)
    return removeBubbyIndex(bubby_index, amount);

  return false;
}

/*
 * Description: Attempts to remove Bubbies by a given unique ID
 *
 * Inputs: unique_id - the unique ID of Bubbies to be removed
 *         amount - the amount of Bubbies to be removed
 * Output: bool - true if the removal was successful
 */
bool Inventory::removeBubbyUID(const uint32_t& unique_id,
                               const uint16_t& amount)
{
  auto bubby_index = -1;

  for(uint32_t i = 0; i < bubbies.size(); i++)
    if(bubbies[i].first != nullptr)
      if(bubbies[i].first->getID() == static_cast<int32_t>(unique_id))
        bubby_index = i;

  if(bubby_index != -1)
    return removeBubbyIndex(bubby_index, amount);

  return false;
}

// /*
//  * Description: Attempts to remove equipments by a given index
//  *
//  * Inputs: index - the index of equipments to be removed
//  *         amount - the amount of Bubbies to be removed
//  * Output: bool - true if the removal was successful
//  */
// bool Inventory::removeEquipIndex(const uint32_t& index, const uint16_t& amount)
// {
//   if(index < equipments.size())
//   {
//     if(bubbies.at(index).first != nullptr)
//     {
//       auto count = equipments[index].second;

//       if(count > 1 && count > amount)
//       {
//         std::cout << "count: " << count << " count > 1 && count > amount\n";
//         decreaseCount(equipments.at(index).first->getGameID(), amount);
//       }
//       else if(count == amount)
//       {
//         std::cout << "count == amount\n";
//         if(!getFlag(InvState::SHOP_STORAGE))
//           delete equipments[index].first;

//         equipments[index].first = nullptr;
//         equipments.erase(begin(equipments) + index);
//       }
//       else
//         return false;

//       calcMass();
//     }

//     return true;
//   }

//   return false;
// }


//  * Description: Attempts to remove equipments by an ID
//  *
//  * Inputs: game_id - the game ID of equipments to be removed
//  *         amount - the amount of equipment of the given ID to be removed
//  * Output: bool - true if the removal was successful
 
// bool Inventory::removeEquipID(const uint32_t& game_id, const uint16_t& amount)
// {
//   auto equip_index = getEquipIndex(game_id);

//   if(equip_index != -1)
//     return removeEquipIndex(equip_index, amount);

//   return false;
// }

// /*
//  * Description: Remove an equipment from the Inventory by a given unique item
//  *              ID value
//  *
//  * Inputs: unique_id - the unique ID of equipment to be removed
//  *         amount - the amount of Bubbies to be removed
//  * Output: bool - true if the removal was successful
//  */
// bool Inventory::removeEquipUID(const uint32_t& unique_id,
//                                const uint16_t& amount)
// {
//   auto equip_index = -1;

//   for(uint32_t i = 0; i < equipments.size(); i++)
//     if(equipments[i].first != nullptr)
//       if(equipments[i].first->getID() == static_cast<int32_t>(unique_id))
//         equip_index = i;

//   if(equip_index != -1)
//     return removeEquipIndex(equip_index, amount);

//   return false;
// }

/*
 * Description: Attempts to remove Items by a given index
 *
 * Inputs: index - the Item index to be removed
 *         amount - the amount of Items at the index to be removed
 * Output: bool - true if the removal was successful
 */
bool Inventory::removeItemIndex(const uint32_t& index, const uint16_t& amount)
{
  if(index < items.size())
  {
    if(items.at(index).first != nullptr)
    {
      auto count = items[index].second;

      if(count > 1 && count > amount)
      {
        decreaseCount(items.at(index).first->getGameID(), amount);
      }
      else if(count == amount)
      {
        if(!getFlag(InvState::SHOP_STORAGE))
        {
          if(!items[index].first->isBaseItem())
            delete items[index].first;
        }

        items[index].first = nullptr;
        items.erase(begin(items) + index);
      }
      else
      {
        return false;
      }

      calcMass();

      return true;
    }
  }

  return false;
}

/*
 * Description: Attempts to remove an Item by a given game ID
 *
 * Inputs: game_id - game ID of an item to be removed
 *         amount - the amount of Items of the game ID to be removed
 * Output: bool - true if the removal was successful
 */
// TODO: Loop through and remove Item game ID; could be multiple instances
// Post beta T#355
bool Inventory::removeItemID(const uint32_t& game_id, const uint16_t& amount)
{
  auto item_index = getItemIndex(game_id);

  if(item_index != -1)
    return removeItemIndex(item_index, amount);

  return false;
}

/*
 * Description: Attempts to remove an amount of Item of a given unique ID
 *
 * Inputs: unique_id - the unique Item ID to be removed
 *         amount - the amount of Items of the unique ID to be removed
 * Output: bool - true if the removal was successful
 */
bool Inventory::removeItemUID(const uint32_t& unique_id, const uint16_t& amount)
{
  auto item_index = -1;

  for(uint32_t i = 0; i < items.size(); i++)
    if(items[i].first != nullptr)
      if(items[i].first->getID() == static_cast<int32_t>(unique_id))
        item_index = i;

  if(item_index != -1)
    return removeItemIndex(item_index, amount);

  return false;
}

/*
 * Description: Saves the data of this inventory to the file handler pointer.
 *
 * Inputs: FileHandler* fh - the saving file handler
 * Output: bool - true if successful
 */
bool Inventory::saveData(FileHandler* fh)
{
  if(fh != nullptr)
  {
    fh->writeXmlElement("inventory");

    /* Limits */
    fh->writeXmlElement("limits");
    if(mass_limit > kMIN_MASS)
      fh->writeXmlData("mass", static_cast<float>(mass_limit));

    // TODO: remove. Not needed?
    // if(item_each_limit > kMIN_EACH_ITEM)
    //  fh->writeXmlData("stack", item_each_limit);
    if(item_limit > kMIN_ITEM)
      fh->writeXmlData("items", item_limit);
    if(equip_limit > kMIN_ITEM)
      fh->writeXmlData("equipments", equip_limit);
    if(bubby_limit > kMIN_ITEM)
      fh->writeXmlData("bubbies", bubby_limit);

    fh->writeXmlElementEnd();

    /* Items */
    for(auto const& pair : items)
    {
      if(pair.first)
      {
        std::string set = std::to_string(pair.first->getGameID()) + "," +
                          std::to_string(pair.second);
        fh->writeXmlData("item", set);
      }
    }

    // /* Equipment */
    // for(auto const& pair : equipments)
    // {
    //   if(pair.first)
    //   {
    //     std::string set = std::to_string(pair.first->getGameID()) + "," +
    //                       std::to_string(pair.second);
    //     fh->writeXmlData("equip", set);
    //   }
    // }

    /* Bubbies */
    for(auto const& pair : bubbies)
    {
      if(pair.first)
      {
        std::string set = std::to_string(pair.first->getGameID()) + "," +
                          std::to_string(pair.second);
        fh->writeXmlData("bubby", set);
      }
    }

    fh->writeXmlElementEnd();
    return true;
  }
  return false;
}

/*
 * Description: Attempts to sort a given object by a given SortType in asc or
 *              desc order.
 *
 * Inputs: sort_type - the type of sort to be performed
 *         object - the object to perform the sort on
 *         ascending - whether to perform the sort in asc or desc order
 * Output: bool - the outcome of th sort
 */
bool Inventory::sort(const SortType sort_type, SortObjects object,
                     const bool ascending)
{
  auto sort_status = false;

  switch(object)
  {
  case(SortObjects::BUBBIES):
  {
    if(sort_type == SortType::ID)
    {
      std::sort(begin(bubbies), end(bubbies), Helpers::CompPairByID());
      sort_status = true;
    }
    else if(sort_type == SortType::MASS)
    {
      std::sort(begin(bubbies), end(bubbies), Helpers::CompPairByMass());
      sort_status = true;
    }
    else if(sort_type == SortType::VALUE)
    {
      std::sort(begin(bubbies), end(bubbies), Helpers::CompPairByValue());
      sort_status = true;
    }
    else if(sort_type == SortType::VALUEPERMASS)
    {
      std::sort(begin(bubbies), end(bubbies), Helpers::CompPairByValueMass());
      sort_status = true;
    }

    break;
  }
  case(SortObjects::ITEMS):
  {
    if(sort_type == SortType::COUNT)
    {
      std::sort(begin(items), end(items), Helpers::CompPairByCount());
      sort_status = true;
    }
    else if(sort_type == SortType::ID)
    {
      std::sort(begin(items), end(items), Helpers::CompPairByID());
      sort_status = true;
    }
    else if(sort_type == SortType::MASS)
    {
      std::sort(begin(items), end(items), Helpers::CompPairByMass());
      sort_status = true;
    }
    else if(sort_type == SortType::NAME)
    {
      std::sort(begin(items), end(items), Helpers::CompPairByName());
      sort_status = true;
    }
    else if(sort_type == SortType::VALUE)
    {
      std::sort(begin(items), end(items), Helpers::CompPairByValue());
      sort_status = true;
    }
    else if(sort_type == SortType::VALUEPERMASS)
    {
      std::sort(begin(items), end(items), Helpers::CompPairByValueMass());
      sort_status = true;
    }

    break;
  }
  default:
    break;
  }

  /* Swap the order of the elements in the object if asc is false */
  if(sort_status && !ascending)
  {
    switch(object)
    {
    case(SortObjects::BUBBIES):
      std::reverse(begin(bubbies), end(bubbies));
      break;
    // case(SortObjects::EQUIPMENTS):
    //   std::reverse(begin(equipments), end(equipments));
    //   break;
    case(SortObjects::ITEMS):
      std::reverse(begin(items), end(items));
      break;
    default:
      break;
    }
  }

  return sort_status;
}

/*
 * Description: Returns the pointer to the backdrop of the Inventory
 *
 * Inputs: none
 * Output: Frame* - pointer to the backdrop
 */
Frame* Inventory::getBackdrop()
{
  return backdrop;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint32_t Inventory::getBubbyCount(const uint32_t& game_id, const uint16_t& tier)
{
  for(auto it = begin(bubbies); it != end(bubbies); ++it)
  {
    if((*it).first != nullptr)
    {
      if((*it).first->getGameID() == static_cast<int32_t>(game_id) &&
         (*it).first->getTier() == static_cast<int32_t>(tier))
        return (*it).second;
    }
  }

  return 0;
}

/*
 * Description: Returns the currently assigned bubby_limit
 *
 * Inputs: none
 * Output: uint32_t - the total bubby_limit
 */

uint32_t Inventory::getBubbyLimit()
{
  return bubby_limit;
}

/*
 * Description: Calculates and returns the total count of T0 & TX bubbies
 *
 * Inputs: none
 * Output: uint32_t - the total count of bubbies in the inventory.
 */
uint32_t Inventory::getBubbyTotalCount()
{
  int32_t total{0};

  /* First, add up values of each T0 Bubby */
  for(auto it = begin(bubbies); it != end(bubbies); ++it)
    if((*it).first)
      total += (*it).second;

  return total;
}

// /*
//  * Description: Returns the total number of equipment of given game_id type.
//  *
//  * Inputs: game_id - the game ID to check the equipment count for
//  * Output: int32_t - the count of the equipment at the given game_id
//  */
// int32_t Inventory::getEquipCount(const uint32_t& game_id)
// {
//   for(auto it = begin(equipments); it != end(equipments); ++it)
//     if((*it).first != nullptr)
//       if((*it).first->getGameID() == static_cast<int32_t>(game_id))
//         return (*it).second;

//   return 0;
// }

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t Inventory::getEquipTotalCount()
{
  int32_t total{0};
  
  //TODO: Totalize Item by equipment flag

  return total;
}

/*
 * Description: Returns the currently assigned equipment limit.
 *
 * Inputs: none
 * Output: uint32_t - the current equip limit
 */
uint32_t Inventory::getEquipmentLimit()
{
  return equip_limit;
}

/*
 * Description: Returns the index of bubbies on a given game ID
 *
 * Inputs: game_id - the game ID to find the index on bubbies for
 * Output: int32_t - the index of Bubby having the given game ID
 */
int32_t Inventory::getBubbyIndex(const uint32_t& game_id)
{
  for(uint32_t i = 0; i < bubbies.size(); i++)
    if(bubbies[i].first != nullptr)
      if(bubbies[i].first->getGameID() == static_cast<int32_t>(game_id))
        return i;

  return -1;
}

// /*
//  * Description: Returns the index of a given equipment game ID
//  *
//  * Inputs: game_id - the game ID to find the equipment index for
//  * Output: int32_t - the index of equipment having the given game ID
//  */
// int32_t Inventory::getEquipIndex(const uint32_t& game_id)
// {
//   for(uint32_t i = 0; i < equipments.size(); i++)
//     if(equipments[i].first != nullptr)
//       if(equipments[i].first->getGameID() == static_cast<int32_t>(game_id))
//         return i;

//   return -1;
// }

/*
 * Description: Returns the index of a given item game ID
 *
 * Inputs: game_id - the game ID to find the equipment index for
 * Output: int32_t - the index of item having the given game ID
 */
int32_t Inventory::getItemIndex(const uint32_t& game_id)
{
  for(uint32_t i = 0; i < items.size(); i++)
    if(items[i].first != nullptr)
      if(items[i].first->getGameID() == static_cast<int32_t>(game_id))
        return i;

  return -1;
}

/*
 * Description: Returns the currently assigned item limit
 *
 * Inputs: none
 * Output: uint32_t - the current item limit assigned
 */
uint32_t Inventory::getItemLimit()
{
  return item_limit;
}

/*
 * Description: Returns the vector of bubbies
 *
 * Inputs: none
 * Output: std::vector<std::pair<Bubby*, uint16_t>> - vector of bubbies
 */
std::vector<std::pair<Bubby*, uint16_t>> Inventory::getBubbies()
{
  return bubbies;
}


//  * Description: Returns the vector of equipment
//  *
//  * Inputs: none
//  * Output: std::vector<std::pair<Equipment*, uint16_t>> - vector of equipment
 
// std::vector<std::pair<Equipment*, uint16_t>> Inventory::getEquipments()
// {
//   return equipments;
// }

/*
 * Description: Returns the vector of item
 *
 * Inputs: none
 * Output: std::vector<std::pair<Item*, uint16_t>> - vector of items
 */
std::vector<std::pair<Item*, uint16_t>> Inventory::getItems()
{
  return items;
}

/*
 * Description: Compiles and returns a vector an amount of battle items
 *
 * Inputs: none
 * Output: std::vector<std::pair<Item*, uint16_t>> - vector of battle items
 */
std::vector<std::pair<Item*, uint16_t>> Inventory::getBattleItems()
{
  std::vector<std::pair<Item*, uint16_t>> battle_items;

  for(auto battle_item : items)
  {
    if(battle_item.first != nullptr)
    {
      if((battle_item.first->getOccasion() == ActionOccasion::BATTLE ||
          battle_item.first->getOccasion() == ActionOccasion::ALWAYS) &&
         battle_item.first->getUseSkill() != nullptr)
      {
        battle_items.push_back(battle_item);
      }
    }
  }

  return battle_items;
}

/*
 * Description: Compiles and returns a vector of key items
 *
 * Inputs: none
 * Output: std::vector<std::pair<Item*, uint16_t>> - vector of key items
 */
std::vector<std::pair<Item*, uint16_t>> Inventory::getKeyItems()
{
  std::vector<std::pair<Item*, uint16_t>> key_items;

  for(auto item : items)
    if(item.first != nullptr)
      if(item.first->getFlag(ItemFlags::KEY_ITEM))
        key_items.push_back(std::make_pair(item.first, item.second));

  return key_items;
}

/*
 * Description: Returns a vector of one of each bubby
 *
 * Inputs: none
 * Output: std::vector<Bubby*> - a vector of each bubby
 */
std::vector<Bubby*> Inventory::getUniqueBubbies()
{
  std::vector<Bubby*> temps;

  for(auto bubby : bubbies)
    if(bubby.first != nullptr)
      temps.push_back(bubby.first);

  return temps;
}

// /*
//  * Description: Returns a vector of one of each bubby
//  *
//  * Inputs: none
//  * Output: std::vector<Bubby*> - a vector of each bubby
//  */
// std::vector<Equipment*> Inventory::getUniqueEquipments()
// {
//   std::vector<Equipment*> temps;

//   for(auto equipment : equipments)
//     if(equipment.first != nullptr)
//       temps.push_back(equipment.first);

//   return temps;
// }

/*
 * Description: Returns a vector of one of each item
 *
 * Inputs: none
 * Output: std::vector<Item*> - a vector of each item
 */
std::vector<Item*> Inventory::getUniqueItems()
{
  std::vector<Item*> temps;

  for(auto item : items)
    if(item.first)
      temps.push_back(item.first);

  return temps;
}

/*
 * Description: Returns the count of a given Item game ID
 *
 * Inputs: game_id - the game ID to find the item count for
 * Output: uint32_t - the count of the item of the given game ID
 */
uint32_t Inventory::getItemCount(const uint32_t& game_id)
{
  for(auto& item : items)
    if(item.first != nullptr)
      if(item.first->getGameID() == static_cast<int32_t>(game_id))
        return item.second;

  return 0;
}

/*
 * Description: Returns the total number of item stacks within the inventory.
 *              This is associated with item_limit total.
 *
 * Inputs: bool count_keys - include key item stack count in total
 * Output: uint32_t - the total item stack count
 */
uint32_t Inventory::getItemStackCount(const bool& count_keys)
{
  uint32_t total = 0;

  for(auto item : items)
    if(item.first != nullptr)
      if(!item.first->getFlag(ItemFlags::KEY_ITEM) || count_keys)
        total++;

  return total;
}

/*
 * Description: Returns the total number of items contained in the iventory
 *
 * Inputs: count_keys - boolean value whether to include key item counts
 * Output: uint32_t - the total item count
 */
uint32_t Inventory::getItemTotalCount(const bool& count_keys)
{
  uint32_t total = 0;

  for(auto item : items)
    if(item.first != nullptr)
      if(!item.first->getFlag(ItemFlags::KEY_ITEM) || count_keys)
        total += item.second;

  return total;
}

/*
 * Description: Returns the total mass stored in the inventory (first calls
 *              a function to calculate it)
 *
 * Inputs: none
 * Output: double - the total mass of the Inventory
 */
double Inventory::getMass()
{
  calcMass();

  return curr_mass;
}

/*
 * Description: Returns the mass limit of the Inventory
 *
 * Inputs: none
 * Output: double - the mass limit of the Inventory
 */
double Inventory::getMassLimit()
{
  return mass_limit;
}

/*
 * Description: Returns the thumbnail image of the Inventory
 *
 * Inputs: none
 * Output: Frame* - ptr to the thumbnail image
 */
Frame* Inventory::getThumbnail()
{
  return thumbnail;
}

/*
 * Description: Returns the description of the inventory
 *
 * Inputs: none
 * Output: std::string - the string description of the Inventory
 */
/* Returns the description of the Inventory */
std::string Inventory::getDescription()
{
  return description;
}

/*
 * Description: Returns the string name of the Inventory
 *
 * Inputs: none
 * Output: std::string - the string name of the Inventory
 */
std::string Inventory::getName()
{
  return name;
}

/*
 * Description: Returns the value of a given InvState flag
 *
 * Inputs: test_flag - test flag to check the value for
 * Output: bool - the value of the flag
 */
bool Inventory::getFlag(const InvState& test_flag)
{
  return static_cast<bool>((test_flag & flags) == test_flag);
}

/*
 * Description: Returns the string description of the Inventory
 *
 * Inputs: std::string - the new description for the Inventory
 * Output: bool - true if the new description is within range
 */
bool Inventory::setDescription(const std::string new_description)
{
  if(new_description.size() < StringDb::kMAX_BRIEF_DESC)
  {
    description = new_description;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a given InvState flag a given boolean value
 *
 * Inputs: InvState flag - the flag to be assigned a value
 *         set_value - the boolean value to assign to the flag
 * Output: none
 */
void Inventory::setFlag(const InvState flag, const bool set_value)
{
  (set_value) ? (flags |= flag) : (flags ^= ~flag);
}

/*
 * Description: Assigns backdrop and thumbnail images to the Inventory
 *
 * Inputs: new_backdrop - the new backdrop image to assign for the Inventory
 *         new_thumbnail - the new thumbnail image to assign for the Inventory
 * Output: bool - true if both images are not nullptr
 */
bool Inventory::setImages(Frame* const new_backdrop, Frame* const new_thumbnail)
{
  backdrop = new_backdrop;
  thumbnail = new_thumbnail;

  return (new_backdrop != nullptr && new_thumbnail != nullptr);
}

/*
 * Description: Assigns a new set of limits to the Inventory. This function
 *              will cause all limits to be set within valid ranges.
 *
 * Inputs: bubby_lim - new limit to the maximum number of bubbies
 *         equip_lim - new limit to the maximum number of equipments
 *         item_lim - new limit to the maximum number of items
 *         item_e - new limit to the maximum number of each items
 *         mass_lim - new limit to the total mass of the inventory
 * Output: none
 */
void Inventory::setLimits(const uint32_t bubby_lim, const uint32_t equip_lim,
                          const uint32_t item_lim, const double mass_lim)
{
  bubby_limit = Helpers::setInRange(bubby_lim, kMIN_ITEM, kMAX_ITEM);
  equip_limit = Helpers::setInRange(equip_lim, kMIN_ITEM, kMAX_ITEM);
  item_limit = Helpers::setInRange(item_lim, kMIN_ITEM, kMAX_ITEM);
  mass_limit = Helpers::setInRange(mass_lim, kMIN_MASS, kMAX_MASS);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Assigns an ID to be used for the purpose of currency only
 *
 * Inputs: new_money_id - new ID for money
 * Output: none
 */
void Inventory::setMoneyID(const uint32_t& new_money_id)
{
  money_id = new_money_id;
}
