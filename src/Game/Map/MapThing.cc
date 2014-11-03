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
  frames = NULL;
  MapThing::clear();
}

/* 
 * Description: Constructor for this class. Takes data to create the thing.
 *
 * Inputs: Sprite* frames - the frame data to encapsalate by the thing
 *         uint16_t width - the tile width of the thing
 *         uint16_t height - the tile height of the thing
 *         std::string name - the name of the thing, default to ""
 *         std::string description - the description of the thing, default to ""
 *         int id - the ID of the thing, default to -1
 */
MapThing::MapThing(Sprite* frames, uint16_t width, uint16_t height, 
                   std::string name, std::string description, int id) 
        : MapThing()
{
  setDescription(description);
  //setFrames(frames); // TODO: Repair with constructor
  setHeight(height);
  setID(id);
  setName(name);
  setWidth(width);
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
 * Description: Finds a valid TileSprite in the matrix of frames, stored
 *              within the thing. If there are no frames set, it returns
 *              NULL. 
 * Note: Do not delete the pointer. It'll cause issues in the class.
 * 
 * Inputs: none
 * Output: TileSprite* - the tile pointer found with data
 */
TileSprite* MapThing::getValidFrame()
{
  bool found = false;
  uint32_t i = 0;
  uint32_t j = 0;
  TileSprite* valid_frame = NULL;

  while(!found && i < frame_matrix.size())
  {
    j = 0;

    while(!found && j < frame_matrix[i].size())
    {
      std::cout << frame_matrix[i][j] << std::endl;
      if(frame_matrix[i][j] != NULL)
      {
        valid_frame = frame_matrix[i][j];
        found = true;
      }
      j++;
    }
    i++;
  }

  return valid_frame;
}

/* 
 * Description: Grow the frame matrix to be capable of addressing a tile at
 *              the indicated size. If the tile is already feasible, nothing
 *              happens.
 * 
 * Inputs: uint32_t x - row tile coordinate, for the matrix
 *         uint32_t y = col tile coordinate, for the matrix
 * Output: none
 */
void MapThing::growMatrix(uint32_t x, uint32_t y)
{
  TileSprite* null_sprite = NULL;

  /* Determine if the array is empty or not -> different processing */
  if(frame_matrix.size() == 0)
  {
    for(uint32_t i = 0; i <= x; i++)
    {
      std::vector<TileSprite*> col_data;

      for(uint32_t j = 0; j <= y; j++)
        col_data.push_back(null_sprite);

      frame_matrix.push_back(col_data);
    }
  }
  else
  {
    /* First deal with the x direction, if it's less than the new dimension */
    if(frame_matrix.size() <= x)
    {
      uint32_t height = frame_matrix[0].size();
      std::vector<TileSprite*> col_data;

      /* Generate the column of data */
      for(uint32_t i = 0; i < height; i++)
        col_data.push_back(null_sprite);

      /* Push back the necessary number of columns to bring it up to size */
      for(uint32_t i = frame_matrix.size(); i <= x; i++)
        frame_matrix.push_back(col_data);
    }

    /* Finally, deal with the y direction - we can assume enough rows */
    if(frame_matrix[0].size() <= y)
    {
      for(uint32_t i = 0; i < frame_matrix.size(); i++)
      {
        for(uint32_t j = frame_matrix[i].size(); j <= y; j++)
          frame_matrix[i].push_back(null_sprite);
      }
    }
  }
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
  if(frames != NULL)
  {
    /* Reset back to head */
    if(reset && !skip_head && !frames->isAtFirst())
    {
      frames->setAtFirst();
      shift = true;
    }
    
    if(reset)
      shift |= frames->update(0, skip_head);
    else
      shift |= frames->update(cycle_time, skip_head);
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
  if(tile_main != NULL)
  {
    int x_diff = tile_main->getPixelX();
    int y_diff = tile_main->getPixelY();

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
  
  return false;
}

/* 
 * Description: Checks if a move is allowed from the current thing main 
 *              tile and the next tile that it is trying to move to. Utilizes
 *              the move request inside the thing class to determine where to
 *              move.
 * 
 * Inputs: Tile* next_tile - the next tile that corresponds to the move request
 *                           direction.
 * Output: bool - returns if the move is allowed.
 */
bool MapThing::isMoveAllowed(Tile* next_tile)
{
  if(tile_main != NULL && next_tile != NULL && isMoveRequested())
  {
    Direction move_request = getMoveRequest();
    bool thing_passable = true;

    /* Determine if the thing is passable */
    if(next_tile->getThing() != NULL)
      thing_passable = next_tile->getThing()->isPassable();

    return thing_passable && tile_main->getPassabilityExiting(move_request) && 
           next_tile->getPassabilityEntering(move_request);
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
  if(tile_previous != NULL)
    tile_previous->unsetThing();
  tile_previous = NULL;
}

/* 
 * Description: The tile move initialization call. To be called after
 *              passability checks have passed and the thing can be moved to
 *              the next tile. Sets the new main pointer and moves the current
 *              to the old spot.
 * 
 * Inputs: Tile* next_tile - the tile to move to
 * Output: bool - if the tile start was successfully started
 */
bool MapThing::tileMoveStart(Tile* next_tile)
{
  if(next_tile != NULL && !next_tile->isThingSet())
  {
    tile_previous = tile_main;
    tile_main = next_tile;
    tile_main->setThing(this);
    return true;
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
  /*--------------------- SPEED -----------------*/
  else if(identifier == "speed" && elements.size() == 1)
  {
    setSpeed(data.getDataInteger(&success));
  }
  /*------------------- OLD SPRITE DATA ---------------*/
  else if(identifier == "sprite") // TODO: REMOVE - not needed
  {
    if(frames == NULL)
      frames = new Sprite();
    success &= frames->addFileInformation(data, file_index + 1, 
                                          renderer, base_path);
  }
  /*--------------------- SPRITE DATA -----------------*/
  else if(identifier == "sprites") // TODO: Develop
  {
    std::cout << "Sprites setup" << std::endl;
    
    /* Only proceed if there are elements within the sprites element */
    if(elements.size() > 2)
    {
      if(elements[1] == "multiple")
      {
        /* Get the range and ensure there are that many sprites in the array */
        /* Note: if other sprites exist, make sure to copy all settings */
        
        /* Do an if to check if its a path or passability */
          /* If path, get the matrix array and separate */
            /* Then, proceed to insert the new sprites into the frame matrix */
            /* If the sprite at the path don't exist, it'll be unset. */
        
          /* Else if passability, proceed to insert the passability into 
           * the sprite - possibly put passability in the sprite itself */

        std::cout << "multiple sprites" << std::endl;
      }
      else if(elements[1] == "x")
      {
        /* Make sure there's X, Y, and then another element within */
        /* If not, fill in the matrix of sprites up to it */
        
        /* Check to see if the frame matrix is of size to fit the X,Y coord */
        /* Now, with the selected sprite, mirror the functionality above
         * for multiple but with only one path */
        
        // TODO
        std::cout << "single sprite" << std::endl;
      }
      else if(elements[1] == "sprite")
      {
        /* These are the sprite settings */
        /* Check and implement on every existing sprite in the matrix */
        /* If there is no sprites, make one at the origin (0,0) */
        
        // TODO
        std::cout << "sprite details" << std::endl;
      }
    }

    // TODO
    /* Once all the implementation is done above, design render matrix */
    /* And then, build the functions around accessing and painting */
    /* There also needs to be the storing in Tile dealt with... */
    std::cout << "-------------------------" << std::endl;
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
  /* Reset tile parameters */
  tile_main = NULL;
  tile_previous = NULL;
  tile_section = 0;
  
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
  x = 0;
  x_raw = 0;
  y = 0;
  y_raw = 0;

  unsetFrames();
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
  TileSprite* frame = NULL;

  if(frame_matrix.size() > x && frame_matrix[x].size() > y)
  {
    frame = frame_matrix[x][y];
  }

  return frame; 
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
  return frame_matrix;
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
 * Output: Direction - the movement direction indicator
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
 * Description: Returns the tile that the thing is currently at. Used for all
 *              indication of where the thing is going and if someone talks to
 *              it while moving, etc.
 * 
 * Inputs: none
 * Output: Tile* - the tile pointer of the tile this thing resides at
 */
Tile* MapThing::getTile()
{
  return tile_main;
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
  return (x % getWidth() == 0) && 
         (y % getHeight() == 0);
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
 * Description: The render function for the thing. This takes the active state
 *              and renders it based on location and offset (from paint engine)
 *              and if it is set within the thing class.
 * 
 * Inputs: SDL_Renderer* renderer - the graphical rendering engine pointer
 *         int offset_x - the paint offset in the x direction
 *         int offset_y - the paint offset in the y direction
 * Output: bool - if the render succeeded
 */
bool MapThing::render(SDL_Renderer* renderer, int offset_x, int offset_y)
{
  if(isVisible() && frames != NULL && tile_main != NULL)
  {
    int render_x = x - offset_x;
    int render_y = y - offset_y;
    
    frames->render(renderer, render_x, render_y, width, height);
    return true;
  }
  return false;
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
  /* Only proceed if frame isn't NULL */
  if(frame != NULL)
  {
    /* Grow the matrix, if necessary */
    if(x >= frame_matrix.size() || y >= frame_matrix[0].size())
      growMatrix(x, y);

    /* Remove existing, if relevant */
    unsetFrame(x, y, delete_old);

    frame_matrix[x][y] = frame;

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
  /* First, unset the existing sprites */
  unsetFrames(delete_old);

  /* Set the new frames */
  frame_matrix = frames;
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
 * Description: Sets the connected tile information for the map thing. This is
 *              the initial starting point and where the thing is initially
 *              placed. If this is unset, the thing will not move or paint.
 *
 * Inputs: uint16_t section_id - the map id that the tile is from
 *         Tile* new_tile - the tile to set the starting pointer to
 *         bool no_events - don't execute any events when set
 * Output: bool - status if the change was able to occur
 */
bool MapThing::setStartingTile(uint16_t section_id, Tile* new_tile, 
                                                    bool no_events)
{
  if(new_tile != NULL && !new_tile->isThingSet())
  {
    /* Unset the main tile */
    unsetStartingTile(no_events);
  
    /* Set the new tile */
    tile_main = new_tile;
    this->x = tile_main->getPixelX();
    this->x_raw = this->x * kRAW_MULTIPLIER;
    this->y = tile_main->getPixelY();
    this->y_raw = this->y * kRAW_MULTIPLIER;
    tile_main->setThing(this);
    tile_section = section_id;

    return true;
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
 *         Tile* next_tile - the next tile that the thing is moving towards
 * Output: none 
 */
void MapThing::update(int cycle_time, Tile* next_tile)
{
  (void)next_tile;

  if(tile_main != NULL)
  {
    /* Move the thing */
    moveThing(cycle_time);

    /* Animate the thing */
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
  if(frame_matrix.size() > x && frame_matrix[x].size() > y)
  {
    if(delete_frames && frame_matrix[x][y] != NULL)
      delete frame_matrix[x][y];
    frame_matrix[x][y] = NULL;
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
  if(delete_frames)
  {
    for(uint32_t i = 0; i < frame_matrix.size(); i++)
    {
      for(uint32_t j = 0; j < frame_matrix[i].size(); j++)
      {
        //if(frame_matrix[i][j] != NULL)
          delete frame_matrix[i][j];
        frame_matrix[i][j] = NULL;
      }
    }
  }

  /* Clear out the array */
  for(uint32_t i = 0; i < frame_matrix.size(); i++)
    frame_matrix[i].clear();
  frame_matrix.clear();
}

/*
 * Description: Unsets the starting tile location that is stored within the
 *              thing.
 *
 * Inputs: bool no_events - fire no events when unsetting (virtual for person)
 * Output: none
 */
void MapThing::unsetStartingTile(bool no_events)
{
  (void)no_events;
  
  /* Unset the main tile */
  if(tile_main != NULL)
    tile_main->unsetThing();
  tile_main = NULL;
  
  /* Resets the coordinates */
  this->x = 0;
  this->x_raw = 0;
  this->y = 0;
  this->y_raw = 0;
}
