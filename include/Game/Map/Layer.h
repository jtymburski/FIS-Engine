/******************************************************************************
* Class Name: Layer
* Date Created: May 10, 2013
* Inheritance: QGraphicsItem
* Description: This class becomes the middle interface in between a sprite 
*              and converting it into the data needed to add it to the
*              QGraphicsScene. Essentially it just offers the re-implemented
*              functions needed to define a sprite.
******************************************************************************/
#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItem>
#include <QPainter>

#include "Game/Sprite.h"

class Layer : public QGraphicsItem
{
public:
  /* Constructor functions */
  Layer();
  Layer(Sprite* item, int width, int height, 
        int x = 0, int y = 0, int z = 0);

  /* Destructor function */
  ~Layer();

private:
  /* The information that defines the item */
  Sprite* item;
  int width;
  int height;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Virtual bounding rectangle - The rectangle that encapsulates the item */
  QRectF boundingRect() const;

  /* Clear out the layer definition */
  void clear();

  /* Returns the sprite stored in the layer for control */
  Sprite* getItem();

  /* Virtual painter reimplementation - for painting the item */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
             QWidget* widget);
};

#endif // LAYER_H
