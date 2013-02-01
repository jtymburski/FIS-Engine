/******************************************************************************
* Class Name: Item
* Date Created: Oct 28 2012
* Inheritance: InventoryUnit
* Description: The Item Header that defines all elements within Inventory.
*              This includes Equipment and Bubby.
* TODO: Gets / Sets for stats [01-26-13]
******************************************************************************/
#ifndef ITEM_H
#define ITEM_H

#include "EnumDatabase.h"
#include <QImage>
#include <QString>

class Item
{
public:
  /* Constructor function */
  Item(QWidget *parent = 0);

  /* Destructor function */
  ~Item();

private:
  /* Category unit belongs to */
  QString item_category;

  /* Number of these that exist in inventory */
  int count;

  /* Description of unit */
  QString description;

  /* Name of unit*/
  QString name;

  /* Thumbnail of unit*/
  QImage thumbnail;

  /* Number of turns the item will last in battle */
  int turn_count;

  /* Thermal aggression alteration values */
  int thermal_aggression;
  int temp_thermal_aggression;

  /* Thermal fortitude alteration values */
  int thermal_fortitude;
  int temp_thermal_fortitude;

  /* Polar aggression alteration values */
  int polar_aggression;
  int temp_polar_aggression;

  /* Polar fortitude alteration values */
  int polar_fortitude;
  int temp_polar_fortitude;

  /* Primal aggression alteration values */
  int primal_aggression;
  int temp_primal_aggression;

  /* Primal fortitude alteration values */
  int primal_fortitude;
  int temp_primal_fortitde;

  /* Charged aggression alteration values */
  int charged_aggression;
  int temp_charged_aggression;

  /* Charged fortitude alteration values */
  int charged_fortitude;
  int temp_charged_fortitude;

  /* Cybernetic aggression alteration values */
  int cybernetic_aggression;
  int temp_cybernetic_aggression;

  /* Cybernetic fortitude alteration values */
  int cybernetic_fortitude;
  int temp_cybernetic_fortitude;

  /* Nihil aggression alteration values */
  int nihil_aggression;
  int temp_nihil_aggression;

  /* Nihil fortitude alteration values */
  int nihil_fortitude;
  int temp_nihil_fortitude;

  /* Vitality alteration values */
  int vitality;
  int temp_vitality;

  /* Quantum Drive alteration values */
  int quantum_drive;
  int temp_quantum_drive;

  /* Agility alteration values */
  int agility;
  int temp_agility;

  /* Limbertude alteration values */
  int limbertude;
  int temp_limbertude;

  /* Unbearability alteration values */
  int unbearability;
  int temp_unbearability;

public:
  /* Evaulates BATTLEREADY flag */
  bool isBattleReady();

  /* Evaluates STACKABLE flag */
  bool isStackable();

  /* Evaluates INDEFINITE flag */
  bool isIndefinite();

  /* Gets category of unit */
  QString getCategory();

  /* Gets unit count */
  int getCount();

  /* Gets description of unit */
  QString getDescription();

  /* Gets name of unit */
  QString getName();

  /* Gets thumbnail of unit */
  QImage getThumbnail();

  /* Gets the turn count */
  int getTurnCount();

  /* Sets unit count */
  void setCount(int i);
};

#endif // ITEM_H

