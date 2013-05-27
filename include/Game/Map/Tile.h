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

#include <QObject>

#include "Game/Map/Layer.h"
#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapPerson.h"
#include "Game/Map/MapWalkOver.h"
#include "XmlData.h"

class Tile : public QObject
{
    Q_OBJECT

public:
  /* Constructor functions */
  Tile();
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
  enum Status{OFF, ACTIVE, INACTIVE};

private:
  /* Basic information for the tile */
  int height;
  int width;
  int x;
  int y;

  /* The lowest level of sprite on tile, passibility varies based on tile
   * (eg. Grass, lava, water) */
  QVector<Layer*> base;

  /* The enhancment layer on the base. This is things like water bodies, 
   * ground enhancers, etc. */
  Layer* enhancer;

  /* Player or NPC or impassible item (Causes the passibility of all directions
   * to be false if not null) */
  MapInteractiveObject* impassable_object;
  MapPerson* person;
  ImpassableObjectState impassable_set;
  
  /* The lower layer (Tree Trunks, shrubs, etc) */
  Layer* lower;

  /* The lower sprite, passible (eg. Bubby, equipment) */
  MapWalkOver* passable_object;
  bool passable_set;

  /* The status of the tile */
  Status tile_status;

  /* The upper sprite(s), fully pass under */
  QVector<Layer*> upper;

  /* The passibility of each direction of the tile */
  bool north_passibility,east_passibility,south_passibility,west_passibility;

  /*------------------- Constants -----------------------*/
  const static int kBASE_COUNT_MAX;   /* The maximum number of base layers */
  const static int kBASE_DEPTH;       /* The starting base layer depth */
  const static int kDATA_ELEMENT;     /* The inserted data element location */
  const static int kDATA_ROTATION;    /* The inserted data rotation location */
  const static int kENHANCER_DEPTH;   /* The enhancer layer depth */
  const static int kLOWER_DEPTH;      /* The lower layer depth */
  const static int kMAP_INTERACTIVE_DEPTH; /* The interactive object depth */
  const static int kMAP_PERSON_DEPTH; /* The Map person layer depth */
  const static int kNE_ENHANCER;      /* The NE enhancer quarter index */
  const static int kNW_ENHANCER;      /* The NW enhancer quarter index */
  const static int kSE_ENHANCER;      /* The SE enhancer quarter index */
  const static int kSW_ENHANCER;      /* The SW enhancer quarter index */
  const static int kUPPER_COUNT_MAX;  /* The max number of upper layers */
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

  /* Sets each layer to if its enabled or not */
  void setEnabled(bool enabled);

  /* Sets each layer to if its visible or not (otherwise black square) */
  void setVisible(bool visible);

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
 
  /* Adds data from the file to this tile. Returns if valid */
  bool addData(XmlData data, QPixmap image, bool append = false);

  /* Adds another upper layer to the stack */
  Layer* addUpper(Sprite* upper_sprite, RotatedAngle angle = NONE);
  
  /* Animates all sprites on tile (Including thing and walkover sprites) */
  void animate();

  /* Append to the layers - this just attempts to add image data to the tail
   * end of the sequence of the layer. */
  bool appendBase(QPixmap frame, RotatedAngle angle);
  bool appendEnhancer(QPixmap frame, RotatedAngle angle);
  bool appendLower(QPixmap frame, RotatedAngle angle);
  bool appendUpper(QPixmap frame, RotatedAngle angle);

  /* Gets the base layer(s) */
  QVector<Layer*> getBase();

  /* Gets the enhancer layer */
  Layer* getEnhancer();

  /* Returns the stored height of the tile */
  int getHeight();

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

  /* Returns the tile status */
  Status getStatus();

  /* Gets the upper layer(s) */
  QVector<Layer*> getUpper();

  /* Returns the width of the tile */
  int getWidth();

  /* Returns the coordinates of the Tile (x or y) */
  int getX();
  int getY();

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

  /* Sets the new height for the tile (must be >= 0) */
  bool setHeight(int height);

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

  /* Sets a new status for the tile */
  void setStatus(Status updated_status);

  /* Sets the width of the tile (and all corresponding layers) */
  bool setWidth(int width);

  /* Sets the coordinate location of the tile (and all corresponding layers) */
  void setX(int x);
  void setY(int y);

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
