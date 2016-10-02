/*******************************************************************************
 * Class Name: Equipment [Declaration]
 * Date Created: December 14th, 2013
 * Inheritance: Item
 * Description: Handles the equipment specialization of items which adds
 *              signature implementation to the stock item to allow for bubby
 *              addition.
 *
 * Notes
 * -----
 * [1]:
 *
 * TODO
 * ----
 * - Develop actual classes of Equipment [01-21-14]
 ******************************************************************************/
#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <iostream>
#include <memory>

//#include "EnumDb.h"
//#include "EnumFlags.h"
//#include "Frame.h"
#include "Game/Player/Item.h"
#include "Game/Player/Signature.h"
//#include "Game/Player/SkillSet.h"

/* Flags unique to equipments */
ENUM_FLAGS(EquipState)
enum class EquipState
{
  TWO_HANDED  = 1 <<  0,
  EQUIPPED    = 1 <<  1,
  STAFF       = 1 <<  2,
  SWORD       = 1 <<  3,
  CLAWS       = 1 <<  4,
  SMALL_ARMS  = 1 <<  5,
  LONG_ARMS   = 1 <<  6,
  BIG_ARMS    = 1 <<  7,
  LIGHT_ARMOR = 1 <<  8,
  MID_ARMOR   = 1 <<  9,
  HEAVY_ARMOR = 1 << 10,
};

class Equipment : public Item
{
public:
  /* Blank constructor */
  Equipment();

  /* Constructs a base equipment item */
  Equipment(const uint32_t &game_id, const std::string &name,
  	        const uint32_t &value, const uint32_t &mass, const uint32_t &dura,
  	        Frame* thumb, const size_t &x, const size_t &y);

  /* Creates an equipment that is a copy of another equipment */
  Equipment(Equipment* const source);

  /* Annihilates an equipment object */
  ~Equipment();

private:
  /* Pointer to the signature */
  Signature* equip_signature;

  /* Equipment flags */
  EquipState equip_flags;

  /* The slot for the Equipment */
  EquipSlots equip_slot;

  /* Skill Sets */
  SkillSet* base_skill_list;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Creates a new signature of a given size */
  bool createSig(const size_t size_x, const size_t size_y);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Prints out the state of the Equipment */
  void print(const bool print_signature = false, const bool item_info = false);

  /* Returns the base skill set */
  SkillSet* getBaseSkills();

  /* Returns the base stat set */
  AttributeSet getBaseStats();

  /* Returns the mass */
  uint32_t getMass();

  /* Evaluates and returns a given EquipState flag */
  bool getEquipFlag(EquipState flags);

  /* Grabs the EquipSlot this equipment can be placed on */
  EquipSlots getEquipSlot();



  /* Returns a ptr to the SkillSet including the Signature's skills */
  SkillSet getSkills();

  /* Returns the Equipment's current total stat buff */
  AttributeSet getStats();

  /* Returns the value of the equipment (based on value of the Signature) */
  uint32_t getValue();

  /* Assigns a new base skill set ptr */
  bool setBaseSkills(SkillSet* new_base_skills);

  /* Assigns a given EquipState flag to a given value */
  void setEquipFlag(const EquipState flags, const bool set_value = false);

  /* Assigns a new equip slot for the Equipment */
  void setEquipSlot(const EquipSlots new_equip_slot);
};

#endif //EQUIPMENT_H
