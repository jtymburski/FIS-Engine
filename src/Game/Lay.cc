/*******************************************************************************
* Class Name: Lay [Implementation]
* Date Created: February 14, 2016
* Inheritance: None
* Description: A lay ins an underlay, midlay, or overlay Sprite rendered on
*              the map or in Battle. A lay is seamless with any velocity
*              movement (x, y).
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/Lay.h"

/*=============================================================================
 * CONSTANTS - See .h for Details
 *============================================================================*/

const float Lay::kABS_MAX_VELOCITY_X{3};
const float Lay::kABS_MAX_VELOCITY_Y{2};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default Lay constructor.
 *
 * Inputs: none
 */
Lay::Lay()
    : animation_time{0},
      error{Floatinate(0, 0)},
      flags{static_cast<LayState>(0)},
      lay_sprite{nullptr},
      lay_tiles{},
      path{""},
      range_top_left{Coordinate(0, 0)},
      range_bot_right{Coordinate(0, 0)},
      screen_size{Coordinate(0, 0)},
      velocity{Floatinate(0, 0)},
      lay_type{LayType::NONE}
{
}

/*
 * Description: Relative Lay constructor. Constructs a Lay with a Floatinate
 *              for velocity representing a fraction of the velocity of the
 *              Lay from the Player's (ex. 0.50 --> 50% of the Player's
 *              velocity).
 *
 * Inputs: std:string path - the string path to the Lay.
 *         Floatinate velocity - velocity relative to the player
 *         LayType lay_type - the enumerated lay type (over, under, mid etc.)
 *         Coordinate screen_size - the size of the screen
 *         SDL_Renderer* renderer - pointer to the renderer
 */
Lay::Lay(std::string path, Floatinate velocity, LayType lay_type,
         Coordinate screen_size) : Lay()
{
  this->path = path;
  this->lay_type = lay_type;
  this->screen_size = screen_size;
  setVelocity(velocity);

  setFlag(LayState::SCREEN_SIZE, true);
  setFlag(LayState::PLAYER_RELATIVE, true);
}

/*
 * Description: General Lay constructor. Constructs a normal Lay with
 *              a floatinate velocity, animation time, and type.
 *
 * Inputs: std::string path - the path to the lay's sprite to tile
 *         Floatinate velocity - (x, y) velocity to stream the sprite
 *         LayType lay_type - enumerated type of the lay (under, over, etc.)
 *         Coordinate screen_size - the size of the scren
 *         SDL_Renderer* renderer - pointer to the renderer
 */
Lay::Lay(std::string path, uint32_t animation_time, Floatinate velocity,
         LayType lay_type, Coordinate screen_size)
    : Lay()
{
  this->path = path;
  this->animation_time = animation_time;
  this->lay_type = lay_type;
  this->screen_size = screen_size;
  setVelocity(velocity);

  setFlag(LayState::SCREEN_SIZE, true);
}

/*
 * Description: Annihilates a Lay object. Upon destruction, each lay tile
 *              and lay tile sprite must be cleared.
 */
Lay::~Lay()
{
  if(lay_sprite)
    delete lay_sprite;
  lay_sprite = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Creates a tiled lay given an enumerated index (NORTH_EAST,
 *              EAST, etc.). To create a lay, the path must be split to
 *              determine which Sprite constructor is to be called. This method
 *              assings the appropriate starting (x, y) for the given tiled lay.
 *
 * Inputs: LayIndex lay_index - enumerated index of tiled lay to create
 * Output: bool - true if the tiled lay was created successfully
 */
 void Lay::createTiledLay(LayIndex lay_index)
{
  Coordinate new_tile;

  if(lay_index == LayIndex::NORTH_WEST)
    new_tile = {-screen_size.x, -screen_size.y};
  else if(lay_index == LayIndex::NORTH)
    new_tile = {0, -screen_size.y};
  else if(lay_index == LayIndex::NORTH_EAST)
    new_tile = {screen_size.x, -screen_size.y};
  else if(lay_index == LayIndex::WEST)
    new_tile = {-screen_size.x, 0};
  else if(lay_index == LayIndex::CENTRE)
    new_tile = {0, 0};
  else if(lay_index == LayIndex::EAST)
    new_tile = {screen_size.x, 0};
  else if(lay_index == LayIndex::SOUTH_WEST)
    new_tile = {-screen_size.x, screen_size.y};
  else if(lay_index == LayIndex::SOUTH)
    new_tile = {0, screen_size.y};
  else if(lay_index == LayIndex::SOUTH_EAST)
    new_tile = {screen_size.x, screen_size.y};

  lay_tiles.push_back(new_tile);
}



/*
 * Description: Determines whether a given range is valid within the lay
 *              RANGE_BOUND context.
 *
 * Inputs: none
 * Output: bool - whether the currently assigned range bounds are valid.
 */
bool Lay::isRangeValid()
{
  bool valid = range_top_left.x >= 0;
  valid &= range_top_left.y >= 0;
  valid &= range_bot_right.x >= 0;
  valid &= range_bot_right.y >= 0;
  valid &= (range_top_left.x <= range_bot_right.x &&
            range_top_left.y <= range_bot_right.y);

  return valid;
}

/*
 * Description: Updates the location of the Lay by a shift_x and shift_y.
 *              Generally called either by a cycle time update or a float
 *              shift amount.
 *
 * Inputs: dist_x - the x-distance to shift the lay by
 *         disT_y - the y-distance to shift the lay by
 * Output: none
 */
void Lay::updateLocations(int32_t dist_x, int32_t dist_y)
{
  for(auto& lay_tile : lay_tiles)
  {
    /* Wrap the left to horizontal tiles back to the beginning */
    if((lay_tile.x + dist_x) > screen_size.x)
      lay_tile.x -= screen_size.x * 2;

    /* Wrap the bottom to vertical tiles back to the beginning */
    if((lay_tile.y + dist_y) > screen_size.y)
      lay_tile.y -= screen_size.y * 2;

    /* Wrap the right to horizontal tiles back to the beginning */
    if((lay_tile.x + dist_x) < -screen_size.x)
      lay_tile.x += screen_size.x * 2;

    /* Wrap the top to vertical tiles back to the beginning */
    if((lay_tile.y + dist_y) < -screen_size.y)
      lay_tile.y += screen_size.y * 2;

    lay_tile.x += dist_x;
    lay_tile.y += dist_y;
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Creates the lay over sprite and velocity from the lay over
 *              struct. If a renderer is passed in, it also renders the new
 *              information.
 *
 * Inputs: LayOver lay_data - the struct of sprite information
 *         SDL_Renderer* renderer - the rendering engine
 * Output: none
 */
void Lay::createFromLayStruct(LayOver lay_data, SDL_Renderer* renderer)
{
  /* Set the info */
  setSpritePath(lay_data.path);
  if(lay_data.anim_time >= 0)
    setSpriteAnimation(static_cast<uint32_t>(lay_data.anim_time));
  else
    setSpriteAnimation(0);
  setVelocity({lay_data.velocity_x, lay_data.velocity_y});

  /* Generate texture if render engine is available */
  if(renderer)
    createTiledLays(renderer);
}

/*
 * Description: Determines which lay tiles (NORTH_EAST, EAST, etc.) need to
 *              be created for the given velocity (x, y) floatinate. This
 *              method will call for the creation of the needed lays and
 *              return whether they were successfully constructed.
 *
 * Inputs: none
 * Output: bool - true if the lays were created successfully
 */
bool Lay::createTiledLays(SDL_Renderer* renderer)
{
  /* Delete the sprite if it exists */
  if(lay_sprite)
    delete lay_sprite;
  lay_sprite = nullptr;
  lay_tiles.clear();

  /* The sprite should not already be created */
  if(!lay_sprite && renderer && path != "")
  {
    auto split = Helpers::split(path, '|');
    auto num_frames = 1;
    if(split.size() == 3)
    {
      num_frames = std::stoi(split.at(1));
      lay_sprite = new Sprite(split.at(0), num_frames, split.at(2), renderer);
    }
    else if(split.size() == 1)
    {
      lay_sprite = new Sprite(path, renderer);
    }

    if(split.size() == 1 || split.size() == 3)
    {
      lay_sprite->setAnimationTime(animation_time);
      //lay_sprite->setNonUnique(true, num_frames); // LEAK.
      lay_sprite->createTexture(renderer);
    }

    createTiledLay(LayIndex::CENTRE);

    if(velocity.x < 0)
      createTiledLay(LayIndex::EAST);
    if(velocity.x > 0)
      createTiledLay(LayIndex::WEST);
    if(velocity.y < 0)
      createTiledLay(LayIndex::SOUTH);
    if(velocity.y > 0)
      createTiledLay(LayIndex::NORTH);

    if(velocity.x > 0 && velocity.y > 0)
      createTiledLay(LayIndex::NORTH_WEST);
    else if(velocity.x > 0 && velocity.y < 0)
      createTiledLay(LayIndex::SOUTH_WEST);
    else if(velocity.x < 0 && velocity.y > 0)
      createTiledLay(LayIndex::NORTH_EAST);
    else if(velocity.x < 0 && velocity.y < 0)
      createTiledLay(LayIndex::SOUTH_EAST);

    return true;
  }

  return false;
}

/*
 * Description: Evaluates and returns a given LayState flag.
 *
 * Inputs: const LayState& test_flag - given flag to find the value for
 * Output: bool - the value of the given flag
 */
bool Lay::getFlag(const LayState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Renders the lay at its given position. If the lay flag
 *              is set to screen_size, the configuration screen width will be
 *              used as the size to render the lay.
 *
 * Inputs: none
 * Output: bool - true if rendering was successful
 */
bool Lay::render(SDL_Renderer* renderer)
{
  bool success{true};

  if(getFlag(LayState::SCREEN_SIZE) && renderer)
  {
    /* If the sprite has not been created, create it */
    if(lay_sprite == nullptr && path != "")
      createTiledLays(renderer);

    /* Attempt render */
    if(lay_sprite)
    {
      for(const auto& lay_tile : lay_tiles)
      {
        success &= lay_sprite->render(renderer, lay_tile.x, lay_tile.y,
                                      screen_size.x, screen_size.y);
      }
    }
  }

  return success;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Lay::render(SDL_Renderer* renderer, Coordinate player_position)
{
  /* Check to see if the range is within bounds */
  if(getFlag(LayState::RANGE_BOUND))
  {
    if(Helpers::isWithinRange(player_position, range_top_left, range_bot_right))
      return render(renderer);
  }
  else
  {
    return render(renderer);
  }

  /* Return true if no rendering took place */
  return true;
}

/*
 * Description: Assigns a given LayState flag to a given boolean value.
 *
 * Inputs: const LayState& flag - enumerated flag to be assigned
 *         const bool& set_value - value to assign to the flag
 * Output: none
 */
void Lay::setFlag(const LayState& flag, const bool& set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description: Sets the range bound for player location based lays
 *
 * Inputs: Coordinate top_left - top left range
 *         Coordinate bot_right - bottom right range
 * Output: none
 */
void Lay::setRange(Coordinate top_left, Coordinate bot_right)
{
  this->range_top_left = top_left;
  this->range_bot_right = bot_right;
  setFlag(LayState::RANGE_BOUND, true);
}

/*
 * Description: Assigns the screen size for rendering the lay. This will stretch
 *              the existing sprite to the size of screen regardless of size.
 *
 * Inputs: int32_t screen_x - the x width of screen
 *         int32_t screen_y - the y height of screen
 * Output: bool - true if changed
 */
bool Lay::setScreenSize(const int32_t &screen_x, const int32_t &screen_y)
{
  if(screen_x > 0 && screen_y > 0)
  {
    screen_size.x = screen_x;
    screen_size.y = screen_y;
    return true;
  }
  return false;
}

/*
 * Description: Changes the sprite animation time to a new value.
 *
 * Inputs: uint32_t animation_time - animation time for sprite in milliseconds
 * Output: none
 */
void Lay::setSpriteAnimation(const uint32_t &animation_time)
{
  this->animation_time = animation_time;

  /* If sprite is valid, update the animation time */
  if(lay_sprite)
    lay_sprite->setAnimationTime(animation_time);
}

/*
 * Description: Changes the sprite path to a new string. It will delete the
 *              existing sprite prior if it exists
 *
 * Inputs: std::string path - the new sprite path
 * Output: none
 */
void Lay::setSpritePath(const std::string &path)
{
  /* Delete lay sprite if already created */
  if(lay_sprite)
    delete lay_sprite;
  lay_sprite = nullptr;
  lay_tiles.clear();

  /* Change path */
  this->path = path;
}

/*
 * Description: Assigns a given Floatinate velocity (x, y) such that it will
 *              be within acceptable parameters
 *
 * Inputs: Floatinate new_velocity - the velocity (x, y) to be assigned
 * Output: none
 */
void Lay::setVelocity(Floatinate new_velocity)
{
  velocity.x = Helpers::setInRange(new_velocity.x, -kABS_MAX_VELOCITY_X,
                                   kABS_MAX_VELOCITY_X);
  velocity.y = Helpers::setInRange(new_velocity.y, -kABS_MAX_VELOCITY_Y,
                                   kABS_MAX_VELOCITY_Y);
}

/*
 * Description: Shifts the tiled lays by a given floatinate shift, keeping track
 *              of the error margin.
 *
 * Inputs: Floatinate shift - amount to shift the lay by
 * Output: none
 */
void Lay::shift(Floatinate shift)
{
  error.x += shift.x * velocity.x;
  error.y += shift.y * velocity.y;

  auto dist_x = std::floor(error.x);
  auto dist_y = std::floor(error.y);

  updateLocations(dist_x, dist_y);

  error.x -= dist_x;
  error.y -= dist_y;
}

/*
 * Description: Updates the position of the Lay based upon its (x, y) velocity
 *              if it is not a player_relative lay. This method will shuffle
 *              the lays back into appropriate positions to create the seamless
 *              transfer effect for any value of (x, y) velocity.
 *
 * Inputs: int32_t cycle_time - the update cycle time, affecting the velocity
 * Output: none
 */
void Lay::update(int32_t cycle_time)
{
  error.x += cycle_time * velocity.x;
  error.y += cycle_time * velocity.y;

  auto dist_x = std::floor(error.x);
  auto dist_y = std::floor(error.y);

  updateLocations(dist_x, dist_y);

  if(lay_sprite)
    lay_sprite->update(cycle_time);

  error.x -= dist_x;
  error.y -= dist_y;
}
