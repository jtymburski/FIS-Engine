/******************************************************************************
* Category Name: Category
* Date Created: December 2nd, 2012
* Inheritance: Parent Category - Person
* Description: Header for Category that defines the Category for a person.
*              This is a battle class like Warrior, etc.
******************************************************************************/

#include "Game/Player/Category.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor object. Constructs a Category given a string and
 *              a full set of base stat values. None of the Category const.
 *              set the description of the Category, or the actions available.
 *              These must be set up afterwards. The maximum values have
 *              default values that will default to the maximum allowable.
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
 *      mom  - base momentum
 *      lim  - base limbertude
 *      unb  - base unbearability
 *      x_max - the maximum value for all the stats listed above
 */
Category::Category(QString n, uint ph_a, uint ph_f, uint th_a, uint th_f, uint po_a,
                   uint po_f, uint pr_a, uint pr_f, uint ch_a, uint ch_f,
                   uint cy_a, uint cy_f, uint ni_a, uint ni_f, uint vit,
                   uint qd, uint mom, uint lim, uint unb, uint ph_a_max,
                   uint ph_f_max, uint th_a_max, uint th_f_max,
                   uint po_a_max, uint po_f_max, uint pr_a_max,
                   uint pr_f_max, uint ch_a_max, uint ch_f_max,
                   uint cy_a_max, uint cy_f_max, uint ni_a_max,
                   uint ni_f_max, uint vit_max, uint qd_max,
                   uint mom_max, uint lim_max, uint unb_max, QWidget *parent)
{
    setName(n);

    setUp(ph_a,ph_f,th_a,th_f,po_a,po_f,pr_a,pr_f,ch_a,ch_f,cy_a,cy_f,
          ni_a,ni_f,vit,qd,mom,lim,unb,ph_a_max,ph_f_max,th_a_max,
          th_f_max,po_a_max,po_f_max,pr_a_max,pr_f_max,ch_a_max,
          ch_f_max,cy_a_max,cy_f_max,ni_a_max,ni_f_max,vit_max,qd_max,
          mom_max,lim_max,unb_max);
}

/*
 * Description: Default Category object constructor
 *
 * Inputs: n - name of the Category
 */
Category::Category(QString n, QWidget *parent)
{
    setName(n);
    setUp(10,7,8,5,8,5,8,5,8,5,8,5,8,5,200,100,10,12,3);
}

/*
 * Description: Destroys a Category object
 */
Category::~Category()
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
void Category::setUp(uint ph_a, uint ph_f, uint th_a, uint th_f, uint po_a,
                     uint po_f, uint pr_a, uint pr_f, uint ch_a, uint ch_f,
                     uint cy_a, uint cy_f, uint ni_a, uint ni_f, uint vit,
                     uint qd, uint mom, uint lim, uint unb, uint ph_a_max,
                     uint ph_f_max, uint th_a_max, uint th_f_max,
                     uint po_a_max, uint po_f_max, uint pr_a_max,
                     uint pr_f_max, uint ch_a_max, uint ch_f_max,
                     uint cy_a_max, uint cy_f_max, uint ni_a_max,
                     uint ni_f_max, uint vit_max, uint qd_max,
                     uint mom_max, uint lim_max, uint unb_max)
{
    setPhysicalAggression(ph_a, ph_a_max);
    setPhysicalFortitude(ph_f, ph_f_max);
    setThermalAggression(th_a, th_a_max);
    setThermalFortitude(th_f, th_a_max);
    setPolarAggression(po_a, po_a_max);
    setPolarFortitude(po_f, po_f_max);
    setPrimalAggression(pr_a, pr_a_max);
    setPrimalFortitude(pr_f, pr_f_max);
    setChargedAggression(ch_a, ch_a_max);
    setChargedFortitude(ch_f, ch_f_max);
    setCyberneticAggression(cy_a, cy_a_max);
    setCyberneticFortitude(cy_f, cy_f_max);
    setNihilAggression(ni_a, ni_a_max);
    setNihilFortitude(ni_f, ni_f_max);
    setVitality(vit, vit_max);
    setQuantumDrive(qd, qd_max);
    setMomentum(mom, mom_max);
    setLimbertude(lim, lim_max);
    setUnbearability(unb, unb_max);
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
 * Inputs: QString - string of the Category description
 * Output: none
 */
void Category::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the physical aggression bonus stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setPhysicalAggression(uint value, uint max_value)
{
  if (max_value < kPHYS_ATK_MAX && max_value != 0)
    max_physical_aggression = max_value;
  if (max_value == 0)
    max_physical_aggression = kPHYS_ATK_MAX;
  if (value < max_physical_aggression)
    physical_aggression = value;
}

/*
 * Description: Sets the physical fortitude bonus stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setPhysicalFortitude(uint value, uint max_value)
{
  if (max_value < kPHYS_DEF_MAX && max_value != 0)
    max_physical_fortitude = max_value;
  if (max_value == 0)
    max_physical_fortitude = kPHYS_DEF_MAX;
  if (value < max_physical_aggression)
    physical_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the thermal attack stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setThermalAggression(uint value, uint max_value)
{
  if (max_value < kTHER_ATK_MAX && max_value != 0)
    max_thermal_aggression = max_value;
  if (max_value == 0)
    max_thermal_aggression = kTHER_ATK_MAX;
  if (value < max_thermal_aggression)
    thermal_aggression = value;
}

/*
 * Description: Sets the Category bonus value of the thermal attack stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setThermalFortitude(uint value, uint max_value)
{
  if (max_value < kTHER_DEF_MAX && max_value != 0)
    max_thermal_fortitude = max_value;
  if (max_value == 0)
    max_thermal_fortitude = kTHER_DEF_MAX;
  if (value < max_thermal_fortitude)
    thermal_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the polar attack stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setPolarAggression(uint value, uint max_value)
{
  if (max_value < kPOLA_ATK_MAX && max_value != 0)
    max_polar_aggression = max_value;
  if (max_value == 0)
    max_polar_aggression = kPOLA_ATK_MAX;
  if (value < max_polar_aggression)
    polar_aggression = value;

}

/*
 * Description: Sets the Category bonus value of the polar defense stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setPolarFortitude(uint value, uint max_value)
{
  if (max_value < kPOLA_DEF_MAX && max_value != 0)
    max_polar_fortitude = max_value;
  if (max_value == 0)
    max_polar_fortitude = kPOLA_DEF_MAX;
  if (value < max_polar_aggression)
    polar_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the primal attack stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setPrimalAggression(uint value, uint max_value)
{
  if (max_value < kPRIM_ATK_MAX && max_value != 0)
    max_primal_aggression = max_value;
  if (max_value == 0)
    max_primal_aggression = kPRIM_ATK_MAX;
  if (value < max_primal_aggression)
    primal_aggression = value;
}

/*
 * Description: Sets the Category bonus value of the primal defense stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setPrimalFortitude(uint value, uint max_value)
{
  if (max_value < kPRIM_DEF_MAX && max_value != 0)
    max_primal_fortitude = max_value;
  if (max_value == 0)
    max_primal_fortitude = kPRIM_DEF_MAX;
  if (value < max_primal_fortitude)
    primal_fortitude = value;
}

/*
 * Description: Sets the Category bonus value of the charged attack stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setChargedAggression(uint value, uint max_value)
{
  if (max_value < kCHAR_ATK_MAX && max_value != 0)
    max_charged_aggression = max_value;
  if (max_value == 0)
    max_charged_aggression = kCHAR_ATK_MAX;
  if (value < max_charged_aggression)
    charged_aggression = value;
}

/*
 * Description: Sets the Category bonus value of the charged defense stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setChargedFortitude(uint value, uint max_value)
{
  if (max_value < kCHAR_DEF_MAX && max_value != 0)
    max_charged_fortitude = max_value;
  if (max_value == 0)
    max_charged_fortitude = kCHAR_DEF_MAX;
  if (value < max_charged_fortitude)
    charged_aggression = value;
}

/*
 * Description: Sets the Category bonus of the cybernetic attack stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setCyberneticAggression(uint value, uint max_value)
{
  if (max_value < kCYBE_ATK_MAX && max_value != 0)
    max_cybernetic_aggression = max_value;
  if (max_value == 0)
    max_cybernetic_aggression = kCYBE_ATK_MAX;
  if (value < max_cybernetic_aggression)
    cybernetic_aggression = value;
}

/*
 * Description: Sets the Category bonus of the cybernetic defense stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat * Output: none
 */
void Category::setCyberneticFortitude(uint value, uint max_value)
{
  if (max_value < kCYBE_DEF_MAX && max_value != 0)
    max_cybernetic_fortitude = max_value;
  if (max_value == 0)
    max_cybernetic_fortitude = kCYBE_DEF_MAX;
  if ( value < max_cybernetic_fortitude)
    cybernetic_fortitude = value;
}

/*
 * Description: Sets the Category bonus of the nihil attack stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setNihilAggression(uint value, uint max_value)
{
  if (max_value < kNIHI_ATK_MAX && max_value != 0)
    max_nihil_aggression = max_value;
  if (max_value == 0)
    max_nihil_aggression = kNIHI_ATK_MAX;
  if (value < max_nihil_aggression)
    nihil_aggression = value;
}

/*
 * Description: Sets the Category bonus of the nihil fortitude stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setNihilFortitude(uint value, uint max_value)
{
  if (max_value < kNIHI_DEF_MAX && max_value != 0)
    max_nihil_fortitude = max_value;
  if (max_value == 0)
    max_nihil_fortitude = kNIHI_DEF_MAX;
  if (value < max_nihil_fortitude)
    nihil_fortitude = value;
}

/*
 * Description: Sets the Category bonus of the vitality stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setVitality(uint value, uint max_value)
{
  if (max_value < kVITA_MAX && max_value != 0)
    max_vitality = max_value;
  if (max_value == 0)
    max_vitality = kVITA_MAX;
  if (value < max_vitality)
    vitality = value;
}

/*
 * Description: Sets the Category min and max quantum drive stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setQuantumDrive(uint value, uint max_value)
{
  if (max_value < kQTMN_MAX && max_value != 0)
    max_quantum_drive = max_value;
  if (max_value == 0)
    max_quantum_drive = kQTMN_MAX;
  if (value < max_quantum_drive)
    quantum_drive = value;
}

/*
 * Description: Sets the Category min and max momentum stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setMomentum(uint value, uint max_value)
{
  if (max_value < kMOME_MAX && max_value != 0)
    max_momentum = max_value;
  if (max_value == 0)
    max_momentum = kMOME_MAX;
  if (value < max_momentum)
    momentum = value;
}

/*
 * Description: Sets the Category bonus of the limbertude stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setLimbertude(uint value, uint max_value)
{
  if (max_value < kLIMB_MAX && max_value != 0)
    max_limbertude = max_value;
  if (max_value == 0)
    max_limbertude = kLIMB_MAX;
  if (value < max_limbertude)
    limbertude = value;
}

/*
 * Description: Sets the Category bonus of the unbearability stat
 *
 * Inputs: uint - starting value of the stat
 *         uint - max value of the stat
 * Output: none
 */
void Category::setUnbearability(uint value, uint max_value)
{
  if (max_value < kUNBR_MAX && max_value != 0)
    max_unbearability = max_value;
  if (max_value == 0)
    max_unbearability = kUNBR_MAX;
  if (value < max_unbearability)
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
 * Description: Returns the minimum physical atk stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getPhysicalAggression()
{
  return physical_aggression;
}

/*
 * Description: Returns the maximum physical atk stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxPhysicalAggression()
{
  return max_physical_aggression;
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
 * Description: Returns the maximum physical def stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxPhysicalFortitude()
{
  return max_physical_fortitude;
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
 * Description: Returns the maximum thermal atk stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxThermalAggression()
{
  return max_thermal_aggression;
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
 * Description: Returns the maximum thermal def stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxThermalFortitude()
{
  return max_thermal_fortitude;
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
 * Description: Returns the maximum polar atk stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxPolarAggression()
{
  return max_polar_aggression;
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
 * Description: Returns the maximum polar def stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxPolarFortitude()
{
  return max_polar_fortitude;
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
 * Description: Returns the maximum primal atk stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxPrimalAggression()
{
  return max_primal_aggression;
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
 * Description: Returns the maximum primal def stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxPrimalFortitude()
{
  return max_primal_fortitude;
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
 * Description: Returns the maximum charged atk stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxChargedAggression()
{
  return max_charged_aggression;
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
 * Description: Returns the maximum charged def stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxChargedFortitude()
{
  return max_charged_fortitude;
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
 * Description: Returns the maximum cybernetic atk stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxCyberneticAggression()
{
  return max_cybernetic_aggression;
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
 * Description: Returns the maximum cybernetic def stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxCyberneticFortitude()
{
  return max_cybernetic_fortitude;
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
 * Description: Returns the maximum nihil atk stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxNihilAggression()
{
  return max_nihil_aggression;
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
 * Description: Returns the maximum nihil def stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxNihilFortitude()
{
  return max_nihil_fortitude;
}

/*
 * Description: Returns the Category bonus of the vitality stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getVitality()
{
    qDebug() << vitality;
  return vitality;
}

/*
 * Description: Returns the maximum vitality stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxVitality()
{
  return max_vitality;
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
 * Description: Returns the maximum quantum drive stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxQuantumDrive()
{
  return max_quantum_drive;
}

/*
 * Description: Returns the Category bonus of the momentum stat
 *
 * Inputs: none
 * Output: int - value of the Category bonus
 */
uint Category::getMomentum()
{
  return momentum;
}

/*
 * Description: Returns the maximum momentum stat
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxMomentum()
{
  return max_momentum;
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
 * Description: Returns the maximum limbertude
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxLimbertude()
{
  return max_limbertude;
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

/*
 * Description: Returns the maximum unbearability
 *
 * Inputs: none
 * Output: int - value of the category's max of the stat
 */
uint Category::getMaxUnbearability()
{
  return max_unbearability;
}
