/******************************************************************************
* Class Name: MapViewport
* Date Created: April 24, 2013
* Inheritance: none
* Description: This class handles the viewport that sits on top of the map
*              class to allow for proper viewing. This will be the front
*              interface with the outside classes for allowing viewing to the
*              map itself.
* TODO: 1. Add in the feature so that when the lock on is changed, an
*          animation line is drawn and the viewport shifts over that line.
******************************************************************************/
#include "Game/Map/MapViewport.h"

/* Constant Implementation - see header file for descriptions */
const int MapViewport::kMIN_HEIGHT = 1;
const int MapViewport::kMIN_WIDTH = 1;
const int MapViewport::kTRAVEL_DIFF = 156;
const int MapViewport::kTRAVEL_DIV = 100000;
const float MapViewport::kTRAVEL_MIN = 2.0;
const uint16_t MapViewport::kVIB_DEG_DEFAULT = 360;
const uint32_t MapViewport::kVIB_PEAK_DEFAULT = 4000;
const uint32_t MapViewport::kVIB_TOTAL_DEFAULT = 5000;
const int16_t MapViewport::kVIB_X_DEFAULT = 100;
const int16_t MapViewport::kVIB_Y_DEFAULT = 0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Default constructor. Just sets up the viewport with blank data.
 *
 * Inputs: none
 */
MapViewport::MapViewport()
{
  clear();
}

/*
 * Description: Constructor with width and height of viewport along with tile
 *              width and tile height.
 *
 * Inputs: uint16_t width - width in pixels of the game viewport
 *         uint16_t height - height in pixels of the game viewport
 *         uint16_t tile_width - width of map tile in pixels
 *         uint16_t tile_height - height of map tile in pixels
 */
MapViewport::MapViewport(uint16_t width, uint16_t height,
                         uint16_t tile_width, uint16_t tile_height)
           : MapViewport()
{
  setSize(width, height);
  setTileSize(tile_width, tile_height);
}

/*
 * Description: Destructor function
 */
MapViewport::~MapViewport()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Correlate the values with modifications to tile size and map
 *              size.
 *
 * Inputs: none
 * Output: none
 */
void MapViewport::correlateSize()
{
  /* X correlation */
  int old_width = map_width;
  map_width = map_width_tiles * tile_width;
  if(old_width > 0 && old_width != map_width)
    x = map_width * x / old_width;

  /* Y correlation */
  int old_height = map_height;
  map_height = map_height_tiles * tile_height;
  if(old_height > 0 && old_height != map_height)
    y = map_height * y / old_height;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Clears all data and resets it within the viewport.
 *
 * Inputs: none
 * Output: none
 */
void MapViewport::clear()
{
  travel = false;
  travel_force = false;

  /* Reset the map and viewport coordinates */
  setMapSize(0, 0);
  setSize(kMIN_WIDTH, kMIN_HEIGHT);
  setTileSize(kMIN_WIDTH, kMIN_HEIGHT);

  /* Reset the lock on status */
  lockOn(0.0, 0.0);

  /* Reset the vibrating status */
  vibrating = false;
  vib_delta_deg = kVIB_DEG_DEFAULT;
  vib_peak_x = kVIB_X_DEFAULT;
  vib_peak_y = kVIB_Y_DEFAULT;
  vib_time_peak = kVIB_PEAK_DEFAULT;
  vib_time_total = kVIB_TOTAL_DEFAULT;
}

/*
 * Description: Clears out the information in the class and sets it to default.
 *
 * Inputs: none
 * Output: none
 */
void MapViewport::clearLocation()
{
  x = 0.0;
  y = 0.0;
}

/*
 * Description: Gets the height of the game viewport in pixels.
 *
 * Inputs: none
 * Output: uint16_t - height in pixels
 */
uint16_t MapViewport::getHeight()
{
  return height;
}

/*
 * Description: Returns the lock on thing. Null if not locked on thing.
 *
 * Inputs: none
 * Output: MapThing* - the thing reference that is locked on
 */
MapThing* MapViewport::getLockThing()
{
  return lock_on_thing;
}

/*
 * Description: Returns the lock on tile. Null if not locked on tile.
 *
 * Inputs: none
 * Output: Tile* - the tile reference that is locked on
 */
Tile* MapViewport::getLockTile()
{
  return lock_on_tile;
}

/*
 * Description: Gets the height of the map in pixels.
 *
 * Inputs: none
 * Output: int - height in pixels
 */
int MapViewport::getMapHeight()
{
  return map_height;
}

/*
 * Description: Gets the width of the map in pixels.
 *
 * Inputs: none
 * Output: int - width in pixels
 */
int MapViewport::getMapWidth()
{
  return map_width;
}

/*
 * Description: Gets the tile height of each tile captured by the viewport
 *
 * Inputs: none
 * Output: uint16_t - height in pixels
 */
uint16_t MapViewport::getTileHeight()
{
  return tile_height;
}

/*
 * Description: Gets the tile width of each tile captured by the viewport
 *
 * Inputs: none
 * Output: uint16_t - width in pixels
 */
uint16_t MapViewport::getTileWidth()
{
  return tile_width;
}

/*
 * Description: Gets the width of the game viewport in pixels.
 *
 * Inputs: none
 * Output: uint16_t - width in pixels
 */
uint16_t MapViewport::getWidth()
{
  return width;
}

/*
 * Description: Gets the X location relative to the map in pixels of the top
 *              left location of the viewport.
 *
 * Inputs: none
 * Output: float - the X location decimal
 */
float MapViewport::getX()
{
  if(vibrating)
    return (x + vibrating_x);
  return x;
}

/*
 * Description: Returns an integer of the end x pixel location of the render
 *              limit of the viewport relative to the map space
 *
 * Inputs: none
 * Output: int - x pixel location on map for viewport end
 */
int MapViewport::getXEnd()
{
  /* Perform the end x coordinate calculation */
  int end_x = (((int)getX() + width) / tile_width) * tile_width + tile_width;
  if((int)getX() % tile_width != 0)
    end_x += tile_width;

  /* Check to see if the end_x is in the valid range */
  if(end_x > map_width)
    end_x = map_width;
  else if(end_x < 0)
    end_x = 0;

  return end_x;
}

/*
 * Description: Returns an integer of the start x pixel location of the render
 *              limit of the viewport relative to the map space
 *
 * Inputs: none
 * Output: int - x pixel location on map for viewport start
 */
int MapViewport::getXStart()
{
  int start_x = (((int)getX() / tile_width) * tile_width) - tile_width;

  /* Check to see if the start_x is in the valid range */
  if(start_x < 0)
    start_x = 0;

  return start_x;
}

/*
 * Description: Returns an integer representing the x pixel end location of the
 *              viewport in map tiles according to the internally set variable
 *              sizes.
 *
 * Inputs: none
 * Output: uint16_t - the number of tiles at the x end
 */
uint16_t MapViewport::getXTileEnd()
{
  return (getXEnd() / tile_width);
}

/*
 * Description: Returns an integer representing the x pixel start location of
 *              the viewport in map tiles according to the internally set
 *              variable sizes.
 *
 * Inputs: none
 * Output: uint16_t - the number of tiles at the x start
 */
uint16_t MapViewport::getXTileStart()
{
  return (getXStart() / tile_width);
}

/*
 * Description: Gets the Y location relative to the map in pixels of the top
 *              left location of the viewport.
 *
 * Inputs: none
 * Output: float - the Y location decimal
 */
float MapViewport::getY()
{
  if(vibrating)
    return (y + vibrating_y);
  return y;
}

/*
 * Description: Returns an integer of the end y pixel location of the render
 *              limit of the viewport relative to the map space
 *
 * Inputs: none
 * Output: int - y pixel location on map for viewport end
 */
int MapViewport::getYEnd()
{
  /* Perform the end y coordinate calculation */
  int end_y = (((int)getY() + height) / tile_height) * tile_height
            + tile_height;
  if((int)getY() % tile_height != 0)
    end_y += tile_height;

  /* Check to see if the end_x is in the valid range */
  if(end_y > map_height)
    end_y = map_height;
  else if(end_y < 0)
    end_y = 0;

  return end_y;
}

/*
 * Description: Returns an integer of the start y pixel location of the render
 *              limit of the viewport relative to the map space
 *
 * Inputs: none
 * Output: int - y pixel location on map for viewport start
 */
int MapViewport::getYStart()
{
  int start_y = (((int)getY() / tile_height) * tile_height) - tile_height;

  /* Check to see if the start_x is in the valid range */
  if(start_y < 0)
    start_y = 0;

  return start_y;
}

/*
 * Description: Returns an integer representing the y pixel end location of the
 *              viewport in map tiles according to the internally set variable
 *              sizes.
 *
 * Inputs: none
 * Output: uint16_t - the number of tiles at the y end
 */
uint16_t MapViewport::getYTileEnd()
{
  return (getYEnd() / tile_height);
}

/*
 * Description: Returns an integer representing the y pixel start location of
 *              the viewport in map tiles according to the internally set
 *              variable sizes.
 *
 * Inputs: none
 * Output: uint16_t - the number of tiles at the y start
 */
uint16_t MapViewport::getYTileStart()
{
  return (getYStart() / tile_height);
}

/*
 * Description: Returns if the travel is forced. If true, once travel is enabled
 *              it will not reset until travel force is disabled
 *
 * Inputs: none
 * Output: bool - true if travel force is enabled
 */
bool MapViewport::isTravelForce()
{
  return travel_force;
}

/*
 * Description: Returns if the viewport is in a traveling state.
 *
 * Inputs: none
 * Output: bool - true if the viewport is moving
 */
bool MapViewport::isTravelling()
{
  return travel;
}

/*
 * Description: Returns if it is vibrating.
 *
 * Inputs: none
 * Output: bool - true if it is vibrating
 */
bool MapViewport::isVibrating()
{
  return vibrating;
}

/*
 * Description: Modifies the lock on state of the viewport to a new location
 *              based on the x and y location relative to the map pixels.
 *
 * Inputs: float x - the x location in pixels to lock
 *         float y - the y location in pixels to lock
 *         bool travel - true to travel to new location. default true
 * Output: bool - true if set was successful
 */
bool MapViewport::lockOn(float x, float y, bool travel)
{
  if((static_cast<int>(x) == 0 && static_cast<int>(y) == 0) ||
     (x >= 0 && y >= 0 &&
      x < map_width && y < map_height)
    )
  {
    /* Ensure no repeat call */
    if(x != lock_on_x || y != lock_on_y)
    {
      lock_on_thing = nullptr;
      lock_on_tile = nullptr;
      lock_on_x = x;
      lock_on_y = y;
      lock_on = PIXEL;
      this->travel = travel;
      return true;
    }
  }

  return false;
}

/*
 * Description: Modifies the lock on state of the viewport to a new location
 *              based on a reference map thing object
 *
 * Inputs: MapThing* thing - the reference thing pointer
 *         bool travel - true to travel to new location. default true
 * Output: bool - true if set was successful
 */
bool MapViewport::lockOn(MapThing* thing, bool travel)
{
  if(thing != nullptr && thing != lock_on_thing)
  {
    lock_on_thing = thing;
    lock_on_tile = nullptr;
    lock_on_x = 0.0;
    lock_on_y = 0.0;
    lock_on = THING;
    this->travel = travel;
    return true;
  }

  return false;
}

/*
 * Description: Modifies the lock on state of the viewport to a new location
 *              based on a reference map tile object
 *
 * Inputs: Tile* map_tile - the reference tile pointer
 *         bool travel - true to travel to new location. default true
 * Output: bool - true if set was successful
 */
bool MapViewport::lockOn(Tile* map_tile, bool travel)
{
  if(map_tile != nullptr && map_tile != lock_on_tile)
  {
    lock_on_thing = nullptr;
    lock_on_tile = map_tile;
    lock_on_x = 0.0;
    lock_on_y = 0.0;
    lock_on = TILE;
    this->travel = travel;
    return true;
  }

  return false;
}

/*
 * Description: Sets the map size the viewport is handling in tiles.
 *
 * Inputs: uint16_t tile_width - the map section width in tiles
 *         uint16_t tile_height - the map section height in tiles
 *         int map_index - the map index of the section being viewed
 * Output: none
 */
void MapViewport::setMapSize(uint16_t tile_width, uint16_t tile_height,
                             int map_index)
{
  /* Set the new width */
  if(tile_width > 0)
    map_width_tiles = tile_width;
  else
    map_width_tiles = 0;

  /* Set the new height */
  if(tile_height > 0)
    map_height_tiles = tile_height;
  else
    map_height_tiles = 0;

  /* Size correlation */
  correlateSize();

  /* Set the map index */
  this->map_index = map_index;
}

/*
 * Description: Sets the viewport width and height in pixels
 *
 * Inputs: uint16_t pixel_width - the pixel viewport width (aka resolution)
 *         uint16_t pixel_height - the pixel viewport height (aka resolution)
 * Output: none
 */
void MapViewport::setSize(uint16_t pixel_width, uint16_t pixel_height)
{
  /* Set the new width */
  if(pixel_width > kMIN_WIDTH)
    width = pixel_width;
  else
    width = kMIN_WIDTH;

  /* Set the new height */
  if(pixel_height > kMIN_HEIGHT)
    height = pixel_height;
  else
    height = kMIN_HEIGHT;
}

/*
 * Description: Sets the individual tile width and height of the map. This
 *              correlates with map width and height of tiles to determine
 *              overall map size
 *
 * Inputs: uint16_t pixel_width - the width of the tile in pixels
 *         uint16_t pixel_height - the height of the tile in pixels
 * Output: none
 */
void MapViewport::setTileSize(uint16_t pixel_width, uint16_t pixel_height)
{
  /* Set the new width */
  if(pixel_width > kMIN_WIDTH)
    tile_width = pixel_width;
  else
    tile_width = kMIN_WIDTH;

  /* Set the new height */
  if(pixel_height > kMIN_HEIGHT)
    tile_height = pixel_height;
  else
    tile_height = kMIN_HEIGHT;

  /* Size correlation */
  correlateSize();
}

/*
 * Description: Sets if the movement should travel between locations on drastic
 *              changes.
 *
 * Inputs: bool travel - true to travel between location changes
 * Output: none
 */
void MapViewport::setToTravel(bool travel)
{
  this->travel = travel;
}

/*
 * Description: Sets if the movement travel should be forced on until manually
 *              reset.
 *
 * Inputs: bool force - true to force the travel shift
 * Output: none
 */
void MapViewport::setTravelForce(bool force)
{
  travel_force = force;
}

/*
 * Description: Triggers vibration initiation using the passed input parameters.
 *              This will fail if it already vibrating
 *
 * Inputs: uint32_t time_total - the total time in ms of the vibration
 *         uint32_t time_peak - the total time at peak vibration
 *         int16_t peak_x - the peak delta x change
 *         int16_t peak_y - the peak delta y change
 *         uint16_t degree_delta - the delta degree per 1000 ms
 * Output: bool - true if vibration started with passed in parameters
 */
bool MapViewport::triggerVibration(uint32_t time_total, uint32_t time_peak,
                                   int16_t peak_x, int16_t peak_y,
                                   uint16_t degree_delta)
{
  if(!vibrating && time_total > 0 && time_total >= time_peak &&
     (peak_x != 0 || peak_y != 0) && degree_delta > 0)
  {
    vibrating = true;
    vibrating_deg = 0.0;
    vibrating_time = 0;
    vibrating_x = 0.0;
    vibrating_y = 0.0;

    vib_delta_deg = degree_delta;
    vib_peak_x = peak_x;
    vib_peak_y = peak_y;
    vib_time_peak = time_peak;
    vib_time_total = time_total;

    return true;
  }
  return false;
}

/*
 * Description: Takes the vibration and puts it into the finishing wind down
 *              sequence or proceeds to end it abruptly based on the input.
 *
 * Inputs: bool force - true to force the finish. False default (smooth)
 * Output: bool - true if there was a vibration to end
 */
bool MapViewport::triggerVibrationFinish(bool force)
{
  if(vibrating)
  {
    /* If force, just abruptly end the vibration and return to base state */
    if(force)
    {
      vibrating = false;
    }
    /* Otherwise, transition to the wind down sequence for ending it */
    else
    {
      uint32_t vib_finish_time = (vib_time_total - vib_time_peak) / 2;
      vib_finish_time += vib_time_peak;
      if(vibrating_time < vib_finish_time)
        vibrating_time = vib_finish_time;
    }

    return true;
  }
  return false;
}

/*
 * Description: Updates the viewport and all pertinent information. Controls
 *              visualization of the map data for the game view
 *
 * Inputs: int cycle_time - the ms update cycle for the viewport
 * Output: none
 */
void MapViewport::update(int cycle_time)
{
  float center_x = 0.0;
  float center_y = 0.0;
  float delta_x = 0.0;
  float delta_y = 0.0;
  bool modify = false;

  /* If the locked on information is a coordinate pair (x,y) */
  if(lock_on == PIXEL)
  {
    center_x = lock_on_x;
    center_y = lock_on_y;
    modify = true;
  }
  /* Else if the locked on information is a map thing */
  else if(lock_on == THING)
  {
    if(static_cast<int>(lock_on_thing->getMapSection()) == map_index)
    {
      center_x = lock_on_thing->getCenterX();
      center_y = lock_on_thing->getCenterY();
      modify = true;
    }
  }
  /* Else if the locked on information is a tile */
  else if(lock_on == TILE)
  {
    center_x = lock_on_tile->getX() * lock_on_tile->getWidth() +
               (lock_on_tile->getWidth() / 2.0);
    center_y = lock_on_tile->getY() * lock_on_tile->getHeight() +
               (lock_on_tile->getHeight() / 2.0);
    modify = true;
  }

  /* Only proceed if center x/y set */
  if(modify)
  {
    /* Calculations for centering the width offset coordinate */
    if(width > map_width)
    {
      delta_x = (map_width - width) / 2.0;
    }
    else
    {
      delta_x = center_x - (width / 2.0);
      if((delta_x + width) > map_width)
        delta_x = map_width - width;
      if(delta_x < 0)
        delta_x = 0.0;
    }

    /* Calculations for centering the height offset coordinate */
    if(height > map_height)
    {
      delta_y = (map_height - height) / 2.0;
    }
    else
    {
      delta_y = center_y - (height / 2.0);
      if((delta_y + height) > map_height)
        delta_y = map_height - height;
      if(delta_y < 0)
        delta_y = 0.0;
    }

    /* Base differential */
    float diff_x = delta_x - this->x;
    float diff_y = delta_y - this->y;
    //float travel_delta = cycle_time * kTRAVEL_DIFF / 1000;

    /* Travel: slowly go to point */
    if(travel)
    {
      /* Calculate diff x abs */
      float diff_x_abs = diff_x;
      if(diff_x < 0)
        diff_x_abs = -diff_x;

      /* Calculate diff y abs */
      float diff_y_abs = diff_y;
      if(diff_y < 0)
        diff_y_abs = -diff_y;

      /* Determine if out of range */
      if(travel_force || diff_x_abs > kTRAVEL_MIN || diff_y_abs > kTRAVEL_MIN)
      {
        /* Determine calculated scaled x and y move */
        if(travel_force)
          diff_x_abs = (cycle_time * diff_x_abs * kTRAVEL_DIFF / kTRAVEL_DIV);
        else if(diff_x_abs > kTRAVEL_MIN)
          diff_x_abs = (cycle_time * diff_x_abs * kTRAVEL_DIFF / kTRAVEL_DIV)
                     + kTRAVEL_MIN;
        if(travel_force)
          diff_y_abs = (cycle_time * diff_y_abs * kTRAVEL_DIFF / kTRAVEL_DIV);
        else if(diff_y_abs > kTRAVEL_MIN)
          diff_y_abs = (cycle_time * diff_y_abs * kTRAVEL_DIFF / kTRAVEL_DIV)
                     + kTRAVEL_MIN;

        /* Final calculation */
        if(diff_x < 0)
          diff_x = -diff_x_abs;
        else
          diff_x = diff_x_abs;
        if(diff_y < 0)
          diff_y = -diff_y_abs;
        else
          diff_y = diff_y_abs;
      }
      /* If in range, end travel */
      else
      {
        travel = false;
      }
    }

    /* Add differential */
    this->x += diff_x;
    this->y += diff_y;
  }

  /* Vibrating handling */
  if(vibrating)
  {
    /* Degree update */
    vibrating_deg += cycle_time * vib_delta_deg / 1000.0;
    if(vibrating_deg >= 360.0)
      vibrating_deg -= 360.0;

    /* Sine value */
    float sin_val = sin(vibrating_deg * PI / 180.0);

    /* The peaking factor */
    uint32_t time_scale = (vib_time_total - vib_time_peak) / 2;
    float peak_factor = 1.0;
    if(vibrating_time < time_scale)
    {
      peak_factor = 1.0 * vibrating_time / time_scale;
    }
    else if(vibrating_time > (time_scale + vib_time_peak))
    {
      peak_factor = 1.0 - 1.0 *
                    (vibrating_time - time_scale - vib_time_peak) / time_scale;
      if(peak_factor < 0.0)
        peak_factor = 0.0;
    }

    /* Vibrating X and Y */
    vibrating_x = sin_val * peak_factor * vib_peak_x;
    vibrating_y = sin_val * peak_factor * vib_peak_y;

    /* Add to delta vibrating time */
    vibrating_time += cycle_time;
    if(vibrating_time > vib_time_total)
      vibrating = false;
  }
}
