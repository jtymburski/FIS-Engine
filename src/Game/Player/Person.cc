/******************************************************************************
* Class Name: Person Implementation
* Date Created: December 2nd, 2012 (Rewritten March 6th, 2013)
* Inheritance: None
* Description: Head for Person class. Container for all the info describing
*              a character in the game, including their statistics for battle,
*              flags, etc.
* // TODO: Race and category equip item types [03-06-13]
* // TODO: Finish use item
* // TODO: Setup Functions
* // TODO: Skill functions?? [06-23-13]
******************************************************************************/
#include "Game/Player/Person.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
QVector<int> Person::exp_table;

const uint Person::kMAX_LEVEL             =        127;
const uint Person::kMIN_LVL_EXP           =        156;
const uint Person::kMAX_LVL_EXP           =    5327426;
const uint Person::kMAX_EXPERIENCE        = 1000000000; /* Billion */
const uint Person::kMAX_EXP_DROP          =    1000000; /* Million */
const uint Person::kMAX_CREDIT_DROP       =   10000000; /* Ten Million */
const uint Person::kMAX_EQUIP_SLOTS       =          5;
const uint Person::kMAX_ITEM_DROPS        =          5;
const double Person::kMAX_DAMAGE_MODIFIER =    10.0000;
const double Person::kPRIM_X_MODI = 1.250;
const double Person::kPRIM_S_MODI = 1.200;
const double Person::kPRIM_A_MODI = 1.500;
const double Person::kPRIM_B_MODI = 1.100;
const double Person::kPRIM_C_MODI = 1.050;
const double Person::kSECD_X_MODI = 1.150;
const double Person::kSECD_S_MODI = 1.120;
const double Person::kSECD_A_MODI = 1.090;
const double Person::kSECD_B_MODI = 1.060;
const double Person::kSECD_C_MODI = 1.030;
const int Person::kMINIMUM_ID =  0;
const int Person::kUNSET_ID   = -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* Description: Default constructor object
 *
 * Inputs: none
 */
Person::Person(QWidget* parent)
    : QWidget(parent)
{
  setParent(parent);
}

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
 */
Person::Person(QString pname, Race* prace, Category* pcat, QString p, QString s,
               QWidget* parent, int id)
    : QWidget(parent),
      damage_modifier(1.00),
      level(1),
      total_exp(0),
      id(id),
      experience_drop(0),
      credit_drop(0),
      cat(pcat),
      person_parent(0),
      race(prace),
      name(pname),
      base_skill_list(0),
      temp_skill_list(0),
      first_person(0),
      third_person(0),
      bubbified_sprite(0)
{
  setParent(parent);

  /* Setup the primary and secondary curves */
  setPrimary(p);
  setSecondary(s);

  /* Set up all three attribute sets */
  AttributeSet base_set = calcBaseLevelStats();

  setStats(base_set);
  setMax(base_set);
  setTemp(base_set);
  setMaxTemp(base_set);

  /* Setup a person record */
  person_record = new PersonRecord(EnumDb::NUBEAR);

  /* Construct the Exp Table */
  if (exp_table.isEmpty())
    exp_table = buildExponentialTable(kMIN_LVL_EXP, kMAX_LVL_EXP, kMAX_LEVEL);

  /* Initially gather all usable skills */
  calcSkills();
}

/*
 * Description: Copy constructor
 *
 * Notes [1]: The copy constructor does not assign the same flags as the
 *            copy, nor does it assign equipment to the person. It instead just
 *            creates a copy skeleton of the person.
 */
Person::Person(const Person &source, QWidget* parent)
    : QWidget(parent)
{
  setParent(parent);
  copySelf(source);
}

/*
 * Description: Annihilates a Person object
 */
Person::~Person()
{
  if (person_parent != 0)
  {
    for (int i = 0; i < item_drops.size(); i++)
    {
      delete item_drops[i];
      item_drops[i] = 0;
    }
  }

  delete temp_skill_list;
  temp_skill_list = 0;

  delete person_record;
  person_record = 0;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Method to calculate an AttributeSet representing the Person's
 *              level 1 Attribute values based upon their race's and category's
 *              Attribute amounts.
 *
 * Inputs: none
 * Output: AttributeSet representing level 1 values.
 */
AttributeSet Person::calcBaseLevelStats()
{
  AttributeSet cat_set = cat->getBaseSet();
  AttributeSet race_set = race->getBaseSet();
  AttributeSet base_level_set;

  for (int i = 0; i < base_stats.getSize(); i++)
    base_level_set.setStat(i, cat_set.getStat(i) + race_set.getStat(i));

  return base_level_set;
}

/*
 * Description: Method to calculate an AttributeSet representing the Person's
 *              final level values based upon their race's and category's final
 *              Attribute amounts.
 *
 * Inputs: none
 * Output: AttributeSet representing final level values
 */
AttributeSet Person::calcMaxLevelStats()
{
  AttributeSet cat_set = cat->getMaxSet();
  AttributeSet race_set = race->getMaxSet();
  AttributeSet max_level_set;

  for (int i = 0; i < base_stats.getSize(); i++)
    max_level_set.setStat(i, cat_set.getStat(i) + race_set.getStat(i));

  return max_level_set;
}

/*
 * Description: Deep copy method called by operator= and copy constructor.
 *              Constructs a deep copy of the Person based on the source
 *              Person
 *
 * Inputs: source - const reference to a Person
 * Output: none
 */
void Person::copySelf(const Person &source)
{
  damage_modifier = source.damage_modifier;
  level = source.level;
  total_exp = source.total_exp;
  experience_drop = source.experience_drop;
  credit_drop = source.credit_drop;
  cat = source.cat;
  race = source.race;
  name = source.name;
  first_person = source.first_person;
  third_person = source.third_person;
  bubbified_sprite = source.bubbified_sprite;
  base_skill_list = source.base_skill_list;
  temp_skill_list = source.temp_skill_list;
  primary = source.primary;
  primary_curve = source.primary_curve;
  secondary = source.secondary;
  secondary_curve = source.secondary_curve;

  /* Sets up the Attribute Sets */
  AttributeSet base_set = calcBaseLevelStats();

  setStats(base_set);
  setMax(base_set);
  setTemp(base_set);
  setMaxTemp(base_set);

  /* Setup a new Person Record */
  person_record = new PersonRecord(*(source.person_record));

  /* Deep copy the item drops of the person */
  //for (int i = 0; i < source.item_drops.size(); i++)
  //  item_drops.push_back(new Item(source.item_drops.value(i)));
  //TODO: Item drop construction? [11-11-13]

  /* Initially calculates all the skills */
  calcSkills();
}

/*
 * Description: Assigns a new value to ID of the Person. If the ID is in range,
 *              the id is set and true is returned. Otherwise, the ID set to the
 *              minimum storage value (OOR).
 *
 *
 * Inputs: int new_id - the new integer ID to define the person.
 * Output: bool - status if the new_id is within the allowable range.
 */
 bool Person::setID(int new_id)
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

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds an amount to the experience of the person and calls
 *              setLevel() if necessary to deal with level ups accordingly
 *              Will deal with multiple level ups. 
 *
 * Inputs: value - amount of experience to be added
 * Output: none
 */
void Person::addExperience(uint value)
{
  if (total_exp + value > kMAX_EXPERIENCE)
    total_exp = kMAX_EXPERIENCE;
  else
    total_exp += value;

  /* Level the character to the proper value (if necessary) */
  while (!getPersonFlag(Person::MAX_LVL) && total_exp >= getExpAt(getLevel()+1))
    setLevel(getLevel() + 1);
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
    temp_stats.setStat(i, current_stats.getStat(i));
    temp_max_stats.setStat(i, current_max_stats.getStat(i));
  }
}

/*
 * Description: This function calculates and updates the skills of the
 *              person, based on the skills allowed by their Race and Category,
 *              the equipment carried by them, and their personal base skill
 *              set.
 *
 * Inputs: none
 * Output: none
 */
void Person::calcSkills()
{
  /* Add the category skill sets, race skill sets, and base skill set */
  SkillSet* temp_person_skills = new SkillSet();
  SkillSet* race_set = getRace()->getSkillSet();
  SkillSet* cate_set = getCategory()->getSkillSet();

  if (base_skill_list !=0)
    temp_person_skills->addSkills(base_skill_list);

  if (race_set != 0)
    temp_skill_list->addSkills(race_set);

  if (cate_set != 0)
    temp_skill_list->addSkills(cate_set);

  /* Add the skills gained by the Person's equipment */
  for (int i = 0; i < (int)kMAX_EQUIP_SLOTS; i++)
  {
    // SkillSet* equip_skills = getEquipSlot(i)->getSkills();
    // temp_skill_list->addSkills(equip_skills);
  }

  temp_skill_list = temp_person_skills;
  delete temp_person_skills;
  temp_person_skills = 0;
}

/*
 * Description: Quick function for dealing damage to a Person with much nicer
 *              looking code lines.
 *
 * Inputs: ushort - amount of damage to do to the Person
 * Output: bool - returns true If the Person is now dead
 */
bool Person::damage(ushort amount)
{
  getTemp()->changeStat(0, -amount);
  if (getTemp()->getStat(0) == 0)
    return true;
  return false;
}

/*
 * Description: Method for printing all the info describing a person, by
 *              calling sub-print functions
 *
 * Inputs: none
 * Output: none
 */
void Person::printAll()
{
  qDebug() << " --- Person --- ";
  printBasics();
  printEquipment();
  printFlags();
  printSkills();
  qDebug() << " --- / Person --- ";
}

/*
 * Description: Method for printing the basic info of a person
 *
 * Inputs: none
 * Output: none
 */
void Person::printBasics()
{
  qDebug() << "Name: " << getName();
  qDebug() << "Level: " << getLevel();
  qDebug() << "Total Exp: " << getExp();
  qDebug() << "Category: " << cat->getName();
  qDebug() << "Race: " << race->getName();
  qDebug() << "Rank: " << getRankString(getPersonRecord()->getRank());
  qDebug() << "Dmg Modifier: " << damage_modifier;
  qDebug() << "Primary Element: " << primary;
  qDebug() << "Secondary Element: " << secondary;
  qDebug() << "Primary Curve: " << primary_curve;
  qDebug() << "Secondary Curve: " << secondary_curve;
  qDebug() << "Item Drops: " << getItemLoot().size();
  qDebug() << "Exp Drop: " << experience_drop;
  qDebug() << "Credit Drop: " << credit_drop;
}

/*
 * Description: Method for printing the basic info of a person
 *
 * Inputs: none
 * Output: none
 */
void Person::printEquipment()
{
  for (int i = 0; i < equipment.size(); i++)
    qDebug() << "Equipment #" << i << ": " << equipment[i]->getName();
}

/*
 * Description: Method for printing out the flags of the Person
 *
 * Inputs: none
 * Output: none
 */
void Person::printFlags()
{
  qDebug() << "ALIVE: " << getPersonFlag(Person::ALIVE);
  qDebug() << "INBATTLE: " << getPersonFlag(Person::IN_BATTLE);
  qDebug() << "SLEUTH: " << getPersonFlag(Person::SLEUTH);
  qDebug() << "BEARACKS: " << getPersonFlag(Person::BEARACKS);
  qDebug() << "RENDERING: " << getPersonFlag(Person::RENDERING);
  qDebug() << "MAINCHARACTER: " << getPersonFlag(Person::MAIN_CHARACTER);
  qDebug() << "FINALBOSS: " << getPersonFlag(Person::FINAL_BOSS);
  qDebug() << "BOSS: " << getPersonFlag(Person::BOSS);
  qDebug() << "MINIBOSS: " << getPersonFlag(Person::MINI_BOSS);
  qDebug() << "MAXLVLL " << getPersonFlag(Person::MAX_LVL);
  qDebug() << "CANATTACK: " << getPersonFlag(Person::ATTACK_ENABLED);
  qDebug() << "CANUSEITEM: " << getPersonFlag(Person::SKILL_ENABLED);
  qDebug() << "CANRUN: " << getPersonFlag(Person::RUN_ENABLED);
  qDebug() << "SKIPNEXTTURN: " << getPersonFlag(Person::SKIP_NEXT_TURN);
  qDebug() << "MISSNEXTTARGET: " << getPersonFlag(Person::MISS_NEXT_TARGET);
  qDebug() << "NOEFFECT: " << getPersonFlag(Person::NO_EFFECT);
  qDebug() << "ISBUBBY: " << getPersonFlag(Person::IS_BUBBY);
  qDebug() << "TWOSKILLS: " << getPersonFlag(Person::TWO_SKILLS);
  qDebug() << "THREESKILLS: " << getPersonFlag(Person::THREE_SKILLS);
  qDebug() << "REFLECT: " << getPersonFlag(Person::REFLECT);
}

/*
 * Description: Method for printing out the skills of a person
 *
 * Inputs: none
 * Output: none
 */
void Person::printSkills()
{
  base_skill_list->printInfo();
}

/*
 * Description: Evaluates a given person state flag
 *
 * Inputs: Person state flag
 * Output: Boolean evaluation of the flag
 */
bool Person::getPersonFlag(PersonState flags)
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
void Person::setPersonFlag(PersonState flags, bool set_value)
{
  (set_value) ? (state_set |= flags) : (state_set &= ~flags);
}

/*
 * Description: Returns the bubby sprite of the Person
 *
 * Inputs: none
 * Output: Sprite* - pointer to the bubby sprite of the person
 */
Sprite* Person::getBubbySprite()
{
  return bubbified_sprite;
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
 * Description: Returns the damage modifier of the Person
 *
 * Inputs: none
 * Output: double - the damage modifier of the Person
 */
double Person::getDmgMod()
{
  return damage_modifier;
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
  return equipment[index];
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
  return 0;
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
  return base_skill_list;
}

/*
 * Description: Returns the set of skills the Person can currently use, based
 *              on the amount of QD available and their enabled or disabled
 *
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the useable Skills of the person
 */
SkillSet* Person::getUseableSkills()
{
  calcSkills();

  QVector<Skill*> temp_skills = temp_skill_list->getSkills();
  QVector<ushort> temp_levels = temp_skill_list->getSkillLevels();

  /* Remove skills which have too high of QD cost */
  for (int i = 0; i < temp_skills.size(); i++)
  {
    if (temp_skills.at(i)->getQdValue() > (uint)temp_stats.getStat("QTDR"))
    {
      temp_skills.remove(i);
      temp_levels.remove(i);
    }
  }

  /* Remove physical skills if physical skill flag disabled */
  if (getPersonFlag(Person::ATTACK_ENABLED) == false)
  {
    for (int i = 0; i < temp_skills.size(); i++)
    {
      if (temp_skills.at(i)->getFlag(Skill::PHYSICAL))
      {
        temp_skills.remove(i);
        temp_levels.remove(i);
      }
    }
  }

  /* Remove non-physical skills if non-physical skill flag disabled */
  if (getPersonFlag(Person::SKILL_ENABLED) == false)
  {
    for (int i = 0; i < temp_skills.size(); i++)
    {
      if (!temp_skills.at(i)->getFlag(Skill::PHYSICAL))
      {
        temp_skills.remove(i);
        temp_levels.remove(i);
      }
    }
  }

  /* Use the wittled down levels and skill vectors to create a new skill list */
  temp_skill_list->addSkills(temp_skills, temp_levels);
  return temp_skill_list;
}

/*
 * Description: Returns the experience of the person
 *
 * Inputs: none
 * Output: uint - experience of the person
 */
uint Person::getExp()
{
  return total_exp;
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
QVector<Item*> Person::getItemLoot()
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
 * Description: Returns a pointer to the personal record of the person.
 *
 * Inputs: none
 * Output: PersonRecord* - pointer to a person record object
 */
PersonRecord* Person::getPersonRecord()
{
  return person_record;
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
 * Description:
 *
 * Inputs:
 * Output:
 */
AttributeSet* Person::getStats()
{
  return &current_stats;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AttributeSet* Person::getMax()
{
  return &current_max_stats;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AttributeSet* Person::getTemp()
{
  return &temp_stats;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AttributeSet* Person::getMaxTemp()
{
  return &temp_max_stats;
}

/*
 * Description: Returns the enumeration of a given QString
 *              representation of a PlayerRank enum.
 *
 * Inputs: QString - string of a PlayerRank enum
 * Output: EnumDb::PersonRanks - corresponding enumeration
 */
EnumDb::PersonRanks Person::getRankEnum(QString rank_string)
{
  const std::string &rank = rank_string.toUtf8().constData();
  EnumDb::PersonRanks person_rank = EnumDb::NUBEAR;
  EnumString<EnumDb::PersonRanks>::To(person_rank, rank);
  return person_rank;
}

/*
 * Description: Returns the QString registered to the enumerated
 *              PlayerRanks type.
 *
 * Inputs: EnumDb::PersonRanks - enumeration to find QString for
 * Output: QString - the QString of the enumeration
 */
QString Person::getRankString(EnumDb::PersonRanks person_rank)
{
  const std::string &rank_string
          = EnumString<EnumDb::PersonRanks>::From(person_rank);
  QString rank_qstring(rank_string.c_str());
  return rank_qstring;
}

/*
 * Description: Equips a given item to a given slot.
 *
 * Inputs: QString - slot to equip the item to.
 *         Equipment* - pointer to the equipment to attach
 * Output: bool - true if the equip took place
 */
bool Person::setEquipment(QString slot, Equipment* new_equipment)
{
  if (slot == "TWOHAND" && new_equipment->getEquipmentFlag(Equipment::TWOHAND))
  {
    if (getEquipSlot("LEFTARM") == 0 && getEquipSlot("RIGHTARM") == 0)
    {
      equipment[1] = new_equipment;
      equipment[2] = new_equipment;
      return true;
    }
  }
  if (slot == "HEAD" && new_equipment->getEquipmentFlag(Equipment::HEAD))
  {
    if (getEquipSlot("HEAD") == 0 && getEquipSlot("HEAD") == 0)
    {
      equipment[0] = new_equipment;
      return true;
    }
  }
  if (slot == "LEFTARM" && new_equipment->getEquipmentFlag(Equipment::LEFTARM))
  {
    if (getEquipSlot("LEFTARM") == 0)
    {
      equipment[1] = new_equipment;
      return true;
    }
  }
  if (slot == "RIGHTARM" && new_equipment->getEquipmentFlag(Equipment::RIGHTARM))
  {
    if (getEquipSlot("RIGHTARM") == 0)
    {
      equipment[2] = new_equipment;
      return true;
    }
  }
  if (slot == "TORSO" && new_equipment->getEquipmentFlag(Equipment::TORSO))
  {
    if (getEquipSlot("TORSO") == 0)
    {
      equipment[3] = new_equipment;
      return true;
    }
  }
  if (slot == "LEGS" && new_equipment->getEquipmentFlag(Equipment::LEGS))
  {
    if (getEquipSlot("LEGS") == 0)
    {
      equipment[4] = new_equipment;
      return true;
    }
  }
  return false;
}

/*
 * Description: Sets the bubby version sprite of the person
 *
 * Inputs: Sprite* - the sprite to be set for the bubby version of person
 * Output: none
 */
void Person::setBubbySprite(Sprite* new_sprite)
{
  bubbified_sprite = new_sprite;
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
 * Description: Assigns a new Category to the person
 *
 * Inputs: Category* - new category for the Person
 * Output: none
 */
void Person::setDmgMod(double value)
{
    (value < kMAX_DAMAGE_MODIFIER) ? (damage_modifier = value) :
                                     (damage_modifier = kMAX_DAMAGE_MODIFIER);
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
  (value < kMAX_EXPERIENCE) ? (total_exp = value) :
                              (total_exp = kMAX_EXPERIENCE);
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
 * Description: Sets the item drops of the person by creating new pointers.
 *
 *
 * Inputs: QVector<Item> - vector of items the loot is to be set to.
 * Output: none
 */
void Person::setItemLoot(QVector<Item*> items)
{
  for (int i = 0; i < item_drops.size(); i++)
  {
    delete item_drops.value(i);
      item_drops[i] = NULL;
  }
  item_drops.clear();

  //for (int i = 0; i < items.size() && (uint)i < kMAX_ITEM_DROPS; i++)
  //    item_drops.push_back(new Item(items.value(i)));
  //Item drop construction [11-11-13]
}

/*
 * Description: Sets the level of the Person. Will account for higher levels,
 *              and sets the MAXLVL flag if the Person has reached kMAX_LEVEL.
 *              This function will calculate their new stat attributes.
 *
 * Notes [1]: The difference between old_stat(i) and new_stat(i) is
 *            curr_stat(i) and temp_stat(i)
 *
 * Inputs: new_level - the level to change the person to.
 * Output: bool      - true if the level was changed
 */
bool Person::setLevel(const uint &new_level)
{
  /* Booleans if the person can level up, and if a level up actually occured */
  bool can_level = true;
  bool leveled_up = false;

  /* Level number checks */
  if (new_level == getLevel() || !getPersonFlag(Person::CAN_LEVEL_UP))
    can_level = false;

  if (can_level)
  {
    /* If the level is increasing, signal will be emitted */
    if (new_level > getLevel())
      leveled_up = true;

    /* Level will top out at max level if set level goes beyond */
    if (new_level < kMAX_LEVEL)
    {
      level = new_level;
    }
    else
    {
      level = kMAX_LEVEL;
      setPersonFlag(Person::MAX_LVL, true);
    }

    /* Recalculate the current base and max level stat values for the Person */
    AttributeSet base_stats = calcBaseLevelStats();
    AttributeSet max_stats = calcMaxLevelStats();

    /* Use the temp stats AttributeSet as a current */
    setTemp(*getMax());

    /* For each stat, build a distribution table and find the value */
    for (int i = 0; i < base_stats.getSize(); i++)
    {
        int base = base_stats.getStat(i);
        int max = max_stats.getStat(i);
        QString current = base_stats.getName(i);
        current.chop(2);

        if (getPrimary() == current)
        {
          if (getPrimaryCurve() == 'X') base = floor(base * kPRIM_X_MODI);
          if (getPrimaryCurve() == 'S') base = floor(base * kPRIM_S_MODI);
          if (getPrimaryCurve() == 'A') base = floor(base * kPRIM_A_MODI);
          if (getPrimaryCurve() == 'B') base = floor(base * kPRIM_B_MODI);
          if (getPrimaryCurve() == 'C') base = floor(base * kPRIM_C_MODI);
        }
        else if (getSecondary() == current)
        {
          if (getSecondaryCurve() == 'X') base = floor(base * kSECD_X_MODI);
          if (getSecondaryCurve() == 'S') base = floor(base * kSECD_S_MODI);
          if (getSecondaryCurve() == 'A') base = floor(base * kSECD_A_MODI);
          if (getSecondaryCurve() == 'B') base = floor(base * kSECD_B_MODI);
          if (getSecondaryCurve() == 'C') base = floor(base * kSECD_C_MODI);
        }

        int iterations = (int)kMAX_LEVEL;
        qDebug() << base << " " << max << " " << iterations;
        QVector<int> stat_table = buildExponentialTable(base, max, iterations);
        getMax()->setStat(i, stat_table.at(level));
    }

    /* Set the other Attribute sets to this set for now */
    setStats(*getMax());
    setTemp(*getMax());
    setMaxTemp(*getMax());
  }

  /* Emit the level up signal for the victory screen, etc. */
  if (leveled_up)
      emit levelUp();

  return can_level;
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
 * Description: Sets the person's skill set
 *
 * Inputs: SkillSet* - pointer to a new skill set to be assigned
 * Output: none
 */
void Person::setSkills(SkillSet* new_skill_set)
{
  base_skill_list = new_skill_set;
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
 * Description:
 *
 * Inputs:
 * Output:
 */
void Person::setStats(AttributeSet new_stat_set)
{
  current_stats = new_stat_set;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Person::setMax(AttributeSet new_stat_set)
{
  current_max_stats = new_stat_set;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Person::setTemp(AttributeSet new_stat_set)
{
  temp_stats = new_stat_set;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Person::setMaxTemp(AttributeSet new_stat_set)
{
  temp_max_stats = new_stat_set;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the value of the experience table at a given point
 *
 * Inputs: ushort - level to look up on the table
 * Output: uint   - total experience required for given level
 */
uint Person::getExpAt(ushort level)
{
  if (level < exp_table.size())
    return exp_table.at(level);
  return 0;
}

/*
 * Description: Returns the value of the maximum level constant
 *
 * Inputs: none
 * Output: uint - the value of the maximum level constant
 */
uint Person::getMaxLevel()
{
  return kMAX_LEVEL;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Overloaded operator = function for deep copy of Person. Calls
 *              the copySelf function for copying after a check for self-assign
 *
 * Inputs: source - const reference of object to be copied
 * Output: person - reference
 */
Person& Person::operator= (const Person &source)
{
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
