/*******************************************************************************
* Class Name: Category
* Date Created: Sunday, October 28th, 2012
* Inheritance: None
* Description: Header for the Category class. Every Person has a Category, which
*              is equivalent to their "Battle Class". This holds their starting
*              and ending values for attributes (In an attribute set), and a
*              set of skills which will become available to the person at
*              various levels.
* // TODO: SKILL SORTING [03-05-13]
* // TODO: ADD CATEGORY FLAGS [03-05-13]
*******************************************************************************/
#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtGui/QWidget>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "Game/Player/Skill.h"

class Category
{
public:
  /* Category constructor */
  Category(QString name, AttributeSet attributes, SkillSet* skills = 0);

  /* Default Category Constructor given a name */
  Category(QString name);

  /* Annihilates a Category object */
  ~Category();

private:
  /* Name & description */
  QString description;
  QString denonym;
  QString name;

  /* Set of attributes and skills for the category */
  AttributeSet stat_set;
  SkillSet* skill_set;

  QList<EnumDb::Infliction> immunities;

  /* ------------ Constants --------------- */
  static const ushort kMAX_VITALITY;
  static const ushort kMAX_QD;
  static const ushort kMAX_PHYSICAL;
  static const ushort kMAX_ELEMENTAL;
  static const ushort kMAX_SPECIAL;

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds an immunity */
  void addImmunity(EnumDb::Infliction new_immunity);

  /* Cleans up the attributes and stat set */
  void cleanUp();

  /* Checks the Immunity of the Category to a given Infliction */
  bool isImmune(EnumDb::Infliction check_immunity);

  /* Removes an immunity from the category */
  void removeImmunity(EnumDb::Infliction remove_immunity);

  /* Method for printing out the information of the category */
  void printInfo();

  /* Gets the description & name */
  QString getDescription();
  QString getDenonym();
  QString getName();

  /* Returns the attribute and skill sets */
  AttributeSet getAttrSet();
  SkillSet* getSkillSet();

  /* Sets the description & name */
  void setDescription(QString new_description);
  void setDenonym(QString new_denonym);
  void setName(QString new_name);

  /* Sets the skill set for the category */
  void setAttrSet(AttributeSet new_stat_set);
  void setSkillSet(SkillSet* new_skill_set);

};
#endif // CATEGORY_H
