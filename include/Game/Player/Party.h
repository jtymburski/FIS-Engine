/******************************************************************************
* Class Name: Party
* Date Created: Sunday, November 4th, 2012
* Inheritance: None?
* Description: Party is an object designed to hold a number of persons, such as
*              your sleuth (maximum of five members) or the members of your
*              barracks on the ship, or a group of foes you will face in battle
*  
* TODO: CONSTRUCTORS TO BE FINISHED
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
  Party(QWidget* parent = 0, int max = 5);

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

  /* Removes a person from the party by index */
  bool removeMember(int index);

  /* Removes a person from the party by name */
  bool removeMember(QString);

  /* Clears everyone but hte main character from the party */
  void clearParty();

  /* Returns the inventory of the party */
  Inventory* getInventory();

  /* Returns a person at a certain index */
  Person* getMember(int index);

  /* Returns the party size */
  int getPartySize();

  /* Returns a person's name at a certain index */
  QString getMemberName(int index);

  /* Returns the currently set max size of the party */
  int getMaxSize();

  /* Sets the inventory of the party */
  void setInventory(Inventory* i = NULL);

  /* Sets the main member of the party */
  void setMainMember(Person* p = NULL);

  /* Sets the temp max size of the party (max 5) */
  bool setMaxSize(int value);
};

#endif // PARTY_H
