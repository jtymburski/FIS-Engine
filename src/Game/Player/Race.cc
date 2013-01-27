/******************************************************************************
* Class Name: Race
* Date Created: December 2nd, 2012
* Inheritance: Parent class - Person
* Description: Header for Race that defines the race for the particular person.
*              Some examples includes Bears, Humans, etc.
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/

#include "Game/Player/Race.h"

/*
 * Description: Race constructor object
 *
 * Inputs: none
 */
Race::Race(QWidget* parent)
{
}

/*
 * Description: Destroys a race object
 */
Race::~Race()
{
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
 * Description: Sets the race bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setThermalAggression(int value)
{
  thermal_aggression = value;
}

/*
 * Description: Sets the race bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setThermalFortitude(int value)
{
   thermal_fortitude = value;
}

/*
 * Description: Sets the race bonus value of the polar attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPolarAggression(int value)
{
  polar_aggression = value;
}

/*
 * Description: Sets the race bonus value of the polar defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPolarFortitude(int value)
{
  polar_fortitude = value;
}

/*
 * Description: Sets the race bonus value of the primal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPrimalAggression(int value)
{
  primal_aggression = value;
}

/*
 * Description: Sets the race bonus value of the primal defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setPrimalFortitude(int value)
{
  primal_fortitude = value;
}

/*
 * Description: Sets the race bonus value of the charged attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setChargedAggression(int value)
{
  charged_aggression = value;
}

/*
 * Description: Sets the race bonus value of the charged defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setChargedFortitude(int value)
{
  charged_fortitude = value;
}

/*
 * Description: Sets the race bonus of the cybernetic attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setCyberneticAggression(int value)
{
  cybernetic_aggression = value;
}

/*
 * Description: Sets the race bonus of the cybernetic defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setCyberneticFortitude(int value)
{
  cybernetic_fortitude = value;
}

/*
 * Description: Sets the race bonus of the nihil attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setNihilAggression(int value)
{
  nihil_aggression = value;
}

/*
 * Description: Sets the race bonus of the nihil fortitude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setNihilFortitude(int value)
{
  nihil_fortitude = value;
}

/*
 * Description: Sets the race bonus of the vitality stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setVitality(int value)
{
  vitality = value;
}

/*
 * Description: Sets the race bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setQuantumDrive(int value)
{
  quantum_drive = value;
}

/*
 * Description: Sets the race bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setAgility(int value)
{
  agility = value;
}

/*
 * Description: Sets the race bonus of the limbertude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setLimbertude(int value)
{
  limbertude = value;
}

/*
 * Description: Sets the race bonus of the unbearability stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Race::setUnbearability(int value)
{
  unbearability = value;
}

/*
 * Description: Returns the race bonus of the thermal attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getThermalAggression()
{
  return thermal_aggression;
}

/*
 * Description: Returns the race bonus of the thermal defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getThermalFortitude()
{
  return thermal_fortitude;
}

/*
 * Description: Returns the race bonus of the polar attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getPolarAggression()
{
  return polar_aggression;
}

/*
 * Description: Returns the race bonus of the polar defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getPolarFortitude()
{
  return polar_fortitude;
}

/*
 * Description: Returns the race bonus of the primal attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getPrimalAggression()
{
  return primal_aggression;
}

/*
 * Description: Returns the race bonus of the primal defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getPrimalFortitude()
{
  return primal_fortitude;
}

/*
 * Description: Returns the race bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getChargedAggression()
{
  return charged_aggression;
}

/*
 * Description: Returns the race bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getChargedFortitude()
{
  return charged_fortitude;
}

/*
 * Description: Returns the race bonus of the cybernetic attack stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/*
 * Description: Returns the race bonus of the cybernetic defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getCyberneticFortitude()
{
  return cybernetic_fortitude;
}

/*
 * Description: Returns the race bonus
 *
 * Inputs: none
 * Output: int - value of the bonus
 */
int Race::getNihilAggression()
{
  return nihil_aggression;
}

/*
 * Description: Returns the race bonus of the nihil defense stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getNihilFortitude()
{
  return nihil_fortitude;
}

/*
 * Description: Returns the race bonus of the vitality stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getVitality()
{
  return vitality;
}

/*
 * Description: Returns the race bonus of the quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getQuantumDrive()
{
  return quantum_drive;
}

/*
 * Description: Returns the race bonus of the agility stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getAgility()
{
  return agility;
}

/*
 * Description: Returns the race bonus of the limbertude stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getLimbertude()
{
  return limbertude;
}

/*
 * Description: Returns the race bonus of the unbearability stat
 *
 * Inputs: none
 * Output: int - value of the race bonus
 */
int Race::getUnbearability()
{
  return unbearability;
}
