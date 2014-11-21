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
  MapItem(Sprite* frames, uint16_t width, uint16_t height, 
          std::string name = "", std::string description = "", 
          int id = kUNSET_ID);

  /* Destructor function */
  virtual ~MapItem();
  
private:
  /* Status if the item is being brightened. Otherwise, darken it */
  bool brighter;
  
  /* The core ID that is connected to the game item */
  int core_id;
  
  /* The number of items available */
  uint16_t count;

  /* States if the map item can be seen */
  //bool visible;
  
  /* States if the map item is picked up by walking over it */
  bool walkover;
  
  /* -------------------------- Constants ------------------------- */
  const static uint8_t kDEFAULT_COUNT; /* Default number of items on setup */
  const static float kDELTA_TIME_ONE_POINT; /* otal time for 1.0 change */
  const static float kMAX_BRIGHTNESS; /* The max brightness setting */
  const static float kMIN_BRIGHTNESS; /* The min brightness setting */
  const static int kUNSET_ID; /* The unset ID value */
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds item information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index, 
                                   int section_index, SDL_Renderer* renderer, 
                                   std::string base_path = "");

  /* Returns the class descriptor, useful for casting */
  virtual std::string classDescriptor();

  /* Clears out the item construct, void of painting */
  void clear();
  
  /* Returns the core (game representation) ID. -1 if unset */
  int getCoreID();
  
  /* Returns the count of how many of these items are available */
  uint16_t getCount();
  
  /* Returns if the Map Item can be seen */
  bool isVisible();
  
  /* Returns if the item is picked up merely by walking over it */
  bool isWalkover();
  
  /* Sets the core (game representation) ID. If invalid, sets to -1 */
  void setCoreID(int id);
  
  /* Sets the number of this item */
  void setCount(uint16_t count);
  
  /* Set the tile to hook the map item to */
  bool setStartingTile(uint16_t section_id, Tile* new_tile, 
                                            bool no_events = false);
  
  /* Sets if the item is picked up by merely walking over it */
  void setWalkover(bool walkover);
  
  /* Updates the thing, based on the tick */
  void update(int cycle_time, std::vector<std::vector<Tile*>> tile_set);
};

#endif // MAPITEM_H
