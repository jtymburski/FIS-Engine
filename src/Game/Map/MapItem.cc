/******************************************************************************
* Class Name: MapItem
* Date Created: Dec 2 2012
* Inheritance: MapThing
* Description: The MapItem class - TODO: description
******************************************************************************/
#include "Game/Map/MapItem.h"

/* Constant Implementation - see header file for descriptions */
const short MapItem::kDEFAULT_COUNT = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapItem::MapItem() : MapThing()
{
  /* Set the count to 0 since the map item is not configured */
  setCount(0);
}

MapItem::MapItem(Sprite* frames, int width, int height, QString name,
                 QString description, int id)
       : MapThing(frames, width, height, name, description, id)
{
  /* Set the count to the default since the map item is configured */
  setCount(kDEFAULT_COUNT);
}

/* Destructor function */
MapItem::~MapItem()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the class descriptor, useful for casting */
QString MapItem::classDescriptor()
{
  return "MapItem";
}

/* Clears out the item construct, void of painting */
void MapItem::clear()
{
  /* Clear out the item variables */
  setCount(0);
  
  /* Clear out parent */
  MapThing::clear();
}

/* Returns the count of how many of these items are available */
int MapItem::getCount()
{
  return count;
}
  
/* Sets the number of this item */
bool MapItem::setCount(int count)
{
  if(count >= 0)
  {
    this->count = count;
    return true;
  }
  
  return false;
}

/* 
 * Description: Sets the connected tile information for the map item. This is
 *              the initial starting point and where the item is initially
 *              placed. If this is unset, the item will not move or paint.
 *              This replaces the generic mapthing call to allow for map 
 *              person in tile to be modified.
 *
 * Inputs: int section_id - the map id that the tile is from
 *         Tile* new_tile - the tile to set the starting pointer to
 *         bool no_events - don't execute any events when set
 * Output: bool - status if the change was able to occur
 */
bool MapItem::setStartingTile(int section_id, Tile* new_tile, bool no_events)
{
  /* Unused variable - used in virtual call for Person */
  (void)no_events;

  if(section_id >= 0 && new_tile != 0)
  {
    /* Unset the main tile */
    if(tile_main != 0)
      tile_main->unsetItem(this);
    tile_main = 0;
  
    /* Set the new tile */
    tile_main = new_tile;
    this->x = tile_main->getPixelX();
    this->y = tile_main->getPixelY();
    tile_main->addItem(this);
    tile_section = section_id;
    
    return true;
  }

  return false;
}