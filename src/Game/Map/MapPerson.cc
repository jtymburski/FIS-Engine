/*******************************************************************************
 * Class Name: MapPerson
 * Date Created: Dec 2 2012
 * Inheritance: MapThing
 * Description: The MapPerson class. An addition on top of MapThing that
 *              expands the Thing into possible states to allow for walking
 *              in multiple directions and on multiple surfaces. At present,
 *              it allows for the 4 directions (N,S,E,W) all on one surface
 *              (Ground). Future expansion is available for other surfaces
 *              such as water, flying, etc. The animation speed of the sprites
 *              is based on 2 frames of moving (3 total). During the
 *              cleanMatrix() phase, the animation speed is modified to
 *              match the number of frames. For example, if left has 4 frames,
 *              the animation speed will be halved.
 *
 * TODO: Glitch - when running up and then reverse part way through tile,
 *                the guy floats for a tiny period of time [2014-07-25]
 ******************************************************************************/
#include "Game/Map/MapPerson.h"

/* Constant Implementation - see header file for descriptions */
const int MapPerson::kDELAY_MAX = 600000;
const int MapPerson::kDELAY_MIN = 60000;
const int8_t MapPerson::kDIR_EAST = 1;
const int8_t MapPerson::kDIR_NORTH = 0;
const int8_t MapPerson::kDIR_SOUTH = 2;
const int8_t MapPerson::kDIR_UNKNOWN = -1;
const int8_t MapPerson::kDIR_WEST = 3;
const uint8_t MapPerson::kTOTAL_DIRECTIONS = 4;
const uint8_t MapPerson::kTOTAL_SURFACES = 1;

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
  ignore_passability = false;
  interaction_disabled = false;
  move_freeze = false;
  running = false;
  sound_delay = -1;
  starting_dir = Direction::NORTH;
  starting_section = 0;
  steps = 0;

  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = Direction::NORTH;
  initializeStates();
}

/*
 * Description: Constructor for this class. Sets up a person with the
 *              appropriate height and width (for the tile) and other info
 *              that may be required by the person.
 *
 * Inputs: int id - a unique numerical identifier, for the person
 *         std::string name - a string name for the person
 *         std::string description - a description of the person
 */
MapPerson::MapPerson(int id, std::string name, std::string description)
    : MapThing(id, name, description)
{
  running = false;
  starting_section = 0;
  steps = 0;

  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = Direction::NORTH;
  initializeStates();
}

/*
 * Description: Destructor function
 */
MapPerson::~MapPerson()
{
  unsetMatrix();
  clear();
  deleteStates();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes all created matrix states. Only called on class
 *              destruction.
 *
 * Inputs: none
 * Output: none
 */
void MapPerson::deleteStates()
{
  for(uint16_t i = 0; i < states.size(); i++)
  {
    for(uint16_t j = 0; j < states[i].size(); j++)
    {
      delete states[i][j];
      states[i][j] = NULL;
    }
  }

  states.clear();
}

/*
 * Description: Returns the top most sound trigger that should be executed.
 *              Only called if the player, and whenever a move start trigger
 *              is initiated.
 *
 * Inputs: none
 * Output: int32_t - the sound ID found. If less than 0, none found
 */
int32_t MapPerson::getSoundTrigger()
{
  int sound_id = -1;

  /* Blank out tile set */
  std::vector<std::vector<Tile*>> set = getTileRender(0);

  /* MapInteractiveObject - top level */
  for(uint32_t i = 0; sound_id < 0 && i < set.size(); i++)
  {
    for(uint32_t j = 0; sound_id < 0 && j < set[i].size(); j++)
    {
      if(set[i][j] != nullptr && set[i][j]->getIO(0) != nullptr)
        sound_id = set[i][j]->getIO(0)->getSoundID();
    }
  }

  /* MapThing - second level */
  for(uint32_t i = 0; sound_id < 0 && i < set.size(); i++)
  {
    for(uint32_t j = 0; sound_id < 0 && j < set[i].size(); j++)
    {
      if(set[i][j] != nullptr && set[i][j]->getThing(0) != nullptr)
        sound_id = set[i][j]->getThing(0)->getSoundID();
    }
  }

  /* Tile Sprites - third level */
  for(uint32_t i = 0; sound_id < 0 && i < set.size(); i++)
  {
    for(uint32_t j = 0; sound_id < 0 && j < set[i].size(); j++)
    {
      if(set[i][j] != nullptr)
        sound_id = set[i][j]->getSoundID();
    }
  }

  return sound_id;
}

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
  deleteStates();

  for(uint8_t i = 0; i < kTOTAL_SURFACES; i++)
  {
    std::vector<SpriteMatrix*> row;

    for(uint8_t j = 0; j < kTOTAL_DIRECTIONS; j++)
      row.push_back(new SpriteMatrix());
    states.push_back(row);
  }

  /* Set the initial frames in the thing */
  setMatrix(getState(surface, direction));
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
 * Description: Can the tile be set with the passed frame. Fails if there is
 *              already a person set on said tile
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the sprite frames pointer to set in the tile
 *         bool avoid_player - true that canSet will return false if 0 render
 *                             player is on location. default false
 * Output: bool - true if the set was successful
 */
bool MapPerson::canSetTile(Tile* tile, TileSprite* frames, bool avoid_player)
{
  if(tile != nullptr)
  {
    /* Check if player is there */
    bool player = false;
    if(avoid_player && frames->getRenderDepth() == 0)
    {
      MapPerson* person = tile->getPerson(frames->getRenderDepth());
      if(person != nullptr && person->getID() == kPLAYER_ID)
        player = true;
    }

    /* Final return */
    return (!tile->isPersonSet(frames->getRenderDepth()) && !player);
  }
  return false;
}

/*
 * Description: Checks if there is data to save for the particular person. This
 *              is virtualized for all children
 *
 * Inputs: none
 * Output: bool - true if save call will result in text
 */
bool MapPerson::isDataToSave()
{
  /* Check parent first (includes changed variable) */
  if(MapThing::isDataToSave())
    return true;

  /* Check direction */
  if(direction != starting_dir)
    return true;

  /* Check move freeze */
  if(move_freeze)
    return true;

  return false;
}

/*
 * Description: Checks if a move is allowed from the current person main
 *              tile to the next tile that it is trying to move to. This
 *              handles the individual calculations for a single tile; used
 *              by the isMoveAllowed() function.
 *
 * Inputs: Tile* previous - the tile moving from
 *         Tile* next - the next tile moving to
 *         uint8_t render_depth - the rendering depth, in the stack
 *         Direction move_request - the direction moving
 * Output: bool - returns if the move is allowed.
 */
bool MapPerson::isTileMoveAllowed(Tile* previous, Tile* next,
                                  uint8_t render_depth, Direction move_request)
{
  bool move_allowed = true;

  /* If the next tile is NULL, move isn't allowed */
  if(next == NULL || move_freeze)
    move_allowed = false;

  /* Check if the thing can move there */
  if(move_allowed)
  {
    if(render_depth == 0)
    {
      MapInteractiveObject* prev_io = previous->getIO(render_depth);
      MapThing* prev_thing = previous->getThing(render_depth);
      MapInteractiveObject* next_io = next->getIO(render_depth);
      MapThing* next_thing = next->getThing(render_depth);

      if(!previous->getPassabilityExiting(move_request) ||
         !next->getPassabilityEntering(move_request) ||
         (prev_thing != NULL &&
          !prev_thing->getPassabilityExiting(previous, move_request)) ||
         (prev_io != NULL &&
          !prev_io->getPassabilityExiting(previous, move_request)) ||
         (next_thing != NULL &&
          !next_thing->getPassabilityEntering(next, move_request)) ||
         (next_io != NULL &&
          !next_io->getPassabilityEntering(next, move_request)) ||
         (next->isPersonSet(render_depth) &&
          next->getPerson(render_depth) != this))
      {
        move_allowed = false;
      }
    }
    else if(next->getStatus() == Tile::OFF ||
            (next->isPersonSet(render_depth) &&
             next->getPerson(render_depth) != this))
    {
      move_allowed = false;
    }
  }

  return (move_allowed || ignore_passability);
}

/*
 * Description: Calculates the move amount based on the cycle time and the
 *              speed for how many pixels should be shifted. The calculation
 *              is based on 16ms for 2 pixel at speed 8. This is virtual from
 *              thing and adds on the check for running.
 *
 * Inputs: int cycle_time - the time since the last update call
 * Output: float - the move amount in 0.0 to 1.0 of a pixel width
 */
float MapPerson::moveAmount(uint16_t cycle_time)
{
  float move_amount = 0.0;

  if(running)
    move_amount = (cycle_time * getSpeed() * 1.75) / kMOVE_FACTOR;
  else
    move_amount = (cycle_time * getSpeed()) / kMOVE_FACTOR;

  /* Check to make sure it maxes out at one tile */
  if(move_amount > 1.0)
    move_amount = 1.0;

  return move_amount;
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
 * Description: Saves the data for the person. This does not include the person
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         const bool &save_event - true to save the base event set (thing)
 * Output: none
 */
bool MapPerson::saveData(FileHandler* fh, const bool& save_event)
{
  bool success = true;

  /* Parent property saves */
  success &= MapThing::saveData(fh, save_event);

  /* Property change saves */
  if(changed)
  {
    /* Speed */
    fh->writeXmlData("speed", getSpeed());
  }

  /* Direction */
  if(direction != starting_dir)
    fh->writeXmlData("dirfacing", Helpers::directionToString(direction));

  /* Move frozen */
  if(move_freeze)
    fh->writeXmlData("movefreeze", move_freeze);

  return success;
}

/*
 * Description: Sets a new direction for the person on the map. It will update
 *              the parent frame so a new classifier is printed.
 *
 * Inputs: Direction direction - the new direction to set
 *         bool set_movement - if the movement should be set as well
 *         bool forced - set the state direction no matter what
 * Output: bool - indicates if the directional movement changed
 */
bool MapPerson::setDirection(Direction direction, bool set_movement,
                             bool forced)
{
  (void)forced;

  bool changed = (this->direction != direction);
  bool movement_changed = false;

  /* If moving, set the direction in map thing */
  if(set_movement)
    movement_changed = MapThing::setDirection(direction);
  else
    MapThing::setDirection(Direction::DIRECTIONLESS);

  /* If it's a movement direction, rotate the fellow */
  SpriteMatrix* state = getState(surface, direction);
  if(state != NULL)
  {
    if(changed)
      setMatrix(state);

    /* Finally set the in class direction */
    this->direction = direction;
  }

  return movement_changed;
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects. If it fails, it resets the pointers.
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the sprite frames pointer to set in the tile
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
bool MapPerson::setTile(Tile* tile, TileSprite* frames, bool no_events)
{
  uint8_t render_depth = frames->getRenderDepth();

  /* Attempt and set the tile */
  if(tile != NULL)
    return tile->setPerson(this, render_depth, no_events);

  return false;
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects with regards to finishing a tile move.
 *
 * Inputs: Tile* old_tile - the tile the object was on previously
 *         Tile* new_tile - the tile the object is moving to
 *         uint8_t render_depth - the depth the frame is rendered on this tile
 *         bool reverse_last - if the last start should be reversed
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
void MapPerson::setTileFinish(Tile* old_tile, Tile* new_tile,
                              uint8_t render_depth, bool reverse_last,
                              bool no_events)
{
  if(reverse_last)
  {
    old_tile->personMoveFinish(render_depth, no_events, reverse_last);
    new_tile->unsetPerson(render_depth, no_events);

    /* Special events if person and thing is set on tile at render level 0 */
    if(getID() == kPLAYER_ID && render_depth == 0 && new_tile->isIOSet(0) &&
       old_tile->getIO(0) != new_tile->getIO(0))
      new_tile->getIO(0)->triggerWalkOff(this);
  }
  else
  {
    old_tile->personMoveFinish(render_depth, no_events);

    /* Special events if person and thing is set on tile at render level 0 */
    if(getID() == kPLAYER_ID && render_depth == 0 && old_tile->isIOSet(0) &&
       old_tile->getIO(0) != new_tile->getIO(0))
      old_tile->getIO(0)->triggerWalkOff(this);
  }
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects with regards to beginning a tile move. If it
 *              fails, it resets the pointers back to the original tile.
 *
 * Inputs: Tile* old_tile - the tile the object was on previously
 *         Tile* new_tile - the tile the object is moving to
 *         uint8_t render_depth - the depth the frame is rendered on this tile
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
bool MapPerson::setTileStart(Tile* old_tile, Tile* new_tile,
                             uint8_t render_depth, bool no_events)
{
  /* Attempt and set the tile */
  if(new_tile != NULL && old_tile != NULL)
  {
    bool is_same = false;
    if(render_depth == 0)
      is_same = (new_tile->getPerson(0) == this);
    if(new_tile->setPerson(this, render_depth, no_events || is_same))
    {
      old_tile->personMoveStart(render_depth);

      /* Special events if person and thing is set on tile at render level 0 */
      if(getID() == kPLAYER_ID && render_depth == 0 && new_tile->isIOSet(0) &&
         old_tile->getIO(0) != new_tile->getIO(0))
        new_tile->getIO(0)->triggerWalkOn(this);

      return true;
    }
  }

  return false;
}

/*
 * Description: The tile move initialization call. To be called after
 *              passability checks have passed and the thing can be moved to
 *              the next tile. Sets the new main pointer and moves the current
 *              to the old spot. This is reimplemented from MapThing since it
 *              uses "Person" instead of "Thing" in tile.
 *
 * Inputs: std::vector<std::vector<Tile*>> tile_set - the next set of frames
 *         bool no_events - should events trigger on move?
 * Output: bool - if the tile start was successfully started
 */
bool MapPerson::tileMoveStart(std::vector<std::vector<Tile*>> tile_set,
                              bool no_events)
{
  if(MapThing::tileMoveStart(tile_set, no_events))
  {
    /* Sound trigger - only for the player person */
    if(getID() == kPLAYER_ID && event_handler != nullptr)
    {
      int32_t sound_id = getSoundTrigger();
      if(sound_id >= 0)
        event_handler->triggerSound(sound_id, SoundChannels::TILES);
    }

    /* Step increment */
    steps++;

    return true;
  }
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
void MapPerson::unsetTile(uint32_t x, uint32_t y, bool no_events)
{
  uint8_t render_depth = sprite_set->at(x, y)->getRenderDepth();

  /* Remove from main tile, if applicable */
  tile_main[x][y]->unsetPerson(render_depth, no_events);
  if(getID() == kPLAYER_ID && render_depth == 0 && tile_main[x][y]->isIOSet(0))
    tile_main[x][y]->getIO(0)->triggerWalkOff(this);

  /* Remove from previous tile, if applicable */
  if(tile_prev.size() > 0)
  {
    tile_prev[x][y]->unsetPerson(render_depth, no_events);
    if(getID() == kPLAYER_ID && render_depth == 0 &&
       tile_prev[x][y]->isIOSet(0))
      tile_prev[x][y]->getIO(0)->triggerWalkOff(this);
  }
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
 *         bool from_save - true if the load is from a save file
 * Output: bool - status if successful
 */
bool MapPerson::addThingInformation(XmlData data, int file_index,
                                    int section_index, SDL_Renderer* renderer,
                                    std::string base_path, bool from_save)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  bool success = true;

  /* Parse the identifier for setting the person information */
  /*--------------------- FRAMES -----------------*/
  if(elements.size() >= 4 && elements[2] == "sprites")
  {
    /* Create the surface identifier */
    SurfaceClassifier surface = GROUND;
    if(elements[0] == "ground")
      surface = GROUND;

    /* Create the direction identifier */
    Direction direction = Helpers::directionFromString(elements[1]);

    /* Only proceed if the direction was a valid direction */
    SpriteMatrix* matrix = getState(surface, direction);
    if(matrix != NULL)
      success &=
          matrix->addFileInformation(data, file_index + 3, renderer, base_path);
    else
      success = false;
  }
  /*----------------- DIRECTION FACING -----------------*/
  else if(elements.size() == 1 && elements.front() == "dirfacing")
  {
    Direction dir = Helpers::directionFromString(data.getDataString(&success));
    if(success)
      setDirection(dir, false, from_save);
  }
  /*----------------- MOVE FREEZE -----------------*/
  else if(elements.size() == 1 && elements.front() == "movefreeze")
  {
    bool freeze = data.getDataBool(&success);
    if(success)
      move_freeze = freeze;
  }
  /*----------------- RENDER MATRIX -----------------*/
  else if(elements.size() == 1 && elements.front() == "rendermatrix")
  {
    for(uint8_t i = 0; i < states.size(); i++)
    {
      for(uint8_t j = 0; j < states[i].size(); j++)
      {
        states[i][j]->setRenderMatrix(data.getDataString(&success));
      }
    }
  }
  /*----------------- STARTING DIRECTION -----------------*/
  else if(data.getElement(file_index) == "startingdir")
  {
    Direction dir = Helpers::directionFromString(data.getDataString());
    success &= setStartingDirection(dir);
  }
  /* Failed conditions with sprites at root - invalid and keep from thing */
  else if(data.getElement(file_index) == "sprites")
  {
    success = false;
  }
  /* -- All Other Conditions to Thing -- */
  else
  {
    /* Proceed to parent */
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
 *
 * Inputs: none
 * Output: ThingBase - the thing description enum defining the type to cast
 */
ThingBase MapPerson::classDescriptor()
{
  return ThingBase::PERSON;
}

/*
 * Description: Takes the frame matrix, as it's been set up and removes any
 *              rows or columns at the end that have no valid frames set. A
 *              frame is classified as valid if it's not NULL and has renderable
 *              frames stored within it.
 *
 * Inputs: bool first_call - is this the first call? default true
 * Output: bool - true if clean validated frame data
 */
bool MapPerson::cleanMatrix(bool first_call)
{
  bool equal_size = true;
  uint16_t height = 0;
  uint16_t width = 0;

  for(uint8_t i = 0; i < states.size(); i++)
  {
    for(uint8_t j = 0; j < states[i].size(); j++)
    {
      states[i][j]->cleanMatrix();
      if(first_call)
        states[i][j]->tuneAnimationSpeed();

      /* Do some additional parsing to confirm data */
      if(i == 0 && j == 0)
      {
        height = states[i][j]->height();
        width = states[i][j]->width();
      }
      else if(states[i][j]->height() != height ||
              states[i][j]->width() != width)
      {
        equal_size = false;
      }
    }
  }

  return equal_size;
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
  unsetStates();

  /* Clear direction and movement information */
  direction = Direction::NORTH;
  clearAllMovement();
  running = false;
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
 * Description: Disables other NPCs from using this person data for tracking
 *              and such.
 *
 * Inputs: bool disable - disable the other NPCs from tracking, etc
 * Output: none
 */
void MapPerson::disableInteraction(bool disable)
{
  interaction_disabled = disable;
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
 * Description: Returns the first valid frame pointer. NULL if none found. This
 *              overrides the default map thing functionality.
 *
 * Inputs: none
 * Output: TileSprite* - the first valid frame reference
 */
TileSprite* MapPerson::getFrameValid()
{
  for(uint32_t i = 0; i < states.size(); i++)
  {
    for(uint32_t j = 0; j < states[i].size(); j++)
    {
      TileSprite* found_sprite = nullptr;
      if(states[i][j] != nullptr)
        found_sprite = states[i][j]->getSpriteValid();
      if(found_sprite != nullptr)
        return found_sprite;
    }
  }
  return nullptr;
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
 * Description: Returns the player reference. NULL if not set. Virtualized for
 *              MapNPC
 *
 * Inputs: none
 * Output: MapPerson* - the person reference pointer
 */
MapPerson* MapPerson::getPlayer()
{
  return nullptr;
}

/*
 * Description: Returns the starting facing direction of the person
 *
 * Inputs: none
 * Output: Direction - the facing direction enum
 */
Direction MapPerson::getStartingDirection() const
{
  return starting_dir;
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
 *         bool include_base - include base in possible set to return
 * Output: SpriteMatrix* - the state matrix pointer, that defines the data
 */
SpriteMatrix* MapPerson::getState(SurfaceClassifier surface,
                                  Direction direction, bool include_base)
{
  int surface_index = static_cast<int>(surface);
  int dir_index = dirToInt(direction);

  /* Check if it's a base and the frames from it should be used instead */
  if(include_base && base != nullptr &&
     (base_category == ThingBase::PERSON || base_category == ThingBase::NPC))
  {
    MapPerson* ref_base = static_cast<MapPerson*>(base);

    if(surface_index >= 0 && dir_index >= 0 &&
       surface_index < static_cast<int>(ref_base->states.size()) &&
       dir_index < static_cast<int>(ref_base->states[surface_index].size()))
      return ref_base->states[surface_index][dir_index];
  }
  else
  {
    if(surface_index >= 0 && dir_index >= 0 &&
       surface_index < static_cast<int>(states.size()) &&
       dir_index < static_cast<int>(states[surface_index].size()))
      return states[surface_index][dir_index];
  }

  return nullptr;
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
 * Description: Returns if the NPC will force interaction - always false when
 *              called on parent MapPerson class.
 *
 * Inputs: bool false_if_active - true to also include if there has been a
 *                                recent forced interaction. If so, this will
 *                                return false regardless. Default true
 * Output: bool - true if the interaction is set to be forced
 */
bool MapPerson::isForcedInteraction(bool false_if_active)
{
  (void)false_if_active;
  return false;
}

/*
 * Description: Returns if interaction by other NPCs has been disabled
 *
 * Inputs: none
 * Output: bool - interaction disabled status
 */
bool MapPerson::isInteractionDisabled()
{
  return interaction_disabled;
}

/*
 * Description: Determines if the person's movement is frozen in place. This
 *              restricts if the person will ever leave the given tile.
 *
 * Inputs: none
 * Output: bool - true if the movement is frozen
 */
bool MapPerson::isMoveFrozen()
{
  return move_freeze;
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
 * Description: Returns if the person is running (moving at twice set speed) or
 *              not.
 *
 * Inputs: none
 * Output: bool - is the person running?
 */
bool MapPerson::isRunning()
{
  return running;
}

/*
 * Description: The key press event in the window through the SDL system.
 *
 * Inputs: SDL_KeyboardEvent event - the the event triggered on the SDL engine
 * Output: none
 */
void MapPerson::keyDownEvent(KeyHandler& key_handler)
{
  if(getTarget() == nullptr)
  {
    if(key_handler.isDepressed(GameKey::MOVE_DOWN))
    {
      addDirection(Direction::SOUTH);
      // key_handler.setHeld(GameKey::MOVE_DOWN);
    }
    if(key_handler.isDepressed(GameKey::MOVE_UP))
    {
      addDirection(Direction::NORTH);
      // key_handler.setHeld(GameKey::MOVE_UP);
    }
    if(key_handler.isDepressed(GameKey::MOVE_RIGHT))
    {
      addDirection(Direction::EAST);
      // key_handler.setHeld(GameKey::MOVE_RIGHT);
    }
    if(key_handler.isDepressed(GameKey::MOVE_LEFT))
    {
      addDirection(Direction::WEST);
      // key_handler.setHeld(GameKey::MOVE_LEFT);
    }
  }
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
void MapPerson::keyUpEvent(KeyHandler& key_handler)
{
  if(!key_handler.isDepressed(GameKey::MOVE_DOWN))
    removeDirection(Direction::SOUTH);
  if(!key_handler.isDepressed(GameKey::MOVE_UP))
    removeDirection(Direction::NORTH);
  if(!key_handler.isDepressed(GameKey::MOVE_RIGHT))
    removeDirection(Direction::EAST);
  if(!key_handler.isDepressed(GameKey::MOVE_LEFT))
    removeDirection(Direction::WEST);

  if(!key_handler.isDepressed(GameKey::MOVE_DOWN) &&
     !key_handler.isDepressed(GameKey::MOVE_UP) &&
     !key_handler.isDepressed(GameKey::MOVE_RIGHT) &&
     !key_handler.isDepressed(GameKey::MOVE_LEFT))
  {
    keyFlush();
  }
}

/*
 * Description: Resets the step count back to 0. Typically used after a
 *              translation call up to game level parent of Player class.
 *
 * Inputs: none
 * Output: none
 */
void MapPerson::resetStepCount()
{
  steps = 0;
}

/*
 * Description: Resets the position of the person back to the initial starting
 *              point. This is the position that was set when the last
 *              setStartingTile() was called.
 *
 * Inputs: bool no_set - true to not set tiles and just reset. false default
 * Output: bool - status if successful
 */
bool MapPerson::resetToStart(bool no_set)
{
  if(MapThing::resetToStart(no_set))
  {
    MapPerson::setDirection(starting_dir, false);
    return true;
  }
  return false;
}

/*
 * Description: Saves the person data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 * Output: bool - true if the save was successful
 */
bool MapPerson::save(FileHandler* fh)
{
  if(fh != nullptr)
  {
    bool success = true;

    /* Only proceed if there is data to save */
    if(isDataToSave())
    {
      fh->writeXmlElement("mapperson", "id", getID());
      success &= saveData(fh);
      fh->writeXmlElementEnd();
    }

    return success;
  }
  return false;
}

/*
 * Description: Sets the base thing class. If set, the primary data will be set
 *              from this with only location and movement handled by this class.
 *
 * Inputs: MapThing* base - the reference base class
 * Output: bool - true if the base was set
 */
bool MapPerson::setBase(MapThing* base)
{
  bool success = false;

  if(classDescriptor() == ThingBase::PERSON)
  {
    if(base != NULL && base->classDescriptor() == ThingBase::PERSON)
    {
      this->base = base;
      base_category = ThingBase::PERSON;
      setMatrix(getState(surface, direction));
      setSpeed(base->getSpeed());
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
 * Description: Sets the color mode of all sprites within the matrix of the
 *              thing. Virtualized for the purpose of children.
 *
 * Inputs: ColorMode mode - the mode of how to color the thing
 * Output: none
 */
void MapPerson::setColorMode(ColorMode mode)
{
  for(uint32_t i = 0; i < states.size(); i++)
    for(uint32_t j = 0; j < states[i].size(); j++)
      if(states[i][j] != nullptr)
        states[i][j]->setColorMode(mode);
}

/*
 * Description: Sets if the person movement is frozen at the current (main)
 *              tile. If it is mid movement, it will finish the move sequence
 *              to the next tile and then freeze.
 *
 * Inputs: bool freeze - should movement be frozen?
 * Output: none
 */
void MapPerson::setMoveFreeze(bool freeze)
{
  move_freeze = freeze;
}

/*
 * Description: Sets the person to either running or not. Running is defined
 *              as twice the normal speed.
 *
 * Inputs: bool running - true if the person is running.
 * Output: none
 */
void MapPerson::setRunning(bool running)
{
  this->running = running;
}

/*
 * Description: Sets the person starting facing direction. This is used on
 *              initial creation and certain inactive cases.
 *
 * Inputs: Direction starting - the starting direction facing of the person
 * Output: bool - returns if the direction was set
 */
bool MapPerson::setStartingDirection(Direction starting)
{
  if(starting != Direction::DIRECTIONLESS)
  {
    if(starting != starting_dir)
    {
      starting_dir = starting;
      setDirection(starting_dir, false);
    }
    return true;
  }
  return false;
}

/*
 * Description: Sets an individial tile sprite in a specific surface and
 *              direction with a given x and y coordinate.
 *
 * Inputs: TileSprite* frame - the rendering tile sprite to add
 *         SurfaceClassifier surface - the surface the person is on
 *         Direction direction - the direction the person is facing
 *         uint32_t x - the x coordinate, relative to the top left of matrix
 *         uint32_t y - the y coordinate, relative to the top left of matrix
 *         bool delete_old - should old frame in its place be deleted?
 * Output: bool - true if the set was successful
 */
bool MapPerson::setState(TileSprite* frame, SurfaceClassifier surface,
                         Direction direction, uint32_t x, uint32_t y,
                         bool delete_old)
{
  SpriteMatrix* matrix = getState(surface, direction);

  /* Proceed to set up the frame */
  if(matrix != NULL && frame != NULL)
  {
    unsetState(surface, direction, x, y, delete_old);
    matrix->setSprite(frame, x, y, delete_old);
    return true;
  }
  return false;
}

/*
 * Description: Sets a set of tile sprites in a specific surface and
 *              direction.
 *
 * Inputs: TileSprite* frame - the rendering tile sprite to add
 *         SurfaceClassifier surface - the surface the person is on
 *         Direction direction - the direction the person is facing
 *         bool delete_old - should old frame in its place be deleted?
 * Output: bool - true if the set was successful
 */
bool MapPerson::setStates(std::vector<std::vector<TileSprite*>> frames,
                          SurfaceClassifier surface, Direction direction,
                          bool delete_old)
{
  /* Unset the existing frames */
  unsetStates(surface, direction, delete_old);

  /* Proceed to set up the frame set */
  SpriteMatrix* matrix = getState(surface, direction, false);
  if(matrix != NULL)
  {
    matrix->setSprites(frames, delete_old);
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
  setMatrix(getState(surface, direction));
}

#ifdef UDEBUG
/*
 * Description: Toggles the ignore all passability to freely roam. Expect bugs!
 *
 * Inputs: none
 * Output: none
 */
void MapPerson::toggleIgnorePassability()
{
  ignore_passability = !ignore_passability;
}
#endif

/*
 * Description: Updates the state of the person. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the time elapsed between updates
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 *         bool active_map - true if this persons section is the active map
 * Output: Floatinate - the delta x and y of the moved person
 */
Floatinate MapPerson::update(int cycle_time,
                             std::vector<std::vector<Tile*>> tile_set,
                             bool active_map)
{
  Floatinate delta_move;

  /* For active and set tiles, update movement and animation */
  if(isActive() && isTilesSet())
  {
    /* All moving and animation is only valid if on the active map */
    if(active_map)
    {
      bool almost_there = false;
      bool reset = false;

      /* If moving and almost on tile, finish move and check what to do next */
      if(!isMoving() || (almost_there = isAlmostOnTile(cycle_time)))
      {
        /* Finish tile move */
        if(almost_there)
          tileMoveFinish(getID() != kPLAYER_ID);
        if(getMovementPaused())
        {
          if(getTarget() != nullptr ||
             (isForcedInteraction(false) && getPlayer() != nullptr))
          {
            int delta_x = 0;
            int delta_y = 0;
            if(getTarget() != nullptr)
            {
              delta_x = getTarget()->getCenterX() - getCenterX();
              delta_y = getTarget()->getCenterY() - getCenterY();
            }
            else
            {
              delta_x = getPlayer()->getCenterX() - getCenterX();
              delta_y = getPlayer()->getCenterY() - getCenterY();
            }

            /* Determine the absolute values of each variable */
            int pos_x = delta_x < 0 ? 0 - delta_x : delta_x;
            int pos_y = delta_y < 0 ? 0 - delta_y : delta_y;

            /* Determine which direction to face */
            if(delta_x < 0 && pos_x > pos_y)
              setDirection(Direction::WEST, false);
            else if(delta_x >= 0 && pos_x > pos_y)
              setDirection(Direction::EAST, false);
            else if(delta_y < 0 && pos_y >= pos_x)
              setDirection(Direction::NORTH, false);
            else if(delta_y >= 0 && pos_y >= pos_x)
              setDirection(Direction::SOUTH, false);
          }
          else
          {
            setDirection(Direction::DIRECTIONLESS);
          }

          reset = true;
        }
        /* Otherwise, if move is requested, start the new move */
        else if(isMoveRequested())
        {
          bool can_move = isMoveAllowed(tile_set, getMoveRequest());

          if(setDirection(getMoveRequest(), can_move) || !can_move)
            reset = true;
          if(can_move)
            tileMoveStart(tile_set, getID() != kPLAYER_ID);
        }
        /* Otherwise, make sure animation is on default state */
        else
        {
          setDirection(Direction::DIRECTIONLESS);
          reset = true;
        }
      }

      /* Animate and move the person */
      delta_move = moveThing(cycle_time);
      animate(cycle_time, reset, getMovement() != Direction::DIRECTIONLESS);

      /* Sound trigger for person/npc - for now, only player (TODO:FUTURE) */
      if(getID() == 0 && base_category != ThingBase::ISBASE &&
         getSoundID() >= 0)
      {
        if(sound_delay > 0)
        {
          sound_delay -= cycle_time;
          if(sound_delay <= 0 && event_handler != nullptr)
            event_handler->triggerSound(getSoundID(), SoundChannels::THINGS);
        }

        /* If sound delay is nil or less, re-generate */
        if(sound_delay <= 0)
          sound_delay = Helpers::randU(kDELAY_MIN, kDELAY_MAX);
      }
    }
  }
  /* Base Person */
  else if(base == nullptr)
  {
    for(uint32_t i = 0; i < states.size(); i++)
      for(uint32_t j = 0; j < states[i].size(); j++)
        if(states[i][j] != nullptr)
          states[i][j]->update(cycle_time, false, true);
  }
  /* Not active or tiles not set but instant thing */
  else
  {
    delta_move = MapThing::update(cycle_time, tile_set, active_map);
  }

  return delta_move;
}

/*
 * Description: Unsets an individual tile sprite located on a given surface and
 *              with a given direction, on a specific x and y coordinate.
 *
 * Inputs: SurfaceClassifier surface - the surface the sprite relates to
 *         Direction direction - the direction the sprite relates to
 *         uint32_t x - the x coordinate of the sprite
 *         uint32_t y - the y coordinate of the sprite
 *         bool delete_frames - true if the frames should be deleted from mem
 * Output: none
 */
void MapPerson::unsetState(SurfaceClassifier surface, Direction direction,
                           uint32_t x, uint32_t y, bool delete_frames)
{
  SpriteMatrix* matrix = getState(surface, direction, false);
  if(matrix != NULL)
  {
    matrix->unsetSprite(x, y, delete_frames);
    if(isTilesSet())
      unsetTile(x, y, true);
  }
}

/*
 * Description: Unsets a set of tile sprites located on a given surface and
 *              with a given direction.
 *
 * Inputs: SurfaceClassifier surface - the surface the sprite relates to
 *         Direction direction - the direction the sprite relates to
 *         bool delete_frames - true if the frames should be deleted from mem
 * Output: none
 */
void MapPerson::unsetStates(SurfaceClassifier surface, Direction direction,
                            bool delete_frames)
{
  unsetTiles(true);

  SpriteMatrix* matrix = getState(surface, direction, false);
  if(matrix != NULL)
    matrix->unsetSprites(delete_frames);
}

/*
 * Description: Unsets all tile sprites on all surfaces and directions.
 *
 * Inputs: bool delete_frames - true if the frames should be deleted from mem
 * Output: none
 */
void MapPerson::unsetStates(bool delete_frames)
{
  unsetTiles(true);

  for(uint8_t i = 0; i < states.size(); i++)
    for(uint8_t j = 0; j < states[i].size(); j++)
      if(states[i][j] != NULL)
        states[i][j]->unsetSprites(delete_frames);
}

/*
 * Description: Unsets the tiles from the thing. This will stop the rendering
 *              process, since it doesn't have any corresponding tiles to render
 *              to. However, the starting coordinate is untouched.
 *
 * Inputs: bool no_events - true if no events will trigger on removal from tile
 * Output: none
 */
void MapPerson::unsetTiles(bool no_events)
{
  /* Unset in each frame of the thing */
  if(isTilesSet())
  {
    /* Reset movement and animation */
    keyFlush();

    MapThing::unsetTiles(no_events);
  }
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

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
