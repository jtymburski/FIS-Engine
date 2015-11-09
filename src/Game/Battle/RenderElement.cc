/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
* Date Redesigned: November 8, 2015
* Inheritance: None
*
* See .h for description and TODOs
******************************************************************************/
#include "Game/Battle/RenderElement.h"
#include "Game/Battle/Battle.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/* ---- Color Constants ---- */
const SDL_Color RenderElement::kSTRD_DMG_COLOR = {225, 225, 225, 255};
const SDL_Color RenderElement::kCRIT_DMG_COLOR = {255, 255, 0, 255};
const SDL_Color RenderElement::kPOIS_DMG_COLOR = {138, 43, 226, 255};
const SDL_Color RenderElement::kBURN_DMG_COLOR = {172, 0, 0, 255};
const SDL_Color RenderElement::kHEAL_DMG_COLOR = {50, 215, 49, 255};
const SDL_Color RenderElement::kVITA_REGEN_COLOR = {50, 205, 50, 255};
const SDL_Color RenderElement::kQTDR_REGEN_COLOR = {0, 128, 255, 255};
const SDL_Color RenderElement::kHIBERNATION_REGEN_COLOR = {75, 205, 50, 255};
const uint16_t RenderElement::kACTION_COLOR_R = 175;
const uint16_t RenderElement::kACTION_SHADOW = 3;
const uint16_t RenderElement::kDAMAGE_SHADOW = 2;
const uint16_t RenderElement::kACTION_TEXT_X = 800;
const uint16_t RenderElement::kACTION_CENTER = 381;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTOR
 *============================================================================*/

RenderElement::RenderElement()
    : has_shadow{false},
      time_fade_in{0},
      time_fade_out{0},
      time_left{0},
      alpha{0},
      element_sprite{nullptr},
      element_text{Text()},
      element_font{nullptr},
      color{0, 0, 0, 0},
      shadow_color{0, 0, 0, 0},
      renderer{nullptr},
      target{nullptr},
      render_type{RenderType::NONE}
{
}

RenderElement::RenderElement(SDL_Renderer* renderer, TTF_Font* element_font)
    : RenderElement()
{
  this->element_font = element_font;
  this->renderer = renderer;
}

RenderElement::RenderElement(SDL_Renderer* renderer, Sprite* plep_sprite,
                             int32_t num_loops)
    : RenderElement()
{
  this->renderer = renderer;
  buildSprite(plep_sprite);

  if(element_sprite)
    element_sprite->setNumLoops(num_loops);
  updateStatus();
  render_type = RenderType::PLEP;
}

RenderElement::~RenderElement()
{
  if(element_sprite)
    delete element_sprite;

  element_sprite = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

bool RenderElement::buildSprite(Sprite* build_sprite)
{
  if(renderer && build_sprite && !element_sprite)
  {
    element_sprite = new Sprite(*(build_sprite));

    if(element_sprite)
    {
      element_sprite->setNonUnique(true, build_sprite->getSize());
      element_sprite->createTexture(renderer);

      return true;
    }
  }

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void RenderElement::createAsActionText(std::string action_name)
{
  color = {0, 0, 0, 255};
  setShadow({kACTION_COLOR_R, 0, 0, 255}, kACTION_SHADOW, kACTION_SHADOW);
  setTimes(900, 100, 100);
  render_type = RenderType::ACTION_TEXT;

  if(element_font && renderer)
  {
    element_text = Text(element_font);
    element_text.setText(renderer, action_name, color);

    location.point.x = kACTION_TEXT_X - element_text.getWidth();
    location.point.y = kACTION_CENTER - element_text.getHeight() / 2 - 8;
  }
}

void RenderElement::createAsDamageText(std::string text, DamageType type,
                                       int32_t sc_height, int32_t x, int32_t y)
{
  color = {0, 0, 0, 255};
  setShadow(colorFromDamageType(type), kDAMAGE_SHADOW, kDAMAGE_SHADOW - 1);
  setTimes(475, 65, 100);
  render_type = RenderType::DAMAGE_TEXT;

  if(element_font && renderer)
  {
    element_text = Text(element_font);
    element_text.setText(renderer, text, color);

    auto half_w = element_text.getWidth() / 2;
    location.point.x = x + (Battle::kPERSON_WIDTH / 2) - half_w;
    location.point.y = y + half_w + (sc_height / 13);
  }
}

void RenderElement::createAsDamageValue(int32_t amount, DamageType type,
                                        int32_t sc_height, int32_t x, int32_t y)
{
  createAsDamageText(std::to_string(amount), type, sc_height, x, y);
  setTimes(550, 115, 155);
  setAcceleration(0.040, 0.000);
  setVelocity(0.020, -0.450);
  render_type = RenderType::DAMAGE_VALUE;
}

void RenderElement::createAsRegenValue(int32_t amount, DamageType type,
                                       int32_t sc_height, int32_t x, int32_t y)
{
  createAsDamageText(std::to_string(amount), type, sc_height, x, y);
  setTimes(650, 150, 150);
  setAcceleration(0.000, 0.005);
  setVelocity(0.000, -0.250);
  render_type = RenderType::DAMAGE_VALUE;
}

void RenderElement::createAsSpriteFlash(BattleActor* target, SDL_Color color,
                                        int32_t flash_time)
{
  auto fade_time = std::floor(flash_time * 3.00 / 7.00);
  setTimes(flash_time, fade_time, fade_time);
  updateStatus();
  this->color = color;
  this->target = target;
  render_type = RenderType::RGB_SPRITE_FLASH;
}

void RenderElement::setShadow(SDL_Color shadow_color, int32_t offset_x,
                              int32_t offset_y)
{
  this->shadow_color = shadow_color;
  this->has_shadow = true;
  shadow_offset.x = offset_x;
  shadow_offset.y = offset_y;
  render_type = RenderType::DAMAGE_VALUE;
}

bool RenderElement::setTimes(uint32_t time_begin, uint32_t time_fade_in,
                             uint32_t time_fade_out)
{
  auto valid = false;

  if(time_fade_in > 0 && time_fade_out == 0)
    valid &= time_fade_in <= time_begin;
  else if(time_fade_out > 0 && time_fade_in == 0)
    valid &= time_fade_out <= time_begin;
  else if(time_fade_in > 0 && time_fade_out > 0)
    valid &= time_fade_in + time_fade_out <= time_begin;

  this->time_begin = time_begin;
  this->time_left = time_begin;
  this->time_fade_in = time_fade_in;
  this->time_fade_out = time_fade_out;

  return valid;
}

void RenderElement::setVelocity(float velocity_x, float velocity_y)
{
  velocity.x = velocity_x;
  velocity.y = velocity_y;
}

void RenderElement::setAcceleration(float acceleration_x, float acceleration_y)
{
  acceleration.x = acceleration_x;
  acceleration.y = acceleration_y;
}

bool RenderElement::update(int32_t cycle_time)
{
  (void)cycle_time;
  updateStatus();
  return false;
}

bool RenderElement::updateStatus()
{
  if(render_type == RenderType::PLEP)
    updateStatusPlep();
  else
    updateStatusFade();

  return false;
}

void RenderElement::updateStatusPlep()
{

}

void RenderElement::updateStatusFade()
{

}

/*=============================================================================
 * PRIVATE STATIC FUNCTIONS
 *============================================================================*/

SDL_Color RenderElement::colorFromDamageType(DamageType type)
{
  if(type == DamageType::CRITICAL)
    return kCRIT_DMG_COLOR;
  else if(type == DamageType::POISON)
    return kPOIS_DMG_COLOR;
  else if(type == DamageType::HEALING)
    return kHEAL_DMG_COLOR;
  else if(type == DamageType::BURN)
    return kBURN_DMG_COLOR;
  else if(type == DamageType::VITA_REGEN)
    return kVITA_REGEN_COLOR;
  else if(type == DamageType::QTDR_REGEN)
    return kQTDR_REGEN_COLOR;
  else if(type == DamageType::HIBERNATION_REGEN)
    return kHIBERNATION_REGEN_COLOR;

  return kSTRD_DMG_COLOR;
}
