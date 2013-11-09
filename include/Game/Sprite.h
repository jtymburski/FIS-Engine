/******************************************************************************
* Class Name: Sprite
* Date Created: Oct 28, 2012
* Inheritance: none
* Description: The Sprite class. This handles the linked list control that 
*              wraps the Frame. This will allow for a sequence of events, 
*              that emulate a GIF for animation or just store one image. This
*              class also has the functionality for direction GL painting 
*              through the native API.
******************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

//#include <QDebug>

#include "Game/Frame.h"

class Sprite
{
public:
  /* Constructor: Set up no images */
  Sprite();

  /* Constructor: Set up one image */
  Sprite(QString image_path, int rotate_angle = 0);

  /* Constructor: Set up sequence of images */
  Sprite(QString head_path, int num_frames, 
         QString tail_path, int rotate_angle = 0);

  /* Destructor function */
  ~Sprite();

  /* Direction to pass through the sequence of frames */
  enum Direction{ REVERSE, FORWARD };

  /* NONE - the object isn't rotated
   * CLOCKWISE - rotate the object 90 degrees clockwise
   * COUNTERCLOCKWISE - rotate the object 90 degrees counterclockwise
   * FLIP - rotate the object 180 degrees */
  enum RotatedAngle{NONE, CLOCKWISE, COUNTERCLOCKWISE, HALFCIRCLE};

private:
  /* The stored brightness for rendering */
  float brightness;
  
  /* The current frame */
  Frame* current;

  /* The first frame */
  Frame* head;

  /* The number of frames */
  int size;

  /* Direction */
  Direction sequence;
  
  /* Animation time */
  short animation_time;
  short elapsed_time;
  
  /*------------------- Constants -----------------------*/
  const static short kDEFAULT_ANIMATE_TIME; /* The default animation time */
  const static float kDEFAULT_BRIGHTNESS; /* the default brightness value */
  const static short kDOUBLE_DIGITS; /* the borderline to double digits */
  const static short kUNSET_ANIMATE_TIME; /* The unset animate time value */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Executes the necessary image adjustments, as per the file data handlers */
  bool execImageAdjustment(QString adjustment);
  bool execImageAdjustments(QList<QString> adjustments);
  
  /* Flips all the images in each frame */
  bool flipAll(bool horizontal = true, bool vertical = false);

  /* Returns the total animation time between frame changes */
  short getAnimationTime();
  
  /* Gets the brightness (0-0.99: darker, 1.0: same, 1.0+: brighter) */
  float getBrightness();
  
  /* Gets the current frame */
  QPixmap getCurrent();

  /* Gets the current frame and then shifts to the next one */
  QPixmap getCurrentAndShift();

  /* Returns the head frame */
  Frame* getFirstFrame();
 
  /* Returns the position that the linked list pointer is at */
  int getPosition();

  /* Returns the size of the sequence */
  int getSize();

  /* Initializes GL in all the frames stored within this sprite */
  bool initializeGl();
  
  /* Inserts the image into the sprite sequence at the given position */
  bool insert(QString image_path, int position, int rotate_angle = 0);

  /* Inserts the first image if the frame sequence is empty
   * Note: This isn't for inserting the head, just the first one */
  bool insertFirst(QString image_path, int rotate_angle = 0);

  /* Inserts a sequence of images that are stored. This allows for 
   * quick insertion of stored frames
   * For example: head_path = ":/animation/image_"
   *              num_frames = 5
   *              file_type = ".png"
   *   This will allow for image_00.png -> image_04.png to be added into
   *   a sequence */
  bool insertSequence(QString head_path, int num_frames, 
                      QString tail_path, int rotate_angle = 0);

  /* Inserts the image at the end of the sprite sequence */
  bool insertTail(QString image_path, int rotate_angle = 0);

  /* Returns if the linked list pointer is at the head or at the tail */
  bool isAtFirst();
  bool isAtEnd();

  /* Returns if the direction parsing the frames is forward */
  bool isDirectionForward();

  /* Paints the active frame using GL direct calls */
  bool paintGl(float x, float y, int width, int height, float opacity);

  /* Removes the frame in the sequence at the given position */
  bool remove(int position);

  /* Removes all the frames in the sequence */
  bool removeAll();

  /* Removes the last frame in the sequence */
  bool removeTail();

  /* Rotates all the frames within this sprite a specific angle */
  bool rotateAll(int angle);

  /* Sets the frame animation time (in ms) */
  void setAnimationTime(short time);
  
  /* Sets the brightness (0-0.99: darker, 1.0: same, 1.0+: brighter) */
  bool setBrightness(float brightness);
  
  /* Asserts the direction is forward for when accessing the linked list */
  bool setDirectionForward();

  /* Asserts the direction is reverse for when accessing the linked list */
  bool setDirectionReverse();

  /* Asserts that the current pointer in the linked list is at the head */
  bool setAtFirst();
  
  /* Shifts to the given position in the sequence */
  bool shift(int position);

  /* Shifts to the next frame in the sprite */
  bool shiftNext(bool skipHead = false);

  /* Switches the direction that the linked list is parsed in */
  bool switchDirection();

  /* Updates the frames within the sprite */
  void updateSprite(int cycle_time);
  
/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the degrees of the string identifier */
  static int getAngle(QString identifier);

  /* Returns the degrees of the angle enumerator */
  static int getAngle(RotatedAngle angle);
};

#endif // SPRITE_H
