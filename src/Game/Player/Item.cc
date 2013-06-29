/******************************************************************************
* Class Name: Item
* Date Created: Dec 2 2012
* Inheritance: InventoryUnit
* Description: The Item Header that defines all elements within Inventory.
*              This includes Equipment and Bubby.
******************************************************************************/
#include "Game/Player/Item.h"

/*============================================================================
 * CONSTANTS (Explanation for each in header file)
 *============================================================================*/
int Item::id = 0; /* Starting value for the Item ID */

const ushort Item::kMAX_ACTION_SET = 3; /* Max action set item can perform */
const ushort Item::kMAX_BRIEF_DESCRIPTION = 75; /* Max length brief desc. */
const ushort Item::kMAX_DESCRIPTION = 500; /* Max length desc. of the item */
const double Item::kMAX_MASS        = 250; /* Max mass the item can have */
const ushort Item::kMAX_SKILL_SET   = 5; /* Max # skills the item can unlock */
const uint Item::kMAX_VALUE      = 100000000; /* Max value of items - 100 mil */

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor object for an Item
 *
 * Inputs: QString name - the name of the item
 *         Sprite* thumb - thumbnail of the item for menu uses
 *         uint value - the value of the item in a shop
 *         double mass - the mass of the item
 */
Item::Item(QString name, uint value, Sprite* thumbnail, double mass)
    : my_id(setId()) /* Increment the Item's ID */
{
  setName(name);
  setThumb(thumbnail);
  setValue(value);
  setMass(mass);

  /* Pointer initialization */
  setBuffSet(0);
  setActionSet(0);
  setSkillSet(0);
  setUsingAnimation(0);
  setUsingMessage(0);
  setUsingSound(0);
}

/*
 * Description: Copy constructor for item
 *
 * Inputs: Item* copy - pointer to an item to be copied
 */
Item::Item(Item *copy) : my_id(setId())
{
  setName(copy->getName());
  setThumb(copy->getThumb());
  setValue(copy->getValue());
  setMass(copy->getMass());
  setBuffSet(copy->getBuffSet());
  setActionSet(copy->getActionSet());
  setSkillSet(copy->getSkillSet());
  setUsingAnimation(copy->getUsingAnimation());
  setUsingMessage(copy->getUsingMessage());
  setUsingSound(copy->getUsingSound());
}

/*
 * Description: Annihilates an Item object
 */
Item::~Item() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the ID of the current object and increments the ID
 *              counter.
 *
 * Inputs: none
 * Output: static int - the ID of the Item
 */
int Item::setId()
{
  return id++;
}

/*============================================================================
 * VIRTUAL FUNCTIONS
 *===========================================================================*/

/*
 * Description: Function to print out all the information pretaining to the
 *              state of the Item.
 *
 * Inputs: bool print_extra - default false, prints out ALL the info if true
 * Output: none
 */
void Item::printAll()
{
  qDebug() << " --- Item --- ";
  printInfo();
  printFlags();

  qDebug() << " --- /Item --- ";
}

/*
 * Description: Method to print out the evaluation of the ItemState flags
 *
 * Inputs: none
 * Output: none
 */
void Item::printFlags()
{
  qDebug() << "BATTLEREADY: " << getItemFlag(Item::BATTLEREADY);
  qDebug() << "MENUREADY: " << getItemFlag(Item::MENUREADY);
  qDebug() << "HEAL ITEM: " << getItemFlag(Item::HEALITEM);
  qDebug() << "CURE: " << getItemFlag(Item::CURE);
  qDebug() << "OFFENSIVE: " << getItemFlag(Item::OFFENSIVE);
  qDebug() << "INDEFINITE: " << getItemFlag(Item::INDEFINITE);
  qDebug() << "STACKABLE: " << getItemFlag(Item::STACKABLE);
  qDebug() << "EQUIPMENT: " << getItemFlag(Item::EQUIPMENT);
  qDebug() << "BUBBY: " << getItemFlag(Item::BUBBY);
  qDebug() << "KEYITEM: " << getItemFlag(Item::KEYITEM);
  qDebug() << "MULTIHIT: " << getItemFlag(Item::MULTIITEM);
  qDebug() << "PARTYITEM: " << getItemFlag(Item::PARTYITEM);
}

/*
 * Description: Method to print out the basic info pretaining to the item.
 *
 * Inputs: none
 * Output: none
 */
void Item::printInfo()
{
  qDebug() << "Name: " << name;
  qDebug() << "Brief Description: " << brief_description;
  qDebug() << "Duration: " << duration;
  qDebug() << "Value: " << value;
  qDebug() << "Mass: " << mass;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the set of actions the item can performon use
 *
 * Inputs: none
 * Output: SkillSet* - the set of actions the item can perform on use
 */
SkillSet* Item::getActionSet()
{
  return action_set;
}

/*
 * Description: Returns the AttributeSet describing the amount by which the
 *              item can alter another AttributeSet (i.e. heal, damage, or buff)
 *
 * Inputs: none
 * Output: AttributeSet* - set describing the change in stat values
 */
AttributeSet* Item::getBuffSet()
{
  return buff_set;
}

/*
 * Description: Returns the brief_description of the item
 *
 * Inputs: none
 * Output: QString - the brief description of the item
 */
QString Item::getBriefDescription()
{
  return brief_description;
}

/*
 * Description: Returns the description of an item
 *
 * Inputs: none
 * Output: QString - description of the item
 */
QString Item::getDescription()
{
  return description;
}

/*
 * Description: Returns the duration of an item (turns that it can be used)
 *
 * Inputs: none
 * Output: uint - # of turns the item can be used in battle
 */
ushort Item::getDuration()
{
  return duration;
}

/*
 * Description: Returns the ID of the current Item object
 *
 * Inputs: none
 * Output: int - the ID of the current Item object
 */
int Item::getId()
{
  return my_id;
}

/*
 * Description: Evaluates a given ItemState flag
 *
 * Inputs: ITemState flag to be evaluated
 * Output: boolean value of the flag
 */
bool Item::getItemFlag(ItemState flag)
{
  return (iflag_set.testFlag(flag));
}

/*
 * Description: Returns the mass of the item
 *
 * Inputs: none
 * Output: double - the mass of the item
 */
double Item::getMass()
{
  return mass;
}

/*
 * Description: Returns the name of the item
 *
 * Inputs: QString - name of the item
 * Output: none
 */
QString Item::getName()
{
  return name;
}

/*
 * Description: Returns the SkillSet the item can unlock
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the skillset unlocked by the item
 */
SkillSet* Item::getSkillSet()
{
  return skill_set;
}

/*
 * Description: Returns the image thumbnail of the item
 *
 * Inputs: none
 * Output: QImage - image thumbnail of the item
 */
Sprite* Item::getThumb()
{
  return thumbnail;
}

/*
 * Description: Returns the animation the item will perform during consumption
 *              or use
 *
 * Inputs: none
 * Output: Sprite* - pointer to the animation used by the item
 */
Sprite* Item::getUsingAnimation()
{
  return using_animation;
}

/*
 * Description: Returns the message the item will put out when it is used
 *
 * Inputs: none
 * Output: QString - the using message of the item
 */
QString Item::getUsingMessage()
{
  return using_message;
}

/*
 * Description: Returns the sound the item will perform when used
 *
 * Inputs: none
 * Output: Sound* - pointer to the sound the item will perform when used
 */
Sound* Item::getUsingSound()
{
  return using_sound;
}

/*
 * Description: Returns the value of the item
 *
 * Inputs: none
 * Output: uint - the value of the item
 */
uint Item::getValue()
{
  return value;
}

/*
 * Description: Assigns a set of actions which the item uses
 *
 * Inputs: SkillSet* - set of actions the item performs
 * Output: none
 */
void Item::setActionSet(SkillSet* new_action_set)
{
  if (new_action_set != 0)
  {
    if (new_action_set->getSkills().size() <= kMAX_ACTION_SET)
    {
      action_set = new_action_set;
    }
    else
    {
      SkillSet* temp_action_set = new_action_set;

      while (temp_action_set->getSkills().size() > kMAX_ACTION_SET)
          temp_action_set->removeSkill(temp_action_set->getSkills().size() - 1);

      action_set = temp_action_set;
    }
  }
  else
    action_set = 0;
}

/*
 * Description: Assigns an AttributeSet which describes the amount by which
 *              the item will alter stats when used
 *
 * Inputs: AttributeSet* new_buff_set
 * Output: none
 */
void Item::setBuffSet(AttributeSet* new_buff_set)
{
  buff_set = new_buff_set;
}

/*
 * Description: Assigns a brief description to the item
 *
 * Inputs: QString - new value of brief description
 * Output: none
 */
void Item::setBriefDescription(QString new_brief_description)
{
  if (new_brief_description.size() <= kMAX_BRIEF_DESCRIPTION)
    brief_description = new_brief_description;
  if (brief_description.isEmpty())
    brief_description = "?????";
}

/*
 * Description: Assigns a full length description to the item
 *
 * Inputs: QString new_description - new value of the full description
 * Output: none
 */
void Item::setDescription(QString new_description)
{
  if (description.size() <= kMAX_DESCRIPTION)
    description = new_description;
  if (description.isEmpty())
      description = "?????";
}

/*
 * Description: Assigns a duration to the item
 *
 * Inputs: ushort - duration of the item
 * Output: none
 */
void Item::setDuration(ushort new_duration)
{
  duration = new_duration;
}

/*
 * Description: Given an ItemState object and a bool, assigns the flags
 *              contained in the object to the given boolean value.
 *
 * Inputs: ItemState flag to be set
 *         set_value - boolean value to set flags to (default true)
 * Output: none
 */
void Item::setItemFlag(ItemState flag, bool set_value)
{
  (set_value) ? (iflag_set |= flag) : (iflag_set &= flag);
}

/*
 * Description: Assigns a value to the items mass.
 *
 * Inputs: double - the new value of mass to be assigned
 * Output: none
 */
void Item::setMass(double new_mass)
{
  (new_mass < kMAX_MASS) ? (mass = new_mass) : (mass = kMAX_MASS);
}

/*
 * Description: Assigns a name to the item
 *
 * Input: QString name - name of the item to be assigned
 * Output: none
 */
void Item::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Assigns a new skill set to the item
 *
 * Inputs: SkillSet* - the new skill set to be assigned
 * Output: none
 */
void Item::setSkillSet(SkillSet* new_skill_set)
{
  if (new_skill_set != 0)
  {
    if (new_skill_set->getSkills().size() <= kMAX_SKILL_SET)
    {
      skill_set = new_skill_set;
    }
    else
    {
      SkillSet* temp_skill_set = new_skill_set;

      while (temp_skill_set->getSkills().size() > kMAX_SKILL_SET)
        temp_skill_set->removeSkill(temp_skill_set->getSkills().size() - 1);

      skill_set = temp_skill_set;
    }
  }
  else
    skill_set = 0;
}

/*
 * Description: Assigns a sprite to the item
 *
 * Inputs: Sprite* - pointer to the sprite object to be assigned
 * Output: none
 */
void Item::setThumb(Sprite* new_thumb)
{
  thumbnail = new_thumb;
}

/*
 * Description: Assigns a using animation to the item
 *
 * Inputs: Sprite* - new value of animation to be performed
 * Output: none
 */
void Item::setUsingAnimation(Sprite* new_using_animation)
{
  using_animation = new_using_animation;
}

/*
 * Description: Assigns a using message to the item
 *
 * Inputs: QString - message displayed when the item is used
 * Output: none
 */
void Item::setUsingMessage(QString new_using_message)
{
  using_message = new_using_message;
}

/*
 * Description: Assigns a using sound to the item
 *
 * Inputs: Sound* - sound heard when the item is consumed.
 * Output: none
 */
void Item::setUsingSound(Sound* new_using_sound)
{
  using_sound = new_using_sound;
}

/*
 * Description: Assigns a value to the item
 *
 * Inputs: uint - new value of the item
 * Output: none
 */
void Item::setValue(uint new_value)
{
  (new_value < kMAX_VALUE) ? (value = new_value) : (value = kMAX_VALUE);
}
