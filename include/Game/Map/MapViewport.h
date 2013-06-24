/******************************************************************************
* Class Name: MapViewport
* Date Created: April 24, 2013
* Inheritance: QGraphicsView
* Description: This class handles the viewport that sits on top of the map 
*              class to allow for proper viewing. This will be the front 
*              interface with the outside classes for allowing viewing to the
*              map itself.
******************************************************************************/
#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

//#include <QDebug>
#include <QEvent>
#include <QGraphicsView>
#include <QScrollBar>

class MapViewport : public QGraphicsView
{
  Q_OBJECT

public:
  /* Constructor functions */
  MapViewport();
  MapViewport(QGraphicsScene* scene, short resolution_x, 
              short resolution_y, QWidget* parent = 0);

  /* Destructor function */
  ~MapViewport();

  /* Public enumerators */

  /* This controls what the viewport locks to 
   * COORDINATE - lock the viewport on a specific coordinate tile
   * ITEM - lock to a qgraphicsitem, which has the possibility of moving */
  enum LockStatus{COORDINATE, ITEM};
  
private:
  /* The lock qualifiers, for who the viewport is centered on */
  MapViewport::LockStatus lock_on;
  QGraphicsItem* lock_on_item;
  int lock_on_x;
  int lock_on_y;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  bool event(QEvent* event);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* To change what the viewport locks to, when the update is called */
  bool lockOn(int x, int y);
  bool lockOn(QGraphicsItem* item);

  /* Updates the viewport, call at the end of the update call in the scene */
  void updateView();
};

#endif // MAPVIEWPORT_H
