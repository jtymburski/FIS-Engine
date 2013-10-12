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
#ifndef CATEGORY_H
#define CATEGORY_H

#include <QDebug>
#include <QString>
#include <QList>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "EnumDb.h"

class Category
{
public:
  /* Default Constructor */
  Category();

  /* Copy Constructor */
  Category(const Category &source);

  /* Constructs a generi category just given a name */
  Category(QString name);

  /* Category constructor */
  Category(QString name, QString denonym, QString desc, AttributeSet base_set,
           AttributeSet max_set, SkillSet* skills = 0);

  /* Annihilates a Category object */
  ~Category();

private:
  /* Name & description */
  QString description;
  QString denonym;
  QString name;

  /* Attribute Sets for a Category */
  AttributeSet base_stat_set;
  AttributeSet max_stat_set;

  /* The skills that this category can use */
  SkillSet* skill_set;

  /* A list of enumerated immunities */
  QList<EnumDb::Infliction> immunities;

  /* ------------ Constants --------------- */
  static const uint kMIN_VITA;
  static const uint kMIN_QTDR;
  static const uint kMIN_PHYS;
  static const uint kMIN_ELMT;
  static const uint kMIN_SPEC;
  static const uint kMAX_VITA;
  static const uint kMAX_QTDR;
  static const uint kMAX_PHYS;
  static const uint kMAX_ELMT;
  static const uint kMAX_SPEC;

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Deep copy for Category class */
  void copySelf(const Category& source);

  /* Loads the category with default objects and values */
  void loadDefaults();

  /* Builds a minimum attribute set based on min constants */
  static AttributeSet buildMinSet();

  /* Builds a maximum attribute set based on max constants */
  static AttributeSet buildMaxSet();

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

  /* Gets the description, denonym, and name */
  QString getDescription();
  QString getDenonym();
  QString getName();

  /* Returns the attribute sets of the category */
  AttributeSet getBaseSet();
  AttributeSet getMaxSet();

  /* Returns the skill set of the category */
  SkillSet* getSkillSet();

  /* Returns the list of immunities of the category */
  QList<EnumDb::Infliction> getImmunities();

  /* Sets the description, name, and denonym of the category */
  void setDescription(QString new_description);
  void setDenonym(QString new_denonym);
  void setName(QString new_name);

  /* Assigns new attribute sets to the category */
  void setAttrSet(AttributeSet new_stat_set);
  void setMaxSet(AttributeSet new_max_set);

  /* Assigns a new skill set to the category */
  void setSkillSet(SkillSet* new_skill_set);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  Category& operator= (const Category &source);
};
#endif // CATEGORY_H
