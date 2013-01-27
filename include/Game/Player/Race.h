/******************************************************************************
* Class Name: Race
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class - Person
* Description: Header for Race that defines the race for the particular person.
*              Some examples includes Bears, Humans, etc.
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/
#ifndef RACE_H
#define RACE_H

#include <QtGui/QWidget>

#include "Game/Player/Action.h"

class Race : public QWidget
{
public:
   /* Race constructor */
   Race(QWidget *parent = 0);

   /* Annihilates a race object */
   ~Race();
  
private:
  /* Description */
  QString description;

  /* Name */
  QString name;

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

  /* When the actions become available (based on level) */
  QVector<int> action_available; 

  /* Actions available for this race */
  QVector<Action*> action_list; 

public:
  /* Sets name of the race */
  void setName(QString s); 

  /* Sets the description of the race */
  void setDescription(QString s); 

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
};

#endif // RACE_H
