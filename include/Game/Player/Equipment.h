/******************************************************************************
* Class Name: Equipment
* Date Created: Nov 04 2012
* Inheritance: Item
* Description: Equipment that categorizes under Item in an Inventory unit. Gets
*              called by Person as well for attached Equipment.
* TODO: Implement Action List [01-25-13]
* TODO: Bubby signature check [01-25-13]
******************************************************************************/
#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QVector>

#include "Game/Player/Action.h"
#include "Game/Player/Bubby.h"
//#include "Game/Player/Item.h"

class Equipment : public Item
{
public:
  /* Constructor function*/
  Equipment();

  /* Destructor function*/
  ~Equipment();

private:
  /* 2D 9x9 array for bubby signiture*/
  Bubby* bubby_signature[9][9];

  /* The list of actions offered by the equipment*/
  QVector<Action*> action_list;

  /* A parallel list that shows when actions become available (Based on level)*/
  QVector<int> action_available;

  /* A list that shows useable locations of equipment */
  QVector<bool> equip_locations;

public:
  /* Checks if the bubby will fit into the bubby signature
   * X is the left most coordinate, Y is the top most coordinate
   * Returns if space is available for attachment */
  bool isBubbyAttachable(Bubby* b, int x, int y);

  /* Attempt to attach bubby into the signature
   * X is the left most coordinate, Y is the top most coordinate
   * Returns status of attachment */
  bool attachBubby(Bubby* b, int x, int y);

  /* Gets the list of equipment actions (Used for total action lists in battle)*/
  QVector<Action*> getActionList();

  /* Gets the list of useable locations of equipment */
  bool canEquip(QString location);

  /* Sets the list of useable locations of equipment */
  void setEquipLocations(bool hd = 0, bool la = 0, bool ra = 0,
                         bool to = 0, bool lg = 0);
};

#endif // EQUIPMENT_H
