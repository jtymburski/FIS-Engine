/******************************************************************************
* Category Name: Category
* Date Created: December 2nd, 2012
* Inheritance: Parent Category - Person
* Description: Header for Category that defines the Category for a person.
*              This is a battle class like Warrior, etc.
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/

#include "Game/Player/Category.h"

/*
 * Description: Category constructor object. Constructs a Category given a string and
 *              a full set of base stat values. None of the Category constructors
 *              set the description of the Category, or the actions available.
 *              These must be set up afterwards.
 *
 * Inputs: n - name of the Category
 *      ph_a - base physical aggression
 *      ph_f - base physical fortitude
 *      th_a - base thermal aggression
 *      th-f - base thermal fortitude
 *      po_a - base polar aggression
 *      po_f - base polar fortitude
 *      pr_a - base primal aggresion
 *      pr_f - base primal fortitude
 *      ch_a - base charged aggression
 *      ch_f - base charged fortitue
 *      cy_a - base cybernetic aggression
 *      cy_f - base cybernetic fortitude
 *      ni_a - base nihil aggression
 *      ni_f - base nihil fortitude
 *      vit  - base vitality
 *      qd   - base quantum drive
 *      agi  - base agility
 *      lim  - base limbertude
 *      unb  - base unbearability
 */
Category::Category(QString n, uint ph_a, uint ph_f, uint th_a,uint th_f, uint po_a,
           uint po_f, uint pr_a, uint pr_f, uint ch_a,uint ch_f, uint cy_a,
           uint cy_f, uint ni_a, uint ni_f, uint vit, uint qd, uint agi,
           uint lim, uint unb, QWidget *parent)
{
    setName(n);
    setUpStats(ph_a,ph_f,th_a,th_f,po_a,po_f,pr_a,pr_f,ch_a,ch_f,cy_a,cy_f,
               ni_a,ni_f,vit,qd,agi,lim,unb);
}

/*
 * Description: Default Category object constructor
 *
 * Inputs: n - name of the Category
 */
Category::Category(QString n, QWidget *parent)
{
    setName(n);
    setUpStats(10,7,8,5,8,5,8,5,8,5,8,5,8,5,200,100,10,12,3);
}

/*
 * Description: Destroys a Category object
 */
Category::~Category()
{
}

/*
 * Description: Set up stats function (for constructors)
 *
 * Inputs: See list in constructor.
 */
void Category::setUpStats(uint ph_a, uint ph_f, uint th_a, uint th_f, uint po_a,
                      uint po_f, uint pr_a, uint pr_f, uint ch_a,uint ch_f, uint cy_a,
                      uint cy_f, uint ni_a, uint ni_f, uint vit, uint qd, uint agi,
                      uint lim, uint unb)
{
    setPhysicalAggression(ph_a);
    setPhysicalFortitude(ph_f);
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
    setAgility(agi);
    setLimbertude(lim);
    setUnbearability(unb);
}

/*
 * Description: Sets the name of the Category
 *
 * Inputs: QString - new name to be set
 * Output: none
 */
void Category::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the Category bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the physical aggression bonus stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPhysicalAggression(uint value)
{
  physical_aggression = value;
}

/*
 * Description: Sets the physical fortitude bonus stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPhysicalFortitude(uint value)
{
  physical_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setThermalAggression(uint value)
{
  thermal_aggression = value;
}

/*
 * Description: Sets the Category bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setThermalFortitude(uint value)
{
   thermal_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the polar attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPolarAggression(uint value)
{
  polar_aggression = value;
}

/*
 * Description: Sets the Category bonus value of the polar defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPolarFortitude(uint value)
{
  polar_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the primal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPrimalAggression(uint value)
{
  primal_aggression = value;
}

/*
 * Description: Sets the Category bonus value of the primal defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPrimalFortitude(uint value)
{
  primal_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the charged attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setChargedAggression(uint value)
{
  charged_aggression = value;
}

/*
 * Description: Sets the Category bonus value of the charged defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setChargedFortitude(uint value)
{
  charged_fortitude = value;
}

/*
 * Description: Sets the Category bonus of the cybernetic attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setCyberneticAggression(uint value)
{
  cybernetic_aggression = value;
}

/*
 * Description: Sets the Category bonus of the cybernetic defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setCyberneticFortitude(uint value)
{
  cybernetic_fortitude = value;
}

/*
 * Description: Sets the Category bonus of the nihil attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setNihilAggression(uint value)
{
  nihil_aggression = value;
}

/*
 * Description: Sets the Category bonus of the nihil fortitude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setNihilFortitude(uint value)
{
  nihil_fortitude = value;
}

/*
 * Description: Sets the Category bonus of the vitality stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setVitality(uint value)
{
  vitality = value;
}

/*
 * Description: Sets the Category bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setQuantumDrive(uint value)
{
  quantum_drive = value;
}

/*
 * Description: Sets the Category bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setAgility(uint value)
{
  agility = value;
}

/*
 * Description: Sets the Category bonus of the limbertude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setLimbertude(uint value)
{
  limbertude = value;
}

/*
 * Description: Sets the Category bonus of the unbearability stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setUnbearability(uint value)
{
  unbearability = value;
}

/*
 * Description: Returns the name of the Category
 *
 * Inputs: none
 * Output: QString - name of the Category
 */
QString Category::getName()
{
  return name;
}


/*
 * Description: Returns the description of the Category
 *
 * Inputs: none
 * Output: QString - description of the Category
 */
QString Category::getDescription()
{
  return description;
}


/*
 * Description: Returns the physical atk bonus
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getPhysicalAggression()
{
  return physical_aggression;
}

/*
 * Description: Returns the physical def bonus
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getPhysicalFortitude()
{
  return physical_fortitude;
}

/*
 * Description: Returns the Category bonus of the thermal attack stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getThermalAggression()
{
  return thermal_aggression;
}

/*
 * Description: Returns the Category bonus of the thermal defense stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getThermalFortitude()
{
  return thermal_fortitude;
}

/*
 * Description: Returns the Category bonus of the polar attack stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getPolarAggression()
{
  return polar_aggression;
}

/*
 * Description: Returns the Category bonus of the polar defense stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getPolarFortitude()
{
  return polar_fortitude;
}

/*
 * Description: Returns the Category bonus of the primal attack stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getPrimalAggression()
{
  return primal_aggression;
}

/*
 * Description: Returns the Category bonus of the primal defense stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getPrimalFortitude()
{
  return primal_fortitude;
}

/*
 * Description: Returns the Category bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getChargedAggression()
{
  return charged_aggression;
}

/*
 * Description: Returns the Category bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getChargedFortitude()
{
  return charged_fortitude;
}

/*
 * Description: Returns the Category bonus of the cybernetic attack stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/*
 * Description: Returns the Category bonus of the cybernetic defense stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getCyberneticFortitude()
{
  return cybernetic_fortitude;
}

/*
 * Description: Returns the Category bonus
 *
 * Inputs: none
 * Output: int - value of the bonus
 */
uint Category::getNihilAggression()
{
  return nihil_aggression;
}

/*
 * Description: Returns the Category bonus of the nihil defense stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getNihilFortitude()
{
  return nihil_fortitude;
}

/*
 * Description: Returns the Category bonus of the vitality stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getVitality()
{
  return vitality;
}

/*
 * Description: Returns the Category bonus of the quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getQuantumDrive()
{
  return quantum_drive;
}

/*
 * Description: Returns the Category bonus of the agility stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getAgility()
{
  return agility;
}

/*
 * Description: Returns the Category bonus of the limbertude stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getLimbertude()
{
  return limbertude;
}

/*
 * Description: Returns the Category bonus of the unbearability stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getUnbearability()
{
  return unbearability;
}
