/*******************************************************************************
* Class Name: Bubby
* Date Created: November 4th, 2012 (Rewritten March 6th, 2013)
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, BubbyFlavour, etc.
*******************************************************************************/
#ifndef BUBBY_H
#define BUBBY_H

#include <QtGui/QWidget>

#include "MathHelper.h"
#include "Game/Player/BubbyFlavour.h"
#include "Game/Player/SkillSet.h"
#include "Game/Player/Item.h"

class Bubby : public Item
{
  Q_OBJECT

public:
  /* Constructor function */
  Bubby(BubbyFlavour* type);

  /* Destructor function */
  ~Bubby();

private:
  /* Bubby Type */
  BubbyFlavour* type;

  /* The experience table for Bubbies */
  static QVector<int> exp_table;

  /* The Bubby's current level */
  ushort level;

  /* The Bubby's current tier */
  ushort tier;

  /* Bubby's totaly experience gained */
  uint total_exp;

  /* ------------ Constants --------------- */
  static const ushort kTIER1_LVL;
  static const ushort kTIER2_LVL;
  static const ushort kLEVEL_CAP;
  static const uint kMIN_LVL_EXP;
  static const uint kMAX_LVL_EXP;
  static const uint kMAX_EXP;
  static const double kBASE_VALUE;
  static const double kVALUE_PER_LEVEL;
  static const double kVALUE_PER_TIER;
  static const double kVALUE_MULTIPLIER;
  static const double kTIER_0_MASS;
  static const double kTIER_1_MASS;
  static const double kTIER_2_MASS;
  static const double kTIER_3_MASS;

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Updates the Bubby's value on Level-Up */
  void calcNewValue();

  /* Updates the Bubby's mass on Tier-Up */
  void calcNewMass();

  /* Updates the Bubby to the appropriate sprite (on tier level up) */
  bool updateTierSprite();

/*============================================================================
 * SIGNALS
 *============================================================================*/
signals:
 /* Emitted on Level-Up */
 void levelUp();

 /* Emitted on Tier-Up */
 void tierUp();

 /* Emitted on kMAX_EXP */
 void maxExp();

 /* Emitted when the Bubby reaches the maximum level for its current tier */
 void maxLevelForTier();

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds experience to the Bubby */
  void addExperience(uint amount);

  /* Prints the info describing the Bubby */
  void printInfo();

  /* Gets the bubbies exp */
  uint getExp();

  /* Gets the exp required at a given Bubby level */
  uint getExpAt(ushort level);

  /* Returns the bubbies level */
  ushort getLevel();

  /* Gets the bubbies tier */
  ushort getTier();

  /* Gets the type of the Bubby */
  BubbyFlavour* getType();

  /* Sets the exp of the bubby based on use in battle */
  void setExperience(uint new_experience);

  /* Sets the level of the bubby based on exp amounts */
  void setLevel(ushort new_level);

  /* Sets the tier of the bubby based on leveling */
  bool setTier(ushort new_tier);
};

#endif // BUBBY_H
