/******************************************************************************
* Class Name: PersonStatusBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: StatusBar
* Description: Displays information on persons on the main battle menu,
*              including their health, thumbnails of inflicted statu ailments,
*              and their level.
******************************************************************************/
#ifndef ALLYSTATUSBAR_H
#define ALLYSTATUSBAR_H

#include <QLabel>
#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include "Game/Battle/PersonStatusBar.h"
#include "Game/Player/Person.h"

#include <QDebug>

class AllyStatusBar : public PersonStatusBar
{
public:
  /* Constructs an AllyStatusBar object */
  AllyStatusBar(Person*, uint width, uint height, QWidget* parent);

  /* Annihilates an AllyStatusBar object */
  virtual ~AllyStatusBar();

protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

  /* Setup */
  void additionalSetup();

  /* Rebuild the status boxes */
  void rebuildStatusBoxes();

  /* Name & QD Labels */
  QLabel* name_label;
  QLabel* qd_label;

  /* Additional Bounding Boxes */
  QRect* qd_outline;
  QRect* qd_bar;
  QRect* name_box;

  /* Additional Gradients */
  QLinearGradient* qd_grad;



public:
  /* Gets for Stat displays */
  QString getDisplayQD();

};

#endif // PERSONSTATUSBAR_H
