/******************************************************************************
* Class Name: Person Implementation
* Date Created: December 2nd, 2012
* Inheritance: QWidget
* Description: Holder for all the info describing a person (character)
*
* TODO: FLAGS FOR STATUS AILMENTS [01-20-13]
* TODO: STATS INCREASE ON LEVEL UP [01-21-13]
* TODO: ?? addEquipment function [01-21-13]
* TODO: action_list actions, [01-25-13]
* TODO: Future class multiplier for level ups [01-25-13]
******************************************************************************/
#include "Game/Player/Person.h"
#include <cmath>

/*
 * Description: Default person constructor (generic values)
 *
 */
Person::Person(QWidget* pointer)
{
  /* Set up the normal statistics for a person
   * th_a, th_f, po_a, po_f, pr_a, pr_f,
   * ch_a, ch_f, cy_a,  cy_f, ni_a, ni_f,
   * vit, qd, ag, lim, unb, lev, exp, nam,
   * prim, secd, rank */
  setupStats(5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2,
               100, 50, 5, 5, 5, 5, 300, "Arcadius",
               "polar", "cybernetic", "Recruit");
}

/*
 * Description: Primary person constructor (requires all values)
 *
 * Inputs: int th_a - base thermal aggression statistic
 *         int th_f - base thermal fortitude statistic
 *         int po_a - base polar aggression statistic
 *         int po_f - base polar fortitude statistic
 *         int pr_a - base primal aggression statistic
 *         int pr_f - base primal fortitude statistic
 *         int ch_a - base charged aggression statistic
 *         int ch_f - base charged fortitude statistic
 *         int cy_a - base cybernetic aggression statistic
 *         int cy_f - base cybernetic fortitude statistic
 *         int ni_a - base nihil aggression statistic
 *         int ni_f - base nihil fortitude statistic
 *         int vit - base vitality statistic
 *         int qd - base quantum drive statisti
 *         int ag - base agility statistic
 *         int lim - base limbertude statistic
 *         int unb - base unbearability statistic
 *         int lev - person's starting level
 *         int exp - person's starting experience
 *         QString nam - name of person
 *         QString prim - person's primary elemental ability
 *         QString secd - person's secondary elemental ability
 *         QString rank - person's rank [recruit, sleuthmaster, etc.]
 */
Person::Person(int th_a, int th_f, int po_a,  int po_f, int pr_a,
               int pr_f, int ch_a, int ch_f,  int cy_a, int cy_f,
               int ni_a, int ni_f, int vit,   int qd,   int ag,
               int lim,  int unb,  int lev,   int exp,  QString nam,
               QString prim, QString secd, QString rank, QWidget* pointer)
{
  /* Sets up the starting statistics for the person */
  setupStats(th_a, th_f, po_a, po_f, pr_a, pr_f, ch_a, ch_f, cy_a,
             cy_f, ni_a, ni_f, vit, qd, ag, lim, unb, lev, exp, nam,
             prim, secd, rank);


}

/*
 * Description: Person destructor
 */
Person::~Person()
{
}

/*
 * Description:
 *
 * Inputs: int th_a - base thermal aggression statistic
 *         int th_f - base thermal fortitude statistic
 *         int po_a - base polar aggression statistic
 *         int po_f - base polar fortitude statistic
 *         int pr_a - base primal aggression statistic
 *         int pr_f - base primal fortitude statistic
 *         int ch_a - base charged aggression statistic
 *         int ch_f - base charged fortitude statistic
 *         int cy_a - base cybernetic aggression statistic
 *         int cy_f - base cybernetic fortitude statistic
 *         int ni_a - base nihil aggression statistic
 *         int ni_f - base nihil fortitude statistic
 *         int vit - base vitality statistic
 *         int qd - base quantum drive statisti
 *         int ag - base agility statistic
 *         int lim - base limbertude statistic
 *         int unb - base unbearability statistic
 *         int lev - person's starting level
 *         int exp - person's starting experience
 *         QString nam - name of person
 *         QString prim - person's primary elemental ability
 *         QString secd - person's secondary elemental ability
 *         QString rank - person's rank [recruit, sleuthmaster, etc.]
 * Output: none
 */
void Person::setupStats(int th_a, int th_f, int po_a, int po_f, int pr_a,
                        int pr_f, int ch_a, int ch_f,  int cy_a,  int cy_f,
                        int ni_a, int ni_f, int vit,   int qd,   int ag,
                        int lim,  int unb,  int lev,   int exp, QString nam,
                        QString prim, QString secd, QString rank)
{
    /* Sets up base stats */
    setBaseThermalAggression(th_a);
    setBaseThermalFortitude(th_f);
    setBasePolarAggression(po_a);
    setBasePolarFortitude(po_f);
    setBasePrimalAggression(pr_a);
    setBasePrimalFortitude(pr_f);
    setBaseChargedAggression(ch_a);
    setBaseChargedFortitude(ch_f);
    setBaseCyberneticAggression(cy_a);
    setBaseCyberneticFortitude(cy_f);
    setBaseNihilAggression(ni_a);
    setBaseNihilFortitude(ni_f);
    setBaseVitality(vit);
    setBaseQuantumDrive(qd);
    setBaseAgility(ag);
    setBaseLimbertude(lim);

    /* Sets up current stas as base stats (initially) */
    setThermalAggression(th_a);
    setThermalFortitude(th_f);
    setPolarAggression(po_a);
    setPolarFortitude(po_f);
    setPrimalAggression(pr_a);
    setPrimalFortitude(pr_f);
    setChargedAggression(ch_a);
    setChargedFortitude(ch_f);
    setCyberneticAggression(cy_a);
    setCyberneticFortitude(cy_f);
    setNihilAggression(ni_a);
    setNihilFortitude(ni_f);
    setVitality(vit);
    setQuantumDrive(qd);
    setAgility(ag);
    setLimbertude(lim);

    /* Sets up temporary stats */
    setTempThermalAggression(th_a);
    setTempThermalFortitude(th_f);
    setTempPolarAggression(po_a);
    setTempPolarFortitude(po_f);
    setTempPrimalAggression(pr_a);
    setTempPrimalFortitude(pr_f);
    setTempChargedAggression(ch_a);
    setTempChargedFortitude(ch_f);
    setTempCyberneticAggression(cy_a);
    setTempCyberneticFortitude(cy_f);
    setTempNihilAggression(ni_a);
    setTempNihilFortitude(ni_f);
    setTempVitality(vit);
    setTempQuantumDrive(qd);
    setTempAgility(ag);
    setTempLimbertude(lim);

    /* Sets up temporary stats as base stats (intiially) */
    setExp(exp);
    setLevel(lev);
    setName(nam);
    setPrimary(prim);
    setSecondary(secd);
    setRank(rank);

    /* Sets equipment pointers to NULL initially */
    // setHeadEquipment();
    // setLeftArmEquipment();
    // setRightArmEquipment();
    // setTorsoEquipment();
    // setLegEquipment();
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Person::addEquipment(Equipment* equipment)
{
  return TRUE;
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
void Person::addExperience(int value)
{
 /* Finds the experience value of next level and if the current
   * experience value of the person exceeds this, level up the
   * character until its experience matches the correct level */

  int l = getLevel();
  int y = 0;
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
 * Description: Sets up temporary statistics for a battle, and
 *              other preparations
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Person::isStatusAilment(QString s)
{
  //TODO: Finish status ailments [12-02-12]
  return TRUE;
}

/*
 * Description: Uses an item
 *
 * Inputs: used_item - pointer of an item to be used
 * Output: bool - tue if item used successfully
 */
bool useItem(Item* used_item)
{
    return TRUE;
}

/*
 * Description: Gets a list of available actions for current person
 *
 * Inputs: none
 * Output: vector of action pointers - pointers to action the player can take
 */
QVector<Action*>& Person::getAvailableActions()
{
  // TODO: Setup list of actions for person [01-25-13]
  return action_list;
}

/*
 * Description: Evaluates the rendering flag
 *
 * Inputs: none
 * Output: bool - value of RENDERING flag
 */
bool Person::getRendering()
{
  return RENDERING;
}


/*
 * Description: Returns the person's class
 *
 * Inputs: none
 * Output: Equipment* - pointer to class of the person
 */
Category* Person::getCategory()
{
  return character_class;
}


/*
 * Description: Returns the person's race
 *
 * Inputs: none
 * Output: Equipment* - pointer to race of the person
 */
Race* Person::getRace()
{
  return character_race;
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
int Person::getExp()
{
  return experience;
}

/*
 * Description: Returns value of the person's current level
 *
 * Inputs: none
 * Output: int - value of person's level
 */
int Person::getLevel()
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
 * Description: Returns value of the thermal attack stat
 *
 * Inputs: none
 * Output: int - value of the thermal attack stat
 */
int Person::getThermalAggression()
{
  return thermal_aggression;
}

/*
 * Description: Returns value of the thermal fortitude stat
 *
 * Inputs: none
 * Output: int - value of the thermal defense stat
 */
int Person::getThermalFortitude()
{
  return thermal_fortitude;
}

/*
 * Description: Returns value of the polar attack stat
 *
 * Inputs: none
 * Output: int - value of polar attack stat
 */
int Person::getPolarAggression()
{
  return polar_aggression;
}

/*
 * Description: Returns value of the polar defense stat
 *
 * Inputs: none
 * Output: int - value of the polar defense stat
 */
int Person::getPolarFortitude()
{
  return polar_fortitude;
}

/*
 * Description: Returns value of the primal attack stat
 *
 * Inputs: none
 * Output: int - value of the primal attack stat
 */
int Person::getPrimalAggression()
{
  return primal_aggression;
}

/*
 * Description: Returns value of the primal defense stat
 *
 * Inputs: none
 * Output: int - vaue of the primal attack stat
 */
int Person::getPrimalFortitude()
{
  return primal_fortitude;
}

/*
 * Description: Returns value of the charged attack stat
 *
 * Inputs: none
 * Output: int - value of the charged attack stat
 */
int Person::getChargedAggression()
{
  return charged_aggression;
}

/*
 * Description: Returns value of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the charged defense stat
 */
int Person::getChargedFortitude()
{
  return charged_fortitude;
}

/*
 * Description: Returns value of the cybernetic attack stat
 *
 * Inputs: none
 * Output: int - value of the cybernetic attack stat
 */
int Person::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/*
 * Description: Returns value of the cybernetic defense stat
 *
 * Inputs: none
 * Output: int - value of the cybernetic defense stat
 */
int Person::getCyberneticFortitude()
{
  return cybernetic_fortitude;
}

/*
 * Description: Returns value of the nihil attack stat
 *
 * Inputs: none
 * Output: int - value of the nihil attack stat
 */
int Person::getNihilAggression()
{
  return nihil_aggression;
}

/*
 * Description: Returns value of the nihil defense stat
 *
 * Inputs: none
 * Output: int - value of the nihil defense stat
 */
int Person::getNihilFortitude()
{
  return nihil_fortitude;
}

/*
 * Description: Returns value of the vitality stat
 *
 * Inputs: none
 * Output: int - value of the vitality stat
 */
int Person::getVitality()
{
  return vitality;
}

/*
 * Description: Returns value of quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the quantum drive stat
 */
int Person::getQuantumDrive()
{
  return quantum_drive;
}

/*
 * Description: Returns value of agility stat
 *
 * Inputs: none
 * Output: int - value of the agility stat
 */
int Person::getAgility()
{
  return agility;
}

/*
 * Description: Returns value of the limbertude stat
 *
 * Inputs: none
 * Output: int - value of the limbertude stat
 */
int Person::getLimbertude()
{
  return limbertude;
}

/*
 * Description: Returns value of the unbearability stat
 *
 * Inputs: none
 * Output: int - value of the unbearability stat
 */
int Person::getUnbearability()
{
  return unbearability;
}

/*
 * Description: Returns value of the temp thermal attack stat
 *
 * Inputs: none
 * Output: int - value of the temp thermal attack stat
 */
int Person::getTempThermalAggression()
{
  return temp_thermal_aggression;
}

/*
 * Description: Returns value of the temp thermal defense stat
 *
 * Inputs: none
 * Output: int - value of the temp thermal fortitude stat
 */
int Person::getTempThermalFortitude()
{
  return temp_thermal_fortitude;
}

/*
 * Description: Returns value of the temp polar attack stat
 *
 * Inputs: none
 * Output: int - value of the temp polar attack stat
 */
int Person::getTempPolarAggression()
{
  return temp_polar_aggression;
}

/*
 * Description: Returns value of the temp polar defense stat
 *
 * Inputs: none
 * Output: int - value of the temp polar defense stat
 */
int Person::getTempPolarFortitude()
{
  return temp_polar_fortitude;
}

/*
 * Description: Returns value of the temp primal attack stat
 *
 * Inputs: none
 * Output: int - value of the temp primal attack stat
 */
int Person::getTempPrimalAggression()
{
  return temp_primal_aggression;
}

/*
 * Description: Returns value of the temp primal defene stat
 *
 * Inputs: none
 * Output: int - value of the temp primal defense stat
 */
int Person::getTempPrimalFortitude()
{
  return temp_primal_fortitude;
}

/*
 * Description: Returns vlue of the temp charged attack stat
 *
 * Inputs: none
 * Output: int - value of the temp charged attack stat
 */
int Person::getTempChargedAggression()
{
  return temp_charged_aggression;
}

/*
 * Description: Returns value of the temp charged defense stat
 *
 * Inputs: none
 * Output: int - value of the temp charged defense stat
 */
int Person::getTempChargedFortitude()
{
  return temp_charged_fortitude;
}

/*
 * Description: Returns value of the temp cybernetic attack stat
 *
 * Inputs: none
 * Output: int - value of the temp charged attack stat
 */
int Person::getTempCyberneticAggression()
{
  return temp_cybernetic_aggression;
}

/*
 * Description: Returns value of the temp cybernetic defense stat
 *
 * Inputs: none
 * Output: int - value of the temp cybernetc defense stat
 */
int Person::getTempCyberneticFortitude()
{
  return temp_cybernetic_fortitude;
}

/*
 * Description: Returns value of the temp nihil attack stat
 *
 * Inputs: none
 * Output: int - value of the temp nihil attack stat
 */
int Person::getTempNihilAggression()
{
  return temp_nihil_aggression;
}

/*
 * Description: Returns value of the temp nihil defense stat
 *
 * Inputs: none
 * Output: int - value of the temp nihil defense stat
 */
int Person::getTempNihilFortitude()
{
  return temp_nihil_fortitude;
}

/*
 * Description: Returns value of the temp vitality stat
 *
 * Inputs: none
 * Output: int - value of the temp vitality stat
 */
int Person::getTempVitality()
{
  return temp_vitality;
}

/*
 * Description: Returns value of the temp quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the temp quantum drive sta
 */
int Person::getTempQuantumDrive()
{
  return temp_quantum_drive;
}

/*
 * Description: Returns value of the temp agility stat
 *
 * Inputs: none
 * Output: int - value of the temp agility stat
 */
int Person::getTempAgility()
{
  return temp_agility;
}

/*
 * Description: Returns value of the temp limbertude stat
 *
 * Inputs: none
 * Output: int - value of the temp limbertude stat
 */
int Person::getTempLimbertude()
{
  return temp_limbertude;
}

/*
 * Description: Returns value of the temp unbearability stat
 *
 * Inputs: none
 * Output: int - value of the temp unbearability stat
 */
int Person::getTempUnbearability()
{
  return temp_unbearability;
}

/*
 * Description: Returns value of the base thermal attack stat
 *
 * Inputs: none
 * Output: int - value of the base thermal defense stat
 */
int Person::getBaseThermalAggression()
{
  return base_thermal_aggression;
}

/*
 * Description: Returns value of the base thermal defense stat
 *
 * Inputs: none
 * Output: int - value of the base thermal defense stat
 */
int Person::getBaseThermalFortitude()
{
  return base_thermal_fortitude;
}

/*
 * Description: Returns value of the base polar attack stat
 *
 * Inputs: none
 * Output: int - value of the base polar attack stat
 */
int Person::getBasePolarAggression()
{
  return base_polar_aggression;
}

/*
 * Description: Returns value of the base polar defense stat
 *
 * Inputs: none
 * Output: int - value of the base polar defense stat
 */
int Person::getBasePolarFortitude()
{
  return base_polar_fortitude;
}

/*
 * Description: Returns value of the base primal attack stat
 *
 * Inputs: none
 * Output: int - value of the base primal attack stat
 */
int Person::getBasePrimalAggression()
{
  return base_primal_aggression;
}

/*
 * Description: Returns value of the base primal defense stat
 *
 * Inputs: none
 * Output: int - value of the base primal defense stat
 */
int Person::getBasePrimalFortitude()
{
  return base_primal_fortitude;
}

/*
 * Description: Returns value of the base charged attack stat
 *
 * Inputs: none
 * Output: int - value of the base charged attack stat
 */
int Person::getBaseChargedAggression()
{
  return base_charged_aggression;
}

/*
 * Description: Returns value of the base charged defense stat
 *
 * Inputs: none
 * Output: int - value of the base charged defense stat
 */
int Person::getBaseChargedFortitude()
{
  return base_charged_fortitude;
}

/*
 * Description: Returns value of the base cybernetic attack stat
 *
 * Inputs: none
 * Output: int - value of the base cybnetic attack stat
 */
int Person::getBaseCyberneticAggression()
{
  return base_cybernetic_aggression;
}

/*
 * Description: Returns value of the base cybernetic defense stat
 *
 * Inputs: none
 * Output: int - vale of the base cybernetic defense stat
 */
int Person::getBaseCyberneticFortitude()
{
  return base_cybernetic_fortitude;
}

/*
 * Description: Returns value of the base nihil attack stat
 *
 * Inputs: none
 * Output: int - value of the base nihil attack stat
 */
int Person::getBaseNihilAggression()
{
  return base_nihil_aggression;
}

/*
 * Description: Returns value of the base nihil defense stat
 *
 * Inputs: none
 * Output: int - value of the base nihil defense stat
 */
int Person::getBaseNihilFortitude()
{
  return base_nihil_fortitude;
}

/*
 * Description: Returns value of the base vitality stat
 *
 * Inputs: none
 * Output: int - value of the base vitality stat
 */
int Person::getBaseVitality()
{
  return base_vitality;
}

/*
 * Description: Returns value of the base quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the base quantum drive stat
 */
int Person::getBaseQuantumDrive()
{
  return base_quantum_drive;
}

/*
 * Description: Returns value of the base agility stat
 *
 * Inputs: none
 * Output: int - value of the base agility stat
 */
int Person::getBaseAgility()
{
  return base_agility;
}

/*
 * Description: Returns value of the base limbertude stat
 *
 * Inputs: none
 * Output: int - value of the base limbertude stat
 */
int Person::getBaseLimbertude()
{
  return base_limbertude;
}

/*
 * Description: Returns value of the base unbearability stat
 *
 * Inputs: none
 * Output: int - value of the base unbearability stat
 */
int Person::getBaseUnbearability()
{
  return base_unbearability;
}

/*
 * Description: Sets a given equipment on to the head of a person
 *
 * Inputs: none
 * Output: bool - true on successful equip
 */
bool Person::setHeadEquipment(Equipment* new_equipment)
{
  if (new_equipment != NULL || new_equipment->canEquip("head"))
  {
    head = new_equipment;
    if (head != NULL)
      return true;
  }
  return false;
}

/*
 * Description: Sets a given equipment on to the left arm of a person
 *
 * Inputs: none
 * Output: bool - true on successful equip
 */
bool Person::setLeftArmEquipment(Equipment* new_equipment)
{
  if (new_equipment->canEquip("left arm"))
  {
    left_arm = new_equipment;
    if (legs != NULL)
      return true;
  }
  return false;

}

/*
 * Description: Sets a given equipment on to the right arm of a person
 *
 * Inputs: none
 * Output: bool - true if equip successful
 */
bool Person::setRightArmEquipment(Equipment* new_equipment)
{
  if (new_equipment->canEquip("right arm"))
  {
    right_arm = new_equipment;
    if (right_arm != NULL)
      return true;
    }
  return false;
}

/*
 * Description: Sets a given equipment on to the torso of a person
 *
 * Inputs: none
 * Output: bool - true if equip successful
 */
bool Person::setTorsoEquipment(Equipment* new_equipment)
{
  if (new_equipment->canEquip("torso"))
  {
  torso = new_equipment;
  if (torso != NULL)
    return true;
  }
  return false;
}

/*
 * Description: Sets a given equipment on to the leg of a person
 *
 * Inputs: none
 * Output: bool - true if equip successful
 */
bool Person::setLegEquipment(Equipment* new_equipment)
{
  if (new_equipment->canEquip("torso"))
  {
  legs = new_equipment;
  if (legs != NULL)
    return true;
  }
  return false;
}

/*
 * Description: Sets the category (class) of the person
 *
 * Inputs: Category* - pointer to class to change person to
 * Output: none
 */
void Person::setCategory(Category* new_category)
{
  character_class = new_category;
}

/*
 * Description: Sets the experience of the person
 * Note: Does NOT level the character, use addExp(int) for that
 *
 * Inputs: int - value of experience to be set
 * Output: none
 */
void Person::setExp(int exp)
{
  experience = exp;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Person::setStatusAilment(bool b, int ailment_index)
{
  //TODO: FINISH STATUS AILMENTS
}

/*
 * Description: Sets the level of the person, then based off the base stats,
 *              resets the person's current stats to new, correct values
 *
 * Inputs: int - level to set person to
 * Output: none
 */
void Person::setLevel(int new_level)
{
  /* Update display value of the level (should be 0-126) */
  level = new_level;

  /* TODO: Future class bonus? */
  int class_bonus = 0;


  /* Set basic combat stats */
  // TODO: Future class multipliers [01-25-13]
  float multiplier = 1.20;
  float dm         = 0.67;
  int adder = floor(pow(getLevel(), multiplier));

  setThermalFortitude(getBaseThermalAggression() + adder);
  setThermalFortitude((getBaseThermalFortitude() + adder) * dm);
  setPolarAggression(getBasePolarAggression() + adder);
  setPolarFortitude((getBasePolarFortitude() + adder) * dm);
  setPrimalAggression(getBasePrimalAggression() + adder);
  setPrimalFortitude((getBasePrimalFortitude() + adder) * dm);
  setChargedAggression(getBaseChargedAggression() + adder);
  setChargedFortitude((getBaseChargedFortitude() + adder) * dm);
  setCyberneticAggression(getBaseCyberneticAggression() + adder);
  setCyberneticFortitude((getBaseCyberneticFortitude() + adder) * dm);
  setNihilAggression(getBaseNihilAggression() + adder);
  setNihilFortitude((getBaseNihilFortitude() + adder) * dm);

  /* Set other stats */
  multiplier = 1.10;
  adder = floor(pow(getLevel(), multiplier));

  setAgility(getBaseAgility() + adder);
  setLimbertude(getBaseLimbertude() + adder);
  setUnbearability(getBaseUnbearability() + adder);

  multiplier = 1.30;
  adder = floor(pow(getLevel(), multiplier));

  setVitality(getBaseVitality() + adder);
  setQuantumDrive(getBaseQuantumDrive()+ adder);

  /* Re-update stats for primary curve */
  if (primary_curve == 'A')
    multiplier = 1.80;
  else if (primary_curve == 'B')
    multiplier = 1.60;
  else if (primary_curve == 'C')
    multiplier = 1.40;
  else if (primary_curve == 'D')
    multiplier = 1.20;

  adder = floor(pow(getLevel(), multiplier));

  if (primary == "Thermal")
  {
      setThermalAggression(getBaseThermalAggression() + adder);
      setThermalFortitude((getBaseThermalFortitude() + adder) * dm);
  }
  else if (primary == "Polar")
  {
      setPolarAggression(getBasePolarAggression() + adder);
      setPolarFortitude((getBasePolarFortitude() + adder) * dm);
  }
  else if (primary == "Primal")
  {
      setPrimalAggression(getBasePrimalAggression() + adder);
      setPrimalFortitude((getBasePrimalFortitude() + adder) * dm);
  }
  else if (primary == "Charged")
  {
      setChargedAggression(getBaseChargedAggression() + adder);
      setChargedFortitude((getBaseChargedFortitude() + adder) * dm);
  }
  else if (primary == "Cybernetic")
  {
      setCyberneticAggression(getBaseCyberneticAggression() + adder);
      setCyberneticFortitude((getBaseCyberneticFortitude() + adder) * dm);
  }
  else if (primary == "Nihil")
  {
      setNihilAggression(getBaseNihilAggression() + adder);
      setNihilFortitude((getBaseNihilFortitude() + adder) * dm);
  }

  /* Re-updates stats for secondary curve */
  if (secondary_curve == 'A')
    multiplier = 1.60;
  else if (secondary_curve == 'C')
    multiplier = 1.40;
  else if (secondary_curve == 'D')
    multiplier = 1.20;
  else if (secondary_curve == 'E')
    multiplier = 1.10;

  adder = floor(pow(getLevel(), multiplier));

  if (secondary == "Thermal")
  {
      setThermalAggression(getBaseThermalAggression() + adder);
      setThermalFortitude((getBaseThermalFortitude() + adder) * dm);
  }
  else if (secondary == "Polar")
  {
      setPolarAggression(getBasePolarAggression() + adder);
      setPolarFortitude((getBasePolarFortitude() + adder) * dm);
  }
  else if (secondary == "Primal")
  {
      setPrimalAggression(getBasePrimalAggression() + adder);
      setPrimalFortitude((getBasePrimalFortitude() + adder) * dm);
  }
  else if (secondary == "Charged")
  {
      setChargedAggression(getBaseChargedAggression() + adder);
      setChargedFortitude((getBaseChargedFortitude() + adder) * dm);
  }
  else if (secondary == "Cybernetic")
  {
      setCyberneticAggression(getBaseCyberneticAggression() + adder);
      setCyberneticFortitude((getBaseCyberneticFortitude() + adder) * dm);
  }
  else if (secondary == "Nihil")
  {
      setNihilAggression(getBaseNihilAggression() + adder);
      setNihilFortitude((getBaseNihilFortitude() + adder) * dm);
  }

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
  character_race = new_race;
}

/*
 * Description: Sets the person's primary elemental strength and curve
 *              Template "<Element><Curve(A-F)>"
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
 *              Template "<Element><Curve(A-F)>"
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
 * Description: Sets the person's secondary curve (also set from setSecondary())
 *
 * Inputs: QString - string representing the strength and curve
 * Output: none
 */
void Person::setSecondaryCurve(QChar value)
{
  secondary_curve = value;
}

/*
 * Description: Sets the value of the RENDERING flag
 *
 * Inputs: bool - new RENDERING value
 * Output: none
 */
void Person::setRendering(bool b)
{
  RENDERING = b;
}

/*
 * Description: Sets the value of the thermal attack stat
 *
 * Inputs: int - new value of thermal attack stat
 * Output: none
 */
void Person::setThermalAggression(int value)
{
  thermal_aggression = value;
}

/*
 * Description: Sets the value of the thermal defense stat
 *
 * Inputs: int - new value of the thermal defense stat
 * Output: none
 */
void Person::setThermalFortitude(int value)
{
  thermal_fortitude = value;
}

/*
 * Description: Sets the value of the polar attack stat
 *
 * Inputs: int - new value of the polar attack stat
 * Output: none
 */
void Person::setPolarAggression(int value)
{
  polar_aggression = value;
}

/*
 * Description: Sets the value of the polar defense stat
 *
 * Inputs: int - new value of the polar defense stat
 * Output: none
 */
void Person::setPolarFortitude(int value)
{
  polar_fortitude = value;
}

/*
 * Description: Sets the value of the primal attack stat
 *
 * Inputs: int - new value of the polar attack stat
 * Output: none
 */
void Person::setPrimalAggression(int value)
{
  primal_aggression = value;
}

/*
 * Description: Sets the value of the primal defense stat
 *
 * Inputs: int - new value of the primal defense stat
 * Output: none
 */
void Person::setPrimalFortitude(int value)
{
  primal_fortitude = value;
}

/*
 * Description: Sets the value of the charged attack stat
 *
 * Inputs: int - new value of the primal attack stat
 * Output: none
 */
void Person::setChargedAggression(int value)
{
  charged_aggression = value;
}

/*
 * Description: Sets the value of the charged defense stat
 *
 * Inputs: int - new value of the charged defense stat
 * Output: none
 */
void Person::setChargedFortitude(int value)
{
  charged_fortitude = value;
}

/*
 * Description: Sets the value of the cybernetic attack stat
 *
 * Inputs: int - new value of the cybernetic attack stat
 * Output: none
 */
void Person::setCyberneticAggression(int value)
{
  cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the cybernetic defense stat
 *
 * Inputs: int - new value of the cybernetic defense stat
 * Output: none
 */
void Person::setCyberneticFortitude(int value)
{
  cybernetic_fortitude = value;
}

/*
 * Description: Sets the value of the nihil attack stat
 *
 * Inputs: int - new value of the nihil attack stat
 * Output: none
 */
void Person::setNihilAggression(int value)
{
  nihil_aggression = value;
}

/*
 * Description: Sets the value of the nihil defense stat
 *
 * Inputs: int - new value of the nihil defense stat
 * Output: none
 */
void Person::setNihilFortitude(int value)
{
  nihil_fortitude = value;
}

/*
 * Description: Sets the value of the vitality stat
 *
 * Inputs: int - new value of the vitaltiy stat
 * Output: none
 */
void Person::setVitality(int value)
{
  vitality = value;
}

/*
 * Description: Sets the value of the quantum drive stat
 *
 * Inputs: int - new value of the quantum drive stat
 * Output: none
 */
void Person::setQuantumDrive(int value)
{
  quantum_drive = value;
}

/*
 * Description: Sets the value of the agility stat
 *
 * Inputs: int - new value of the agility stat
 * Output: none
 */
void Person::setAgility(int value)
{
  agility = value;
}

/*
 * Description: Sets the value of the limbertude stat
 *
 * Inputs: int - new value of the limbertude stat
 * Output: noe
 */
void Person::setLimbertude(int value)
{
  limbertude = value;
}

/*
 * Description: Sets the value of the unbearability stat
 *
 * Inputs: int - new value of the unbearability stat
 * Output: none
 */
void Person::setUnbearability(int value)
{
  unbearability = value;
}

/*
 * Description: Sets the value of the temp thermal attack stat
 *
 * Inputs: int - new value of the temp thermal attack stat
 * Output: none
 */
void Person::setTempThermalAggression(int value)
{
  temp_thermal_aggression = value;
}

/*
 * Description: Sets the value of the temp thermal defense stat
 *
 * Inputs: int - new value of the temp thermal defense stat
 * Output: none
 */
void Person::setTempThermalFortitude(int value)
{
  temp_thermal_fortitude = value;
}

/*
 * Description: Sets the value of the temp polar attack stat
 *
 * Inputs: int - new value of the temp polar attack stat
 * Output: none
 */
void Person::setTempPolarAggression(int value)
{
  temp_polar_aggression = value;
}

/*
 * Description: Sets the value of the temp polar defense stat
 *
 * Inputs: int - new value of the temp polar defense stat
 * Output: none
 */
void Person::setTempPolarFortitude(int value)
{
  temp_polar_fortitude = value;
}

/*
 * Description: Sets the value of the temp primal attack stat
 *
 * Inputs: int - new value of the temp primal attack stat
 * Output: none
 */
void Person::setTempPrimalAggression(int value)
{
  temp_primal_aggression = value;
}

/*
 * Description: Sets the value of the temp primal defense stat
 *
 * Inputs: int - new value of the temp primal defense stat
 * Output: none
 */
void Person::setTempPrimalFortitude(int value)
{
  temp_primal_fortitude = value;
}

/*
 * Description: Sets the value of the temp charged attack stat
 *
 * Inputs: int - new value of the temp charged attack stat
 * Output: none
 */
void Person::setTempChargedAggression(int value)
{
  temp_charged_aggression = value;
}

/*
 * Description: Sets the value of the temp charged defense stat
 *
 * Inputs: int - new value of the temp charged defense stat
 * Output: none
 */
void Person::setTempChargedFortitude(int value)
{
  temp_charged_fortitude = value;
}

/*
 * Description: Sets the value of the temp cybernetic attack stat
 *
 * Inputs: int - new value of the temp cybernetic attack
 * Output: none
 */
void Person::setTempCyberneticAggression(int value)
{
  temp_cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the temp cybernetic defense stat
 *
 * Inputs: int - new value of the cybernetic defense stat
 * Output: none
 */
void Person::setTempCyberneticFortitude(int value)
{
  temp_cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the temp nihil attack stat
 *
 * Inputs: int - new value of the temp nihil attack stat
 * Output: none
 */
void Person::setTempNihilAggression(int value)
{
  temp_nihil_aggression = value;
}

/*
 * Description: Sets the value of the temp nihil defense stat
 *
 * Inputs: int - new value of the temp nihil defense stat
 * Output: none
 */
void Person::setTempNihilFortitude(int value)
{
  temp_nihil_fortitude = value;
}

/*
 * Description: Sets the value of the temp vitality stat
 *
 * Inputs: int - new value of the temp vitality stat
 * Output: none
 */
void Person::setTempVitality(int value)
{
  temp_vitality = value;
}

/*
 * Description: Sets the value of the temp quantum drive stat
 *
 * Inputs: int - new value of the temp quantum drive stat
 * Output: none
 */
void Person::setTempQuantumDrive(int value)
{
  temp_quantum_drive = value;
}

/*
 * Description: Sets the value of the temp agility stat
 *
 * Inputs: int - new value of the temp agility stat
 * Output: none
 */
void Person::setTempAgility(int value)
{
  temp_agility = value;
}

/*
 * Description: Sets the value of the temp limbertude stat
 *
 * Inputs: int - new value of the temp limbertude stat
 * Output: none
 */
void Person::setTempLimbertude(int value)
{
  temp_limbertude = value;
}

/*
 * Description: Sets the value of the temp unbearability stat
 *
 * Inputs: int - new value of the temp unbearability stat
 * Output: none
 */
void Person::setTempUnbearability(int value)
{
  temp_unbearability = value;
}

/*
 * Description: Sets the value of the base thermal attack stat
 *
 * Inputs: int - new value of the base thermal attack stat
 * Output: none
 */
void Person::setBaseThermalAggression(int value)
{
  base_thermal_aggression = value;
}

/*
 * Description: Sets the value of the base thermal defense stat
 *
 * Inputs: int - new value of te base thermal defense stat
 * Output: none
 */
void Person::setBaseThermalFortitude(int value)
{
  base_thermal_fortitude = value;
}

/*
 * Description: Sets the value of the base polar attack stat
 *
 * Inputs: int - new value of the base polar attack stat
 * Output: none
 */
void Person::setBasePolarAggression(int value)
{
  base_polar_aggression = value;
}

/*
 * Description: Sets the value of te base polar defense stat
 *
 * Inputs: int - new value of the base polar defense stat
 * Output: none
 */
void Person::setBasePolarFortitude(int value)
{
  base_polar_fortitude = value;
}

/*
 * Description: Sets the value of the base primal attack stat
 *
 * Inputs: int - new value of the base primal attack stat
 * Output: none
 */
void Person::setBasePrimalAggression(int value)
{
  base_primal_aggression = value;
}

/*
 * Description: Sets the value of the base primal defense stat
 *
 * Inputs: int - new value of the base primal defense stat
 * Output: none
 */
void Person::setBasePrimalFortitude(int value)
{
  base_primal_fortitude = value;
}

/*
 * Description: Sets the value of the base charged attack stat
 *
 * Inputs: int - new value of the base charged attack stat
 * Output: none
 */
void Person::setBaseChargedAggression(int value)
{
  base_charged_aggression = value;
}

/*
 * Description: Sets the value of the base charged defense stat
 *
 * Inputs: int - new value of the base charged defense stat
 * Output: none
 */
void Person::setBaseChargedFortitude(int value)
{
  base_charged_fortitude = value;
}

/*
 * Description: Sets the value of the base cybernetic attack stat
 *
 * Inputs: int - new value of the base cybernetic defense stat
 * Output: none
 */
void Person::setBaseCyberneticAggression(int value)
{
  base_cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the base cybernetic defense stat
 *
 * Inputs: int - new value of the base cybernetic defense stat
 * Output: none
 */
void Person::setBaseCyberneticFortitude(int value)
{
  base_cybernetic_aggression = value;
}

/*
 * Description: Sets the value of the base nihil attack stat
 *
 * Inputs: int - new value of the base nihil attack stat
 * Output: none
 */
void Person::setBaseNihilAggression(int value)
{
  base_nihil_aggression = value;
}

/*
 * Description: Sets the value of the base nihil defense stat
 *
 * Inputs: int - new value of the base nihil defense stat
 * Output: none
 */
void Person::setBaseNihilFortitude(int value)
{
  base_nihil_fortitude = value;
}

/*
 * Description: Sets the value of the base vitality stat
 *
 * Inputs: int - new value of the base vitality stat
 * Output: none
 */
void Person::setBaseVitality(int value)
{
  base_vitality = value;
}

/*
 * Description: Sets the value of the base quantum drive stat
 *
 * Inputs: int - new value of the base quantum drive stat
 * Output: none
 */
void Person::setBaseQuantumDrive(int value)
{
  base_quantum_drive = value;
}

/*
 * Description: Sets the value of the base agility stat
 *
 * Inputs: int - new value of the base agility stat
 * Output: none
 */
void Person::setBaseAgility(int value)
{
  base_agility = value;
}

/*
 * Description: Sets the value of the base limbertude stat
 *
 * Inputs: int - new value of the base limbertude stat
 * Output: none
 */
void Person::setBaseLimbertude(int value)
{
  base_limbertude = value;
}

/*
 * Description: Sets the value of the base unbearability stat
 *
 * Inputs: int - new value of the base unbearability stat
 * Output: none
 */
void Person::setBaseUnbearability(int value)
{
  base_unbearability = value;
}
