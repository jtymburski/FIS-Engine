/******************************************************************************
* Class Name: Party
* Date Created: Sunday, November 4th, 2012
* Inheritance: None?
* Description: Party is an object designed to hold a number of persons, such as
*              your sleuth (maximum of five members) or the members of your
*              barracks on the ship, or a group of foes you will face in battle
******************************************************************************/
#ifndef PARTY_H
#define PARTY_H

#include <QtGui/QWidget>

#include "Game/Player/Inventory.h"
#include "Game/Player/Person.h"

class Party : public QWidget
{
public:
  /* Creates a party object */
  Party(Person* p_main, uint max = 5, Inventory* = NULL, QWidget* parent = 0);

  /* Annihilates a party object */
  ~Party();

private:
  /* The shared inventory of the party */
  Inventory* pouch;

  /* The main party member (rendered on the map */
  Person* main; 

  /* The party members */
  QVector<Person*> members;

  /* Temporary maximum size of the party (max 5) */
  int max_size;

public:
  /* Adds a person to party, returns false if no space or failure */
  bool addMember(Person* p); 

  /* Clears the party */
  void clearParty();

  /* Removes a person from the party by index */
  bool removeMember(uint index);

  /* Removes a person from the party by name */
  bool removeMember(QString);

  /* Returns the inventory of the party */
  Inventory* getInventory();

  /* Returns a person at a certain index */
  Person* getMember(uint index);

  /* Returns the party size */
  uint getPartySize();

  /* Returns a person's name at a certain index */
  QString getMemberName(uint index);

  /* Returns the currently set max size of the party */
  int getMaxSize();

  /* Sets the inventory of the party */
  void setInventory(Inventory* i = NULL);

  /* Sets the main member of the party */
  void setMainMember(Person* p = NULL);

  /* Sets the temp max size of the party (max 5) */
  bool setMaxSize(uint value);
};

#endif // PARTY_H
