/******************************************************************************
* Class Name: Bubby
* Date Created: Nov 04 2012
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, etc.
* Notes : This uses the following formula: Exp(Level) = 50 + Exp(Level – 1)
*         x [1 + Multiplier / 100] Multiplier: 10-25
* TODO: Method of "storing" and creating different types of Bubby [02-07-13]
******************************************************************************/
#ifndef BUBBY_H
#define BUBBY_H

#include <QImage>
#include <QtGui/QWidget>
#include <QVector>

#include "Game/Player/Action.h"
#include "Game/Player/Item.h"

class Bubby : public Item
{
public:
  /* Constructor function */
  Bubby();

  /* Destructor function */
  ~Bubby();

private:
  /* Sets the tier of the bubby based on leveling */
  void setTier(uint new_tier);

  /* A parallel list that shows when actions become available */
  QVector<uint> action_available;

  /* The list of actions offered by the bubby */
  QVector<Action*> action_list;

  /* The maximum level a bubby can reach,
     Tier-1: Level 1-10, Tier-2: Level 11-20, Tier-3: Level 20 */
  const static uint kLEVELCAP = 20; // 20

  /* Bubby's Id */
  static int id;
  int myId;

  /* ID of the Bubby (for deleting purposes) */
  // static uint id;

  /* The bubby's experience */
  uint experience;

  /* The bubby's level */
  uint level;

  /* The bubbies tier classification, the tier level can vary between 0 to 3 */
  uint tier;

  /* The sprite image for each tier */
  QImage tier_0, tier_1, tier_2, tier_3;

public:
  /* Gets the list of bubby actions (Used for total action lists in battle)*/
  QVector<Action*> getActionList();

  /* Increments the bubbie's Id */
  static int setId();

  /* Gets the bubbies Id */
  int getId();

  /* Gets the bubbies exp */
  uint getExp();

  /* Gets the bubbies level */
  uint getLevel();

  /* Gets the bubbies tier */
  uint getTier();

  /* Sets the exp of the bubby based on use in battle */
  void setExperience(uint new_experience);

  /* Sets the level of the bubby based on exp amounts */
  void setLevel(uint new_level);
};

#endif // BUBBY_H
