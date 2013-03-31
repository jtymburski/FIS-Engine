/******************************************************************************
* Class Name: Sector
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Sector class
******************************************************************************/
#include "Game/Map/Sector.h"

/* Constructor function */
Sector::Sector(QWidget* parent) : QWidget(parent)
{
}

/* Destructor function */
Sector::~Sector()
{
}

/* Painting function */
void Sector::paintEvent(QPaintEvent* event)
{
    (void)event;//warning
}

/* Sets all tiles in the sector to the given Status */
void Sector::setStatus(Tile::Status status)
{
    (void)status;
}
