/********************************************************************************
* Class Name: Inventory
* Date Created: Nov 04 2012
* Inheritance: QWidget
* Description: The Inventory class
********************************************************************************/
#ifndef INVENTORY_H
#define INVENTORY_H
#include <QtGui/QWidget>
#include <QVector>
#include <QImage>
#include <QString>
#include "InventoryUnit.h"
#include "Equipment.h"
#include "Bubby.h"
#include "Item.h"

class Inventory : public QWidget
{
public:
  /*Constructor function*/
  Inventory(QWidget *parent = 0);

  /*Destructor function*/
  ~Inventory();
private:
  /*Equipment in Inventory*/
  QVector<InventoryUnit> equipment;

  /*Items in Inventory*/
  QVector<InventoryUnit> items;

  /*Bubbies in Inventory*/
  QVector<InventoryUnit> bubbies;

  /*Maximum equipment allowed in Inventory (Can be increased)*/
  int equipment_limit;

  /*Current amount*/
  int equipment_count; //0

  /*Maximum items allowed in Inventory (Can be increased)*/
  int item_limit; //10

  /*Current amount*/
  int item_count; //0

  /*Maximum bubbies allowed in Inventory (Can be increased)*/
  int bubby_limit; //10

  /*Current amount*/
  int bubby_count; //0

  /*Storage level*/
  int level; //0

  /*Name of Inventory storage unit*/
  QString name;

  /*Image of storage unit*/
  QImage thumbnail;

public:

  /*Attempts to add equipment, returns true if there is room to add*/
  bool addTo(Equipment e);

  /*Attempts to add item, returns true if there is room to add*/
  bool addTo(Item i);

  /*Attempts to add bubby, returns true if there is room to add*/
  bool addTo(Bubby b);

  /*Attemots to remove equipment*/
  bool removeFrom(Equipment e);

  /*Attempts to remove item*/
  bool removeFrom(Item i);

  /*Attempts to remove bubby*/
  bool removeFrom(Bubby b);

  /*Upgrades the storage unit*/
  void upgrade(QString name, QImage thumbnail, int equipment_limit, int item_limit, int bubby_limit);

  /*Gets bubby current amount*/
  int getBubbyCount();

  /*Gets bubby limit*/
  int getBubbyLimit();

  /*Gets equipment limit*/
  int getEquipmentLimit();

  /*Gets equipment current amount*/
  int getEquipmentCount();

  /*Gets item limit*/
  int getItemLimit();

  /*Gets item current amount*/
  int getItemCount();

  /*Gets storage level*/
  int getLevel();

  /*Gets storage unit name*/
  QString getName();

  /*Gets thumbnail image*/
  QImage getThumbnail();
};


#endif // INVENTORY_H
