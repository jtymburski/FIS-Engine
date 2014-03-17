/*******************************************************************************
* Class Name: InfoBar [Declaration]
* Date Created: March 1st, 2014
* Inheritance: None
* Description: 
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* 
*******************************************************************************/

#ifndef INFOBAR_H
#define INFOBAR_H

#include "EnumDb.h"
#include "Frame.h"
#include "Helpers.h"
#include "Options.h"
#include "Sound.h"
#include "Text.h"

using std::begin;
using std::end;

enum class MessageMode
{
  DISABLED,
  SIMPLIFIED,
  DETAILED
};

enum class MessagePriority
{
  ESSENTIAL,
  NORMAL,
  EXPENDABLE
};

struct BarMessage
{
  Text message_text;
  uint32_t time_to_display;
  uint32_t time_displayed;
  bool displayed;
};

class InfoBar
{
public:
  /* Creates an Ino Bar */
  InfoBar(Options* const running_config, const MessageMode &mode, 
          Frame* const bar_bg = nullptr);

  /* Annihilates an InfoBar objet */
  ~InfoBar();

private:
  /* Bar settings */
  uint16_t height;
  uint16_t width;
  float opacity;

  /* Background image of the info bar */
  Frame* bg;

  /* The vector of messages to be displayed */
  std::vector<BarMessage> messages;
 
  /* Enumerated options */
  MessageMode message_mode;

  /* Time counters */
  uint32_t time_elapsed;

  /* Rendering font */
  TTF_Font* font_regular;
  TTF_Font* font_title;

  /* Pointer to system configuration */
  Options* config;
  
  /* ------------ Constants --------------- */
  static const uint8_t  kDEFAULT_BAR_OPACITY; /* The def. opacity for the bar */
  static const float    kDEFAULT_TEXT_OPACITY; /* The default text opacity */
  static const float    kDEFAULT_TEXT_SPEED; /* The default text speed */
  static const uint16_t kMARGIN_SIDES; /* Spacing on sides of the Info Bar */
  static const uint16_t kMARGIN_TOP; /* Spacing on the top of the info bar */
  static const uint16_t kMAX_BUFFER; /* Maximum # of messages to buffer */
  static const uint16_t kMAX_CHAR; /* Maximum # of char. for displayed msg */
  static const uint16_t kMAX_TIME; /* Maximum amt. time for displayed message */
  static const uint16_t kMIN_TIME; /* Minimum amt. time for displayed msg. */
  static const uint16_t kWAIT_TIME; /* Initial waiting time before displaying */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Calculates the number of characters that can be displayed on a line */
  uint32_t calcLineLength();

  /* Attempts to create and insert the fonts */
  bool createFonts();

  /* Deletes the stored fonts */
  void deleteFonts();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to add a message into the Info Bar buffer */
  bool addMessage(SDL_Renderer* renderer, const std::string &message, 
                 uint32_t display_time, 
                 const MessagePriority &priority = MessagePriority::NORMAL);

  /* Clears the info bar of all current messages */
  void clear();

  /* Skips the current message and goes to the next message */
  bool next();

  /* Updates the info bar */
  void update(const int32_t &cycle_time);

  /* */
  bool render(SDL_Renderer* renderer);

  /* Assigns a configuration to the info bar */
  bool setRunningConfig(Options* const new_config);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

};

#endif //FRAME_H
