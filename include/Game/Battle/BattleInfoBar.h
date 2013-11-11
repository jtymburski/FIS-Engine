/******************************************************************************
* Class Name: BattleInfoBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: Battle
* Description: Display information on the screen about battle events
*
* //TODO: Finish update functions [11-04-13]
******************************************************************************/
#ifndef BATTLEINFOBAR_H
#define BATTLEINFOBAR_H

#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "Game/Frame.h"

class BattleInfoBar : public QWidget
{
  Q_OBJECT

public:
  /* Creates a BattleInfoBar object */
  BattleInfoBar(uint bar_height, uint bar_width, QWidget* parent = 0);

  /* Annihilates a BattleInfoBar object */
  ~BattleInfoBar();

  /* Mode for displaying the BattleInfoBar messages */
  enum MessageDisplayMode
  {
    DISABLED,
    DEBUG,
    BASIC,
    FULL
  };

  /* Mode for display the text of messages */
  enum TextDisplayMode
  {
    INSTANT,
    SCROLLING
  };

private:
  /* Structure to hold the data for a message to be displayed */
  struct Message
  {
    QString text;
    short time_visible;
  };

  /* Bar's height (screen_height * 0.05) */
  ushort bar_height;

  /* Opacity of the BattleInfoBar */
  float bar_opacity;

  /* Bar's width (screen width) */
  ushort bar_width;

  /* Background of the info bar */
  Frame* bg;

  /* Time the current string has displayed for */
  uint time_elapsed;

  /* The enumerated message display mode */
  MessageDisplayMode message_display_mode;

  /* Time the current string has displayed for */
  ushort message_time_elapsed;

  /* List of information to be displayed */
  QList<Message> message_buffer;

  /* The enumerated text display mode */
  TextDisplayMode text_display_mode;

  /* Font of the text */
  QFont text_font;

  /* Opacity of the text */
  float text_opacity;

  /* The speed to display the text at */
  float text_speed;

  /* Style of the text */
  QPen text_style;

  /*------------------- Constants -----------------------*/
  static const ushort kDEFAULT_BAR_HEIGHT; /* Default height of the bar */
  static const ushort kDEFAULT_BAR_WIDTH; /* The default width of the bar */
  static const float kDEFAULT_BAR_OPACITY; /* The default opacity for the bar */
  static const float kDEFAULT_TEXT_OPACITY; /* The default text opacity */
  static const float kDEFAULT_TEXT_SPEED; /* The default text speed */
  static const ushort kMARGIN_SIDES; /* Spacing on the sides of the Info Bar */
  static const ushort kMARGIN_TOP; /* Spacing on the top of the info bar */
  static const ushort kMAX_BUFFER; /* Maximum # of messages to buffer */
  static const ushort kMAX_CHAR; /* Maximum # of characters for displayed msg */
  static const ushort kMAX_TIME; /* Maximum amt. time for displayed message */
  static const ushort kMIN_TIME; /* Minimum amt. time for displayed msg. */
  static const ushort kMIN_CHAR; /* Minimum # of characters for displayed msg */
  static const ushort kWAIT_TIME; /* Initial waiting time before displaying */

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Assigns all values of the BattleInfo bar to defaults */
  void loadDefaults();

  /* Clears the current message */
  void clear();

  /* Clears the stack of messages */
  void clearAll();

  /* Returns the height of the bar */
  uint getBarHeight();

  /* Returns the width of the bar */
  uint getBarWidth();

  /* Returns the display mode of the message */
  MessageDisplayMode getMessageDisplayMode();

  /* Returns the display mode of the text */
  TextDisplayMode getTextDisplayMode();

  /* Obtains the speed of the text */
  uint getTextSpeed();

  /* Obtains the currently elapsed time */
  uint getTimeElapsed();

  /* Assigns the y-dimension of the bar */
  void setBarHeight(uint new_value);

  /* Assigns the x-dimension of the bar */
  void setBarWidth(uint new_value);

  /* Assigns the display mode for the message */
  void setMessageDisplayMode(MessageDisplayMode new_message_display_mode);

  /* Assigns the text display mode */
  void setTextDisplayMode(TextDisplayMode new_text_display_mode);

  /* Assigns the speed of the text */
  void setTextSpeed(float new_value);

  /* Updates the time elapsed */
  void setTimeElapsed(uint new_value);

/*==============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

/*==============================================================================
 * PUBLIC SLOTS
 *============================================================================*/
public slots:

/*==============================================================================
 * SIGNALS
 *============================================================================*/
signals:
  void invalidMessage(QString reasons);

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds a message to the BattleInfoBar message buffer */
  bool addMessage(QString text, short time_visible = 0);

  /* Update the cycle time of BattleInfoBar */
  void update(int cycle_time);

  /* Returns the opacity of the bar */
  float getBarOpacity();

  /* Returns the assigned background of the bar */
  Frame* getBackground();

  /* Returns the font of the text */
  QFont getTextFont();

  /* Returns the opacity of the text */
  float getTextOpacity();

  /* Returns the style of the text */
  QPen getTextStyle();

  /* Sets the opacity of the bar */
  bool setBarOpacity(float new_value);

  /* Assigns a new background */
  void setBackground(Frame* new_backround);

  /* Assigns a new font to the info bar */
  void setTextFont(QFont new_font);

  /* Assigns a new value to the text opacity */
  bool setTextOpacity(float new_value);

  /* Assigns a new font style to the info bar */
  void setTextStyle(QPen new_style);
};

#endif
