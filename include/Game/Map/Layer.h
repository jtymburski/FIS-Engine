/******************************************************************************
* Class Name: Layer
* Date Created: May 10, 2013
* Inheritance: QGraphicsPixmapItem
* Description: This class becomes the middle interface in between a sprite 
*              and converting it into the data needed to add it to the
*              QGraphicsScene. Essentially it just offers the re-implemented
*              functions needed to define a sprite.
******************************************************************************/
#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsPixmapItem>

class Layer : public QGraphicsPixmapItem
{
public:
  /* Constructor function */
  Layer();

  /* Destructor function */
  ~Layer();

private:
  /* The map to be displayed */
  //Map* display_map;
};

#endif // LAYER_H
