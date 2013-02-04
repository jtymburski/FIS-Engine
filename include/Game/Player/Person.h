/******************************************************************************
* Class Name: Person
* Date Created: Sunday, October 28th, 2012
* Inheritance : QWidget
* Description: Holder for all the info describing a person (character)
*  
* TODO: FLAGS FOR STATUS AILMENTS [01-20-13]
******************************************************************************/
#ifndef PERSON_H
#define PERSON_H

#include <QtGui/QWidget>
#include "Game/Player/Action.h"
#include "Game/Player/Category.h"
#include "Game/Player/Item.h"
#include "Game/Player/Equipment.h"
#include "Game/Player/Race.h"
#include "Game/Sprite.h"

class Person : public QWidget
{


public:
  /* Constructor for a person object */
  Person(uint ph_a = 5, uint ph_d = 2, uint th_a = 5, uint th_f = 2, uint po_a = 5,
         uint po_f = 2, uint pr_a = 5, uint pr_f = 2, uint ch_a = 5, uint ch_f = 2,
         uint cy_a = 5, uint cy_f = 2, uint ni_a = 5, uint ni_f = 2, uint vit = 300,
         uint qd = 15,  uint ag = 7,   uint lim = 7,  uint unb = 7,  uint lev = 5,
         uint exp = 200, QString name = "Person", QString prim = "CYA",
         QString secd = "POB", QString rank = "Recruit", QWidget* pointer = 0);

  /* Annihilates a person object */
  ~Person();

  /* Enumerated flags for states of a person */
  enum PersonState
  {
    ALIVE     = 1 << 0, /* Is the person alive? */
    INBATTLE  = 1 << 1,  /* Is the person in a battle? */
    SLEUTH    = 1 << 2,
    BEARACKS  = 1 << 3,
    RENDERING = 1 << 4
  };
  Q_DECLARE_FLAGS(PersonFlags, PersonState);

  /* Enumerated flags for status ailments */
  enum StatusAilment
  {
    CLEAR            = 1 << 0,  /* Is the person to be clear of all status ailments? */
    POISONED         = 1 << 1,
    BURNED1          = 1 << 2,  /* 1st Degree Burn */
    BURNED2          = 1 << 3,  /* 2nd Degree Burn */
    BURNED3          = 1 << 4,  /* 3rd Degree Burn */
    BERSERK          = 1 << 5,
    CONFUSE          = 1 << 6,
    SILENCE          = 1 << 7,
    SLOW             = 1 << 8,
    BUBBIFY          = 1 << 9,
    DEATHTIMER       = 1 << 10,
    PARALYSIS        = 1 << 11,
    BLINDNESS        = 1 << 12,
    DREADSTRUCK      = 1 << 13,
    DREAMSNARED      = 1 << 14,
    HELLBOUND        = 1 << 15,
    BONDED           = 1 << 16,
    PHYSICALATKBUFF  = 1 << 17,
    PHYSICALDEFBUFF  = 1 << 18,
    PRIMARYATKBUFF   = 1 << 19,
    PRIMARYDEFBUFF   = 1 << 20,
    SECONDARYATKBUFF = 1 << 21,
    SECONDARYDEFBUFF = 1 << 22,
    ALLDEFBUFF       = 1 << 23,
    HEALTHBUFF       = 1 << 24,
    QDBUFF           = 1 << 25,
    ROOTBOUND        = 1 << 26,
    DOUBLECAST       = 1 << 27,
    TRIPLECAST       = 1 << 28,
    HALFCOST         = 1 << 29
  };
  Q_DECLARE_FLAGS(StatusFlags, StatusAilment);

private:

  /* Physical atk stat */
  uint physical_aggression;

  /* Physical def stat */
  uint physical_fortitude;

  /* Fire atk stat */
  uint thermal_aggression;

  /* Fire def stat */
  uint thermal_fortitude;

  /* Ice atk stat */
  uint polar_aggression;

  /* Ice def stat */
  uint polar_fortitude;

  /* Forest atk stat */
  uint primal_aggression;

  /* Forest def stat */
  uint primal_fortitude;

  /* Electric atk stat */
  uint charged_aggression;

  /* Electric def stat */
  uint charged_fortitude;

  /* Digital atk stat */
  uint cybernetic_aggression;

  /* Digital def stat */
  uint cybernetic_fortitude;

  /* Void atk stat */
  uint nihil_aggression;

  /* Void def stat */
  uint nihil_fortitude;

  /* Health stat */
  uint vitality;

  /* Quantum drive (MP), shown as percentage */
  uint quantum_drive;

  /* Speed stat */
  uint agility;

  /* Dodge stat */
  uint limbertude;

  /* Critical chance stat */
  uint unbearability;

  /* Temporary physical atk stat */
  uint temp_physical_aggression;

  /* Temporary physical def stat */
  uint temp_physical_fortitude;

  /* Temporary fire atk stat */
  uint temp_thermal_aggression;

  /* Temporaryfire def stat */
  uint temp_thermal_fortitude;

  /* Temporary ice atk stat */
  uint temp_polar_aggression;

  /* Temporary ice def stat */
  uint temp_polar_fortitude;

  /* Temporary forest atk stat */
  uint temp_primal_aggression;

  /* Temporary forest def stat */
  uint temp_primal_fortitude;

  /* Temporary electric atk stat */
  uint temp_charged_aggression;

  /* Temporary electri def stat */
  uint temp_charged_fortitude;

  /* Temporary digital atk stat */
  uint temp_cybernetic_aggression;

  /* Temporary digital def stat */
  uint temp_cybernetic_fortitude;

  /* Temporary void atk stat */
  uint temp_nihil_aggression;

  /* Temporary void def stat */
  uint temp_nihil_fortitude;

  /* Temporary health stat */
  uint temp_vitality;

  /* Temporary power points (MP) stat */
  uint temp_quantum_drive;

  /* Temporary speed stat */
  uint temp_agility;

  /* Temporary dodge_chance stat */
  uint temp_limbertude;

  /* Temporary crit chance stat */
  uint temp_unbearability;

  /* Base physical atk stat */
  uint base_physical_aggression;

  /* Base physical def stat */
  uint base_physical_fortitude;

  /* Base fire atk stat */
  uint base_thermal_aggression;

  /* Base def stat */
  uint base_thermal_fortitude;

  /* Base ice atk stat */
  uint base_polar_aggression;

  /* Base ice def stat */
  uint base_polar_fortitude;

  /* Base forest atk stat */
  uint base_primal_aggression;

  /* Base forest def stat */
  uint base_primal_fortitude;

  /* Base electric atk stat */
  uint base_charged_aggression;

  /* Base electri def stat */
  uint base_charged_fortitude;

  /* Base digital atk stat */
  uint base_cybernetic_aggression;

  /* Base digital def stat */
  uint base_cybernetic_fortitude;

  /* Base void atk stat */
  uint base_nihil_aggression;

  /* Base void def stat */
  uint base_nihil_fortitude;

  /* Base health stat */
  uint base_vitality;

  /* Base power points (MP) stat */
  uint base_quantum_drive;

  /* Base speed stat */
  uint base_agility;

  /* Base dodge_chance stat */
  uint base_limbertude;

  /* Base crit chance stat */
  uint base_unbearability;

  /* Currently equipped head piece */
  Equipment* head;

  /* Currently equipped left arm piece */
  Equipment* left_arm;

  /* Currently equipped right arm piece */
  Equipment* right_arm;

  /* Currently equipped leg piece */
  Equipment* legs;

  /* Currently equipped torso piece */
  Equipment* torso;

  /* Person's level and experience */
  uint level, experience;

  /* The person's class */
  Category* character_class; 

  /* The person's race */
  Race* character_race; 

  /* Person's word ranking */
  QString rank;

  /* Set of flags for the person */
  PersonFlags state_set;

  /* Set the status ailment flags for the person */
  StatusFlags ailment_set;

  /* List of actions that the person has */
  QVector<Action*> action_list;

  /* Parallel list of when person's actions become available */
  QVector<int>  action_available;

  /* List of status ailment strings */
  QVector<QString> status_ailment_list;

  /* The person's name */
  QString name;  

  /* The person's primary elemental strength */
  QString primary;

  /* The person's secondary elemental strength */
  QString secondary;

  /* The person's primary elemental strength curve */
  QChar primary_curve;

  /* The persons' secondary elemental strenth curve */
  QChar secondary_curve;

  /* Sprites for the person, first person and third person versions */
  Sprite* first_person;
  Sprite* third_person;

  /* Set up stats for constructor */
  void setupStats(uint ph_a = 5, uint ph_f = 2, uint th_a = 5, uint th_f = 2, uint po_a = 5, uint po_f = 2,
                  uint pr_a = 5, uint pr_f = 2, uint ch_a = 5, uint ch_f = 2,
                  uint cy_a = 5, uint cy_f = 2, uint ni_a = 5, uint ni_f = 2,
                  uint vit = 300, uint qd = 15, uint ag = 7, uint lim = 7,
                  uint unb = 7,  uint lev = 5, uint exp = 200,
                  QString name = "Person", QString prim = "CYA",
                  QString secd = "POB", QString rank = "Recruit", QWidget* pointer = 0);

public:
  /* Adds an equipment to the given person */
  bool addEquipment(Equipment* equipment);

  /* Adds experience of a given amount */
  void addExperience(uint n);

  /* Use a given item */
  bool useItem(Item* used_item);

  /* Toggles a StatusAilments flag */
  void toggleAilment(StatusAilment flags);

  /* Toggles a PersonState flag */
  void togglePersonFlag(PersonState flags);

  /* Evaluates the boolean value of a StatusAilment */
  const bool getAilment(StatusAilment flags);

  /* Gets the boolean value of PersonState flag */
  const bool getPersonFlag(PersonState flags);

  /* Returns the available actions of the person */
  QVector<Action*>& getAvailableActions();

  /* Gets the person's category */
  Category* getCategory();

  /* Gets the person's race */
  Race* getRace();

  /* Gets the head equipment */
  Equipment* getHeadEquipment();

  /* Gets the left arm equipment */
  Equipment* getLeftArmEquipment();

  /* Gets the right arm equipment */
  Equipment* getRightArmEquipment();

  /* Gets the leg equipment */
  Equipment* getLegEquipment();

  /* Gets the torso equpiment */
  Equipment* getTorsoEquipment();

  /* Gets the person's experience */
  uint getExp();

  /* Gets the person's level */
  uint getLevel();

  /* Gets the persons name (set in construction and never again) */
  QString getName();

  /* Gets the person's primary elemental strength */
  QString getPrimary();

  /* Gets the person's curve level for their primary elemental strength */
  QString getPrimaryCurve();

  /* Gets the person's secondary elemental strength */
  QString getSecondary();

  /* Gets the person's curve level for their primary elemental strength */
  QString getSecondaryCurve();

  /* Gets the person's word rank */
  QString getRank();

  /* Gets the first person sprite */
  Sprite* getFirstPerson();

  /* Gets the third person person sprite */
  Sprite* getThirdPerson();

  /* Gets the physical atk stat */
  uint getPhysicalAggression();

  /* Sets the physical def stat */
  uint getPhysicalFortitude();

  /* Gets the fire attack stat */
  uint getThermalAggression();

  /* Gets the fire defense stat */
  uint getThermalFortitude();

  /* Gets the ice attack stat */
  uint getPolarAggression();

  /* Gets the ice defense stat */
  uint getPolarFortitude();

  /* Gets the forest attack stat */
  uint getPrimalAggression();

  /* Gets the forest defense stat */
  uint getPrimalFortitude();

  /* Gets the electric attack stat */
  uint getChargedAggression();

  /* Gets the electric defense stat */
  uint getChargedFortitude();

  /* Gets the digital attack stat */
  uint getCyberneticAggression();

  /* Gets the digital defense stat */
  uint getCyberneticFortitude();

  /* Gets the void attack stat */
  uint getNihilAggression();

  /* Gets the void defense stat */
  uint getNihilFortitude();

  /* Gets the health stat */
  uint getVitality();

  /* Gets the power points (shown as a percentage) */
  uint getQuantumDrive();

  /* Gets the speed stat */
  uint getAgility();

  /* Gets the dodge stat */
  uint getLimbertude();

   /* Gets the critical chance stat */
  uint getUnbearability();

  /* Sets the temp physical atk stat */
  uint getTempPhysicalAggression();

  /* Sets the temp physical def stat */
  uint getTempPhysicalFortitude();

  /* Gets the fire atk stat */
  uint getTempThermalAggression();

  /* Gets the fire def stat */
  uint getTempThermalFortitude();

  /* Gets the ice atk stat */
  uint getTempPolarAggression();

  /* Gets the ice def stat */
  uint getTempPolarFortitude();

  /* Gets the forest atk stat */
  uint getTempPrimalAggression();

  /* Gets the forest def stat */
  uint getTempPrimalFortitude();

  /* Gets the electric atk stat */
  uint getTempChargedAggression();

  /* Gets the electric def stat */
  uint getTempChargedFortitude();

  /* Gets the digital atk stat */
  uint getTempCyberneticAggression();

  /* Gets the digital def stat */
  uint getTempCyberneticFortitude();

  /* Gets the void atk stat */
  uint getTempNihilAggression();

  /* Gets the void def stat */
  uint getTempNihilFortitude();

  /* Gets the health stat */
  uint getTempVitality();

  /* Gets the power points (shown as a percentage) */
  uint getTempQuantumDrive();

  /* Gets the speed stat */
  uint getTempAgility();

  /* Gets the avoidance stat */
  uint getTempLimbertude();

  /* Gets the critcial chance stat */
  uint getTempUnbearability();

  /* Sets the base physical atk stat */
  uint getBasePhysicalAggression();

  /* Sets the base physical def stat */
  uint getBasePhysicalFortitude();

  /* Gets the fire atk stat */
  uint getBaseThermalAggression();

  /* Gets the fire def stat */
  uint getBaseThermalFortitude();

  /* Gets the ice atk stat */
  uint getBasePolarAggression();

  /* Gets the ice def stat */
  uint getBasePolarFortitude();

  /* Gets the forest atk stat */
  uint getBasePrimalAggression();

  /* Gets the forest def stat */
  uint getBasePrimalFortitude();

  /* Gets the electric atk stat */
  uint getBaseChargedAggression();

  /* Gets the electric def stat */
  uint getBaseChargedFortitude();

  /* Gets the digital atk stat */
  uint getBaseCyberneticAggression();

  /* Gets the digital def stat */
  uint getBaseCyberneticFortitude();

  /* Gets the void atk stat */
  uint getBaseNihilAggression();

  /* Gets the void def stat */
  uint getBaseNihilFortitude();

  /* Gets the health stat */
  uint getBaseVitality();

  /* Gets the power points (shown as a percentage) */
  uint getBaseQuantumDrive();

  /* Gets the speed stat */
  uint getBaseAgility();

  /* Gets the avoidance stat */
  uint getBaseLimbertude();

  /* Gets the critcial chance stat */
  uint getBaseUnbearability();

  /* Sets the value of a status ailment */
  void setAilment(StatusAilment flags, const bool set_value);

  /* Sets the value of a PersonState flag */
  void setPersonFlag(PersonState flags, const bool set_value);

  /* Sets the head equipment */
  const bool setHeadEquipment(Equipment* e = NULL);

  /* Sets the left arm equipmet */
  const bool setLeftArmEquipment(Equipment* e = NULL);

  /* Sets the right arm equipment */
  const bool setRightArmEquipment(Equipment* e = NULL);

  /* Sets the leg equipment */
  const bool setLegEquipment(Equipment* e = NULL);

  /* Sets the torso equipment */
  const bool setTorsoEquipment(Equipment* e = NULL);

  /* Sets the person's category */
  void setCategory(Category* c);

  /* Set's the person's experience */
  void setExp(uint value);

  /* Sets the person's level */
  void setLevel(uint new_level);

  /* Sets the person's level */
  void setName(QString value);

  /* Sets the person's primary elemental strength */
  void setPrimary(QString value);

  /* Sets the person's curve level for their primary elemental strength */
  void setPrimaryCurve(QChar value);

  /* Sets the person's secondary elemental strength */
  void setSecondary(QString value);

  /* Sets the person's curve level for their secondary elemental strength */
  void setSecondaryCurve(QChar value);

  /* Sets the person's race */
  void setRace(Race* r);

  /* Sets the persons word rank */
  void setRank(QString rank);

  /* Sets the first person sprite */
  void setFirstPerson(Sprite* s = NULL);

  /* Sets the third person sprite */
  void setThirdPerson(Sprite* s = NULL);

  /* Sets temporary stats as current stats */
  void setTemporaryStats();

  /* Sets the base stats as current stats */
  void setBaseStats();

  /* Sets the physical atk stat */
  void setPhysicalAggression(uint value);

  /* Sets the physical def stat */
  void setPhysicalFortitude(uint value);

  /* Sets the fire attack stat */
  void setThermalAggression(uint value);

  /* Sets the fire defense stat */
  void setThermalFortitude(uint value);

  /* Sets the ice attack stat */
  void setPolarAggression(uint value);

  /* Sets the ice defense stat */
  void setPolarFortitude(uint value);

  /* Sets the forest attack stat */
  void setPrimalAggression(uint value);

  /* Sets the forest defense stat */
  void setPrimalFortitude(uint value);

  /* Sets the electric attack stat */
  void setChargedAggression(uint value);

  /* Sets the electric defense stat */
  void setChargedFortitude(uint value);

  /* Sets the digital attack stat */
  void setCyberneticAggression(uint value);

  /* Sets the digital defense stat */
  void setCyberneticFortitude(uint value);

  /* Sets the void attack stat */
  void setNihilAggression(uint value);

  /* Sets the void defense stat */
  void setNihilFortitude(uint value);

  /* Sets the health stat */
  void setVitality(uint value);

  /* Sets the power points (shown as percentage) */
  void setQuantumDrive(uint value);

  /* Sets the speed stat */
  void setAgility(uint value);

  /* Sets the dodge stat */
  void setLimbertude(uint value);

  /* Sets the critical chance stat */
  void setUnbearability(uint value);

  /* Sets the temp physical atk stat */
  void setTempPhysicalAggression(uint value);

  /* Sets the temp physical def stat */
  void setTempPhysicalFortitude(uint vaule);

  /* Sets the temp fire attack stat */
  void setTempThermalAggression(uint value);

  /* Sets the temp fire def stat */
  void setTempThermalFortitude(uint value);

  /* Sets the temp ice atk stat */
  void setTempPolarAggression(uint value);

   /* Sets the temp ice def stat */
  void setTempPolarFortitude(uint value);

  /* Sets the temp forest atk stat */
  void setTempPrimalAggression(uint value);

  /* Sets the temp forest def stat */
  void setTempPrimalFortitude(uint value);

  /* Sets the temp electric atk stat */
  void setTempChargedAggression(uint value);

  /* Sets the temp electric def stat */
  void setTempChargedFortitude(uint value);

  /* Sets the temp digital atk stat */
  void setTempCyberneticAggression(uint value);

  /* Sets the temp digital def stat */
  void setTempCyberneticFortitude(uint value);

  /* Setst the temp void atk stat */
  void setTempNihilAggression(uint value);

  /* Sets the temp void def stat */
  void setTempNihilFortitude(uint value);

  /* Sets the temp health stat */
  void setTempVitality(uint value);

  /* Sets the temp power points (shown as percentage) */
  void setTempQuantumDrive(uint value);

  /* Sets the temp speed stat */
  void setTempAgility(uint value);

  /* Sets the temp avoidance stat */
  void setTempLimbertude(uint value);

  /* Sets the temp critical chance stat */
  void setTempUnbearability(uint value);

  /* Sets the base physical atk stat */
  void setBasePhysicalAggression(uint value);

  /* Sets the base physical def stat */
  void setBasePhysicalFortitude(uint value);

  /* Sets the Base fire attack stat */
  void setBaseThermalAggression(uint value);

  /* Sets the Base fire def stat */
  void setBaseThermalFortitude(uint value);

  /* Sets the Base ice atk stat */
  void setBasePolarAggression(uint value);

   /* Sets the Base ice def stat */
  void setBasePolarFortitude(uint value);

  /* Sets the Base forest atk stat */
  void setBasePrimalAggression(uint value);

  /* Sets the Base forest def stat */
  void setBasePrimalFortitude(uint value);

  /* Sets the Base electric atk stat */
  void setBaseChargedAggression(uint value);

  /* Sets the Base electric def stat */
  void setBaseChargedFortitude(uint value);

  /* Sets the Base digital atk stat */
  void setBaseCyberneticAggression(uint value);

  /* Sets the Base digital def stat */
  void setBaseCyberneticFortitude(uint value);

  /* Setst the Base void atk stat */
  void setBaseNihilAggression(uint value);

  /* Sets the Base void def stat */
  void setBaseNihilFortitude(uint value);

  /* Sets the Base health stat */
  void setBaseVitality(uint value);

  /* Sets the Base power points (shown as percentage) */
  void setBaseQuantumDrive(uint value);

  /* Sets the Base speed stat */
  void setBaseAgility(uint value);

  /* Sets the Base avoidance stat */
  void setBaseLimbertude(uint value);

  /* Sets the Base critical chance stat */
  void setBaseUnbearability(uint value);
};

#endif // PERSON_H
