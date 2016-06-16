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
  /* Past frame index */
  uint16_t frame_index;

  /* Matrix of sprites */
  std::vector<std::vector<TileSprite*>> sprite_matrix;

  /* -------------------------- Constants ------------------------- */
  const static float kBASE_FRAME_COUNT; /* Base num frames for tuning anim */

/*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const SpriteMatrix &source);

  /* Counts the valid sprites */
  uint16_t countValidSprites();

  /* Modify the matrix of paths based on adjustments (needed for multi tile
   * width things */
  std::vector<std::vector<std::string>> flipArray(
                                 std::vector<std::vector<std::string>> original,
                                 std::vector<std::string> adjustments);

  /* Gets a valid sprite from the frame matrix. If one doesn't exist, one is
   * made */
  TileSprite* getValidSprite();

  /* Grow the sprite matrix to the indicated size */
  void growMatrix(uint16_t x, uint16_t y);

  /* Remove sprite from the matrix */
  void removeSprite(uint16_t x, uint16_t y, bool delete_sprite = true);

/*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Adds the matrix information from the XML data classifier from file */
  bool addFileInformation(XmlData data, int file_index, SDL_Renderer* renderer,
                          std::string base_path = "");

  /* Returns the sprite at a given coordinate - unprotected */
  TileSprite* at(uint16_t x, uint16_t y);

  /* Shrinks the sprite matrix to the valid size and removes all null and
   * void pointers */
  void cleanMatrix();

  /* Returns the animation time of the first valid sprite */
  uint32_t getAnimationTime();

  /* Returns the number of frames of the first valid sprite */
  uint16_t getFrameCount();

  /* Returns the stored sprite matrix */
  std::vector<std::vector<TileSprite*>> getMatrix() const;

  /* Returns the render matrix for the stack, in a matrix format */
  std::string getRenderMatrix();

  /* Returns the sprite at a given coordinate - protected */
  TileSprite* getSprite(uint16_t x, uint16_t y);

  /* Returns the first valid sprite within the matrix */
  TileSprite* getSpriteValid();

  /* Returns the height of the sprite matrix */
  uint16_t height() const;

  /* Determines where the reference pointer is in the sprite */
  bool isAtEnd();
  bool isAtFirst();

  /* Determines which shifting direction the layer of sprites in the matrix
   * is utilizing */
  bool isDirectionForward();
  bool isDirectionReverse();

  /* Rendering function, for the entire matrix */
  bool render(SDL_Renderer* renderer, int x, int y,
              int width, int height);
  bool render(uint16_t frame, SDL_Renderer* renderer, int x, int y,
              int width, int height);

  /* Sets all sprites in the matrix at the first frame */
  bool setAtFirst();

  /* Sets the color mode of all sprites within the matrix */
  void setColorMode(ColorMode mode);

  /* Sets the direction of movement for the entire matrix */
  bool setDirectionForward();
  bool setDirectionReverse();

  /* Sets the rendering set corresponding to the matrix */
  void setRenderMatrix(std::string render_str);

  /* Sets an individual sprite in the matrix */
  bool setSprite(TileSprite* sprite, uint16_t x, uint16_t y,
                 bool delete_old = true);

  /* Sets the entire set of sprites in the matrix */
  void setSprites(std::vector<std::vector<TileSprite*>> sprites,
                  bool delete_old = false);

  /* Shifts all the matrix frames to the next in the stack */
  bool shiftNext(bool skip_head = false);

  /* Shifts to the given frame index */
  bool shiftTo(uint16_t index);

  /* Tunes animation speed based on speed which is based on movement of 2
   * frames (total of 3) */
  void tuneAnimationSpeed();

  /* Unsets an individual sprite in the matrix */
  void unsetSprite(uint16_t x, uint16_t y, bool delete_sprite = true);

  /* Unsets the entire matrix of sprites in the matrix */
  void unsetSprites(bool delete_sprites = true);

  /* Returns the width of the sprite matrix */
  uint16_t width() const;

/*======================= OPERATOR FUNCTIONS ===============================*/
public:
  SpriteMatrix& operator= (const SpriteMatrix &source);
};

#endif // SPRITE_MATRIX_H
