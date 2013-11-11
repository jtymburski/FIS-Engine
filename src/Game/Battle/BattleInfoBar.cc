/******************************************************************************
* Class Name: BattleInfoBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Display information on the screen about battle events
******************************************************************************/
#include "Game/Battle/BattleInfoBar.h"
#include <QDebug>

/*==============================================================================
 * CONSTANTS - See header file for descriptions
 *============================================================================*/
const ushort BattleInfoBar::kDEFAULT_BAR_HEIGHT    = 100;
const ushort BattleInfoBar::kDEFAULT_BAR_WIDTH     = 1216;
const float  BattleInfoBar::kDEFAULT_BAR_OPACITY   = 0.5;
const float  BattleInfoBar::kDEFAULT_TEXT_OPACITY  = 1.0;
const float BattleInfoBar::kDEFAULT_TEXT_SPEED    = 0.5;
const ushort BattleInfoBar::kMARGIN_SIDES          = 25;
const ushort BattleInfoBar::kMARGIN_TOP            = 15;
const ushort BattleInfoBar::kMAX_BUFFER            = 300;
const ushort BattleInfoBar::kMAX_CHAR              = 80;
const ushort BattleInfoBar::kMAX_TIME              = 75;
const ushort BattleInfoBar::kMIN_TIME              =  5;
const ushort BattleInfoBar::kMIN_CHAR              =  5;
const ushort BattleInfoBar::kWAIT_TIME             =  5;

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a BattleInfoBar
 *
 * Inputs: QWidget* - pointer to the parent object
 */
BattleInfoBar::BattleInfoBar(uint bar_height, uint bar_width, QWidget* parent)
  : QWidget(parent)
{
  loadDefaults();
  this->bar_height = bar_height;
  this->bar_width  = bar_width;
}

/*
 * Description: Destroys a BattleInfoBar object
 */
BattleInfoBar::~BattleInfoBar() {}

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Loads the default settings into the battle info bar.
 *
 * Inputs: none
 * Output: none
 */
void BattleInfoBar::loadDefaults()
{
  bar_height = kDEFAULT_BAR_HEIGHT;
  bar_opacity = kDEFAULT_BAR_OPACITY;
  bar_width = kDEFAULT_BAR_WIDTH;

  bg = 0;

  time_elapsed = 0;

  message_display_mode = DEBUG;
  message_time_elapsed = 0;
  message_buffer.clear();
  text_display_mode = INSTANT;

  // text_font    = QFont();
  text_opacity = kDEFAULT_TEXT_OPACITY;
  text_speed   = kDEFAULT_TEXT_SPEED;
  // text_style   = QPen();
}

/*
 * Description: Clears the mos recent message on the BattleInfoBar and resets
 *              the time displayed.
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::clear()
{
  if (!message_buffer.isEmpty())
    message_buffer.removeFirst();

  message_time_elapsed = 0;
}

/*
 * Description: Clears all the messages stored in the buffer and resets the
 *              time displayed on the message.
 *
 * Inputs: none
 * Output: none
 */
void BattleInfoBar::clearAll()
{
  message_buffer.clear();
  message_time_elapsed = 0;
}

/*
 * Description: Returns the height of the bar.
 *
 * Inputs: none
 * Output: uint - the stored height of the info bar
 */
uint BattleInfoBar::getBarHeight()
{
  return bar_height;
}

/*
 * Description: Returns the width of the bar.
 *
 * Inputs: none
 * Output: uint - the stored width of the bar.
 */
uint BattleInfoBar::getBarWidth()
{
  return bar_width;
}

/*
 * Description: Returns the message display mode of the info bar
 *
 * Inputs: none
 * Output: BattleInfoBar::MessageDisplayMode - the msg display mode currently set
 */
BattleInfoBar::MessageDisplayMode BattleInfoBar::getMessageDisplayMode()
{
  return message_display_mode;
}

/*
 * Description: Returns the text display mode of the info bar
 *
 * Inputs: none
 * Output: BattleInfoBar::TextDisplayMode - the text display mode currently set
 */
BattleInfoBar::TextDisplayMode BattleInfoBar::getTextDisplayMode()
{
  return text_display_mode;
}

/*
 * Description: Returns the currently set text speed.
 *
 * Inputs: none
 * Output: uint - the text speed currently set.
 */
uint BattleInfoBar::getTextSpeed()
{
  return text_speed;
}

/*
 * Description: Returns the total time elapsed no the Info Bar.
 *
 * Inputs: none
 * Output: uint - the total time elapsed
 */
uint BattleInfoBar::getTimeElapsed()
{
  return time_elapsed;
}

/*
 * Description: Assigns a new value to the height of the bar.
 *
 * Inputs: uint new_value - new value to be assigned to the bar height.
 * Output: none
 */
void BattleInfoBar::setBarHeight(uint new_value)
{
  bar_height = new_value;
}

/*
 * Description: Assigns a new value to the width of the bar.
 *
 * Inputs: uint new_value - new value to be assigned to the bar width
 * Output: none
 */
void BattleInfoBar::setBarWidth(uint new_value)
{
  bar_width = new_value;
}

/*
 * Description: Assigns a new message display mode to the BattleInfoBar (DEBUG,
 *              GUI, etc.)
 *
 * Inputs: BattleInfoBar::MessageDisplayMode - new enumerated value
 * Output: none
 */
void BattleInfoBar::setMessageDisplayMode(BattleInfoBar::MessageDisplayMode
                                          new_message_display_mode)
{
  message_display_mode = new_message_display_mode;
}

/*
 * Description: Assigns a new text display mode to the BattleInfoBar (INSTANT,
 *              SCROLLING)
 *
 * Inputs: BattleInfoBar::TextDisplayMode - new enumerated value
 * Output: none
 */
void BattleInfoBar::setTextDisplayMode(BattleInfoBar::TextDisplayMode
                                       new_text_display_mode)
{
  text_display_mode = new_text_display_mode;
}

/*
 * Description: Assigns a new value to the speed of the test for the Battle
 *              Info Bar.
 *
 * Inputs: float new_value - the new value for the speed of the text
 * Output: none
 */
void BattleInfoBar::setTextSpeed(float new_value)
{
  text_speed = new_value;
}

/*
 * Description: Assigns a new value to the elapsed time of the Battle Info Bar.
 *
 * Inputs: uint new_value - new value to assign to the elapsed time
 * Output: none
 */
void BattleInfoBar::setTimeElapsed(uint new_value)
{
  time_elapsed = new_value;
}

/*==============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
}

/*==============================================================================
 * PUBLIC SLOTS
 *============================================================================*/

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add a message to the buffer of messages to be
 *              displayed on the Info Bar. This function asserts that the message
 *              is valid and attempts to make it as valid as possible.
 *
 * Inputs: QString text - raw text to attempt to be added
 *         time_visible - short value of time to be displayed for the message
 * Output: bool - the validity of the new message
 */
bool BattleInfoBar::addMessage(QString text, short time_visible)
{
  bool valid_message = true;

  /* Check for info message validity, adjust accordingly */
  if (message_buffer.size() < kMAX_BUFFER)
  {
    valid_message = false;
    emit invalidMessage("Error: Info Message Buffer Overflow");
  }
  if (text.size() < kMIN_CHAR)
  {
    valid_message = false;
    emit invalidMessage("Error: Info Message too short");
  }
  if (text.size() > kMAX_CHAR)
  {
    text = text.left(80);
    emit invalidMessage("Warning: Info essage Exceeds Allowed Length");
  }
  if (!getTextDisplayMode() == DEBUG || time_visible < kMIN_TIME)
  {
    time_visible = kMIN_TIME;
    emit invalidMessage("Warning: Info mmsg time lower than minimum allowed");
  }
  if (!getTextDisplayMode() == DEBUG || time_visible > kMAX_TIME)
  {
    time_visible = kMAX_TIME;
    emit invalidMessage("Warning: Info msg time visible exceeds maximum value");
  }

  if (valid_message)
  {
    Message new_message;
    new_message.text = text;
    new_message.time_visible = time_visible;

    message_buffer.push_back(new_message);
  }

  return valid_message;
}

/*
 * Description: Updates the elapsed time on the BattleInfo bar given a new cycle
 *              time.
 *
 * Inputs: int cycle_time - value to be added to elapsed time.
 * Output:
 */
void BattleInfoBar::update(int cycle_time)
{
  cycle_time;//WARNING
  //TODO
}

/*
 * Description: Returns the value stored for the opacity of the bar.
 *
 * Inputs: none
 * Output: float - the opacity of the bar
 */
float BattleInfoBar::getBarOpacity()
{
  return bar_opacity;
}

/*
 * Description: Returns a pointer to the background image of the BattleInfoBar.
 *
 * Inputs: none
 * Output: Frame* - pointer to the background image
 */
Frame* BattleInfoBar::getBackground()
{
  return bg;
}

/*
 * Description: Returns the current font setting of the BattleInfoBar.
 *
 * Inputs: none
 * Output: QFont - the font of the BIB.
 */
QFont BattleInfoBar::getTextFont()
{
  return text_font;
}

/*
 * Description: Returns the value of the text opacity
 *
 * Inputs: none
 * Output: float - the opacity of the text
 */
float BattleInfoBar::getTextOpacity()
{
  return text_opacity;
}

/*
 * Description: Returns the value of the text style
 *
 * Inputs: none
 * Output: QPen
 */
QPen BattleInfoBar::getTextStyle()
{
  return text_style;
}

/*
 * Description: Assigns a new value to the opacity of the bar.
 *
 * Inputs: float new_value - new value for the opacity of the bar.
 * Output: bool - true if the opacity was set to the value wanted
 */
bool BattleInfoBar::setBarOpacity(float new_value)
{
  if (new_value <= 1)
  {
    bar_opacity = new_value;
    return true;
  }
  else
  {
    bar_opacity = 1.0;
    return false;
  }
}

/*
 * Description: Assigns a new background image to the BIB.
 *
 * Inputs: Frame* new_bg - pointer to a new image for the BIB.
 * Output: none
 */
void BattleInfoBar::setBackground(Frame* new_bg)
{
  bg = new_bg;
}

/*
 * Description: Assigns a new font to the text
 *
 * Inputs: QFont new_font - new font to be assigned
 * Output: none
 */
void BattleInfoBar::setTextFont(QFont new_font)
{
  text_font = new_font;
}

/*
 * Description: Assigns a new value to the opacity of the text. Returns true
 *              if the value intended was set.
 *
 * Inputs: float new_value - new value to set the opacity of the text to
 * Output: bool - true if the intended value was set
 */
bool BattleInfoBar::setTextOpacity(float new_value)
{
  if (new_value <= 1)
  {
    text_opacity = new_value;
    return true;
    }
  else
  {
    text_opacity = 1.0;
    return false;
  }
}

/*
 * Description: Assigns a new text style to the message text
 *
 * Inputs: QPen new_style - new pen style to be assigned
 * Output: none
 */
void BattleInfoBar::setTextStyle(QPen new_style)
{
  text_style = new_style;
}
