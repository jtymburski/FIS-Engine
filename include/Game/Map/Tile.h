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
******************************************************************************/
#ifndef TILE_H
#define TILE_H

//#include <QDebug>
//#include <QGraphicsScene>

#include "EnumDb.h"
//#include "Game/Map/Layer.h"
#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapPerson.h"
#include "Game/Map/MapWalkOver.h"
#include "Game/Sprite.h"

class Tile
{
public:
  /* Constructor functions */
  Tile();
  Tile(int width, int height, int x = 0, int y = 0);

  /* Destructor function */
  ~Tile();
  
  /* UNSET - The impassable object is unused
   * DECOR - The impassable object is an object (Tree, etc)
   * PERSON - The impassable object is a person (player, npc) */
  enum ImpassableObjectState{UNSET, DECOR, PERSON};
  
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

  /* The lower information */
  QList<Sprite*> lower;
  QList<char> lower_passability;

  /* Player or NPC or impassible item (Causes the passibility of all directions
   * to be false if not null). TODO */
  MapInteractiveObject* impassable_object;
  MapPerson* person;
  ImpassableObjectState impassable_set;
  
  /* The passable sprite (eg. Bubby, equipment). TODO */
  MapWalkOver* passable_object;
  bool passable_set;

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

  /* Gets the base layer and passability */
  Sprite* getBase();
  bool getBasePassability(EnumDb::Direction dir);

  /* Gets the enhancer layer */
  Sprite* getEnhancer();

  /* Returns the stored height of the tile */
  int getHeight();

  /* Gets the impassable object sprite */
  MapThing* getImpassableObject();

  /* Gets the lower layer(s) */
  QList<Sprite*> getLower();
  bool getLowerPassability(EnumDb::Direction dir);
  bool getLowerPassability(int index, EnumDb::Direction dir);
  
  /* Gets the passable object sprite */
  MapThing* getPassableObject();

  /* Returns the passability of the tile based on direction */
  bool getPassability(EnumDb::Direction dir);

  /* Returns the tile status */
  TileStatus getStatus();

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
  
  /* Returns if the Impassable Sprite is set */
  ImpassableObjectState isImpassableObjectSet();

  /* Returns if the Lower Layer is set */
  bool isLowerSet();

  /* Returns if the Passable Sprite is set */
  bool isPassableObjectSet();

  /* Returns if the Upper Layer is set (ie. at least one) */
  bool isUpperSet();

  /* Paints the active sprites in this tile using GL direct calls */
  bool paintLower(int x, int y, int width, int height, float opacity);
  bool paintUpper(int x, int y, int width, int height, float opacity);

  /* Sets the base portion of the layer and the passability */
  bool setBase(Sprite* base);
  bool setBasePassability(EnumDb::Direction dir, bool set_value);

  /* Set the enhancer portion of the layer */
  bool setEnhancer(Sprite* enhancer);

  /* Sets the new height for the tile (must be >= 0) */
  bool setHeight(int height);

  /* Sets the impassable object sprite */
  bool setImpassableObject(QString path, ImpassableObjectState type);

  /* Sets the lower portion of the layer(s) and the passability */
  bool setLower(Sprite* lower);
  bool setLowerPassability(int index, EnumDb::Direction dir, bool set_value);

  /* Sets the passable object sprite */
  bool setPassableObject(QString path);

  /* Sets a new status for the tile */
  void setStatus(TileStatus status);

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

  /* Unsets the impassable object sprite */
  bool unsetImpassableObject();

  /* Unsets the lower layer */
  void unsetLower();
  bool unsetLower(int index);

  /* Unsets the passable object sprite */
  bool unsetPassableObject();

  /* Unsets the upper layer(s) */
  void unsetUpper();
  bool unsetUpper(int index);
};

#endif // TILE_H
