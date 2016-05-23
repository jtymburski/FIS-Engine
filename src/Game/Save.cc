/*******************************************************************************
* Class Name: Save [Implementation]
* Date Created: May 22, 2016
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/Save.h"

/*=============================================================================
* CONSTANTS
*============================================================================*/
const int32_t Save::kUNSET_ID = 1;
const SDL_Color Save::kCOLOR_TEXT{255, 255, 255, 255};

/*=============================================================================
* CONSTRUCTORS / DESTRUCTORS
*============================================================================*/

/*
* Description:
*
* Inputs:
*/
Save::Save()
    : count_level{0},
      count_steps{0},
      count_credits{0},
      date_year{0},
      date_month{0},
      date_day{0},
      date_hour{0},
      date_minute{0},
      flags{static_cast<SaveState>(0)},
      id{kUNSET_ID},
      map_name{""},
      time_hrs{0},
      time_min{0},
      time_sec{0},
      location{Box()}
{
  setFlag(SaveState::EMPTY);
}

/*
* Description:
*
* Inputs: int32_t id - the Save Slot ID for this Save
*         Options* config - running configuration
*/
Save::Save(int32_t id, Options* config) : Save()
{
  this->id = id;
  this->config = config;
}

/*=============================================================================
* PRIVATE FUNCTIONS
*============================================================================*/

/*
* Description:
*
* Inputs:
* Output:
*/
std::string Save::formattedNumber(uint32_t n)
{
  if(n < 1)
    return "00";
  if(n < 10)
    return "0" + std::to_string(n);

  return std::to_string(n);
}

/* Formatting function for format a date (ex. 2016-03-06) */
std::string Save::formattedDate()
{
  return std::to_string(date_year) + formattedNumber(date_month) +
         formattedNumber(date_day);
}

/* Formatting function for format a play time (ex. 00:06:51) */
std::string Save::formattedPlayTime()
{
  return formattedNumber(date_hour) + formattedNumber(date_minute);
}

/* Formatting function for formatting the date time */
std::string Save::formattedTime()
{
  return formattedNumber(time_hrs) + formattedNumber(time_min) +
         formattedNumber(time_sec);
}

/* Formatting function for formatting the step count */
std::string Save::formattedSteps()
{
  return Helpers::formatUInt(count_steps, " ");
}

/* Formatting function for formatting the credit count */
std::string Save::formattedCredits()
{
  return Helpers::formatUInt(count_credits, " ");
}

/*=============================================================================
* PUBLIC FUNCTIONS
*============================================================================*/

/* Clears the save data */
bool Save::clear()
{
  if(id != kUNSET_ID && !getFlag(SaveState::EMPTY))
  {
    count_level = 0;
    count_steps = 0;
    count_credits = 0;
    date_year = 0;
    date_month = 0;
    date_day = 0;
    date_hour = 0;
    date_minute = 0;
    setFlag(SaveState::EMPTY, true);
    map_name = "";
    time_hrs = 0;
    time_min = 0;
    time_sec = 0;

    return true;
  }

  return false;
}

/* Returns the value of a MenuState flag */
bool Save::getFlag(const SaveState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

bool Save::render(SDL_Renderer* renderer)
{
  if(renderer && config)
  {
    auto font_title = config->getFontTTF(FontName::M_VALUE);
    auto font_header = config->getFontTTF(FontName::M_ITEM_HEADER);
    auto font_normal = config->getFontTTF(FontName::M_SUBHEADER);

    Text t_title(font_title);
    Text t_date(font_normal);
    Text t_date_time(font_normal);
    Text t_play_title(font_header);
    Text t_play_time(font_normal);
    Text t_steps_title(font_header);
    Text t_steps(font_normal);
    Text t_level_title(font_header);
    Text t_level(font_normal);
    Text t_credits_title(font_header);
    Text t_credits(font_normal);
    Text t_map_name(font_normal);

    t_title.setText(renderer, "SLOT " + std::to_string(id), kCOLOR_TEXT);

    //TODO [05-22-16] Assign location priorities
    location.render(renderer);

    /* Render a blank save title if this save is an empty one */
    if(getFlag(SaveState::EMPTY))
    {
      Text t_empty(font_normal);
      t_empty.setText(renderer, "- Empty", kCOLOR_TEXT);

      //TODO [05-22-16] Render blank title box
    }
    /* If the save data is set, render a normal save title */
    else
    {
      /* Generate the map thumbnail */
      //Frame* map_frame = nullptr;

      //TODO [05-22-16]
      //Check if the file exists for the frame
      //Create the frame with the given file
      //Render the frame at the location coordinate

      t_date.setText(renderer, formattedDate(), kCOLOR_TEXT);
      t_date_time.setText(renderer, formattedTime(), kCOLOR_TEXT);
      t_play_title.setText(renderer, "PLAY TIME", kCOLOR_TEXT);
      t_play_time.setText(renderer, formattedPlayTime(), kCOLOR_TEXT);
      t_steps_title.setText(renderer, "STEP COUNT", kCOLOR_TEXT);
      t_steps.setText(renderer, formattedSteps(), kCOLOR_TEXT);
      t_credits_title.setText(renderer, "CREDIT COUNT", kCOLOR_TEXT);
      t_credits.setText(renderer, formattedCredits(), kCOLOR_TEXT);
      t_map_name.setText(renderer, map_name, kCOLOR_TEXT);

      //TOOD [05-22-16] Render text values

      //TODO [05-22-16] Render cross bars
    }

    return true;
  }

  return false;
}

void Save::setCountLevel(uint32_t count_level)
{
  this->count_level = count_level;
}

void Save::setCountSteps(uint32_t count_steps)
{
  this->count_steps = count_steps;
}

void Save::setCountCredits(uint32_t count_credits)
{
  this->count_credits = count_credits;
}

bool Save::setConfig(Options* config)
{
  this->config = config;

  return (this->config != nullptr);
}

void Save::setDate(uint32_t date_year, uint32_t date_month, uint32_t date_day,
                   uint32_t date_hour, uint32_t date_minute)
{
  this->date_year = date_year;
  this->date_month = date_month;
  this->date_day = date_day;
  this->date_hour = date_hour;
  this->date_minute = date_minute;
}

/* Assign a MenuState flag a value */
void Save::setFlag(SaveState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

void Save::setMapName(std::string map_name)
{
  this->map_name = map_name;
}

void Save::setTime(uint32_t time_hrs, uint32_t time_min, uint32_t time_sec)
{
  this->time_hrs = time_hrs;
  this->time_min = time_min;
  this->time_sec = time_sec;
}

/*=============================================================================
* PUBLIC STATIC FUNCTIONS
*============================================================================*/
