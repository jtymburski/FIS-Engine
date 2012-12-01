/********************************************************************************
* Class Name: Bubby
* Date Created: Nov 04 2012
* Inheritance: InventoryUnit
* Description: The Bubby class
* Notes : This uses the following formula: Exp(Level) = 50 + Exp(Level – 1)
*         x [1 + Multiplier / 100] Multiplier: 10-25
********************************************************************************/
#ifndef BUBBY_H
#define BUBBY_H
#include <QtGui/QWidget>
#include <QImage>
#include <QVector>
#include "InventoryUnit.h"
#include "Action.h"


class Bubby : public InventoryUnit
{
public:
  /*Constructor function*/
  Bubby();

  /*Destructor function*/
  ~Bubby();

private:
  /*A parallel list that shows when actions become available (Based on level)*/
  QVector<int> action_available;

  /*The list of actions offered by the bubby*/
  QVector<Action*> action_list;

  /*The bubbies exp*/
  int experience;

  /*The maximum level a bubby can reach, Tier-1: Level 1-10, Tier-2: Level 11-20,
    Tier-3: Level 20*/
  const int kLEVELCAP; //20

  /*The bubbies level*/
  int level;

  /*The bubbies tier classification, the tier level can vary between 0 to 3*/
  int tier;

  /*The sprite image for each tier*/
  QImage tier_0, tier_1, tier_2, tier_3;

public:
  /*Gets the list of bubby actions (Used for total action lists in battle)*/
  QVector<Action*> getActionList();

  /*Gets the bubbies exp*/
  int getExp();

  /*Gets the bubbies level*/
  int getLevel();

  /*Gets the bubbies tier*/
  int getTier();

  /*Sets the exp of the bubby based on use in battle*/
  void setExperience(int exp);

  /*Sets the level of the bubby based on exp amounts*/
  void setLevel(int lvl);

  /*Sets the tier of the bubby based on leveling*/
  void setTier(int tier_num);
};


#endif // BUBBY_H
