/******************************************************************************
* Class Name: MapViewport
* Date Created: April 24, 2013
* Inheritance: QGraphicsView
* Description: This class handles the viewport that sits on top of the map 
*              class to allow for proper viewing. This will be the front 
*              interface with the outside classes for allowing viewing to the
*              map itself.
******************************************************************************/
#include "Game/Map/MapViewport.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapViewport::MapViewport()
{
  setFrameShape(QFrame::NoFrame);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MapViewport::MapViewport(QGraphicsScene* scene, QWidget* parent)
{
  /* The initial viewport setup */
  QGraphicsView(scene, parent);
  setScene(scene);

  /* Set viewport parameters */
  setMaximumWidth(1216);
  setMaximumHeight(704);
  setMinimumWidth(1216);
  setMinimumHeight(704);
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
    verticalScrollBar()->setValue(verticalScrollBar()->value() + 4);
  else if(event->key() == Qt::Key_Up)
    verticalScrollBar()->setValue(verticalScrollBar()->value() - 4);
  else if(event->key() == Qt::Key_Right)
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 4);
  else if(event->key() == Qt::Key_Left)
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 4);
  else if(event->key() == Qt::Key_A)
    emit animateTiles();
}

// TODO [2013-05-23]
void MapViewport::keyReleaseEvent(QKeyEvent* event)
{
  /* Do nothing, right now */
  (void)event;
}

void MapViewport::wheelEvent(QWheelEvent* event)
{
  /* Do nothing */
  (void)event;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

void MapViewport::closeMap()
{
  emit closingMap(2);
}
