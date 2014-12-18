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
 *  - Covnert flags to ENUM_FLAGS set for easy addition later?
 ******************************************************************************/
#include "Options.h"

/* Constant Implementation - see header file for descriptions */
const std::string Options::kFONTS[] = {"fonts/colab_light.otf", 
                                       "fonts/crimes.ttf"};
const uint8_t  Options::kNUM_FONTS = 2;
const uint8_t  Options::kNUM_RESOLUTIONS = 4;
const uint16_t Options::kRESOLUTIONS_X[] = {1216, 1366, 1920, 2560};
const uint16_t Options::kRESOLUTIONS_Y[] = {704, 768, 1080, 1080};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* Constructor function */
Options::Options(std::string base_path)
{
  this->base_path = base_path;
  setAllToDefault();
}

Options::Options(const Options &source)
{
  copySelf(source);
}

/* Destructor function */
Options::~Options()
{
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Options::copySelf(const Options &source)
{
  /* Battle Options */
  ailment_update_state = source.ailment_update_state;
  battle_hud_state = source.ailment_update_state;
  base_path = source.base_path;
  battle_mode = source.battle_mode;

  resolution_x  = source.resolution_x;
  resolution_y  = source.resolution_y;
  vsync_enabled = source.vsync_enabled;
}

void Options::setAllToDefault()
{
  /* Battle Options */
  setAilmentUpdateState(BattleOptions::FOREST_WALK);
  setBattleHudState(BattleOptions::FOREST_WALK);
  setBattleMode(BattleMode::TEXT);

  /* Map Options */
  setAutoRun(false);

  /* Rendering Options */
  setFont(0, true);
  setLinearFiltering(false);
  setScreenResolution(0);
  setVsync(true);
}

void Options::setAutoRun(bool auto_run)
{
  this->auto_run = auto_run;
}

void Options::setAilmentUpdateState(BattleOptions new_state)
{
  ailment_update_state = new_state;
}

void Options::setBattleHudState(BattleOptions new_state)
{
  battle_hud_state = new_state;
}

void Options::setBattleMode(BattleMode new_state)
{
  battle_mode = new_state;
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
  this->linear_filtering = linear_filtering;
  
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

// Have you heard of der flags??
void Options::setVsync(bool enabled)
{
  vsync_enabled = enabled;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

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

BattleOptions Options::getAilmentUpdateState()
{
  return ailment_update_state;
}

std::string Options::getBasePath()
{
  return base_path;
}

BattleOptions Options::getBattleHudState()
{
  return battle_hud_state;
}

BattleMode Options::getBattleMode()
{
  return battle_mode;
}

/* Returns the path to the font to use throughout the application */
std::string Options::getFont()
{
  return kFONTS[font];
}
  
uint16_t Options::getScreenHeight()
{
  return kRESOLUTIONS_Y[resolution_y];
}

uint16_t Options::getScreenWidth()
{
  return kRESOLUTIONS_X[resolution_x];
}

/* Returns if the player is instructed to always run */
bool Options::isAutoRun()
{
  return auto_run;
}

/* Returns if linear filtering mode is enabled */
bool Options::isLinearFilteringEnabled()
{
  return linear_filtering;
}
  
bool Options::isVsyncEnabled()
{
  return vsync_enabled;
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
