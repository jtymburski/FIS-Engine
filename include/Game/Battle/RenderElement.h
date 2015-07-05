/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
* Inheritance: None
* Description: RenderElement is an abstraction of a object in the battle
*              which requires updating/rendering. This file holds data
*              to aid in choosing where to render these elements. (ex. velocity,
*              acceleration, fading in/out etc.)
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#ifndef RENDERELEMENT_H
#define RENDERELEMENT_H

#include "Text.h"
#include "Sprite.h"
#include "Helpers.h"

enum class RenderType : std::uint16_t
{
  NONE         =  0, 
  ACTION_TEXT  =  1,
  DAMAGE_VALUE =  2,
};

class RenderElement
{
public:
  /* Constructors for RenderElements */
  RenderElement();
  RenderElement(RenderType render_type);

  /* Annihilates a render element */
  ~RenderElement() = default;

private: 
  /* Type of the Render */
  RenderType type;

  /* Color of the element (+ shadow color, if needed) */
  SDL_Color color;
  SDL_Color shadow_color;

  /* The font and string text */
  TTF_Font* font;
  std::string text;

  /* Whether to show shadow or not */
  bool shadow;

  /* Remaining time and elapsed time of rendering */
  int32_t remaining_time;
  int32_t elapsed_time;

  /* Acceleration of text parameters */
  int16_t acceleration_x;
  int16_t acceleration_y;

  float delta_x;
  float delta_y;

  /* Coordinates for rendering */
  int16_t x;
  int16_t y;
  int16_t shadow_x;
  int16_t shadow_y;
  int16_t velocity_x;
  int16_t velocity_y;
  int32_t fade_in_time;
  int32_t fade_out_time;

  /* Current alpha value calculations */
  int8_t alpha;
  int8_t shadow_alpha;

  /* ------------ Constants --------------- */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns whether the element has a shadow */
  bool hasShadow();

  /* Returns true if the element has timed out (needs to be cleared ) */
  bool isTimedOut();

  /* Updates the position and info of the render element */
  bool update(int32_t cycle_time);

  /* Returns the alphs values for text and shadow text */
  int32_t getAlpha();
  int32_t getShadowAlpha();

  /* Methods to return the colors */
  SDL_Color getColor();
  SDL_Color getShadowColor();

  /* Returns a pointer to the font of the render element */
  TTF_Font* getFont();

  /* Returns the assigned string for the render element */
  std::string getText();

  /* Returns the type of the render element */
  RenderType getType();

  /* (X/Y) coordinates for the text */
  int32_t getX();
  int32_t getY();
  int32_t getShadowX();
  int32_t getShadowY();

  /* Assigns new acceleration values */
  void setAcceleration(int32_t new_acceleration_x, int32_t new_acceleration_y);

  /* Assigns coordinates for the text and shadow text */
  void setCoordinates(int32_t new_x, int32_t new_y);
  void setShadowCoordinates(int32_t new_shadow_x, int32_t new_shadow_y);

  /* Assigns a new color for the text and shadow text */
  void setColor(SDL_Color new_color);
  void setShadowColor(SDL_Color new_shadow_color);

  /* Sets the font of the text */
  void setFont(TTF_Font* new_font);

  /* Method to set the text to have a shadow */
  void setShadow(bool to_show = true);

  /* Assigns the time values for the RenderElement */
  void setTimes(int32_t new_remaining_time, int32_t fade_in = 0, 
      int32_t fade_out = 0);

  /* Assigns the string value for the text */
  void setText(std::string new_text);

  /* Assigns new velocity values */
  void setVelocity(int32_t new_velocity_x, int32_t new_velocity_y);

  /* Regular rendering coordinates */
  void setX(int32_t new_x);
  void setY(int32_t new_y);

  /* Shadow (for text renders) offset X,Y values */
  void setShadowX(int32_t new_shadow_x);
  void setShadowY(int32_t new_shadow_y);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:

};

#endif //CLASSNAME_H
