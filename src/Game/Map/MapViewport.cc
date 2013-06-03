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
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setAutoFillBackground(true);
  setOptimizationFlag(QGraphicsView::DontSavePainterState, true);

  /* Remove the scroll bars and frames for the viewport */
  setFrameShape(QFrame::NoFrame);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  /* Shifting handlers */
  direction = NONE;
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

  /* Shifting handlers */
  direction = NONE;
}

MapViewport::~MapViewport()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

void MapViewport::addDirection(MovementDirection new_direction)
{
  direction_stack.append(new_direction);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void MapViewport::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Escape)
    closeMap();
  else if(event->key() == Qt::Key_Down)
    addDirection(SOUTH);
  else if(event->key() == Qt::Key_Up)
    addDirection(NORTH);
  else if(event->key() == Qt::Key_Right)
    addDirection(EAST);
  else if(event->key() == Qt::Key_Left)
    addDirection(WEST);
  else if(event->key() == Qt::Key_A)
    emit animateTiles();
}

void MapViewport::keyReleaseEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down  && direction_stack.contains(SOUTH))
    direction_stack.remove(direction_stack.indexOf(SOUTH));
  else if(event->key() == Qt::Key_Up && direction_stack.contains(NORTH))
    direction_stack.remove(direction_stack.indexOf(NORTH));
  else if(event->key() == Qt::Key_Left && direction_stack.contains(WEST))
    direction_stack.remove(direction_stack.indexOf(WEST));
  else if(event->key() == Qt::Key_Right && direction_stack.contains(EAST))
    direction_stack.remove(direction_stack.indexOf(EAST));
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

int MapViewport::newX(int old_x)
{
  /* Shift the X, based on the direction */
  if(direction == EAST)
    return old_x + 1;
  else if(direction == WEST && old_x > 0)
    return old_x - 1;

  return old_x;
}

int MapViewport::newY(int old_y)
{
  /* Shift the Y, based on the direction */
  if(direction == SOUTH)
    return old_y + 1;
  else if(direction == NORTH && old_y > 0)
    return old_y - 1;

  return old_y;
}

void MapViewport::updateDirection(int x, int y)
{
  /* Once a tile end has reached, cycle the direction */
  if(x % 64 == 0 && y % 64 == 0)
  {
    if(direction_stack.isEmpty())
      direction = NONE;
    else
      direction = direction_stack.last();
  }
}
