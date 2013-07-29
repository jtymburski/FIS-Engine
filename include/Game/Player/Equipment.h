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
#include <QDebug>

#include "Game/Player/Action.h"
#include "Game/Player/Signature.h"
#include "Game/Player/Item.h"

class Equipment : public Item
{
public:
  /* Constructor function */
  Equipment(QString name, ushort x, ushort y, Sprite* thumb, uint value = 0, double mass = 0);

  /* Destructor function */
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
  /* The signature of the Equipment */
  Signature equip_signature;

  /* The list of actions offered by the equipment */
  QVector<Action*> action_list;

  /* A parallel list that shows when actions become available (Based on level)*/
  QVector<ushort> action_available;

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Method to print out the bubby signature on the Equipment */
  void printAll();
  void printFlags();
  void printInfo();
  void printEquipmentFlags();

  /* Gets the boolean value of flag */
  bool getEquipmentFlag(EquipmentState flags);

  /* Gets the weight added on with Bubbies */
  double getEquipmentMass();

  /* Calculates and returns the skills the equipment provides */
  SkillSet* getSkills(ushort level);

  /* Sets the value of a flag */
  void setEquipmentFlag(EquipmentState flags, bool set_value = true);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Equipment::EquipmentFlags);


#endif // EQUIPMENT_H
