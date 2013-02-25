/******************************************************************************
* Class Name: Person Implementation
* Date Created: December 2nd, 2012
* Inheritance: QWidget
* Description: Holder for all the info describing a person (character)
*
* TODO: FINISH STATUS AILMENTS [02-08-13]
* TODO: skill_list skills, [01-25-13]
* TODO: Future class multiplier for level ups [01-25-13]
******************************************************************************/
#include <QDebug>
#include <cmath>
#include <algorithm>

#include "Game/Player/Person.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Primary person constructor (requires all values)
 *
 * Inputs:
 */
Person::Person(QString name, Race* race, Category* category, QString prim,
                QString secd, QWidget* pointer)
{
  /* Prepares non-battle related statistics for the person */
  setName(name);
  setRace(race);
  setCategory(category);
  setPrimary(prim);
  setSecondary(secd);

  /* Prepares all the battle-related statistics for the person */
  setBaseStats();
  setupStats();
  setTemporaryStats();

  /* Sets equipment & sprite pointers to NULL initially */
  setHeadEquipment();
  setLeftArmEquipment();
  setRightArmEquipment();
  setTorsoEquipment();
  setLegEquipment();
  setFirstPerson();
  setThirdPerson();
}

/*
 * Description: Person destructor
 */
Person::~Person()
{
    delete head;
    delete left_arm;
    delete right_arm;
    delete torso;
    delete legs;
    delete cat;
    delete race;
    delete first_person;
    delete third_person;
    setHeadEquipment();
    setLeftArmEquipment();
    setRightArmEquipment();
    setTorsoEquipment();
    setLegEquipment();
    setCategory(NULL);
    setRace(NULL);
    setFirstPerson(NULL);
    setThirdPerson(NULL);
}

/*=============================================================================
 * FUNCTIONS
 *============================================================================*/

/*
 * Descriptions: Max function for level up routine
 *
 * Inputs: a, b - two values
 * Output: int - larger of the values
 */
int Person::personMax(int a, int b)
{
    if (a < b) return b;
    return a;
}

/*
 * Description: Sets up the regular stats of the person, for initial
 *              construction, based off the base stats
 *
 * Inputs: none
 * Output: none
 */
void Person::setupStats()
{
    setPhysicalAggression(getBasePhysicalAggression());
    setPhysicalFortitude(getBasePhysicalFortitude());
    setThermalAggression(getBaseThermalAggression());
    setThermalFortitude(getBaseThermalFortitude());
    setPolarAggression(getBasePolarAggression());
    setPolarFortitude(getBasePolarFortitude());
    setPrimalAggression(getBasePrimalAggression());
    setPrimalFortitude(getBasePrimalFortitude());
    setChargedAggression(getBaseChargedAggression());
    setChargedFortitude(getBaseChargedFortitude());
    setCyberneticAggression(getBaseCyberneticAggression());
    setCyberneticFortitude(getBaseCyberneticFortitude());
    setNihilAggression(getBaseNihilAggression());
    setNihilFortitude(getBaseNihilFortitude());
    setVitality(getBaseVitality());
    setQuantumDrive(getBaseQuantumDrive());
    setMomentum(getBaseMomentum());
    setLimbertude(getBaseLimbertude());
}

/*
 * Description: Adds an amount to the experience of the person and calls
 *              setLevel() if necessary to deal with level ups accordingly
 *              Will deal with multiple level ups.
 *
 * Note: The formula for experience in mathematical form is currently:
 *
 *       let x = player's level + 1
 *        f(x) = minimum experience needed to reach level x
 *
 *        f(x) = floor(x + 300 * 2 ^ 1/7), 0 < x <= 126
 *
 * Inputs: value - amount of experience to be added
 * Output: none
 */
void Person::addExperience(uint value)
{
 /* Finds the experience value of next level and if the current
   * experience value of the person exceeds this, level up the
   * character until its experience matches the correct level */

  uint l = getLevel();
  uint y = 0;
  experience += value;

  do
  {
    y = floor(l + 300 * pow(2, l / 7));
    if (experience >= y)
      if ((l + 1 > 0) && (l + 1 < 126))
              setLevel(l++);
  } while (experience >= y);
}

/*
 * Description: Sets up the base statistics based on the character class
 *              which the character is set to, plus the additional bonuses
 *              from the Race class.
 *
 * Inputs: none
 * Output: none
 */
void Person::setBaseStats()
{
    setBasePhysicalAggression(cat->getPhysicalAggression() +
                              race->getPhysicalAggression());
    setBasePhysicalFortitude(cat->getPhysicalFortitude() +
                             race->getPhysicalFortitude());
    setBaseThermalAggression(cat->getThermalAggression() +
                             race->getThermalAggression());
    setBaseThermalFortitude(cat->getThermalFortitude() +
                            race->getThermalFortitude());
    setBasePolarAggression(cat->getPolarAggression() +
                           race->getPolarAggression());
    setBasePolarFortitude(cat->getPolarFortitude() +
                          race->getPolarFortitude());
    setBasePrimalAggression(cat->getPrimalAggression() +
                            race->getPrimalAggression());
    setBasePrimalFortitude(cat->getPrimalFortitude() +
                           race->getPrimalFortitude());
    setBaseChargedAggression(cat->getChargedAggression() +
                             race->getChargedAggression());
    setBaseChargedFortitude(cat->getChargedFortitude() +
                            race->getChargedFortitude());
    setBaseCyberneticAggression(cat->getCyberneticAggression() +
                                race->getCyberneticAggression());
    setBaseCyberneticFortitude(cat->getCyberneticFortitude() +
                               race->getCyberneticFortitude());
    setBaseNihilAggression(cat->getNihilAggression() +
                           race->getNihilAggression());
    setBaseNihilFortitude(cat->getNihilFortitude() +
                          race->getNihilFortitude());
    setBaseVitality(cat->getVitality() + race->getVitality());
    setBaseQuantumDrive(cat->getQuantumDrive() + race->getQuantumDrive());
    setBaseMomentum(cat->getMomentum() + race->getMomentum());
    setBaseLimbertude(cat->getLimbertude() + race->getLimbertude());
}

/*
 * Description: Sets up temporary statistics: for a battle, and
 *              other preparations
 *
 * Inputs: none
 * Output: none
 */
void Person::setTemporaryStats()
{
    /* Sets up temporary stats */
    setTempPhysicalAggression(getPhysicalAggression());
    setTempPhysicalFortitude(getPhysicalFortitude());
    setTempThermalAggression(getThermalAggression());
    setTempThermalFortitude(getThermalFortitude());
    setTempPolarAggression(getPolarAggression());
    setTempPolarFortitude(getPolarFortitude());
    setTempPrimalAggression(getPrimalAggression());
    setTempPrimalFortitude(getPrimalFortitude());
    setTempChargedAggression(getChargedAggression());
    setTempChargedFortitude(getChargedFortitude());
    setTempCyberneticAggression(getCyberneticAggression());
    setTempCyberneticFortitude(getCyberneticFortitude());
    setTempNihilAggression(getNihilAggression());
    setTempNihilFortitude(getNihilFortitude());
    setTempVitality(getVitality());
    setTempQuantumDrive(getQuantumDrive());
    setTempMomentum(getMomentum());
    setTempLimbertude(getLimbertude());
}

/*
 * Description: Uses an item
 *
 * Inputs: used_item - pointer of an item to be used
 * Output: bool - tue if item used successfully
 */
bool Person::useItem(Item* used_item)
{
    return TRUE;
}

/*
 * Description: Toggles the value of a status ailment flag
 *
 * Inputs: StatusAilment flag
 * Output: none
 */
void Person::toggleAilment(StatusAilment flags)
{
    setAilment(flags, !getAilment(flags));
}

/*
 * Description: Toggles the value of a status buff flag
 *
 * Inputs: StatusBuff flag
 * Output: none
 */
void Person::toggleBuff(StatusBuff flags)
{
    setBuff(flags, !getBuff(flags));
}

/*
 * Description: Togges the value of a person ailment
 *
 * Inputs: PersonState flag
 * Output: none
 */
void Person::togglePersonFlag(PersonState flags)
{
    setPersonFlag(flags, !getPersonFlag(flags));
}

/*
 * Description: Gets a list of available skills for current person
 *
 * Inputs: none
 * Output: vector of skill pointers - pointers to skill the player can take
 */
QVector<Skill*>& Person::getAvailableSkills()
{
  // TODO: Setup list of skills for person [01-25-13]
  return skill_list;
}


/*
 * Description: Returns the person's class
 *
 * Inputs: none
 * Output: Equipment* - pointer to class of the person
 */
Category* Person::getCategory()
{
  return cat;
}


/*
 * Description: Returns the person's race
 *
 * Inputs: none
 * Output: Equipment* - pointer to race of the person
 */
Race* Person::getRace()
{
  return race;
}

/*
 * Description: Returns a pointer to the equipment attached to the head
 *
 * Inputs: none
 * Output: Equipment* - pointer to the head equipment
 */
Equipment* Person::getHeadEquipment()
{
  return head;
}

/*
 * Description: Returns a pointer to the equipment attached to the left arm
 *
 * Inputs: none
 * Output: Equipment* - pointer to the left arm equipment
 */
Equipment* Person::getLeftArmEquipment()
{
  return left_arm;
}

/*
 * Description: Retuns a pointer to the equipment attached to the right arm
 *
 * Inputs: none
 * Output: Equipment* - pointer to the right arm equipment
 */
Equipment* Person::getRightArmEquipment()
{
  return right_arm;
}

/*
 * Description: Returns a pointer to the equipment attached to the leg
 *
 * Inputs: none
 * Output: Equipment* - pointer to the leg equipment
 */
Equipment* Person::getLegEquipment()
{
  return legs;
}

/*
 * Description: Returns a pointer to the equpiment attached to the torso
 *
 * Inputs: none
 * Output: Equipment* - pointer to the torso equipment
 */
Equipment* Person::getTorsoEquipment()
{
  return torso;
}

/*
 * Description: Returns value of the person's current experience
 *
 * Inputs: none
 * Output: int - value of person's current experience
 */
uint Person::getExp()
{
  return experience;
}

/*
 * Description: Returns the awarded experience on defeat of person
 *
 * Inputs: none
 * Output: int - value of person's exp drop
 */
uint Person::getExpLoot()
{
  return experience_drop;
}


/*
 * Description: Returns the awarded credits on defeat of a person
 *
 * Inputs: none
 * Output: int - value of person's credit drop
 */
uint Person::getCreditLoot()
{
  return credit_drop;
}


/*
 * Description: Returns the items on defeat of a person
 *
 * Inputs: none
 * Output: QVector<Item> - vector of item drops.
 */
QVector<Item> Person::getItemLoot()
{
  return item_drops;
}

/*
 * Description: Returns value of the person's current level
 *
 * Inputs: none
 * Output: uint - value of person's level
 */
uint Person::getLevel()
{
  return level;
}

/*
 * Description: Returns a string of the person's name
 *
 * Inputs: none
 * Output: QString - person's name
 */
QString Person::getName()
{
  return name;
}

/*
 * Description: Returns string of the person's elemental strength
 *              Legend: "<Element><Level of Proificiency(A-C)>"
 * Inputs: none
 * Output: QString - person's primary elemental strength and proificiency
 */
QString Person::getPrimary()
{
    return primary;
}

/*
 * Description: Returns string of the person's secondary elemental strength
                Legend: "<Element><Level of Proificiency(C-F)>"
 *
 * Inputs: none
 * Output: QString - person's secondary elemental strength and proificiency
 */
QString Person::getSecondary()
{
    return secondary;
}

/*
 * Description: Returns string of the person's rank
 *
 * Inputs: none
 * Output: QString - person's rank
 */
QString Person::getRank()
{
  return rank;
}

/*
 * Description: Returns the first person view of the person's sprite
 *
 * Inputs: none
 * Output: Sprite - first person view of the sprite of the person
 */
Sprite* Person::getFirstPerson()
{
  return first_person;
}

/*
 * Description: Returns the third person view of the person's sprite
 *
 * Inputs: none
 * Output: Sprite - third person view of the sprite of the person
 */
Sprite* Person::getThirdPerson()
{
  return third_person;
}

/*
 * Description: Evaluates a given status ailment flag
 *
 * Inputs: Status ailment flag
 * Output: Boolean evaluation of the flag
 */
const bool Person::getAilment(StatusAilment flags)
{
  return ailment_set.testFlag(flags);
}

/*
 * Description: Evaluates a given status buff flag
 *
 * Inputs: Status buff flag
 * Output: Boolean evaluation of the flag
 */
const bool Person::getBuff(StatusBuff flags)
{
  return buff_set.testFlag(flags);
}

/*
 * Description: Evaluates a given person state flag
 *
 * Inputs: Person state flag
 * Output: Boolean evaluation of the flag
 */
const bool Person::getPersonFlag(PersonState flags)
{
  return state_set.testFlag(flags);
}

/*
 * Description: Returns value of the physical attack stat
 *
 * Inputs: none
 * Output: uint - value of the physical attack stat
 */
uint Person::getPhysicalAggression()
{
  return physical_aggression;
}

/*
 * Description: Returns value of the physical defens stat
 *
 * Inputs: none
 * Output: uint - value of the physical attack stat
 */
uint Person::getPhysicalFortitude()
{
  return physical_fortitude;
}

/*
 * Description: Returns value of the thermal attack stat
 *
 * Inputs: none
 * Output: uint - value of the thermal attack stat
 */
uint Person::getThermalAggression()
{
  return thermal_aggression;
}

/*
 * Description: Returns value of the thermal fortitude stat
 *
 * Inputs: none
 * Output: uint - value of the thermal defense stat
 */
uint Person::getThermalFortitude()
{
  return thermal_fortitude;
}

/*
 * Description: Returns value of the polar attack stat
 *
 * Inputs: none
 * Output: uint - value of polar attack stat
 */
uint Person::getPolarAggression()
{
  return polar_aggression;
}

/*
 * Description: Returns value of the polar defense stat
 *
 * Inputs: none
 * Output: uint - value of the polar defense stat
 */
uint Person::getPolarFortitude()
{
  return polar_fortitude;
}

/*
 * Description: Returns value of the primal attack stat
 *
 * Inputs: none
 * Output: uint - value of the primal attack stat
 */
uint Person::getPrimalAggression()
{
  return primal_aggression;
}

/*
 * Description: Returns value of the primal defense stat
 *
 * Inputs: none
 * Output: uint - vaue of the primal attack stat
 */
uint Person::getPrimalFortitude()
{
  return primal_fortitude;
}

/*
 * Description: Returns value of the charged attack stat
 *
 * Inputs: none
 * Output: uint - value of the charged attack stat
 */
uint Person::getChargedAggression()
{
  return charged_aggression;
}

/*
 * Description: Returns value of the charged defense stat
 *
 * Inputs: none
 * Output: uint - value of the charged defense stat
 */
uint Person::getChargedFortitude()
{
  return charged_fortitude;
}

/*
 * Description: Returns value of the cybernetic attack stat
 *
 * Inputs: none
 * Output: uint - value of the cybernetic attack stat
 */
uint Person::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/*
 * Description: Returns value of the cybernetic defense stat
 *
 * Inputs: none
 * Output: uint - value of the cybernetic defense stat
 */
uint Person::getCyberneticFortitude()
{
  return cybernetic_fortitude;
}

/*
 * Description: Returns value of the nihil attack stat
 *
 * Inputs: none
 * Output: uint - value of the nihil attack stat
 */
uint Person::getNihilAggression()
{
  return nihil_aggression;
}

/*
 * Description: Returns value of the nihil defense stat
 *
 * Inputs: none
 * Output: uint - value of the nihil defense stat
 */
uint Person::getNihilFortitude()
{
  return nihil_fortitude;
}

/*
 * Description: Returns value of the vitality stat
 *
 * Inputs: none
 * Output: uint - value of the vitality stat
 */
uint Person::getVitality()
{
  return vitality;
}

/*
 * Description: Returns value of quantum drive stat
 *
 * Inputs: none
 * Output: uint - value of the quantum drive stat
 */
uint Person::getQuantumDrive()
{
  return quantum_drive;
}

/*
 * Description: Returns value of momentum stat
 *
 * Inputs: none
 * Output: uint - value of the momentum stat
 */
uint Person::getMomentum()
{
  return momentum;
}

/*
 * Description: Returns value of the limbertude stat
 *
 * Inputs: none
 * Output: uint - value of the limbertude stat
 */
uint Person::getLimbertude()
{
  return limbertude;
}

/*
 * Description: Returns value of the unbearability stat
 *
 * Inputs: none
 * Output: uint - value of the unbearability stat
 */
uint Person::getUnbearability()
{
  return unbearability;
}

/*
 * Description: Returns value of the temp phys atk stat
 *
 * Inputs: none
 * Output: uint - value of the temp phys atk stat
 */
uint Person::getTempPhysicalAggression()
{
  return temp_physical_aggression;
}

/*
 * Description: Returns value of the temp phys def stat
 *
 * Inputs: none
 * Output: uint - value of the temp phys atk stat
 */
uint Person::getTempPhysicalFortitude()
{
  return temp_physical_fortitude;
}


/*
 * Description: Returns value of the temp thermal attack stat
 *
 * Inputs: none
 * Output: uint - value of the temp thermal attack stat
 */
uint Person::getTempThermalAggression()
{
  return temp_thermal_aggression;
}

/*
 * Description: Returns value of the temp thermal defense stat
 *
 * Inputs: none
 * Output: uint - value of the temp thermal fortitude stat
 */
uint Person::getTempThermalFortitude()
{
  return temp_thermal_fortitude;
}

/*
 * Description: Returns value of the temp polar attack stat
 *
 * Inputs: none
 * Output: uint - value of the temp polar attack stat
 */
uint Person::getTempPolarAggression()
{
  return temp_polar_aggression;
}

/*
 * Description: Returns value of the temp polar defense stat
 *
 * Inputs: none
 * Output: uint - value of the temp polar defense stat
 */
uint Person::getTempPolarFortitude()
{
  return temp_polar_fortitude;
}

/*
 * Description: Returns value of the temp primal attack stat
 *
 * Inputs: none
 * Output: uint - value of the temp primal attack stat
 */
uint Person::getTempPrimalAggression()
{
  return temp_primal_aggression;
}

/*
 * Description: Returns value of the temp primal defene stat
 *
 * Inputs: none
 * Output: uint - value of the temp primal defense stat
 */
uint Person::getTempPrimalFortitude()
{
  return temp_primal_fortitude;
}

/*
 * Description: Returns vlue of the temp charged attack stat
 *
 * Inputs: none
 * Output: uint - value of the temp charged attack stat
 */
uint Person::getTempChargedAggression()
{
  return temp_charged_aggression;
}

/*
 * Description: Returns value of the temp charged defense stat
 *
 * Inputs: none
 * Output: uint - value of the temp charged defense stat
 */
uint Person::getTempChargedFortitude()
{
  return temp_charged_fortitude;
}

/*
 * Description: Returns value of the temp cybernetic attack stat
 *
 * Inputs: none
 * Output: uint - value of the temp charged attack stat
 */
uint Person::getTempCyberneticAggression()
{
  return temp_cybernetic_aggression;
}

/*
 * Description: Returns value of the temp cybernetic defense stat
 *
 * Inputs: none
 * Output: uint - value of the temp cybernetc defense stat
 */
uint Person::getTempCyberneticFortitude()
{
  return temp_cybernetic_fortitude;
}

/*
 * Description: Returns value of the temp nihil attack stat
 *
 * Inputs: none
 * Output: uint - value of the temp nihil attack stat
 */
uint Person::getTempNihilAggression()
{
  return temp_nihil_aggression;
}

/*
 * Description: Returns value of the temp nihil defense stat
 *
 * Inputs: none
 * Output: uint - value of the temp nihil defense stat
 */
uint Person::getTempNihilFortitude()
{
  return temp_nihil_fortitude;
}

/*
 * Description: Returns value of the temp vitality stat
 *
 * Inputs: none
 * Output: uint - value of the temp vitality stat
 */
uint Person::getTempVitality()
{
  return temp_vitality;
}

/*
 * Description: Returns value of the temp quantum drive stat
 *
 * Inputs: none
 * Output: uint - value of the temp quantum drive sta
 */
uint Person::getTempQuantumDrive()
{
  return temp_quantum_drive;
}

/*
 * Description: Returns value of the temp momentum stat
 *
 * Inputs: none
 * Output: uint - value of the temp momentum stat
 */
uint Person::getTempMomentum()
{
  return temp_momentum;
}

/*
 * Description: Returns value of the temp limbertude stat
 *
 * Inputs: none
 * Output: uint - value of the temp limbertude stat
 */
uint Person::getTempLimbertude()
{
  return temp_limbertude;
}

/*
 * Description: Returns value of the temp unbearability stat
 *
 * Inputs: none
 * Output: uint - value of the temp unbearability stat
 */
uint Person::getTempUnbearability()
{
  return temp_unbearability;
}

/*
 * Description: Returns value of the base physical attack stat
 *
 * Inputs: none
 * Output: uint - value of the base physical attack stat
 */
uint Person::getBasePhysicalAggression()
{
  return base_physical_aggression;
}

/*
 * Description: Returns value of the base physical defense stat
 *
 * Inputs: none
 * Output: uint - value of the base physical defense stat
 */
uint Person::getBasePhysicalFortitude()
{
  return base_physical_fortitude;
}

/*
 * Description: Returns value of the base thermal attack stat
 *
 * Inputs: none
 * Output: uint - value of the base thermal defense stat
 */
uint Person::getBaseThermalAggression()
{
  return base_thermal_aggression;
}

/*
 * Description: Returns value of the base thermal defense stat
 *
 * Inputs: none
 * Output: uint - value of the base thermal defense stat
 */
uint Person::getBaseThermalFortitude()
{
  return base_thermal_fortitude;
}

/*
 * Description: Returns value of the base polar attack stat
 *
 * Inputs: none
 * Output: uint - value of the base polar attack stat
 */
uint Person::getBasePolarAggression()
{
  return base_polar_aggression;
}

/*
 * Description: Returns value of the base polar defense stat
 *
 * Inputs: none
 * Output: uint - value of the base polar defense stat
 */
uint Person::getBasePolarFortitude()
{
  return base_polar_fortitude;
}

/*
 * Description: Returns value of the base primal attack stat
 *
 * Inputs: none
 * Output: uint - value of the base primal attack stat
 */
uint Person::getBasePrimalAggression()
{
  return base_primal_aggression;
}

/*
 * Description: Returns value of the base primal defense stat
 *
 * Inputs: none
 * Output: uint - value of the base primal defense stat
 */
uint Person::getBasePrimalFortitude()
{
  return base_primal_fortitude;
}

/*
 * Description: Returns value of the base charged attack stat
 *
 * Inputs: none
 * Output: uint - value of the base charged attack stat
 */
uint Person::getBaseChargedAggression()
{
  return base_charged_aggression;
}

/*
 * Description: Returns value of the base charged defense stat
 *
 * Inputs: none
 * Output: uint - value of the base charged defense stat
 */
uint Person::getBaseChargedFortitude()
{
  return base_charged_fortitude;
}

/*
 * Description: Returns value of the base cybernetic attack stat
 *
 * Inputs: none
 * Output: uint - value of the base cybnetic attack stat
 */
uint Person::getBaseCyberneticAggression()
{
  return base_cybernetic_aggression;
}

/*
 * Description: Returns value of the base cybernetic defense stat
 *
 * Inputs: none
 * Output: uint - vale of the base cybernetic defense stat
 */
uint Person::getBaseCyberneticFortitude()
{
  return base_cybernetic_fortitude;
}

/*
 * Description: Returns value of the base nihil attack stat
 *
 * Inputs: none
 * Output: uint - value of the base nihil attack stat
 */
uint Person::getBaseNihilAggression()
{
  return base_nihil_aggression;
}

/*
 * Description: Returns value of the base nihil defense stat
 *
 * Inputs: none
 * Output: uint - value of the base nihil defense stat
 */
uint Person::getBaseNihilFortitude()
{
  return base_nihil_fortitude;
}

/*
 * Description: Returns value of the base vitality stat
 *
 * Inputs: none
 * Output: uint - value of the base vitality stat
 */
uint Person::getBaseVitality()
{
  return base_vitality;
}

/*
 * Description: Returns value of the base quantum drive stat
 *
 * Inputs: none
 * Output: uint - value of the base quantum drive stat
 */
uint Person::getBaseQuantumDrive()
{
  return base_quantum_drive;
}

/*
 * Description: Returns value of the base momentum stat
 *
 * Inputs: none
 * Output: uint - value of the base momentum stat
 */
uint Person::getBaseMomentum()
{
  return base_momentum;
}

/*
 * Description: Returns value of the base limbertude stat
 *
 * Inputs: none
 * Output: uint - value of the base limbertude stat
 */
uint Person::getBaseLimbertude()
{
  return base_limbertude;
}

/*
 * Description: Returns value of the base unbearability stat
 *
 * Inputs: none
 * Output: uint - value of the base unbearability stat
 */
uint Person::getBaseUnbearability()
{
  return base_unbearability;
}

/*
 * Description: Sets a given equipment on to the head of a person
 *
 * Inputs: Equipment* - set of head equipment to be equipped
 * Output: bool - true on successful equip
 */
const bool Person::setHeadEquipment(Equipment* new_equipment)
{
  if (new_equipment == NULL)
      return FALSE;
  if (new_equipment->getEquipmentFlag(Equipment::HEAD) &&
      new_equipment->getEquipmentFlag(Equipment::EQUIPPED))
  {
    head = new_equipment;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Sets a given equipment on to the left arm of a person
 *
 * Inputs: Equipment* - set of left arm equipment to be equipped
 * Output: bool - true on successful equip
 */
const bool Person::setLeftArmEquipment(Equipment* new_equipment)
{
  if (new_equipment == NULL)
    return FALSE;
  if (new_equipment->getEquipmentFlag(Equipment::TWOHAND) && right_arm == NULL)
  {
    left_arm = new_equipment;
    return TRUE;
  }
  if (new_equipment->getEquipmentFlag(Equipment::LEFTARM))
  {
    left_arm = new_equipment;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Sets a given equipment on to the right arm of a person
 *
 * Inputs: Equipment* - set of right arm equipment to be equipped
 * Output: bool - true if equip successful
 */
const bool Person::setRightArmEquipment(Equipment* new_equipment)
{
  if (new_equipment == NULL)
    return FALSE;
  if (new_equipment->getEquipmentFlag(Equipment::TWOHAND) && left_arm == NULL)
  {
    right_arm = new_equipment;
    return TRUE;
  }
  if (new_equipment->getEquipmentFlag(Equipment::RIGHTARM))
  {
    right_arm = new_equipment;
    return TRUE;
  }
  return TRUE;
}

/*
 * Description: Sets a given equipment on to the torso of a person
 *
 * Inputs: Equipment* - set of torso equipment to be equipped
 * Output: bool - true if equip successful
 */
const bool Person::setTorsoEquipment(Equipment* new_equipment)
{
  if (new_equipment == NULL)
    return FALSE;
  if (new_equipment->getEquipmentFlag(Equipment::TORSO))
  {
    torso = new_equipment;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Sets a given equipment on to the leg of a person
 *
 * Inputs: Equipment* - piece of leg equipment to be equipped
 * Output: bool - true if equip successful
 */
const bool Person::setLegEquipment(Equipment* new_equipment)
{
  if (new_equipment == NULL)
    return FALSE;
  if (new_equipment->getEquipmentFlag(Equipment::LEGS))
  {
    legs = new_equipment;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Sets the category (class) of the person
 *
 * Inputs: Category* - pointer to class to change person to
 * Output: none
 */
void Person::setCategory(Category* new_category)
{
  cat = new_category;
}

/*
 * Description: Sets the experience of the person
 * Note: Does NOT level the character, use addExp(int) for that.
 *       Use (getLevel()) to adjust the level accordingly!
 *
 * Inputs: uint - value of experience to be set
 * Output: none
 */
void Person::setExp(uint value)
{
  (value < kMAX_EXPERIENCE) ? (experience = value) :
                              (experience = kMAX_EXPERIENCE);
}

/*
 * Description: Sets the experience loot of the person
 *
 * Inputs: uint - value of experience loot to be set
 * Output: none
 */
void Person::setExpLoot(uint value)
{
  (value < kMAX_EXP_DROP) ? (experience_drop = value) :
                            (experience_drop = kMAX_EXP_DROP);
}

/*
 * Description: Sets the credit loot of a person
 *
 * Inputs: uint - value of credit loot to be set
 * Output: none
 */
void Person::setCreditLoot(uint value)
{
    (value < kMAX_CREDIT_DROP) ? (credit_drop = value) :
                                 (credit_drop = kMAX_CREDIT_DROP);
}

/*
 * Description: Sets the item drops of the person
 *
 * Inputs: QVector<Item> - vector of items the loot is to be set to.
 * Output: none
 */
void Person::setItemLoot(QVector<Item> items)
{
    item_drops.clear();
    for (int i = 0; i < items.size(); i++)
        item_drops.push_back(items[i]);
}

/*
 * Description: Sets a status ailment flag to a given boolean value
 *
 * Inputs: StatusAilment flag
 *         Boolean value to set the flag to
 * Output: none
 */
void Person::setAilment(StatusAilment flags, const bool set_value)
{
  (set_value) ? (ailment_set |= flags) : (ailment_set ^= flags);
}

/*
 * Description: Sets a status buff flag to a given boolean value
 *
 * Inputs: StatusBuff flag
 *         Boolean value to set the flag to
 * Output: none
 */
void Person::setBuff(StatusBuff flags, const bool set_value)
{
  (set_value) ? (buff_set |= flags) : (buff_set ^= flags);
}


/*
 * Description: Sets a person state flag to a given boolean value
 *
 * Inputs: PersonState flag
 *         Boolean value to set the flag to
 * Output: none
 */
void Person::setPersonFlag(PersonState flags, const bool set_value)
{
  (set_value) ? (state_set |= flags) : (state_set ^= flags);
}

/*
 * Description: Sets the level of the person, then based off the base stats,
 *              resets the person's current stats to newer, correct values
 *
 * Note [1]: All temporary statistics are unchanged in this function, thus
 *       the difference between the regular value of a given statistic and
 *       the temporary value of such a statistic will correctly correspond
 *       to the value it has been increased by, for purposes of displaying
 *       to a level up screen, etc.
 * Note [2]: This function does nothing about skills or skills the person
 *           has available to her.
 *
 * Inputs: int new_level- level to set person to
 * Output: bool - returns true if the level was adjusted
 */
const bool Person::setLevel(const uint &new_level)
{
  uint temp =      0;
  uint addr =      0; /* Temp variable used in formulae (value dm) */
  double mome =  0.10000; /* Difference for mulr for momentum */
  double limb =  0.10000; /* Difference for mulr for limbertude */
  double unbr =  0.10000; /* Difference for mulr for unbearability */
  double vita =  0.50000; /* Difference  for mulr for vitality */
  double qtmn =  0.10000; /* Difference for mulr for quantum_drive */
  double mulr =  1.15000; /* Basic D rate of increase of statistics */
  double defm =  0.74000; /* Def Mod - ratio of def increase to offense */
  double difm =  0.08000; /* Difference between curve level multiplier */
  double sinc = mulr + 4 * difm; /* S-Level increase multiplier */
  double ainc = mulr + 3 * difm; /* A-Level increase multiplier */
  double binc = mulr + 2 * difm; /* B-Level increase multiplier */
  double cinc = mulr + 1 * difm; /* C-Level increase multiplier */

  /* Iteratively update all elem stats, then update them based upon the level
   * of the person's primary curve statistic, then update them based on the
   * level of the person's secondary statistic.
   *
   * Basic formula for determing the value of an elemental stat
   *
   * x = level; f(x) = stat
   * f(x) = floor((race bonus + x) ^ curve exponent + class bonus)
   */
  for (uint i = 0; i < 3; i++)
  {
    if (i == 1)
    {
      if (primary_curve == 'S') mulr = sinc + difm;
      if (primary_curve == 'A') mulr = ainc + difm;
      if (primary_curve == 'B') mulr = binc + difm;
      if (primary_curve == 'C') mulr = cinc + difm;
    }
    if (i == 2)
    {
      if (secondary_curve == 'S') mulr = sinc;
      if (secondary_curve == 'A') mulr = ainc;
      if (secondary_curve == 'B') mulr = binc;
      if (secondary_curve == 'C') mulr = cinc;
    }
    if (i == 0 || (i == 1 && primary == "PH") || (i == 2 && secondary == "PH"))
    {
      /* Calculate the physical attack statistic */
      addr = floor(pow(race->getPhysicalAggression() + getLevel(), mulr));
      temp = floor(getBasePhysicalAggression() + addr + cat->getPhysicalAggression());
      if (temp < kMAX_PHYS_ATK && temp < cat->getMaxPhysicalAggression())
        setPhysicalAggression(temp);
      else
        setPhysicalAggression(personMax(kMAX_PHYS_ATK,cat->getMaxPhysicalAggression()) );

      /* Calculate the physical defense statistic */
      addr = floor(pow(race->getPhysicalFortitude() + getLevel(), mulr));
      temp = (getBasePhysicalFortitude() + addr + cat->getPhysicalFortitude()) * defm;
      if (temp < kMAX_PHYS_DEF && temp < cat->getMaxPhysicalFortitude())
        setPhysicalFortitude(temp * defm);
      else
        setPhysicalFortitude(personMax(kMAX_PHYS_DEF,cat->getMaxPhysicalFortitude()));
    }
    if (i == 0 || (i == 1 && primary == "TH") || (i == 2 && secondary == "TH"))
    {
      /* Calculate the thermal attack statistic */
      addr = floor(pow(race->getThermalAggression() + getLevel(), mulr));
      temp = getBaseThermalAggression() + addr + cat->getThermalAggression();
      if (temp < kMAX_THER_ATK && temp < cat->getMaxThermalAggression())
        setThermalAggression(temp);
      else
        setThermalAggression(personMax(kMAX_THER_ATK,cat->getMaxThermalAggression()));

      /* Calculate the thermal defense statistic */
      addr = floor(pow(race->getThermalFortitude() + getLevel(), mulr));
      temp = (getBaseThermalFortitude() + addr + cat->getThermalFortitude()) * defm;
      if (temp < kMAX_THER_DEF && temp < cat->getMaxThermalFortitude())
        setThermalFortitude(temp);
      else
        setThermalFortitude(personMax(kMAX_THER_DEF,cat->getMaxThermalFortitude()));
    }
    if (i == 0 || (i == 1 && primary == "PO") || (i == 2 && secondary == "PO"))
    {
      /* Calculate the the polar attack statistic */
      addr = floor(pow(race->getPolarAggression() + getLevel(), mulr));
      temp = getBasePolarAggression() + addr + cat->getPolarAggression();
      if (temp < kMAX_POLA_ATK && temp < cat->getMaxPolarAggression())
        setPolarAggression(temp);
      else
        setPolarAggression(personMax(kMAX_THER_ATK,cat->getMaxThermalAggression()));

      /* Calculate the polar defense statistic */
      addr = floor(pow(race->getPolarFortitude() + getLevel(), mulr));
      temp = (getBasePolarFortitude() + addr + cat->getPolarFortitude()) * defm;
      if (temp < kMAX_POLA_DEF && temp < cat->getMaxPolarFortitude())
        setPolarFortitude(temp);
      else
        setPolarFortitude(personMax(kMAX_POLA_DEF,cat->getMaxPolarFortitude()));
    }
    if (i == 0 || (i == 1 && primary == "PR") || (i == 2 && secondary == "PR"))
    {
      /* Calculate the primal attack statistic */
      addr = floor(pow(race->getPrimalAggression() + getLevel(), mulr));
      temp = getBasePrimalAggression() + addr + cat->getPrimalAggression();
      if (temp < kMAX_PRIM_ATK && temp < cat->getMaxPrimalAggression())
        setPrimalAggression(temp);
      else
        setPrimalAggression(personMax(kMAX_PRIM_ATK,cat->getMaxPrimalAggression()));

      /* Calculate the primal defense statistic */
      addr = floor(pow(race->getPrimalFortitude() + getLevel(), mulr));
      temp = (getBasePrimalFortitude() + addr + cat->getPrimalFortitude()) * defm;
      if (temp < kMAX_PRIM_DEF && temp < cat->getMaxPrimalFortitude())
        setPrimalFortitude(temp);
      else
        setPrimalFortitude(personMax(kMAX_PRIM_DEF,cat->getMaxPrimalFortitude()));
    }
    if (i == 0 || (i == 1 && primary == "CH") || (i == 2 && secondary == "CH"))
    {
      /* Calculate the charged attack statistic */
      addr = floor(pow(race->getChargedAggression() + getLevel(), mulr));
      temp = getBaseChargedAggression() + addr + cat->getChargedAggression();
      if (temp < kMAX_CHAR_ATK && temp < cat->getMaxChargedAggression())
        setChargedAggression(temp);
      else
        setChargedAggression(personMax(kMAX_CHAR_ATK,cat->getMaxThermalAggression()));

      /* Calculate the charged defense statistic */
      addr = floor(pow(race->getChargedFortitude() + getLevel(), mulr));
      temp = (getBaseChargedFortitude() + addr + cat->getChargedFortitude()) * defm;
      if (temp < kMAX_CHAR_DEF && temp < cat->getMaxChargedFortitude())
        setChargedFortitude(temp);
      else
        setChargedFortitude(personMax(kMAX_CHAR_DEF,cat->getMaxChargedFortitude()));
    }
    if (i == 0 || (i == 1 && primary == "CY") || (i == 2 && secondary == "CY"))
    {
      /* Calculate the cybernetic attack statistic */
      addr = floor(pow(race->getCyberneticAggression() + getLevel(), mulr));
      temp = getBaseCyberneticAggression() + addr + cat->getCyberneticAggression();
      if (temp < kMAX_CYBE_ATK && temp < cat->getMaxCyberneticAggression())
        setCyberneticAggression(temp);
      else
        setCyberneticAggression(personMax(kMAX_CYBE_ATK,cat->getMaxCyberneticAggression()));

      /* Calculate the Cybernetic defense statistic */
      addr = floor(pow(race->getCyberneticFortitude() + getLevel(), mulr));
      temp = (getBaseCyberneticFortitude() + addr + cat->getCyberneticFortitude()) * defm;
      if (temp < kMAX_CYBE_DEF && temp < cat->getMaxCyberneticFortitude())
        setCyberneticFortitude(temp);
      else
        setCyberneticFortitude(personMax(kMAX_CYBE_DEF,cat->getMaxCyberneticFortitude()));
    }
    if (i == 0 || (i == 1 && primary == "NI") || (i == 2 && secondary == "NI"))
    {
      /* Calculate the Nihil attack statistic */
      addr = floor(pow(race->getNihilAggression() + getLevel(), mulr));
      temp = getBaseNihilAggression() + addr + cat->getNihilAggression();
      if (temp < kMAX_NIHI_ATK && temp < cat->getMaxNihilAggression())
        setNihilAggression(temp);
      else
        setNihilAggression(personMax(kMAX_NIHI_ATK,cat->getMaxNihilAggression()));

      /* Calculate the Nihil defense statistic */
      addr = floor(pow(race->getNihilFortitude() + getLevel(), mulr));
      temp = getBaseNihilFortitude() + addr + cat->getNihilFortitude() * defm;
      if (temp < kMAX_NIHI_DEF && temp < cat->getMaxNihilFortitude())
        setNihilFortitude(temp);
      else
        setNihilFortitude(personMax(kMAX_NIHI_DEF,cat->getMaxNihilFortitude()));
    }
  }

  mulr = cinc - difm;

  addr = floor(pow(race->getMomentum() + getLevel(), mulr + mome));
  temp = getBaseMomentum() + addr + cat->getMomentum();
  if (temp < kMAX_MOME && temp < cat->getMaxMomentum())
    setMomentum(temp);
  else
    setMomentum(personMax(kMAX_MOME,cat->getMaxMomentum()));

  addr = floor(pow(race->getLimbertude() + getLevel(), mulr + limb));
  temp = getBaseLimbertude() + addr + cat->getLimbertude();
  if (temp < kMAX_LIMB && temp < cat->getMaxLimbertude())
    setLimbertude(temp);
  else
    setLimbertude(personMax(kMAX_LIMB,cat->getMaxLimbertude()));

  addr = floor(pow(race->getUnbearability() + getLevel(), mulr + unbr));
  temp = getBaseUnbearability() + addr + cat->getUnbearability();
  if (temp < kMAX_UNBR && temp < cat->getMaxUnbearability())
    setUnbearability(temp);
  else
    setUnbearability(personMax(kMAX_UNBR,cat->getMaxUnbearability()));

  addr = floor(pow(race->getVitality() + getLevel(), mulr + vita));
  temp = getBaseVitality() + addr + cat->getVitality();
  if (temp < kMAX_VITA && temp < cat->getMaxVitality())
    setVitality(temp);
  else
    setVitality(personMax(kMAX_VITA,cat->getMaxVitality()));

  addr = floor(pow(race->getQuantumDrive() + getLevel(), mulr + qtmn));
  temp = getBaseQuantumDrive() + addr + cat->getQuantumDrive();
  if (temp < kMAX_QNTM && temp < cat->getMaxQuantumDrive())
    setQuantumDrive(temp);
  else
    setQuantumDrive(personMax(kMAX_QNTM,cat->getMaxQuantumDrive()));

  /* Set the person's new level value */
  level = new_level;
}

/*
 * Description: Sets the name of the person
 *
 * Inputs: QString - name of the person to be set
 * Output: none
 */
void Person::setName(QString value)
{
  name = value;
}

/*
 * Description: Sets the race of the person
 *
 * Inputs: Race* - pointer to race to be set
 * Output: none
 */
void Person::setRace(Race* new_race)
{
  race = new_race;
}

/*
 * Description: Sets the person's primary elemental strength and curve
 *              Template "<2-Letter Element Code><Curve(S,A-D)>"
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setRank(QString new_rank)
{
  rank = new_rank;
}

/*
 * Description: Sets the first person view sprite for the person
 *
 * Inputs: Sprite* - pointer to the sprite to be set
 * Output: none
 */
void Person::setFirstPerson(Sprite* s)
{
  first_person = s;
}

/*
 * Description: Sets the third person view sprite for the person
 *
 * Inputs: Sprite* - pointer to the sprite to be set
 * Output: none
 */
void Person::setThirdPerson(Sprite* s)
{
  third_person = s;
}

/*
 * Description: Sets the person's primary elemental strength and curve
 *              Template "<2 Letter Element Code><Curve(S,A-D)>"
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setPrimary(QString value)
{
  setPrimaryCurve(value.at(value.length() - 1));
  value.chop(1);
  primary = value;

}

/*
 * Description: Sets the person's primary curve (also set from setPrimary())
 *
 * Inputs: QChar - character of the curve
 * Output: none
 */
void Person::setPrimaryCurve(QChar value)
{
  primary_curve = value;
}

/*
 * Description: Sets the person's secondary elemental strength and curve
 *              Template "<Element><Curve(A-F)>"
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setSecondary(QString value)
{
  setSecondaryCurve(value.at(value.length() - 1));
  value.chop(1);
  secondary = value;
}

/*
 * Description: Sets the person's secd curve (also set from setSecondary())
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setSecondaryCurve(QChar value)
{
  secondary_curve = value;
}

/*
 * Description: Sets the value of the physical attack stat
 *
 * Inputs: uint - new value of the physical attack stat
 * Output: none
 */
void Person::setPhysicalAggression(uint value)
{
  (value <= cat->getMaxPhysicalAggression()) ? (physical_aggression = value)
                  : (physical_aggression = cat->getMaxPhysicalAggression());
}

/*
 * Description: Sets the value of the physical defense stat
 *
 * Inputs: uint - new value of the physical defense stat
 * Output: none
 */
void Person::setPhysicalFortitude(uint value)
{
  (value <= cat->getMaxPhysicalFortitude()) ? (physical_fortitude = value)
                  : (physical_fortitude = cat->getMaxPhysicalFortitude());
}

/*
 * Description: Sets the value of the thermal attack stat
 *
 * Inputs: uint - new value of thermal attack stat
 * Output: none
 */
void Person::setThermalAggression(uint value)
{
  (value <= cat->getMaxThermalAggression()) ? (thermal_aggression = value)
                  : (thermal_aggression = cat->getMaxThermalAggression());
}

/*
 * Description: Sets the value of the thermal defense stat
 *
 * Inputs: uint - new value of the thermal defense stat
 * Output: none
 */
void Person::setThermalFortitude(uint value)
{
  (value <= cat->getMaxThermalFortitude()) ? (thermal_fortitude = value)
                  : (thermal_fortitude = cat->getMaxThermalFortitude());
}

/*
 * Description: Sets the value of the polar attack stat
 *
 * Inputs: uint - new value of the polar attack stat
 * Output: none
 */
void Person::setPolarAggression(uint value)
{
  (value <= cat->getMaxPolarAggression()) ? (polar_aggression = value)
                  : (polar_aggression = cat->getMaxPolarAggression());
}

/*
 * Description: Sets the value of the polar defense stat
 *
 * Inputs: uint - new value of the polar defense stat
 * Output: none
 */
void Person::setPolarFortitude(uint value)
{
  (value <= cat->getMaxPolarFortitude()) ? (polar_fortitude = value)
                    : (polar_fortitude = cat->getMaxPolarFortitude());
}

/*
 * Description: Sets the value of the primal attack stat
 *
 * Inputs: uint - new value of the polar attack stat
 * Output: none
 */
void Person::setPrimalAggression(uint value)
{
  (value <= cat->getMaxPrimalAggression()) ? (primal_aggression = value)
                  : (primal_aggression = cat->getMaxPrimalAggression());
}

/*
 * Description: Sets the value of the primal defense stat
 *
 * Inputs: uint - new value of the primal defense stat
 * Output: none
 */
void Person::setPrimalFortitude(uint value)
{
  (value <= cat->getPrimalFortitude()) ? (primal_fortitude = value)
               : (primal_fortitude = cat->getMaxPrimalFortitude());
}

/*
 * Description: Sets the value of the charged attack stat
 *
 * Inputs: uint - new value of the primal attack stat
 * Output: none
 */
void Person::setChargedAggression(uint value)
{
  (value <= cat->getMaxChargedAggression()) ? (charged_aggression = value)
                  : (charged_aggression = cat->getMaxChargedAggression());
}

/*
 * Description: Sets the value of the charged defense stat
 *
 * Inputs: uint - new value of the charged defense stat
 * Output: none
 */
void Person::setChargedFortitude(uint value)
{
  (value <= cat->getMaxChargedFortitude()) ? (charged_fortitude = value)
                 : (charged_fortitude = cat->getMaxChargedFortitude());
}

/*
 * Description: Sets the value of the cybernetic attack stat
 *
 * Inputs: uint - new value of the cybernetic attack stat
 * Output: none
 */
void Person::setCyberneticAggression(uint value)
{
  (value <= cat->getMaxCyberneticAggression()) ? (cybernetic_aggression = value)
                 : (cybernetic_aggression = cat->getMaxCyberneticAggression());
}

/*
 * Description: Sets the value of the cybernetic defense stat
 *
 * Inputs: uint - new value of the cybernetic defense stat
 * Output: none
 */
void Person::setCyberneticFortitude(uint value)
{
  (value <= cat->getMaxCyberneticFortitude()) ? (cybernetic_fortitude = value)
                  : (cybernetic_fortitude = cat->getMaxCyberneticFortitude());
}

/*
 * Description: Sets the value of the nihil attack stat
 *
 * Inputs: uint - new value of the nihil attack stat
 * Output: none
 */
void Person::setNihilAggression(uint value)
{
  (value <= cat->getMaxNihilAggression()) ? (nihil_aggression = value)
                  : (nihil_aggression = cat->getMaxNihilAggression());
}

/*
 * Description: Sets the value of the nihil defense stat
 *
 * Inputs: uint - new value of the nihil defense stat
 * Output: none
 */
void Person::setNihilFortitude(uint value)
{
  (value <= cat->getMaxNihilFortitude()) ? (nihil_fortitude = value)
                  : (nihil_fortitude = cat->getMaxNihilFortitude());
}

/*
 * Description: Sets the value of the vitality stat
 *
 * Inputs: uint - new value of the vitaltiy stat
 * Output: none
 */
void Person::setVitality(uint value)
{
  (value <= cat->getMaxVitality()) ? (vitality = value)
                  : (vitality = cat->getMaxVitality());
}

/*
 * Description: Sets the value of the quantum drive stat
 *
 * Inputs: uint - new value of the quantum drive stat
 * Output: none
 */
void Person::setQuantumDrive(uint value)
{
  (value <= cat->getMaxQuantumDrive()) ? (quantum_drive = value)
                  : (quantum_drive = cat->getMaxQuantumDrive());
}

/*
 * Description: Sets the value of the momentum stat
 *
 * Inputs: uint - new value of the momentum stat
 * Output: none
 */
void Person::setMomentum(uint value)
{
  (value <= cat->getMaxMomentum()) ? (momentum = value)
                  : (momentum = cat->getMaxMomentum());
}

/*
 * Description: Sets the value of the limbertude stat
 *
 * Inputs: uint - new value of the limbertude stat
 * Output: noe
 */
void Person::setLimbertude(uint value)
{
  (value <= cat->getMaxLimbertude()) ? (limbertude = value)
                  : (limbertude = cat->getMaxLimbertude());
}

/*
 * Description: Sets the value of the unbearability stat
 *
 * Inputs: uint - new value of the unbearability stat
 * Output: none
 */
void Person::setUnbearability(uint value)
{
  (value <= cat->getMaxUnbearability()) ? (unbearability = value)
                  : (unbearability = cat->getMaxUnbearability());
}

/*
 * Description: Sets the value of the temp physical aggression stat
 *
 * Inputs: uint - new value of the physical aggression stat
 * Output: none
 */
void Person::setTempPhysicalAggression(uint value)
{
  temp_physical_aggression = value;
}

/*
 * Description: Sets the value of the temp physical defense stat
 *
 * Inputs: uint - new value of the temp physical defense stat
 * Output: none
 */
void Person::setTempPhysicalFortitude(uint value)
{
  temp_physical_fortitude = value;
}

/*
 * Description: Sets the value of the temp thermal attack stat
 *
 * Inputs: uint - new value of the temp thermal attack stat
 * Output: none
 */
void Person::setTempThermalAggression(uint value)
{
  temp_thermal_aggression = value;
}

/*
 * Description: Sets the value of the temp thermal defense stat
 *
 * Inputs: uint - new value of the temp thermal defense stat
 * Output: none
 */
void Person::setTempThermalFortitude(uint value)
{
  temp_thermal_fortitude = value;
}

/*
 * Description: Sets the value of the temp polar attack stat
 *
 * Inputs: uint - new value of the temp polar attack stat
 * Output: none
 */
void Person::setTempPolarAggression(uint value)
{
  temp_polar_aggression = value;
}

/*
 * Description: Sets the value of the temp polar defense stat
 *
 * Inputs: uint - new value of the temp polar defense stat
 * Output: none
 */
void Person::setTempPolarFortitude(uint value)
{
  temp_polar_fortitude = value;
}

/*
 * Description: Sets the value of the temp primal attack stat
 *
 * Inputs: uint - new value of the temp primal attack stat
 * Output: none
 */
void Person::setTempPrimalAggression(uint value)
{
  temp_primal_aggression = value;
}

/*
 * Description: Sets the value of the temp primal defense stat
 *
 * Inputs: uint - new value of the temp primal defense stat
 * Output: none
 */
void Person::setTempPrimalFortitude(uint value)
{
  temp_primal_fortitude = value;
}

/*
 * Description: Sets the value of the temp charged attack stat
 *
 * Inputs: uint - new value of the temp charged attack stat
 * Output: none
 */
void Person::setTempChargedAggression(uint value)
{
  temp_charged_aggression = value;
}

/*
 * Description: Sets the value of the temp charged defense stat
 *
 * Inputs: uint - new value of the temp charged defense stat
 * Output: none
 */
void Person::setTempChargedFortitude(uint value)
{
  temp_charged_fortitude = value;
}

/*
 * Description: Sets the value of the temp cybernetic attack stat
 *
 * Inputs: uint - new value of the temp cybernetic attack
 * Output: none
 */
void Person::setTempCyberneticAggression(uint value)
{
  temp_cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the temp cybernetic defense stat
 *
 * Inputs: uint - new value of the cybernetic defense stat
 * Output: none
 */
void Person::setTempCyberneticFortitude(uint value)
{
  temp_cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the temp nihil attack stat
 *
 * Inputs: uuint - new value of the temp nihil attack stat
 * Output: none
 */
void Person::setTempNihilAggression(uint value)
{
  temp_nihil_aggression = value;
}

/*
 * Description: Sets the value of the temp nihil defense stat
 *
 * Inputs: uint - new value of the temp nihil defense stat
 * Output: none
 */
void Person::setTempNihilFortitude(uint value)
{
  temp_nihil_fortitude = value;
}

/*
 * Description: Sets the value of the temp vitality stat
 *
 * Inputs: uint - new value of the temp vitality stat
 * Output: none
 */
void Person::setTempVitality(uint value)
{
  temp_vitality = value;
}

/*
 * Description: Sets the value of the temp quantum drive stat
 *
 * Inputs: uint - new value of the temp quantum drive stat
 * Output: none
 */
void Person::setTempQuantumDrive(uint value)
{
  temp_quantum_drive = value;
}

/*
 * Description: Sets the value of the temp momentum stat
 *
 * Inputs: uint - new value of the temp momentum stat
 * Output: none
 */
void Person::setTempMomentum(uint value)
{
  temp_momentum = value;
}

/*
 * Description: Sets the value of the temp limbertude stat
 *
 * Inputs: uint - new value of the temp limbertude stat
 * Output: none
 */
void Person::setTempLimbertude(uint value)
{
  temp_limbertude = value;
}

/*
 * Description: Sets the value of the temp unbearability stat
 *
 * Inputs: uint - new value of the temp unbearability stat
 * Output: none
 */
void Person::setTempUnbearability(uint value)
{
  temp_unbearability = value;
}

/*
 * Description: Sets the value of the base thermal attack stat
 *
 * Inputs: uint - new value of the base thermal attack stat
 * Output: none
 */
void Person::setBasePhysicalAggression(uint value)
{
  base_physical_aggression = value;
}


/*
 * Description: Sets the value of the temp unbearability stat
 *
 * Inputs: uint - new value of the temp unbearability stat
 * Output: none
 */
void Person::setBasePhysicalFortitude(uint value)
{
  base_physical_fortitude = value;
}

/*
 * Description: Sets the value of the base thermal attack stat
 *
 * Inputs: uint - new value of the base thermal attack stat
 * Output: none
 */
void Person::setBaseThermalAggression(uint value)
{
  base_thermal_aggression = value;
}

/*
 * Description: Sets the value of the base thermal defense stat
 *
 * Inputs: uint - new value of te base thermal defense stat
 * Output: none
 */
void Person::setBaseThermalFortitude(uint value)
{
  base_thermal_fortitude = value;
}

/*
 * Description: Sets the value of the base polar attack stat
 *
 * Inputs: uint - new value of the base polar attack stat
 * Output: none
 */
void Person::setBasePolarAggression(uint value)
{
  base_polar_aggression = value;
}

/*
 * Description: Sets the value of te base polar defense stat
 *
 * Inputs: uint - new value of the base polar defense stat
 * Output: none
 */
void Person::setBasePolarFortitude(uint value)
{
  base_polar_fortitude = value;
}

/*
 * Description: Sets the value of the base primal attack stat
 *
 * Inputs: uint - new value of the base primal attack stat
 * Output: none
 */
void Person::setBasePrimalAggression(uint value)
{
  base_primal_aggression = value;
}

/*
 * Description: Sets the value of the base primal defense stat
 *
 * Inputs: uint - new value of the base primal defense stat
 * Output: none
 */
void Person::setBasePrimalFortitude(uint value)
{
  base_primal_fortitude = value;
}

/*
 * Description: Sets the value of the base charged attack stat
 *
 * Inputs: uint - new value of the base charged attack stat
 * Output: none
 */
void Person::setBaseChargedAggression(uint value)
{
  base_charged_aggression = value;
}

/*
 * Description: Sets the value of the base charged defense stat
 *
 * Inputs: uint - new value of the base charged defense stat
 * Output: none
 */
void Person::setBaseChargedFortitude(uint value)
{
  base_charged_fortitude = value;
}

/*
 * Description: Sets the value of the base cybernetic attack stat
 *
 * Inputs: uint - new value of the base cybernetic defense stat
 * Output: none
 */
void Person::setBaseCyberneticAggression(uint value)
{
  base_cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the base cybernetic defense stat
 *
 * Inputs: uint - new value of the base cybernetic defense stat
 * Output: none
 */
void Person::setBaseCyberneticFortitude(uint value)
{
  base_cybernetic_fortitude = value;
}

/*
 * Description: Sets the value of the base nihil attack stat
 *
 * Inputs: uint - new value of the base nihil attack stat
 * Output: none
 */
void Person::setBaseNihilAggression(uint value)
{
  base_nihil_aggression = value;
}

/*
 * Description: Sets the value of the base nihil defense stat
 *
 * Inputs: uint - new value of the base nihil defense stat
 * Output: none
 */
void Person::setBaseNihilFortitude(uint value)
{
  base_nihil_fortitude = value;
}

/*
 * Description: Sets the value of the base vitality stat
 *
 * Inputs: uint - new value of the base vitality stat
 * Output: none
 */
void Person::setBaseVitality(uint value)
{
  base_vitality = value;
}

/*
 * Description: Sets the value of the base quantum drive stat
 *
 * Inputs: uint - new value of the base quantum drive stat
 * Output: none
 */
void Person::setBaseQuantumDrive(uint value)
{
  base_quantum_drive = value;
}

/*
 * Description: Sets the value of the base momentum stat
 *
 * Inputs: uint - new value of the base momentum stat
 * Output: none
 */
void Person::setBaseMomentum(uint value)
{
  base_momentum = value;
}

/*
 * Description: Sets the value of the base limbertude stat
 *
 * Inputs: uint - new value of the base limbertude stat
 * Output: none
 */
void Person::setBaseLimbertude(uint value)
{
  base_limbertude = value;
}

/*
 * Description: Sets the value of the base unbearability stat
 *
 * Inputs: uint - new value of the base unbearability stat
 * Output: none
 */
void Person::setBaseUnbearability(uint value)
{
  base_unbearability = value;
}
