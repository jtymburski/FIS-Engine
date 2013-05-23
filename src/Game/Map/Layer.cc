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

/* 
 * Description: Constructor function - Sets up a blank layer
 *
 * Input: none
 */
Layer::Layer()
{
  /* Initialize the item to a blank slate */
  item = 0;
  width = 0;
  height = 0;
  visible = true;
}

/* 
 * Description: Constructor function - Sets up the layer by defining the
 *              sprite, width and height. Optional variables include the X and 
 *              Y value of the widget as well as the layer in the scene (Z).
 *
 * Inputs: Sprite* item - the new sprite to create the layer around
 *         int width - the pixel width of the tile layer
 *         int height - the pixel height of the tile layer
 *         int x - the x location in the scene for the tile layer
 *         int y - the y location in the scene for the tile layer
 *         int z - the layer depth for the QGraphicsScene
 */
Layer::Layer(Sprite* item, int width, int height, int x, int y, int z)
{
  /* Initialize the item */
  this->item = item;
  this->width = width;
  this->height = height;
  visible = true;
  setEnabled(false);

  /* Set the coordinates */
  setX(x);
  setY(y);
  setZValue(z);
}

/* 
 * Description: Destructor function 
 */
Layer::~Layer()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Implemented virtual function. Returns the bounding rectangle
 *              around the image that is being added into the scene. 
 *
 * Inputs: none
 * Output: QRectF - the rectangle around the internal image
 */
QRectF Layer::boundingRect() const
{
  QRectF rect(0, 0, width, height);

  return rect;
}

/* 
 * Description: Clears out the class by deleting the necessary sprite pointer
 *              (if set) and then clears all the internal variables.
 *
 * Inputs: none
 * Output: none
 */
void Layer::clear()
{
  if(item != 0)
    delete item;
  item = 0;

  width = 0;
  height = 0;
}

/* 
 * Description: Gets the sprite item encapsulated by the layer
 *
 * Inputs: none
 * Output: Sprite* - the sprite housed within this layer
 */
Sprite* Layer::getItem()
{
  return item;
}

/* 
 * Description: Returns if the layer is set to be visible
 *
 * Inputs: none
 * Output: bool - the layer visibility status
 */
bool Layer::isVisible()
{
  return visible;
}

/* 
 * Description: Reimplemented virtual function. Handles the painting of the 
 *              image stored within the layer. Runs based on the same data as 
 *              the above QRectF for the bounding box. Only paints if the tile 
 *              is set.
 *
 * Inputs: QPainter* painter - the painter to send all the image data to
 *         QStyleOptionGraphicsItem* option - any painting options.
 *         QWidget* widget - the related widget, if applicable
 * Output: none
 */
void Layer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
                  QWidget* widget)
{
  /* Remove unused parameter warnings */
  (void)option;
  (void)widget;

  /* Set painter information */
  painter->setBrush(QBrush(Qt::black));
  painter->setPen(Qt::NoPen);

  /* Only paint if enabled */
  if(isEnabled())
  {
    if(isVisible())
    {
      /* Only paint if sprite exists */
      if(item != 0)
        painter->drawPixmap(0, 0, width, height, item->getCurrent());
    }
    else
    {
      painter->drawRect(QRectF(0, 0, width, height));
    }
  }
}

/* 
 * Description: Sets if the sprite layer is visible
 *
 * Inputs: bool status - the visibility status
 * Output: none
 */
void Layer::setVisible(bool status)
{
  visible = status;
}
