/******************************************************************************
* Class Name: BattleInfoBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: Battle
* Description: Display information on the screen about battle events
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/
#ifndef BATTLEINFOBAR_H
#define BATTLEINFOBAR_H

#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QTimer>

class BattleInfoBar : public QWidget
{
public:
  BattleInfoBar(QWidget* parent = NULL);
  ~BattleInfoBar();

private:
  /* Bar's height (screen_height * 0.05) */
  uint bar_height;

  /* Bar's width (screen width) */
  uint bar_width;

  /* Time the string is displayed for */
  uint display_time;

  /* Information currently being displayed */
  QString info;

  /* Bounding box for the string */
  QRect string_border;

  /* String's style */
  //QPen string_style;

  /* Timer that runs how long a string will be on the bar for */
  QTimer display_timer;

  /*------------------- Constants -----------------------*/
  static const ushort kMAX_TIME; /* Maximum # seconds for displayed message */
  static const ushort kMAX_CHAR; /* Maximum # of characters for displayed msg */
  static const ushort kWAIT_TIME; /* Time the Info Bar will pause before set */

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Set display info */
  void setDisplayTime(uint new_time);

  /* Sets the string on the bar */
  void setInfo(QString new_info);

/*==============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

/*==============================================================================
 * SIGNALS
 *============================================================================*/
signals:
  /* Emitted after message is on the bar for d time, connect to person_upkeep */
  void finished();

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds a time to the message */
  void addTime(uint value);

  /* Gets the display time for the string */
  uint getDisplayTime();

  /* Returns the height fo the bar */
  uint getHeight();

  /* Returns the width of the bar */
  uint getWidth();
    
  /* Sets the display time for the string */
  void setMessage(QString new_message, uint new_time);

  /* Sets the height of the bar */
  void setHeight(uint new_height);

  /* Sets the width of the bar */
  void setWidth(uint new_width);
};

#endif
