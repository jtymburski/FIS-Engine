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

#include <QGraphicsView>
#include <QKeyEvent>
#include <QScrollBar>
#include <QWheelEvent>

class MapViewport : public QGraphicsView
{
  Q_OBJECT

public:
  /* Constructor functions */
  MapViewport();
  MapViewport(QGraphicsScene* scene, QWidget* parent = 0);

  /* Destructor function */
  ~MapViewport();

  /* NONE - not currently moving the viewport
   * NORTH - move viewport up
   * EAST  - move viewport to the right
   * SOUTH - move viewport down
   * WEST  - move viewport to the left */
  enum MovementDirection{NONE, NORTH, EAST, SOUTH, WEST};

private:
  /* The current direction that the map is moving in */
  MovementDirection direction;

  /* The stack of currently pressed directions, on the keyboard */
  QVector<MovementDirection> direction_stack;

private:
  void addDirection(MovementDirection new_direction);

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);
  void wheelEvent(QWheelEvent* event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Call to animate the tiles in the scene - TODO: temporary? */
  void animateTiles();

  /* Closes the map, based on a key command - TODO: temporary? */
  void closingMap(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Initializes the closes map call based on a signal from the viewport */
  void closeMap();

  /* Shifting calls, based on keys being pressed and the status of the View */
  int newX(int old_x);
  int newY(int old_y);

  /* Updates the direction, based on the TICK */
  void updateDirection(int x, int y);
};

#endif // MAPVIEWPORT_H
