/******************************************************************************
* Class Name: Race
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class - Person
* Description: Header for Race that defines the race for the particular person.
*              Some examples includes Bears, Humans, etc. The Race class will
*              provide some unique Actions for persons, as well as provide
*              bonuses to the category stats when making up a person.
******************************************************************************/
#ifndef RACE_H
#define RACE_H

#include <QtGui/QWidget>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"

class Race : public QWidget
{
public:
   /* Race constructor */
   Race(QString name, AttributeSet attributes, SkillSet* skill_set = 0);

   /* Default Race Constructor given a name */
   Race(QString name);

   /* Annihilates a race object */
   ~Race();
  
private:
  /* Name & description */
  QString description;
  QString denonym;
  QString name;

  /* List of racial immunities */
  QList<EnumDb::Infliction> immunities;

  /* Set of attributes */
  AttributeSet stat_set;
  SkillSet* skill_set;

  /* ------------ Constants --------------- */
  static const ushort kMAX_VITALITY;
  static const ushort kMAX_QD;
  static const ushort kMAX_PHYSICAL;
  static const ushort kMAX_ELEMENTAL;
  static const ushort kMAX_SPECIAL;

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

  /* Gets the attribute and skill sets */
  AttributeSet getAttrSet();
  SkillSet* getSkillSet();

  /*  Sets Description & Name */
  void setDescription(QString new_description);
  void setDenonym(QString new_denonym);
  void setName(QString new_name);

  /* Sets the Attribute and Skill sets */
  void setAttrSet(AttributeSet new_stat_set);
  void setSkillSet(SkillSet* new_skill_set);
};
#endif // RACE_H
