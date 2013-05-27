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
   * UP   - move viewport up
   * DOWN - move viewport down
   * LEFT - move viewport to the left
   * RIGHT - move viewport to the right */
  enum MovementDirection{NONE, UP, DOWN, LEFT, RIGHT};

private:
  /* The current direction that the map is moving in */
  QVector<MovementDirection> direction;

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

  /* Shifts the viewport, based on what directional keys are pressed */
  void shiftViewport();
};

#endif // MAPVIEWPORT_H
