/******************************************************************************
* Class Name: Item
* Date Created: Dec 2 2012
* Inheritance: InventoryUnit
* Description: The Item Header that defines all elements within Inventory.
*              This includes Equipment and Bubby.
* //TODO: Remove incrementing id [10-28-13]
******************************************************************************/
#include "Game/Player/Item.h"

/*============================================================================
 * CONSTANTS (Explanation for each in header file)
 *============================================================================*/
const ushort Item::kMAX_ACTION_SET = 3; /* Max action set item can perform */
const ushort Item::kMAX_BRIEF_DESCRIPTION = 75; /* Max length brief desc. */
const ushort Item::kMAX_DESCRIPTION = 500; /* Max length desc. of the item */
const double Item::kMAX_MASS        = 250; /* Max mass the item can have */
const ushort Item::kMAX_SKILL_SET   = 5; /* Max # skills the item can unlock */
const uint Item::kMAX_VALUE      = 100000000; /* Max value of items - 100 mil */
const int Item::kMINIMUM_ID =  0; /* The minimum ID # for an Item */
const int Item::kUNSET_ID   = -1; /* Placeholder unset ID */

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
Item::Item(QString name, uint value, Sprite* thumbnail, double mass, int id)
    : QWidget(0)
{
  setName(name);
  setID(id);
  setThumb(thumbnail);
  setValue(value);
  setMass(mass);
  setDuration(0);
  setAction(0);
  setBuffSet(0);
  setSkill(0);
  setUsingAnimation(0);
  setUsingMessage("");
  setUsingSound(0);
}

/*
 * Description: Copy constructor for item
 *
 * Inputs: Item* copy - pointer to an item to be copied
 */
Item::Item(Item *copy) 
{
  setBuffSet(copy->getBuffSet());
  setBriefDescription(copy->getBriefDescription());
  setDescription(copy->getDescription());
  setDuration(copy->getDuration());
  setID(copy->getId());
  setMass(copy->getMass());
  setName(copy->getName());
  setAction(copy->getAction());
  setSkill(copy->getSkill());
  setThumb(copy->getThumb());
  setUsingAnimation(copy->getUsingAnimation());
  setUsingMessage(copy->getUsingMessage());
  setUsingSound(copy->getUsingSound());
  setValue(copy->getValue());
}

/*
 * Description: Annihilates an Item object
 */
Item::~Item() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Sets the ID for the item. If out of the allowable range, the
 *              value is set to a minimum storage value (OOR).
 *
 * Inputs: int new_id - the new integer ID to define the item.
 * Output: bool - status if the new_id is within the allowable range.
 */
bool Item::setID(int new_id)
{
  /* If the ID is out of range */
  if (new_id < kMINIMUM_ID)
  {
    id = kUNSET_ID;
    return false;
  }

  /* Otherwise, the ID is good */
  id = new_id;
  return true;
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
 * Output: Skill* - the set of actions the item can perform on use
 */
Skill* Item::getAction()
{
  return action;
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
   return id;
}

/*
 * Description: Evaluates a given ItemState flag
 *
 * Inputs: ItemState flag to be evaluated
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
 * Description: Returns the prefix of the item
 *
 * Inputs: QString - prefix of the item
 * Output: none
 */
QString Item::getPrefix()
{
  return prefix;
}

/*
 * Description: Returns the Skill the item can unlock
 *
 * Inputs: none
 * Output: Skill* - pointer to the skillset unlocked by the item
 */
Skill* Item::getSkill()
{
  return skill;
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
 * Inputs: Skill* - set of actions the item performs
 * Output: none
 */
void Item::setAction(Skill* new_action)
{
  action = new_action;
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
 * Input: QString new_name - name of the item to be assigned
 * Output: none
 */
void Item::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Assigns a new prefix to the item
 *
 * Input:  QString new_prefix - the new prefix to be assigned
 * Output: none
 */
void Item::setPrefix(QString new_prefix)
{
  prefix = new_prefix;
}

/*
 * Description: Assigns a new skill set to the item
 *
 * Inputs: SkillSet* - the new skill set to be assigned
 * Output: none
 */
void Item::setSkill(Skill* new_skill)
{
  skill = new_skill;
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
