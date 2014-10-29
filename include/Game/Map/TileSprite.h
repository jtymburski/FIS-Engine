/******************************************************************************
 * Class Name: TileSprite
 * Date Created: October 28, 2014
 * Inheritance: Sprite
 * Description: This class is for sprites that exist within the tile structure
 *              of the map. It adds additional functionality for dealing with
 *              passability and any other map desired functionality.
 *****************************************************************************/
#ifndef TILESPRITE_H
#define TILESPRITE_H

//#include <string>
//#include <vector>

//#include "EnumDb.h"
//#include "Game/EventHandler.h"
//#include "Helpers.h"
#include "Sprite.h"

class TileSprite : public Sprite
{
public:
  /* Constructor functions */
  TileSprite();
  TileSprite(std::string path, SDL_Renderer* renderer);
  TileSprite(std::string head_path, int num_frames, 
             std::string tail_path, SDL_Renderer* renderer);

  /* Copy constructor */
  TileSprite(const TileSprite &source);

  /* Destructor function */
  ~TileSprite();
  
  /* OFF - Not rendered at all
   * BLANKED - Blacked out
   * ACTIVE - Rendered */
  //enum TileStatus{OFF, BLANKED, ACTIVE};

private:
  /* Passability of the tile sprite */
  uint8_t passability;

  /*------------------- Constants -----------------------*/
  //const static uint8_t kLOWER_COUNT_MAX; /* The max number of lower layers */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const TileSprite &source);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds sprite information from the XML data classifier from the file */
  bool addFileInformation(XmlData data, int index, SDL_Renderer* renderer, 
                          std::string base_path = "");

  /* Call to add passability, as extracted from file data */
  bool addPassability(std::string data, std::string classifier, 
                                        std::string index);
  
  /* Passability class control */
  bool getPassability(Direction dir) const;
  void resetPassability();
  void setPassability(Direction dir, bool set_value);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  TileSprite& operator= (const TileSprite &source);
};

#endif // TILESPRITE_H
