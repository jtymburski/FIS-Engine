/******************************************************************************
* Class Name: PersonStatusBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Person status bar displays five thumbnails displaying the
*              ailments or buffs affecting the character, her health current
*              and maximum values, her quantum drive curretn and maximum values
*              and the character's name.
*
* Notes [1]: In Battle, Base Stats are the curent maximum values, and Temp
*            stats are current value, and the Normal stat set act as the Normal
*            Maximum values (without buffs and debuffs, etc.)
*
******************************************************************************/

#include "Game/Battle/PersonStatusBar.h"


PersonStatusBar::PersonStatusBar(Person* character, uint width, uint height, QWidget* parent)
{
  bar_width = width;
  bar_height = height;
  this->character = character;
  setName(character->getName());
}

PersonStatusBar::~PersonStatusBar()
{
}

void PersonStatusBar::paintEvent(QPaintEvent*)
{
    /* Preparation */
    QPainter painter(this);

    setDisplayMaxHP(character->getBaseVitality());
    setDisplayMaxQD(character->getBaseQuantumDrive());
    setDisplayHP(character->getTempVitality());
    setDisplayQD(character->getTempQuantumDrive());


    /* Print character name */

    /* Print status thumbs */

    /* Print Health */

    /* Print QD */
}


QString PersonStatusBar::getName()
{
  return name;
}

void PersonStatusBar::setDisplayHP(int new_health)
{
  health = new_health;
}


void PersonStatusBar::setDisplayQD(int new_qd)
{
  qd = new_qd;
}


void PersonStatusBar::setDisplayMaxHP(int new_max_health)
{
  health_max = new_max_health;
}

void PersonStatusBar::setDisplayMaxQD(int new_max_qd)
{
  qd_max = new_max_qd;
}

void PersonStatusBar::setName(QString name)
{
  this->name = name;
}
