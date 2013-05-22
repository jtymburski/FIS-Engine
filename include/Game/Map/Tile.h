/******************************************************************************
* Class Name: Tile
* Date Created: Oct 28 2012
* Inheritance: QObject
* Description: The Tile class
******************************************************************************/
#ifndef TILE_H
#define TILE_H

#include <QObject>

#include "Game/Map/Layer.h"
#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapPerson.h"
#include "Game/Map/MapWalkOver.h"

class Tile : public QObject
{
    Q_OBJECT

public:
  /* Constructor function */
  Tile(int width, int height, int x = 0, int y = 0, QObject* parent = 0);

  /* Destructor function */
  ~Tile();
  
  /* UNSET - The impassable object is unused
   * DECOR - The impassable object is an object (Tree, etc)
   * PERSON - The impassable object is a person (player, npc) */
  enum ImpassableObjectState{UNSET, DECOR, PERSON};

  /* NONE - the object isn't rotated
   * CLOCKWISE - rotate the object 90 degrees clockwise
   * COUNTERCLOCKWISE - rotate the object 90 degrees counterclockwise
   * FLIP - rotate the object 180 degrees */
  enum RotatedAngle{NONE, CLOCKWISE, COUNTERCLOCKWISE, FLIP};

  /* STATUSOFF - Not rendered at all
   * ACTIVE - Rendered
   * INACTIVE - Blacked out (sector past a door) */
  enum Status{STATUSOFF, ACTIVE, INACTIVE};

private:
  /* Basic information for the tile */
  int height;
  int width;
  int x;
  int y;

  /* The lowest level of sprite on tile, passibility varies based on tile
   * (eg. Grass, lava, water) */
  QVector<Layer*> base;
  bool base_set;

  /* The enhancment layer on the base. This is things like water bodies, 
   * ground enhancers, etc. */
  Layer* enhancer;
  bool enhancer_set;

  /* Player or NPC or impassible item (Causes the passibility of all directions
   * to be false if not null) */
  MapInteractiveObject* impassable_object;
  MapPerson* person;
  ImpassableObjectState impassable_set;
  
  /* The lower layer (Tree Trunks, shrubs, etc) */
  Layer* lower;
  bool lower_set;

  /* The lower sprite, passible (eg. Bubby, equipment) */
  MapWalkOver* passable_object;
  bool passable_set;

  /* The status of the tile */
  Status tile_status;

  /* The upper sprite(s), fully pass under */
  QVector<Layer*> upper;
  bool upper_set;

  /* The passibility of each direction of the tile */
  bool north_passibility,east_passibility,south_passibility,west_passibility;

  /*------------------- Constants -----------------------*/
  const static int kBASE_DEPTH;       /* The starting base layer depth */
  const static int kENHANCER_DEPTH;   /* The enhancer layer depth */
  const static int kLOWER_DEPTH;      /* The lower layer depth */
  const static int kMAP_INTERACTIVE_DEPTH; /* The interactive object depth */
  const static int kMAP_PERSON_DEPTH; /* The Map person layer depth */
  const static int kMAX_BASE_COUNT;   /* The maximum number of base layers */
  const static int kMAX_UPPER_COUNT;  /* The max number of upper layers */
  const static int kNE_ENHANCER;      /* The NE enhancer quarter index */
  const static int kNW_ENHANCER;      /* The NW enhancer quarter index */
  const static int kSE_ENHANCER;      /* The SE enhancer quarter index */
  const static int kSW_ENHANCER;      /* The SW enhancer quarter index */
  const static int kUPPER_DEPTH;      /* The starting upper layer depth */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Determines the angle that's associated to the local enumerator */
  int getAngle(RotatedAngle angle);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void addLayer(Layer* item);
  void deleteLayer(Layer* item);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds another base layer to the stack */
  Layer* addBase(Sprite* base_sprite, RotatedAngle angle = NONE);
  
  /* Adds another upper layer to the stack */
  Layer* addUpper(Sprite* upper_sprite, RotatedAngle angle = NONE);
  
  /* Animates all sprites on tile (Including thing and walkover sprites) */
  void animate();

  /* Gets the base layer(s) */
  QVector<Layer*> getBase();

  /* Gets the enhancer layer */
  Layer* getEnhancer();

  /* Gets the impassable object sprite */
  MapThing* getImpassableObject();

  /* Gets the lower layer */
  Layer* getLower();

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

  /* Gets the upper layer(s) */
  QVector<Layer*> getUpper();

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

  /* Set the enhancer layer */
  Layer* setEnhancer(Sprite* enhancer_sprite, RotatedAngle angle = NONE);

  /* Sets the impassable object sprite */
  bool setImpassableObject(QString path, ImpassableObjectState type);

  /* Sets the lower layer */
  Layer* setLower(Sprite* lower_sprite, RotatedAngle angle = NONE);

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

  /* Unsets the base layer(s) */
  bool unsetBase();

  /* Unsets the enhancer layer */
  bool unsetEnhancer();

  /* Unsets the impassable object sprite */
  bool unsetImpassableObject();

  /* Unsets the lower layer */
  bool unsetLower();

  /* Unsets the passable object sprite */
  bool unsetPassableObject();

  /* Unsets the upper layer(s) */
  bool unsetUpper();
};

#endif // TILE_H
