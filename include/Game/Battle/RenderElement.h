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
#include "Game/Player/Person.h"

enum class RenderType : std::uint32_t
{
  NONE             =  0, 
  ACTION_TEXT      =  1,
  ACTION_FRAME     =  2,
  DAMAGE_VALUE     =  3,
  RGB_OVERLAY      =  4,
  RGB_SPRITE_FLASH =  5,
  // RGB_SPRITE_DEATH =  6,
  PLEP             =  7
};

enum class RenderStatus : std::uint32_t
{
  CONSTRUCTING = 1,
  FADING_IN    = 2,
  DISPLAYING   = 3,
  FADING_OUT   = 4,
  BOBBING      = 5,
  TIMED_OUT    = 6
};

class RenderElement
{
public:
  /* Constructors for RenderElements */
  RenderElement();
  RenderElement(RenderType type, int32_t remaining_time = 0, 
      int32_t fade_in_time = 0, int32_t fade_out_time = 0);

  /* Create a Plep */
  RenderElement(Sprite* plep, int32_t x, int32_t y, int32_t num_loops);

  /* Annihilates a render element */
  ~RenderElement();

private: 
  /* Pointer to a Frame */
  Frame* action_frame;
  
  /* Pointer to a Sprite (plep) (or the sprite to be mixed with Color) */
  Sprite* render_sprite;

  /* Person to flash sprite for */
  Person* flasher;

  /* Enumerated status of the Render Element */
  RenderStatus status;

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
  int32_t num_loops;

  /* Acceleration of text parameters */
  int32_t acceleration_x;
  int32_t acceleration_y;

  /* Change in X/Y which has not been rendered yet */
  float delta_x;
  float delta_y;

  /* Coordinates for rendering */
  int32_t x;
  int32_t y;
  int32_t shadow_x;
  int32_t shadow_y;
  int32_t size_x;
  int32_t size_y;
  int32_t velocity_x;
  int32_t velocity_y;
  int32_t fade_in_time;
  int32_t fade_out_time;

  /* Current alpha value calculations */
  uint8_t alpha;
  uint8_t shadow_alpha;

  /* ------------ Constants --------------- */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *========================================f====================================*/
public:
  /* Calculates current colour levels for sprite-overlay flashing */
  uint8_t calcColorRed();
  uint8_t calcColorGreen();
  uint8_t calcColorBlue();

  /* Returns whether the element has a shadow */
  bool hasShadow();

  /* Returns true if the element has timed out (needs to be cleared ) */
  bool isTimedOut();

  /* Updates the position and info of the render element */
  bool update(int32_t cycle_time);

  /* Returns the pointer to the action frame */
  Frame* getActionFrame();

  /* Returns the alphs values for text and shadow text */
  uint8_t getAlpha();
  uint8_t getShadowAlpha();

  /* Methods to return the colors */
  SDL_Color getColor();
  SDL_Color getShadowColor();

  int32_t getElapsedTime();

  int32_t getFadeInTime();

  int32_t getFadeOutTime();

  /* Returns a pointer to the font of the render element */
  TTF_Font* getFont();

  /* Obtains pointer to the person to be flashed */
  Person* getFlasher();

  int32_t getNumLoops();

  int32_t getRemainingTime();

  /* Returns a pointer to the Sprite Plep */
  Sprite* getSprite();

  /* Return the enumerated status for the render element */
  RenderStatus getStatus();

  /* Returns the assigned string for the render element */
  std::string getText();

  /* Returns the type of the render element */
  RenderType getType();

  /* (X/Y) coordinates for the text */
  int32_t getX();
  int32_t getY();
  int32_t getShadowX();
  int32_t getShadowY();
  int32_t getSizeX();
  int32_t getSizeY();

  void setActionFrame(Frame* action_frame);

  void setAlpha(uint8_t new_alpha);

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

  /* Assign a person to the render element */
  void setFlasher(Person* new_flasher);

  /* Assigns a new plep for the Element */
  void setSprite(Sprite* new_render_sprite);

  /* Method to set the text to have a shadow */
  void setShadow(bool to_show = true);

  /* Assign both 'x' and 'y' sizes for the RenderElement */
  void setSizes(int32_t new_size_x, int32_t new_size_y);

  /* Assigns the time values for the RenderElement */
  bool setTimes(int32_t new_remaining_time, int32_t fade_in = 0, 
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

  /* Assign a new value for the sign of the Element (only some objects) */
  void setSizeX(int32_t new_size_x);
  void setSizeY(int32_t new_size_y);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:

};

#endif //CLASSNAME_H
