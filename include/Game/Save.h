/*******************************************************************************
* Class Name: Save [Header]
* Date Created: May 22, 2016
* Inheritance: None
* Description: Contains and processes save data for use in representation and
*              compilation for the loading and saving process
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#ifndef SAVE_H
#define SAVE_H

#include "Game/Map/Map.h"
#include "Box.h"
#include "EnumFlags.h"
#include "Options.h"

ENUM_FLAGS(SaveState)
enum class SaveState
{
  EMPTY = 1 << 0
};

class Save
{
public:
  /* Constructs a save object */
  Save();

  /* Construct a save object */
  Save(int32_t id, Options* config = nullptr);

private:
  /* Object counts for the save file */
  uint32_t count_level;
  uint32_t count_steps;
  uint32_t count_credits;

  /* Configuration pointer for the Save object */
  Options* config;

  /* The time date data for the save file */
  uint32_t date_year;
  uint32_t date_month;
  uint32_t date_day;
  uint32_t date_hour;
  uint32_t date_minute;

  /* Flags for the current Save object */
  SaveState flags;

  /* Save file ID */
  int32_t id;

  /* Name of the map */
  std::string map_name;

  /* The save snapshot path */
  std::string snapshot_path;

  /* The play time data for the save file */
  uint32_t time_hrs;
  uint32_t time_min;
  uint32_t time_sec;

  /* ------------ Constants --------------- */
  static const SDL_Color kCOLOR_TEXT;
  static const int32_t kUNSET_ID;

public:
  /* Location, width, height of the save title */
  Box location;

/*=========================================================================
 * PRIVATE FUNCTIONS
 *========================================================================*/
private:
  /* Format a number 0 --> 00, 1 --> 01, 11  --> 11 etc. */
  std::string formattedNumber(uint32_t n);

  /* Formatting function for format a date (ex. 2016-03-06) */
  std::string formattedDate();

  /* Formatting function for format a play time (ex. 00:06:51) */
  std::string formattedPlayTime();

  /* Formatting function for formatting the date time */
  std::string formattedTime();

  /* Formatting function for formatting the step count */
  std::string formattedSteps();

  /* Formatting function for formatting the credit count */
  std::string formattedCredits();

/*=========================================================================
 * PUBLIC FUNCTIONS
 *========================================================================*/
public:
  /* Clear the data for the save file */
  bool clear();

  /* Return the value of a given SaveState flag */
  bool getFlag(const SaveState& test_flag);

  /* Returns the save snapshot path */
  std::string getSnapshotPath();

  /* Returns the time data elements */
  uint32_t getTimeHours();
  uint32_t getTimeMinutes();
  uint32_t getTimeSeconds();

  /* Prints the class data - primarily for testing */
  void print();

  /* Return the Render frame for this Save */
  Frame* createRenderFrame(SDL_Renderer* renderer);

  /* Assigns configuration to the save file */
  bool setConfig(Options* config);

  /* Assign game count information */
  void setCountLevel(uint32_t count_level);
  void setCountSteps(uint32_t count_steps);
  void setCountCredits(uint32_t count_credits);

  /* Assign the date data to the object */
  void setDate(uint32_t date_year, uint32_t date_month, uint32_t date_day,
               uint32_t date_hour, uint32_t date_minute);

  /* Set the value of a given SaveState flag */
  void setFlag(SaveState set_flags, const bool& set_value = true);

  /* Assign a map name to the save */
  void setMapName(std::string map_name);

  /* Sets the save snapshot path */
  void setSnapshotPath(std::string path);

  /* Assign time data to the object */
  void setTime(uint32_t time_hrs, uint32_t time_min, uint32_t time_sec);

/*=========================================================================
 * PUBLIC STATIC FUNCTIONS
 *========================================================================*/
public:
};

#endif // SAVE_H
