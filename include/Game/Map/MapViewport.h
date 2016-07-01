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
  bool travel_force;

  /* Vibration handling */
  bool vibrating;
  float vibrating_deg;
  uint32_t vibrating_time;
  float vibrating_x;
  float vibrating_y;

  /* Vibration properties */
  uint16_t vib_delta_deg; /* delta degrees per 1000 ms */
  int16_t vib_peak_x;
  int16_t vib_peak_y;
  uint32_t vib_time_peak;
  uint32_t vib_time_total;

  /*------------------- Constants -----------------------*/
  const static int kMIN_HEIGHT; /* Minimum viewport height, in pixels */
  const static int kMIN_WIDTH;  /* Minimum viewport length, in pixels */
  const static int kTRAVEL_DIFF; /* Pixels per 1000ms travel */
  const static int kTRAVEL_DIV; /* Factor to scale the calculated value */
  const static float kTRAVEL_MIN; /* Minimum travel required before reset */
  const static uint16_t kVIB_DEG_DEFAULT; /* Vibrate degree delta default */
  const static uint32_t kVIB_PEAK_DEFAULT; /* Vibrate peak time default */
  const static uint32_t kVIB_TOTAL_DEFAULT; /* Vibrate total time default */
  const static int16_t kVIB_X_DEFAULT; /* Vibrate peak x default */
  const static int16_t kVIB_Y_DEFAULT; /* Vibrate peak y default */

/*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Correlate the values with modifications to tile size and map size */
  void correlateSize();

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
  bool isTravelForce();
  bool isTravelling();

  /* Returns if it is vibrating */
  bool isVibrating();

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
  void setTravelForce(bool force);

  /* Trigger vibration call */
  bool triggerVibration(uint32_t time_total = kVIB_TOTAL_DEFAULT,
                        uint32_t time_peak = kVIB_PEAK_DEFAULT,
                        int16_t peak_x = kVIB_X_DEFAULT,
                        int16_t peak_y = kVIB_Y_DEFAULT,
                        uint16_t degree_delta = kVIB_DEG_DEFAULT);
  bool triggerVibrationFinish(bool force = false);

  /* Updates the viewport, call at the end of the update call in the map */
  void update(int cycle_time);
};

#endif // MAPVIEWPORT_H
