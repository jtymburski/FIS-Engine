/*******************************************************************************
 * Class Name: MapInteractiveObject
 * Date Created: Oct 28 2012
 * Inheritance: MapThing
 * Description: This is the object is a specialization of the stock map thing
 *              that allows interaction from key presses by players in
 *              nearby slots. Pressing the key will toggle the object and allow
 *              it to change states. These objects are unmovable and are of the
 *              typical interactive objects such as doors, chests, etc.
 ******************************************************************************/
#ifndef MAPINTERACTIVEOBJECT_H
#define MAPINTERACTIVEOBJECT_H

#include "Game/Map/MapPerson.h"
#include "Game/Map/MapState.h"
#include "Game/Map/MapThing.h"

class MapInteractiveObject : public MapThing
{
public:
  /* Constructor functions */
  MapInteractiveObject();
  MapInteractiveObject(int id, std::string name = "",
                       std::string description = "");

  /* Destructor function */
  virtual ~MapInteractiveObject();

private:
  /* The action initiator person pointer - last one called */
  MapPerson* action_initiator;

  /* Locked interaction status */
  Locked lock_struct;

  /* The nodes that indicate the available states and transitions */
  StateNode* node_current;
  StateNode* node_head;
  bool nodes_delete;

  /* Indicator if a person is standing on the interactive object */
  MapPerson* person_on;

  /* Shifting the nodes in the forward direction indicator */
  bool shifting_forward;

  /* This defines the time before returning to the previous state after no
   * activity */
  int time_elapsed;
  int time_return;

  /* -------------------------- Constants ------------------------- */
  const static short kRETURN_TIME_UNUSED; /* The unused time inidicator */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Appends an empty node to the tail of the sequence list */
  void appendEmptyNode();

  /* Appends the node to the tail of the sequence list */
  void appendNode(StateNode* node);

  /* Deletes all memory stored within the given node */
  bool clearNode(int id);

  /* Returns the node based on the id, NULL if doeesn't exist */
  StateNode* getNode(int id);

  /* Returns the length of the node sequence */
  int getNodeLength();

  /* Returns the tail state */
  StateNode* getTailNode();

  /* Only to be called by the handler */
  void handlerInteract();

  /* Sets the current sequence of the node to the parent frames and resets the
   * pointers, where applicable */
  void setParentFrames();

  /* Shift the node sequence to next or previous */
  bool shift();
  bool shiftAvailable();
  bool shiftNext();
  bool shiftPrevious();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Check if the tile can be set with the thing */
  virtual bool canSetTile(Tile* tile, TileSprite* frames, 
                          bool avoid_player = false);

  /* Is move allowed, based on main tile and the next tile */
  virtual bool isTileMoveAllowed(Tile* previous, Tile* next,
                                 uint8_t render_depth, Direction move_request);

  /* Sets the tile of the selected with the corresponding frames */
  virtual bool setTile(Tile* tile, TileSprite* frames,
                       bool no_events = true);
  virtual void setTileFinish(Tile* old_tile, Tile* new_tile,
                             uint8_t render_depth, bool reverse_last = false,
                             bool no_events = false);
  virtual bool setTileStart(Tile* old_tile, Tile* new_tile,
                            uint8_t render_depth, bool no_events = false);

  /* This unsets the tile, at the given frame coordinate */
  virtual void unsetTile(uint32_t x, uint32_t y, bool no_events);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds IO information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index,
                                   int section_index, SDL_Renderer* renderer,
                                   std::string base_path = "");

  /* Returns the class descriptor, useful for casting */
  virtual std::string classDescriptor();

  /* Shrink the frame matrix to the valid size and removes all null and void
   * pointers.  */
  virtual bool cleanMatrix(bool first_call = true);

  /* Clears all information from the class (including deleting necessary
   * pointers) */
  virtual void clear();

  /* Returns the inactive time before returning down the state path */
  int getInactiveTime() const;

  /* Returns the locked state struct */
  Locked getLock();

  /* Returns the state, corresponding to the parameters listed */
  StateNode* getStateCurrent();
  StateNode* getStateHead();

  /* Called by the event handler to trigger the appropriate event */
  bool handlerTrigger(int interaction_state, MapPerson* initiator);

  /* Interact with the thing (use key) */
  bool interact(MapPerson* initiator);

  /* Returns if the MIO state change is currently locked */
  bool isLocked();

  /* Reset back to head state */
  void reset();

  /* Sets the base class */
  virtual bool setBase(MapThing* base);

  /* Sets the inactive time before returning down the state path (ms) */
  void setInactiveTime(int time);

  /* Sets the locked status */
  bool setLock(Locked new_locked);

  /* Sets the states, to be painted and used */
  bool setState(MapState* state);
  bool setState(SpriteMatrix* transition);

  /* Triggers walk on / walk off events on the thing */
  void triggerWalkOff(MapPerson* trigger);
  void triggerWalkOn(MapPerson* trigger);

  /* Unlock trigger call */
  bool unlockTrigger(UnlockIOMode mode, int state_num, 
                     UnlockIOEvent mode_events);

  /* Updates the thing, based on the tick */
  virtual void update(int cycle_time, std::vector<std::vector<Tile*>> tile_set);

  /* Unsets all states - this handles deletion */
  void unsetFrames(bool delete_frames = true);
};

#endif // MAPINTERACTIVEOBJECT_H
