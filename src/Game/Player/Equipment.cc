/*******************************************************************************
 * Class Name: Equipment [Implementation]
 * Date Created: Decmber 14, 2013
 * Inheritance: Item
 * Description: Handles the equipment specialization of items which adds 
 *              signature implementation to the stock item to allow for bubby
 *              addition.
 *
 * Notes
 * -----
 * [1]:
 *
 * See .h file for TODOs
 ******************************************************************************/
#include "Game/Player/Equipment.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: The blank constructor.
 *
 * Inputs: none
 */
Equipment::Equipment() : Item()
{
}

/*
 * Description: Creates a base equipment object.
 *
 * Inputs: game_id - the Game ID for the base item
 *         name    - the string name of the item
 *         value   - the base value of the item in a store
 *         mass    - the base mass of the equip excluding signature
 *         dura    - the maximum durabilty of the equipment
 *         thumb   - pointer to a thumbnail Frame object
 *         x       - the x-dimension of signature to be created
 *         y-      - the y-dimension of signature to be created
 */
Equipment::Equipment(const uint32_t &game_id, const std::string &name, 
  	                 const uint32_t &value, const uint32_t &mass, 
                     const uint32_t &dura, Frame* thumb, const size_t &x, 
                     const size_t &y)
  : Item(game_id, name, value, thumb, mass, dura)
{
  createSig(x, y);
  
  this->setFlag(ItemFlags::EQUIPMENT, true);
}

/*
 * Description: Creates an equipment object based off another equipment
 *
 * Inputs: source - pointer to a base Equipment object
 */
Equipment::Equipment(Equipment* const source) 
  : Item(source)
{
  if (source->getSignature() == nullptr || !source->getSignature()->isEmpty())
    std::cerr << "Warning: Cloning equipment with non-empty Signature\n";

  createSig(source->getSignature()->getX(), source->getSignature()->getY());
  this->setFlag(ItemFlags::EQUIPMENT, true);
}

/*
 * Description: Annihilates an Equipment object.
 *
 */
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
 * Description: Dynamically allocates a new Signature object for use with this
 *              unique equipment.
 *
 * Inputs: size_x - the x-dimension of signature to be created.
 *         size_y - the y-dimension of signature to be created.
 * Output: none
 */
bool Equipment::createSig(const size_t size_x, const size_t size_y)
{
  if (equip_signature == nullptr)
  {
    equip_signature = new Signature(size_x, size_y);

    return true;
  }

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Prints out the information describing the Signature for 
 *              testing purposes.
 *
 * Inputs: print_signature - boolean whether to print the signature
 *         item_info - boolean whether to print the item portion of info
 * Output: none
 */
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

/*
 * Description: Returns the pointer to the base skill set the equipment will 
 *              grant a bearer.
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the base skill set
 */
SkillSet* Equipment::getBaseSkills()
{
  return base_skill_list;
}

/*
 * Description: Returns the base attribute set of the equipment
 *
 * Inputs: none
 * Output: AttributeSet - the base attribute set
 */
AttributeSet Equipment::getBaseStats()
{
  return buff_set;
}

/*
 * Description: Returns the total mass of the equipment including the mass
 *              contained within the equipment.
 *
 * Inputs: none
 * Output: double - the total mass of the equipment
 */
uint32_t Equipment::getMass()
{
  /* If there a signature, add its mass to the base mass */
  if (equip_signature != nullptr)
    return mass + equip_signature->getMass();

  return mass;
}

/*
 * Description: Returns the value of a given EquipState flag.
 *
 * Inputs: EquipState test_flag - the flag to check a value for
 * Output: bool - the current value of the given flag
 */
bool Equipment::getEquipFlag(EquipState test_flag)
{
  return static_cast<bool>((equip_flags & test_flag) == test_flag);
}

/*
 * Description: Returns the corresponding equipment slot the equipment
 *              may be placed on.
 *
 * Inputs: none
 * Output: EquipSlots - enumerated equipment slot
 */
EquipSlots Equipment::getEquipSlot()
{
  return equip_slot;
}

/*
 * Description: Returns the pointer to the equipment's unique signature
 *
 * Inputs: none
 * Output: Signature* - pointer to the signature object
 */
//Signature* Equipment::getSignature()
//{
//  return equip_signature;
//}

/*
 * Description: Returns a combined SkillSet object of the base SkillSet for the
 *              equipment plus the SkillSet granted by the equipment's
 *              signature.
 *
 * Inputs: none
 * Output: SkillSet - created SkilSet containing the combined skills
 */
SkillSet Equipment::getSkills()
{
  return *base_skill_list;
}

/*
 * Description: Returns the combined attribute set granted by the equipment
 *              by adding the base set to the buff set granted by the signature.
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the base skill set
 */
AttributeSet Equipment::getStats()
{
  return getBaseStats() + equip_signature->getBonusStats();
}

/*
 * Description: Returns the total value of the equipment by adding the base 
 *              value to the equip_signature's value.
 *
 * Inputs: none
 * Output: uint32_t - the total value of the equipment
 */
uint32_t Equipment::getValue()
{
  if (equip_signature != nullptr)
    return value + equip_signature->getValue();

  return value;
}

/*
 * Description: Assigns a new base skills pointer to the equipment and 
 *              returns true if the new base skills is not nullptr.
 *
 * Inputs: new_base-skills - pointer to a new skill set to be assigned
 * Output: bool - true if the new SkillSet is not nullptr
 */
bool Equipment::setBaseSkills(SkillSet* new_base_skills)
{
  base_skill_list = new_base_skills;

  return new_base_skills != nullptr;
}

/*
 * Description: Assigns a given EquipState flag a given boolean value
 *
 * Inputs: EquipState flags - the flag to be assigned the value for
 *         const bool set_value - the value to assign the flag to
 * Output: none
 */
void Equipment::setEquipFlag(const EquipState flags, const bool set_value)
{
  (set_value) ? (equip_flags |= flags) : (equip_flags &= ~flags);
}

/*
 * Description: Assigns a new equip slot for the equipment
 *
 * Inputs: EquipSlots - new enumerated equip slot for the equipment
 * Output: none
 */
void Equipment::setEquipSlot(const EquipSlots new_equip_slot)
{
  equip_slot = new_equip_slot;

  /* If two-handed and the new slot is not an arm slot, unset two handed */
  if (getEquipFlag(EquipState::TWO_HANDED))
    if (equip_slot != EquipSlots::LARM && equip_slot != EquipSlots::RARM)
      setEquipFlag(EquipState::TWO_HANDED, false);
}
