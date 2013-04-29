/******************************************************************************
* Class Name: MapViewport
* Date Created: April 24, 2013
* Inheritance: QScrollArea
* Description: This class handles the viewport that sits on top of the map 
*              class to allow for proper viewing. This will be the front 
*              interface with the outside classes for allowing viewing to the
*              map itself.
******************************************************************************/
#include "Game/Map/MapViewport.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapViewport::MapViewport(QWidget* parent)
{
  display_map = new Map();

  setWidget(display_map);
  setGeometry(0, 0, 1216, 704);
  setFrameShape(QFrame::NoFrame);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MapViewport::~MapViewport()
{

}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void MapViewport::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Escape)
    closeMap();
  else if(event->key() == Qt::Key_Down)
    verticalScrollBar()->setValue(verticalScrollBar()->value() + 8);
  else if(event->key() == Qt::Key_Up)
    verticalScrollBar()->setValue(verticalScrollBar()->value() - 8);
  else if(event->key() == Qt::Key_Right)
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 8);
  else if(event->key() == Qt::Key_Left)
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 8);
}

//void MapViewport::wheelEvent(QWheelEvent* event)
//{
//  /* Do nothing */
//  (void)event;
//}

/*============================================================================
 * SLOTS
 *===========================================================================*/

void MapViewport::closeMap()
{
  emit closingMap(2);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

Map* MapViewport::getMap()
{
  return display_map;
}
