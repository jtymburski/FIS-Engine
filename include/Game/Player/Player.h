/*******************************************************************************
* Class Name: Player [Declaration]
* Date Created: January 18th, 2014
* Inheritance: None
* Description: A Player contains the information of the Parties that are usable
*              by the player and represents pertinent information to the
*              progression of the game.
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* - Print function [01-18-14]
*******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "Game/Player/Party.h"

/* Struct to indicate time played by player */
struct TimeStore
{
  uint32_t hours;
  uint32_t minutes;
  uint32_t milliseconds;
};

/* Main player class */
class Player
{
public:
  /* General player construction */
  Player(Party* sleuth = nullptr, Party* bearacks = nullptr);

private:
  /* The slueth and bearacks parties. The sleuth is the player's main party,
   * while the bearacks is the reserve party */
  Party* sleuth;
  Party* bearacks;

  /* Current value of credits */
  uint32_t credits;

  /* Current gravity experienced by player */
  uint32_t gravity;

  /* Time played */
  TimeStore play_time;

  /* Step count */
  uint32_t steps;

  /* ------------ Constants --------------- */
  static const uint32_t kDEFAULT_GRAVITY;
  static const uint32_t kSTARTING_CREDITS;
  static const uint32_t kMAX_CREDITS;

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Attempt to add an amount of credits to the current total */
  bool addCredits(const uint32_t &value);

  /* Attempts to add a learned skill, assuming it's valid */
  bool addLearnedSkill(std::string party_type, Skill* skill,
                       const uint32_t &person_index = 0,
                       const uint32_t &req_level = 200);

  /* Add play time to the total */
  void addPlayTime(const uint32_t &milliseconds);

  /* Adds steps onto the total executed - triggered on save or changed map */
  void addSteps(const uint32_t &value);

  /* Returns the current credit value */
  uint32_t getCredits();

  /* Returns the bearacks Party */
  Party* getBearacks();

  /* Returns the carry weight of the player */
  double getCarryWeight();

  /* Returns the gravity being experiencd by the player */
  uint32_t getGravity();

  /* Returns the play time of the player */
  TimeStore getPlayTime();
  std::string getPlayTimeStr();

  /* Gets the sleuth party */
  Party* getSleuth();

  /* Returns the number of steps this player has undertaken */
  uint32_t getSteps();

  /* Load data from file */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer,
                std::string base_path);

  /* Methods for printing ou the info of the player */
  void print();

  /* Removes a bearacks member by index by calling Party's remove function */
  bool removeBearacksMember(const uint32_t &index);

  /* Removes a bearacks member by name by calling Party's remove function */
  bool removeBearacksMember(const std::string &name);

  /* Attempt to remove an amount of credits from the current total */
  bool removeCredits(const uint32_t &value);

  /* Removes a sleuth member by index by calling Party's remove function */
  bool removeSleuthMember(const uint32_t &index);

  /* Removes a sleuth member by name by calling Party's remove function */
  bool removeSleuthMember(const std::string &name);

  /* Resets the play time of the player */
  void resetPlayTime();

  /* Save data to file */
  bool saveData(FileHandler* fh);

  /* Sets the bearcks party WARNING: Possibly a dangerous function */
  void setBearacks(Party* const new_bearacks = nullptr);

  /* Sets the amount of money the player has */
  bool setCredits(const uint32_t &value);

  /* Assigns the gravity the player is experiencing */
  bool setGravity(const uint32_t &new_value);

  /* Sets the TOTAL time played of the player */
  void setPlayTime(const uint32_t &hours = 0, const uint32_t &minutes = 0,
                   const uint32_t &milliseconds = 0);

  /* Sets the sleuth party WARNING: DANGEROUS FUNCTION */
  void setSleuth(Party* const new_sleuth);

  /* Assigns the steps the player has executed */
  void setSteps(const uint32_t &new_value);

  /*===================== PUBLIC STATIC FUNCTIONS ============================*/
public:
  /* Return the defaul gravity */
  static uint32_t getDefaultGravity();

  /* Return the starting credits of the player */
  static uint32_t getStartingCredits();

  /* Return the maximum credits */
  static uint32_t getMaxCredits();
};

#endif //PLAYER_H
