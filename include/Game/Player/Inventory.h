/******************************************************************************
* Class Name: Inventory
* Date Created: Nov 04 2012
* Inheritance: QWidget
* Description: The Inventory class
\*****************************************************************************/
#ifndef INVENTORY_H
#define INVENTORY_H

#include <QtGui/QWidget>

#include "Game/Player/Bubby.h"
#include "Game/Player/Equipment.h"
#include "Game/Player/Item.h"

class Inventory : public QWidget
{
public:
  /* Constructor function */
  Inventory(QString name, QPixmap* thumbnail = NULL, uint limit = 0,
            QWidget *parent = NULL);

  /* Destructor function */
  ~Inventory();
  
private:
  /* Data structures holding the item objects */
  QVector<Bubby> bubbies;
  QVector<Equipment> equipments;
  QVector<Item> items;
  QVector<Item> key_items;

  /* Data structures holding the item counts */
  QVector<uint> equip_count;
  QVector<uint> item_count;

  /* Maximum equipment allowed in vectors */
  uint equip_limit;
  uint item_limit;
  uint bubby_limit;

  /* Storage level */
  uint level;
  double max_carry_weight;

  /* Name of Inventory storage unit */
  QString name;

  /* Image of storage unit */
  QPixmap* thumb;

  /* ------------ Constants --------------- */
  static const uint kBASIC_ITEM_LIMIT;
  static const uint kMAX_EQUIP_LIMIT;
  static const uint kMAX_ITEM_LIMIT;
  static const uint kMAX_BUBBY_LIMIT;
  static const uint kMAX_LEVEL;
  static const uint kMAX_EQUIP_STACK;
  static const uint kMAX_ITEM_STACK;
  static const uint kMAX_CARRY_WEIGHT;

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to reset the level of the inventory */
  bool resetLevel(const uint new_level);

  /* Methods for printing out the info of the Inventory */
  void printAll();
  void printInfo();
  void printItems();

  /* Attempts to add equipment, returns true if there is room to add */
  bool addTo(Equipment equipment);

  /* Attempts to add item, returns true if there is room to add */
  bool addTo(Item item);

  /* Attempts to add bubby, returns true if there is room to add */
  bool addTo(Bubby bubby);

  /* Attemots to remove equipment */
  bool removeFrom(Equipment equipment);

  /* Attempts to remove item */
  bool removeFrom(Item item);

  /* Attempts to remove bubby */
  bool removeFrom(Bubby b);

  /* Attempts to upgrade the storage unit */
  void upgrade(QString name, QPixmap* thumbnail, uint equipment_limit,
                     uint item_limit, uint bubby_limit);

  /* Calculates the current weight of all items */
  double getCurrentWeight();

  /* Gets the count of equipment or items */
  uint getCount(const Item item);
  uint getCount(Equipment equipment);

  /* Gets bubby current amount */
  uint getBubbyCount();

  /* Gets bubby limit */
  uint getBubbyLimit();

  /* Gets equipment limit */
  uint getEquipmentLimit();

  /* Gets equipment current amount */
  uint getEquipmentCount();

  /* Gets item limit */
  uint getItemLimit();

  /* Gets item current amount */
  uint getItemCount();

  /* Gets storage level */
  uint getLevel();

  /* Returns the current maximum carry weight of the Inv. */
  double getMaxCarryWeight();

  /* Gets storage unit name */
  QString getName();

  /* Gets thumbnail image */
  QPixmap* getThumbnail();

  /* Sets the carry weight of the inventory */
  void setMaxCarryWeight(double gravity, ushort party_size);
};

#endif // INVENTORY_H
