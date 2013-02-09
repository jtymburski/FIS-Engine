/******************************************************************************
* Class Name: Category
* Date Created: Sunday, October 28th, 2012
* Inheritance:
* Description: Header that defines the category for the person class. This
*              category is like Warrior, Clerk, etc.
*
* TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/
#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtGui/QWidget>

#include "Game/Player/Action.h"

class Category : public QWidget
{
public:
   /* Category constructor */
   Category(QString n, uint ph_a, uint ph_f, uint th_a, uint th_f, uint po_a,
            uint po_f, uint pr_a, uint pr_f, uint ch_a,uint ch_f, uint cy_a,
            uint cy_f, uint ni_a, uint ni_f, uint vit, uint qd, uint agi,
            uint lim, uint unb, QWidget *parent = 0);

   /* Default Category Constructor given a name */
   Category(QString n, QWidget *parent = 0);

   /* Annihilates a Category object */
   ~Category();

private:
   /* Set up stats function for the constructor */
   void setUpStats(uint ph_a, uint ph_f, uint th_a, uint th_f, uint po_a,
                   uint po_f, uint pr_a, uint pr_f, uint ch_a, uint ch_f,
                   uint cy_a, uint cy_f, uint ni_a, uint ni_f, uint vit,
                   uint qd, uint agi, uint lim, uint unb);

  /* Description */
  QString description;

  /* Name */
  QString name;

  /* Physical atk stat */
  uint physical_aggression;

  /* Physical def stat */
  uint physical_fortitude;

  /* Fire atk stat */
  uint thermal_aggression;

  /* Fire def stat */
  uint thermal_fortitude;

  /* Ice atk stat */
  uint polar_aggression;

  /* Ice def stat */
  uint polar_fortitude;

  /* Forest atk stat */
  uint primal_aggression;

  /* Forest def stat */
  uint primal_fortitude;

  /* Electric atk stat */
  uint charged_aggression;

  /* Electric def stat */
  uint charged_fortitude;

  /* Digital atk stat */
  uint cybernetic_aggression;

  /* Digital def stat */
  uint cybernetic_fortitude;

  /* Void atk stat */
  uint nihil_aggression;

  /* Void def stat */
  uint nihil_fortitude;

  /* Health stat */
  uint vitality;

  /* Quantum drive (MP), shown as percentage */
  uint quantum_drive;

  /* Speed stat */
  uint agility;

  /* Dodge stat */
  uint limbertude;

  /* Critical chance stat */
  int unbearability;

  /* When the actions become available (based on level) */
  QVector<uint> action_available;

  /* Actions available for this Category */
  QVector<Action*> action_list;

public:
  /* Sets name of the Category */
  void setName(QString s);

  /* Sets the description of the Category */
  void setDescription(QString s);

  /* Sets the physical attack stat */
  void setPhysicalAggression(uint value);

  /* Sets the physical defense stat */
  void setPhysicalFortitude(uint value);

  /* Sets the fire attack stat */
  void setThermalAggression(uint value);

  /* Sets the fire defense stat */
  void setThermalFortitude(uint value);

  /* Sets the ice attack stat */
  void setPolarAggression(uint value);

  /* Sets the ice defense stat */
  void setPolarFortitude(uint value);

  /* Sets the forest attack stat */
  void setPrimalAggression(uint value);

  /* Sets the forest defense stat */
  void setPrimalFortitude(uint value);

  /* Sets the electric attack stat */
  void setChargedAggression(uint value);

  /* Sets the electric defense stat */
  void setChargedFortitude(uint value);

  /* Sets the digital attack stat */
  void setCyberneticAggression(uint value);

  /* Sets the digital defense stat */
  void setCyberneticFortitude(uint value);

  /* Sets the void attack stat */
  void setNihilAggression(uint value);

  /* Sets the void defense stat */
  void setNihilFortitude(uint value);

  /* Sets the health stat */
  void setVitality(uint value);

  /* Sets the power points (shown as percentage) */
  void setQuantumDrive(uint value);

  /* Sets the speed stat */
  void setAgility(uint value);

  /* Sets the dodge stat */
  void setLimbertude(uint value);

  /* Sets the critical chance stat */
  void setUnbearability(uint value);

  /* Gets the name of the class */
  QString getName();

  /* Gets the description of the class */
  QString getDescription();

  /* Sets the physical attack stat */
  uint getPhysicalAggression();

  /* Sets the physical defense stat */
  uint getPhysicalFortitude();

  /* Gets the fire attack stat */
  uint getThermalAggression();

  /* Gets the fire defense stat */
  uint getThermalFortitude();

  /* Gets the ice attack stat */
  uint getPolarAggression();

  /* Gets the ice defense stat */
  uint getPolarFortitude();

  /* Gets the forest attack stat */
  uint getPrimalAggression();

  /* Gets the forest defense stat */
  uint getPrimalFortitude();

  /* Gets the electric attack stat */
  uint getChargedAggression();

  /* Gets the electric defense stat */
  uint getChargedFortitude();

  /* Gets the digital attack stat */
  uint getCyberneticAggression();

  /* Gets the digital defense stat */
  uint getCyberneticFortitude();

  /* Gets the void attack stat */
  uint getNihilAggression();

  /* Gets the void defense stat */
  uint getNihilFortitude();

  /* Gets the health stat */
  uint getVitality();

  /* Gets the power points (shown as a percentage) */
  uint getQuantumDrive();

  /* Gets the speed stat */
  uint getAgility();

  /* Gets the dodge stat */
  uint getLimbertude();

   /* Gets the critical chance stat */
  uint getUnbearability();
};

#endif // CATEGORY_H
