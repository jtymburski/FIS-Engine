/*******************************************************************************
 * Class Name: MapNPC
 * Date Created: Oct 28 2012
 * Inheritance: MapPerson
 * Description: The MapNPC class, this covers all AI found on the map in game
 *
 * TODO: Reimplement the movement functions based on NPC scripts
 ******************************************************************************/
#ifndef MAPNPC_H
#define MAPNPC_H

#include "Game/Map/MapPerson.h"

/* Path node struct for moving */
struct Path
{
  uint16_t x;
  uint16_t y;
  uint16_t delay;
  bool xy_flip;
  Path* previous;
  Path* next;
};

class MapNPC : public MapPerson
{
public:
  /* Constructor function */
  MapNPC();
  MapNPC(int id, std::string name = "", std::string description = "");

  /* Destructor function */
  virtual ~MapNPC();

  /* LOOPED - Goes from node 1->2->3->1->2...
   * BACKANDFORTH - Goes from node 1->2->3->2->1->2...
   * RANDOMRANGE - Can move anywhere within the defined rect range (2 nodes)
   * RANDOM - Can move anywhere within the map
   * LOCKED - Halts movement once the next node is reached */
  enum NodeState{LOOPED, BACKANDFORTH, RANDOMRANGE, RANDOM, LOCKED};

  /* -------------------------- Constants ------------------------- */
  const static uint16_t kFORCED_FREEZE; /* Time before moving after triggering
                                           a forced. In ms */
  const static uint16_t kFORCED_NOTRIGGER; /* Time before triggering a forced
                                              again. In ms */
  const static uint16_t kFORCED_RESET; /* Reset delay if trying to go home */
  const static uint16_t kMAX_DELAY; /* The max delay for random delay calcs */
  const static uint16_t kMAX_RANGE; /* THe max range for random x,y move */
  const static float kPYTH_APPROX; /* Pythagorean c side approx factor */
  const static uint16_t kSPOTTED_FADE; /* Time for spotted fade out sequence */
  const static uint16_t kSPOTTED_INIT; /* Time for spotted image total */
  const static uint16_t kSTUCK_DELAY; /* The time to sit when stuck */
  const static uint16_t kTRACK_DELAY; /* The track delay before trying dir */
  const static uint16_t kTRACK_DIST_MIN; /* The default min distance to track */
  const static uint16_t kTRACK_DIST_MAX; /* The default max distance to give up
                                            tracking */
  const static uint16_t kTRACK_DIST_RUN; /* The run distance. Between min max */

private:
  /* Does the NPC force interaction if possible? */
  bool forced_interaction;
  bool forced_recent;
  int forced_time;

  /* Spotted image reference */
  Frame* img_spotted;

  /* Is the NPC currently going forward or backward - used for BACKANDFORTH */
  bool moving_forward;

  /* The nodes for control */
  Path* node_current;
  Path* node_head;
  Path node_player;
  Path* node_previous;
  Path node_random;
  SDL_Rect node_rect;
  Path node_start;

  /* The state of movement */
  NodeState node_state;

  /* Should the nodes be deleted on destruction - relevant if base npc */
  bool nodes_delete;

  /* The active delay of the NPC on the node, if relevant */
  int npc_delay;

  /* The player reference - for tracking purposes */
  MapPerson* player;

  /* Spotted animation information */
  Frame* spotted_img;
  int spotted_time;

  /* Is this the first run of the path move? */
  bool starting;

  /* Stuck delay */
  int stuck_delay;
  bool stuck_flip;

  /* The state of tracking */
  int track_delay;
  int track_dist;
  int track_dist_max;
  int track_dist_run;
  bool track_initial;
  bool track_recent;
  TrackingState track_state;
  bool tracking;

/*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Appends an empty node onto the back of the movement stack */
  void appendEmptyNode();

  /* Returns if the XY movement is flipped for the direction to move */
  bool getXYFlip();

  /* Returns the node pointer at the index */
  Path* getNode(uint16_t index);

  /* Initializes class - called on first construction */
  void initializeClass();

  /* Inserts a node, at the given index */
  bool insertNode(uint16_t index, Path* node);

  /* Takes the path node and randomizes the x, y, delay, and flip */
  void randomizeNode();

  /* Tracking functions - called by update */
  void trackAvoidPlayer(int cycle_time, bool stopped);
  int trackOutOfRange(MapPerson* ref = nullptr);

  /* update the node bounding rect */
  void updateBound();

/*======================= PROTECTED FUNCTIONS ==============================*/
protected:
  /* Is data available to save */
  virtual bool isDataToSave();

  /* Additional rendering call for overlays - virtualized */
  virtual bool renderAdditional(SDL_Renderer* renderer, Tile* tile,
                                int tile_x, int tile_y,
                                int render_x, int render_y);

  /* Saves the NPC data - virtualized */
  virtual bool saveData(FileHandler* fh, const bool &save_event = true);

  /* Sets the direction that the NPC is travelling in */
  bool setDirection(Direction direction, bool set_movement = true);

/*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Adds npc information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index,
                                   int section_index, SDL_Renderer* renderer,
                                   std::string base_path = "",
                                   bool from_save = false);

  /* Returns the class descriptor, useful for casting */
  virtual ThingBase classDescriptor();

  /* Clears out the NPC construct, void of painting */
  void clear();

  /* Get the specific details of the movement information - additional
   * information provided from forced interaction */
  bool getMovementPaused();

  /* Returns the node movement state - how it traverses */
  NodeState getNodeState();

  /* Returns the number of nodes in the NPC path */
  uint16_t getPathLength();

  /* Returns the reference player */
  MapPerson* getPlayer();

  /* Returns the predicted move request in the class */
  Direction getPredictedMoveRequest();

  /* Returns the spotted information */
  Frame* getSpottedImage();
  int getSpottedTime();

  /* Tracking distance setpoints getters */
  int getTrackDistMax();
  int getTrackDistMin();
  int getTrackDistRun();

  /* Returns the tracking state - how the NPC reacts to others */
  TrackingState getTrackingState();

  /* Path node handling functions */
  bool insertNode(uint16_t index, uint16_t x, uint16_t y, uint16_t delay = 0);
  bool insertNodeAtTail(uint16_t x, uint16_t y, uint16_t delay = 0);

  /* Starts interaction - on a forced trigger */
  bool interactForced(MapPerson* initiator);

  /* Returns if the NPC will force interaction */
  bool isForcedInteraction(bool false_if_active = true);

  /* Returns if there is any form of tracking */
  bool isTracking();

  /* Path nodes removal handling */
  bool removeAllNodes();
  bool removeNode(uint16_t index);
  bool removeNodeAtTail();

  /* Resets the tile position */
  bool resetToStart(bool no_set = false);

  /* Saves the delta data within the npc */
  virtual bool save(FileHandler* fh);

  /* Sets the base class */
  virtual bool setBase(MapThing* base);

  /* Sets the forced interaction of the npc */
  void setForcedInteraction(bool forced);

  /* Sets the node movement state - how it traverses */
  void setNodeState(NodeState state);

  /* Sets the reference player */
  void setPlayer(MapPerson* player);

  /* Sets the spotted reference image */
  void setSpottedImage(Frame* new_img);

  /* Sets the starting x and y coordinate */
  void setStartingLocation(uint16_t section_id, uint16_t x, uint16_t y);

  /* Sets the tracking distance setpoints */
  void setTrackingDist(int trigger = kTRACK_DIST_MIN,
                       int max = kTRACK_DIST_MAX, int run = kTRACK_DIST_RUN);

  /* Sets the tracking state - how the NPC reacts */
  void setTrackingState(TrackingState state);

  /* Updates the thing, based on the tick - reimplemented */
  Floatinate update(int cycle_time, std::vector<std::vector<Tile*>> tile_set,
                    bool active_map = false);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the node state string */
  static std::string getNodeString(NodeState state);

  /* Returns the tracking state string */
  static std::string getTrackingString(TrackingState state);
};

#endif // MAPNPC_H
