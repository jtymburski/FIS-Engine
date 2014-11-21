/******************************************************************************
 * Class Name: SpriteMatrix
 * Date Created: November 20, 2014
 * Inheritance: none
 * Description: This class contains the logic for matrix of sprites that will
 *              be rendered as a group. 
 *****************************************************************************/
#include "Game/Map/SpriteMatrix.h"

/* Constant Implementation - see header file for descriptions */
//const uint8_t TileSprite::kMAX_RENDER_DEPTH = 10;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

SpriteMatrix::SpriteMatrix()
{
//  render_depth = 0;
//
//  resetPassability();
//  resetTile();
}

SpriteMatrix::SpriteMatrix(std::vector<std::vector<TileSprite*>> sprites)
{

}

SpriteMatrix::SpriteMatrix(const SpriteMatrix &source) : SpriteMatrix()
{
//  render_depth = 0;
//
//  resetPassability();
//  resetTile();
//  
//  copySelf(source);
}

SpriteMatrix::~SpriteMatrix()
{
//  render_depth = 0;
//  
//  resetPassability();
//  resetTile();
//  
//  Sprite::clear();
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

}

/* Counts the valid sprites */
uint16_t SpriteMatrix::countValidSprites()
{

}

/* Gets a valid sprite from the frame matrix. If one doesn't exist, one is
 * made */
TileSprite* SpriteMatrix::getValidSprite()
{

}

/* Grow the sprite matrix to the indicated size */
void SpriteMatrix::growMatrix(uint16_t x, uint16_t y)
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Adds the matrix information from the XML data classifier from file */
bool SpriteMatrix::addFileInformation(XmlData data, int index, 
                                      SDL_Renderer* renderer, 
                                      std::string base_path)
{

}

/* Returns the sprite at a given coordinate */
TileSprite* SpriteMatrix::at(uint16_t x, uint16_t y)
{

}

/* Shrinks the sprite matrix to the valid size and removes all null and
 * void pointers */
void SpriteMatrix::cleanMatrix()
{

}

/* Returns the height of the sprite matrix */
uint16_t SpriteMatrix::getHeight() const
{

}
 
/* Returns the stored sprite matrix */
std::vector<std::vector<TileSprite*>> SpriteMatrix::getMatrix() const
{

}
  
/* Returns the width of the sprite matrix */
uint16_t SpriteMatrix::getWidth() const
{

}

/* Rendering function, for the entire matrix */
bool SpriteMatrix::render(SDL_Renderer* renderer, int start_x, int start_y, 
                          int offset_x, int offset_y)
{

}

/* Sets an individual sprite in the matrix */
bool SpriteMatrix::setSprite(TileSprite* sprite, uint16_t x, uint16_t y, 
                             bool delete_old)
{

}

/* Sets the entire set of sprites in the matrix */
bool SpriteMatrix::setSprites(std::vector<std::vector<TileSprite*>> sprites, 
                              bool delete_old)
{

}

/* Sets all tiles for all entities in the sprite matrix */
bool SpriteMatrix::setTiles(std::vector<std::vector<Tile*>> tiles, 
                            bool fresh_start)
{

}

/* Unsets an individual sprite in the matrix */
void SpriteMatrix::unsetSprite(uint16_t x, uint16_t y, bool delete_sprite)
{

}

/* Unsets the entire matrix of sprites in the matrix */
void SpriteMatrix::unsetSprites(bool delete_sprites)
{

}

/* Unsets all tiles for all entities in the sprite matrix */
void SpriteMatrix::unsetTiles()
{

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
