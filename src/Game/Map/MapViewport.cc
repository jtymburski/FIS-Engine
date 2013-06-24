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

#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapViewport::MapViewport()
{
  /* Set some viewport parameters for optimal painting */
  setAutoFillBackground(false);
  setOptimizationFlag(QGraphicsView::DontSavePainterState, true);

  /* Remove the scroll bars and frames for the viewport */
  setFrameShape(QFrame::NoFrame);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MapViewport::MapViewport(QGraphicsScene* scene, short resolution_x,
                         short resolution_y, QWidget* parent)
{
  /* The initial viewport setup */
  QGraphicsView(scene, parent);
  setScene(scene);

  /* Set viewport parameters */
  setMaximumWidth(resolution_x);
  setMaximumHeight(resolution_y);
  setMinimumWidth(resolution_x);
  setMinimumHeight(resolution_y);
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

bool MapViewport::event(QEvent* event)
{
  /* Only allow the events that are being used. Disable everything else */
  if(event->type() == QEvent::KeyPress || 
     event->type() == QEvent::KeyRelease)
  {
    return QGraphicsView::event(event);
  }

  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

bool MapViewport::lockOn(int x, int y)
{
  if(x >= 0 && y >= 0)
  {
    lock_on_item = 0;
    lock_on_x = x;
    lock_on_y = y;
    lock_on = COORDINATE;
    return true;
  }

  return false;
}

bool MapViewport::lockOn(QGraphicsItem* item)
{
  if(item != 0)
  {
    lock_on_item = item;
    lock_on_x = 0;
    lock_on_y = 0;
    lock_on = ITEM;
    return true;
  }

  return false;
}

void MapViewport::updateView()
{
  if(lock_on == COORDINATE)
    centerOn(lock_on_x, lock_on_y);
  else if(lock_on == ITEM)
    centerOn(lock_on_item);
}
