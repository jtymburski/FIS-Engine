/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
* Date Redesigned: November 8, 2015
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
#ifndef RENDER_ELEMENT_H
#define RENDER_ELEMENT_H

#include "Game/Battle/BattleActor.h"
#include "Sprite.h"
#include "Text.h"

/* Forward declare for Battle constants */
class Battle;

/* Coordinate with ints */
struct Coordinate
{
  Coordinate() : x{0}, y{0} {};

  int32_t x;
  int32_t y;
};

/* Box with an integer coordinate and a size width by height */
struct Box
{
  Box() : point{Coordinate()}, width{0}, height{0} {};

  Coordinate point;
  int32_t width;
  int32_t height;
};

/* Coodinate with floats */
struct Floatinate
{
  Floatinate() : x{0}, y{0} {};

  float x;
  float y;
};

enum class RenderType
{
  PLEP,
  ACTION_TEXT,
  DAMAGE_TEXT,
  DAMAGE_VALUE,
  RGB_SPRITE_FLASH,
  NONE
};

enum class RenderStatus
{
  FADING_IN,
  DISPLAY,
  FADING_OUT,
  TIMED_OUT
};

class RenderElement
{
public:
  /* Default constructor */
  RenderElement();

  /* Textual element constructor */
  RenderElement(SDL_Renderer* renderer, TTF_Font* element_font);

  /* Spriteish element constructor */
  RenderElement(SDL_Renderer* renderer, Sprite* plep_sprite, int32_t num_loops);

  /* Elemental annihilator */
  ~RenderElement();

  /* Does the render element have a shadow? */
  bool has_shadow;

  /* The boxed location */
  Box location;

  /* Coordinate for the element to reach an end point */
  //Coordinate end;

  /* Shadow's offset, if the element has a shadow */
  Coordinate shadow_offset;

  /* Floate coordinate for velocity/accel and acquired delta */
  Floatinate velocity;
  Floatinate acceleration;
  Floatinate delta;

  /* Timing variables */
  uint32_t time_fade_in;
  uint32_t time_fade_out;
  uint32_t time_begin;
  uint32_t time_left;

  /* Alpha to display the element at (depending on fade state, perhaps) */
  uint8_t alpha;

  /* Pointer to an element sprite */
  Sprite* element_sprite;

  /* Textual class members */
  Text element_text;
  TTF_Font* element_font;

  /* SDL colors to display the element */
  SDL_Color color;
  SDL_Color shadow_color;

  /* Pointer to the renderer */
  SDL_Renderer* renderer;

  /* RenderStatus */
  RenderStatus status;

  /* Actor target pointer */
  BattleActor* target;

  /* Type of the render */
  RenderType render_type;

private:
  /* ---- Color Constants ---- */
  const static SDL_Color kSTRD_DMG_COLOR;
  const static SDL_Color kCRIT_DMG_COLOR;
  const static SDL_Color kPOIS_DMG_COLOR;
  const static SDL_Color kBURN_DMG_COLOR;
  const static SDL_Color kHEAL_DMG_COLOR;
  const static SDL_Color kVITA_REGEN_COLOR;
  const static SDL_Color kQTDR_REGEN_COLOR;
  const static SDL_Color kHIBERNATION_REGEN_COLOR;

  const static uint16_t kACTION_COLOR_R; /* Red color for middle text */
  const static uint16_t kACTION_SHADOW; /* Shadow offset of middle text */
  const static uint16_t kDAMAGE_SHADOW;
  const static uint16_t kACTION_TEXT_X; /* Right edge of middle text */
  const static uint16_t kACTION_CENTER;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  bool buildSprite(Sprite* sprite);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Creates the render element as an action text*/
  void createAsActionText(std::string text);

  /* Creates the render element as a damage text*/
  void createAsDamageText(std::string text, DamageType type, int32_t sc_height,
                          int32_t x, int32_t y);

  void createAsDamageValue(int32_t amount, DamageType type, int32_t sc_height,
                           int32_t x, int32_t y);

  void createAsRegenValue(int32_t amount, DamageType type, int32_t sc_height,
                          int32_t x, int32_t y);

  void createAsSpriteFlash(BattleActor* target, SDL_Color color,
                           int32_t flash_time);

  /* Assigns floatinate acceleration coordinate point */
  void setAcceleration(float acceleration_x, float acceleration_y);

  /* Sets the render element to have a shadow */
  void setShadow(SDL_Color shadow_color, int32_t offset_x, int32_t offset_y);

  /* Assigns the time values (life time, fade in) for the elment */
  bool setTimes(uint32_t time_begin, uint32_t time_fade_in = 0,
                uint32_t time_fade_out = 0);

  /* Assigns velocity floatinate coordinate point */
  void setVelocity(float velocity_x, float velocity_y);

  /* Updates the state of the render element */
  bool update(int32_t cycle_time);

  bool updateStatus();
  void updateStatusPlep();
  void updateStatusFade();

  /*=============================================================================
   * PRIVATE STATIC FUNCTIONS
   *============================================================================*/
private:
  static SDL_Color colorFromDamageType(DamageType type);
};

#endif // RENDER_ELEMENT_H
