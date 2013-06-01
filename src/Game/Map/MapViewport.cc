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
 * PRIVATE FUNCTIONS
 *===========================================================================*/

void MapViewport::addDirection(MovementDirection new_direction)
{
  if(direction_stack.isEmpty() && direction == NONE)
    direction = new_direction;
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
    addDirection(DOWN);
  else if(event->key() == Qt::Key_Up)
    addDirection(UP);
  else if(event->key() == Qt::Key_Right)
    addDirection(RIGHT);
  else if(event->key() == Qt::Key_Left)
    addDirection(LEFT);
  else if(event->key() == Qt::Key_A)
    emit animateTiles();
}

void MapViewport::keyReleaseEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down  && direction_stack.contains(DOWN))
    direction_stack.remove(direction_stack.indexOf(DOWN));
  else if(event->key() == Qt::Key_Up && direction_stack.contains(UP))
    direction_stack.remove(direction_stack.indexOf(UP));
  else if(event->key() == Qt::Key_Left && direction_stack.contains(LEFT))
    direction_stack.remove(direction_stack.indexOf(LEFT));
  else if(event->key() == Qt::Key_Right && direction_stack.contains(RIGHT))
    direction_stack.remove(direction_stack.indexOf(RIGHT));
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

void MapViewport::shiftViewport()
{
  if(direction != NONE)
  {
    if(direction == DOWN)
      verticalScrollBar()->setSliderPosition(
                              verticalScrollBar()->sliderPosition() + 1);
    else if(direction == UP)
      verticalScrollBar()->setSliderPosition(
                              verticalScrollBar()->sliderPosition() - 1);
    else if(direction == LEFT)
      horizontalScrollBar()->setSliderPosition(
                              horizontalScrollBar()->sliderPosition() - 1);
    else if(direction == RIGHT)
      horizontalScrollBar()->setSliderPosition(
                              horizontalScrollBar()->sliderPosition() + 1);

    qDebug() << horizontalScrollBar()->sliderPosition() << " " 
             << verticalScrollBar()->sliderPosition() << " "
             << direction_stack << " " 
             << direction;

    /* Only do movement based on tiles */
    if(horizontalScrollBar()->sliderPosition() % 64 == 0 && verticalScrollBar()->sliderPosition() % 64 == 0)
    {
      if(direction_stack.isEmpty())
        direction = NONE;
      else
        direction = direction_stack.last();
    }
  }
  /*
  if(direction_stack.endsWith(DOWN))
    verticalScrollBar()->setSliderPosition(
                              verticalScrollBar()->sliderPosition() + 1);
  else if(direction_stack.endsWith(UP))
    verticalScrollBar()->setSliderPosition(
                              verticalScrollBar()->sliderPosition() - 1);
  else if(direction_stack.endsWith(LEFT))
    horizontalScrollBar()->setSliderPosition(
                              horizontalScrollBar()->sliderPosition() - 1);
  else if(direction_stack.endsWith(RIGHT))
    horizontalScrollBar()->setSliderPosition(
                              horizontalScrollBar()->sliderPosition() + 1);
  */
}
