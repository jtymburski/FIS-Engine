/******************************************************************************
* Class Name: Person Implementation
* Date Created: December 2nd, 2012 (Rewritten March 6th, 2013)
* Inheritance: None
* Description: Head for Person class. Container for all the info describing
*              a character in the game, including their statistics for battle,
*              status ailments, flags, etc.
* // TODO: Race and category equip item types [03-06-13]
******************************************************************************/
#ifndef PERSON_H
#define PERSON_H

#include <QtGui/QWidget>
#include <cmath>
#include <algorithm>
#include "EnumDatabase.h"
#include "Game/Player/Ailment.h"
#include "Game/Player/Category.h"
#include "Game/Player/Race.h"
#include "Game/Player/Item.h"
#include "Game/Player/Equipment.h"
#include "Game/Sprite.h"

class Person
{
public:
  /* Constructor for a person object */
  Person(QString name, Race* race, Category* cat, QString p, QString s);

  /* Annihilates a person object */
  ~Person();

  /* Enumerated flags for states of a person */
  enum PersonState
  {
    ALIVE         = 1 << 0, /* Is the person alive? */
    INBATTLE      = 1 << 1, /* Is the person in a battle? */
    SLEUTH        = 1 << 2, /* Person is currently in your sleuth */
    BEARACKS      = 1 << 3, /* Person is currently in your RESERVE */
    RENDERING     = 1 << 4, /* Person is rendering on the map */
    MAINCHARACTER = 1 << 5, /* Is this person the main character? */
    BOSS          = 1 << 6, /* Is this person a Boss character? */
    MINIBOSS      = 1 << 7,  /* Is this person a miniboss? */
    MAXLVL        = 1 << 8  /* Is this person the max level? */
  };
  Q_DECLARE_FLAGS(PersonFlags, PersonState)

private:
  /* Person class constants */
  static const uint kMAX_LEVEL       =        127;
  static const uint kMIN_LVL_EXP     =        156;
  static const uint kMAX_LVL_EXP     =    5327426;
  static const uint kMAX_EXPERIENCE  = 1000000000; /* Billion */
  static const uint kMAX_EXP_DROP    =    1000000; /* Million */
  static const uint kMAX_CREDIT_DROP =   10000000; /* Ten Million */
  static const int kMAX_AILMENTS     =          5; /* Ailments at one time */
  static const uint kMAX_EQUIP_SLOTS =          5;

  /* Calculate the experience table */
  static void calcExpTable();

  /* Equipment names and slots */
  QVector<Equipment> equipment;

  /* Person's level and experience */
  uint level;
  uint total_exp;

  /* Table of experience */
  static QVector<uint> exp_table;

  /* Person's loot when killed */
  QVector<Item> item_drops;
  uint experience_drop;
  uint credit_drop;

  /* The person's class */
  Category* cat;

  /* The person's race */
  Race* race;

  /* The person's name & rank */
  QString name;
  QString rank;

  /* Set of flags for the person */
  PersonFlags state_set;

  /* List of skills and attributes that the person has */
  AttributeSet base_stats;
  AttributeSet stats;
  AttributeSet temp_stats;

  /* List of currently available skills */
  SkillSet* skills;

  /* List of status ailment strings */
  QVector<Ailment> ailments;

  /* Person's primary and secondary curves */
  QString primary;
  QString secondary;
  QChar primary_curve;
  QChar secondary_curve;

  /* Sprites for the person, first person and third person versions */
  Sprite* first_person;
  Sprite* third_person;

public:
  /* Inflicts a Status ailment */
  void addAilment(Ailment new_ailment);

  /* Adds experience of a given amount --> level up */
  void addExperience(uint n);

  /* Prepare temp_stats to be used for a battle */
  void battlePrep();

  /* Calculates the usable skill set */
  void calcSkills();

  /* Clears status ailments */
  const bool removeAilment(Infliction remove_ailment);
  void removeAilment(int index);
  void clearAll();

  /* Stat set up functions */
  void setUpBaseStats();

  /* Uses a given item on a person */
  const bool useItem(Item* used_item);

  /* PersonState flag functions */
  const bool getPersonFlag(PersonState flags);
  void setPersonFlag(PersonState flags, const bool set_value);

  /* Methods for returning ailments */
  QVector<Ailment> getAilments();
  Ailment getAilment(Infliction type);
  Ailment getAilByIndex(int index);

  /* Gets the person's category */
  Category* getCategory();

  /* Methods for returning equipments */
  QVector<Equipment> getEquipment();
  Equipment* getEquipSlot(int index);
  Equipment* getEquipSlot(QString name);

  /* Gets the experience at a given level */
  static uint getExpAt(ushort level);

  /* Gets the person's race */
  Race* getRace();

  /* Returns the available skills of the person */
  SkillSet* getSkills();

  /* Gets the person's experience */
  uint getExp();

  /* Get loot values */
  uint getExpLoot();
  uint getCreditLoot();
  QVector<Item> getItemLoot();

  /* Gets the person's level */
  uint getLevel();

  /* Gets the persons name (set in construction and never again) */
  QString getName();

  /* Gets the person's primary and secondary elemental strengths */
  QString getPrimary();
  QChar   getPrimaryCurve();
  QString getSecondary();
  QChar   getSecondaryCurve();

  /* Gets the person's word rank */
  QString getRank();

  /* Gets the first and third person sprites */
  Sprite* getFirstPerson();
  Sprite* getThirdPerson();

  /* Methods for obtaining stat sets */
  AttributeSet* baseStats();
  AttributeSet* getStats();
  AttributeSet* tempStats();

  /* Sets an equipment to a slot */
  const bool setEquipment(QString slot, Equipment e);

  /* Sets the person's category */
  void setCategory(Category* c);

  /* Set's the person's experience */
  void setExp(uint value);

  /* Set's the person's experience drop */
  void setExpLoot(uint value);

  /* Set's the person's credit drop */
  void setCreditLoot(uint value);

  /* Set's the person's item drops. */
  void setItemLoot(QVector<Item> items);

  /* Sets the person's level */
  const bool setLevel(const uint &new_level);

  /* Sets the person's level */
  void setName(QString new_name);

  /* Sets the person's primary and elemental strength curves */
  void setPrimary(QString value);
  void setPrimaryCurve(QChar value);
  void setSecondary(QString value);
  void setSecondaryCurve(QChar value);

  /* Sets the person's race */
  void setRace(Race* new_race);

  /* Sets the persons word rank */
  void setRank(QString new_rank);

  /* Sets the first person sprite */
  void setFirstPerson(Sprite* s = NULL);

  /* Sets the third person sprite */
  void setThirdPerson(Sprite* s = NULL);

  /* Methods for setting stat values */
  void setBaseStats(AttributeSet new_stat_set);
  void setStats(AttributeSet new_stat_set);
  void setTempStats(AttributeSet new_stat_set);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Person::PersonFlags)

#endif // PERSON_H
