/*******************************************************************************
 * Class Name: MapItem
 * Date Created: Dec 2 2012
 * Inheritance: MapThing
 * Description: The item class contains the data that is used for pickups
 *              scattered throughout the map. This class expands upon MapThing
 *              to add counts and if it can be picked up.
 ******************************************************************************/
#include "Game/Map/MapItem.h"

/* Constant Implementation - see header file for descriptions */
const short MapItem::kDEFAULT_COUNT = 1;
const float MapItem::kDELTA_TIME_ONE_POINT = 333.0;
const float MapItem::kMAX_BRIGHTNESS = 3.0;
const float MapItem::kMIN_BRIGHTNESS = 1.0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapItem::MapItem() : MapThing()
{
  brighter = false;
  visible = true;
  walkover = false;
  
  /* Set the count to 0 since the map item is not configured */
  setCount(0);
}

MapItem::MapItem(Sprite* frames, int width, int height, QString name,
                 QString description, int id)
       : MapThing(frames, width, height, name, description, id)
{
  brighter = false;
  visible = true;
  walkover = false;
  
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

/*
 * Description: Adds item information from the XML file. This will be
 *              virtually called by children where the child will deal with
 *              additional sets needed and then pass call to this. Works off
 *              reading the XML file in a strict manner. Passes call to parent
 *              after it is complete.
 *
 * Inputs: XmlData data - the read XML data
 *         int file_index - the index in the xml data where this detail begins
 *         int section_index - the map section index of the item
 * Output: bool - status if successful
 */
bool MapItem::addThingInformation(XmlData data, int file_index, 
                                                  int section_index)
{
  QList<QString> elements = data.getTailElements(file_index + 1);
  QString identifier = data.getElement(file_index + 1);
  bool success = true;
  
  /* Parse the identifier for setting the item information */
  /*--------------------- COUNT --------------------*/
  if(identifier == "count" && elements.size() == 1)
  {
    int count = data.getDataInteger(&success);
    if(success)
      success &= setCount(count);
  }
  /*--------------------- VISIBILITY --------------------*/
  if(identifier == "visibility" && elements.size() == 1)
  {
    bool visible = data.getDataBool(&success);
    if(success)
      setVisibility(visible);
  }
  /*--------------------- WALKOVER --------------------*/
  if(identifier == "walkover" && elements.size() == 1)
  {
    bool walkover = data.getDataBool(&success);
    if(success)
      setWalkover(walkover);
  }

  /* Proceed to parent */
  return (success && 
          MapThing::addThingInformation(data, file_index, section_index));
}

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
  setVisibility(true);
  setWalkover(false);
  
  /* Clear out parent */
  MapThing::clear();
}

/* Returns the count of how many of these items are available */
int MapItem::getCount()
{
  return count;
}
  
/* Returns the opacity of the item - overwritten from Map Thing */
float MapItem::getOpacity()
{
  if(!isVisible())
    return 0.0;
  return MapThing::getOpacity();
}

/* Returns if the Map Item can be seen */
bool MapItem::isVisible()
{
  return visible;
}
  
/* Returns if the item is picked up merely by walking over it */
bool MapItem::isWalkover()
{
  return walkover;
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
      tile_main->unsetItem();
    tile_main = 0;
  
    /* Set the new tile */
    tile_main = new_tile;
    this->x = tile_main->getPixelX();
    this->y = tile_main->getPixelY();
    tile_main->setItem(this);
    tile_section = section_id;
    
    return true;
  }

  return false;
}

/* Sets the item visibility */
void MapItem::setVisibility(bool visible)
{
  this->visible = visible;
}

/* Sets if the item is picked up by merely walking over it */
void MapItem::setWalkover(bool walkover)
{
  this->walkover = walkover;
}

/*
 * Description: Updates the state of the item. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: float cycle_time - the time elapsed between updates
 *         Tile* next_tile - the next tile to be travelled onto
 * Output: none 
 */
void MapItem::updateThing(float cycle_time, Tile* next_tile)
{
  /* Update the brightness, to create the item pulse */
  if(frames != 0)
  {
    float brightness = frames->getBrightness();
    
    /* If brighter, increase brightness. */
    if(brighter)
    {
      brightness += cycle_time / kDELTA_TIME_ONE_POINT;
      if(brightness > kMAX_BRIGHTNESS)
      {
        brightness = kMAX_BRIGHTNESS;
        brighter = false;
      }
    }
    /* Otherwise, dim the frames instead */
    else
    {
      brightness -= cycle_time / kDELTA_TIME_ONE_POINT;
      if(brightness < kMIN_BRIGHTNESS)
      {
        brightness = kMIN_BRIGHTNESS;
        brighter = true;
      }
    }
    frames->setBrightness(brightness);
  }
  
  /* Finally update the thing */
  MapThing::updateThing(cycle_time, next_tile);
}
