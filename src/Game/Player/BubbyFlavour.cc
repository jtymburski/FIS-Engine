/*******************************************************************************
* Class Name: BubbyFlavour
* Date Created: March 6th, 2013
* Inheritance: None
* Description: Describes the flavour of a Bubby. Every Bubby of certain flavours
*              will have the same SkillSet and AttributeSet as others.
*******************************************************************************/
#include "Game/Player/BubbyFlavour.h"

/*============================================================================
 * CONSTANTS
 *===========================================================================*/
const ushort BubbyFlavour::kTIER_CAP         = 3; /* Max # of Bubby Tiers */
QVector<QString> BubbyFlavour::flavour_list; /* Vector of Flavour names */
QVector<ushort> BubbyFlavour::kTIER_LEVELS;  /* Vector of Skill levels */

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS [PRIVATE CONSTRUCTOR]
 *===========================================================================*/

/*
 * Description: Constructs a BubbyFlavour object (representing a type of bubby)
 *              given a set of Attributes that the bubby will increase, as well
 *              as a set of skills it may allow the user
 * 
 * Note[1]: The purpose of the flavour_parent is to easily instantiate similar
 *          flavours. Flavours with parents will utilize the same SkillSet,
 *          and AttributeSet, but must have different names and may have 
 *          different descriptions, sprites.
 * 
 * Inputs: QString - name to call the new Bubby type
 *         AttributeSet - set of attributes the Bubby will power an equipment by
 *         SkillSet* - pointer to a set of skills the Bubby allows
 */
BubbyFlavour::BubbyFlavour(BubbyFlavour* flavour_parent, QString flavour_name,
                           AttributeSet* stat_set, SkillSet* skills)
  : parent(flavour_parent)
{
  setAttr(stat_set);
  setSkillSet(skills);
  setName(flavour_name);

  /* Build the skill levels per tier if not built */
  if (kTIER_LEVELS.isEmpty())
    buildLevels();

  /* If the Flavour has a parent, assign the parent's parameters to this */
  if (getParent() != 0)
  {
    setFlavourFlag(BubbyFlavour::CREATED_WITH_PARENT, true);
  }
  else
  {
    setFlavourFlag(BubbyFlavour::CREATED_WITH_PARENT, false);
    setDescription("");
    for (ushort i = 0; i < kTIER_CAP; i++)
      sprites.push_back(0);
  }

  /* Check to add the flavour to the static list */
  assert(addFlavour(name));

  /* Instantiate it with empty sprites */

}

/*
 * Description: Annihilates a Bubby object
 */
BubbyFlavour::~BubbyFlavour() {}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds a flavour to the vector.
 *
 * Inputs: QString new_flavour - new flavour to add to the vector.
 * Output: bool - true if the flavour was added.
 */
bool BubbyFlavour::addFlavour(QString new_flavour)
{
  if (isFlavour(new_flavour) + 1)
    return false;
  else
    flavour_list.push_back(new_flavour);
  return true;
}

/*
 * Description: Builds the levels vector for the skills useable at certain
 *              levels.
 *
 * Inputs: none
 * Output: none
 */
void BubbyFlavour::buildLevels()
{
  kTIER_LEVELS.push_back(0);
  kTIER_LEVELS.push_back(30);
  kTIER_LEVELS.push_back(50);
  kTIER_LEVELS.push_back(127);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: This function is the Bubby factory, creates a new Bubby
 *              given a pointer to this (a flavour).
 *
 * Inputs: none
 * Output: Bubby object created in the likeness of this flavour.
 */
//Bubby* BubbyFlavour::createBubby()
//{
  //return new Bubby(*this);
//}

/*
 * Description: Prints the state of the class
 *
 * Inputs: none
 * Output: none
 */
void BubbyFlavour::printInfo()
{
  qDebug() << " --- BubbyFlavour -- ";
  qDebug() << "Name: " << name;
  qDebug() << "Description: " << description;
  qDebug() << "# Skills: " << skill_list->getSkills().size();
  qDebug() << "Mass: " << mass;
  qDebug() << " --- / BubbyFlavour ---";
}

/*
 * Description: Evaluates a given FlavourFlag set
 *
 * Inputs: FlavourFlag set
 * Output: Boolean evaluation of the flag
 */
bool BubbyFlavour::getFlavourFlag(FlavourState flags)
{
  return flag_set.testFlag(flags);
}

/*
 * Description: Sets a BubbyFlavour flag with a given value
 *
 * Inputs: BubbyFlavour flags - enumerated FlavourFlag object 
 *         set_value - value to set the flavour flag to.
 * Output: none
 */
void BubbyFlavour::setFlavourFlag(FlavourState flags, bool set_value)
{
  (set_value) ? (flag_set |= flags) : (flag_set &= flags);
}

/*
 * Description: Returns the pointer to the attribute set of the BubbyFlavour.
 *
 * Inputs: none
 * Output: AttributeSet* - pointer to the attribute set of the Bubby
 */
AttributeSet* BubbyFlavour::getAttr()
{
  /* Override */
  if (stats != 0 || parent == 0)
    return stats;

  /* Inherit */
  return parent->getAttr();
}

/*
 * Description: Returns the list of instantiated flavours
 *
 * Inputs: none
 * Output: QVector<QString> - list of flavours that've been instantiated.
 */
QVector<QString> BubbyFlavour::getFlavourList()
{
  return flavour_list;
}

/*
 * Description: Returns the mass of a Bubby of this flavour
 *
 * Inputs: none
 * Output: double mass - the mass of a Bubby of this flavour
 */
 double BubbyFlavour::getMass()
 {
  /* Override */
  if (mass != 0 || parent == 0)
    return mass;

  /* Inherit */
  return parent->getMass();
 }


/*
 * Description: Returns the name of the BubbyFlavour
 *
 * Inputs: none
 * Output: QString - the name of the BubbyFlavour
 */
QString BubbyFlavour::getName()
{
  return name;
}

/*
 * Description: Returns the description of the BubbyFlavour
 *
 * Inputs: none
 * Output: QString - description of the Bubby flavour
 */
QString BubbyFlavour::getDescription()
{
  /* Override */
  if (description != "" || parent == 0)
    return description;

  /* Inherit */
  return parent->getDescription();
}

/*
 * Description: Returns the pointer to the parent object (Bubby Flavour)
 *
 * Inputs: none
 * Output: BubbyFlavour* - pointer to the parent flavour
 */
BubbyFlavour* BubbyFlavour::getParent()
{
  return parent;
}

/*
 * Description: Returns the pointer to the skillset
 *
 * Inputs: none
 * Output: SkillSet*
 */
SkillSet* BubbyFlavour::getSkillSet()
{
  /* Override */
  if (skill_list != 0 || parent == 0)
    return skill_list;

  /* Inherit */
  return parent->getSkillSet();
}

/*
 * Description: Returns the vector of sprites describing the sprites at each
 *              tier of the Bubby.
 *
 * Inputs: none
 * Output: QVector<Sprite*> - vector of sprite pointers (for each tier)
 */
QVector<Sprite*> BubbyFlavour::getSprites()
{
  /* Override */
  if (!sprites.isEmpty() || parent == 0)
    return sprites;

  /* Inherit */
  return parent->getSprites();
}

/*
 * Description: Sets the AttributeSet of the BubbyFlavour
 *
 * Inputs: AttributeSet - new set of attributes to assign to the Bubby
 * Output: none
 */
void BubbyFlavour::setAttr(AttributeSet* new_stats)
{
  stats = new_stats;
}

/*
 * Description: Sets the name of the BubbyFlavour
 *
 * Inputs: QString - new name of the Bubby flavour
 * Output: none
 */
void BubbyFlavour::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the description of the BubbyFlavour.
 *
 * Inputs: QString - new description of the Bubby flavour
 * Output: none
 */
void BubbyFlavour::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Returns the description of the BubbyFlavour
 *
 * Notes [1]: A BubbyFlavour instantiated without a parent can never
 *            be assigned an unknown parent object since some members
 *            may have been intentially overriden.
 *
 * Inputs: BubbyFlavour* new_parent - pointer to a new parent object
 * Output: bool - true if the parent was set successfully
 */
 bool BubbyFlavour::setParent(BubbyFlavour* new_parent)
{
  bool parent_set = false;

  if (getFlavourFlag(BubbyFlavour::CREATED_WITH_PARENT) && new_parent != 0)
  {
    parent = new_parent;
    parent_set = true;
  }
  else
    parent_set = false;
  
  return parent_set;
}

/*
 * Description: Sets the pointer to the set of skills for the BubbyFlavour.
 *
 * Inputs: SkillSet* - new skill set pointer to set to
 * Output: none
 */
void BubbyFlavour::setSkillSet(SkillSet* new_skill_list)
{
  skill_list = new_skill_list;
}

/*
 * Description: Sets the vector of sprites for the Bubby. If the size of the
 *              vector exceeds the alloted max number of tiers, then it will
 *              just add the first #oftiers of sprite pointers.
 *
 * Inputs: none
 * Output: bool - true if the sprites were assigned
 */
bool BubbyFlavour::setSprites(QVector<Sprite*> new_sprites)
{
  if (new_sprites.isEmpty())
    return false;

  /* Clear the sprite vectors before adding */
  qDeleteAll(sprites.begin(), sprites.end());
  sprites.clear();

  if (new_sprites.size() < kTIER_CAP)
    sprites = new_sprites;
  else
    for (int i = 0; i < new_sprites.size(); i++)
        sprites.push_back(new_sprites.at(i));

  sprites.squeeze();
  return true;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Checks if a flavour with a given name has already been
 *              instantiated.
 *
 * Inputs: QString - name of the flavour to check.
 * Output: int - returned as -1 if the flavour does not exist, otherwise its
 *               index
 */
int BubbyFlavour::isFlavour(QString flavour_name)
{
  if (flavour_list.contains(flavour_name))
    return flavour_list.indexOf(flavour_name);
  return -1;
}

/*
 * Description: Returns the number of tiers for a Bubby
 *
 * Inputs: none
 * Output: ushort - the number of tiers for Bubbies
 */
ushort BubbyFlavour::getNumTier()
{
  return kTIER_CAP;
}

/*
 * Description: Returns the maximum level of
 *
 * Inputs:
 * Output: ushort -
 */
ushort BubbyFlavour::getMaxSkillLevel(ushort tier_level)
{
  return kTIER_LEVELS.at(tier_level);
}
