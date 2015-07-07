/******************************************************************************
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
    : type{RenderType::DAMAGE_VALUE}
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
    , x{0}
    , y{0}
    , shadow_x{0}
    , shadow_y{0}
    , velocity_x{0}
    , velocity_y{0}
    , fade_in_time{0}
    , fade_out_time{0}
    , alpha{0}
    , shadow_alpha{0}
{}

/*
 * Description:
 *
 * Inputs:
 */
RenderElement::RenderElement(RenderType render_type) 
    : RenderElement()
{
  type = render_type;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

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
  return (remaining_time < 0);
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

  /* Before updating coordinates, update the pixels/s from the pixels/s/s */
  velocity_x += acceleration_x;
  velocity_y += acceleration_y;

  /* Then, update the (X, Y) coordinates for the text and the shadow */
  auto temp_delta_x = 0.0;
  auto temp_delta_y = 0.0;

  temp_delta_x += static_cast<float>(velocity_x) / 100.0f;
  temp_delta_y += static_cast<float>(velocity_y) / 100.0f;

  if (delta_x >= 1.0 || delta_x <= -1.0)
  {
    x += std::floor(delta_x);

    if (shadow)
      shadow_x += std::floor(delta_x);

    delta_x = 0;
  }
  else
  {
    delta_x += temp_delta_x;
  }

  if (delta_y >= 1.0 || delta_y <= -1.0)
  {
    y += std::floor(delta_y);

    if (shadow)
      shadow_y += std::floor(delta_y);

    delta_y = 0;
  }
  else
  {
    delta_y += temp_delta_y;
  }

  if (elapsed_time >= fade_in_time && remaining_time >= fade_out_time)
  {
    alpha = color.a;
  }
  else if (elapsed_time < fade_in_time)
  {
    std::cout << "Color: A" << (int)color.a << std::endl;
    double time_per_alpha = (double)fade_in_time / color.a;
    alpha = std::floor(time_per_alpha * elapsed_time);
  }
  else if (remaining_time < fade_out_time)
  {
    double time_per_alpha = (double)fade_out_time / color.a;
    auto time_diff = fade_out_time - remaining_time;
    alpha = color.a - std::floor(time_per_alpha * time_diff);
  }

  shadow_alpha = alpha;

  std::cout << "Remaining Time: " << remaining_time << std::endl;
  std::cout << "Elapsed Time: "   << elapsed_time << std::endl;
  std::cout << "Alpha: "          << alpha << std::endl;

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


/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t RenderElement::getAlpha()
{
  return alpha;
}

TTF_Font* RenderElement::getFont()
{
  return font;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t RenderElement::getShadowAlpha()
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
void RenderElement::setTimes(int32_t new_remaining_time, int32_t fade_in, 
    int32_t fade_out)
{
  remaining_time = new_remaining_time;
  fade_in_time   = fade_in;
  fade_out_time  = fade_out;
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

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/