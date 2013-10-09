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
*  3. If mode is switched, end all notification animations and such. -> battle
*  4. Animation event for teleport requires fade out -> teleport -> fade in
*  5. If tile set fails, don't quit the map. Useful only multi tile setups.
*     Maybe only pass if it is a multi tile setup ??
*
* Extra:
*  1. As the player is teleported to a new map, maybe have the option to have
*     it at a tile and then walking in. More true animation. How to?
******************************************************************************/
#include "Game/Map/Map.h"

/* Constant Implementation - see header file for descriptions */
const int Map::kDOUBLE_DIGITS = 10;
const int Map::kELEMENT_ANGLE = 1;
const int Map::kELEMENT_DATA = 0;
const short Map::kFILE_CLASSIFIER = 3;
const short Map::kFILE_GAME_TYPE = 1;
const short Map::kFILE_SECTION_ID = 2;
const short Map::kFILE_TILE_COLUMN = 5;
const short Map::kFILE_TILE_ROW = 4;
const short Map::kPLAYER_INDEX = 0;
const int Map::kTICK_DELAY = 10;
const int Map::kTILE_HEIGHT = 64;
const int Map::kTILE_WIDTH = 64;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(const QGLFormat & format, Options running_config, 
         EventHandler::Event blank_event) : QGLWidget(format)
{
  /* Set some initial class flags */
  //setAttribute(Qt::WA_PaintOnScreen);
  //setAttribute(Qt::WA_NoSystemBackground);
  setAutoBufferSwap(false);
  setAutoFillBackground(false);
  
  /* Configure the scene */
  loaded = false;
  map_index = 0;
  persons.clear();
  player = 0;

  /* Configure the FPS animation and time elapsed, and reset to 0 */
  paint_animation = 0;
  paint_frames = 0;
  paint_time = 0;
  time_elapsed = 0;

  /* Test the things and persons */
  thing = 0;

  //setMinimumSize(2000, 2000);

  /* Set up the map displays */
  //map_dialog.setFont(QFont("Times", 10));
  map_dialog.setDialogImage("sprites/Map/Overlay/dialog.png");
  map_dialog.setViewportDimension(running_config.getScreenWidth(), 
                                  running_config.getScreenHeight());

  connect(&map_dialog, SIGNAL(setThingData(QList<int>)), 
          this, SLOT(getThingData(QList<int>)));
  connect(&map_dialog, SIGNAL(finishThingTarget()), 
          this, SLOT(finishThingTarget()));

  /* Setup the viewport */
  viewport = new MapViewport(running_config.getScreenWidth(), 
                             running_config.getScreenHeight(), 
                             kTILE_WIDTH, kTILE_HEIGHT);

  /* Initialize the event handler, for map creation */
  this->blank_event = blank_event;

  /* Call the initial update to start OpenGL */
  updateGL();
}

/* Destructor function */
Map::~Map()
{
  unloadMap();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

bool Map::addTileData(XmlData data, int section_index)
{
  int angle = 0;
  bool success = true;

  /* Get the element information */
  QStringList element = data.getElement(data.getNumElements()-1).split("_");

  /* Split based on the element information if it's for a path */
  if(element[0].toLower().trimmed() == "path")
  {
    /* If there is rotational information available, use it */
    if(element.size() > kELEMENT_ANGLE)
      angle = Sprite::getAngle(element[kELEMENT_ANGLE]);

    /* Get the actual set of paths and add them */
    QList< QList<QString> > path_stack = splitTilePath(data.getDataString());
    for(int y = 0; y < path_stack.size(); y++)
      for(int x = 0; x < path_stack[y].size(); x++)
        success &= addTileSprite(path_stack[y][x], x, y, 
                                 angle, section_index, data);

    return success;
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
          success &= geography[section_index][r][c]->
                  addPassability(data.getDataString(), 
                                 data.getElement(kFILE_CLASSIFIER), 
                                 data.getKeyValue(kFILE_CLASSIFIER));
    }
    return success;
  }

  return false;
}

/* Adds a tile sprite, based on the path and some XMLData */
// TODO: Add search existing sprites to see if it exists
bool Map::addTileSprite(QString path, int x_diff, int y_diff, 
                        int angle, int section_index, XmlData data)
{
  bool success = false;
  Sprite* tile_frames = 0;

  /* Handle the image data, split the path based on "|". This is the 
   * identifier if there is multiple frames */
  QStringList data_list = path.split("|");
  if(data_list.size() > 1)
    tile_frames = new Sprite(data_list[0], data_list[1].toInt(), 
                                           data_list[2], angle);
  else
    tile_frames = new Sprite(path, angle);
 
  /* Run through this list, checking ranges and add the corresponding
   * tiles, only if the sprite data is legitimate */
  if(tile_frames->getSize() > 0)
  {
    /* Split up the coordinates for the tile sprite */
    QStringList row_list = data.getKeyValue(kFILE_TILE_ROW).split(",");
    QStringList col_list = data.getKeyValue(kFILE_TILE_COLUMN).split(",");
    for(int i = 0; i < row_list.size(); i++) /* Coordinate set index */
    {
      QStringList rows = row_list[i].split("-"); /* x range for coordinate */
      QStringList cols = col_list[i].split("-"); /* y range for coordinate */

      /* Shift through all the rows and column pairs of the coordinate */
      for(int r = rows[0].toInt(); r <= rows[rows.size() - 1].toInt(); r++)
      {
        for(int c = cols[0].toInt(); c <= cols[cols.size() - 1].toInt(); c++)
        {
          int x = r + x_diff;
          int y = c + y_diff;

          if(section_index >= 0 && section_index < geography.size() && 
             x >= 0 && x < geography[section_index].size() && 
             y >= 0 && y < geography[section_index][x].size())
          {
            success |= geography[section_index][x][y]->
                  addSprite(tile_frames, data.getElement(kFILE_CLASSIFIER), 
                                         data.getKeyValue(kFILE_CLASSIFIER));
          }
        }
      }
    }
  }

  /* If successful, add the data to the stack. Else, delete it */
  if(success)
    tile_sprites.append(tile_frames);
  else
  {
    delete tile_frames;
    qDebug() << "[ERROR] Map sprite add fail with path: " + path;
  }

  return success;
}

bool Map::initiateMapSection(int section_index, int width, int height)
{
  /* Check if the initiation will be viable */
  if(section_index >= 0 && width > 0 && height > 0 && 
    (geography.size() <= section_index || 
     geography[section_index].size() == 0))
  {
    /* Get the geography subset filled up past the point of being set */
    while(geography.size() <= section_index)
    {
      QList< QList<Tile*> > map_section;
      geography.append(map_section);
    }

    /* Fill the section */
    for(int i = 0; i < width; i++)
    {
      QList<Tile*> col;
      
      for(int j = 0; j < height; j++)
      {
        Tile* t = new Tile(blank_event, kTILE_WIDTH, kTILE_HEIGHT, i, j);
        col.append(t);
      }
      
      geography[section_index].append(col);
    }

    return true;    
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
    int index = map_index;
    if((x >= 0 && x < geography[index].size() && y >= 0 && 
        y < geography[index][0].size()) &&
       (geography[index][x][y]->getImpassableThing() != 0 &&
        geography[index][x][y]->getImpassableThing()->getTile()->getX() == x &&
        geography[index][x][y]->getImpassableThing()->getTile()->getY() == y))
    {
      MapThing* target_thing = geography[index][x][y]->getImpassableThing();
      Conversation target_convo = target_thing->getConversation();

      /* Check if the conversation is relevant and use if so */
      if(!target_convo.text.isEmpty() || target_convo.next.size() > 0)
      {
        if(player->setTarget(target_thing) && 
           target_thing->setTarget(player) &&
           map_dialog.initConversation(target_convo, player->getID()))
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

/* Splites the tile path, to determine if numerous tiles are needed */
QList< QList<QString> > Map::splitTilePath(QString path)
{
  QList< QList<QString> > path_matrix;

  /* First split, to pull out the range */
  QStringList split_1 = path.split("{");
  if(split_1.size() == 2)
  {
    /* Second split, to pull out the range */
    QStringList split_2 = split_1[1].split("}");
    if(split_2.size() == 2)
    {
      /* Split the two directional coordinates */
      QStringList range = split_2[0].split("x");
      if(range.size() == 2)
      {
        /* Split the range */
        QStringList first_digit = range[0].split("-");
        int first_max = first_digit[first_digit.size()-1].toInt();
        int first_start = first_digit[0].toInt();
        QStringList second_digit = range[1].split("-");
        int second_max = second_digit[second_digit.size()-1].toInt();
        int second_start = second_digit[0].toInt();

        /* Loop through and create all the paths */
        for(int i = first_start; i <= first_max; i++)
        {
          QList<QString> x_paths;

          for(int j = second_start; j <= second_max; j++)
            x_paths.append(split_1[0] + QChar(64+i) + QChar(64+j) + split_2[1]);

          path_matrix.append(x_paths);
        }

        return path_matrix;
      }
    }
  }

  /* If here, the split was unsuccessful and therefore only one path */
  QList<QString> path_stack;
  path_stack.append(path);
  path_matrix.append(path_stack);
  return path_matrix;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* TODO: Possibly add it so that any map can keep updating? This might get too
 *       heavy. 
 *       Should updates be restricted to a larger space around the viewport
 *       and no more? */
void Map::animate(short time_since_last)
{
  /* Only proceed if the animation time is positive */
  if(time_since_last > 0)
  {
    /* The movement handling for things*/
    for(int i = 0; i < persons.size(); i++)
    {
      if(persons[i] != 0)
      {
        Tile* next_tile = 0;

        if(persons[i]->getMapSection() == map_index && 
           persons[i]->getTile() != 0)
        {
          int tile_x = persons[i]->getTile()->getX();
          int tile_y = persons[i]->getTile()->getY();

          /* Based on the move request, provide the next tile in line using the
           * current centered tile and move request */
          switch(persons[i]->getMoveRequest())
          {
            case EnumDb::NORTH:
              if(--tile_y >= 0)
                next_tile = geography[map_index][tile_x][tile_y];
              break;
            case EnumDb::EAST:
              if(++tile_x < geography[map_index].size())
                next_tile = geography[map_index][tile_x][tile_y];
              break;
            case EnumDb::SOUTH:
              if(++tile_y < geography[map_index][tile_x].size())
                next_tile = geography[map_index][tile_x][tile_y];
              break;
            case EnumDb::WEST:
              if(--tile_x >= 0)
                next_tile = geography[map_index][tile_x][tile_y];
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
  else if(key_event->key() == Qt::Key_F4)
    player->setSpeed(player->getSpeed() - 1);
  else if(key_event->key() == Qt::Key_F5)
    player->setSpeed(player->getSpeed() + 1);
  else if(key_event->key() == Qt::Key_0)
  {
    Conversation convo;
    convo.category = EnumDb::TEXT;
    convo.action_event = blank_event;
    convo.text = "This is the initial conversation point that will start it. ";
    convo.text += "How can this continue? It must pursue to complete ";
    convo.text += "embodiment. Ok, maybe I'll just keep typing until I break ";
    convo.text += "the entire compiler.";
    convo.thing_id = 1;
    Conversation test1, test2, test3, test4, test5;
    test1.category = EnumDb::TEXT;
    test1.action_event = blank_event;
    test1.text = "This is a test to see how the data performs. The line will split once ";
    test1.text += "unless it is an option based selection in which case it will restrict."; 
    test1.thing_id = 3;
    test2.category = EnumDb::TEXT;
    test2.action_event = blank_event;
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
    test3.action_event = blank_event;
    test3.text = "Back to finish off with a clean case with a couple of lines.";
    test3.text += " So this requires me to write a bunch of BS to try and fill";
    test3.text += " these lines.";
    test3.text += test3.text;
    //test3.text += test3.text;
    test3.thing_id = 24;
    test3.next.append(test2);
    test4.category = EnumDb::TEXT;
    test4.action_event = blank_event;
    test4.text = "Option 1 - This goes on and on and on and on and on and lorem ipsum. This is way too long to be an option. Loser";
    test4.thing_id = -1;
    test4.next.append(test2);
    test5.category = EnumDb::TEXT;
    test5.action_event = blank_event;
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
    if(player != 0 && map_dialog.initConversation(convo, player->getID()))
      player->keyFlush();
  }
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
    animate(time_elapsed);
    viewport->updateView();

    /* Paint the lower half */
    for(int i = viewport->getXTileStart(); i < viewport->getXTileEnd(); i++)
      for(int j = viewport->getYTileStart(); j < viewport->getYTileEnd(); j++)
        geography[map_index][i][j]->paintLower(viewport->getX(), 
                                               viewport->getY());
   
    /* Paint the persons (player and NPCs) */
    for(int i = 0; i < persons.size(); i++)
    {
      if(persons[i] != 0 && persons[i]->getMapSection() == map_index && 
                            persons[i]->getX() >= viewport->getXStart() &&
                            persons[i]->getX() <= viewport->getXEnd())
        persons[i]->paintGl(viewport->getX(), viewport->getY());
    }

    /* Paint the upper half */
    for(int i = viewport->getXTileStart(); i < viewport->getXTileEnd(); i++)
      for(int j = viewport->getYTileStart(); j < viewport->getYTileEnd(); j++)
        geography[map_index][i][j]->paintUpper(viewport->getX(), 
                                               viewport->getY());

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
    frames_per_second.setNum(paint_frames /(paint_time / 1000.0), 'f', 2);
    paint_animation = 20;
  }
  paint_animation--;

  /* Check the FPS monitor to see if it needs to be reset */
  if((paint_frames % 100) == 0)
  {
    paint_frames = 0;
    paint_time = 0;
  }
  paint_frames++;
  paint_time += time_elapsed;
  
  /* Finish by swapping the buffers and then restarting the timer to update 
   * the map again */
  swapBuffers();
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

// TODO: Separate file add success and XML read success to parse error
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
    /* Run through the map components and add them to the map */
    int height = -1;
    int index = -1;
    int width = -1;
    data = fh.readXmlData(&done, &success);
    do
    {
      /* Temporary variables */
      int temp_index = -1;
      
      if(data.getElement(kFILE_GAME_TYPE) == "map")
      {
        /* Determine if it's a new section */
        if(index != 0 && data.getElement(kFILE_SECTION_ID) == "main")
          temp_index = 0;
        else if(index != data.getKeyValue(kFILE_SECTION_ID).toInt() && 
                data.getElement(kFILE_SECTION_ID) == "section")
          temp_index = data.getKeyValue(kFILE_SECTION_ID).toInt();
        
        /* Update the index information accordingly */
        if(temp_index >= 0)
        {
          index = temp_index;
          if(geography.size() > index && geography[index].size() > 0)
          {
            height = geography[index][0].size();
            width = geography[index].size();
          }
          else
          {
            height = -1;
            width = -1;
          }
        }
        
        /* Parse the data, if it is relevant to the map */
        if(height == -1 && data.getElement(kFILE_CLASSIFIER) == "height")
        {
          height = data.getDataInteger();
          if(width > 0 && height > 0)
            initiateMapSection(index, width, height);
        }
        else if(width == -1 && data.getElement(kFILE_CLASSIFIER) == "width")
        {
          width = data.getDataInteger();
          if(width > 0 && height > 0)
            initiateMapSection(index, width, height);
        }
        else if(index >= 0 && height > 0 && width > 0)
          success &= addTileData(data, index);
      }
      
      /* Get the next element */
      data = fh.readXmlData(&done, &success);
    } while(!done && success);
    
    /* Add teleport to door - temporary */
    geography[1][3][6]->setEnterEvent(
          ((EventHandler*)blank_event.handler)->createTeleportEvent(12, 9, 0));
    geography[0][0][0]->setExitEvent(
          ((EventHandler*)blank_event.handler)->createStartBattleEvent());
    geography[0][12][8]->setEnterEvent(
          ((EventHandler*)blank_event.handler)->createTeleportEvent(3, 5, 1));

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
    person->setStartingTile(0, geography[0][8][8]); // 11, 9

    person->setState(MapPerson::GROUND, EnumDb::NORTH, up_sprite);
    person->setState(MapPerson::GROUND, EnumDb::SOUTH, down_sprite);
    person->setState(MapPerson::GROUND, EnumDb::EAST, right_sprite);
    person->setState(MapPerson::GROUND, EnumDb::WEST, left_sprite);
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
    person->setStartingTile(1, geography[1][2][4]);
    person->setState(MapPerson::GROUND, EnumDb::NORTH, up_sprite);
    person->setState(MapPerson::GROUND, EnumDb::SOUTH, down_sprite);
    person->setState(MapPerson::GROUND, EnumDb::EAST, right_sprite);
    person->setState(MapPerson::GROUND, EnumDb::WEST, left_sprite);
    person->setDialogImage("sprites/Map/Dialog/el_oroso.png");
    person->setID(24);
    person->setName("El Oroso");
    person->setOpacity(0.5);
    Conversation person_convo;
    person_convo.category = EnumDb::TEXT;
    person_convo.action_event = 
          ((EventHandler*)blank_event.handler)->createTeleportEvent(30, 30, 0);
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
    npc->setState(MapPerson::GROUND, EnumDb::NORTH, up_sprite);
    npc->setState(MapPerson::GROUND, EnumDb::SOUTH, down_sprite);
    npc->setState(MapPerson::GROUND, EnumDb::EAST, right_sprite);
    npc->setState(MapPerson::GROUND, EnumDb::WEST, left_sprite);
    npc->insertNodeAtTail(geography[map_index][10][10], 750);
    //npc->insertNodeAtTail(geography[map_index][5][10], 250);
    //npc->insertNodeAtTail(geography[map_index][0][10], 500);
    npc->insertNodeAtTail(geography[map_index][1][1], 1000);
    npc->insertNodeAtTail(geography[map_index][5][1], 250);
    //npc->insertNodeAtTail(geography[map_index][10][0], 50);
    npc->setStartingTile(0, geography[0][12][12]);
    npc->setDialogImage("sprites/Map/Dialog/arcadius.png");
    npc->setID(3);
    npc->setName("Arcadius");
    npc->setNodeState(MapNPC::BACKANDFORTH);
    npc->setSpeed(200);
    person_convo.action_event = blank_event;
    person_convo.text = "I don't like to be stopped. Why must you insist to stop progress. ";
    person_convo.text += "On second thought, I really don't like you and want to destroy you.";
    person_convo.thing_id = 3;
    Conversation second_set;
    second_set.category = EnumDb::TEXT;
    second_set.action_event = blank_event;
    second_set.text = "Here are your options:";
    second_set.thing_id = 3;
    Conversation option;
    option.category = EnumDb::TEXT;
    option.text = "Be teleported to an unknown place";
    option.thing_id = 3;
    option.action_event = 
            ((EventHandler*)blank_event.handler)->createTeleportEvent(3, 2, 1);
    second_set.next.append(option);
    option.text = "Be slaughtered by my hand";
    option.action_event = 
            ((EventHandler*)blank_event.handler)->createStartBattleEvent();
    second_set.next.append(option);
    person_convo.next.append(second_set);
    npc->setConversation(person_convo);
    persons.append(npc);

    /* Make the map thing */
    //thing = new MapThing(new Sprite("sprites/Map/Map_Things/main_AA_D00.png"),
    //                     kTILE_WIDTH, kTILE_HEIGHT);
    //thing->setStartingTile(0, geography[0][2][2]);
    //thing->initializeGl();
    //persons.append(thing);
  }

  success &= fh.stop();

  /* If the map load failed, unload the map */
  if(!success)
  {
    unloadMap();
  }
  else
  {
    if(geography[map_index].size() > 0)
    {
      viewport->setMapSize(geography[map_index].size(), 
                           geography[map_index][0].size());
      if(persons.size() > kPLAYER_INDEX)
        player = persons[kPLAYER_INDEX];
      if(player != 0)
        viewport->lockOn(player);
    }
    
    for(int i = 0; i < geography.size(); i++)
      for(int j = 0; j < geography[i].size(); j++)
        for(int k = 0; k < geography[i][j].size(); k++) 
          geography[i][j][k]->initializeGl();
    
    for(int i = 0; i < persons.size(); i++)
      persons[i]->initializeGl();
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

/* Changes the map section index - what is displayed */
bool Map::setSectionIndex(int index)
{
  if(index >= 0 && index < geography.size() && geography[index].size() > 0)
  {
    map_index = index;
    viewport->setMapSize(geography[index].size(), geography[index][0].size());
    
    return true;
  }
  
  return false;
}
  
// TODO: Add check to see if the person is within map_index.
// Possibly make the teleport add the ability of shifting map thing
void Map::teleportThing(int id, int tile_x, int tile_y, int section_id)
{
  /* If the section id is below 0, then set to internal map index */
  if(section_id < 0)
    section_id = map_index;
  
  /* Ensure that the tile x and y is within the range */
  if(section_id < geography.size() && 
     geography[section_id].size() > tile_x && tile_x >= 0 && 
     geography[section_id][tile_x].size() > tile_y && tile_y >= 0)
  {
    /* Change the starting tile for the thing */
    for(int i = 0; i < persons.size(); i++)
    {
      if(persons[i]->getID() == id)
      {
        if(persons[i]->setStartingTile(section_id, 
                                       geography[section_id][tile_x][tile_y]))
        {
          map_dialog.endConversation();
          if(map_index != section_id)
            setSectionIndex(section_id);
          persons[i]->clearAllMovement();
        }
      }
    }
  }
}

void Map::unloadMap()
{
  /* Delete all the tiles that have been set */
  for(int i = 0; i < geography.size(); i++)
  {
    for(int j = 0; j < geography[i].size(); j++)
    {
      for(int k = 0; k < geography[i][j].size(); k++)
      {
        delete geography[i][j][k];
        geography[i][j][k] = 0;
      }
      geography[i][j].clear();
    }
    geography[i].clear();
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

/* Updates the map - called by the cycle timer call from game */
void Map::updateMap(int cycle_time)
{
  time_elapsed = cycle_time;
  updateGL();
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
