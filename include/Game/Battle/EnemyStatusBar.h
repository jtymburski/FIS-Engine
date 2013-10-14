/*******************************************************************************
* Class Name: Enemy Status Bar
* Date Created: Saturday, March 2nd, 2013
* Inheritance: Parent class: QWidget (Contained within Battle)
* Description: Displays information on enemies on the main battle menu,
*              including their health, thumbnails of inflicted status ailments,
*              and their level.
*******************************************************************************/
#ifndef ENEMYSTATUSBAR_H
#define ENEMYSTATUSBAR_H

#include <QLabel>
#include <QWidget>

#include "Game/Player/Person.h"
#include "Game/Battle/PersonStatusBar.h"

class EnemyStatusBar : public PersonStatusBar
{
public:
  /* Constructs an EnemyStatusBar object */
  EnemyStatusBar(Person* character, uint width, uint height, QWidget* parent);

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *===========================================================================*/
  /* Annihilates an EnemyStatusBar object */
 virtual ~EnemyStatusBar();

/*=============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

  /* Additional Setup after Parent's */
  void additionalSetup();
  void rebuildStatusBoxes() {}
};

#endif // ENEMYSTATUSBAR_H
