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
  Party(Person* p_main, Inventory* inventory = 0,
        EnumDb::PartyType party = EnumDb::REGULAR_FOE);

  /* Annihilates a party object */
  ~Party();

  /* Enumerated flags for party */
  enum PartyFlag
  {
    INVENTORY_ENABLED = 1 << 0, /**/
    MEMBERS_ENABLED    = 1 << 1
  };
  Q_DECLARE_FLAGS(PartyFlags, PartyFlag)
  PartyFlags pflag_set;

private:
  /* The shared inventory of the party */
  Inventory* pouch;

  /* The party members */
  QList<Person*> members;

  /* Temporary maximum size of the party (max 5) */
  ushort max_size;

  /* Enumeration describing the type of party */
  EnumDb::PartyType party_type;

  /*------------------- Constants -----------------------*/
  static const ushort kMAX_MEMBERS_BEARACKS = 50; /* Maximum bearacks members */
  static const ushort kMAX_MEMBERS_SLEUTH   =  5; /* Maximum sleuth members */
  static const ushort kMAX_MEMBERS_FOE      =  5; /* Maximum foe members */

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Functionality for using items in battle */
  bool battleUseItem(Item* used_item, ushort target = 0);

  /* Functionality for using items in menu */
  bool menuUseItem(Item* used_item, ushort target = 0);

  /* Sets the enumerated party type (usually called by construction) */
  void setPartyType(EnumDb::PartyType new_party_type);

  /* Updates the maximum size of the party based on its current type */
  ushort updateMaxSize();

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds a person to party, returns false if no space or failure */
  bool addMember(Person* p); 

  /* Clears the party */
  void clearParty();

  /* Methods for printing out the state of the class */
  void printAll();
  void printInfo();
  void printFlags();

  /* Removes a person from the party by index */
  bool removeMember(uint i);

  /* Removes a person from the party by name */
  bool removeMember(QString);

  /* Uses the item on the party */
  bool useItem(Item* used_item, ushort target = 0);

  /* Returns the inventory of the party */
  Inventory* getInventory();

  /* Returns a person at a certain index */
  Person* getMember(uint index);

  /* Gets a party flag */
  bool getFlag(PartyFlag flag);

  /* Returns the enumerated party type */
  EnumDb::PartyType getPartyType();

  /* Returns the party size */
  ushort getPartySize();

  /* Returns a person's name at a certain index */
  QString getMemberName(ushort index);

  /* Returns the currently set max size of the party */
  ushort getMaxSize();

  /* Sets the inventory of the party */
  void setInventory(Inventory* i = 0);

  /* Sets the main member of the party */
  void setMainMember(Person* p = 0);

  /* Sets the temp max size of the party (max 5) */
  bool setMaxSize(ushort value);

  /* Sets a party flag */
  void setFlag(PartyFlag flag, bool set_value);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Obtains the maximum number of members in a bearacks */
  static const ushort getMaxBearacksSize();

  /* Obtains the maximum number of members in a party of foes */
  static const ushort getMaxFoeSize();

  /* Obtains the maximum number of members in a slueth */
  static const ushort getMaxSleuthSize();
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Party::PartyFlags)

#endif // PARTY_H
