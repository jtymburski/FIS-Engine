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
const uint8_t Map::kFADE_BLACK = 255;
const uint8_t Map::kFADE_FACTOR = 3;
const uint8_t Map::kFADE_MAX = 5;
const uint8_t Map::kFADE_VIS = 0;
const uint8_t Map::kFILE_CLASSIFIER = 3;
const uint8_t Map::kFILE_GAME_TYPE = 1;
const uint8_t Map::kFILE_SECTION_ID = 2;
const uint8_t Map::kFILE_TILE_COLUMN = 5;
const uint8_t Map::kFILE_TILE_ROW = 4;
const uint32_t Map::kMUSIC_REPEAT = 300000; /* 5 minutes */
const uint8_t Map::kPLAYER_ID = 0;
const uint16_t Map::kZOOM_TILE_SIZE = 16;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Map::Map(Options* running_config, EventHandler* event_handler)
{
  /* Set initial variables */
  base_path = "";
  this->event_handler = NULL;
  fade_alpha = 255;
  fade_status = BLACK;
  loaded = false;
  map_index = 0;
  map_index_next = -1;
  mode_curr = DISABLED;
  mode_next = NONE;
  music_id = -1;
  music_runtime = -1;
  player = NULL;
  system_options = NULL;
  view_acc = 0;
  view_section = 0;
  view_start = false;
  view_thing = nullptr;
  view_tile = nullptr;
  view_time = 0;
  view_travel = false;

  /* Configure the width / height of tiles and sets default zooming */
  tile_height = Helpers::getTileSize();
  tile_width = tile_height;
  zoom_in = false;
  zoom_out = false;
  viewport.setTileSize(tile_width, tile_height);

  /* Set options and event handler */
  setConfiguration(running_config);
  setEventHandler(event_handler);
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
  int32_t access_id = -1;
  Sprite* access_sprite = NULL;
  int32_t copy_id = -1;
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
  if(access_id >= 0 && access_id < 65535)
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
    std::vector< std::vector<int32_t> > id_matrix;
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
        if(id_matrix[i][j] >= 0)
        {
          for(uint32_t k = 0; k < tile_sprites.size(); k++)
          {
            if(tile_sprites[k]->getId() == id_matrix[i][j])
              found_sprite = tile_sprites[k];
          }
        }

        /* If the sprite is found, add it to the tile */
        // TODO: Review revising to incorporate new helper function.
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
                success &= sub_map[section_index].tiles[r][c]->
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
            success &= sub_map[section_index].tiles[r][c]->
                    addPassability(data.getDataString(),
                                   data.getElement(kFILE_CLASSIFIER),
                                   data.getKeyValue(kFILE_CLASSIFIER));
          }
          /* Otherwise, it's a tile event */
          else
          {
            /* Classify between enter and exit events */
            if(data.getKeyValue(kFILE_CLASSIFIER) == "enter" ||
               data.getKeyValue(kFILE_CLASSIFIER) == "enterset")
            {
              success &= sub_map[section_index].tiles[r][c]->
                    updateEventEnter(data, kFILE_CLASSIFIER, section_index);
            }
            else if(data.getKeyValue(kFILE_CLASSIFIER) == "exit" ||
                    data.getKeyValue(kFILE_CLASSIFIER) == "exitset")
            {
              success &= sub_map[section_index].tiles[r][c]->
                     updateEventExit(data, kFILE_CLASSIFIER, section_index);
            }
          }
        }
      }
      index++;
    }
    return success;
  }
  return false;
}

// TODO: Comment
bool Map::addThingBaseData(XmlData data, int file_index, SDL_Renderer* renderer)
{
  std::string identifier = data.getElement(file_index);
  uint16_t id = std::stoul(data.getKeyValue(file_index));
  MapThing* modified_thing = NULL;
  bool new_thing = false;

  /* Identify which thing to be created */
  if(identifier == "mapthing")
  {
    /* Create a new thing, if one doesn't exist */
    modified_thing = getThingBase(id);
    if(modified_thing == NULL)
    {
      modified_thing = new MapThing();
      new_thing = true;
      base_things.push_back(modified_thing);
    }
  }
  else if(identifier == "mapio")
  {
    /* Create a new map interactive object, if one doesn't exist */
    modified_thing = getIOBase(id);
    if(modified_thing == NULL)
    {
      modified_thing = new MapInteractiveObject();
      new_thing = true;
      base_ios.push_back(static_cast<MapInteractiveObject*>(modified_thing));
    }
  }
  else if(identifier == "mapperson" || identifier == "mapnpc")
  {
    /* Create a new person, if one doesn't exist */
    modified_thing = getPersonBase(id);
    if(modified_thing == NULL)
    {
      if(identifier == "mapperson")
        modified_thing = new MapPerson();
      else
        modified_thing = new MapNPC();
      new_thing = true;
      base_persons.push_back(static_cast<MapPerson*>(modified_thing));
    }
  }
  else if(identifier == "mapitem")
  {
    /* Create a new item, if one doesn't exist */
    modified_thing = getItemBase(id);
    if(modified_thing == NULL)
    {
      modified_thing = new MapItem();
      new_thing = true;
      base_items.push_back(static_cast<MapItem*>(modified_thing));
    }
  }

  /* If new, set the ID and event handler */
  if(new_thing)
  {
    modified_thing->setEventHandler(event_handler);
    modified_thing->setID(id);
  }

  /* Proceed to update the thing information from the XML data */
  if(modified_thing != NULL)
    return modified_thing->addThingInformation(data, file_index + 1, 0,
                                               renderer, base_path);
  return false;
}

// TODO: Comment
bool Map::addThingData(XmlData data, uint16_t section_index,
                       SDL_Renderer* renderer)
{
  int32_t base_id = -1;
  std::string identifier = data.getElement(kFILE_CLASSIFIER);
  uint16_t id = std::stoul(data.getKeyValue(kFILE_CLASSIFIER));
  MapThing* modified_thing = NULL;
  bool new_thing = false;
  bool success = true;

  /* Check if it's base */
  if(data.getElement(kFILE_CLASSIFIER + 1) == "base")
    base_id = data.getDataInteger();

  /* Identify which thing to be created */
  if(identifier == "mapthing")
  {
    /* Create a new thing, if one doesn't exist */
    modified_thing = getThing(id);
    if(modified_thing == NULL)
    {
      modified_thing = new MapThing();
      new_thing = true;
      things.push_back(modified_thing);
    }

    /* Attempt to add base, if applicable */
    if(base_id >= 0)
      success &= modified_thing->setBase(getThingBase(base_id));
  }
  else if(identifier == "mapio")
  {
    /* Create a new MIO, if one doesn't exist */
    modified_thing = getIO(id);
    if(modified_thing == NULL)
    {
      modified_thing = new MapInteractiveObject();
      modified_thing->setEventHandler(event_handler);
      new_thing = true;
      ios.push_back(static_cast<MapInteractiveObject*>(modified_thing));
    }

    /* Attempt to add base, if applicable */
    if(base_id >= 0)
      success &= modified_thing->setBase(getIOBase(base_id));
  }
  else if(identifier == "mapperson" || identifier == "mapnpc")
  {
    /* Create a new person, if one doesn't exist */
    modified_thing = getPerson(id);
    if(modified_thing == NULL)
    {
      if(identifier == "mapperson")
        modified_thing = new MapPerson();
      else
        modified_thing = new MapNPC();
      new_thing = true;
      persons.push_back(static_cast<MapPerson*>(modified_thing));

      /* If the ID is the player ID, tie to the player */
      if(id == kPLAYER_ID)
      {
        player = static_cast<MapPerson*>(modified_thing);
        if(system_options != NULL && system_options->isAutoRun())
          player->setRunning(true);
      }
    }

    /* Attempt to add base, if applicable */
    if(base_id >= 0)
      success &= modified_thing->setBase(getPersonBase(base_id));
  }
  else if(identifier == "mapitem")
  {
    /* Create a new item, if one doesn't exist */
    modified_thing = getItem(id);
    if(modified_thing == NULL)
    {
      modified_thing = new MapItem();
      new_thing = true;
      items.push_back(static_cast<MapItem*>(modified_thing));
    }

    /* Attempt to add base, if applicable */
    if(base_id >= 0)
      success &= modified_thing->setBase(getItemBase(base_id));
  }

  /* If new, set the ID and event handler */
  if(new_thing)
  {
    modified_thing->setEventHandler(event_handler);
    modified_thing->setID(id);
  }

  /* Proceed to update the thing information from the XML data */
  if(modified_thing != NULL)
  {
    success &= modified_thing->addThingInformation(
                                      data, kFILE_CLASSIFIER + 1, section_index,
                                      renderer, base_path);
    return success;
  }

  return false;
}

/* Audio start/stop triggers */
void Map::audioStart()
{
  /* Try and stop, if relevant */
  if(music_id >= 0)
    audioStop();

  /* Fire the ship up */
  audioUpdate(true);
}

/* Audio start/stop triggers */
void Map::audioStop()
{
  if(event_handler)
  {
    if(map_index < sub_map.size() && music_id >= 0)
    {
      event_handler->triggerAudioStop(SoundChannels::MUSIC1);
      if(sub_map[map_index].weather >= 0)
        event_handler->triggerAudioStop(SoundChannels::WEATHER1);
    }
  }

  music_id = -1;
  music_runtime = -1;
}

/* Audio update trigger. Either on song switch time or sub-map change */
void Map::audioUpdate(bool sub_change)
{
  if(event_handler != nullptr && map_index < sub_map.size())
  {
    /* Find music index */
    bool music_base = true;
    int music_index = -1;
    if(sub_map[map_index].music.size() > 0)
    {
      music_base = false;
      music_index = Helpers::randInt(sub_map[map_index].music.size() - 1);
    }
    else if(sub_map[0].music.size() > 0)
    {
      music_base = true;
      music_index = Helpers::randInt(sub_map[0].music.size() - 1);
    }

    /* If index found, process it */
    if(music_index >= 0)
    {
      /* Check ID */
      int new_id = -1;
      if(music_base)
        new_id = sub_map[0].music[music_index];
      else
        new_id = sub_map[map_index].music[music_index];

      /* If different, update */
      if(new_id != music_id)
      {
        std::cout << "   > Map Music Trigger ID: " << new_id << std::endl;
        music_id = new_id;
        event_handler->triggerMusic(music_id);
      }

      music_runtime = kMUSIC_REPEAT;
    }
    else
    {
      music_id = -1;
    }

    /* Weather */
    if(sub_change)
    {
      /* Find weather */
      if(sub_map[map_index].weather >= 0)
        event_handler->triggerWeather(sub_map[map_index].weather);
      else
        event_handler->triggerAudioStop(SoundChannels::WEATHER1);
    }
  }
}

/* Change the mode that the game is running */
bool Map::changeMode(MapMode mode)
{
  /* Run logic to determine if mode switch is allowed - currently always true */
  bool allow = true;

  /* If allowed, make change */
  if(allow && mode_next != mode)// && mode_curr != mode)
  {
    /* Changes to execute on the old modes - if relevant */
    //if(this->mode == MAP)
    //  map_ctrl.enableView(false);

    mode_next = mode;

    /* PRINT - TEMPORARY */
    if(mode_next == DISABLED)
      std::cout << "-MAP MODE: DISABLED" << std::endl;
    else if(mode_next == NORMAL)
      std::cout << "-MAP MODE: NORMAL" << std::endl;
    else if(mode_next == SWITCHSUB)
      std::cout << "-MAP MODE: SWITCH SUB" << std::endl;
    else if(mode_next == VIEW)
      std::cout << "-MAP MODE: VIEW" << std::endl;
    else if(mode_next == NONE)
      std::cout << "-MAP MODE: NONE" << std::endl;

    /* Changes to execute on the new modes - if relevant */
    //if(this->mode == MAP)
    //  map_ctrl.enableView(true);
  }

  return allow;
}

/* Returns the interactive object, based on the ID */
MapInteractiveObject* Map::getIO(uint16_t id)
{
  for(uint32_t i = 0; i < ios.size(); i++)
    if(ios[i]->getID() == id)
      return ios[i];
  return NULL;
}

/* Returns the base interactive object, based on the ID */
MapInteractiveObject* Map::getIOBase(uint16_t id)
{
  for(uint32_t i = 0; i < base_ios.size(); i++)
    if(base_ios[i]->getID() == id)
      return base_ios[i];
  return NULL;
}

/* Returns the item, based on the ID */
MapItem* Map::getItem(uint16_t id)
{
  for(uint32_t i = 0; i < items.size(); i++)
    if(items[i]->getID() == id)
      return items[i];
  return NULL;
}

/* Returns the base item, based on the ID */
MapItem* Map::getItemBase(uint16_t id)
{
  for(uint32_t i = 0; i < base_items.size(); i++)
    if(base_items[i]->getID() == id)
      return base_items[i];
  return NULL;
}

/* Returns the person, based on the ID */
MapPerson* Map::getPerson(uint16_t id)
{
  for(uint32_t i = 0; i < persons.size(); i++)
    if(persons[i]->getID() == id)
      return persons[i];
  return NULL;
}

/* Returns the base person, based on the ID */
MapPerson* Map::getPersonBase(uint16_t id)
{
  for(uint32_t i = 0; i < base_persons.size(); i++)
    if(base_persons[i]->getID() == id)
      return base_persons[i];
  return NULL;
}

/* Returns the thing, based on the ID */
MapThing* Map::getThing(uint16_t id)
{
  for(uint32_t i = 0; i < things.size(); i++)
    if(things[i]->getID() == id)
      return things[i];
  return NULL;
}

/* Returns the base thing, based on the ID */
MapThing* Map::getThingBase(uint16_t id)
{
  for(uint32_t i = 0; i < base_things.size(); i++)
    if(base_things[i]->getID() == id)
      return base_things[i];
  return NULL;
}

// TODO: Comment
std::vector<MapThing*> Map::getThingData(std::vector<int> thing_ids)
{
  std::vector<MapThing*> used_things;

  /* Loop through all the ids to find the associated things */
  for(auto i = thing_ids.begin(); i != thing_ids.end(); i++)
  {
    /* Only continue if the ID is valid and >= 0 */
    if(*i >= 0)
    {
      MapThing* found_thing = NULL;

      /* Check if is thing */
      found_thing = getThing(*i);

      /* Otherwise, check if IO */
      if(found_thing == NULL)
        found_thing = getIO(*i);

      /* Otherwise, check person */
      if(found_thing == NULL)
        found_thing = getPerson(*i);

      /* Otherwise, check if item */
      if(found_thing == NULL)
        found_thing = getItem(*i);

      /* If found, append to stack */
      if(found_thing != NULL)
        used_things.push_back(found_thing);

    }
  }

  return used_things;
}

/* Returns a matrix of tiles that match the frames in the thing */
// TODO: Comment
std::vector<std::vector<Tile*>> Map::getTileMatrix(MapThing* thing,
                                                   Direction direction)
{
  std::vector<std::vector<Tile*>> tile_set;

  if(thing != NULL)
  {
    SDL_Rect render_box = thing->getBoundingBox();

    /* Check the direction to modify the range if needed */
    if(direction == Direction::NORTH)
      render_box.y--;
    else if(direction == Direction::EAST)
      render_box.x++;
    else if(direction == Direction::SOUTH)
      render_box.y++;
    else if(direction == Direction::WEST)
      render_box.x--;
    return getTileMatrix(thing->getMapSection(), render_box.x, render_box.y,
                         render_box.w, render_box.h);
  }

  return tile_set;
}

// TODO: Comment
std::vector<std::vector<Tile*>> Map::getTileMatrix(uint16_t section,
                                                   uint16_t x, uint16_t y,
                                                   uint16_t width,
                                                   uint16_t height)
{
  std::vector<std::vector<Tile*>> tile_set;
  uint16_t range_x = x + width - 1;
  uint16_t range_y = y + height - 1;

  /* Confirm range is within valid parameters of map */
  if(sub_map.size() > section && sub_map[section].tiles.size() > range_x &&
     sub_map[section].tiles[range_x].size() > range_y)
  {
    /* Load the tiles that correspond to the thing */
    for(uint16_t i = x; i <= range_x; i++)
    {
      std::vector<Tile*> tile_col;

      for(uint16_t j = y; j <= range_y; j++)
        tile_col.push_back(sub_map[section].tiles[i][j]);
      tile_set.push_back(tile_col);
    }
  }

  return tile_set;
}

bool Map::initiateMapSection(uint16_t section_index, int width, int height)
{
  /* Make sure width and height are valid */
  if(width > 0 && height > 0)
  {
    /* Create clear section */
    SubMap map_section;
    map_section.weather = -1;

    /* Make sure the vector is of correct size first */
    while(sub_map.size() <= section_index)
      sub_map.push_back(map_section);

    /* Make sure it is of the correct width */
    std::vector<Tile*> line_width;
    while(sub_map[section_index].tiles.size() < static_cast<uint32_t>(width))
      sub_map[section_index].tiles.push_back(line_width);

    /* Make sure it is of the correct height for each column */
    for(uint32_t i = 0; i < sub_map[section_index].tiles.size(); i++)
    {
      while(sub_map[section_index].tiles[i].size() <
                                                 static_cast<uint32_t>(height))
      {
        int height = sub_map[section_index].tiles[i].size();
        sub_map[section_index].tiles[i].push_back(
                                            new Tile(event_handler, tile_width,
                                                     tile_height, i, height));
      }
    }

    return true;
  }

  return false;
}

/* Initiates a thing action, based on the action key being hit */
// TODO: Fix for generic things
void Map::initiateThingInteraction(MapPerson* initiator)
{
  // FIND EACH AT 0. IF 0, check direction facing and tile adjacent for
  // if it's 0. If not, proceed. If found, interact

  if(initiator != NULL)
  {
    std::vector<std::vector<Tile*>> thing_tiles = initiator->getTileRender(0);
    Direction direction = initiator->getDirection();
    bool finished = false;
    bool invalid = false;
    int16_t starting_x = initiator->getTileX();
    int16_t starting_y = initiator->getTileY();

    /* Things to look for */
    std::vector<MapItem*> items_found;
    MapInteractiveObject* io_found = NULL;
    MapPerson* person_found = NULL;
    MapThing* thing_found = NULL;

    /* Check direction for tile that needs to be determined */
    if(direction == Direction::NORTH)
      starting_y--;
    else if(direction == Direction::EAST)
      starting_x++;
    else if(direction == Direction::SOUTH)
      starting_y++;
    else if(direction == Direction::WEST)
      starting_x--;
    else
      invalid = true;

    if(!invalid)
    {
      /* Loop through the tile set, looking for valid tiles on the render lvl */
      for(uint16_t i = 0; !finished && i < thing_tiles.size(); i++)
      {
        for(uint16_t j = 0; !finished && j < thing_tiles[i].size(); j++)
        {
          /* If thing is not NULL, this is rendering depth 0 tile to check */
          if(thing_tiles[i][j] != NULL)
          {
            /* Get the x and y of tile to check and confirm validity */
            uint16_t x = starting_x + i;
            uint16_t y = starting_y + j;

            if((starting_x + i) >= 0 && (starting_y + j) >= 0 &&
               x < sub_map[map_index].tiles.size() &&
               y < sub_map[map_index].tiles[x].size())
            {
              /* Check for person */
              person_found = sub_map[map_index].tiles[x][y]->getPersonMain(0);
              if(person_found != NULL)
              {
                if(person_found == initiator)
                  person_found = NULL;
                else
                  finished = true;
              }

              /* Check for thing */
              if(!finished && thing_found == NULL)
                thing_found = sub_map[map_index].tiles[x][y]->getThing(0);

              /* Check for IO */
              if(!finished && io_found == NULL)
                io_found = sub_map[map_index].tiles[x][y]->getIO(0);
            }

            /* Check for item(s) */
            if(!finished && items_found.size() == 0)
              items_found = thing_tiles[i][j]->getItems();
          }
        }
      }

      /* Interact with the object found (in the order listed below) */
      if(person_found != NULL)
      {
        person_found->interact(initiator);
      }
      else if(io_found != NULL)
      {
        io_found->interact(initiator);
      }
      else if(thing_found != NULL)
      {
        thing_found->interact(initiator);
      }
      else if(items_found.size() > 0)
      {
        bool found = false;

        for(uint16_t i = 0; !found && (i < items_found.size()); i++)
        {
          if(!items_found[i]->isWalkover() && items_found[i]->getCount() > 0)
          {
            event_handler->executePickup(items_found[i]);
            found = true;
          }
        }
      }
    }
  }
}

/* Mode view update */
bool Map::modeViewStart(int cycle_time, bool travel)
{
  bool proceed = travel;

  /* If travel, complete fade */
  if(!proceed)
  {
    fade_status = FADINGOUT;
    proceed = updateFade(cycle_time);
  }

  /* If finished, update viewport and proceed */
  if(proceed)
  {
    if(view_thing != nullptr)
      viewport.lockOn(view_thing, travel);
    else
      viewport.lockOn(view_tile, travel);
    view_start = false;
  }

  return proceed;
}

/* Mode view update */
bool Map::modeViewStop(int cycle_time, bool travel)
{
  bool proceed = travel;

  /* If travel, complete fade */
  if(!proceed)
  {
    fade_status = FADINGOUT;
    proceed = updateFade(cycle_time);
  }

  /* If finished, update viewport and proceed */
  if(proceed)
  {
    viewport.lockOn(player, travel);
    mode_curr = NORMAL;
    mode_next = NONE;
  }

  return proceed;
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
    if(*r_start > sub_map[index].tiles.size())
      *r_start = sub_map[index].tiles.size();
    if(*r_end > sub_map[index].tiles.size())
      *r_end = sub_map[index].tiles.size();

    /* Determine the column of parsing - limit to map size */
    *c_start += std::stoul(col_list.front());
    *c_end += std::stoul(col_list.back()) + 1;
    if(*c_start > sub_map[index].tiles[*r_start].size())
      *c_start = sub_map[index].tiles[*r_start].size();
    if(*c_end > sub_map[index].tiles[*r_start].size())
      *c_end = sub_map[index].tiles[*r_start].size();

    return true;
  }
  return false;
}

/* Changes the map section index - what is displayed */
bool Map::setSectionIndex(uint16_t index)
{
  if(index < sub_map.size() && sub_map[index].tiles.size() > 0)
  {
    map_index_next = index;
    changeMode(SWITCHSUB);
    return true;
  }
  return false;
}

/* Changes the map section index - what is displayed - called from mode */
bool Map::setSectionIndexMode(int index_next)
{
  if(index_next < 0)
    index_next = map_index_next;
  if(index_next >= 0 && index_next < 256)
  {
    uint8_t index = static_cast<uint8_t>(index_next);

    if(map_index != index && index < sub_map.size() &&
       sub_map[index].tiles.size() > 0)
    {
      /* Update the index and viewport */
      map_index = index;
      viewport.setMapSize(sub_map[index].tiles.size(),
                          sub_map[index].tiles[0].size(),
                          map_index);

      /* Update sound and dialog now that index is fresh */
      if(index_next < 0)
      {
        audioUpdate(true);
        map_dialog.clearAll();
      }

      map_index_next = -1;
      return true;
    }
  }

  map_index_next = -1;
  return false;
}

/* Splits the ID into a vector of IDs */
std::vector< std::vector<int32_t> > Map::splitIdString(std::string id,
                                                       bool matrix)
{
  std::vector< std::vector<int32_t> > id_stack;
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
        std::vector<int32_t> id_row;

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
        std::vector<int32_t> id_row;

        std::vector<std::string> row_split = Helpers::split(xy_split[i], '.');
        for(uint16_t j = 0; j < row_split.size(); j++)
          id_row.push_back(std::stoul(row_split[j]));

        id_stack.push_back(id_row);
      }
    }
  }

  return id_stack;
}

/* Triggers a view of the passed in data */
bool Map::triggerViewThing(MapThing* view_thing, UnlockView view_mode,
                           int view_time)
{
  /* Parse and check view data */
  bool view, scroll;
  EventSet::dataEnumView(view_mode, view, scroll);
  if(view && view_time > 0)
  {
    /* Set data */
    this->view_acc = 0;
    this->view_section = view_thing->getMapSection();
    this->view_thing = view_thing;
    this->view_tile = nullptr;
    this->view_time = view_time;
    this->view_travel = scroll;

    /* Trigger mode change */
    changeMode(VIEW);
    view_start = true;
    return true;
  }
  return false;
}

/* Triggers a view of the passed in data */
bool Map::triggerViewTile(Tile* view_tile, uint16_t view_section,
                          UnlockView view_mode, int view_time)
{
  /* Parse and check view data */
  bool view, scroll;
  EventSet::dataEnumView(view_mode, view, scroll);
  if(view && view_time > 0)
  {
    /* Set data */
    this->view_acc = 0;
    this->view_section = view_section;
    this->view_thing = nullptr;
    this->view_tile = view_tile;
    this->view_time = view_time;
    this->view_travel = scroll;

    /* Trigger mode change */
    changeMode(VIEW);
    view_start = true;
    return true;
  }
  return false;
}

/* Updates the map fade */
bool Map::updateFade(int cycle_time)
{
  bool end_status = false;

  /* -- FADE BLACK -- */
  if(fade_status == BLACK)
  {
    fade_alpha = kFADE_BLACK;
  }
  /* -- FADE VISIBLE -- */
  else if(fade_status == VISIBLE)
  {
    fade_alpha = kFADE_VIS;
  }
  /* -- FADING IN -- */
  else if(fade_status == FADINGIN)
  {
    int diff = cycle_time / kFADE_FACTOR;
    if(diff > kFADE_MAX)
      diff = kFADE_MAX; /* Over-run protection */
    if(diff < fade_alpha)
    {
      fade_alpha -= diff;
    }
    else
    {
      fade_alpha = kFADE_VIS;
      fade_status = VISIBLE;
      end_status = true;
    }
  }
  /* -- FADING OUT -- */
  else if(fade_status == FADINGOUT)
  {
    int diff = cycle_time / kFADE_FACTOR;
    if(diff > kFADE_MAX)
      diff = kFADE_MAX; /* Over-run protection */
    if((diff + fade_alpha) < kFADE_BLACK)
    {
      fade_alpha += diff;
    }
    else
    {
      fade_alpha = kFADE_BLACK;
      fade_status = BLACK;
      end_status = true;
    }
  }
  /* -- FADING INVALID -- */
  else
  {
    fade_status = BLACK;
  }

  return end_status;
}

/* Updates the map mode */
void Map::updateMode(int cycle_time)
{
  /* First run parsing */
  if(mode_curr == mode_next)
  {
    if(mode_next == DISABLED)
      mode_curr = NORMAL;
    else
      mode_next = NONE;
  }

  /* -- CURRENT MODE DISABLED -- */
  if(mode_curr == DISABLED)
  {
    /* -- NEXT MODE NORMAL -- */
    if(mode_next != NONE)
    {
      /* Fade in */
      fade_status = FADINGIN;
      if(updateFade(cycle_time))
      {
        mode_curr = NORMAL;
        if(mode_next == NORMAL)
          mode_next = NONE;
      }
    }
  }
  /* -- CURRENT MODE NORMAL -- */
  else if(mode_curr == NORMAL)
  {
    /* -- NEXT MODE DISABLED or SWITCHSUB -- */
    if(mode_next == DISABLED || mode_next == SWITCHSUB)
    {
      fade_status = FADINGOUT;
      if(updateFade(cycle_time))
      {
        mode_curr = mode_next;
        mode_next = NONE;
      }
    }
    /* -- NEXT MODE VIEW -- */
    else if(mode_next == VIEW)
    {
      mode_curr = mode_next;
      mode_next = NONE;
    }
    /* NEXT MODE NONE -- */
    else if(mode_next == NONE)
    {
      if(fade_status != VISIBLE)
        fade_status = FADINGIN;
      updateFade(cycle_time);
    }
  }
  /* -- CURRENT MODE SWITCH SUB MAP -- */
  else if(mode_curr == SWITCHSUB)
  {
    setSectionIndexMode();
    fade_status = FADINGIN;
    if(updateFade(cycle_time))
    {
      mode_curr = NORMAL;
      mode_next = NONE;
    }
  }
  /* -- CURRENT MODE VIEW -- */
  else if(mode_curr == VIEW)
  {
    /* First half - going to view point */
    if(view_start)
    {
      /* Same sub-section */
      if(view_section == map_index)
      {
        modeViewStart(cycle_time, view_travel);
      }
      /* Different sub-section */
      else
      {
        if(modeViewStart(cycle_time, false))
          setSectionIndexMode(view_section);
      }
    }
    /* Second half - viewing and returning */
    else
    {
      /* No time accumulated */
      if(view_acc == 0)
      {
        fade_status = FADINGIN;
        if(updateFade(cycle_time))
          view_acc++;
      }
      /* Time accumulated or finished */
      else
      {
        if(!viewport.isTravelling())
        {
          /* Still timing */
          if(view_acc < view_time)
          {
            view_acc += cycle_time;
          }
          /* Finished, begin return process */
          else
          {
            /* Same sub-section */
            if(player->getMapSection() == map_index)
            {
              modeViewStop(cycle_time, view_travel);
            }
            /* Different sub-section */
            else
            {
              if(modeViewStop(cycle_time, false))
                setSectionIndexMode(player->getMapSection());
            }
          }
        }
      }
    }
  }
  /* -- CURRENT MODE NONE or INVALID -- */
  else
  {
    mode_curr = DISABLED;
  }
}

/* Updates the height and width, based on zoom factors */
void Map::updateTileSize()
{
  bool updated = false;

  /* Try and zoom out the map */
  if(zoom_out)
  {
    /* Modify the tile height and width, limited by the constants */
    tile_height--;
    tile_width--;
    if(tile_height < kZOOM_TILE_SIZE || tile_width < kZOOM_TILE_SIZE)
    {
      tile_height = kZOOM_TILE_SIZE;
      tile_width = kZOOM_TILE_SIZE;
      zoom_out = false;
    }

    updated = true;
  }
  /* Otherwise, try and zoom back in */
  else if(zoom_in)
  {
    /* Modify the tile height and width, limited by the constants */
    tile_height++;
    tile_width++;
    if(tile_height > Helpers::getTileSize() ||
       tile_width > Helpers::getTileSize())
    {
      tile_height = Helpers::getTileSize();
      tile_width = Helpers::getTileSize();
      zoom_in = false;
    }

    updated = true;
  }

  /* If updated, update the height and width everywhere */
  if(updated)
  {
    /* Update map tiles */
    for(uint16_t i = 0; i < sub_map.size(); i++)
      for(uint16_t j = 0; j < sub_map[i].tiles.size(); j++)
        for(uint16_t k = 0; k < sub_map[i].tiles[j].size(); k++)
        {
          sub_map[i].tiles[j][k]->setHeight(tile_height);
          sub_map[i].tiles[j][k]->setWidth(tile_width);
        }

    /* Update viewport */
    viewport.setTileSize(tile_width, tile_height);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Enable view trigger */
void Map::enableView(bool enable)
{
  /* Stop call for other music - in case no music is available in the map */
  if(event_handler)
    event_handler->triggerAudioStop(SoundChannels::MUSIC1);

  if(enable)
  {
    changeMode(NORMAL);
    audioStart();
  }
  else
  {
    changeMode(DISABLED);
    audioStop();
  }
}

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

bool Map::initConversation(Conversation* convo, MapThing* source)
{
  if(player != NULL && player->getTarget() == NULL
                    && map_dialog.initConversation(convo, player, source))
  {
    /* Finalize conversation setup */
    std::vector<int> list = map_dialog.getConversationIDs();
    map_dialog.setConversationThings(getThingData(list));

    /* Flush player keys */
    player->keyFlush();

    /* Set the targets */
    player->setTarget(source);
    if(source != NULL)
      source->setTarget(player);

    /* Clear running status */
    if(system_options != NULL && !system_options->isAutoRun())
      player->setRunning(false);

    return true;
  }

  return false;
}

bool Map::initNotification(std::string notification)
{
  return map_dialog.initNotification(notification);
}

bool Map::initNotification(Frame* image, int count)
{
  return map_dialog.initPickup(image, count);
}

/* Initializes item store display, within the map */
bool Map::initStore(ItemStore::StoreMode mode, std::vector<Item*> items,
                    std::vector<uint32_t> counts,
                    std::vector<int32_t> cost_modifiers,
                    std::string name, bool show_empty)
{
  (void)show_empty;

  //TODO: Fix
  bool status = item_menu.initDisplay(mode, items, counts,
                                      cost_modifiers, name);//, show_empty);

  /* If successful, flush player keys */
  if(status)
    player->keyFlush();

  return true;
}

// /* Checks whether the viewport contains any tiles with the given sector */
// bool Map::isInSector(int index)
// {
  // (void)index;//warning
  // return true;
// }

/* Returns if the map has been currently loaded with data */
bool Map::isLoaded()
{
  return loaded;
}

/* Mode checks - only returns true if DISABLED current mode and fade status is
 * BLACK */
bool Map::isModeDisabled()
{
  return (mode_curr == DISABLED && fade_status == BLACK);
}

/* Mode checks - only returns true if NORMAL current mode and fade status is
 * VISIBLE */
bool Map::isModeNormal()
{
  return (mode_curr == NORMAL && fade_status == VISIBLE && !
          viewport.isTravelling());
}

/* The key up and down events to be handled by the class */
bool Map::keyDownEvent(SDL_KeyboardEvent event)
{
  /* Control is only permitted if the mode is normal */
  if(isModeNormal())
  {
    /* Exit the map, map has finished processing */
    if(event.keysym.sym == SDLK_ESCAPE)
    {
      if(item_menu.isActive())
        item_menu.close();
      else
      {
        unfocus();
        return true;
      }
    }
    /* ---- START TEST CODE ---- */
    /* Test: trigger grey scale */
    else if(event.keysym.sym == SDLK_g)
    {
      bool enable = !tile_sprites[0]->isGreyScale();
      for(auto i = tile_sprites.begin(); i != tile_sprites.end(); i++)
        (*i)->useGreyScale(enable);
    }
    /* Test: Pause dialog */
    else if(event.keysym.sym == SDLK_p)
      map_dialog.setPaused(!map_dialog.isPaused());
    /* Test: Reset player location */
    else if(event.keysym.sym == SDLK_r)
    {
      player->resetPosition();
  	  setSectionIndex(player->getStartingSection());
    }
    /* Test: Dialog Reset */
    else if(event.keysym.sym == SDLK_5)
      map_dialog.clearAll(true);
    /* Test: Pick up test. Time limit */
    else if(event.keysym.sym == SDLK_6)
      map_dialog.initPickup(items[1]->getDialogImage(), 15, 2500);
    /* Test: Pick up test. No time limit */
    else if(event.keysym.sym == SDLK_7)
      map_dialog.initPickup(items.front()->getDialogImage(), 5);
    /* Test: single line chop off notification */
    else if(event.keysym.sym == SDLK_8)
      map_dialog.initNotification("Hello sunshine, what a glorious day and I'll keep writing forever and forever and forever and forever and forever and forever and forFU.", true, 0);
    /* Test: multi line notification */
    else if(event.keysym.sym == SDLK_9)
      map_dialog.initNotification("Hello sunshine, what a glorious day and I'll keep writing forever and forever and forever and forever and forever and forever and forFU.");
    /* Test: full conversation */
    else if(event.keysym.sym == SDLK_0)
    {
      Event blank_event = EventSet::createBlankEvent();

      Conversation* convo = new Conversation;
      convo->category = DialogCategory::TEXT;
      convo->action_event = blank_event;
      convo->text = "This is the initial conversation point that will start ";
      convo->text += "it. How can this continue? It must pursue to complete ";
      convo->text += "embodiment. Ok, maybe I'll just keep typing until I ";
      convo->text += "break the entire compiler.";
      convo->thing_id = 0;
      Conversation convo2;
      convo2.category = DialogCategory::TEXT;
      convo2.action_event = blank_event;
      convo2.text = "Pass the chips please.";
      convo2.thing_id = 24;
      Conversation test1, test2, test3, test4, test5;
      test1.category = DialogCategory::TEXT;
      test1.action_event = blank_event;
      test1.text = "This is a test to see how data runs. The line will split ";
      test1.text += "once unless it is an option based selection in which ";
      test1.text += "case it will restrict.";
      test1.thing_id = 3;
      test2.category = DialogCategory::TEXT;
      test2.action_event = blank_event;
      test2.text = "This is a no man case. See what happens!! Ok, this is the ";
      test2.text += "too long case where the lines never cease to exist and ";
      test2.text += "the points aren't for real. I'm feeling a bit hungry ";
      test2.text += "though so I don't know if I'll have the stamina to clean ";
      test2.text += "up this case in all it's glory. Repeat: ";
      test2.text += test2.text;
      test2.text += test2.text;
      test2.thing_id = 2;
      test2.next.push_back(test1);
      test3.category = DialogCategory::TEXT;
      test3.action_event = EventSet::createEventStartBattle();
      test3.text = "Back to finish off with a clean case with a couple of ";
      test3.text += "lines. So this requires me to write a bunch of BS to try ";
      test3.text += "and fill these lines.";
      test3.text += test3.text;
      test3.text += test3.text;
      test3.thing_id = 1003;
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
      convo2.next.push_back(test1);
      convo->next.push_back(convo2);

      /* Run the conversation and then delete */
      if(map_dialog.initConversation(convo, player, NULL))
      {
        std::vector<int> list = map_dialog.getConversationIDs();
        map_dialog.setConversationThings(getThingData(list));
      }

      delete convo;
    }
    /* Test: Zoom back out */
    else if(event.keysym.sym == SDLK_z)
      zoom_out = true;
    /* Test: Zoom back in */
    else if(event.keysym.sym == SDLK_x)
      zoom_in = true;
    /* Test: Get persons at index 1 to target player */
    else if(event.keysym.sym == SDLK_t)
    {
      if(persons[1]->getTarget() == NULL)
        persons[1]->setTarget(player);
      else
        persons[1]->clearTarget();
    }
    /* Test: Location of player */
    else if(event.keysym.sym == SDLK_l)
    {
      if(player != NULL)
  	  {
  	    SDL_Rect bbox = player->getBoundingBox();
  	    SDL_Rect bpixel = player->getBoundingPixels();

  	    std::cout << "----" << std::endl;
  	    std::cout << "Location X: " << bbox.x << " - " << bpixel.x << std::endl;
  	    std::cout << "Location Y: " << bbox.y << " - " << bpixel.y << std::endl;
  	    std::cout << "Width: " << bbox.w << " - " << bpixel.w << std::endl;
  	    std::cout << "Height: " << bbox.h << " - " << bpixel.h << std::endl;
  	    std::cout << "----" << std::endl;
  	  }
    }
    /* ---- END TEST CODE ---- */
    /* If item menu is active, send keys there */
    else if(item_menu.isActive())
      item_menu.keyDownEvent(event);
    /* If conversation is active, send keys there */
    else if(map_dialog.isConversationActive())
      map_dialog.keyDownEvent(event);
    /* Interact initiation from player */
    else if(event.keysym.sym == SDLK_SPACE)
      initiateThingInteraction(player);
    /* Otherwise, send keys to player for control */
    else if(player != NULL)
    {
      if(event.keysym.sym == SDLK_LSHIFT || event.keysym.sym == SDLK_RSHIFT)
        player->setRunning(true);
      else if(event.keysym.sym == SDLK_1)
      {
        viewport.lockOn(player);
      }
      else if(event.keysym.sym == SDLK_2)
      {
        if(getPerson(10000) != nullptr)
          viewport.lockOn(getPerson(10000));
      }
      else
        player->keyDownEvent(event);
    }
  }

  return false;
}

void Map::keyUpEvent(SDL_KeyboardEvent event)
{
  if(isModeNormal())
  {
    if(item_menu.isActive())
      item_menu.keyUpEvent(event);
    else if(map_dialog.isConversationActive())
      map_dialog.keyUpEvent(event);
    else if(player != NULL)
    {
      if((event.keysym.sym == SDLK_LSHIFT || event.keysym.sym == SDLK_RSHIFT) &&
         system_options != NULL && !system_options->isAutoRun())
        player->setRunning(false);
      else
        player->keyUpEvent(event);
    }
  }
}

/* Loads the map data - called from game */
bool Map::loadData(XmlData data, int index, SDL_Renderer* renderer,
                   std::string base_path)
{
  (void)base_path;
  bool success = true;

  /* ---- BASE SPRITES ---- */
  if(data.getElement(index) == "sprite" && !data.getKeyValue(index).empty())
  {
    success &= addSpriteData(data, data.getKeyValue(index),
                             index + 1, renderer);
  }
  /* ---- BASE THINGS ---- */
  else if((data.getElement(index) == "mapthing" ||
           data.getElement(index) == "mapperson" ||
           data.getElement(index) == "mapnpc" ||
           data.getElement(index) == "mapitem" ||
           data.getElement(index) == "mapio") &&
          !data.getKeyValue(index).empty())
  {
    success &= addThingBaseData(data, index, renderer);
  }
  /* ---- SUB MAPS ---- */
  else if(data.getElement(index) == "main" ||
          data.getElement(index) == "section")
  {
    int map_index = -1;
    int height = -1;
    int width = -1;

    /* Determine section */
    if(data.getElement(index) == "main")
      map_index = 0;
    else if(data.getElement(index) == "section")
      map_index = std::stoi(data.getKeyValue(index));

    if(map_index >= 0)
    {
      /* Determine current height and width */
      if(sub_map.size() > static_cast<uint16_t>(map_index) &&
         sub_map[map_index].tiles.size() > 0)
      {
        height = sub_map[map_index].tiles.front().size();
        width = sub_map[map_index].tiles.size();
      }
      else
      {
        height = 1;
        width = 1;
        initiateMapSection(map_index, width, height);
      }

      /* -- SECTION WIDTH -- */
      if(data.getElement(index + 1) == "width")
      {
        width = data.getDataInteger(&success);
        if(success)
          initiateMapSection(map_index, width, height);
      }
      /* -- SECTION HEIGHT -- */
      else if(data.getElement(index + 1) == "height")
      {
        height = data.getDataInteger(&success);
        if(success)
          initiateMapSection(map_index, width, height);
      }
      /* -- MUSIC -- */
      else if(data.getElement(index + 1) == "music")
      {
        int32_t music_id = data.getDataInteger(&success);
        if(music_id >= 0)
          sub_map[map_index].music.push_back(music_id);
      }
      /* -- WEATHER -- */
      else if(data.getElement(index + 1) == "weather")
      {
        int32_t weather_id = data.getDataInteger(&success);
        if(weather_id >= 0)
          sub_map[map_index].weather = weather_id;
      }
      /* -- TILE SPRITES/EVENTS -- */
      else if(data.getElement(index + 1) == "base" ||
              data.getElement(index + 1) == "enhancer" ||
              data.getElement(index + 1) == "lower" ||
              data.getElement(index + 1) == "upper" ||
              data.getElement(index + 1) == "tileevent")
      {
        success &= addTileData(data, map_index);
      }
      /* -- TILE THINGS -- */
      else if(data.getElement(index + 1) == "mapthing" ||
              data.getElement(index + 1) == "mapperson" ||
              data.getElement(index + 1) == "mapnpc" ||
              data.getElement(index + 1) == "mapitem" ||
              data.getElement(index + 1) == "mapio")
      {
        success &= addThingData(data, map_index, renderer);
      }
    }
  }

  return success;
}

/* Finishes the load - last call on successful data */
void Map::loadDataFinish(SDL_Renderer* renderer)
{
  /* Load the item menu sprites - TODO: In file? */
  item_menu.loadImageBackend("sprites/Overlay/item_store_left.png",
                             "sprites/Overlay/item_store_right.png",
                             renderer);

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
  map_dialog.loadImagePickupTopBottom("sprites/Overlay/notification_corner.png",
                                      renderer);

  /* Clean up base things */
  for(uint16_t i = 0; i < base_things.size(); i++)
    if(!base_things[i]->cleanMatrix())
      base_things[i]->unsetFrames(true);
  for(uint16_t i = 0; i < base_items.size(); i++)
    if(!base_items[i]->cleanMatrix())
      base_items[i]->unsetFrames(true);
  for(uint16_t i = 0; i < base_persons.size(); i++)
    if(!base_persons[i]->cleanMatrix())
      base_persons[i]->unsetFrames(true);
  for(uint16_t i = 0; i < base_ios.size(); i++)
    if(!base_ios[i]->cleanMatrix())
      base_ios[i]->unsetFrames(true);

  /* Thing clean-up and tile set-up */
  for(uint16_t i = 0; i < things.size(); i++)
  {
    /* Clean the matrix - fixes up the rendering box */
    if((things[i]->getBase() != NULL &&
        things[i]->getBase()->getFrames().size() > 0) ||
       things[i]->cleanMatrix())
    {
      /* Get the tile matrix to match the frames and set */
      std::vector<std::vector<Tile*>> tile_set = getTileMatrix(things[i]);
      if(tile_set.size() > 0)
        things[i]->setStartingTiles(tile_set,
                                    things[i]->getStartingSection(), true);
      else
        things[i]->unsetFrames(true);
    }
    else
    {
      things[i]->unsetFrames(true);
    }
  }

  /* IO clean-up and tile set-up */
  for(uint16_t i = 0; i < ios.size(); i++)
  {
    /* Clean the matrix - fixes up the rendering box */
    if((ios[i]->getBase() != NULL &&
        ios[i]->getBase()->getFrames().size() > 0) ||
       ios[i]->cleanMatrix())
    {
      /* Get the tile matrix to match the frames and set */
      std::vector<std::vector<Tile*>> tile_set = getTileMatrix(ios[i]);
      if(tile_set.size() > 0)
        ios[i]->setStartingTiles(tile_set,
                                 ios[i]->getStartingSection(), true);
      else
        ios[i]->unsetFrames(true);
    }
    else
    {
      ios[i]->unsetFrames(true);
    }
  }

  /* Person clean-up and tile set-up */
  for(uint16_t i = 0; i < persons.size(); i++)
  {
    if((persons[i]->getBase() != NULL &&
        persons[i]->getBase()->cleanMatrix(false))  ||
       persons[i]->cleanMatrix())
    {
      std::vector<std::vector<Tile*>> tile_set = getTileMatrix(persons[i]);
      if(tile_set.size() > 0)
        persons[i]->setStartingTiles(tile_set,
                                     persons[i]->getStartingSection(), true);
      else
        persons[i]->unsetStates(true);
    }
    else
    {
      persons[i]->unsetStates(true);
    }
  }

  /* Items clean-up and tile set-up */
  for(uint16_t i = 0; i < items.size(); i++)
  {
    /* Clean the matrix - fixes up the rendering box */
    if((items[i]->getBase() != NULL &&
        items[i]->getBase()->cleanMatrix(false)) ||  items[i]->cleanMatrix())
    {
      /* Get the tile matrix to match the frames and set */
      std::vector<std::vector<Tile*>> tile_set = getTileMatrix(items[i]);
      if(tile_set.size() > 0)
        items[i]->setStartingTiles(tile_set, items[i]->getStartingSection(),
                                   true);
      else
        items[i]->unsetFrames(true);
    }
  }

  /* Modify the map index */
  if(player != NULL)
    map_index = player->getStartingSection();
  if(sub_map.size() > map_index && sub_map[map_index].tiles.size() > 0)
  {
    viewport.clearLocation();
    viewport.setMapSize(sub_map[map_index].tiles.size(),
                        sub_map[map_index].tiles.front().size(),
                        map_index);
    if(player != NULL)
      viewport.lockOn(player);
  }
}

/* Proceeds to pickup the total number of this marked item
 * Default is invalid parameter which picks up all */
bool Map::pickupItem(MapItem* item, int count)
{
  if(item != NULL && item->getCount() > 0 && count != 0)
  {
    /* Set the map count */
    if(count < 0 || count >= (int)item->getCount())
      item->setCount(0);
    else
      item->setCount(item->getCount() - count);

    /* Trigger sound */
    if(event_handler != nullptr)
    {
      uint32_t item_id = Sound::kID_SOUND_PICK_ITEM;

      if(item->getSoundID() >= 0)
      {
        item_id = item->getSoundID();
      }
      else if(item->getGameID() >= 0 &&
              (uint32_t)item->getGameID() == Item::kID_MONEY)
      {
        item_id = Sound::kID_SOUND_PICK_COIN;
      }

      event_handler->triggerSound(item_id, SoundChannels::TRIGGERS);
    }

    return true;
  }

  return false;
}

/* Renders the title screen */
bool Map::render(SDL_Renderer* renderer)
{
  bool success = true;
  if(sub_map.size() > map_index)
  {
    /* Grab the variables for viewport */
    uint16_t tile_x_start = viewport.getXTileStart();
    uint16_t tile_x_end = viewport.getXTileEnd();
    uint16_t tile_y_start = viewport.getYTileStart();
    uint16_t tile_y_end = viewport.getYTileEnd();
    float x_offset = viewport.getX();
    float y_offset = viewport.getY();

    /* Render the lower tiles within the range of the viewport */
    for(uint16_t i = tile_x_start; i < tile_x_end; i++)
    {
      for(uint16_t j = tile_y_start; j < tile_y_end; j++)
      {
        Tile* ref_tile = sub_map[map_index].tiles[i][j];

        /* Lower sprites */
        ref_tile->renderLower(renderer, x_offset, y_offset);

        /* Map Items, if relevant */
        if(ref_tile->isItemsSet())
        {
          std::vector<MapItem*> item_set = ref_tile->getItems();
          bool found = false;

          for(uint16_t i = 0; !found && (i < item_set.size()); i++)
          {
            if(item_set[i]->getCount() > 0)
            {
              item_set[i]->render(renderer, x_offset, y_offset);
              found = true;
            }
          }
        }

        /* Base map thing, if relevant */
        MapThing* render_thing = sub_map[map_index].tiles[i][j]->getThing(0);
        if(render_thing != NULL)
          render_thing->renderMain(renderer, sub_map[map_index].tiles[i][j], 0,
                                   x_offset, y_offset);

        /* Base map IO, if relevant */
        MapInteractiveObject* render_io =
                                      sub_map[map_index].tiles[i][j]->getIO(0);
        if(render_io != NULL)
          render_io->renderMain(renderer, sub_map[map_index].tiles[i][j], 0,
                                x_offset, y_offset);
      }
    }

    /* Render the map things within the range of the viewport */
    for(uint8_t index = 0; index < Helpers::getRenderDepth(); index++)
    {
      for(uint16_t i = tile_x_start; i < tile_x_end; i++)
      {
        for(uint16_t j = tile_y_start; j < tile_y_end; j++)
        {
          MapInteractiveObject* render_io = NULL;
          MapPerson* render_person = NULL;
          MapThing* render_thing = NULL;

          /* Acquire render things and continue forward if some are not null */
          if(sub_map[map_index].tiles[i][j]->getRenderThings(index,
                                                             render_person,
                                                             render_thing,
                                                             render_io))
          {
            /* Different indexes result in different rendering procedures
             * If base index, render order is top item, thing, then person */
            if(index == 0)
            {
              if(render_person != NULL)
              {
                if(render_person->getMovement() == Direction::EAST ||
                   render_person->getMovement() == Direction::SOUTH)
                {
                  render_person->renderPrevious(renderer,
                                                sub_map[map_index].tiles[i][j],
                                                index, x_offset, y_offset);
                }
                else
                {
                  render_person->renderMain(renderer,
                                            sub_map[map_index].tiles[i][j],
                                            index, x_offset, y_offset);
                }
              }
            }
            /* Otherwise, render order is person, then thing */
            else
            {
              if(render_person != NULL)
              {
                if(render_person->getMovement() == Direction::EAST ||
                   render_person->getMovement() == Direction::SOUTH)
                {
                  render_person->renderPrevious(renderer,
                                                sub_map[map_index].tiles[i][j],
                                                index, x_offset, y_offset);
                }
                else
                {
                  render_person->renderMain(renderer,
                                            sub_map[map_index].tiles[i][j],
                                            index, x_offset, y_offset);
                }
              }

              if(render_thing != NULL)
                render_thing->renderMain(renderer,
                                         sub_map[map_index].tiles[i][j],
                                         index, x_offset, y_offset);

              if(render_io != NULL)
                render_io->renderMain(renderer, sub_map[map_index].tiles[i][j],
                                      index, x_offset, y_offset);
            }
          }
        }
      }
    }

    /* Render the upper tiles within the range of the viewport */
    for(uint16_t i = tile_x_start; i < tile_x_end; i++)
    {
      for(uint16_t j = tile_y_start; j < tile_y_end; j++)
      {
        sub_map[map_index].tiles[i][j]->renderUpper(renderer,
                                                    x_offset, y_offset);
      }
    }

    /* Render the map dialogs / pop-ups */
    item_menu.render(renderer);
    //map_dialog.render(renderer);

    /* Overlay (for fading */
    if(fade_status != VISIBLE)
    {
      SDL_SetTextureAlphaMod(Helpers::getMaskBlack(), fade_alpha);
      SDL_RenderCopy(renderer, Helpers::getMaskBlack(), NULL, NULL);
    }

    /* Map dialog finally */
    map_dialog.render(renderer);

    /* Map menu last to render - TODO */
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

    /* Update the dialogs with options information */
    item_menu.setConfiguration(running_config);
    map_dialog.setConfiguration(running_config);

    return true;
  }

  return false;
}

/* Sets the operational event handler */
void Map::setEventHandler(EventHandler* event_handler)
{
  /* Primary link */
  this->event_handler = event_handler;

  /* Secondary links */
  map_dialog.setEventHandler(event_handler);
}

// Possibly make the teleport add the ability of shifting map thing
void Map::teleportThing(int id, int tile_x, int tile_y, int section_id)
{
  /* If the section id is below 0, then set to internal map index */
  if(section_id < 0)
    section_id = map_index;

  if(tile_x >= 0 && tile_y >= 0 && section_id >= 0 && id >= 0)
  {
    uint16_t x = tile_x;
    uint16_t y = tile_y;
    uint16_t section = section_id;

    /* Ensure that the tile x and y is within the range */
    if(section < sub_map.size() && sub_map[section].tiles.size() > x
                                && sub_map[section].tiles[x].size() > y)
    {
      /* Find the thing */
      MapThing* found_thing = getPerson(id);
      if(found_thing == nullptr)
        found_thing = getThing(id);
      if(found_thing == nullptr)
        found_thing = getIO(id);

      /* Change the starting tile for the thing */
      if(found_thing != nullptr)
      {
        std::vector<std::vector<Tile*>> matrix = getTileMatrix(section, x, y,
                             found_thing->getWidth(), found_thing->getHeight());

        if(found_thing->setStartingTiles(matrix, section))
        {
          if(id == kPLAYER_ID)
          {
            if(map_index != section)
              setSectionIndex(section);
            else
              viewport.setToTravel(true);
            unfocus();
          }
        }
      }
    }
  }
}

void Map::unfocus()
{
  /* If player is set, clear movement */
  if(player != NULL)
  {
    player->keyFlush();
    player->setRunning(false);
  }
}

void Map::unloadMap()
{
  /* Reset the index and applicable parameters */
  map_index = 0;
  map_dialog.clearAll(true);
  tile_height = Helpers::getTileSize();
  tile_width = tile_height;

  /* Reset music references */
  audioStop();

  /* Deletes the sprite data stored */
  for(uint16_t i = 0; i < tile_sprites.size(); i++)
  {
    delete tile_sprites[i];
    tile_sprites[i] = NULL;
  }
  tile_sprites.clear();

   /* Delete the items */
  for(uint16_t i = 0; i < items.size(); i++)
  {
    delete items[i];
    items[i] = NULL;
  }
  items.clear();

  /* Delete the persons */
  for(uint16_t i = 0; i < persons.size(); i++)
  {
    delete persons[i];
    persons[i] = NULL;
  }
  persons.clear();

  /* Delete the IOs */
  for(uint16_t i = 0; i < ios.size(); i++)
  {
    delete ios[i];
    ios[i] = NULL;
  }
  ios.clear();

  /* Delete the things */
  for(uint16_t i = 0; i < things.size(); i++)
  {
    delete things[i];
    things[i] = NULL;
  }
  things.clear();

  /* Delete the base items */
  for(uint16_t i = 0; i < base_items.size(); i++)
  {
    delete base_items[i];
    base_items[i] = NULL;
  }
  base_items.clear();

  /* Delete the base persons */
  for(uint16_t i = 0; i < base_persons.size(); i++)
  {
    delete base_persons[i];
    base_persons[i] = NULL;
  }
  base_persons.clear();

  /* Delete the base interactive objects */
  for(uint16_t i = 0; i < base_ios.size(); i++)
  {
    delete base_ios[i];
    base_ios[i] = NULL;
  }
  base_ios.clear();

  /* Delete the base things */
  for(uint16_t i = 0; i < base_things.size(); i++)
  {
    delete base_things[i];
    base_things[i] = NULL;
  }
  base_things.clear();

  /* Delete all the tiles that have been set */
  for(uint16_t i = 0; i < sub_map.size(); i++)
  {
    for(uint16_t j = 0; j < sub_map[i].tiles.size(); j++)
    {
      for(uint16_t k = 0; k < sub_map[i].tiles[j].size(); k++)
      {
        delete sub_map[i].tiles[j][k];
        sub_map[i].tiles[j][k] = NULL;
      }
      sub_map[i].tiles[j].clear();
    }
    sub_map[i].tiles.clear();
  }
  sub_map.clear();

  /* Reset the viewport */
  int zero = 0;
  viewport.setMapSize(zero, zero);
  viewport.lockOn(zero, zero, false);

  /* Clear the remaining and disable the loading */
  loaded = false;
}

/* Unlock triggers, based on parameter information */
void Map::unlockIO(int io_id, UnlockIOMode mode, int state_num,
                   UnlockIOEvent mode_events, UnlockView mode_view,
                   int view_time)
{
  /* Find io ptr */
  MapInteractiveObject* found = getIO(io_id);

  /* Parse and attempt unlock, if found */
  if(found != nullptr && found->unlockTrigger(mode, state_num, mode_events))
  {
    /* Was unlocked: check if view is required */
    triggerViewThing(found, mode_view, view_time);

    // TODO: Possibly pull out into function (or in MapViewport)
    //bool view, scroll;
    //EventSet::dataEnumView(mode_view, view, scroll);
    //if(view && view_time > 0)
    //{
      // TODO
      //std::cout << "TODO - View IO: " << io_id << ",Scroll: " << scroll
      //          << ",Time: " << view_time << std::endl;
    //}
  }
}

/* Unlock triggers, based on parameter information */
void Map::unlockThing(int thing_id, UnlockView mode_view, int view_time)
{
  /* Find thing (or person or npc) ptr */
  MapThing* found = getThing(thing_id);
  if(found == nullptr)
    found = getPerson(thing_id);

  /* Unlock, if found */
  if(found != nullptr)
  {
    EventSet* set = found->getEventSet();
    if(set != nullptr && set->unlockTrigger())
    {
      /* Was unlocked: check if view is required */
      triggerViewThing(found, mode_view, view_time);

      /* Was unlocked: check if view is required */
      //bool view, scroll;
      //EventSet::dataEnumView(mode_view, view, scroll);
      //if(view && view_time > 0)
      //{
        // TODO
        //std::cout << "TODO - View thing: " << thing_id << ",Scroll: "
        //          << scroll << ",Time: " << view_time << std::endl;
      //}
    }
  }
}

/* Unlock triggers, based on parameter information */
void Map::unlockTile(int section_id, int tile_x, int tile_y,
                     UnlockTileMode mode, UnlockView mode_view, int view_time)
{
  /* Find the tile */
  if(section_id < 0)
    section_id = map_index;
  if(section_id >= 0 && section_id < (int)sub_map.size() &&
     tile_x >= 0 && tile_x < (int)sub_map[section_id].tiles.size() &&
     tile_y >= 0 && tile_y < (int)sub_map[section_id].tiles[tile_x].size())
  {
    Tile* found = sub_map[section_id].tiles[tile_x][tile_y];
    if(found != nullptr)
    {
      bool enter, exit;
      bool unlocked = false;
      EventSet::dataEnumTileEvent(mode, enter, exit);

      /* Attempt to unlock enter/exit events */
      if(enter && found->getEventEnter() != nullptr)
        unlocked |= found->getEventEnter()->unlockTrigger();
      if(exit && found->getEventExit() != nullptr)
        unlocked |= found->getEventExit()->unlockTrigger();

      /* If unlocked, proceed to parse view and if required */
      if(unlocked)
      {
        triggerViewTile(found, section_id, mode_view, view_time);

        //bool view, scroll;
        //EventSet::dataEnumView(mode_view, view, scroll);
        //if(view && view_time > 0)
        //{
          // TODO
          //std::cout << "TODO - View tile: section: " << section_id << ",x: "
          //          << tile_x << ",y: " << tile_y << ",Scroll: " << scroll
          //          << ",Time: " << view_time << std::endl;
        //}
      }
    }
  }
}

/* Updates the game state */
bool Map::update(int cycle_time)
{
  std::vector<std::vector<Tile*>> tile_set;

  /* Check on music */
  if(music_id >= 0)
  {
    music_runtime -= cycle_time;
    if(music_runtime < 0)
      audioUpdate();
  }

  /* Check on player interaction */
  if(player != NULL && player->getTarget() != NULL
                    && !map_dialog.isConversationActive()
                    && !map_dialog.isConversationReady()
                    && !map_dialog.isConversationWaiting())
  {
    player->getTarget()->clearTarget();
    player->clearTarget();
  }

  /* Update the sprite animation */
  for(uint16_t i = 0; i < tile_sprites.size(); i++)
    tile_sprites[i]->update(cycle_time);

  /* Update map items */
  for(uint16_t i = 0; i < base_items.size(); i++)
    base_items[i]->update(cycle_time, tile_set);
  for(uint16_t i = 0; i < items.size(); i++)
    items[i]->update(cycle_time, tile_set);

  /* Update persons for movement and animation */
  for(uint16_t i = 0; i < persons.size(); i++)
  {
    if(persons[i] != NULL && persons[i]->getMapSection() == map_index &&
       persons[i]->isTilesSet())
    {
      if(persons[i]->isMoving() || persons[i]->isMoveRequested())
        tile_set = getTileMatrix(persons[i],
                                 persons[i]->getPredictedMoveRequest());

      /* Proceed to update the person */
      persons[i]->update(cycle_time, tile_set);
    }
  }

  /* Update map things */
  for(uint16_t i = 0; i < base_things.size(); i++)
    base_things[i]->update(cycle_time, tile_set);
  for(uint16_t i = 0; i < things.size(); i++)
    if(things[i]->getBase() == NULL)
      things[i]->update(cycle_time, tile_set);

  /* Update map interactive objects */
  for(uint16_t i = 0; i < ios.size(); i++)
    ios[i]->update(cycle_time, tile_set);

  /* If conversation is active, confirm that player is not moving */
  if(map_dialog.isConversationActive() || !isModeNormal())
    unfocus();

  /* Finally, update the viewport and dialogs */
  item_menu.update(cycle_time);
  map_dialog.update(cycle_time);
  updateMode(cycle_time);
  updateTileSize();
  viewport.update();

  return false;
}
