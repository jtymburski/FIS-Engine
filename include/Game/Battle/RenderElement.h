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

/* Forward declare for Battle constants */
class Battle;

#include "Box.h"
#include "Sprite.h"
#include "Text.h"

enum class RenderType
{
  PLEP,
  ACTION_TEXT,
  DAMAGE_TEXT,
  ENTER_TEXT,
  VICTORY_TEXT,
  DAMAGE_VALUE,
  RGB_OVERLAY,
  RGB_SPRITE_FLASH,
  RGB_SPRITE_DEATH,
  NONE
};

class RenderElement
{
public:
  /* Default constructor */
  RenderElement();

  /* Textual element constructor */
  RenderElement(SDL_Renderer* renderer, TTF_Font* element_font);

  /* Spriteish element constructor */
  RenderElement(SDL_Renderer* renderer, Sprite* plep_sprite, int32_t num_loops,
                int32_t x, int32_t y);

  /* Plep constructor with sprite path */
  RenderElement(SDL_Renderer* renderer, Sprite* animation, int32_t num_loops,
                Coordinate point);

  /* Overlay and Midlay constructor */
  RenderElement(SDL_Renderer* renderer, Box location, RenderType type,
                Floatinate velocity, uint8_t alpha, int32_t animation_time);

  /* Elemental annihilator */
  ~RenderElement();

  /* Does the render element have a shadow? */
  bool has_shadow;

  /* The render element does not time out */
  bool timeable;

  /* The boxed location */
  Box location;

  /* Coordinate for the element to reach an end point */
  // Coordinate end;

  /* Shadow's offset, if the element has a shadow */
  Coordinate shadow_offset;

  /* Floate coordinate for velocity/accel and acquired delta */
  Floatinate velocity;
  Floatinate acceleration;
  Floatinate delta;

  std::string text_string;

  /* Timing variables */
  int32_t time_fade_in;
  int32_t time_fade_out;
  int32_t time_begin;
  int32_t time_left;

  /* Alpha to display the element at (depending on fade state, perhaps) */
  uint8_t alpha;
  uint32_t fade_rate;

  /* Pointer to an element sprite */
  Sprite* element_sprite;
  uint32_t loops_to_do;

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
  const static SDL_Color kMISS_TEXT_COLOR;

  const static uint16_t kACTION_COLOR_R; /* Red color for middle text */
  const static uint16_t kACTION_SHADOW;  /* Shadow offset of middle text */
  const static uint16_t kDAMAGE_SHADOW;
  const static float kACTION_TEXT_X; /* Right edge of middle text */
  const static float kACTION_CENTER;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  bool buildSprite(Sprite* sprite);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  // bool buildSprite(std::string sprite_path, int32_t num_frames = 1);
  bool buildSpriteLay(std::string sprite_path);

  /* Creates the render element as an action text*/
  void createAsActionText(std::string text, int32_t screen_width, int32_t screen_height);

  /* Creates the render element as a damage text*/
  void createAsDamageText(std::string text, DamageType type, int32_t sc_height,
                          int32_t x, int32_t y);

  /* Creates the render element as a damage value */
  void createAsDamageValue(int32_t amount, DamageType type, int32_t sc_height,
                           int32_t x, int32_t y);

  /* Creates the render element as a turn text */
  void createAsEnterText(std::string text, int32_t sc_height, int32_t sc_width);

  /* Creates the render element as a regen value */
  void createAsRegenValue(int32_t amount, DamageType type, int32_t sc_height,
                          int32_t x, int32_t y);

  /* Creates the render element as an RGB Overlay */
  void createAsRGBOverlay(SDL_Color color, int32_t overlay_time,
                          int32_t fade_in_time, int32_t fade_out_time,
                          int32_t sc_height, int32_t sc_width);

  /* Creates the render element as a sprite death */
  void createAsSpriteDeath(SDL_Color color, int32_t death_time,
                           int32_t fade_in_time, int32_t fade_out_time);

  /* Victorious Text */
  void createAsVictoryText(std::string victory_text, int32_t sc_height,
                           int32_t sc_width);

  /* Creates the render element as a sprite flash */
  void createAsSpriteFlash(SDL_Color color, int32_t flash_time);

  /* Assigns floatinate acceleration coordinate point */
  void setAcceleration(float acceleration_x, float acceleration_y);

  /* Sets the render element to have a shadow */
  void setShadow(SDL_Color shadow_color, int32_t offset_x, int32_t offset_y);

  /* Assigns the time values (life time, fade in) for the elment */
  bool setTimes(int32_t time_begin, int32_t time_fade_in = 0,
                int32_t time_fade_out = 0);

  /* Sete the render element to not time out */
  void setTimeable(bool new_timeable_value);

  /* Assigns velocity floatinate coordinate point */
  void setVelocity(float velocity_x, float velocity_y);

  /* Updates the state of the render element */
  bool update(int32_t cycle_time);
  void updateStatusPlep(int32_t cycle_time);
  void updateStatusFade(int32_t cycle_time);

  RenderStatus initialStatusFade();

  /*===================== PUBLIC STATIC FUNCTIONS ============================*/
private:
  static SDL_Color colorFromDamageType(DamageType type);
};

#endif // RENDER_ELEMENT_H
