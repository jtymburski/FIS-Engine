/******************************************************************************
* Class Name: Party
* Date Created: Sunday, November 4th, 2012
* Inheritance:
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
  Party(Person* p_main, ushort max, Inventory* = NULL, QWidget* parent = 0);

  /* Annihilates a party object */
  ~Party();

  /* Enumerated flags for party */
  enum PartyFlag
  {
    MAIN      = 1ul << 0, /* This is the main character's party */
    BEARACKS  = 1ul << 1, /* This is the bearacks party */
    FOE       = 1ul << 2, /* This is an enemy troop */
    MINIBOSS  = 1ul << 3, /* This party is a miniboss */
    BOSS      = 1ul << 4, /* This party is a boss */
    FINALBOSS = 1ul << 5  /* This party is the final boss */
  };
  Q_DECLARE_FLAGS(PartyFlags, PartyFlag)
  PartyFlags pflag_set;

private:
  /* The shared inventory of the party */
  Inventory* pouch;

  /* The party members */
  QVector<Person*> members;

  /* Temporary maximum size of the party (max 5) */
  ushort max_size;

public:
  /* Constants*/
  static const ushort kMAX_MEMBERS = 5;

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

  /* Gets a party flag */
  bool getPartyFlag(PartyFlag flag);

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

  /* Sets a party flag */
  void setPartyFlag(PartyFlag flag, bool set_value);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Party::PartyFlags)

#endif // PARTY_H
