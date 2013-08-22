/******************************************************************************
* Class Name: Map
* Date Created: Oct 28 2012
* Inheritance: QGLWidget
* Description: The map class, this is the top level with regard to an actual
*              in-game map. This contains all the tiles that a map is composed
*              of, it also holds pointers to all of the NPC's contained in the
*              maps tiles. This also handles the viewport for the map.
******************************************************************************/
#ifndef MAP_H
#define MAP_H

//#include <QDebug>
#include <QGLWidget>

#include "Game/Map/MapDialog.h"
#include "Game/Map/MapNPC.h"
#include "Game/Map/MapMenu.h"
#include "Game/Map/MapStatusBar.h"
#include "Game/Map/MapThing.h"
#include "Game/Map/MapViewport.h"
#include "Game/Map/Sector.h"
#include "Game/Map/Tile.h"
#include "Game/Sprite.h"
#include "Game/Weather.h"
#include "FileHandler.h"

class Map : public QGLWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  Map(const QGLFormat & format, short viewport_width, short viewport_height);

  /* Destructor function */
  ~Map();

private:
  /* Vector of all ai's on map */
  QList<MapNPC*> ai;

  /* The actual tiles that comprise a map, dynamically sized */
  QList< QList<Tile*> > geography;
  QList<Sprite*> tile_sprites;

  /* Indication if the map has been loaded */
  bool loaded;

  /* The menus and dialogs on top of the map */
  MapDialog map_dialog;
  MapMenu map_menu;

  /* The status bar on the map */
  MapStatusBar map_status_bar;

  /* The players info on the map */
  QList<MapPerson*> persons;
  MapPerson* player; /* The actively controlled player */
  MapThing* thing;

  /* The sectors on the map (for rooms, caves, houses etc) */
  QList<Sector> sectors;

  /* The time that has elapsed for each draw cycle */
  QTime time_elapsed;
  QTimer timer; // TODO: Move up, the tick

  /* The viewport for the map, controlled by QGraphicsView */
  MapViewport* viewport;

  /* Weather effect on the overall map (May be pushed to the sector level at 
   * a later time) */
  Weather* weather_effect;

  /* The painting monitoring parameters */
  QString frames_per_second;
  int frames;
  int paint_animation;
  int paint_count;
  QTime paint_time;
  double paint_time_average;

  /*------------------- Constants -----------------------*/
  const static int kDOUBLE_DIGITS;    /* The point when integers are more than
                                       * a single digit */
  const static int kELEMENT_ANGLE;    /* Element angle for sprite */
  const static int kELEMENT_DATA;     /* Element data type for sprite */
  const static int kFILE_CLASSIFIER;  /* The file tile classification text */
  const static int kFILE_SECTION_ID;  /* The section identifier, for file */
  const static int kFILE_TILE_COLUMN; /* The tile depth in XML of column tag */
  const static int kFILE_TILE_ROW;    /* The tile depth in XML of row tag */
  const static short kPLAYER_INDEX;   /* The player index, in the thing set */
  const static int kTICK_DELAY;       /* Tick timer delay constant */
  const static int kTILE_HEIGHT;      /* The tile height, as constant (TEMP) */
  const static int kTILE_WIDTH;       /* The tile width, as constant (TEMP) */
  const static int kVIEWPORT_HEIGHT;  /* The viewport height, in tiles */
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
  /* Animates the map */
  void animate(short time_since_last);

  /* GL initialization call */
  void initializeGL();

  /* Key Press/Release Events */
  void keyPressEvent(QKeyEvent* key_event);
  void keyReleaseEvent(QKeyEvent* key_event);

  /* GL painting call */
  void paintGL();
  
  /* GL resize call */
  void resizeGL(int width, int height);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void closingMap(int index);
 
/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Animates the tiles within the map */
  void animateTiles();

  /* Acquires the thing data and sets it in the dialog class, if applicable */
  void getThingData(QList<int> thing_ids);

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

  /* Checks the tile you are attempting to enter for passibility of the given
  direction */
  bool passible(EnumDb::Direction dir, int x, int y);

  /* Causes the thing you are moving into to start its interactive action */
  void passOver();

  /* The tick handling methods for starting and stopping the map */
  void tickStart();
  void tickStop();

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
