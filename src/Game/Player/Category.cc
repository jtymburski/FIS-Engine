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

/* Creates a Category object */
Category::Category(QWidget *parent = 0)
{
}

/* Annihilate a Category object */
Category::~Category()
{
}

/* Sets name of the race */
void Category::setName(QString new_name)
{
  name = new_name
}

/* Sets the description of the race */
void Category::setDescription(QString new_description)
{
  description = new_description;
}

/* Sets the Fire atk stat */
void Category::setThermalAggression(int value)
{
  thermal_aggression = value; 
}

/* Sets the Fire def stat */
void Category::setThermalFortitude(int value)
{
   thermal_fortitude = value;
}

/* Sets the Ice attack stat */
void Category::setPolarAggression(int value)
{
  polar_aggression = value;
}

/* Sets the Ice defense stat */ 
void Category::setPolarFortitude(int value)
{
  polar_fortitude = value;
}

/* Sets the Forest attack stat */
void Category::setPrimalAggression(int value)
{
  primal_aggression = value;
}

/* Sets the Forest defense stat */
void Categoy::setPrimalFortitude(int value)
{
  primal_fortitude = value;
}

/* Sets the Electric attack stat */
void Category::setChargedAggression(int value)
{
  charged_aggression = value;
}

/* Sets the Electric defence stat */
void Category::setChargedFortitude(int value)
{
  charged_fortitude = value;
}

/* Sets the Digital attack stat */
void Category::setCyberneticAggression(int value)
{
  cybernetic_aggression = value;
}

/* Sets the Digital defence stat*/
void Category::setCyberneticFortitude(int value)
{
  cybernetics_fortitude = value;
}

/* Sets the Void attack stat */
void Category::setNihilAggression(int value)
{
  nihil_aggression = value;
}

/* Sets the Void defence stat */
void Category::setNihilFortitude(int value)
{
  nihil_fortitude = value;
}

/* Sets the Health stat */
void Category::setVitality(int value)
{
  vitality = value;
}

/* Sets the Power points (shown as a percentage) */
void Category::setQuantumDrive(int value)
{
  quantum_drive = value;
}

/* Sets the Speed stat */
void Category::setAgility(int value)
{
  agility = value;
}

/* Sets the Avoidance stat */
void Category::setLimbertude(int value)
{
  limbertude = value;
}

/* Sets the Critical chance stat */
void Category::setUnbearability(int value)
{
  unbearability = value;
}

/* Gets the Fire attack stat */
int Category::getThermalAggression()
{
  return thermal_aggression;
}

/* Gets the Fire defense stat */
int Category::getThermalFortitude()
{
  return thermal_fortitude;
}

/*Gets the Ice attack stat */
int Category::getPolarAggression() 
{
  return polar_aggression;
}

/* Gets the Ice defense stat */
int Category::getPolarFortitude()
{
  return polar_fortitude;
}

/* Gets the Forest attack stat */
int Category::getPrimalAggression()
{
  return primal_aggression;
}

/* Gets the Forest defense stat */
 int Category::getPrimalFortitude()
 {
   return primal_fortitude;
 }

/* Gets the Electric attack stat */
int Category::getChargedAggression()
{
  return charged_aggression;
}

/* Gets the Electric defence stat */
int Category::getChargedFortitude()
{
  return charged_fortitude;
}

/* Gets the Digital attack stat */
int Category::getCyberneticAggression()
{
  return cybernetic_aggression;
}

/* Gets the Digital defence stat */
 int Category::getCyberneticFortitude()
 {
   return cybernetic_fortitude;
 }

/* Gets the Void attack stat */
int Category::getNihilAggression()
{
  return nihil_aggression;
}

/* Gets the Void defence stat */
int Category::getNihilFortitude()
{
  return nihil_fortiude;
}

/* Gets the Health stat */
int Category::getVitality() {
  return vitality;
}

/* Gets the Power points (shown as a percentage) */
int Category::getQuantumDrive()
{
  return quantum_drive;
}

/* Gets the Speed stat */
int Category::getAgility()
{
  return agility;
}

/* Gets the Avoidance stat */
int Category::getLimbertude()
{
  return limbertude;
}

/* Gets the Critical chance stat */
int Category::getUnbearability()
{
  return unbearability;
}