/******************************************************************************
* Class Name: Person Implementation
* Date Created: December 2nd, 2012 (Rewritten March 6th, 2013)
* Inheritance: None
* Description: Head for Person class. Container for all the info describing
*              a character in the game, including their statistics for battle,
*              status ailments, flags, etc.
* // TODO: Race and category equip item types [03-06-13]
* // TODO: Finish use item
* // TODO: Rewrite LEVEL UP
* // TODO: Setup Functions
******************************************************************************/

#include "Game/Player/Person.h"
#include <algorithm>

/*
 * Description: Constructs a Person object gievn a name, race and category
 *              pointers, plus two strings describing which stats they are
 *              focused on and how much (their curve)
 *
 * Inputs: QString name - name of the person
 *         Race*   - pointer to the race of the person
 *         Category* - pointer to the category of the person
 *         QString p - primary element and curve of the person
 *         QString s - secondary element and curve of the person
 * Output: none
 */
Person::Person(QString name, Race *race, Category* cat, QString p, QString s)
{
  /* Initial preparation */
  setName(name);
  setRace(race);
  setCategory(cat);
  setPrimary(p);
  setSecondary(s);
  skills = NULL;
  setExp(0);
  setLevel(1);

  /* Set up all three attribute sets */
  setUpBaseStats();
  setStats(base_stats);
  setTempStats(base_stats);

  /* Initially gather all usable skills */
  calcSkills();

  /* Append kEQUIP_SLOTS of NULL equipment initially */
  for (int i = 0; i < kEQUIP_SLOTS; i++)
    equipment.append(NULL);

  setFirstPerson();
  setThirdPerson();
}

/*
 * Description: Annihilates a Person object
 */
Person::~Person()
{
  for (int i = 0; i < equipment.size(); i++)
  {
    delete equipment.at(i);
    equipment[i] = NULL;
  }
  // delete cat;
  // delete race;
  if (skills != NULL)
  {
    delete skills;
    skills = NULL;
  }
  delete first_person;
  delete third_person;
  // cat = NULL;
  // race = NULL;
  first_person = NULL;
  third_person = NULL;
}

/*
 * Description: Adds an ailment to to the Person.
 *
 * Inputs: Ailment - new ailment to be inflicted
 * Output: none
 * // TODO: Check for duplicates [03-06-13]
 */
void Person::addAilment(Ailment new_ailment)
{
  if (ailments.size() > 4)
    removeAilment(0);
  ailments.push_back(new_ailment);
}

/*
 * Description: Adds an amount to the experience of the person and calls
 *              setLevel() if necessary to deal with level ups accordingly
 *              Will deal with multiple level ups.
 *
 * Note: The formula for experience in mathematical form is currently:
 *
 *       let x = player's level + 1
 *        f(x) = minimum experience needed to reach level x
 *
 *        f(x) = floor(x + 300 * 2 ^ 1/7), 0 < x <= 126
 *
 * Inputs: value - amount of experience to be added
 * Output: none
 */
void Person::addExperience(uint value)
{
 /* Finds the experience value of next level and if the current
   * experience value of the person exceeds this, level up the
   * character until its experience matches the correct level */
  uint l = getLevel();
  uint y = 0;
  experience += value;

  do
  {
    y = floor(l + 300 * pow(2, l / 7));
    if (experience >= y)
      if ((l + 1 > 0) && (l + 1 < 126))
              setLevel(l++);
  } while (experience >= y);
}

/*
 * Description: Prepares the Person for a battle, by assigning his temp min and
 *              max stats as the current stats value.
 *
 * Inputs: none
 * Output: none
 */
void Person::battlePrep()
{
  for (int i = 0 ; i < temp_stats.getSize(); i++)
  {
    temp_stats.setStat(i, stats.getStat(i));
    temp_stats.setMax(i, stats.getStat(i));
  }
}

/* // TODO NOT FINISHED
 * Description: This function calculates the currently usable skills of the
 *              person, based on the skills allowed by their Race and Category.
 *
 *
 * Inputs: none
 * Output: none
 */
void Person::calcSkills()
{

}

/*
 * Description: Removes an ailment of a given type
 *
 * Inputs: Infliction - ailment type to be removed from the Person
 * Output: bool - TRUE if the ailment was removed successfully
 */
const bool Person::removeAilment(Infliction r_ailment)
{
  for (int i = 0; i < ailments.size(); i++)
  {
      if (ailments[i].getType() == r_ailment)
    {
      ailments.remove(i);
      return TRUE;
    }
  }
  return FALSE;
}

/*
 * Description: Removes an ailment at a given index
 *
 * Inputs: int - index of ailment to be removed
 * Output: none
 */
void Person::removeAilment(int index)
{
  if (index > 0 && index < ailments.size())
    ailments.remove(index);
}

/*
 * Description: Clears all the ailments.
 *
 * Inputs: none
 * Output: none
 */
void Person::clearAll()
{
  ailments.clear();
}

/*
 * Description: Sets up the base stats (grabs Category min and max values
 *              and adds them to the Race bonuses)
 *
 * Inputs: none
 * Output: none
 */
void Person::setUpBaseStats()
{
  for (int i = 0; i < base_stats.getSize(); i++)
  {
    base_stats.setStat(i, cat->getAttrSet().getStat(i) +
                       race->getAttrSet().getStat(i));
    base_stats.setMax(i, cat->getAttrSet().getMax(i) +
                      race->getAttrSet().getMax(i));
  }
}

/*
 * Description: Uses an item
 *
 * Inputs: used_item - pointer of an item to be used
 * Output: bool - tue if item used successfully
 */
const bool Person::useItem(Item* used_item)
{
    return TRUE;
}

/*
 * Description: Evaluates a given person state flag
 *
 * Inputs: Person state flag
 * Output: Boolean evaluation of the flag
 */
const bool Person::getPersonFlag(PersonState flags)
{
  return state_set.testFlag(flags);
}

/*
 * Description: Sets a person state flag to a given boolean value
 *
 * Inputs: PersonState flag
 *         Boolean value to set the flag to
 * Output: none
 */
void Person::setPersonFlag(PersonState flags, const bool set_value)
{
  (set_value) ? (state_set |= flags) : (state_set ^= flags);
}

/*
 * Description: Returns the vector of ailments of a person.
 *
 * Inputs: None
 * Output: QVector<Ailment> - vector of ailments a person has
 */
QVector<Ailment> Person::getAilments()
{
  return ailments;
}

/*
 * Description: Returns an Ailment by type of a person.
 *
 * Inputs: Infliction - type of the Ailment to be checked for
 * Output: Ailment - ailment of the given type
 */
Ailment Person::getAilment(Infliction type)
{
  for (int i = 0; i < ailments.size(); i++)
    if (ailments[i].getType() == type)
      return ailments.at(i);
  return ailments.at(0);
}

/*
 * Description: Returns an Ailment at a given index
 *
 * Inputs: int - index of ailment to be checked
 * Output: Ailment - ailment at the given index
 */
Ailment Person::getAilByIndex(int index)
{
  return ailments.at(index);
}

/*
 * Description: Returns a pointer to the category of the Person.
 *
 * Inputs: none
 * Output: Category* - pointer to the category
 */
Category* Person::getCategory()
{
  return cat;
}

/*
 * Description: Returns the vector of equipment for the person.
 *
 * Inputs: None
 * Output: QVector<Equipment*> - the equipment of the person
 */
QVector<Equipment*> Person::getEquipment()
{
  return equipment;
}

/*
 * Description: Returns a pointer to an Equipment at a given index
 *
 * Inputs: int - index of the equipment to be checked
 * Output: Equipment* - pointer at the given index
 */
Equipment* Person::getEquipSlot(int index)
{
  return equipment.at(index);
}

/*
 * Description: Returns the pointer to an Equipment at a given slot
 *
 * Inputs: QString - name of the slot to check
 * Output: Equipment* - pointer at the given slot
 */
Equipment* Person::getEquipSlot(QString slot_name)
{
  if (slot_name == "HEAD")     return getEquipSlot(0);
  if (slot_name == "LEFTARM")  return getEquipSlot(1);
  if (slot_name == "RIGHTARM") return getEquipSlot(2);
  if (slot_name == "TORSO")    return getEquipSlot(3);
  if (slot_name == "LEGS")     return getEquipSlot(4);
  return NULL;
}

/*
 * Description: Returns the pointer to the Person's Race
 *
 * Inputs: none
 * Output: Race* - pointer to the person's Race
 */
Race* Person::getRace()
{
  return race;
}

/*
 * Description: Returns the set of skills describing a person.
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the Skills of the person
 */
SkillSet* Person::getSkills()
{
  return skills;
}

/*
 * Description: Returns the experience of the person
 *
 * Inputs: none
 * Output: uint - experience of the person
 */
uint Person::getExp()
{
  return experience;
}

/*
 * Description: Returns the experience rewarded on battle victory
 *
 * Inputs: none
 * Output: uint - experience reward
 */
uint Person::getExpLoot()
{
  return experience_drop;
}

/*
 * Description: Returns the credits dropped on battle victory
 *
 * Inputs: none
 * Output: uint - credits dropped on battle victory
 */
uint Person::getCreditLoot()
{
  return credit_drop;
}

/*
 * Description: Returns the item loot of the person
 *
 * Inputs: none
 * Output: QVector<Item> - vector of items containing loot
 */
QVector<Item> Person::getItemLoot()
{
  return item_drops;
}

/*
 * Description: Returns the level of the Person
 *
 * Inputs: AttributeSet - new attribute set to be assigned
 * Output: none
 */
uint Person::getLevel()
{
  return level;
}

/*
 * Description: Returns the name of the Person
 *
 * Inputs: none
 * Output: QString - name of the Person
 */
QString Person::getName()
{
  return name;
}

/*
 * Description: Returns the primary elemental bonus
 *
 * Inputs: none
 * Output: QString - the primary elemental bonus
 */
QString Person::getPrimary()
{
  return primary;
}

/*
 * Description: Returns the secondary elemental bonus
 *
 * Inputs: none
 * Output: QString - the secondary elemental bonus
 */
QString Person::getSecondary()
{
  return secondary;
}

/*
 * Description: Returns a QChar of the Primary Curve
 *
 * Inputs: none
 * Output: QChar - curve of the primary element
 */
QChar Person::getPrimaryCurve()
{
  return primary_curve;
}

/*
 * Description: Returns a QChar of the secondary_curve
 *
 * Inputs: none
 * Output: QChar - curve of the secondary element
 */
QChar Person::getSecondaryCurve()
{
  return secondary_curve;
}

/*
 * Description: Returns the rank string
 *
 * Inputs: none
 * Output: QString - rank of the Person
 */
QString Person::getRank()
{
  return rank;
}

/*
 * Description: Returns a pointer to the third person sprite
 *
 * Inputs: none
 * Output: Sprite* - pointer to the third peron sprite
 */
Sprite* Person::getThirdPerson()
{
  return third_person;
}

/*
 * Description: Returns a pointer to the first person sprite
 *
 * Inputs: none
 * Output: Sprite* - pointer to the first person sprite
 */
Sprite* Person::getFirstPerson()
{
  return first_person;
}

/*
 * Description: Returns the base AttributeSet
 *
 * Inputs: none
 * Output: AttributeSet - the base stat AttributeSet
 */
AttributeSet* Person::baseStats()
{
  return &base_stats;
}

/*
 * Description: Returns the standard AttributeSet
 *
 * Inputs: none
 * Output: AttributeSet - the standard stat AttributeSet
 */
AttributeSet* Person::getStats()
{
  return &stats;
}

/*
 * Description: Returns the temporary AttributeSet
 *
 * Inputs: none
 * Output: AttributeSet - the temporary AttributeSet
 */
AttributeSet* Person::tempStats()
{
  return &temp_stats;
}

/*
 * Description: Equips a given item to a given slot.
 *
 * Inputs: QString - slot to equip the item to.
 *         Equipment* - pointer to the equipment to attach
 * Output: bool - TRUE if the equip took place
 */
const bool Person::setEquipment(QString slot, Equipment* e)
{
  if (e == NULL)
    return FALSE;
  if (slot == "TWOHAND" && e->getEquipmentFlag(Equipment::TWOHAND))
  {
    if (getEquipSlot("LEFTARM") == NULL && getEquipSlot("RIGHTARM") == NULL)
    {
      equipment[1] = e;
      equipment[2] = e;
      return TRUE;
    }
  }
  if (slot == "HEAD" && e->getEquipmentFlag(Equipment::HEAD))
  {
    if (getEquipSlot("HEAD") == NULL && getEquipSlot("HEAD") == NULL)
    {
      equipment[0] = e;
      return TRUE;
    }
  }
  if (slot == "LEFTARM" && e->getEquipmentFlag(Equipment::LEFTARM))
  {
    if (getEquipSlot("LEFTARM") == NULL)
    {
      equipment[1] = e;
      return TRUE;
    }
  }
  if (slot == "RIGHTARM" && e->getEquipmentFlag(Equipment::RIGHTARM))
  {
    if (getEquipSlot("RIGHTARM") == NULL)
    {
      equipment[2] = e;
      return TRUE;
    }
  }
  if (slot == "TORSO" && e->getEquipmentFlag(Equipment::TORSO))
  {
    if (getEquipSlot("TORSO") == NULL)
    {
      equipment[3] = e;
      return TRUE;
    }
  }
  if (slot == "LEGS" && e->getEquipmentFlag(Equipment::LEGS))
  {
    if (getEquipSlot("LEGS") == NULL)
    {
      equipment[4] = e;
      return TRUE;
    }
  }
  return FALSE;
}

/*
 * Description: Assigns a new Category to the person
 *
 * Inputs: Category* - new category for the Person
 * Output: none
 */
void Person::setCategory(Category* c)
{
  cat = c;
}

/*
 * Description: Sets the experience of the person
 * Note: Does NOT level the character, use addExp(int) for that.
 *       Use (setLevel()) to adjust the level accordingly!
 *
 * Inputs: uint - value of experience to be set
 * Output: none
 */
void Person::setExp(uint value)
{
  (value < kMAX_EXPERIENCE) ? (experience = value) :
                              (experience = kMAX_EXPERIENCE);
}

/*
 * Description: Sets the experience loot of the person
 *
 * Inputs: uint - value of experience loot to be set
 * Output: none
 */
void Person::setExpLoot(uint value)
{
  (value < kMAX_EXP_DROP) ? (experience_drop = value) :
                            (experience_drop = kMAX_EXP_DROP);
}

/*
 * Description: Sets the credit loot of a person
 *
 * Inputs: uint - value of credit loot to be set
 * Output: none
 */
void Person::setCreditLoot(uint value)
{
    (value < kMAX_CREDIT_DROP) ? (credit_drop = value) :
                                 (credit_drop = kMAX_CREDIT_DROP);
}

/*
 * Description: Sets the item drops of the person
 *
 * Inputs: QVector<Item> - vector of items the loot is to be set to.
 * Output: none
 */
void Person::setItemLoot(QVector<Item> items)
{
    item_drops.clear();
    for (int i = 0; i < items.size(); i++)
        item_drops.push_back(items[i]);
}

const bool Person::setLevel(const uint &new_level)
{
  level = new_level;
  // TODO: Finish level-up function
}

/*
 * Description: Assigns a new name
 *
 * Inputs: QString - new name
 * Output: none
 */
void Person::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the rank of the person
 *
 * Inputs: QString - string representing a new rank
 * Output: none
 */
void Person::setRank(QString new_rank)
{
  rank = new_rank;
}

/*
 * Description: Sets the race pointer of the Person
 *
 * Inputs: Race* - pointer to a new race value for the Person
 * Output: none
 */
void Person::setRace(Race* new_race)
{
  race = new_race;
}

/*
 * Description: Sets the person's primary elemental strength and curve
 *              Template "<2 Letter Element Code><Curve(S,A-D)>"
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setPrimary(QString value)
{
  setPrimaryCurve(value.at(value.length() - 1));
  value.chop(1);
  primary = value;

}

/*
 * Description: Sets the person's primary curve (also set from setPrimary())
 *
 * Inputs: QChar - character of the curve
 * Output: none
 */
void Person::setPrimaryCurve(QChar value)
{
  primary_curve = value;
}

/*
 * Description: Sets the person's secondary elemental strength and curve
 *              Template "<Element><Curve(A-F)>"
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setSecondary(QString value)
{
  setSecondaryCurve(value.at(value.length() - 1));
  value.chop(1);
  secondary = value;
}

/*
 * Description: Sets the person's secd curve (also set from setSecondary())
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setSecondaryCurve(QChar value)
{
  secondary_curve = value;
}

/*
 * Description: Sets the first person view sprite for the person
 *
 * Inputs: Sprite* - pointer to the sprite to be set
 * Output: none
 */
void Person::setFirstPerson(Sprite* s)
{
  first_person = s;
}

/*
 * Description: Sets the third person view sprite for the person
 *
 * Inputs: Sprite* - pointer to the sprite to be set
 * Output: none
 */
void Person::setThirdPerson(Sprite* s)
{
  third_person = s;
}

/*
 * Description: Assigns a new base stat set.
 *
 * Inputs: AttributeSet - new attribute set to be assigned
 * Output: none
 */
void Person::setBaseStats(AttributeSet new_stat_set)
{
  base_stats = new_stat_set;
}

/*
 * Description: Assigns a new standard stat set
 *
 * Inputs: AttributeSet - new attribute set to be assigned
 * Output: none
 */
void Person::setStats(AttributeSet new_stat_set)
{
  stats = new_stat_set;
}

/*
 * Description: Assigns a new temp stat set.
 *
 * Inputs: AttributeSet - new attribute set to be assigned
 * Output: none
 */
void Person::setTempStats(AttributeSet new_stat_set)
{
  temp_stats = new_stat_set;
}
