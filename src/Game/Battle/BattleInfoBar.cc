/******************************************************************************
* Class Name: BattleInfoBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Display information on the screen about battle events
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/

#include "Game/Battle/BattleInfoBar.h"
#include <QDebug>

/* Creates a BattleInfoBar object */
BattleInfoBar::BattleInfoBar(QWidget* parent)
{

}

/* Annihilates a BattleInfoBar objet */
BattleInfoBar::~BattleInfoBar()
{
}

void BattleInfoBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

}


/* Gets the display time for the string */
int BattleInfoBar::getDisplayTime()
{
  return display_time;
}

/* Returns the height fo the bar */
int BattleInfoBar::getHeight()
{
  return bar_height;
}

/* Returns the width of the bar */
int BattleInfoBar::getWidth()
{
  //return get_width;
  return 0;
}

/* Sets the display time for the string */
void BattleInfoBar::setDisplayTime(int new_time)
{
  display_time = new_time;
}

/* Sets the height of the bar */
void BattleInfoBar::setHeight(int new_height)
{
  bar_height = new_height;
}

/* Sets the string on the bar */
void BattleInfoBar::setInfo(QString new_info)
{
  info = new_info;
}

/* Sets the pen style of the string */
//TODO: QPEN Implemneted out? [12-02-12]
void BattleInfoBar::setStringStyle()
{
}

/* Sets the string with regard to the timer */
void BattleInfoBar::setTimedInfo(QString new_timed_info)
{
 info = new_timed_info;
}

/* Sets the width of the bar */
void BattleInfoBar::setWidth(int new_width)
{
  bar_width = new_width;
}
