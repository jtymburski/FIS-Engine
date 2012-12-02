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

class Race : public QWidget
{
public:
   Race(QWidget *parent = 0);
   ~Race();
  
private:
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

  /* Sets the Fire atk stat */
  void setThermalAggression(int value);

  /* Sets the Fire def stat */
  void setThermalFortitude(int value);

  /* Sets the Ice attack stat */
  void setPolarAggression(int value);

  /* Sets the Ice defense stat */ 
  void setPolarFortitude(int value); 

  /* Sets the Forest attack stat */
  void setPrimalAggression(int value); 

  /* Sets the Forest defense stat */
  void setPrimalFortitude(int value); 

  /* Sets the Electric attack stat */
  void setChargedAggression(int value); 

  /* Sets the Electric defence stat */
  void setChargedFortitude(int value); 

  /* Sets the Digital attack stat */
  void setCyberneticAggression(int value); 

  /* Sets the Digital defence stat*/
  void setCyberneticFortitude(int value); 

  /* Sets the Void attack stat */
  void setNihilAggression(int value); 

  /* Sets the Void defence stat */
  void setNihilFortitude(int value); 

  /* Sets the Health stat */
  void setVitality(int value); 

  /* Sets the Power points (shown as a percentage) */
  void setQuantumDrive(int value);

  /* Sets the Speed stat */
  void setAgility(int value);

  /* Sets the Avoidance stat */
  void setLimbertude(int value); 

  /* Sets the Critical chance stat */
  void setUnbearability(int value); 

  /* Gets the Fire attack stat */
  int getThermalAggression(); 

  /* Gets the Fire defense stat */
  int getThermalFortitude(); 

  /*Gets the Ice attack stat */
  int getPolarAggression();

  /* Gets the Ice defense stat */
  int getPolarFortitude();

  /* Gets the Forest attack stat */
  int getPrimalAggression();

  /* Gets the Forest defense stat */
  int getPrimalFortitude(); 

  /* Gets the Electric attack stat */
  int getChargedAggression();

  /* Gets the Electric defence stat */
  int getChargedFortitude(); 

  /* Gets the Digital attack stat */
  int getCyberneticAggression(); 

  /* Gets the Digital defence stat */
  int getCyberneticFortitude(); 

  /* Gets the Void attack stat */
  int getNihilAggression(); 

  /* Gets the Void defence stat */
  int getNihilFortitude(); 

  /* Gets the Health stat */
  int getVitality(); 

  /* Gets the Power points (shown as a percentage) */
  int getQuantumDrive(); 

  /* Gets the Speed stat */
  int getAgility(); 

  /* Gets the Avoidance stat */
  int getLimbertude();

  /* Gets the Critical chance stat */
   int getUnbearability();   
};

#endif // RACE_H
