/*******************************************************************************
* Class Name: Equipment [Declaration]
* Date Created: December 14th, 2013
* Inheritance: None
* Description: 
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*******************************************************************************/

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <iostream>

#include "Game/Player/EnumDb.h"
#include "Game/Player/EnumFlags.h"
#include "Game/Player/Item.h"
#include "Game/Player/Signature.h"

ENUM_FLAGS(EquipState)
enum class EquipState
{ 
  TWO_HANDED = 1 << 0,
  EQUIPPED   = 1 << 1
};

class Equipment : public Item
{
public:

  Equipment();

  Equipment(const Equipment& source);

  Equipment(Equipment&& source);

  Equipment(const uint32_t game_id, const std::string name, 
  	        const uint32_t value, const uint32_t mass,
  	        Sprite* thumb, const size_t x, const size_t y);

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
  SkillSet* temp_skill_list;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  void createSig(const size_t size_x, const size_t size_y);

  void unsetAll();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  void print(const bool print_signature = false, const bool item_info = false);

  SkillSet* getBaseSkills();

  AttributeSet getBaseStats();

  double getMass();

  bool getEquipFlag(EquipmentState flags);

  EquipSlots getEqupSlot();

  Signature* getSignature();

  SkillSet* getSkills();

  AttributeSet getStats();

  uint32_t getValue();

  bool setBaseSkills(SkillSet* new_base_skills);

  void setEquipFlag(const EquipState flags, const bool set_value = false);

  void setEquipSlot(const EquipmentSlot new_equip_slot);
};

#endif //EQUIPMENT_H
