/******************************************************************************
* Class Name: Person Record [Implementation]
* Date Created: October 6th, 2013
* Inheritance: None
* Description: A class which represents an object holding the records of a
*              person. This class will be used to keep track of the progress
*              a person (character) has.
******************************************************************************/
#include "Game/Player/PersonRecord.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

QVector<int> PersonRecord::rank_points_required;

/* Defaults */
const uint PersonRecord::kDEFAULT_GRAND_SCORE =  2;
const uint PersonRecord::kRANKS               = 12;

/* Maximums */
const uint PersonRecord::kMAX_BATTLES_WON     =      99999;
const uint PersonRecord::kMAX_KILLS           =     999999;
const uint PersonRecord::kMAX_GRAND_SCORE     = 2000000000;
const uint PersonRecord::kMAX_TIMES_KOD       =     999999;
const uint PersonRecord::kMAX_RANK_SCORE      =  500000000;

/* Point Values */
const uint PersonRecord::kPOINTS_BATTLE_WON               =   100;
const uint PersonRecord::kPOINTS_BATTLE_WON_NAKED         =   200;
const uint PersonRecord::kPOINTS_BATTLE_WON_EMPTY_HANDED  =   150;
const uint PersonRecord::kPOINTS_BATTLE_WON_MINI_BOSS     =  1000;
const uint PersonRecord::kPOINTS_BATTLE_WON_BOSS          =  5000;
const uint PersonRecord::kPOINTS_BATTLE_WON_FINAL_BOSS    = 50000;

const uint PersonRecord::kPOINTS_KILLS_BEAR  = 25;
const uint PersonRecord::kPOINTS_KILLS_HUMAN = 20;
const uint PersonRecord::kPOINTS_KILLS_FIEND = 15;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Creates a default PersonRecord object
 *
 * Inputs:
 * Output:
 */
PersonRecord::PersonRecord()
{
  loadDefault();
}

PersonRecord::PersonRecord(EnumDb::PersonRanks rank_preset)
{
  loadPreset(rank_preset);
}

/*
 * Description: Annihilates a PersonRecord object
 */
PersonRecord::~PersonRecord() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Creates a default PersonRecord object
 *
 * Inputs:
 * Output:
 */
void PersonRecord::setUpRankPoints()
{
    rank_points_required = buildExponentialTable(kDEFAULT_GRAND_SCORE,
                                                  kMAX_RANK_SCORE,
                                                  kRANKS);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Resets the PersonRecord object to a default state.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::loadDefault()
{

}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::loadPreset(EnumDb::PersonRanks rank_preset)
{
  if (rank_preset == EnumDb::NUBEAR)
  {


  }
  else if (rank_preset == EnumDb::CUB)
  {


  }
  else if (rank_preset == EnumDb::RECRUIT)
  {


  }
  else if (rank_preset == EnumDb::SLEUTHLING)
  {


  }
  else if (rank_preset == EnumDb::SERGEANT)
  {


  }
  else if (rank_preset == EnumDb::SLEUTHMASTER)
  {


  }
  else if (rank_preset == EnumDb::OFFICER)
  {


  }
  else if (rank_preset == EnumDb::URSAMINOR)
  {


  }
  else if (rank_preset == EnumDb::ADMIRAL)
  {


  }
  else if (rank_preset == EnumDb::URSAMAJOR)
  {


  }
  else if (rank_preset == EnumDb::FOREBEAR)
  {


  }
  else if (rank_preset == EnumDb::ALPHABEAR)
  {


  }
  else if (rank_preset == EnumDb::BOAT)
  {


  }
}

/*
 * Description: Assigns a new value to battles_won
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setBattlesWon(uint new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won = kMAX_BATTLES_WON;
  else
    battles_won = new_value;
}

/*
 * Description: Assigns a new value to battles_won_naked
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setBattlesWonNaked(uint new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_naked = kMAX_BATTLES_WON;
  else
    battles_won_naked = new_value;
}

/*
 * Description: Assigns a new value to battles_won_empty_handed
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setBattlesWonEmptyHanded(uint new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_empty_handed = kMAX_BATTLES_WON;
  else
    battles_won_empty_handed = new_value;
}

/*
 * Description: Assigns a new value to battles_won_mini_boss
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setBattlesWonMiniBoss(uint new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_mini_boss = kMAX_BATTLES_WON;
  else
    battles_won_mini_boss = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setBattlesWonBoss(uint new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_boss = kMAX_BATTLES_WON;
  else
    battles_won_boss = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setBattlesWonFinalBoss(uint new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_final_boss = kMAX_BATTLES_WON;
  else
    battles_won_final_boss = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setKillsBear(uint new_value)
{
  if (new_value > kMAX_KILLS)
    kills_bear = kMAX_KILLS;
  else
    kills_bear = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setKillsHuman(uint new_value)
{
  if (new_value > kMAX_KILLS)
    kills_human = kMAX_KILLS;
  else
    kills_human = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setKillsFiend(uint new_value)
{
  if (new_value > kMAX_KILLS)
    kills_fiend = kMAX_KILLS;
  else
    kills_fiend = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setGrandScore(uint new_value)
{
  if (new_value < kDEFAULT_GRAND_SCORE)
    grand_score = kDEFAULT_GRAND_SCORE;
  else if (new_value > kMAX_GRAND_SCORE)
    grand_score = kMAX_GRAND_SCORE;
  else
    grand_score = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setTimesKOd(uint new_value)
{
  if (new_value > kMAX_TIMES_KOD)
    times_kod = kMAX_TIMES_KOD;
  else
    times_kod = new_value;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getBattlesWon()
{
  return battles_won;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getBattlesWonNaked()
{
  return battles_won_naked;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getBattlesWonEmptyHanded()
{
  return battles_won_empty_handed;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getBattlesWonMiniBoss()
{
  return battles_won_mini_boss;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getBattlesWonBoss()
{
  return battles_won_boss;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getBattlesWonFinalBoss()
{
  return battles_won_final_boss;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getKillsBear()
{
  return kills_bear;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getKillsHuman()
{
  return kills_human;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getKillsFiend()
{
  return kills_fiend;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getGrandScore()
{
   return grand_score;
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: none
 * Output: none
 */
uint PersonRecord::getTimesKOd()
{
   return times_kod;
}
