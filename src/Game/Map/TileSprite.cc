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
  // TODO: passability initialization
}

TileSprite::TileSprite(std::string path, SDL_Renderer* renderer)
          : Sprite(path, renderer)
{
  // TODO: passability initialization
}

TileSprite::TileSprite(std::string head_path, int num_frames, 
                       std::string tail_path, SDL_Renderer* renderer)
          : Sprite(head_path, num_frames, tail_path, renderer)
{
  // TODO: passability initialization
}

/* Copy constructor */
TileSprite::TileSprite(const TileSprite &source) : Sprite()
{
  copySelf(source);
}

TileSprite::~TileSprite()
{
  // TODO: Add this class cleanup
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
  // TODO: Passability copying

  //setAnimationTime(source.getAnimationTime());
  //setBrightness(source.getBrightness());
  //setColorBalance(source.getColorRed(), source.getColorGreen(), 
  //                                      source.getColorBlue());
  //if(source.isDirectionForward())
  //  setDirectionForward();
  //else
  //  setDirectionReverse();
  //setOpacity(source.getOpacity());
  //setRotation(source.getRotation());

  Sprite::copySelf(source);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Clear out the state definition */
/*void MapState::clear()
{
  clearEvents();
  unsetSprite();
}*/

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
