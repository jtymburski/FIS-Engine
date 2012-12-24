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
#include "Game/Map/MapWalkOver.h"
#include "Game/Sprite.h"

/* Off - Not rendered at all
  Active - Rendered
  Inactive - Blacked out (sector past a door) */
enum Status{STATUSOFF,ACTIVE,INACTIVE};

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
    (eg. Grass, lava, water) */
  Sprite* base;

  /* The lower sprite, impassible (eg. Tree trunk) */
  Sprite* lower;

  /* Player or NPC or impassible item (Causes the passibility of all directions
    to be false if not null) */
  MapInteractiveObject* thing;

  /* The status of the tile */
  Status tile_status;

  /* The upper sprite, fully passible (eg. Treetop) */
  Sprite* upper;

  /* The lower sprite, passible (eg. Bubby, tall grass) */
  MapWalkOver* walkover;

  /* The passibility of each direction of the tile */
  bool north_passibility,east_passibility,south_passibility,west_passibility;

public:
  /* Animates all sprites on tile (Including thing and walkover sprites) */
  void animate();

  /* gets east passiblity */
  bool getPassibilityEast();

  /* gets north passiblity */
  bool getPassibilityNorth();

  /* gets south passiblity */
  bool getPassibilitySouth();

  /* gets west passiblity */
  bool getPassibilityWest();

  /* Sets the base sprite */
  bool setBase(QString path);

  /* Sets all passibility */
  void setPassibility(bool yorn);

  /* Sets east passiblity */
  void setPassibilityEast(bool yorn);

  /* Sets north passiblity */
  void setPassibilityNorth(bool yorn);

  /* Sets south passiblity */
  void setPassibilitySouth(bool yorn);

  /* Sets west passiblity */
  void setPassibilityWest(bool yorn);
};

#endif // TILE_H
