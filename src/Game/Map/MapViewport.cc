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

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapViewport::MapViewport()
{
  clear();
}

MapViewport::MapViewport(short width, short height, 
                         short tile_width, short tile_height)
{
  clear();

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
  /* Reset the map and viewport coordinates */
  setMapSize(0, 0);
  setSize(kMIN_WIDTH, kMIN_HEIGHT);
  setTileSize(kMIN_WIDTH, kMIN_HEIGHT);

  /* Reset the lock on status */
  lockOn(0.0, 0.0);
}

short MapViewport::getHeight()
{
  return height;
}

short MapViewport::getMapHeight()
{
  return map_height;
}

short MapViewport::getMapWidth()
{
  return map_width;
}

short MapViewport::getTileHeight()
{
  return tile_height;
}

short MapViewport::getTileWidth()
{
  return tile_width;
}

short MapViewport::getWidth()
{
  return width;
}

float MapViewport::getX()
{
  //int x_rounded = x * 10;
  //return (x_rounded / 10.0);
  return x;
}

short MapViewport::getXEnd()
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

short MapViewport::getXStart()
{
  int start_x = (((int)x / tile_width) * tile_width) - tile_width;
  
  /* Check to see if the start_x is in the valid range */
  if(start_x < 0)
    start_x = 0;

  return start_x;
}

short MapViewport::getXTileEnd()
{
  return (getXEnd() / tile_width);
}

short MapViewport::getXTileStart()
{
  return (getXStart() / tile_width);
}

float MapViewport::getY()
{
  //int y_rounded = y * 10;
  //return (y_rounded / 10.0);
  return y;
}

short MapViewport::getYEnd()
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

short MapViewport::getYStart()
{
  int start_y = (((int)y / tile_height) * tile_height) - tile_height;

  /* Check to see if the start_x is in the valid range */
  if(start_y < 0)
    start_y = 0;

  return start_y;
}

short MapViewport::getYTileEnd()
{
  return (getYEnd() / tile_height);
}

short MapViewport::getYTileStart()
{
  return (getYStart() / tile_height);
}

bool MapViewport::lockOn(float x, float y)
{
  if(((int)x == 0 && (int)y == 0) || 
     (x >= 0 && y >= 0 && 
      x < map_width && y < map_height)
    )
  {
    lock_on_thing = 0;
    lock_on_tile = 0;
    lock_on_x = x;
    lock_on_y = y;
    lock_on = PIXEL;
    return true;
  }

  return false;
}

bool MapViewport::lockOn(MapThing* thing)
{
  if(thing != 0)
  {
    lock_on_thing = thing;
    lock_on_tile = 0;
    lock_on_x = 0.0;
    lock_on_y = 0.0;
    lock_on = THING;
    return true;
  }

  return false;
}

bool MapViewport::lockOn(Tile* map_tile)
{
  if(map_tile != 0)
  {
    lock_on_thing = 0;
    lock_on_tile = map_tile;
    lock_on_x = 0.0;
    lock_on_y = 0.0;
    lock_on = TILE;
    return true;
  }

  return false;
}

void MapViewport::setMapSize(short tile_width, short tile_height)
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
}

void MapViewport::setSize(short pixel_width, short pixel_height)
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

void MapViewport::setTileSize(short pixel_width, short pixel_height)
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

void MapViewport::updateView()
{
  float center_x = 0.0;
  float center_y = 0.0;
  float delta_x = 0.0;
  float delta_y = 0.0;

  /* If the locked on information is a coordinate pair (x,y) */
  if(lock_on == PIXEL)
  {
    center_x = lock_on_x;
    center_y = lock_on_y;
  }
  /* Else if the locked on information is a map thing */
  else if(lock_on == THING)
  {
    center_x = lock_on_thing->getX() + (lock_on_thing->getWidth() / 2.0);
    center_y = lock_on_thing->getY() + (lock_on_thing->getHeight() / 2.0);
  }
  /* Else if the locked on information is a tile */
  else if(lock_on == TILE)
  {
    center_x = lock_on_tile->getX() + (lock_on_tile->getWidth() / 2.0);
    center_y = lock_on_tile->getY() + (lock_on_tile->getHeight() / 2.0);
  }
  
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
  
  /* Set the internal X and Y to the newly calculated values */
  this->x = delta_x;
  this->y = delta_y;
}
