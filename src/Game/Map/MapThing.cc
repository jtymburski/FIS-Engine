/******************************************************************************
 * Class Name: MapThing
 * Date Created: Dec 2 2012
 * Inheritance: none
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapWalkOver, MapSolid, etc. Also handles the basic setup for 
 *              name, description, id, sprite. No interaction is handled in 
 *              this class since its a generic parent.
 *****************************************************************************/
#include "Game/Map/MapThing.h"

/* Constant Implementation - see header file for descriptions */
const uint16_t MapThing::kDEFAULT_SPEED = 10;
const int MapThing::kPLAYER_ID = 0;
const uint8_t MapThing::kRAW_MULTIPLIER = 10;
const int MapThing::kUNSET_ID = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Sets up an empty thing with no
 *              data.
 *
 * Inputs: none
 */
MapThing::MapThing()
{
  event_handler = NULL;
  MapThing::clear();
}

/* 
 * Description: Constructor for this class. Takes data to create the thing.
 *
 * Inputs: uint16_t width - the tile width of the thing
 *         uint16_t height - the tile height of the thing
 *         std::string name - the name of the thing, default to ""
 *         std::string description - the description of the thing, default to ""
 *         int id - the ID of the thing, default to -1
 */
MapThing::MapThing(uint16_t width, uint16_t height, std::string name, 
                   std::string description, int id)
        : MapThing()
{
  setDescription(description);
  setHeight(height);
  setID(id);
  setName(name);
  setWidth(width);
}

/* 
 * Description: Constructor for this class. Takes data to create the thing and
 *              also includes the frame data.
 *
 * Inputs: std::vector<std::vector<TileSprite*>> - the frame matrix data
 *         uint16_t width - the tile width of the thing
 *         uint16_t height - the tile height of the thing
 *         std::string name - the name of the thing, default to ""
 *         std::string description - the description of the thing, default to ""
 *         int id - the ID of the thing, default to -1
 */
MapThing::MapThing(std::vector<std::vector<TileSprite*>> frames, uint16_t width,
                   uint16_t height, std::string name, std::string description,
                   int id)
        : MapThing(width, height, name, description, id)
{
  setFrames(frames, true);
}

/* 
 * Description: Destructor function 
 */
MapThing::~MapThing()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

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
void MapThing::unsetTile(uint32_t x, uint32_t y, bool no_events)
{
  (void)no_events;
  uint8_t render_depth = sprite_set.at(x, y)->getRenderDepth();

  /* Remove from main tile, if applicable */
  if(sprite_set.at(x, y)->isTileMainSet())
    sprite_set.at(x, y)->getTileMain()->unsetThing(render_depth);

  /* Remove from previous tile, if applicable */
  if(sprite_set.at(x, y)->isTilePreviousSet())
    sprite_set.at(x, y)->getTilePrevious()->unsetThing(render_depth);

  /* Clean up frame */
  sprite_set.at(x, y)->resetTile();
}
  
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Animates the frames of the thing, based on the animate offset
 *              of the thing as well as calls to the sprite holder
 * 
 * Inputs: int cycle_time - the msec time between the last animate call
 *         bool reset - Resets the animation back to head. Used for either 
 *                      restarting animation or stopping it.
 *         bool skip_head - Skip the head of the list of frames
 * Output: bool - a status on the animate, if the frame sequence changed.
 */
bool MapThing::animate(int cycle_time, bool reset, bool skip_head)
{
  bool shift = false;
  
  /* Check if an animation can occur */
  for(uint16_t i = 0; i < sprite_set.width(); i++)
  {
    for(uint16_t j = 0; j < sprite_set.height(); j++)
    {
      if(sprite_set.at(i, j) != NULL)
      {
        /* Reset back to head */
        if(reset && !skip_head && !sprite_set.at(i, j)->isAtFirst())
        {
          sprite_set.at(i, j)->setAtFirst();
          shift = true;
        }
    
        if(reset)
          shift |= sprite_set.at(i, j)->update(0, skip_head);
        else
          shift |= sprite_set.at(i, j)->update(cycle_time, skip_head);
      }
    }
  }

  return shift;
}

/* 
 * Description: Is the thing almost on the tile (less than 1 pulse away).
 *              This is used to check movement limits to allow the update
 *              cycle to know when passability needs to be checked or when
 *              to stop the man from moving.
 * 
 * Inputs: int cycle_time - time since the last update call (ms)
 * Output: bool - is the thing almost on the tile?
 */
bool MapThing::isAlmostOnTile(int cycle_time)
{
  int x_diff = tile_x * width;
  int y_diff = tile_y * height;

  /* X differential calculations to ensure positive number */
  if(x_diff > x)
    x_diff = x_diff - x;
  else
    x_diff = x - x_diff;
      
  /* Y differential calculations to ensure positive number */
  if(y_diff > y)
    y_diff = y_diff - y;
  else
    y_diff = y - y_diff;

  return ((moveAmount(cycle_time) / kRAW_MULTIPLIER) >= (x_diff + y_diff));
}

/* 
 * Description: Checks if a move is allowed from the current thing main 
 *              tile and the next tile that it is trying to move to. Utilizes
 *              the move request inside the thing class to determine where to
 *              move.
 * 
 * Inputs: std::vector<std::vector<Tile*>> tile_set - tile set to check against
 *             frame matrix for moving into these tiles
 * Output: bool - returns if the move is allowed.
 */
bool MapThing::isMoveAllowed(std::vector<std::vector<Tile*>> tile_set)
{
  bool move_allowed = true;
  
  if(tile_set.size() > 0 && 
     tile_set.size() == sprite_set.width() && 
     tile_set.front().size() == sprite_set.height())
  {
    for(uint16_t i = 0; move_allowed && (i < sprite_set.width()); i++)
    {
      bool found = false;
      
      for(uint16_t j = 0; !found && (j < sprite_set.height()); j++)
      {
        if(sprite_set.at(i, j) != NULL)
        {
          if(tile_set[i][j] == NULL || 
             tile_set[i][j]->isThingSet(sprite_set.at(i, j)->getRenderDepth()))
          {
            move_allowed = false;
          }
          found = true;
        }
      }
    }
    
    return move_allowed;
  }
  
  return false;
}

/* 
 * Description: Calculates the move amount based on the cycle time and the 
 *              speed for how many pixels should be shifted. The calculation
 *              is based on 16ms for 2 pixel at speed 10.
 * 
 * Inputs: int cycle_time - the time since the last update call
 * Output: int - the move amount in pixels x 10
 */
int MapThing::moveAmount(int cycle_time)
{
  int move_amount = cycle_time * speed * 0.125;
  //int move_amount = 2 * speed;
  if(move_amount > width)
    move_amount = width;
  
  return move_amount;
}

/* 
 * Description: Move the thing based on the cycle update time. Utilizes the 
 *              moveAmount() calculation for determining how much to move.
 * 
 * Inputs: int cycle_time - the time since the last update call
 * Output: none
 */
void MapThing::moveThing(int cycle_time)
{
  int move_amount = moveAmount(cycle_time);
  
  if(movement == Direction::EAST)
  {
    x_raw += move_amount;
    x = x_raw / kRAW_MULTIPLIER;
  }
  else if(movement == Direction::WEST)
  {
    x_raw -= move_amount;
    x = x_raw / kRAW_MULTIPLIER;
  }
  else if(movement == Direction::SOUTH)
  {
    y_raw += move_amount;
    y = y_raw / kRAW_MULTIPLIER;
  }
  else if(movement == Direction::NORTH)
  {
    y_raw -= move_amount;
    y = y_raw / kRAW_MULTIPLIER;
  }
}

/* 
 * Description: Sets the direction that the class should be moving in. Once
 *              the direction has been changed, the animation buffer is forced
 *              to occur so that the result will be noticed.
 * 
 * Inputs: Direction new_direction - the new movement direction
 * Output: bool - if the direction changed since before the call
 */
bool MapThing::setDirection(Direction new_direction)
{
  bool changed = false;

  /* Check if the direction is changed */
  if(movement != new_direction)
    changed = true;

  /* Update the direction */
  movement = new_direction;
  
  return changed;
}

/* 
 * Description: The tile move finish call. To be called after a move and it's
 *              determined that the thing is on the main tile (for the first
 *              time). Essentially just cleans up the previous tile pointer.
 * 
 * Inputs: none
 * Output: none
 */
void MapThing::tileMoveFinish()
{
  for(uint16_t i = 0; i < sprite_set.width(); i++)
  {
    for(uint16_t j = 0; j < sprite_set.height(); j++)
    {
      if(sprite_set.at(i, j) != NULL && 
         sprite_set.at(i, j)->getTilePrevious() != NULL)
      {
        sprite_set.at(i, j)->getTilePrevious()
                           ->unsetThing(sprite_set.at(i, j)->getRenderDepth());
        sprite_set.at(i, j)->tileMoveFinish();
      }
    }
  }
}

/* 
 * Description: The tile move initialization call. To be called after
 *              passability checks have passed and the thing can be moved to
 *              the next tile. Sets the new main pointer and moves the current
 *              to the old spot.
 * 
 * Inputs: std::vector<std::vector<Tile*>> tile_set - the next set of frames
 * Output: bool - if the tile start was successfully started
 */
// TODO: Undo if fail??
bool MapThing::tileMoveStart(std::vector<std::vector<Tile*>> tile_set)
{
  bool success = true;
  TileSprite* test_frames = NULL;

  /* Data prechecks -> to confirm equivalency */
  if(tile_set.size() > 0 && 
     tile_set.size() == sprite_set.width() && 
     tile_set.back().size() == sprite_set.height())
  {
    /* Go through each frame and update */
    for(uint16_t i = 0; i < sprite_set.width(); i++)
    {
      for(uint16_t j = 0; j < sprite_set.height(); j++)
      {
        if(sprite_set.at(i, j) != NULL)
        {
          /* Get one test frame, for testing differential on move distance */
          if(test_frames == NULL)
            test_frames = sprite_set.at(i, j);

          success &= sprite_set.at(i, j)->tileMoveStart(tile_set[i][j]);
          success &= tile_set[i][j]->setThing(this, 
                                        sprite_set.at(i, j)->getRenderDepth());
        }
      }
    }

    /* If successful, update move coordinates of class */
    if(success)
    {
      int32_t diff_x = test_frames->getTileMain()->getX() - 
                       test_frames->getTilePrevious()->getX();
      int32_t diff_y = test_frames->getTileMain()->getY() - 
                       test_frames->getTilePrevious()->getY();

      tile_x += diff_x;
      tile_y += diff_y;
    }

    return success;
  }
  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds thing information from the XML file. This will be
 *              virtually called by children where the child will deal with
 *              additional sets needed and then pass call to this. Works off
 *              reading the XML file in a strict manner.
 *
 * Inputs: XmlData data - the read XML data
 *         int file_index - the index in the xml data where this detail begins
 *         int section_index - the map section index of the thing
 *         SDL_Renderer* renderer - the graphical rendering engine pointer
 *         std::string base_path - the base path for resources
 * Output: bool - status if successful
 */
bool MapThing::addThingInformation(XmlData data, int file_index, 
                                   int section_index, SDL_Renderer* renderer, 
                                   std::string base_path)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;
  
  /* Parse the identifier for setting the thing information */
  /*--------------------- DESCRIPTION -----------------*/
  if(identifier == "description" && elements.size() == 1)
  {
    setDescription(data.getDataString(&success));
  }
  /*--------------------- DIALOG IMAGE -----------------*/
  else if(identifier == "image" && elements.size() == 1)
  {
    success &= setDialogImage(base_path + data.getDataString(&success), 
                              renderer);
  }
  /*--------------------- EVENT -----------------*/
  else if(identifier == "event")
  {
    if(event_handler != NULL)
    {
      interact_event = event_handler->
              updateEvent(interact_event, data, file_index + 1, section_index);
    }
    else
      success = false;
  }
  /*--------------------- NAME -----------------*/
  else if(identifier == "name" && elements.size() == 1)
  {
    setName(data.getDataString(&success));
  }
  /*----------------- RENDER MATRIX -----------------*/
  else if(identifier == "rendermatrix" && elements.size() == 1)
  {
    sprite_set.setRenderMatrix(data.getDataString(&success));
  }
  /*--------------------- SPEED -----------------*/
  else if(identifier == "speed" && elements.size() == 1)
  {
    setSpeed(data.getDataInteger(&success));
  }
  /*--------------------- SPRITE DATA -----------------*/
  else if(identifier == "sprites")
  {
    success &= sprite_set.addFileInformation(data, file_index + 1, renderer, 
                                             base_path);
  }
  /*--------------------- STARTPOINT -----------------*/
  else if(identifier == "startpoint")
  {
    std::vector<std::string> points = Helpers::split(data.getDataString(), ',');
    if(points.size() == 2) /* There needs to be an x and y point */
    {
      setStartingLocation(section_index, std::stoul(points[0]), 
                                         std::stoul(points[1]));
    }
    else
    {
      success = false;
    }
  }
  /*--------------------- VISIBILITY -----------------*/
  else if(identifier == "visible")
  {
    setVisibility(data.getDataBool(&success));
  }

  return success;
}
  
/*
 * Description: This is the class descriptor. Primarily used for encapsulation
 *              to determine which class to cast it to for specific parameters.
 *
 * Inputs: none
 * Output: std::string - the string descriptor, it will be the same as the class
 *                       name. For example, "MapThing", "MapPerson", etc.
 */
std::string MapThing::classDescriptor()
{
  return "MapThing";
}

/* 
 * Description: Clears out all information stored in the class
 * 
 * Inputs: none
 * Output: none
 */
void MapThing::clear()
{
  /* Resets the class parameters */
  MapThing::clearAllMovement();
  setDescription("");
  setEventHandler(NULL);
  setID(kUNSET_ID);
  setMovementPaused(false);
  setName("");
  setSpeed(kDEFAULT_SPEED);
  target = NULL;
  
  height = 1;
  visible = true;
  width = 1;
  
  unsetFrames();
  resetLocation();
}

/* 
 * Description: Clears all active movement on the thing. Since the thing is
 *              not movable, this is merely virtual and useful for other calls.
 * 
 * Inputs: none
 * Output: none
 */
void MapThing::clearAllMovement()
{
  movement = Direction::DIRECTIONLESS;
}

/*
 * Description: Clears the given target from this class. Once cleared, the 
 *              movement being paused will be removed to allow for continuing
 *              movement. The target is another MapThing.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::clearTarget()
{
  if(target != NULL)
    setMovementPaused(false);
  target = NULL;
}

/*
 * Description: Takes the frame matrix, as it's been set up and removes any 
 *              rows or columns at the end that have no valid frames set. A
 *              frame is classified as valid if it's not NULL and has renderable
 *              frames stored within it.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::cleanMatrix()
{
  sprite_set.cleanMatrix();
}

/*
 * Description: Returns the bounding box for the entire object of the thing.
 *              This bounding box is in Tile units, void of any tile width
 *              or height considerations. 
 *
 * Inputs: none
 * Output: SDL_Rect - rect definition of bounding box with top left x and y as
 *                    well as width and height.
 */
SDL_Rect MapThing::getBoundingBox()
{
  SDL_Rect rect;

  rect.x = x / width;
  rect.y = y / height;
  rect.w = sprite_set.width();
  rect.h = sprite_set.height();

  return rect;
}

/*
 * Description: Returns the bounding box for the entire object of the thing.
 *              This bounding box is in pixel units, based on the tile width
 *              and height. 
 *
 * Inputs: none
 * Output: SDL_Rect - rect definition of bounding box with top left x and y as
 *                    well as width and height.
 */
SDL_Rect MapThing::getBoundingPixels()
{
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = sprite_set.width() * width;
  rect.h = sprite_set.height() * height;

  return rect;
}

/* 
 * Description: Gets the things description.
 * 
 * Inputs: none
 * Output: std::string - the description for the thing, as a string
 */
std::string MapThing::getDescription()
{
  return description;
}

/* 
 * Description: Gets the dialog image data, that is used in conversational
 *              interaction painting.
 * 
 * Inputs: none
 * Output: Frame* - the dialog image data
 */
Frame* MapThing::getDialogImage()
{
  return &dialog_image;
}

/* 
 * Description: Gets the frame data for the thing at a particular coordinate.
 *              Coordinates are with respect to the top left of the sprite.
 *              If out of range or the frame isn't set, it will return NULL.
 *
 * Inputs: uint32_t x - x coordinate in matrix
 *         uint32_t y - y coordinate in matrix
 * Output: TileSprite* - the pointer to the data for the frame. Do not delete
 *                       since it would seg fault the game.
 */
TileSprite* MapThing::getFrame(uint32_t x, uint32_t y)
{
  return sprite_set.getSprite(x, y);
}

/* 
 * Description: Gets all the frame data for the thing, in vector form.
 *              Coordinates are with respect to the top left of the sprite and
 *              follow the form [x][y]. If there are none set, the matrix will 
 *              be empty.
 *
 * Inputs: none
 * Output: std::vector<std::vector<TileSprite*>> - the matrix of frame pointers
 *             as set in the class. Do not delete any individual frame since it
 *             would seg fault the game.
 */
std::vector<std::vector<TileSprite*>> MapThing::getFrames()
{
  return sprite_set.getMatrix();
}

/* 
 * Description: Gets the height of the internal tile.
 *
 * Inputs: none
 * Output: uint16_t - the height, in pixels
 */
uint16_t MapThing::getHeight()
{
  return height;
}

/* 
 * Description: Gets the things ID. If the ID is not set, it will be an
 *              out of range number (-1)
 *
 * Inputs: none
 * Output: int - the ID for the thing, as an integer
 */
int MapThing::getID()
{
  return id;
}

/*
 * Description: Returns the interaction event that fires upon an interact()
 *              call to this thing.
 *
 * Inputs: none
 * Output: Event - the event fired
 */
Event MapThing::getInteraction()
{
  return interact_event;
}

/* 
 * Description: Gets the map section attributed to the tile(s) that the thing
 *              currently resides on. It will default to 0 (main) if unset.
 *
 * Inputs: none
 * Output: uint16_t - the section ID, based on the map
 */
uint16_t MapThing::getMapSection()
{
  return tile_section;
}

/* 
 * Description: Returns the movement that the class is set to. This indicates
 *              the current direction that it will shift in when an update
 *              call is selected
 * 
 * Inputs: none
 * Output: Direction - the movezment direction indicator
 */
Direction MapThing::getMovement()
{
  return movement;
}

/*
 * Description: Returns if the thing movement has been paused.
 *
 * Inputs: none
 * Output: bool - the movement paused state
 */
bool MapThing::getMovementPaused()
{
  return movement_paused;
}

/* 
 * Description: This is a virtual move request call that needs to be
 *              reimplemented by whoever needs it. It returns what the current
 *              move request is.
 * 
 * Inputs: none
 * Output: Direction - the move request direction indicator
 */
Direction MapThing::getMoveRequest()
{
  return Direction::DIRECTIONLESS;
}

/* 
 * Description: Gets the things name.
 *
 * Inputs: none
 * Output: std::string - the name for the thing, as a string
 */
std::string MapThing::getName()
{
  return name;
}

/* 
 * Description: Returns the speed that the thing is moving in. Default is 150.
 * 
 * Inputs: none
 * Output: uint16_t - the speed integer movement indicator
 */
uint16_t MapThing::getSpeed()
{
  return speed;
}

/*
 * Description: Returns the map thing target that this class is targetted at.
 *              Do not delete this pointer as that would cause errors.
 *
 * Inputs: none
 * Output: MapThing* - the pointer to the targetted thing
 */
MapThing* MapThing::getTarget()
{
  return target;
}

/* 
 * Description: Returns the tile based X of the top left of the bounding box
 *              the thing. If the object is moving, this coordinate will be
 *              where the tile will be, as opposed to where it was. 
 *
 * Inputs: none
 * Output: uint16_t - X tile coordinate
 */
uint16_t MapThing::getTileX()
{
  return tile_x;
}

/* 
 * Description: Returns the tile based Y of the top left of the bounding box
 *              the thing. If the object is moving, this coordinate will be
 *              where the tile will be, as opposed to where it was. 
 *
 * Inputs: none
 * Output: uint16_t - Y tile coordinate
 */
uint16_t MapThing::getTileY()
{
  return tile_y;
}

/* 
 * Description: Gets the width of the internal tile.
 *
 * Inputs: none
 * Output: int - the width, in pixels
 */
uint16_t MapThing::getWidth()
{
  return width;
}

/* 
 * Description: Returns the top left X coordinate of the thing
 * 
 * Inputs: none
 * Output: int - the X coordinate, in pixels, of the top left corner
 */
int MapThing::getX()
{
  return x;
}

/* 
 * Description: Returns the top left Y coordinate of the thing
 * 
 * Inputs: none
 * Output: int - the Y coordinate, in pixels, of the top left corner
 */
int MapThing::getY()
{
  return y;
}

/*
 * Description: The generic interact call. This is what fires when a player
 *              presses a use key and then this searches for if an event is
 *              available and starts it.
 *
 * Inputs: MapPerson* initiator - the pointer to the person that initiated it
 * Output: bool - if the call can occur (Event handler needs to be set
 */
bool MapThing::interact(MapPerson* initiator)
{
  if(event_handler != NULL)
  {
    event_handler->executeEvent(interact_event, initiator, this);
    return true;
  }

  return false;
}

/* 
 * Description: Returns if a move is currently in the request queue.
 * 
 * Inputs: none
 * Output: bool - true if there is a move request.
 */
bool MapThing::isMoveRequested()
{
  return false;
}

/* 
 * Description: Returns if the thing is currently moving.
 * 
 * Inputs: none
 * Output: bool - movement status
 */
bool MapThing::isMoving()
{
  return (movement != Direction::DIRECTIONLESS);
}

/* 
 * Description: Returns if the thing is currently centered on a tile 
 *              (approximately).
 * 
 * Inputs: none
 * Output: bool - tile centered status
 */
bool MapThing::isOnTile()
{
  return (x  == tile_x * getWidth()) && (y == tile_y * getHeight());
}

/*
 * Description: Returns if the thing is passable and can be walked through.
 *              Pertinent to movement on the map.
 *
 * Inputs: none
 * Output: bool - true if the thing can be walked onto
 */
bool MapThing::isPassable()
{
  return false;
}

/*
 * Description: Returns if the tiles have been set, that associate to where the
 *              thing is being rendered to.
 *
 * Inputs: none
 * Output: bool - true if the tiles have been set
 */
bool MapThing::isTilesSet()
{
  return tiles_set;
}

/*
 * Description: Returns if the thing is visible for rendering.
 *
 * Inputs: none
 * Output: bool - visibility status
 */
bool MapThing::isVisible()
{
  return visible;
}

/*
 * Description: Render the entire thing object, at it's designated coordinate
 *              and with the offset passed by the viewport. This does no 
 *              rendering with rendering depths and just renders the object 
 *              flat.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine
 *         int offset_x - the X viewport offset
 *         int offset_y - the Y viewport offset
 * Output: bool - true if the object rendered
 */
bool MapThing::render(SDL_Renderer* renderer, int offset_x, int offset_y)
{
  if(isVisible())
    return sprite_set.render(renderer, x - offset_x, y - offset_y, 
                             width, height);

  return false;
}

/*
 * Description: Render the single frame, located on the designated tile.
 *              This frame will correspond to a given rendering depth.
 *              The offset is passed in from the viewport.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine
 *         Tile* tile - the tile to render the corresponding thing frame on
 *         int offset_x - the X viewport offset
 *         int offset_y - the Y viewport offset
 * Output: bool - true if the object rendered
 */
bool MapThing::render(SDL_Renderer* renderer, Tile* tile, 
                      int offset_x, int offset_y)
{
  if(tile != NULL)
  {
    uint16_t render_tile_x = tile->getX() - tile_x;
    uint16_t render_tile_y = tile->getY() - tile_y;
    TileSprite* render_frame = sprite_set.getSprite(render_tile_x, 
                                                    render_tile_y);
   
    /* If frame is valid and visible, render */
    if(render_frame != NULL && render_frame->getTileMain() == tile 
                            && isVisible())
    {
      int render_x = x + render_tile_x * width - offset_x;
      int render_y = y + render_tile_y * height - offset_y;
      
      return render_frame->render(renderer, render_x, render_y, width, height);
    }
  }
  
  return false;
}

/*
 * Description: Resets the rendering location of the thing. This will also
 *              remove all association of tiles and then cleans up the rendering
 *              coordinate back to default (0).
 *
 * Inputs: none
 * Output: none
 */
void MapThing::resetLocation()
{
  /* Unset the tiles, prior to unseting the point */
  unsetTiles(true);
  
  /* Clean up the point variables */
  tile_section = 0;
  tile_x = 0;
  tile_y = 0;
  this->x = 0;
  this->x_raw = 0;
  this->y = 0;
  this->y_raw = 0;
}

/* 
 * Description: Sets the description that defines the thing.
 *
 * Inputs: std::string new_description - the data for the new description
 * Output: none
 */
void MapThing::setDescription(std::string new_description)
{
  description = new_description;
}

/*
 * Description: Sets the dialog image internal to the map thing. This will be
 *              the image displayed if the particular map thing is involved
 *              in a conversational interaction throughout the map scene
 *
 * Inputs: std::string path - the path to the image data.
 *         SDL_Renderer* renderer - the rendering graphical engine
 * Output: bool - If the path is invalid, this set will fail
 */
bool MapThing::setDialogImage(std::string path, SDL_Renderer* renderer)
{
  return dialog_image.setTexture(path, renderer);
}

/*
 * Description: Sets the event handler to create and manage all existing events
 *              that get fired throughout interaction with the class. This is 
 *              necessary to ensure that any events work.
 *
 * Inputs: EventHandler* event_handler - the new handler pointer (must not be 0)
 * Output: none
 */
void MapThing::setEventHandler(EventHandler* event_handler)
{
  /* Clean up the existing event handler */
  if(this->event_handler != NULL)
    interact_event = this->event_handler->deleteEvent(interact_event);

  /* Set the new event handler and clean up the interact event */
  this->event_handler = event_handler;
  if(event_handler != NULL)
    interact_event = event_handler->createBlankEvent();
}

/*
 * Description: Sets a single frame sequence in the matrix of the rendering
 *              thing.
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
bool MapThing::setFrame(TileSprite* frame, uint32_t x, uint32_t y, 
                        bool delete_old)
{
  if(frame != NULL)
  {
    unsetFrame(x, y, delete_old);
    sprite_set.setSprite(frame, x, y, delete_old);
    return true;
  }
  return false;
}

/*
 * Description: Sets the frames that defines the thing
 *
 * Inputs: std::vector<std::vector<TileSprite*>> frames - the new matrix of 
 *             frames to define to insert into the thing
 *         bool delete_old - delete the old frames from memory?
 * Output: none
 */
void MapThing::setFrames(std::vector<std::vector<TileSprite*>> frames, 
                         bool delete_old)
{
  unsetFrames(delete_old);
  sprite_set.setSprites(frames, delete_old);
}

/*
 * Description: Sets the new tile height for the thing. This number must
 *              be greater than 0 and if it's not, the call will fail.
 *
 * Inputs: uint16_t new_height - the new height of the thing, in pixels
 * Output: bool - status if the height change was successful
 */
bool MapThing::setHeight(uint16_t new_height)
{
  if(new_height > 0)
  {
    float height_tiles = y * 1.0 / height;
    height = new_height;
    y = height_tiles * height;
    y_raw = y * kRAW_MULTIPLIER;
    return true;
  }

  return false;
}

/*
 * Description: Sets the ID for the thing. If out of the allowable range, 
 *              the value is set to an minimum storage value. (Out of range)
 *
 * Inputs: int new_id - the new integer ID to define the thing
 * Output: bool - status if the new_id is within the allowable range.
 */
bool MapThing::setID(int new_id)
{
  /* If the ID is out of range */
  if(new_id <= kUNSET_ID)
  {
    id = kUNSET_ID;
    return false;
  }

  /* Otherwise, the ID is good */
  id = new_id;
  return true;
}

/*
 * Description: Sets the player ID for the thing. This is a constant and used
 *              for referencing during actions throughout.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::setIDPlayer()
{
  id = kPLAYER_ID;
}

/* 
 * Description: Sets the interaction event that gets fired when the interact()
 *              function is called. This requires a relevant event handler
 *              to work and needs to be set first. Once an event is set, the
 *              class it is set in handles deletion of conversation pointers,
 *              if relevant.
 *
 * Inputs: Event interact_event - the event to set in the class
 * Output: bool - if the event could be set
 */
bool MapThing::setInteraction(Event interact_event)
{
  if(event_handler != NULL)
  {
    /* Delete the existing event and set the new event */
    this->interact_event = event_handler->deleteEvent(this->interact_event);
    this->interact_event = interact_event;
    return true;
  }

  return false;
}

/*
 * Description: Sets if the class should be paused. When it's paused, it will
 *              finish walking to the tile and then stop there until unpaused.
 *
 * Inputs: bool paused - if the movement is paused status
 * Output: none
 */
void MapThing::setMovementPaused(bool paused)
{
  movement_paused = paused;
}

/*
 * Description: Sets the name for the thing.
 *
 * Inputs: std::string new_name - the new name for the thing
 * Output: none
 */
void MapThing::setName(std::string new_name)
{
  name = new_name;
}
 
/* 
 * Description: Sets the speed of the thing that it is moving in. Default is
 *              150 and gets set if the speed is invalid (less than 0).
 * 
 * Inputs: uint16_t speed - the speed to set the thing movement to
 * Output: none
 */
void MapThing::setSpeed(uint16_t speed)
{
  this->speed = speed;
}

/*
 * Description: Sets the starting location which needs the map section id and 
 *              an x and y coordinate. 
 *
 * Inputs: uint16_t section_id - the map section id
 *         uint16_t x - the x coordinate of the thing
 *         uint16_t y - the y coordinate of the thing
 * Output: none
 */
void MapThing::setStartingLocation(uint16_t section_id, uint16_t x, uint16_t y)
{
  /* Unset the tiles, currently in use */
  resetLocation();

  /* Set the new tile coordinate */
  this->x = x * getWidth();
  this->x_raw = this->x * kRAW_MULTIPLIER;
  this->y = y * getHeight();
  this->y_raw = this->y * kRAW_MULTIPLIER;
  tile_section = section_id;
  tile_x = x;
  tile_y = y;
}

/* 
 * Description: Sets the starting tiles, for rendering the thing. This tile
 *              set needs to be equal to the size of the bounding box and 
 *              each corresponding frame will be set to the tile. Will fail
 *              if a thing is already set up in the corresponding spot.
 *
 * Inputs: std::vector<std::vector<Tile*>> tile_set - the tile matrix
 *         bool no_events - if no events should occur from setting the thing
 * Output: bool - true if the tiles are set
 */
bool MapThing::setStartingTiles(std::vector<std::vector<Tile*>> tile_set, 
                                bool no_events)
{
  bool success = true;

  if(tile_set.size() > 0 && tile_set.size() == sprite_set.width() && 
     tile_set.back().size() == sprite_set.height())
  {
    /* First, unset all tiles */
    unsetTiles(no_events);

    /* Attempt to set the new tiles */
    for(uint32_t i = 0; success && (i < sprite_set.width()); i++)
    {
      for(uint32_t j = 0; j < success && (j < sprite_set.height()); j++)
      {
        if(sprite_set.at(i, j) != NULL)
        {
          uint8_t render_depth = sprite_set.at(i, j)->getRenderDepth();

          success &= sprite_set.at(i, j)->setTile(tile_set[i][j]);
          if(success)
            success &= tile_set[i][j]->setThing(this, render_depth);
          if(!success)
            sprite_set.at(i, j)->resetTile();
        }
      }
    }

    /* If unsuccessful, unset all that were set */
    unsetTiles(true);

    tiles_set = success;
    return success;
  }
  return false;
}

/* 
 * Description: Sets the target for this map thing. This is only permitted if
 *              the class is currently not targetting anything. However, if the
 *              pointer is null, the target will be cleared. Once the target is
 *              set, the movement will be paused.
 *
 * Inputs: MapThing* target - a map thing target that is to be targetted
 * Output: bool - returns if the target could be set.
 */
bool MapThing::setTarget(MapThing* target)
{
  if((target != NULL && this->target == NULL) || target == NULL)
  {
    if(target == NULL)
      clearTarget();
    else
      setMovementPaused(true);
  
    this->target = target;
    return true;
  }
  
  return false;
}

/* Description: Sets if the thing is visible or not through the rendering 
 *              engine.
 *
 * Inputs: bool visible - status if the thing will be visible
 * Output: none
 */
void MapThing::setVisibility(bool visible)
{
  this->visible = visible;
}

/*
 * Description: Sets the new tile width for the thing. This number must
 *              be greater than 0 and if it's not, the call will fail.
 *
 * Inputs: uint16_t new_width - the new width of the thing, in pixels
 * Output: bool - status if the height change was successful
 */
bool MapThing::setWidth(uint16_t new_width)
{
  if(new_width > 0)
  {
    float width_tiles = x * 1.0 / width;
    width = new_width;
    x = width_tiles * width;
    x_raw = x * kRAW_MULTIPLIER;
    return true;
  }

  return false;
}

/*
 * Description: Updates the frames of the thing. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the ms time to update the movement/animation
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 * Output: none 
 */
void MapThing::update(int cycle_time, std::vector<std::vector<Tile*>> tile_set)
{
  (void)tile_set;
  
  if(isTilesSet())
  {
    moveThing(cycle_time);
    animate(cycle_time);
  }
}
 
/*
 * Description: Unsets an individual frame that is embedded in this as the 
 *              Map Thing and at the x, y coordinates specified.
 *
 * Inputs: uint32_t x - x coordinate of frame to delete
 *         uint32_t y - y coordinate of frame to delete
 *         bool delete_state - should the old frames be deleted?
 * Output: none 
 */
void MapThing::unsetFrame(uint32_t x, uint32_t y, bool delete_frames)
{
  if(sprite_set.getSprite(x, y) != NULL)
  {
    unsetTile(x, y, true);
    sprite_set.unsetSprite(x, y, delete_frames);
  }
}

/*
 * Description: Unsets the frames that are embedded in this as the Map Thing
 *
 * Inputs: bool delete_state - should the old frames be deleted?
 * Output: none 
 */
void MapThing::unsetFrames(bool delete_frames)
{
  unsetTiles(true);
  sprite_set.unsetSprites(delete_frames);
}

/*
 * Description: Unsets the tiles from the thing. This will stop the rendering
 *              process, since it doesn't have any corresponding tiles to render
 *              to. However, the starting coordinate is untouched.
 *
 * Inputs: bool no_events - true if no events will trigger on removal from tile
 * Output: none
 */
void MapThing::unsetTiles(bool no_events)
{
  (void)no_events;
  
  /* Unset in each frame of the thing */
  for(uint16_t i = 0; i < sprite_set.width(); i++)
    for(uint16_t j = 0; j < sprite_set.height(); j++)
      if(sprite_set.at(i, j) != NULL)
        unsetTile(i, j, no_events);

  tiles_set = false;
}
