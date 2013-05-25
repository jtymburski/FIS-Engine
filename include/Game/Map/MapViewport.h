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
  /* Constructor function */
  MapViewport(QGraphicsScene* scene, QWidget* parent = 0);

  /* Destructor function */
  ~MapViewport();

private:
  /* The map to be displayed */
  //Map* display_map;

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
  void closingMap(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Initializes the closes map call based on a signal from the viewport */
  void closeMap();
};

#endif // MAPVIEWPORT_H
