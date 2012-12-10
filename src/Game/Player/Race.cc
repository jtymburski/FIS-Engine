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

/* Creates a Race object */
Race::Race(QWidget* parent)
{
}

/* Annihilate a Race object */
Race::~Race()
{
}

/* Sets name of the race */
void Race::setName(QString new_name)
{
  name = new_name;
}

/* Sets the description of the race */
void Race::setDescription(QString new_description)
{
  description = new_description;
}

/* Sets the Fire atk stat */
void Race::setThermalAggression(int value)
{
  thermal_aggression = value; 
}

/* Sets the Fire def stat */
void Race::setThermalFortitude(int value)
{
   thermal_fortitude = value;
}

/* Sets the Ice attack stat */
void Race::setPolarAggression(int value)
{
  polar_aggression = value;
}

/* Sets the Ice defense stat */ 
void Race::setPolarFortitude(int value)
{
  polar_fortitude = value;
}

/* Sets the Forest attack stat */
void Race::setPrimalAggression(int value)
{
  primal_aggression = value;
}

/* Sets the Forest defense stat */
void Race::setPrimalFortitude(int value)
{
  primal_fortitude = value;
}

/* Sets the Electric attack stat */
void Race::setChargedAggression(int value)
{
  charged_aggression = value;
}

/* Sets the Electric defence stat */
void Race::setChargedFortitude(int value)
{
  charged_fortitude = value;
}

/* Sets the Digital attack stat */
void Race::setCyberneticAggression(int value)
{
  cybernetic_aggression = value;
}

/* Sets the Digital defence stat*/
void Race::setCyberneticFortitude(int value)
{
  cybernetic_fortitude = value;
}

/* Sets the Void attack stat */
void Race::setNihilAggression(int value)
{
  nihil_aggression = value;
}

/* Sets the Void defence stat */
void Race::setNihilFortitude(int value)
{
  nihil_fortitude = value;
}

/* Sets the Health stat */
void Race::setVitality(int value)
{
  vitality = value;
}

/* Sets the Power points (shown as a percentage) */
void Race::setQuantumDrive(int value)
{
  quantum_drive = value;
}

/* Sets the Speed stat */
void Race::setAgility(int value)
{
  agility = value;
}

/* Sets the Avoidance stat */
void Race::setLimbertude(int value)
{
  limbertude = value;
}

/* Sets the Critical chance stat */
void Race::setUnbearability(int value)
{
  unbearability = value;
}

/* Gets the Fire attack stat */
int Race::getThermalAggression()
{
  return thermal_aggression;
}

/* Gets the Fire defense stat */
int Race::getThermalFortitude()
{
  return thermal_fortitude;
}

/*Gets the Ice attack stat */
int Race::getPolarAggression() 
{
  return polar_aggression;
}

/* Gets the Ice defense stat */
int Race::getPolarFortitude()
{
  return polar_fortitude;
}

/* Gets the Forest attack stat */
int Race::getPrimalAggression()
{
  return primal_aggression;
}

/* Gets the Forest defense stat */
 int Race::getPrimalFortitude()
 {
   return primal_fortitude;
 }

/* Gets the Electric attack stat */
int Race::getChargedAggression()
{
  return charged_aggression;
}

/* Gets the Electric defence stat */
int Race::getChargedFortitude()
{
  return charged_fortitude;
}

/* Gets the Digital attack stat */
int Race::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/* Gets the Digital defence stat */
 int Race::getCyberneticFortitude()
 {
   return cybernetic_fortitude;
 }

/* Gets the Void attack stat */
int Race::getNihilAggression()
{
  return nihil_aggression;
}

/* Gets the Void defence stat */
int Race::getNihilFortitude()
{
  return nihil_fortitude;
}

/* Gets the Health stat */
int Race::getVitality() {
  return vitality;
}

/* Gets the Power points (shown as a percentage) */
int Race::getQuantumDrive()
{
  return quantum_drive;
}

/* Gets the Speed stat */
int Race::getAgility()
{
  return agility;
}

/* Gets the Avoidance stat */
int Race::getLimbertude()
{
  return limbertude;
}

/* Gets the Critical chance stat */
int Race::getUnbearability()
{
  return unbearability;
}
