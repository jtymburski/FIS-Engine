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
const float MapItem::kMIN_BRIGHTNESS = 0.9;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for this class. Sets up an empty item with no
 *              data.
 *
 * Inputs: none
 */
MapItem::MapItem() : MapThing()
{
  brighter = false;
  walkover = false;

  /* Set the count to default - active status maintained */
  setStartCount(kDEFAULT_COUNT);
}

/*
 * Description: Constructor for this class. Takes data to create the item.
 *
 * Inputs: TileSprite* frames - the frame pointer to render on map
 *         int id - the ID of the thing
 *         std::string name - the name of the thing, default to ""
 *         std::string description - the description of the thing, default to ""
 */
MapItem::MapItem(TileSprite* frames, int id, std::string name,
                 std::string description)
       : MapThing(id, name, description)
{
  setFrame(frames, 0, 0);
  brighter = false;
  walkover = false;

  /* Set the count to the default since the map item is configured */
  setStartCount(kDEFAULT_COUNT);
}

/*
 * Description: Destructor function
 */
MapItem::~MapItem()
{
  clear();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Can the tile be set with the passed frame. Fails if there is
 *              already a item set on said tile
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the sprite frames pointer to set in the tile
 *         bool avoid_player - true that canSet will return false if 0 render
 *                             player is on location. default false
 * Output: bool - true if the set was successful
 */
bool MapItem::canSetTile(Tile* tile, TileSprite* frames, bool avoid_player)
{
  (void)avoid_player;
  (void)frames;

  if(tile != nullptr)
    return !tile->isItemsAtLimit();
  return false;
}

/*
 * Description: Checks if there is data to save for the particular IO. This
 *              is virtualized for all children
 *
 * Inputs: none
 * Output: bool - true if save call will result in text
 */
bool MapItem::isDataToSave()
{
  /* Check parent first */
  if(MapThing::isDataToSave())
    return true;

  /* Check count */
  if(count != start_count)
    return true;

  return false;
}

/*
 * Description: Saves the data for the item. This does not include the item
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         const bool &save_event - true to save the base event set (thing)
 * Output: none
 */
bool MapItem::saveData(FileHandler* fh, const bool &save_event)
{
  bool success = true;
  int drop_id = EnumDb::kBASE_ID_ITEMS + EnumDb::kMAX_COUNT_ITEMS;

  /* Is a dropped item by the active map */
  if(getID() >= drop_id)
  {
    if(getBase() != nullptr)
    {
      fh->writeXmlData("base", getBase()->getID());
      fh->writeXmlData("startpoint", std::to_string(getStartingX()) + "," +
                                     std::to_string(getStartingY()));
      fh->writeXmlData("startcount", getCount());
      fh->writeXmlData("walkover", isWalkover());
    }
  }
  /* Was there when the map was created by the designer */
  else
  {
    /* Parent property saves */
    success &= MapThing::saveData(fh, save_event);

    /* Count */
    if(count != start_count)
      fh->writeXmlData("count", count);
  }

  return success;
}

/*
 * Description: Sets the tile with the given object and frames and allows for
 *              event triggers to be controlled (relevant for MapPerson only).
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the frames pointer to set in the tile
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
bool MapItem::setTile(Tile* tile, TileSprite* frames, bool no_events)
{
  (void)no_events;
  (void)frames;

  /* Attempt and set the tile */
  if(tile != NULL)
    return tile->addItem(this);
  return false;
}

/*
 * Description: Unsets the tile corresponding to the matrix at the x and y
 *              coordinate. However, since this is an private function, it does
 *              not confirm that the X and Y are in the valid range. Must be
 *              checked or results are unknown. This will unset the thing from
 *              the tile corresponding to the frame and the tile from the frame.
 *
 * Inputs: uint32_t x - the x coordinate of the frame (horizontal)
 *         uint32_t y - the y coordinate of the frame (vertical)
 *         bool no_events - should events trigger with the change?
 * Output: none
 */
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
 *         bool from_save - true if the load is from a save file
 * Output: bool - status if successful
 */
bool MapItem::addThingInformation(XmlData data, int file_index,
                                  int section_index, SDL_Renderer* renderer,
                                  std::string base_path, bool from_save)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;

  /* Parse the identifier for setting the item information */
  /*------------------- DELTA COUNT --------------------*/
  if(identifier == "count" && elements.size() == 1)
  {
    int count = data.getDataInteger(&success);
    if(success && count >= 0 && static_cast<uint32_t>(count) <= start_count)
      setCount(count);
  }
  /*--------------------- SPRITE DATA -----------------*/
  else if(identifier == "sprites")
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
  /*------------------- START COUNT --------------------*/
  else if(identifier == "startcount" && elements.size() == 1)
  {
    int count = data.getDataInteger(&success);
    if(success)
      setStartCount(count);
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
                                             renderer, base_path, from_save);
  }

  /* If not from save, reset changed back to false */
  if(!from_save)
    changed = false;

  return success;
}

/*
 * Description: This is the class descriptor. Primarily used for encapsulation
 *              to determine which class to cast it to for specific parameters.
 *              This is the virtual re-do of the parent class function.
 *
 * Inputs: none
 * Output: ThingBase - the thing description enum defining the type to cast
 */
ThingBase MapItem::classDescriptor()
{
  return ThingBase::ITEM;
}

/*
 * Description: Takes the frame matrix, as it's been set up and removes any
 *              rows or columns at the end that have no valid frames set. A
 *              frame is classified as valid if it's not NULL and has renderable
 *              frames stored within it. This controls the virtual equivalent
 *              in MapThing and also adds the additional check of only
 *              one frame is allowed in the item.
 *
 * Inputs: bool first_call - is this the first call? default true
 * Output: bool - true if clean validated frame data
 */
bool MapItem::cleanMatrix(bool first_call)
{
  bool success = MapThing::cleanMatrix();
  (void)first_call;

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

/*
 * Description: Clears out all information stored in the item class. This
 *              overwrites the thing function for extra functionality.
 *
 * Inputs: none
 * Output: none
 */
void MapItem::clear()
{
  /* Clear out the item variables */
  setCount(0);
  setStartCount(0);
  setWalkover(false);

  /* Clear out parent */
  MapThing::clear();
}

/*
 * Description: Returns the number of items in the MapItem stack.
 *
 * Inputs: none
 * Output: uint32_t - unsigned integer to represent count
 */
uint32_t MapItem::getCount()
{
  return count;
}

/*
 * Description: Returns the starting number of items in the MapItem stack.
 *
 * Inputs: none
 * Output; uint32_t - unsigned integer to represent starting count
 */
uint32_t MapItem::getStartCount()
{
  return start_count;
}

/*
 * Description: Returns if the item is currently active. Supercedes the parent
 *              function as active is defined by count.
 *
 * Inputs: none
 * Output: bool - true if the item is presently active
 */
bool MapItem::isActive()
{
  return (this->active && (count > 0));
}

/*
 * Description: Returns if the item is walkover (picked up when the person
 *              enters the tile) or not (picked up only when the person is on
 *              the tile and the action tile is selected).
 *
 * Inputs: none
 * Output: bool - true if the item is a walkover
 */
bool MapItem::isWalkover()
{
  //if(base != NULL && base_category == ThingBase::ITEM)
  //  return static_cast<MapItem*>(base)->walkover;
  return walkover;
}

/*
 * Description: Saves the item data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 * Output: bool - true if the save was successful
 */
bool MapItem::save(FileHandler* fh)
{
  if(fh != nullptr)
  {
    bool success = true;
    bool drop_item = (getID() >= (EnumDb::kBASE_ID_ITEMS +
                                  EnumDb::kMAX_COUNT_ITEMS));

    /* Only write if there is data to save */
    if((drop_item && getCount() > 0) || (!drop_item && isDataToSave()))
    {
      fh->writeXmlElement("mapitem", "id", getID());
      success &= saveData(fh, false);
      fh->writeXmlElementEnd();
    }

    return success;
  }
  return false;
}

/*
 * Description: Sets if the item is active and usable within the space
 *
 * Inputs: bool active - true if the item should be active. false otherwise
 *         bool set_tiles - true to set/unset tiles as well. default true.
 * Output: bool - returns if the item is active
 */
bool MapItem::setActive(bool active, bool set_tiles)
{
  bool previous = this->active;
  bool next = MapThing::setActive(active, set_tiles);

  /* Checks if status changed from false to true, update count */
  // TODO: Consideration to be given for on respawn time, to also refresh
  //       count even if there is items left but less than start count
  if(!previous && next)
    count = start_count;
  else if(!next)
    count = 0;

  return next;
}

/*
 * Description: Sets the base thing class. If set, the primary data will be set
 *              from this with only location and movement handled by this class.
 *
 * Inputs: MapThing* base - the reference base class
 * Output: bool - true if the base was set
 */
bool MapItem::setBase(MapThing* base)
{
  bool success = false;

  if(classDescriptor() == ThingBase::ITEM)
  {
    if(base != NULL && base->classDescriptor() == ThingBase::ITEM)
    {
      this->base = base;
      base_category = ThingBase::ITEM;
      setVisibility(base->isVisible());
      success = true;
    }
    else if(base == NULL)
    {
      this->base = NULL;
      base_category = ThingBase::ISBASE;
      success = true;
    }

    /* Update the event set */
    setEventBase();
  }

  return success;
}

/*
 * Description: Sets the number of items in the MapItem stack.
 *
 * Inputs: uint32_t count - unsigned integer for number of items
 *         bool set_tiles - set tiles on the active delta. Default true
 * Output: none
 */
void MapItem::setCount(uint32_t count, bool set_tiles)
{
  this->count = count;
  if(count > 0)
    setActive(true, set_tiles);
  else
    setActive(false, set_tiles);
}

/*
 * Description: Sets a single frame sequence in the matrix of the rendering
 *              item. This takes over control of MapThing function. If the
 *              x and y aren't 0 (as only one frame is allowed in item),
 *              the call fails.
 *
 * Inputs: TileSprite* frame - the new frame to insert into the Map Thing
 *                             matrix. Must actually have a sprite set.
 *         uint32_t x - the x coordinate, relative to the top left of the
 *                      Map Thing render matrix.
 *         uint32_t y - the y coordinate, relative to the top left of the
 *                      Map Thing render matrix.
 *         bool unset_old - delete the old frames from memory?
 * Output: bool - returns if the new frame was set successfully
 */
bool MapItem::setFrame(TileSprite* frame, uint32_t x, uint32_t y,
                       bool delete_old)
{
  if(x == 0 && y == 0)
    return MapThing::setFrame(frame, x, y, delete_old);
  return false;
}

/*
 * Description: Sets the frames that defines the item. This takes over control
 *              of the MapThing function. If the size of array isn't 1x1 (as
 *              only one frame is allowed in item), the call fails.
 *
 * Inputs: std::vector<std::vector<TileSprite*>> frames - the new matrix of
 *             frames to define to insert into the thing
 *         bool delete_old - delete the old frames from memory?
 * Output: none
 */
void MapItem::setFrames(std::vector<std::vector<TileSprite*>> frames,
                         bool delete_old)
{
  if(frames.size() == 1 && frames.front().size() == 1)
    MapThing::setFrames(frames, delete_old);
}

/*
 * Description: Sets the starting number of items on the map. This is also the
 *              count that respawn uses if it is enabled.
 *
 * Inputs: uint32_t count - the number of items to start with on the pile
 * Output: none
 */
void MapItem::setStartCount(uint32_t count)
{
  start_count = count;
  if(this->active)
    setCount(start_count, false);
}

/*
 * Description: Sets if the item is walkover (picked up when the person
 *              enters the tile) or not (picked up only when the person is on
 *              the tile and the action tile is selected).
 *
 * Inputs: bool walkover - true if the item is a walkover
 * Output: none
 */
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
 *         bool active_map - true if this items section is the active map
 * Output: Floatinate - the delta x and y of the moved item
 */
Floatinate MapItem::update(int cycle_time,
                           std::vector<std::vector<Tile*>> tile_set,
                           bool active_map)
{
  Floatinate delta_move;
  SpriteMatrix* sprite_set = getMatrix();

  if(sprite_set != nullptr && sprite_set->at(0, 0) != nullptr)
  {
    /* Proceed to update sprite rendering */
    SpriteMatrix* sprite_set = getMatrix();
    if(base == nullptr)
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
    }
    /* Instance */
    else
    {
      /* Check active status */
      //if(!isActive() && MapThing::isActive())
      //  count = start_count;
    }

    /* Finally update the thing */
    delta_move = MapThing::update(cycle_time, tile_set, active_map);
  }

  return delta_move;
}
