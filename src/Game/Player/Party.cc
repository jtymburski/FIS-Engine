/******************************************************************************
* Class Name: Party Implementation
* Date Created: December 2nd, 2012
* Inheritance: None?
* Description: Party is an object holding the five members (Person class) of 
*              a sleuth
*  
* TODO: CONSTRUCTORS TO BE FINISHED
* TODO: CHANGE SPEED (CONSTANT) ?
* TODO: For clearing, removing party members -- where do they go? [12-02-12]
******************************************************************************/

#include "Game/Player/Party.h"

/* Constructs a Party Object */
Party::Party(QWidget* parent)
{
}

/* Annihilates a Party Object */
Party::~Party()
{
}

/* Adds a person to party, returns false if no space or failure */
bool Party::addMember(Person* person)
{
  // if (party_size >= 5)
  //  return false;
  //members.at(party_size) = person;
  // return true;
}

/* Removes a person from the party false if not able to */
bool Party::removeMember(int index)
{
  // if (party-size <= 1)
  //   return false;
  // members.at(index) = NULL;
  // return true;
}

/* Clears everyone but the main character (index 0) from the party */
void Party::clearParty()
{
  for (int i = 1; i < 5;)
    members[i] = NULL;
}

/* Returns a person at a certain index */
Person* Party::getMember(int index)
{
  //  if (members.at(index))
  //    return members[index];
  //  return NULL;
}


/* Returns the party size */
int Party::getPartySize()
{
  return party_size;
}

/* Sets the temp max size of the party (max 5) */
int Party::setMaxSize()
{
  return max_size;
}
