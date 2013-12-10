/******************************************************************************
 * Class Name: Tile
 * Date Created: October 28, 2012
 * Inheritance: none
 * Description: This class handles the basic tile that is set up on the map.
 *              It is the overall structure. The tile class creates a base, 
 *              enhancer, lower, upper, passable, and impassable to define
 *              all the possibilities on the tile. This also handles printing
 *              its own tile data and ensuring that movement isn't entered 
 *              through the tile. For additional information, read the comments
 *              below for each function.
 *****************************************************************************/
#ifndef TILE_H
#define TILE_H

class MapItem;
class MapPerson;
class MapThing;

#include <string>
#include <vector>

#include "EnumDb.h"
#include "Game/EventHandler.h"
#include "Helpers.h"
#include "Sprite.h"

class Tile
{
public:
  /* Constructor functions */
  Tile();
  Tile(EventHandler* event_handler, uint16_t width, uint16_t height, 
       uint16_t x = 0, uint16_t y = 0);

  /* Destructor function */
  ~Tile();
  
  /* OFF - Not rendered at all
   * BLANKED - Blacked out
   * ACTIVE - Rendered */
  enum TileStatus{OFF, BLANKED, ACTIVE};

private:
  /* Basic information for the tile */
  uint16_t height;
  uint16_t width;
  uint16_t x;
  uint16_t y;
  TileStatus status;

  /* The base information */
  Sprite* base;
  uint8_t base_passability;

  /* The enhancer information */
  Sprite* enhancer;

  /* Events for entering and exiting the tile and the handler */
  EventHandler* event_handler;
  Event enter_event;
  Event exit_event;
  
  /* The lower information */
  std::vector<Sprite*> lower;
  std::vector<uint8_t> lower_passability;

  /* The things that are on the given tile - only used to store location */
  MapItem* item;
  MapPerson* person;
  MapThing* thing;

  /* The upper information */
  std::vector<Sprite*> upper;

  /*------------------- Constants -----------------------*/
  const static uint8_t kLOWER_COUNT_MAX; /* The max number of lower layers */
  const static uint8_t kUPPER_COUNT_MAX; /* The max number of upper layers */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Call to add data, as extracted from file data */
  bool addPassability(std::string data, std::string classifier, 
                                        std::string index);
  bool addSprite(Sprite* frames, std::string classifier, std::string index);

  /* Animates all sprites on tile (Including thing and walkover sprites) */
  void animate();

  /* Clears out data from the class */
  void clear(bool just_sprites = false);
  bool clearEvents();
  
  /* Gets the base layer and passability */
  Sprite* getBase();
  bool getBasePassability(Direction dir);

  /* Gets the enhancer layer */
  Sprite* getEnhancer();

  /* Returns the stored height of the tile */
  uint16_t getHeight();
  
  /* Returns the map thing pointer for the item object */
  MapItem* getItem(); 
  
  /* Gets the lower layer(s) */
  std::vector<Sprite*> getLower();
  bool getLowerPassability(Direction dir);
  bool getLowerPassability(uint8_t index, Direction dir);
  
  /* Returns the passability of the tile based on direction */
  bool getPassabilityEntering(Direction dir);
  bool getPassabilityExiting(Direction dir);
  
  /* Returns the person pointer in the tile */
  MapPerson* getPerson();

  /* Returns the tile x and y pixel count */
  int getPixelX();
  int getPixelY();
 
  /* Returns the tile status */
  TileStatus getStatus();

  /* Returns the map thing pointer for the generic thing */
  MapThing* getThing();

  /* Gets the upper layer(s) */
  std::vector<Sprite*> getUpper();

  /* Returns the width of the tile */
  uint16_t getWidth();

  /* Returns the coordinates of the Tile (x or y) */
  uint16_t getX();
  uint16_t getY();

  /* Inserts the lower and upper onto the stack (where applicable). This
   * functionality is essentially entirely handled by Layer */
  bool insertLower(Sprite* lower, uint8_t index);
  bool insertUpper(Sprite* upper, uint8_t index);

  /* Returns if the Base Layer is set (ie. at least one) */
  bool isBaseSet();

  /* Returns if the Enhancer Layer is set */
  bool isEnhancerSet();
  
  /* Returns if an item thing is set */
  bool isItemSet();
  
  /* Returns if the Lower Layer is set */
  bool isLowerSet();

  /* Returns if the map person thing is set */
  bool isPersonSet();

  /* Returns if the generic thing is set */
  bool isThingSet();

  /* Returns if the Upper Layer is set (ie. at least one) */
  bool isUpperSet();
  
  /* Paints the active sprites in this tile using GL direct calls */
  bool renderLower(SDL_Renderer* renderer, int offset_x = 0, int offset_y = 0);
  bool renderUpper(SDL_Renderer* renderer, int offset_x = 0, int offset_y = 0);
  
  /* Sets the base portion of the layer and the passability */
  bool setBase(Sprite* base);
  bool setBasePassability(Direction dir, bool set_value);

  /* Set the enhancer portion of the layer */
  bool setEnhancer(Sprite* enhancer);

  /* Set the enter event, for the tile */
  bool setEnterEvent(Event enter_event);
  
  /* Sets the event handler - this call also clears out all existing events */
  void setEventHandler(EventHandler* event_handler);
  
  /* Set the exit event, for the tile */
  bool setExitEvent(Event exit_event);
  
  /* Sets the new height for the tile (must be >= 0) */
  void setHeight(uint16_t height);

  /* Sets the item thing sprite */
  bool setItem(MapItem* item);
  
  /* Sets the lower portion of the layer(s) and the passability */
  bool setLower(Sprite* lower);
  bool setLowerPassability(uint8_t index, Direction dir, bool set_value);

  /* Sets the stored MapPerson sprite pointer */
  bool setPerson(MapPerson* person, bool no_events = false);

  /* Sets a new status for the tile */
  void setStatus(TileStatus status);

  /* Sets the thing sprite pointer, stored within the class */
  bool setThing(MapThing* thing);
  
  /* Sets the upper portion of the layer */
  bool setUpper(Sprite* upper);

  /* Sets the width of the tile (and all corresponding layers) */
  void setWidth(uint16_t width);

  /* Sets the coordinate location of the tile (and all corresponding layers) */
  void setX(uint16_t x);
  void setY(uint16_t y);
  
  /* Updates the relevent enter and exit events, from file data */
  bool updateEnterEvent(XmlData data, int file_index, uint16_t section_index);
  bool updateExitEvent(XmlData data, int file_index, uint16_t section_index);
  
  /* Unsets the base layer(s) */
  void unsetBase();

  /* Unsets the enhancer layer */
  void unsetEnhancer();

  /* Unsets the item stored within the tile */
  void unsetItem();

  /* Unsets the lower layer */
  void unsetLower();
  bool unsetLower(uint8_t index);

  /* Unsets the stored person pointer */
  void unsetPerson(bool no_events = false);

  /* Unsets the stored thing pointer */
  void unsetThing();

  /* Unsets the upper layer(s) */
  void unsetUpper();
  bool unsetUpper(uint8_t index);

  /* Unsets the stored walkover pointer */
  void unsetWalkOver();
};

#endif // TILE_H
