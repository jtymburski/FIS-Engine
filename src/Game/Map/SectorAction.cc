/******************************************************************************
* Class Name: SectorAction
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The SectorAction class
******************************************************************************/
#include "Game/Map/SectorAction.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
SectorAction::SectorAction(QWidget* parent) : QWidget(parent)
{
}

/* Destructor function */
SectorAction::~SectorAction()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* True if the action can be performed */
bool SectorAction::isValid()
{
    return true;
}

/* Performs the sector action */
void SectorAction::perform()
{
}
