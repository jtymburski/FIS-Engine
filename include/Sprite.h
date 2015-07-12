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

#include <iostream>
#include <vector>

#include "Helpers.h"
#include "Frame.h"
#include "XmlData.h"

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

  /* Copy constructor */
  Sprite(const Sprite &source);

  /* Create a plep sprite */
  Sprite(bool plep, int32_t size);
  
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
  double brightness;
  
  /* Color values to indicate the composition of the image */
  uint8_t temp_red;
  uint8_t temp_green;
  uint8_t temp_blue;
  uint8_t color_red;
  uint8_t color_green;
  uint8_t color_blue;
  
  /* The current frame */
  Frame* current;

  /* Grey scale control information */
  uint8_t grey_scale_alpha;
  bool grey_scale_update;
  
  /* The first frame */
  Frame* head;

  /* The sprite ID, useful when keeping track of a large number of sprites */
  uint16_t id;

  /* Set the sprite to signify it is flashing (revert color balance) */
  bool flashing;

  /* The number of loops the Sprite has gone through */
  uint32_t loops;

  /* Sets the opacity of the rendered sprite */
  uint8_t opacity;

  /* Is this Sprite a plep? */
  bool plep;
  
  /* The rotation angle for rendering */
  float rotation_angle;
  
  /* The number of frames */
  int size;

  /* Direction */
  Sequencer sequence;
  
  /* The texture with the presently displayed frame + modifications */
  SDL_Texture* texture;
  bool texture_update;
  
  /*------------------- Constants -----------------------*/
  const static uint16_t kDEFAULT_ANIMATE_TIME; /* The default animation time */
  const static float kDEFAULT_BRIGHTNESS; /* the default brightness value */
  const static uint8_t kDEFAULT_COLOR; /* the default color rating */
  const static uint8_t kDEFAULT_OPACITY; /* the default rendered alpha */
  const static uint8_t kDELTA_GREY_SCALE; /* Delta grey scale alpha */
  const static uint8_t kDOUBLE_DIGITS; /* the borderline to double digits */
  const static float kMAX_BRIGHTNESS; /* The max brightness value */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Returns the angle, if one exists in the list of modifications */
  uint16_t parseAdjustments(std::vector<std::string> adjustments);
  
  /* Sets the color modification with the texture */
  void setColorMod();

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
  /* Contains the destructor implementation (so children can call it) */
  void clear();

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const Sprite &source);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds sprite information from the XML data classifier from the file */
  bool addFileInformation(XmlData data, int index, SDL_Renderer* renderer, 
                          std::string base_path = "", bool no_warnings = false);
 
    /* Creates a texture for a sprite (for Pleps) */
  void createTexture(SDL_Renderer* renderer);
  
  /* Executes the necessary image adjustments, to all frames */
  bool execImageAdjustments(std::vector<std::string> adjustments);

  /* Returns the total animation time between frame changes */
  short getAnimationTime() const;
  
  /* Gets the brightness (0-0.99: darker, 1.0: same, 1.0+: brighter) */
  double getBrightness() const;
  
  /* Gets the color balance in the spectrum */
  uint8_t getColorBlue() const;
  uint8_t getColorGreen() const;
  uint8_t getColorRed() const;
  
  /* Gets the current frame */
  Frame* getCurrent();

  /* Gets the current frame and then shifts to the next one */
  Frame* getCurrentAndShift();

  /* Returns the head frame */
  Frame* getFirstFrame();
 
  /* Returns the ID */
  uint16_t getId() const;

  /* Get the number of loops the sprite has completed */
  uint32_t getLoops();
  
  /* Returns the opacity that it is rendered at */
  uint8_t getOpacity() const;
  
  /* Returns the position that the linked list pointer is at */
  int getPosition();

  /* Returns the rotation angle for rendering */
  float getRotation() const;
  
  /* Returns the size of the sequence */
  int getSize() const;

  /* Gets the temporary color balance spectrum values */
  uint8_t getTempColorBlue() const;
  uint8_t getTempColorGreen() const;
  uint8_t getTempColorRed() const;
  
  /* Inserts the image into the sprite sequence at the given position */
  Frame* insert(std::string path, SDL_Renderer* renderer, int position, 
                uint16_t angle = 0, bool no_warnings = false);
  
  /* Inserts the first image if the frame sequence is empty
   * Note: This isn't for inserting the head, just the first one */
  Frame* insertFirst(std::string path, SDL_Renderer* renderer, 
                     uint16_t angle = 0, bool no_warnings = false);
  
  /* This inserts all the given frames at the tail. If there are any "|" 
   * delimiters, it splits the path and adds the sequence */
  std::vector<Frame*> insertFrames(std::string path, SDL_Renderer* renderer, 
                                   uint16_t angle = 0, 
                                   bool no_warnings = false);
  
  /* Inserts a sequence of images that are stored. This allows for 
   * quick insertion of stored frames
   * For example: head_path = ":/animation/image_"
   *              count = 5
   *              file_type = ".png"
   *   This will allow for image_00.png -> image_04.png to be added into
   *   a sequence */
  std::vector<Frame*> insertSequence(std::string head_path, int count, 
                                     std::string tail_path, 
                                     SDL_Renderer* renderer, uint16_t angle = 0,
                                     bool no_warnings = false);
  
  /* Inserts the image at the end of the sprite sequence */
  Frame* insertTail(std::string path, SDL_Renderer* renderer, 
                    uint16_t angle = 0, bool no_warnings = false);
  
  /* Returns if the linked list pointer is at the head or at the tail */
  bool isAtFirst();
  bool isAtEnd();

  /* Returns if the direction parsing the frames is forward */
  bool isDirectionForward() const;

  /* Whether the sprite is flashing */
  bool isFlashing() const;

  /* Check if there are set frames in the sprite */
  bool isFramesSet() const;

  /* Returns if the grey scale mode is active */
  bool isGreyScale();
  
  /* Removes the frame in the sequence at the given position */
  bool remove(int position);

  /* Removes all the frames in the sequence */
  bool removeAll();

  /* Removes the last frame in the sequence */
  bool removeTail();

  /* Reset the number of loops for the sprite */
  void resetLoops();

  /* Render the texture to the given renderer with the given parameters */
  bool render(SDL_Renderer* renderer, int x = 0, int y = 0, 
                                      int w = 0, int h = 0);

  /* Take the temp. stored color balance values and restore them */
  void revertColorBalance();

  /* Sets the frame animation time (in ms) */
  void setAnimationTime(uint16_t time);
  
  /* Asserts that the current pointer in the linked list is at the head */
  bool setAtFirst();
  
  /* Sets the brightness (0-0.99: darker, 1.0: same, 1.0+: brighter) */
  bool setBrightness(double brightness);
  
  /* Sets the color balance of the sprite */
  void setColorBalance(uint8_t red, uint8_t green, uint8_t blue);
  void setColorBlue(uint8_t color);
  void setColorGreen(uint8_t color);
  void setColorRed(uint8_t color);
  
  /* Asserts the direction is forward for when accessing the linked list */
  bool setDirectionForward();

  /* Asserts the direction is reverse for when accessing the linked list */
  bool setDirectionReverse();

  /* Assigns a head frame */
  void setHead(Frame* head);

  /* Sets the numerical identifier */
  void setId(uint16_t id);

  /* Set whether the sprite is flashing */
  void setFlashing(bool flashing);
  
  /* Sets the opacity rating */
  void setOpacity(uint8_t opacity);
  
  /* Sets the rotation for all frames to be rendered at */
  void setRotation(float angle);
  void setRotation(int angle);

  /* Sets the temporary color balances for the sprite */
  void setTempColorBalance(uint8_t temp_red, uint8_t new_green, uint8_t new_blue);
  void setTempColorRed(uint8_t temp_red);
  void setTempColorGreen(uint8_t temp_green);
  void setTempColorBlue(uint8_t temp_blue);

  /* Shifts to the given position in the sequence */
  bool shift(int position);

  /* Shifts to the next frame in the sprite */
  bool shiftNext(bool skip_head = false);

  /* Switches the direction that the linked list is parsed in */
  bool switchDirection();

  /* Updates the frames within the sprite */
  bool update(int cycle_time, bool skip_head = false);

  /* Sets if the greyscale texture is active and returned on getTexture() */
  bool useGreyScale(bool enable);
  
/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  Sprite& operator= (const Sprite &source);

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
