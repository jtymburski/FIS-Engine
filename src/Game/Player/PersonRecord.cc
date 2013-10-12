/******************************************************************************
* Class Name: Person Record [Implementation]
* Date Created: October 6th, 2013
* Inheritance: None
* Description: A class which represents an object holding the records of a
*              person. This class will be used to keep track of the progress
*              a person (character) has.
*
* //TODO: Build preset values [10-06-13]
******************************************************************************/
#include "Game/Player/PersonRecord.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/* Vector of points required for each rank */
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
 * Inputs: none
 */
PersonRecord::PersonRecord()
{
  if (rank_points_required.isEmpty())
    setUpRankPoints();

  loadDefault();
}

/*
 * Description: Creates a default PersonRecord object
 *
 * Inputs: PersonRecord& - object to be copied.
 */
PersonRecord::PersonRecord(const PersonRecord &source)
{
  copySelf(source);
}

/*
 * Description: Creates a PersonRecord object with a rank_preset
 *
 * Inputs: EnumDb::PersonRanks - the rank for the PersonRecord to be preset at
 */
PersonRecord::PersonRecord(EnumDb::PersonRanks rank_preset)
{
  if (rank_points_required.isEmpty())
    setUpRankPoints();

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
 * Description: Sets up the rank_points_required vector with an exponential
 *              table built between the default grand score and the highest
 *              score needed to achieve the top rank.
 *
 * Notes [1]: Static function.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::setUpRankPoints()
{
  rank_points_required = buildExponentialTable(kDEFAULT_GRAND_SCORE,
                                               kMAX_RANK_SCORE, kRANKS);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void PersonRecord::copySelf(const PersonRecord &source)
{
  current_rank = source.current_rank;

  battles_won = source.battles_won;
  battles_won_naked = source.battles_won_naked;
  battles_won_empty_handed = source.battles_won_empty_handed;
  battles_won_mini_boss = source.battles_won_mini_boss;
  battles_won_boss = source.battles_won_boss;
  battles_won_final_boss = source.battles_won_final_boss;

  kills_bear = source.kills_bear;
  kills_human = source.kills_human;
  kills_fiend = source.kills_fiend;

  grand_score = source.grand_score;
  times_kod = source.times_kod;
}

/*
 * Description: Prints everything related to this and general PersonalRecords.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::calcGrandScore()
{
  uint temp = kDEFAULT_GRAND_SCORE;

  temp += battles_won * kPOINTS_BATTLE_WON;
  temp += battles_won_naked  * kPOINTS_BATTLE_WON_NAKED;
  temp += battles_won_empty_handed * kPOINTS_BATTLE_WON_EMPTY_HANDED;
  temp += battles_won_mini_boss * kPOINTS_BATTLE_WON_MINI_BOSS;
  temp += battles_won_boss * kPOINTS_BATTLE_WON_BOSS;
  temp += battles_won_final_boss * kPOINTS_BATTLE_WON_FINAL_BOSS;

  temp += kills_bear  * kPOINTS_KILLS_BEAR;
  temp += kills_human * kPOINTS_KILLS_HUMAN;
  temp += kills_fiend * kPOINTS_KILLS_FIEND;

  setGrandScore(temp);
}

/*
 * Description: Prints everything related to this and general PersonalRecords.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::printAll()
{
  qDebug() << " --- Personal Record ---";
  printConstants();
  printRecord();
  qDebug() << " --- / Personal Record --- ";
}

/*
 * Description: Prints out the constants of the general PersonalRecord
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::printConstants()
{
  qDebug() << " --- Personal Record Constants ---";
  qDebug() << "Default Grand Score: " << kDEFAULT_GRAND_SCORE;
  qDebug() << "Number of Ranks: " << kRANKS;
  qDebug() << "Max Battles Won: " << kMAX_BATTLES_WON;
  qDebug() << "Max Kills: " << kMAX_KILLS;
  qDebug() << "Max Grand Score: " << kMAX_GRAND_SCORE;
  qDebug() << "Max Times KOd: " << kMAX_TIMES_KOD;
  qDebug() << "Max Rank Score: " << kMAX_RANK_SCORE;
  qDebug() << "Points Battle Won: " << kPOINTS_BATTLE_WON;
  qDebug() << "Points Battle Won Naked: " << kPOINTS_BATTLE_WON_EMPTY_HANDED;
  qDebug() << "Points Battle Won Empty Handed: " << kPOINTS_BATTLE_WON_MINI_BOSS;
  qDebug() << "Points Battle Won Mini Boss: " << kPOINTS_BATTLE_WON_BOSS;
  qDebug() << "Points Battle Won Boss: " << kPOINTS_BATTLE_WON_BOSS;
  qDebug() << "Points Battle Won Final Boss: " << kPOINTS_BATTLE_WON_FINAL_BOSS;
  qDebug() << "Points Kills Bear: " << kPOINTS_KILLS_BEAR;
  qDebug() << "Points Kills Human: " << kPOINTS_KILLS_HUMAN;
  qDebug() << "Points Kills Fiend: " << kPOINTS_KILLS_FIEND;
}

/*
 * Description: Prints out the information of (*this) PersonalRecord
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::printRecord()
{
  qDebug() << "Battles Won: " << battles_won;
  qDebug() << "Battles Won Naked: " << battles_won_naked;
  qDebug() << "Battles Won Empty Handed: " << battles_won_empty_handed;
  qDebug() << "Battles Won Mini Boss: " << battles_won_mini_boss;
  qDebug() << "Battles Won Boss: " << battles_won_boss;
  qDebug() << "Battles Won Final Boss: " << battles_won_final_boss;
  qDebug() << "Kills Bear: " << kills_bear;
  qDebug() << "Kills Human: " << kills_human;
  qDebug() << "Kills Fiend: " << kills_fiend;
  qDebug() << "Grand Score: " << grand_score;
  qDebug() << "Times KOd: " << times_kod;
}

/*
 * Description: Performs an update to the PersonRecord. For instance, this
 *              upgrades the current rank of the person to the correct value.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::update()
{
  setRank(rankAtScore(getGrandScore()));
}

/*
 * Description: Resets the PersonRecord object to a default state.
 *
 * Inputs: none
 * Output: none
 */
void PersonRecord::loadDefault()
{
  setRank(EnumDb::NUBEAR);

  setBattlesWon(0);
  setBattlesWonNaked(0);
  setBattlesWonEmptyHanded(0);
  setBattlesWonMiniBoss(0);
  setBattlesWonBoss(0);
  setBattlesWonFinalBoss(0);

  setKillsBear(0);
  setKillsHuman(0);
  setKillsFiend(0);
  setGrandScore(kDEFAULT_GRAND_SCORE);
  setTimesKOd(0);
}

/*
 * Description: Loads the PersonObject with one of the pre-configured presets.
 *
 * Inputs: EnumDb::PersonRanks - rank for record to be preset to.
 * Output: none
 * //TODO: Set values for presets [10-06-13]
 */
void PersonRecord::loadPreset(EnumDb::PersonRanks rank_preset)
{
  /* Nubear Preset */
  if (rank_preset == EnumDb::NUBEAR)
  {
    loadDefault();
  }

  /* Cub Preset */
  else if (rank_preset == EnumDb::CUB)
  {
    setRank(EnumDb::CUB);
    setBattlesWon(10);
    setBattlesWonNaked(1);
    setBattlesWonEmptyHanded(1);
    setBattlesWonMiniBoss(0);
    setBattlesWonBoss(0);
    setBattlesWonFinalBoss(0);

    setKillsBear(3);
    setKillsHuman(3);
    setKillsFiend(1);
    setTimesKOd(0);

    calcGrandScore();
  }

  /* Recruit Preset */
  else if (rank_preset == EnumDb::RECRUIT)
  {
    setRank(EnumDb::RECRUIT);
    setBattlesWon(20);
    setBattlesWonNaked(5);
    setBattlesWonEmptyHanded(3);
    setBattlesWonMiniBoss(1);
    setBattlesWonBoss(0);
    setBattlesWonFinalBoss(0);

    setKillsBear(7);
    setKillsHuman(8);
    setKillsFiend(5);
    setTimesKOd(0);

    calcGrandScore();
  }

  /* Sleuthling Preset */
  else if (rank_preset == EnumDb::SLEUTHLING)
  {


  }

  /* Sergeant Preset */
  else if (rank_preset == EnumDb::SERGEANT)
  {


  }

  /* Sleuthmaster Preset */
  else if (rank_preset == EnumDb::SLEUTHMASTER)
  {


  }

  /* Officer Preset */
  else if (rank_preset == EnumDb::OFFICER)
  {


  }

  /* Ursa Minor Preset */
  else if (rank_preset == EnumDb::URSAMINOR)
  {


  }

  /* Admiral Preset */
  else if (rank_preset == EnumDb::ADMIRAL)
  {


  }

  /* Ursa Major Preset */
  else if (rank_preset == EnumDb::URSAMAJOR)
  {


  }

  /* Forebear Preset */
  else if (rank_preset == EnumDb::FOREBEAR)
  {


  }

  /* Alpha Bear Preset */
  else if (rank_preset == EnumDb::ALPHABEAR)
  {


  }

  /* BOAT Preset */
  else if (rank_preset == EnumDb::BOAT)
  {


  }
}

/*
 * Description: Returns the rank at a given index.
 *
 * Inputs: uint index - index of the rank to be checked.
 * Output: EnumDb::PersonRank - the rank at the index, if it exists.
 */
EnumDb::PersonRanks PersonRecord::rankAtIndex(uint index)
{
  if      (index ==  0) return EnumDb::NUBEAR;
  else if (index ==  1) return EnumDb::CUB;
  else if (index ==  2) return EnumDb::RECRUIT;
  else if (index ==  3) return EnumDb::SLEUTHLING;
  else if (index ==  4) return EnumDb::SERGEANT;
  else if (index ==  5) return EnumDb::SLEUTHMASTER;
  else if (index ==  6) return EnumDb::OFFICER;
  else if (index ==  7) return EnumDb::URSAMINOR;
  else if (index ==  8) return EnumDb::ADMIRAL;
  else if (index ==  9) return EnumDb::URSAMAJOR;
  else if (index == 10) return EnumDb::FOREBEAR;
  else if (index == 11) return EnumDb::ALPHABEAR;
  else if (index == 12) return EnumDb::BOAT;

  return EnumDb::NULL_RANK;
}

/*
 * Description: Returns the highest rank achieveable at a given rank.
 *
 * Inputs: uint score - score to be checked against rank-scores.
 * Output: EnumDb::PersonRank - the highest rank achieveable
 */
EnumDb::PersonRanks PersonRecord::rankAtScore(uint score)
{
  uint index = 1;
  EnumDb::PersonRanks temporary_rank = rankAtIndex(index);

  /* Loop through every rank until the score is greater than required */
  while (score < (uint)scoreAtRank(temporary_rank) && index <= kRANKS)
    temporary_rank = rankAtIndex(++index);

  return rankAtIndex(index - 1);
}

/*
 * Description: Returns the lowest score required to achieve a given rank.
 *
 * Inputs: EnumDb::PersonRanks - the rank to be checked against score-ranks.
 * Output: int - the lowest score required to achieve the given rank.
 */
int PersonRecord::scoreAtRank(EnumDb::PersonRanks rank)
{
  EnumDb::PersonRanks check_rank;

  /* Loop through each rank until the rank to be checked is found */
  for (int index = 0; index <= (int)kRANKS; index++)
  {
    check_rank = rankAtIndex(index);

    /* Return the points required at the found rank */
    if (rank == check_rank)
      return rank_points_required.at(index);
  }

  return -1;
}

/*
 * Description: Assigns a new rank to the PersonRecord
 *
 * Inputs: EnumDb::PersonRecord - new rank for the PersonRecord
 * Output: none
 */
void PersonRecord::setRank(EnumDb::PersonRanks new_rank)
{
  current_rank = new_rank;
}

/*
 * Description: Assigns a new value to battles_won
 *
 * Inputs: uint new_value - new value for battles_won
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
 * Inputs: uint new_value - new value for battles_won_naked
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
 * Inputs: uint new_value - new value for battles_won_empty_handed
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
 * Inputs: uint new_value - new value for battles_won_mini_boss
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
 * Description: Assigns a new value to battles_won_boss
 *
 * Inputs: uint new_value - new value for battles_won_boss
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
 * Description: Assgns a new value to battles_won_final_boss
 *
 * Inputs: uint new_value - new value for battles_won_final_boss
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
 * Description: Assigns a new value to kills_bear
 *
 * Inputs: uint new_value - new value for kills_bear
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
 * Description: Assigns a new value to kills_human
 *
 * Inputs: uint new_value - new value for kills_human
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
 * Description: Assigns a new value to kills_fiend
 *
 * Inputs: uint new_value - new value for kills_fiend
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
 * Description: Assigns a new value to grand_score
 *
 * Inputs: uint new_value - new value for grand_score
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
 * Description: Assigns a new value to times_kod
 *
 * Inputs: uint new_value - new value to be assigned times_kod
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
 * Description: Returns the rank of the person
 *
 * Inputs: none
 * Output: EnumDb::PersonRanks - the current rank of the Person.
 */
EnumDb::PersonRanks PersonRecord::getRank()
{
  return current_rank;
}

/*
 * Description: Returns the value of battles_won
 *
 * Inputs: none
 * Output: uint - battles_won
 */
uint PersonRecord::getBattlesWon()
{
  return battles_won;
}

/*
 * Description: Returns the value of battles_won_naked
 *
 * Inputs: none
 * Output: uint - battles_won_naked
 */
uint PersonRecord::getBattlesWonNaked()
{
  return battles_won_naked;
}

/*
 * Description: Returns the value of battles_won_empty_handed
 *
 * Inputs: none
 * Output: uint - battles_won_empty_handed
 */
uint PersonRecord::getBattlesWonEmptyHanded()
{
  return battles_won_empty_handed;
}

/*
 * Description: Returns the value of battles_won_mini_boss
 *
 * Inputs: none
 * Output: uint - battles_won_mini_boss
 */
uint PersonRecord::getBattlesWonMiniBoss()
{
  return battles_won_mini_boss;
}

/*
 * Description: Returns the value of battles_won_boss
 *
 * Inputs: none
 * Output: uint - battles_won_boss
 */
uint PersonRecord::getBattlesWonBoss()
{
  return battles_won_boss;
}

/*
 * Description: Returns the value of battles_won_final_boss
 *
 * Inputs: none
 * Output: uint - battles_won_final_boss
 */
uint PersonRecord::getBattlesWonFinalBoss()
{
  return battles_won_final_boss;
}

/*
 * Description: Returns the value of kills_bear
 *
 * Inputs: none
 * Output: uint - kills_bear
 */
uint PersonRecord::getKillsBear()
{
  return kills_bear;
}

/*
 * Description: Returns the value of kills_human
 *
 * Inputs: none
 * Output: uint - kills_human
 */
uint PersonRecord::getKillsHuman()
{
  return kills_human;
}

/*
 * Description: Returns the value of kills_fiend
 *
 * Inputs: none
 * Output: uint - kills_fiend
 */
uint PersonRecord::getKillsFiend()
{
  return kills_fiend;
}

/*
 * Description: Returns the value of grand_score
 *
 * Inputs: none
 * Output: uint - grand_score
 */
uint PersonRecord::getGrandScore()
{
   return grand_score;
}

/*
 * Description: Returns the value of times_kod
 *
 * Inputs: none
 * Output: uint - times_kod
 */
uint PersonRecord::getTimesKOd()
{
   return times_kod;
}

/*
 * Description: Overloaded operator = function for deep copy of PersonRecord.
 *              Calls copySelf function for copying after a check for self-
 *              assignment.
 *
 * Inputs: source - const reference of object to be copied
 * Output:
 */
PersonRecord& PersonRecord::operator= (const PersonRecord &source)
{
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
