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
  SCREEN_SIZE = 1 << 0,    /* If the Lay is to be rendered as the screen size */
  PLAYER_RELATIVE = 2 << 1 /* If the Lay is relative to the player movement */
};

/* LayTile Structure */
struct LayTile
{
  /* Construct a LayTile object */
  LayTile() : lay_sprite{nullptr}, location{0, 0} {};

  /* Pointer to the Lay Tile's sprite */
  Sprite* lay_sprite;

  /* The current rendering coordinate */
  Coordinate location;
};

/* Lay Class */
class Lay
{
public:
  /* Default constructor for a Lay */
  Lay();

  /* Player Relative lay constructor */
  Lay(std::string path, Floatinate velocity, LayType lay_type,
      SDL_Renderer* renderer, Options* config);

  /* General lay constructor */
  Lay(std::string path, uint32_t animation_time, Floatinate velocity,
      LayType lay_type, SDL_Renderer* renderer, Options* config);

  /* Annihilate a Lay object */
  ~Lay();

private:
  /* Animation tiles for the lay sprites */
  uint32_t animation_time;

  /* The lay's configuration */
  Options* config;

  /* The running error margin on velocity updates */
  Floatinate error;

  /* The enumerated flags for the lay state */
  LayState flags;

  /* The vector of lay tiles */
  std::vector<LayTile*> lay_tiles;

  /* Base sprite bath */
  std::string path;

  /* The renderer */
  SDL_Renderer* renderer;

  /* The velocity for the layed tiles */
  Floatinate velocity;

  /* ------- CONSTANTS ------- */
  static const float kABS_MAX_VELOCITY_X; /* MAX pixels / ms x-velocity */
  static const float kABS_MAX_VELOCITY_Y; /* MAX pixels / ms y-velocity */

public:
  /* The type of lay this object is */
  LayType lay_type;

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Renders the lay */
  bool render();

  /* Updates the lay based on the cycle time of the object */
  void update(int32_t cycle_time);

  /* Evaluates and returns the state of a given LayState flag */
  bool getFlag(const LayState& test_flag);

  /* Assign a configuration */
  void setConfig(Options* config);

  /* Assign a given enumerated flag a given value */
  void setFlag(const LayState& flag, const bool& set_value = true);

  /* Assigns a new renderer */
  void setRenderer(SDL_Renderer* renderer);

  /* Assigns a velocity to the way, within maximum parameters */
  void setVelocity(Floatinate new_velocity);

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Create a lay at a given index */
  bool createTiledLay(LayIndex lay_index);

  /* Create the starting tiled lays based on the velocity */
  bool createTiledLays();
};

#endif // LAY_H
