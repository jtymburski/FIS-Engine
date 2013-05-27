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
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void MapViewport::keyPressEvent(QKeyEvent* event)
{ 
  if(event->key() == Qt::Key_Escape)
    closeMap();
  else if(event->key() == Qt::Key_Down)
    direction.append(DOWN);
  else if(event->key() == Qt::Key_Up)
    direction.append(UP);
  else if(event->key() == Qt::Key_Right)
    direction.append(RIGHT);
  else if(event->key() == Qt::Key_Left)
    direction.append(LEFT);
  else if(event->key() == Qt::Key_A)
    emit animateTiles();
  else if(event->key() == Qt::Key_J)
    shiftViewport();
}

void MapViewport::keyReleaseEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down  && direction.contains(DOWN))
    direction.remove(direction.indexOf(DOWN));
  else if(event->key() == Qt::Key_Up && direction.contains(UP))
    direction.remove(direction.indexOf(UP));
  else if(event->key() == Qt::Key_Left && direction.contains(LEFT))
    direction.remove(direction.indexOf(LEFT));
  else if(event->key() == Qt::Key_Right && direction.contains(RIGHT))
    direction.remove(direction.indexOf(RIGHT));
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
  if(direction.endsWith(DOWN))
    verticalScrollBar()->setSliderPosition(
                              verticalScrollBar()->sliderPosition() + 1);
  else if(direction.endsWith(UP))
    verticalScrollBar()->setSliderPosition(
                              verticalScrollBar()->sliderPosition() - 1);
  else if(direction.endsWith(LEFT))
    horizontalScrollBar()->setSliderPosition(
                              horizontalScrollBar()->sliderPosition() - 1);
  else if(direction.endsWith(RIGHT))
    horizontalScrollBar()->setSliderPosition(
                              horizontalScrollBar()->sliderPosition() + 1);
}
