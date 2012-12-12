/******************************************************************************
* Class Name: Party
* Date Created: Sunday, November 4th, 2012
* Inheritance: None?
* Description: Party is an object holding the five members (Person class) of 
*              a sleuth
*  
* TODO: CONSTRUCTORS TO BE FINISHED
* TODO: CHANGE SPEED (CONSTANT) ?
******************************************************************************/
#ifndef PARTY_H
#define PARTY_H

#include <QtGui/QWidget>

#include "Game/Player/Inventory.h"
#include "Game/Player/Person.h"

class Party : public QWidget
{
public:
  Party(QWidget* parent = 0);
  ~Party();

protected:


private:
  /* The shared inventory of the party */
  Inventory* inven; 

  /* The main party member (rendered on the map */
  Person* main; 

  /* The party members */
  Person* members[5];

  /* The size of the party (<= 5) */
  int party_size; 

  /* Temporary maximum size of the party (max 5) */
  int max_size;

public:
  /* Adds a person to party, returns false if no space or failure */
  bool addMember(Person* p); 

  /* Removes a person from the party false if not able to */
  bool removeMember(int index);

  /* Clears everyone but hte main character from the party */
  void clearParty(); 

  /* Returns the party size */
  int getPartySize();

  /* Sets the temp max size of the party (max 5) */
  int setMaxSize();
};

#endif // PARTY_H
