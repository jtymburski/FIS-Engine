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
//const uint8_t TileSprite::kMAX_RENDER_DEPTH = 10;

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
 *              elements get offset based on the given index. Passes along
 *              to parent if no elements are found within child.
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
  std::vector<std::string> base_element = Helpers::split(elements.back(), '_');
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
    if(good_data && base_element.front() == "path")
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
                                                  renderer, base_path);
        }
      }
    }
    else if(good_data && base_element.front() == "passability")
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
          if(base_element.front() == "path")
          {
            if(sprite_matrix[i][j]->isFramesSet())
              sprite_matrix[i][j]->removeAll();

            sprite_matrix[i][j]->addFileInformation(
                                    data, file_index + 2, renderer, base_path);
          }
          else if(base_element.front() == "passability")
          {
            sprite_matrix[i][j]->resetPassability();
            sprite_matrix[i][j]->addPassability(data.getDataString()); 
          }
        }
      }

      index++;
    }
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
  return sprite_matrix[x][y];
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
 * Description: Sets the render matrix from a string of data. The data should
 *              have integers delimited by periods for each 'X' set and each
 *              'Y' tile delimited by commas. For example, "0,0.1,1 would be
 *              a render matrix like: 0 0
 *                                    1 1
 *
 * Inputs: std::string render_str - the render data
 * Output: none
 */
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
 * Description: Sets the tiles that correspond to the rendering location for 
 *              the matrix. This tile set needs to be equal to the size of the 
 *              matrix and each corresponding sprite will be set to the tile.
 *              This does not set the matrix sprite in the Tile.
 *
 * Inputs: std::vector<std::vector<Tile*>> tile_set - the tile matrix
 *         bool fresh_start - set to true if it unsets other tiles.
 *                            false if it initiates a Tile move
 * Output: bool - true if the tiles are set
 */
bool SpriteMatrix::setTiles(std::vector<std::vector<Tile*>> tiles, 
                            bool fresh_start)
{
  bool success = true;

  if(tiles.size() > 0 && tiles.size() == sprite_matrix.size() && 
     tiles.back().size() == sprite_matrix.back().size())
  {
    /* Attempt to set the new tiles */
    for(uint32_t i = 0; success && (i < sprite_matrix.size()); i++)
    {
      for(uint32_t j = 0; j < success && (sprite_matrix[i].size()); j++)
      {
        if(sprite_matrix[i][j] != NULL)
        {
          if(fresh_start)
            success &= sprite_matrix[i][j]->setTile(tiles[i][j]);
          else
            success &= sprite_matrix[i][j]->tileMoveStart(tiles[i][j]);
        }
      }
    }

    /* If unsuccessful, unset all */
    unsetTiles();

    return success;
  }
  return false;
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

/*
 * Description: Unsets the tiles from each sprite in the matrix. This does not
 *              remove the corresponding sprite(s) from the Thing and that
 *              should be done prior to unsetting them here.
 *
 * Inputs: none
 * Output: none
 */
void SpriteMatrix::unsetTiles()
{
  for(uint32_t i = 0; i < sprite_matrix.size(); i++)
    for(uint32_t j = 0; j < sprite_matrix[i].size(); j++)
      if(sprite_matrix[i][j] != NULL)
        sprite_matrix[i][j]->resetTile();
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
