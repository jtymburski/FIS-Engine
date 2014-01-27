/*******************************************************************************
* Class Name: Record (Formerly PersonRecord) [Declaration]
* Date Created: January 23rd, 2014
* Inheritance: None
* Description: A class which represents an object holding the records of a
*              person. This class will be used to keep track of the progress
*              a person (character) has
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* //TODO: Build preset values [10-06-13]
* //TODO: Signal/function for Rank-Up [10-06-13]
* //TODO: Grand Score calculations [10-06-13]
*******************************************************************************/

#ifndef RECORD_H
#define RECORD_H

#include "Helpers.h"
#include "EnumDb.h"

class Record
{
public:
  /* Constructs a PersonRecord at a given rank value */
  Record(const Ranks& preset_rank = Ranks::NUBEAR);

private:
  /* Current rank of the Person of this PersonRank */
  Ranks current_rank;

  /* Battles Won records */
  uint32_t battles_won;
  uint32_t battles_won_naked;
  uint32_t battles_won_empty_handed;
  uint32_t battles_won_mini_boss;
  uint32_t battles_won_boss;
  uint32_t battles_won_final_boss;

  /* Kills records */
  uint32_t kills_bear;
  uint32_t kills_human;
  uint32_t kills_fiend;

  /* Miscellaneous */
  uint32_t grand_score;
  uint32_t times_kod;

  /* ------------ Constants --------------- */

  /* Points required to reach the various Ranks */
  static std::vector<int32_t> rank_points_required;

  /* Defaults */
  static const uint32_t kDEFAULT_GRAND_SCORE;
  static const uint32_t kRANKS;

  /* Maximums */
  static const uint32_t kMAX_BATTLES_WON;
  static const uint32_t kMAX_KILLS;
  static const uint32_t kMAX_GRAND_SCORE;
  static const uint32_t kMAX_TIMES_KOD;
  static const uint32_t kMAX_RANK_SCORE;

  /* Point Values */
  static const uint32_t kPOINTS_BATTLE_WON;
  static const uint32_t kPOINTS_BATTLE_WON_NAKED;
  static const uint32_t kPOINTS_BATTLE_WON_EMPTY_HANDED;
  static const uint32_t kPOINTS_BATTLE_WON_MINI_BOSS;
  static const uint32_t kPOINTS_BATTLE_WON_BOSS;
  static const uint32_t kPOINTS_BATTLE_WON_FINAL_BOSS;

  static const uint32_t kPOINTS_KILLS_BEAR;
  static const uint32_t kPOINTS_KILLS_HUMAN;
  static const uint32_t kPOINTS_KILLS_FIEND;
  
/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Initially sets up the points required to reach various ranks */
  static void setUpRankPoints();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Calculates the grandscore of the PersonRecord */
  void calcGrandScore();

  /* Methods for printing information about the PersonRecord object */
  void print(const bool &p_const = false);

  /* Updates neccessary information relating to the grand_score, etc. */
  void update();

  /* Loads a default PersonRecord object */
  void loadDefault();

  /* Loads the PersonRecord object with one of a particular preset */
  void loadPreset(const Ranks &rank_preset);

  /* Assigns a new rank to the Person Record */
  void setRank(const Ranks &new_rank);

  /* Assigns a new value to battles_won to the Person Record */
  void setBattlesWon(const uint32_t &new_value);

  /* Assigns a new value to battles_won_naked to the Person Record */
  void setBattlesWonNaked(const uint32_t &new_value);

  /* Assigns a new value to battles_won_empty_handed to the Person Record */
  void setBattlesWonEmptyHanded(const uint32_t &new_value);

  /* Assigns a new value to battles_won_mini_boss to the Person Record */
  void setBattlesWonMiniBoss(const uint32_t &new_value);

  /* Assigns a new value to battles_won_boss to the Person Record */
  void setBattlesWonBoss(const uint32_t &new_value);

  /* Assigns a new value to battles_won_final_boss to the Person Record */
  void setBattlesWonFinalBoss(const uint32_t &new_value);

  /* Assigns a new value to kills_bear to the Person Record */
  void setKillsBear(const uint32_t &new_value);

  /* Assigns a new value to kills_human to the Person Record */
  void setKillsHuman(const uint32_t &new_value);

  /* Assigns a new value to kills_fiend to the Person Record */
  void setKillsFiend(const uint32_t &new_value);

  /* Assigns a new value to grand_score to the Person Record */
  void setGrandScore(const uint32_t &new_value);

  /* Assigns a new value to times_kod to the Person Record */
  void setTimesKOd(const uint32_t &new_value);

  /* Returns the rank of the Person */
  Ranks getRank();

  /* Returns the value of battles_won */
  uint32_t getBattlesWon();

  /* Returns the value of battles_won_naked */
  uint32_t getBattlesWonNaked();

  /* Returns the value of battles_won_empty_handed */
  uint32_t getBattlesWonEmptyHanded();

  /* Returns the value of battles_won_mini_boss */
  uint32_t getBattlesWonMiniBoss();

  /* Returns the value of battles_won_boss */
  uint32_t getBattlesWonBoss();

  /* Returns the value of battles_won_final_boss */
  uint32_t getBattlesWonFinalBoss();

  /* Returns the value of kills_bear */
  uint32_t getKillsBear();

  /* Returns the value of kills_human */
  uint32_t getKillsHuman();

  /* Returns the value of kills_fiend */
  uint32_t getKillsFiend();

  /* Returns the value of grand_score */
  uint32_t getGrandScore();

  /* Returns the value of times_kod */
  uint32_t getTimesKOd();

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the rank of a given index */
  static Ranks rankAtIndex(const uint32_t &index);

  /* Returns the highest rank achieveable at a given score */
  static Ranks rankAtScore(const uint32_t &score);

  /* Returns the minimum score required to achieve a given rank */
  static uint32_t scoreAtRank(const Ranks &rank);

};

#endif //RECORD_H
