/*******************************************************************************
* Class Name: x [Implementation]
* Date Created: 
* Inheritance: 
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Equipment.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Equipment::Equipment(const uint32_t &game_id, const std::string &name, 
  	                 const uint32_t &value, const uint32_t &mass, 
                     const uint32_t &dura, Frame* thumb, const size_t &x, 
                     const size_t &y)
  : Item(game_id, name, value, thumb, mass, dura)
{
  createSig(x, y);
}

Equipment::Equipment(Equipment* const source) 
  : Item(source)
{
  if (source->getSignature() == nullptr || !source->getSignature()->isEmpty())
    std::cerr << "Warning: Cloning equipment with non-empty Signature\n";

  createSig(source->getSignature()->getX(), source->getSignature()->getY());
}

Equipment::~Equipment()
{
  if (equip_signature != nullptr)
    delete equip_signature;

  equip_signature = nullptr;
}


/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Equipment::createSig(const size_t size_x, const size_t size_y)
{
  equip_signature = new Signature(size_x, size_y);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void Equipment::print(const bool print_signature, const bool item_info)
{
  std::cout << "--- Equipment ---";
  std::cout << "Equip Signature? " << (equip_signature != nullptr) << "\n";
  std::cout << "[void]Equip Slot: " << "\n";
  std::cout << "Base Skill List? " << (base_skill_list != nullptr) << "\n";
  std::cout << "TWO_HANDED: " << getEquipFlag(EquipState::TWO_HANDED) << "\n";
  std::cout << "EQUIPPED: " << getEquipFlag(EquipState::EQUIPPED) << "\n";

  if (item_info)
  {
    printInfo();
    printFlags();
  }

  if (print_signature)
    equip_signature->print();
}

SkillSet* Equipment::getBaseSkills()
{
  return base_skill_list;
}

AttributeSet Equipment::getBaseStats()
{
  return buff_set;
}

double Equipment::getMass()
{
  if (equip_signature != nullptr)
    return mass + equip_signature->getMass();

  return mass;
}

bool Equipment::getEquipFlag(EquipState test_flag)
{
  return static_cast<bool>((equip_flags & test_flag) == test_flag);
}

EquipSlots Equipment::getEquipSlot()
{
  return equip_slot;
}

Signature* Equipment::getSignature()
{
  return equip_signature;
}

SkillSet Equipment::getSkills()
{
  return *base_skill_list + *equip_signature->getSkillBonus();
}

AttributeSet Equipment::getStats()
{
  return getBaseStats() + equip_signature->getBonusStats();
}

uint32_t Equipment::getValue()
{
  if (equip_signature != nullptr)
    return value + equip_signature->getValue();

  return value;
}

bool Equipment::setBaseSkills(SkillSet* new_base_skills)
{
  base_skill_list = new_base_skills;

  return new_base_skills != nullptr;
}

void Equipment::setEquipFlag(const EquipState flags, const bool set_value)
{
  (set_value) ? (equip_flags |= flags) : (equip_flags &= ~flags);
}

void Equipment::setEquipSlot(const EquipSlots new_equip_slot)
{
  equip_slot = new_equip_slot;

  /* If two-handed and the new slot is not an arm slot, unset two handed */
  if (getEquipFlag(EquipState::TWO_HANDED))
    if (equip_slot != EquipSlots::LARM && equip_slot != EquipSlots::RARM)
      setEquipFlag(EquipState::TWO_HANDED, false);
}