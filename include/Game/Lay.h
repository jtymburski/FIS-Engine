/*******************************************************************************
* Class Name: Lay
* Date Created: February 14, 2016
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*******************************************************************************/
#ifndef LAY_H
#define LAY_H

#include "Sprite.h"
#include "Helpers.h"
#include "Options.h"

struct LayTile
{
  LayTile() : lay_sprite{nullptr}, location{0, 0} {};
  Sprite* lay_sprite;
  Coordinate location;
};

class Lay
{
public:
  Lay();

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

  /* The vector of lay tiles */
  std::vector<LayTile*> lay_tiles;

  /* Base sprite bath */
  std::string path;

  /* The renderer */
  SDL_Renderer* renderer;

  /* The velocity for the layed tiles */
  Floatinate velocity;

  /* ------- CONSTANTS ------- */
  static const float kABS_MAX_VELOCITY_X; // MAX pixels / ms x-velocity
  static const float kABS_MAX_VELOCITY_Y; // MAX pixels / ms y-velocity

public:
  /* The type of lay this object is */
  LayType lay_type;

public:
  /* Assign a configuration */
  void setConfig(Options* config);

  /* Assigns a new renderer */
  void setRenderer(SDL_Renderer* renderer);

  /* Assigns a velocity to the way, within maximum parameters */
  void setVelocity(Floatinate new_velocity);

  /* Renders the lay */
  bool render();

  /* Updates the lay based on the cycle time of the object */
  void update(int32_t cycle_time);

private:
  /* Create the starting tiled lays based on the velocity */
  bool createTiledLays();

  /* Create a lay at a given index */
  bool createLay(LayIndex lay_index);
};

#endif // LAY_H
