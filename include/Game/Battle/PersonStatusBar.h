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

  /* Person's health */
  uint health;

  /* Person's max health */
  uint health_max;

  /* The qd bar bounding box (scales with usage) */
  uint qd;

  /* Person's maximum qd */
  uint qd_max;

  /* Name text label */
  QLabel* name_label;

  /* Object Bounding Boxes */
  QVector<QRect*> status_thumbs;
  QRect* health_outline;
  QRect* health_bar;
  QRect* qd_outline;
  QRect* qd_bar;
  QRect* name_box;

  /* Person's name */
  QString name;

  /* Person pointer the status bar is constructed for */
  Person* character;

  /* Sprite pointers to status_thumbnails */
  QVector<Sprite*> status_thumb_sprites;

protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);
  
public:
  /* Gets for Stat displays */
  uint getDisplayHP();
  uint getDisplayMaxHP();
  uint getDisplayQD();
  uint getDisplayMaxQD();

  /* Get size of box */
  uint getWidth();
  uint getHeight();

  /* Gets the name of the character for the box */
  QString getName();
  void setBox(QRect* box);
  void setCharacter(Person* character);

  /* Sets current HP for display */
  void setDisplayHP(uint amount);
  void setDisplayMaxHP(uint amount);
  void setDisplayQD(uint amount);
  void setDisplayMaxQD(uint amount);

  /* Sets the name of the Person */
  void setName(QString name);

  /* Size of the box */
  void setWidth(uint new_value);
  void setHeight(uint new_value);
};

#endif // PERSONSTATUSBAR_H
