/******************************************************************************
* Class Name: MapViewport
* Date Created: April 24, 2013
* Inheritance: none
* Description: This class handles the viewport that sits on top of the map 
*              class to allow for proper viewing. This will be the front 
*              interface with the outside classes for allowing viewing to the
*              map itself.
******************************************************************************/
#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

//#include <QDebug>

#include "Game/Map/MapThing.h"
#include "Game/Map/Tile.h"

class MapViewport
{
public:
  /* Constructor functions */
  MapViewport();
  MapViewport(short width, short height, short tile_width, short tile_height);

  /* Destructor function */
  ~MapViewport();

  /* Public enumerators */

  /* This controls what the viewport locks to
   * PIXEL - lock to a specific coordinate on the map
   * THING - lock to a MapThing, which has the possibility of moving
   * TILE - lock to a Tile on the Map */
  enum LockStatus{PIXEL, THING, TILE};
  
private:
  /* The size of the viewport */
  short height;
  int map_height;
  short map_height_tiles;
  int map_width;
  short map_width_tiles;
  short tile_height;
  short tile_width;
  short width;

  /* The location of the viewport */
  float x;
  float y;

  /* The lock qualifiers, for who the viewport is centered on */
  LockStatus lock_on;
  MapThing* lock_on_thing;
  Tile* lock_on_tile;
  float lock_on_x;
  float lock_on_y;

  /*------------------- Constants -----------------------*/
  const static int kMIN_HEIGHT; /* Minimum viewport height, in pixels */
  const static int kMIN_WIDTH;  /* Minimum viewport length, in pixels */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears out the information in the class and sets it to default */
  void clear();

  /* Gets the size of the viewport and related information */
  short getHeight();
  int getMapHeight();
  int getMapWidth();
  short getTileHeight();
  short getTileWidth();
  short getWidth();
  
  /* Gets the painting ranges for the map of the valid visible range and the offset */
  float getX();
  int getXEnd();
  int getXStart();
  short getXTileEnd();
  short getXTileStart();
  float getY();
  int getYEnd();
  int getYStart();
  short getYTileEnd();
  short getYTileStart();

  /* To change what the viewport locks to, when the update is called */
  bool lockOn(float x, float y);
  bool lockOn(MapThing* thing);
  bool lockOn(Tile* map_tile);

  /* Sets the size of the viewport and related information */
  void setMapSize(short tile_width, short tile_height);
  void setSize(short pixel_width, short pixel_height);
  void setTileSize(short pixel_width, short pixel_height);

  /* Updates the viewport, call at the end of the update call in the map */
  void updateView();
};

#endif // MAPVIEWPORT_H
