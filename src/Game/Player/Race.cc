/******************************************************************************
* Class Name: Race
* Date Created: October 28th, 2012
*               Rewritten - March 5th, 2013
*               Refactored - August 11th, 2013
*
* Inheritance: None
* Description: Header for Race that defines the race for the particular person.
*              Some examples includes Bears, Humans, etc. The Race class will
*              provide some unique Actions for persons, as well as provide
*              bonuses to the category stats when making up a person.
******************************************************************************/
#include "Game/Player/Race.h"

/*============================================================================
 * CONSTANTS
 *===========================================================================*/
const uint Race::kMIN_VITA =    0;
const uint Race::kMIN_QTDR =    0;
const uint Race::kMIN_PHYS =    0;
const uint Race::kMIN_ELMT =    0;
const uint Race::kMIN_SPEC =    0;
const uint Race::kMAX_VITA = 5000;
const uint Race::kMAX_QTDR =  500;
const uint Race::kMAX_PHYS =  700;
const uint Race::kMAX_ELMT =  550;
const uint Race::kMAX_SPEC =  200;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Race::Race()
{
  loadDefaults();
}

/*
 * Description:
 *
 * Inputs:
 */
Race::Race(const Race &source, QWidget* parent)
    : QWidget(parent)
{
  copySelf(source);
}

/*
 * Description: Default Race object constructor. Constructs a Race with
 *              a default set of attributes.
 *
 * Inputs: QString - name of the Race
 */
Race::Race(QString name)
{
  loadDefaults();
  setName(name);
}

/*
 * Description: Constructor object. Constructs a Race given a string and
 *             a full set of base stat values. None of the Race constructors
 *             set the description of the Race.
 *
 * Inputs: QString - name of the Race
 *         AttributeSet - statistic set to add to the Race
 */
Race::Race(QString name, QString denonym, QString desc, AttributeSet base_set,
           AttributeSet max_set, SkillSet *skill_set)
    : description(desc),
      denonym(denonym),
      name(name),
      base_stat_set(base_set),
      max_stat_set(max_set),
      skill_set(skill_set)
{}

/*
 * Description: Destroys a Race object
 */
Race::~Race() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deep copy constructor for Race.
 *
 * Inputs: source - const reference for Race object to be copied
 * Output: none
 */
void Race::copySelf(const Race &source)
{
  description = source.description;
  denonym = source.denonym;
  name = source.name;
  base_stat_set = source.base_stat_set;
  max_stat_set = source.max_stat_set;
  skill_set = source.skill_set;
  immunities = source.immunities;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Race::loadDefaults()
{
  /* Default string params */
  setDenonym("");
  setDescription("");
  setName("");

  /* Set up basic min and max Attribute Sets */
  setAttrSet(buildMinSet());
  setMaxSet(buildMaxSet());

  /* No skill set */
  setSkillSet(0);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AttributeSet Race::buildMinSet()
{
  QList<uint> min_values;

  min_values << kMIN_VITA << kMIN_QTDR << kMIN_PHYS << kMIN_PHYS
             << kMIN_ELMT << kMIN_ELMT << kMIN_ELMT << kMIN_ELMT
             << kMIN_ELMT << kMIN_ELMT << kMIN_ELMT << kMIN_ELMT
             << kMIN_ELMT << kMIN_ELMT << kMIN_ELMT << kMIN_ELMT
             << kMIN_SPEC << kMIN_SPEC << kMIN_SPEC;

  return AttributeSet(min_values);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AttributeSet Race::buildMaxSet()
{
  QList<uint> max_values;

  max_values << kMAX_VITA << kMAX_QTDR << kMAX_PHYS << kMAX_PHYS
             << kMAX_ELMT << kMAX_ELMT << kMAX_ELMT << kMAX_ELMT
             << kMAX_ELMT << kMAX_ELMT << kMAX_ELMT << kMAX_ELMT
             << kMAX_ELMT << kMAX_ELMT << kMAX_ELMT << kMAX_ELMT
             << kMAX_SPEC << kMAX_SPEC << kMAX_SPEC;

  return AttributeSet(max_values);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds an immunity to the list of immunities.
 *
 * Inputs: EnumDb::Inflictio - the type of Immunity to be added
 * Output: none
 */
void Race::addImmunity(EnumDb::Infliction new_immunity)
{
  if (!isImmune(new_immunity))
    immunities.append(new_immunity);
}

/*
 * Description: Cleans up the skill vector by calling its clean up function,
 *              and also ensures all the maximum attribute values are within
 *              the limits specified by the consts
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Race::cleanUp()
{
  /* Clean up the base and maximum attribute sets */
  AttributeSet temp_min = buildMinSet();
  AttributeSet temp_max = buildMaxSet();

  for (int i = 0; i < temp_min.getSize(); i++)
  {
    if (getBaseSet().getStat(i) < temp_min.getStat(i))
      getBaseSet().setStat(i, temp_min.getStat(i));

    if (getMaxSet().getStat(i) < temp_max.getStat(i))
      getMaxSet().setStat(i, temp_max.getStat(i));
  }

  /* Clean up the skill set, if one is set */
  if (getSkillSet() != 0)
    getSkillSet()->cleanUp();
}

/*
 * Description: Evaluates whether the race is immune to a given Infliction
 *
 * Inputs: EnumDb::Infliction - the immunity to be checked for
 * Output: bool - true if the category is immune to the infliction
 */
bool Race::isImmune(EnumDb::Infliction check_immunity)
{
  QList<EnumDb::Infliction>::iterator it;
  for (it = immunities.begin(); it < immunities.end(); ++it)
    if ((*it) == check_immunity)
      return true;
  return false;
}

/*
 * Description: Removes an immunity for the list if it is found.
 *
 * Inputs: EnumDb::Infliction - the immunity to be removed.
 * Output: none
 */
void Race::removeImmunity(EnumDb::Infliction remove_immunity)
{
 QList<EnumDb::Infliction>::iterator it;
 for (it = immunities.begin(); it < immunities.end(); ++it)
   if ((*it) == remove_immunity)
     immunities.erase(it);
}

/*
 * Description: Prints out the info of the Race
 *
 * Inputs: none
 * Output: none
 */
void Race::printInfo()
{
  qDebug() << "Name: " << name;
  qDebug() << "Denonym: " << denonym;
  qDebug() << "Description: " << description;
  //qDebug() << "Base Set: " << base_stat_set.printInfo();
  //qDebug() << "Max Set " << max_stat_set.printInfo();

  if (getSkillSet() != 0)
    qDebug() << "Sills: " << getSkillSet()->getSkills().size();
}

/*
 * Description: Returns the description of the Race
 *
 * Inputs: none
 * Output: QString - description of the Race
 */
QString Race::getDescription()
{
  return description;
}

/*
 * Description: Returns the denonym of the Race
 *
 * Inputs: none
 * Output: QString - denonym of the Race
 */
QString Race::getDenonym()
{
  return denonym;
}

/*
 * Description: Returns the name of the Race
 *
 * Inputs: none
 * Output: QString - name of the Race
 */
QString Race::getName()
{
  return name;
}

/*
 * Description: Returns the set of attributes
 *
 * Inputs: none
 * Output: AttributeSet - attribute set of the Race
 */
AttributeSet Race::getBaseSet()
{
  return base_stat_set;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AttributeSet Race::getMaxSet()
{
  return max_stat_set;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
SkillSet* Race::getSkillSet()
{
  return skill_set;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QList<EnumDb::Infliction> Race::getImmunities()
{
  return immunities;
}

/*
 * Description: Set the description of the Race
 *
 * Inputs: QString - string of the Race description
 * Output: none
 */
void Race::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the denonym of the Race
 *
 * Inputs: QString - new race denonym string
 * Output: none
 */
void Race::setDenonym(QString new_denonym)
{
  denonym = new_denonym;
}

/*
 * Description: Sets the name of the Race
 *
 * Inputs: QString - new name to be set
 * Output: none
 */
void Race::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the AttributeSet of the Race and calls clean up
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Race::setAttrSet(AttributeSet new_stat_set)
{
  base_stat_set = new_stat_set;
}

/*
 * Description: Sets the AttributeSet of the Race and calls clean up
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Race::setMaxSet(AttributeSet new_stat_set)
{
  max_stat_set = new_stat_set;
}

/*
 * Description: Sets the SkillSet* and calls clean up
 *
 * Inputs: SkillSet* - pointer to a new skill set
 * Output: none
 */
void Race::setSkillSet(SkillSet* new_skill_set)
{
  skill_set = new_skill_set;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Overloaded = operator for Race class.
 *
 * Inputs: source - const reference of object to be copied
 * Output: Race& - reference to copied race object
 */
Race& Race::operator= (const Race &source)
{
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
