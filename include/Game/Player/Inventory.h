/*******************************************************************************
* Class Name: Inventory [Declaration]
* Date Created: December 17th, 2013
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* - Class testing & conventions [01-18-14]
*******************************************************************************/
#ifndef INVENTORY_H
#define INVENTORY_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "Game/Player/Item.h"
#include "Game/Player/Bubby.h"
#include "Game/Player/Equipment.h"
#include "EnumDb.h"
#include "EnumFlags.h"
#include "Frame.h"
#include "Helpers.h"
#include "StringDb.h"

ENUM_FLAGS(InvState)
enum class InvState
{
  PLAYER_STORAGE = 1 << 0,
  SHIP_STORAGE   = 1 << 1,
  ENEMY_STORAGE  = 1 << 3,
  SHOP_STORAGE   = 1 << 4,
  UPGRADEABLE    = 1 << 5,
  OVERWEIGHT     = 1 << 6,
  ENABLED        = 1 << 7
};

class Inventory
{
public:
	/* Constructs an Inventory using an ID, name, and Icon image */
  Inventory(const uint32_t game_id, const std::string name = "",
  	        Frame* thumb = nullptr);

  /* Annihilates an Inventory object */
  ~Inventory();

private:
  /* Flag state of the Inventory */
  InvState flags;

  /* Items */
  std::vector<std::pair<Bubby*, uint16_t>>     bubbies;
  std::vector<std::pair<Equipment*, uint16_t>> equipments;
  std::vector<std::pair<Item*, uint16_t>>      items;

  /* Current Mass */
  double curr_mass;

  /* Item Limitations */
  uint32_t bubby_limit;
  uint32_t equip_limit;
  uint32_t item_limit;
  uint16_t item_each_limit;
  double   mass_limit;

  /* Running ID counter */
  static uint32_t id;

  /* Game ID of the Inventory */
  uint32_t game_id;

  /* Unique ID of the Inventory */
  uint32_t my_id;

  /* Invalid money ID */
  static uint32_t money_id;

  /* String name and description */
  std::string name;
  std::string description;

  /* Image backdrop and thumbnail image */
  Frame* backdrop;
  Frame* thumbnail;

  /* ------------ Constants --------------- */
  static const double   kMIN_MASS;
  static const double   kMAX_MASS;
  static const uint32_t kMIN_ITEM;
  static const uint32_t kMAX_ITEM;
  static const uint16_t  kMIN_EACH_ITEM;
  static const uint16_t  kMAX_EACH_ITEM;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Calculates the current mass of the Inventory */
  void calcMass();

  /* Increases the count of an item of a given game ID by a given amount */
  bool increaseCount(const uint32_t &game_id, const uint16_t &amount = 1);

  /* Decreases the count of an item of a given game ID by a given amount */
  bool decreaseCount(const uint32_t &game_id, const uint16_t &amount = 1);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
	/* Attempts to add a Bubby */
	AddStatus add(Bubby* new_bubby, const uint32_t &amount = 1,
                bool bypass = false);

  /* Attempts to add an equipment */
	AddStatus add(Equipment* new_equipment, const uint32_t &amount = 1,
                bool bypass = false);

  /* Adds an item to the Inventory */
	AddStatus add(Item* new_item, const uint32_t &amount = 1,
                    bool bypass = false);

  /* Clears the memory of the inventory and the vectors of data */
  void clear(const bool &free = true);

  /* Check if a given item ID exists in the inventory */
	bool contains(const int32_t &id_check);

  /* Calcs and returns the number of spaces in the Inv. for a given Equip */
  uint32_t hasRoom(Equipment* const equip, uint32_t amount = 1);

  /* Calcs and returns the number of spaces in the Inv. for a given Bubby */
  uint32_t hasRoom(Bubby* const bubby, uint32_t amount = 1);

  /* Calcs and returns the number of spaces in the Inv. for a given Item */
  uint32_t hasRoom(Item* const item, uint32_t amount = 1);

  /* Prints out the state of the inventory */
	void print(bool simple = true);

  /* Attempts to remove any item given a game ID and an amount to rem */
  bool removeID(const uint32_t &game_id, const uint16_t &amount = 1);

  /* Attempts to remove any item given a unique ID and an amount to rem */
  bool removeUID(const uint32_t &unique_id, const uint16_t &amount = 1);

  /* Removes a Bubby from the Inventory at a given index */
	bool removeBubbyIndex(const uint32_t &index, const uint16_t &amount = 1);

  /* Removes a Bubby by a given game ID value */
  bool removeBubbyID(const uint32_t &game_id, const uint16_t &amount = 1);

  /* Remove a Bubby from the Inventory by a given unique item ID value */
  bool removeBubbyUID(const uint32_t &unique_id, const uint16_t &amount = 1);

  /* Removes an equipment at a given index */
	bool removeEquipIndex(const uint32_t &index, const uint16_t &amount = 1);

  /* Removes an equipment by a game ID */
  bool removeEquipID(const uint32_t &game_id, const uint16_t &amount = 1);

  /* Remove an equipment from the Inventory by a given unique item ID value */
  bool removeEquipUID(const uint32_t &unique_id, const uint16_t &amount = 1);

  /* Removes an item at a given index */
  bool removeItemIndex(const uint32_t &index, const uint16_t &amount = 1);

  /* Removes an item by a given game ID */
  bool removeItemID(const uint32_t &game_id, const uint16_t &amount = 1);

  /* Remove an item from the Inventory by a given unique item ID value */
  bool removeItemUID(const uint32_t &unique_id, const uint16_t &amount = 1);

  /* Sorts an object in the inventory a certain way */
	bool sort(SortType sort_by, SortObjects object_to_sort,
		        const bool ascending = false);

	/* Evaluates and returns the value of a given Inventory flag */
	bool getFlag(const InvState &test_flag);

  /* Returns the backdrop of the inventory */
  Frame* getBackdrop();

  /* Returns the count of TX Bubbies of a given game_id */
  uint32_t getBubbyCount(const uint32_t &game_id, const uint16_t &tier = 0);

  /* Counts the total number of bubbies */
  uint32_t getBubbyTotalCount();

  /* Counts the # of equipments of a given game ID */
  int32_t getEquipCount(const uint32_t &game_id);

  /* Returns the currently set bubby limit */
  uint32_t getBubbyLimit();

  /* Returns the currently set equipment limit */
  uint32_t getEquipmentLimit();

  int32_t getBubbyIndex(const uint32_t &game_id);

  int32_t getEquipIndex(const uint32_t &game_id);

  /* Returns the index of an item ID in the vector */
  int32_t getItemIndex(const uint32_t &game_id);

  /* Returns the currently set item limit */
  uint32_t getItemLimit();

  /* Returns the count of a given Item game id */
  uint32_t getItemCount(const uint32_t &game_id);

  /* Returns the currently set item each limit */
  uint32_t getItemEachLimit();

  /* Returns the total # of items in the Inventory */
  uint32_t getItemTotalCount(const bool &count_keys = false);

  /* Returns a vector of all Bubbies */
  std::vector<std::pair<Bubby*, uint16_t>> getBubbies();

  /* Returns a vector of all equipment */
  std::vector<std::pair<Equipment*, uint16_t>> getEquipments();

  /* Returns the vector of all standard items */
  std::vector<std::pair<Item*, uint16_t>> getItems();

  /* Returns a vector of all items useable in battle */
  std::vector<std::pair<Item*, uint16_t>> getBattleItems();

  /* Returns the vector of all key items */
  std::vector<std::pair<Item*, uint16_t>> getKeyItems();

  /* Returns a vector of each Bubby */
  std::vector<Bubby*> getUniqueBubbies();

  /* Returns a vector of each Equipment */
  std::vector<Equipment*> getUniqueEquipments();

  /* Returns a vector of each Item */
  std::vector<Item*> getUniqueItems();

  /* Calculates and returns the mass */
  double getMass();

  /* Returns the mass limit */
  double getMassLimit();

  /* Returns the thumbnail image of the Inventory */
  Frame* getThumbnail();

  /* Returns the description of the Inventory */
  std::string getDescription();

  /* Returns the name of the Inventory */
	std::string getName();

	/* Assigns a description to the Inventory */
	bool setDescription(const std::string new_description);

  void setFlag(const InvState flag, const bool set_value = true);

	/* Assigns new images for the backdrop and thumbnail */
	bool setImages(Frame* const new_backdrop, Frame* const new_thumbnail);

  /* Assigns new limits given parameters */
  void setLimits(const uint32_t bubby_lim, const uint32_t equip_lim,
  	             const uint32_t item_lim, const uint16_t item_e,
  	             const double mass_lim);
/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Assigns the invalid money ID */
  static void setMoneyID(const uint32_t &new_money_id);
};

#endif //INVENTORY_H
