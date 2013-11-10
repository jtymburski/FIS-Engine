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

#include "FileHandler.h"
#include "Game/EventHandler.h"
#include "Game/Map/MapDialog.h"
#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapItem.h"
#include "Game/Map/MapMenu.h"
#include "Game/Map/MapNPC.h"
#include "Game/Map/MapStatusBar.h"
#include "Game/Map/MapThing.h"
#include "Game/Map/MapViewport.h"
#include "Game/Map/Sector.h"
#include "Game/Map/Tile.h"
#include "Game/Sprite.h"
#include "Game/Weather.h"
#include "Options.h"

class Map : public QGLWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  Map(const QGLFormat & format, Options running_config, 
      EventHandler* event_handler);

  /* Destructor function */
  ~Map();

private:
  /* A reference blank event for setting events in the game */
  EventHandler* event_handler;
  
  /* The actual tiles that comprise a map, dynamically sized */
  QList< QList< QList<Tile*> > > geography;
  short map_index;
  QList<Sprite*> tile_sprites;

  /* Indication if the map has been loaded */
  bool loaded;

  /* The menus and dialogs on top of the map */
  MapDialog map_dialog;
  MapMenu map_menu;

  /* The status bar on the map */
  MapStatusBar map_status_bar; // TODO: Remove

  /* The players info on the map */
  QList<MapItem*> items;
  QList<MapPerson*> persons;
  MapPerson* player; /* The actively controlled player */
  QList<MapThing*> things;

  /* The sectors on the map (for rooms, caves, houses etc) */
  QList<Sector> sectors;

  /* The time that has elapsed for each draw cycle */
  int time_elapsed;

  /* The viewport for the map, controlled by QGraphicsView */
  MapViewport* viewport;

  /* Weather effect on the overall map (May be pushed to the sector level at 
   * a later time) */
  Weather* weather_effect;

  /* The painting monitoring parameters */
  QString frames_per_second;
  int paint_animation;
  int paint_frames;
  int paint_time;

  /*------------------- Constants -----------------------*/
  const static int kDOUBLE_DIGITS;    /* The point when integers are more than
                                      * a single digit */
  const static int kELEMENT_DATA;     /* Element data type for sprite */
  const static short kFILE_CLASSIFIER;  /* The file tile classification text */
  const static short kFILE_GAME_TYPE;   /* The game type classifier */
  const static short kFILE_SECTION_ID;  /* The section identifier, for file */
  const static short kFILE_TILE_COLUMN; /* The tile depth in XML of column */
  const static short kFILE_TILE_ROW;    /* The tile depth in XML of row */
  const static short kPLAYER_ID;      /* The player ID for computer control */
  const static int kTILE_HEIGHT;      /* The tile height, as constant (TEMP) */
  const static int kTILE_WIDTH;       /* The tile width, as constant (TEMP) */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds tile data, as per data from the file */
  bool addTileData(XmlData data, int section_index);

  /* Adds a tile sprite, based on the path and some XMLData */
  bool addTileSprite(QString path, int x_diff, int y_diff, 
                     QList<QString> adjustments, int section_index, 
                     XmlData data);

  /* Adds thing data, as per data from the file */
  bool addThingData(XmlData data, int section_index);

  /* Initiates a section block of map. Triggered from the file data */
  bool initiateMapSection(int section_index, int width, int height);
  
  /* Initiates a thing action, based on the action key being hit */
  void initiateThingInteraction();
 
  /* Splites the tile path, to determine if numerous tiles are needed */
  QList< QList<QString> > splitTilePath(QString path);

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
  void closeMap();
 
/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Complete the action sequence with the thing */
  void finishThingTarget();
  
  /* Acquires the thing data and sets it in the dialog class, if applicable */
  void getThingData(QList<int> thing_ids);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the map person, for access */
  MapPerson* getPlayer();

  /* Returns the map viewport, for displaying the scene to the screen */
  MapViewport* getViewport();

  /* Initial call when map is displayed */
  void initialization();

  /* Initiates a conversation, within the map. */
  bool initConversation(Conversation* convo, MapPerson* initiator, 
                                             MapThing* source);

  /* Initiates a notification, within the map */
  bool initNotification(QString notification);

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

  /* Picks up the total number of the item */
  bool pickupItem(MapItem* item);
  
  /* Changes the map section index - what is displayed */
  bool setSectionIndex(int index);
  
  /* Teleport a thing, based on the given coordinates */
  void teleportThing(int id, int tile_x, int tile_y, int section_id = -1);

  /* Unload the map, if there is one loaded */
  void unloadMap();

  /* Updates the map - called by the cycle timer call from game */
  void updateMap(int cycle_time);
};

#endif
