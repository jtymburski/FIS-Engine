/******************************************************************************
* Class Name: PersonStatusBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Displays iformation for health and QD to the screen
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/

#include "Game/Battle/PersonStatusBar.h"

/* Creates a PersonStatusBar Obect */
PersonStatusBar::PersonStatusBar(QWidget* parent)
{
}

/* Annihilates a PersonStatusBar Object */
PersonStatusBar::~PersonStatusBar()
{
}

void PersonStatusBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
}

/* Sets current HP for display */
void PersonStatusBar::setDisplayHP(int new_health)
{
  health = new_health;
}

/* Sets current QD for display */
void PersonStatusBar::setDisplayQD(int new_qd)
{
  qd = new_qd;
}

/* Sets current max HP for display */
void PersonStatusBar::setDisplayMaxHP(int new_max_health)
{
  health_max = new_max_health;
}

/* Sets current max QD for display */
void PersonStatusBar::setDisplayMaxQD(int new_max_qd)
{
  qd_max = new_max_qd;
}
