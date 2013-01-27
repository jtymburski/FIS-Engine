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
  /* Creates a default person object */
  Person(QWidget* pointer = 0);

  /* Creates a person object */
  Person(int th_a, int th_f, int po_ag, int po_f, int pr_a, int pr_f,
         int ch_a, int ch_f, int cy_a,  int cy_f, int ni_a, int ni_f,
         int vit,  int qd,   int ag,    int lim,  int unb,  int lev,
         int exp, QString nam, QString prim, QString secd, QString rank,
         QWidget* pointer = 0);

  /* Annihilates a person object */
  ~Person();

private:
  /* Fire atk stat */
  int thermal_aggression; 

  /* Fire def stat */
  int thermal_fortitude;

  /* Ice atk stat */
  int polar_aggression; 

  /* Ice def stat */
  int polar_fortitude; 

  /* Forest atk stat */
  int primal_aggression; 

  /* Forest def stat */
  int primal_fortitude; 

  /* Electric atk stat */
  int charged_aggression; 

  /* Electric def stat */
  int charged_fortitude;

  /* Digital atk stat */
  int cybernetic_aggression; 

  /* Digital def stat */
  int cybernetic_fortitude; 

  /* Void atk stat */
  int nihil_aggression; 

  /* Void def stat */
  int nihil_fortitude; 

  /* Health stat */
  int vitality; 

  /* Quantum drive (MP), shown as percentage */
  int quantum_drive;

  /* Speed stat */
  int agility; 

  /* Dodge stat */
  int limbertude;

  /* Critical chance stat */
  int unbearability; 

  /* Temporary fire atk stat */
  int temp_thermal_aggression;

  /* Temporaryfire def stat */
  int temp_thermal_fortitude;

  /* Temporary ice atk stat */
  int temp_polar_aggression;

  /* Temporary ice def stat */
  int temp_polar_fortitude;

  /* Temporary forest atk stat */
  int temp_primal_aggression;

  /* Temporary forest def stat */
  int temp_primal_fortitude;

  /* Temporary electric atk stat */
  int temp_charged_aggression;

  /* Temporary electri def stat */
  int temp_charged_fortitude;

  /* Temporary digital atk stat */
  int temp_cybernetic_aggression;

  /* Temporary digital def stat */
  int temp_cybernetic_fortitude;

  /* Temporary void atk stat */
  int temp_nihil_aggression;

  /* Temporary void def stat */
  int temp_nihil_fortitude;

  /* Temporary health stat */
  int temp_vitality;

  /* Temporary power points (MP) stat */
  int temp_quantum_drive;

  /* Temporary speed stat */
  int temp_agility;

  /* Temporary dodge_chance stat */
  int temp_limbertude;

  /* Temporary crit chance stat */
  int temp_unbearability;

  /* Base fire atk stat */
  int base_thermal_aggression;

  /* Base def stat */
  int base_thermal_fortitude;

  /* Base ice atk stat */
  int base_polar_aggression;

  /* Base ice def stat */
  int base_polar_fortitude;

  /* Base forest atk stat */
  int base_primal_aggression;

  /* Base forest def stat */
  int base_primal_fortitude;

  /* Base electric atk stat */
  int base_charged_aggression;

  /* Base electri def stat */
  int base_charged_fortitude;

  /* Base digital atk stat */
  int base_cybernetic_aggression;

  /* Base digital def stat */
  int base_cybernetic_fortitude;

  /* Base void atk stat */
  int base_nihil_aggression;

  /* Base void def stat */
  int base_nihil_fortitude;

  /* Base health stat */
  int base_vitality;

  /* Base power points (MP) stat */
  int base_quantum_drive;

  /* Base speed stat */
  int base_agility;

  /* Base dodge_chance stat */
  int base_limbertude;

  /* Base crit chance stat */
  int base_unbearability;

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
  int level, experience;

  /* The person's class */
  Category* character_class; 

  /* The person's race */
  Race* character_race; 

  /* Person's word ranking */
  QString rank;

  /* List of actions that the person has */
  QVector<Action*> action_list;

  /* Parallel list of when person's actions become available */
  QVector<int>  action_available;

  /* List of status ailment Strings */
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

  /* First and third person sprites */
  Sprite first_person, third_person;

  /* Set up stats for constructor */
  void setupStats(int th_a, int th_f, int po_ag, int po_f, int pr_a, int pr_f,
                  int ch_a, int ch_f, int cy_a,  int cy_f, int ni_a, int ni_f,
                  int vit,  int qd,   int ag,    int lim,  int unb,  int lev,
                  int exp, QString nam, QString prim, QString secd, QString rank);

  /* Flags */

  /* The status ailments the person can have (not complete) */
  bool status_ailment; //TODO: NOT FINISHED

  /* Flag for if the person is rendering or not */
  bool RENDERING; 

public:
  /* Adds an equipment to the given person */
  bool addEquipment(Equipment* equipment);

  /* Adds experience of a given amount */
  void addExperience(int n);

  /* Sets up total and temporary stats */
  void initiateForBattle(); //Sets up temp stats and total stats

  /* Checks the given status ailment status */
  bool isStatusAilment(QString s);

  /* Use a given item */
  bool useItem(Item* used_item);

  /* Returns the available actions of the person */
  QVector<Action*>& getAvailableActions();

  /* Evaluates the RENDERING flag */
  bool getRendering();

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
  int getExp();

  /* Gets the person's level */
  int getLevel();

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
  Sprite getFirstPerson();

  /* Gets the third person person sprite */
  Sprite getThirdPerson();

  /* Gets the fire attack stat */
  int getThermalAggression();

  /* Gets the fire defense stat */
  int getThermalFortitude();

  /* Gets the ice attack stat */
  int getPolarAggression();

  /* Gets the ice defense stat */
  int getPolarFortitude();

  /* Gets the forest attack stat */
  int getPrimalAggression();

  /* Gets the forest defense stat */
  int getPrimalFortitude();

  /* Gets the electric attack stat */
  int getChargedAggression();

  /* Gets the electric defense stat */
  int getChargedFortitude();

  /* Gets the digital attack stat */
  int getCyberneticAggression();

  /* Gets the digital defense stat */
  int getCyberneticFortitude();

  /* Gets the void attack stat */
  int getNihilAggression();

  /* Gets the void defense stat */
  int getNihilFortitude();

  /* Gets the health stat */
  int getVitality();

  /* Gets the power points (shown as a percentage) */
  int getQuantumDrive();

  /* Gets the speed stat */
  int getAgility();

  /* Gets the dodge stat */
  int getLimbertude();

   /* Gets the critical chance stat */
  int getUnbearability();

  /* Gets the fire atk stat */
  int getTempThermalAggression();

  /* Gets the fire def stat */
  int getTempThermalFortitude();

  /* Gets the ice atk stat */
  int getTempPolarAggression();

  /* Gets the ice def stat */
  int getTempPolarFortitude();

  /* Gets the forest atk stat */
  int getTempPrimalAggression();

  /* Gets the forest def stat */
  int getTempPrimalFortitude();

  /* Gets the electric atk stat */
  int getTempChargedAggression();

  /* Gets the electric def stat */
  int getTempChargedFortitude();

  /* Gets the digital atk stat */
  int getTempCyberneticAggression();

  /* Gets the digital def stat */
  int getTempCyberneticFortitude();

  /* Gets the void atk stat */
  int getTempNihilAggression();

  /* Gets the void def stat */
  int getTempNihilFortitude();

  /* Gets the health stat */
  int getTempVitality();

  /* Gets the power points (shown as a percentage) */
  int getTempQuantumDrive();

  /* Gets the speed stat */
  int getTempAgility();

  /* Gets the avoidance stat */
  int getTempLimbertude();

  /* Gets the critcial chance stat */
  int getTempUnbearability();

  /* Gets the fire atk stat */
  int getBaseThermalAggression();

  /* Gets the fire def stat */
  int getBaseThermalFortitude();

  /* Gets the ice atk stat */
  int getBasePolarAggression();

  /* Gets the ice def stat */
  int getBasePolarFortitude();

  /* Gets the forest atk stat */
  int getBasePrimalAggression();

  /* Gets the forest def stat */
  int getBasePrimalFortitude();

  /* Gets the electric atk stat */
  int getBaseChargedAggression();

  /* Gets the electric def stat */
  int getBaseChargedFortitude();

  /* Gets the digital atk stat */
  int getBaseCyberneticAggression();

  /* Gets the digital def stat */
  int getBaseCyberneticFortitude();

  /* Gets the void atk stat */
  int getBaseNihilAggression();

  /* Gets the void def stat */
  int getBaseNihilFortitude();

  /* Gets the health stat */
  int getBaseVitality();

  /* Gets the power points (shown as a percentage) */
  int getBaseQuantumDrive();

  /* Gets the speed stat */
  int getBaseAgility();

  /* Gets the avoidance stat */
  int getBaseLimbertude();

  /* Gets the critcial chance stat */
  int getBaseUnbearability();

  /* Sets the head equipment */
  bool setHeadEquipment(Equipment* e = NULL);

  /* Sets the left arm equipmet */
  bool setLeftArmEquipment(Equipment* e = NULL);

  /* Sets the right arm equipment */
  bool setRightArmEquipment(Equipment* e = NULL);

  /* Sets the leg equipment */
  bool setLegEquipment(Equipment* e = NULL);

  /* Sets the torso equipment */
  bool setTorsoEquipment(Equipment* e = NULL);

  /* Sets the person's category */
  void setCategory(Category* c);

  /* Set's the person's experience */
  void setExp(int);

  /* Sets the person's level */
  void setLevel(int);

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

  /* Sets the given status ailment */
  void setStatusAilment(bool b, int ailment_index);

  /* Sets the person's race */
  void setRace(Race* r);

  /* Sets the persons word rank */
  void setRank(QString rank);

  /* Sets the RENDERING flag */
  void setRendering(bool b);

  /* Sets the fire attack stat */
  void setThermalAggression(int value);

  /* Sets the fire defense stat */
  void setThermalFortitude(int value);

  /* Sets the ice attack stat */
  void setPolarAggression(int value);

  /* Sets the ice defense stat */
  void setPolarFortitude(int value);

  /* Sets the forest attack stat */
  void setPrimalAggression(int value);

  /* Sets the forest defense stat */
  void setPrimalFortitude(int value);

  /* Sets the electric attack stat */
  void setChargedAggression(int value);

  /* Sets the electric defense stat */
  void setChargedFortitude(int value);

  /* Sets the digital attack stat */
  void setCyberneticAggression(int value);

  /* Sets the digital defense stat */
  void setCyberneticFortitude(int value);

  /* Sets the void attack stat */
  void setNihilAggression(int value);

  /* Sets the void defense stat */
  void setNihilFortitude(int value);

  /* Sets the health stat */
  void setVitality(int value);

  /* Sets the power points (shown as percentage) */
  void setQuantumDrive(int value);

  /* Sets the speed stat */
  void setAgility(int value);

  /* Sets the dodge stat */
  void setLimbertude(int value);

  /* Sets the critical chance stat */
  void setUnbearability(int value);

  /* Sets the temp fire attack stat */
  void setTempThermalAggression(int value);

  /* Sets the temp fire def stat */
  void setTempThermalFortitude(int value);

  /* Sets the temp ice atk stat */
  void setTempPolarAggression(int value);

   /* Sets the temp ice def stat */
  void setTempPolarFortitude(int value);

  /* Sets the temp forest atk stat */
  void setTempPrimalAggression(int value);

  /* Sets the temp forest def stat */
  void setTempPrimalFortitude(int value);

  /* Sets the temp electric atk stat */
  void setTempChargedAggression(int value);

  /* Sets the temp electric def stat */
  void setTempChargedFortitude(int value);

  /* Sets the temp digital atk stat */
  void setTempCyberneticAggression(int value);

  /* Sets the temp digital def stat */
  void setTempCyberneticFortitude(int value);

  /* Setst the temp void atk stat */
  void setTempNihilAggression(int value);

  /* Sets the temp void def stat */
  void setTempNihilFortitude(int value);

  /* Sets the temp health stat */
  void setTempVitality(int value);

  /* Sets the temp power points (shown as percentage) */
  void setTempQuantumDrive(int value);

  /* Sets the temp speed stat */
  void setTempAgility(int value);

  /* Sets the temp avoidance stat */
  void setTempLimbertude(int value);

  /* Sets the temp critical chance stat */
  void setTempUnbearability(int value);

  /* Sets the Base fire attack stat */
  void setBaseThermalAggression(int value);

  /* Sets the Base fire def stat */
  void setBaseThermalFortitude(int value);

  /* Sets the Base ice atk stat */
  void setBasePolarAggression(int value);

   /* Sets the Base ice def stat */
  void setBasePolarFortitude(int value);

  /* Sets the Base forest atk stat */
  void setBasePrimalAggression(int value);

  /* Sets the Base forest def stat */
  void setBasePrimalFortitude(int value);

  /* Sets the Base electric atk stat */
  void setBaseChargedAggression(int value);

  /* Sets the Base electric def stat */
  void setBaseChargedFortitude(int value);

  /* Sets the Base digital atk stat */
  void setBaseCyberneticAggression(int value);

  /* Sets the Base digital def stat */
  void setBaseCyberneticFortitude(int value);

  /* Setst the Base void atk stat */
  void setBaseNihilAggression(int value);

  /* Sets the Base void def stat */
  void setBaseNihilFortitude(int value);

  /* Sets the Base health stat */
  void setBaseVitality(int value);

  /* Sets the Base power points (shown as percentage) */
  void setBaseQuantumDrive(int value);

  /* Sets the Base speed stat */
  void setBaseAgility(int value);

  /* Sets the Base avoidance stat */
  void setBaseLimbertude(int value);

  /* Sets the Base critical chance stat */
  void setBaseUnbearability(int value);
};

#endif // PERSON_H
