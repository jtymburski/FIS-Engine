/******************************************************************************
* Class Name: Map
* Date Created: Dec 2 2012
* Inheritance: QGLWidget
* Description: The map class, this is the top level with regard to an actual
*              in-game map. This contains all the tiles that a map is composed
*              of, it also holds pointers to all of the NPC's contained in the
*              maps tiles. This also handles the viewport for the map.
*
* TODO: 
*  1. If the tile image does not exist, disable the painting sequence
*    - There seems to be a small glitch in this where it paints a white tile
*  2. Allow for map to run with a parameterless constructor. Is that possible
*     with the QGLFormat in the mix that is required for setting up the 
*     QGLWidget?
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
const int Map::kTICK_DELAY = 10;
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

  /* Set up the map displays */
  //map_dialog.setFont(QFont("Times", 10));
  map_dialog.setDialogImage("sprites/Map/Overlay/dialog.png");
  map_dialog.setViewportDimension(viewport_width, viewport_height);

  connect(&map_dialog, SIGNAL(setThingData(QList<int>)), 
          this, SLOT(getThingData(QList<int>)));
  connect(&map_dialog, SIGNAL(finishThingTarget()), 
          this, SLOT(finishThingTarget()));

  /* Setup the viewport */
  viewport = new MapViewport(viewport_width, viewport_height, 
                             kTILE_WIDTH, kTILE_HEIGHT);
 
  /* Bring the timer in to provide a game tick */
  //connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
  //timer.setSingleShot(true);
  //timer.start(kTICK_DELAY);
  updateGL();

  /* The time elapsed draw time */
  //time_elapsed.start();
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

/* Initiates a thing action, based on the action key being hit */
void Map::initiateThingAction()
{
  if(player != 0)
  {
    int x = player->getTile()->getX();
    int y = player->getTile()->getY();

    /* Determine the direction and offset coordinate tile selection */
    EnumDb::Direction direction = player->getDirection();
    if(direction == EnumDb::NORTH)
      y--;
    else if(direction == EnumDb::EAST)
      x++;
    else if(direction == EnumDb::SOUTH)
      y++;
    else if(direction == EnumDb::WEST)
      x--;
    /* Throw X out of bounds if no direction */
    else
      x = -1;

    /* Aquire the thing, that's being pointed at */
    if((x >= 0 && x < geography.size() && y >= 0 && y < geography[0].size()) &&
       (geography[x][y]->getImpassableThing() != 0 &&
        geography[x][y]->getImpassableThing()->getTile()->getX() == x &&
        geography[x][y]->getImpassableThing()->getTile()->getY() == y))
    {
      MapThing* target_thing = geography[x][y]->getImpassableThing();
      Conversation target_convo = target_thing->getConversation();

      /* Check if the conversation is relevant and use if so */
      if(!target_convo.text.isEmpty() || target_convo.next.size() > 0)
      {
        if(player->setTarget(target_thing) && 
           target_thing->setTarget(player) &&
           map_dialog.initConversation(target_convo))
        {
          player->keyFlush();
        }
        else
        {
          player->clearTarget();
          target_thing->clearTarget();
        }
      }
    }
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void Map::animate(short time_since_last)
{
  /* The movement handling for things*/
  for(int i = 0; i < persons.size(); i++)
  {
    if(persons[i] != 0)
    { 
      Tile* next_tile = 0;

      if(persons[i]->getTile() != 0)
      {
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
      }

      /* Proceed to update the thing */
      persons[i]->updateThing(time_since_last, next_tile);
    }
  }

  /* Animate the displays on the screen */
  map_dialog.update(time_since_last);
}

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

  /* Initialize Gl in other classes */
  map_dialog.initializeGl();
}

void Map::keyPressEvent(QKeyEvent* key_event)
{
  if(key_event->key() == Qt::Key_Escape)
    emit closeMap();
  else if(key_event->key() == Qt::Key_P)
    map_dialog.setPaused(!map_dialog.isPaused());
  else if(key_event->key() == Qt::Key_F1)
  {
    Frame* image = new Frame("sprites/Map/Tiles/Ground/BlueMetalTile/BlueMetalTile01_AA_A00.png");
    image->initializeGl();
    map_dialog.initPickup(image, 50, 1000);
  }
  else if(key_event->key() == Qt::Key_F2)
  {
    Frame* image = new Frame("sprites/Battle/Bubbies/blazing_t1.png");
    //Frame* image = new Frame("sprites/Battle/Battle_Persons/auroraagent.png");
    image->initializeGl();
    map_dialog.initPickup(image, 1);
  }
  else if(key_event->key() == Qt::Key_4)
    map_dialog.initNotification("Testing", 1000, true);
  else if(key_event->key() == Qt::Key_5)
    map_dialog.initNotification("This is a really long message. It goes on and on without end. Who makes notifications this long except for crazy deranged eutherlytes. Yes, I made a new word. You want to fight about it?");//map_dialog.haltDialog();
  else if(key_event->key() == Qt::Key_6)
    map_dialog.initNotification("This is a really long message. It goes on and on without end. Who makes notifications this long except for crazy deranged eutherlytes. Yes, I made a new word. You want to fight about it?", 5000, true);
  else if(map_dialog.isInConversation())
    map_dialog.keyPress(key_event);
  else if(key_event->key() == Qt::Key_Space)
    initiateThingAction();
  else if(key_event->key() == Qt::Key_Down || key_event->key() == Qt::Key_Up ||
          key_event->key() == Qt::Key_Right || key_event->key() == Qt::Key_Left)
  {
    if(player != 0)
      player->keyPress(key_event);
  }
  else if(key_event->key() == Qt::Key_A)
    animateTiles();
  else if(key_event->key() == Qt::Key_1)
  {
    if(persons.size() > kPLAYER_INDEX)
    {
      player->keyFlush();
      player = persons[kPLAYER_INDEX];
      viewport->lockOn(player);
    }
  }
  else if(key_event->key() == Qt::Key_2)
  {
    if(persons.size() > (kPLAYER_INDEX + 1))
    {
      player->keyFlush();
      player = persons[kPLAYER_INDEX + 1];
      viewport->lockOn(player);
    }
  }
  else if(key_event->key() == Qt::Key_3)
    viewport->lockOn(609, 353);
  else if(persons.size() >= 3 && key_event->key() == Qt::Key_7)
    ((MapNPC*)persons[2])->setNodeState(MapNPC::LOOPED);
  else if(persons.size() >= 3 && key_event->key() == Qt::Key_8)
    ((MapNPC*)persons[2])->setNodeState(MapNPC::BACKANDFORTH);
  else if(persons.size() >= 3 && key_event->key() == Qt::Key_9)
    ((MapNPC*)persons[2])->setNodeState(MapNPC::LOCKED);
  else if(key_event->key() == Qt::Key_0)
  {
    Conversation convo;
    convo.text = "This is the initial conversation point that will start it. ";
    convo.text += "How can this continue? It must pursue to complete ";
    convo.text += "embodiment. Ok, maybe I'll just keep typing until I break ";
    convo.text += "the entire compiler.";
    convo.thing_id = 1;
    Conversation test1, test2, test3, test4, test5;
    test1.category = EnumDb::TEXT;
    test1.text = "This is a test to see how the data performs. The line will split once ";
    test1.text += "unless it is an option based selection in which case it will restrict."; 
    test1.thing_id = 3;
    test2.category = EnumDb::TEXT;
    test2.text = "This is a no man case. See what happens!! Ok, this is the ";
    test2.text += "too long case where the lines never cease to exist and the ";
    test2.text += "points aren't for real. I'm feeling a bit hungry though ";
    test2.text += "so I don't know if I'll have the stamina to clean up this ";
    test2.text += "case in all it's glory. Repeat: ";
    test2.text += test2.text;
    test2.text += test2.text;
    test2.thing_id = 2;
    test2.next.append(test1);
    test3.category = EnumDb::TEXT;
    test3.text = "Back to finish off with a clean case with a couple of lines.";
    test3.text += " So this requires me to write a bunch of BS to try and fill";
    test3.text += " these lines.";
    test3.text += test3.text;
    //test3.text += test3.text;
    test3.thing_id = 24;
    test3.next.append(test2);
    test4.category = EnumDb::TEXT;
    test4.text = "Option 1 - This goes on and on and on and on and on and lorem ipsum. This is way too long to be an option. Loser";
    test4.thing_id = -1;
    test4.next.append(test2);
    test5.category = EnumDb::TEXT;
    test5.text = "Option 2";
    test5.thing_id = -1;
    test5.next.append(test3);
    test1.next.append(test4);
    test1.next.append(test5);
    test4.text = "Option 3";
    test1.next.append(test4);
    test5.text = "Option 4";
    test1.next.append(test5);
    test4.text = "Option 5";
    test1.next.append(test4);
    test5.text = "Option 6";
    test1.next.append(test5);
    /*test3.next.append(test4);
    test1.next.append(test3);
    test1.next.append(test2);*/
    convo.next.append(test1);
    /*convo.next.append(test5);
    convo.next.append(test1);
    convo.next.append(test3);*/
    if(map_dialog.initConversation(convo))
      player->keyFlush();
  }

  //else if(key_event->key() == Qt::Key_F1)
  //{
  //  map_dialog.setPersonDisplay("sprites/Map/Dialog/ulterius.png");
  //  map_dialog.setPersonName("Ulterius");
  //}
  //else if(key_event->key() == Qt::Key_F2)
  //{
  //  map_dialog.setPersonDisplay("sprites/Map/Dialog/peltrance.png");
  //  map_dialog.setPersonName("Peltrance");
  //}
  //else if(key_event->key() == Qt::Key_F3)
  //{
  //  map_dialog.setPersonDisplay("sprites/Map/Dialog/arcadius.png");
  //  map_dialog.setPersonName("Arcadius");
  //}
}

void Map::keyReleaseEvent(QKeyEvent* key_event)
{
  if(map_dialog.isInConversation())
    map_dialog.keyRelease(key_event);
  else if(key_event->key() == Qt::Key_Down || key_event->key() == Qt::Key_Up ||
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
  QTime time;
  time.start();
  
  /* Swap the buffers from the last call - put it on top or bottom? */
  //swapBuffers();
  //qDebug() << "1: " << time.elapsed();

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

    /* Paint the dialog */
    map_dialog.paintGl(this);
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
  //glFlush();
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

  /* Finish by swapping the buffers and then restarting the timer to update 
   * the map again */
  swapBuffers();
  //update();
  timer.start(kTICK_DELAY);
  //qDebug() << "2: " << time.elapsed();
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

void Map::animateTiles()
{
  for(int i = 0; i < tile_sprites.size(); i++)
    tile_sprites[i]->shiftNext();
}

void Map::finishThingTarget()
{
  if(player != 0)
  {
    if(player->getTarget() != 0)
      player->getTarget()->clearTarget();
    player->clearTarget();
  }
}

void Map::getThingData(QList<int> thing_ids)
{
  QList<MapThing*> used_things;

  /* Loop through all the ids to find the associated things */
  for(int i = 0; i < thing_ids.size(); i++)
  {
    /* Only continue if the ID is valid and >= than 0 */
    if(thing_ids[i] >= 0)
    {
      bool found = false;
      int j = 0;

      /* Loop through the stack of things to find the associated ID */
      while(!found && j < persons.size())
      {
        if(persons[j]->getID() == thing_ids[i])
        {
          used_things.append(persons[j]);
          found = true;
        }
        j++;
      }
    }
  }

  /* Set the dialog with the new stack of things */
  map_dialog.setThingData(used_things);
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
                                     5, ".png");
    left_sprite->flipAll();
    Sprite* right_sprite = new Sprite("sprites/Map/Map_Things/main_AA_S", 
                                      5, ".png");
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
    person->setDialogImage("sprites/Map/Dialog/player.png");
    person->setID(1);
    person->setName("REally LOng NAme");
    persons.append(person);

    /* Add a second player */
    up_sprite = new Sprite("sprites/Map/Map_Things/aurora_AA_D", 3, ".png");
    down_sprite = new Sprite("sprites/Map/Map_Things/aurora_AA_U", 3, ".png");
    left_sprite = new Sprite("sprites/Map/Map_Things/aurora_AA_S", 5, ".png");
    left_sprite->flipAll();
    right_sprite = new Sprite("sprites/Map/Map_Things/aurora_AA_S", 5, ".png");
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
    person->setDialogImage("sprites/Map/Dialog/el_oroso.png");
    person->setID(24);
    person->setName("El Oroso");
    Conversation person_convo;
    person_convo.text = "I don't have anything to say to you!!";
    person_convo.thing_id = 24;
    person->setConversation(person_convo);
    persons.append(person);

    /* Add an NPC */
    up_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_D",3,".png");
    down_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_U",3,".png");
    left_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_R",5,".png");
    right_sprite = new Sprite("sprites/Map/Map_Things/arcadius_AA_L",5,".png");
    MapNPC* npc = new MapNPC(kTILE_WIDTH, kTILE_HEIGHT);
    npc->setState(MapPerson::GROUND, EnumDb::NORTH, new MapState(up_sprite));
    npc->setState(MapPerson::GROUND, EnumDb::SOUTH, new MapState(down_sprite));
    npc->setState(MapPerson::GROUND, EnumDb::EAST, new MapState(right_sprite));
    npc->setState(MapPerson::GROUND, EnumDb::WEST, new MapState(left_sprite));
    npc->insertNodeAtTail(geography[10][10], 750);
    //npc->insertNodeAtTail(geography[5][10], 250);
    //npc->insertNodeAtTail(geography[0][10], 500);
    npc->insertNodeAtTail(geography[1][1], 1000);
    npc->insertNodeAtTail(geography[5][1], 250);
    //npc->insertNodeAtTail(geography[10][0], 50);
    npc->setDialogImage("sprites/Map/Dialog/arcadius.png");
    npc->setID(3);
    npc->setName("Arcadius");
    npc->setNodeState(MapNPC::BACKANDFORTH);
    npc->setSpeed(200);
    person_convo.text = "I don't like to be stopped. Why must you insist to stop progress. ";
    person_convo.text += "On second thought, I really don't like you and want to run away.";
    person_convo.thing_id = 3;
    Conversation second_set;
    second_set.text = "Fine, I'm leaving.";
    second_set.thing_id = 1;
    person_convo.next.append(second_set);
    npc->setConversation(person_convo);
    persons.append(npc);

    /* Make the map thing */
    //thing = new MapThing(new MapState(up_sprite), kTILE_WIDTH, kTILE_HEIGHT);
    //thing->setStartingTile(geography[2][2]);
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
