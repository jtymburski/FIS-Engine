/******************************************************************************
* Class Name: MapStatusBar
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The MapStatusBar class, this represents the dialog box in the
*              map.
******************************************************************************/
#include "Game/Map/MapStatusBar.h"

/* Constructor function */
MapStatusBar::MapStatusBar(QWidget* parent) : QWidget(parent)
{
}

/* Destructor function */
MapStatusBar::~MapStatusBar()
{
}

/* Painting function */
void MapStatusBar::paintEvent(QPaintEvent* event)
{
    (void)event;//warning
}

/* Sets up a conversation with the given persons */
void MapStatusBar::initConversation(MapPerson* left, MapPerson* right)
{
    (void)left;//warning
    (void)right;//warning
}

/* Sets up the popout box */
void MapStatusBar::initPopout(QImage* img, QString* dialog)
{
    (void)img;//warning
    (void)dialog;//warning
}

/* Proceeds in the conversation, enter key triggers this */
void MapStatusBar::proceed()
{
}
