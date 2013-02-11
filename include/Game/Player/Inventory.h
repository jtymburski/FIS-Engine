/******************************************************************************
* Class Name: Inventory
* Date Created: Nov 04 2012
* Inheritance: QWidget
* Description: The Inventory class
******************************************************************************/
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
  Inventory(QString name, QImage thumbnail, uint limit = 0, QWidget *parent = 0);

  /* Destructor function */
  ~Inventory();
  
private:
  /* Data structures holding the item objects */
  QVector<Item> equipments;
  QVector<Item> items;
  QVector<Item> key_items;
  QVector<Item> bubbies;

  /* Data structures holding the item counts */
  QVector<uint> equip_count;
  QVector<uint> item_count;

  /* Maximum equipment allowed in Inventory (Can be increased) */
  uint equip_limit;

  /* Maximum items allowed in Inventory (Can be increased) */
  uint item_limit; //10

  /* Maximum bubbies allowed in Inventory (Can be increased) */
  uint bubby_limit; //10

  /* Storage level */
  uint level; //0

  /* Storage and level limits */
  static const uint kBASIC_ITEM_LIMIT = 50;
  static const uint kMAX_EQUIP_LIMIT  = 10000;
  static const uint kMAX_ITEM_LIMIT   = 10000;
  static const uint kMAX_BUBBY_LIMIT  = 10000;
  static const uint kMAX_LEVEL        = 25;
  static const uint kMAX_EQUIP_STACK  = 10;
  static const uint kMAX_ITEM_STACK   = 50;

  /* Name of Inventory storage unit */
  QString name;

  /* Image of storage unit */
  QImage thumb;

public:
  /* Attempts to reset the level of the inventory */
  const bool resetLevel(const uint new_level);

  /* Attempts to add equipment, returns true if there is room to add */
  bool addTo(Equipment equipment);

  /* Attempts to add item, returns true if there is room to add */
  bool addTo(Item item);

  /* Attempts to add bubby, returns true if there is room to add */
  bool addTo(Bubby bubby);

  /* Attemots to remove equipment */
  const bool removeFrom(Equipment equipment);

  /* Attempts to remove item */
  const bool removeFrom(Item item);

  /* Attempts to remove bubby */
  const bool removeFrom(Bubby b);

  /* Attempts to upgrade the storage unit */
  void upgrade(QString name, QImage thumbnail, uint equipment_limit,
                     uint item_limit, uint bubby_limit);

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

  /* Gets storage unit name */
  QString getName();

  /* Gets thumbnail image */
  QImage getThumbnail();
};

#endif // INVENTORY_H
