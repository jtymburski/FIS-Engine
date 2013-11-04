/******************************************************************************
* Class Name: BattleInfoBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Display information on the screen about battle events
******************************************************************************/
#include "Game/Battle/BattleInfoBar.h"
#include <QDebug>

/*==============================================================================
 * CONSTANTS - See header file for descriptions
 *============================================================================*/
const ushort kDEFAULT_BAR_HEIGHT    = 100;
const ushort kDEFAULT_BAR_WIDTH     = 1216;
const float  kDEFAULT_BAR_OPACITY   = 0.5;
const float  kDEFUALT_TEXT_OPACITY  = 1.0;
const ushort kDEFAULT_TEXT_SPEED    = 0.5;
const ushort kMARGIN_SIDES          = 25;
const ushort kMARGIN_TOP            = 15;
const ushort kMAX_BUFFER            = 300;
const ushort kMAX_CHAR              = 80;
const ushort kMAX_TIME              = 75;
const ushort kMIN_TIME              =  5;
const ushort kMIN_CHAR              =  5;
const ushort kWAIT_TIME             =  5;

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a BattleInfoBar
 *
 * Inputs: QWidget* - pointer to the parent object
 */
BattleInfoBar::BattleInfoBar(uint bar_height, uint bar_width, QWidget* parent)
  : bar_height(bar_height),
    bar_width(bar_width),
    QWidget(parent)
{


}

/*
 * Description: Destroys a BattleInfoBar object
 */
BattleInfoBar::~BattleInfoBar() {}

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::loadDefaults()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::clear()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::clearAll()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint BattleInfoBar::getBarHeight()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint BattleInfoBar::getBarWidth()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
BattleInfoBar::MessageDisplayMode BattleInfoBar::getMessageDisplayMode()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
BattleInfoBar::TextDisplayMode BattleInfoBar::getTextDisplayMode()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint BattleInfoBar::getTextSpeed()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint BattleInfoBar::getTimeElapsed()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleInfoBar::setBarHeight(uint new_value)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleInfoBar::setBarWidth(uint new_value)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::setMessageDisplayMode(BattleInfoBar::MessageDisplayMode
                                         new_message_display_mode)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::setTextDisplayMode(BattleInfoBar::TextDisplayMode
                                       new_text_display_mode)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleInfoBar::setTextSpeed(uint new_value)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::setTimeElapsed(uint new_value)
{

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
 * PUBLIC SLOTS
 *============================================================================*/

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleInfoBar::addMessage(QString text, short time_visible)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::updateBattleInfoBar(int cycle_time)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float BattleInfoBar::getBarOpacity()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Frame* BattleInfoBar::getBackground()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QFont BattleInfoBar::getTextFont()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float BattleInfoBar::getTextOpacity()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QPen BattleInfoBar::getTextStyle()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleInfoBar::setBarOpacity(float new_value)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::setBackground(Frame* new_backround)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::setTextFont(QFont new_font)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::setTextOpacity(float new_value)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleInfoBar::setTextStyle(QPen new_style)
{

}
