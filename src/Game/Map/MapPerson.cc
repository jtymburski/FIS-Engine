/*******************************************************************************
 * Class Name: MapPerson
 * Date Created: Dec 2 2012
 * Inheritance: MapThing
 * Description: The MapPerson class. An addition on top of MapThing that 
 *              expands the Thing into possible states to allow for walking
 *              in multiple directions and on multiple surfaces. At present, 
 *              it allows for the 4 directions (N,S,E,W) all on one surface
 *              (Ground). Future expansion is available for other surfaces
 *              such as water, flying, etc.
 *
 * TODO: Glitch - when running up and then reverse part way through tile, 
 *                the guy floats for a tiny period of time [2014-07-25]
 ******************************************************************************/
#include "Game/Map/MapPerson.h"

/* Constant Implementation - see header file for descriptions */
const int8_t MapPerson::kDIR_EAST = 1;
const int8_t MapPerson::kDIR_NORTH = 0;
const int8_t MapPerson::kDIR_SOUTH = 2;
const int8_t MapPerson::kDIR_UNKNOWN = -1;
const int8_t MapPerson::kDIR_WEST = 3;
const uint8_t MapPerson::kTOTAL_DIRECTIONS = 4;
const uint8_t MapPerson::kTOTAL_SURFACES   = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Sets up an empty person with no
 *              data.
 *
 * Inputs: none
 */
MapPerson::MapPerson() : MapThing()
{
  initializeStates();
  active_secondary = NULL;
  starting_section = 0;
  starting_tile = NULL;
  steps = 0;
  
  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = Direction::NORTH;
}

/* 
 * Description: Constructor for this class. Sets up a person with the
 *              appropriate height and width (for the tile) and other info
 *              that may be required by the person.
 *
 * Inputs: uint16_t width - the width of the tile for the person
 *         uint16_t height - the height of the tile for the person
 *         std::string name - a string name for the person
 *         std::string description - a description of the person
 *         int id - a unique numerical identifier, for the person
 */
MapPerson::MapPerson(uint16_t width, uint16_t height, std::string name, 
                     std::string description, int id)
          : MapThing(width, height, name, description, id)
{
  initializeStates();
  active_secondary = NULL;
  starting_section = 0;
  starting_tile = NULL;
  steps = 0;
  
  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = Direction::NORTH;
}

/* 
 * Description: Destructor function 
 */
MapPerson::~MapPerson()
{
  clear();
  states.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Initializes the internal states for all possibilities, based
 *              on the constants. Only called once, by the constructor for 
 *              initialization.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::initializeStates()
{
  states.clear();
  states_secondary.clear();

  for(uint8_t i = 0; i < kTOTAL_SURFACES; i++)
  {
    std::vector<Sprite*> row;

    for(uint8_t j = 0; j < kTOTAL_DIRECTIONS; j++)
      row.push_back(NULL);
    states.push_back(row);
    states_secondary.push_back(row);
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Adds the direction from the direction stack, if it's not
 *              currently on the stack. Used for movement of the person.
 *
 * Inputs: Direction direction - the direction to add
 * Output: none
 */
void MapPerson::addDirection(Direction direction)
{
  bool contains = false;
  
  /* Check if the direction already exists */
  for(uint16_t i = 0; i < movement_stack.size(); i++)
    if(movement_stack[i] == direction)
      contains = true;
  
  /* If it doesn't exist, push it onto the stack */
  if(!contains)
    movement_stack.push_back(direction);
}

/* 
 * Description: Animates the frames of the person, based on the animate offset
 *              of the person as well as calls to the sprite holder
 * 
 * Inputs: int cycle_time - the msec time between the last animate call
 *         bool reset - Resets the animation back to head. Used for either 
 *                      restarting animation or stopping it.
 *         bool skip_head - Skip the head of the list of frames
 * Output: bool - a status on the animate, if the frame sequence changed.
 */
bool MapPerson::animate(int cycle_time, bool reset, bool skip_head)
{
  bool shift = false;
  //Sprite* frames = getFrames(); // TODO: Repair
 
  /* Check if an animation can occur */
  if(true)//frames != NULL) // TODO: Fix
  {
    /* Reset back to head */
    if(reset && !skip_head)// && !frames->isAtFirst()) // TODO:Fix
    {
      //frames->setAtFirst(); // TODO: Fix
      if(active_secondary != NULL)
        active_secondary->setAtFirst();

      shift = true;
    }
    
    if(reset)
    {
      //shift |= frames->update(0, skip_head); // TODO: Fix
      if(active_secondary != NULL)
        active_secondary->update(0, skip_head);
    }
    else
    {
      if(true)//frames->update(cycle_time, skip_head)) // TODO: Fix
      {
        if(active_secondary != NULL)
          active_secondary->shiftNext(skip_head);
        shift = true;
      }
    }
  }
  
  return shift;
}

/* 
 * Description: Integer converter from Direction enumerator to ensure that 
 *              the compiler type doesn't affect the operation. This info
 *              is used within internal array operation
 * 
 * Inputs: Direction dir - the direction enumerator
 * Output: int - the converted integer from the enumerator
 */
int MapPerson::dirToInt(Direction dir)
{
  if(dir == Direction::NORTH)
    return kDIR_NORTH;
  else if(dir == Direction::EAST)
    return kDIR_EAST;
  else if(dir == Direction::SOUTH)
    return kDIR_SOUTH;
  else if(dir == Direction::WEST)
    return kDIR_WEST;
  return kDIR_UNKNOWN;
}

/* 
 * Description: Direction enumerator converter from integer to ensure that 
 *              the compiler type doesn't affect the operation. This info
 *              is used within internal array operation
 * 
 * Inputs: int dir_index - the direction index, associated with the enum
 * Output: Direction - the direction enumerator, from the index
 */
Direction MapPerson::intToDir(int dir_index)
{
  if(dir_index == kDIR_NORTH)
    return Direction::NORTH;
  else if(dir_index == kDIR_EAST)
    return Direction::EAST;
  else if(dir_index == kDIR_SOUTH)
    return Direction::SOUTH;
  else if(dir_index == kDIR_WEST)
    return Direction::WEST;
  return Direction::DIRECTIONLESS;
}

/*
 * Description: Removes the direction from the direction stack, if it's
 *              currently on the stack. Used for movement of the person.
 *
 * Inputs: Direction direction - the direction to remove
 * Output: none
 */
void MapPerson::removeDirection(Direction direction)
{
  int index = -1;
  
  /* Check if the direction already exists */
  for(uint16_t i = 0; i < movement_stack.size(); i++)
    if(movement_stack[i] == direction)
      index = i;
  
  /* Try and find the direction */
  if(index >= 0)
    movement_stack.erase(movement_stack.begin() + index);
}

/* 
 * Description: Sets a new direction for the person on the map. It will update
 *              the parent frame so a new classifier is printed.
 * 
 * Inputs: Direction direction - the new direction to set
 *         bool set_movement - if the movement should be set as well
 * Output: bool - indicates if the directional movement changed
 */
bool MapPerson::setDirection(Direction direction, bool set_movement)
{
  bool changed = (this->direction != direction);
  bool movement_changed = false;
  
  /* If moving, set the direction in map thing */
  if(set_movement)
    movement_changed = MapThing::setDirection(direction);
  else
    MapThing::setDirection(Direction::DIRECTIONLESS);

  /* If it's a movement direction, rotate the fellow */
  int surface_index = static_cast<int>(surface);
  int dir_index = dirToInt(direction);
  if(surface_index >= 0 && dir_index >= 0)
  {
    if(changed && states[surface_index][dir_index] != NULL)
    { // TODO: Repair
      //MapThing::setFrames(states[surface_index][dir_index], false);
      active_secondary = getStateSecondary(surface, direction);
    }

    /* Finally set the in class direction */
    this->direction = direction;
  }

  return movement_changed;
}

/* 
 * Description: The tile move finish call. To be called after a move and it's
 *              determined that the thing is on the main tile (for the first
 *              time). Essentially just cleans up the previous tile pointer.
 *              This is reimplemented from MapThing since it uses "Person"
 *              instead of "Thing" in tile.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::tileMoveFinish()
{
  //if(tile_previous != NULL) // TODO: Fix for new matrix person
  //  tile_previous->unsetPerson(getID() != kPLAYER_ID);
  //tile_previous = NULL;
}

/* 
 * Description: The tile move initialization call. To be called after
 *              passability checks have passed and the thing can be moved to
 *              the next tile. Sets the new main pointer and moves the current
 *              to the old spot. This is reimplemented from MapThing since it
 *              uses "Person" instead of "Thing" in tile.
 * 
 * Inputs: std::vector<std::vector<Tile*>> tile_set - the next set of frames
 * Output: bool - if the tile start was successfully started
 */
bool MapPerson::tileMoveStart(std::vector<std::vector<Tile*>> tile_set)
{
  //if(next_tile != NULL)// && !next_tile->isPersonSet()) // TODO: Fix
  //{
    /* Increment step counter. */
    steps++;
    
    /* Set the new main tile */
    //tile_previous = tile_main;
    //tile_main = next_tile;
    // TODO: Fix for matrix map person
    //tile_main->setPerson(this, getID() != kPLAYER_ID);
    
    return true;
  //}
  //return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds person information from the XML file. This will be
 *              virtually called by children where the child will deal with
 *              additional sets needed and then pass call to this. Works off
 *              reading the XML file in a strict manner. Passes call to parent
 *              after it is complete.
 *
 * Inputs: XmlData data - the read XML data
 *         int file_index - the index in the xml data where this detail begins
 *         int section_index - the map section index of the person
 *         SDL_Renderer* renderer - the graphical rendering engine pointer
 *         std::string base_path - the base path for resources
 * Output: bool - status if successful
 */
bool MapPerson::addThingInformation(XmlData data, int file_index, 
                                    int section_index, SDL_Renderer* renderer, 
                                    std::string base_path)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  bool success = true;
  
  /* Parse the identifier for setting the person information */
  if(elements.size() >= 3)
  {
    /*--------------------- FRAMES -----------------*/
    if(elements[2] == "spritebot" || elements[2] == "spritetop" || 
                                     elements[2] == "sprites")
    {
      /* Create the surface identifier */
      SurfaceClassifier surface = GROUND;
      if(elements[0] == "ground")
        surface = GROUND;
      
      /* Create the direction identifier */
      Direction direction = Direction::DIRECTIONLESS;
      if(elements[1] == "north")
        direction = Direction::NORTH;
      else if(elements[1] == "south")
        direction = Direction::SOUTH;
      else if(elements[1] == "east")
        direction = Direction::EAST;
      else if(elements[1] == "west")
        direction = Direction::WEST;

      /* Only proceed if the direction was a valid direction */
      if(direction != Direction::DIRECTIONLESS)
      {
        Sprite* frames = NULL;
        Sprite* frames_upper = NULL;
        
        /* Determine if it's the main sprite or the secondary top sprite */
        if(elements[2] == "spritebot") /* Main Lower Sprite */
        {
          frames = getState(surface, direction);
          if(frames == NULL)
          {
            frames = new Sprite();
            if(!setState(surface, direction, frames))
            {
              delete frames;
              frames = NULL;
              success = false;
            }
          }
        }
        else if(elements[2] == "spritetop") /* Sprite Upper Top */
        {
          frames_upper = getStateSecondary(surface, direction);
          if(frames_upper == NULL)
          {
            frames_upper = new Sprite();
            if(!setStateSecondary(surface, direction, frames_upper))
            {
              delete frames_upper;
              frames_upper = NULL;
              success = false;
            }
          }
        }
        else if(elements[2] == "sprites") /* Combined sprite */
        {
          std::cout << elements[3] << std::endl;
          //Helpers::split();
        }

        /* Proceed with the add once valid frames are acquired. This will
           add both the upper and lower frames for the person, if possible */
        if(frames != NULL)
          success &= frames->addFileInformation(data, file_index + 3, 
                                                renderer, base_path);
        if(frames_upper != NULL)
          success &= frames_upper->addFileInformation(data, file_index + 3,
                                                      renderer, base_path);
      }
      else
      {
        success = false;
      }
    }
  }
  
  /* Proceed to parent */
  return (success && MapThing::addThingInformation(data, file_index, 
                                                   section_index, renderer, 
                                                   base_path));
}

/*
 * Description: This is the class descriptor. Primarily used for encapsulation
 *              to determine which class to cast it to for specific parameters.
 *
 * Inputs: none
 * Output: std::string - The string descriptor, it will be the same as the class
 *                       name. For example, "MapThing", "MapPerson", etc.
 */
std::string MapPerson::classDescriptor()
{
  return "MapPerson";
}

/* 
 * Description: Clears out all person states that were initialized into this
 *              class. This includes the appropriate procedure of cleaning
 *              up the parent class and deleting all the pointers.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::clear()
{
  for(uint8_t i = 0; i < kTOTAL_SURFACES; i++)
  {
    for(uint8_t j = 0; j < kTOTAL_DIRECTIONS; j++)
      unsetState(static_cast<SurfaceClassifier>(i), intToDir(j));
  }
  
  /* Clear direction and movement information */
  active_secondary = NULL;
  direction = Direction::NORTH;
  clearAllMovement();
  starting_tile = NULL;
  surface = GROUND;

  /* Clear the parent */
  MapThing::clear();
}

/* 
 * Description: Clears all active movement pointers that are in the current
 *              movement stack. This allows to halt all movement once the
 *              person has reached the next tile.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::clearAllMovement()
{
  movement_stack.clear();
}

/* 
 * Description: Returns the direction that the MapPerson is currently set to. 
 * 
 * Inputs: none
 * Output: Direction - the direction enumerator for this class
 */
Direction MapPerson::getDirection()
{
  return direction;
}

/* 
 * Description: This is a reimplemented call from MapThing, gets the actual
 *              move request which will be the last key pressed by the
 *              keyboard, since it's utilized as a stack.
 * 
 * Inputs: none
 * Output: Direction - the direction movement.
 */
Direction MapPerson::getMoveRequest()
{
  if(isMoveRequested())
    return movement_stack.back();
  return Direction::DIRECTIONLESS;
}

/*
 * Description: Returns the number of steps for the person, aka the number of 
 *              tiles walked on.
 *
 * Inputs: none
 * Output: uint32_t - the number of steps
 */
uint32_t MapPerson::getStepCount()
{
  return steps;
}

/*
 * Description: Returns the predicted move request.This simply returns the move
 *              request in map person but can be used to predict what the move
 *              sequence will be before updating the thing.
 *
 * Inputs: none
 * Output: Direction - the direction enumerator identifier
 */
Direction MapPerson::getPredictedMoveRequest()
{
  return getMoveRequest();
}

/*
 * Description: Returns the sprite state that is connected with the surface 
 *              definition and the direction. Returns NULL if unset or
 *              invalid.
 *
 * Inputs: SurfaceClassifier surface - the surface that the person is on
 *         Direction direction - the direction moving in
 * Sprite* - the state sprite pointer, that defines the image pointer
 */
Sprite* MapPerson::getState(SurfaceClassifier surface, Direction direction)
{
  int surface_index = static_cast<int>(surface);
  int dir_index = dirToInt(direction);
  
  if(surface_index >= 0 && dir_index >= 0 &&
     surface_index < static_cast<int>(states.size()) && 
     dir_index < static_cast<int>(states[surface_index].size()))
    return states[surface_index][dir_index];
    
  Sprite* null_sprite = NULL;
  return null_sprite;
}

/*
 * Description: Returns the secondary sprite state that is connected with the 
 *              surface definition and the direction. Returns NULL if unset or
 *              invalid.
 *
 * Inputs: SurfaceClassifier surface - the surface that the person is on
 *         Direction direction - the direction moving in
 * Sprite* - the secondary state sprite pointer, that defines the image pointer
 */
Sprite* MapPerson::getStateSecondary(SurfaceClassifier surface, 
                                     Direction direction)
{
  int surface_index = static_cast<int>(surface);
  int dir_index = dirToInt(direction);
  
  if(surface_index >= 0 && dir_index >= 0 && 
     surface_index < static_cast<int>(states_secondary.size()) &&
     dir_index < static_cast<int>(states_secondary[surface_index].size()))
    return states_secondary[surface_index][dir_index];
    
  Sprite* null_sprite = NULL;
  return null_sprite;
}

/* 
 * Description: Returns the surface classifier to what the map person is 
 *              standing on.
 * 
 * Inputs: none
 * Output: SurfaceClassifier - the surface enumerator for this class
 */
MapPerson::SurfaceClassifier MapPerson::getSurface()
{
  return surface;
}

/* 
 * Description: Reimplemented is move request call from map thing. This 
 *              utilizes the key press stack to get movement options.
 * 
 * Inputs: none
 * Output: bool - returns if a move is requested.
 */
bool MapPerson::isMoveRequested()
{
  return !movement_stack.empty();
}

/*
 * Description: The key press event in the window through the SDL system.
 *
 * Inputs: SDL_KeyboardEvent event - the the event triggered on the SDL engine
 * Output: none
 */
void MapPerson::keyDownEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_DOWN)
    addDirection(Direction::SOUTH);
  else if(event.keysym.sym == SDLK_UP)
    addDirection(Direction::NORTH);
  else if(event.keysym.sym == SDLK_RIGHT)
    addDirection(Direction::EAST);
  else if(event.keysym.sym == SDLK_LEFT)
    addDirection(Direction::WEST);
}

/*
 * Description: Flushes the key presses. Used to stop the person movement
 *              without key release events.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::keyFlush()
{
  movement_stack.clear();
}

/*
 * Description: Handles the key release event through the SDL system.
 *
 * Inputs: SDL_KeyboardEvent event - the event triggered on the SDL engine
 * Output: none
 */
void MapPerson::keyUpEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_DOWN)
    removeDirection(Direction::SOUTH);
  else if(event.keysym.sym == SDLK_UP)
    removeDirection(Direction::NORTH);
  else if(event.keysym.sym == SDLK_RIGHT)
    removeDirection(Direction::EAST);
  else if(event.keysym.sym == SDLK_LEFT)
    removeDirection(Direction::WEST);
}

/* 
 * Description: The render secondary function for [erspm. This takes the 
 *              active secondary state and renders it based on location and 
 *              offset (from paint engine) and if it is set within the person 
 *              class.
 * 
 * Inputs: SDL_Renderer* renderer - the graphical rendering engine pointer
 *         int offset_x - the paint offset in the x direction
 *         int offset_y - the paint offset in the y direction
 * Output: bool - if the render succeeded
 */
bool MapPerson::renderSecondary(SDL_Renderer* renderer, 
                                int offset_x, int offset_y)
{
  if(isVisible())// && frames != NULL && tile_main != NULL) // TODO: Fix
  {
    if(active_secondary != NULL)
    {
      int render_x = x - offset_x;
      int render_y = y - offset_y - width;
      
      active_secondary->render(renderer, render_x, render_y, width, height);
      return true;
    }
  }
  return false;
}

/*
 * Description: Resets the position of the person back to the initial starting
 *              point. This is the position that was set when the last 
 *              setStartingTile() was called.
 *
 * Inputs: none
 * Output: bool - status if successful
 */
bool MapPerson::resetPosition()
{
  if(starting_tile != NULL)
    return setStartingTile(starting_section, starting_tile, true);
  return false;
}

/* 
 * Description: Sets the connected tile information for the map person. This is
 *              the initial starting point and where the person is initially
 *              placed. If this is unset, the person will not move or paint.
 *              This replaces the generic mapthing call to allow for map 
 *              person in tile to be modified.
 *
 * Inputs: uint16_t section_id - the map id that the tile is from
 *         Tile* new_tile - the tile to set the starting pointer to
 *         bool no_events - don't execute any events when set
 * Output: bool - status if the change was able to occur
 */
// TODO: Remove
bool MapPerson::setStartingTile(uint16_t section_id, Tile* new_tile, 
                                                     bool no_events)
{
  if(new_tile != NULL)// && !new_tile->isPersonSet()) // TODO: Fix
  {
    /* Unsets the old tile information */
    unsetStartingTile(no_events);
  
    /* Set the new tile */
    //tile_main = new_tile; // TODO: Fix
    //this->x = tile_main->getPixelX();
    this->x_raw = this->x * kRAW_MULTIPLIER;
    //this->y = tile_main->getPixelY();
    this->y_raw = this->y * kRAW_MULTIPLIER;
    //tile_main->setPerson(this, no_events);
    tile_section = section_id;
    
    /* Store the starting tile */
    starting_section = section_id;
    //starting_tile = tile_main; // TODO: Fix
    
    return true;
  }

  return false;
}

/* 
 * Description: Sets a state within the class, based on the double set of 
 *              enumerators, for surface and direction. This will automatically
 *              unset a state that is currently in its place, if one does
 *              exist. 
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         Direction direction - the direction for the state
 *         Sprite* frames - the frame data to set the state at
 * Output: bool - if the call was successful
 */
bool MapPerson::setState(SurfaceClassifier surface, 
                         Direction direction, Sprite* frames)
{
  /* Only proceed with insertion if the sprite and state data is valid */
  if(frames != NULL)
  {
    unsetState(surface, direction);
    states[static_cast<int>(surface)][dirToInt(direction)] = frames;
    
    /* If the updated state is the active one, automatically set the printable
     * sprite */
    //if(this->surface == surface && this->direction == direction)
    //  MapThing::setFrames(frames, false); // TODO: Repair
    
    return true;
  }

  return false;
}

/* 
 * Description: Sets a secondary state within the class, based on the double 
 *              set of enumerators, for surface and direction. This will
 *              automatically unset a state that is currently in its place, if 
 *              one does exist. 
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         Direction direction - the direction for the state
 *         Sprite* frames - the frame data to set the secondary state at
 * Output: bool - if the call was successful
 */
bool MapPerson::setStateSecondary(SurfaceClassifier surface, 
                                  Direction direction, Sprite* frames)
{
  /* Only proceed with insertion if the sprite and state data is valid */
  if(frames != NULL)
  {
    unsetStateSecondary(surface, direction);
    states_secondary[static_cast<int>(surface)][dirToInt(direction)] = frames;
   
    /* If the updated state is the active one, automatically set the printable
     * sprite */
    if(this->surface == surface && this->direction == direction)
      active_secondary = frames;
 
    return true;
  }

  return false;
}

/* 
 * Description: Sets a new surface for the person on the map to sit on. It 
 *              will update the parent frame so the new classifier is printed.
 * 
 * Inputs: SurfaceClassifier surface - the new surface to set
 * Output: none
 */
void MapPerson::setSurface(SurfaceClassifier surface)
{
  this->surface = surface;

  Sprite* current = getState(surface, direction);
  //if(current != NULL)
  //  MapThing::setFrames(current, false); // TODO: Repair
}

/*
 * Description: Updates the state of the person. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the time elapsed between updates
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 * Output: none 
 */
void MapPerson::update(int cycle_time, std::vector<std::vector<Tile*>> tile_set)
{
  // TODO: Fix
  bool can_move = false;//isMoveAllowed(next_tile) && !getMovementPaused();
  bool reset = false;

  /* Once a tile end has reached, cycle the movement direction */
  if(isAlmostOnTile(cycle_time))
  {
    tileMoveFinish();
    
    /* If paused and there is a target, change direction to the target */
    if(getMovementPaused())
    {
      if(getTarget())
      { // TODO: Fix
        int delta_x = 0;//getTile()->getX() - getTarget()->getTile()->getX();
        int delta_y = 0;//getTile()->getY() - getTarget()->getTile()->getY();

        if(delta_x < 0)
          setDirection(Direction::EAST, can_move);
        else if(delta_x > 0)
          setDirection(Direction::WEST, can_move);
        else if(delta_y < 0)
          setDirection(Direction::SOUTH, can_move);
        else if(delta_y > 0)
          setDirection(Direction::NORTH, can_move);
      }
      else
      {
        setDirection(Direction::DIRECTIONLESS);
      }

      reset = true;
    }
    /* Update direction based on move request and if it should move */
    else if(isMoveRequested())
    {
      /* Set the new direction and if the direction is changed, or it's
       * not allowed to move, recenter the thing */
      reset = !can_move;
      if(setDirection(getMoveRequest(), can_move) || !can_move)
      {
        //x = tile_main->getPixelX(); // TODO: Fix
        //y = tile_main->getPixelY();
        reset = true;
      }
      
      /* If it can move, initiate tile shifting */
      //if(can_move) // TODO: Fix
        //tileMoveStart(next_tile);
    }
    /* If there is no move request, stop movement */
    else
    {
      // TODO: Fix
      //if(tile_main != NULL)
      //{
      //  x = tile_main->getPixelX();
      //  y = tile_main->getPixelY();
      //}

      setDirection(Direction::DIRECTIONLESS);
      reset = true;
    }
  }

  /* Proceed to move the thing */
  moveThing(cycle_time);

  /* Only animate if the direction exists */
  animate(cycle_time, reset, getMovement() != Direction::DIRECTIONLESS);
}

/* 
 * Description: Unsets the state in this class based on the two classifiers.
 *              This includes the appropriate delete functionality for the 
 *              stored pointers. Also, unsets the parent classifier, if 
 *              the current state data is being used.
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         Direction direction - the direction for the state
 * Output: none
 */
void MapPerson::unsetState(SurfaceClassifier surface, 
                           Direction direction)
{
  Sprite* state = getState(surface, direction);
  if(state != NULL)
  {
    delete state;
    states[static_cast<int>(surface)][dirToInt(direction)] = NULL;
    
    /* Clear out the parent call if the direction or surface lines up */
    if(this->surface == surface && this->direction == direction)
      MapThing::unsetFrames(false);
  }
}

/* 
 * Description: Unsets the secondary state in this class based on the two
 *              classifiers. This includes the appropriate delete functionality
 *              for the stored pointers.
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         Direction direction - the direction for the state
 * Output: none
 */
void MapPerson::unsetStateSecondary(SurfaceClassifier surface, 
                                    Direction direction)
{
  Sprite* state = getStateSecondary(surface, direction);
  if(state != NULL)
  {
    delete state;
    states[static_cast<int>(surface)][dirToInt(direction)] = NULL;

    /* Clear out the parent call if the direction or surface lines up */
    if(this->surface == surface && this->direction == direction)
      active_secondary = NULL;
  }
}

/*
 * Description: Unsets the starting tile location that is stored within the
 *              thing.
 *
 * Inputs: bool no_events - fire no events when unsetting
 * Output: none
 */
void MapPerson::unsetStartingTile(bool no_events)
{ 
  /* Stop movement */
  setDirection(Direction::DIRECTIONLESS);
  
  /* Unset the previous tile */
  //if(tile_previous != NULL) // TODO: Fix
  //  tile_previous->unsetPerson(no_events);
  //tile_previous = NULL;
  
  /* Unset the main tile */
  //if(tile_main != NULL) // TODO: Fix
  //  tile_main->unsetPerson(no_events);
  //tile_main = NULL;
    
  /* Resets the coordinates */
  this->x = 0;
  this->y = 0;
}
