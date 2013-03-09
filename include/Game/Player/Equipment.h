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

  /* Enumerated flags for Equipment */
  enum EquipmentState
  {
    WEAPON     = 1ul << 0, /* Is the item a weapon? */
    METAL      = 1ul << 1, /* Is the equipment metal? */
    BROKEN     = 1ul << 2, /* Is the item currently equippable? */
    HEAD       = 1ul << 3, /* Can be attached to the head */
    LEFTARM    = 1ul << 4, /* Can be attached to the left arm */
    RIGHTARM   = 1ul << 5, /* Can be attached to the right arm */
    TORSO      = 1ul << 6, /* Can be attached to torso */
    LEGS       = 1ul << 7, /* Can be attached to legs slot */
    TWOHAND    = 1ul << 8, /* Does the item require both hands? */
    EQUIPPED   = 1ul << 9  /* Is the item currently equipped to a person? */
  };
  Q_DECLARE_FLAGS(EquipmentFlags, EquipmentState);
  EquipmentFlags eflag_set;

private:
  /* Equipment Constants */
  const static uint kSIG_X = 9;
  const static uint kMAX_SIG_Y = 9;

  /* 2D 9x9 array for bubby signature*/
  Bubby* bubby_signature[9][9];

  /* The list of actions offered by the equipment*/
  QVector<Action*> action_list;

  /* A parallel list that shows when actions become available (Based on level)*/
  QVector<unsigned short> action_available;

public:
  /* Checks if the bubby will fit into the bubby signature
   * X is the left most coordinate, Y is the top most coordinate
   * Returns if space is available for attachment */
  bool isBubbyAttachable(Bubby* b, uint x, uint y);

  /* Attempt to attach bubby into the signature
   * X is the left most coordinate, Y is the top most coordinate
   * Returns status of attachment */
  bool attachBubby(Bubby* b, uint x, uint y);

  /* Gets the list of useable locations of equipment */
  bool canEquip(QString location);

  /* Gets the list of equipment actions (Used for total action lists in battle)*/
  QVector<Action*> getActionList();

  /* Clears the action list */
  void clearActionList();

  /* Toggles an EquipmentState flag */
  void toggleEquipmentFlag(EquipmentState flags);

  /* Gets the boolean value of flag */
  const bool getEquipmentFlag(EquipmentState flags);

  /* Sets the value of a flag */
  void setEquipmentFlag(EquipmentState flags, const bool set_value);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Equipment::EquipmentFlags);


#endif // EQUIPMENT_H
