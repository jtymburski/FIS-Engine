/******************************************************************************
* Class Name: Equipment
* Date Created: Nov 04 2012
* Inheritance: Item
* Description: Equipment that categorizes under Item in an Inventory unit. Gets
*              called by Person as well for attached Equipment.
******************************************************************************/
#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "Game/Player/Signature.h"
#include "Game/Player/Item.h"

class Equipment : public Item
{
public:
  /* Constructor function */
  Equipment(QString name, uint value, Sprite* thumb, double weight,
            ushort size_x, ushort size_y);

  /* Destructor function */
  ~Equipment();

  /* Enumerated flags for Equipment */
  enum EquipmentState
  {
    WEAPON     = 1ul << 0, /* Is the item a weapon? */
    BROKEN     = 1ul << 1, /* Is the item currently equippable? */
    HEAD       = 1ul << 2, /* Can be attached to the head */
    LEFTARM    = 1ul << 3, /* Can be attached to the left arm */
    RIGHTARM   = 1ul << 4, /* Can be attached to the right arm */
    TORSO      = 1ul << 5, /* Can be attached to torso */
    LEGS       = 1ul << 6, /* Can be attached to legs slot */
    TWOHAND    = 1ul << 7, /* Does the item require both hands? */
    EQUIPPED   = 1ul << 8,  /* Is the item currently equipped to a person? */
    SIGNATURE_ENABLED = 1ul << 9 /* Can the signature have Bubbies? */
  };
  Q_DECLARE_FLAGS(EquipmentFlags, EquipmentState);
  EquipmentFlags eflag_set;

private:
  /* The signature of the Equipment */
  Signature* equip_signature;

  /* The base equipment skill list */
  SkillSet* base_skill_list;
  SkillSet* temp_skill_list;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Clears the temporary skills */
  void clearTempSkills();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Method to print out the bubby signature on the Equipment */
  void printAll();
  void printFlags();
  void printEquipmentFlags();

  /* Returns the basic skill list provided by the Equipment */
  SkillSet* getBaseSkillList();

  /* Returns an Attribute Set of stat bonuses in the Signature */
  AttributeSet getBubbyBonus();

  /* Gets the boolean value of flag */
  bool getEquipmentFlag(EquipmentState flags);

  /* Gets the weight added on with Bubbies */
  double getEquipmentMass();

  /* Calculates and returns the skills the equipment provides (at a level) */
  SkillSet* getSkills();

  /* Returns a pointer to the signature of the Equipment */
  Signature* getSignature();

  /* Assigns a new base skill list to the Equipment */
  void setBaseSkillList(SkillSet* new_base_skill_list);

  /* Sets the value of a flag */
  void setEquipmentFlag(EquipmentState flags, bool set_value = true);

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  void createSignature(ushort size_x, ushort size_y);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Equipment::EquipmentFlags);


#endif // EQUIPMENT_H
