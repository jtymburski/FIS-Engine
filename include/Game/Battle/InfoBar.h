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

#include "Frame.h"
#include "Text.h"

enum class MessageMode
{
  DISABLED,
  SIMPLIFIED,
  DETAILED
};

struct BarMessage
{
  Text message_text;
  uint32_t time_to_display;
  uint32_t time_displayed;
};

class InfoBar
{
public:

  /* Creates an Ino Bar */
  InfoBar(const uint16_t &bar_height, const uint16_t &bar_width, 
          const MessageMode &mode, Frame* const bar_bg = nullptr);

  /* Annihilates an InfoBar objet */
  ~InfoBar() = default;

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
  
  /* ------------ Constants --------------- */
  static const float    kDEFAULT_BAR_OPACITY; /* The def. opacity for the bar */
  static const float    kDEFAULT_TEXT_OPACITY; /* The default text opacity */
  static const float    kDEFAULT_TEXT_SPEED; /* The default text speed */
  static const uint16_t kMARGIN_SIDES; /* Spacing on sides of the Info Bar */
  static const uint16_t kMARGIN_TOP; /* Spacing on the top of the info bar */
  static const uint16_t kMAX_BUFFER; /* Maximum # of messages to buffer */
  static const uint16_t kMAX_CHAR; /* Maximum # of char. for displayed msg */
  static const uint16_t kMAX_TIME; /* Maximum amt. time for displayed message */
  static const uint16_t kMIN_TIME; /* Minimum amt. time for displayed msg. */
  static const uint16_t kMIN_CHAR; /* Minimum # of char. for displayed msg */
  static const uint16_t kWAIT_TIME; /* Initial waiting time before displaying */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  /*  */
  void clear();

  

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

};

#endif //FRAME_H
