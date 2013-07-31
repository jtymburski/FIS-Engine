/******************************************************************************
* Class Name: Equipment
* Date Created: Dec 2 2012
* Inheritance: Item
* Description: Equipment that categorizes under Item in an Inventory unit. Gets
*              called by Person as well for attached Equipment.
* TODO: Get Mass [07-28-13]
* TODO: Check over class, add functionality for signature back.
* TODO: Print for Signature, etc.
******************************************************************************/
#include "Game/Player/Equipment.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/


/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Equipment class consructor
 *
 * Inputs: none
 */
Equipment::Equipment(QString name, ushort size_x, ushort size_y,
                     Sprite* thumb, uint value, double weight)
    : Item(name, value, thumb, mass)
{
  createSignature(size_x, size_y);
}

/*
 * Description: Equipment class destructor
 */
Equipment::~Equipment()
{
  if (!getSignature()->isEmpty())
      qDebug() << "Warning: Equipment Signature is NOT EMPTY!";

  delete equip_signature;
  equip_signature = NULL;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Prints out all the info of the equipment by calling the other
 *              print functions.
 *
 * Inputs: none
 * Output: none
 */
void Equipment::printAll()
{
  qDebug() << " --- Equipment --- ";
  printFlags();
  printEquipmentFlags();
  printInfo();
  qDebug() << " --- / Equipment --- ";
}

void Equipment::printFlags()
{
  qDebug() << "BATTLEREADY: " << getItemFlag(Item::BATTLEREADY);
  qDebug() << "MENUREADY: " << getItemFlag(Item::MENUREADY);
  qDebug() << "HEAL ITEM: " << getItemFlag(Item::HEALITEM);
  qDebug() << "CURE: " << getItemFlag(Item::CURE);
  qDebug() << "OFFENSIVE: " << getItemFlag(Item::OFFENSIVE);
  qDebug() << "INDEFINITE: " << getItemFlag(Item::INDEFINITE);
  qDebug() << "STACKABLE: " << getItemFlag(Item::STACKABLE);
  qDebug() << "EQUIPMENT: " << getItemFlag(Item::EQUIPMENT);
  qDebug() << "KEYITEM: " << getItemFlag(Item::KEYITEM);
  qDebug() << "MULTIHIT: " << getItemFlag(Item::MULTIITEM);
  qDebug() << "PARTYITEM: " << getItemFlag(Item::PARTYITEM);
}

/*
 * Description: Prints out the flags of the equipment
 *
 * Inputs: none
 * Output: none
 */
void Equipment::printEquipmentFlags()
{
  qDebug() << "WEAPON: " << getEquipmentFlag(Equipment::WEAPON);
  qDebug() << "BROKEN: " << getEquipmentFlag(Equipment::BROKEN);
  qDebug() << "HEAD: " << getEquipmentFlag(Equipment::HEAD);
  qDebug() << "LEFT ARM: " << getEquipmentFlag(Equipment::LEFTARM);
  qDebug() << "RIGHT ARM: " << getEquipmentFlag(Equipment::RIGHTARM);
  qDebug() << "TORSO: "<< getEquipmentFlag(Equipment::TORSO);
  qDebug() << "LEGS: " << getEquipmentFlag(Equipment::LEGS);
  qDebug() << "TWOHAND: " << getEquipmentFlag(Equipment::TWOHAND);
  qDebug() << "EQUIPPED: " << getEquipmentFlag(Equipment::EQUIPPED);
  qDebug() << "SIGNATURE ENABLED: "
           << getEquipmentFlag(Equipment::SIGNATURE_ENABLED);
}

/*
 * Description: Evaluates a given EquipmentState flag
 *
 * Inputs: EquipmentState flag to be evaluated
 * Output: boolean value of the flag
 */
bool Equipment::getEquipmentFlag(EquipmentState flag)
{
  return (eflag_set.testFlag(flag));
}

/*
 * Description: Returns the mass of the Equipment, including the mass of the
 *              signature.
 *
 * Inputs: none
 * Output: double - total weight of the Equipment.
 */
double Equipment::getEquipmentMass()
{
  return getMass() + getSignature()->getMass();
}

/*
 * Description: Calculates and returns the skills the equipmenmt makes available
 *              for used based off an input (the level of the character the
 *              equipment is equipped to
 *
 * Inputs: ushort level - the level of the person the equipment is attached to
 * Output: SkillSet* - pointer to the skill set the equipment provides
 */
SkillSet* Equipment::getSkills(ushort level)
{
  SkillSet* skills = 0;

  QVector<Skill*> temp_skills;
  QVector<ushort> temp_skill_levels;

  QList<BubbyFlavour*> flavour_list = getSignature()->getUniqueFlavours();
  QList<BubbyFlavour*>::Iterator it = flavour_list.begin();

  for (; it < flavour_list.end(); ++it)
  {
    int max_tier = getSignature()->getHighestTier((*it)->getName());
    int max_level = (*it)->getMaxSkillLevel(max_tier);

    temp_skills = (*it)->getSkillSet()->getSkills();
    temp_skill_levels = (*it)->getSkillSet()->getSkillLevels();

    for (int i = 0; i < temp_skills.size(); i++)
    {
      if (temp_skill_levels.at(i) <= max_level)
        skills->addSkill(temp_skills.at(i));
    }

    temp_skills.clear();
    temp_skill_levels.clear();
  }

  return skills;
}

/*
 * Description: Returns a pointer to the signature.
 *
 * Inputs: none
 * Output: Pointer to the signature object in the equipment.
 */
Signature* Equipment::getSignature()
{
  return equip_signature;
}

/*
 * Description: Given an EquipmentFlags object and a bool, sets the flags
 *              contained in the object to the boolean value.
 *
 * Inputs: EquipmentState flag to be set
 *         set_value - boolean value to set flags to
 * Output: none
 */
void Equipment::setEquipmentFlag(EquipmentState flag, bool set_value)
{
    (set_value) ? (eflag_set |= flag) : (eflag_set &= flag);
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
void Equipment::createSignature(ushort size_x, ushort size_y)
{
  if (equip_signature != NULL)
    delete equip_signature;

  equip_signature = new Signature(size_x, size_y);
}
