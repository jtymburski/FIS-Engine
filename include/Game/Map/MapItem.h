/*******************************************************************************
 * Class Name: MapItem
 * Date Created: Oct 28 2012
 * Inheritance: MapThing
 * Description: The item class contains the data that is used for pickups
 *              scattered throughout the map. This class expands upon MapThing
 *              to add counts and if it can be picked up.
 ******************************************************************************/
#ifndef MAPITEM_H
#define MAPITEM_H

#include "Game/Map/MapThing.h"

class MapItem : public MapThing
{
public:
  /* Constructor functions */
  MapItem();
  MapItem(TileSprite* frames, int id = kUNSET_ID, std::string name = "",
          std::string description = "");

  /* Destructor function */
  virtual ~MapItem();

private:
  /* Status if the item is being brightened. Otherwise, darken it */
  bool brighter;

  /* The number of items available */
  uint32_t count;

  /* States if the map item is picked up by walking over it */
  bool walkover;

  /* The number of items in the stack at the start */
  uint32_t start_count;

  /* -------------------------- Constants ------------------------- */
  const static uint8_t kDEFAULT_COUNT; /* Default number of items on setup */
  const static float kDELTA_TIME_ONE_POINT; /* otal time for 1.0 change */
  const static float kMAX_BRIGHTNESS; /* The max brightness setting */
  const static float kMIN_BRIGHTNESS; /* The min brightness setting */

  /*======================= PROTECTED FUNCTIONS ==============================*/
protected:
  /* Check if the tile can be set with the item */
  virtual bool canSetTile(Tile* tile, TileSprite* frames,
                          bool avoid_player = false);

  /* Sets the tile of the selected with the corresponding frames */
  virtual bool setTile(Tile* tile, TileSprite* frames,
                       bool no_events = true);

  /* This unsets the tile, at the given frame coordinate */
  virtual void unsetTile(uint32_t x, uint32_t y, bool no_events);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Adds item information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index,
                                   int section_index, SDL_Renderer* renderer,
                                   std::string base_path = "");

  /* Returns the class descriptor, useful for casting */
  virtual ThingBase classDescriptor();

  /* Shrink the frame matrix to the valid size (1x1) and removes all null and
   * void pointers.  */
  virtual bool cleanMatrix(bool first_call = true);

  /* Clears out the item construct, void of painting */
  void clear();

  /* Returns the count of how many of these items are available */
  uint32_t getCount();

  /* Returns the starting count of items available on map */
  uint32_t getStartCount();

  /* Check if the item is active */
  bool isActive();

  /* Returns if the Map Item can be seen */
  //bool isVisible(); // TODO: DELETE

  /* Returns if the item is picked up merely by walking over it */
  bool isWalkover();

  /* Sets if the item is active */
  virtual bool setActive(bool active, bool set_tiles = true);

  /* Sets the base class */
  virtual bool setBase(MapThing* base);

  /* Sets the number of this item */
  void setCount(uint32_t count);

   /* Sets the state frames of the thing */
  virtual bool setFrame(TileSprite* frame, uint32_t x, uint32_t y,
                        bool delete_old = true);
  virtual void setFrames(std::vector<std::vector<TileSprite*>> frames,
                         bool delete_old = false);

  /* Sets the starting number of this item on the map */
  void setStartCount(uint32_t count);

  /* Sets if the item is picked up by merely walking over it */
  void setWalkover(bool walkover);

  /* Updates the thing, based on the tick */
  Floatinate update(int cycle_time, std::vector<std::vector<Tile*>> tile_set);
};

#endif // MAPITEM_H
