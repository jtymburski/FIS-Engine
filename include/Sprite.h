/*******************************************************************************
 * Class Name: Sprite
 * Date Created: Oct 28, 2012
 * Inheritance: none
 * Description: The Sprite class. This handles the linked list control that 
 *              wraps the Frame. This will allow for a sequence of events, 
 *              that emulate a GIF for animation or just store one image. This
 *              class also has the functionality for modded rendering through
 *              the SDL engine. The list of these mods are brightening or
 *              darkening, opacity, and color tuning.
 ******************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include <vector>

#include "Helpers.h"
#include "Frame.h"

class Sprite
{
public:
  /* Constructor: Set up no images */
  Sprite();

  /* Constructor: Set up one image */
  Sprite(std::string path, SDL_Renderer* renderer);

  /* Constructor: Set up sequence of images */
  Sprite(std::string head_path, int num_frames, 
         std::string tail_path, SDL_Renderer* renderer);

  /* Destructor function */
  ~Sprite();

  /* Direction to pass through the sequence of frames */
  enum Sequencer{REVERSE, FORWARD};

  /* NONE - the object isn't rotated
   * CLOCKWISE - rotate the object 90 degrees clockwise
   * COUNTERCLOCKWISE - rotate the object 90 degrees counterclockwise
   * FLIP - rotate the object 180 degrees */
  enum RotatedAngle{NONE, CLOCKWISE, COUNTERCLOCKWISE, HALFCIRCLE};

private:
  /* Animation time */
  uint16_t animation_time;
  uint16_t elapsed_time;
  
  /* The stored brightness for rendering */
  float brightness;
  
  /* Color values to indicate the composition of the image */
  uint8_t color_red;
  uint8_t color_green;
  uint8_t color_blue;
  
  /* The current frame */
  Frame* current;

  /* The first frame */
  Frame* head;

  /* How the SDL texture should be flipped while rendering */
  SDL_RendererFlip flip;

  /* Sets the opacity of the rendered sprite */
  uint8_t opacity;
  
  /* The rotation angle for rendering */
  int rotation_angle;
  
  /* The number of frames */
  int size;

  /* Direction */
  Sequencer sequence;
  
  /* The texture with the presently displayed frame + modifications */
  SDL_Texture* texture;
  bool texture_update;
  
  /* Sets the white mask that can be used for modifying the brightness */
  SDL_Texture* white_mask;
  
  /*------------------- Constants -----------------------*/
  const static uint16_t kDEFAULT_ANIMATE_TIME; /* The default animation time */
  const static float kDEFAULT_BRIGHTNESS; /* the default brightness value */
  const static uint8_t kDEFAULT_COLOR; /* the default color rating */
  const static uint8_t kDEFAULT_OPACITY; /* the default rendered alpha */
  const static uint8_t kDOUBLE_DIGITS; /* the borderline to double digits */
  const static float kMAX_BRIGHTNESS; /* The max brightness value */
  const static uint16_t kUNSET_ANIMATE_TIME; /* The unset animate time value */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Sets the color modification with the texture */
  void setColorMod();
  
/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Executes the necessary image adjustments, as per the file data handlers */
  bool execImageAdjustment(std::string adjustment);
  bool execImageAdjustments(std::vector<std::string> adjustments);
  
  /* Flips the sprite SDL image - either horizontal or vertical */
  void flipHorizontal(bool flip = true);
  void flipVertical(bool flip = true);
  
  /* Returns the total animation time between frame changes */
  short getAnimationTime();
  
  /* Gets the brightness (0-0.99: darker, 1.0: same, 1.0+: brighter) */
  float getBrightness();
  
  /* Gets the current frame */
  Frame* getCurrent();

  /* Gets the current frame and then shifts to the next one */
  Frame* getCurrentAndShift();

  /* Returns the head frame */
  Frame* getFirstFrame();
 
  /* Returns the position that the linked list pointer is at */
  int getPosition();

  /* Returns the size of the sequence */
  int getSize();
  
  /* Inserts the image into the sprite sequence at the given position */
  bool insert(std::string path, SDL_Renderer* renderer, int position);

  /* Inserts the first image if the frame sequence is empty
   * Note: This isn't for inserting the head, just the first one */
  bool insertFirst(std::string path, SDL_Renderer* renderer);

  /* Inserts a sequence of images that are stored. This allows for 
   * quick insertion of stored frames
   * For example: head_path = ":/animation/image_"
   *              num_frames = 5
   *              file_type = ".png"
   *   This will allow for image_00.png -> image_04.png to be added into
   *   a sequence */
  bool insertSequence(std::string head_path, int num_frames, 
                      std::string tail_path, SDL_Renderer* renderer);

  /* Inserts the image at the end of the sprite sequence */
  bool insertTail(std::string path, SDL_Renderer* renderer);

  /* Returns if the linked list pointer is at the head or at the tail */
  bool isAtFirst();
  bool isAtEnd();

  /* Returns if the direction parsing the frames is forward */
  bool isDirectionForward();

  /* Removes the frame in the sequence at the given position */
  bool remove(int position);

  /* Removes all the frames in the sequence */
  bool removeAll();

  /* Removes the last frame in the sequence */
  bool removeTail();

  /* Render the texture to the given renderer with the given parameters */
  bool render(SDL_Renderer* renderer, int x = 0, int y = 0, 
                                      int h = 0, int w = 0);

  /* Sets the frame animation time (in ms) */
  void setAnimationTime(short time);
  
  /* Asserts that the current pointer in the linked list is at the head */
  bool setAtFirst();
  
  /* Sets the brightness (0-0.99: darker, 1.0: same, 1.0+: brighter) */
  bool setBrightness(float brightness);
  
  /* Sets the color balance of the sprite */
  void setColorBalance(uint8_t red, uint8_t green, uint8_t blue);
  
  /* Asserts the direction is forward for when accessing the linked list */
  bool setDirectionForward();

  /* Asserts the direction is reverse for when accessing the linked list */
  bool setDirectionReverse();

  /* Sets the opacity rating */
  void setOpacity(uint8_t opacity);
  
  /* Sets the rotation for all frames to be rendered at */
  void setRotation(int angle);
  
  /* Sets the white mask texture, for tuning brightness */
  bool setWhiteMask(SDL_Texture* texture);
  
  /* Shifts to the given position in the sequence */
  bool shift(int position);

  /* Shifts to the next frame in the sprite */
  bool shiftNext(bool skipHead = false);

  /* Switches the direction that the linked list is parsed in */
  bool switchDirection();

  /* Updates the frames within the sprite */
  void updateSprite(int cycle_time, SDL_Renderer* renderer);
  
/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the degrees of the string identifier */
  static int getAngle(std::string identifier);

  /* Returns the degrees of the angle enumerator */
  static int getAngle(RotatedAngle angle);
};

#endif // SPRITE_H
