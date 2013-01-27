/******************************************************************************
* Class Name: Category Implementation
* Date Created: December 2nd, 2012
* Inheritance: Parent class: Person 
* Description: Header that defines the category for the person class. This
*              category is like Warrior, Clerk, etc.
*
* TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/

#include "Game/Player/Category.h"

/*
 * Description: Constructor for a category object.
 *
 * Inputs: none
 */
Category::Category(QWidget* parent)
{
}

/*
 * Description: Annihilates a category object
 */
Category::~Category()
{
}

/*
 * Description: Sets the name of the category
 *
 * Inputs: QString - new value of the category
 * Output: none
 */
void Category::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the description of the category
 *
 * Inputs: QString - new value of the description
 * Output: none
 */
void Category::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the class bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setThermalAggression(int value)
{
  thermal_aggression = value; 
}

/*
 * Description: Sets the class bonus value of the thermal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setThermalFortitude(int value)
{
   thermal_fortitude = value;
}

/*
 * Description: Sets the class bonus value of the polar attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPolarAggression(int value)
{
  polar_aggression = value;
}

/*
 * Description: Sets the class bonus value of the polar defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPolarFortitude(int value)
{
  polar_fortitude = value;
}

/*
 * Description: Sets the class bonus value of the primal attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPrimalAggression(int value)
{
  primal_aggression = value;
}

/*
 * Description: Sets the class bonus value of the primal defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setPrimalFortitude(int value)
{
  primal_fortitude = value;
}

/*
 * Description: Sets the class bonus value of the charged attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setChargedAggression(int value)
{
  charged_aggression = value;
}

/*
 * Description: Sets the class bonus value of the charged defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setChargedFortitude(int value)
{
  charged_fortitude = value;
}

/*
 * Description: Sets the class bonus of the cybernetic attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setCyberneticAggression(int value)
{
  cybernetic_aggression = value;
}

/*
 * Description: Sets the class bonus of the cybernetic defense stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setCyberneticFortitude(int value)
{
  cybernetic_fortitude = value;
}

/*
 * Description: Sets the class bonus of the nihil attack stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setNihilAggression(int value)
{
  nihil_aggression = value;
}

/*
 * Description: Sets the class bonus of the nihil fortitude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setNihilFortitude(int value)
{
  nihil_fortitude = value;
}

/*
 * Description: Sets the class bonus of the vitality stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setVitality(int value)
{
  vitality = value;
}

/*
 * Description: Sets the class bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setQuantumDrive(int value)
{
  quantum_drive = value;
}

/*
 * Description: Sets the class bonus of the quantum drive stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setAgility(int value)
{
  agility = value;
}

/*
 * Description: Sets the class bonus of the limbertude stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setLimbertude(int value)
{
  limbertude = value;
}

/*
 * Description: Sets the class bonus of the unbearability stat
 *
 * Inputs: int - value of the bonus
 * Output: none
 */
void Category::setUnbearability(int value)
{
  unbearability = value;
}

/*
 * Description: Returns the class bonus of the thermal attack stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getThermalAggression()
{
  return thermal_aggression;
}

/*
 * Description: Returns the class bonus of the thermal defense stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getThermalFortitude()
{
  return thermal_fortitude;
}

/*
 * Description: Returns the class bonus of the polar attack stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getPolarAggression() 
{
  return polar_aggression;
}

/*
 * Description: Returns the class bonus of the polar defense stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getPolarFortitude()
{
  return polar_fortitude;
}

/*
 * Description: Returns the class bonus of the primal attack stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getPrimalAggression()
{
  return primal_aggression;
}

/*
 * Description: Returns the class bonus of the primal defense stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getPrimalFortitude()
{
  return primal_fortitude;
}

/*
 * Description: Returns the class bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getChargedAggression()
{
  return charged_aggression;
}

/*
 * Description: Returns the class bonus of the charged defense stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getChargedFortitude()
{
  return charged_fortitude;
}

/*
 * Description: Returns the class bonus of the cybernetic attack stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/*
 * Description: Returns the class bonus of the cybernetic defense stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getCyberneticFortitude()
{
  return cybernetic_fortitude;
}

/*
 * Description: Returns the class bonus
 *
 * Inputs: none
 * Output: int - value of the bonus
 */
int Category::getNihilAggression()
{
  return nihil_aggression;
}

/*
 * Description: Returns the class bonus of the nihil defense stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getNihilFortitude()
{
  return nihil_fortitude;
}

/*
 * Description: Returns the class bonus of the vitality stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getVitality()
{
  return vitality;
}

/*
 * Description: Returns the class bonus of the quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getQuantumDrive()
{
  return quantum_drive;
}

/*
 * Description: Returns the class bonus of the agility stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getAgility()
{
  return agility;
}

/*
 * Description: Returns the class bonus of the limbertude stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getLimbertude()
{
  return limbertude;
}

/*
 * Description: Returns the class bonus of the unbearability stat
 *
 * Inputs: none
 * Output: int - value of the class bonus
 */
int Category::getUnbearability()
{
  return unbearability;
}
