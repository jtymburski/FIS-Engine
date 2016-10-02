/******************************************************************************
 * Class Name: SpriteMatrix
 * Date Created: November 20, 2014
 * Inheritance: none
 * Description: This class contains the logic for matrix of sprites that will
 *              be rendered as a group. Primarily utilized by MapThing and
 *              children.
 *****************************************************************************/
#include "Game/Map/SpriteMatrix.h"

/* Constant Implementation - see header file for descriptions */
const float SpriteMatrix::kBASE_FRAME_COUNT = 2.0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Base constructor function - initial clean setup
 *
 * Inputs: none
 */
SpriteMatrix::SpriteMatrix()
{
  frame_index = 0;
}

/*
 * Description: Constructor function - sets up the base tile sprites using
 *              a passed in matrix of sprites. This does take control of
 *              deleting the sprites in the matrix, unless the sprites are
 *              removed during clean-up using a 'don't delete' flag.
 *
 * Inputs: std::vector<std::vector<TileSprite*>> sprites - matrix of sprites
 */
SpriteMatrix::SpriteMatrix(std::vector<std::vector<TileSprite*>> sprites)
            : SpriteMatrix()
{
  setSprites(sprites);
  cleanMatrix();
}

/*
 * Description: The copy constructor to transfer all non-graphical parameters
 *              from each sprite in the matrix and generate new sprites.
 *
 * Inputs: const SpriteMatrix &source - the source matrix data to copy over
 */
SpriteMatrix::SpriteMatrix(const SpriteMatrix &source) : SpriteMatrix()
{
  copySelf(source);
}

/*
 * Description: Destructor function - executes clean-up
 */
SpriteMatrix::~SpriteMatrix()
{
  unsetSprites();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const SpriteMatrix &source - the reference sprite matrix class
 * Output: none
 */
void SpriteMatrix::copySelf(const SpriteMatrix &source)
{
  std::vector<std::vector<TileSprite*>> sprite_set = source.getMatrix();

  for(uint32_t i = 0; i < sprite_set.size(); i++)
  {
    for(uint32_t j = 0; j < sprite_set[i].size(); j++)
    {
      if(sprite_set[i][j] != NULL)
        setSprite(new TileSprite(*sprite_set[i][j]), i, j);
    }
  }
}

/*
 * Description: Counts the number of valid sprites within the matrix. The
 *              definition of valid sprites is non-NULL and each TileSprite
 *              has some frames set within it.
 *
 * Inputs: none
 * Output: uint16_t - returns the count (unsigned)
 */
uint16_t SpriteMatrix::countValidSprites()
{
  uint16_t count = 0;

  for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != NULL && sprite_matrix[i][j]->isFramesSet())
        count++;

  return count;
}

/*
 * Description: Flips the string matrix of paths, based on the adjustment value.
 *              This takes in a horizontal flip (HF or hf), which flips all the
 *              paths on the X direction over the middle or a vertical flip (VF
 *              or vf), which flips all the paths on the Y direction over the
 *              middle.
 *
 * Inputs: std::vector<std::vector<std::string>> original - the original 2D
 *             matrix of paths.
 *         std::vector<std::string> adjustments - adjustment codes for flipping
 * Output: std::vector<std::vector<std::string>> - returned flipped array
 */
std::vector<std::vector<std::string>> SpriteMatrix::flipArray(
                                 std::vector<std::vector<std::string>> original,
                                 std::vector<std::string> adjustments)
{
  /* Determine the length and width */
  uint16_t size_x = original.size();
  uint16_t size_y = 0;
  if(size_x > 0)
    size_y = original.front().size();

  /* Only proceed if size(s) are valid */
  if(size_x > 1 || size_y > 1)
  {
    /* Parse all adjustments */
    for(uint16_t i = 0; i < adjustments.size(); i++)
    {
      /* Vertical flip of paths */
      if((adjustments[i] == "vf" || adjustments[i] == "VF") && size_y > 1)
      {
        uint16_t distance = size_y / 2;

        /* Parse through all the x values and flip columns */
        for(uint16_t j = 0; j < size_x; j++)
        {
          for(uint16_t k = 0; k < distance; k++)
          {
            std::string temp = original[j][k];
            original[j][k] = original[j][size_y - k - 1];
            original[j][size_y - k - 1] = temp;
          }
        }
      }
      /* Horizontal flip of paths */
      else if((adjustments[i] == "hf" || adjustments[i] == "HF") && size_x > 1)
      {
        uint16_t distance = size_x / 2;

        /* Parse through all the y values and flip rows */
        for(uint16_t j = 0; j < size_y; j++)
        {
          for(uint16_t k = 0; k < distance; k++)
          {
            std::string temp = original[k][j];
            original[k][j] = original[size_x - k - 1][j];
            original[size_x - k - 1][j] = temp;
          }
        }
      }
    }
  }

  return original;
}

/*
 * Description: Finds a valid TileSprite in the matrix of sprites, stored
 *              within the matrix. If there are no sprites set, it will create
 *              a blank one and place it at the origin of the matrix.
 * Note: Do not delete the pointer. It'll cause issues in the class.
 *
 * Inputs: none
 * Output: TileSprite* - the sprite pointer found with data
 */
TileSprite* SpriteMatrix::getValidSprite()
{
  bool found = false;
  uint32_t i = 0;
  uint32_t j = 0;
  TileSprite* valid_sprite = NULL;

  while(!found && i < sprite_matrix.size())
  {
    j = 0;

    while(!found && j < sprite_matrix[i].size())
    {
      if(sprite_matrix[i][j] != NULL)
      {
        valid_sprite = sprite_matrix[i][j];
        found = true;
      }
      j++;
    }
    i++;
  }

  /* Ensure there is at least one valid frame */
  if(valid_sprite == NULL)
  {
    valid_sprite = new TileSprite();
    setSprite(valid_sprite, 0, 0);
  }

  return valid_sprite;
}

/*
 * Description: Grow the matrix to be capable of addressing a sprite at
 *              the indicated size. If the matrix is already big enough,
 *              nothing happens.
 *
 * Inputs: uint32_t x - matrix X coordinate (horizontal)
 *         uint32_t y = matrix Y coordinate (vertical)
 * Output: none
 */
void SpriteMatrix::growMatrix(uint16_t x, uint16_t y)
{
  TileSprite* null_sprite = NULL;

  /* Determine if the array is empty or not -> different processing */
  if(sprite_matrix.size() == 0)
  {
    for(uint32_t i = 0; i <= x; i++)
    {
      std::vector<TileSprite*> col_data;

      for(uint32_t j = 0; j <= y; j++)
        col_data.push_back(null_sprite);

      sprite_matrix.push_back(col_data);
    }
  }
  else
  {
    /* First deal with the y direction (the backend of the array) */
    if(sprite_matrix.back().size() <= y)
    {
      for(uint32_t i = 0; i < sprite_matrix.size(); i++)
      {
        for(uint32_t j = sprite_matrix[i].size(); j <= y; j++)
          sprite_matrix[i].push_back(null_sprite);
      }
    }

    /* Next deal with the x direction, if it's less than the new dimension */
    if(sprite_matrix.size() <= x)
    {
      uint32_t height = sprite_matrix.back().size();
      std::vector<TileSprite*> col_data;

      /* Generate the column of data */
      for(uint32_t i = 0; i < height; i++)
        col_data.push_back(null_sprite);

      /* Push back the necessary number of columns to bring it up to size */
      for(uint32_t i = sprite_matrix.size(); i <= x; i++)
        sprite_matrix.push_back(col_data);
    }
  }
}

/*
 * Description: Removes sprite from the given matrix at the given X and Y
 *              coordinate. If delete sprite boolean is true, sprite is also
 *              removed from memory.
 * Note: this does not check to make sure it's in range or resize matrix after
 *       removal.
 *
 * Inputs: uint16_t x - the x coordinate in the matrix
 *         uint16_t y - the y coordinate in the matrix
 *         bool delete_sprite - true to delete sprite from memory
 * Output: none
 */
void SpriteMatrix::removeSprite(uint16_t x, uint16_t y, bool delete_sprite)
{
  if(delete_sprite)
    delete sprite_matrix[x][y];
  sprite_matrix[x][y] = NULL;
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
 * Output: bool - true if the add was successful
 */
bool SpriteMatrix::addFileInformation(XmlData data, int file_index,
                                      SDL_Renderer* renderer,
                                      std::string base_path)
{
  bool success = true;
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::vector<std::string> base_tag = Helpers::split(elements.back(), '_');
  TileSprite* valid_sprite = getValidSprite();

  /* Only proceed if there are elements within the sprites element */
  /*--------------------- MATRIX SPRITE -----------------*/
  if(elements.size() == 2 && elements.front() == "multiple" &&
                             data.getKey(file_index) == "range")
  {
    uint32_t x_max = 0;
    uint32_t x_min = 0;
    uint32_t y_max = 0;
    uint32_t y_min = 0;
    bool good_data = Helpers::parseRange(data.getKeyValue(file_index),
                                         x_min, x_max, y_min, y_max);

    /* Check what the element inside the multiple encapsulator is */
    if(good_data && base_tag.front() == "path")
    {
      std::vector<std::vector<std::string>> str_matrix =
                         Helpers::frameSeparator(data.getDataString(&success));

      /* Modify x_max and y_max if matrix of strings is not large enough */
      if((x_max - x_min) >= str_matrix.size())
        x_max = str_matrix.size() + x_min - 1;
      if((y_max - y_min) >= str_matrix.front().size())
        y_max = str_matrix.front().size() + y_min - 1;
      growMatrix(x_max, y_max);

      /* Go through and set the frames in all relevant sprites */
      for(uint32_t i = x_min; i <= x_max; i++)
      {
        for(uint32_t j = y_min; j <= y_max; j++)
        {
          if(sprite_matrix[i][j] == NULL)
            setSprite(new TileSprite(*valid_sprite), i, j);
          else if(sprite_matrix[i][j]->isFramesSet())
            sprite_matrix[i][j]->removeAll();
          data.addDataOfType(str_matrix[i-x_min][j-y_min]);
          sprite_matrix[i][j]->addFileInformation(data, file_index + 1,
                                                  renderer, base_path, true);
        }
      }
    }
    else if(good_data && base_tag.front() == "passability")
    {
      /* Ensure the range is within the range of existing sprites */
      growMatrix(x_max, y_max);

      /* Fill the range with the passability indicated */
      for(uint32_t i = x_min; i <= x_max; i++)
      {
        for(uint32_t j = y_min; j <= y_max; j++)
        {
          if(sprite_matrix[i][j] == NULL)
            setSprite(new TileSprite(*valid_sprite), i, j);

          sprite_matrix[i][j]->resetPassability();
          sprite_matrix[i][j]->addPassability(data.getDataString());
        }
      }
    }
  }
  /*------------------PATH RANGE SPRITE -----------------*/
  else if(elements.size() == 3 && elements.front() == "x" &&
                                  elements[1] == "y")
  {
    uint32_t index = 0;

    /* Get the x and y coordinate */
    std::vector<std::vector<uint16_t>> x_set =
                          Helpers::parseRangeSet(data.getKeyValue(file_index));
    std::vector<std::vector<uint16_t>> y_set =
                      Helpers::parseRangeSet(data.getKeyValue(file_index + 1));

    /* Run through all the coordinates */
    while(index < x_set.size() && index < y_set.size())
    {
      /* Ensure that the range is sufficient */
      growMatrix(x_set[index].back(), y_set[index].back());

      /* Loop through the range and add it to each one */
      for(uint32_t i = x_set[index].front(); i <= x_set[index].back(); i++)
      {
        for(uint32_t j = y_set[index].front(); j <= y_set[index].back(); j++)
        {
          if(sprite_matrix[i][j] == NULL)
            setSprite(new TileSprite(*valid_sprite), i, j);

          /* Check if it's a path or passability and set the info */
          if(base_tag.front() == "path")
          {
            //if(sprite_matrix[i][j]->isFramesSet())
            //  sprite_matrix[i][j]->removeAll();

            sprite_matrix[i][j]->addFileInformation(
                                    data, file_index + 2, renderer, base_path);
          }
          else if(base_tag.front() == "passability")
          {
            sprite_matrix[i][j]->resetPassability();
            sprite_matrix[i][j]->addPassability(data.getDataString());
          }
        }
      }

      index++;
    }
  }
  /*----------------- PATH SPRITE -----------------*/
  else if(elements.size() == 1 && base_tag.front() == "path")
  {
    sprite_matrix.front().front()->addFileInformation(data, file_index,
                                                      renderer, base_path);
  }
  /*-----------------SPRITE DETAILS -----------------*/
  else if(elements.size() == 2 && elements.front() == "sprite")
  {
    /* Set the new sprite setting in all frames */
    for(uint32_t i = 0; i < sprite_matrix.size(); i++)
      for(uint32_t j = 0; j < sprite_matrix[i].size(); j++)
        if(sprite_matrix[i][j] != NULL)
          sprite_matrix[i][j]->addFileInformation(data, file_index + 1,
                                                  renderer, base_path);
  }

  return success;
}

/*
 * Description: Returns the sprite in the matrix at the given X and Y
 *              coordinate. This function is NOT protected from out of range
 *              seg faults (use getSprite() for protected).
 *
 * Inputs: uint16_t x - the X coordinate in matrix
 *         uint16_t y - the Y coordinate in matrix
 * Output: TileSprite* - the sprite at the given coordinate
 */
TileSprite* SpriteMatrix::at(uint16_t x, uint16_t y)
{
  if(x < width() && y < height())
    return sprite_matrix[x][y];
  return nullptr;
}

/*
 * Description: Takes the sprite matrix, as it's been set up and removes any
 *              rows or columns at the end that have no valid sprites set. A
 *              sprite is classified as valid if it's not NULL and has
 *              renderable frames stored within it.
 *
 * Inputs: none
 * Output: none
 */
void SpriteMatrix::cleanMatrix()
{
  std::vector<int> sprite_depth;
  uint16_t max_y = 0;
  uint16_t new_height = 1;
  uint16_t new_width = 1;

  /* Take into account all frames and trim edges of matrix */
  if(countValidSprites() > 0)
  {
    /* Count up all valid frames */
    for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    {
      int count = -1;

      for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
      {
        if(sprite_matrix[i][j] != NULL)
        {
          if(sprite_matrix[i][j]->isFramesSet())
            count = j;
          else
            removeSprite(i, j);
        }
      }

      if(count > max_y)
        max_y = count;
      sprite_depth.push_back(count);
    }

    /* Set up width and height check parameters */
    bool finished = false;
    new_height = max_y + 1; /* Increment for size, not ref */
    int i = sprite_depth.size() - 1;

    /* Find the new width */
    while(!finished && (i >= 0))
    {
      if(sprite_depth[i] >= 0)
        finished = true;
      else
        i--;
    }
    new_width = i + 1; /* Increment for size, not ref */
  }

  /* Remove parts of matrix, if new dimension is smaller */
  if(sprite_matrix.size() > new_width)
    sprite_matrix.erase(sprite_matrix.begin() + new_width, sprite_matrix.end());
  for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    if(sprite_matrix[i].size() > new_height)
      sprite_matrix[i].erase(sprite_matrix[i].begin() + new_height,
                             sprite_matrix[i].end());
}

/*
 * Description: Returns the animation time of the first valid sprite in the
 *              matrix.
 *
 * Inputs: none
 * Output: uint32_t - the animation time, in ms
 */
uint32_t SpriteMatrix::getAnimationTime()
{
  TileSprite* sprite = getValidSprite();
  if(sprite != NULL)
    return sprite->getAnimationTime();
  return 0;
}

/*
 * Description: Returns the number of frames of the first valid sprite in the
 *              matrix.
 *
 * Inputs: none
 * Output: uint16_t - the number of frames
 */
uint16_t SpriteMatrix::getFrameCount()
{
  TileSprite* sprite = getValidSprite();
  if(sprite != NULL)
    return sprite->getSize();
  return 0;
}

/*
 * Description: Returns the full sprite matrix, stored within the class.
 * Note: Do not delete pointers within the matrix; managed by the class
 *
 * Inputs: none
 * Output: std::vector<std::vector<TileSprite*>> - sprite matrix
 */
std::vector<std::vector<TileSprite*>> SpriteMatrix::getMatrix() const
{
  return sprite_matrix;
}

/*
 * Description: Returns the render matrix of the frames in the matrix, as a
 *              single string. The 'y' direction is separated by '.' and the
 *              'x' direction is separated by ','.
 *              For example: "2,2.0,0" is 2 2 based on map rendering
 *                                        0 0
 *
 * Inputs: none
 * Output: std::string - the linear render matrix, delimited
 */
std::string SpriteMatrix::getRenderMatrix()
{
  std::string linear_matrix;

  if(sprite_matrix.size() > 0)
  {
    /* Parse the entire matrix of sprites */
    for(uint16_t i = 0; i < sprite_matrix.front().size(); i++)
    {
      /* Row delimiter */
      if(i != 0)
        linear_matrix += ".";

      for(uint16_t j = 0; j < sprite_matrix.size(); j++)
      {
        /* Column delimiter */
        if(j != 0)
          linear_matrix += ",";

        /* Find the render value */
        if(sprite_matrix[j][i] != NULL)
          linear_matrix +=
                          std::to_string(sprite_matrix[j][i]->getRenderDepth());
        else
          linear_matrix += "-1";
      }
    }
  }

  return linear_matrix;
}

/*
 * Description: Returns the sprite in the matrix at the given X and Y
 *              coordinate. Returns NULL if out of range or no sprite set
 *              there.
 *
 * Inputs: uint16_t x - the X coordinate in matrix
 *         uint16_t y - the Y coordinate in matrix
 * Output: TileSprite* - the sprite at the given coordinate
 */
TileSprite* SpriteMatrix::getSprite(uint16_t x, uint16_t y)
{
  TileSprite* returned_sprite = NULL;

  if(sprite_matrix.size() > x && sprite_matrix[x].size() > y)
    returned_sprite = sprite_matrix[x][y];

  return returned_sprite;
}

/*
 * Description: Returns the first valid sprite pointer. NULL if none found.
 *
 * Inputs: none
 * Output: TileSprite* - the first valid sprite reference
 */
TileSprite* SpriteMatrix::getSpriteValid()
{
  for(uint32_t i = 0; i < sprite_matrix.size(); i++)
    for(uint32_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != nullptr && sprite_matrix[i][j]->isFramesSet())
        return sprite_matrix[i][j];
  return nullptr;
}

/*
 * Description: Returns the height of the matrix (Y coordinate).
 *
 * Inputs: none
 * Output: uint16_t - unsigned int corresponding to height
 */
uint16_t SpriteMatrix::height() const
{
  if(sprite_matrix.size() > 0)
    return sprite_matrix.back().size();
  return 0;
}

/*
 * Description: Checks if the matrix set of sprites is at the tail of the frame
 *              stack.
 *
 * Inputs: none
 * Output: bool - is the sprite matrix at the end
 */
bool SpriteMatrix::isAtEnd()
{
  TileSprite* sprite = getValidSprite();
  if(sprite != NULL)
    return sprite->isAtEnd();
  return false;
}

/*
 * Description: Checks if the matrix set of sprites is at the front of the
 *              frame stack.
 *
 * Inputs: none
 * Output: bool - is the sprite matrix at the front
 */
bool SpriteMatrix::isAtFirst()
{
  TileSprite* sprite = getValidSprite();
  if(sprite != NULL)
    return sprite->isAtFirst();
  return false;
}

/*
 * Description: Returns if the parsing direction through the frame sequence in
 *              the matrix is forward.
 *
 * Inputs: none
 * Output: bool - true if the parsing direction is forward
 */
bool SpriteMatrix::isDirectionForward()
{
  TileSprite* sprite = getValidSprite();
  if(sprite != NULL)
    return sprite->isDirectionForward();
  return false;
}

/*
 * Description: Returns if the parsing direction through the frame sequence in
 *              the matrix is reverse.
 *
 * Inputs: none
 * Output: bool - true if the parsing direction is reverse
 */
bool SpriteMatrix::isDirectionReverse()
{
  TileSprite* sprite = getValidSprite();
  if(sprite != NULL)
    return !sprite->isDirectionForward();
  return false;
}

/*
 * Description: Render the entire matrix, at the given X and Y (corresponding
 *              to the top left) and with the width, height defined of the
 *              tile. This does no rendering with rendering depths and just
 *              renders the matrix flat.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine
 *         int x - the X coordinate of the top left
 *         int y - the Y coordinate of the top left
 *         int width - width of the tile to render
 *         int height - height of the tile to render
 * Output: bool - true if the matrix rendered
 */
bool SpriteMatrix::render(SDL_Renderer* renderer, int x, int y,
                          int width, int height)
{
  bool rendered = false;

	for(uint32_t i = 0; i < sprite_matrix.size(); i++)
	{
	  int render_x = x + width * i;

	  for(uint32_t j = 0; j < sprite_matrix[i].size(); j++)
	  {
	    if(sprite_matrix[i][j] != NULL)
		  {
		    int render_y = y + height * j;
		    sprite_matrix[i][j]->render(renderer, render_x, render_y,
		                                width, height);
        rendered = true;
		  }
    }
	}

  return rendered;
}

/*
 * Description: Render the entire matrix, at the given X and Y (corresponding
 *              to the top left) and with the width, height defined of the
 *              tile. This does no rendering with rendering depths and just
 *              renders the matrix flat. This renders at the given frame index.
 *
 * Inputs: uint16_t frame - the index of the frame to render
 *         SDL_Renderer* renderer - the rendering engine
 *         int x - the X coordinate of the top left
 *         int y - the Y coordinate of the top left
 *         int width - width of the tile to render
 *         int height - height of the tile to render
 * Output: bool - true if the matrix rendered
 */
bool SpriteMatrix::render(uint16_t frame, SDL_Renderer* renderer, int x,
                          int y, int width, int height)
{
  bool success = true;

  /* First, make sure the matrix is on the right frame */
  success &= shiftTo(frame);

  /* Render the result */
  success &= render(renderer, x, y, width, height);

  return success;
}

/*
 * Description: Sets all sprites in the matrix to the head frame in the stack.
 *
 * Inputs: none
 * Output: bool - was it successful?
 */
bool SpriteMatrix::setAtFirst()
{
  bool success = true;

  for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != nullptr)
        success &= sprite_matrix[i][j]->setAtFirst();

  return success;
}

/*
 * Description: Sets the color mode of all sprites within the matrix.
 *
 * Inputs: ColorMode mode - the mode of how to color the matrix
 * Output: none
 */
void SpriteMatrix::setColorMode(ColorMode mode)
{
  for(uint32_t i = 0; i < sprite_matrix.size(); i++)
    for(uint32_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != nullptr)
        sprite_matrix[i][j]->setColorMode(mode);
}

/*
 * Description: Sets all sprites in the matrix to parse the frames in the stack
 *              in the forward direction.
 *
 * Inputs: none
 * Output: bool - was it successful?
 */
bool SpriteMatrix::setDirectionForward()
{
  bool success = true;

  for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != nullptr)
        success &= sprite_matrix[i][j]->setDirectionForward();

  return success;
}

/*
 * Description: Sets all sprites in the matrix to parse the frames in the stack
 *              in the reverse direction.
 *
 * Inputs: none
 * Output: bool - was it successful?
 */
bool SpriteMatrix::setDirectionReverse()
{
  bool success = true;

  for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != NULL)
        success &= sprite_matrix[i][j]->setDirectionReverse();

  return success;
}

/*
 * Description: Sets the render matrix from a string of data. The data should
 *              have integers delimited by periods for each 'X' set and each
 *              'Y' tile delimited by commas. For example, "0,0.1,1 would be
 *              a render matrix like: 0 0
 *                                    1 1
 *
 * Inputs: std::string render_str - the render data
 * Output: none
 */
// TODO: Possibly move into thing?
void SpriteMatrix::setRenderMatrix(std::string render_str)
{
  /* Parse data */
  std::vector<std::string> rows = Helpers::split(render_str, '.');
  std::vector<std::vector<int16_t>> render_matrix;
  TileSprite* valid_sprite = getValidSprite();

  /* Parse the rendering matrix and separate into integers */
  for(uint32_t i = 0; i < rows.size(); i++)
  {
    std::vector<std::string> split_set = Helpers::split(rows[i], ',');
    std::vector<int16_t> split_int_set;

    /* Do the conversion to integer */
    for(uint32_t j = 0; j < split_set.size(); j++)
      split_int_set.push_back(atoi(split_set[j].c_str()));
    render_matrix.push_back(split_int_set);
  }

  /* Ensure the rendering matrix is capable of frame modifiers added */
  if(render_matrix.size() > 0 && render_matrix.back().size() > 0)
    growMatrix(render_matrix.back().size() - 1, render_matrix.size() - 1);

  /* Go through and set the render matrix level in all sprites */
  for(uint32_t j = 0; j < render_matrix.size(); j++)
  {
    for(uint32_t i = 0; i < render_matrix[j].size(); i++)
    {
      /* Only access data if it's positive */
      if(render_matrix[j][i] >= 0)
      {
        /* Ensure there exists a sprite to modify, and then modify */
        if(sprite_matrix[i][j] == NULL)
          setSprite(new TileSprite(*valid_sprite), i, j);
        sprite_matrix[i][j]->setRenderDepth(render_matrix[j][i]);
      }
    }
  }
}

/*
 * Description: Sets a single sprite in the matrix.
 *
 * Inputs: TileSprite* sprite - the new sprite to insert into the matrix.
 *                              Must actually have a sprite set.
 *         uint32_t x - the x coordinate, relative to the top left of the
 *                      render matrix.
 *         uint32_t y - the y coordinate, relative to the top left of the
 *                      render matrix.
 *         bool delete_old - delete the old frames from memory?
 * Output: bool - returns if the new sprite was set successfully
 */
bool SpriteMatrix::setSprite(TileSprite* sprite, uint16_t x, uint16_t y,
                             bool delete_old)
{
  /* Only proceed if frame isn't NULL */
  if(sprite != NULL)
  {
    /* Either remove existing sprite or grow matrix to accomodate */
    if(sprite_matrix.size() > x && sprite_matrix[x].size() > y)
      removeSprite(x, y, delete_old);
    else
      growMatrix(x, y);

    /* Set the new sprite */
    sprite_matrix[x][y] = sprite;
    return true;
  }
  return false;
}

/*
 * Description: Sets the sprites that defines the matrix
 *
 * Inputs: std::vector<std::vector<TileSprite*>> frames - the new vector set of
 *                                               sprites to place in the matrix.
 *         bool delete_old - delete the old sprites from memory?
 * Output: none
 */
void SpriteMatrix::setSprites(std::vector<std::vector<TileSprite*>> sprites,
                              bool delete_old)
{
  unsetSprites(delete_old);
  sprite_matrix = sprites;
}

/*
 * Description: Sets all sprites in the matrix to go to the next frame in the
 *              stack.
 *
 * Inputs: bool skip_head - should the head of the stack be skipped?
 * Output: bool - was it successful?
 */
bool SpriteMatrix::shiftNext(bool skip_head)
{
  bool success = true;

  for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != NULL)
        success &= sprite_matrix[i][j]->shiftNext(skip_head);

  return success;
}

/*
 * Description: Shifts all frames in the matrix to the given frame index.
 *              0 corresponds to head, 1 to next after head, etc.
 *
 * Inputs: uint16_t index - the number of frames off the head frame
 * Output: bool - true if successful
 */
bool SpriteMatrix::shiftTo(uint16_t index)
{
  bool success = true;

  /* Make sure the matrix is on the right frame */
  if(frame_index != index)
  {
    frame_index = index;

    for(uint16_t i = 0; i < sprite_matrix.size(); i++)
      for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
        if(sprite_matrix[i][j] != NULL)
          success &= sprite_matrix[i][j]->shift(index);
  }

  return success;
}

/*
 * Description: Takes the animation speed and modifies it, based on the
 *              number of frames. This is only used for persons who have
 *              a base animation speed common across the class and modified
 *              for side animations (with different num of frames).
 *
 * Inputs: none
 * Output: none
 */
void SpriteMatrix::tuneAnimationSpeed()
{
  if(sprite_matrix.size() > 0)
  {
    /* Determine animation speed and number of frames */
    TileSprite* corner_sprite = sprite_matrix.front().front();
    if(corner_sprite == NULL)
      corner_sprite = getValidSprite();
    float rate = (corner_sprite->getSize() - 1) / kBASE_FRAME_COUNT;
    uint16_t animation_time = corner_sprite->getAnimationTime() / rate;

    /* Modify each sprite animation time */
    for(uint16_t i = 0; i < sprite_matrix.size(); i++)
      for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
        if(sprite_matrix[i][j] != NULL)
          sprite_matrix[i][j]->setAnimationTime(animation_time);
  }
}

/*
 * Description: Unsets an individual sprite at the x, y coordinate specified.
 *
 * Inputs: uint32_t x - x coordinate of sprite to delete
 *         uint32_t y - y coordinate of sprite to delete
 *         bool delete_sprite - should the old sprite be deleted?
 * Output: none
 */
void SpriteMatrix::unsetSprite(uint16_t x, uint16_t y, bool delete_sprite)
{
  if(sprite_matrix.size() > x && sprite_matrix[x].size() > y)
    removeSprite(x, y, delete_sprite);
}

/*
 * Description: Unsets the sprites from the entire matrix.
 *
 * Inputs: bool delete_sprites - should the old sprites be deleted?
 * Output: none
 */
void SpriteMatrix::unsetSprites(bool delete_sprites)
{
  if(delete_sprites)
  {
    for(uint32_t i = 0; i < sprite_matrix.size(); i++)
      for(uint32_t j = 0; j < sprite_matrix[i].size(); j++)
        removeSprite(i, j);
  }

  sprite_matrix.clear();
}
  
/* Updates the frames within the sprite matrix */
/*
 * Description: Updates the sprite times for animation that fill the matrix.
 *              Necessary for automated animation.
 *
 * Inputs: int cycle_time - the update time that has elapsed, in milliseconds
 *         bool skip_head - skip the head frame when updating?
 *         bool color_only - only updates coloring/greying. Default false
 * Output: bool - returns if the frame changed
 */
bool SpriteMatrix::update(int cycle_time, bool skip_head, bool color_only)
{
  bool frame_changed = false;

  for(uint16_t i = 0; i < sprite_matrix.size(); i++)
    for(uint16_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != nullptr)
        frame_changed |= sprite_matrix[i][j]->update(cycle_time, skip_head,
                                                     color_only);

  return frame_changed;
}

/*
 * Description: Returns the width of the matrix (X coordinate).
 *
 * Inputs: none
 * Output: uint16_t - unsigned int corresponding to width
 */
uint16_t SpriteMatrix::width() const
{
  return sprite_matrix.size();
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              SpriteMatrix.
 *
 * Inputs: const SpriteMatrix &source - the source class constructor
 * Output: SpriteMatrix& - pointer to the copied class
 */
SpriteMatrix& SpriteMatrix::operator= (const SpriteMatrix &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
