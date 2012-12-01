/********************************************************************************
* Class Name: MapNPC
* Date Created: Oct 28 2012
* Inheritance: MapPerson
* Description: The MapNPC class, this covers all AI found on the map in game
* TODO: Reimplement the movement functions based on NPC scripts
********************************************************************************/
#ifndef MAPNPC_H
#define MAPNPC_H
#include <QVector>
#include <QString>
#include "MapPerson.h"


/*Path node struct*/
struct Path
{
  int xpos,intypos;
  Path* next;
};


class MapNPC : public MapPerson
{
public:
  /*Constructor function*/
  MapNPC();

  /*Destructor function*/
  ~MapNPC();

private:
  /*The object given by the NPC, can be NULL*/
  MapThing* gift;

  /*The starting node of the NPCs Path*/
  Path* head;

  /*The nodes for the NPCs path*/
  Path* nodes;

  /*The dialog sequence for the NPC and the Player*/
  QVector<QString*> talking_points; //The dialog sequence

  /*The dialog sources for each talking point*/
  QVector<bool> talking_sources;

  /*The other dialog person (usually the player)*/
  MapPerson* target;

public:
  /*Gets a pointer to the Persons conversation*/
  QVector<QString*>* getConversation();

  /*Gets a pointer to the gift the NPC has*/
  MapThing* getGift();

};

#endif // MAPNPC_H
