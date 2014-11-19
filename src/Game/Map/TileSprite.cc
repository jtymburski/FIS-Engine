/******************************************************************************
 * Class Name: TileSprite
 * Date Created: October 28, 2014
 * Inheritance: Sprite
 * Description: This class is for sprites that exist within the tile structure
 *              of the map. It adds additional functionality for dealing with
 *              passability and any other map desired functionality. Default
 *              passability is NO passability.
 *****************************************************************************/
#include "Game/Map/TileSprite.h"

/* Constant Implementation - see header file for descriptions */
//const uint8_t TileSprite::kMAX_RENDER_DEPTH = 10;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor function - Set up empty base with no frames
 *
 * Input: none
 */
TileSprite::TileSprite() : Sprite()
{
  render_depth = 0;

  resetPassability();
  resetTile();
}

/* 
 * Description: Constructor function - Set up one frame, using the string path
 *              with an integer rotated angle.
 *
 * Input: std::string path - frame path to set as one sprite
 *        SDL_Renderer* renderer - the rendering engine for creating the image
 */
TileSprite::TileSprite(std::string path, SDL_Renderer* renderer)
          : Sprite(path, renderer)
{
  render_depth = 0;

  resetPassability();
  resetTile();
}

/* 
 * Description: Constructor function - Set up sequence of frames.
 *
 * Inputs: std::string head_path - the start part of the path
 *         int num_frames - the number of frames in this path sequence
 *         std::string tail_path - the end of the path, after the count index
 *         SDL_Renderer* renderer - the rendering engine for creating the images
 */
TileSprite::TileSprite(std::string head_path, int num_frames, 
                       std::string tail_path, SDL_Renderer* renderer)
          : Sprite(head_path, num_frames, tail_path, renderer)
{
  render_depth = 0;

  resetPassability();
  resetTile();
}

/*
 * Description: The copy constructor to transfer all non-graphical parameters
 *              from the master sprite to this one.
 *
 * Inputs: const TileSprite &source - the source sprite data to copy over
 */
TileSprite::TileSprite(const TileSprite &source) : Sprite()
{
  render_depth = 0;

  resetPassability();
  resetTile();
  
  copySelf(source);
}

/* 
 * Description: Destructor function 
 */
TileSprite::~TileSprite()
{
  render_depth = 0;
  
  resetPassability();
  resetTile();
  
  Sprite::clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const Sprite &source - the reference sprite class
 * Output: none
 */
void TileSprite::copySelf(const TileSprite &source)
{
  /* Copy base class information */
  Sprite::copySelf(source);

  /* Copy passability information - don't copy. Independent to each tile */
  /*if(source.getPassability(Direction::NORTH))
    setPassability(Direction::NORTH, true);
  if(source.getPassability(Direction::SOUTH))
    setPassability(Direction::SOUTH, true);
  if(source.getPassability(Direction::EAST))
    setPassability(Direction::EAST, true);
  if(source.getPassability(Direction::WEST))
    setPassability(Direction::WEST, true);*/

  /* Copy render depth information - don't copy. Independent to each tile */
  /*setRenderDepth(source.getRenderDepth());*/
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
bool TileSprite::addFileInformation(XmlData data, int index, 
                                    SDL_Renderer* renderer, 
                                    std::string base_path)
{
  std::string element = data.getElement(index);
  bool success = true;

  /* Parse the tile sprite information - based on the element tag name */
  if(element == "passability")
    addPassability(data.getDataString());
  else if(element == "renderdepth" && data.getDataInteger() >= 0)
    setRenderDepth(data.getDataInteger());
  else
    success &= Sprite::addFileInformation(data, index, renderer, base_path);

  return success;
}

/*
 * Description: Takes a comma delimited list of direction codes and sets
 *              the passability as true for each direction.
 * Example: data = "N,E,S" => Makes all 3 directions passable
 *
 * Inputs: std::string data - comma delimited direction list
 * Output: none
 */
void TileSprite::addPassability(std::string data)
{
  Direction new_direction = Direction::DIRECTIONLESS;
  std::vector<std::string> data_list = Helpers::split(data, ',');
  
  /* Loop through each value of the data list to add */
  for(uint16_t i = 0; i < data_list.size(); i++)
  {
    /* Determine the data identifier first */
    if(data_list[i] == "N")
      new_direction = Direction::NORTH;
    else if(data_list[i] == "E")
      new_direction = Direction::EAST;
    else if(data_list[i] == "S")
      new_direction = Direction::SOUTH;
    else if(data_list[i] == "W")
      new_direction = Direction::WEST;

    /* Add the passability to the specific classifier */
    setPassability(new_direction, true);
  }
}

/* 
 * Description: Gets the passability for the sprite.
 *
 * Inputs: Direction dir - the direction to get the passability for
 * Output: bool - the passability directional status
 */
bool TileSprite::getPassability(Direction dir) const
{
  if(dir == Direction::DIRECTIONLESS)
    return (passability == static_cast<uint8_t>(Direction::DIRECTIONLESS));
  return ((passability & static_cast<uint8_t>(dir)) > 0);
}

/* 
 * Description: Gets the render depth for the individual sprite.
 *
 * Inputs: none
 * Output: uint8_t - render depth integer (0 is base, up to max)
 */
uint8_t TileSprite::getRenderDepth() const
{
  return render_depth;
}

/* 
 * Description: Gets the primary tile, where the sprite is rendered.
 * Note: Do not delete the tile pointer; results would be unknown
 *
 * Inputs: none
 * Output: Tile* - the main tile object pointer
 */
Tile* TileSprite::getTileMain() const
{
  return tile_main;
}

/* 
 * Description: Gets the previous tile, where the sprite was rendered.
 * Note: Do not delete the tile pointer; results would be unknown
 *
 * Inputs: none
 * Output: Tile* - the previous tile object pointer
 */
Tile* TileSprite::getTilePrevious() const
{
  return tile_previous;
}

/*
 * Description: Returns if the main rendering tile is set in the sprite.
 *
 * Inputs: none;
 * Output: bool - true if the tile is set
 */
bool TileSprite::isTileMainSet() const
{
  return (tile_main != NULL);
}

/*
 * Description: Returns if the previous rendering tile is set in the sprite.
 *
 * Inputs: none;
 * Output: bool - true if the tile is set
 */
bool TileSprite::isTilePreviousSet() const
{
  return (tile_previous != NULL);
}
  
/* 
 * Description: Resets the sprite passability back to default state. Default
 *              state is no passability in all directions.
 *
 * Inputs: none
 * Output: none
 */
void TileSprite::resetPassability()
{
  setPassability(Direction::DIRECTIONLESS, true);
}

/* 
 * Description: Resets the tile pointers in this sprite. This does not reset
 *              the corresponding pointer in the tile but just clears the 
 *              reference from this individual sprite.
 *
 * Inputs: none
 * Output: none
 */
void TileSprite::resetTile()
{
  tile_main = NULL;
  tile_previous = NULL;
}

/* 
 * Description: Sets the passability using a given direction and the 
 *              set boolean value.
 *
 * Inputs: Direction dir - the direction to set
 *         bool set_value - the value to set it to.
 * Output: none
 */
void TileSprite::setPassability(Direction dir, bool set_value)
{
  if(dir == Direction::DIRECTIONLESS && set_value)
    passability = static_cast<uint8_t>(Direction::DIRECTIONLESS);
  else
    (set_value) ? (passability |= static_cast<uint8_t>(dir)) 
                : (passability &= ~static_cast<uint8_t>(dir));
}

/* 
 * Description: Sets the render depth. Used for layering the rendering process
 *              on individual tiles. Max is 10 (0-9).
 *
 * Inputs: uint8_t depth - the depth value to set it to
 * Output: bool - status if the depth was in range. If not, nothing happens
 */
bool TileSprite::setRenderDepth(uint8_t depth)
{
  if(depth < Helpers::getRenderDepth())
  {
    render_depth = depth;
    return true;
  }

  return false;
}

/*
 * Description: Sets a new starting tile for rendering and passability handling.
 *              This will also unset any existing tiles, if set.
 *
 * Inputs: Tile* new_tile - the new tile to place the frame
 * Output: bool - true if the tile was set successfully.
 */
bool TileSprite::setTile(Tile* starting_tile)
{
  if(starting_tile != NULL)
  {
    /* Reset the existing */
    resetTile();
    
    /* Set the new */
    tile_main = starting_tile;
    return true;
  }
  return false;
}

/*
 * Description: Finishes a move on the tile. This just clears the previous
 *              pointer of the tile that the sprite was on. This does not call
 *              Tile and make any modifications to the corresponding stored
 *              sprite.
 *
 * Inputs: none
 * Output: none
 */
void TileSprite::tileMoveFinish()
{
  tile_previous = NULL;
}

/*
 * Description: Starts a move on the tile. This moves the new tile, if valid,
 *              to the main pointer and shifts the other to previous to begin
 *              the move process. This does not call Tile and make any 
 *              modifications to the corresponding stored sprite.
 *
 * Inputs: Tile* next_tile - the next tile that the sprite is moving to
 * Output: bool - status if the move was started
 */
bool TileSprite::tileMoveStart(Tile* next_tile)
{
  if(next_tile != NULL)
  {
    tile_previous = tile_main;
    tile_main = next_tile;
    return true;
  }
  return false;
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another TileSprite.
 *
 * Inputs: const TileSprite &source - the source class constructor
 * Output: TileSprite& - pointer to the copied class
 */
TileSprite& TileSprite::operator= (const TileSprite &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;
  
  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
