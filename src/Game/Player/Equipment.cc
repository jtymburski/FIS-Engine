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
Equipment::Equipment(QString name, ushort x, ushort y, Sprite* thumb, uint value, double weight)
    : Item(name, value, thumb, mass)
{

}

/*
 * Description: Equipment class destructor
 */
Equipment::~Equipment()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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
 * Description: Returns the weight added on to the original weight of the
 *              equipment by the Bubbies which are attached to it.
 *
 * Inputs: none
 * Output: double - amount of weight added to the original equipment
 */
double Equipment::getEquipmentMass()
{
  /*
  double mass = 0;
  QVector<Bubby*> attachments = getAttachedBubbies();

  for (int i = 0; i < attachments.size(); i++)
      mass += attachments[i]->getMass();

  return mass; */
  return 0;
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
  /*
  SkillSet* skills = 0;
  QVector<Skill*> temp_skills;
  QVector<ushort> temp_skill_levels;

  QVector<BubbyFlavour*> flavour_list = getAttachedFlavours();

  for (int i = 0; i < flavour_list.size(); i++)
  {
    int tier = getLargestTier(flavour_list.at(i));
    int max_level = BubbyFlavour::getMaxSkillLevel(tier);
    temp_skills = flavour_list[i]->getSkillSet()->getSkills();
    temp_skill_levels = flavour_list[i]->getSkillSet()->getSkillLevels();\

    for (int j = 0; j < temp_skills.size(); i++)
    {
      if (temp_skill_levels.at(j) <= max_level)
        skills->addSkill(temp_skills.at(i));
    }

    temp_skills.clear();
    temp_skill_levels.clear();
  }

  return skills;
  */
  return 0;
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
 * VIRTUAL FUNCTIONS
 *============================================================================*/

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
  qDebug() << "METAL: " << getEquipmentFlag(Equipment::METAL);
  qDebug() << "BROKEN: " << getEquipmentFlag(Equipment::BROKEN);
  qDebug() << "HEAD: " << getEquipmentFlag(Equipment::HEAD);
  qDebug() << "LEFT ARM: " << getEquipmentFlag(Equipment::LEFTARM);
  qDebug() << "RIGHT ARM: " << getEquipmentFlag(Equipment::RIGHTARM);
  qDebug() << "TORSO: "<< getEquipmentFlag(Equipment::TORSO);
  qDebug() << "LEGS: " << getEquipmentFlag(Equipment::LEGS);
  qDebug() << "TWOHAND: " << getEquipmentFlag(Equipment::TWOHAND);
  qDebug() << "EQUIPPED: " << getEquipmentFlag(Equipment::EQUIPPED);
}

/*
 * Description: Prints out the info of the equipment
 *
 * Inputs: none
 * Output: none
 */
void Equipment::printInfo()
{

}
