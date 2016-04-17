/*******************************************************************************
* Class Name: Player [Implementation]
* Date Created: January 18th, 2014
* Inheritance: none
* Description: A Player contains the information of the Parties that are usable
*              by the player and represents pertinent information to the
*              progression of the game.
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Player.h"

/*=============================================================================
 * CONSTANTS - See header declaration for details
 *============================================================================*/

const uint32_t Player::kDEFAULT_GRAVITY{981};
const uint32_t Player::kSTARTING_CREDITS{100};
const uint32_t Player::kMAX_CREDITS{3141592654};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Player construction! Constructs a player with a Sleuth and
 *              Bearacks party pointers.
 *
 * Inputs: Party* sleuth - pointer to the sleuth party for the player.
 *         Party* bearacks - pointer to the bearacks party for the player.
 */
Player::Player(Party* sleuth, Party* bearacks)
    : sleuth{sleuth}
    , bearacks{bearacks}
    , credits{kSTARTING_CREDITS}
    , gravity{kDEFAULT_GRAVITY}
    , steps{0}
{
  play_time = {0, 0, 0};
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add an amount of credits to the player's total.
 *
 * Inputs: uint32_t value - the number of credits to add
 * Output: bool - true if credits added (even if max limit reached)
 */
bool Player::addCredits(const uint32_t &value)
{
  auto added = false;

  if (value < kMAX_CREDITS)
  {
    if(kMAX_CREDITS - value >= credits)
      credits += value;
    else
      credits = kMAX_CREDITS;
    added = true;
  }

  return added;
}

/*
 * Description: Attempts to add a learned skill to the given party type with a
 *              required level. The party types are "sleuth" and "bearacks".
 *
 * Inputs: std::string party_type - the party type: sleuth or bearacks
 *         Skill* skill - the skill to attempt to load
 *         uint32_t person_index - the person index within the party. Default 0
 *         uint32_t req_level - the level required for the person to use
 * Output: bool - true if the skill was added to the learned set
 */
bool Player::addLearnedSkill(std::string party_type, Skill* skill,
                       const uint32_t &person_index, const uint32_t &req_level)
{
  bool success = false;

  if(skill != nullptr)
  {
    /* Get the reference party */
    Party* ref_party = nullptr;
    if(party_type == "sleuth")
      ref_party = sleuth;
    else if(party_type == "bearacks")
      ref_party = bearacks;

    /* Access the party */
    if(ref_party != nullptr && ref_party->getMember(person_index) != nullptr)
    {
      success = ref_party->getMember(person_index)->getLearnedSet(true)
                                                  ->addSkill(skill, req_level);
    }
  }

  return success;
}

/*
 * Description: Adds play time in milliseconds to the total time played of the
 *              player.
 *
 * Inputs: const uint32_t &milliseconds - the delta time in milliseconds
 * Output: none
 */
void Player::addPlayTime(const uint32_t &milliseconds)
{
  /* Add time */
  play_time.milliseconds += milliseconds;

  /* If beyond thresholds for milliseconds, address */
  if(play_time.milliseconds >= 60000)
  {
    play_time.minutes += (play_time.milliseconds / 60000);
    play_time.milliseconds = (play_time.milliseconds % 60000);

    /* If beyond thresholds for minutes, address */
    if(play_time.minutes >= 60)
    {
      play_time.hours += (play_time.minutes / 60);
      play_time.minutes = (play_time.minutes % 60);
    }
  }
}

/*
 * Description: Adds steps onto the total that the player has traversed in
 *              number of tiles. Triggered on save or changed map.
 *
 * Inputs: const uint32_t &value - the value of steps in tiles to add
 * Output: none
 */
void Player::addSteps(const uint32_t &value)
{
  steps += value;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
/* Returns the amount of credits (money) the player has */
uint32_t Player::getCredits()
{
  return credits;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
/* Gets the bearacks party */
Party* Player::getBearacks()
{
  return bearacks;
}

/*
 * Description: REturns the cu
 *
 * Inputs:
 * Output:
 */
/* Returns the carry weight */
double Player::getCarryWeight()
{
  if (sleuth->getInventory() != nullptr)
    return gravity * sleuth->getInventory()->getMass();

  return 0.0;
}

/*
 * Description: Returns the gravity the player is currently experiencing.
 *
 * Inputs: none
 * Output: uint32_t - the gravity the player is experiencing
 */
uint32_t Player::getGravity()
{
  return gravity;
}

/*
 * Description: Returns the play time structure of hours, minutes, and
 *              milliseconds of the player.
 *
 * Inputs: none
 * Output: TimeStore - the time storage structure
 */
TimeStore Player::getPlayTime()
{
  return play_time;
}

/*
 * Description: Returns the play time string formatted in Days:Hours:Minutes.
 *
 * Inputs: none
 * Output: std::string - the string output of time for printing, etc
 */
std::string Player::getPlayTimeStr()
{
  int day_ref = 0;
  int hour_ref = 0;
  std::string time_str = "";

  /* Determine days */
  if(play_time.hours >= 24)
  {
    day_ref = (play_time.hours / 24);
    hour_ref = (play_time.hours % 24);
  }
  else
  {
    hour_ref = play_time.hours;
  }

  /* Format */
  if(day_ref > 0)
    time_str += std::to_string(day_ref) + "d:";
  time_str += std::to_string(hour_ref) + "h:";
  time_str += std::to_string(play_time.minutes) + "m";
  //time_str += ":" + std::to_string(play_time.milliseconds) + "ms";

  return time_str;
}

/*
 * Description: Returns the pointer to the sleuth party.
 *
 * Inputs: none
 * Output: Party* - pointer to the sleuth party.
 */
Party* Player::getSleuth()
{
  return sleuth;
}

/*
 * Description: Returns the number of tile steps the player has executed on all
 *              maps.
 *
 * Inputs: none
 * Output: uint32_t - the number of tile steps
 */
uint32_t Player::getSteps()
{
  return steps;
}

/*
 * Description: Loads the data from file associated with the player.
 *
 * Inputs: XmlData data - the xml data structure
 *         int index - the element reference index
 *         SDL_Renderer* renderer - the rendering engine
 *         std::string base_path - the base path for file handling
 * Output: bool - true if load was successful
 */
bool Player::loadData(XmlData data, int index, SDL_Renderer* renderer,
                      std::string base_path)
{
  bool success = true;

  /* ---- BEARACKS PARTY ---- */
  if(data.getElement(index) == "bearacks")
  {
    if(bearacks != nullptr)
      success &= bearacks->loadData(data, index + 1, renderer, base_path);
    else
      success = false;
  }
  /* ---- CREDITS ---- */
  else if(data.getElement(index) == "credits")
  {
    success &= setCredits(data.getDataInteger(&success));
  }
  /* ---- GRAVITY ---- */
  else if(data.getElement(index) == "gravity")
  {
    success &= setGravity(data.getDataInteger(&success));
  }
  /* ---- PLAY TIME ---- */
  else if(data.getElement(index) == "playtime")
  {
    if(data.getElement(index + 1) == "hours")
    {
      setPlayTime(data.getDataInteger(&success), play_time.minutes,
                  play_time.milliseconds);
    }
    else if(data.getElement(index + 1) == "minutes")
    {
      setPlayTime(play_time.hours, data.getDataInteger(&success),
                  play_time.milliseconds);
    }
    else if(data.getElement(index + 1) == "milliseconds")
    {
      setPlayTime(play_time.hours, play_time.minutes,
                  data.getDataInteger(&success));
    }
  }
  /* ---- SLEUTH PARTY ---- */
  else if(data.getElement(index) == "sleuth")
  {
    if(sleuth != nullptr)
      success &= sleuth->loadData(data, index + 1, renderer, base_path);
    else
      success = false;
  }
  /* ---- STEPS ---- */
  else if(data.getElement(index) == "steps")
  {
    setSteps(data.getDataInteger(&success));
  }

  return success;
}

/*
 * Description: Prints out the information about the player
 *
 * Inputs:
 * Output:
 */
void Player::print()
{
  //TODO: Print function [01-18-14]
}

/*
 * Description: Removes a bearacks member by index by calling the Paty's
 *              remove function.
 *
 * Inputs: uint32_t index the index of the bearacks member to be removed.
 * Output: bool - true if the removal occurs.
 */
bool Player::removeBearacksMember(const uint32_t &index)
{
  return bearacks->removeMember(index);
}

/*
 * Description: Removes a bearacks member by name by calling Party's remove
 *              function.
 *
 * Inputs: const std::string name - the name of the bearacks member to be rem.
 * Output: bool - true if the member was removed.
 */
bool Player::removeBearacksMember(const std::string &name)
{
  return bearacks->removeMember(name);
}

/*
 * Description: Attempts to remove an amount of credits to the player's total.
 *
 * Inputs: uint32_t value - the number of credits to remove
 * Output: bool - true if credits removed
 */
bool Player::removeCredits(const uint32_t &value)
{
  bool removed = false;

  if(value <= credits)
  {
    credits -= value;
    removed = true;
  }

  return removed;
}

/*
 * Description: Removes a sleuth member by index by calling its party's removal
 *              function.
 *
 * Inputs: uint32_t index - the index of sleuth member to be removed.
 * Output: bool - true if the member was removed successfully.
 */
bool Player::removeSleuthMember(const uint32_t &index)
{
  return sleuth->removeMember(index);
}

/*
 * Description: Removes a sleuth member by name by calling Party's remove
 *              function.
 *
 * Inputs: const std::string name - the name of the sleuth member to remo.
 * Output: bool - true if the member was removed.
 */
bool Player::removeSleuthMember(const std::string &name)
{
  return sleuth->removeMember(name);
}

/*
 * Description: Resets the play time of the player back to 0 on all fronts
 *
 * Inputs: none
 * Output: none
 */
void Player::resetPlayTime()
{
  play_time = {0, 0, 0};
}

/*
 * Description: Saves the data of this player to the file handler pointer.
 *
 * Inputs: FileHandler* fh - the saving file handler
 * Output: bool - true if successful
 */
bool Player::saveData(FileHandler* fh)
{
  if(fh != nullptr)
  {
    fh->writeXmlElement("player");

    /* Write class data */
    fh->writeXmlData("credits", credits);
    if(gravity != kDEFAULT_GRAVITY)
      fh->writeXmlData("gravity", gravity);
    fh->writeXmlElement("playtime");
    fh->writeXmlData("hours", play_time.hours);
    fh->writeXmlData("minutes", play_time.minutes);
    fh->writeXmlData("milliseconds", play_time.milliseconds);
    fh->writeXmlElementEnd();
    fh->writeXmlData("steps", steps);

    /* Write parties */
    if(sleuth != nullptr)
      sleuth->saveData(fh, "sleuth");
    if(bearacks != nullptr)
      bearacks->saveData(fh, "bearacks");

    fh->writeXmlElementEnd();
    return true;
  }
  return false;
}

/*
 * Description: Assigns a pointer to the Bearacks party.
 *
 * Inputs: Party* pointer to the bearacks party.
 * Output: none
 */
void Player::setBearacks(Party* const new_bearacks)
{
  bearacks = new_bearacks;
}

/*
 * Description: Assigns the amount of money the player has.
 *
 * Inputs: uint32_t value - the amoutn of credtis the player has.
 * Output: bool - true if the credit assignment was kosher.
 */
bool Player::setCredits(const uint32_t &value)
{
  if (value < kMAX_CREDITS)
  {
    credits = value;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a new value to gravity for the gravity that the player
 *              is experiencing.
 *
 * Inputs: uint32_t new_value - new value for gravity for the player.
 * Output: bool - true if the assignment was successful.
 */
bool Player::setGravity(const uint32_t &new_value)
{
  gravity = new_value;

  return true;
}

/*
 * Description: Sets the total time played by the Player. This overwrites all
 *              existing information.
 *
 * Inputs: uint32_t hours - the hours played
 *         uint32_t minutes - the minutes played
 *         uint32_t milliseconds - the milliseconds played
 * Output: none
 */
void Player::setPlayTime(const uint32_t &hours, const uint32_t &minutes,
                         const uint32_t &milliseconds)
{
  play_time.hours = hours;
  play_time.minutes = minutes;
  play_time.milliseconds = milliseconds;

  /* Conform */
  addPlayTime(0);
}

/*
 * Description: Assigns the Sleuth party.
 *
 * Inputs: Party* new_slueth - new sleuth party.
 * Output: none
 */
void Player::setSleuth(Party* const new_sleuth)
{
  sleuth = new_sleuth;
}

/*
 * Description: Sets the number of steps in tiles the player has traversed.
 *
 * Inputs: const uint32_t &new_value - the complete count of steps for player
 * Output: none
 */
void Player::setSteps(const uint32_t &new_value)
{
  steps = new_value;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the default value for gravity.
 *
 * Inputs: none
 * Output: uint32_t - the default level of gravity.
 */
uint32_t Player::getDefaultGravity()
{
  return kDEFAULT_GRAVITY;
}

/*
 * Description: Returns the starting credits for the Player.
 *
 * Inputs: none.
 * Output: uint32_t - the starting amount of credits for the player
 */
uint32_t Player::getStartingCredits()
{
  return kSTARTING_CREDITS;
}

/*
 * Description: Returns the maximum possible credits that a Player may have.
 *
 * Inputs: none
 * Output: uint32_t - the maximum possible value for credits
 */
uint32_t Player::getMaxCredits()
{
  return kMAX_CREDITS;
}
