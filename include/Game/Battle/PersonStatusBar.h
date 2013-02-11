/******************************************************************************
* Class Name: PersonStatusBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: BattleStatusBar
* Description: Displays iformation for health and QD to the screen
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/
#ifndef PERSONSTATUSBAR_H
#define PERSONSTATUSBAR_H

#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include "Game/Sprite.h"

class PersonStatusBar : public QWidget
{
public:
  PersonStatusBar(QWidget* parent = 0);
  ~PersonStatusBar();

private:
  /* Bar width (screen_width * 0.06) */
  int bar_width;

  /* Bar height (screen_height * 0.16) */
  int bar_height;

  /* Person's health */
  int health;

  /* Person's max health */
  int health_max;

  /* The qd bar bounding box (scales with usage) */
  int qd;

  /* Person's maximum qd */
  int qd_max;

  /* HP Bounding box */
  QRect health_outline; 

  /* HP Bar bounding box (scales with health shift) */
  QRect health_bar; 

  /* QD Bounding box */
  QRect qd_outline; 

  /* QD Bar Bounding box (scales with usage) */
  QRect qd_bar;

  /* Bounding box for Person's name */
  QRect name_box; 

  /* Person's name */
  QString name;

  /* Sprite pointers to status_thumbnails */
  QVector<Sprite* > status_thumbs;

protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);
  
public:
  /* Sets current HP for display */
  void setDisplayHP(int amount);

  /* Sets current QD for display */
  void setDisplayQD(int amount);

  /* Sets the max HP for display */
  void setDisplayMaxHP(int amount);

  /* Sets the max QD for display */
  void setDisplayMaxQD(int amount);
};

#endif // PERSONSTATUSBAR_H
