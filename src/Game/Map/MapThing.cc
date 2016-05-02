/******************************************************************************
 * Class Name: MapThing
 * Date Created: Dec 2 2012
 * Inheritance: none
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapItem, etc. Also handles the basic setup for
 *              name, description, id, sprite. No interaction is handled in
 *              this class since its a generic parent.
 * Notes: Any private or protected function do not ensure sprite_set is not
 *        NULL or necessarily in range.
 *****************************************************************************/
#include "Game/Map/MapThing.h"

/* Constant Implementation - see header file for descriptions */
const int MapThing::kACTIVE_DEFAULT = -1;
const uint16_t MapThing::kDEFAULT_SPEED = 8;
const float MapThing::kMOVE_FACTOR = 4096.0;
const int MapThing::kPLAYER_ID = 0;
const uint8_t MapThing::kRAW_MULTIPLIER = 10;
const int MapThing::kUNSET_ID = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for this class. Sets up an empty thing with no
 *              data.
 *
 * Inputs: none
 */
MapThing::MapThing()
{
  base = NULL;
  base_category = ThingBase::ISBASE;
  base_control = NULL;
  event_handler = NULL;
  sound_id = kUNSET_ID;
  sprite_set = NULL;

  MapThing::clear();
}

/*
 * Description: Constructor for this class. Takes data to create the thing.
 *
 * Inputs: int id - the ID of the thing
 *         std::string name - the name of the thing, default to ""
 *         std::string description - the description of the thing, default to ""
 */
MapThing::MapThing(int id, std::string name, std::string description)
        : MapThing()
{
  setDescription(description);
  setID(id);
  setName(name);
}

/*
 * Description: Constructor for this class. Takes data to create the thing and
 *              also includes the frame data.
 *
 * Inputs: std::vector<std::vector<TileSprite*>> - the frame matrix data
 *         int id - the ID of the thing, default to -1
 *         std::string name - the name of the thing, default to ""
 *         std::string description - the description of the thing, default to ""
 */
MapThing::MapThing(std::vector<std::vector<TileSprite*>> frames, int id,
                   std::string name, std::string description)
        : MapThing(id, name, description)
{
  setFrames(frames, true);
}

/*
 * Description: Destructor function
 */
MapThing::~MapThing()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Animates the frames of the thing, based on the animate offset
 *              of the thing as well as calls to the sprite holder
 *
 * Inputs: int cycle_time - the msec time between the last animate call
 *         bool reset - Resets the animation back to head. Used for either
 *                      restarting animation or stopping it.
 *         bool skip_head - Skip the head of the list of frames
 * Output: bool - a status on the animate, if the frame sequence changed.
 */
bool MapThing::animate(int cycle_time, bool reset, bool skip_head)
{
  bool shift = false;
  SpriteMatrix* sprite_set = getMatrix();

  /* Check which animation controller to use - depending on the set base */
  if(base_category >= ThingBase::PERSON)
  {
    /* Reset back to head */
    if(reset && !skip_head && base_control->curr_frame != 0)
    {
      base_control->curr_frame = 0;
      base_control->time = 0;
      shift = true;
    }
    /* Reset back to one before head */
    else if(reset && skip_head && base_control->num_frames > 0)
    {
      base_control->curr_frame = 1;
      base_control->time = 0;
      shift = true;
    }
    /* If not reset, just update frames */
    else if(!reset)
    {
      /* Increment the time. If greater than frame time, got to next frame */
      base_control->time += cycle_time;
      if(base_control->time >= base_control->frame_time)
      {
        base_control->time -= base_control->frame_time;

        /* Check direction of movement */
        if(base_control->forward)
        {
          /* Increment */
          base_control->curr_frame++;

          /* Check to make sure the end limits haven't been reached */
          if(base_control->curr_frame >= base_control->num_frames)
          {
            if(skip_head && base_control->num_frames > 1)
              base_control->curr_frame = 1;
            else
              base_control->curr_frame = 0;
          }
        }
        else
        {
          /* Decrement */
          if(base_control->curr_frame == 0)
            base_control->curr_frame = base_control->num_frames - 1;
          else
            base_control->curr_frame--;

          /* Check to make sure the end limits haven't been reached */
          if(skip_head && base_control->curr_frame == 0 &&
             base_control->num_frames > 0)
            base_control->curr_frame = base_control->num_frames - 1;
        }

        shift = true;
      }
    }
  }
  else
  {
    /* Check if an animation can occur */
    for(uint16_t i = 0; i < sprite_set->width(); i++)
    {
      for(uint16_t j = 0; j < sprite_set->height(); j++)
      {
        if(sprite_set->at(i, j) != NULL)
        {
          /* Reset back to head */
          if(reset && !skip_head && !sprite_set->at(i, j)->isAtFirst())
          {
            sprite_set->at(i, j)->setAtFirst();
            shift = true;
          }

          if(reset)
            shift |= sprite_set->at(i, j)->update(0, skip_head);
          else
            shift |= sprite_set->at(i, j)->update(cycle_time, skip_head);
        }
      }
    }
  }

  return shift;
}

/*
 * Description: Can the tile be set with the passed frame. Fails if there is
 *              already a thing set on said tile
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the sprite frames pointer to set in the tile
 *         bool avoid_player - true that canSet will return false if 0 render
 *                             player is on location. default false
 * Output: bool - true if the set was successful
 */
bool MapThing::canSetTile(Tile* tile, TileSprite* frames, bool avoid_player)
{
  if(tile != nullptr)
  {
    /* Check if player is there */
    bool player = false;
    if(avoid_player && frames->getRenderDepth() == 0)
    {
      MapThing* person = (MapThing*)tile->getPerson(frames->getRenderDepth());
      if(person != nullptr && person->getID() == kPLAYER_ID)
        player = true;
    }

    /* Final return */
    return (!tile->isThingSet(frames->getRenderDepth()) && !player);
  }
  return false;
}

/*
 * Description: Returns the tile coordinate for the X direction with the
 *              addition of what percentage it has moved into the next tile.
 *              For example, if the thing was moving west from tile #4 to
 *              tile #3 and it was 1/4 way there, the returned value would
 *              be 3.75.
 *
 * Inputs: none
 * Output: float - the X location of the thing, in tile units.
 */
float MapThing::getFloatTileX()
{
  if(tile_prev.size() > 0)
    return tile_prev.front().front()->getX() + getMoveX();
  else if(tile_main.size() > 0)
    return tile_main.front().front()->getX();
  return 0.0;
}

/*
 * Description: Returns the tile coordinate for the Y direction with the
 *              addition of what percentage it has moved into the next tile.
 *              For example, if the thing was moving south from tile #6 to
 *              tile #7 and it was 1/4 way there, the returned value would
 *              be 6.25.
 *
 * Inputs: none
 * Output: float - the Y location of the thing, in tile units.
 */
float MapThing::getFloatTileY()
{
  if(tile_prev.size() > 0)
    return tile_prev.front().front()->getY() + getMoveY();
  else if(tile_main.size() > 0)
    return tile_main.front().front()->getY();
  return 0.0;
}

/*
 * Description: Returns the sprite matrix pointer. NULL if unset.
 *
 * Inputs: none
 * Output: SpriteMatrix* - the sprite matrix pointer
 */
SpriteMatrix* MapThing::getMatrix()
{
  if(base_category == ThingBase::THING || base_category == ThingBase::ITEM)
    return base->sprite_set;
  return sprite_set;
}

/*
 * Description: Returns the main rendering tile at the x and y coordinate.
 *
 * Inputs: uint32_t x - the x coordinate of the tile, with respect to the top
 *                      left of the matrix
 *         uint32_t y - the y coordinate of the tile, with respect to the top
 *                      left of the matrix
 * Output: Tile* - pointer to tile at the x,y. NULL if unset or out of range
 */
Tile* MapThing::getTileMain(uint32_t x, uint32_t y)
{
  if(tile_main.size() > x && tile_main[x].size() > y)
    return tile_main[x][y];
  return NULL;
}

/*
 * Description: Returns the previous rendering tile at the x and y coordinate.
 *
 * Inputs: uint32_t x - the x coordinate of the tile, with respect to the top
 *                      left of the matrix
 *         uint32_t y - the y coordinate of the tile, with respect to the top
 *                      left of the matrix
 * Output: Tile* - pointer to tile at the x,y. NULL if unset or out of range
 */
Tile* MapThing::getTilePrevious(uint32_t x, uint32_t y)
{
  if(tile_prev.size() > x && tile_prev[x].size() > y)
    return tile_prev[x][y];
  return NULL;
}

/*
 * Description: Checks if there is data to save for the particular thing. This
 *              is virtualized for all children
 *
 * Inputs: none
 * Output: bool - true if save call will result in text
 */
bool MapThing::isDataToSave()
{
  /* Changed status */
  if(changed)
    return true;

  /* Event status */
  if(event_set.isDataToSave())
    return true;

  /* Startpoint status */
  if(tile_main.size() > 0 && tile_main.front().size() > 0 &&
     tile_main.front().front() != nullptr)
  {
    Tile* ref_tile = tile_main.front().front();
    if(starting_x != ref_tile->getX() || starting_y != ref_tile->getY())
      return true;
  }

  return false;
}

/*
 * Description: Checks if a move is allowed from the current thing main
 *              tile and the next tile that it is trying to move to. Utilizes
 *              the move request inside the thing class to determine where to
 *              move.
 *
 * Inputs: std::vector<std::vector<Tile*>> tile_set - tile set to check against
 *             frame matrix for moving into these tiles
 * Output: bool - returns if the move is allowed.
 */
bool MapThing::isMoveAllowed(std::vector<std::vector<Tile*>> tile_set,
                             Direction move_request)
{
  bool move_allowed = true;
  SpriteMatrix* sprite_set = getMatrix();

  if(tile_set.size() > 0 &&
     tile_set.size() == sprite_set->width() &&
     tile_set.front().size() == sprite_set->height())
  {
    for(uint16_t i = 0; move_allowed && (i < sprite_set->width()); i++)
    {
      for(uint16_t j = 0; move_allowed && (j < sprite_set->height()); j++)
      {
        if(sprite_set->at(i, j) != NULL)
          move_allowed = isTileMoveAllowed(getTileMain(i, j), tile_set[i][j],
                                      sprite_set->at(i, j)->getRenderDepth(),
                                      move_request);
      }
    }

    return move_allowed;
  }

  return false;
}

/*
 * Description: Checks if a move is allowed from the current person main
 *              tile to the next tile that it is trying to move to. This
 *              handles the individual calculations for a single tile; used
 *              by the isMoveAllowed() function.
 *
 * Inputs: Tile* previous - the tile moving from
 *         Tile* next - the next tile moving to
 *         uint8_t render_depth - the rendering depth, in the stack
 *         Direction move_request - the direction moving
 * Output: bool - returns if the move is allowed.
 */
bool MapThing::isTileMoveAllowed(Tile* previous, Tile* next,
                                 uint8_t render_depth, Direction move_request)
{
  bool move_allowed = true;

  /* If the next tile is NULL, move isn't allowed */
  if(next == NULL)
    move_allowed = false;

  /* Check if the thing can move there */
  if(move_allowed)
  {
    if(render_depth == 0)
    {
      if(!previous->getPassabilityExiting(move_request) ||
         !next->getPassabilityEntering(move_request) ||
         next->isThingSet(render_depth))
      {
        move_allowed = false;
      }
    }
    else if(next->getStatus() == Tile::OFF ||
            next->isThingSet(render_depth))
    {
      move_allowed = false;
    }
  }

  return move_allowed;
}

/*
 * Description: Renders additional images or frames on top of the designated
 *              tile. Called from the render call. This is virtualized for use
 *              by all children. MapThing has no additional rendering.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine pointer
 *         Tile* tile - the reference tile to be rendering
 *         int tile_x - the x location in the matrix in tiles
 *         int tile_y - the y location in the matrix in tiles
 *         int render_x - the top x corner of the tile rendering in px
 *         int render_y - the top y corner of the tile rendering in px
 * Output: bool - status if call was successful
 */
bool MapThing::renderAdditional(SDL_Renderer* renderer, Tile* tile,
                                int tile_x, int tile_y,
                                int render_x, int render_y)
{
  (void)renderer;
  (void)tile;
  (void)tile_x;
  (void)tile_y;
  (void)render_x;
  (void)render_y;

  return true;
}

/*
 * Description: Calculates the move amount based on the cycle time and the
 *              speed for how many pixels should be shifted. The calculation
 *              is based on 16ms for 2 pixel at speed 8.
 *
 * Inputs: int cycle_time - the time since the last update call
 * Output: float - the move amount in 0.0 to 1.0 of a pixel width
 */
float MapThing::moveAmount(uint16_t cycle_time)
{
  float move_amount = (cycle_time * getSpeed()) / kMOVE_FACTOR;
  if(move_amount > 1.0)
    move_amount = 1.0;

  return move_amount;
}

/*
 * Description: Move the thing based on the cycle update time. Utilizes the
 *              moveAmount() calculation for determining how much to move.
 *
 * Inputs: int cycle_time - the time since the last update call
 * Output: Floatinate - the delta move amount of x and y
 */
Floatinate MapThing::moveThing(int cycle_time)
{
  float move_amount = moveAmount(cycle_time);
  Floatinate delta_move;

  if(movement == Direction::EAST || movement == Direction::WEST)
  {
    x += move_amount;
    y = 0.0;
    if(x > 1.0)
      x -= 1.0;

    if(movement == Direction::EAST)
      delta_move.x = move_amount;
    else
      delta_move.x = -move_amount;
  }
  else if(movement == Direction::NORTH || movement == Direction::SOUTH)
  {
    x = 0.0;
    y += move_amount;
    if(y > 1.0)
      y -= 1.0;

    if(movement == Direction::SOUTH)
      delta_move.y = move_amount;
    else
      delta_move.y = -move_amount;
  }
  else
  {
    x = 0.0;
    y = 0.0;
  }

  return delta_move;
}

/*
 * Description: Saves the data for the thing. This does not include the thing
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         const bool &save_event - true to save the base event set (thing)
 * Output: none
 */
bool MapThing::saveData(FileHandler* fh, const bool &save_event)
{
  bool success = true;

  /* Property change saves */
  if(changed)
  {
    /* Active */
    fh->writeXmlData("active", isActive());

    /* Active Time */
    fh->writeXmlData("activetime", getActiveRespawn());

    /* Visible */
    fh->writeXmlData("visible", isVisible());
  }

  /* Event Set */
  if(save_event)
    event_set.saveData(fh);

  /* Startpoint */
  if(tile_main.size() > 0 && tile_main.front().size() > 0 &&
     tile_main.front().front() != nullptr)
  {
    Tile* ref_tile = tile_main.front().front();
    if(starting_x != ref_tile->getX() || starting_y != ref_tile->getY())
      fh->writeXmlData("startpoint", std::to_string(ref_tile->getX()) + "," +
                                     std::to_string(ref_tile->getY()));
  }

  return success;
}

/*
 * Description: Sets the direction that the class should be moving in. Once
 *              the direction has been changed, the animation buffer is forced
 *              to occur so that the result will be noticed.
 *
 * Inputs: Direction new_direction - the new movement direction
 * Output: bool - if the direction changed since before the call
 */
bool MapThing::setDirection(Direction new_direction)
{
  bool changed = false;

  /* Check if the direction is changed */
  if(movement != new_direction)
    changed = true;

  /* Update the direction */
  movement = new_direction;

  return changed;
}

/*
 * Description: Sets the base event within the set. Called from setBase() call
 *
 * Inputs: none
 * Output: none
 */
void MapThing::setEventBase()
{
  if(base != nullptr)
    event_set.setBase(&base->event_set);
  else
    event_set.setBase(nullptr);
}

/*
 * Description: Sets the sprite matrix in the base thing class. Warning, this
 *              does not delete existing memory. This is intended for use
 *              by children classes that need to constantly switch the
 *              matrix.
 *
 * Inputs: SpriteMatrix* matrix - the new matrix to use
 * Output: bool - true if the matrix was set
 */
bool MapThing::setMatrix(SpriteMatrix* matrix)
{
  if(matrix != NULL)
  {
    sprite_set = matrix;

    /* Set up the base control variable */
    if(base_control == NULL)
      base_control = new AnimationControl;
    base_control->curr_frame = 0;
    base_control->forward = matrix->isDirectionForward();
    base_control->frame_time = matrix->getAnimationTime();
    base_control->num_frames = matrix->getFrameCount();
    base_control->time = 0;

    matrix->shiftTo(base_control->curr_frame);

    return true;
  }
  return false;
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects. If it fails, it resets the pointers.
 *
 * Inputs: Tile* tile - the tile pointer to set the frame
 *         TileSprite* frames - the sprite frames pointer to set in the tile
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
bool MapThing::setTile(Tile* tile, TileSprite* frames, bool no_events)
{
  (void)no_events;
  uint8_t render_depth = frames->getRenderDepth();

  /* Attempt and set the tile */
  if(tile != NULL)
    return tile->setThing(this, render_depth);

  return false;
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects with regards to finishing a tile move.
 *
 * Inputs: Tile* old_tile - the tile the object was on previously
 *         Tile* new_tile - the tile the object is moving to
 *         uint8_t render_depth - the depth the frame is rendered on this tile
 *         bool reverse_last - if the last start should be reversed
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
void MapThing::setTileFinish(Tile* old_tile, Tile* new_tile,
                             uint8_t render_depth, bool reverse_last,
                             bool no_events)
{
  (void)no_events;

  if(reverse_last)
    new_tile->unsetThing(render_depth);
  else
    old_tile->unsetThing(render_depth);
}

/*
 * Description: Sets the tile in the sprite and sprite in the tile for the
 *              passed in objects with regards to beginning a tile move. If it
 *              fails, it resets the pointers back to the original tile.
 *
 * Inputs: Tile* old_tile - the tile the object was on previously
 *         Tile* new_tile - the tile the object is moving to
 *         uint8_t render_depth - the depth the frame is rendered on this tile
 *         bool no_events - if events should trigger on the set
 * Output: bool - true if the set was successful
 */
bool MapThing::setTileStart(Tile* old_tile, Tile* new_tile,
                            uint8_t render_depth, bool no_events)
{
  (void)no_events;
  (void)old_tile;

  /* Attempt and set the tile */
  if(new_tile != NULL)
    return new_tile->setThing(this, render_depth);

  return false;
}

/*
 * Description: The tile move finish call. To be called after a move and it's
 *              determined that the thing is on the main tile (for the first
 *              time). Essentially just cleans up the previous tile pointer.
 *
 * Inputs: bool no_events - should events trigger on move finish
 * Output: none
 */
void MapThing::tileMoveFinish(bool no_events)
{
  SpriteMatrix* sprite_set = getMatrix();

  for(uint16_t i = 0; i < sprite_set->width(); i++)
  {
    for(uint16_t j = 0; j < sprite_set->height(); j++)
    {
      if(sprite_set->at(i, j) != NULL)
      {
        setTileFinish(tile_prev[i][j], tile_main[i][j],
                      sprite_set->at(i, j)->getRenderDepth(), false, no_events);
      }
    }
  }

  /* Clear out previous tile set */
  tile_prev.clear();
}

/*
 * Description: The tile move initialization call. To be called after
 *              passability checks have passed and the thing can be moved to
 *              the next tile. Sets the new main pointer and moves the current
 *              to the old spot.
 *
 * Inputs: std::vector<std::vector<Tile*>> tile_set - the next set of frames
 *         bool no_events - should events trigger on move?
 * Output: bool - if the tile start was successfully started
 */
bool MapThing::tileMoveStart(std::vector<std::vector<Tile*>> tile_set,
                             bool no_events)
{
  SpriteMatrix* sprite_set = getMatrix();
  bool success = true;
  uint16_t end_i = 0;
  uint16_t end_j = 0;

  /* Data prechecks -> to confirm equivalency */
  if(tile_set.size() > 0 &&
     tile_set.size() == sprite_set->width() &&
     tile_set.back().size() == sprite_set->height())
  {
    /* If moving north or west, parse top down. Otherwise, parse up */
    if(movement == Direction::NORTH || movement == Direction::WEST)
    {
      /* Go through each frame and update */
      for(uint16_t i = 0; success && (i < sprite_set->width()); i++)
      {
        for(uint16_t j = 0; success && (j < sprite_set->height()); j++)
        {
          if(sprite_set->at(i, j) != NULL)
          {
            /* Set the tile start */
            success &= setTileStart(tile_main[i][j], tile_set[i][j],
                                    sprite_set->at(i, j)->getRenderDepth(),
                                    no_events);

            /* If unsuccessful, store how far it parsed */
            if(!success)
            {
              end_i = i;
              end_j = j;
            }
          }
        }
      }
    }
    else
    {
      /* Go through each frame and update */
      for(int16_t i = sprite_set->width() - 1; success && (i >= 0); i--)
      {
        for(int16_t j = sprite_set->height() - 1; success && (j >= 0); j--)
        {
          if(sprite_set->at(i, j) != NULL)
          {
            /* Set the tile start */
            success &= setTileStart(tile_main[i][j], tile_set[i][j],
                                    sprite_set->at(i, j)->getRenderDepth(),
                                    no_events);

            /* If unsuccessful, store how far it parsed */
            if(!success)
            {
              end_i = i;
              end_j = j;
            }
          }
        }
      }
    }

    /* If successful, set tile set. Else, unsuccessful, reverse course */
    if(success)
    {
      tile_prev = tile_main;
      tile_main = tile_set;
    }
    else
    {
      bool finished = false;

      /* If moving north or west, parse top down. Otherwise, parse up */
      if(movement == Direction::NORTH || movement == Direction::WEST)
      {
        for(uint16_t i = 0; !finished && (i < sprite_set->width()); i++)
        {
          for(uint16_t j = 0; !finished && (j < sprite_set->height()); j++)
          {
            if(i == end_i && j == end_j)
              finished = true;
            else if(sprite_set->at(i, j) != NULL)
              setTileFinish(tile_main[i][j], tile_set[i][j],
                            sprite_set->at(i, j)->getRenderDepth(), true, true);
          }
        }
      }
      else
      {
        for(int16_t i = sprite_set->width() - 1; !finished && (i >= 0); i++)
        {
          for(int16_t j = sprite_set->height() - 1; !finished && (j >= 0); j++)
          {
            if(i == end_i && j == end_j)
              finished = true;
            else if(sprite_set->at(i, j) != NULL)
              setTileFinish(tile_main[i][j], tile_set[i][j],
                            sprite_set->at(i, j)->getRenderDepth(), true, true);
          }
        }
      }
    }

    return success;
  }
  return false;
}

/*
 * Description: Unsets the matrix in the class. This removes it without
 *              deleting. Only use if the matrix was set using the
 *              setMatrix() call.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::unsetMatrix()
{
  unsetTiles(true);
  sprite_set = NULL;
}

/*
 * Description: Unsets the tile corresponding to the matrix at the x and y
 *              coordinate. However, since this is an private function, it does
 *              not confirm that the X and Y are in the valid range. Must be
 *              checked or results are unknown. This will unset the thing from
 *              the tile corresponding to the frame and the tile from the frame.
 *
 * Inputs: uint32_t x - the x coordinate of the frame (horizontal)
 *         uint32_t y - the y coordinate of the frame (vertical)
 *         bool no_events - should events trigger with the change?
 * Output: none
 */
void MapThing::unsetTile(uint32_t x, uint32_t y, bool no_events)
{
  (void)no_events;
  SpriteMatrix* sprite_set = getMatrix();
  uint8_t render_depth = sprite_set->at(x, y)->getRenderDepth();

  /* Remove from main tile, if applicable */
  tile_main[x][y]->unsetThing(render_depth);

  /* Remove from previous tile, if applicable */
  if(tile_prev.size() > 0)
    tile_prev[x][y]->unsetThing(render_depth);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds thing information from the XML file. This will be
 *              virtually called by children where the child will deal with
 *              additional sets needed and then pass call to this. Works off
 *              reading the XML file in a strict manner.
 *
 * Inputs: XmlData data - the read XML data
 *         int file_index - the index in the xml data where this detail begins
 *         int section_index - the map section index of the thing
 *         SDL_Renderer* renderer - the graphical rendering engine pointer
 *         std::string base_path - the base path for resources
 *         bool from_save - true if the load is from a save file
 * Output: bool - status if successful
 */
bool MapThing::addThingInformation(XmlData data, int file_index,
                                   int section_index, SDL_Renderer* renderer,
                                   std::string base_path, bool from_save)
{
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);
  bool success = true;

  /* Parse the identifier for setting the thing information */
  /*--------------------- ACTIVE -----------------*/
  if(identifier == "active" && elements.size() == 1)
  {
    setActive(data.getDataBool(&success));
  }
  /*--------------------- ACTIVE TIME -----------------*/
  else if(identifier == "activetime" && elements.size() == 1)
  {
    setActiveRespawn(data.getDataInteger(&success));
  }
  /*--------------------- DESCRIPTION -----------------*/
  else if(identifier == "description" && elements.size() == 1)
  {
    setDescription(data.getDataString(&success));
  }
  /*--------------------- DIALOG IMAGE -----------------*/
  else if(identifier == "image" && elements.size() == 1)
  {
    success &= setDialogImage(base_path + data.getDataString(&success),
                              renderer);
  }
  /*--------------------- EVENT -----------------*/
  else if(identifier == "event")
  {
    event_set.loadData(data, file_index, section_index);
  }
  /*--------------------EVENT SET------------------*/
  else if(identifier == "eventset")
  {
    event_set.loadData(data, file_index + 1, section_index);
  }
  /*------------------- GAME ID -----------------*/
  else if(identifier == "game_id" && elements.size() == 1)
  {
    success &= setGameID(data.getDataInteger(&success));
  }
  /*--------------------- NAME -----------------*/
  else if(identifier == "name" && elements.size() == 1)
  {
    setName(data.getDataString(&success));
  }
  /*----------------- RENDER MATRIX -----------------*/
  else if(identifier == "rendermatrix" && elements.size() == 1)
  {
    if(sprite_set == NULL)
      sprite_set = new SpriteMatrix();
    sprite_set->setRenderMatrix(data.getDataString(&success));
  }
  /*----------------- SOUND ID -----------------*/
  else if(identifier == "sound_id" && elements.size() == 1)
  {
    setSoundID(data.getDataInteger(&success));
  }
  /*--------------------- SPEED -----------------*/
  else if(identifier == "speed" && elements.size() == 1)
  {
    setSpeed(data.getDataInteger(&success));
  }
  /*--------------------- SPRITE DATA -----------------*/
  else if(identifier == "sprites")
  {
    if(sprite_set == NULL)
      sprite_set = new SpriteMatrix();
    success &= sprite_set->addFileInformation(data, file_index + 1, renderer,
                                              base_path);
  }
  /*--------------------- STARTPOINT -----------------*/
  else if(identifier == "startpoint")
  {
    std::vector<std::string> points = Helpers::split(data.getDataString(), ',');
    if(points.size() == 2) /* There needs to be an x and y point */
      setStartingLocation(section_index, std::stoul(points[0]),
                                         std::stoul(points[1]));
    else
      success = false;
  }
  /*--------------------- VISIBILITY -----------------*/
  else if(identifier == "visible")
  {
    setVisibility(data.getDataBool(&success));
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
ThingBase MapThing::classDescriptor()
{
  //return "MapThing";
  return ThingBase::THING;
}

/*
 * Description: Takes the frame matrix, as it's been set up and removes any
 *              rows or columns at the end that have no valid frames set. A
 *              frame is classified as valid if it's not NULL and has renderable
 *              frames stored within it.
 *
 * Inputs: bool first_call - first time calling - true by default
 * Output: bool - true if clean validated frame data
 */
bool MapThing::cleanMatrix(bool first_call)
{
  (void)first_call;

  if(sprite_set != NULL)
  {
    unsetTiles(true);
    sprite_set->cleanMatrix();

    return true;
  }
  return false;
}

/*
 * Description: Clears out all information stored in the class
 *
 * Inputs: none
 * Output: none
 */
void MapThing::clear()
{
  /* Unset tiles before proceeding to any class parameters */
  unsetTiles(true);

  /* Resets the class parameters */
  active = true;
  active_lapsed = 0;
  active_time = kACTIVE_DEFAULT;
  base = NULL;
  base_category = ThingBase::ISBASE;
  if(base_control != NULL)
    delete base_control;
  base_control = NULL;
  changed = false;
  MapThing::clearAllMovement();
  setDescription("");
  setEventHandler(NULL);
  event_set.clear();
  setGameID(kUNSET_ID);
  setID(kUNSET_ID);
  setMovementPaused(false);
  setName("");
  setSpeed(kDEFAULT_SPEED);
  target = NULL;
  visible = true;

  unsetFrames();
  resetLocation();
}

/*
 * Description: Clears all active movement on the thing. Since the thing is
 *              not movable, this is merely virtual and useful for other calls.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::clearAllMovement()
{
  movement = Direction::DIRECTIONLESS;
}

/*
 * Description: Clears the given target from this class. Once cleared, the
 *              movement being paused will be removed to allow for continuing
 *              movement. The target is another MapThing.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::clearTarget()
{
  if(target != NULL)
    setMovementPaused(false);
  target = NULL;
}

/*
 * Description: Returns the time in milliseconds that must lapse when a thing
 *              is inactive prior to it becoming active.
 *
 * Inputs: none
 * Output: int - the time in milliseconds
 */
int MapThing::getActiveRespawn() const
{
  return active_time;
}

/*
 * Description: Returns the base for the class. If null, this class is a base.
 *
 * Inputs: none
 * Output: MapThing* - the reference pointer for the base class. NULL if unset
 */
MapThing* MapThing::getBase()
{
  return base;
}

/*
 * Description: Returns the bounding box for the entire object of the thing.
 *              This bounding box is in Tile units, void of any tile width
 *              or height considerations.
 *
 * Inputs: none
 * Output: SDL_Rect - rect definition of bounding box with top left x and y as
 *                    well as width and height.
 */
SDL_Rect MapThing::getBoundingBox()
{
  SDL_Rect rect;
  SpriteMatrix* sprite_set = getMatrix();

  rect.x = getTileX();
  rect.y = getTileY();
  if(sprite_set != NULL)
  {
    rect.w = sprite_set->width();
    rect.h = sprite_set->height();
  }
  else
  {
    rect.w = 0;
    rect.h = 0;
  }

  return rect;
}

/*
 * Description: Returns the bounding box for the entire object of the thing.
 *              This bounding box is in pixel units, based on the tile width
 *              and height.
 *
 * Inputs: none
 * Output: SDL_Rect - rect definition of bounding box with top left x and y as
 *                    well as width and height.
 */
SDL_Rect MapThing::getBoundingPixels()
{
  SDL_Rect rect;
  SpriteMatrix* sprite_set = getMatrix();

  rect.x = getX();
  rect.y = getY();
  if(sprite_set != NULL)
  {
    rect.w = sprite_set->width() * getTileWidth();
    rect.h = sprite_set->height() * getTileHeight();
  }
  else
  {
    rect.w = 0;
    rect.h = 0;
  }

  return rect;
}

/*
 * Description: Returns the pixel location of the center X of the thing
 *
 * Inputs: none
 * Output: uint32_t - pixel location output of X
 */
uint32_t MapThing::getCenterX()
{
  return (getX() + (getWidth() * getTileWidth()) / 2.0);
}

/*
 * Description: Returns the pixel location of the center Y of the thing
 *
 * Inputs: none
 * Output: uint32_t - pixel location output of Y
 */
uint32_t MapThing::getCenterY()
{
  return (getY() + (getHeight() * getTileHeight()) / 2.0);
}

/*
 * Description: Gets the things description.
 *
 * Inputs: none
 * Output: std::string - the description for the thing, as a string
 */
std::string MapThing::getDescription() const
{
  if(description != "")
    return description;
  else if(base != NULL)
    return base->getDescription();
  return description;
}

/*
 * Description: Gets the dialog image data, that is used in conversational
 *              interaction painting.
 *
 * Inputs: none
 * Output: Frame* - the dialog image data
 */
Frame* MapThing::getDialogImage()
{
  if(base != NULL)
    return base->getDialogImage();
  return &dialog_image;
}

/*
 * Description: Returns a reference of the event set being stored within the
 *              map thing object.
 *
 * Inputs: none
 * Output: EventSet* - the event object reference pointer
 */
EventSet* MapThing::getEventSet()
{
  return &event_set;
}

/*
 * Description: Gets the frame data for the thing at a particular coordinate.
 *              Coordinates are with respect to the top left of the sprite.
 *              If out of range or the frame isn't set, it will return NULL.
 *
 * Inputs: uint32_t x - x coordinate in matrix
 *         uint32_t y - y coordinate in matrix
 * Output: TileSprite* - the pointer to the data for the frame. Do not delete
 *                       since it would seg fault the game.
 */
TileSprite* MapThing::getFrame(uint32_t x, uint32_t y)
{
  TileSprite* found_sprite = NULL;
  SpriteMatrix* sprite_set = getMatrix();

  if(sprite_set != NULL)
    found_sprite = sprite_set->getSprite(x, y);

  return found_sprite;
}

/*
 * Description: Returns the frame that corresponds to the main tile passed in.
 *              The main tile is the tile the object is moving to, as opposed
 *              to moving from. If it's not moving, it will be rendering on
 *              the main tile.
 *
 * Inputs: Tile* tile - the tile to get the corresponding frame from
 * Output: TileSprite* - tile sprite pointer, on said tile
 */
TileSprite* MapThing::getFrameMain(Tile* tile)
{
  TileSprite* selected = NULL;

  if(isTilesSet() && tile != NULL)
  {
    /* Get tile data */
    uint16_t tile_x = tile->getX() - tile_main.front().front()->getX();
    uint16_t tile_y = tile->getY() - tile_main.front().front()->getY();

    if(getTileMain(tile_x, tile_y) == tile)
      selected = getMatrix()->getSprite(tile_x, tile_y);
  }

  return selected;
}

/*
 * Description: Returns the frame that corresponds to the previous tile passed
 *              in. The previous tile is the tile the object is moving from, as
 *              opposed to moving to. The previous tile is only set if the
 *              object is moving.
 *
 * Inputs: Tile* tile - the tile to get the corresponding frame from
 * Output: TileSprite* - tile sprite pointer, on said tile
 */
TileSprite* MapThing::getFramePrevious(Tile* tile)
{
  TileSprite* selected = NULL;

  if(tile_prev.size() > 0 && tile != NULL)
  {
    /* Get tile data */
    uint16_t tile_x = tile->getX() - tile_prev.front().front()->getX();
    uint16_t tile_y = tile->getY() - tile_prev.front().front()->getY();

    if(getTilePrevious(tile_x, tile_y) == tile)
      selected = getMatrix()->getSprite(tile_x, tile_y);
  }

  return selected;
}

/*
 * Description: Gets all the frame data for the thing, in vector form.
 *              Coordinates are with respect to the top left of the sprite and
 *              follow the form [x][y]. If there are none set, the matrix will
 *              be empty.
 *
 * Inputs: none
 * Output: std::vector<std::vector<TileSprite*>> - the matrix of frame pointers
 *             as set in the class. Do not delete any individual frame since it
 *             would seg fault the game.
 */
std::vector<std::vector<TileSprite*>> MapThing::getFrames()
{
  std::vector<std::vector<TileSprite*>> set;
  SpriteMatrix* sprite_set = getMatrix();

  if(sprite_set != NULL)
    set = sprite_set->getMatrix();

  return set;
}

/*
 * Description: Returns the connected game object ID. The connected object is
 *              stored at the top level.
 *
 * Inputs: none
 * Output: int - the connected ID
 */
int MapThing::getGameID() const
{
  if(base != NULL && game_id < 0)
    return base->getGameID();
  return game_id;
}

/*
 * Description: Gets the height of the internal tile.
 *
 * Inputs: none
 * Output: uint16_t - the height, in pixels
 */
uint16_t MapThing::getHeight()
{
  SpriteMatrix* sprite_set = getMatrix();

  if(sprite_set != NULL)
    return sprite_set->height();

  return 0;
}

/*
 * Description: Gets the things ID. If the ID is not set, it will be an
 *              out of range number (-1)
 *
 * Inputs: none
 * Output: int - the ID for the thing, as an integer
 */
int MapThing::getID() const
{
  return id;
}

/*
 * Description: Returns the interaction event that fires upon an
 * interact()
 *              call to this thing.
 *
 * Inputs: none
 * Output: Event - the event fired
 */
Event MapThing::getInteraction()
{
  if(!event_set.isEmpty())
  {
    return event_set.getEvent(false);
  }
  else if(base != nullptr)
  {
    return base->getInteraction();
  }
  return event_set.getEvent(false);
}

/*
 * Description: Gets the map section attributed to the tile(s) that the thing
 *              currently resides on. It will default to 0 (main) if unset.
 *
 * Inputs: none
 * Output: uint16_t - the section ID, based on the map
 */
uint16_t MapThing::getMapSection()
{
  if(tile_main.size() > 0)
    return tile_section;
  return starting_section;
}

/*
 * Description: Returns the movement that the class is set to. This indicates
 *              the current direction that it will shift in when an update
 *              call is selected
 *
 * Inputs: none
 * Output: Direction - the movezment direction indicator
 */
Direction MapThing::getMovement()
{
  return movement;
}

/*
 * Description: Returns if the thing movement has been paused.
 *
 * Inputs: none
 * Output: bool - the movement paused state
 */
bool MapThing::getMovementPaused()
{
  return movement_paused;
}

/*
 * Description: This is a virtual move request call that needs to be
 *              reimplemented by whoever needs it. It returns what the current
 *              move request is.
 *
 * Inputs: none
 * Output: Direction - the move request direction indicator
 */
Direction MapThing::getMoveRequest()
{
  return Direction::DIRECTIONLESS;
}

/*
 * Description: Returns the move X, as a float 0 to 1 with respect to the width.
 *              This value will also reflect the moving direction (if EAST, the
 *              number will be positive; otherwise, if WEST, it will be
 *              negative.
 *
 * Inputs: none
 * Output: float - the float value of movement between tiles [-1 to +1]
 */
float MapThing::getMoveX()
{
  if(movement == Direction::EAST)
    return x;
  else if(movement == Direction::WEST)
    return -x;
  return 0.0;
}

/*
 * Description: Returns the move Y, as a float 0 to 1 with respect to the
 *              height. This value will also reflect the moving direction (if
 *              SOUTH, thenumber will be positive; otherwise, if NORTH, it will
 *              be negative.
 *
 * Inputs: none
 * Output: float - the float value of movement between tiles [-1 to +1]
 */
float MapThing::getMoveY()
{
  if(movement == Direction::SOUTH)
    return y;
  else if(movement == Direction::NORTH)
    return -y;
  return 0.0;
}

/*
 * Description: Gets the things name.
 *
 * Inputs: none
 * Output: std::string - the name for the thing, as a string
 */
std::string MapThing::getName() const
{
  if(name != "")
    return name;
  else if(base != NULL)
    return base->getName();
  return name;
}

/*
 * Description: Gets if the thing is passable entering from the given
 *              direction and with the frame at the given tile.
 *
 * Inputs: Tile* frame_tile - the tile which contains the relevant moving frame
 *         Direction dir - the direction enumerated for passability
 * Output: bool - status if the tile passability is possible.
 */
bool MapThing::getPassabilityEntering(Tile* frame_tile, Direction dir)
{
  if(dir == Direction::NORTH)
    return getPassabilityExiting(frame_tile, Direction::SOUTH);
  else if(dir == Direction::EAST)
    return getPassabilityExiting(frame_tile, Direction::WEST);
  else if(dir == Direction::SOUTH)
    return getPassabilityExiting(frame_tile, Direction::NORTH);
  else if(dir == Direction::WEST)
    return getPassabilityExiting(frame_tile, Direction::EAST);
  return getPassabilityExiting(frame_tile, dir);
}

/*
 * Description: Gets if the thing is passable exiting from the given
 *              direction and with the frame at the given tile.
 *
 * Inputs: Tile* frame_tile - the tile which contains the relevant moving frame
 *         Direction dir - the direction enumerated for passability
 * Output: bool - status if the tile passability is possible.
 */
bool MapThing::getPassabilityExiting(Tile* frame_tile, Direction dir)
{
  TileSprite* frame = getFrameMain(frame_tile);
  if(frame != NULL)
    return frame->getPassability(dir);
  return true;
}

/*
 * Description: Returns the reference sound ID. If less than 0, it is unset.
 *
 * Inputs: none
 * Output: int32_t - the sound ID
 */
int32_t MapThing::getSoundID() const
{
  if(base != nullptr)
    return base->getSoundID();
  return sound_id;
}

/*
 * Description: Returns the speed that the thing is moving in. Default is 150.
 *
 * Inputs: none
 * Output: uint16_t - the speed integer movement indicator
 */
uint16_t MapThing::getSpeed() const
{
  if(base != NULL)
  {
    if(speed != base->getSpeed())
      return speed;
    return base->getSpeed();
  }
  return speed;
}

/*
 * Description: Returns the section of map the thing started in, as set by
 *              setStartingLocation(*).
 *
 * Inputs: none
 * Output: uint16_t - the starting section integer
 */
uint16_t MapThing::getStartingSection()
{
  return starting_section;
}

/*
 * Description: Returns the x coordinate, in tile units, of where the thing
 *              started, as set by setStartingLocation(*).
 *
 * Inputs: none
 * Output: uint16_t - the start x integer
 */
uint16_t MapThing::getStartingX()
{
  return starting_x;
}

/*
 * Description: Returns the y coordinate, in tile units, of where the thing
 *              started, as set by setStartingLocation(*).
 *
 * Inputs: none
 * Output: uint16_t - the start y integer
 */
uint16_t MapThing::getStartingY()
{
  return starting_y;
}

/*
 * Description: Returns the map thing target that this class is targetted at.
 *              Do not delete this pointer as that would cause errors.
 *
 * Inputs: none
 * Output: MapThing* - the pointer to the targetted thing
 */
MapThing* MapThing::getTarget()
{
  return target;
}

/*
 * Description: Returns the tile height, of the tiles the thing is stored and
 *              rendering on.
 *
 * Inputs: none
 * Output: uint16_t - tile height
 */
uint16_t MapThing::getTileHeight()
{
  if(tile_main.size() > 0)
    return tile_main.front().front()->getHeight();
  return 0;
}

/*
 * Description: Returns the tiles the thing is stored on based on the render
 *              depth. The matrix is the same size as the thing and the tiles
 *              that don't match the rendering depth are NULL.
 *
 * Inputs: uint8_t render_depth - the rendering depth of the person frame(s)
 * Output: std::vector<std::vector<Tile*>> - 2d matrix of tiles
 */
std::vector<std::vector<Tile*>> MapThing::getTileRender(uint8_t render_depth)
{
  std::vector<std::vector<Tile*>> tile_set;

  if(isTilesSet())
  {
    /* Choose the sprite matrix before proceeding */
    SpriteMatrix* sprite_set = getMatrix();
    tile_set = tile_main;

    /* Go through all tiles */
    for(uint16_t i = 0; i < tile_set.size(); i++)
    {
      for(uint16_t j = 0; j < tile_set[i].size(); j++)
      {
        if(sprite_set->at(i, j) == NULL ||
           sprite_set->at(i, j)->getRenderDepth() != render_depth)
          tile_set[i][j] = NULL;
      }
    }
  }

  return tile_set;
}

/*
 * Description: Returns the tile width, of the tiles the thing is stored and
 *              rendering on.
 *
 * Inputs: none
 * Output: uint16_t - tile width
 */
uint16_t MapThing::getTileWidth()
{
  if(tile_main.size() > 0)
    return tile_main.front().front()->getWidth();
  return 0;
}

/*
 * Description: Returns the tile based X of the top left of the bounding box
 *              the thing. If the object is moving, this coordinate will be
 *              where the tile will be, as opposed to where it was.
 *
 * Inputs: bool previous - true to access previous tile instead of main.
 *                         If there is no previous, gets main. default false
 * Output: uint16_t - X tile coordinate
 */
uint16_t MapThing::getTileX(bool previous)
{
  if(previous && tile_prev.size() > 0)
    return tile_prev.front().front()->getX();
  else if(tile_main.size() > 0)
    return tile_main.front().front()->getX();
  return starting_x;
}

/*
 * Description: Returns the tile based Y of the top left of the bounding box
 *              the thing. If the object is moving, this coordinate will be
 *              where the tile will be, as opposed to where it was.
 *
 * Inputs: bool previous - true to access previous tile instead of main.
 *                         If there is no previous, gets main. default false
 * Output: uint16_t - Y tile coordinate
 */
uint16_t MapThing::getTileY(bool previous)
{
  if(previous && tile_prev.size() > 0)
    return tile_prev.front().front()->getY();
  else if(tile_main.size() > 0)
    return tile_main.front().front()->getY();
  return starting_y;
}

/*
 * Description: Gets the width of the internal tile.
 *
 * Inputs: none
 * Output: uint16_t - the width, in pixels
 */
uint16_t MapThing::getWidth()
{
  SpriteMatrix* sprite_set = getMatrix();

  if(sprite_set != NULL)
    return sprite_set->width();

  return 0;
}

/*
 * Description: Returns the top left X coordinate of the thing
 *
 * Inputs: none
 * Output: uint32_t - the X coordinate, in pixels, of the top left corner
 */
uint32_t MapThing::getX()
{
  if(tile_prev.size() > 0)
  {
    Tile* top_left = tile_prev.front().front();

    if(movement == Direction::EAST)
      return (top_left->getX() + x) * top_left->getWidth();
    else if(movement == Direction::WEST)
      return (top_left->getX() - x) * top_left->getWidth();
    return top_left->getPixelX();
  }
  else if(tile_main.size() > 0)
  {
    return tile_main.front().front()->getPixelX();
  }
  return 0;
}

/*
 * Description: Returns the top left Y coordinate of the thing
 *
 * Inputs: none
 * Output: uint32_t - the Y coordinate, in pixels, of the top left corner
 */
uint32_t MapThing::getY()
{
  if(tile_prev.size() > 0)
  {
    Tile* top_left = tile_prev.front().front();

    if(movement == Direction::SOUTH)
      return (top_left->getY() + y) * top_left->getHeight();
    else if(movement == Direction::NORTH)
      return (top_left->getY() - y) * top_left->getHeight();
    return top_left->getPixelY();
  }
  else if(tile_main.size() > 0)
  {
    return tile_main.front().front()->getPixelY();
  }
  return 0;
}

/*
 * Description: The generic interact call. This is what fires when a player
 *              presses a use key and then this searches for if an event is
 *              available and starts it.
 *
 * Inputs: MapPerson* initiator - the pointer to the person that initiated it
 * Output: bool - if the call can occur (Event handler needs to be set)
 */
bool MapThing::interact(MapPerson* initiator)
{
  if(event_handler != NULL && !getEventSet()->isNoInteraction())
  {
    event_handler->executeEventSet(getEventSet(), initiator, this);
    return true;
  }

  return false;
}

/*
 * Description: Checks if the thing is currently active.
 *
 * Inputs: none
 * Output: bool - true if the thing is active within the game
 */
bool MapThing::isActive() const
{
  return active;
}

/*
 * Description: Is the thing almost on the tile (less than 1 pulse away).
 *              This is used to check movement limits to allow the update
 *              cycle to know when passability needs to be checked or when
 *              to stop the man from moving.
 *
 * Inputs: int cycle_time - time since the last update call (ms)
 * Output: bool - is the thing almost on the tile?
 */
bool MapThing::isAlmostOnTile(int cycle_time)
{
  if(x > 0.0)
  {
    return (moveAmount(cycle_time) > (1.0 - x));
  }
  else if(y > 0.0)
  {
    return (moveAmount(cycle_time) > (1.0 - y));
  }

  return false;
}

/*
 * Description: Returns if a move is currently in the request queue.
 *
 * Inputs: none
 * Output: bool - true if there is a move request.
 */
bool MapThing::isMoveRequested()
{
  return false;
}

/*
 * Description: Returns if the thing is currently moving.
 *
 * Inputs: none
 * Output: bool - movement status
 */
bool MapThing::isMoving()
{
  return (movement != Direction::DIRECTIONLESS);
}

/*
 * Description: Returns if the thing is currently centered on a tile
 *              (approximately).
 *
 * Inputs: none
 * Output: bool - tile centered status
 */
bool MapThing::isOnTile()
{
  if(tile_main.size() > 0)
  {
    if(movement == Direction::NORTH || movement == Direction::SOUTH)
      return (x == 1.0);
    else if(movement == Direction::EAST || movement == Direction::WEST)
      return (y == 1.0);
    return (x == 0.0) && (y == 0.0);
  }

  return false;
}

/*
 * Description: Returns if the tiles have been set, that associate to where the
 *              thing is being rendered to.
 *
 * Inputs: none
 * Output: bool - true if the tiles have been set
 */
bool MapThing::isTilesSet()
{
  return (tile_main.size() > 0);
}

/*
 * Description: Returns if the thing is visible for rendering.
 *
 * Inputs: none
 * Output: bool - visibility status
 */
bool MapThing::isVisible() const
{
  //bool visible = this->visible; // TODO: Delete
  //if(base != NULL)
  //  visible &= base->visible;

  return visible;
}

/*
 * Description: Render the entire thing object, at it's designated coordinate
 *              and with the offset passed by the viewport. This does no
 *              rendering with rendering depths and just renders the object
 *              flat.
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine
 *         int offset_x - the X viewport offset
 *         int offset_y - the Y viewport offset
 * Output: bool - true if the object rendered
 */
bool MapThing::render(SDL_Renderer* renderer, int offset_x, int offset_y)
{
  if(isTilesSet() && isActive() && isVisible())
  {
    int render_x = getX() - offset_x;
    int render_y = getY() - offset_y;
    bool success = true;

    /* Attempt render */
    if(base_category >= ThingBase::PERSON)
      success =  getMatrix()->render(base_control->curr_frame, renderer,
                                     render_x, render_y,
                                     tile_main.front().front()->getWidth(),
                                     tile_main.front().front()->getHeight());
    else
      success = getMatrix()->render(renderer, render_x, render_y,
                                 tile_main.front().front()->getWidth(),
                                 tile_main.front().front()->getHeight());

    /* If successful, render additional */
    if(success)
    {
      renderAdditional(renderer, tile_main.front().front(),
                       0, 0, render_x, render_y);
      return success;
    }
  }

  return false;
}


/*
 * Description: Render the single frame, located on the designated tile.
 *              This frame will correspond to a given rendering depth.
 *              The offset is passed in from the viewport (using the main tile).
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine
 *         Tile* tile - the tile to render the corresponding thing frame on
 *         uint8_t render_depth - the render depth of the indicated frame
 *         int offset_x - the X viewport offset
 *         int offset_y - the Y viewport offset
 * Output: bool - true if the object rendered
 */
bool MapThing::renderMain(SDL_Renderer* renderer, Tile* tile,
                          uint8_t render_depth, int offset_x, int offset_y)
{
  if(isActive() && isVisible())
  {
    TileSprite* render_frame = getFrameMain(tile);

    /* If frame is valid and visible, render */
    if(render_frame != NULL && render_frame->getRenderDepth() == render_depth)
    {
      /* Make sure the render sprite is on the right frame */
      if(base_category >= ThingBase::PERSON)
        getMatrix()->shiftTo(base_control->curr_frame);

      int tile_x = tile->getX() - tile_main.front().front()->getX();
      int tile_y = tile->getY() - tile_main.front().front()->getY();
      int render_x = (tile_x + getFloatTileX()) * tile->getWidth() - offset_x;
      int render_y = (tile_y + getFloatTileY()) * tile->getHeight() - offset_y;

      /* Render */
      if(render_frame->render(renderer, render_x, render_y,
                              tile->getWidth(), tile->getHeight()))
      {
        renderAdditional(renderer, tile, tile_x, tile_y, render_x, render_y);
        return true;
      }
    }
  }

  return false;
}

/*
 * Description: Render the single frame, located on the designated tile.
 *              This frame will correspond to a given rendering depth.
 *              The offset is passed in from the viewport (using the prev tile).
 *
 * Inputs: SDL_Renderer* renderer - the rendering engine
 *         Tile* tile - the tile to render the corresponding thing frame on
 *         uint8_t render_depth - the render depth of the indicated frame
 *         int offset_x - the X viewport offset
 *         int offset_y - the Y viewport offset
 * Output: bool - true if the object rendered
 */
bool MapThing::renderPrevious(SDL_Renderer* renderer, Tile* tile,
                              uint8_t render_depth, int offset_x, int offset_y)
{
  if(isActive() && isVisible())
  {
    TileSprite* render_frame = getFramePrevious(tile);

    /* If frame is valid and visible, render */
    if(render_frame != NULL && render_frame->getRenderDepth() == render_depth)
    {
      /* Make sure the render sprite is on the right frame */
      if(base_category >= ThingBase::PERSON)
        getMatrix()->shiftTo(base_control->curr_frame);

      int tile_x = tile->getX() - tile_prev.front().front()->getX();
      int tile_y = tile->getY() - tile_prev.front().front()->getY();
      int render_x = (tile_x + getFloatTileX()) * tile->getWidth() - offset_x;
      int render_y = (tile_y + getFloatTileY()) * tile->getHeight() - offset_y;

      /* Render */
      if(render_frame->render(renderer, render_x, render_y,
                              tile->getWidth(), tile->getHeight()))
      {
        renderAdditional(renderer, tile, tile_x, tile_y, render_x, render_y);
        return true;
      }
    }
  }

  return false;
}

/*
 * Description: Resets the rendering location of the thing. This will also
 *              remove all association of tiles and then cleans up the rendering
 *              coordinate back to default (0).
 *
 * Inputs: none
 * Output: none
 */
void MapThing::resetLocation()
{
  /* Unset the tiles, prior to unsetting the point */
  unsetTiles(true);

  /* Clean up the point variables */
  starting_section = 0;
  starting_x = 0;
  starting_y = 0;
  x = 0.0;
  y = 0.0;
}

/*
 * Description: Saves the thing data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 * Output: bool - true if the save was successful
 */
bool MapThing::save(FileHandler* fh)
{
  if(fh != nullptr && isDataToSave())
  {
    bool success = true;

    fh->writeXmlElement("mapthing", "id", getID());
    success &= saveData(fh);
    fh->writeXmlElementEnd();

    return success;
  }
  return false;
}

/*
 * Description: Sets if the thing is active and usable within the space
 *
 * Inputs: bool active - true if the thing should be active. false otherwise
 *         bool set_tiles - true to set/unset tiles as well. default true.
 * Output: bool - returns if the thing is active
 */
bool MapThing::setActive(bool active, bool set_tiles)
{
  /* Check if the active status is changing and update accordingly */
  if(this->active != active)
    changed = true;

  /* Change value */
  this->active = active;
  active_lapsed = 0;

  /* Update thing placement */
  if(set_tiles)
  {
    if(active)  // TODO: Implement fade instead of instant
    {
      this->active = setStartingTiles(starting_tiles, starting_section, true,
                                      false, true);
    }
    else
    {
      unsetTiles(true);
    }
  }

  return this->active;
}

/*
 * Description: Sets the time once a thing is inactive before it will respawn
 *              on the map (assuming no thing is placed in its starting tile
 *              location). If set less than 0, it will never respawn (default).
 *
 * Inputs: int time - the time in milliseconds before respawning the thing
 * Output: none
 */
void MapThing::setActiveRespawn(int time)
{
  int old_time = active_time;

  /* Set the active time */
  if(time < 0)
    active_time = kACTIVE_DEFAULT;
  else
    active_time = time;

  /* Check if it was changed */
  if(old_time != active_time)
    changed = true;
}

/*
 * Description: Sets the base thing class. If set, the primary data will be set
 *              from this with only location and movement handled by this class.
 *
 * Inputs: MapThing* base - the reference base class
 * Output: bool - true if the base was set
 */
bool MapThing::setBase(MapThing* base)
{
  bool success = false;

  if(classDescriptor() == ThingBase::THING)
  {
    if(base != NULL && base->classDescriptor() == ThingBase::THING)
    {
      this->base = base;
      base_category = ThingBase::THING;
      setVisibility(base->isVisible());
      success = true;
    }
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
 * Description: Sets the description that defines the thing.
 *
 * Inputs: std::string new_description - the data for the new description
 * Output: none
 */
void MapThing::setDescription(std::string new_description)
{
  description = new_description;
}

/*
 * Description: Sets the dialog image internal to the map thing. This will be
 *              the image displayed if the particular map thing is involved
 *              in a conversational interaction throughout the map scene
 *
 * Inputs: std::string path - the path to the image data.
 *         SDL_Renderer* renderer - the rendering graphical engine
 * Output: bool - If the path is invalid, this set will fail
 */
bool MapThing::setDialogImage(std::string path, SDL_Renderer* renderer)
{
  return dialog_image.setTexture(path, renderer);
}

/*
 * Description: Sets the event handler to create and manage all existing events
 *              that get fired throughout interaction with the class. This is
 *              necessary to ensure that any events work.
 *
 * Inputs: EventHandler* event_handler - the new handler pointer (must not be 0)
 * Output: none
 */
void MapThing::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;
}

/*
 * Description: Sets a single frame sequence in the matrix of the rendering
 *              thing.
 *
 * Inputs: TileSprite* frame - the new frame to insert into the Map Thing
 *                             matrix. Must actually have a sprite set.
 *         uint32_t x - the x coordinate, relative to the top left of the
 *                      Map Thing render matrix.
 *         uint32_t y - the y coordinate, relative to the top left of the
 *                      Map Thing render matrix.
 *         bool unset_old - delete the old frames from memory?
 * Output: bool - returns if the new frame was set successfully
 */
bool MapThing::setFrame(TileSprite* frame, uint32_t x, uint32_t y,
                        bool delete_old)
{
  /* Make sure the matrix class is initialized */
  if(sprite_set == NULL)
    sprite_set = new SpriteMatrix();

  /* Proceed to set up the frame */
  if(frame != NULL)
  {
    unsetFrame(x, y, delete_old);
    sprite_set->setSprite(frame, x, y, delete_old);
    return true;
  }
  return false;
}

/*
 * Description: Sets the frames that defines the thing
 *
 * Inputs: std::vector<std::vector<TileSprite*>> frames - the new matrix of
 *             frames to define to insert into the thing
 *         bool delete_old - delete the old frames from memory?
 * Output: none
 */
void MapThing::setFrames(std::vector<std::vector<TileSprite*>> frames,
                         bool delete_old)
{
  /* Unset the existing frames */
  unsetFrames(delete_old);

  if(sprite_set == NULL)
    sprite_set = new SpriteMatrix();

  /* Proceed to set up the frame set */
  sprite_set->setSprites(frames, delete_old);
}

/*
 * Description: Sets the connected game object ID for the thing. If out of the
 *              allowable range, the value is set to an minimum storage value.
 *              (Out of range)
 *
 * Inputs: int new_id - the new integer ID to define the game object ID
 * Output: bool - status if the new id is within the allowable range.
 */
bool MapThing::setGameID(int new_id)
{
  /* If the ID is out of range */
  if(new_id < 0)
  {
    game_id = kUNSET_ID;
    return false;
  }

  /* Otherwise, the ID is good */
  game_id = new_id;
  return true;
}

/*
 * Description: Sets the ID for the thing. If out of the allowable range,
 *              the value is set to an minimum storage value. (Out of range)
 *
 * Inputs: int new_id - the new integer ID to define the thing
 * Output: bool - status if the new_id is within the allowable range.
 */
bool MapThing::setID(int new_id)
{
  /* If the ID is out of range */
  if(new_id < 0)
  {
    id = kUNSET_ID;
    return false;
  }

  /* Otherwise, the ID is good */
  id = new_id;

  return true;
}

/*
 * Description: Sets the player ID for the thing. This is a constant and used
 *              for referencing during actions throughout.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::setIDPlayer()
{
  id = kPLAYER_ID;
}

/*
 * Description: Sets if the class should be paused. When it's paused, it will
 *              finish walking to the tile and then stop there until unpaused.
 *
 * Inputs: bool paused - if the movement is paused status
 * Output: none
 */
void MapThing::setMovementPaused(bool paused)
{
  movement_paused = paused;
}

/*
 * Description: Sets the name for the thing.
 *
 * Inputs: std::string new_name - the new name for the thing
 * Output: none
 */
void MapThing::setName(std::string new_name)
{
  name = new_name;
}

/*
 * Description: Sets the reference sound ID. If less than 0, unsets it.
 *
 * Inputs: int32_t id - the new sound reference id
 * Output: none
 */
void MapThing::setSoundID(int32_t id)
{
  if(id < 0)
    sound_id = kUNSET_ID;
  else
    sound_id = id;
}

/*
 * Description: Sets the speed of the thing that it is moving in. Default is
 *              150 and gets set if the speed is invalid (less than 0).
 *
 * Inputs: uint16_t speed - the speed to set the thing movement to
 * Output: none
 */
void MapThing::setSpeed(uint16_t speed)
{
  this->speed = speed;
}

/*
 * Description: Sets the starting location which needs the map section id and
 *              an x and y coordinate.
 *
 * Inputs: uint16_t section_id - the map section id
 *         uint16_t x - the x coordinate of the thing
 *         uint16_t y - the y coordinate of the thing
 * Output: none
 */
void MapThing::setStartingLocation(uint16_t section_id, uint16_t x, uint16_t y)
{
  /* Unset the tiles, currently in use */
  resetLocation();

  /* Set the new tile coordinate */
  starting_section = section_id;
  starting_x = x;
  starting_y = y;
}

/*
 * Description: Sets the starting tiles, for rendering the thing. This tile
 *              set needs to be equal to the size of the bounding box and
 *              each corresponding frame will be set to the tile. Will fail
 *              if a thing is already set up in the corresponding spot.
 *
 * Inputs: std::vector<std::vector<Tile*>> tile_set - the tile matrix
 *         uint16_t section - map section corresponding to tiles
 *         bool no_events - if no events should occur from setting the thing
 *         bool just_store - true to just store it as the starting tiles and
 *                           not call setTile(). Default false
 *         bool avoid_player - do not place if player is on location
 * Output: bool - true if the tiles are set
 */
bool MapThing::setStartingTiles(std::vector<std::vector<Tile*>> tile_set,
                                uint16_t section, bool no_events,
                                bool just_store, bool avoid_player)
{
  SpriteMatrix* sprite_set = getMatrix();
  bool success = true;

  if(sprite_set != nullptr && tile_set.size() > 0 &&
     tile_set.size() == sprite_set->width() &&
     tile_set.back().size() == sprite_set->height())
  {
    /* First, unset all tiles */
    unsetTiles(no_events);
    uint32_t end_x = 0;
    uint32_t end_y = 0;

    /* Check if the thing can be placed */
    if(getID() != kPLAYER_ID)
      for(uint32_t i = 0; i < sprite_set->width(); i++)
        for(uint32_t j = 0; j < sprite_set->height(); j++)
          if(sprite_set->at(i, j) != NULL &&
             sprite_set->at(i, j)->getSize() > 0)
            success &= canSetTile(tile_set[i][j], sprite_set->at(i, j),
                                  avoid_player);

    /* Attempt to set the new tiles */
    if(!just_store)
    {
      for(uint32_t i = 0; success && (i < sprite_set->width()); i++)
      {
        for(uint32_t j = 0; success && (j < sprite_set->height()); j++)
        {
          if(sprite_set->at(i, j) != NULL &&
             sprite_set->at(i, j)->getSize() > 0)
          {
            success &= setTile(tile_set[i][j], sprite_set->at(i, j), no_events);
            if(!success)
            {
              end_x = i;
              end_y = j;
            }
          }
        }
      }
    }

    /* If unsuccessful, unset all that were set */
    if(!success)
    {
      if(!just_store)
      {
        bool finished = false;

        for(uint32_t i = 0; !finished && (i < sprite_set->width()); i++)
        {
          for(uint32_t j = 0; !finished && (j < sprite_set->height()); j++)
          {
            if(sprite_set->at(i, j) != NULL)
            {
              if(i == end_x && j == end_y)
                finished = true;
              else
                unsetTile(i, j, true);
            }
          }
        }
      }
    }
    else
    {
      starting_tiles = tile_set;

      if(!just_store)
      {
        active = true;
        tile_main = tile_set;
        tile_section = section;
      }
    }

    return success;
  }
  return false;
}

/*
 * Description: Sets the target for this map thing. This is only permitted if
 *              the class is currently not targetting anything. However, if the
 *              pointer is null, the target will be cleared. Once the target is
 *              set, the movement will be paused.
 *
 * Inputs: MapThing* target - a map thing target that is to be targetted
 * Output: bool - returns if the target could be set.
 */
bool MapThing::setTarget(MapThing* target)
{
  if(target != NULL && this->target == NULL)
  {
    setMovementPaused(true);
    this->target = target;
    return true;
  }

  return false;
}

/* Description: Sets if the thing is visible or not through the rendering
 *              engine.
 *
 * Inputs: bool visible - status if the thing will be visible
 * Output: none
 */
void MapThing::setVisibility(bool visible)
{
  /* Check if the visibility is changed */
  if(this->visible != visible)
    changed = true;

  /* Update the visibility */
  // TODO: Implement fade instead of instant
  this->visible = visible;
}

/*
 * Description: Triggers on the walk off of the passed in person pointer. This
 *              does nothing in the MapThing (virtual -> used elsewhere)
 *
 * Inputs: MapPerson* trigger - the triggering person pointer
 * Output: none
 */
void MapThing::triggerWalkOff(MapPerson* trigger)
{
  (void) trigger;
}

/*
 * Description: Triggers on the walk on of the passed in person pointer. This
 *              does nothing in the MapThing (virtual -> used elsewhere)
 *
 * Inputs: MapPerson* trigger - the triggering person pointer
 * Output: none
 */
void MapThing::triggerWalkOn(MapPerson* trigger)
{
  (void) trigger;
}

/*
 * Description: Updates the frames of the thing. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: int cycle_time - the ms time to update the movement/animation
 *         std::vector<std::vector<Tile*>> tile_set - the next tiles to move to
 *         bool active_map - true if this things section is the active map
 * Output: Floatinate - the delta x and y of the moved thing
 */
Floatinate MapThing::update(int cycle_time,
                            std::vector<std::vector<Tile*>> tile_set,
                            bool active_map)
{
  (void)tile_set;
  Floatinate delta_move;

  /* For base, just update animation of thing */
  if(getBase() == nullptr)
  {
    if(sprite_set != nullptr)
      animate(cycle_time);
  }
  /* For active, update movement and animation */
  else if(isActive() && isTilesSet())
  {
    if(active_map)
      delta_move = moveThing(cycle_time);
    //if(getBase() == nullptr)
    //  animate(cycle_time);
  }
  /* Otherwise, just update the inactive respawn time */
  else if(active_time >= 0)
  {
    active_lapsed += cycle_time;
    if(active_lapsed >= active_time)
      setActive(true);
  }

  return delta_move;
}

/*
 * Description: Unsets an individual frame that is embedded in this as the
 *              Map Thing and at the x, y coordinates specified.
 *
 * Inputs: uint32_t x - x coordinate of frame to delete
 *         uint32_t y - y coordinate of frame to delete
 *         bool delete_state - should the old frames be deleted?
 * Output: none
 */
void MapThing::unsetFrame(uint32_t x, uint32_t y, bool delete_frames)
{
  if(sprite_set != NULL && sprite_set->getSprite(x, y) != NULL)
  {
    unsetTile(x, y, true);
    sprite_set->unsetSprite(x, y, delete_frames);
  }
}

/*
 * Description: Unsets the frames that are embedded in this as the Map Thing
 *
 * Inputs: bool delete_state - should the old frames be deleted?
 * Output: none
 */
void MapThing::unsetFrames(bool delete_frames)
{
  unsetTiles(true);
  if(sprite_set != NULL)
    sprite_set->unsetSprites(delete_frames);
}

/*
 * Description: Unsets the tiles from the thing. This will stop the rendering
 *              process, since it doesn't have any corresponding tiles to render
 *              to. However, the starting coordinate is untouched.
 *
 * Inputs: bool no_events - true if no events will trigger on removal from tile
 * Output: none
 */
void MapThing::unsetTiles(bool no_events)
{
  /* Unset in each frame of the thing */
  if(isTilesSet())
  {
    SpriteMatrix* sprite_set = getMatrix();

    /* Reset movement and animation */
    if(isMoving())
      tileMoveFinish();
    setDirection(Direction::DIRECTIONLESS);
    animate(0, true);

    /* Reset X and Y back to coordinates of top left tile */
    x = 0.0;
    y = 0.0;

    /* Loop through all the frames and unset each tile */
    for(uint16_t i = 0; i < sprite_set->width(); i++)
      for(uint16_t j = 0; j < sprite_set->height(); j++)
        if(sprite_set->at(i, j) != NULL)
          unsetTile(i, j, no_events);
  }

  /* Clear all tiles */
  tile_main.clear();
  tile_prev.clear();
  tile_section = 0;
}
