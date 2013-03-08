/*******************************************************************************
* Class Name: Bubby
* Date Created: November 4th, 2012 (Rewritten March 6th, 2013)
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, etc.
* Notes : This uses the following formula: Exp(Level) = 50 + Exp(Level – 1)
*         x [1 + Multiplier / 100] Multiplier: 10-25
* TODO: Method of "storing" and creating different types of Bubby [02-07-13]
*******************************************************************************/
#ifndef BUBBY_H
#define BUBBY_H

#include <QtGui/QWidget>
#include "Game/Player/BubbyFlavour.h"
#include "Game/Player/SkillSet.h"
#include "Game/Player/Item.h"

class Bubby : public Item
{
public:
  /* Constructor function */
  Bubby(BubbyFlavour* type);

  /* Destructor function */
  ~Bubby();

private:
  /* Maximum level a bubby can reach, (T1: 0-9; T2: 10-19: T3: 20) */
  const static ushort kTIER_CAP  =  3;
  const static ushort kTIER1_LVL =  9;
  const static ushort kTIER2_LVL = 19;
  const static ushort kLEVEL_CAP = 20;

  /* Bubby Type */
  BubbyFlavour* type;

  /* Static ID data, and current object's ID */
  static int id;
  int myId;

  /* Bubby's Experience, Level, and Tier */
  uint experience;
  ushort level;
  ushort tier;

public:
  /* Adds experience to the Bubby */
  void addExperience(uint amount);

  /* Gets the bubbies Id */
  int getId();

  /* Gets the bubbies exp */
  uint getExp();

  /* Gets the bubbies level */
  ushort getLevel();

  /* Gets the bubbies tier */
  ushort getTier();

  /* Gets the type of the Bubby */
  BubbyFlavour* getType();

  /* Increments the bubbie's Id */
  static int setId();

  /* Sets the exp of the bubby based on use in battle */
  void setExperience(uint new_experience);

  /* Sets the level of the bubby based on exp amounts */
  void setLevel(ushort new_level);

  /* Sets the tier of the bubby based on leveling */
  void setTier(ushort new_tier);

  /* Sets the type of the Bubby */
  void setType(BubbyFlavour* new_type);
};

#endif // BUBBY_H
