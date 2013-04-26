/******************************************************************************
* Class Name: Map
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The map class, this is the top level with regard to an actual
*              in-game map. This contains all the tiles that a map is composed
*              of, it also holds pointers to all of the NPC's contained in the
*              maps tiles. This also handles the viewport for the map.
******************************************************************************/
#ifndef MAP_H
#define MAP_H

#include <QKeyEvent>
#include <QtGui/QWidget>

#include "EnumDatabase.h"
#include "Game/Map/MapNPC.h"
#include "Game/Map/Tile.h"
#include "Game/Map/MapMenu.h"
#include "Game/Map/MapStatusBar.h"
#include "Game/Map/Sector.h"
#include "Game/Sprite.h"
#include "Game/Weather.h"
#include "FileHandler.h"

class Map : public QWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  Map(QWidget* parent = 0);

  /* Destructor function */
  ~Map();

private:
  /* Vector of all ai's on map */
  QVector <MapNPC*> ai;

  /* The actual tiles that comprise a map, dynamically sized */
  QVector <QVector <Tile*> > geography;

  /* The menu on the map */
  MapMenu map_menu;

  /* The status bar on the map */
  MapStatusBar map_status_bar;

  /* The players position on the map */
  int playerx, playery;

  /* The sectors on the map (for rooms, caves, houses etc) */
  QVector <Sector> sectors;

  /* The viewoport for the map, also dynamically sized */
  QVector <QVector<Tile*> > viewport;

  /* Weather effect on the overall map (May be pushed to the sector level at 
   * a later time) */
  Weather* weather_effect;

  /*------------------- Constants -----------------------*/
  const static int kFILE_COLUMN; /* The depth in XML of the column tag */
  const static int kFILE_DATA;   /* The depth in XML of the data information */
  const static int kFILE_ROW;    /* The depth in XML of the row tag */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  void paintEvent(QPaintEvent*);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets a pointer to the NPC in the given position in the NPC vector */
  Person* getNPC(int index);

  /* Gets x position of NPC in the given position in the NPC vector */
  int getNPCx(int index);

  /* Gets y position of NPC in the given position in the NPC vector */
  int getNPCy(int index);

   /* Gets players x position */
  int getPlayerX();

  /* Gets players y position */
  int getPlayerY();

  /* Causes the thing you are facing and next to start its interactive action */
  void interact(Direction dir);

  /* Checks whether the viewport contains any tiles with the given sector */
  bool isInSector(int index);

  /* Loads the map */
  bool loadMap(QString file);

  /* Shifts the viewport */
  void move(Direction dir, int step_length, Sprite s);

  /* Checks the tile you are attempting to enter for passibility of the given
  direction */
  bool passible(Direction dir, int x, int y);

  /* Causes the thing you are moving into to start its interactive action */
  void passOver();

  /* Changes NPC spirtes */
  void updateNPC();

  /* Changes the players sprite (Facing direction) */
  void updatePlayer(Sprite s);

  /* Returns a vector of the indexes of the NPC's who are in the viewport */
  QVector<int> visibleNPCs();

  /* Checks if the NPC at the given index in the NPC vector is in the current 
   * viewport */
  bool zNPCInViewport(int index);
};

#endif
