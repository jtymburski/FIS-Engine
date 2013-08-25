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
#ifndef RACE_H
#define RACE_H

#include <QDebug>
#include <QString>
#include <QDebug>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "EnumDb.h"

class Race : public QWidget
{
public:
   /* Default Race Constructor */
   Race();

   /* Copy Constructor */
   Race(Race& other);

   /* Default Race Constructor given a name */
   Race(QString name);

   /* Race constructor */
   Race(QString name, QString denonym, QString desc, AttributeSet base_set,
        AttributeSet max_set, SkillSet* skill_set = 0);

   /* Annihilates a race object */
   ~Race();
  
private:
  /* Name & description */
  QString description;
  QString denonym;
  QString name;

  /* Attribute Sets for the Race */
  AttributeSet base_stat_set;
  AttributeSet max_stat_set;

  /* Pointer to skill set the race can use */
  SkillSet* skill_set;

  /* List of racial immunities */
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
  /* Adds an ailment to the immunity list */
  void addImmunity(EnumDb::Infliction new_immunity);

  /* Cleans up the attributes and stat set */
  void cleanUp();

  /* Checks whether the Race is immune to a given Infliction */
  bool isImmune(EnumDb::Infliction check_immunity);

  /* Removes a given infliction from the list (if it exists) */
  void removeImmunity(EnumDb::Infliction remove_immuity);

  /* Method for printing the info of the Race */
  void printInfo();

  /* Gets the Description & Name */
  QString getDescription();
  QString getDenonym();
  QString getName();

  /* Returns the Attribute Sets of the Race */
  AttributeSet getBaseSet();
  AttributeSet getMaxSet();

  /* Returns the Skill Set of the Race */
  SkillSet* getSkillSet();

  /* Returns the list of immunities of the category */
  QList<EnumDb::Infliction> getImmunities();

  /*  Sets Description & Name */
  void setDescription(QString new_description);
  void setDenonym(QString new_denonym);
  void setName(QString new_name);

  /* Assigns new base and max attribute sets to the Race */
  void setAttrSet(AttributeSet new_stat_set);
  void setMaxSet(AttributeSet new_stat_set);

  /* Assigns a new skill set */
  void setSkillSet(SkillSet* new_skill_set);
};
#endif // RACE_H
