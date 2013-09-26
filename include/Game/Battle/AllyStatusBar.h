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

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include "Game/Player/Person.h"
#include "Game/Battle/PersonStatusBar.h"

class AllyStatusBar : public PersonStatusBar
{
public:
  /* Constructs an AllyStatusBar object */
  AllyStatusBar(Person*, uint width, uint height, QWidget* parent);

  /* Annihilates an AllyStatusBar object */
  virtual ~AllyStatusBar();

  /* QLabel for the name box and QD */
  QLabel* name_label;
  QLabel* qd_label;

  /* QGradient for the QD value */
  QGradient* qd_grad;

  /* QRects for the outline box and gradient box for QD */
  QRect* qd_bar;
  QRect* qd_outline;

/*============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/
protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

  /* Setup */
  void additionalSetup();

  /* Rebuild the status boxes */
  void rebuildStatusBoxes();

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Gets for Stat displays */
  QString getDisplayQD();
};

#endif // PERSONSTATUSBAR_H
