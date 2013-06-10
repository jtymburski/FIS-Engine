/******************************************************************************
* Class Name: Map
* Date Created: Dec 2 2012
* Inheritance: QGraphicsScene
* Description: The map class, this is the top level with regard to an actual
*              in-game map. This contains all the tiles that a map is composed
*              of, it also holds pointers to all of the NPC's contained in the
*              maps tiles. This also handles the viewport for the map.
******************************************************************************/
#include "Game/Map/Map.h"

/* Constant Implementation - see header file for descriptions */
const int Map::kDOUBLE_DIGITS = 10;
const int Map::kFILE_SECTION_ID = 2;
const int Map::kTILE_COLUMN = 4;
const int Map::kTILE_DATA = 5;
const int Map::kTILE_LENGTH = 64;
const int Map::kTILE_ROW = 3;
const int Map::kTILE_WIDTH = 64;
const int Map::kVIEWPORT_LENGTH = 19;
const int Map::kVIEWPORT_WIDTH = 11;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(short resolution_x, short resolution_y)
{
  /* Configure the scene */
  loaded = false;
  player = 0;
 
  /* Setup the OpenGL Widget */
  QGLFormat gl_format(QGL::SampleBuffers);
  gl_format.setSwapInterval(1);
  viewport_widget = new QGLWidget(gl_format);

  /* Setup the viewport */
  viewport = new MapViewport(this, resolution_x, resolution_y);
  viewport->setViewport(viewport_widget);
  viewport->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    /* FullViewportUpdate, SmartViewportUpdate, MinimalViewportUpdate */
  viewport->centerOn(0, 0);
 
  /* Connect the signals */
  QObject::connect(viewport, SIGNAL(animateTiles()),
                   this,     SLOT(animateTiles()));
  QObject::connect(viewport, SIGNAL(closingMap(int)),
                   this,     SIGNAL(closingMap(int)));

  /* Bring the timer in to provide a game tick */
  connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer.start(20);
}

/* Destructor function */
Map::~Map()
{
  unloadMap();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

bool Map::addTileData(XmlData data)
{
  bool success = true;

  /* Pull the row and column pairs, comma delimited */
  QStringList row_list = data.getKeyValue(kTILE_ROW).split(",");
  QStringList col_list = data.getKeyValue(kTILE_COLUMN).split(",");
  QPixmap image = Frame::openImage(data.getDataString());

  /* Handle the image data, split the path based on "|" */
  QVector<QPixmap> images;
  QStringList data_list = data.getDataString().split("|");
  QString path;
  QString path_tail;

  /* If the split occurred, there are multiple paths to add */
  if(data_list.size() > 1)
  {
    QString path = data_list[0]; /* The front half */
    QString path_tail = data_list[2]; /* The latter half */
    data_list = data_list[1].split("-"); /* The middle (integer range) */

    /* Loop through the number of path options (|0-2|), 00 - 02 */
    for(int i = data_list[0].toInt(); i <= data_list[1].toInt(); i++)
    {
      QString image_path;
      if(i < kDOUBLE_DIGITS)
        image_path = "0";
      image_path += QString("%1").arg(i) + path_tail;

      /* Finally, append it to the list */
      images.append(Frame::openImage(path + image_path));
    }
  }
  else
  {
    images.append(Frame::openImage(data.getDataString()));
  }

  /* Run through this list, checking ranges and add the corresponding
   * tiles */
  for(int index = 0; index < images.size(); index++) /* Image index */
  {
    for(int i = 0; i < row_list.size(); i++) /* Coordinate index */
    {
      QStringList rows = row_list[i].split("-");
      QStringList cols = col_list[i].split("-");

      /* Use the newly found range and add the tile */
      for(int r = rows[0].toInt(); r <= rows[rows.size() - 1].toInt(); r++)
      {
        for(int c = cols[0].toInt(); c <= cols[cols.size() - 1].toInt(); c++)
        {
          QString path = data.getElement(kTILE_DATA);
          if(path.startsWith("base") || path.startsWith("enhancer") || 
             path.startsWith("lower") || path.startsWith("upper"))
            success &= geography[r][c]->addData(data, images[index], 
                                                images.size() > 1);
        }
      }
    }
  }

  return success;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Painting function */
//void Map::paintEvent(QPaintEvent* event)
//{
  /*
  //setUpdatesEnabled(false);

  QPainter painter(this);

  (void)event;
  
  //painter.drawPixmap(256, 256, geography[0][0]->getBase()->getCurrent());

  for(int i = 0; i < 1000; i++)
  {
    for(int j = 0; j < 1000; j++)
    {
      painter.drawPixmap(i*64, j*64, geography[0][0]->getBase()->getCurrent());
    }
  }

  //setUpdatesEnabled(true);
  */
//}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

void Map::addLayer(Layer* item)
{
  addItem(item);
}

void Map::animate()
{
  bool just_started = false;

  if(player != 0)
  {
    /* If the udpate direction changes, update the player direction */
    if(viewport->updateDirection(player->x(), player->y()))
    {
      just_started = true;

      if(viewport->movingNorth())
        player->setDirection(MapPerson::NORTH);
      else if(viewport->movingSouth())
        player->setDirection(MapPerson::SOUTH);
      else if(viewport->movingWest())
        player->setDirection(MapPerson::WEST);
      else if(viewport->movingEast())
        player->setDirection(MapPerson::EAST);
      else
        player->resetAnimation();
    }

    /* Update the X and Y coordinates, if moving */
    if(viewport->moving())
    {
      player->setX(viewport->newX(player->x()));
      player->setY(viewport->newY(player->y()));
      player->animate(true, just_started);
    }

    if(viewport != 0)
      viewport->centerOn(player);

    player->update();
  }

  viewport_widget->updateGL();
}

void Map::animateTiles()
{
  for(int i = 0; i < geography.size(); i++)
    for(int j = 0; j < geography[i].size(); j++)
      geography[i][j]->animate();
}

void Map::deleteLayer(Layer* item)
{
  removeItem(item);
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

MapPerson* Map::getPlayer()
{
  return player;
}

/* Returns the map viewport, for scrolling through the scene */
MapViewport* Map::getViewport()
{
  return viewport;
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

bool Map::isLoaded()
{
  return loaded;
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
      {
        /* Create the tile */
        Tile* t = new Tile(kTILE_LENGTH, kTILE_WIDTH, 
                           j*kTILE_LENGTH, i*kTILE_WIDTH);
 
        /* Connect the signals */
        QObject::connect(t, SIGNAL(addLayer(Layer*)), 
                         this, SLOT(addLayer(Layer*)));
        QObject::connect(t, SIGNAL(deleteLayer(Layer*)), 
                         this, SLOT(deleteLayer(Layer*)));

        /* Add the new tile to the list */
        row.append(t);
      }

      geography.append(row);
    }
   
    /* Run through the map components and add them to the map */
    data = fh.readXmlData(&done, &success);
    do
    {
      if(data.getElement(kFILE_SECTION_ID) == "tiles")
        success &= addTileData(data);

      /* Get the next element */
      data = fh.readXmlData(&done, &success);
    } while(!done && success);

    /* Add in temporary player information */
    Sprite* up_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_D", 3, ".png");
    Sprite* down_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_U", 3, ".png");
    Sprite* left_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_R", 3, ".png");
    Sprite* right_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_L", 3, ".png");

    /* Make the map person */
    player = new MapPerson(kTILE_LENGTH, kTILE_WIDTH);
    player->setCoordinates(kTILE_LENGTH*7, kTILE_WIDTH*5, 9);

    player->setState(MapPerson::GROUND, MapPerson::NORTH, 
                                        new MapState(up_sprite));
    player->setState(MapPerson::GROUND, MapPerson::SOUTH, 
                                        new MapState(down_sprite));
    player->setState(MapPerson::GROUND, MapPerson::EAST, 
                                        new MapState(right_sprite));
    player->setState(MapPerson::GROUND, MapPerson::WEST, 
                                        new MapState(left_sprite));

    /* Add it */
    addItem(player);
  }

  success &= fh.stop();
  
  /* If the map load failed, unload the map */
  if(!success)
    unloadMap();
  loaded = success;

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

void Map::unloadMap()
{
  /* Delete all the tiles that have been set */
  for(int i = 0; i < geography.size(); i++)
  {
    for(int j = 0; j < geography[i].size(); j++)
    {
      delete geography[i][j];
      geography[i][j] = 0;
    }
  }

  /* Clear the remaining and disable the loading */
  clear();
  loaded = false;
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
