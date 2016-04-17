/******************************************************************************
 * Class Name: MapViewport
 * Date Created: April 24, 2013
 * Inheritance: none
 * Description: This class handles the viewport that sits on top of the map
 *              class to allow for proper viewing. This will be the front
 *              interface with the outside classes for allowing viewing to the
 *              map itself.
 *****************************************************************************/
#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

#include "Game/Map/MapThing.h"
#include "Game/Map/Tile.h"

class MapViewport
{
public:
  /* Constructor functions */
  MapViewport();
  MapViewport(uint16_t width, uint16_t height,
              uint16_t tile_width, uint16_t tile_height);

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
  uint16_t height;
  int map_height;
  uint16_t map_height_tiles;
  int map_index;
  int map_width;
  uint16_t map_width_tiles;
  uint16_t tile_height;
  uint16_t tile_width;
  uint16_t width;

  /* The location of the viewport */
  float x;
  float y;

  /* The lock qualifiers, for who the viewport is centered on */
  LockStatus lock_on;
  MapThing* lock_on_thing;
  Tile* lock_on_tile;
  float lock_on_x;
  float lock_on_y;

  /* Travel to new point */
  bool travel;

  /*------------------- Constants -----------------------*/
  const static int kMIN_HEIGHT; /* Minimum viewport height, in pixels */
  const static int kMIN_WIDTH;  /* Minimum viewport length, in pixels */
  const static int kTRAVEL_DIFF; /* Differential for travelling */

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Clears out the information in the class and sets it to default */
  void clear();
  void clearLocation();

  /* Gets the size of the viewport and related information */
  uint16_t getHeight();
  MapThing* getLockThing();
  Tile* getLockTile();
  int getMapHeight();
  int getMapWidth();
  uint16_t getTileHeight();
  uint16_t getTileWidth();
  uint16_t getWidth();

  /* Gets the ranges for the map of the valid visible range and the offset */
  float getX();
  int getXEnd();
  int getXStart();
  uint16_t getXTileEnd();
  uint16_t getXTileStart();
  float getY();
  int getYEnd();
  int getYStart();
  uint16_t getYTileEnd();
  uint16_t getYTileStart();

  /* Returns if travelling */
  bool isTravelling();

  /* To change what the viewport locks to, when the update is called */
  bool lockOn(float x, float y, bool travel = true);
  bool lockOn(MapThing* thing, bool travel = true);
  bool lockOn(Tile* map_tile, bool travel = true);

  /* Sets the size of the viewport and related information */
  void setMapSize(uint16_t tile_width, uint16_t tile_height,
                  int map_index = -1);
  void setSize(uint16_t pixel_width, uint16_t pixel_height);
  void setTileSize(uint16_t pixel_width, uint16_t pixel_height);

  /* Sets if the movement should travel to the new location */
  void setToTravel(bool travel);

  /* Updates the viewport, call at the end of the update call in the map */
  void update();
};

#endif // MAPVIEWPORT_H
