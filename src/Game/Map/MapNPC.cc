/*******************************************************************************
 * Class Name: MapNPC
 * Date Created: Dec 2 2012
 * Inheritance: MapPerson
 * Description: The MapNPC class, this covers all AI found on the map in game
 *
 * TODO: See header for list
 ******************************************************************************/
#include "Game/Map/MapNPC.h"

/* Constant Implementation - see header file for descriptions */
const uint16_t MapNPC::kFORCED_FREEZE = 5000;
const uint16_t MapNPC::kFORCED_NOTRIGGER = 30000;
const uint16_t MapNPC::kFORCED_RESET = 5000;
const uint16_t MapNPC::kMAX_DELAY = 2000;
const uint16_t MapNPC::kMAX_RANGE = 10;
const float MapNPC::kPYTH_APPROX = 0.4;
const uint16_t MapNPC::kSPOTTED_FADE = 1000;
const uint16_t MapNPC::kSPOTTED_INIT = 3000;
const uint16_t MapNPC::kSTUCK_DELAY = 250;
const uint16_t MapNPC::kTRACK_DELAY = 250;
const uint16_t MapNPC::kTRACK_DIST_MIN = 4;
const uint16_t MapNPC::kTRACK_DIST_MAX = 8;
const uint16_t MapNPC::kTRACK_DIST_RUN = 5;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for this class. Sets up an empty NPC with no data.
 *
 * Inputs: none
 */
MapNPC::MapNPC() : MapPerson()
{
  initializeClass();
}

/*
 * Description: Constructor for this class. Takes basic data to start with a
 *              basic version of the NPC.
 *
 * Inputs: int id - the ID of the NPC
 *         std::string name - the name of the NPC
 *         std::string description - the description of the NPC
 */
MapNPC::MapNPC(int id, std::string name, std::string description)
      : MapPerson(id, name, description)
{
  initializeClass();
}

/*
 * Description: Destructor function
 */
MapNPC::~MapNPC()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Appends an empty node onto the back of the movement stack.
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::appendEmptyNode()
{
  /* Set up the node structure */
  Path* new_node = new Path();
  new_node->x = 0;
  new_node->y = 0;
  new_node->delay = 0;
  new_node->xy_flip = false;
  new_node->previous = NULL;
  new_node->next = NULL;

  /* Append the node to the end of the list */
  if(!insertNode(getPathLength(), new_node))
    delete new_node;
}

/*
 * Description: Returns if the xy directional movement between nodes is flipped.
 *              If false, the order is x, then y. Otherwise, the order is
 *              reversed (y then x). This works with the current target node and
 *              the node it was previously at.
 *
 * Inputs: none
 * Output: bool - true if order is y then x. false if order is x then y.
 */
bool MapNPC::getXYFlip()
{
  if(starting)
  {
    return node_start.xy_flip;
  }
  else if(tracking)
  {
    return false;
  }
  else
  {
    if(node_state == LOOPED)
    {
      if(node_current->previous != nullptr)
        return node_current->previous->xy_flip;
    }
    else if(node_state == RANDOMRANGE || node_state == RANDOM)
    {
      return node_current->xy_flip;
    }
    else if(node_state == BACKANDFORTH)
    {
      if(node_current->previous != nullptr)
      {
        if(moving_forward)
          return node_current->previous->xy_flip;
        else
          return !node_current->xy_flip;
      }
    }
  }

  return false;
}

/*
 * Description: Returns the node at the given index. NULL if out of range.
 *
 * Inputs: uint16_t index - the node index, offset from HEAD
 * Output: Path* - the path structure at the node index
 */
Path* MapNPC::getNode(uint16_t index)
{
  bool failed = false;
  Path* returned_node = NULL;

  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
  {
    returned_node = static_cast<MapNPC*>(base)->getNode(index);
  }
  else
  {
    if(node_head != NULL)
    {
      /* Loop through to find the node */
      returned_node = node_head;
      for(uint16_t i = 0; i < index; i++)
      {
        returned_node = returned_node->next;
        if(returned_node == node_head)
          failed = true;
      }
    }
  }

  /* Execute the return sequence, if applicable */
  if(failed)
    returned_node = NULL;
  return returned_node;
}

/*
 * Description: Initializes the class. Only called on initial construction.
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::initializeClass()
{
  /* Clear the path pointers */
  forced_interaction = false;
  forced_recent = false;
  forced_time = 0;
  moving_forward = true;
  node_current = &node_start;
  node_head = nullptr;
  node_previous = nullptr;
  node_state = LOOPED;
  nodes_delete = true;
  npc_delay = 0;
  player = nullptr;
  spotted_img = nullptr;
  spotted_time = 0;
  starting = true;
  stuck_delay = 0;
  stuck_flip = false;
  track_delay = 0;
  track_dist = kTRACK_DIST_MIN;
  track_dist_max = kTRACK_DIST_MAX;
  track_dist_run = kTRACK_DIST_RUN;
  track_initial = false;
  track_recent = false;
  track_state = TrackingState::NOTRACK;
  tracking = false;

  /* Set the path player node to blank state */
  node_player.x = 0;
  node_player.y = 0;
  node_player.delay = 0;
  node_player.xy_flip = false;
  node_player.previous = nullptr;
  node_player.next = nullptr;

  /* Set the path random node to blank state */
  node_random.x = 0;
  node_random.y = 0;
  node_random.delay = 0;
  node_random.xy_flip = false;
  node_random.previous = NULL;
  node_random.next = NULL;

  /* Bounding rect init to 0 state */
  node_rect = {0, 0, 0, 0};

  /* Blank out start node */
  node_start.x = 0;
  node_start.y = 0;
  node_start.delay = 0;
  node_start.xy_flip = false;
  node_start.previous = &node_start;
  node_start.next = &node_start;
}

/*
 * Description: Inserts a node, at the given index. If inserted on a node where
 *              one exists already, it pushes it back to index + 1.
 *
 * Inputs: uint16_t index - the node index, offset from HEAD
 *         Path* node - the new path node struct to insert
 * Output: bool - true if successful
 */
bool MapNPC::insertNode(uint16_t index, Path* node)
{
  bool success = false;

  /* Only proceed if node is non-null */
  if(node != NULL)
  {
    /* Initial checks to see if nodes are set by base class */
    if(!nodes_delete)
    {
      node_head = NULL;
      nodes_delete = true;
    }

    /* If the head is 0 and the index is 0, drop it in the first slot */
    if(node_head == NULL && index == 0)
    {
      node->previous = node;
      node->next = node;
      node_head = node;
      success = true;
    }
    /* If it's at the end of the
     * Otherwise, drop it behind the first, if the index is in range */
    else if(index <= getPathLength())
    {
      Path* temp_node = node_head;
      success = true;

      /* Loop through to find the insertion point */
      for(uint16_t i = 0; i < index; i++)
        temp_node = temp_node->next;

      /* Re-reference the double linked list pointers */
      temp_node->previous->next = node;
      node->previous = temp_node->previous;
      temp_node->previous = node;
      node->next = temp_node;

      /* If the index is at 0, reset the head */
      if(index == 0)
        node_head = node;
    }

    /* If successful, reset the current node ptr of the class */
    if(success)
    {
      resetPosition();
      node_current = &node_start;
      npc_delay = 0;
      updateBound();
    }
  }

  return success;
}

/*
 * Description: Takes the current random path node and re-randomizes the x, y,
 *              delay, and flip parameters for random NPC movement. This is
 *              called each time the node is reached or movement is prevented
 *              by permanent object.
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::randomizeNode()
{
  /* X calc */
  int delta_x = Helpers::randU(-kMAX_RANGE, kMAX_RANGE);
  if(delta_x + node_random.x >= 0)
    node_random.x += delta_x;
  else
    node_random.x = 0;

  /* Y calc */
  int delta_y = Helpers::randU(-kMAX_RANGE, kMAX_RANGE);
  if(delta_y + node_random.y >= 0)
    node_random.y += delta_y;
  else
    node_random.y = 0;

  /* Delay calc */
  int delay = Helpers::randU(0, kMAX_DELAY); // -kMAX_DELAY, kMAX_DELAY);
  if(delay > 0)
    node_random.delay = delay;
  else
    node_random.delay = 0;

  /* Flip calc */
  node_random.xy_flip = Helpers::flipCoin();

  /* Validate range if it's a randomrange move */
  if(node_state == RANDOMRANGE)
  {
    /* Determine x and y range first */
    int x1 = node_head->x;
    int y1 = node_head->y;
    int x2 = node_head->next->x;
    int y2 = node_head->next->y;

    /* Order properly */
    if(x1 > x2)
    {
      int temp_x = x1;
      x1 = x2;
      x2 = temp_x;
    }
    if(y1 > y2)
    {
      int temp_y = y1;
      y1 = y2;
      y2 = temp_y;
    }

    /* Ensure within range */
    if(node_random.x < x1)
      node_random.x = x1;
    if(node_random.x > x2)
      node_random.x = x2;
    if(node_random.y < y1)
      node_random.y = y1;
    if(node_random.y > y2)
      node_random.y = y2;
  }
}

/*
 * Description: This function is called by the update call when an NPC is
 *              triggered to avoid the player and each update cycle to determine
 *              where the NPC should proceed to go (new node locations).
 *
 * Inputs: int cycle_time - the current previous cycle time that has lapsed
 *         bool stopped - true if the NPC is currently stopped and not moving
 * Output: none
 */
void MapNPC::trackAvoidPlayer(int cycle_time, bool stopped)
{
  /* Check if movement has stopped */
  if(stopped)// && (getTileX() != node_player.x || getTileY() != node_player.y))
    track_delay += cycle_time;

  /* Determine if an update should occur based on location of npc */
  if((!stopped && getTileX() == node_player.x &&
      getTileY() == node_player.y) ||
     track_initial || track_delay > kTRACK_DELAY)
  {
    Direction dir_curr = getDirection();
    int play_delt_x = getTileX() - player->getTileX();
    int play_delt_y = getTileY() - player->getTileY();
    int new_x = getTileX();
    int new_y = getTileY();
    track_initial = false;

    /* Change direction handle */
    bool change_dir = (track_delay > kTRACK_DELAY);
    if(change_dir)
      track_delay = 0;

    /* Parse and determine quadrant - on X-Axis - E-W */
    if(play_delt_x == 0)
    {
      if(!change_dir || (change_dir && dir_curr == Direction::WEST))
      {
        /* In N quadrant */
        if(play_delt_y > 0)
          new_y++;
        /* In S quadrant */
        else
          new_y--;
      }
      else
      {
        if(dir_curr == Direction::EAST)
          new_x--;
        else
          new_x++;
      }
    }
    /* On Y-Axis - N-S */
    else if(play_delt_y == 0)
    {
      if(!change_dir || (change_dir && dir_curr == Direction::NORTH))
      {
        /* In W quadrant */
        if(play_delt_x > 0)
          new_x++;
        /* In E quadrant */
        else
          new_x--;
      }
      else
      {
        if(dir_curr == Direction::SOUTH)
          new_y--;
        else
          new_y++;
      }
    }
    /* In NW quadrant */
    else if(play_delt_x > 0 && play_delt_y > 0)
    {
      if((!change_dir && dir_curr == Direction::SOUTH) ||
         (change_dir && dir_curr != Direction::SOUTH))
        new_y++;
      else
        new_x++;
    }
    /* In SW quadrant */
    else if(play_delt_x > 0 && play_delt_y < 0)
    {
      if((!change_dir && dir_curr == Direction::NORTH) ||
         (change_dir && dir_curr != Direction::NORTH))
        new_y--;
      else
        new_x++;
    }
    /* In NE quadrant */
    else if(play_delt_x < 0 && play_delt_y > 0)
    {
      if((!change_dir && dir_curr == Direction::SOUTH) ||
         (change_dir && dir_curr != Direction::SOUTH))
        new_y++;
      else
        new_x--;
    }
    /* In SE quadrant */
    else if(play_delt_x < 0 && play_delt_y < 0)
    {
      if((!change_dir && dir_curr == Direction::NORTH) ||
         (change_dir && dir_curr != Direction::NORTH))
        new_y--;
      else
        new_x--;
    }

    /* Check the new point */
    if(new_x < 0)
      new_x = 0;
    node_player.x = new_x;
    if(new_y < 0)
      new_y = 0;
    node_player.y = new_y;
  }
}

/*
 * Description: This is called by the update call to check if the given map
 *              person reference is out of the calculated 'home' bounding
 *              box range. It returns the number of tiles in approx distance
 *              from the 'home' range.
 *
 * Inputs: MapPerson* ref - the reference person to check the tile x and y
 * Output: int - the number of tiles in distance from home
 */
int MapNPC::trackOutOfRange(MapPerson* ref)
{
  if(ref == nullptr)
    ref = this;
  if(node_state != RANDOM)
  {
    int dist = 0;
    int npc_x = ref->getTileX();
    int npc_y = ref->getTileY();
    int x1 = node_rect.x;
    int x2 = node_rect.x + node_rect.w;
    int y1 = node_rect.y;
    int y2 = node_rect.y + node_rect.h;

    /* Determine if it outside the range rect */
    if(npc_x < x1 || npc_x > x2 || npc_y < y1 || npc_y > y2)
    {
      /* Separate out into the given range - North */
      if(npc_y < y1)
      {
        /* North West */
        if(npc_x < x1)
        {
          int temp_x = (x1 - npc_x);
          int temp_y = (y1 - npc_y);
          if(temp_x > temp_y)
            dist = temp_x + temp_y * kPYTH_APPROX;
          else
            dist = temp_y + temp_x * kPYTH_APPROX;
        }
        /* North East */
        else if(npc_x > x2)
        {
          int temp_x = (npc_x - x2);
          int temp_y = (y1 - npc_y);
          if(temp_x > temp_y)
            dist = temp_x + temp_y * kPYTH_APPROX;
          else
            dist = temp_y + temp_x * kPYTH_APPROX;
        }
        /* North */
        else
        {
          dist = (y1 - npc_y);
        }
      }
      /* South */
      else if(npc_y > y2)
      {
        /* South West */
        if(npc_x < x1)
        {
          int temp_x = (x1 - npc_x);
          int temp_y = (npc_y - y2);
          if(temp_x > temp_y)
            dist = temp_x + temp_y * kPYTH_APPROX;
          else
            dist = temp_y + temp_x * kPYTH_APPROX;
        }
        /* South East */
        else if(npc_x > x2)
        {
          int temp_x = (npc_x - x2);
          int temp_y = (npc_y - y2);
          if(temp_x > temp_y)
            dist = temp_x + temp_y * kPYTH_APPROX;
          else
            dist = temp_y + temp_x * kPYTH_APPROX;
        }
        /* South */
        else
        {
          dist = (npc_y - y2);
        }
      }
      /* West */
      else if(npc_x < x1)
      {
        dist = (x1 - npc_x);
      }
      /* East */
      else if(npc_x > x2)
      {
        dist = (npc_x - x2);
      }

      return dist;
    }
  }
  return -1;
}

/*
 * Description: This is called in order to update the home bound rect struct.
 *              It is required to be accessed each time any node changes occur
 *              to update what is defined as home.
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::updateBound()
{
  Path* node_parse = node_head;
  int x_max = -1;
  int x_min = -1;
  int y_max = -1;
  int y_min = -1;

  /* Proceed to generate new information - on node set*/
  if(node_parse != nullptr)
  {
    do
    {
      /* Min */
      if(x_min < 0 || (node_parse->x < x_min))
        x_min = node_parse->x;
      if(y_min < 0 || (node_parse->y < y_min))
        y_min = node_parse->y;

      /* Max */
      if(x_max < 0 || (node_parse->x > x_max))
        x_max = node_parse->x;
      if(y_max < 0 || (node_parse->y > y_max))
        y_max = node_parse->y;

      /* Go to next */
      node_parse = node_parse->next;

    } while(node_parse != node_head);
  }
  /* If no nodes, it is the starting point */
  else
  {
    x_min = getStartingX();
    x_max = getStartingX();
    y_min = getStartingY();
    y_max = getStartingY();
  }

  /* If data is valid, load it in */
  if(x_min >= 0 && y_min >= 0 && x_max >= 0 && y_max >= 0)
  {
    node_rect = {x_min, y_min, x_max - x_min + 1, y_max - y_min + 1};
  }
  else
  {
    node_rect = {0, 0, 0, 0};
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Renders additional images or frames on top of the designated
 *              tile. Called from the render call. This is virtualized for use
 *              by all children. MapNPC has spotted frame to render at times.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine pointer
 *         Tile* tile - the reference tile to be rendering
 *         int tile_x - the x location in the matrix in tiles
 *         int tile_y - the y location in the matrix in tiles
 *         int render_x - the top x corner of the tile rendering in px
 *         int render_y - the top y corner of the tile rendering in px
 * Output: bool - status if call was successful
 */
bool MapNPC::renderAdditional(SDL_Renderer* renderer, Tile* tile,
                              int tile_x, int tile_y,
                              int render_x, int render_y)
{
  bool success = true;

  /* Render spotted if valid and in correct tile */
  if(spotted_img != nullptr && tile_y == 0 && spotted_time > 0)
  {
    Direction dir = getDirection();
    SpriteMatrix* matrix = getMatrix();

    /* Set alpha */
    if((uint16_t)spotted_time >= kSPOTTED_FADE)
      spotted_img->setAlpha();
    else
      spotted_img->setAlpha(spotted_time * 255.0 / kSPOTTED_FADE);

    /* Determine render location - if facing east, top left */
    if(dir == Direction::EAST)
    {
      if(tile_x == 0)
      {
        success &= spotted_img->render(renderer, render_x, render_y);
      }
    }
    /* Otherwise, top right */
    else
    {
      if((tile_x + 1) == matrix->width())
      {
        success &= spotted_img->render(renderer,
                         render_x + tile->getWidth() - spotted_img->getWidth(),
                         render_y);
      }
    }
  }
  return success;
}

/*
 * Description: Sets a new direction for the person on the map. It will update
 *              the parent frame so a new classifier is printed.
 *
 * Inputs: Direction direction - the new direction to set
 *         bool set_movement - if the movement should be set as well
 * Output: bool - indicates if the directional movement changed
 */
bool MapNPC::setDirection(Direction direction, bool set_movement)
{
  //return MapPerson::setDirection(direction, set_movement);

  bool changed = (this->direction != direction);
  bool movement_changed = false;

  /* If moving, set the direction in map thing */
  if(set_movement)
  {
    /* Update preferred movement direction */
    movement_changed = MapThing::setDirection(direction);
  }
  else
  {
    MapThing::setDirection(Direction::DIRECTIONLESS);
  }

  /* Rotate direction */
  if(set_movement || getTarget() != nullptr || forced_recent ||
     node_current == nullptr || node_current->x == getTileX() ||
     node_current->y == getTileY() )
  {
    /* If it's a movement direction, rotate the fellow */
    SpriteMatrix* state = getState(surface, direction);
    if(state != NULL)
    {
      if(changed)
        setMatrix(state);

      /* Finally set the in class direction */
      this->direction = direction;
    }
  }

  return movement_changed;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds npc information from the XML file. This will be
 *              virtually called by children where the child will deal with
 *              additional sets needed and then pass call to this. Works off
 *              reading the XML file in a strict manner. Passes call to parent
 *              after it is complete.
 *
 * Inputs: XmlData data - the read XML data
 *         int file_index - the index in the xml data where this detail begins
 *         int section_index - the map section index of the npc
 *         SDL_Renderer* renderer - the graphical rendering engine pointer
 *         std::string base_path - the base path for resources
 *         bool from_save - true if the load is from a save file
 * Output: bool - status if successful
 */
bool MapNPC::addThingInformation(XmlData data, int file_index,
                                 int section_index, SDL_Renderer* renderer,
                                 std::string base_path, bool from_save)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;

  /* Parse the identifier for setting the person information */
  /*--------------------- NODESTATE --------------------*/
  if(identifier == "nodestate" && elements.size() == 1)
  {
    std::string state = data.getDataString(&success);
    if(success)
    {
      if(state == "looped")
        setNodeState(MapNPC::LOOPED);
      else if(state == "backandforth")
        setNodeState(MapNPC::BACKANDFORTH);
      else if(state == "locked")
        setNodeState(MapNPC::LOCKED);
      else if(state == "randomrange")
        setNodeState(MapNPC::RANDOMRANGE);
      else if(state == "random")
        setNodeState(MapNPC::RANDOM);
      else
        success = false;
    }
  }
  /*--------------------- NODE MOVEMENT --------------------*/
  else if(identifier == "node" && elements.size() == 2)
  {
    uint16_t index = std::stoul(data.getKeyValue(file_index));
    uint16_t length = getPathLength();

    /* Increase the length if necessary to allow for the new node */
    while(index >= length)
    {
      appendEmptyNode();
      length++;
    }

    /* Get the node and work with it, if relevant */
    Path* node = getNode(index);
    if(node != NULL)
    {
      /* Parse the node identity */
      if(elements[1] == "x")
      {
        uint16_t x = data.getDataInteger(&success);
        if(success)
        {
          node->x = x;
          updateBound();
        }
      }
      else if(elements[1] == "y")
      {
        uint16_t y = data.getDataInteger(&success);
        if(success)
        {
          node->y = y;
          updateBound();
        }
      }
      else if(elements[1] == "delay")
      {
        uint16_t delay = data.getDataInteger(&success);
        if(success)
          node->delay = delay;
      }
      else if(elements[1] == "xyflip")
      {
        bool flip = data.getDataBool(&success);
        if(success)
          node->xy_flip = flip;
      }
    }
    else
    {
      success = false;
    }
  }
  /*--------------------- START NODE --------------------*/
  else if(identifier == "startnode" && elements.size() == 2)
  {
    /* Parse the node identity */
    if(elements.back() == "delay")
    {
      uint16_t delay = data.getDataInteger(&success);
      if(success)
        node_start.delay = delay;
    }
    else if(elements.back() == "xyflip")
    {
      bool flip = data.getDataBool(&success);
      if(success)
        node_start.xy_flip = flip;
    }
  }
  /* -- TRACKING DIST MAX -- */
  else if(identifier == "trackmax" && elements.size() == 1)
  {
    setTrackingDist(track_dist, data.getDataInteger(), track_dist_run);
  }
  /* -- TRACKING DIST MIN -- */
  else if(identifier == "trackmin" && elements.size() == 1)
  {
    setTrackingDist(data.getDataInteger(), track_dist_max, track_dist_run);
  }
  /* -- TRACKING DIST RUN -- */
  else if(identifier == "trackrun" && elements.size() == 1)
  {
    setTrackingDist(track_dist, track_dist_max, data.getDataInteger());
  }
  /*--------------------- TRACKING --------------------*/
  else if(identifier == "tracking" && elements.size() == 1)
  {
    std::string state = data.getDataString(&success);
    if(success)
    {
      if(state == "none")
        setTrackingState(TrackingState::NOTRACK);
      else if(state == "toplayer")
        setTrackingState(TrackingState::TOPLAYER);
      else if(state == "avoidplayer")
        setTrackingState(TrackingState::AVOIDPLAYER);
      else
        success = false;
    }
  }
  /*----------------- FORCED INTERACTION --------------------*/
  else if(identifier == "forcedinteraction" && elements.size() == 1)
  {
    bool interact = data.getDataBool(&success);
    if(success)
      setForcedInteraction(interact);
  }
  /* Proceed to parent */
  else
  {
    success &= MapPerson::addThingInformation(data, file_index, section_index,
                                              renderer, base_path);
    if(identifier == "startpoint")
      updateBound();
  }

  /* If not from save, reset changed back to false */
  if(!from_save)
    changed = false;

  return success;
}

/*
 * Description: This is the class descriptor. Primarily used for encapsulation
 *              to determine which class to cast it to for specific parameters.
 *
 * Inputs: none
 * Output: ThingBase - the thing description enum defining the type to cast
 */
ThingBase MapNPC::classDescriptor()
{
  //return "MapNPC";
  return ThingBase::NPC;
}

/*
 * Description: Clears out all information stored in the class
 *
 * Inputs: none
 * Output: none
 */
void MapNPC::clear()
{
  /* Clear out all the nodes */
  removeAllNodes();
  node_head = nullptr;
  node_current = &node_start;

  /* Clear out other variables */
  npc_delay = 0;
  player = nullptr;
  spotted_img = nullptr;
  spotted_time = 0;

  /* Clear out parent */
  MapPerson::clear();
}

/*
 * Description: Gets if the movement is paused. This calls on parent MapThing
 *              for paused information in addition to forced recent freeze
 *              holds triggered by NPC.
 *
 * Inputs: none
 * Output: bool - true if the movement is paused
 */
bool MapNPC::getMovementPaused()
{
  return MapThing::getMovementPaused() ||
         (forced_recent && forced_time < kFORCED_FREEZE);
}

/*
 * Description: Returns the current node state, which is an Enum.
 *
 * Inputs: none
 * Output: MapNPC::NodeState - a node state enum
 */
MapNPC::NodeState MapNPC::getNodeState()
{
  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
    return static_cast<MapNPC*>(base)->node_state;
  return node_state;
}

/*
 * Description: Returns the number of nodes in the node sequence of the NPC.
 *
 * Inputs: none
 * Output: uint16_t - the number of nodes from HEAD to TAIL
 */
uint16_t MapNPC::getPathLength()
{
  uint16_t size = 1;
  Path* temp_node = node_head;

  /* Check if base is used or not */
  if(base != NULL && base_category == ThingBase::NPC && !nodes_delete)
  {
    size = static_cast<MapNPC*>(base)->getPathLength();
  }
  else
  {
    if(node_head == NULL)
      size = 0;
    else
    {
      while(temp_node->next != node_head)
      {
        temp_node = temp_node->next;
        size++;
      }
    }
  }

  return size;
}

/*
 * Description: Returns the player reference. NULL if not set
 *
 * Inputs: none
 * Output: MapPerson* - the person reference pointer
 */
MapPerson* MapNPC::getPlayer()
{
  return player;
}

/*
 * Description: Returns the predicted move request for the class, based on the
 *              next node in the sequence.
 *
 * Inputs: none
 * Output: Direction - the direction the NPC will be pointing
 */
Direction MapNPC::getPredictedMoveRequest()
{
  if(isTilesSet() && node_current != NULL)
  {
    int delta_x = node_current->x - tile_main.front().front()->getX();
    int delta_y = node_current->y - tile_main.front().front()->getY();
    Direction direction = Direction::DIRECTIONLESS;
    bool xy_flip = getXYFlip() ^ stuck_flip;

    /* If the npc needs to move on the X plane */
    if(delta_x != 0 && (!xy_flip || (xy_flip && delta_y == 0)))
    {
      if(delta_x < 0)
        direction = Direction::WEST;
      else
        direction = Direction::EAST;
    }
    /* Else if the npc needs to move on the Y plane */
    else if(delta_y != 0)
    {
      if(delta_y < 0)
        direction = Direction::NORTH;
      else
        direction = Direction::SOUTH;
    }

    return direction;
  }

  return getMoveRequest();
}

/*
 * Description: Returns the spotted reference image frame.
 *
 * Inputs: none
 * Output: Frame* - the spotted image reference
 */
Frame* MapNPC::getSpottedImage()
{
  return spotted_img;
}

/*
 * Description: Returns the spotted time remaining for visibility. If greater
 *              than 0, it is currently visible. Otherwise, not being rendered.
 *
 * Inputs: none
 * Output: int - the time in milliseconds remaining for visibility
 */
int MapNPC::getSpottedTime()
{
  return spotted_time;
}

/*
 * Description: Returns the tracking distance max in tile count when the
 *              tracking NPC releases its hold on the player.
 *
 * Inputs: none
 * Output: int - the distance in tiles for the max
 */
int MapNPC::getTrackDistMax()
{
  return track_dist_max;
}

/*
 * Description: Returns the tracking distance min in tile count when the
 *              tracking NPC initiates its hold on the player and follows or
 *              runs away.
 *
 * Inputs: none
 * Output: int - the distance in tiles for the min
 */
int MapNPC::getTrackDistMin()
{
  return track_dist;
}

/*
 * Description: Returns the tracking distance run in tile count when the
 *              tracking NPC which is set to keep away from the player attempts
 *              to maintain this distance at all times.
 *
 * Inputs: none
 * Output: int - the distance in tiles for the keep away
 */
int MapNPC::getTrackDistRun()
{
  return track_dist_run;
}

/*
 * Description: Returns the tracking state. This defines how the NPC reacts to
 *              the player as it nears the NPC.
 *
 * Inputs: none
 * Output: MapNPC::TrackingState - the tracking state enum
 */
TrackingState MapNPC::getTrackingState()
{
  //if(base != nullptr)
  //{
  //  MapNPC* base_npc = static_cast<MapNPC*>(base);

  //  if(base_npc->track_state != track_state)
  //    return track_state;
  //  else
  //    return base_npc->track_state;
  //}
  return track_state;
}

/*
 * Description: Inserts a new node that is created with an x and y point and a
 *              delay for pausing at the given index. If a node already exists
 *              at the index, it pushes all other nodes back one.
 *
 * Inputs: uint16_t index - the node index, offset from HEAD
 *         uint16_t x - the x tile location for the node
 *         uint16_t y - the y tile location for the node
 *         uint16_t delay - the time to pause on the node when reached (ms)
 * Output: bool - true if the insertion was successful
 */
bool MapNPC::insertNode(uint16_t index, uint16_t x, uint16_t y, uint16_t delay)
{
  bool success = true;

  /* Set up the node structure */
  Path* new_node = new Path();
  new_node->x = x;
  new_node->y = y;
  new_node->delay = delay;

  /* Attempt to insert the node */
  if(!insertNode(index, new_node))
  {
    delete new_node;
    success = false;
  }

  return success;
}

/*
 * Description: Inserts a new node that is created with an x and y point and a
 *              delay for pausing at the tail of the node stack
 *
 * Inputs: uint16_t x - the x tile location for the node
 *         uint16_t y - the y tile location for the node
 *         uint16_t delay - the time to pause on the node when reached (ms)
 * Output: bool - true if the insertion was successful
 */
bool MapNPC::insertNodeAtTail(uint16_t x, uint16_t y, uint16_t delay)
{
  return insertNode(getPathLength(), x, y, delay);
}

/*
 * Description: This is the call map will make when a forced interaction is
 *              triggered after it is determined the 'initiator' is nearby.
 *              Also sets some hold delays for when the NPC will trigger again
 *
 * Inputs: MapPerson* initiator - the event initiator, typically the player
 * Output: bool - true if an event was triggered
 */
bool MapNPC::interactForced(MapPerson* initiator)
{
  if(!forced_recent && interact(initiator))
  {
    forced_recent = true;
    forced_time = 0;
    return true;
  }
  return false;
}

/*
 * Description: Returns if the NPC will force interaction upon the player if it
 *              moves within the vicinity of the NPC.
 *
 * Inputs: bool false_if_active - true to also include if there has been a
 *                                recent forced interaction. If so, this will
 *                                return false regardless. Default true
 * Output: bool - true if the NPC will force interaction
 */
bool MapNPC::isForcedInteraction(bool false_if_active)
{
  //bool curr = forced_interaction;

  //if(base != nullptr)
  //{
  //  MapNPC* base_npc = static_cast<MapNPC*>(base);

  //  if(base_npc->forced_interaction != forced_interaction)
  //    curr = forced_interaction;
  //  else
  //    curr = base_npc->forced_interaction;
  //}
  //return (curr && !forced_recent);
  if(false_if_active)
    return (forced_interaction && !forced_recent);
  return forced_interaction;
}

/*
 * Description: Returns if there is any tracking within the NPC.
 *
 * Inputs: none
 * Output: bool - true if any tracking is set
 */
bool MapNPC::isTracking()
{
  return (getTrackingState() != TrackingState::NOTRACK);
}

/*
 * Description: Removes all nodes in the NPC sequence.
 *
 * Inputs: none
 * Output: bool - true if successful
 */
bool MapNPC::removeAllNodes()
{
  if(nodes_delete)
    while(removeNodeAtTail());
  else
  {
    node_head = NULL;
    node_current = &node_start;
  }

  return true;
}

/*
 * Description: Removes the node at the given index. If this has nodes after it,
 *              those nodes are bumped up (index - 1).
 *
 * Inputs: uint16_t index - the index of the node to remove
 * Output: bool - true if successful
 */
bool MapNPC::removeNode(uint16_t index)
{
  uint16_t length = getPathLength();
  bool success = false;
  Path* temp_node = NULL;

  if(nodes_delete)
  {
    if(index == 0 && length == 1)
    {
      temp_node = node_head;
      node_head = NULL;
      node_current = &node_start;
      success = true;
    }
    else if(index < length)
    {
      /* Loop through to find the node to remove */
      temp_node = node_head;
      for(uint16_t i = 0; i < index; i++)
        temp_node = temp_node->next;

      /* Reset the pointers around the path to delete */
      temp_node->previous->next = temp_node->next;
      temp_node->next->previous = temp_node->previous;

      /* Fix the pointers if they need to be changed */
      if(index == 0)
        node_head = temp_node->next;

      updateBound();
      success = true;
    }
  }

  /* Finish by deleting the pointer */
  if(success)
    delete temp_node;

  return success;
}

/*
 * Description: Removes the node at the tail of the node sequence.
 *
 * Inputs: none
 * Output: bool - true if successful
 */
bool MapNPC::removeNodeAtTail()
{
  if(node_head != NULL)
    return removeNode(getPathLength() - 1);
  return false;
}

/* Resets the tile position */
/*
 * Description: Resets the position of the npc back to the initial starting
 *              point. This is the position that was set when the last
 *              setStartingTile() was called.
 *
 * Inputs: none
 * Output: bool - status if successful
 */
bool MapNPC::resetPosition()
{
  if(MapPerson::resetPosition())
  {
    forced_recent = false;
    node_current = &node_start;
    starting = true;
    stuck_flip = false;
    track_recent = false;
    return true;
  }
  return false;
}

/*
 * Description: Sets the base thing class. If set, the primary data will be set
 *              from this with only location and movement handled by this class.
 *
 * Inputs: MapThing* base - the reference base class
 * Output: bool - true if the base was set
 */
bool MapNPC::setBase(MapThing* base)
{
  bool success = false;

  if(classDescriptor() == ThingBase::NPC)
  {
    /* Set the base with the new ptr */
    if(base != NULL && base->classDescriptor() == ThingBase::NPC)
    {
      this->base = base;
      base_category = ThingBase::NPC;
      setMatrix(getState(getSurface(), getDirection()));
      setSpeed(base->getSpeed());
      setVisibility(base->isVisible());
      forced_interaction = static_cast<MapNPC*>(base)->forced_interaction;
      track_state = static_cast<MapNPC*>(base)->track_state;
      if(node_head == NULL)
      {
        node_head = static_cast<MapNPC*>(base)->node_head;
        nodes_delete = false;
        node_current = &node_start;
        npc_delay = 0;
        updateBound();
      }
      success = true;
    }
    /* Clear out the old base and make this class a base class */
    else if(base == NULL)
    {
      this->base = NULL;
      base_category = ThingBase::ISBASE;
      success = true;
    }

    /* Update the event set */
    setEventBase();
  }

  return success;
}

/*
 * Description: Sets if the NPC will force interaction on a player if it moves
 *              within the vicinity.
 *
 * Inputs: bool forced - true if the NPC should force interaction
 * Output: none
 */
void MapNPC::setForcedInteraction(bool forced)
{
  forced_interaction = forced;
  forced_recent = false;
}

/*
 * Description: Sets the node state and how nodes are parsed with the controller
 *              enum.
 *
 * Inputs: NodeState state - the new node state
 * Output: none
 */
void MapNPC::setNodeState(NodeState state)
{
  node_state = state;

  /* Set starting moving direction */
  if(state == BACKANDFORTH)
    moving_forward = true;

  /* Fix the current node state */
//  if(node_state == NodeState::LOOPED ||
//     node_state == NodeState::BACKANDFORTH)
//  {
//    node_current = node_head;
//  }
//  else if(node_state == NodeState::RANDOM ||
//          node_state == NodeState::RANDOMRANGE)
//  {
//    node_current = &node_random;
//  }
}

/*
 * Description: Sets the reference player. The reference player is used for
 *              tracking and event forced triggering by the NPC object
 *
 * Inputs: MapPerson* player - the new player reference. NULL to unset
 * Output: none
 */
void MapNPC::setPlayer(MapPerson* player)
{
  this->player = player;
}

/*
 * Description: Sets the spotted reference image. If null is passed in, it
 *              unsets all actively used rendering frames.
 *
 * Inputs: none
 * Output: Frame* new_img - the new spotted frame to render
 */
void MapNPC::setSpottedImage(Frame* new_img)
{
  spotted_img = new_img;
}

/*
 * Description: Sets the starting location which needs the map section id and
 *              an x and y coordinate. Virtually called before sending to thing
 *              to set the random starting location.
 *
 * Inputs: uint16_t section_id - the map section id
 *         uint16_t x - the x coordinate of the thing
 *         uint16_t y - the y coordinate of the thing
 * Output: none
 */
void MapNPC::setStartingLocation(uint16_t section_id, uint16_t x, uint16_t y)
{
  node_random.x = x;
  node_random.y = y;

  node_start.x = x;
  node_start.y = y;

  /* Set to parent */
  MapThing::setStartingLocation(section_id, x, y);
}

/*
 * Description: Sets the three tracking distance tile setpoints for the
 *              current NPC. For explanation of each, see the associated
 *              getter functions.
 *
 * Inputs: int trigger - the distance from the player to trigger tracking
 *         int max - the distance from the player or home to release tracking
 *         int run - the distance from the player to stay away if running
 * Output: none
 */
void MapNPC::setTrackingDist(int trigger, int max, int run)
{
  if(trigger > 0)
    track_dist = trigger;
  if(max > 0)
    track_dist_max = max;
  if(run > 0)
    track_dist_run = run;
}

/*
 * Description: Sets the tracking state of the NPC and how it reacts as a player
 *              comes in the vicinity, based on a set range.
 *
 * Inputs: TrackingState state - the tracking state that the NPC should execute
 * Output: none
 */
void MapNPC::setTrackingState(TrackingState state)
{
  track_state = state;
  track_recent = false;
}

/*
 * Description: Updates the frames of the NPC. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the ms time to update the movement/animation
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 *         bool active_map - true if this npcs section is the active map
 * Output: Floatinate - the delta x and y of the moved npc
 */
Floatinate MapNPC::update(int cycle_time,
                          std::vector<std::vector<Tile*>> tile_set,
                          bool active_map)
{
  /* For active and set tiles, update movement and animation */
  if(isActive() && isTilesSet() && active_map)
  {
    /* Begin the check to handle each time the NPC is on a tile */
    if(node_current != nullptr) //getNodeState() != LOCKED)
    {
      /* Acquire direction */
      Direction direction = getPredictedMoveRequest();

      /* Variables */
      int delta = 0;
      int delta_range = 0;
      bool stopped = !isMoving();

      /* Do checks if stuck */
      if(stopped && node_current->x != getTileX() && /* removed !tracking */
         node_current->y != getTileY())
      {
        stuck_delay += cycle_time;
        if(stuck_delay > kSTUCK_DELAY)
        {
          stuck_delay = 0;
          stuck_flip = !stuck_flip;
        }
      }

      /* If starting sequence, operate on different parameters */
      if(starting)
      {
        if(stopped)
        {
          /* If reached node and done movement again, kill starting */
          if(node_current != &node_start &&
             node_current->x == tile_main.front().front()->getX() &&
             node_current->y == tile_main.front().front()->getY())
          {
            if(node_state == RANDOMRANGE)
            {
              node_current = &node_random;
              node_random.x = node_head->x;
              node_random.y = node_head->y;
            }
            starting = false;
          }
          else if(node_current == &node_start)
          {
            /* Delay first */
            if(node_start.delay > npc_delay)
            {
              npc_delay += cycle_time;
            }
            /* Then updates current node */
            else
            {
              if(node_state == LOOPED || node_state == BACKANDFORTH ||
                 node_state == RANDOMRANGE)
              {
                if(node_head != nullptr)
                  node_current = node_head;
                else
                  starting = false;
              }
              else if(node_state == RANDOM)
              {
                node_current = &node_random;
                starting = false;
              }
              else
              {
                node_current = node_head;
                starting = false;
              }

              npc_delay = 0;
            }
          }
        }
      }
      else
      {
        /* Handle tracking */
        if(track_state != TrackingState::NOTRACK && player != nullptr &&
           !player->isInteractionDisabled())
        {
          /* Delta X/Y distances */
          int delta_x = 0;
          if(player->getTileX() >= getTileX())
            delta_x = player->getTileX() - getTileX();
          else
            delta_x = getTileX() - player->getTileX();
          int delta_y = 0;
          if(player->getTileY() >= getTileY())
            delta_y = player->getTileY() - getTileY();
          else
            delta_y = getTileY() - player->getTileY();

          /* Main delta */
          if(delta_x >= delta_y)
            delta = delta_x + delta_y * kPYTH_APPROX;
          else
            delta = delta_y + delta_x * kPYTH_APPROX;

          /* Logic for when NPC is not currently tracking */
          if(!tracking)
          {
            /* Check the recent status */
            if(track_recent)
            {
              /* If stopped, compute stuck time */
              if(stopped)
                track_delay += cycle_time;
              else
                track_delay = 0;

              /* Once limits reach, reset tracking recent */
              if((getTileX() == node_current->x &&
                  getTileY() == node_current->y) ||
                 (stopped && track_delay > kFORCED_RESET))
              {
                track_recent = false;
              }
            }

            /* Check if tracking should be enabled */
            if(delta <= track_dist && !track_recent && !forced_recent)
            {
              tracking = true;
              track_delay = 0;
              track_initial = true;
              node_previous = node_current;
              node_current = &node_player;

              /* Initialize node */
              node_player.x = getTileX();
              node_player.y = getTileY();

              /* Trigger spotted */
              spotted_time = kSPOTTED_INIT;
              if(event_handler != nullptr)
                event_handler->triggerSound(Sound::kID_SOUND_SPOTTED,
                                            SoundChannels::TRIGGERS);
            }
          }
          /* Otherwise it is tracking - handle */
          else
          {
            /* Check the distance */
            delta_range = trackOutOfRange();

            /* Check if tracking should be disabled */
            if(delta > track_dist_max || delta_range > track_dist_max ||
               forced_recent || getTarget() != nullptr ||
               (track_state == TrackingState::AVOIDPLAYER &&
                delta_range == track_dist_max &&
                trackOutOfRange(player) >= delta_range))
            {
              tracking = false;
              track_delay = 0;
              track_initial = false;
              track_recent = true;
              node_current = node_previous;
              node_previous = nullptr;
            }
          }
        }

        /* If tracking, modify how movement is handled */
        if(tracking)// && getTarget() == nullptr)
        {
          /* Track to the player location */
          if(track_state == TrackingState::TOPLAYER)
          {
            node_player.x = player->getTileX();
            node_player.y = player->getTileY();
          }
          /* Track from the player location */
          else
          {
            if(delta < track_dist_run && delta_range < track_dist_max)
              trackAvoidPlayer(cycle_time, stopped);
            else
              track_initial = true;
          }
        }
        /* On tile if not moving so handle pauses or shifts */
        else if(stopped && (direction == Direction::DIRECTIONLESS ||
                node_state == RANDOM || node_state == RANDOMRANGE))
        {
          /* Clear stuck data */
          stuck_delay = 0;
          stuck_flip = false;

          if(node_current->delay > npc_delay)
          {
            npc_delay += cycle_time;
          }
          else
          {
            if(node_state == LOOPED)
            {
              if(node_head != nullptr)
                node_current = node_current->next;
              else
                setDirection(getStartingDirection(), false);
            }
            else if(node_state == BACKANDFORTH)
            {
              if(node_head != nullptr)
              {
                /* Check to see if the ends are reached */
                if(moving_forward && node_current->next == node_head)
                  moving_forward = false;
                else if(!moving_forward && node_current == node_head)
                  moving_forward = true;

                /* Move in the new / old direction */
                if(moving_forward)
                  node_current = node_current->next;
                else
                  node_current = node_current->previous;
              }
              else
              {
                setDirection(getStartingDirection(), false);
              }
            }
            else if(node_state == RANDOM ||
                    (node_state == RANDOMRANGE && node_head != NULL))
            {
              /* If direction is not directionless, reset random location */
              if(direction != Direction::DIRECTIONLESS)
              {
                node_random.x = tile_main.front().front()->getX();
                node_random.y = tile_main.front().front()->getY();
              }

              /* Randomize a new location */
              randomizeNode();
            }
            npc_delay = 0;
          }
        }
        else
        {
          npc_delay = 0;
        }
      }

      /* Update the new direction */
      if(getMoveRequest() != direction)
      {
        clearAllMovement();
        addDirection(direction);
      }
    }
    /* Locked state - just handle visible direction */
    else
    {
      /* If not targetting the player, restore direction */
      if(!getMovementPaused() && getTarget() == nullptr &&
         getDirection() != getStartingDirection())
      {
        setDirection(getStartingDirection(), false);
      }
    }

    /* Checks on the forced interaction delays and holds */
    if(forced_recent && getTarget() == nullptr)
    {
      forced_time += cycle_time;
      if(forced_time > kFORCED_NOTRIGGER)
        forced_recent = false;
    }

    /* Check on spotted display image */
    if(spotted_time > 0)
    {
      spotted_time -= cycle_time;
      if(spotted_time < 0)
        spotted_time = 0;
    }
  }

  /* Send call to parent */
  return MapPerson::update(cycle_time, tile_set, active_map);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the node state converted into a string.
 *
 * Inputs: NodeState state - the state of the node to convert
 * Output: std::string - the converted string
 */
std::string MapNPC::getNodeString(NodeState state)
{
  std::string node = "";

  if(state == LOOPED)
    node = "looped";
  else if(state == BACKANDFORTH)
    node = "backandforth";
  else if(state == RANDOMRANGE)
    node = "randomrange";
  else if(state == RANDOM)
    node = "random";
  else if(state == LOCKED)
    node = "locked";

  return node;
}

/*
 * Description: Returns the tracking state converted into a string.
 *
 * Inputs: NodeState state - the tracking state of the npc to convert
 * Output: std::string - the converted string
 */
std::string MapNPC::getTrackingString(TrackingState state)
{
  std::string node = "";

  if(state == TrackingState::NOTRACK)
    node = "none";
  else if(state == TrackingState::TOPLAYER)
    node = "toplayer";
  else if(state == TrackingState::AVOIDPLAYER)
    node = "avoidplayer";

  return node;
}
