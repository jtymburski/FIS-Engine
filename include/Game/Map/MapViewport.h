/******************************************************************************
* Class Name: MapViewport
* Date Created: April 24, 2013
* Inheritance: QScrollArea
* Description: This class handles the viewport that sits on top of the map 
*              class to allow for proper viewing. This will be the front 
*              interface with the outside classes for allowing viewing to the
*              map itself.
******************************************************************************/
#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

//#include <QDebug>
//#include <QEvent>
//#include <QGraphicsView>
#include <QKeyEvent> // TODO: delete?
#include <QScrollArea>
#include <QScrollBar> // TODO: delete?

class MapViewport : public QScrollArea
{
//  Q_OBJECT
//
public:
  /* Constructor functions */
  MapViewport();
  MapViewport(short width, short height);

  /* Destructor function */
  ~MapViewport();

  /* Public enumerators */

  /* This controls what the viewport locks to 
   * COORDINATE - lock the viewport on a specific coordinate tile
   * ITEM - lock to a qgraphicsitem, which has the possibility of moving */
//  enum LockStatus{COORDINATE, ITEM};
  
private:
  /* The size of the viewport */
  short height;
  short width;

  /* The lock qualifiers, for who the viewport is centered on */
//  MapViewport::LockStatus lock_on;
//  QGraphicsItem* lock_on_item;
//  int lock_on_x;
//  int lock_on_y;

  /*------------------- Constants -----------------------*/
  const static int kMIN_HEIGHT; /* Minimum viewport height, in pixels */
  const static int kMIN_WIDTH;  /* Minimum viewport length, in pixels */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
//  bool event(QEvent* event);

  /* TODO: remove - for testing */
  void keyPressEvent(QKeyEvent* event);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the size of the viewport */
  short getHeight();
  short getWidth();

  /* To change what the viewport locks to, when the update is called */
//  bool lockOn(int x, int y);
//  bool lockOn(QGraphicsItem* item);

  /* Sets the size of the viewport */
  void setSize(short width, short height);

  /* Updates the viewport, call at the end of the update call in the scene */
//  void updateView();
};

#endif // MAPVIEWPORT_H
