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
const int MapViewport::kTRAVEL_DIFF = 20;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapViewport::MapViewport()
{
  clear();
}

MapViewport::MapViewport(uint16_t width, uint16_t height,
                         uint16_t tile_width, uint16_t tile_height)
           : MapViewport()
{
  /* Set the new parameters */
  setSize(width, height);
  setTileSize(tile_width, tile_height);
}

MapViewport::~MapViewport()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

void MapViewport::clear()
{
  travel = false;

  /* Reset the map and viewport coordinates */
  setMapSize(0, 0);
  setSize(kMIN_WIDTH, kMIN_HEIGHT);
  setTileSize(kMIN_WIDTH, kMIN_HEIGHT);

  /* Reset the lock on status */
  lockOn(0.0, 0.0);
}
  
/* Clears out the information in the class and sets it to default */
void MapViewport::clearLocation()
{
  x = 0.0;
  y = 0.0;
}

uint16_t MapViewport::getHeight()
{
  return height;
}

int MapViewport::getMapHeight()
{
  return map_height;
}

int MapViewport::getMapWidth()
{
  return map_width;
}

uint16_t MapViewport::getTileHeight()
{
  return tile_height;
}

uint16_t MapViewport::getTileWidth()
{
  return tile_width;
}

uint16_t MapViewport::getWidth()
{
  return width;
}

float MapViewport::getX()
{
  //int x_rounded = x * 10;
  //return (x_rounded / 10.0);
  return x;
}

int MapViewport::getXEnd()
{
  /* Perform the end x coordinate calculation */
  int end_x = (((int)x + width) / tile_width) * tile_width + tile_width;
  if((int)x % tile_width != 0)
    end_x += tile_width;

  /* Check to see if the end_x is in the valid range */
  if(end_x > map_width)
    end_x = map_width;

  return end_x;
}

int MapViewport::getXStart()
{
  int start_x = (((int)x / tile_width) * tile_width) - tile_width;

  /* Check to see if the start_x is in the valid range */
  if(start_x < 0)
    start_x = 0;

  return start_x;
}

uint16_t MapViewport::getXTileEnd()
{
  return (getXEnd() / tile_width);
}

uint16_t MapViewport::getXTileStart()
{
  return (getXStart() / tile_width);
}

float MapViewport::getY()
{
  //int y_rounded = y * 10;
  //return (y_rounded / 10.0);
  return y;
}

int MapViewport::getYEnd()
{
  /* Perform the end y coordinate calculation */
  int end_y = (((int)y + height) / tile_height) * tile_height + tile_height;
  if((int)y % tile_height != 0)
    end_y += tile_height;

  /* Check to see if the end_x is in the valid range */
  if(end_y > map_height)
    end_y = map_height;

  return end_y;
}

int MapViewport::getYStart()
{
  int start_y = (((int)y / tile_height) * tile_height) - tile_height;

  /* Check to see if the start_x is in the valid range */
  if(start_y < 0)
    start_y = 0;

  return start_y;
}

uint16_t MapViewport::getYTileEnd()
{
  return (getYEnd() / tile_height);
}

uint16_t MapViewport::getYTileStart()
{
  return (getYStart() / tile_height);
}
  
/* Returns if travelling */
bool MapViewport::isTravelling()
{
  return travel;
}

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
      lock_on_thing = NULL;
      lock_on_tile = NULL;
      lock_on_x = x;
      lock_on_y = y;
      lock_on = PIXEL;
      this->travel = travel;
      return true;
    }
  }

  return false;
}

bool MapViewport::lockOn(MapThing* thing, bool travel)
{
  if(thing != NULL && thing != lock_on_thing)
  {
    lock_on_thing = thing;
    lock_on_tile = NULL;
    lock_on_x = 0.0;
    lock_on_y = 0.0;
    lock_on = THING;
    this->travel = travel;
    return true;
  }

  return false;
}

bool MapViewport::lockOn(Tile* map_tile, bool travel)
{
  if(map_tile != NULL && map_tile != lock_on_tile)
  {
    lock_on_thing = NULL;
    lock_on_tile = map_tile;
    lock_on_x = 0.0;
    lock_on_y = 0.0;
    lock_on = TILE;
    this->travel = travel;
    return true;
  }

  return false;
}

void MapViewport::setMapSize(uint16_t tile_width, uint16_t tile_height,
                             int map_index)
{
  /* Set the new width */
  if(tile_width > 0)
    map_width_tiles = tile_width;
  else
    map_width_tiles = 0;
  map_width = map_width_tiles * this->tile_width;

  /* Set the new height */
  if(tile_height > 0)
    map_height_tiles = tile_height;
  else
    map_height_tiles = 0;
  map_height = map_height_tiles * this->tile_height;

  /* Set the map index */
  this->map_index = map_index;
  //x = 0.0;
  //y = 0.0;
}

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

void MapViewport::setTileSize(uint16_t pixel_width, uint16_t pixel_height)
{
  /* Set the new width */
  if(pixel_width > kMIN_WIDTH)
    tile_width = pixel_width;
  else
    tile_width = kMIN_WIDTH;
  map_width = map_width_tiles * tile_width;

  /* Set the new height */
  if(pixel_height > kMIN_HEIGHT)
    tile_height = pixel_height;
  else
    tile_height = kMIN_HEIGHT;
  map_height = map_height_tiles * tile_height;
}
  
/* Sets if the movement should travel to the new location */
void MapViewport::setToTravel(bool travel)
{
  this->travel = travel;
}

void MapViewport::update()
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
    center_x = lock_on_tile->getX() + (lock_on_tile->getWidth() / 2.0);
    center_y = lock_on_tile->getY() + (lock_on_tile->getHeight() / 2.0);
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
      if(diff_x_abs > kTRAVEL_DIFF || diff_y_abs > kTRAVEL_DIFF)
      {
        /* Determine scale - X dominant */
        if(diff_x_abs >= diff_y_abs)
        {
          diff_y_abs = (diff_y_abs / diff_x_abs) * kTRAVEL_DIFF;
          diff_x_abs = kTRAVEL_DIFF;
        }
        /* Y dominant */
        else
        {
          diff_x_abs = (diff_x_abs / diff_y_abs) * kTRAVEL_DIFF;
          diff_y_abs = kTRAVEL_DIFF;
        }

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
}
