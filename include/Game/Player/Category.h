/******************************************************************************
* Class Name: Category
* Date Created: Sunday, October 28th, 2012
* Inheritance:
* Description: Header for the Category class. Every Person has a Category,
*              Which holds the values for their bonus stats for battle, and
*              determines their new stats when they level up. A person's race
*              will give them a bonus in addition to what these stats provide.
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
            uint po_f, uint pr_a, uint pr_f, uint ch_a, uint ch_f,
            uint cy_a, uint cy_f, uint ni_a, uint ni_f, uint vit,
            uint qd, uint agi, uint lim, uint unb, uint ph_a_max = 0,
            uint ph_f_max = 0, uint th_a_max = 0, uint th_f_max = 0,
            uint po_a_max = 0, uint po_f_max = 0, uint pr_a_max = 0,
            uint pr_f_max = 0, uint ch_a_max = 0, uint ch_f_max = 0,
            uint cy_a_max = 0, uint cy_f_max = 0, uint ni_a_max = 0,
            uint ni_f_max = 0, uint vit_max = 0, uint qd_max = 0,
            uint agi_max = 0, uint lim_max = 0, uint unb_max = 0,
            QWidget* parent = 0);

   /* Default Category Constructor given a name */
   Category(QString n, QWidget *parent = 0);

   /* Annihilates a Category object */
   ~Category();

private:
   /* Const maximum limits for any stat */
   static const uint kPHYS_ATK_MAX = 2000;
   static const uint kPHYS_DEF_MAX = 2000;
   static const uint kTHER_ATK_MAX = 2000;
   static const uint kTHER_DEF_MAX = 2000;
   static const uint kPOLA_ATK_MAX = 2000;
   static const uint kPOLA_DEF_MAX = 2000;
   static const uint kPRIM_ATK_MAX = 2000;
   static const uint kPRIM_DEF_MAX = 2000;
   static const uint kCHAR_ATK_MAX = 2000;
   static const uint kCHAR_DEF_MAX = 2000;
   static const uint kCYBE_ATK_MAX = 2000;
   static const uint kCYBE_DEF_MAX = 2000;
   static const uint kNIHI_ATK_MAX = 2000;
   static const uint kNIHI_DEF_MAX = 2000;
   static const uint kVITA_MAX = 20000;
   static const uint kQTMN_MAX = 300;
   static const uint kAGIL_MAX = 2000;
   static const uint kLIMB_MAX = 2000;
   static const uint kUNBR_MAX = 200;

   /* Set up stats function for the constructor */
   void setUp(uint ph_a, uint ph_f, uint th_a, uint th_f, uint po_a,
              uint po_f, uint pr_a, uint pr_f, uint ch_a, uint ch_f,
              uint cy_a, uint cy_f, uint ni_a, uint ni_f, uint vit,
              uint qd, uint agi, uint lim, uint unb, uint ph_a_max = 0,
              uint ph_f_max = 0, uint th_a_max = 0, uint th_f_max = 0,
              uint po_a_max = 0, uint po_f_max = 0, uint pr_a_max = 0,
              uint pr_f_max = 0, uint ch_a_max = 0, uint ch_f_max = 0,
              uint cy_a_max = 0, uint cy_f_max = 0, uint ni_a_max = 0,
              uint ni_f_max = 0, uint vit_max = 0, uint qd_max = 0,
              uint agi_max = 0, uint lim_max = 0, uint unb_max = 0);

  /* Description */
  QString description;

  /* Name */
  QString name;

  /* Physical atk stat */
  uint physical_aggression;
  uint max_physical_aggression;

  /* Physical def stat */
  uint physical_fortitude;
  uint max_physical_fortitude;

  /* Fire atk stat */
  uint thermal_aggression;
  uint max_thermal_aggression;

  /* Fire def stat */
  uint thermal_fortitude;
  uint max_thermal_fortitude;

  /* Ice atk stat */
  uint polar_aggression;
  uint max_polar_aggression;

  /* Ice def stat */
  uint polar_fortitude;
  uint max_polar_fortitude;

  /* Forest atk stat */
  uint primal_aggression;
  uint max_primal_aggression;

  /* Forest def stat */
  uint primal_fortitude;
  uint max_primal_fortitude;

  /* Electric atk stat */
  uint charged_aggression;
  uint max_charged_aggression;

  /* Electric def stat */
  uint charged_fortitude;
  uint max_charged_fortitude;

  /* Digital atk stat */
  uint cybernetic_aggression;
  uint max_cybernetic_aggression;

  /* Digital def stat */
  uint cybernetic_fortitude;
  uint max_cybernetic_fortitude;

  /* Void atk stat */
  uint nihil_aggression;
  uint max_nihil_aggression;

  /* Void def stat */
  uint nihil_fortitude;
  uint max_nihil_fortitude;

  /* Health stat */
  uint vitality;
  uint max_vitality;

  /* Quantum drive (MP), shown as percentage */
  uint quantum_drive;
  uint max_quantum_drive;

  /* Speed stat */
  uint agility;
  uint max_agility;

  /* Dodge stat */
  uint limbertude;
  uint max_limbertude;

  /* Critical chance stat */
  uint unbearability;
  uint max_unbearability;

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
  void setPhysicalAggression(uint value, uint max_value = 0);

  /* Sets the physical defense stat */
  void setPhysicalFortitude(uint value, uint max_value = 0);

  /* Sets the fire attack stat */
  void setThermalAggression(uint value, uint max_value = 0);

  /* Sets the fire defense stat */
  void setThermalFortitude(uint value, uint max_value = 0);

  /* Sets the ice attack stat */
  void setPolarAggression(uint value, uint max_value = 0);

  /* Sets the ice defense stat */
  void setPolarFortitude(uint value, uint max_value = 0);

  /* Sets the forest attack stat */
  void setPrimalAggression(uint value, uint max_value =0);

  /* Sets the forest defense stat */
  void setPrimalFortitude(uint value, uint max_value = 0);

  /* Sets the electric attack stat */
  void setChargedAggression(uint value, uint max_value = 0);

  /* Sets the electric defense stat */
  void setChargedFortitude(uint value, uint max_value = 0);

  /* Sets the digital attack stat */
  void setCyberneticAggression(uint value, uint max_value = 0);

  /* Sets the digital defense stat */
  void setCyberneticFortitude(uint value, uint max_value = 0);

  /* Sets the void attack stat */
  void setNihilAggression(uint value,uint max_value = 0);

  /* Sets the void defense stat */
  void setNihilFortitude(uint value, uint max_value = 0);

  /* Sets the health stat */
  void setVitality(uint value, uint max_value = 0);

  /* Sets the power points (shown as percentage) */
  void setQuantumDrive(uint value, uint max_value = 0);

  /* Sets the speed stat */
  void setAgility(uint value, uint max_value = 0);

  /* Sets the dodge stat */
  void setLimbertude(uint value, uint max_value = 0);

  /* Sets the critical chance stat */
  void setUnbearability(uint value, uint max_value = 0);

  /* Gets the name of the class */
  QString getName();

  /* Gets the description of the class */
  QString getDescription();

  /* Sets the physical attack stat */
  uint getPhysicalAggression();
  uint getMaxPhysicalAggression();

  /* Sets the physical defense stat */
  uint getPhysicalFortitude();
  uint getMaxPhysicalFortitude();

  /* Gets the fire attack stat */
  uint getThermalAggression();
  uint getMaxThermalAggression();

  /* Gets the fire defense stat */
  uint getThermalFortitude();
  uint getMaxThermalFortitude();

  /* Gets the ice attack stat */
  uint getPolarAggression();
  uint getMaxPolarAggression();

  /* Gets the ice defense stat */
  uint getPolarFortitude();
  uint getMaxPolarFortitude();

  /* Gets the forest attack stat */
  uint getPrimalAggression();
  uint getMaxPrimalAggression();

  /* Gets the forest defense stat */
  uint getPrimalFortitude();
  uint getMaxPrimalFortitude();

  /* Gets the electric attack stat */
  uint getChargedAggression();
  uint getMaxChargedAggression();

  /* Gets the electric defense stat */
  uint getChargedFortitude();
  uint getMaxChargedFortitude();

  /* Gets the digital attack stat */
  uint getCyberneticAggression();
  uint getMaxCyberneticAggression();

  /* Gets the digital defense stat */
  uint getCyberneticFortitude();
  uint getMaxCyberneticFortitude();

  /* Gets the void attack stat */
  uint getNihilAggression();
  uint getMaxNihilAggression();

  /* Gets the void defense stat */
  uint getNihilFortitude();
  uint getMaxNihilFortitude();

  /* Gets the health stat */
  uint getVitality();
  uint getMaxVitality();

  /* Gets the power points (shown as a percentage) */
  uint getQuantumDrive();
  uint getMaxQuantumDrive();

  /* Gets the speed stat */
  uint getAgility();
  uint getMaxAgility();

  /* Gets the dodge stat */
  uint getLimbertude();
  uint getMaxLimbertude();

   /* Gets the critical chance stat */
  uint getUnbearability();
  uint getMaxUnbearability();
};

#endif // CATEGORY_H
