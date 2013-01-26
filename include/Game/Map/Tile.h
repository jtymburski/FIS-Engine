/******************************************************************************
* Class Name: Tile
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The Tile class
******************************************************************************/
#ifndef TILE_H
#define TILE_H

#include <QtGui/QWidget>
#include <QtGui/QPainter>

#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapPerson.h"
#include "Game/Map/MapWalkOver.h"
#include "Game/Sprite.h"

/* STATUSOFF - Not rendered at all
 * ACTIVE - Rendered
 * INACTIVE - Blacked out (sector past a door) */
enum Status{STATUSOFF, ACTIVE, INACTIVE};

/* UNSET - The impassable object is unused
 * DECOR - The impassable object is an object (Tree, etc)
 * PERSON - The impassable object is a person (player, npc) */
enum ImpassableObjectState{UNSET, DECOR, PERSON};

class Tile : public QWidget
{
public:
  /* Constructor function */
  Tile(int width, int height, int x = 0, int y = 0, QWidget* parent = 0);

  /* Destructor function */
  ~Tile();

protected:
  void paintEvent(QPaintEvent*);

private:
  /* The lowest level of sprite on tile, passibility varies based on tile
   * (eg. Grass, lava, water) */
  Sprite* base;
  bool base_set;

  /* The enhancment layer on the base. This is things like water bodies, 
   * ground enhancers, etc. */
  QVector<Sprite*> enhancer;
  bool enhancer_set;

  /* Player or NPC or impassible item (Causes the passibility of all directions
   * to be false if not null) */
  MapInteractiveObject* impassable_object;
  MapPerson* person;
  ImpassableObjectState impassable_set;
  
  /* The lower sprite, impassible (eg. Tree trunk) */
  Sprite* lower;
  bool lower_set;

  /* The lower sprite, passible (eg. Bubby, equipment) */
  MapWalkOver* passable_object;
  bool passable_set;

  /* The status of the tile */
  Status tile_status;

  /* The upper sprite, fully passible (eg. Treetop) */
  Sprite* upper;
  bool upper_set;

  /* The passibility of each direction of the tile */
  bool north_passibility,east_passibility,south_passibility,west_passibility;

public:
  /* Animates all sprites on tile (Including thing and walkover sprites) */
  void animate();

  /* Gets the base sprite */
  Sprite* getBase();

  /* Gets the enhancer sprite qvector */
  QVector<Sprite*> getEnhancer();

  /* Gets the impassable object sprite */
  MapThing* getImpassableObject();

  /* Gets the lower sprite */
  Sprite* getLower();

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

  /* Gets the upper sprite */
  Sprite* getUpper();

  /* Returns if the Base Sprite is set */
  bool isBaseSet();

  /* Returns if the Enhancer Sprite(s) is set */
  bool isEnhancerSet();
  
  /* Returns if the Impassable Sprite is set */
  ImpassableObjectState isImpassableObjectSet();

  /* Returns if the Lower Sprite is set */
  bool isLowerSet();

  /* Returns if the Passable Sprite is set */
  bool isPassableObjectSet();

  /* Returns if the Upper Sprite is set */
  bool isUpperSet();

  /* Sets the base sprite */
  bool setBase(QString path);

  /* Set the enhancer sprite */
  bool setEnhancer(QString path);
  bool setEnhancer(QString nw_path, QString ne_path, 
                   QString sw_path, QString se_path);

  /* Sets the impassable object sprite */
  bool setImpassableObject(QString path, ImpassableObjectState type);

  /* Sets the lower sprite */
  bool setLower(QString path);

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

  /* Sets the upper sprite */
  bool setUpper(QString path);

  /* Unsets the base sprite */
  bool unsetBase();

  /* Unsets the enhancer sprite(s) */
  bool unsetEnhancer();

  /* Unsets the impassable object sprite */
  bool unsetImpassableObject();

  /* Unsets the lower sprite */
  bool unsetLower();

  /* Unsets the passable object sprite */
  bool unsetPassableObject();

  /* Unsets the upper sprite */
  bool unsetUpper();
};

#endif // TILE_H
