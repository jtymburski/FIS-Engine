/******************************************************************************
* Class Name: BattleStatusBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: Battle
* Description: Larger box display the BattleStatus
*
* TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/
#ifndef BATTLESTATUSBAR_H
#define BATTLESTATUSBAR_H

#include <QtGui/QWidget>

#include "Game/Battle/PersonStatusBar.h"

class BattleStatusBar : public QWidget
{
public:
  BattleStatusBar(QWidget *pointer = 0);
  ~BattleStatusBar();

private:
  /* Party's size (1-5) */
  int party_Size;

  /* All status bars */
  QVector<PersonStatusBar> bars;

  /* Height of the bar (screen_height * 0.20) */
  int bar_height;

  /* Width of the bar (screen_width * 0.35) */
  int bar_width;

  public: 
  	/* Adds a person to the vector of bars */
  void addPerson(QString name, int health, int health_max, int qd, int qd_max);

  /* Retunrs height of the bar */
  int getHeight();

  /* Returns width of the bar */
  int getWidth();

  /* Sets height of the bar */
  int setHeight(int h);

  /* Sets displayed HP of person in the vector bars by given amount */
  void setDisplayHP(int amount, int person_num);

  /* Sets width of the bar */
  int setWidth(int w);
};

#endif // BATTLESTATUSBAR_H
