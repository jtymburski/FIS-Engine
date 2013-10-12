/*******************************************************************************
* Class Name: Category
* Date Created: October 28th, 2012
*               Rewritten - March 5th, 2013
*               Refactored - August 11th, 2013
*
* Inheritance: None
* Description: Header for the Category class. Every Person has a Category, which
*              is equivalent to their "Battle Class". This holds their starting
*              and ending values for attributes (In an attribute set), and a
*              set of skills which will become available to the person at
*              various levels.
*******************************************************************************/
#include "Game/Player/Category.h"

/*============================================================================
 * CONSTANTS
 *============================================================================*/
const uint Category::kMIN_VITA  =    30;
const uint Category::kMIN_QTDR  =     2;
const uint Category::kMIN_PHYS  =     5;
const uint Category::kMIN_ELMT  =     4;
const uint Category::kMIN_SPEC  =     1;
const uint Category::kMAX_VITA  = 50000;
const uint Category::kMAX_QTDR  =  5000;
const uint Category::kMAX_PHYS  =  7000;
const uint Category::kMAX_ELMT  =  5500;
const uint Category::kMAX_SPEC  =  2000;


/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
/*
 * Description: Default Category construction: loads the object with default
 *              values.
 *
 * Inputs: none
 */
Category::Category()
{
  loadDefaults();
}

/*
 * Description: Copy constructor for Category
 *
 * Inputs: Category& - reference of a Category to be copied
 */
Category::Category(const Category &source)
{
  copySelf(source);
}

/*
 * Description: Minimal category construction: construct a category object with
 *              default values except with a proper name.
 *
 * Inputs: QString - name of the category
 */
Category::Category(QString name)
{
  loadDefaults();
  setName(name);
}


/*
 * Description: Constructor object. Constructs a Category given a string and
 *             a full set of base stat values. None of the Category constructors
 *             set the description of the Category.
 *
 * Inputs: QString - name of the Category
 *         AttributeSet - statistic set to add to the category
 */
Category::Category(QString name, QString denonym, QString desc,
                   AttributeSet base, AttributeSet max, SkillSet* skills)
    : description(desc),
      denonym(denonym),
      name(name),
      base_stat_set(base),
      max_stat_set(max),
      skill_set(skills)
{}

/*
 * Description: Destroys a Category object
 */
Category::~Category() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Performs a deep copy of the Category.
 *
 * Inputs: source - const reference to object to be copied
 * Output: none
 */
void Category::copySelf(const Category &source)
{
  description   = source.description;
  denonym       = source.denonym;
  name          = source.name;
  base_stat_set = source.base_stat_set;
  max_stat_set  = source.max_stat_set;
  skill_set     = source.skill_set;
  immunities    = source.immunities;
}

/*
 * Description: This method loads default values into all of the categories
 *              data, including attribute sets. The default skill set is empty.
 *
 * Notes [1]: Static function
 *
 * Inputs: none
 * Output: none
 */
void Category::loadDefaults()
{
  /* Default string parameters */
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
 * Description: Method for loading an attribute set with min values based on
 *              the min constants defined in the class.
 *
 * Notes [1]: Static function
 *
 * Inputs: none
 * Output: AttributeSet - a minimum attribute set
 */
AttributeSet Category::buildMinSet()
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
 * Description: Method for loading an attribute set with max values based on
 *              the max constants defined in the class.
 *
 * Notes [1]: Static function
 *
 * Inputs: none
 * Output: AttributeSet - a maximum attribute set
 */
AttributeSet Category::buildMaxSet()
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
void Category::addImmunity(EnumDb::Infliction new_immunity)
{
  if (!isImmune(new_immunity))
    immunities.append(new_immunity);
}

/*
 * Description: Cleans up the categories skill set, attribute sets.
 *
 * Inputs: none
 * Output: none
 */
void Category::cleanUp()
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
 * Description: Evaluates whether the category is immune to a given Infliction
 *
 * Inputs: EnumDb::Infliction - the immunity to be checked for
 * Output: bool - true if the category is immune to the infliction
 */
bool Category::isImmune(EnumDb::Infliction check_immunity)
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
void Category::removeImmunity(EnumDb::Infliction remove_immunity)
{
 QList<EnumDb::Infliction>::iterator it;
 for (it = immunities.begin(); it < immunities.end(); ++it)
   if ((*it) == remove_immunity)
     immunities.erase(it);
}


/*
 * Description: Prints out all the information pertaining to the category
 *
 * Inputs: none
 * Output: none
 */
void Category::printInfo()
{
  qDebug() << " --- Category  --- ";
  qDebug() << "Name: " << name;
  qDebug() << "Denonym: " << denonym;
  qDebug() << "Description: " << description;
  //qDebug() << "Base Set: " << base_stat_set.printInfo();
  //qDebug() << "Max Set " << max_stat_set.printInfo();

  if (getSkillSet() != 0)
    qDebug() << "Sills: " << getSkillSet()->getSkills().size();

  qDebug() << " --- / Category --- ";
}

/*
 * Description: Returns the description of the Category
 *
 * Inputs: none
 * Output: QString - description of the Category
 */
QString Category::getDescription()
{
  return description;
}

/*
 * Description: Returns the denonym of the Category
 *
 * Inputs: none
 * Output: QString - denonym of the category
 */
QString Category::getDenonym()
{
  return denonym;
}

/*
 * Description: Returns the name of the Category
 *
 * Inputs: none
 * Output: QString - name of the Category
 */
QString Category::getName()
{
  return name;
}

/*
 * Description: Returns the set of attributes
 *
 * Inputs: none
 * Output: AttributeSet - attribute set of the Category
 */
AttributeSet Category::getBaseSet()
{
  return base_stat_set;
}

/*
 * Description: Returns the maximum stat values of the category
 *
 * Inputs: none
 * Output: AttributeSet - attribute set of the Category
 */
AttributeSet Category::getMaxSet()
{
  return max_stat_set;
}

/*
 * Description: Returns the SkillSet*
 *
 * Inputs: none
 * Output: SkillSet* - the skills of the Category
 */
SkillSet* Category::getSkillSet()
{
  return skill_set;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QList<EnumDb::Infliction> Category::getImmunities()
{
  return immunities;
}

/*
 * Description: Assigns a new description to the Category.
 *
 * Inputs: QString - string of the Category description
 * Output: none
 */
void Category::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Assigns a new denonym to the category.
 *
 * Inputs: QString - string of the Category denonym
 * Output: none
 */
void Category::setDenonym(QString new_denonym)
{
  denonym = new_denonym;
}

/*
 * Description: Sets the name of the Category
 *
 * Inputs: QString - new name to be set
 * Output: none
 */
void Category::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the AttributeSet of the Category
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Category::setAttrSet(AttributeSet new_base_set)
{
  base_stat_set = new_base_set;
}

/*
 * Description: Assigns a new maximum stat set to the Category
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Category::setMaxSet(AttributeSet new_max_set)
{
  max_stat_set = new_max_set;
}

/*
 * Description: Sets the SkillSet of the Category
 *
 * Inputs: SkillSet* - pointer to the new skill set
 * Output: none
 */
void Category::setSkillSet(SkillSet* new_skill_set)
{
  skill_set = new_skill_set;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Overloaded = operator for Category class. Checks for self
 *              assignment and calls the deep copy for the category.
 *
 * Inputs: SkillSet* - pointer to the new skill set
 * Output: none
 */
Category& Category::operator= (const Category &source)
{
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
