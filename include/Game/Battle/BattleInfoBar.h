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
  BattleInfoBar(QWidget *parent = NULL);
  ~BattleInfoBar();

private:
  /* Bar's height (screen_height * 0.05) */
  int bar_height;

  /* Bar's width (screen width) */
  int bar_width;

  /* Information currently being displayed */
  QString info;
    
  /* The bar's border (may be coded instead of image) */
  QImage border;

  /* Bounding box for the string */
  QRect string_border;

  /* String's style */
  //QPen string_style;

  /* Time the string is displayed for */
  int display_time;

  /* Timer that runs how long a string will be on the bar for */
  QTimer display_timer;

protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

signals:
  /* Emitted after message is on the bar for display_time
  (connects to personalUpkeep() in battle) */
  void finished();

public:
  /* Gets the display time for the string */
  int getDisplayTime();

  /* Returns the height fo the bar */
  int getHeight();

  /* Returns the width of the bar */
  int getWidth();
    
  /* Sets the display time for the string */
  void setDisplayTime(int t);

  /* Sets the height of the bar */
  void setHeight(int h);

  /* Sets the string on the bar */
  void setInfo(QString str);

  /* Sets the pen style of the string */
  //void setStringStyle(QPen p);
  void setStringStyle();

  /* Sets the string with regard to the timer */
  void setTimedInfo(QString str);

  /* Sets the width of the bar */
  void setWidth(int w);
};

#endif
