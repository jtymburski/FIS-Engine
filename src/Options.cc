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
const std::string Options::kFONTS[] = {
    "fonts/colab_light.otf", "fonts/crimes.ttf", "fonts/blanch_caps.otf",
    "fonts/colab_reg.otf", "fonts/colab_med.otf"};
const uint8_t Options::kNUM_FONTS = 5;
const uint8_t Options::kNUM_RESOLUTIONS = 10;
const uint16_t Options::kRESOLUTIONS_X[] = {1216, 1217, 1366, 1920, 1920,
                                            2560, 2460, 3440, 3860, 4160};
const uint16_t Options::kRESOLUTIONS_Y[] = {704,  705,  768,  1080, 1440,
                                            1080, 1440, 1440, 2160, 2160};

/* Default Screen Width and Height */
const std::uint32_t Options::kDEF_SCREEN_WIDTH{1216};
const std::uint32_t Options::kDEF_SCREEN_HEIGHT{704};

/* Default Audio and Music Levels */
const std::uint32_t Options::kDEF_AUDIO_LEVEL{40};
const std::uint32_t Options::kDEF_MUSIC_LEVEL{30};

/* Default Text and UI Scaling Options */
const std::uint32_t Options::kDEF_SCALING_TEXT{50};
const std::uint32_t Options::kDEF_SCALING_UI{0};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* Constructor function */
Options::Options(std::string base_path)
{
  audio_level = kDEF_AUDIO_LEVEL;
  music_level = kDEF_MUSIC_LEVEL;

  this->base_path = base_path;
  font = 0;
  resolution_x = 0;
  resolution_y = 0;
  sound_handler = nullptr;

  setAllToDefault();
  font_data = new Fonts(this);
}

Options::Options(const Options& source)
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

void Options::copySelf(const Options& source)
{
  /* Battle Options */
  base_path = source.base_path;
  resolution_x = source.resolution_x;
  resolution_y = source.resolution_y;
  flags = source.flags;
  flags_default = source.flags_default;
}

void Options::setAllToDefault()
{
  /* Sound options */
  setAudioLevel(kDEF_MUSIC_LEVEL);
  setMusicLevel(kDEF_AUDIO_LEVEL);

  /* Scaling options */
  setScalingText(kDEF_SCALING_TEXT);
  setScalingUI(kDEF_SCALING_UI);

  /* Flags */
  setLinearFiltering(false);

  setFlag(OptionState::VSYNC, true);
  setFlag(OptionState::FULLSCREEN, false);
  setFlag(OptionState::AUTO_RUN, false);
  setFlag(OptionState::BATTLE_ANIMATIONS, true);
  setFlag(OptionState::GUI_ENABLED, true);
  setFlag(OptionState::MUTE, false);

  /* Move the flags into default flags */
  flags_default = flags;

  /* Rendering Options */
  setFont(0, true);
  setScreenResolution(3);
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

/* Confirms correct font set-up */
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

int32_t Options::getAudioLevel()
{
  return audio_level;
}

std::string Options::getBasePath()
{
  return base_path;
}

/* Returns the default value for a flag */
bool Options::getDefaultFlag(const OptionState& test_flag)
{
  return static_cast<bool>((flags_default & test_flag) == test_flag);
}

/* Returns the value for an Options flag */
bool Options::getFlag(const OptionState& test_flag)
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

/* Returns a constructed font by enumerated value */
TTF_Font* Options::getFontTTF(FontName font)
{
  if(font_data)
    return font_data->getFont(font);

  return nullptr;
}

int32_t Options::getMusicLevel()
{
  return music_level;
}

int32_t Options::getScaledHeight()
{
  auto value = kDEF_SCREEN_HEIGHT * ((float)scaling_ui + 100) / 100;

  if(value > getScreenHeight())
    return getScreenHeight();

  return value;
}

int32_t Options::getScaledWidth()
{
  auto value = kDEF_SCREEN_WIDTH * ((float)scaling_ui + 100) / 100;

  if(value > getScreenWidth())
    return getScreenWidth();

  return value;
}

int32_t Options::getScalingText()
{
  return scaling_text;
}

int32_t Options::getScalingUI()
{
  return scaling_ui;
}

uint16_t Options::getScreenHeight()
{
  return kRESOLUTIONS_Y[resolution_y];
}

uint16_t Options::getScreenWidth()
{
  return kRESOLUTIONS_X[resolution_x];
}

bool Options::isAudioEnabled()
{
  return !getFlag(OptionState::MUTE);
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

bool Options::isVsyncEnabled()
{
  return getFlag(OptionState::VSYNC);
}

bool Options::loadData(XmlData data, int index)
{
  bool success{true};

  if(data.getElement(index) == "linear_filtering")
  {
    setFlag(OptionState::LINEAR_FILTERING, data.getDataBool(&success));
  }
  else if(data.getElement(index) == "vsync")
  {
    setFlag(OptionState::VSYNC, data.getDataBool(&success));
  }
  else if(data.getElement(index) == "fullscreen")
  {
    setFlag(OptionState::FULLSCREEN, data.getDataBool(&success));
  }
  else if(data.getElement(index) == "autorun")
  {
    setFlag(OptionState::AUTO_RUN, data.getDataBool(&success));
  }
  else if(data.getElement(index) == "battle_animations")
  {
    setFlag(OptionState::BATTLE_ANIMATIONS, data.getDataBool(&success));
  }
  else if(data.getElement(index) == "gui_enabled")
  {
    setFlag(OptionState::GUI_ENABLED, data.getDataBool(&success));
  }
  else if(data.getElement(index) == "FAST_BATTLE")
  {
    setFlag(OptionState::FAST_BATTLE, data.getDataBool(&success));
  }
  else if(data.getElement(index) == "audio_level")
  {
    audio_level = data.getDataInteger(&success);
  }
  else if(data.getElement(index) == "music_level")
  {
    music_level = data.getDataInteger(&success);
  }
  else if(data.getElement(index) == "scaling_text")
  {
    scaling_text = data.getDataInteger(&success);
  }
  else if(data.getElement(index) == "scaling_ui")
  {
    scaling_ui = data.getDataInteger(&success);
  }

  return success;
}

bool Options::saveData(FileHandler* fh)
{
  if(fh)
  {
    fh->writeXmlElement("options");

    /* Write option flags to file */
    fh->writeXmlData("linear_filtering",
                     getFlag(OptionState::LINEAR_FILTERING));
    fh->writeXmlData("vsync", getFlag(OptionState::VSYNC));
    fh->writeXmlData("fullscreen", getFlag(OptionState::FULLSCREEN));
    fh->writeXmlData("autorun", getFlag(OptionState::AUTO_RUN));
    fh->writeXmlData("battle_animations",
                     getFlag(OptionState::BATTLE_ANIMATIONS));
    fh->writeXmlData("gui_enabled", getFlag(OptionState::MUTE));
    fh->writeXmlData("fast_battle", getFlag(OptionState::FAST_BATTLE));

    /* Write option values to file */
    // fh->writeXmlData("resolution_x", resolution_x);
    // fh->writeXmlData("resolution_y", resolution_y);
    fh->writeXmlData("audio_level", audio_level);
    fh->writeXmlData("music_level", music_level);
    fh->writeXmlData("scaling_text", scaling_text);
    fh->writeXmlData("scaling_ui", scaling_ui);

    fh->writeXmlElementEnd();

    return true;
  }

  return false;
}

void Options::setAudioLevel(int32_t new_level)
{
  audio_level = Sound::setAudioVolumes(new_level);

  if(sound_handler)
    sound_handler->setAudioLevel(audio_level);
}

void Options::setFlag(OptionState flag, bool set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);

  if(getFlag(OptionState::MUTE) && sound_handler)
  {
    sound_handler->setAudioLevel(0);
    sound_handler->setMusicLevel(0);
  }

  // TODO: repair. Especially regarding VSync, Full Screen, etc SDL
}

void Options::setMusicLevel(int32_t new_level)
{
  music_level = Sound::setMusicVolumes(new_level);

  if(sound_handler && !getFlag(OptionState::MUTE))
    sound_handler->setMusicLevel(music_level);
  else if(sound_handler)
    sound_handler->setMusicLevel(0);
}

void Options::setScalingText(int32_t new_level)
{
  scaling_text = new_level;
}

void Options::setScalingUI(int32_t new_level)
{
  scaling_ui = new_level;
}

/* Sets the sound handler used. If unset, no sounds will play */
void Options::setSoundHandler(SoundHandler* new_handler)
{
  sound_handler = new_handler;
}

void Options::update()
{
  if(sound_handler)
  {
    if(getFlag(OptionState::MUTE))
    {
      sound_handler->setAudioLevel(0);
      sound_handler->setMusicLevel(0);
    }
    else
    {
      /* Scale the Audio / Music % Level From (0-100) to (0-255) */
      uint8_t scaled_audio_level = MIX_MAX_VOLUME * audio_level / 100;
      uint8_t scaled_music_level = MIX_MAX_VOLUME * music_level / 100;

      sound_handler->setAudioLevel(scaled_audio_level);
      sound_handler->setMusicLevel(scaled_music_level);
    }

    sound_handler->update();
  }
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

Options& Options::operator=(const Options& source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
