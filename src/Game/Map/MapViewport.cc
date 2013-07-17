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
  lockOn(0, 0);
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


bool MapViewport::lockOn(int x, int y)
{
  if((x == 0 && y == 0) || 
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
    lock_on_x = 0;
    lock_on_y = 0;
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
    lock_on_x = 0;
    lock_on_y = 0;
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
  int delta_x = 0;
  int delta_y = 0;

  /* If the locked on information is a coordinate pair (x,y) */
  if(lock_on == PIXEL)
  {
    /* Calculations for centering the width offset coordinate */
    delta_x = lock_on_x - (width >> 1);
    if((delta_x + width) > map_width)
      delta_x = map_width - width;
    if(delta_x < 0)
      delta_x = 0;

    /* Calculations for centering the height offset coordinate */
    delta_y = lock_on_y - (height >> 1);
    if((delta_y + height) > map_height)
      delta_y = map_height - height;
    if(delta_y < 0)
      delta_y = 0;
  }
  /* Else if the locked on information is a map thing */
  else if(lock_on == THING)
  {

  }
  /* Else if the locked on information is a tile */
  else if(lock_on == TILE)
  {

  }
}
