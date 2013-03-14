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

#define STARTX 7
#define STARTY 0
bool turn_on_platform = 1;


/* Constructor function */
Map::Map(QWidget* parent) : QWidget(parent)
{
  for(int i = 0; i < 11; i++)
  {
    QVector<Tile*> row;

    for(int j = 0; j < 19; j++)
    {
      Tile* t = new Tile(64, 64, j*64, i*64, this);

      /* Setup the base sprites */
      t->setBase(":/GrassTile01_AA_A00");

      /* Setup the enhancer sprites */
      if(i > 3 && i < 8 && j > 3 && j < 8)
      {
        if(i == 4 && j == 4)
          t->setEnhancer("", "", ":/WaterTile_AA_A00", ":/WaterTile_AB_A00");
        else if(i == 4 && j == 7)
          t->setEnhancer("", "", ":/WaterTile_AB_A00", ":/WaterTile_AC_A00");
        else if(i == 7 && j == 4)
          t->setEnhancer(":/WaterTile_BA_A00", ":/WaterTile_BB_A00",
                         ":/WaterTile_CA_A00", ":/WaterTile_CB_A00");
        else if(i == 7 && j == 7)
          t->setEnhancer(":/WaterTile_BB_A00", ":/WaterTile_BC_A00",
                         ":/WaterTile_CB_A00", ":/WaterTile_CC_A00");
        else if(i == 4)
          t->setEnhancer("", "", ":/WaterTile_AB_A00", ":/WaterTile_AB_A00");
        else if(i == 7)
          t->setEnhancer(":/WaterTile_BB_A00", ":/WaterTile_BB_A00",
                         ":/WaterTile_CB_A00", ":/WaterTile_CB_A00");
        else if(j == 4)
          t->setEnhancer(":/WaterTile_BA_A00", ":/WaterTile_BB_A00",
                         ":/WaterTile_BA_A00", ":/WaterTile_BB_A00");
        else if(j == 7)
          t->setEnhancer(":/WaterTile_BB_A00", ":/WaterTile_BC_A00",
                         ":/WaterTile_BB_A00", ":/WaterTile_BC_A00");
        else
          t->setEnhancer(":/WaterTile_BB_A00");
      }

      /* Setup the lower sprites */
      if((i == 5 && j == 8) || (i == 6 && j == 8) || (i == 8 && j == 5))
        t->setLower(":/tree_BA_A00");
      else if((i == 5 && j == 9) || (i == 6 && j == 9) || (i == 8 && j == 6))
        t->setLower(":/tree_BB_A00");
      else if(i == 8 && j == 7)
        t->setLower(":/bubby_AA_A00");
      else if(i == 8 && j == 4)
        t->setLower(":/shrub_AA_A00");
      else if(i == 9 && j == 7)
        t->setLower(":/cloud_AA_D00");
      else if(i == 9 && j == 6)
        t->setLower(":/arcadius_AA_U00");
      else if(i == 9 && j == 5)
        t->setLower(":/ulterius_AA_U00");
      else if(i == 9 && j == 4)
        t->setLower(":/aurumba_AA_U00");
      else if(i == 9 && j == 3)
        t->setLower(":/main_AA_U00");
      else if(i == 10 && j == 6)
        t->setLower(":/ulterius_AA_D00");
      else if(i == 10 && j == 5)
        t->setLower(":/aurumba_AA_D00");
      else if(i == 10 && j == 4)
        t->setLower(":/main_AA_D00");
      else if(i == 10 && j == 3)
        t->setLower(":/arcadius_AA_D00");

      /* Setup the upper sprites */
      if((i == 4 && j == 8) || (i == 5 && j == 8) || (i == 7 && j == 5))
        t->setUpper(":/tree_AA_A00");
      else if((i == 4 && j == 9) || (i == 5 && j == 9) || (i == 7 && j == 6))
        t->setUpper(":/tree_AB_A00");


      if(turn_on_platform)
      {
          if((i == STARTY && j == STARTX+1))
              t->setUpper(":/TreePlatform_AB_A00");

          if((i == STARTY && j == STARTX+2))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+3))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+4))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+5))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+6))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+7))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+8))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+9))
              t->setUpper(":/TreePlatform_AC_A00");

          if((i == STARTY && j == STARTX+10))
              t->setUpper(":/TreePlatform_AD_A00");



          if((i == STARTY+1 && j == STARTX))
              t->setUpper(":/TreePlatform_AB_A00");

          if((i == STARTY+1 && j == STARTX+1))
              t->setUpper(":/TreePlatform_BB_A00");

          if((i == STARTY+1 && j == STARTX+2))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+3))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+4))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+5))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+6))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+7))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+8))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+9))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+1 && j == STARTX+10))
              t->setUpper(":/TreePlatform_BD_A00");

          if((i == STARTY+1 && j == STARTX+11))
              t->setUpper(":/TreePlatform_AD_A00");




          if((i == STARTY+2 && j == STARTX))
              t->setUpper(":/TreePlatform_CA_A00");

          if((i == STARTY+2 && j == STARTX+1))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+2))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+3))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+4))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+5))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+6))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+7))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+8))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+9))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+10))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+2 && j == STARTX+11))
              t->setUpper(":/TreePlatform_CE_A00");




          if((i == STARTY+3 && j == STARTX))
              t->setUpper(":/TreePlatform_DA_A00");

          if((i == STARTY+3 && j == STARTX+1))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+2))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+3))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+4))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+5))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+6))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+7))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+8))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+9))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+10))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+3 && j == STARTX+11))
              t->setUpper(":/TreePlatform_DE_A00");



          if((i == STARTY+4 && j == STARTX))
              t->setUpper(":/TreePlatform_DA_A00");

          if((i == STARTY+4 && j == STARTX+1))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+2))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+3))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+4))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+5))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+6))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+7))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+8))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+9))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+10))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+4 && j == STARTX+11))
              t->setUpper(":/TreePlatform_DE_A00");



          if((i == STARTY+5 && j == STARTX))
              t->setUpper(":/TreePlatform_DA_A00");

          if((i == STARTY+5 && j == STARTX+1))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+2))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+3))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+4))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+5))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+6))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+7))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+8))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+9))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+10))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+5 && j == STARTX+11))
              t->setUpper(":/TreePlatform_DE_A00");


          if((i == STARTY+6 && j == STARTX))
              t->setUpper(":/TreePlatform_DA_A00");

          if((i == STARTY+6 && j == STARTX+1))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+2))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+3))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+4))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+5))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+6))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+7))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+8))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+9))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+10))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+6 && j == STARTX+11))
              t->setUpper(":/TreePlatform_DE_A00");




          if((i == STARTY+7 && j == STARTX))
              t->setUpper(":/TreePlatform_EA_A00");

          if((i == STARTY+7 && j == STARTX+1))
              t->setUpper(":/TreePlatform_EB_A00");

          if((i == STARTY+7 && j == STARTX+2))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+3))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+4))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+5))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+6))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+7))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+8))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+9))
              t->setUpper(":/TreePlatform_BC_A00");

          if((i == STARTY+7 && j == STARTX+10))
              t->setUpper(":/TreePlatform_ED_A00");

          if((i == STARTY+7 && j == STARTX+11))
              t->setUpper(":/TreePlatform_EE_A00");




          if((i == STARTY+8 && j == STARTX+1))
              t->setUpper(":/TreePlatform_FB_A00");

          if((i == STARTY+8 && j == STARTX+2))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+3))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+4))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+5))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+6))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+7))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+8))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+9))
              t->setUpper(":/TreePlatform_FC_A00");

          if((i == STARTY+8 && j == STARTX+10))
              t->setUpper(":/TreePlatform_FD_A00");
      }




      row.append(t);
    }
    geography.append(row);
  }
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
void Map::paintEvent(QPaintEvent* event)
{
    (void)event;//warning
}

void Map::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Escape)
    closeMap();
}

void Map::closeMap()
{
  emit closingMap(2);
}

/* Causes the thing you are facing and next to start its interactive action */
void Map::interact(Direction dir)
{
  (void)dir;//warning
}

/* Checks whether the viewport contains any tiles with the given sector */
bool Map::isInSector(int index)
{
    (void)index;//warning
    return TRUE;
}

/* Shifts the viewport */
void Map::move(Direction dir, int step_length, Sprite dir_sprite)
{
    (void)dir;//warning
    (void)step_length;//warning
    (void)dir_sprite;//warning
}

/* Checks the tile you are attempting to enter for passibility of the given
direction */
bool Map::passible(Direction dir, int x, int y)
{
    (void)dir;//warning
    (void)x;//warning
    (void)y;//warning
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
    (void)sprite;//warning
}

/* Returns a vector of the indexes of the NPC's who are in the viewport */
QVector<int> Map::visibleNPCs()
{
    QVector<int> joe;//warning
    return joe;//warning
    //return NULL;
}

/* Checks if the NPC at the given index in the NPC vector is in the current viewport */
bool Map::zNPCInViewport(int index)
{
    (void)index;//warning
    return TRUE;
}

/* Gets a pointer to the NPC in the given position in the NPC vector */
Person* Map::getNPC(int index)
{
    (void)index;//warning
    return NULL;//warning
    //return ai.at(index);
}

/* Gets x position of NPC in the given position in the NPC vector */
int Map::getNPCx(int index)
{
    (void)index;//warning
    return 0;
}

/* Gets y position of NPC in the given position in the NPC vector */
int Map::getNPCy(int index)
{
    (void)index;//warning
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
