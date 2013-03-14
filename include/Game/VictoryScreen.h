/******************************************************************************
* Class Name: VictoryScreen
* Date Created: Sunday, October 28th, 2012
* Inheritance: QWidget
* Description: Displays screen after winning a battle
*
* TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/
#ifndef VICTORYSCREEN_H
#define VICTORYSCREEN_H

#include <QImage>
#include <QTimer>
#include <QtGui/QWidget>

#include "Game/Player/Party.h"

class VictoryScreen : public QWidget
{
public:
  VictoryScreen(QWidget *parent = NULL);
  ~VictoryScreen();

private:
  /* The party being shown on this screen */
  Party* team;

  /* The backdrop for the victory screen */
  QImage bgimage;

  /* The bar boundary image */
  QImage cursor;

  /* The current bar (stretched accordingly) */
  QImage current_bar;

  /* The empty bar background (stretched accordingly) */
  QImage empty_bar;

  /* The future bar (gets replaced by current_bar, stretched accordingly) */
  QImage future_bar;

  /* The timer that operates the bar displays */
  QTimer bar_timer;

  /* Timer that operates the number displays */
  QTimer number_timer;

  /* The bounding boxes used for each sprite */
  QRect bounding_boxes[7];

  /* Each party member */
  QVector <Person*> output_team;

protected:


signals:


public slots:


public:
  /* Sets up the screen */
  void setUp(); 

  /* Sets up the output_team vector */
  void teamSetUp(); 
};

#endif // VICTORYSCREEN_H
