/******************************************************************************
* Class Name: Map
* Date Created: Dec 2 2012
* Inheritance: QGLWidget
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
const short Map::kPLAYER_INDEX = 0;
const int Map::kTICK_DELAY = 5;
const int Map::kTILE_HEIGHT = 64;
const int Map::kTILE_WIDTH = 64;
const int Map::kVIEWPORT_HEIGHT = 11;
const int Map::kVIEWPORT_WIDTH = 19;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(const QGLFormat & format, short viewport_width, 
         short viewport_height) : QGLWidget(format)
{
  /* Set some initial class flags */
  //setAttribute(Qt::WA_PaintOnScreen);
  //setAttribute(Qt::WA_NoSystemBackground);
  setAutoBufferSwap(false);
  setAutoFillBackground(false);
  
  /* Configure the scene */
  loaded = false;
  persons.clear();
  player = 0;

  /* Configure the FPS animation and reset to 0 */
  frames = 0;
  paint_animation = 0;
  paint_count = 0;
  paint_time_average = 0.0;

  /* Test the things and persons */
  thing = 0;

  //setMinimumSize(2000, 2000);

  /* Setup the viewport */
  viewport = new MapViewport(viewport_width, viewport_height, 
                             kTILE_WIDTH, kTILE_HEIGHT);
 
  /* Bring the timer in to provide a game tick */
  connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
  timer.start(kTICK_DELAY);

  /* The time elapsed draw time */
  time_elapsed.start();
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

  /* Get the element information */
  QStringList element = data.getElement(data.getNumElements()-1).split("_");

  /* Split based on the element information if it's for a path */
  if(element[0].toLower().trimmed() == "path")
  {
    /* If there is rotational information available, use it */
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
  }
  /* Otherwise, access the passability information for the tile */
  else if(element[0].toLower().trimmed() == "passability")
  {
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
                  addPassability(data.getDataString(), 
                                 data.getElement(kFILE_CLASSIFIER), 
                                 data.getKeyValue(kFILE_CLASSIFIER));
    }
    return success;
  }

  return false;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void Map::initializeGL()
{
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_BLEND);
  //glEnable(GL_CULL_FACE); // Performance Add? Only for 3d
  //glEnable(GL_POLYGON_SMOOTH); // Causes strange lines drawn between tiles
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0, 0, 0, 0);
}

void Map::keyPressEvent(QKeyEvent* key_event)
{
  if(key_event->key() == Qt::Key_Down || key_event->key() == Qt::Key_Up ||
     key_event->key() == Qt::Key_Right || key_event->key() == Qt::Key_Left)
  {
    if(player != 0)
      player->keyPress(key_event);
  }
  else if(key_event->key() == Qt::Key_Escape)
    closeMap();
  else if(key_event->key() == Qt::Key_A)
    animateTiles();
  else if(key_event->key() == Qt::Key_1)
  {
    if(persons.size() > kPLAYER_INDEX)
    {
      player = persons[kPLAYER_INDEX];
      viewport->lockOn(player);
    }
  }
  else if(key_event->key() == Qt::Key_2)
  {
    if(persons.size() > (kPLAYER_INDEX + 1))
    {
      player = persons[kPLAYER_INDEX + 1];
      viewport->lockOn(player);
    }
  }
  else if(key_event->key() == Qt::Key_3)
    viewport->lockOn(609, 353);
}

void Map::keyReleaseEvent(QKeyEvent* key_event)
{
  if(key_event->key() == Qt::Key_Down || key_event->key() == Qt::Key_Up ||
     key_event->key() == Qt::Key_Right || key_event->key() == Qt::Key_Left)
  {
    if(player != 0)
      player->keyRelease(key_event);
  }
}

/* TODO: might need to restrict animation for things out of the display
 *       due to lag (high number of Things). Further testing required.
 *
 * Seem to be getting more consistent results for displaying, and it seems
 * better with vsync disabled */
void Map::paintGL()
{
  /* Start a QTimer to determine time elapsed for painting */
  //QTime time;
  //time.start();
    
  /* Start by setting the context and clearing the screen buffers */
  //makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glPushMatrix();
  
  /* Only proceed if the map is loaded */
  if(loaded)
  {
    /* Animate the map based on the elapsed time and then update
     * the viewport due to the animate */
    animate(time_elapsed.restart());
    viewport->updateView();
   
    /* Paint the lower half */
    for(int i = viewport->getXTileStart(); i < viewport->getXTileEnd(); i++)
      for(int j = viewport->getYTileStart(); j < viewport->getYTileEnd(); j++)
        geography[i][j]->paintLower(viewport->getX(), viewport->getY());
   
    /* Paint the persons (player and NPCs) */
    for(int i = 0; i < persons.size(); i++)
    {
      if(persons[i] != 0 && persons[i]->getX() >= viewport->getXStart() &&
                            persons[i]->getX() <= viewport->getXEnd())
        persons[i]->paintGl(viewport->getX(), viewport->getY());

    }

    /* Paint the upper half */
    for(int i = viewport->getXTileStart(); i < viewport->getXTileEnd(); i++)
      for(int j = viewport->getYTileStart(); j < viewport->getYTileEnd(); j++)
        geography[i][j]->paintUpper(viewport->getX(), viewport->getY());
  }
    
  /* Paint the frame rate */
  glColor4f(0.0, 0.0, 0.0, 0.5);
  glBegin(GL_QUADS);
    glVertex3f(7, 40, 0);//7,40
    glVertex3f(7, 10, 0);//7,10
    glVertex3f(64, 10, 0);//64,10
    glVertex3f(64, 40, 0);//64,40
  glEnd();
  glColor4f(1.0, 1.0, 1.0, 1.0);
  renderText(20, 30, frames_per_second);

  /* Clean up the drawing procedure */
  glFlush();
  //glPopMatrix();
  //glFinish();
  
  /* Determine the FPS sample rate */
  if(paint_animation <= 0)
  {
    frames_per_second.setNum(frames /(paint_time.elapsed() / 1000.0), 'f', 2);
    //qDebug() << frames_per_second << " " << isValid();
    paint_animation = 20;
  }
  paint_animation--;

  /* Check the FPS monitor to see if it needs to be reset */
  if (!(frames % 100))
  {
    paint_time.start();
    frames = 0;
  }
  frames++;

  /* Finish by updating the viewport widget - currently in auto */
  //qDebug() << time.elapsed();
  swapBuffers();
  //update();
}

void Map::resizeGL(int width, int height) 
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0); // set origin to bottom left corner
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

void Map::animate(short time_since_last)
{
  /* The movement handling for things*/
  for(int i = 0; i < persons.size(); i++)
  {
    if(persons[i] != 0 && persons[i]->getTile() != 0)
    { 
      Tile* next_tile = 0;
      int tile_x = persons[i]->getTile()->getX();
      int tile_y = persons[i]->getTile()->getY();
    
      /* Based on the move request, provide the next tile in line using the
       * current centered tile and move request */
      switch(persons[i]->getMoveRequest())
      {
        case EnumDb::NORTH:
          if(--tile_y >= 0)
            next_tile = geography[tile_x][tile_y];
          break;
        case EnumDb::EAST:
          if(++tile_x < geography.size())
            next_tile = geography[tile_x][tile_y];
          break;
        case EnumDb::SOUTH:
          if(++tile_y < geography[tile_x].size())
            next_tile = geography[tile_x][tile_y];
          break;
        case EnumDb::WEST:
          if(--tile_x >= 0)
            next_tile = geography[tile_x][tile_y];
          break;
        case EnumDb::DIRECTIONLESS:
          next_tile = 0;
      }
    
      /* Proceed to update the thing */
      persons[i]->updateThing(time_since_last, next_tile);
    }
  }
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
  //player->setFocus();
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
    int width = fh.readXmlData().getDataInteger(); // TODO: fix to ensure that
    int height = fh.readXmlData().getDataInteger();// it's actually this data
    for(int i = 0; i < width; i++)
    {
      QList<Tile*> col;

      for(int j = 0; j < height; j++)
      {
        /* Create the tile */
        Tile* t = new Tile(kTILE_WIDTH, kTILE_HEIGHT, i, j);
       
        // TODO: Remove - tile status testing
        //if(i == 10 && j == 10)
        //  t->setStatus(Tile::OFF);
        //else if(i == 12 && j == 10)
        //  t->setStatus(Tile::BLANKED);

        /* Add the new tile to the list */
        col.append(t);
      }

      geography.append(col);
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

    /* Add the player information */
    Sprite* up_sprite = new Sprite("sprites/Map/Map_Things/main_AA_D", 
                                   3, ".png");
    Sprite* down_sprite = new Sprite("sprites/Map/Map_Things/main_AA_U", 
                                     3, ".png");
    Sprite* left_sprite = new Sprite("sprites/Map/Map_Things/main_AA_S", 
                                     3, ".png");
    left_sprite->flipAll();
    Sprite* right_sprite = new Sprite("sprites/Map/Map_Things/main_AA_S", 
                                      3, ".png");
    MapPerson* person = new MapPerson(kTILE_WIDTH, kTILE_HEIGHT);
    person->setStartingTile(geography[8][8]); // 11, 9

    person->setState(MapPerson::GROUND, EnumDb::NORTH, 
                                        new MapState(up_sprite));
    person->setState(MapPerson::GROUND, EnumDb::SOUTH, 
                                        new MapState(down_sprite));
    person->setState(MapPerson::GROUND, EnumDb::EAST, 
                                        new MapState(right_sprite));
    person->setState(MapPerson::GROUND, EnumDb::WEST, 
                                        new MapState(left_sprite));
    persons.append(person);

    /* Add a second player */
    up_sprite = new Sprite("sprites/Map/Map_Things/aurumba_AA_D", 3, ".png");
    down_sprite = new Sprite("sprites/Map/Map_Things/aurumba_AA_U", 3, ".png");
    left_sprite = new Sprite("sprites/Map/Map_Things/aurumba_AA_S", 3, ".png");
    left_sprite->flipAll();
    right_sprite = new Sprite("sprites/Map/Map_Things/aurumba_AA_S", 3, ".png");
    person = new MapPerson(kTILE_WIDTH, kTILE_HEIGHT);
    person->setStartingTile(geography[2][4]);
    person->setState(MapPerson::GROUND, EnumDb::NORTH, 
                                        new MapState(up_sprite));
    person->setState(MapPerson::GROUND, EnumDb::SOUTH, 
                                        new MapState(down_sprite));
    person->setState(MapPerson::GROUND, EnumDb::EAST, 
                                        new MapState(right_sprite));
    person->setState(MapPerson::GROUND, EnumDb::WEST, 
                                        new MapState(left_sprite));
    persons.append(person);

    /* Make the map thing */
    //thing = new MapThing(new MapState(up_sprite), kTILE_WIDTH, kTILE_HEIGHT);
    //thing->setCoordinates(128, 128);
  }

  success &= fh.stop();

  /* If the map load failed, unload the map */
  if(!success)
  {
    unloadMap();
  }
  else
  {
    if(geography.size() > 0)
    {
      viewport->setMapSize(geography.size(), geography[0].size());
      if(persons.size() > kPLAYER_INDEX)
        player = persons[kPLAYER_INDEX];
      if(player != 0)
        viewport->lockOn(player);
    }

    for(int i = 0; i < geography.size(); i++)
      for(int j = 0; j < geography[i].size(); j++)
        geography[i][j]->initializeGl();
  }
  loaded = success;

  return success;
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

/* Starts the internal class animation tick */
void Map::tickStart()
{
  timer.start(kTICK_DELAY);
  time_elapsed.restart();
}

/* Stops the internal class animation tick */
void Map::tickStop()
{
  timer.stop();
}

void Map::unloadMap()
{
  /* Delete all the tiles that have been set */
  for(int i = 0; i < geography.size(); i++)
  {
    for(int j = 0; j < geography[i].size(); j++)
    {
      //geography[i][j]->removeFromScene(this);
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

  /* Delete the things */
  for(int i = 0; i < persons.size(); i++)
  {
    if(persons[i] != 0)
      delete persons[i];
    persons[i] = 0;
  }
  persons.clear();

  /* Reset the viewport */
  viewport->setMapSize(0, 0);
  viewport->lockOn(0, 0);

  /* Clear the remaining and disable the loading */
  //clear();
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
