/*******************************************************************************
 * Class Name: Tile
 * Date Created: December 2, 2012
 * Inheritance: none
 * Description: This class handles the basic tile that is set up on the map.
 *              It is the overall structure. The tile class creates a base, 
 *              enhancer, lower, upper, item, person, and thing to define
 *              all the possibilities on the tile. This also handles printing
 *              its own tile data and ensuring that movement isn't entered 
 *              through the tile. For additional information, read the comments
 *              below for each function.
 ******************************************************************************/
#include "Game/Map/Tile.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t Tile::kLOWER_COUNT_MAX = 5;
const uint8_t Tile::kUPPER_COUNT_MAX = 5;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Set up the tile class to an empty 
 *              null state. In this state, you will have to manually set
 *              the width, height, x, and y coordinates of the tile and then
 *              enable the tile using setStatus()
 *
 * Input: none
 */
Tile::Tile()
{
  event_handler = NULL;
  clear();
}

/* 
 * Description: Constructor for this class. Takes information on the tile 
 *              such as size, location and the parent and sets up the given
 *              tile. This will be framed by an upper class that handles 
 *              overall control (Map).
 *
 * Inputs: EventHandler* event_handler - the handler for all events
 *         uint16_t width - the width of the tile in pixels
 *         uint16_t height - the height of the tile in pixels
 *         uint16_t x - the x location respective to the parent (in tile count)
 *         uint16_t y - the y location respective to the parent (in tile count)
 */
Tile::Tile(EventHandler* event_handler, uint16_t width, uint16_t height, 
                                        uint16_t x, uint16_t y) 
    : Tile()
{
  /* Reset the parameters based on this alternate constructor */
  setEventHandler(event_handler);
  setHeight(height);
  setStatus(ACTIVE);
  setWidth(width);
  setX(x);
  setY(y);
}

/* 
 * Description: Destructor function
 */
Tile::~Tile()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Adds a passability to the given tile based on data from a file
 *              that is read in. The data is a comma delimited directional
 *              list, the classifier is if it's base or lower and the index
 *              corresponds to which lower layer (if applicable).
 * 
 * Inputs: std::string data - the passability data, as mentioned above.
 *         std::string classifier - the layer classifier (base, lower)
 *         std::string index - the lower layer index
 * Output: bool - status if the set was successful
 */
bool Tile::addPassability(std::string data, std::string classifier, 
                                            std::string index)
{
  bool success = true;
  Direction new_direction = Direction::DIRECTIONLESS;
  std::vector<std::string> data_list = Helpers::split(data, ',');
  
  /* Loop through each value of the data list to add */
  for(uint16_t i = 0; i < data_list.size(); i++)
  {
    /* Determine the data identifier first */
    if(data_list[i] == "N")
      new_direction = Direction::NORTH;
    else if(data_list[i] == "E")
      new_direction = Direction::EAST;
    else if(data_list[i] == "S")
      new_direction = Direction::SOUTH;
    else if(data_list[i] == "W")
      new_direction = Direction::WEST;

    /* Add the layer passability to the specific classifier */
    if(classifier == "base")
      success &= setBasePassability(new_direction, true);
    else if(classifier == "lower")
      success &= setLowerPassability(std::stoi(index), new_direction, true);
  }

  return success;
}

/* 
 * Description: Adds in sprite data for the applicable frames. Based on a
 *              classifier, which indicates which layer and an index (which
 *              is only applicable for stackable layers, such as lower and 
 *              upper).
 * 
 * Inputs: Sprite* frames - the sprite data to add in (pointer only).
 *         std::string classifier - the layer classifier (base, lower, etc.)
 *         std::string index - the layering index (for lower and upper)
 * Output: bool - status if addition was successful
 */
bool Tile::addSprite(Sprite* frames, std::string classifier, std::string index)
{
  /* Add the layer data to the specific classifier */
  if(classifier == "base")
    return setBase(frames);
  else if(classifier == "enhancer")
    return setEnhancer(frames);
  else if(classifier == "lower")
    return insertLower(frames, std::stoi(index));
  else if(classifier == "upper")
    return insertUpper(frames, std::stoi(index));
  return false;
}

/*
 * Description: Animates all sprite layer sets on the tile. This allows for 
 *              the fine control over when the timer hits and how it's updated.
 *
 * Inputs: none
 * Output: none
 */
void Tile::animate()
{
  /* Animate the base first */
  if(base != NULL)
    base->shiftNext();

  /* Then the enhancer sprite */
  if(enhancer != NULL)
    enhancer->shiftNext();

  /* Then animate the set of lower layers */
  for(uint8_t i = 0; i < lower.size(); i++)
    if(lower[i] != NULL)
      lower[i]->shiftNext();

  /* Finish by animating the upper set, if set */
  for(uint8_t i = 0; i < upper.size(); i++)
    if(upper[i] != NULL)
      upper[i]->shiftNext();
}

/*
 * Description: Clears the data within the tile. Usually used for cleaning
 *              up after the tile operation is complete. Has the functionality
 *              just to clean out the sprites and not the width/height and
 *              coordinates of the tile.
 *
 * Inputs: bool just_sprites - Only clean up the sprites or all of the class.
 * Output: none
 */
void Tile::clear(bool just_sprites)
{
  /* Clear sprite layer data */
  unsetBase();
  unsetEnhancer();
  unsetItem();
  unsetLower();
  unsetPerson();
  unsetThing();
  unsetUpper();

  if(!just_sprites)
  {
    /* Reset tile parameters */
    setWidth(1);
    setHeight(1);
    setStatus(OFF);

    /* Reset coordinates */
    setX(0);
    setY(0);
  }
  
  clearEvents();
}

/*
 * Description: Clears all events stored within the class. Call only works if 
 *              the event handler has already been set up and is available.
 *              Note that if there is no event handler, no events fire.
 *
 * Inputs: none
 * Output: bool - status if the events were cleared.
 */
bool Tile::clearEvents()
{
  if(event_handler != NULL)
  {
    enter_event = event_handler->deleteEvent(enter_event);
    exit_event = event_handler->deleteEvent(exit_event);
    return true;
  }
  
  return false;
}

/* 
 * Description: Gets the base sprite sequence and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the base sprite pointer
 */
Sprite* Tile::getBase()
{
  return base;
}

/* 
 * Description: Gets the passability for the base portion of the tile. This 
 *              only responds true passability for set sprites. Otherwise,
 *              it returns true.
 *
 * Inputs: Direction dir - the direction to get
 * Output: bool - the base passability directional status
 */
bool Tile::getBasePassability(Direction dir)
{
  if(base != 0)
  {
    if(dir == Direction::DIRECTIONLESS)
      return (base_passability == 
                                static_cast<uint8_t>(Direction::DIRECTIONLESS));
    return ((base_passability & static_cast<uint8_t>(dir)) > 0);
  }

  return false;
}

/* 
 * Description: Gets the enhancer sprite sequence and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the enhancer sprite pointer
 */
Sprite* Tile::getEnhancer()
{
  return enhancer;
}

/* 
 * Description: Gets the height stored within the tile (and all the layers).
 *
 * Inputs: none
 * Output: uint16_t - the height in pixels
 */
uint16_t Tile::getHeight()
{
  return height;
}

/*
 * Description: Returns the item that is stored within the class.
 *
 * Inputs: none
 * Output: MapItem* - the MapItem* pointer stored within
 */
MapItem* Tile::getItem()
{
  return item;
}

/* 
 * Description: Gets the lower sprite(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: std::vector<Sprite*> - the lower sprites, in a vector
 */
std::vector<Sprite*> Tile::getLower()
{
  return lower;
}

/* 
 * Description: Gets the passability for the entire lower portion of the tile.
 *              Only returns for set lower sprites. Otherwise, it returns 
 *              true.
 *
 * Inputs: Direction dir - the direction to get
 * Output: bool - the lower passability directional status
 */
bool Tile::getLowerPassability(Direction dir)
{
  bool passability = true;

  if(dir == Direction::DIRECTIONLESS)
  {
    passability = false;

    for(uint8_t i = 0; i < lower_passability.size(); i++)
      if(lower[i] != 0)
        passability |= getLowerPassability(i, dir);
  }
  else
  {
    for(uint8_t i = 0; i < lower_passability.size(); i++)
      if(lower[i] != 0)
        passability &= getLowerPassability(i, dir);
  }

  return passability;
}

/* 
 * Description: Gets the passability for the lower specific index of the stack.
 *              Only returns an actual value for set sprites. If it's
 *              not set or if it's out of range, it returns true.
 *
 * Inputs: int index - the lower sprite stack index
 *         Direction dir - the direction to get
 * Output: bool - the lower passability directional status
 */
bool Tile::getLowerPassability(uint8_t index, Direction dir)
{
  if(index < lower_passability.size() && lower[index] != NULL)
  {
    if(dir == Direction::DIRECTIONLESS)
      return (lower_passability[index] == 
                                static_cast<uint8_t>(Direction::DIRECTIONLESS));
    return ((lower_passability[index] & static_cast<uint8_t>(dir)) > 0);
  }
  return true;
}

/* 
 * Description: Gets if the tile is passable entering from the given direction.
 *              This does not take into account the status of the thing(s) and 
 *              if it's passable.
 * 
 * Inputs: Direction dir - the direction enumerated for passability
 * Output: bool - status if the tile passability is possible.
 */
bool Tile::getPassabilityEntering(Direction dir)
{
  if(dir == Direction::NORTH)
    return (person == NULL) && getPassabilityExiting(Direction::SOUTH);
  else if(dir == Direction::EAST)
    return (person == NULL) && getPassabilityExiting(Direction::WEST);
  else if(dir == Direction::SOUTH)
    return (person == NULL) && getPassabilityExiting(Direction::NORTH);
  else if(dir == Direction::WEST)
    return (person == NULL) && getPassabilityExiting(Direction::EAST);
  return (person != NULL) || getPassabilityExiting(dir);
}

/*
 * Description: Gets if the tile is passable leaving the given direction.
 *              This does not take into account the status of the thing(s)
 *              and if it's passable.
 *
 * Inputs: Direction dir - the direction enumerated for passability
 * Output: bool - status if the tile passability is possible.
 */
bool Tile::getPassabilityExiting(Direction dir)
{
  if(status != OFF)
  {
    if(dir == Direction::DIRECTIONLESS)
      return (getBasePassability(dir) || getLowerPassability(dir));
    return (getBasePassability(dir) && getLowerPassability(dir));
  }
  return false;
}
  
/*
 * Description: Returns the person pointer that is residing on the tile.
 *
 * Inputs: none
 * Output: MapPerson* - the person pointer
 */
MapPerson* Tile::getPerson()
{
  return person;
}

/* 
 * Description: Returns the X coordinates of the top left of the tile, in
 *              pixels.
 * 
 * Inputs: none
 * Output: int - the X coordinate, in pixels
 */
int Tile::getPixelX()
{
  return (x * width);
}

/* 
 * Description: Returns the Y coordinates of the top left of the tile, in
 *              pixels.
 * 
 * Inputs: none
 * Output: int - the Y coordinate, in pixels
 */
int Tile::getPixelY()
{
  return (y * height);
}

/* 
 * Description: Returns the status the tile is currently classified in. Uses
 *              the enum from Layer. See the corresponding setStatus()
 *
 * Inputs: none
 * Output: Status - public enum from layer identifying state
 */
Tile::TileStatus Tile::getStatus()
{
  return status;
}
  
/*
 * Description: Returns the stored thing from within the tile. 
 *
 * Inputs: none
 * Output: MapThing* - the stored Map Thing object pointer
 */
MapThing* Tile::getThing()
{
  return thing;
}

/* 
 * Description: Gets the upper sprite(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: std::vector<Sprite*> - the upper sprites, in a QList
 */
std::vector<Sprite*> Tile::getUpper()
{
  return upper;
}

/* 
 * Description: Gets the width stored within the tile (and all the layers).
 *
 * Inputs: none
 * Output: uint16_t - the width in pixels
 */
uint16_t Tile::getWidth()
{
  return width;
}

/* 
 * Description: Gets the X coordinate stored within the tile. This is free of
 *              width and height changes and only is a reference.
 *
 * Inputs: none
 * Output: uint16_t - the x coordinate, in tile count
 */
uint16_t Tile::getX()
{
  return x;
}

/* 
 * Description: Gets the Y coordinate stored within the tile. This is free of
 *              width and height changes and only is a reference.
 *
 * Inputs: none
 * Output: uint16_t - the y coordinate, in tile count
 */
uint16_t Tile::getY()
{
  return y;
}

/* 
 * Description: Inserts the new Sprite frame data onto the created lower
 *              portion for this tile.
 *
 * Inputs: Sprite* lower - the lower tile data, as a Sprite set
 *         uint8_t index - the index to where the lower sprite is inserted
 * Output: bool - if the insert call succeeded and the data was used
 */
bool Tile::insertLower(Sprite* lower, uint8_t index)
{
  if(lower != NULL && lower->getSize() > 0 && 
     index < kLOWER_COUNT_MAX)
  {
    /* Increase the size of lower if it isn't big enough */
    Sprite* null_sprite = NULL;
    if(index >= this->lower.size())
    {
      for(int i = this->lower.size(); i <= index; i++)
      {
        this->lower.push_back(null_sprite);
        lower_passability.push_back(
                                static_cast<uint8_t>(Direction::DIRECTIONLESS));
      }
    }

    /* Actually set the lower layer now */
    this->lower[index] = lower;
    lower_passability[index] = static_cast<uint8_t>(Direction::DIRECTIONLESS);

    return true;
  }

  return false;
}

/* 
 * Description: Inserts the new Sprite frame data onto the created upper
 *              portion for this tile. 
 *
 * Inputs: Sprite* upper - the upper tile data, as a Sprite set
 *         uint8_t index - the index to where the upper sprite is inserted
 * Output: bool - if the insert call succeeded and the data was used
 */
bool Tile::insertUpper(Sprite* upper, uint8_t index)
{
  if(upper != 0 && upper->getSize() > 0 && 
     index < kUPPER_COUNT_MAX)
  {
    /* Increase the size of upper if it isn't big enough */
    Sprite* null_sprite = NULL;
    if(index >= this->upper.size())
      for(int i = this->upper.size(); i <= index; i++)
        this->upper.push_back(null_sprite);

    /* Actually set the upper layer now */
    this->upper[index] = upper;

    return true;
  }

  return false;
}

/* 
 * Description: Returns if the Base Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the base sprite is set
 */
bool Tile::isBaseSet()
{
  return (base != NULL);
}

/* 
 * Description: Returns if the Enhancer Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the enhancer sprite is set
 */
bool Tile::isEnhancerSet()
{
  return (enhancer != NULL);
}

/*
 * Description: Returns if the item thing has been set.
 *
 * Inputs: none
 * Output: bool - true if the item pointer is not null.
 */
bool Tile::isItemSet()
{
  return (item != NULL);
}

/* 
 * Description: Returns if the Lower Sprite(s) is(are) set 
 *
 * Inputs: none
 * Output: bool - status if there is at least one lower sprite
 */
bool Tile::isLowerSet()
{
  return (lower.size() > 0);
}

/*
 * Description: Returns if the map person pointer is set and residing on the
 *              tile.
 *
 * Inputs: none
 * Output: bool - true if the person pointer is set
 */
bool Tile::isPersonSet()
{
  return (person != NULL);
}

/*
 * Description: Returns if the map thing pointer is set and residing on the
 *              tile.
 *
 * Inputs:none
 * Output: bool - true if the thing pointer is set
 */
bool Tile::isThingSet()
{
  return (thing != NULL);
}

/* 
 * Description: Returns if the Upper Sprite(s) is(are) set 
 *
 * Inputs: none
 * Output: bool - status if there is at least one upper sprite
 */
bool Tile::isUpperSet()
{
  return (upper.size() > 0);
}

/* 
 * Description: Renders the lower sprites in the tile using SDL calls. The 
 *              sprite initialization must have occurred before any rendering.
 *
 * Inputs: SDL_Renderer* renderer - the sdl graphical rendering context
 *         int offset_x - the offset in X off of base coordinates
 *         int offset_y - the offset in Y off of base coordinates
 * Output: bool - status if the sprite was painted.
 */
bool Tile::renderLower(SDL_Renderer* renderer, int offset_x, int offset_y)
{
  bool success = true;
  int pixel_x = getPixelX() - offset_x;
  int pixel_y = getPixelY() - offset_y;
  
  /* Only proceed if the status isn't off */
  if(status != OFF)
  {
    if(status == ACTIVE)
    {
      /* Paint the base first */
      if(base != NULL)
        success &= base->render(renderer, pixel_x, pixel_y, width, height);
      
      /* Then the enhancer sprite */
      if(enhancer != NULL)
        success &= enhancer->render(renderer, pixel_x, pixel_y, width, height);

      /* Then Paint the set of lower layers */
      for(uint8_t i = 0; i < lower.size(); i++)
        if(lower[i] != NULL)
          success &= lower[i]->render(renderer, pixel_x, pixel_y, 
                                                width, height);
    }
    else if(status == BLANKED)
    {
      SDL_Rect tile_rect = {pixel_x, pixel_y, width, height};
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);		
      SDL_RenderFillRect(renderer, &tile_rect);
    }
  }
  
  return success;
}

/* 
 * Description: Renders the upper sprites in the tile using SDL calls. The 
 *              sprite initialization must have occurred before any rendering.
 *
 * Inputs: SDL_Renderer* renderer - the sdl graphical rendering context
 *         int offset_x - the offset in X off of base coordinates
 *         int offset_y - the offset in Y off of base coordinates
 * Output: bool - status if the sprite(s) were painted. If failed, make sure 
 *         there is an image in the sprite.
 */
bool Tile::renderUpper(SDL_Renderer* renderer, int offset_x, int offset_y)
{
  bool success = true;
  
  /* Only paint if the status is enabled */
  if(status != OFF)
  {
    if(status == ACTIVE)
    {
      int pixel_x = getPixelX() - offset_x;
      int pixel_y = getPixelY() - offset_y;
  
      /* Paint the upper set, if set */
      for(uint8_t i = 0; i < upper.size(); i++)
        if(upper[i] != 0)
          success &= upper[i]->render(renderer, pixel_x, pixel_y, 
                                                width, height);
    }
  }
  return success;
}
  
/* 
 * Description: Sets the base sprite stored within the tile. Only sets it 
 *              if the pointer is valid and the number of frames is greater 
 *              than 0. The old base is set if the new pointer is valid.
 *
 * Inputs: Sprite* base - the new base sprite to attempt to set
 * Output: bool - status if the set procedure succeeded
 */
bool Tile::setBase(Sprite* base)
{
  if(base != NULL && base->getSize() > 0)
  {
    unsetBase();
    this->base = base;
    base_passability = static_cast<uint8_t>(Direction::DIRECTIONLESS);

    return true;
  }
  return false;
}

/* 
 * Description: Sets the base passability using a given direction and the 
 *              set boolean value. Only fails if the base internal to the
 *              tile is not set.
 *
 * Inputs: Direction dir - the direction to set
 *         bool set_value - the value to set it to.
 * Output: bool - the status if the set was successful.
 */
bool Tile::setBasePassability(Direction dir, bool set_value)
{
  /* Only set if the base is set */
  if(base != NULL)
  {
    if(dir == Direction::DIRECTIONLESS && set_value)
      base_passability = static_cast<uint8_t>(Direction::DIRECTIONLESS);
    else
      (set_value) ? (base_passability |= static_cast<uint8_t>(dir)) 
                  : (base_passability &= ~static_cast<uint8_t>(dir));

    return true;
  }
  return false;
}

/* 
 * Description: Sets the enhancer sprite stored within the tile. Only sets it 
 *              if the pointer is valid and the number of frames is greater 
 *              than 0. The old enhancer is set if the new pointer is valid.
 *
 * Inputs: Sprite* enhancer - the new enhancer sprite to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Tile::setEnhancer(Sprite* enhancer)
{
  if(enhancer != NULL && enhancer->getSize() > 0)
  {
    unsetEnhancer();
    this->enhancer = enhancer;

    return true;
  }
  return false;
}

/*
 * Description: Sets the enter event. This goes off when an impassable object
 *              gets set onto the tile.
 *
 * Inputs: Event enter_event - the event to be executed
 * Output: bool - if the setting was able to occur
 */
bool Tile::setEnterEvent(Event enter_event)
{
  if(event_handler != NULL)
  {
    /* Delete the existing event and set new event */
    this->enter_event = event_handler->deleteEvent(this->enter_event);
    this->enter_event = enter_event;
    return true;
  }

  return false;
}

/*
 * Description: Sets the event handler to create and manage all existing events
 *              that get fired throughout interaction with the class. This is 
 *              necessary to ensure that any events work.
 *
 * Inputs: EventHandler* event_handler - the new handler pointer (must not be 0)
 * Output: none
 */
void Tile::setEventHandler(EventHandler* event_handler)
{
  /* Clean up the existing event handler */
  clearEvents();

  /* Set the new event handler and clean up the interact event */
  this->event_handler = event_handler;
  if(event_handler != NULL)
  {
    enter_event = event_handler->createBlankEvent();
    exit_event = event_handler->createBlankEvent();
  }
}

/*
 * Description: Sets the exit event. This goes off when an impassable object
 *              gets cleared off of a tile.
 *
 * Inputs: Event exit_event - the event to be executed
 * Output: bool - status if the event could be set
 */
bool Tile::setExitEvent(Event exit_event)
{
  if(event_handler != NULL)
  {
    /* Delet the existing event and set the new event */
    this->exit_event = event_handler->deleteEvent(this->exit_event);
    this->exit_event = exit_event;
    return true;
  }

  return false;
}
  
/*
 * Description: Sets the height of the tile.
 *
 * Inputs: uint16_t height - the tile height, in pixels
 * Output: none
 */
void Tile::setHeight(uint16_t height)
{
  this->height = height;
}

/*
 * Description: Sets the item in the tile. This call will unset the previous
 *              item and sets it to the new one if not null. 
 *
 * Inputs: MapItem* item - the item to put in the list
 * Output: bool - status if successful (item needs to be non-null)
 */
bool Tile::setItem(MapItem* item)
{
  if(item != NULL)
  {
    /* First unset all existing items */
    unsetItem();

    /* Sets the new item in the tile */
    this->item = item;
    return true;
  }

  return false;
}

/* 
 * Description: Sets the lower sprite stored within the tile. Only sets it if 
 *              the pointer is valid and the number of frames is greater than 
 *              0. Since lower is a stack, it unsets the stack if the sprite
 *              is valid and puts this new lower at the front of the list.
 *
 * Inputs: Sprite* lower - the new lower sprite to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Tile::setLower(Sprite* lower)
{
  if(lower != NULL && lower->getSize() > 0)
  {
    Sprite* null_sprite = NULL;

    unsetLower();
    this->lower.push_back(null_sprite);
    this->lower[0] = lower;
    lower_passability[0] = static_cast<uint8_t>(Direction::DIRECTIONLESS);

    return true;
  }
  return false;
}

/*
 * Description: Sets the lower passability for a sprite based on an index, 
 *              a direction enumerator, and the value it should be. Only 
 *              fails if the index is out of range of the allowable bounds
 *              or the lower isn't set at that index.
 *
 * Inputs: uint8_t index - the lower layer index
 *         Direction dir - the direction passability to set
 *         bool set_value - the value to set that direction to
 * Output: bool - if the set was successful
 */
bool Tile::setLowerPassability(uint8_t index, Direction dir, bool set_value)
{
  /* Only set if the lower layer is valid and in the right range */
  if(index < kLOWER_COUNT_MAX && index < lower.size() && lower[index] != NULL)
  {
    if(dir == Direction::DIRECTIONLESS && set_value)
      lower_passability[index] = static_cast<uint8_t>(Direction::DIRECTIONLESS);
    else
      (set_value) ? (lower_passability[index] |= static_cast<uint8_t>(dir)) 
                  : (lower_passability[index] &= ~static_cast<uint8_t>(dir));

    return true;
  }
  return false;
}

/*
 * Description: Sets the person stored within the tile. The stored person
 *              indicates that the person is either on the tile, walking to
 *              the tile, or walking away from. This allows to disable
 *              walk-on events, if relevant.
 *
 * Inputs: MapPerson* person - the new person pointer to set onto the tile
 *         bool no_events - true if the events are disabled (default false)
 * Output: bool - true if successful (fails if person is null)
 */
bool Tile::setPerson(MapPerson* person, bool no_events)
{
  if(person != NULL)
  {
    /* First unset the existing person */
    unsetPerson();

    /* Set the new person */
    this->person = person;

    /* Execute enter event, if applicable */
    if(!no_events && event_handler != NULL)
    {
      /* Pickup the item, if applicable */
      if(item != NULL)
        event_handler->executePickup(item, true);
      
      /* Execute the enter event, if applicable */
      if(enter_event.classification != EventClassifier::NOEVENT)
        event_handler->executeEvent(enter_event, person);
    }

    return true;
  }

  return false;
}

/* 
 * Description: Sets the tile status. This allows of 3 possible states that 
 *              the tile can be in. This affects the visibility and painting \
 *              of the tile.
 * 
 * Inputs: TileStatus status - the new status to update the tile to
 * Output: none
 */
void Tile::setStatus(TileStatus status)
{
  this->status = status;
}

/*
 * Description: Sets the MapThing pointer in the class. This is the inactive
 *              object that doesn't move but can be interacted with. This will
 *              unset a different thing if it's already on the tile.
 *
 * Inputs: MapThing* thing - the new thing pointer to inject into the tile
 * Output: bool - true if successful, fails if thing ptr is null
 */
bool Tile::setThing(MapThing* thing)
{
  if(thing != NULL)
  {
    /* First unset the existing thing */
    unsetThing();

    /* Set the new thing */
    this->thing = thing;

    return true;
  }

  return false;
}

/* 
 * Description: Sets the upper sprite stored within the tile. Only sets it if 
 *              the pointer is valid and the number of frames is greater than 
 *              0. Since upper is a stack, it unsets the stack if the sprite
 *              is valid and puts this new upper at the front of the list.
 *
 * Inputs: Sprite* upper - the new upper layer to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Tile::setUpper(Sprite* upper)
{
  if(upper != NULL && upper->getSize() > 0)
  {
    unsetUpper();
    this->upper[0] = upper;

    return true;
  }
  return false;
}

/*
 * Description: Sets the width of the tile.
 *
 * Inputs: int width - the tile width, in pixels
 * Output: none
 */
void Tile::setWidth(uint16_t width)
{
  this->width = width;
}

/*
 * Description: Sets the X coordinate of the tile. 
 *
 * Inputs: uint16_t x - the tile x coordinate, in tile count
 * Output: none
 */
void Tile::setX(uint16_t x)
{
  this->x = x;
}

/*
 * Description: Sets the Y coordinate of the tile.
 *
 * Inputs: uint16_t y - the tile y coordinate, in tile count
 * Output: none
 */
void Tile::setY(uint16_t y)
{
  this->y = y;
}

/*
 * Description: Updates the enter event, from the relevant file data. This then
 *              passes the call to the event handler which parses the file data.
 *
 * Inputs: XmlData data - the data calculated from the file
 *         int file_index - the offset index where the event is
 *         int section_index - the relevant map section index
 * Output: bool - returns if the call was successful
 */
bool Tile::updateEnterEvent(XmlData data, int file_index, 
                                          uint16_t section_index)
{
  /* Ensure that the event handler is relevant */
  if(event_handler != NULL)
  {
    /* Update the enter event */
    enter_event = event_handler->updateEvent(enter_event, data, 
                                             file_index, section_index);
    return true;
  }
  
  return false;
}

/*
 * Description: Updates the exit event, from the relevant file data. This then
 *              passes the call to the event handler which parses the file data.
 *
 * Inputs: XmlData data - the data calculated from the file
 *         int file_index - the offset index where the event is
 *         int section_index - the relevant map section index
 * Output: bool - returns if the call was successful
 */
bool Tile::updateExitEvent(XmlData data, int file_index, uint16_t section_index)
{
  /* Ensure that the event handler is relevant */
  if(event_handler != NULL)
  {
    /* Update the exit event */
    exit_event = event_handler->updateEvent(exit_event, data, 
                                            file_index, section_index);
    
    return true;
  }
  
  return false;
}

/* 
 * Description: Unsets the base sprite in the tile.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetBase()
{
  base = NULL;
  base_passability = static_cast<uint8_t>(Direction::DIRECTIONLESS);
}

/* 
 * Description: Unsets the enhancer sprite in the tile.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetEnhancer()
{
  enhancer = NULL;
}

/* 
 * Description: Unsets the item stored within the class.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetItem()
{
  item = NULL;
}

/* 
 * Description: Unsets the lower sprite(s) in the tile. 
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetLower()
{
  /* Disconnect all references */
  for(uint8_t i = 0; i < lower.size(); i++)
  {
    lower[i] = NULL;
    lower_passability[i] = static_cast<uint8_t>(Direction::DIRECTIONLESS);
  }

  /* Clean out the array */
  lower.clear();
  lower_passability.clear();
}

/* 
 * Description: Unsets the lower sprite in the tile based on the index, only
 *              if it exists.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: uint8_t index - the index of the lower to unset
 * Output: bool - status if the unset occurred
 */
bool Tile::unsetLower(uint8_t index)
{
  if(index < lower.size())
  {
    lower[index] = NULL;
    lower_passability[index] = static_cast<uint8_t>(Direction::DIRECTIONLESS);
    return true;
  }
  return false;
}

/*
 * Description: Unsets the person pointer from the tile. This does not delete
 *              the pointer and merely clears it. This allows for you to enable
 *              or disable the leave event from firing as well.
 *
 * Inputs: bool no_events - status if events should be ignored, default false
 * Output: none
 */
void Tile::unsetPerson(bool no_events)
{
  if(!no_events && person != NULL)
  {
    /* Execute exit event, if applicable */
    if(event_handler != NULL && 
       exit_event.classification != EventClassifier::NOEVENT)
      event_handler->executeEvent(exit_event, person);
  }

  person = NULL;
}
  
/*
 * Description: Unsets the map thing stored in the tile. This does not delete
 *              the pointer and merely clears it from the Tile.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetThing()
{
  thing = NULL;
}

/* 
 * Description: Unsets the upper sprite(s) in the tile. 
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetUpper()
{
  /* Disconnect all references */
  for(uint8_t i = 0; i < upper.size(); i++)
    upper[i] = NULL;

  /* Clear out the array */
  upper.clear();
}

/* 
 * Description: Unsets the upper sprite in the tile based on the index, only
 *              if it exists.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: uint8_t index - the index of the upper to unset
 * Output: bool - status if the unset occurred
 */
bool Tile::unsetUpper(uint8_t index)
{
  if(index < upper.size())
  {
    upper[index] = NULL;
    return true;
  }
  return false;
}
