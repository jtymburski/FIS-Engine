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

class RenderElement
{
public:
  RenderElement();
  RenderElement(SDL_Renderer* renderer, TTF_Font* element_font);
  RenderElement(SDL_Renderer* renderer, Sprite* plep_sprite);

  ~RenderElement();

  bool has_shadow;

  Box location;
  Coordinate end;
  Coordinate shadow_offset;

  Floatinate velocity;
  Floatinate acceleration;
  Floatinate delta;

  uint32_t time_fade_in;
  uint32_t time_fade_out;
  uint32_t time_begin;
  uint32_t time_left;

  uint8_t alpha;

  Sprite* element_sprite;

  Text element_text;
  TTF_Font* element_font;

  SDL_Color color;
  SDL_Color shadow_color;

  SDL_Renderer* renderer;

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

  /* Creates the render element as a damge text*/
  void createAsDamageText(int32_t amount, DamageType type, int32_t sc_height,
                          int32_t x, int32_t y);

  /* Sets the render element to have a shadow */
  void setShadow(SDL_Color shadow_color, int32_t offset_x, int32_t offset_y);

  /* Assigns the time values (life time, fade in) for the elment */
  bool setTimes(uint32_t time_begin, uint32_t time_fade_in = 0,
                uint32_t time_fade_out = 0);

  /*=============================================================================
   * PRIVATE STATIC FUNCTIONS
   *============================================================================*/
private:
  static SDL_Color colorFromDamageType(DamageType type);
};

#endif // RENDER_ELEMENT_H
