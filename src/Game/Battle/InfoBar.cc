/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: February 23rd, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Battle/InfoBar.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const uint8_t  InfoBar::kDEFAULT_BAR_OPACITY{2};
const float    InfoBar::kDEFAULT_TEXT_OPACITY{1.0}; 
const float    InfoBar::kDEFAULT_TEXT_SPEED{0.5};
const uint16_t InfoBar::kMARGIN_SIDES{25}; 
const uint16_t InfoBar::kMARGIN_TOP{15}; 
const uint16_t InfoBar::kMAX_BUFFER{300}; 
const uint16_t InfoBar::kMAX_CHAR{80}; 
const uint16_t InfoBar::kMAX_TIME{75}; 
const uint16_t InfoBar::kMIN_TIME{5}; 
const uint16_t InfoBar::kWAIT_TIME{5};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
InfoBar::InfoBar(Options* const running_config, 
                 const MessageMode &mode, Frame* const bar_bg)
  : opacity{kDEFAULT_BAR_OPACITY}
  , bg{bar_bg}
  , message_mode{mode}
  , time_elapsed{0}
  , font_regular{nullptr}
  , font_title{nullptr}
  , config{running_config}
{
  if (!createFonts())
    std::cerr << "[Error]: Unable to make InfoBar fonts\n";
}

InfoBar::~InfoBar()
{
  deleteFonts();
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
/* Calculates the number of characters that can be displayed on a line */
uint32_t InfoBar::calcLineLength()
{
  return 0; //TODO: Calculation [03-14-14]
}

bool InfoBar::createFonts()
{
  auto success = false;

  if (config != nullptr)
  {
    /* Attempt to create new fonts */
    auto regular_font = Text::createFont(config->getBasePath() + 
                                         config->getFont(), 20);
    auto title_font   = Text::createFont(config->getBasePath() +
                                         config->getFont(), 20, TTF_STYLE_BOLD);
    
    /* Insert fonts if successful, otherwise delete */
    if (regular_font != nullptr && title_font != nullptr)
    {
      deleteFonts();
      font_regular = regular_font;
      font_title   = title_font;
      success      = true;
    }
    else
    {
      TTF_CloseFont(regular_font);
      regular_font = nullptr;

      TTF_CloseFont(title_font);
      title_font = nullptr;
    }
  }

  return success;
}

void InfoBar::deleteFonts()
{
  TTF_CloseFont(font_regular);
  font_regular = nullptr;

  TTF_CloseFont(font_title);
  font_title = nullptr;
}

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool InfoBar::addMessage(SDL_Renderer* renderer, const std::string &message, 
                         uint32_t display_time, const MessagePriority &priority)
{
  Helpers::setInRange(display_time, kMIN_TIME, kMAX_TIME);
  
  auto line_length = calcLineLength();
  auto lines = Text::splitLine(font_regular, message, line_length, false);
  
  for (auto it = begin(lines); it != end(lines); ++it)
  {
    Text single_line;
    single_line.setFont(font_regular);
    single_line.setText(renderer, *it, {255, 255, 255, kDEFAULT_TEXT_OPACITY});

    BarMessage new_message;
    new_message.message_text    = single_line;
    new_message.time_to_display = display_time;
    new_message.time_displayed  = 0;
    new_message.displayed       = false;
    messages.push_back(new_message);

    return true;
  }

  return false;
}

void InfoBar::clear()
{
  messages.clear();
}

bool InfoBar::next()
{
  if (!messages.empty())
  {
    messages.erase(begin(messages));

    return true;
  }

  return false;
}

void InfoBar::update(const int32_t &cycle_time)
{
  auto old_time = time_elapsed;
  time_elapsed += static_cast<uint32_t>(cycle_time);
}

bool InfoBar::render(SDL_Renderer* renderer)
{
  (void)renderer;
  return false;//TODO: Render [03-14-14]
}

bool InfoBar::setRunningConfig(Options* const new_config)
{
  config = new_config;

  return (new_config != nullptr);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
