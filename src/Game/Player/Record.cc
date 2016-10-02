/*******************************************************************************
* Class Name: Record (Formerly PersonRecord) [Implementation]
* Date Created: January 23rd, 2014
* Inheritance: 
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Record.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/* Vector of points required for each rank */
std::vector<int32_t> Record::rank_points_required;

/* Defaults */
const uint32_t Record::kDEFAULT_GRAND_SCORE =  2;
const uint32_t Record::kRANKS               = 12;

/* Maximums */
const uint32_t Record::kMAX_BATTLES_WON     =      99999;
const uint32_t Record::kMAX_KILLS           =     999999;
const uint32_t Record::kMAX_GRAND_SCORE     = 2000000000;
const uint32_t Record::kMAX_TIMES_KOD       =     999999;
const uint32_t Record::kMAX_RANK_SCORE      =  500000000;

/* Point Values */
const uint32_t Record::kPOINTS_BATTLE_WON               =   100;
const uint32_t Record::kPOINTS_BATTLE_WON_NAKED         =   200;
const uint32_t Record::kPOINTS_BATTLE_WON_EMPTY_HANDED  =   150;
const uint32_t Record::kPOINTS_BATTLE_WON_MINI_BOSS     =  1000;
const uint32_t Record::kPOINTS_BATTLE_WON_BOSS          =  5000;
const uint32_t Record::kPOINTS_BATTLE_WON_FINAL_BOSS    = 50000;

const uint32_t Record::kPOINTS_KILLS_BEAR  = 25;
const uint32_t Record::kPOINTS_KILLS_HUMAN = 20;
const uint32_t Record::kPOINTS_KILLS_FIEND = 15;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Creates a Record object with a rank_preset
 *
 * Inputs: EnumDb::PersonRanks - the rank for the Record to be preset at
 */
Record::Record(const Ranks &rank_preset)
{
  if (rank_points_required.isEmpty())
    setUpRankPoints();

  loadPreset(rank_preset);
}

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
void Record::setUpRankPoints()
{
  rank_points_required = Helpers::buildExpTable(kDEFAULT_GRAND_SCORE,
                                                kMAX_RANK_SCORE, kRANKS);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/


/*
 * Description: Prints everything related to this and general PersonalRecords.
 *
 * Inputs: none
 * Output: none
 */
void Record::calcGrandScore()
{
  uint32_t temp = kDEFAULT_GRAND_SCORE;

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
void Record::print(const bool &p_const)
{
  cout << " --- Personal Record ---\n";

  if (p_const)
  {
    cout << " --- Personal Record Constants ---\n";
    cout << "Default Grand Score: " << kDEFAULT_GRAND_SCORE;
    cout << "\nNumber of Ranks: " << kRANKS;
    cout << "\nMax Battles Won: " << kMAX_BATTLES_WON;
    cout << "\nMax Kills: " << kMAX_KILLS;
    cout << "\nMax Grand Score: " << kMAX_GRAND_SCORE;
    cout << "\nMax Times KOd: " << kMAX_TIMES_KOD;
    cout << "\nMax Rank Score: " << kMAX_RANK_SCORE;
    cout << "\nPoints Battle Won: " << kPOINTS_BATTLE_WON;
    cout << "\nPoints Battle Won Naked: " << kPOINTS_BATTLE_WON_EMPTY_HANDED;
    cout << "\nPoints Battle Won Empty Handed: " << kPOINTS_BATTLE_WON_MINI_BOSS;
    cout << "\nPoints Battle Won Mini Boss: " << kPOINTS_BATTLE_WON_BOSS;
    cout << "\nPoints Battle Won Boss: " << kPOINTS_BATTLE_WON_BOSS;
    cout << "\nPoints Battle Won Final Boss: " << kPOINTS_BATTLE_WON_FINAL_BOSS;
    cout << "\nPoints Kills Bear: " << kPOINTS_KILLS_BEAR;
    cout << "\nPoints Kills Human: " << kPOINTS_KILLS_HUMAN;
    cout << "Points Kills Fiend: " << kPOINTS_KILLS_FIEND << "\n";
  }

  cout << "Battles Won: " << battles_won;
  cout << "\nBattles Won Naked: " << battles_won_naked;
  cout << "\nBattles Won Empty Handed: " << battles_won_empty_handed;
  cout << "\nBattles Won Mini Boss: " << battles_won_mini_boss;
  cout << "\nBattles Won Boss: " << battles_won_boss;
  cout << "\nBattles Won Final Boss: " << battles_won_final_boss;
  cout << "\nKills Bear: " << kills_bear;
  cout << "\nKills Human: " << kills_human;
  cout << "\nKills Fiend: " << kills_fiend;
  cout << "\nGrand Score: " << grand_score;
  cout << "Times KOd: " << times_kod << "\n";
  cout << " --- / Personal Record ---\n";
}

/*
 * Description: Performs an update to the Record. For instance, this
 *              upgrades the current rank of the person to the correct value.
 *
 * Inputs: none
 * Output: none
 */
void Record::update()
{
  setRank(rankAtScore(getGrandScore()));
}

/*
 * Description: Resets the Record object to a default state.
 *
 * Inputs: none
 * Output: none
 */
void Record::loadDefault()
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
void Record::loadPreset(const Ranks &rank_preset)
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
 * Inputs: uint32_t index - index of the rank to be checked.
 * Output: EnumDb::PersonRank - the rank at the index, if it exists.
 */
EnumDb::PersonRanks Record::rankAtIndex(uint32_t index)
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
 * Inputs: uint32_t score - score to be checked against rank-scores.
 * Output: EnumDb::PersonRank - the highest rank achieveable
 */
EnumDb::PersonRanks Record::rankAtScore(uint32_t score)
{
  uint32_t index = 1;
  EnumDb::PersonRanks temporary_rank = rankAtIndex(index);

  /* Loop through every rank until the score is greater than required */
  while (score < (uint32_t)scoreAtRank(temporary_rank) && index <= kRANKS)
    temporary_rank = rankAtIndex(++index);

  return rankAtIndex(index - 1);
}

/*
 * Description: Returns the lowest score required to achieve a given rank.
 *
 * Inputs: EnumDb::PersonRanks - the rank to be checked against score-ranks.
 * Output: int - the lowest score required to achieve the given rank.
 */
int Record::scoreAtRank(EnumDb::PersonRanks rank)
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
 * Description: Assigns a new rank to the Record
 *
 * Inputs: EnumDb::Record - new rank for the Record
 * Output: none
 */
void Record::setRank(EnumDb::PersonRanks new_rank)
{
  current_rank = new_rank;
}

/*
 * Description: Assigns a new value to battles_won
 *
 * Inputs: uint32_t new_value - new value for battles_won
 * Output: none
 */
void Record::setBattlesWon(uint32_t new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won = kMAX_BATTLES_WON;
  else
    battles_won = new_value;
}

/*
 * Description: Assigns a new value to battles_won_naked
 *
 * Inputs: uint32_t new_value - new value for battles_won_naked
 * Output: none
 */
void Record::setBattlesWonNaked(uint32_t new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_naked = kMAX_BATTLES_WON;
  else
    battles_won_naked = new_value;
}

/*
 * Description: Assigns a new value to battles_won_empty_handed
 *
 * Inputs: uint32_t new_value - new value for battles_won_empty_handed
 * Output: none
 */
void Record::setBattlesWonEmptyHanded(uint32_t new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_empty_handed = kMAX_BATTLES_WON;
  else
    battles_won_empty_handed = new_value;
}

/*
 * Description: Assigns a new value to battles_won_mini_boss
 *
 * Inputs: uint32_t new_value - new value for battles_won_mini_boss
 * Output: none
 */
void Record::setBattlesWonMiniBoss(uint32_t new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_mini_boss = kMAX_BATTLES_WON;
  else
    battles_won_mini_boss = new_value;
}

/*
 * Description: Assigns a new value to battles_won_boss
 *
 * Inputs: uint32_t new_value - new value for battles_won_boss
 * Output: none
 */
void Record::setBattlesWonBoss(uint32_t new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_boss = kMAX_BATTLES_WON;
  else
    battles_won_boss = new_value;
}

/*
 * Description: Assgns a new value to battles_won_final_boss
 *
 * Inputs: uint32_t new_value - new value for battles_won_final_boss
 * Output: none
 */
void Record::setBattlesWonFinalBoss(uint32_t new_value)
{
  if (new_value > kMAX_BATTLES_WON)
    battles_won_final_boss = kMAX_BATTLES_WON;
  else
    battles_won_final_boss = new_value;
}

/*
 * Description: Assigns a new value to kills_bear
 *
 * Inputs: uint32_t new_value - new value for kills_bear
 * Output: none
 */
void Record::setKillsBear(uint32_t new_value)
{
  if (new_value > kMAX_KILLS)
    kills_bear = kMAX_KILLS;
  else
    kills_bear = new_value;
}

/*
 * Description: Assigns a new value to kills_human
 *
 * Inputs: uint32_t new_value - new value for kills_human
 * Output: none
 */
void Record::setKillsHuman(uint32_t new_value)
{
  if (new_value > kMAX_KILLS)
    kills_human = kMAX_KILLS;
  else
    kills_human = new_value;
}

/*
 * Description: Assigns a new value to kills_fiend
 *
 * Inputs: uint32_t new_value - new value for kills_fiend
 * Output: none
 */
void Record::setKillsFiend(uint32_t new_value)
{
  if (new_value > kMAX_KILLS)
    kills_fiend = kMAX_KILLS;
  else
    kills_fiend = new_value;
}

/*
 * Description: Assigns a new value to grand_score
 *
 * Inputs: uint32_t new_value - new value for grand_score
 * Output: none
 */
void Record::setGrandScore(uint32_t new_value)
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
 * Inputs: uint32_t new_value - new value to be assigned times_kod
 * Output: none
 */
void Record::setTimesKOd(uint32_t new_value)
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
EnumDb::PersonRanks Record::getRank()
{
  return current_rank;
}

/*
 * Description: Returns the value of battles_won
 *
 * Inputs: none
 * Output: uint32_t - battles_won
 */
uint32_t Record::getBattlesWon()
{
  return battles_won;
}

/*
 * Description: Returns the value of battles_won_naked
 *
 * Inputs: none
 * Output: uint32_t - battles_won_naked
 */
uint32_t Record::getBattlesWonNaked()
{
  return battles_won_naked;
}

/*
 * Description: Returns the value of battles_won_empty_handed
 *
 * Inputs: none
 * Output: uint32_t - battles_won_empty_handed
 */
uint32_t Record::getBattlesWonEmptyHanded()
{
  return battles_won_empty_handed;
}

/*
 * Description: Returns the value of battles_won_mini_boss
 *
 * Inputs: none
 * Output: uint32_t - battles_won_mini_boss
 */
uint32_t Record::getBattlesWonMiniBoss()
{
  return battles_won_mini_boss;
}

/*
 * Description: Returns the value of battles_won_boss
 *
 * Inputs: none
 * Output: uint32_t - battles_won_boss
 */
uint32_t Record::getBattlesWonBoss()
{
  return battles_won_boss;
}

/*
 * Description: Returns the value of battles_won_final_boss
 *
 * Inputs: none
 * Output: uint32_t - battles_won_final_boss
 */
uint32_t Record::getBattlesWonFinalBoss()
{
  return battles_won_final_boss;
}

/*
 * Description: Returns the value of kills_bear
 *
 * Inputs: none
 * Output: uint32_t - kills_bear
 */
uint32_t Record::getKillsBear()
{
  return kills_bear;
}

/*
 * Description: Returns the value of kills_human
 *
 * Inputs: none
 * Output: uint32_t - kills_human
 */
uint32_t Record::getKillsHuman()
{
  return kills_human;
}

/*
 * Description: Returns the value of kills_fiend
 *
 * Inputs: none
 * Output: uint32_t - kills_fiend
 */
uint32_t Record::getKillsFiend()
{
  return kills_fiend;
}

/*
 * Description: Returns the value of grand_score
 *
 * Inputs: none
 * Output: uint32_t - grand_score
 */
uint32_t Record::getGrandScore()
{
   return grand_score;
}

/*
 * Description: Returns the value of times_kod
 *
 * Inputs: none
 * Output: uint32_t - times_kod
 */
uint32_t Record::getTimesKOd()
{
   return times_kod;
}