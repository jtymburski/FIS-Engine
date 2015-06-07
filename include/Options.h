/*******************************************************************************
 * Class Name: Options
 * Date Created: Sunday, December 2, 2012
 * Inheritance: none
 * Description: This handles all options used throughout the application as well
 *              as the API for changing this. It's activated by calling the
 *              render class by the higher end classes.
 *  
 * TODO: See cc file
 ******************************************************************************/
#ifndef OPTIONS_H
#define OPTIONS_H

#include "EnumFlags.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

ENUM_FLAGS(OptionState)
enum class OptionState
{
  LINEAR_FILTERING  = 1 << 0,
  VSYNC             = 1 << 1,
  FULLSCREEN        = 1 << 2,
  AUTO_RUN          = 1 << 3,
  BATTLE_ANIMATIONS = 1 << 4,
  GUI_ENABLED       = 1 << 5,
  AUDIO_ENABLED     = 1 << 6
};

enum class BattleOptions
{
  FOREST_WALK,
  BEARLY_DIFFICULT,
  GRIZZLY
};

class Options
{
public:
  /* Constructor(s) */
  Options(std::string base_path = "");

  /* Copy constructor */
  Options(const Options &source);

  /* Destructor */
  ~Options();

private:
  /* Amount of information to display during ailment updates */
  BattleOptions ailment_update_state;

  /* Sound levels */
  int32_t audio_level;
  int32_t music_level;

  /* Auto run, in the map */
  //bool auto_run;

  /* The base string path for accessing file information */
  std::string base_path;
  
  /* Amount of information to display on the Battle HUD */
  BattleOptions battle_hud_state;

  /* The index for the selected font */
  uint8_t font;
  
  /* If the linear filtering is enabled */
  //bool linear_filtering;

  /* Enum flags option state set */
  OptionState flags;
  
  /* Resolution of the screen currently being used */
  uint8_t resolution_x;
  uint8_t resolution_y;

  /* Vertical refresh syncing enable */
  //bool vsync_enabled;

  /* Horizontal resolution options */
  //QVector<int> x_options; 

  /* Vertical resolution options */
  //QVector<int> y_options; 

  /* Primary options menu */
  //QList<QString> main_options;

  /* Secondary options menu */
  /* Notes: other options to be added as need (audio, animations, font size) */
  //QList<QString> secondary_Options;

  /*------------------- Constants -----------------------*/
  const static std::string kFONTS[]; /* A list of all the fonts available */
  const static uint8_t kNUM_FONTS; /* The number of fonts available */
  const static uint8_t kNUM_RESOLUTIONS; /* Number of resolutions available */
  const static uint16_t kRESOLUTIONS_X[]; /* X Resolution sizes available */
  const static uint16_t kRESOLUTIONS_Y[]; /* Y Resolution sizes available */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy all parameters from the other to the current if called */
  void copySelf(const Options &source);

  /* Sets all parameters to default */
  void setAllToDefault();

  /* Assign a new ailment update state to BattleOptions */
  void setAilmentUpdateState(BattleOptions new_state);

  /* Assigns if the player is to always run on the map */
  //void setAutoRun(bool auto_run);

  /* Sets the chosen font */
  void setFont(uint8_t index, bool first_call = false);
  
  /* Sets if linear filtering should be enabled or not */
  void setLinearFiltering(bool linear_filtering);

  /* Set the screen dimensions to be painted, based on the array index */
  void setScreenResolution(uint8_t index);
  
  /* Enable or disable vertical refresh syncing */
  void setVsync(bool enabled);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Function to confirm the setup of the font */
  bool confirmFontSetup();

  /* Is the player instructed to always run? */
  bool isAutoRun();

  /* Returns if the game is full screen mode */
  bool isFullScreen();
  
  /* Returns if linear filtering mode is enabled */
  bool isLinearFilteringEnabled();
  
  /* Returns true if the sound is enabled */
  bool isAudioEnabled();

  /* Returns if vertical refresh syncing is enabled */
  bool isVsyncEnabled();

  /* Sound/Audio getters */
  int32_t getAudioLevel();
  int32_t getMusicLevel();

  /* Returns the value of a given OptionState flag */
  bool getFlag(const OptionState &test_flag);

  /* Returns the current state of ailment updates */
  BattleOptions getAilmentUpdateState();

  /* Returns the program configured base path, used for pathing throughout */
  std::string getBasePath();

  /* Returns the path to the font to use throughout the application */
  std::string getFont();
  std::string getFont(uint8_t index);
  
  /* Returns the screen dimensions to be painted */
  uint16_t getScreenHeight();
  uint16_t getScreenWidth(); 

  /* Audio setter */
  bool setAudioLevel(int32_t new_level);

  /* Sound setter */
  bool setMusicLevel(int32_t new_level);

  /* Sets an option flag to a given state */
  void setFlag(OptionState flags, bool set_value = true); 

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  Options& operator= (const Options &source);
};

#endif // OPTIONS_H
