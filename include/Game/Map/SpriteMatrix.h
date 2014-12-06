/******************************************************************************
 * Class Name: SpriteMatrix
 * Date Created: November 20, 2014
 * Inheritance: none
 * Description: This class contains the logic for matrix of sprites that will
 *              be rendered as a group. Primarily utilized by MapThing and 
 *              children.
 *****************************************************************************/
#ifndef SPRITEMATRIX_H
#define SPRITEMATRIX_H

#include "Game/Map/TileSprite.h"

class SpriteMatrix
{
public:
  /* Constructor functions */
  SpriteMatrix();
  SpriteMatrix(std::vector<std::vector<TileSprite*>> sprites);

  /* Copy constructor */
  SpriteMatrix(const SpriteMatrix &source);

  /* Destructor function */
  ~SpriteMatrix();
  
private:
  /* Matrix of sprites */
  std::vector<std::vector<TileSprite*>> sprite_matrix;

  /* -------------------------- Constants ------------------------- */
  const static float kBASE_FRAME_COUNT; /* Base num frames for tuning anim */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const SpriteMatrix &source);

  /* Counts the valid sprites */
  uint16_t countValidSprites();

  /* Gets a valid sprite from the frame matrix. If one doesn't exist, one is
   * made */
  TileSprite* getValidSprite();

  /* Grow the sprite matrix to the indicated size */
  void growMatrix(uint16_t x, uint16_t y);

  /* Remove sprite from the matrix */
  void removeSprite(uint16_t x, uint16_t y, bool delete_sprite = true);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds the matrix information from the XML data classifier from file */
  bool addFileInformation(XmlData data, int file_index, SDL_Renderer* renderer, 
                          std::string base_path = "");

  /* Returns the sprite at a given coordinate - unprotected */
  TileSprite* at(uint16_t x, uint16_t y);

  /* Shrinks the sprite matrix to the valid size and removes all null and
   * void pointers */
  void cleanMatrix();

  /* Returns the stored sprite matrix */
  std::vector<std::vector<TileSprite*>> getMatrix() const;
  
  /* Returns the sprite at a given coordinate - protected */
  TileSprite* getSprite(uint16_t x, uint16_t y);

  /* Returns the height of the sprite matrix */
  uint16_t height() const;

  /* Rendering function, for the entire matrix */
  bool render(SDL_Renderer* renderer, int start_x, int start_y, 
              int width, int height);

  /* Sets the rendering set corresponding to the matrix */
  void setRenderMatrix(std::string render_str);

  /* Sets an individual sprite in the matrix */
  bool setSprite(TileSprite* sprite, uint16_t x, uint16_t y, 
                 bool delete_old = true);

  /* Sets the entire set of sprites in the matrix */
  void setSprites(std::vector<std::vector<TileSprite*>> sprites, 
                  bool delete_old = false);

  /* Tunes animation speed based on speed which is based on movement of 2
   * frames (total of 3) */
  void tuneAnimationSpeed();

  /* Unsets an individual sprite in the matrix */
  void unsetSprite(uint16_t x, uint16_t y, bool delete_sprite = true);

  /* Unsets the entire matrix of sprites in the matrix */
  void unsetSprites(bool delete_sprites = true);

  /* Returns the width of the sprite matrix */
  uint16_t width() const;

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  SpriteMatrix& operator= (const SpriteMatrix &source);
};

#endif // SPRITE_MATRIX_H
