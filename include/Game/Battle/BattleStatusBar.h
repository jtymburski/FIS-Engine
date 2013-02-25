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
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

#include "Game/Battle/PersonStatusBar.h"
#include "Game/Player/Party.h"

class BattleStatusBar : public QWidget
{
public:
  BattleStatusBar(Party* persons, uint width, uint height, QWidget *parent = 0);
  ~BattleStatusBar();

private:
  /* Static const max limits */
  static const uint kMAX_WIDTH = 1920;
  static const uint kMAX_HEIGHT = 180;

  /* All status bars */
  QVector<PersonStatusBar*> bars;
  QVector<QRect*> boxes;

  /* Box Dimensions and Coordinates */
  uint left_margin;
  uint top_distance;
  uint height;
  uint width;

protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

public:
  /* Adds a person to the vector of bars */
  void addPerson(Person* character, uint person_index);

  /* Returns the dimensions of the box */
  uint getLeftMargin();
  uint getTopDistance();
  uint getWidth();
  uint getHeight();

  /* Gets displayed values of a person in the vector given an amount */
  uint getDisplayHP(int person_index);
  uint getDisplayQD(int person_index);
  uint getDisplayMaxHP(int person_index);
  uint getDisplayMaxQD(int person_index);

  /* Sets displayed values of a person in the vector given an amount */
  void setDisplayHP(uint vitality, int person_index);
  void setDisplayQD(uint qd, int person_index);
  void setDisplayMaxHP(uint max_vitality, int person_index);
  void setDisplayMaxQD(uint max_qd, int person_index);

  /* Sets thse size of the box */
  void setSize(QRect* box);
  void setLeftMargin(uint left_margin);
  void setTopDistance(uint top_distance);
  void setWidth(uint new_width);
  void setHeight(uint new_height);
};

#endif // BATTLESTATUSBAR_H
