/******************************************************************************
* Class Name: Map
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The map class, this is the top level with regard to an actual
*              in-game map. This contains all the tiles that a map is composed
*              of, it also holds pointers to all of the NPC's contained in the
*              maps tiles. This also handles the viewport for the map.
******************************************************************************/

#include "Game/Map/Map.h"

/* Constructor function */
Map::Map(QWidget* parent)
{
    this->show();
}

/* Destructor function */
Map::~Map()
{
}

/* Painting function */
void Map::paintEvent(QPaintEvent *event)
{
}

/* Causes the thing you are facing and next to start its interactive action */
void Map::interact(Direction dir)
{
}

/* Checks whether the viewport contains any tiles with the given sector */
bool Map::isInSector(int index)
{
    return TRUE;
}

/* Shifts the viewport */
void Map::move(Direction dir, int step_length, Sprite dir_sprite)
{
}

/* Checks the tile you are attempting to enter for passibility of the given
direction */
bool Map::passible(Direction dir, int x, int y)
{
    return TRUE;
}

/* Causes the thing you are moving into to start its interactive action */
void Map::passOver()
{
}

/* Changes NPC spirtes */
void Map::updateNPC()
{
}

/* Changes the players sprite (Facing direction) */
void Map::updatePlayer(Sprite sprite)
{
}

/* Returns a vector of the indexes of the NPC's who are in the viewport */
QVector<int> Map::visibleNPCs()
{
    //return NULL;
}

/* Checks if the NPC at the given index in the NPC vector is in the current viewport */
bool Map::zNPCInViewport(int index)
{
    return TRUE;
}

/* Gets a pointer to the NPC in the given position in the NPC vector */
Person* Map::getNPC(int index)
{
    //return ai.at(index);
}

/* Gets x position of NPC in the given position in the NPC vector */
int Map::getNPCx(int index)
{
    return 0;
}

/* Gets y position of NPC in the given position in the NPC vector */
int Map::getNPCy(int index)
{
    return 0;
}

 /* Gets players x position */
int Map::getPlayerX()
{
    return playerx;
}

/* Gets players y position */
int Map::getPlayerY()
{
    return playery;
}
