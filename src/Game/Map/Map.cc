/******************************************************************************
 * Class Name: Map
 * Date Created: Dec 2 2012
 * Inheritance: none
 * Description: The map class, this is the top level with regard to an actual
 *              in-game map. This contains all the tiles that a map is composed
 *              of, it also holds pointers to all of the NPC's contained in the
 *              maps tiles. This also handles the viewport for the map.
 *
 * TODO: 
 *  1. If the tile image does not exist, disable the painting sequence
 *    - There seems to be a small glitch in this where it paints a white tile
 *  2. If mode is switched, end all notification animations and such. -> battle
 *  3. Animation event for teleport requires fade out -> teleport -> fade in
 *  4. If tile set fails, don't quit the map. Useful only multi tile setups.
 *     Maybe only pass if it is a multi tile setup ??
 *
 * Extra:
 *  1. As the player is teleported to a new map, maybe have the option to have
 *     it at a tile and then walking in. More true animation. How to?
 ******************************************************************************/
#include "Game/Map/Map.h"

/* Constant Implementation - see header file for descriptions */
// const int Map::kDOUBLE_DIGITS = 10;
// const int Map::kELEMENT_DATA = 0;
const uint8_t Map::kFILE_CLASSIFIER = 3;
const uint8_t Map::kFILE_GAME_TYPE = 1;
const uint8_t Map::kFILE_SECTION_ID = 2;
const uint8_t Map::kFILE_TILE_COLUMN = 5;
const uint8_t Map::kFILE_TILE_ROW = 4;
const uint8_t Map::kPLAYER_ID = 0;
const uint16_t Map::kTILE_SIZE = 64;
// const short Map::kZOOM_TILE_SIZE = 16;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(Options* running_config, EventHandler* event_handler)
{
  /* Set initial variables */
  base_path = "";
  this->event_handler = event_handler;
  loaded = false;
  map_index = 0;
  player = NULL;
  running = false;
  system_options = NULL;
  
  /* Configure the width / height of tiles and sets default zooming */
  tile_height = kTILE_SIZE;
  tile_width = kTILE_SIZE;
  // zoom_in = false;
  // zoom_out = false;
  
  /* Set options configuration */
  viewport.setTileSize(tile_width, tile_height);
  setConfiguration(running_config);

  // /* Configure the scene */
  // items.clear();
  // loaded = false;
  // map_index = 0;
  // persons.clear();
  // player = 0;
  // things.clear();
  
  // /* Configure the FPS animation and time elapsed, and reset to 0 */
  // paint_animation = 0;
  // paint_frames = 0;
  // paint_time = 0;
  // time_elapsed = 0;

  // //setMinimumSize(2000, 2000);

  // /* Set up the map displays */
  map_dialog.setEventHandler(event_handler);
  // //map_dialog.setFont(QFont("Times", 10));
  // map_dialog.setDialogImage("sprites/Map/Overlay/dialog.png");
  // map_dialog.setEventHandler(event_handler);
  // map_dialog.setViewportDimension(running_config.getScreenWidth(), 
                                  // running_config.getScreenHeight());

  // /* Necessary connections for child functions */
  // connect(&map_dialog, SIGNAL(setThingData(QList<int>)), 
          // this, SLOT(getThingData(QList<int>)));
  // connect(&map_dialog, SIGNAL(finishThingTarget()), 
          // this, SLOT(finishThingTarget()));
}

/* Destructor function */
Map::~Map()
{
  unloadMap();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

bool Map::addSpriteData(XmlData data, std::string id, 
                        int file_index, SDL_Renderer* renderer)
{
  uint16_t access_id = 0;
  Sprite* access_sprite = NULL;
  uint16_t copy_id = 0;
  Sprite* copy_sprite = NULL;
  
  /* Get the ID information */
  std::vector<std::string> id_set = Helpers::split(id, '>');
  if(id_set.size() == 1)
    access_id = std::stoi(id);
  else if(id_set.size() == 2)
  {
    access_id = std::stoi(id_set.back());
    copy_id = std::stoi(id_set.front());
  }
  
  /* Only move forward if the access id is valid */
  if(access_id > 0)
  {
    /* Search for the sprite in the existing stack */
    for(uint16_t i = 0; i < tile_sprites.size(); i++)
    {
      if(tile_sprites[i]->getId() == access_id)
        access_sprite = tile_sprites[i];
      else if(tile_sprites[i]->getId() == copy_id)
        copy_sprite = tile_sprites[i];
    }

    /* If the sprite doesn't exist, create it */
    if(access_sprite == NULL)
    {
      access_sprite = new Sprite();
      access_sprite->setId(access_id);
      access_sprite->setWhiteMask(white_mask.getTexture());
      tile_sprites.push_back(access_sprite);
      
      /* If the copy sprite isn't null, copy the data into the new sprite */
      if(copy_sprite != NULL)
        *access_sprite = *copy_sprite;
    }
    
    return access_sprite->addFileInformation(data, file_index, 
                                             renderer, base_path);
  }
  
  return false;
}

bool Map::addTileData(XmlData data, uint16_t section_index)
{
  std::string classifier = data.getElement(kFILE_CLASSIFIER);
  std::string element = data.getAllElements().back();
  bool success = true;
  
  /* Split based on the element information if it's for a path */
  if(element == "sprite_id" || element == "sprite_matrix")
  {
    std::vector< std::vector<uint16_t> > id_matrix;
    if(element == "sprite_id")
      id_matrix = splitIdString(data.getDataString());
    else
      id_matrix = splitIdString(data.getDataString(), true);
    
    /* Go through all the available IDs, in the matrix */
    for(uint16_t i = 0; i < id_matrix.size(); i++)
    {
      for(uint16_t j = 0; j < id_matrix[i].size(); j++)
      {
        Sprite* found_sprite = NULL;
        
        /* Try and find the id */
        if(id_matrix[i][j] != 0)
        {
          for(uint32_t k = 0; k < tile_sprites.size(); k++)
          {
            if(tile_sprites[k]->getId() == id_matrix[i][j])
              found_sprite = tile_sprites[k];
          }
        }
        
        /* If the sprite is found, add it to the tile */
        if(found_sprite != NULL)
        {
          std::vector<std::string> col_list =
                       Helpers::split(data.getKeyValue(kFILE_TILE_COLUMN), ',');
          uint16_t index = 0;
          std::vector<std::string> row_list = 
                          Helpers::split(data.getKeyValue(kFILE_TILE_ROW), ',');
          
          /* Loop through all the column and rows of the indexes */
          while(index < row_list.size() && index < col_list.size())
          {
            uint16_t col_end = i;
            uint16_t col_start = i;
            uint16_t row_end = j;
            uint16_t row_start = j;
            parseCoordinateInfo(row_list[index], col_list[index], section_index,
                                &row_start, &row_end, &col_start, &col_end);
            
            /* Add the sprite to all the applicable tiles */
            for(int r = row_start; r < row_end; r++)
              for(int c = col_start; c < col_end; c++)
                success &= geography[section_index][r][c]->
                                  addSprite(found_sprite, 
                                            data.getElement(kFILE_CLASSIFIER), 
                                            data.getKeyValue(kFILE_CLASSIFIER));
            index++;
          }
        }
      }
    }

    return success;
  }
  /* Otherwise, access the passability information for the tile */
  else if(element == "passability" || 
          classifier == "tileevent")
  {
    std::vector<std::string> col_list =
                       Helpers::split(data.getKeyValue(kFILE_TILE_COLUMN), ',');
    uint16_t index = 0;
    std::vector<std::string> row_list = 
                          Helpers::split(data.getKeyValue(kFILE_TILE_ROW), ',');
    
    /* Loop through all the column and rows of the indexes */
    while(index < row_list.size() && index < col_list.size())
    {
      uint16_t col_end = 0;
      uint16_t col_start = 0;
      uint16_t row_end = 0;
      uint16_t row_start = 0;
      parseCoordinateInfo(row_list[index], col_list[index], section_index,
                          &row_start, &row_end, &col_start, &col_end);
      
      /* Add the sprite to all the applicable tiles */
      for(int r = row_start; r < row_end; r++)
      {
        for(int c = col_start; c < col_end; c++)
        {
          /* Set the passability */
          if(element == "passability")
          {
            success &= geography[section_index][r][c]->
                    addPassability(data.getDataString(), 
                                   data.getElement(kFILE_CLASSIFIER), 
                                   data.getKeyValue(kFILE_CLASSIFIER));
          }
          /* Otherwise, it's a tile event */
          else
          {
            /* Classify between enter and exit events */
            if(data.getKeyValue(kFILE_CLASSIFIER) == "enter")
              success &= geography[section_index][r][c]->
                    updateEnterEvent(data, kFILE_CLASSIFIER + 3, section_index);
            else if(data.getKeyValue(kFILE_CLASSIFIER) == "exit")
              success &= geography[section_index][r][c]->
                     updateExitEvent(data, kFILE_CLASSIFIER + 3, section_index);
          }
        }
      }
      index++;
    }
    return success;
  }
  return false;
}

// /* Adds a tile sprite, based on the path and some XMLData */
// TODO: Add search existing sprites to see if it exists
// bool Map::addTileSprite(QString path, int x_diff, int y_diff, 
                        // QList<QString> adjustments, int section_index, 
                        // XmlData data)
// {
  // bool success = false;

  // /* Run through this list, checking ranges and add the corresponding
   // * tiles, only if the sprite data is legitimate */
  // Sprite* tile_frames = new Sprite(path);
  // tile_frames->execImageAdjustments(adjustments);
  // if(tile_frames->getSize() > 0)
  // {
    // /* Split up the coordinates for the tile sprite */
    // QStringList row_list = data.getKeyValue(kFILE_TILE_ROW).split(",");
    // QStringList col_list = data.getKeyValue(kFILE_TILE_COLUMN).split(",");
    // for(int i = 0; i < row_list.size(); i++) /* Coordinate set index */
    // {
      // QStringList rows = row_list[i].split("-"); /* x range for coordinate */
      // QStringList cols = col_list[i].split("-"); /* y range for coordinate */

      // /* Shift through all the rows and column pairs of the coordinate */
      // for(int r = rows[0].toInt(); r <= rows[rows.size() - 1].toInt(); r++)
      // {
        // for(int c = cols[0].toInt(); c <= cols[cols.size() - 1].toInt(); c++)
        // {
          // int x = r + x_diff;
          // int y = c + y_diff;

          // if(section_index >= 0 && section_index < geography.size() && 
             // x >= 0 && x < geography[section_index].size() && 
             // y >= 0 && y < geography[section_index][x].size())
          // {
            // success |= geography[section_index][x][y]->
                  // addSprite(tile_frames, data.getElement(kFILE_CLASSIFIER), 
                                         // data.getKeyValue(kFILE_CLASSIFIER));
          // }
        // }
      // }
    // }
  // }

  // /* If successful, add the data to the stack. Else, delete it */
  // if(success)
    // tile_sprites.append(tile_frames);
  // else
  // {
    // delete tile_frames;
    // qDebug() << "[ERROR] Map sprite add fail with path: " + path;
  // }

  // return success;
// }

bool Map::addThingData(XmlData data, uint16_t section_index, 
                                     SDL_Renderer* renderer)
{
  std::string identifier = data.getElement(kFILE_CLASSIFIER);
  uint16_t id = std::stoul(data.getKeyValue(kFILE_CLASSIFIER));
  uint16_t index = 0;
  MapThing* modified_thing = NULL;

  /* Identify which thing to be created */
  if(identifier == "mapthing" || identifier == "mapio")
  {
    /* Search for the existing map object */
    while(modified_thing == NULL && index < things.size())
    {
      if(things[index]->getID() == id)
        modified_thing = things[index];
      index++;
    }

    /* Create a new thing if one does not exist */
    if(modified_thing == NULL)
    {
      if(identifier == "mapthing")
        modified_thing = new MapThing(NULL, tile_width, tile_height);
      else
        modified_thing = new MapInteractiveObject(tile_width, tile_height);
      modified_thing->setEventHandler(event_handler);
      modified_thing->setID(id);

      /* Append the new one */
      things.push_back(modified_thing);
    }
  }
  else if(identifier == "mapperson" || identifier == "mapnpc")
  {
    /* Search for the existing map object */
    while(modified_thing == NULL && index < persons.size())
    {
      if(persons[index]->getID() == id)
        modified_thing = persons[index];
      index++;
    }

    /* Create a new person if one does not exist */
    if(modified_thing == NULL)
    {
      if(identifier == "mapperson")
        modified_thing = new MapPerson(tile_width, tile_height);
      else
        modified_thing = new MapNPC(tile_width, tile_height);
      modified_thing->setEventHandler(event_handler);
      modified_thing->setID(id);
      
      /* If the ID is the player ID, tie to the player */
      if(id == kPLAYER_ID)
        player = static_cast<MapPerson*>(modified_thing);
      
      /* Append the new one */
      persons.push_back(static_cast<MapPerson*>(modified_thing));
    }
  }
  else if(identifier == "mapitem")
  {
    /* Search for the existing map object */
    while(modified_thing == NULL && index < items.size())
    {
      if(items[index]->getID() == id)
        modified_thing = items[index];
      index++;
    }
    
    /* If no item found, create one */
    if(modified_thing == NULL)
    {
      /* Create the new thing */
      modified_thing = new MapItem(NULL, tile_width, tile_height);
      modified_thing->setEventHandler(event_handler);
      modified_thing->setID(id);
      
      /* Append the new one */
      items.push_back(static_cast<MapItem*>(modified_thing));
    }
  }

  /* Proceed to update the thing information from the XML data */
  if(modified_thing != NULL)
  {
    /* Handle the startpoint case if it's the identifying element */
    if(data.getElement(kFILE_CLASSIFIER + 1) == "startpoint")
    {
      std::vector<std::string> points = 
                                      Helpers::split(data.getDataString(), ',');
      if(points.size() == 2) /* There needs to be an x and y point */
      {
        uint32_t x = std::stoul(points[0]);
        uint32_t y = std::stoul(points[1]);

        /* Check if the tile data is relevant */
        if(geography.size() > section_index && 
           geography[section_index].size() > x && 
           geography[section_index][x].size() > y)
        {
          return modified_thing->setStartingTile(section_index, 
                                          geography[section_index][x][y], true);
        }
      }      
      
      return false;
    }

    /* Otherwise, add the thing information (virtual function) */
    return modified_thing->addThingInformation(data, kFILE_CLASSIFIER + 1, 
                                               section_index, renderer, 
                                               base_path);
  }

  return false;
}

std::vector<MapThing*> Map::getThingData(std::vector<int> thing_ids)
{
  std::vector<MapThing*> used_things;

  /* Loop through all the ids to find the associated things */
  for(auto i = thing_ids.begin(); i != thing_ids.end(); i++)
  {
    /* Only continue if the ID is valid and >= 0 */
    if(*i >= 0)
    {
      bool found = false;

      /* Loop through things */
      for(auto j = things.begin(); j != things.end(); j++)
      {
        if(!found && *i == (*j)->getID())
        {
          used_things.push_back(*j);
          found = true;
        }
      }

      /* Loop through persons */
      if(!found)
      {
        for(auto j = persons.begin(); j != persons.end(); j++)
        {
          if(!found && *i == (*j)->getID())
          {
            used_things.push_back(static_cast<MapThing*>(*j));
            found = true;
          }
        }
      }

      /* Loop through items */
      if(!found)
      {
        for(auto j = items.begin(); j != items.end(); j++)
        {
          if(!found && *i == (*j)->getID())
          {
            used_things.push_back(static_cast<MapThing*>(*j));
            found = true;
          }
        }
      }
    }
  }

  return used_things;
}

bool Map::initiateMapSection(uint16_t section_index, int width, int height)
{
  /* Check if the initiation will be viable */
  if(width > 0 && height > 0 && (geography.size() <= section_index || 
                                 geography[section_index].size() == 0))
  {
    /* Get the geography subset filled up past the point of being set */
    while(geography.size() <= section_index)
    {
      std::vector< std::vector<Tile*> > map_section;
      geography.push_back(map_section);
    }

    /* Fill the section */
    for(int i = 0; i < width; i++)
    {
      std::vector<Tile*> col;
      
      for(int j = 0; j < height; j++)
      {
        Tile* t = new Tile(event_handler, tile_width, tile_height, i, j);
        col.push_back(t);
      }
      
      geography[section_index].push_back(col);
    }

    return true;    
  }
  
  return false;
}

/* Initiates a thing action, based on the action key being hit */
void Map::initiateThingInteraction()
{
  if(player != NULL)
  {
    bool interacted = false;
    bool out_of_range = false;
    uint16_t x = player->getTile()->getX();
    uint16_t y = player->getTile()->getY();

    /* Determine the direction and offset coordinate tile selection */
    Direction direction = player->getDirection();
    if(direction == Direction::NORTH)
    {
      if(y == 0)
        out_of_range = true;
      else
        y--;
    }
    else if(direction == Direction::EAST)
      x++;
    else if(direction == Direction::SOUTH)
      y++;
    else if(direction == Direction::WEST)
    {
      if(x == 0)
        out_of_range = true;
      else
        x--;
    }
    /* Throw X out of bounds if no direction */
    else
      out_of_range = true;

    /* Aquire the thing, that's being pointed at and try to interact */
    if(!out_of_range && x < geography[map_index].size() && 
       y < geography[map_index][0].size())
    {
      if(geography[map_index][x][y]->isPersonSet() &&
         geography[map_index][x][y]->getPerson()->getTile()->getX() == x &&
         geography[map_index][x][y]->getPerson()->getTile()->getY() == y)
      {
        geography[map_index][x][y]->getPerson()->interact(player);
        interacted = true;
      }
      else if(geography[map_index][x][y]->isThingSet())
      {
        geography[map_index][x][y]->getThing()->interact(player);
        interacted = true;
      }
    }
    
    /* If there was no thing to interact with, proceed to try and pickup the
     * tile item. */
    if(!interacted && player->getTile()->getItem() != NULL && 
       player->getTile()->getItem()->getCount() > 0 && event_handler != NULL)
    {
      event_handler->executePickup(player->getTile()->getItem());
    }
  }
}

/* Parse coordinate info from file to give the designated tile coordinates
 * to update */
bool Map::parseCoordinateInfo(std::string row, std::string col, uint16_t index, 
                              uint16_t* r_start, uint16_t* r_end, 
                              uint16_t* c_start, uint16_t* c_end)
{
  std::vector<std::string> row_list = Helpers::split(row, '-');
  std::vector<std::string> col_list = Helpers::split(col, '-');
  if(row_list.size() > 0 && col_list.size() > 0)
  {
    /* Determine the row of parsing - limit to map size */
    *r_start += std::stoul(row_list.front());
    *r_end += std::stoul(row_list.back()) + 1;
    if(*r_start > geography[index].size())
      *r_start = geography[index].size();
    if(*r_end > geography[index].size())
      *r_end = geography[index].size();
    
    /* Determine the column of parsing - limit to map size */
    *c_start += std::stoul(col_list.front());
    *c_end += std::stoul(col_list.back()) + 1;
    if(*c_start > geography[index][*r_start].size())
      *c_start = geography[index][*r_start].size();
    if(*c_end > geography[index][*r_start].size())
      *c_end = geography[index][*r_start].size();
      
    return true;
  }
  
  return false;
}

/* Splits the ID into a vector of IDs */
std::vector< std::vector<uint16_t> > Map::splitIdString(std::string id, 
                                                        bool matrix)
{
  std::vector< std::vector<uint16_t> > id_stack;
  std::vector<std::string> xy_split = Helpers::split(id, ',');

  /* Only proceed if the string isn't empty and a split occurred */
  if(xy_split.size() > 0)
  {
    /* If it is a non-matrix split, handle accordingly */
    if(!matrix)
    {
      uint16_t id_max = 0;
      uint16_t id_min = 0;
      uint16_t multiplier_max = 0;
      uint16_t multiplier_min = 0;
    
      /* Work with the first element to determine the first and last row id */
      std::vector<std::string> x_split = Helpers::split(xy_split.front(), '-');
      if(x_split.size() > 0)
      {
        id_min = std::stoul(x_split.front());
        id_max = std::stoul(x_split.back());
      }
      
      /* Check if there is a multiplier element to work with */
      if(xy_split.size() > 1)
      {
        std::vector<std::string> y_split = Helpers::split(xy_split.back(), '-');
        if(y_split.size() > 0)
        {
          multiplier_min = std::stoul(y_split.front());
          multiplier_max = std::stoul(y_split.back());
        }
      }
      
      /* Loop through multipliers and id min and max to construct ID matrix */
      for(uint16_t mult = multiplier_min; mult <= multiplier_max; mult++)
      {
        uint16_t addition = (id_max - id_min + 1) * mult;
        std::vector<uint16_t> id_row;
        
        for(uint16_t id_x = id_min; id_x <= id_max; id_x++)
          id_row.push_back(id_x + addition);
        id_stack.push_back(id_row);
      }
    }
    /* Otherwise, it's an explicitely defined matrix */
    else
    {
      for(uint16_t i = 0; i < xy_split.size(); i++)
      {
        std::vector<uint16_t> id_row;
        
        std::vector<std::string> row_split = Helpers::split(xy_split[i], '.');
        for(uint16_t j = 0; j < row_split.size(); j++)
          id_row.push_back(std::stoul(row_split[j]));
          
        id_stack.push_back(id_row);
      }
    }
  }
  
  return id_stack;
}

// /* Updates the height and width, based on zoom factors */
// void Map::updateTileSize()
// {
  // bool updated = false;
  
  // /* Try and zoom out the map */
  // if(zoom_out)
  // {
    // /* Modify the tile height and width, limited by the constants */
    // tile_height--;
    // tile_width--;
    // if(tile_height < kZOOM_TILE_SIZE || tile_width < kZOOM_TILE_SIZE)
    // {
      // tile_height = kZOOM_TILE_SIZE;
      // tile_width = kZOOM_TILE_SIZE;
      // zoom_out = false;
    // }
    
    // updated = true;
  // }
  // /* Otherwise, try and zoom back in */
  // else if(zoom_in)
  // {
    // /* Modify the tile height and width, limited by the constants */
    // tile_height++;
    // tile_width++;
    // if(tile_height > kTILE_SIZE || tile_width > kTILE_SIZE)
    // {
      // tile_height = kTILE_SIZE;
      // tile_width = kTILE_SIZE;
      // zoom_in = false;
    // }
    
    // updated = true;
  // }
  
  // /* If updated, update the height and width everywhere */
  // if(updated)
  // {
    // /* Update map tiles */
    // for(int i = 0; i < geography.size(); i++)
      // for(int j = 0; j < geography[i].size(); j++)
        // for(int k = 0; k < geography[i][j].size(); k++)
        // {
          // geography[i][j][k]->setHeight(tile_height);
          // geography[i][j][k]->setWidth(tile_width);
        // }
        
    // /* Update map things */
    // for(int i = 0; i < things.size(); i++)
    // {
      // things[i]->setHeight(tile_height);
      // things[i]->setWidth(tile_width);
    // }
    
    // /* Update map items */
    // for(int i = 0; i < items.size(); i++)
    // {
      // items[i]->setHeight(tile_height);
      // items[i]->setWidth(tile_width);
    // }
    
    // /* Update map persons */
    // for(int i = 0; i < persons.size(); i++)
    // {
      // persons[i]->setHeight(tile_height);
      // persons[i]->setWidth(tile_width);
    // }
    
    // /* Update viewport */
    // if(viewport != 0)
      // viewport->setTileSize(tile_width, tile_height);
  // }
// }
  
// /*============================================================================
 // * PROTECTED FUNCTIONS
 // *===========================================================================*/

// /* TODO: Possibly add it so that any map can keep updating? This might get too
 // *       heavy. 
 // *       Should updates be restricted to a larger space around the viewport
 // *       and no more? */
// void Map::animate(short time_since_last)
// {
  // /* Only proceed if the animation time is positive */
  // if(time_since_last > 0)
  // {
    // /* The movement handling for persons */
    // for(int i = 0; i < persons.size(); i++)
    // {
      // if(persons[i] != 0)
      // {
        // Tile* next_tile = 0;

        // if(persons[i]->getMapSection() == map_index && 
           // persons[i]->getTile() != 0)
        // {
          // int tile_x = persons[i]->getTile()->getX();
          // int tile_y = persons[i]->getTile()->getY();

          // /* Based on the move request, provide the next tile in line using the
           // * current centered tile and move request */
          // switch(persons[i]->getPredictedMoveRequest())
          // {
            // case EnumDb::NORTH:
              // if(--tile_y >= 0)
                // next_tile = geography[map_index][tile_x][tile_y];
              // break;
            // case EnumDb::EAST:
              // if(++tile_x < geography[map_index].size())
                // next_tile = geography[map_index][tile_x][tile_y];
              // break;
            // case EnumDb::SOUTH:
              // if(++tile_y < geography[map_index][tile_x].size())
                // next_tile = geography[map_index][tile_x][tile_y];
              // break;
            // case EnumDb::WEST:
              // if(--tile_x >= 0)
                // next_tile = geography[map_index][tile_x][tile_y];
              // break;
            // case EnumDb::DIRECTIONLESS:
              // next_tile = 0;
          // }
        // }

        // /* Proceed to update the thing */
        // persons[i]->updateThing(time_since_last, next_tile);
      // }
    // }

    // /* Update the sprites on the map (tiles) */
    // for(int i = 0; i < tile_sprites.size(); i++)
    // {
      // if(tile_sprites[i] != 0)
        // tile_sprites[i]->updateSprite(time_since_last);
    // }
    
    // /* Update items on the map */
    // for(int i = 0; i < items.size(); i++)
    // {
      // if(items[i] != 0)
        // items[i]->updateThing(time_since_last, 0);
    // }

    // /* Update things on the map */
    // for(int i = 0; i < things.size(); i++)
    // {
      // if(things[i] != 0)
        // things[i]->updateThing(time_since_last, 0);
    // }

    // /* Animate the displays on the screen */
    // map_dialog.update(time_since_last);
  // }
// }

// void Map::initializeGL()
// {
  // glDisable(GL_TEXTURE_2D);
  // glDisable(GL_DEPTH_TEST);
  // glDisable(GL_COLOR_MATERIAL);
  // glEnable(GL_BLEND);
  // glEnable(GL_CULL_FACE); // Performance Add? Only for 3d
  // glEnable(GL_POLYGON_SMOOTH); // Causes strange lines drawn between tiles
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glClearColor(0, 0, 0, 0);

  // /* Initialize Gl in other classes */
  // map_dialog.initializeGl();
// }

// void Map::keyPressEvent(QKeyEvent* key_event)
// {
  // if(key_event->key() == Qt::Key_Escape)
  // {
    // if(player != 0)
    // {
      // player->clearAllMovement();
      // player->setSpeed(150);
    // }
    // emit closeMap();
  // }
  // else if(key_event->key() == Qt::Key_P)
    // map_dialog.setPaused(!map_dialog.isPaused());
  // else if(key_event->key() == Qt::Key_1 && player != 0)
    // player->resetPosition();
  // else if(key_event->key() == Qt::Key_4)
    // map_dialog.initNotification("Testing", 1000, true);
  // else if(key_event->key() == Qt::Key_5)
    // map_dialog.initNotification("This is a really long message. It goes on and on without end. Who makes notifications this long except for crazy deranged eutherlytes. Yes, I made a new word. You want to fight about it?");//map_dialog.haltDialog();
  // else if(key_event->key() == Qt::Key_6)
    // map_dialog.initNotification("This is a really long message. It goes on and on without end. Who makes notifications this long except for crazy deranged eutherlytes. Yes, I made a new word. You want to fight about it?", 5000, true);
  // else if(map_dialog.isInConversation())
    // map_dialog.keyPress(key_event);
  // else if(key_event->key() == Qt::Key_Shift && !key_event->isAutoRepeat())
  // {
    // if(player != 0)
      // player->setSpeed(300);
  // }
  // else if(key_event->key() == Qt::Key_Space)
    // initiateThingInteraction();
  // else if(key_event->key() == Qt::Key_Down || key_event->key() == Qt::Key_Up ||
          // key_event->key() == Qt::Key_Right || key_event->key() == Qt::Key_Left)
  // {
    // if(player != 0)
      // player->keyPress(key_event);
  // }
  // else if(key_event->key() == Qt::Key_F1)
    // zoom_out = true;
  // else if(key_event->key() == Qt::Key_F2)
    // zoom_in = true;
  // else if(key_event->key() == Qt::Key_F3)
    // viewport->lockOn(persons[1]);
  // else if(key_event->key() == Qt::Key_F4)
    // viewport->lockOn(player);
  // else if(key_event->key() == Qt::Key_F5)
    // qDebug() << "Player steps: " << player->getStepCount();
  // else if(key_event->key() == Qt::Key_0)
  // {
    // Event blank_event = event_handler->createBlankEvent();
    
    // Conversation* convo = new Conversation;
    // convo->category = EnumDb::TEXT;
    // convo->action_event = blank_event;
    // convo->text = "This is the initial conversation point that will start it. ";
    // convo->text += "How can this continue? It must pursue to complete ";
    // convo->text += "embodiment. Ok, maybe I'll just keep typing until I break ";
    // convo->text += "the entire compiler.";
    // convo->thing_id = 0;
    // Conversation test1, test2, test3, test4, test5;
    // test1.category = EnumDb::TEXT;
    // test1.action_event = blank_event;
    // test1.text = "This is a test to see how the data performs. The line will split once ";
    // test1.text += "unless it is an option based selection in which case it will restrict."; 
    // test1.thing_id = 3;
    // test2.category = EnumDb::TEXT;
    // test2.action_event = blank_event;
    // test2.text = "This is a no man case. See what happens!! Ok, this is the ";
    // test2.text += "too long case where the lines never cease to exist and the ";
    // test2.text += "points aren't for real. I'm feeling a bit hungry though ";
    // test2.text += "so I don't know if I'll have the stamina to clean up this ";
    // test2.text += "case in all it's glory. Repeat: ";
    // test2.text += test2.text;
    // test2.text += test2.text;
    // test2.thing_id = 2;
    // test2.next.append(test1);
    // test3.category = EnumDb::TEXT;
    // test3.action_event = blank_event;
    // test3.text = "Back to finish off with a clean case with a couple of lines.";
    // test3.text += " So this requires me to write a bunch of BS to try and fill";
    // test3.text += " these lines.";
    // test3.text += test3.text;
    // test3.text += test3.text;
    // test3.thing_id = 24;
    // test3.next.append(test2);
    // test4.category = EnumDb::TEXT;
    // test4.action_event = blank_event;
    // test4.text = "Option 1 - This goes on and on and on and on and on and lorem ipsum. This is way too long to be an option. Loser";
    // test4.thing_id = -1;
    // test4.next.append(test2);
    // test5.category = EnumDb::TEXT;
    // test5.action_event = blank_event;
    // test5.text = "Option 2";
    // test5.thing_id = -1;
    // test5.next.append(test3);
    // test1.next.append(test4);
    // test1.next.append(test5);
    // test4.text = "Option 3";
    // test1.next.append(test4);
    // test5.text = "Option 4";
    // test1.next.append(test5);
    // test4.text = "Option 5";
    // test1.next.append(test4);
    // test5.text = "Option 6";
    // test1.next.append(test5);
    // convo->next.append(test1);
    // event_handler->executeEvent(event_handler->createConversationEvent(convo),
                                // player);
    
    // delete convo;
  // }
// }

// void Map::keyReleaseEvent(QKeyEvent* key_event)
// {
  // if(map_dialog.isInConversation())
    // map_dialog.keyRelease(key_event);
  // else if(key_event->key() == Qt::Key_Down || key_event->key() == Qt::Key_Up ||
          // key_event->key() == Qt::Key_Right || key_event->key() == Qt::Key_Left)
  // {
    // if(player != 0)
      // player->keyRelease(key_event);
  // }
  // else if(key_event->key() == Qt::Key_Shift && !key_event->isAutoRepeat())
  // {
    // if(player != 0)
      // player->setSpeed(150);
  // }
// }

// /* TODO: might need to restrict animation for things out of the display
 // *       due to lag (high number of Things). Further testing required.
 // *
 // * Seem to be getting more consistent results for displaying, and it seems
 // * better with vsync disabled */
// void Map::paintGL()
// {
  // /* Start a QTimer to determine time elapsed for painting */
  // QTime time;
  // time.start();
  
  // /* Swap the buffers from the last call - put it on top or bottom? */
  // swapBuffers();
  // qDebug() << "1: " << time.elapsed();

  // /* Start by setting the context and clearing the screen buffers */
  // makeCurrent();
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glPushMatrix();

  // /* Only proceed if the map is loaded */
  // if(loaded)
  // {
    // /* Animate the map based on the elapsed time and then update
     // * the viewport due to the animate */
    // animate(time_elapsed);
    // viewport->updateView();

    // /* Grab the variables for viewport */
    // int tile_x_start = viewport->getXTileStart();
    // int tile_x_end = viewport->getXTileEnd();
    // int tile_y_start = viewport->getYTileStart();
    // int tile_y_end = viewport->getYTileEnd();
    // float x_offset = viewport->getX();
    // int x_start = viewport->getXStart();
    // int x_end = viewport->getXEnd();
    // float y_offset = viewport->getY();
    // int y_start = viewport->getYStart();
    // int y_end = viewport->getYEnd();
    
    // /* Paint the lower half */
    // for(int i = tile_x_start; i < tile_x_end; i++)
      // for(int j = tile_y_start; j < tile_y_end; j++)
        // geography[map_index][i][j]->paintLower(x_offset, y_offset);

    // /* Paint the walkover objects */
    // for(int i = tile_x_start; i < tile_x_end; i++)
    // {
      // for(int j = tile_y_start; j < tile_y_end; j++)
      // {
        // if(geography[map_index][i][j]->isItemSet() && 
           // geography[map_index][i][j]->getItem()->getCount() > 0)
        // {
          // geography[map_index][i][j]->getItem()->
                                                  // paintGl(x_offset, y_offset);
        // }
      // }
    // }

    // /* Paint all things on the map */
    // for(int i = 0; i < things.size(); i++)
    // {
      // if(things[i] != 0 && things[i]->getMapSection() == map_index && 
         // things[i]->getX() >= x_start && things[i]->getX() <= x_end && 
         // things[i]->getY() >= y_start && things[i]->getY() <= y_end)
      // {
        // things[i]->paintGl(x_offset, y_offset);
      // }
    // }

    // /* Paint all persons on the map */
    // for(int i = 0; i < persons.size(); i++)
    // {
      // if(persons[i] != 0 && persons[i]->getMapSection() == map_index && 
         // persons[i]->getX() >= x_start && persons[i]->getX() <= x_end && 
         // persons[i]->getY() >= y_start && persons[i]->getY() <= y_end)
      // {
        // persons[i]->paintGl(x_offset, y_offset);
      // }
    // }

    // /* Paint the upper half */
    // for(int i = tile_x_start; i < tile_x_end; i++)
      // for(int j = tile_y_start; j < tile_y_end; j++)
        // geography[map_index][i][j]->paintUpper(x_offset, y_offset);

    // /* Paint the dialog */
    // map_dialog.paintGl(this);
  // }

  // /* Paint the frame rate */
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glColor4f(0.0, 0.0, 0.0, 0.5);
  // glBegin(GL_QUADS);
    // glVertex3f(7, 40, 0);//7,40
    // glVertex3f(7, 10, 0);//7,10
    // glVertex3f(64, 10, 0);//64,10
    // glVertex3f(64, 40, 0);//64,40
  // glEnd();
  // glColor4f(1.0, 1.0, 1.0, 1.0);
  // renderText(20, 30, frames_per_second);

  // /* Clean up the drawing procedure */
  // glFlush();
  // glPopMatrix();
  // glFinish();
  
  // /* Determine the FPS sample rate */
  // if(paint_animation <= 0)
  // {
    // frames_per_second.setNum(paint_frames /(paint_time / 1000.0), 'f', 2);
    // paint_animation = 20;
  // }
  // paint_animation--;

  // /* Check the FPS monitor to see if it needs to be reset */
  // if((paint_frames % 100) == 0)
  // {
    // paint_frames = 0;
    // paint_time = 0;
  // }
  // paint_frames++;
  // paint_time += time_elapsed;
  
  // /* Finish by swapping the buffers and then restarting the timer to update 
   // * the map again */
  // swapBuffers();
// }

// void Map::resizeGL(int width, int height) 
// {
  // glViewport(0, 0, width, height);
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // gluOrtho2D(0, width, height, 0); // set origin to bottom left corner
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
// }

// /*============================================================================
 // * PUBLIC SLOTS
 // *===========================================================================*/

// void Map::finishThingTarget()
// {
  // if(player != 0)
  // {
    // if(player->getTarget() != 0)
      // player->getTarget()->clearTarget();
    // player->clearTarget();
  // }
// }

// void Map::getThingData(QList<int> thing_ids)
// {
  // QList<MapThing*> used_things;

  // /* Loop through all the ids to find the associated things */
  // for(int i = 0; i < thing_ids.size(); i++)
  // {
    // /* Only continue if the ID is valid and >= than 0 */
    // if(thing_ids[i] >= 0)
    // {
      // /* Compile a list of all MapThings */
      // QList<MapThing*> combined = things;
      // for(int k = 0; k < items.size(); k++)
        // combined.append((MapThing*)items[k]);
      // for(int k = 0; k < persons.size(); k++)
        // combined.append((MapThing*)persons[k]);

      // bool found = false;
      // int j = 0;

      // /* Loop through the stack of things to find the associated ID */
      // while(!found && j < combined.size())
      // {
        // if(combined[j]->getID() == thing_ids[i])
        // {
          // used_things.append(combined[j]);
          // found = true;
        // }
        // j++;
      // }
    // }
  // }

  // /* Set the dialog with the new stack of things */
  // map_dialog.setThingData(used_things);
// }

// /*============================================================================
 // * PUBLIC FUNCTIONS
 // *===========================================================================*/

// MapPerson* Map::getPlayer()
// {
  // return player;
// }

// /* Returns the map viewport, for scrolling through the scene */
// MapViewport* Map::getViewport()
// {
  // return viewport;
// }

// void Map::initialization()
// {
  // player->setFocus();
// }

// bool Map::initConversation(Conversation* convo, MapPerson* initiator, 
                                                // MapThing* source)
// {
  // if(player != 0 && map_dialog.initConversation(convo, initiator))
  // {
    // player->keyFlush();

    // /* Set the targets */
    // if(initiator != 0)
      // initiator->setTarget(source);
    // if(source != 0)
      // source->setTarget(initiator);

    // return true;
  // }

  // return false;
// }

// bool Map::initNotification(QString notification)
// {
  // return map_dialog.initNotification(notification);
// }

// /* Checks whether the viewport contains any tiles with the given sector */
// bool Map::isInSector(int index)
// {
  // (void)index;//warning
  // return true;
// }

bool Map::isLoaded()
{
  return loaded;
}

/* The key up and down events to be handled by the class */
bool Map::keyDownEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_1)
  {
    if(geography.size() > 0)
    {
      map_index = 0;
      viewport.setMapSize(geography[map_index].size(), 
                          geography[map_index][0].size());
    }
  }
  else if(event.keysym.sym == SDLK_2)
  {
    if(geography.size() > 1)
    {
      map_index = 1;
      viewport.setMapSize(geography[map_index].size(), 
                          geography[map_index][0].size());
    }
  }
  else if(event.keysym.sym == SDLK_p)
    map_dialog.setPaused(!map_dialog.isPaused());
  else if(map_dialog.isConversationActive())
    map_dialog.keyDownEvent(event);
  else if(event.keysym.sym == SDLK_0)
  {
    Event blank_event = event_handler->createBlankEvent();
    
    Conversation* convo = new Conversation;
    convo->category = DialogCategory::TEXT;
    convo->action_event = blank_event;
    convo->text = "This is the initial conversation point that will start it. ";
    convo->text += "How can this continue? It must pursue to complete ";
    convo->text += "embodiment. Ok, maybe I'll just keep typing until I break ";
    convo->text += "the entire compiler.";
    convo->thing_id = 0;
    Conversation test1, test2, test3, test4, test5;
    test1.category = DialogCategory::TEXT;
    test1.action_event = blank_event;
    test1.text = "This is a test to see how data runs. The line will split ";
    test1.text += "once unless it is an option based selection in which case ";
    test1.text += "it will restrict."; 
    test1.thing_id = 3;
    test2.category = DialogCategory::TEXT;
    test2.action_event = blank_event;
    test2.text = "This is a no man case. See what happens!! Ok, this is the ";
    test2.text += "too long case where the lines never cease to exist and the ";
    test2.text += "points aren't for real. I'm feeling a bit hungry though ";
    test2.text += "so I don't know if I'll have the stamina to clean up this ";
    test2.text += "case in all it's glory. Repeat: ";
    test2.text += test2.text;
    test2.text += test2.text;
    test2.thing_id = 2;
    test2.next.push_back(test1);
    test3.category = DialogCategory::TEXT;
    test3.action_event = event_handler->createStartBattleEvent();
    test3.text = "Back to finish off with a clean case with a couple of lines.";
    test3.text += " So this requires me to write a bunch of BS to try and fill";
    test3.text += " these lines.";
    test3.text += test3.text;
    test3.text += test3.text;
    test3.thing_id = 24;
    test3.next.push_back(test2);
    test4.category = DialogCategory::TEXT;
    test4.action_event = blank_event;
    test4.text = "Option 1 - This goes on and on and on and on and on and ";
    test4.text += "lorem ipsum. This is way too long to be an option. Loser";
    test4.thing_id = -1;
    test4.next.push_back(test2);
    test5.category = DialogCategory::TEXT;
    test5.action_event = blank_event;
    test5.text = "Option 2";
    test5.thing_id = -1;
    test5.next.push_back(test3);
    test1.next.push_back(test4);
    test1.next.push_back(test5);
    test4.text = "Option 3";
    test1.next.push_back(test4);
    test5.text = "Option 4";
    test1.next.push_back(test5);
    test4.text = "Option 5";
    test1.next.push_back(test4);
    test5.text = "Option 6";
    test1.next.push_back(test5);
    convo->next.push_back(test1);

    /* Run the conversation and then delete */
    if(map_dialog.initConversation(convo, player))
    {
      std::vector<int> list = map_dialog.getConversationIDs();
      map_dialog.setConversationThings(getThingData(list));
    }

    delete convo;
  }
  else if(event.keysym.sym == SDLK_SPACE)
    initiateThingInteraction();
  else if(player != NULL)
  {
    if((event.keysym.sym == SDLK_LSHIFT || event.keysym.sym == SDLK_RSHIFT) && 
       !running)
    {
      running = true;
      player->setSpeed(player->getSpeed() * 2);
    }
    else if(event.keysym.sym == SDLK_3)
    {
      viewport.lockOn(player);
    }
    else if(event.keysym.sym == SDLK_4)
    {
      if(persons.size() >= 1)
        viewport.lockOn(persons[1]);
    }
    else
      player->keyDownEvent(event);
  }
  
  return false;
}

void Map::keyUpEvent(SDL_KeyboardEvent event)
{
  if(map_dialog.isConversationActive())
    map_dialog.keyUpEvent(event);
  else if(player != NULL)
  {
    if((event.keysym.sym == SDLK_LSHIFT || event.keysym.sym == SDLK_RSHIFT) && 
       running)
    {
      running = false;
      player->setSpeed(player->getSpeed() / 2);
    }
    else
      player->keyUpEvent(event);
  }
}

// TODO: Separate file add success and XML read success to parse error
bool Map::loadMap(std::string file, SDL_Renderer* renderer, bool encryption)
{
  bool done = false;
  bool success = true;
  FileHandler fh(file, false, true, encryption);
  XmlData data;

  /* Set up the white mask, if it isn't done */
  if(!white_mask.isImageSet())
  {
    SDL_Texture* texture = SDL_CreateTexture(renderer, 
                                             SDL_PIXELFORMAT_RGBA8888, 
                                             SDL_TEXTUREACCESS_TARGET, 
                                             kTILE_SIZE, kTILE_SIZE);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderDrawRect(renderer, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    
    /* Set the new texture as the white mask (with additive blending) */
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    white_mask.setTexture(texture);
  }
  
  /* Start the map read */
  success &= fh.start();
  std::cout << "Date: " << fh.getDate() << std::endl; // TODO: Remove
  
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
      if(data.getElement(kFILE_GAME_TYPE) == "map")
      {
        if(data.getElement(kFILE_SECTION_ID) == "sprite" &&
           !data.getKeyValue(kFILE_SECTION_ID).empty())
        {
          success &= addSpriteData(data, data.getKeyValue(kFILE_SECTION_ID), 
                                   kFILE_SECTION_ID + 1, renderer);
        }
        else
        {
          int temp_index = -1;
          
          /* Determine if it's a new section */
          if(data.getElement(kFILE_SECTION_ID) == "main" && index != 0)
            temp_index = 0;
          else if(data.getElement(kFILE_SECTION_ID) == "section" && 
                  index != std::stoi(data.getKeyValue(kFILE_SECTION_ID)))
            temp_index = std::stoi(data.getKeyValue(kFILE_SECTION_ID));
          
          /* Update the index information accordingly */
          if(temp_index >= 0)
          {
            index = temp_index;
            if(geography.size() > static_cast<uint16_t>(index) && 
               geography[index].size() > 0)
            {
              height = geography[index].front().size();
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
          {
            /* Tile data */
            if(data.getElement(kFILE_CLASSIFIER) == "base" ||
               data.getElement(kFILE_CLASSIFIER) == "enhancer" ||
               data.getElement(kFILE_CLASSIFIER) == "lower" ||
               data.getElement(kFILE_CLASSIFIER) == "upper" ||
               data.getElement(kFILE_CLASSIFIER) == "tileevent")
            {
              success &= addTileData(data, index);
            }
            /* Thing data */
            else if(data.getElement(kFILE_CLASSIFIER) == "mapthing" || 
                    data.getElement(kFILE_CLASSIFIER) == "mapperson" ||
                    data.getElement(kFILE_CLASSIFIER) == "mapnpc" ||
                    data.getElement(kFILE_CLASSIFIER) == "mapitem" || 
                    data.getElement(kFILE_CLASSIFIER) == "mapio")
            {
              success &= addThingData(data, index, renderer);
            }
          }
        }
      }
      
      /* Get the next element */
      data = fh.readXmlData(&done, &success);
    } while(!done && success);
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
      viewport.setMapSize(geography[map_index].size(), 
                          geography[map_index][0].size());

      if(player != NULL)
        viewport.lockOn(player);
    }
    
    /* Go through all map things and add applicable modifications */
    for(uint16_t i = 0; i < items.size(); i++)
     items[i]->setWhiteMask(white_mask.getTexture());
    for(uint16_t i = 0; i < persons.size(); i++)
      persons[i]->setWhiteMask(white_mask.getTexture());
    for(uint16_t i = 0; i < things.size(); i++)
      things[i]->setWhiteMask(white_mask.getTexture());
    
    /* Load map dialog sprites - TODO: In file? */
    map_dialog.loadImageConversation("sprites/Overlay/dialog.png", renderer);
    map_dialog.loadImageDialogShifts("sprites/Overlay/dialog_next.png", 
                                     "sprites/Overlay/dialog_extender.png", 
                                     renderer);
    map_dialog.loadImageNameLeftRight(
                                 "sprites/Overlay/dialog_corner.png", renderer);
    map_dialog.loadImageOptions("sprites/Overlay/option_circle.png", 
                                "sprites/Overlay/option_triangle.png", 
                                renderer);
    map_dialog.loadImagePickupTopBottom(
                           "sprites/Overlay/notification_corner.png", renderer);

    /* TODO: Testing - Remove */
    if(geography.size() > 0 && geography[0].size() > 3 
                            && geography[0][3].size() > 3)
      geography[0][3][3]->setStatus(Tile::OFF);
  }
  loaded = success;

  return success;
}

// /* Checks the tile you are attempting to enter for passibility of the given
 // * direction */
// bool Map::passible(EnumDb::Direction dir, int x, int y)
// {
  // (void)dir;//warning
  // (void)x;//warning
  // (void)y;//warning
  // return true;
// }

// /* Causes the thing you are moving into to start its interactive action */
// void Map::passOver()
// {
// }

// /* Proceeds to pickup the total number of this marked item */
// bool Map::pickupItem(MapItem* item)
// {
  // if(item != 0)
  // {
    // Frame* dialog_image = item->getDialogImage();
    // Sprite* map_image = item->getFrames();
    
    // /* Show pickup dialog */
    // if(dialog_image != 0 && dialog_image->isImageSet())
      // map_dialog.initPickup(dialog_image, item->getCount());
    // else if(map_image != 0 && map_image->getSize() > 0)
      // map_dialog.initPickup(map_image->getFirstFrame(), item->getCount());
      
    // /* Finally, set the on map count to 0 */
    // item->setCount(0);
    
    // return true;
  // }
  
  // return false;
// }

/* Renders the title screen */
bool Map::render(SDL_Renderer* renderer)
{
  bool success = true;
  
  if(geography.size() > map_index)
  {
    /* Grab the variables for viewport */
    uint16_t tile_x_start = viewport.getXTileStart();
    uint16_t tile_x_end = viewport.getXTileEnd();
    uint16_t tile_y_start = viewport.getYTileStart();
    uint16_t tile_y_end = viewport.getYTileEnd();
    float x_offset = viewport.getX();
    int x_start = viewport.getXStart();
    int x_end = viewport.getXEnd();
    float y_offset = viewport.getY();
    int y_start = viewport.getYStart();
    int y_end = viewport.getYEnd();

    /* Render lower half of tile */
    for(uint16_t i = tile_x_start; i < tile_x_end; i++)
      for(uint16_t j = tile_y_start; j < tile_y_end; j++)
        geography[map_index][i][j]->renderLower(renderer, x_offset, y_offset);

    /* Render Map Items */
    for(uint16_t i = 0; i < items.size(); i++)
    {
      if(items[i]->getMapSection() == map_index && 
         items[i]->getX() >= x_start && items[i]->getX() <= x_end && 
         items[i]->getY() >= y_start && items[i]->getY() <= y_end)
      {
        items[i]->render(renderer, x_offset, y_offset);
      }
    }
    
    /* Render Map Things */
    for(uint16_t i = 0; i < things.size(); i++)
    {
      if(things[i]->getMapSection() == map_index && 
         things[i]->getX() >= x_start && things[i]->getX() <= x_end && 
         things[i]->getY() >= y_start && things[i]->getY() <= y_end)
      {
        things[i]->render(renderer, x_offset, y_offset);
      }
    }

    /* Render Map Persons (and NPCs) */
    for(uint16_t i = 0; i < persons.size(); i++)
    {
      if(persons[i]->getMapSection() == map_index && 
         persons[i]->getX() >= x_start && persons[i]->getX() <= x_end && 
         persons[i]->getY() >= y_start && persons[i]->getY() <= y_end)
      {
        persons[i]->render(renderer, x_offset, y_offset);
      }
    }

    /* Render upper half of tile */
    for(uint16_t i = tile_x_start; i < tile_x_end; i++)
      for(uint16_t j = tile_y_start; j < tile_y_end; j++)
        geography[map_index][i][j]->renderUpper(renderer, x_offset, y_offset);
    
    /* Render the map dialog */
    map_dialog.render(renderer);
  }
  
  return success;
}

/* Sets the running configuration, from the options class */
bool Map::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    base_path = system_options->getBasePath();
    
    /* Update the viewport information */
    viewport.setSize(running_config->getScreenWidth(), 
                     running_config->getScreenHeight());

    /* Update the map dialog information */
    map_dialog.setConfiguration(running_config);

    return true;
  }
  
  return false;
}

// /* Changes the map section index - what is displayed */
// bool Map::setSectionIndex(int index)
// {
  // if(index >= 0 && index < geography.size() && geography[index].size() > 0)
  // {
    // map_index = index;
    // viewport->setMapSize(geography[index].size(), geography[index][0].size());
    
    // return true;
  // }
  
  // return false;
// }
  
// Possibly make the teleport add the ability of shifting map thing
// void Map::teleportThing(int id, int tile_x, int tile_y, int section_id)
// {
  // /* If the section id is below 0, then set to internal map index */
  // if(section_id < 0)
    // section_id = map_index;
  
  // /* Ensure that the tile x and y is within the range */
  // if(section_id < geography.size() && 
     // geography[section_id].size() > tile_x && tile_x >= 0 && 
     // geography[section_id][tile_x].size() > tile_y && tile_y >= 0)
  // {
    // /* Change the starting tile for the thing */
    // for(int i = 0; i < persons.size(); i++)
    // {
      // if(persons[i]->getID() == id)
      // {
        // if(persons[i]->setStartingTile(section_id, 
                                       // geography[section_id][tile_x][tile_y]))
        // {
          // map_dialog.endConversation();
          // if(map_index != section_id)
            // setSectionIndex(section_id);
          // persons[i]->clearAllMovement();
        // }
      // }
    // }
  // }
// }

void Map::unfocus()
{
  /* If player is set, clear movement */
  if(player != NULL)
  {
    player->keyFlush();
    
    /* If player was running, reset it */
    if(running)
      player->setSpeed(player->getSpeed() / 2);
    running = false;
  } 
}

void Map::unloadMap()
{
  /* Reset the index and applicable parameters */
  map_index = 0;
  tile_height = kTILE_SIZE;
  tile_width = kTILE_SIZE;
  
  // /* Delete the items */
  // for(int i = 0; i < items.size(); i++)
  // {
    // if(items[i] != 0)
      // delete items[i];
    // items[i] = 0;
  // }
  // items.clear();

  /* Delete the persons */
  for(uint16_t i = 0; i < persons.size(); i++)
  {
    if(persons[i] != NULL)
      delete persons[i];
    persons[i] = NULL;
  }
  persons.clear();
  
  /* Delete the things */
  for(uint16_t i = 0; i < things.size(); i++)
  {
    if(things[i] != NULL)
      delete things[i];
    things[i] = NULL;
  }
  things.clear();
  
  /* Delete all the tiles that have been set */
  for(uint16_t i = 0; i < geography.size(); i++)
  {
    for(uint16_t j = 0; j < geography[i].size(); j++)
    {
      for(uint16_t k = 0; k < geography[i][j].size(); k++)
      {
        delete geography[i][j][k];
        geography[i][j][k] = NULL;
      }
      geography[i][j].clear();
    }
    geography[i].clear();
  }
  geography.clear();

  /* Deletes the sprite data stored */
  for(uint16_t i = 0; i < tile_sprites.size(); i++)
  {
    delete tile_sprites[i];
    tile_sprites[i] = 0;
  }
  tile_sprites.clear();
  
  /* Reset the viewport */
  viewport.setMapSize(0, 0);
  viewport.lockOn(0, 0);

  /* Clear the remaining and disable the loading */
  // clear();
  loaded = false;
}

/* Updates the game state */
bool Map::update(int cycle_time)
{
  /* Update the sprite animation */
  for(uint16_t i = 0; i < tile_sprites.size(); i++)
    tile_sprites[i]->update(cycle_time);

  /* Update map items */
  for(uint16_t i = 0; i < items.size(); i++)
    items[i]->update(cycle_time, NULL);
    
  /* Update persons for movement and animation */
  for(uint16_t i = 0; i < persons.size(); i++)
  {
    if(persons[i] != NULL)
    {
      Tile* next_tile = NULL;

      if(persons[i]->getMapSection() == map_index && 
         persons[i]->getTile() != NULL)
      {
        uint16_t tile_x = persons[i]->getTile()->getX();
        uint16_t tile_y = persons[i]->getTile()->getY();

        /* Based on the move request, provide the next tile in line using the
         * current centered tile and move request */
        switch(persons[i]->getPredictedMoveRequest())
        {
          case Direction::NORTH:
            if(tile_y-- > 0)
              next_tile = geography[map_index][tile_x][tile_y];
            break;
          case Direction::EAST:
            if(++tile_x < geography[map_index].size())
              next_tile = geography[map_index][tile_x][tile_y];
            break;
          case Direction::SOUTH:
            if(++tile_y < geography[map_index][tile_x].size())
              next_tile = geography[map_index][tile_x][tile_y];
            break;
          case Direction::WEST:
            if(tile_x-- > 0)
              next_tile = geography[map_index][tile_x][tile_y];
            break;
          case Direction::DIRECTIONLESS:
            next_tile = NULL;
        }
      }

      /* Proceed to update the thing */
      persons[i]->update(cycle_time, next_tile);
    }
  }
  
  /* Update map things */
  for(uint16_t i = 0; i < things.size(); i++)
    things[i]->update(cycle_time, NULL);
  
  /* Finally, update the viewport and dialog */
  map_dialog.update(cycle_time);
  viewport.update();
  
  return false;
}

// /* Updates the map - called by the cycle timer call from game */
// void Map::updateMap(int cycle_time)
// {
  // time_elapsed = cycle_time;
  // updateTileSize();
  // updateGL();
// }
