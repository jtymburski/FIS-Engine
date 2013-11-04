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

//#include <QDebug>

#include "EnumDb.h"
#include "Game/EventHandler.h"
#include "Game/Sprite.h"

class Tile
{
public:
  /* Constructor functions */
  Tile();
  Tile(EventHandler* event_handler, int width, int height, 
       int x = 0, int y = 0);

  /* Destructor function */
  ~Tile();
  
  /* OFF - Not rendered at all
   * BLANKED - Blacked out
   * ACTIVE - Rendered */
  enum TileStatus{OFF, BLANKED, ACTIVE};

private:
  /* Basic information for the tile */
  short height;
  short width;
  short x;
  short y;
  TileStatus status;

  /* The base information */
  Sprite* base;
  char base_passability;

  /* The enhancer information */
  Sprite* enhancer;

  /* Events for entering and exiting the tile and the handler */
  EventHandler* event_handler;
  Event enter_event;
  Event exit_event;
  
  /* The lower information */
  QList<Sprite*> lower;
  QList<char> lower_passability;

  /* The things that are on the given tile - only used to store location */
  MapItem* item;
  MapPerson* person;
  MapThing* thing;

  /* The upper information */
  QList<Sprite*> upper;

  /*------------------- Constants -----------------------*/
  const static char kLOWER_COUNT_MAX; /* The max number of lower layers */
  const static char kUPPER_COUNT_MAX; /* The max number of upper layers */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Call to add data, as extracted from file data */
  bool addPassability(QString data, QString classifier, QString index);
  bool addSprite(Sprite* frames, QString classifier, QString index);

  /* Animates all sprites on tile (Including thing and walkover sprites) */
  void animate();

  /* Clears out data from the class */
  void clear(bool just_sprites = false);
  bool clearEvents();
  
  /* Gets the base layer and passability */
  Sprite* getBase();
  bool getBasePassability(EnumDb::Direction dir);

  /* Gets the enhancer layer */
  Sprite* getEnhancer();

  /* Returns the stored height of the tile */
  int getHeight();
  
  /* Returns the map thing pointer for the item object */
  MapItem* getItem(); 
  
  /* Gets the lower layer(s) */
  QList<Sprite*> getLower();
  bool getLowerPassability(EnumDb::Direction dir);
  bool getLowerPassability(int index, EnumDb::Direction dir);
  
  /* Returns the passability of the tile based on direction */
  bool getPassabilityEntering(EnumDb::Direction dir);
  bool getPassabilityExiting(EnumDb::Direction dir);
  
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
  QList<Sprite*> getUpper();

  /* Returns the width of the tile */
  int getWidth();

  /* Returns the coordinates of the Tile (x or y) */
  int getX();
  int getY();

  /* Initializes GL in all sprites stored within this tile */
  bool initializeGl();

  /* Inserts the lower and upper onto the stack (where applicable). This
   * functionality is essentially entirely handled by Layer */
  bool insertLower(Sprite* lower, int index);
  bool insertUpper(Sprite* upper, int index);

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
  bool paintLower(float offset_x = 0.0, float offset_y = 0.0, 
                                        float opacity = 1.0);
  bool paintUpper(float offset_x = 0.0, float offset_y = 0.0, 
                                        float opacity = 1.0);
  
  /* Sets the base portion of the layer and the passability */
  bool setBase(Sprite* base);
  bool setBasePassability(EnumDb::Direction dir, bool set_value);

  /* Set the enhancer portion of the layer */
  bool setEnhancer(Sprite* enhancer);

  /* Set the enter event, for the tile */
  bool setEnterEvent(Event enter_event);
  
  /* Sets the event handler - this call also clears out all existing events */
  void setEventHandler(EventHandler* event_handler);
  
  /* Set the exit event, for the tile */
  bool setExitEvent(Event exit_event);
  
  /* Sets the new height for the tile (must be >= 0) */
  bool setHeight(int height);

  /* Sets the item thing sprite */
  bool setItem(MapItem* item);
  
  /* Sets the lower portion of the layer(s) and the passability */
  bool setLower(Sprite* lower);
  bool setLowerPassability(int index, EnumDb::Direction dir, bool set_value);

  /* Sets the stored MapPerson sprite pointer */
  bool setPerson(MapPerson* person, bool no_events = false);

  /* Sets a new status for the tile */
  void setStatus(TileStatus status);

  /* Sets the thing sprite pointer, stored within the class */
  bool setThing(MapThing* thing);
  
  /* Sets the upper portion of the layer */
  bool setUpper(Sprite* upper);

  /* Sets the width of the tile (and all corresponding layers) */
  bool setWidth(int width);

  /* Sets the coordinate location of the tile (and all corresponding layers) */
  void setX(int x);
  void setY(int y);
  
  /* Unsets the base layer(s) */
  void unsetBase();

  /* Unsets the enhancer layer */
  void unsetEnhancer();

  /* Unsets the item stored within the tile */
  void unsetItem();

  /* Unsets the lower layer */
  void unsetLower();
  bool unsetLower(int index);

  /* Unsets the stored person pointer */
  void unsetPerson(bool no_events = false);

  /* Unsets the stored thing pointer */
  void unsetThing();

  /* Unsets the upper layer(s) */
  void unsetUpper();
  bool unsetUpper(int index);

  /* Unsets the stored walkover pointer */
  void unsetWalkOver();
};

#endif // TILE_H
