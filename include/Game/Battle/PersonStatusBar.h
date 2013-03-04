/*******************************************************************************
* Class Name: Status Bar
* Date Created: Saturday, March 2nd, 2013
* Inheritance: Parent class: QWidget (Contained within BattleStatusBar)
* Description: Superclass of Enemy and Person Status Class
*******************************************************************************/

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
  /* Creates a StatusBar object */
  PersonStatusBar(Person* charcter, uint w, uint h, QWidget* parent = 0);

  /* Annihilates a StatusBar object */
  virtual ~PersonStatusBar();

protected:


  // virtual void paintEvent(QPaintEvent*);
  virtual void setup();
  virtual void additionalSetup() = 0;

  void setCharacter(Person* character);

  /* Static const limits */
  static const uint kMAX_WIDTH  = 1920 / 5;
  static const uint kMAX_HEIGHT = 200;

  /* Person the status bar is constructed for */
  Person* character;

  /* Level and Health Labels */
  QLabel* level_label;
  QLabel* health_label;

  /* Gradients */
  QLinearGradient* health_grad;

  /* Object Bounding Boxes */
  QRect* health_outline;
  QRect* health_bar;

  /* Vectors for status thumbnails */
  QVector<Sprite*> status_thumb_sprites;
  QVector<QRect*> status_thumbs;

  /* Dimensions of the Bar */
  uint bar_height;
  uint bar_width;

public:
  /* Virtual Display functions */
  QString getDisplayHP();
  QString getDisplayLevel();

  /* Get size of box */
  uint getWidth();
  uint getHeight();

  /* Set size of box */
  void setWidth(uint new_value);
  void setHeight(uint new_value);
};

#endif // STATUSBAR_H
