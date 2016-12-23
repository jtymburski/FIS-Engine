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

class Fonts;

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "EnumFlags.h"
#include "Fonts.h"
#include "SoundHandler.h"

ENUM_FLAGS(OptionState)
enum class OptionState
{
  LINEAR_FILTERING = 1 << 0,
  VSYNC = 1 << 1,
  FULLSCREEN = 1 << 2,
  AUTO_RUN = 1 << 3, /* Do not change -- Linked to Menu */
  BATTLE_ANIMATIONS = 1 << 4,
  GUI_ENABLED = 1 << 5,
  MUTE = 1 << 6, /* Do not change -- Linked to Menu */
  FAST_BATTLE = 1 << 7
};

class Options
{
public:
  /* Constructor(s) */
  Options(std::string base_path = "");

  /* Copy constructor */
  Options(const Options& source);

  /* Destructor */
  ~Options();

private:
  /* Fonts */
  Fonts* font_data;

  /* The base string path for accessing file information */
  std::string base_path;

  /* The index for the selected font */
  uint8_t font;

  /* If the linear filtering is enabled */
  // bool linear_filtering;

  /* Enum flags option state set */
  OptionState flags;
  OptionState flags_default;

  /* Resolution of the screen currently being used */
  uint8_t resolution_x;
  uint8_t resolution_y;

  /* Sound handling class */
  SoundHandler* sound_handler;

  /*------------------- Constants -----------------------*/
  const static std::string kFONTS[]; /* A list of all the fonts avfhudable */
  const static uint8_t kNUM_FONTS;   /* The number of fonts available */
  const static uint8_t kNUM_RESOLUTIONS;  /* Number of resolutions available */
  const static uint16_t kRESOLUTIONS_X[]; /* X Resolution sizes available */
  const static uint16_t kRESOLUTIONS_Y[]; /* Y Resolution sizes available */

public:
  /* Audio / Music Level */
  uint32_t audio_level;
  uint32_t music_level;

  /* UI and Text Scaling */
  uint32_t scaling_text;
  uint32_t scaling_ui;

  /*--------------------- Constants --------------------*/
  const static std::uint32_t kDEF_SCREEN_WIDTH;
  const static std::uint32_t kDEF_SCREEN_HEIGHT;
  const static uint32_t kDEF_AUDIO_LEVEL;
  const static uint32_t kDEF_MUSIC_LEVEL;
  const static uint32_t kDEF_SCALING_TEXT;
  const static uint32_t kDEF_SCALING_UI;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Copy all parameters from the other to the current if called */
  void copySelf(const Options& source);

  /* Sets all parameters to default */
  void setAllToDefault();

  /* Sets the chosen font */
  void setFont(uint8_t index, bool first_call = false);

  /* Sets if linear filtering should be enabled or not */
  void setLinearFiltering(bool linear_filtering);

  /* Set the screen dimensions to be painted, based on the array index */
  void setScreenResolution(uint8_t index);

  /* Enable or disable vertical refresh syncing */
  void setVsync(bool enabled); // TODO: re-implement - should be in GIT hist

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Function to confirm the setup of the font */
  bool confirmFontSetup();

  /* Sound/Audio getter */
  int32_t getAudioLevel();

  /* Returns the program configured base path, used for pathing throughout */
  std::string getBasePath();

  /* Default flags */
  bool getDefaultFlag(const OptionState& test_flag);

  /* Returns the value of a given OptionState flag */
  bool getFlag(const OptionState& test_flag);

  /* Returns the path to the font to use throughout the application */
  std::string getFont();
  std::string getFont(uint8_t index);

  /* Returns a pointer to a constructed font by enumerated value */
  TTF_Font* getFontTTF(FontName font);

  /* Music getter */
  int32_t getMusicLevel();

  /* Scaled height/width getter */
  int32_t getScaledHeight();
  int32_t getScaledWidth();

  /* Return the text and UI scaling factors */
  int32_t getScalingText();
  int32_t getScalingUI();

  /* Returns the screen dimensions to be painted */
  uint16_t getScreenHeight();
  uint16_t getScreenWidth();

  /* Returns true if the sound is enabled */
  bool isAudioEnabled();

  /* Returns if the game is full screen mode */
  bool isFullScreen();

  /* Returns if linear filtering mode is enabled */
  bool isLinearFilteringEnabled();

  /* Returns if vertical refresh syncing is enabled */
  bool isVsyncEnabled();

  /* Audio setter */
  void setAudioLevel(int32_t new_level);

  /* Sets an option flag to a given state */
  void setFlag(OptionState flags, bool set_value = true);

  /* Sound setter */
  void setMusicLevel(int32_t new_level);

  /* Set scaling factors */
  void setScalingText(int32_t new_level);
  void setScalingUI(int32_t new_level);

  /* Sets the sound handler used. If unset, no sounds will play */
  void setSoundHandler(SoundHandler* new_handler);

  /* Update the options state */
  void update();

  /*======================== OPERATOR FUNCTIONS ==============================*/
public:
  Options& operator=(const Options& source);
};

#endif // OPTIONS_H
