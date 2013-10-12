/******************************************************************************
* Class Name: Person Record [Header Declaration]
* Date Created: October 6th, 2013
* Inheritance: None
* Description: A class which represents an object holding the records of a
*              person. This class will be used to keep track of the progress
*              a person (character) has
*
* //TODO: Build preset values [10-06-13]
* //TODO: Signal/function for Rank-Up [10-06-13]
* //TODO: Grand Score calculations [10-06-13]
******************************************************************************/
#ifndef PERSONRECORD_H
#define PERSONRECORD_H

#include "MathHelper.h"
#include "EnumDb.h"

class PersonRecord
{
public:
  /* Creates a default PersonRecord object */
  PersonRecord();

  /* Copy Constructor */
  PersonRecord(const PersonRecord &source);

  /* Creates a PersonRecord object based on a couple of presets */
  PersonRecord(EnumDb::PersonRanks rank_preset = EnumDb::NUBEAR);

  /* Annihilates a PersonRecord object */
  ~PersonRecord();

private:

  /* Current rank of the Person of this PersonRank */
  EnumDb::PersonRanks current_rank;

  /* Battles Won records */
  uint battles_won;
  uint battles_won_naked;
  uint battles_won_empty_handed;
  uint battles_won_mini_boss;
  uint battles_won_boss;
  uint battles_won_final_boss;

  /* Kills records */
  uint kills_bear;
  uint kills_human;
  uint kills_fiend;

  /* Miscellaneous */
  uint grand_score;
  uint times_kod;

  /*------------------- Constants -----------------------*/

  /* Points required to reach the various Ranks */
  static QVector<int> rank_points_required;

  /* Defaults */
  static const uint kDEFAULT_GRAND_SCORE;
  static const uint kRANKS;

  /* Maximums */
  static const uint kMAX_BATTLES_WON;
  static const uint kMAX_KILLS;
  static const uint kMAX_GRAND_SCORE;
  static const uint kMAX_TIMES_KOD;
  static const uint kMAX_RANK_SCORE;

  /* Point Values */
  static const uint kPOINTS_BATTLE_WON;
  static const uint kPOINTS_BATTLE_WON_NAKED;
  static const uint kPOINTS_BATTLE_WON_EMPTY_HANDED;
  static const uint kPOINTS_BATTLE_WON_MINI_BOSS;
  static const uint kPOINTS_BATTLE_WON_BOSS;
  static const uint kPOINTS_BATTLE_WON_FINAL_BOSS;

  static const uint kPOINTS_KILLS_BEAR;
  static const uint kPOINTS_KILLS_HUMAN;
  static const uint kPOINTS_KILLS_FIEND;

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Deep copy for PersonRecord */
  void copySelf(const PersonRecord &source);

  /* Initially sets up the points required to reach various ranks */
  static void setUpRankPoints();

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the rank of a given index */
  static EnumDb::PersonRanks rankAtIndex(uint index);

  /* Returns the highest rank achieveable at a given score */
  static EnumDb::PersonRanks rankAtScore(uint score);

  /* Returns the minimum score required to achieve a given rank */
  static int scoreAtRank(EnumDb::PersonRanks rank);

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Calculates the grandscore of the PersonRecord */
  void calcGrandScore();

  /* Methods for printing information about the PersonRecord object */
  void printAll();
  void printConstants();
  void printRecord();

  /* Updates neccessary information relating to the grand_score, etc. */
  void update();

  /* Loads a default PersonRecord object */
  void loadDefault();

  /* Loads the PersonRecord object with one of a particular preset */
  void loadPreset(EnumDb::PersonRanks rank_preset);

  /* Assigns a new rank to the Person Record */
  void setRank(EnumDb::PersonRanks new_rank);

  /* Assigns a new value to battles_won to the Person Record */
  void setBattlesWon(uint new_value);

  /* Assigns a new value to battles_won_naked to the Person Record */
  void setBattlesWonNaked(uint new_value);

  /* Assigns a new value to battles_won_empty_handed to the Person Record */
  void setBattlesWonEmptyHanded(uint new_value);

  /* Assigns a new value to battles_won_mini_boss to the Person Record */
  void setBattlesWonMiniBoss(uint new_value);

  /* Assigns a new value to battles_won_boss to the Person Record */
  void setBattlesWonBoss(uint new_value);

  /* Assigns a new value to battles_won_final_boss to the Person Record */
  void setBattlesWonFinalBoss(uint new_value);

  /* Assigns a new value to kills_bear to the Person Record */
  void setKillsBear(uint new_value);

  /* Assigns a new value to kills_human to the Person Record */
  void setKillsHuman(uint new_value);

  /* Assigns a new value to kills_fiend to the Person Record */
  void setKillsFiend(uint new_value);

  /* Assigns a new value to grand_score to the Person Record */
  void setGrandScore(uint new_value);

  /* Assigns a new value to times_kod to the Person Record */
  void setTimesKOd(uint new_value);

  /* Returns the rank of the Person */
  EnumDb::PersonRanks getRank();

  /* Returns the value of battles_won */
  uint getBattlesWon();

  /* Returns the value of battles_won_naked */
  uint getBattlesWonNaked();

  /* Returns the value of battles_won_empty_handed */
  uint getBattlesWonEmptyHanded();

  /* Returns the value of battles_won_mini_boss */
  uint getBattlesWonMiniBoss();

  /* Returns the value of battles_won_boss */
  uint getBattlesWonBoss();

  /* Returns the value of battles_won_final_boss */
  uint getBattlesWonFinalBoss();

  /* Returns the value of kills_bear */
  uint getKillsBear();

  /* Returns the value of kills_human */
  uint getKillsHuman();

  /* Returns the value of kills_fiend */
  uint getKillsFiend();

  /* Returns the value of grand_score */
  uint getGrandScore();

  /* Returns the value of times_kod */
  uint getTimesKOd();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
    PersonRecord& operator= (const PersonRecord &source);
};

#endif // PERSONRECORD_H
