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
const uint8_t MapItem::kDEFAULT_COUNT = 1;
const float MapItem::kDELTA_TIME_ONE_POINT = 3000.0;
const float MapItem::kMAX_BRIGHTNESS = 1.2;
const float MapItem::kMIN_BRIGHTNESS = 1.0;
const int MapItem::kUNSET_ID = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapItem::MapItem() : MapThing()
{
  brighter = false;
  core_id = kUNSET_ID;
  visible = true;
  walkover = false;
  
  /* Set the count to 0 since the map item is not configured */
  setCount(0);
}

MapItem::MapItem(TileSprite* frames, int id, std::string name, 
                 std::string description)
       : MapThing(id, name, description)
{
  setFrame(frames, 0, 0);
  brighter = false;
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
 * PROTECTED FUNCTIONS
 *===========================================================================*/

// TODO: Comment
bool MapItem::setTile(Tile* tile, TileSprite* frames, bool no_events)
{
  (void)no_events;
  (void)frames;

  /* Attempt and set the tile */
  if(tile != NULL)
    return tile->addItem(this);
  return false;
}

// TODO: Comment
void MapItem::unsetTile(uint32_t x, uint32_t y, bool no_events)
{
  (void)no_events;

  tile_main[x][y]->unsetItem(this);
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
 *         SDL_Renderer* renderer - the graphical rendering engine pointer
 *         std::string base_path - the base path for resources
 * Output: bool - status if successful
 */
bool MapItem::addThingInformation(XmlData data, int file_index, 
                                  int section_index, SDL_Renderer* renderer, 
                                  std::string base_path)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;

  /* Parse the identifier for setting the item information */
  /*--------------------- SPRITE DATA -----------------*/
  if(identifier == "sprites")
  {
    success &= MapThing::addThingInformation(data, file_index, section_index, 
                                             renderer, base_path);
    if(success)
    {
      if(sprite_set->height() != 1 || sprite_set->width() != 1)
      {
        sprite_set->unsetSprites(true);
        success = false;
      }
    }
  }
  /*--------------------- CORE ID --------------------*/
  else if(identifier == "core_id" && elements.size() == 1)
  {
    int id = data.getDataInteger(&success);
    if(success)
      setCoreID(id);
  }
  /*--------------------- COUNT --------------------*/
  else if(identifier == "count" && elements.size() == 1)
  {
    int count = data.getDataInteger(&success);
    if(success)
      setCount(count);
  }
  /*--------------------- WALKOVER --------------------*/
  else if(identifier == "walkover" && elements.size() == 1)
  {
    bool walkover = data.getDataBool(&success);
    if(success)
      setWalkover(walkover);
  }
  /* Proceed to parent */
  else
  {
    success &= MapThing::addThingInformation(data, file_index, section_index, 
                                             renderer, base_path);
  }

  return success;
}

/* Returns the class descriptor, useful for casting */
std::string MapItem::classDescriptor()
{
  return "MapItem";
}

/* Clears out the item construct, void of painting */
void MapItem::clear()
{
  /* Clear out the item variables */
  setCount(0);
  setWalkover(false);
  
  /* Clear out parent */
  MapThing::clear();
}

// TODO: Comment
bool MapItem::cleanMatrix()
{
  bool success = MapThing::cleanMatrix();

  if(success)
  {
    if(sprite_set->width() != 1 || sprite_set->height() != 1)
    {
      success = false;
      sprite_set->unsetSprites();
    }
  }

  return success;
}

/* Returns the core (game representation) ID. -1 if unset */
int MapItem::getCoreID()
{
  return core_id;
}

/* Returns the count of how many of these items are available */
uint16_t MapItem::getCount()
{
  return count;
}

/* Returns if the Map Item can be seen */
bool MapItem::isVisible()
{
  if(count > 0)
    return MapThing::isVisible();
  return false;
}
  
/* Returns if the item is picked up merely by walking over it */
bool MapItem::isWalkover()
{
  return walkover;
}

/* Sets the core (game representation) ID. If invalid, sets to -1 */
void MapItem::setCoreID(int id)
{
  if(id >= 0)
    core_id = id;
  else
    core_id = kUNSET_ID;
}

/* Sets the number of this item */
void MapItem::setCount(uint16_t count)
{
  this->count = count;
}

// TODO: Comment
bool MapItem::setFrame(TileSprite* frame, uint32_t x, uint32_t y, 
                       bool delete_old)
{
  if(x == 0 && y == 0)
    return MapThing::setFrame(frame, x, y, delete_old);
  return false;
}

// TODO: Comment
void MapItem::setFrames(std::vector<std::vector<TileSprite*>> frames, 
                         bool delete_old)
{
  if(frames.size() == 1 && frames.front().size() == 1)
    MapThing::setFrames(frames, delete_old);
}

/* 
 * Description: Sets the connected tile information for the map item. This is
 *              the initial starting point and where the item is initially
 *              placed. If this is unset, the item will not move or paint.
 *              This replaces the generic mapthing call to allow for map 
 *              person in tile to be modified.
 *
 * Inputs: uint16_t section_id - the map id that the tile is from
 *         Tile* new_tile - the tile to set the starting pointer to
 *         bool no_events - don't execute any events when set
 * Output: bool - status if the change was able to occur
 */
// TODO: Remove
//bool MapItem::setStartingTile(uint16_t section_id, Tile* new_tile, 
//                                                   bool no_events)
//{
//  /* Unused variable - used in virtual call for Person */
//  (void)no_events;
//
//  if(new_tile != NULL)
//  {
// TODO: Fix
//    /* Unset the main tile */
//    //if(tile_main != NULL) // TODO: Fix
//    //  tile_main->unsetItem();
//    tile_main = NULL;
//  
//    /* Set the new tile */
//    tile_main = new_tile;
//    this->x = tile_main->getPixelX();
//    this->x_raw = this->x * kRAW_MULTIPLIER;
//    this->y = tile_main->getPixelY();
//    this->y_raw = this->y * kRAW_MULTIPLIER;
//    //tile_main->setItem(this); // TODO:Fix
//    tile_section = section_id;
//    
//    return true;
//  }
//
//  return false;
//}

/* Sets if the item is picked up by merely walking over it */
void MapItem::setWalkover(bool walkover)
{
  this->walkover = walkover;
}

/*
 * Description: Updates the state of the item. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the time elapsed between updates
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 * Output: none 
 */
void MapItem::update(int cycle_time, std::vector<std::vector<Tile*>> tile_set)
{
  if(isTilesSet())
  {
    double brightness = sprite_set->at(0, 0)->getBrightness();

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

    /* Update the brightness */
    sprite_set->at(0, 0)->setBrightness(brightness);

    /* Finally update the thing */
    MapThing::update(cycle_time, tile_set);
  }
}
