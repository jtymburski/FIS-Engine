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
  ~MapNPC();

private:
  /* The object given by the NPC, can be NULL */
  MapThing* gift;

  /* The starting node of the NPCs Path */
  Path* current;
  Path* head;
  
  /* The dialog sequence for the NPC and the Player */
  QList<QString> talking_points; //The dialog sequence

  /* The dialog sources for each talking point */
  QList<bool> talking_sources;

  /* The other dialog person (usually the player) */
  MapPerson* target;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears out the NPC construct, void of painting */
  void clear();

  /* Path node handling functions */
  bool insertNode(int index, Tile* tile, int delay = 0);
  bool insertNodeAtTail(Tile* tile, int delay = 0);

  /* Gets a pointer to the Persons conversation */
  QList<QString> getConversation();

  /* Gets a pointer to the gift the NPC has */
  MapThing* getGift();
  
  /* Returns the number of nodes in the NPC path */
  int getPathLength();
  
  /* Path nodes removal handling */
  bool removeAllNodes();
  bool removeNode(int index);
  bool removeNodeAtTail();
};

#endif // MAPNPC_H
