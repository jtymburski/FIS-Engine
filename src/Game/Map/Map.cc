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
  for(int i = 0; i < 11; i++)
  {
    QVector<Tile*> row;

    for(int j = 0; j < 19; j++)
    {
      Tile* t = new Tile(64, 64, j*64, i*64, this);
      t->setBase(":/grass.png");

      if(i > 3 && i < 8 && j > 3 && j < 8)
      {
        if(i == 4 && j == 4)
          t->setEnhancer("", "", ":/water_NW.png", ":/water_N.png");
        else if(i == 4 && j == 7)
          t->setEnhancer("", "", ":/water_N.png", ":/water_NE.png");
        else if(i == 7 && j == 4)
          t->setEnhancer(":/water_SW.png", ":/water_S.png", "", "");
        else if(i == 7 && j == 7)
          t->setEnhancer(":/water_S.png", ":/water_SE.png", "", "");
        else if(i == 4)
          t->setEnhancer("", "", ":/water_N.png", ":/water_N.png");
        else if(i == 7)
          t->setEnhancer(":/water_S.png", ":/water_S.png", "", "");
        else if(j == 4)
          t->setEnhancer(":/water_W.png", ":/water.png", 
                         ":/water_W.png", ":/water.png");
        else if(j == 7)
          t->setEnhancer(":/water.png", ":/water_E.png",
                         ":/water.png", ":/water_E.png");
        else
          t->setEnhancer(":/water.png");
      }

      row.append(t);
    }
    geography.append(row);
  }

  show();
}

/* Destructor function */
Map::~Map()
{
  for(int i = 0; i < geography.size(); i++)
  {
    for(int j = 0; j < geography[i].size(); j++)
    {
      delete geography[i][j];
      geography[i][j] = NULL;
    }
  }
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
