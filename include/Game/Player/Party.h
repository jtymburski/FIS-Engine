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

#include <QWidget>

#include "Game/Player/Inventory.h"
#include "Game/Player/Person.h"

class Party : public QWidget
{
  Q_OBJECT

public:
  /* Creates a party object */
  Party(Person* p_main, Inventory* inventory = 0,
        EnumDb::PartyType party = EnumDb::REGULAR_FOE);

  /* Annihilates a party object */
  ~Party();

  /* Enumerated flags for party */
  enum PartyFlag
  {
    INVENTORY_ENABLED  = 1 << 0, /* Can the inventory be altered? */
    ITEM_USE_ENABLED      = 1 << 1, /* Can the party use items? */
    MEMBERS_ENABLED    = 1 << 2, /* Can the members be altered? */
    BATTLE_ENCOUNTERS  = 1 << 3  /* Can this party battle other parties? */
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
 * SIGNALS
 *============================================================================*/
signals:
  /* Battle Item can not be used error */
  void battleUseItemError(QString reasons);

  /* Signal to Battle to cure an ailment */
  void cureAilment(ushort target, EnumDb::Infliction ailment_type);

  /* Menu Item can not be used error */
  void menuUseItemError(QString reasons);

  /* General Item can not be used error */
  void useItemError(QString reasons);

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds a person to party, returns false if no space or failure */
  bool addMember(Person* p); 

  /* Clears the party */
  void clearParty();

  /* Evaluates true if the Party contains a "Boss" character */
  bool hasBoss();

  /* Evaluates true if the Party contains a "Final Boss" character */
  bool hasFinalBoss();

  /* Evaluates true if the Party contains a "Mini Boss" character */
  bool hasMiniBoss();

  /* Methods for printing out the state of the class */
  void printAll();
  void printInfo();
  void printFlags();

  /* Removes a person from the party by index */
  bool removeMember(uint i);

  /* Removes a person from the party by name */
  bool removeMember(QString);

  /* Uses the item on the party */
  bool useItem(Item* used_item, ushort target, EnumDb::ActionOccasion use_type);

  /* Returns the average speed of the Party */
  ushort getAverageSpeed();

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

  /* Returns the total speed of the Party */
  int getTotalSpeed();

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
  static ushort getMaxBearacksSize();

  /* Obtains the maximum number of members in a party of foes */
  static ushort getMaxFoeSize();

  /* Obtains the maximum number of members in a slueth */
  static ushort getMaxSleuthSize();
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Party::PartyFlags)

#endif // PARTY_H
