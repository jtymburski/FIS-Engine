/*******************************************************************************
 * Class Name: Options
 * Date Created: Sunday, December 2, 2012
 * Inheritance: none
 * Description: This handles all options used throughout the application as well
 *              as the API for changing this. It's activated by calling the
 *              render class by the higher end classes.
 *
 * TODO: CONSTRUCTORS TO BE FINISHED
 *  - Add option to load settings from file and dump to file as well.
 *  - Some way to handle when SDL parameters are changed or anything of
 *    relevance so that the game gets the updated versions of all these
 *    parameters.
 *  - Add sound slider, for global volume. Max 100%.
 *
 * Options to add:
 *  - Set the text display speed in MapDialog (for the character letters)
 *  - [11-23-14] Battle option for alternative victory/loss conditions
 ******************************************************************************/
#include "Options.h"

/* Constant Implementation - see header file for descriptions */
const std::string Options::kFONTS[] = {"fonts/colab_light.otf",
                                       "fonts/crimes.ttf"};
const uint8_t  Options::kNUM_FONTS = 2;
const uint8_t  Options::kNUM_RESOLUTIONS = 6;
const uint16_t Options::kRESOLUTIONS_X[] = {1216, 1217, 1366, 1920, 2560, 4480};
const uint16_t Options::kRESOLUTIONS_Y[] = {704, 705, 768, 1080, 1080, 4480};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* Constructor function */
Options::Options(std::string base_path)
{
  this->base_path = base_path;
  setAllToDefault();

  font_data = new Fonts(this);
}

Options::Options(const Options &source)
{
  copySelf(source);
}

/* Destructor function */
Options::~Options()
{
  if(font_data)
    delete font_data;
  font_data = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Options::copySelf(const Options &source)
{
  /* Battle Options */
  base_path            = source.base_path;
  resolution_x  = source.resolution_x;
  resolution_y  = source.resolution_y;
  flags = source.flags;
}

void Options::setAllToDefault()
{
  audio_level = 50;
  music_level = 50;

  /* Flags */
  setLinearFiltering(false);
  setFlag(OptionState::VSYNC, true);
  setFlag(OptionState::FULLSCREEN, false);
  setFlag(OptionState::AUTO_RUN, false);
  setFlag(OptionState::BATTLE_ANIMATIONS, true);
  setFlag(OptionState::GUI_ENABLED, true);
  setFlag(OptionState::AUDIO_ENABLED, true);

  /* Rendering Options */
  setFont(0, true);
  setScreenResolution(0);
}

bool Options::setAudioLevel(int32_t new_level)
{
  if (new_level <= 128 && new_level >= 0)
  {
    audio_level = new_level;

    return true;
  }

  return false;
}

bool Options::setMusicLevel(int32_t new_level)
{
  if (new_level <= 128 && new_level >= 0)
  {
    music_level = new_level;

    return true;
  }

  return false;
}

void Options::setFlag(OptionState flag, bool set_value)
{
  (set_value) ? (flags |= flag): (flags &= ~flag);
}

/* Sets the chosen font */
void Options::setFont(uint8_t index, bool first_call)
{
  bool success = true;
  if(index < kNUM_FONTS)
  {
    int old_index = font;
    font = index;

    if(!first_call)
      success = confirmFontSetup();

    if(!success)
      font = old_index;
  }
}

void Options::setLinearFiltering(bool linear_filtering)
{
  setFlag(OptionState::LINEAR_FILTERING, true);

  if(linear_filtering)
  {
    /* Set texture filtering to linear */
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
      std::cerr << "[WARNING] Unable to enable linear filtering." << std::endl;
  }
  else
  {
    /* Set texture filtering to nearest pixel */
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
      std::cerr << "[WARNING] Unable to enable nearest pixel filtering."
                << std::endl;
  }
}

void Options::setScreenResolution(uint8_t index)
{
  if(index < kNUM_RESOLUTIONS)
  {
    resolution_x = index;
    resolution_y = index;
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Returns if the player is instructed to always run */
bool Options::isAutoRun()
{
  return getFlag(OptionState::AUTO_RUN);
}

/* Returns if the game is full screen mode */
bool Options::isFullScreen()
{
  return getFlag(OptionState::FULLSCREEN);
}

/* Returns if linear filtering mode is enabled */
bool Options::isLinearFilteringEnabled()
{
  return getFlag(OptionState::LINEAR_FILTERING);
}

bool Options::isAudioEnabled()
{
  return getFlag(OptionState::AUDIO_ENABLED);
}

bool Options::isVsyncEnabled()
{
  return getFlag(OptionState::VSYNC);
}

int32_t Options::getAudioLevel()
{
  return audio_level;
}

int32_t Options::getMusicLevel()
{
  return music_level;
}

bool Options::confirmFontSetup()
{
  bool success = true;
  std::string path = base_path + kFONTS[font];
  TTF_Font* test_font = TTF_OpenFont(path.c_str(), 14);

  /* If the font setup fails, output the error alert */
  if(test_font == NULL)
  {
    std::cerr << "[ERROR] The font " << kFONTS[font]
              << " could not be loaded. SDL_ttf error: " << TTF_GetError()
              << std::endl;
    success = false;
  }

  TTF_CloseFont(test_font);
  return success;
}

std::string Options::getBasePath()
{
  return base_path;
}

// Get fn for OptionState flags
bool Options::getFlag(const OptionState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/* Returns the path to the font to use throughout the application */
std::string Options::getFont()
{
  return kFONTS[font];
}

std::string Options::getFont(uint8_t index)
{
  if(index < kNUM_FONTS)
    return kFONTS[index];
  return "";
}

uint16_t Options::getScreenHeight()
{
  return kRESOLUTIONS_Y[resolution_y];
}

uint16_t Options::getScreenWidth()
{
  return kRESOLUTIONS_X[resolution_x];
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

Options& Options::operator= (const Options &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
