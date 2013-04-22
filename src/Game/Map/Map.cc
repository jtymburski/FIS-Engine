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

/* Constant Implementation - see header file for descriptions */
const int Map::kFILE_COLUMN = 3;
const int Map::kFILE_DATA = 4;
const int Map::kFILE_ROW = 2;
const int Map::kSTARTX = 7;
const int Map::kSTARTY = 0;
const bool Map::kTURN_ON_PLATFORM = true;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(QWidget* parent) : QWidget(parent)
{
  qDebug() << "Success: " << loadMap("maps/test_02");
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


/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

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

/*============================================================================
 * SLOTS
 *===========================================================================*/

void Map::closeMap()
{
  emit closingMap(2);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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

/* Causes the thing you are facing and next to start its interactive action */
void Map::interact(Direction dir)
{
  (void)dir;//warning
}

/* Checks whether the viewport contains any tiles with the given sector */
bool Map::isInSector(int index)
{
    (void)index;//warning
    return true;
}

bool Map::loadMap(QString file)
{
  bool done = false;
  bool success = true;
  FileHandler fh(file, false, true);
  XmlData data;

  /* Start the map read */
  success &= fh.start();

  /* If file open was successful, move forward */
  if(success)
  {
    /* Calculate dimensions and set up the map */
    int length = fh.readXmlData().getDataInteger();
    int width = fh.readXmlData().getDataInteger();
    for(int i = 0; i < width; i++)
    {
      QVector<Tile*> row;

      for(int j = 0; j < length; j++)
        row.append(new Tile(64, 64, j*64, i*64, this));

      geography.append(row);
    }
   
    /* Run through the map components and add them to the map */
    data = fh.readXmlData(&done, &success);
    do
    {
      /* Pull the row and column pairs, comma delimited */
      QStringList row_list = data.getKeyValue(kFILE_ROW).split(",");
      QStringList col_list = data.getKeyValue(kFILE_COLUMN).split(",");

      /* Run through this list, checking ranges and add the corresponding
       * tiles */
      for(int i = 0; i < row_list.size(); i++)
      {
        QStringList rows = row_list[i].split("-");
        QStringList cols = col_list[i].split("-");

        /* Use the newly found range and add the tile */
        for(int r = rows[0].toInt(); r <= rows[rows.size() - 1].toInt(); r++)
        {
          for(int c = cols[0].toInt(); c <= cols[cols.size() - 1].toInt(); c++)
          {
            QStringList path = data.getElement(kFILE_DATA).split("_");
            Tile::RotatedAngle angle = Tile::NONE;

            /* Determine the angle to be rotated */
            if(path.size() > 1)
            {
              if(path[1] == "CW")
                angle = Tile::CLOCKWISE;
              else if(path[1] == "CCW")
                angle = Tile::COUNTERCLOCKWISE;
              else if(path[1] == "F")
                angle = Tile::FLIP;
            }

            /* Determine the type of tile */
            if(path[0] == "base")
              geography[r][c]->setBase(data.getDataString(), angle);
            else if(path[0] == "enhancer")
              geography[r][c]->setEnhancer(data.getDataString(), angle);
            else if(path[0] == "lower")
              geography[r][c]->setLower(data.getDataString(), angle);
            else if(path[0] == "upper")
              geography[r][c]->setUpper(data.getDataString(), angle);
          }
        }
      }

      /* Get the next element */
      data = fh.readXmlData(&done, &success);
    } while(!done && success);
  }

  success &= fh.stop();

  return success;
}

/* Shifts the viewport */
void Map::move(Direction dir, int step_length, Sprite dir_sprite)
{
    (void)dir;//warning
    (void)step_length;//warning
    (void)dir_sprite;//warning
}

/* Checks the tile you are attempting to enter for passibility of the given
 * direction */
bool Map::passible(Direction dir, int x, int y)
{
    (void)dir;//warning
    (void)x;//warning
    (void)y;//warning
    return true;
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

/* Checks if the NPC at the given index in the NPC vector is in the current 
 * viewport */
bool Map::zNPCInViewport(int index)
{
    (void)index;//warning
    return true;
}
