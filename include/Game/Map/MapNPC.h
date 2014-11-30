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
   * LOCKED - Halts movement once the next node is reached */
  enum NodeState{LOOPED, BACKANDFORTH, LOCKED};
  
private:
  /* The starting node of the NPCs Path */
  Path* current;
  Path* head;
  bool moving_forward;
  int npc_delay;
  NodeState state;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Appends an empty node onto the back of the movement stack */
  void appendEmptyNode();
  
  /* Returns the node pointer at the index */
  Path* getNode(uint16_t index);
  
  /* Inserts a node, at the given index */
  bool insertNode(uint16_t index, Path* node);
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds npc information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index, 
                                   int section_index, SDL_Renderer* renderer, 
                                   std::string base_path = "");

  /* Returns the class descriptor, useful for casting */
  virtual std::string classDescriptor();

  /* Clears out the NPC construct, void of painting */
  void clear();

  /* Path node handling functions */
  bool insertNode(uint16_t index, uint16_t x, uint16_t y, uint16_t delay = 0);
  bool insertNodeAtTail(uint16_t x, uint16_t y, uint16_t delay = 0);
  
  /* Returns the node movement state - how it traverses */
  NodeState getNodeState();
  
  /* Returns the number of nodes in the NPC path */
  uint16_t getPathLength();

  /* Returns the predicted move request in the class */
  Direction getPredictedMoveRequest();
  
  /* Path nodes removal handling */
  bool removeAllNodes();
  bool removeNode(uint16_t index);
  bool removeNodeAtTail();

  /* Sets the node movement state - how it traverses */
  void setNodeState(NodeState state);

  /* Updates the thing, based on the tick - reimplemented */
  void update(int cycle_time, std::vector<std::vector<Tile*>> tile_set);
};

#endif // MAPNPC_H
