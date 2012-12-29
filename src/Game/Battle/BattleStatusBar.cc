/******************************************************************************
* Class Name: BattleStatusBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Larger box display the BattleStatus
*
* TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/

#include "Game/Battle/BattleStatusBar.h"

/* Creates a BattleStatusBar object */
BattleStatusBar::BattleStatusBar(QWidget* pointer)
{
}

/* Annihilates a BattleStatusBar object */
BattleStatusBar::~BattleStatusBar()
{
}

void BattleStatusBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
}

/* Adds a person to the vector of bars */
void BattleStatusBar::addPerson(QString name, int health, int health_max, int qd, int qd_max)
{
}

/* Retunrs height of the bar */
int BattleStatusBar::getHeight()
{
  return bar_height;
}

/* Returns width of the bar */
int BattleStatusBar::getWidth()
{
  return bar_width;
}

/* Sets height of the bar */
int BattleStatusBar::setHeight(int new_height)
{
  bar_height = new_height;

  return 0;
}

/* Sets displayed HP of person in the vector bars by given amount */
void BattleStatusBar::setDisplayHP(int new_HP, int person_num)
{
  //if (person_num <= party_size)
  //  bars.at(person_num).setDisplayHP(new_HP);
}

/* Sets width of the bar */
int BattleStatusBar::setWidth(int new_width)
{
  bar_width = new_width;

  return 0;
}
