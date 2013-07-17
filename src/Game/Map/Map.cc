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
const int Map::kTICK_DELAY = 10;
const int Map::kTILE_HEIGHT = 64;
const int Map::kTILE_WIDTH = 64;
const int Map::kVIEWPORT_HEIGHT = 11;
const int Map::kVIEWPORT_WIDTH = 19;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(const QGLFormat & format, short viewport_width, short viewport_height) : QGLWidget(format)
{
  /* Set some initial class flags */
  //setAttribute(Qt::WA_PaintOnScreen);
  //setAttribute(Qt::WA_NoSystemBackground);
  setAutoBufferSwap(true);
  setAutoFillBackground(false);
  
  /* Configure the scene */
  loaded = false;
  player = 0;

  /* Configure the FPS animation and reset to 0 */
  frames = 0;
  paint_animation = 0;
  paint_count = 0;
  paint_time_average = 0.0;

  /* Test the things and persons */
  player = 0;
  thing = 0;

  /* Setup the OpenGL Widget */
  //QGLFormat gl_format(QGL::SampleBuffers);
  //gl_format.setSwapInterval(1);
  //viewport_widget = new QGLWidget(gl_format);
  //setMinimumSize(2000, 2000);

  /* Setup the viewport */
  viewport = new MapViewport(viewport_width, viewport_height, 
                             kTILE_WIDTH, kTILE_HEIGHT);
  //viewport->setWidget(this);
  //viewport->setViewport(viewport_widget);
  //viewport->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
 
  /* Check status of OpenGL */
  //qDebug() << this->format();
  //qDebug() << format().swapInterval();
  
  /* Bring the timer in to provide a game tick */
  connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
  timer.start(10);

  /* The time elapsed draw time */
  time_buffer = 0;
  time_elapsed.start();

  /* Testing */
  shift_index = 0;
  shift_forward = true;
  gl_image = 0;
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

double Map::averagePaintDelay(int time_elapsed)
{
  /* Up the paint count and calculate if it's too large */
  paint_count++;
  if(paint_count < 0)
    paint_count = 1000;

  /* Calculate the newly skewed average */
  double avg_original = paint_time_average * 
                        ((paint_count - 1.0) / paint_count);
  double avg_addition = time_elapsed * (1.0 / paint_count);

  /* Calculate the average and return */
  paint_time_average = avg_original + avg_addition;
  return paint_time_average;
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
  glEnable(GL_POLYGON_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0, 0, 0, 0);
}

void Map::keyPressEvent(QKeyEvent* keyEvent)
{
  if(keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up ||
     keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Left)
    player->keyPress(keyEvent);
  if(keyEvent->key() == Qt::Key_Escape)
    closeMap();
  else if(keyEvent->key() == Qt::Key_A)
    animateTiles();
  else if(keyEvent->key() == Qt::Key_R)
    shift_index = 0;
  else if(keyEvent->key() == Qt::Key_F)
    shift_forward = !shift_forward;
  //else if(keyEvent->key() == Qt::Key_1)
  //  viewport->lockOn(player);
  //else if(keyEvent->key() == Qt::Key_2)
  //  viewport->lockOn(1000, 1000);
}

void Map::keyReleaseEvent(QKeyEvent* keyEvent)
{
  if(keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up ||
     keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Left)
    player->keyRelease(keyEvent);
}

/* TODO: seems to work much better than before. The flicker that is still
 * there is from the video card (I think). The buffers are occassionally
 * switching before the data is loaded. How to prevent? Maybe disable
 * vsync and see if the results are better (probably not) */
void Map::paintGL()
{
  /* Start a QTimer to determine time elapsed for painting */
  //QTime time;
  //time.start();
 
  /* Start by setting the context and clearing the screen buffers */
  makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glPushMatrix();
  
  /* Execute the draw, if loaded */
  if(loaded)
  {
    int start_x = shift_index / 64 - 1;
    if(start_x < 0)
      start_x = 0;
    int end_x = shift_index / 64 + kVIEWPORT_WIDTH + 1;
    if(end_x > geography.size())
      end_x = geography.size();

    /* Paint the lower half */
    for(int i = start_x; i < end_x; i++)
      for(int j = 0; j < kVIEWPORT_HEIGHT; j++)
        geography[i][j]->paintLower(i*kTILE_WIDTH - shift_index, 
                                    j*kTILE_HEIGHT, kTILE_WIDTH, 
                                    kTILE_HEIGHT, 1);
    
    if(player != 0 && player->getX() >= start_x*64 && 
                      player->getX() <= end_x*64)
      player->paintGl(player->getX() - shift_index, player->getY(), 
                      kTILE_WIDTH, kTILE_HEIGHT, 1.0);

    /* Paint the upper half */
    for(int i = start_x; i < end_x; i++)
      for(int j = 0; j < kVIEWPORT_HEIGHT; j++)
        geography[i][j]->paintUpper(i*kTILE_WIDTH - shift_index,
                                    j*kTILE_HEIGHT, kTILE_WIDTH,
                                    kTILE_HEIGHT, 1);

    /* Test the viewport - TODO: delete */
    viewport->updateView();
  }

  /* Paint the frame rate */
  glColor4f(0.0, 0.0, 0.0, 0.5);
  glBegin(GL_QUADS);
    glVertex3f(7, 40, 0);
    glVertex3f(7, 10, 0);
    glVertex3f(64, 10, 0);
    glVertex3f(64, 40, 0);
  glEnd();
  glColor4f(1.0, 1.0, 1.0, 1.0);
  renderText(20, 30, frames_per_second);
  renderText(450, 24, "Press 'r' to reset the painting back to the origin");
  renderText(490, 48, "Press 'a' to animate the grass tile");
  renderText(480, 72, "Press 'f' to flip the movement direction");

  /* Clean up the drawing procedure */
  glFlush();
  //glPopMatrix();
  glFinish();
  
  /* Paint the FPS to the screen */
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
  
  /* Time elapsed from standard update and then animate the screen */
  time_buffer += averagePaintDelay(time_elapsed.restart());
  animate();
  
  /* Finish by updating the viewport widget - currently in auto */
  //swapBuffers();
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

void Map::animate()
{
  short time_constant = 11;

  /* Update the shifting for movement */
  //if(shift_index > 1200)
  //  shift_index = 0;
  if(shift_forward)
  {
    if(shift_index < (geography.size() - kVIEWPORT_WIDTH)*kTILE_WIDTH)
    {
      int shift = (int)(time_buffer / time_constant);
      shift_index += shift;
      time_buffer -= shift * time_constant;
    }
    else
    {
      time_buffer = 0;
    }
  }
  else
  {
    if(shift_index > 0)
    {
      int shift = (int)(time_buffer / time_constant);
      shift_index -= shift;
      time_buffer -= shift * time_constant;
    }
    else
    {
      time_buffer = 0;
    }
  }

  if(player != 0)
  {
    bool movable = true;

    if(player->isOnTile() && player->isMoveRequested())
    {
      EnumDb::Direction moving = player->getMoveRequest();
      int x1 = (int)player->getX() / kTILE_WIDTH;
      int y1 = (int)player->getY() / kTILE_HEIGHT;
      movable = geography[x1][y1]->getPassability(moving);
      int x2, y2;

      if(moving == EnumDb::NORTH)
      {
        x2 = x1;
        y2 = y1 - 1;
        if(y2 >= 0)
          movable &= geography[x2][y2]->getPassability(EnumDb::SOUTH);
        else
          movable = false;
      }
      else if(moving == EnumDb::EAST)
      {
        x2 = x1 + 1;
        y2 = y1;
        if(x2 < geography.size())
          movable &= geography[x2][y2]->getPassability(EnumDb::WEST);
        else
          movable = false;
      }
      else if(moving == EnumDb::SOUTH)
      {
        x2 = x1;
        y2 = y1 + 1;
        if(y2 < geography[x2].size())
          movable &= geography[x2][y2]->getPassability(EnumDb::NORTH);
        else
          movable = false;
      }
      else if(moving == EnumDb::WEST)
      {
        x2 = x1 - 1;
        y2 = y1;
        if(x2 >= 0)
          movable &= geography[x2][y2]->getPassability(EnumDb::EAST);
        else
          movable = false;
      }
    }

    player->updateThing(movable);
    //viewport->updateView();
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
      QVector<Tile*> col;

      for(int j = 0; j < height; j++)
      {
        /* Create the tile */
        Tile* t = new Tile(kTILE_WIDTH, kTILE_HEIGHT, 
                           i*kTILE_WIDTH, j*kTILE_HEIGHT);
        //t->setStatus(Tile::OFF);

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
    player = new MapPerson(kTILE_WIDTH, kTILE_HEIGHT, this);
    player->setCoordinates(kTILE_WIDTH*10, kTILE_HEIGHT*8);

    player->setState(MapPerson::GROUND, EnumDb::NORTH, 
                                        new MapState(up_sprite));
    player->setState(MapPerson::GROUND, EnumDb::SOUTH, 
                                        new MapState(down_sprite));
    player->setState(MapPerson::GROUND, EnumDb::EAST, 
                                        new MapState(right_sprite));
    player->setState(MapPerson::GROUND, EnumDb::WEST, 
                                        new MapState(left_sprite));

    /* Make the map thing */
    //thing = new MapThing(new MapState(up_sprite), kTILE_WIDTH, kTILE_HEIGHT);
    //thing->setCoordinates(128, 128);

    /* Add it */
    //addItem(player);
    //viewport->lockOn(player);
    //setFocusItem(player);
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
      viewport->lockOn(609, 353); // 1216 / 2 + 1, 704 / 2 + 1
    }

    for(int i = 0; i < geography.size(); i++)
      for(int j = 0; j < geography[i].size(); j++)
        geography[i][j]->initializeGl();
  }
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

/* Starts the internal class animation tick */
void Map::tickStart()
{
  timer.start(kTICK_DELAY);

  time_buffer = 0;
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
