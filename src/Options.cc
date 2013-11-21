/*******************************************************************************
 * Class Name: Options
 * Date Created: Sunday, December 2, 2012
 * Inheritance: none
 * Description: This handles all options used throughout the application as well
 *              as the API for changing this. It's activated by calling the
 *              render class by the higher end classes.
 *
 * TODO: CONSTRUCTORS TO BE FINISHED
 *       Add option to load settings from file and dump to file as well.
 *
 * Options to add:
 *  - Set the text display speed in MapDialog (for the character letters)
 ******************************************************************************/
#include "Options.h"

/* Constant Implementation - see header file for descriptions */
const std::string Options::kFONTS[] = {"fonts/colab_light.otf"};
const uint8_t Options::kNUM_FONTS = 1;
const uint8_t Options::kNUM_RESOLUTIONS = 3;
const uint16_t Options::kRESOLUTIONS_X[] = {1216, 1366, 1920};
const uint16_t Options::kRESOLUTIONS_Y[] = {704, 768, 1080};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* Constructor function */
Options::Options()
{
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
  battle_mode = source.battle_mode;

  resolution_x  = source.resolution_x;
  resolution_y  = source.resolution_y;
  vsync_enabled = source.vsync_enabled;
}

void Options::setAllToDefault()
{
  /* Battle Options */
  setAilmentUpdateState(BEARWALK);
  setBattleHudState(BEARWALK);
  setBattleMode(DEBUG);

  setFont(0, true);
  setScreenResolution(0);
  setVsync(true);
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
  
void Options::setScreenResolution(uint8_t index)
{
  if(index < kNUM_RESOLUTIONS)
  {
    resolution_x = index;
    resolution_y = index;
  }
}

void Options::setVsync(bool enabled)
{
  vsync_enabled = enabled;
  
  /* Enable vertical syncing */
  if(enabled)
  {
    if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
      printf("[WARNING] Vsync not enabled.");
  }
  /* Disable vertical syncing */
  else
  {
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Options::confirmFontSetup()
{
  bool success = true;
  TTF_Font* test_font = TTF_OpenFont(kFONTS[font].c_str(), 14);
  
  /* If the font setup fails, output the error alert */
  if(test_font == NULL)
  {
    std::cout << "[ERROR] The font " << kFONTS[font] 
              << " could not be loaded. SDL_ttf error: " << TTF_GetError() 
              << "\n";
    success = false;
  }
  
  TTF_CloseFont(test_font);
  return success;
}

Options::BattleOptions Options::getAilmentUpdateState()
{
  return ailment_update_state;
}

Options::BattleOptions Options::getBattleHudState()
{
  return battle_hud_state;
}

Options::BattleMode Options::getBattleMode()
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
