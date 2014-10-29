/******************************************************************************
 * Class Name: TileSprite
 * Date Created: October 28, 2014
 * Inheritance: Sprite
 * Description: This class is for sprites that exist within the tile structure
 *              of the map. It adds additional functionality for dealing with
 *              passability and any other map desired functionality.
 *****************************************************************************/
#include "Game/Map/TileSprite.h"

/* Constant Implementation - see header file for descriptions */
//const float MapState::kMAX_OPACITY = 1.0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

TileSprite::TileSprite() : Sprite()
{
  resetPassability();
}

TileSprite::TileSprite(std::string path, SDL_Renderer* renderer)
          : Sprite(path, renderer)
{
  resetPassability();
}

TileSprite::TileSprite(std::string head_path, int num_frames, 
                       std::string tail_path, SDL_Renderer* renderer)
          : Sprite(head_path, num_frames, tail_path, renderer)
{
  resetPassability();
}

/* Copy constructor */
TileSprite::TileSprite(const TileSprite &source) : Sprite()
{
  resetPassability();

  copySelf(source);
}

TileSprite::~TileSprite()
{
  resetPassability();
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

  /* Copy passability information */
  if(source.getPassability(Direction::NORTH))
    setPassability(Direction::NORTH, true);
  if(source.getPassability(Direction::SOUTH))
    setPassability(Direction::SOUTH, true);
  if(source.getPassability(Direction::EAST))
    setPassability(Direction::EAST, true);
  if(source.getPassability(Direction::WEST))
    setPassability(Direction::WEST, true);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Adds sprite information from the XML data classifier from the file */
/* TODO: Comment */
bool TileSprite::addFileInformation(XmlData data, int index, 
                                    SDL_Renderer* renderer, 
                                    std::string base_path)
{
  // TODO: Functionality
  return false;
}

/* Call to add passability, as extracted from file data */
/* TODO: Comment */
bool TileSprite::addPassability(std::string data, std::string classifier, 
                                std::string index)
{
  // TODO: Functionality
  return false;
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
