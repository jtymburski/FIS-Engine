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
*******************************************************************************/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "Game/Player/Item.h"
#include "Game/Player/Bubby.h"
#include "Game/Player/Equipment.h"
#include "EnumFlags.h"
#include "Frame.h"
#include "Helpers.h"
#include "StringDb.h"

using Bubby_It  = std::vector<Bubby*>::iterator;
using Bubby0_It = std::vector<std::pair<Bubby*, uint8_t>>::iterator;
using Equip_It  = std::vector<Equipment*>::iterator;
using Item_It   = std::vector<std::pair<Item*, uint8_t>>::iterator;

ENUM_FLAGS(InvState)
enum class InvState
{
  PLAYER_STORAGE = 1 << 0,
  SHIP_STORAGE   = 1 << 1,
  ENEMY_STORAGE  = 1 << 3,
  UPGRADEABLE    = 1 << 4,
  OVERWEIGHT     = 1 << 5,
  ENABLED        = 1 << 6
};

class Inventory
{
public:
	/* Constructs an Inventory using an ID, name, and Icon image */
  Inventory(const uint32_t game_id, const std::string name = "", 
  	        Frame* thumb = nullptr);

  enum class SortObjects
  { BUBBIES, EQUIPMENTS, ITEMS, KEY_ITEMS };

private:
  /* Flag state of the Inventory */
  InvState flags;

  /* Items */
  std::vector<Bubby*>                     bubbies;
  std::vector<std::pair<Bubby*, uint8_t>> zero_bubbies;
  std::vector<Equipment*>                 equipments;
  std::vector<std::pair<Item*, uint8_t>>  items;

  /* Current Mass */
  double curr_mass;

  /* Item Limitations */
  uint32_t bubby_limit;
  uint32_t equip_limit;
  uint32_t item_limit;
  uint8_t  item_each_limit;
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
  static const uint8_t  kMIN_EACH_ITEM;
  static const uint8_t  kMAX_EACH_ITEM;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  double calcMass();

  bool sortBubbies(Bubby_It begin, Bubby_It stop, const ObjectSorts &sort_type, 
  	               const bool &asc = true);

  bool sortEquipments(Equip_It begin, Equip_It end, 
                      const ObjectSorts &sort_type, const bool &asc = true);

  bool sortItems(Item_It begin, Item_It end, const ObjectSorts &sort_type, 
  	             const bool &asc = true);

  bool increaseBubbyCount(const uint32_t &game_id);

  bool increaseItemCount(const uint32_t &game_id);

  bool decreaseBubbyCount(const uint32_t &game_id);

  bool decreaseItemCount(const uint32_t &game_id);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
	/* Attempts to add a Bubby */
	bool addBubby(Bubby* new_bubby, const bool bypass = false);

  /* Attempts to add an equipment */
	bool addEquipment(Equipment* new_equipment, const bool bypass = false);

  /* Adds an item to the Inventory */
	bool addItem(Item* new_item, const bool bypass = false);

  /* Check if a given item ID exists in the inventory */
	bool contains(const int &id_check);

  /* Prints out the state of the inventory */
	void print(bool simple = true);

  /* Removes a Bubby from the Inventory at a given index */
	bool removeBubby(uint32_t index);

  /* Removes an equipment at a given index */
	bool removeEquipment(uint32_t index);
 
  /* Removes an item at a given index */
  bool removeItem(uint32_t index);

  /* Sorts an object in the inventory a certain way */
	bool sort(ObjectSorts sort_by, SortObjects object_to_sort, 
		        const bool ascending = false);

	/* Evaluates and returns the value of a given Inventory flag */
	bool getFlag(const InvState test_flag);

  /* Returns the backdrop of the inventory */
  Frame* getBackdrop();

  /* Returns a vector of all items useable in battle */
  std::vector<std::pair<Item*, uint8_t>> getBattleItems();

  /* Returns the count of Bubbies of a given game_id */
  uint32_t getBubbyCount(const uint32_t &game_id);

  /* Returns the index on the vector of a given Bubby zero ID */
  int32_t getBubbyIndex(const uint32_t &game_id);

  /* Returns the currently set bubby limit */
  uint32_t getBubbyLimit();

  /* Returns the total count of T0 and TX Bubbies */
  uint32_t getTotalBubbyCount();

   /* Returns a vector of all Bubbies */
  std::vector<Bubby*> getBubbies();

  /* Counts the # items with a given Game ID */
  uint32_t getCount(const int &game_id);

  /* Returns the currently set equipment limit */
  uint32_t getEquipmentLimit();

  /* Returns a vector of all equipment */
  std::vector<Equipment*> getEquipments();

  /* Returns the index of an item ID in the vector */
  int32_t getItemIndex(const uint32_t &game_id);

  /* Returns the currently set item limit */
  uint32_t getItemLimit();

  /* Returns the vector of all standard items */
  std::vector<std::pair<Item*, uint8_t>> getItems();

  /* Returns the count of a given Item game id */
  uint32_t getItemCount(const uint32_t &game_id);

  /* Returns the currently set item each limit */
  uint32_t getItemEachLimit();

  /* Returns the total # of items in the Inventory */
  uint32_t getItemTotalCount();

  /* Returns the vector of all key items */
  std::vector<Item*> getKeyItems();

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
  	             const uint32_t item_lim, const uint8_t item_e, 
  	             const double mass_lim);
/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Assigns the invalid money ID */
  static void setMoneyID(const uint32_t &new_money_id);

};

#endif //INVENTORY_H
