/******************************************************************************
* Class Name: BattleInfoBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Display information on the screen about battle events
*
*  TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/
#include "Game/Battle/BattleInfoBar.h"
#include <QDebug>

/*==============================================================================
 * CONSTANTS
 *============================================================================*/
const ushort BattleInfoBar::kMAX_TIME = 10; /* maximum time to display a msg */
const ushort BattleInfoBar::kMAX_CHAR = 75; /* # of characters in a message */
const ushort BattleInfoBar::kWAIT_TIME = 1; /* waiting time for transition */

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a BattleInfoBar
 *
 * Inputs: QWidget* - pointer to the parent object
 */
BattleInfoBar::BattleInfoBar(QWidget* parent) : QWidget(parent) {}

/*
 * Description: Destroys a BattleInfoBar object
 */
BattleInfoBar::~BattleInfoBar() {}

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Sets the display time
 *
 * Inputs: uint - new time to set the BattleInfoBar to
 * Output: none
 */
void BattleInfoBar::setDisplayTime(uint new_time)
{
  display_time = new_time;
}

/*
 * Description: Sets the info to be displayed
 *
 * Inputs: QString - new info
 * Output:
 */
void BattleInfoBar::setInfo(QString new_info)
{
  info = new_info;
}

/*==============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
}

/*==============================================================================
 * SIGNALS
 *============================================================================*/

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds an amount of time to the display_time (up to kMAX_TIME)
 *
 * Inputs: uint - value of time to be added
 * Output: none
 */
void BattleInfoBar::addTime(uint value)
{
  display_time + value < kMAX_TIME ? display_time += value :
                                     display_time = kMAX_TIME;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint BattleInfoBar::getDisplayTime()
{
  return display_time;
}

/*
 * Description: Returns the height of the Battle Info Bar
 *
 * Inputs: none
 * Output: uint - the height of the bar
 */
uint BattleInfoBar::getHeight()
{
  return bar_height;
}

/*
 * Description: Returns the width of the Battle Info Bar
 *
 * Inputs: none
 * Output: uint - the width of the bar
 */
uint BattleInfoBar::getWidth()
{
  return bar_width;
}

/*
 * Description: Sets the message on the Battle Info bar
 *
 * Inputs: none
 * Output: uint - the width of the bar
 */
void BattleInfoBar::setMessage(QString new_message, uint new_time)
{
  setInfo(new_message);
  setDisplayTime(new_time);
}

/*
 * Description: Sets the height of the Battle Info Bar (5% of screen height)
 *
 * Inputs: uint - new height of the screen
 * Output: none
 */
void BattleInfoBar::setHeight(uint new_height)
{
  bar_height = new_height;
}

/*
 * Description: Sets the width of the Battle Info Bar (usually the screen)
 *
 * Inputs: uint - new value for the width of the screen
 * Output: none
 */
void BattleInfoBar::setWidth(uint new_width)
{
  bar_width = new_width;
}
