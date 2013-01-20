/******************************************************************************
* Class Name: Person Implementation
* Date Created: December 2nd, 2012
* Inheritance: QWidget
* Description: Holder for all the info describing a person (character)
*  
* TODO: CONSTRUCTORS TO BE FINISHED
* TODO: FLAGS FOR STATUS AILMENTS
* TODO: Experience formula and related checks [12-02-12] (multiple functions)
******************************************************************************/
#include "Game/Player/Person.h"

/* Constructs a Person object */
Person::Person(QWidget* pointer)
{
}

/* Annihilates a Person object */
Person::~Person()
{
}

/* Add equipment to the current person */
bool Person::addEquipment(Equipment* equipment)
{
    return TRUE;
}

/* Adds experience of a given amount */
void Person::addExperience(int value)
{
  experience += value;

  //TODO: Check for formula - exp [12-02-12]
  if (experience >= 90000)
    setLevel(getLevel() + 1);
}

/* Sets up total and temporary stats */
void Person::initiateForBattle()
{
  temp_thermal_aggression    = thermal_aggression;
  temp_thermal_fortitude     = thermal_fortitude;
  temp_polar_aggression      = polar_aggression;
  temp_polar_fortitude       = polar_fortitude;
  temp_primal_aggression     = primal_aggression;
  temp_primal_fortitude      = primal_fortitude;
  temp_charged_aggression    = charged_aggression;
  temp_charged_fortitude     = charged_fortitude;
  temp_cybernetic_aggression = cybernetic_aggression;
  temp_cybernetic_fortitude  = cybernetic_fortitude;
  temp_nihil_aggression      = nihil_aggression;
  temp_nihil_fortitude       = nihil_fortitude;
  temp_vitality              = vitality;
  temp_quantum_drive         = quantum_drive;
  temp_agility               = agility;
  temp_limbertude            = limbertude;
  temp_unbearability         = unbearability;
}

/* Checks the given status ailment status */
bool Person::isStatusAilment(QString s)
{
  //TODO: Finish status ailments [12-02-12]
  return TRUE;
}

/* Use a given item */
bool useItem(Item* used_item)
{
    return TRUE;
}

/* Returns the available actions of the person */
QVector<Action*>& Person::getAvailableActions()
{
  return action_list;
}

/* Evaluates the RENDERING flag */
bool Person::getRendering()
{
  return RENDERING;
}


/* Gets the person's category */
Category* Person::getCategory()
{
  return character_class;
}


/* Gets the person's race */
Race* Person::getRace()
{
  return character_race;
}


/* Gets the head equipment */
Equipment* Person::getHeadEquipment()
{
  return head;
}


/* Gets the left arm equipment */
Equipment* Person::getLeftArmEquipment()
{
  return &left_arm;
}


/* Gets the right arm equipment */
Equipment* Person::getRightArmEquipment()
{
  return &right_arm;
}


/* Gets the leg equipment */
Equipment* Person::getLegEquipment()
{
  return &legs;
}


/* Gets the torso equpiment */
Equipment* Person::getTorsoEquipment()
{
  return &torso;
}


/* Gets the person's experience */
int Person::getExp()
{
  return experience;
}


/* Gets the person's level */
int Person::getLevel()
{
  return level;
}


/* Gets the persons name (set in construction and never again) */
QString Person::getName()
{
  return name;
}


/* Gets the person's word rank */
QString Person::getRank()
{
  return rank;
}


/* Gets the first person sprite */
Sprite Person::getFirstPerson()
{
  return first_person;
}


/* Gets the third person person sprite */
Sprite Person::getThirdPerson()
{
  return third_person;
}


/* Gets the fire attack stat */
int Person::getThermalAggression()
{
  return thermal_aggression;
}


/* Gets the fire defense stat */
int Person::getThermalFortitude()
{
  return thermal_fortitude;
}


/* Gets the ice attack stat */
int Person::getPolarAggression()
{
  return polar_aggression;
}


/* Gets the ice defense stat */
int Person::getPolarFortitude()
{
  return polar_fortitude;
}


/* Gets the forest attack stat */
int Person::getPrimalAggression()
{
  return primal_aggression;
}


/* Gets the forest defense stat */
int Person::getPrimalFortitude()
{
  return primal_fortitude;
}


/* Gets the electric attack stat */
int Person::getChargedAggression()
{
  return charged_aggression;
}


/* Gets the electric defense stat */
int Person::getChargedFortitude()
{
  return charged_fortitude;
}


/* Gets the digital attack stat */
int Person::getCyberneticAggression()
{
  return cybernetic_aggression;
}


/* Gets the digital defense stat */
int Person::getCyberneticFortitude()
{
  return cybernetic_fortitude;
}


/* Gets the void attack stat */
int Person::getNihilAggression()
{
  return nihil_aggression;
}


/* Gets the void defense stat */
int Person::getNihilFortitude()
{
  return nihil_fortitude;
}


/* Gets the health stat */
int Person::getVitality()
{
  return vitality;
}


/* Gets the power points (shown as a percentage) */
int Person::getQuantumDrive()
{
  return quantum_drive;
}


/* Gets the speed stat */
int Person::getAgility()
{
  return agility;
}


/* Gets the dodge stat */
int Person::getLimbertude()
{
  return limbertude;
}


/* Gets the critical chance stat */
int Person::getUnbearability()
{
  return unbearability;
}


/* Gets the fire atk stat */
int Person::getTempThermalAggression()
{
  return temp_thermal_aggression;
}


/* Gets the fire def stat */
int Person::getTempThermalFortitude()
{
  return temp_thermal_fortitude;
}


/* Gets the ice atk stat */
int Person::getTempPolarAggression()
{
  return temp_polar_aggression;
}


/* Gets the ice def stat */
int Person::getTempPolarFortitude()
{
  return temp_polar_fortitude;
}


/* Gets the forest atk stat */
int Person::getTempPrimalAggression()
{
  return temp_primal_aggression;
}


/* Gets the forest def stat */
int Person::getTempPrimalFortitude()
{
  return temp_primal_fortitude;
}


/* Gets the electric atk stat */
int Person::getTempChargedAggression()
{
  return temp_charged_aggression;
}


/* Gets the electric def stat */
int Person::getTempChargedFortitude()
{
  return temp_charged_fortitude;
}


/* Gets the digital atk stat */
int Person::getTempCyberneticAggression()
{
  return temp_cybernetic_aggression;
}


/* Gets the digital def stat */
int Person::getTempCyberneticFortitude()
{
  return temp_cybernetic_fortitude;
}


/* Gets the void atk stat */
int Person::getTempNihilAggression()
{
  return temp_nihil_aggression;
}


/* Gets the void def stat */
int Person::getTempNihilFortitude()
{
  return temp_nihil_fortitude;
}


/* Gets the health stat */
int Person::getTempVitality()
{
  return temp_vitality;
}


/* Gets the power points (shown as a percentage) */
int Person::getTempQuantumDrive()
{
  return temp_quantum_drive;
}


/* Gets the speed stat */
int Person::getTempAgility()
{
  return temp_agility;
}


/* Gets the avoidance stat */
int Person::getTempLimbertude()
{
  return temp_limbertude;
}


/* Gets the critcial chance stat */
int Person::getTempUnbearability()
{
  return temp_unbearability;
}


/* Sets the head equipment */
bool Person::setHeadEquipment(Equipment* new_equipment)
{
  if (head != NULL)
    return false;
  head = new_equipment;
  return true;
}


/* Sets the left arm equipmet */
bool Person::setLeftArmEquipment(Equipment* new_equipment)
{
  //if (left_arm != NULL)
  //  return false;
  //left_arm = new_equipment;

  return true;
}


/* Sets the right arm equipment */
bool Person::setRightArmEquipment(Equipment* new_equipment)
{
  //if (right_arm != NULL)
  //  return false;
  //right_arm = new_equipment;
  return true;
}


/* Sets the leg equipment */
bool Person::setLegEquipment(Equipment* new_equipment)
{
  //if (legs != NULL)
  //  return false;
  //legs = new_equipment;
  return true;
}


/* Sets the torso equipment */
bool Person::setTorsoEquipment(Equipment* new_equipment)
{
  //if (torso != NULL)
  //  return false;
  //torso = new_equipment;
  return true;
}


/* Sets the person's category */
void Person::setCategory(Category* new_category)
{
  character_class = new_category;
}


/* Set's the person's experience */
void Person::setExp(int exp)
{
  experience = exp;

  //TODO: Correlate to experience formula, change level accordingly [12-02-12]
}


/* Sets (or resets) the given status ailment */
void Person::setStatusAilment(bool b, int ailment_index)
{
  //TODO: FINISH STATUS AILMENTS
}


/* Sets the person's level */
void Person::setLevel(int new_level)
{
  if (new_level <= 126 && new_level >= 0)
    level = new_level;
}


/* Sets the person's race */
void Person::setRace(Race* new_race)
{
  character_race = new_race;
}


/* Sets the persons word rank */
void Person::setRank(QString new_rank)
{
  rank = new_rank;
}


/* Sets the RENDERING flag */
void Person::setRendering(bool b)
{
  RENDERING = b;
}


/* Sets the fire attack stat */
void Person::setThermalAggression(int value)
{
  thermal_aggression = value;
}


/* Sets the fire defense stat */
void Person::setThermalFortitude(int value)
{
  thermal_fortitude = value;
}


/* Sets the ice attack stat */
void Person::setPolarAggression(int value)
{
  polar_aggression = value;
}


/* Sets the ice defense stat */
void Person::setPolarFortitude(int value)
{
  polar_fortitude = value;
}


/* Sets the forest attack stat */
void Person::setPrimalAggression(int value)
{
  primal_aggression = value;
}


/* Sets the forest defense stat */
void Person::setPrimalFortitude(int value)
{
  primal_fortitude = value;
}


/* Sets the electric attack stat */
void Person::setChargedAggression(int value)
{
  charged_aggression = value;
}


/* Sets the electric defense stat */
void Person::setChargedFortitude(int value)
{
  charged_fortitude = value;
}


/* Sets the digital attack stat */
void Person::setCyberneticAggression(int value)
{
  cybernetic_aggression = value;
}


/* Sets the digital defense stat */
void Person::setCyberneticFortitude(int value)
{
  cybernetic_fortitude = value;
}


/* Sets the void attack stat */
void Person::setNihilAggression(int value)
{
  nihil_aggression = value;
}


/* Sets the void defense stat */
void Person::setNihilFortitude(int value)
{
  nihil_fortitude = value;
}


/* Sets the health stat */
void Person::setVitality(int value)
{
  vitality = value;
}

/* Sets the power points (shown as percentage) */
void Person::setQuantumDrive(int value)
{
  quantum_drive = value;
}

/* Sets the speed stat */
void Person::setAgility(int value)
{
  agility = value;
}

/* Sets the dodge stat */
void Person::setLimbertude(int value)
{
  limbertude = value;
}

/* Sets the critical chance stat */
void Person::setUnbearability(int value)
{
  unbearability = value;
}

/* Sets the temp fire attack stat */
void Person::setTempThermalAggression(int value)
{
  temp_thermal_aggression = value;
}

/* Sets the temp fire def stat */
void Person::setTempThermalFortitude(int value)
{
  temp_thermal_fortitude = value;
}

/* Sets the temp ice atk stat */
void Person::setTempPolarAggression(int value)
{
  temp_polar_aggression = value;
}

  /* Sets the temp ice def stat */
void Person::setTempPolarFortitude(int value)
{
  temp_polar_fortitude = value;
}

/* Sets the temp forest atk stat */
void Person::setTempPrimalAggression(int value)
{
  temp_primal_aggression = value;
}

/* Sets the temp forest def stat */
void Person::setTempPrimalFortitude(int value)
{
  temp_primal_fortitude = value;
}

/* Sets the temp electric atk stat */
void Person::setTempChargedAggression(int value)
{
  temp_charged_aggression = value;
}

/* Sets the temp electric def stat */
void Person::setTempChargedFortitude(int value)
{
  temp_charged_fortitude = value;
}

/* Sets the temp digital atk stat */
void Person::setTempCyberneticAggression(int value)
{
  temp_cybernetic_aggression = value;
}

/* Sets the temp digital def stat */
void Person::setTempCyberneticFortitude(int value)
{
  temp_cybernetic_aggression = value;
}

/* Setst the temp void atk stat */
void Person::setTempNihilAggression(int value)
{
  temp_nihil_aggression = value;
}

/* Sets the temp void def stat */
void Person::setTempNihilFortitude(int value)
{
  temp_nihil_fortitude = value;
}

/* Sets the temp health stat */
void Person::setTempVitality(int value)
{
  temp_vitality = value;
}

/* Sets the temp power points (shown as percentage) */
void Person::setTempQuantumDrive(int value)
{
  temp_quantum_drive = value;
}

/* Sets the temp speed stat */
void Person::setTempAgility(int value)
{
  temp_agility = value;
}

/* Sets the temp avoidance stat */
void Person::setTempLimbertude(int value)
{
  temp_limbertude = value;
}

/* Sets the temp critical chance stat */
void Person::setTempUnbearability(int value)
{
  temp_unbearability = value;
}
