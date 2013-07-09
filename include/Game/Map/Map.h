/******************************************************************************
* Class Name: Map
* Date Created: Oct 28 2012
* Inheritance: QGraphicsScene
* Description: The map class, this is the top level with regard to an actual
*              in-game map. This contains all the tiles that a map is composed
*              of, it also holds pointers to all of the NPC's contained in the
*              maps tiles. This also handles the viewport for the map.
******************************************************************************/
#ifndef MAP_H
#define MAP_H

//#include <QDebug>
#include <QGraphicsScene>
#include <QGLWidget>

#include "Game/Map/MapNPC.h"
#include "Game/Map/Tile.h"
#include "Game/Map/MapMenu.h"
#include "Game/Map/MapStatusBar.h"
#include "Game/Map/MapThing.h"
#include "Game/Map/MapViewport.h"
#include "Game/Map/Sector.h"
#include "Game/Sprite.h"
#include "Game/Weather.h"
#include "FileHandler.h"

class Map : public QGLWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  Map(short viewport_width, short viewport_height);

  /* Destructor function */
  ~Map();

private:
  /* Vector of all ai's on map */
  QVector <MapNPC*> ai;

  /* The actual tiles that comprise a map, dynamically sized */
  QVector <QVector <Tile*> > geography;
  QList<Sprite*> tile_sprites;

  /* Indication if the map has been loaded */
  bool loaded;

  /* The menu on the map */
  MapMenu map_menu;

  /* The status bar on the map */
  MapStatusBar map_status_bar;

  /* The players info on the map */
  MapPerson* player;
  MapThing* thing;

  /* The sectors on the map (for rooms, caves, houses etc) */
  QVector <Sector> sectors;

  /* The timer for handling the tick (temporary?) */
  QTimer timer;

  /* The viewoport for the map, controlled by QGraphicsView */
  MapViewport* viewport;
  QGLWidget* viewport_widget;

  /* Weather effect on the overall map (May be pushed to the sector level at 
   * a later time) */
  Weather* weather_effect;

  /* The painting monitoring parameters */
  QString frames_per_second;
  int frames;
  int paint_animation;
  QTime paint_time;

  /* Testing */
  int shift_index;

  /*------------------- Constants -----------------------*/
  const static int kDOUBLE_DIGITS;    /* The point when integers are more than
                                       * a single digit */
  const static int kELEMENT_ANGLE;    /* Element angle for sprite */
  const static int kELEMENT_DATA;     /* Element data type for sprite */
  const static int kFILE_CLASSIFIER;  /* The file tile classification text */
  const static int kFILE_SECTION_ID;  /* The section identifier, for file */
  const static int kFILE_TILE_COLUMN; /* The tile depth in XML of column tag */
  const static int kFILE_TILE_ROW;    /* The tile depth in XML of row tag */
  const static int kTILE_LENGTH;      /* The tile length, as constant (TEMP) */
  const static int kTILE_WIDTH;       /* The tile width, as constant (TEMP) */
  const static int kVIEWPORT_LENGTH;  /* The viewport length, in tiles */
  const static int kVIEWPORT_WIDTH;   /* The viewport width, in tiles */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  bool addTileData(XmlData data);

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* GL initialization call */
  void initializeGL();

  /* Key Press/Release Events */
  void keyPressEvent(QKeyEvent* keyEvent);
  void keyReleaseEvent(QKeyEvent* keyEvent);

  /* GL painting call */
  void paintGL();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void closingMap(int index);
 
/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  void animate();
  void animateTiles();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Close the map - Handles the details and specifics for closing */
  void closeMap();

  /* Gets a pointer to the NPC in the given position in the NPC vector */
  Person* getNPC(int index);

  /* Gets x position of NPC in the given position in the NPC vector */
  int getNPCx(int index);

  /* Gets y position of NPC in the given position in the NPC vector */
  int getNPCy(int index);

  /* Returns the map person, for access */
  MapPerson* getPlayer();

  /* Returns the map viewport, for displaying the scene to the screen */
  MapViewport* getViewport();

  /* Initial call when map is displayed */
  void initialization();

  /* Causes the thing you are facing and next to start its interactive action */
  void interact(EnumDb::Direction dir);

  /* Checks whether the viewport contains any tiles with the given sector */
  bool isInSector(int index);

  /* Returns if the map has been currently loaded with data */
  bool isLoaded();

  /* Loads the map */
  bool loadMap(QString file);

  /* Shifts the viewport */
  void move(EnumDb::Direction dir, int step_length, Sprite s);

  /* Checks the tile you are attempting to enter for passibility of the given
  direction */
  bool passible(EnumDb::Direction dir, int x, int y);

  /* Causes the thing you are moving into to start its interactive action */
  void passOver();

  /* Unload the map, if there is one loaded */
  void unloadMap();

  /* Changes NPC spirtes */
  void updateNPC();

  /* Changes the players sprite (Facing direction) */
  void updatePlayer(Sprite s);

  /* Returns a vector of the indexes of the NPC's who are in the viewport */
  QVector<int> visibleNPCs();

  /* Checks if the NPC at the given index in the NPC vector is in the current 
   * viewport */
  bool zNPCInViewport(int index);
};

#endif
