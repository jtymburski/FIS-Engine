/******************************************************************************
* Class Name: Inventory
* Date Created: November 4th, 2012 (Rewritten June 1st, 2013)
* Inheritance: QWidget
* Description: Inventory class which acts as a container storage and sorting
*              mechanism for storing the player's Items, Equipments, and
*              Bubbies.
\*****************************************************************************/
#ifndef INVENTORY_H
#define INVENTORY_H

#include <QDebug>
#include <QtGui/QWidget>

#include "Game/Player/Bubby.h"
#include "Game/Player/Equipment.h"

class Inventory : public QWidget
{
public:
  /* Constructor function */
  Inventory(QString name, Sprite* thumb = NULL, QWidget *parent = NULL);

  /* Destructor function */
  ~Inventory();
  
  enum InventoryState
  {
    PLAYERSTORAGE = 1ul << 0,
    SHIPSTORAGE   = 1ul << 1,
    ENEMYSTORAGE  = 1ul << 2,
    EMPTY         = 1ul << 3,
    FULL          = 1ul << 4,
    CANBEUPGRADED = 1ul << 5
  };
  Q_DECLARE_FLAGS(InventoryFlags, InventoryState);
  InventoryFlags invflag_set;

private:
  /* Enumerations describing the state of the item sorting */
  ItemSorts bubby_state;
  ItemSorts equipment_state;
  ItemSorts item_state;
  ItemSorts key_item_state;

  /* Vector of pointers to the Bubbies stored in this inventory */
  QVector<Bubby*> bubbies;

  /* Vector of pointers to the equipment stored in this inventory */
  QVector<Equipment*> equipments;

  /* Vector of pointers to the items stored in this inventory */
  QVector<Item*> items;

  /* Vector of pointers to the key items stored in this inventory */
  QVector<Item*> key_items;

  /* Maximum number of Bubbies allowed in the inventory */
  uint bubby_limit;

  /* Maximum # of a certain equipment name allowed in an inventory */
  ushort equip_each_limit;

  /* Maximum equipment allowed in the inventory */
  uint equip_limit;

  /* Maximum # of a certain item name allowed in an inventory */
  ushort item_each_limit;

  /* Maximum items allowed in the inventory */
  uint item_limit;

  /* Current carry mass of the inventory */
  double carry_mass;

  /* Maximum carry mass allowed in the inventory */
  double max_carry_mass;

  /* Name of Inventory storage unit */
  QString name;

  /* Description of the current inventory */
  QString description;

  /* Backdrop image of the inventory */
  Sprite* backdrop;

  /* Image thumbnail of the inventory */
  Sprite* thumbnail;

  /* ------------ Constants (Self-Explanatory) --------------- */
  static const double kMIN_CARRY_MASS;
  static const double kMAX_CARRY_MASS;
  static const ushort kMIN_EACH_ITEM;
  static const ushort kMAX_EACH_ITEM;
  static const ushort kMIN_EACH_EQUIPMENT;
  static const ushort kMAX_EACH_EQUIPMENT;
  static const uint kMIN_BUBBIES;
  static const uint kMAX_BUBBIES;
  static const uint kMIN_EQUIPMENT;
  static const uint kMAX_EQUIPMENT;
  static const uint kMIN_ITEMS;
  static const uint kMAX_ITEMS;

/* ============================================================================
 * PRIVATE FUNCTIONS
 * ===========================================================================*/
private:
  /* Updates the carry mass of the inventory */
  void calcMass();

/*============================================================================
 * SIGNALS
 *============================================================================*/
signals:
  /* Emits a QString describing why the inventory is full -- to player */
  void full(QString reason);

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to add a Bubby to the invetory */
  bool addBubby(Bubby* new_bubby);

  /* Attempts to add a new equipment to the inventory */
  bool addEquipment(Equipment* new_equipment);

  /* Attempts to add a new item to the inventory */
  bool addItem(Item* new_item);

  /* Attempts to add a key item to the inventory */
  bool addKeyItem(Item* new_item);

  /* Removes a Bubby fgrom the inventory */
  Bubby* removeBubby(uint index);

  /* Removes an equipment from the inventory */
  Equipment* removeEquipment(uint index);

  /* Removes an item from the inventory given an index */
  Item* removeItem(uint index);

  /* Removes an item from the inventorygiven a name */
  Item* removeItem(QString name);

  /* Removes a key item from the inventory given an index */
  Item* removeKeyItem(uint index);

  /* Removes an item from the iventory given a name */
  Item* removeKeyItem(QString name);

  /* Methods for printing out the state of the inventory */
  void printAll();
  void printBasic();
  void printBubbies();
  void printEquipment();
  void printFlags();
  void printItems();
  void printKeyItems();

  /* Sorts the vector of bubbies by a given ItemSorts state */
  bool sortBubbies(ItemSorts sort_by);

  /* Sorts the vector of equipments by a given ItemSorts state */
  bool sortEquipments(ItemSorts sort_by);

  /* Sorts the vector of items by a given ItemSorts state */
  bool sortItems(ItemSorts sort_by);

  /* Sorts the vector of items by a given ItemSorts state */
  bool sortKeyItems(ItemSorts sort_by);

  /* Returns a pointer to the backdrop */
  Sprite* getBackdrop();

  /* Returns Bubby given an index */
  Bubby* getBubby(uint index);

  /* Returns the bubby limit */
  uint getBubbyLimit();

  /* Returns the vector of Bubbies in the inventory */
  QVector<Bubby*> getBubbyList();

  /* Returns the enumeration describing the current sorting of Bubbies */
  ItemSorts getBubbyState();

  /* Returns the description of the item */
  QString getDescription();

  /* Returns a pointer to an equipment given an index */
  Equipment* getEquip(uint index);

  /* Gets the # of occurences of a certain name of equipment */
  uint getEquipCount(QString name);

  /* Returns the equip each limit */
  uint getEquipEachLimit();

  /* Returns the equip limit */
  uint getEquipLimit();

  /* Returns the enumeration describing the current sorting of equipment */
  ItemSorts getEquipState();

  /* Returns the vector of equipment in the inventory */
  QVector<Equipment*> getEquipList();

  /* Returns the state of a given InventoryState flag */
  bool getFlag(InventoryState flags);

  /* Returns a pointer to an item in the inventory */
  Item* getItem(uint index);

  /* Gets the # of occurences of a certain name of item */
  uint getItemCount(QString name);

  /* Returns the item each limit */
  uint getItemEachLimit();

  /* Returns the item limit */
  uint getItemLimit();

  /* Returns the vector of items in the inventory */
  QVector<Item*> getItemList();

  /* Returns the enumeration describing the current sorting of items */
  ItemSorts getItemState();

  /* Returns a pointer to a key item given an index */
  Item* getKeyItem(uint index);

  /* Returns a pointer to a key item given a name */
  Item* getKeyItem(QString name);

  /* Returns the vector of key items in the inventory */
  QVector<Item*> getKeyItemList();

  /* Returns the sorted state of the key items */
  ItemSorts getKeyItemState();

  /* Returns the carry mass of the inventory */
  double getMaxCarryMass();

  /* Returns the name of the inventory */
  QString getName();

  /* Returns the thumbnail of the inventory */
  Sprite* getThumb();

  /* Evaluates and returns the current mass of the inventory */
  double getMass();

  /* Assigns a new backdrop image */
  void setBackdrop(Sprite* new_backdrop);

  /* Assigns a new Bubby limit to the inventory */
  void setBubbyLimit(uint new_bubby_limit);

  /* Assigns a new Bubby sorted state */
  void setBubbyState(ItemSorts new_state);

  /* Assigns a new description to the inventory */
  void setDescription(QString new_description);

  /* Assigns an equip each limit */
  void setEquipEachLimit(ushort new_equip_each_limit);

  /* Assigns an equip limit */
  void setEquipLimit(uint new_equip_limit);

  /* Assigns a new equipment sorted state */
  void setEquipState(ItemSorts new_state);

  /* Assigns a given inventory state flag a set_value */
  void setFlag(InventoryState flags, bool set_value = true);

  /* Assigns a new item each limit */
  void setItemEachLimit(ushort new_item_each_limit);

  /* Assigns a set item limit */
  void setItemLimit(uint new_item_limit);

  /* Assigns a new sorted item state */
  void setItemState(ItemSorts new_state);

  /* Assigns a new sorted key item state */
  void setKeyItemState(ItemSorts new_state);

  /* Assigns all the limits of the inventory */
  void setLimits(uint bub_lim, ushort eq_each_lim, uint eq_lim,
                 ushort it_each_lim, uint it_lim, double mass);

  /* Assigns a new max carry mass value */
  void setMaxCarryMass(double new_max_carry_mass);

  /* Assigns a new name of the inventory */
  void setName(QString new_name);

  /* Assigns a new thumbnail to the inventory */
  void setThumb(Sprite* new_thumbnail);

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Inventory::InventoryFlags);

#endif // INVENTORY_H
