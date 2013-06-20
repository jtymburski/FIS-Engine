/******************************************************************************
* Class Name: Tile
* Date Created: October 28, 2012
* Inheritance: QObject
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

#include <QGraphicsScene>

#include "Game/Map/Layer.h"
#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapPerson.h"
#include "Game/Map/MapWalkOver.h"

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

private:
  /* Basic information for the tile */
  int height;
  int width;
  int x;
  int y;

  /* The two primary layers of the tile */
  Layer* lower;
  Layer* upper;

  /* Player or NPC or impassible item (Causes the passibility of all directions
   * to be false if not null) */
  MapInteractiveObject* impassable_object;
  MapPerson* person;
  ImpassableObjectState impassable_set;
  
  /* The lower sprite, passible (eg. Bubby, equipment) */
  MapWalkOver* passable_object;
  bool passable_set;

  /* The passibility of each direction of the tile */
  bool north_passibility,east_passibility,south_passibility,west_passibility;

  /*------------------- Constants -----------------------*/
  const static int kLOWER_DEPTH;      /* The lower layer depth */
  const static int kUPPER_DEPTH;      /* The starting upper layer depth */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Animates all sprites on tile (Including thing and walkover sprites) */
  void animate();

  /* Append the lower and upper onto the stack (where applicable). This
   * functionality is essentially entirely handled by Layer */
  bool appendLower(Sprite* lower);
  bool appendUpper(Sprite* upper);

  /* Gets the base layer(s) */
  Sprite* getBase();

  /* Gets the enhancer layer */
  Sprite* getEnhancer();

  /* Returns the stored height of the tile */
  int getHeight();

  /* Gets the impassable object sprite */
  MapThing* getImpassableObject();

  /* Gets the lower layer */
  QList<Sprite*> getLower();

  /* Gets the passable object sprite */
  MapThing* getPassableObject();

  /* Gets east passiblity */
  bool getPassibilityEast();

  /* Gets north passiblity */
  bool getPassibilityNorth();

  /* Gets south passiblity */
  bool getPassibilitySouth();

  /* Gets west passiblity */
  bool getPassibilityWest();

  /* Returns the tile status */
  Layer::Status getStatus();

  /* Gets the upper layer(s) */
  QList<Sprite*> getUpper();

  /* Returns the width of the tile */
  int getWidth();

  /* Returns the coordinates of the Tile (x or y) */
  int getX();
  int getY();

  /* Insert the items in the class into the scene */
  bool insertIntoScene(QGraphicsScene* scene);

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

  /* Removes the items internally to this class from the scene */
  void removeFromScene(QGraphicsScene* scene);

  /* Sets the base portion of the layer */
  bool setBase(Sprite* base);

  /* Set the enhancer portion of the layer */
  bool setEnhancer(Sprite* enhancer);

  /* Sets the new height for the tile (must be >= 0) */
  bool setHeight(int height);

  /* Sets the impassable object sprite */
  bool setImpassableObject(QString path, ImpassableObjectState type);

  /* Sets the lower portion of the layer */
  bool setLower(Sprite* lower);

  /* Sets the passable object sprite */
  bool setPassableObject(QString path);

  /* Sets all passibility */
  void setPassibility(bool is_passable);
  void setPassibility(bool north_is_passable, bool east_is_passable,
                      bool south_is_passable, bool west_is_passable);

  /* Sets east passiblity */
  void setPassibilityEast(bool is_passable);

  /* Sets north passiblity */
  void setPassibilityNorth(bool is_passable);

  /* Sets south passiblity */
  void setPassibilitySouth(bool is_passable);

  /* Sets west passiblity */
  void setPassibilityWest(bool is_passable);

  /* Sets a new status for the tile */
  void setStatus(Layer::Status updated_status);

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

  /* Unsets the passable object sprite */
  bool unsetPassableObject();

  /* Unsets the upper layer(s) */
  void unsetUpper();
};

#endif // TILE_H
