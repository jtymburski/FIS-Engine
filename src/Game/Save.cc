/*******************************************************************************
* Class Name: Save [Implementation]
* Date Created: May 22, 2016
* Inheritance: None
* Description: Contains and processes save data for use in representation and
*              compilation for the loading and saving process
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
      player_name{"Player"},
      player_sex{Sex::FEMALE},
      snapshot_path{""},
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
  return std::to_string(date_year) + "-" + formattedNumber(date_month) + "-" +
         formattedNumber(date_day);
}

/* Formatting function for format a play time (ex. 00:06:51) */
std::string Save::formattedPlayTime()
{
  return formattedNumber(time_hrs) + ":" + formattedNumber(time_min) + ":" +
         formattedNumber(time_sec);
}

/* Formatting function for formatting the date time */
std::string Save::formattedTime()
{
  return formattedNumber(date_hour) + ":" + formattedNumber(date_minute);
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
    snapshot_path = "";
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

/* Returns the save snapshot path */
std::string Save::getSnapshotPath()
{
  return snapshot_path;
}

/* Returns the time data elements */
uint32_t Save::getTimeHours()
{
  return time_hrs;
}

/* Returns the time data elements */
uint32_t Save::getTimeMinutes()
{
  return time_min;
}

/* Returns the time data elements */
uint32_t Save::getTimeSeconds()
{
  return time_sec;
}

/* Prints the class data - primarily for testing */
void Save::print()
{
  std::cout << "Slot " << id << " --" << std::endl;

  /* Is empty? */
  if(getFlag(SaveState::EMPTY))
  {
    std::cout << " Slot is empty" << std::endl;
  }
  /* Not empty */
  else
  {
    std::cout << " Date: " << formattedDate() << " " << formattedTime()
              << std::endl;
    std::cout << " Play Time: " << formattedPlayTime() << std::endl
              << std::endl;

    std::cout << " Image Path: " << snapshot_path << std::endl;
    std::cout << " Map Name:  " << map_name << std::endl
              << std::endl;

    std::cout << " Credits: " << count_credits << std::endl;
    std::cout << " Level: " << count_level << std::endl;
    std::cout << " Name: " << player_name << std::endl;
    std::cout << " Sex: " << Helpers::sexToStr(player_sex) << std::endl;
    std::cout << " Steps: " << count_steps << std::endl;
  }

  std::cout << "End of Slot --" << std::endl;
}

/* Return a Frame to render for the Save file */
SDL_Texture* Save::createRenderFrame(SDL_Renderer* renderer)
{
  if(config)
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

    t_title.setText(renderer, "SLOT " + std::to_string(id) + " ", kCOLOR_TEXT);

    auto height = Map::kSNAPSHOT_H / 4;
    auto width = Map::kSNAPSHOT_W / 4;
    auto save_height = location.height;
    auto save_width = location.width;

    auto gap = (int32_t)(std::floor((location.height - height) / 2));

    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, save_width, save_height);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    Coordinate current = {location.point.x + gap, location.point.y + gap};

    // TODO [05-22-16] Assign location priorities
    location.render(renderer);

    /* Render a blank save title if this save is an empty one */
    if(getFlag(SaveState::EMPTY))
    {
      Text t_empty(font_normal);
      t_empty.setText(renderer, "- Empty", kCOLOR_TEXT);

      Box blank_tile(current, width, height);
      blank_tile.color_border = {125, 125, 125, 255};
      blank_tile.render(renderer);

      current.x += blank_tile.height + width + gap;
      current.y = location.point.y + location.height / 2;

      /* Render the slot empty text */
      t_title.render(renderer, current.x, current.y);
      current.x += t_title.getWidth();
      t_empty.render(renderer, current.x, current.y);
    }
    /* If the save data is set, render a normal save title */
    else
    {
      t_date.setText(renderer, formattedDate(), kCOLOR_TEXT);
      t_date_time.setText(renderer, formattedTime(), kCOLOR_TEXT);
      t_play_title.setText(renderer, "PLAY TIME", kCOLOR_TEXT);
      t_play_time.setText(renderer, formattedPlayTime(), kCOLOR_TEXT);
      t_steps_title.setText(renderer, "STEP COUNT", kCOLOR_TEXT);
      t_steps.setText(renderer, formattedSteps(), kCOLOR_TEXT);
      t_level_title.setText(renderer, "PLAYER LEVEL", kCOLOR_TEXT);
      t_level.setText(renderer, std::to_string(count_level), kCOLOR_TEXT);
      t_credits_title.setText(renderer, "CREDIT COUNT", kCOLOR_TEXT);
      t_credits.setText(renderer, formattedCredits(), kCOLOR_TEXT);
      t_map_name.setText(renderer, map_name, kCOLOR_TEXT);

      /* Generate the map thumbnail */
      auto map_frame = new Frame(snapshot_path, renderer);

      if(map_frame)
      {
        map_frame->render(renderer, current.x, current.y);
        current.x += map_frame->getWidth() + gap;

        delete map_frame;
        map_frame = nullptr;


      }

      /* Render the first column of text - Slot Title, Date, Time, Map Name */
      t_title.render(renderer, current.x, current.y);
      current.y += 2 * t_title.getHeight();

      t_date.render(renderer, current.x, current.y);
      current.y += t_date.getHeight() + gap;

      t_date_time.render(renderer, current.x, current.y);
      current.y =
          location.point.y + location.height - t_map_name.getHeight() * 2;

      t_map_name.render(renderer, current.x, current.y);

      /* Render the second column of text - Play Time / Step Count */
      current.x = location.point.x + 5 * location.width / 10;
      current.y = location.point.y + gap;

      t_play_title.render(renderer, current.x, current.y);
      current.y += t_play_title.getHeight() + gap / 4;

      t_play_time.render(renderer, current.x, current.y);
      current.y += t_play_time.getHeight() + gap / 2;

      t_steps_title.render(renderer, current.x, current.y);
      current.y += t_steps_title.getHeight() + gap / 4;

      t_steps.render(renderer, current.x, current.y);

      /* Render the third column of text - Player Level and Credit Count */
      current.x = location.point.x + 7 * location.width / 10;
      current.y = location.point.y + gap;

      t_level_title.render(renderer, current.x, current.y);
      current.y += t_level_title.getHeight() + gap / 4;

      t_level.render(renderer, current.x, current.y);
      current.y += t_level.getHeight() + gap / 2;

      t_credits_title.render(renderer, current.x, current.y);
      current.y += t_credits_title.getHeight() + gap / 4;

      t_credits.render(renderer, current.x, current.y);

      /* Render the cross bars */
    }

    SDL_SetRenderTarget(renderer, nullptr);

    return texture;
  }

  return nullptr;
}

/* Assigns configuration to the save file */
bool Save::setConfig(Options* config)
{
  this->config = config;

  return (this->config != nullptr);
}

/* Assign game count information */
void Save::setCountLevel(uint32_t count_level)
{
  this->count_level = count_level;
  setFlag(SaveState::EMPTY, false);
}

/* Assign game count information */
void Save::setCountSteps(uint32_t count_steps)
{
  this->count_steps = count_steps;
  setFlag(SaveState::EMPTY, false);
}

/* Assign game count information */
void Save::setCountCredits(uint32_t count_credits)
{
  this->count_credits = count_credits;
  setFlag(SaveState::EMPTY, false);
}

/* Assign custom player information */
void Save::setCustomPlayerName(std::string player_name)
{
  this->player_name = player_name;
  setFlag(SaveState::EMPTY, false);
}

void Save::setCustomPlayerSex(Sex player_sex)
{
  this->player_sex = player_sex;
  setFlag(SaveState::EMPTY, false);
}

/* Assign the date data to the object */
void Save::setDate(uint32_t date_year, uint32_t date_month, uint32_t date_day,
                   uint32_t date_hour, uint32_t date_minute)
{
  this->date_year = date_year;
  this->date_month = date_month;
  this->date_day = date_day;
  this->date_hour = date_hour;
  this->date_minute = date_minute;
  setFlag(SaveState::EMPTY, false);
}

/* Assign a MenuState flag a value */
void Save::setFlag(SaveState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

/* Assign a map name to the save */
void Save::setMapName(std::string map_name)
{
  this->map_name = map_name;
  setFlag(SaveState::EMPTY, false);
}

/* Sets the save snapshot path */
void Save::setSnapshotPath(std::string path)
{
  this->snapshot_path = path;
  setFlag(SaveState::EMPTY, false);
}

/* Assign time data to the object */
void Save::setTime(uint32_t time_hrs, uint32_t time_min, uint32_t time_sec)
{
  this->time_hrs = time_hrs;
  this->time_min = time_min;
  this->time_sec = time_sec;
  setFlag(SaveState::EMPTY, false);
}

/*=============================================================================
* PUBLIC STATIC FUNCTIONS
*============================================================================*/
