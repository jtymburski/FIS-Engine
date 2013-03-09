/*******************************************************************************
* Class Name: BubbyFlavour
* Date Created: March 6th, 2013
* Inheritance: None
* Description: Describes the flavour of a Bubby. Every Bubby of certain flavours
*              will have the same SkillSet and AttributeSet as others.
*******************************************************************************/

#include "Game/Player/BubbyFlavour.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
QVector<QString> BubbyFlavour::flavour_list;

/*
 * Description: Constructs a BubbyFlavour object (representing a type of bubby)
 *              given a set of Attributes that the bubby will increase, as well
 *              as a set of skills it may allow the user
 *
 * Inputs: QString - name to call the new Bubby type
 *         AttributeSet - set of attributes the Bubby will power an equipment by
 *         SkillSet* - pointer to a set of skills the Bubby allows
 */
BubbyFlavour::BubbyFlavour(QString name, AttributeSet stats, SkillSet* skills)
{
  addFlavour(name);
  setName(name);
  setAttr(stats);
  setSkillSet(skills);

  for (int i = 0; i < kTIER_CAP; i++)
      sprites.push_back(NULL);
}

/*
 * Description: Annihilates a Bubby object
 */
BubbyFlavour::~BubbyFlavour()
{
  delete skill_list;
  skill_list = NULL;
  qDeleteAll(sprites.begin(), sprites.end());
  sprites.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds a flavour to the vector.
 *
 * Inputs: QString new_flavour - new flavour to add to the vector.
 * Output: bool - TRUE if the flavour was added.
 */
const bool BubbyFlavour::addFlavour(QString new_flavour)
{
  if (isFlavour(new_flavour) + 1)
    return FALSE;
  else
    flavour_list.push_back(new_flavour);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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
  if (flavour_list.contains(flavour_name));
    return flavour_list.indexOf(flavour_name);
  return -1;
}

/*
 * Description: Returns the pointer to the attribute set of the BubbyFlavour.
 *
 * Inputs: none
 * Output: AttributeSet* - pointer to the attribute set of the Bubby
 */
AttributeSet* BubbyFlavour::getAttr()
{
  return &stats;
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
  return description;
}

/*
 * Description: Returns the pointer to the skillset
 *
 * Inputs: none
 * Output: SkillSet*
 */
SkillSet* BubbyFlavour::getSkillSet()
{
  return skill_list;
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
  return sprites;
}

/*
 * Description: Sets the AttributeSet of the BubbyFlavour
 *
 * Inputs: AttributeSet - new set of attributes to assign to the Bubby
 * Output: none
 */
void BubbyFlavour::setAttr(AttributeSet new_stats)
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
 * Output: bool - TRUE if the sprites were assigned
 */
const bool BubbyFlavour::setSprites(QVector<Sprite*> new_sprites)
{
  if (new_sprites.isEmpty())
    return FALSE;

  /* Clear the sprite vectors before adding */
  qDeleteAll(sprites.begin(), sprites.end());
  sprites.clear();

  if (new_sprites.size() < kTIER_CAP)
    sprites = new_sprites;
  else
    for (int i = 0; i < new_sprites.size(); i++)
        sprites.push_back(new_sprites.at(i));

  sprites.squeeze();
  return TRUE;
}
