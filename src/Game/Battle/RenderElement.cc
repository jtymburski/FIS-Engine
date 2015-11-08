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
      renderer{nullptr}
{
}

RenderElement::RenderElement(SDL_Renderer* renderer, TTF_Font* element_font)
    : RenderElement()
{
  this->element_font = element_font;
  this->renderer = renderer;
}

RenderElement::RenderElement(SDL_Renderer* renderer, Sprite* plep_sprite)
    : RenderElement()
{
  this->renderer = renderer;
  buildSprite(plep_sprite);
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

  if(element_font && renderer)
  {
    element_text = Text(element_font);
    element_text.setText(renderer, action_name, color);

    location.point.x = kACTION_TEXT_X - element_text.getWidth();
    location.point.y = kACTION_CENTER - element_text.getHeight() / 2 - 8;
  }
}

void RenderElement::createAsDamageText(int32_t amount, DamageType type,
                                       int32_t sc_height, int32_t x,
                                       int32_t y)
{
  color = {0, 0, 0, 255};
  setShadow(colorFromDamageType(type), kDAMAGE_SHADOW, kDAMAGE_SHADOW - 1);
  setTimes(475, 65, 100);

  if(element_font && renderer)
  {
    element_text = Text(element_font);
    element_text.setText(renderer, std::to_string(amount), color);

    auto half_w = element_text.getWidth() / 2;
    location.point.x = x + (Battle::kPERSON_WIDTH / 2) - half_w;
    location.point.y = y + half_w + (sc_height / 13);
  }
}

void RenderElement::setShadow(SDL_Color shadow_color, int32_t offset_x,
                              int32_t offset_y)
{
  this->shadow_color = shadow_color;
  this->has_shadow = true;
  shadow_offset.x = offset_x;
  shadow_offset.y = offset_y;
}

bool RenderElement::setTimes(uint32_t time_begin, uint32_t time_fade_in,
                             uint32_t time_fade_out)
{
  bool valid = time_begin >= 0;

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
