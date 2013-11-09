/******************************************************************************
* Class Name: MapNPC
* Date Created: Oct 28 2012
* Inheritance: MapPerson
* Description: The MapNPC class, this covers all AI found on the map in game
* TODO: Reimplement the movement functions based on NPC scripts
******************************************************************************/
#ifndef MAPNPC_H
#define MAPNPC_H

#include <QDebug>
#include <QString>
#include <QVector>

#include "Game/Map/MapPerson.h"

/* Path node struct for moving */
struct Path
{
  Tile* tile;
  int delay;
  Path* previous;
  Path* next;
};

class MapNPC : public MapPerson
{
public:
  /* Constructor function */
  MapNPC();
  MapNPC(int width, int height, QString name = "", 
         QString description = "", int id = kUNSET_ID);
  
  /* Destructor function */
  virtual ~MapNPC();

  /* LOOPED - Goes from node 1->2->3->1->2...
   * BACKANDFORTH - Goes from node 1->2->3->2->1->2...
   * LOCKED - Halts movement once the next node is reached */
  enum NodeState{LOOPED, BACKANDFORTH, LOCKED};
  
private:
  /* The object given by the NPC, can be NULL */
  MapThing* gift; // TODO

  /* The starting node of the NPCs Path */
  Path* current;
  Path* head;
  bool moving_forward;
  int npc_delay;
  NodeState state;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds npc information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index, 
                                                 int section_index);

  /* Returns the class descriptor, useful for casting */
  virtual QString classDescriptor();

  /* Clears out the NPC construct, void of painting */
  void clear();

  /* Path node handling functions */
  bool insertNode(int index, Tile* tile, int delay = 0);
  bool insertNodeAtTail(Tile* tile, int delay = 0);

  /* Gets a pointer to the gift the NPC has */
  MapThing* getGift(); // TODO
  
  /* Returns the node movement state - how it traverses */
  NodeState getNodeState();
  
  /* Returns the number of nodes in the NPC path */
  int getPathLength();

  /* Path nodes removal handling */
  bool removeAllNodes();
  bool removeNode(int index);
  bool removeNodeAtTail();

  /* Resets the movement of the thing to the new location */
  void resetMovement();

  /* Sets the node movement state - how it traverses */
  void setNodeState(NodeState state);
  
  /* Updates the thing, based on the tick - reimplemented */
  void updateThing(float cycle_time, Tile* next_tile);
};

#endif // MAPNPC_H
