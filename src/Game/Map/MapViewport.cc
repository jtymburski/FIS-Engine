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

/* Constant Implementation - see header file for descriptions */
const int MapViewport::kMIN_HEIGHT = 320;
const int MapViewport::kMIN_WIDTH = 320;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapViewport::MapViewport()
{
  /* Set some viewport parameters for optimal painting */
  setAutoFillBackground(false);

  /* Set viewport height boundaries */
  setSize(0, 0);

  /* Remove the scroll bars and frames for the viewport */
  setFrameShape(QFrame::NoFrame);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MapViewport::MapViewport(short width, short height)
{
  /* Set some viewport parameters for optimal painting */
  setAutoFillBackground(false);

  /* Set viewport height boundaries */
  setSize(width, height);

  /* Remove the scroll bars and frames for the viewport */
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

//bool MapViewport::event(QEvent* event)
//
//{
//  /* Only allow the events that are being used. Disable everything else */
//  if(event->type() == QEvent::KeyPress || 
//     event->type() == QEvent::KeyRelease)
//  {
//    return QGraphicsView::event(event);
//  }
//
//  return false;
//}

void MapViewport::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down)
    verticalScrollBar()->
                setSliderPosition(verticalScrollBar()->sliderPosition() + 2);
  else if(event->key() == Qt::Key_Up)
    verticalScrollBar()->
                setSliderPosition(verticalScrollBar()->sliderPosition() - 2); 
  else if(event->key() == Qt::Key_Right)
    horizontalScrollBar()->
                setSliderPosition(horizontalScrollBar()->sliderPosition() + 2); 
  else if(event->key() == Qt::Key_Left)
    horizontalScrollBar()->
                setSliderPosition(horizontalScrollBar()->sliderPosition() - 2); 
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

short MapViewport::getHeight()
{
  return height;
}

short MapViewport::getWidth()
{
  return width;
}

/*
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
}*/

void MapViewport::setSize(short width, short height)
{
  /* Set the new width */
  if(width > kMIN_WIDTH)
    this->width = width;
  else
    this->width = kMIN_WIDTH;

  /* Set the new height */
  if(height > kMIN_HEIGHT)
    this->height = height;
  else
    this->height = kMIN_HEIGHT;

  /* Sets the width and height boundaries within the class */
  setMaximumWidth(width);
  setMaximumHeight(height);
  setMinimumWidth(width);
  setMinimumHeight(height);
}

/*
void MapViewport::updateView()
{
  if(lock_on == COORDINATE)
    centerOn(lock_on_x, lock_on_y);
  else if(lock_on == ITEM)
    centerOn(lock_on_item);
}*/
