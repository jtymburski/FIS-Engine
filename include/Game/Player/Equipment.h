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
#include <memory>

#include "EnumDB.h"
#include "EnumFlags.h"
#include "Frame.h"
#include "Game/Player/Item.h"
#include "Game/Player/Signature.h"
#include "Game/Player/SkillSet.h"

ENUM_FLAGS(EquipState)
enum class EquipState
{ 
  TWO_HANDED = 1 << 0,
  EQUIPPED   = 1 << 1
};

class Equipment : public Item
{
public:
  Equipment(const uint32_t game_id, const std::string name, 
  	        const uint32_t value, const uint32_t mass,
  	        Frame* thumb, const size_t x, const size_t y);

private:

  /* Pointer to the signature */
  std::shared_ptr<Signature> equip_signature;

  /* Equipment flags */
  EquipState equip_flags;

  /* The slot for the Equipment */
  EquipSlots equip_slot;

  /* Skill Sets */
  SkillSet* base_skill_list;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  void createSig(const size_t size_x, const size_t size_y);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  void print(const bool print_signature = false, const bool item_info = false);

  SkillSet* getBaseSkills();

  AttributeSet getBaseStats();

  double getMass();

  bool getEquipFlag(EquipState flags);

  EquipSlots getEquipSlot();

  std::shared_ptr<Signature> getSignature();

  SkillSet getSkills();

  AttributeSet getStats();

  uint32_t getValue();

  bool setBaseSkills(SkillSet* new_base_skills);

  void setEquipFlag(const EquipState flags, const bool set_value = false);

  void setEquipSlot(const EquipSlots new_equip_slot);

  Equipment& operator=(const Equipment& source);

  Equipment& operator=(Equipment&& source);
};

#endif //EQUIPMENT_H
