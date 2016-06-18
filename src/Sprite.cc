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
 *
 * TODO:
 *   1. Sound added for particular sprite (category based)
*******************************************************************************/
#include "Sprite.h"

/*=============================================================================
 * SPRITE CONSTANTS -- See .h file for details
 *============================================================================*/

const uint16_t Sprite::kDEFAULT_ANIMATE_TIME = 250;
const float Sprite::kDEFAULT_BRIGHTNESS = 1.0;
const uint8_t Sprite::kDEFAULT_COLOR = 255;
const uint8_t Sprite::kDEFAULT_OPACITY = 255;
const uint8_t Sprite::kDELTA_GREY_SCALE = 2;
const uint8_t Sprite::kDOUBLE_DIGITS = 10;
const double Sprite::kMAX_BRIGHTNESS = 2.0;
const int32_t Sprite::kUNSET_SOUND_ID = -1;

/*=============================================================================
 * SPRITE CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructor function - Set up no frames
 *
 * Input: none
 */
Sprite::Sprite()
    : animation_time{Sprite::kDEFAULT_ANIMATE_TIME},
      brightness{Sprite::kDEFAULT_BRIGHTNESS},
      built_texture{false},
      color_alpha{0},
      color_mode{ColorMode::COLOR},
      color_temp_red{Sprite::kDEFAULT_COLOR},
      color_temp_green{Sprite::kDEFAULT_COLOR},
      color_temp_blue{Sprite::kDEFAULT_COLOR},
      color_red{Sprite::kDEFAULT_COLOR},
      color_green{Sprite::kDEFAULT_COLOR},
      color_blue{Sprite::kDEFAULT_COLOR},
      data{},
      elapsed_time{0},
      loops{0},
      non_unique{false},
      opacity{Sprite::kDEFAULT_OPACITY},
      rotation_angle{0},
      sequence{FORWARD},
      size{0},
      current{nullptr},
      head{nullptr},
      id{0},
      sound_id{kUNSET_SOUND_ID},
      src_rect_use{false},
      texture{nullptr},
      texture_update{false}
{
  src_rect = {0, 0, 0, 0};
}

/*
 * Description: Constructor function - Set up one frame, using the string path
 *              with an integer rotated angle.
 *
 * Input: std::string image_path - image path to set as one sprite
 *        SDL_Renderer* renderer - the rendering engine for creating the image
 */
Sprite::Sprite(std::string path, SDL_Renderer* renderer) : Sprite()
{
  insertFirst(path, renderer);
  built_texture = true;
}

/*
 * Description: Constructor function - Set up sequence of frames.
 *
 * Inputs: std::string head_path - the start part of the path
 *         int num_frames - the number of frames in this path sequence
 *         std::string tail_path - the end of the path, after the count index
 *         SDL_Renderer* renderer - the rendering engine for creating the images
 */
Sprite::Sprite(std::string head_path, int num_frames, std::string tail_path,
               SDL_Renderer* renderer)
    : Sprite()
{
  insertSequence(head_path, num_frames, tail_path, renderer);
  built_texture = true;
}

/*
 * Description: The copy constructor to transfer all non-graphical parameters
 *              from the master sprite to this one.
 *
 * Inputs: const Sprite &source - the source sprite data to copy over
 */
Sprite::Sprite(const Sprite& source) : Sprite()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
Sprite::~Sprite()
{
  clear();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Parses the string sequence of angle adjustments. Returns the
 *              first angle that is greater than 0 (the only used one),
 *              otherwise it returns 0.
 *
 * Inputs: std::vector<std::string> adjustments - the string sequence of
 *                                                angle adjustments
 * Output: uint16_t - the unsigned angle
 */
uint16_t Sprite::parseAdjustments(std::vector<std::string> adjustments)
{
  uint16_t angle = 0;

  /* Run through all the adjustments */
  for(uint16_t i = 0; i < adjustments.size(); i++)
  {
    angle = getAngle(adjustments[i]);
    if(angle > 0)
      return angle;
  }

  return angle;
}

/* Description: Sets the texture color modification on the sprite texture. This
 *              is based on the internal stored red, green, blue values which
 *              can be changed using setColorBalance().
 *
 * Inputs: none
 * Output: none
 */
void Sprite::setColorMod()
{
  if(brightness < kDEFAULT_BRIGHTNESS)
  {
    SDL_SetTextureColorMod(texture, brightness * color_red,
                           brightness * color_green, brightness * color_blue);
  }
  else
  {
    SDL_SetTextureColorMod(texture, color_red, color_green, color_blue);
  }
}

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*
 * Description: Clears out all sprite info that were initialized into this
 *              class. This includes the appropriate procedure of cleaning
 *              up and deleting all the pointers / rendering references.
 *
 * Inputs: none
 * Output: none
 */
void Sprite::clear()
{
  /* Delete all class data */
  if(!non_unique)
    removeAll();
  SDL_DestroyTexture(texture);

  /* Reset variables back to blank */
  current = nullptr;
  head = nullptr;
  texture = nullptr;
}

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Note: createTexture(SDL_Renderer*) must be called after this if you want
 *       to manipulate this sprite further.
 *
 * Inputs: const Sprite &source - the reference sprite class
 * Output: none
 */
void Sprite::copySelf(const Sprite& source)
{
  setAnimationTime(source.getAnimationTime());
  setBrightness(source.getBrightness());

  setColorBalance(source.getColorRed(), source.getColorGreen(),
                  source.getColorBlue());
  setTempColorBalance(source.getTempColorRed(), source.getTempColorGreen(),
                      source.getTempColorBlue());

  data = source.data;
  elapsed_time = source.elapsed_time;

  if(source.isDirectionForward())
    setDirectionForward();
  else
    setDirectionReverse();

  setRotation(source.getRotation());
  setHead(source.getFirstFrame());

  setOpacity(source.getOpacity());
  setRotation(source.getRotation());
  setSoundID(source.getSoundID());
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Add the file data information, based on the xml data pointer
 *              retrieved from the file handler during an XML read. The internal
 *              elements get offset based on the given index.
 *
 * Inputs: XmlData data - the xml data storage class
 *         int index - the element offset index to the sprite data
 *         SDL_Renderer* renderer - the rendering engine to add the info
 *         std::string base_path - the base path for resources
 *         bool no_warnings - should warnings not fire? default false.
 * Output: bool - true if the add was successful
 */
bool Sprite::addFileInformation(XmlData data, int index, SDL_Renderer* renderer,
                                std::string base_path, bool no_warnings,
                                bool build_data)
{
  std::string element = data.getElement(index);
  bool success = true;

  /* Splits the element, for underlying categorization */
  std::vector<std::string> split_element = Helpers::split(element, '_');

  /* Parse the sprite information - based on the element tag name */
  if(element == "animation")
    setAnimationTime(data.getDataInteger());
  else if(element == "brightness")
    success &= setBrightness(data.getDataFloat());
  else if(element == "color_b")
    setColorBalance(this->color_red, this->color_green, data.getDataInteger());
  else if(element == "color_g")
    setColorBalance(this->color_red, data.getDataInteger(), this->color_blue);
  else if(element == "color_r")
    setColorBalance(data.getDataInteger(), this->color_green, this->color_blue);
  else if(element == "forward")
  {
    if(data.getDataBool())
      success &= setDirectionForward();
    else
      success &= setDirectionReverse();
  }
  else if(element == "opacity")
    setOpacity(data.getDataInteger());
  else if(split_element.at(0) == "path")
  {
    /* If the sprite data is set to be built from path immediately */
    if(build_data)
    {
      uint16_t angle = parseAdjustments(split_element);

      std::vector<Frame*> new_frames = insertFrames(
          base_path + data.getDataString(), renderer, angle, no_warnings);

      /* If there is element adjustments, do those changes */
      if(split_element.size() > 1)
      {
        split_element.erase(split_element.begin());

        for(uint16_t i = 0; i < new_frames.size(); i++)
          success &= new_frames[i]->execImageAdjustments(split_element);
      }

      built_texture = true;
    }
    /* If the sprite data is set to be stored and loaded upon first render */
    else
    {
      std::vector<std::string> split_path =
          Helpers::split(base_path + data.getDataString(), '|');

      this->data.push_back(SpriteData());

      if(split_path.size() == 3)
      {
        this->data.back().build_path_head = split_path[0];
        this->data.back().build_frames = std::stoi(split_path[1]);
        this->data.back().build_path_tail = split_path[2];
      }
      else
      {
        this->data.back().build_path_head = base_path + data.getDataString();
        this->data.back().build_frames = 1;
      }
    }
  }
  else if(element == "rotation")
    setRotation(data.getDataInteger());
  else if(element == "sound_id")
    setSoundID(data.getDataInteger());

  return success;
}

/*
 * Description: Generates the baseline texture for all handling.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine
 * Output: none
 */
void Sprite::createTexture(SDL_Renderer* renderer)
{
  if(head->isTextureSet() && texture == nullptr)
  {
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, head->getWidth(),
                                head->getHeight());
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    setColorMod();
    setOpacity(opacity);
  }
}

/*
 * Description: Executes a set of image adjustments using a list of strings in
 *              all frames in sprite. See execImageAdjustment() in frame for
 *              more details.
 *
 * Inputs: vector<string> adjustments - a stack of all adjustments
 * Output: bool - status if successful
 */
bool Sprite::execImageAdjustments(std::vector<std::string> adjustments)
{
  if(head != NULL)
  {
    bool success = true;
    Frame* parsed_frame = head;

    do
    {
      success &= parsed_frame->execImageAdjustments(adjustments);
      parsed_frame = parsed_frame->getNext();
    } while(parsed_frame != head);

    return success;
  }
  return false;
}

/*
 * Description: Returns the animation time between frame changes on the sprite
 *              sequence.
 *
 * Inputs: none
 * Output: short - the animation time in milliseconds
 */
uint16_t Sprite::getAnimationTime() const
{
  return animation_time;
}

/*
 * Description: Gets the brightness value that the sprite sequence is being
 *              rendered at.
 *
 * Inputs: none
 * Output: double - the brightness indicator
 *                (<1: darker, 1: default, >1: lighter)
 */
double Sprite::getBrightness() const
{
  return brightness;
}

/*
 * Description: Returns the color delta alpha for the sprite.
 *
 * Inputs: none
 * Output: uint8_t - the alpha value from 0-255
 */
uint8_t Sprite::getColorAlpha()
{
  return color_alpha;
}

/*
 * Description: Returns the color mode for how the sprite is rendered on a call
 *              to render().
 *
 * Inputs: none
 * Output: ColorMode - the mode of how the sprite gets handled
 */
ColorMode Sprite::getColorMode()
{
  return color_mode;
}

/*
 * Description: Returns the color distribution evenness, according to the blue
 *              RGB value. Rated from 0 to 255. (255 full saturation).
 *
 * Inputs: none
 * Output: uint8_t - the rated blue RGB value
 */
uint8_t Sprite::getColorBlue() const
{
  return color_blue;
}

/*
 * Description: Returns the color distribution evenness, according to the green
 *              RGB value. Rated from 0 to 255. (255 full saturation).
 *
 * Inputs: none
 * Output: uint8_t - the rated green RGB value
 */
uint8_t Sprite::getColorGreen() const
{
  return color_green;
}

/*
 * Description: Returns the color distribution evenness, according to the red
 *              RGB value. Rated from 0 to 255. (255 full saturation).
 *
 * Inputs: none
 * Output: uint8_t - the rated red RGB value
 */
uint8_t Sprite::getColorRed() const
{
  return color_red;
}

/*
 * Description: Gets the current frame that is active.
 *
 * Inputs: none
 * Output: Frame* - the current frame where the sprite is at
 */
Frame* Sprite::getCurrent()
{
  return current;
}

/*
 * Description: Gets the current frame and then shifts to the next one
 *
 * Inputs: none
 * Output: Frame* - the frame before the shift occurred
 */
Frame* Sprite::getCurrentAndShift()
{
  Frame* previous = current;
  shiftNext();

  return previous;
}

/*
 * Description: Returns the frame that marks the head of the sprite stack.
 *              Do not delete since this causes erradic results.
 *
 * Inputs: none
 * Output: Frame* - the frame pointer
 */
Frame* Sprite::getFirstFrame() const
{
  return head;
}

/*
 * Description: Returns the numerical identifier for the sprite texture.
 *
 * Inputs: none
 * Output: uin16_t - an integer from 0 - 65535 (16 bit unsigned integer)
 */
uint16_t Sprite::getId() const
{
  return id;
}

/*
 * Description: Returns the number of loops the sprite has completed
 *
 * Inputs: none
 * Output: uint32_t - an integer from 0 - 2^32 - 1 (32 bit unsigned integer)
 */
uint32_t Sprite::getLoops()
{
  return loops;
}

/*
 * Description: Returns the opacity that the sprite is rendered at.
 *
 * Inputs: none
 * Output: uint8_t - the opacity value, from 0-255. 255 full opaque
 */
uint8_t Sprite::getOpacity() const
{
  return opacity;
}

/*
 * Description: Returns the position that the linked list is currently at
 *
 * Inputs: none
 * Output: int - the position in the linked list from the head
 */
int Sprite::getPosition()
{
  int location = 0;
  Frame* shifted = head;

  while(shifted != current)
  {
    shifted = shifted->getNext();
    location++;
  }

  return location;
}

/*
 * Description: Returns the rotation angle for rendering the texture, in
 *              degrees
 *
 * Inputs: none
 * Output: float - the angle, in degrees
 */
float Sprite::getRotation() const
{
  return rotation_angle;
}

/*
 * Description: Returns the size of the sequence
 *
 * Inputs: none
 * Output: int - the size of the sprite list
 */
int Sprite::getSize() const
{
  return size;
}

/*
 * Description: Returns the sound ID reference. If less than 0, invalid.
 *
 * Inputs: none
 * Output: int32_t - the ID of sound chunk
 */
int32_t Sprite::getSoundID() const
{
  return sound_id;
}

/*
 * Description: Returns the source rect used when the render call is accessed.
 *              If no rect has been set, it returns nullptr which by default
 *              renders the entire active frame.
 *
 * Inputs: none
 * Output: SDL_Rect* - reference to the source rect for rendering
 */
SDL_Rect* Sprite::getSourceRect()
{
  if(src_rect_use)
    return &src_rect;
  return nullptr;
}

/*
 * Description: Returns the color distribution evenness, according to the
 *              temp red RGB value. Rated from 0 to 255. (255 full
 *              saturation).
 *
 * Inputs: none
 * Output: uint8_t - the rated temp. RGB value
 */
uint8_t Sprite::getTempColorRed() const
{
  return color_temp_red;
}

/*
 * Description: Returns the color distribution evenness, according to the
 *              temp green RGB value. Rated from 0 to 255. (255 full
 *              saturation).
 *
 * Inputs: none
 * Output: uint8_t - the rated temp. RGB value
 */
uint8_t Sprite::getTempColorGreen() const
{
  return color_temp_green;
}

/*
 * Description: Returns the color distribution evenness, according to the
 *              temp blue RGB value. Rated from 0 to 255. (255 full
 *              saturation).
 *
 * Inputs: none
 * Output: uint8_t - the rated temp RGB value
 */
uint8_t Sprite::getTempColorBlue() const
{
  return color_temp_blue;
}

/*
 * Description: Inserts the image into the sprite sequence at the given
 *              position based on the given string path.
 *
 * Inputs: std::string path - the path to the image to add
 *         SDL_Renderer* renderer - the rendering engine pointer
 *         int position - the location in the linked list sequence
 *         uint16_t angle - angle for frame based rotation (must be mod 90)
 *         bool no_warnings - should warnings not fire? default false.
 * Output: Frame* - the frame that was inserted. NULL if failed
 */
Frame* Sprite::insert(std::string path, SDL_Renderer* renderer, int position,
                      uint16_t angle, bool no_warnings)
{
  Frame* next_frame;
  Frame* new_frame;
  Frame* previous_frame;

  /* Only add if the size is within the bounds of the sprite */
  if(size == 0)
  {
    return insertFirst(path, renderer, angle, no_warnings);
  }
  else if(position <= size && position >= 0)
  {
    new_frame = new Frame();
    new_frame->setTexture(path, renderer, angle, no_warnings);

    if(new_frame->isTextureSet())
    {
      next_frame = head;

      /* Parse through to find where to insert */
      for(int i = 0; i < position; i++)
        next_frame = next_frame->getNext();
      previous_frame = next_frame->getPrevious();

      /* Reassign linked list pointers */
      previous_frame->setNext(new_frame);
      next_frame->setPrevious(new_frame);
      new_frame->setNext(next_frame);
      new_frame->setPrevious(previous_frame);

      /* If inserting to the front, reset the head pointer */
      if(position == 0)
        head = new_frame;

      size++;
      return new_frame;
    }
    delete new_frame;
    new_frame = NULL;
  }

  return new_frame;
}

/*
 * Description: Inserts the first image, based on the image path, if the frame
 *              sequence is empty.
 * Note: This isn't for inserting the head, just the first one in an empty
 *       list.
 *
 * Inputs: std::string path - the path to the image to add
 *         SDL_Renderer* renderer - the rendering engine pointer
 *         uint16_t angle - angle for frame based rotation (must be mod 90)
 *         bool no_warnings - should warnings not fire? default false.
 * Output: Frame* - the frame that was inserted. NULL if failed
 */
Frame* Sprite::insertFirst(std::string path, SDL_Renderer* renderer,
                           uint16_t angle, bool no_warnings)
{
  if(size == 0)
  {
    head = new Frame();
    head->setTexture(path, renderer, angle, no_warnings);

    if(head->isTextureSet())
    {
      /* First set the rendering texture, if unset */
      if(texture == NULL)
      {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, head->getWidth(),
                                    head->getHeight());
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        setColorMod();
        setOpacity(opacity);
      }

      /* Only proceed with finishing if the rendering texture could be set */
      if(texture != NULL)
      {
        head->setNext(head);
        head->setPrevious(head);
        current = head;
        size = 1;
        texture_update = true;

        return head;
      }
    }

    delete head;
    head = NULL;
  }

  Frame* null_frame = NULL;
  return null_frame;
}

/*
 * Description: Insert the frames based on a single path. This is a helper
 *              function that differentiates between a multiple frame path
 *              and a single path. The differentiation occurs between two
 *              vertical bars -> "path_|2|.png". This will send the path node
 *              to the insertSequence for the two frames: path_00.png,
 *              path_01.png. If no vertical bars, it just tries and adds the
 *              single path.
 *
 * Inputs: std::string path - the path frame to add
 *         SDL_Renderer* renderer - the rendering engine for the frames
 *         uint16_t angle - angle for frame based rotation (must be mod 90)
 *         bool no_warnings - should warnings not fire? default false.
 * Output: std::vector<Frame*> - the stack of frames that were inserted
 */
std::vector<Frame*> Sprite::insertFrames(std::string path,
                                         SDL_Renderer* renderer, uint16_t angle,
                                         bool no_warnings)
{
  /* Split the path and see if it split. If it did, insert sequence. Otherwise
   * insert the single frame at tail. */
  std::vector<std::string> split_path = Helpers::split(path, '|');
  if(split_path.size() == 3)
    return insertSequence(split_path[0], std::stoi(split_path[1]),
                          split_path[2], renderer, angle, no_warnings);

  /* Otherwise, put the frame on the tail (single frame) */
  Frame* tail_frame = insertTail(path, renderer, angle, no_warnings);
  std::vector<Frame*> stack;
  if(tail_frame != NULL)
    stack.push_back(tail_frame);
  return stack;
}

/*
 * Description: Inserts a sequence of images that are stored. This allows for
 *              quick insertion of stored frames.
 * For example: head_path = ":/animation/image_"
 *              count = 5
 *              tail_path = ".png"
 *   This will allow for image_00.png -> image_04.png to be added into
 *   a sequence
 *
 * Inputs: std::string head_path - see above for explanation
 *         int count - see above for explanation
 *         std::string tail_path - see above for explanation
 *         SDL_Renderer* renderer - the rendering engine pointer
 *         uint16_t angle - angle for frame based rotation (must be mod 90)
 *         bool no_warnings - should warnings not fire? default false.
 * Output: std::vector<Frame*> - the stack of frames that were inserted
 */
std::vector<Frame*> Sprite::insertSequence(std::string head_path, int count,
                                           std::string tail_path,
                                           SDL_Renderer* renderer,
                                           uint16_t angle, bool no_warnings)
{
  std::vector<Frame*> stack;
  bool status = true;

  /* Test if there are sufficient frames */
  if(count <= 0)
    status = false;

  /* Store the initial condition */
  int old_size = size;

  /* Parse all the frames in the sequence */
  for(int i = 0; i < count; i++)
  {
    if(i >= kDOUBLE_DIGITS)
      stack.push_back(insertTail(head_path + std::to_string(i) + tail_path,
                                 renderer, angle, no_warnings));
    else
      stack.push_back(
          insertTail(head_path + "0" + std::to_string(i) + tail_path, renderer,
                     angle, no_warnings));

    status &= (stack.back() != NULL);
  }

  /* If the sequence failed, delete the created pointers */
  if(!status)
  {
    while(size != old_size)
      removeTail();
    stack.clear();
  }

  return stack;
}

/*
 * Description: Inserts the image, based on the path, at the end of the sprite
 *              sequence
 *
 * Inputs: std::string path - the path to the image to add
 *         SDL_Renderer* renderer - the rendering engine pointer
 *         uint16_t angle - angle for frame based rotation (must be mod 90)
 *         bool no_warnings - should warnings not fire? default false.
 * Output: Frame* - the frame that was inserted. NULL if failed
 */
Frame* Sprite::insertTail(std::string path, SDL_Renderer* renderer,
                          uint16_t angle, bool no_warnings)
{
  return insert(path, renderer, size, angle, no_warnings);
}

/*
 * Description: Checks if the linked list pointer is at the head of the list
 *
 * Inputs: none
 * Output: bool - returns true if the current pointer is at the head
 */
bool Sprite::isAtFirst()
{
  if(head == current)
    return true;
  return false;
}

/*
 * Description: Checks if the linked list pointer is at the tail of the list
 *
 * Inputs: none
 * Output: bool - returns true if the current pointer is at the tail
 */
bool Sprite::isAtEnd()
{
  if(getSize() <= 1)
    return isAtFirst();
  return (head->getPrevious() == current);
}

/*
 * Description: Returns if the color is transitioning between color and grey
 *
 * Inputs: none
 * Output: bool - true if in transition
 */
bool Sprite::isColorTransitioning() const
{
  return (color_mode == ColorMode::GREYING ||
          color_mode == ColorMode::COLORING);
}

/*
 * Description: Returns if the parsing direction through the frame sequence
 *              is forward. Otherwise, it is in reverse.
 *
 * Inputs: none
 * Output: bool - status if the direction is forward. False if reverse.
 */
bool Sprite::isDirectionForward() const
{
  return (sequence == FORWARD);
}

/*
 * Description: Returns if any frames are set within the sprite class
 *
 * Inputs: none
 * Output: bool - true if set
 */
bool Sprite::isFramesSet() const
{
  return (head != nullptr);
}

/*
 * Description: Loads the sprite data from data into the
 *              Sprite class constructing the frames/textures as required.
 *
 * Inputs: none
 * Output: bool - true if the data could have been loaded
 */
bool Sprite::loadData(SDL_Renderer* renderer)
{
  bool success = true;

  if(renderer && !built_texture)
  {
    for(auto& element : data)
    {
      if(element.build_path_head != "" && element.build_path_tail == "")
      {
        // std::cout << "Inserting 1 Frame: " << element.build_path_head
        //                  << std::endl;
        insertFirst(element.build_path_head, renderer);
        success = true;
      }
      else if(element.build_path_head != "" && element.build_path_tail != "")
      {
        // std::cout << "Inserting " << std::endl;
        // std::cout << "Head: " << element.build_path_head << std::endl;
        // std::cout << "# Frames: " << element.build_frames << std::endl;
        // std::cout << "Tail: " << element.build_path_tail << std::endl;
        insertSequence(element.build_path_head, element.build_frames,
                       element.build_path_tail, renderer);
        success = true;
      }
    }
  }

  built_texture = true;

  return success;
}

/*
 * Description: Removes the frame in the sequence at the given position
 *
 * Inputs: int position - the position of the frame to remove in the linked
 *                        list.
 * Output: bool - status if removal was successful
 */
bool Sprite::remove(int position)
{
  Frame* old_frame;
  Frame* next_frame;
  Frame* previous_frame;

  /* Only remove if the position exists within the size boundaries */
  if(position < size && position >= 0)
  {
    old_frame = head;

    /* Find the location to delete */
    for(int i = 0; i < position; i++)
      old_frame = old_frame->getNext();
    next_frame = old_frame->getNext();
    previous_frame = old_frame->getPrevious();

    /* Reset linked list pointers */
    next_frame->setPrevious(previous_frame);
    previous_frame->setNext(next_frame);

    /* If at the front of the list, shift list backwards for new head */
    if(position == 0)
      head = next_frame;

    /* Delete frame and reduce the size after deletion */
    delete old_frame;
    size--;

    /* Reset the current frame based on deletion of frame */
    if(size == 0)
    {
      head = nullptr;
      current = nullptr;
    }
    else
    {
      current = head;
    }

    texture_update = true;

    return true;
  }

  return false;
}

/*
 * Description: Removes all the frames in the sequence. Empties the sprite.
 *
 * Inputs: none
 * Output: bool - status if process was successful. If any faults, will return
 *                false. Also, if the sprite is already empty, it will return
 *                false.
 */
bool Sprite::removeAll()
{
  bool status = true;

  if(size == 0)
    status = false;

  while(size > 0)
    status = status & removeTail();

  return status;
}

/*
 * Description: Removes the last frame in the sequence
 *
 * Inputs: none
 * Output: bool - status if tail removal was successful
 */
bool Sprite::removeTail()
{
  return remove(size - 1);
}

/*
 * Description: Resets the number of loops for the Sprite to zero.
 *
 * Inputs: none
 * Output: none
 */
void Sprite::resetLoops()
{
  loops = 0;
}

/* Render the texture to the given renderer with the given parameters */
/*
 * Description: Renders the sprite data, utilizing a x and y coordinate, and a
 *              width and height value, if relevant. If width and height aren't
 *              entered, it uses the texture size.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine for the sprite texture
 *         int x - the x coordinate on the painted viewport
 *         int y - the y coordinate on the painted viewport
 *         int w - the width of the texture painted
 *         int h - the height of the texture painted
 * Output: bool - status if the render was successful
 */
bool Sprite::render(SDL_Renderer* renderer, int x, int y, int w, int h)
{
  if(!built_texture)
    loadData(renderer);

  if(current != nullptr && renderer != nullptr)
  {
    /* Proceed to update the running texture if it's changed */
    if(texture_update || color_mode == ColorMode::GREYING ||
       color_mode == ColorMode::COLORING)
    {
      SDL_Texture* previous_renderer = SDL_GetRenderTarget(renderer);

      SDL_SetRenderTarget(renderer, texture);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);

      /* Render current frame */
      current->setColorAlpha(color_alpha);
      current->render(renderer, 0, 0, 0, 0, getSourceRect(), true);

      /* Render white mask, if relevant */
      SDL_Texture* white_mask = Helpers::getMaskWhite();
      if(brightness > kDEFAULT_BRIGHTNESS && white_mask != NULL)
      {
        double bright_mod = (brightness - kDEFAULT_BRIGHTNESS);
        if(bright_mod > kDEFAULT_BRIGHTNESS)
          bright_mod = kDEFAULT_BRIGHTNESS;

        SDL_SetTextureAlphaMod(white_mask, 255 * bright_mod);
        SDL_RenderCopy(renderer, white_mask, NULL, NULL);
      }

      /* Release the renderer and end the update */
      SDL_SetRenderTarget(renderer, previous_renderer);
      texture_update = false;
    }

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = current->getHeight();
    rect.w = current->getWidth();

    /* Use parameter height and width if both are viable */
    if(h > 0 && w > 0)
    {
      rect.h = h;
      rect.w = w;
    }

    /* Render and return status */
    return (SDL_RenderCopyEx(renderer, texture, NULL, &rect, rotation_angle,
                             NULL, SDL_FLIP_NONE) == 0);
  }

  return false;
}

/*
 * Description: Revers the normal color mod values to the values to the value
 *              stored in the temporary color mod. variables.
 *
 * Inputs: none
 * Output: none
 */
void Sprite::revertColorBalance()
{
  color_red = color_temp_red;
  color_green = color_temp_green;
  color_blue = color_temp_blue;
  setColorMod();
}

/*
 * Description: Sets the animation time between frame changes. Gets called
 *              from the update call below for updating the frames in the
 *              sequence.
 *
 * Inputs: uint16_t time - the update time in milliseconds
 * Output: none
 */
void Sprite::setAnimationTime(uint16_t time)
{
  /* Set the new animation time */
  animation_time = time;

  /* Reset the elapsed time */
  elapsed_time = 0;
}

/*
 * Description: Set the linked list current pointer to the head of the list
 *
 * Inputs: none
 * Output: bool - status if resetting the linked list to the first element
 *                was successful.
 */
bool Sprite::setAtFirst()
{
  current = head;
  elapsed_time = 0;
  texture_update = true;
  return true;
}

/*
 * Description: Sets the brightness that the entire sprite sequence will be
 *              rendered at. It's range is 0-0.99: darker than default, 1.0:
 *              default brightness, 1.01-2.00: brighter.
 *
 * Inputs: double brightness - the brightness value (0+, 1.0 default)
 * Output: bool - if the set was in proper range. If out of range, it gets
 *                locked to the correct range.
 */
bool Sprite::setBrightness(double brightness)
{
  /* Check whether in limits */
  bool in_limits{Helpers::isInRange(brightness, 0.0, kMAX_BRIGHTNESS)};

  /* Ensure is within limits */
  Helpers::setInRange(brightness, 0.0, kMAX_BRIGHTNESS);

  /* Update the class brightness */
  if(this->brightness != brightness)
  {
    this->brightness = brightness;
    setColorMod();
    texture_update = true;
  }

  return in_limits;
}
/*
 * Description: Assigns the build information for the sprite
 *
 * Inputs: std::string build_path_head - path to the head of the sprite
 *         std::string build_path_tail - path to the tail of the sprite
 *         int32_t build_frames - the number of frames to construct
 * Output: bool - true if the information was able to be set
 */
bool Sprite::setBuildInformation(std::string build_path_head,
                                 std::string build_path_tail,
                                 int32_t build_frames)
{
  if(!built_texture)
  {
    data.push_back(SpriteData());
    data.back().build_path_head = build_path_head;
    data.back().build_path_tail = build_path_tail;
    data.back().build_frames = build_frames;
  }

  return !built_texture;
}

/*
 * Description: Sets the color balance of the rendered texture. If each value
 *              is at 255, that is full color saturation. As the numbers get
 *              lowered, the color is pulled from the rendered texture.
 *
 * Inputs: uint8_t red - the red color 0-255 rating (255 full)
 *         uint8_t green - the green color 0-255 rating (255 full)
 *         uint8_t blue - the blue color 0-255 rating (255 full)
 * Output: none
 */
void Sprite::setColorBalance(uint8_t red, uint8_t green, uint8_t blue)
{
  color_red = red;
  color_green = green;
  color_blue = blue;

  setColorMod();
}

/*
 * Description: Sets the color mask rating of blue for the rendered texture.
 *              If the value is at 255, that is full color saturation. As the
 *              number gets lowered, the color is pulled from the rendered
 *              texture.
 *
 * Inputs: uint8_t color - the color 0-255 rating (255 full)
 * Output: none
 */
void Sprite::setColorBlue(uint8_t color)
{
  color_blue = color;
  setColorMod();
}

/*
 * Description: Sets the color mask rating of green for the rendered texture.
 *              If the value is at 255, that is full color saturation. As the
 *              number gets lowered, the color is pulled from the rendered
 *              texture.
 *
 * Inputs: uint8_t color - the color 0-255 rating (255 full)
 * Output: none
 */
void Sprite::setColorGreen(uint8_t color)
{
  color_green = color;
  setColorMod();
}

/*
 * Description: Sets the color mask rating of red for the rendered texture. If
 *              the value is at 255, that is full color saturation. As the
 *              number gets lowered, the color is pulled from the rendered
 *              texture.
 *
 * Inputs: uint8_t color - the color 0-255 rating (255 full)
 * Output: none
 */
void Sprite::setColorRed(uint8_t color)
{
  color_red = color;
  setColorMod();
}

/*
 * Description: This sets the color mode based on the enumerator. This can
 *              control the grey scale and how it blends/transitions for all
 *              frames.
 *
 * Inputs: ColorMode mode - the new mode to update the mode
 * Output: bool - status if set was successful
 */
bool Sprite::setColorMode(ColorMode mode)
{
  bool set = false;

  if(mode != color_mode && head != nullptr)
  {
    /* Color enabled mode - default */
    if(mode == ColorMode::COLOR ||
       (color_mode == ColorMode::COLOR && mode == ColorMode::COLORING))
    {
      color_mode = ColorMode::COLOR;
      color_alpha = 0;
      set = true;
    }
    else if(mode == ColorMode::GREY ||
            (color_mode == ColorMode::GREY && mode == ColorMode::GREYING))
    {
      color_mode = ColorMode::GREY;
      color_alpha = 0;
      set = true;
    }
    /* All other color modes */
    else
    {
      /* Check if this was a coloring to greying or vice versa */
      bool full_switch = false;
      if((color_mode == ColorMode::COLORING && mode == ColorMode::GREYING) ||
         (color_mode == ColorMode::GREYING && mode == ColorMode::COLORING))
      {
        full_switch = true;
      }

      /* Set the mode */
      color_mode = mode;
      set = true;

      /* Update the alpha accordingly */
      if(full_switch)
        color_alpha = (kDEFAULT_OPACITY - color_alpha);
      else
        color_alpha = 0;
    }

    /* Update all frames with the new color mode values */
    Frame* parse = head;
    do
    {
      parse->setColorMode(color_mode);
      parse->setColorAlpha(color_alpha);
      parse = parse->getNext();
    } while(parse != head && parse != nullptr);

    texture_update = true;
  }

  return set;
}

/*
 * Description: Sets the direction that the linked list is navigated to
 *              FORWARD. In other words, accessing the *next pointer when
 *              parsing it.
 *
 * Inputs: none
 * Output: bool - status if direction set was successful
 */
bool Sprite::setDirectionForward()
{
  sequence = FORWARD;
  return true;
}

/*
 * Description: Sets the direction that the linked list is navigated to
 *              REVERSE. In other words, accessing the *previous pointer when
 *              parsing it.
 *
 * Inputs: none
 * Output: bool - status if direction set was successful
 */
bool Sprite::setDirectionReverse()
{
  sequence = REVERSE;
  return true;
}

/*
 * Description: Sets the head frame as per the passed parameter
 *
 * Inputs: Frame* head - the new head frame
 * Output: none
 */
void Sprite::setHead(Frame* head)
{
  this->head = head;

  setAtFirst();
}

/*
 * Description: Sets the numerical identifier for the sprite. It can only be a
 *              number greater than or equal to 0 and less than 65535.
 *
 * Inputs: uint16_t id - the new identifier
 * Output: none
 */
void Sprite::setId(uint16_t id)
{
  this->id = id;
}

/*
 * Description: Assigns the sprite as a non-unique Sprite with a given number
 *              of frames declared [since the Sprite copy constructor does not
 *              copy the size of the old sprite over . This will allow the
 *              Sprite to be used as an extension of an existing sprite which
 *              wasn't loaded from file and won't be dynamically deleted upon
 *              destruction.
 *
 * Inputs: bool new_value - true if the sprite is not to be deleted
 *         int32_t size - the size of the Sprite to be set
 * Output: none
 */
void Sprite::setNonUnique(bool new_value, int32_t size)
{
  size = size;
  non_unique = new_value;
}

/*
 * Description: Assigns a number of loops to update the sprite for before
 *              stopping. Useful for pleps / action sprites for Battle.
 *
 * Inputs: int32_t loops - the number loops to the loop the sprite for
 * Output: none
 */
void Sprite::setNumLoops(int32_t loops)
{
  if(loops >= 0)
    this->loops = loops;
}

/*
 * Description: Sets the sprite opacity. It utilizes an 8 bit unsigned number
 *              where is 0 is fully transparent and 255 is fully opaque.
 *
 * Inputs: uint8_t opacity - the opacity rating
 * Output: none
 */
void Sprite::setOpacity(uint8_t opacity)
{
  this->opacity = opacity;
  SDL_SetTextureAlphaMod(texture, opacity);
}

/*
 * Description: Sets the rotation that the frames will be rendered at around
 *              the center point
 *
 * Inputs: float angle - angle in degrees and float to rotate the frames to
 * Output: none
 */
void Sprite::setRotation(float angle)
{
  rotation_angle = angle;
}

/*
 * Description: Sets the reference sound ID. If less than 0, unsets it.
 *
 * Inputs: int32_t id - the new sound reference id
 * Output: none
 */
void Sprite::setSoundID(int32_t id)
{
  if(id < 0)
    this->sound_id = kUNSET_SOUND_ID;
  else
    this->sound_id = id;
}

/*
 * Description: Sets the source rect for rendering the active current frame.
 *              This is based on the width and height of the source frame and
 *              how it associates with the destination render. See
 *              unsetSourceRect() for unsetting at which point the entire
 *              source frame will render.
 *
 * Inputs: SDL_Rect rect - the source rect for rendering
 * Output: none
 */
void Sprite::setSourceRect(SDL_Rect rect)
{
  this->src_rect = rect;
  this->src_rect_use = true;
}

/*
 * Description: Stores the given RGB values as the temporary color balance
 *              values, easily able to be reverted.
 *
 * Inputs: uint8_t temp_red - temporary red color 0-255 (255 full)
 *         uint8_t temp_green - temporary green color 0-255 (255 full)
 *         uint8_t temp_blue  - temporary blue color 0-255 (255 full)
 * Output: none
 */
void Sprite::setTempColorBalance(uint8_t temp_red, uint8_t temp_green,
                                 uint8_t temp_blue)
{
  setTempColorRed(temp_red);
  setTempColorGreen(temp_green);
  setTempColorBlue(temp_blue);
}

/*
 * Description: Store the given value as the temporary color mask for red.
 *
 * Inputs: uint8_t temp_red - temporary red color 0-255 (255 full)
 * Output: none
 */
void Sprite::setTempColorRed(uint8_t temp_red)
{
  color_temp_red = temp_red;
}

/*
 * Description: Store the given value as the temporary color mask for green
 *
 * Inputs: uint8_t temp_green - temporary green color 0-255 (255 full)
 * Output: none
 */
void Sprite::setTempColorGreen(uint8_t temp_green)
{
  color_temp_green = temp_green;
}

/*
 * Description: Store the given value as the temporary color mask for blue
 *
 * Inputs: uint8_t temp_blue - temporary blue color 0-255 (255 full)
 * Output: none
 */
void Sprite::setTempColorBlue(uint8_t temp_blue)
{
  color_temp_blue = temp_blue;
}

/*
 * Description: Sets the rotation that the frames will be rendered at around
 *              the center point.
 *
 * Inputs: int angle - the angle in degrees to rotate the frames to
 * Output: none
 */
void Sprite::setRotation(int angle)
{
  rotation_angle = angle;
}

/*
 * Description: Shifts to the given position in the sequence
 *
 * Inputs: int position - position to shift to in the linked list
 * Output: bool - status if moving in the linked list was successful
 */
bool Sprite::shift(int position)
{
  Frame* old_current = current;

  /* Only shift if the position is within the bounds of the sprite */
  if(position < size && position >= 0)
  {
    Frame* new_current_frame = head;

    for(int i = 0; i < position; i++)
      new_current_frame = new_current_frame->getNext();
    current = new_current_frame;

    /* Only update the texture if the current frame has changed */
    if(old_current != current)
      texture_update = true;

    return true;
  }

  return false;
}

/*
 * Description: Shifts to the next frame in the sprite.
 * Note: this function allows you to skip the head of the list while shifting.
 *       However, if the size of the sprite is only one frame, the head will
 *       still be selected.
 *
 * Inputs: bool skip_head - allows for the head of the list to be skipped
 *                          while shifting.
 * Output: bool - status if shift to next was successful
 */
bool Sprite::shiftNext(bool skip_head)
{
  Frame* old_current = current;

  if(size > 0)
  {
    if(sequence == FORWARD)
    {
      if(!skip_head || current->getNext() != head)
        current = current->getNext();
      else
        current = current->getNext()->getNext();
    }
    else
    {
      if(!skip_head || current->getPrevious() != head)
        current = current->getPrevious();
      else
        current = current->getPrevious()->getPrevious();
    }

    /* Only update the texture if the current frame has changed */
    if(old_current != current)
      texture_update = true;

    /* Every time the next element is the head, increase loops, if size is 1,
     * every step is a loop, if size is greater than one, every head touch is
     * a loop. If head is skipped, no loops take place. */
    if(current->getNext() == head)
      loops++;

    return true;
  }
  return false;
}

/*
 * Description: Sets the direction that the linked list is navigated to
 *              opposite of what it was before. FORWARD -> REVERSE or
 *              REVERSE -> FORWARD.
 *
 * Inputs: none
 * Output: bool - status if direction set was successful
 */
bool Sprite::switchDirection()
{
  if(sequence == FORWARD)
    return setDirectionReverse();
  return setDirectionForward();
}

/*
 * Description: Unsets the source rect being used and returns back to default
 *              of just using the entire source frame for rendering. This
 *              relates to setSourceRect()
 *
 * Inputs: none
 * Output: none
 */
void Sprite::unsetSourceRect()
{
  src_rect_use = false;
}

/*
 * Description: Updates the sprite times for animation. Necessary for
 *              automated animation.
 *
 * Inputs: int cycle_time - the update time that has elapsed, in milliseconds
 *         bool skip_head - skip the head frame when updating?
 * Output: bool - returns if the frame changed
 */
bool Sprite::update(int cycle_time, bool skip_head)
{
  bool shift = false;

  /* If skip head is triggered, but it is at head, skip to next */
  if(skip_head && isAtFirst())
  {
    shiftNext(skip_head);
    shift = true;
  }

  /* Do grey scale checking, for animation */
  if(color_mode == ColorMode::GREYING || color_mode == ColorMode::COLORING)
  {
    int val = color_alpha + kDELTA_GREY_SCALE;
    if(val >= kDEFAULT_OPACITY)
    {
      if(color_mode == ColorMode::GREYING)
        setColorMode(ColorMode::GREY);
      else
        setColorMode(ColorMode::COLOR);
    }
    else
    {
      color_alpha = val;
    }
  }

  /* Start by updating the animation and shifting, if necessary */
  if(size > 1 && cycle_time > 0 && animation_time > 0)
  {
    elapsed_time += cycle_time;
    if(elapsed_time > animation_time)
    {
      elapsed_time -= animation_time;
      shiftNext(skip_head);
      shift = true;
    }
  }

  return shift;
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another sprite.
 *
 * Inputs: const Sprite &source - the source class constructor
 * Output: Sprite& - pointer to the copied class
 */
Sprite& Sprite::operator=(const Sprite& source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the degree interpretation of a string representation
 *              for the 3 classifications.
 *              90, clockwise: "90", CW, or cw
 *              180, half circle: "180", HC, or hc
 *              270, counter clockwise: "270", CCW, or ccw
 *
 * Inputs: std::string identifier - the angle string identity
 * Output: int - angle in degrees.
 */
int Sprite::getAngle(std::string identifier)
{
  if(identifier == "90" || identifier == "CW" || identifier == "cw")
    return getAngle(CLOCKWISE);
  else if(identifier == "270" || identifier == "CCW" || identifier == "ccw")
    return getAngle(COUNTERCLOCKWISE);
  else if(identifier == "180" || identifier == "HC" || identifier == "hc")
    return getAngle(HALFCIRCLE);
  return 0;
}

/*
 * Description: Returns the degree interpretation of the rotated angle enum
 *              for the 4 classifications. Can be used above for translating
 *              integer representation from the enumerator.
 *
 * Inputs: RotatedAngle angle - the angle enumerator to classify
 * Output: int - angle in degrees.
 */
int Sprite::getAngle(RotatedAngle angle)
{
  if(angle == CLOCKWISE)
    return 90;
  else if(angle == COUNTERCLOCKWISE)
    return 270;
  else if(angle == HALFCIRCLE)
    return 180;
  return 0;
}
