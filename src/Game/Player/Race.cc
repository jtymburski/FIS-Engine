/******************************************************************************
* Class Name: Race
* Date Created: December 2nd, 2012
* Inheritance: Parent class - Person
* Description: Header for Race that defines the race for the particular person.
*              Some examples includes Bears, Humans, etc.
******************************************************************************/

#include "Game/Player/Race.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Race constructor object. Constructs a race given a string and
 *              a full set of base stat values. None of the race constructors
 *              set the description of the race, or the actions available.
 *              These must be set up afterwards.
 *
 * Inputs: n - name of the Race
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
 *      mom  - base momentum
 *      lim  - base limbertude
 *      unb  - base unbearability
 */
Race::Race(QString n, uint ph_a, uint ph_f, uint th_a,uint th_f, uint po_a,
           uint po_f, uint pr_a, uint pr_f, uint ch_a,uint ch_f, uint cy_a,
           uint cy_f, uint ni_a, uint ni_f, uint vit, uint qd, uint mom,
           uint lim, uint unb, QWidget *parent)
{
    setName(n);
    setUpStats(ph_a,ph_f,th_a,th_f,po_a,po_f,pr_a,pr_f,ch_a,ch_f,cy_a,cy_f,
               ni_a,ni_f,vit,qd,mom,lim,unb);
}

/*
 * Description: Default Race object constructor
 *
 * Inputs: n - name of the Race
 */
Race::Race(QString n, QWidget *parent)
{
    setName(n);
    setUpStats(10,7,8,5,8,5,8,5,8,5,8,5,8,5,200,100,10,12,3);
}

/*
 * Description: Destroys a race object
 */
Race::~Race()
{
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Set up stats function (for constructors)
 *
 * Inputs: See list in constructor.
 */
void Race::setUpStats(uint ph_a, uint ph_f, uint th_a, uint th_f, uint po_a,
                      uint po_f, uint pr_a, uint pr_f, uint ch_a,uint ch_f, uint cy_a,
                      uint cy_f, uint ni_a, uint ni_f, uint vit, uint qd, uint mom,
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
    setMomentum(mom);
    setLimbertude(lim);
    setUnbearability(unb);
}

/*
 * Description: Sets the name of the race
 *
 * Inputs: QString - new name to be set
 * Output: none
 */
void Race::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the race bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the physical aggression bonus stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPhysicalAggression(uint value)
{
  physical_aggression = value;
}

/*
 * Description: Sets the physical fortitude bonus stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPhysicalFortitude(uint value)
{
  physical_fortitude = value;
}

/*
 * Description: Sets the race bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setThermalAggression(uint value)
{
  thermal_aggression = value;
}

/*
 * Description: Sets the race bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setThermalFortitude(uint value)
{
   thermal_fortitude = value;
}

/*
 * Description: Sets the race bonus value of the polar attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPolarAggression(uint value)
{
  polar_aggression = value;
}

/*
 * Description: Sets the race bonus value of the polar defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPolarFortitude(uint value)
{
  polar_fortitude = value;
}

/*
 * Description: Sets the race bonus value of the primal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPrimalAggression(uint value)
{
  primal_aggression = value;
}

/*
 * Description: Sets the race bonus value of the primal defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPrimalFortitude(uint value)
{
  primal_fortitude = value;
}

/*
 * Description: Sets the race bonus value of the charged attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setChargedAggression(uint value)
{
  charged_aggression = value;
}

/*
 * Description: Sets the race bonus value of the charged defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setChargedFortitude(uint value)
{
  charged_fortitude = value;
}

/*
 * Description: Sets the race bonus of the cybernetic attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setCyberneticAggression(uint value)
{
  cybernetic_aggression = value;
}

/*
 * Description: Sets the race bonus of the cybernetic defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setCyberneticFortitude(uint value)
{
  cybernetic_fortitude = value;
}

/*
 * Description: Sets the race bonus of the nihil attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setNihilAggression(uint value)
{
  nihil_aggression = value;
}

/*
 * Description: Sets the race bonus of the nihil fortitude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setNihilFortitude(uint value)
{
  nihil_fortitude = value;
}

/*
 * Description: Sets the race bonus of the vitality stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setVitality(uint value)
{
  vitality = value;
}

/*
 * Description: Sets the race bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setQuantumDrive(uint value)
{
  quantum_drive = value;
}

/*
 * Description: Sets the race bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setMomentum(uint value)
{
  momentum = value;
}

/*
 * Description: Sets the race bonus of the limbertude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setLimbertude(uint value)
{
  limbertude = value;
}

/*
 * Description: Sets the race bonus of the unbearability stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setUnbearability(uint value)
{
  unbearability = value;
}

/*
 * Description: Returns the name of the Race
 *
 * Inputs: none
 * Output: QString - name of the race
 */
QString Race::getName()
{
  return name;
}


/*
 * Description: Returns the description of the Race
 *
 * Inputs: none
 * Output: QString - description of the race
 */
QString Race::getDescription()
{
  return description;
}


/*
 * Description: Returns the physical atk bonus
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getPhysicalAggression()
{
  return physical_aggression;
}

/*
 * Description: Returns the physical def bonus
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getPhysicalFortitude()
{
  return physical_fortitude;
}

/*
 * Description: Returns the race bonus of the thermal attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getThermalAggression()
{
  return thermal_aggression;
}

/*
 * Description: Returns the race bonus of the thermal defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getThermalFortitude()
{
  return thermal_fortitude;
}

/*
 * Description: Returns the race bonus of the polar attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getPolarAggression()
{
  return polar_aggression;
}

/*
 * Description: Returns the race bonus of the polar defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getPolarFortitude()
{
  return polar_fortitude;
}

/*
 * Description: Returns the race bonus of the primal attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getPrimalAggression()
{
  return primal_aggression;
}

/*
 * Description: Returns the race bonus of the primal defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getPrimalFortitude()
{
  return primal_fortitude;
}

/*
 * Description: Returns the race bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getChargedAggression()
{
  return charged_aggression;
}

/*
 * Description: Returns the race bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getChargedFortitude()
{
  return charged_fortitude;
}

/*
 * Description: Returns the race bonus of the cybernetic attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/*
 * Description: Returns the race bonus of the cybernetic defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getCyberneticFortitude()
{
  return cybernetic_fortitude;
}

/*
 * Description: Returns the race bonus
 *
 * Inputs: none
 * Output: int - value of the bonus
 */
uint Race::getNihilAggression()
{
  return nihil_aggression;
}

/*
 * Description: Returns the race bonus of the nihil defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getNihilFortitude()
{
  return nihil_fortitude;
}

/*
 * Description: Returns the race bonus of the vitality stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getVitality()
{
  return vitality;
}

/*
 * Description: Returns the race bonus of the quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getQuantumDrive()
{
  return quantum_drive;
}

/*
 * Description: Returns the race bonus of the momentum stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getMomentum()
{
  return momentum;
}

/*
 * Description: Returns the race bonus of the limbertude stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getLimbertude()
{
  return limbertude;
}

/*
 * Description: Returns the race bonus of the unbearability stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
uint Race::getUnbearability()
{
  return unbearability;
}
