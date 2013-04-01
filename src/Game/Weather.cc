/******************************************************************************
* Class Name: Weather
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Weather class
******************************************************************************/
#include "Game/Weather.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Weather::Weather(QWidget* parent) : QWidget(parent)
{
}

/* Destructor function */
Weather::~Weather()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Painting function */
void Weather::paintEvent(QPaintEvent* event)
{
  (void)event;//warning
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Flips the weather bit */
void Weather::toggleEnabled()
{
    enabled = !enabled;
}

/* Gets the weather enabled status */
bool Weather::getEnabled()
{
    return enabled;
}

/* Gets the weather sound */
//QSound* getSound();

/* Gets the weather speed */
int Weather::getSpeed()
{
    return speed;
}

/* Gets the weather sprite */
Sprite* Weather::getSprite()
{
    return texture_overlay;
}

/* Sets the weather enabled */
void Weather::setEnabled(bool yorn)
{
    enabled = yorn;
}

/* Sets the weather sound */
//void setSound(QSound* s);

/* Sets the weather speed */
void Weather::setSpeed(int newspeed)
{
    speed = newspeed;
}

/* Sets the weather sprite */
void Weather::setSprite(Sprite* sprite)
{
    texture_overlay = sprite;
}
