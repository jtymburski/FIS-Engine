/******************************************************************************
* Class Name: Inventory
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Inventory class
******************************************************************************/
#include "Game/Player/Inventory.h"

/* Constructor function */
Inventory::Inventory(QWidget* parent)
{
}

/* Destructor function */
Inventory::~Inventory()
{
}

/* Attempts to add equipment, returns true if there is room to add */
bool Inventory::addTo(Equipment equipment)
{
  return TRUE;
}

/* Attempts to add item, returns true if there is room to add */
bool Inventory::addTo(Item item)
{
  return TRUE;
}

/* Attempts to add bubby, returns true if there is room to add */
bool Inventory::addTo(Bubby bubby)
{
  return TRUE;
}

/* Attempts to remove equipment */
bool Inventory::removeFrom(Equipment equipment)
{
    return TRUE;
}

/* Attempts to remove item */
bool Inventory::removeFrom(Item item)
{
    return TRUE;
}

/* Attempts to remove bubby */
bool Inventory::removeFrom(Bubby bubby)
{
    return TRUE;
}

/* Upgrades the storage unit */
void Inventory::upgrade(QString name, QImage thumbnail, int equipment_limit, int item_limit, int bubby_limit)
{
}

/* Gets bubby current amount */
int Inventory::getBubbyCount()
{
    return bubby_count;
}

/* Gets bubby limit */
int Inventory::getBubbyLimit()
{
    return bubby_limit;
}

/* Gets equipment limit */
int Inventory::getEquipmentLimit()
{
    return equipment_limit;
}

/* Gets equipment current amount */
int Inventory::getEquipmentCount()
{
    return equipment_count;
}

/* Gets item limit */
int Inventory::getItemLimit()
{
    return item_limit;
}

/* Gets item current amount */
int Inventory::getItemCount()
{
    return item_count;
}

/* Gets storage level */
int Inventory::getLevel()
{
    return level;
}

/* Gets storage unit name */
QString Inventory::getName()
{
    return name;
}

/* Gets thumbnail image */
QImage Inventory::getThumbnail()
{
    return thumbnail;
}
