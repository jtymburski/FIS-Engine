/******************************************************************************
* Class Name: Options
* Date Created: Sunday, December 2, 2012
* Inheritance: None?
* Description: Options menu
*
* TODO: CONSTRUCTORS TO BE FINISHED
*
* Options to add:
*  - Set the text display speed in MapDialog (for the character letters)
******************************************************************************/
#include "Options.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Options::Options(QWidget *parent) : QWidget(parent)
{
}

/* Destructor function */
Options::~Options()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Gets the value of the given option */
int Options::getOption(QString option)
{
    (void)option; //warning
    return 0;
}

/* Sets the given option ot the give value (may require reimplementation) */
void Options::setOption(QString option, int value)
{
    (void)option; //warning
    (void)value; //warning
}
