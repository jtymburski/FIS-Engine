/*******************************************************************************
* Class Name: Lay
* Date Created: February 14, 2016
* Inheritance: None
* Description: A lay ins an underlay, midlay, or overlay Sprite rendered on
*              the map or in Battle. A lay is seamless with any velocity
*              movement (x, y).
*
* TODO
* ----
*
* See .cc for Notes
*******************************************************************************/
#ifndef LAY_H
#define LAY_H

#include "Sprite.h"
#include "Helpers.h"
#include "Options.h"
#include "EnumFlags.h"

/* Enumerated LayState Flags */
ENUM_FLAGS(LayState);
enum class LayState
{
  SCREEN_SIZE = 1 << 0, /* If the Lay is to be rendered as the screen size */
  PLAYER_RELATIVE = 1 << 1, /* If the Lay is relative to the player movement */
  RANGE_BOUND = 1 << 3
};

/* Lay Class */
class Lay
{
public:
  /* Default constructor for a Lay */
  Lay();

  /* Player Relative lay constructor */
  Lay(std::string path, Floatinate velocity, LayType lay_type,
      Coordinate screen_size);

  /* General lay constructor */
  Lay(std::string path, uint32_t animation_time, Floatinate velocity,
      LayType lay_type, Coordinate screen_size);

  /* Lay struct constructor */
  Lay(LayOver data, LayType lay_type, Coordinate screen_size);

  /* Annihilate a Lay object */
  ~Lay();

private:
  /* Animation tiles for the lay sprites */
  uint32_t animation_time;

  /* The running error margin on velocity updates */
  Floatinate error;

  /* The enumerated flags for the lay state */
  LayState flags;

  /* Pointer to the Lay Tile's sprite */
  Sprite* lay_sprite;

  /* The vector of lay tiles */
  std::vector<Coordinate> lay_tiles;

  /* Base sprite bath */
  std::string path;

  /* Range to render a Map Lay within */
  Coordinate range_top_left;
  Coordinate range_bot_right;

  /* The size of the screen */
  Coordinate screen_size;

  /* The velocity for the layed tiles */
  Floatinate velocity;

  /* ----------- CONSTANTS -------------- */
  static const float kABS_MAX_VELOCITY_X; /* MAX pixels / ms x-velocity */
  static const float kABS_MAX_VELOCITY_Y; /* MAX pixels / ms y-velocity */

public:
  /* The type of lay this object is */
  LayType lay_type;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Create a lay at a given index */
  void createTiledLay(LayIndex lay_index);

  /* Asserts the assigned range is valid */
  bool isRangeValid();

  /* Update the tiled lays by a distance x, y */
  void updateLocations(int32_t dist_x, int32_t dist_y);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Create the lay over data from the lay over struct */
  void createFromLayStruct(LayOver lay_data, SDL_Renderer* renderer = nullptr);

  /* Create the starting tiled lays based on the velocity */
  bool createTiledLays(SDL_Renderer* renderer);

  /* Evaluates and returns the state of a given LayState flag */
  bool getFlag(const LayState& test_flag);

  /* Returns the reference sprite within the lay */
  Sprite* getLaySprite();

  /* Renders the lay */
  bool render(SDL_Renderer* renderer);

  /* Renders the lay given a player's position */
  bool render(SDL_Renderer* renderer, Coordinate player_position);

  /* Assign a given enumerated flag a given value */
  void setFlag(const LayState& flag, const bool& set_value = true);

  /* Assigns a range for the Lay, to only be renered in a tile range */
  void setRange(Coordinate top_left, Coordinate bot_right);

  /* Assigns the screen size */
  bool setScreenSize(const int32_t &screen_x, const int32_t &screen_y);

  /* Assigns sprite information */
  void setSpriteAnimation(const uint32_t &animation_time);
  void setSpritePath(const std::string &path);

  /* Assigns a velocity to the way, within maximum parameters */
  void setVelocity(Floatinate new_velocity);

  /* Shifts the lay by a given shift amount, relative to velocity */
  void shift(Floatinate shift);

  /* Updates the lay based on the cycle time of the object */
  void update(int32_t cycle_time);
};

#endif // LAY_H
