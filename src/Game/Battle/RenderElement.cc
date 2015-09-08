/***************************************************************************
* Class Name: /
* Date Created: June 22, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/Battle/RenderElement.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
RenderElement::RenderElement()
    : action_frame{nullptr}
    , render_sprite{nullptr}
    , flasher{nullptr}
    , status{RenderStatus::CONSTRUCTING}
    , type{RenderType::DAMAGE_VALUE}
    , color{0, 0, 0, 0}
    , shadow_color{0, 0, 0, 0}
    , font{nullptr}
    , text{""}
    , shadow{false}
    , remaining_time{0}
    , elapsed_time{0}
    , acceleration_x{0}
    , acceleration_y{0}
    , delta_x{0}
    , delta_y{0}
    , fade_rate{0}
    , x{0}
    , y{0}
    , shadow_x{0}
    , shadow_y{0}
    , size_x{0}
    , size_y{0}
    , velocity_x{0}
    , velocity_y{0}
    , fade_in_time{0}
    , fade_out_time{0}
    , alpha{0}
    , shadow_alpha{0}
{}

/*
 * Description: Creates a render element with a type, remaining and fade in/out
 *              times. Note that for the element to be valid the fade in and
 *              out times must both not exceed or together not exceed the
 *              lifetime of the behaviour.
 *
 * Inputs: RenderType type - the type of render element to be created
 *         int32_t remaining_time - the lifetime of the object
 *         int32_t fade_in_time - time taken to fade in (default 0)
 *         int32_t fade_out_time - the time taken to fade out
 */
RenderElement::RenderElement(RenderType type, int32_t remaining_time,
    int32_t fade_in_time, int32_t fade_out_time)
      : RenderElement()
{
  this->type = type;

  bool success = setTimes(remaining_time, fade_in_time, fade_out_time);

  if (success)
  {
    if (fade_in_time > 0)
      status = RenderStatus::FADING_IN;

    else if (fade_in_time == 0 || fade_out_time == remaining_time)
      status = RenderStatus::FADING_OUT;
    else
      status = RenderStatus::DISPLAYING;
  }
  else
    status = RenderStatus::TIMED_OUT;
}

/*
 * Description:
 *
 * Inputs:
 */
RenderElement::RenderElement(Sprite* plep, int32_t x, int32_t y,
    int32_t num_loops)
      : RenderElement()
{
  setSprite(plep);

  type   = RenderType::PLEP;
  status = RenderStatus::DISPLAYING;

  this->x = x;
  this->y = y;
  this->num_loops = num_loops;
}

/*
 * Description:
 *
 * Inputs:
 */
RenderElement::RenderElement(Sprite* plep, int32_t x, int32_t y,
    int32_t alpha_low, int32_t alpha_high, float fade_rate)
    : RenderElement()
{
  setSprite(plep);
  type = RenderType::CYCLING_FADE;
  status = RenderStatus::CYCLING;

  this->x = x;
  this->y = y;
  this->alpha_low = alpha_low;
  this->alpha_high = alpha_high;

  /* Set the original sprite's opacity in to shadow alpha for storage */
  if(plep)
    shadow_alpha = plep->getOpacity();

  if (fade_rate > 1.0 && fade_rate > 0)
    this->fade_rate = 1.0;
  else
    this->fade_rate = fade_rate;
}

/*
 * Description:
 *
 * Inputs:
 */
RenderElement::~RenderElement()
{
  if (type == RenderType::PLEP)
    if (render_sprite != nullptr)
      delete render_sprite;

  render_sprite = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

uint8_t RenderElement::calcColorRed()
{
  auto pc_fade = 0;

  if (color.a != 0)
    pc_fade = (getAlpha() * 100) / color.a;

  float red_float = (pc_fade * color.r) / (float)100;

  return std::round(red_float);
}

uint8_t RenderElement::calcColorBlue()
{
  auto pc_fade = 0;

  if (color.a != 0)
    pc_fade = (getAlpha() * 100) / color.a;

  float blue_float = (pc_fade * color.b) / (float)100;
  return std::round(blue_float);
}

uint8_t RenderElement::calcColorGreen()
{
  auto pc_fade = 0;

  if (color.a != 0)
    pc_fade = (getAlpha() * 100) / color.a;

  float green_float = (pc_fade * color.g) / (float)100;
  return std::round(green_float);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool RenderElement::hasShadow()
{
  return shadow;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool RenderElement::isTimedOut()
{
  return (status == RenderStatus::TIMED_OUT);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool RenderElement::update(int cycle_time)
{
  /* Update the remaining time, put it into elapsed time */
  remaining_time -= cycle_time;
  elapsed_time   += cycle_time;

  if (type == RenderType::CYCLING_FADE)
  {
    alpha = 255 * abs(sin(cycle_time * fade_rate));
    alpha = Helpers::setInRange(alpha, alpha_low, alpha_high);
  }
  if (type == RenderType::PLEP)
  {
    if (render_sprite != nullptr)
    {
      render_sprite->update(cycle_time);

      if (render_sprite->getLoops() >= static_cast<uint32_t>(num_loops))
        status = RenderStatus::TIMED_OUT;
    }
  }
  else
  {
    if (remaining_time <= 0)
      status = RenderStatus::TIMED_OUT;
    else
    {
      if (elapsed_time >= fade_in_time)
        status = RenderStatus::DISPLAYING;
      if (remaining_time < fade_out_time)
        status = RenderStatus::FADING_OUT;

      /* Before updating coordinates, update the pixels/s from the pixels/s/s */
      velocity_x += std::round(((float)acceleration_x * cycle_time) / 100);
      velocity_y += std::round(((float)acceleration_y * cycle_time) / 100);

      /* Then, update the (X, Y) coordinates for the text and the shadow */
      auto temp_delta_x = ((float)velocity_x * cycle_time) / 100;
      auto temp_delta_y = ((float)velocity_y * cycle_time) / 100;

      /* If the delta has finally reached an X pixel distance, do an update */
      if (std::abs(delta_x) >= 1.0)
      {
        auto neg_delta_x = std::floor(delta_x);
        delta_x -= neg_delta_x;
        x       += neg_delta_x;

        if (shadow)
          shadow_x += neg_delta_x;
      }
      /* Else, grow the change in X by the temp value */
      else
      {
        delta_x += temp_delta_x;
      }

      /* If the delta has finally reached a Y pixel distance, do an update */
      if (std::abs(delta_y) >= 1.0)
      {
        auto neg_delta_y = std::floor(delta_y);
        delta_y -= neg_delta_y;
        y       += neg_delta_y;

        if (shadow)
          shadow_y += neg_delta_y;
      }
      /* Else, grow the change in Y by the temp value. */
      else
      {
        delta_y += temp_delta_y;
      }
    }
  }

  return true;
}

SDL_Color RenderElement::getColor()
{
  return color;
}

SDL_Color RenderElement::getShadowColor()
{
  return shadow_color;
}

int32_t RenderElement::getElapsedTime()
{
  return elapsed_time;
}

int32_t RenderElement::getFadeInTime()
{
  return fade_in_time;
}

int32_t RenderElement::getFadeOutTime()
{
  return fade_out_time;
}

Frame* RenderElement::getActionFrame()
{
  return action_frame;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint8_t RenderElement::getAlpha()
{
  return alpha;
}

TTF_Font* RenderElement::getFont()
{
  return font;
}

Person* RenderElement::getFlasher()
{
  return flasher;
}

int32_t RenderElement::getNumLoops()
{
  return num_loops;
}

int32_t RenderElement::getRemainingTime()
{
  return remaining_time;
}

Sprite* RenderElement::getSprite()
{
  return render_sprite;
}

RenderStatus RenderElement::getStatus()
{
  return status;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint8_t RenderElement::getShadowAlpha()
{
  return shadow_alpha;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::string RenderElement::getText()
{
  return text;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
RenderType RenderElement::getType()
{
  return type;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t RenderElement::getX()
{
  return x;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t RenderElement::getY()
{
  return y;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t RenderElement::getShadowX()
{
  return shadow_x;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t RenderElement::getShadowY()
{
  return shadow_y;
}

int32_t RenderElement::getSizeX()
{
  return size_x;
}

int32_t RenderElement::getSizeY()
{
  return size_y;
}

void RenderElement::setActionFrame(Frame* action_frame)
{
  this->action_frame = action_frame;
}

void RenderElement::setAlpha(uint8_t new_alpha)
{
  alpha = new_alpha;
  shadow_alpha = new_alpha;
}


/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setAcceleration(int32_t new_acceleration_x,
    int32_t new_acceleration_y)
{
  acceleration_x = new_acceleration_x;
  acceleration_y = new_acceleration_y;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setCoordinates(int32_t new_x, int32_t new_y)
{
  setX(new_x);
  setY(new_y);
}

void RenderElement::setSprite(Sprite* new_render_sprite)
{
  if (type == RenderType::PLEP)
    if (render_sprite != nullptr)
      delete render_sprite;

  render_sprite = new_render_sprite;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setShadowCoordinates(int32_t new_shadow_x,
    int32_t new_shadow_y)
{
  setShadowX(new_shadow_x);
  setShadowY(new_shadow_y);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setColor(SDL_Color new_color)
{
  color = new_color;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setShadowColor(SDL_Color new_shadow_color)
{
  shadow_color = new_shadow_color;
}

void RenderElement::setSizes(int32_t new_size_x, int32_t new_size_y)
{
  size_x = new_size_x;
  size_y = new_size_y;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setFont(TTF_Font* new_font)
{
  font = new_font;
}

void RenderElement::setFlasher(Person* new_flasher)
{
  flasher = new_flasher;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setShadow(bool to_show)
{
  shadow = to_show;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool RenderElement::setTimes(int32_t new_remaining_time, int32_t fade_in,
    int32_t fade_out)
{
  bool valid = new_remaining_time >= 0;
  valid &= fade_in >= 0;
  valid &= fade_out >= 0;
  valid &= fade_in + fade_out <= new_remaining_time;
  valid &= fade_in <= new_remaining_time;
  valid &= fade_out <= new_remaining_time;

  if (valid)
  {
    remaining_time = new_remaining_time;
    fade_in_time   = fade_in;
    fade_out_time  = fade_out;
  }

  return valid;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setText(std::string new_text)
{
  text = new_text;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setVelocity(int32_t new_velocity_x, int32_t new_velocity_y)
{
  velocity_x = new_velocity_x;
  velocity_y = new_velocity_y;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setX(int32_t new_x)
{
  x = new_x;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setY(int32_t new_y)
{
  y = new_y;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setShadowX(int32_t new_shadow_x)
{
  shadow_x = x + new_shadow_x;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void RenderElement::setShadowY(int32_t new_shadow_y)
{
  shadow_y = y + new_shadow_y;
}

void RenderElement::setTimedOut()
{
  status = RenderStatus::TIMED_OUT;
}

void RenderElement::setSizeX(int32_t new_size_x)
{
  size_x = new_size_x;
}

void RenderElement::setSizeY(int32_t new_size_y)
{
  size_y = new_size_y;
}

//TODO
// void Battle::createActionText(std::string action_name)
// {
//   RenderElement *action_text =
//       new RenderElement(RenderType::ACTION_TEXT, 900, 100, 100);

//   action_text->setColor({0, 0, 0, 255});
//   action_text->setShadowColor({kACTION_COLOR_R, 0, 0, 255});
//   action_text->setFont(font_action);
//   action_text->setText(action_name);

//   Text t(font_action);
//   t.setText(renderer, action_text->getText(), action_text->getColor());

//   auto x = (kACTION_TEXT_X - t.getWidth());

//   action_text->setX(x);
//   action_text->setY(kACTION_CENTER - t.getHeight() / 2 - 8);
//   action_text->setShadow();
//   action_text->setShadowCoordinates(kACTION_TEXT_SHADOW, kACTION_TEXT_SHADOW);
//   render_elements.push_back(action_text);
// }

// void Battle::createDamageText(Person *target, std::string text)
// {
//   auto element = createDamageValue(target, 0);
//   element->setText(text);

//   Text t(font_damage);
//   t.setText(renderer, element->getText(), {0, 0, 0, 255});

//   auto x = getPersonX(target);
//   x += kPERSON_WIDTH / 2;
//   x -= t.getWidth() / 2;

//   element->setTimes(550, 115, 115);
//   element->setX(x);
//   element->setShadowX(kACTION_TEXT_SHADOW - 2);
//   element->setVelocity(0, -45);
//   element->setAcceleration(0, 8);
// }

// // TODO: Comment
// RenderElement *Battle::createDamageValue(Person *target, uint32_t amount)
// {
//   /* Determine the color of text to use for displaying according to the
//    * appropriate damage type (based on the type of event being processed ) */
//   SDL_Color color = {0, 0, 0, 255};
//   SDL_Color shadow_color = {255, 255, 255, 255};

//   if(curr_event->type == EventType::STANDARD_DAMAGE)
//     shadow_color = kSTRD_DMG_COLOR;
//   else if(curr_event->type == EventType::CRITICAL_DAMAGE)
//     shadow_color = kCRIT_DMG_COLOR;
//   else if(curr_event->type == EventType::POISON_DAMAGE)
//     shadow_color = kPOIS_DMG_COLOR;
//   else if(curr_event->type == EventType::BURN_DAMAGE)
//     shadow_color = kBURN_DMG_COLOR;

//   RenderElement *element =
//       new RenderElement(RenderType::DAMAGE_VALUE, 475, 65, 100);

//   /* Build parameters for the damage text, add to render text elements */
//   element->setColor(color);
//   element->setFont(font_damage);
//   element->setText(std::to_string(amount));

//   Text t(font_damage);
//   t.setText(renderer, element->getText(), color);

//   auto x = getPersonX(target);
//   x += kPERSON_WIDTH / 2;
//   x -= t.getWidth() / 2;

//   auto y = getPersonY(target);
//   y += t.getHeight() / 2;
//   y += system_options->getScreenHeight() / 13;

//   element->setCoordinates(x, y);
//   element->setShadow(true);
//   element->setShadowCoordinates(kACTION_TEXT_SHADOW - 2,
//                                 kACTION_TEXT_SHADOW - 1);
//   element->setShadowColor(shadow_color);
//   element->setAcceleration(4, 0);
//   element->setVelocity(-15, -25);

//   render_elements.push_back(element);

//   return element;
// }

// void Battle::createDeath(Person *target)
// {
//   RenderElement *element =
//       new RenderElement(RenderType::RGB_SPRITE_DEATH, 3000, 300, 2650);
//   element->setColor({255, 0, 0, 255});
//   element->setCoordinates(getPersonX(target), getPersonY(target));
//   element->setSprite(getPersonSprite(target));
//   element->setFlasher(target);
//   render_elements.push_back(element);
// }

// // TODO: Comment
// void Battle::createRegenValue(Person *target, uint64_t amount)
// {
//   /* Determine the appropriate color to show (whether VITA/QD regen) */
//   SDL_Color color = {0, 0, 0, 255};
//   SDL_Color shadow_color = {255, 255, 255, 255};

//   if(curr_event->type == EventType::REGEN_VITA)
//     shadow_color = kVITA_REGEN_COLOR;
//   else if(curr_event->type == EventType::REGEN_QTDR)
//     shadow_color = kQTDR_REGEN_COLOR;
//   else if(curr_event->type == EventType::HIBERNATION ||
//           curr_event->type == EventType::HEAL_HEALTH)
//     shadow_color = kHIBERNATION_REGEN_COLOR;

//   RenderElement *element =
//       new RenderElement(RenderType::DAMAGE_VALUE, 650, 150, 150);

//   element->setColor(color);
//   element->setFont(font_damage);
//   element->setText(std::to_string(amount));

//   Text t(font_damage);
//   t.setText(renderer, element->getText(), color);

//   auto x = getPersonX(target);
//   x += kPERSON_WIDTH / 2;
//   x -= t.getWidth() / 2;

//   auto y = getPersonY(target);
//   y += t.getHeight() / 2;
//   y += 40;

//   element->setCoordinates(x, y);
//   element->setShadow(true);
//   element->setShadowCoordinates(kACTION_TEXT_SHADOW - 2,
//                                 kACTION_TEXT_SHADOW - 1);
//   element->setShadowColor(shadow_color);
//   element->setVelocity(0, -25);
//   element->setAcceleration(0, 4);

//   render_elements.push_back(element);
// }

// RenderElement *Battle::createPlep(Person *target, Sprite *plep)
// {
//   Sprite *new_plep = new Sprite(*plep);
//   new_plep->setNonUnique(true, plep->getSize());
//   new_plep->createTexture(renderer);

//   RenderElement *plep_render =
//       new RenderElement(new_plep, getPersonX(target), getPersonY(target), 1);
//   render_elements.push_back(plep_render);

//   return plep_render;
// }

// void Battle::createSpriteFlash(Person *target, SDL_Color color,
//                                       int32_t flash_time)
// {
//   auto fade_time = std::floor(flash_time * 3.0 / 7.0);
//   RenderElement *sprite_flash = new RenderElement(
//       RenderType::RGB_SPRITE_FLASH, flash_time, fade_time, fade_time);

//   sprite_flash->setSprite(getPersonSprite(target));
//   sprite_flash->setFlasher(target);
//   sprite_flash->setColor(color);

//   render_elements.push_back(sprite_flash);
// }

