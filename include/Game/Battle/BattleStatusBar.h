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

#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

#include "Game/Battle/AllyStatusBar.h"
#include "Game/Battle/EnemyStatusBar.h"
#include "Game/Player/Party.h"

class BattleStatusBar : public QWidget
{
public:
  BattleStatusBar(Party* persons, uint width, uint height,
                  QWidget *parent = 0);
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

/*=============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds a person to the vector of bars */
  void addPerson(Person* character, int person_index);

  /* Returns the dimensions of the box */
  uint getLeftMargin();
  uint getTopDistance();
  uint getWidth();
  uint getHeight();

  /* Sets thse size of the box */
  void setSize(QRect* box);
  void setLeftMargin(uint left_margin);
  void setTopDistance(uint top_distance);
  void setWidth(uint new_width);
  void setHeight(uint new_height);
};

#endif // BATTLESTATUSBAR_H
