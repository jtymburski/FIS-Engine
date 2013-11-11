/******************************************************************************
* Class Name: Equipment
* Date Created: Dec 2 2012
* Inheritance: Item
* Description: Equipment that categorizes under Item in an Inventory unit. Gets
*              called by Person as well for attached Equipment.
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
Equipment::Equipment(QString name, uint value, Sprite* thumb, double mass,
                     ushort size_x, ushort size_y, int id)
    : Item(name, value, thumb, mass, id)
{
  createSignature(size_x, size_y);

  temp_skill_list = new SkillSet();
}

/*
 * Description: Equipment class destructor
 */
Equipment::~Equipment()
{
  if (!getSignature()->isEmpty())
      qDebug() << "Warning: Equipment Signature is NOT EMPTY!";

  delete equip_signature;
  equip_signature = 0;

  delete temp_skill_list;
  temp_skill_list = 0;
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
  qDebug() << "MULTIHIT: " << getItemFlag(Item::MULTI_HIT);
  qDebug() << "PARTYITEM: " << getItemFlag(Item::PARTY_HIT);
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
 * Description: Returns the stored base skill list for the Equipment
 *
 * Inputs: none
 * Output: SkillSet* - the base skill set assigned to the equipment
 */
SkillSet* Equipment::getBaseSkillList()
{
  return base_skill_list;
}

/*
 * Description: Calculates an AttributeSet which is equivalent to the bonuses
 *              the bubbies attached to a signature wil provide to the
 *              stat boosts of the equipment.
 *
 * Inputs: none
 * Output: AttributeSet - combined bubby stats of the signature
 */
AttributeSet Equipment::getBubbyBonus()
{
  QList<uint> bubby_bonus_values;
  QList<Bubby*> bubbies = equip_signature->getBubbyMap();

  /* For each bubby in the bubby map */
  for (int i = 0; i < bubbies.size(); i++)
  {
    /* Get the buff set from the bubby */
    AttributeSet* current_set = bubbies.at(i)->getBuffSet();

    /* If the bubby bonus values is empty, push it full of 0's */
    if (bubby_bonus_values.isEmpty())
      for (int j = 0; j < current_set->getSize(); j++)
        bubby_bonus_values.push_back(0);

    /* For each stat in the current set, add it to the bubby bonus value */
    for (int j = 0; j < current_set->getSize(); j++)
        bubby_bonus_values[i] += current_set->getStat(i);
  }

  return AttributeSet(bubby_bonus_values);
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
SkillSet* Equipment::getSkills()
{
  /* Temporary Skill Storage */
  delete temp_skill_list;
  temp_skill_list = new SkillSet();

  QVector<Skill*> temp_skills;
  QVector<ushort> temp_skill_levels;

  /* Find the unique flavours contained in the signature and iterate through */
  QList<BubbyFlavour*> flavour_list = getSignature()->getUniqueFlavours();
  QList<BubbyFlavour*>::Iterator it = flavour_list.begin();

  while (++it < flavour_list.end())
  {
    /* For each flavou, find the max tier and the max level that tier unlocks */
    int max_tier = getSignature()->getHighestTier((*it)->getName());
    int max_level = (*it)->getMaxSkillLevel(max_tier);

    /* Obtain the skills and skill levels for that particular flavour */
    temp_skills = (*it)->getSkillSet()->getSkills();
    temp_skill_levels = (*it)->getSkillSet()->getSkillLevels();

    /* Add the skill to the temp skill total if it falls within the max_level */
    for (int i = 0; i < temp_skills.size(); i++)
    {
      if (temp_skill_levels.at(i) <= max_level)
        temp_skill_list->addSkill(temp_skills.at(i));
    }

    /* Clear the temp skills */
    temp_skills.clear();
    temp_skill_levels.clear();
  }

  return temp_skill_list;
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
 * Description: Assigns a new base skill list to the equipment
 *
 * Inputs: SkillSet* - pointer to the new equipment's skill list
 * Output: none
 */
void Equipment::setBaseSkillList(SkillSet* new_base_skill_list)
{
  base_skill_list = new_base_skill_list;
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

  setEquipmentFlag(Equipment::SIGNATURE_ENABLED);
}
