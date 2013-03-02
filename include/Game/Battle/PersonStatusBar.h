/******************************************************************************
* Class Name: PersonStatusBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: BattleStatusBar
* Description: Displays iformation for health and QD to the screen
******************************************************************************/
#ifndef PERSONSTATUSBAR_H
#define PERSONSTATUSBAR_H

#include <QLabel>
#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include "Game/Sprite.h"
#include "Game/Player/Person.h"

class PersonStatusBar : public QWidget
{
public:
  /* Constructo for PersonStatusBar object */
  PersonStatusBar(Person* charcter, uint w, uint h, QWidget* parent = 0);

  /* Annihilates a PersonStatusBar object */
  ~PersonStatusBar();

private:
  /* Static const max limits */
  static const uint kMAX_WIDTH  = 1920 / 5;
  static const uint kMAX_HEIGHT = 180;

  /* Setup */
  void setup();

  /* Dimensions and size of box */
  uint bar_width;
  uint bar_height;

  /* Name text label */
  QLabel* name_label;
  QLabel* level_label;
  QLabel* health_label;
  QLabel* qd_label;

  /* Object Bounding Boxes */
  QVector<QRect*> status_thumbs;
  QRect* health_outline;
  QRect* health_bar;
  QRect* qd_outline;
  QRect* qd_bar;
  QRect* name_box;

  /* Gradients */
  QLinearGradient* health_grad;
  QLinearGradient* qd_grad;

  /* Person pointer the status bar is constructed for */
  Person* character;

  /* Sprite pointers to status_thumbnails */
  QVector<Sprite*> status_thumb_sprites;

protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);
  
public:
  /* Gets for Stat displays */
  QString getDisplayHP();
  QString getDisplayQD();
  QString getDisplayLevel();

  /* Get size of box */
  uint getWidth();
  uint getHeight();

  /* Gets the name of the character for the box */
  void setBox(QRect* box);
  void setCharacter(Person* character);

  /* Size of the box */
  void setWidth(uint new_value);
  void setHeight(uint new_value);
};

#endif // PERSONSTATUSBAR_H
