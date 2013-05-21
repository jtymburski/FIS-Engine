/******************************************************************************
* Class Name: Layer
* Date Created: May 10, 2013
* Inheritance: QGraphicsItem
* Description: This class becomes the middle interface in between a sprite 
*              and converting it into the data needed to add it to the
*              QGraphicsScene. Essentially it just offers the re-implemented
*              functions needed to define a sprite.
******************************************************************************/
#include "Game/Map/Layer.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

Layer::Layer()
{
  /* Initialize the item to a blank slate */
  item = 0;
  width = 0;
  height = 0;
}

Layer::Layer(Sprite* new_item, int width, int height, int x, int y, int z)
{
  /* Initialize the item */
  item = new_item;
  this->width = width;
  this->height = height;

  /* Set the coordinates */
  setX(x);
  setY(y);
  setZValue(z);
}

Layer::~Layer()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

QRectF Layer::boundingRect() const
{
  QRectF rect(0, 0, width, height);

  return rect;
}

void Layer::clear()
{
  if(item != 0)
    delete item;
  item = 0;

  width = 0;
  height = 0;
}

Sprite* Layer::getItem()
{
  return item;
}

void Layer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
                  QWidget* widget)
{
  /* Remove unused parameter warnings */
  (void)option;
  (void)widget;

  /* Only paint if sprite exists */
  if(item != 0)
    painter->drawPixmap(0, 0, width, height, item->getCurrent());
}
