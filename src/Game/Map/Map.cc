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
const int Map::kELEMENT_ANGLE = 1;
const int Map::kELEMENT_DATA = 0;
const int Map::kFILE_CLASSIFIER = 3;
const int Map::kFILE_SECTION_ID = 2;
const int Map::kFILE_TILE_COLUMN = 5;
const int Map::kFILE_TILE_ROW = 4;
const int Map::kTILE_LENGTH = 64;
const int Map::kTILE_WIDTH = 64;
const int Map::kVIEWPORT_LENGTH = 19;
const int Map::kVIEWPORT_WIDTH = 11;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(short resolution_x, short resolution_y)
{
  /* Setting the tree indexing method (NoIndex or BspTreeIndex) */
  //setItemIndexMethod(QGraphicsScene::NoIndex);

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
  viewport->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
 
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
  int angle = 0;
  bool success = true;
  Sprite* tile_frames = 0;

  /* Get the rotation angle, from the element */
  QStringList element = data.getElement(data.getNumElements()-1).split("_");
  if(element.size() > kELEMENT_ANGLE)
    angle = Sprite::getAngle(element[kELEMENT_ANGLE]);

  /* Handle the image data, split the path based on "|". This is the 
   * identifier if there is multiple frames */
  QStringList data_list = data.getDataString().split("|");
  if(data_list.size() > 1)
    tile_frames = new Sprite(data_list[0], 
                             data_list[1].toInt(), 
                             data_list[2], angle);
  else
    tile_frames = new Sprite(data.getDataString(), angle);
 
  /* Run through this list, checking ranges and add the corresponding
   * tiles, only if the sprite data is legitimate */
  if(tile_frames->getSize() > 0)
  {
    /* Add to the list of tiles */
    tile_sprites.append(tile_frames);

    QStringList row_list = data.getKeyValue(kFILE_TILE_ROW).split(",");
    QStringList col_list = data.getKeyValue(kFILE_TILE_COLUMN).split(",");

    for(int i = 0; i < row_list.size(); i++) /* Coordinate set index */
    {
      QStringList rows = row_list[i].split("-"); /* x range for coordinate */
      QStringList cols = col_list[i].split("-"); /* y range for coordinate */

      /* Shift through all the rows and column pairs of the coordinate */
      for(int r = rows[0].toInt(); r <= rows[rows.size() - 1].toInt(); r++)
        for(int c = cols[0].toInt(); c <= cols[cols.size() - 1].toInt(); c++)
          success &= geography[r][c]->
                  addSprite(tile_frames, data.getElement(kFILE_CLASSIFIER), 
                                         data.getKeyValue(kFILE_CLASSIFIER));
    }

    return success;
  }

  return false;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Background painting function */
void Map::drawBackground(QPainter* painter, const QRectF& rect)
{
  /* Do nothing */
  (void)painter;
  (void)rect;
}

void Map::keyPressEvent(QKeyEvent* keyEvent)
{
  if(keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up ||
     keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Left)
    sendEvent(player, keyEvent);
  else if(keyEvent->key() == Qt::Key_Escape)
    closeMap();
  else if(keyEvent->key() == Qt::Key_A)
    animateTiles();
  else if(keyEvent->key() == Qt::Key_1)
    viewport->lockOn(player);
  else if(keyEvent->key() == Qt::Key_2)
    viewport->lockOn(1000, 1000);
}

void Map::keyReleaseEvent(QKeyEvent* keyEvent)
{
  if(keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up ||
     keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Left)
    sendEvent(player, keyEvent);
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

void Map::animate()
{
  /* Start a QTimer to determine time elapsed for update */
  QTime time;
  time.start();

  if(player != 0)
  {
    player->updateThing();
    viewport->updateView();
  }

  viewport->viewport()->update();

  /* Time elapsed from standard update */
  //qDebug() << time.elapsed();
}

void Map::animateTiles()
{
  for(int i = 0; i < tile_sprites.size(); i++)
    tile_sprites[i]->shiftNext();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

void Map::closeMap()
{
  emit closingMap(2);
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

MapPerson* Map::getPlayer()
{
  return player;
}

/* Returns the map viewport, for scrolling through the scene */
MapViewport* Map::getViewport()
{
  return viewport;
}

void Map::initialization()
{
  player->setFocus();
}

/* Causes the thing you are facing and next to start its interactive action */
void Map::interact(EnumDb::Direction dir)
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
        //t->setStatus(Tile::OFF);
        t->insertIntoScene(this);

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
    Sprite* up_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_D", 
                                   3, ".png");
    Sprite* down_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_U", 
                                     3, ".png");
    Sprite* left_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_R", 
                                     3, ".png");
    Sprite* right_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_L", 
                                      3, ".png");

    /* Make the map person */
    player = new MapPerson(kTILE_LENGTH, kTILE_WIDTH);
    player->setCoordinates(kTILE_LENGTH*7, kTILE_WIDTH*5, 2);

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
    viewport->lockOn(player);
    //setFocusItem(player);
  }

  success &= fh.stop();
  
  /* If the map load failed, unload the map */
  if(!success)
    unloadMap();
  loaded = success;

  return success;
}

/* Shifts the viewport */
void Map::move(EnumDb::Direction dir, int step_length, Sprite dir_sprite)
{
  (void)dir;//warning
  (void)step_length;//warning
  (void)dir_sprite;//warning
}

/* Checks the tile you are attempting to enter for passibility of the given
 * direction */
bool Map::passible(EnumDb::Direction dir, int x, int y)
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
      geography[i][j]->removeFromScene(this);
      delete geography[i][j];
      geography[i][j] = 0;
    }
  }
  geography.clear();

  /* Deletes the sprite data stored */
  for(int i = 0; i < tile_sprites.size(); i++)
  {
    delete tile_sprites[i];
    tile_sprites[i] = 0;
  }
  tile_sprites.clear();

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
