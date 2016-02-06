
/*******************************************************************************
* Class Name: Party [Declaration]
* Date Created: January 11th, 2014
* Inheritance: None
* Description: Party is an object designed to hold a number of persons, such as
*              your sleuth (maximum of five members) or the members of your
*              barracks on the ship, or a group of foes you will face in battle
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* [08-17-15] - Method to determine inventory name. Needed?
* [08-17-15] - Thumbanils for inventory.
*******************************************************************************/
#ifndef PARTY_H
#define PARTY_H

#include <vector>

#include "Game/Player/Inventory.h"
#include "Game/Player/Person.h"
#include "EnumDb.h"
#include "EnumFlags.h"

ENUM_FLAGS(PartyState)
enum class PartyState
{
  CAN_ADD_MEMBERS = 1 << 0,
  CAN_REMOVE_MEMBERS = 1 << 1,
  ITEM_USE_ENABLED = 1 << 2,
  CAN_ADD_ITEMS = 1 << 3,
  CAN_REMOVE_ITEMS = 1 << 4,
  ENCOUNTERS_ENABLED = 1 << 5,
  HAS_BOND = 1 << 6
};

class Party
{
public:
  /* The blank constructor */
  Party();

  /* Constructs a Party given a main character and a type of Party */
  Party(const int32_t &my_id, Person *const main, const PartyType &type,
        const uint16_t &max);

  /* Constructs a Party given a vector of Person ptrs as members */
  Party(const int32_t &my_id, std::vector<Person *> members, const uint16_t &max,
        const PartyType &type);

  ~Party();

private:
  /* Party flag set */
  PartyState flags;

  /* The ID of the party */
  int32_t id;

  /* The members who are currently in the Party */
  std::vector<Person *> members;
  std::vector<Person *> reserve_members;

  /* The maximum size of the party */
  uint16_t max_size;

  /* The type of the Party */
  PartyType party_type;

  /* The inventory for the party */
  Inventory *pouch;

  /* ------------ Constants --------------- */
public:
  static const uint32_t kID_BEARACKS; /* ID of player bearacks party */
  static const uint32_t kID_SLEUTH;   /* ID of player sleuth party */
  static const uint16_t kMAX_MEMBERS_BEARACKS;
  static const uint16_t kMAX_MEMBERS_SLEUTH;
  static const uint16_t kMAX_MEMBERS_FOES;
  static const int32_t  kUNSET_ID; /* The unset ID for the party */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const Party &source);

  /* Create the inventory */
  void createInventory();

  /* Loads the default settings for a Party */
  void loadDefaults();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to add a person to the party */
  bool addMember(Person *const new_member);

  /* Clears all members of the party except the primary member */
  bool clearParty();

  /* Restores the VITA // QTDR */
  void restorePartyVita();
  void restorePartyQtdr();

  /* Checks if a given person is in the party */
  bool isInParty(Person *const check_person);

  /* Load data from file */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer,
                std::string base_path);

  /* Move a member between the Reserve and the Standard party */
  bool moveMemberToReserve(Person *test_member);
  bool moveReserveMember(Person *test_member);

  /* Prints out the state of the Party */
  void print(const bool &simple = false, const bool &flags = false);

  /* Attempts to a remove a member of the party by a given index */
  bool removeMember(const uint8_t &index);

  /* Attempts to remove a member of the party by a given string name */
  bool removeMember(const std::string &name);

  /* Evaluates and returns a given PartyState flag */
  bool getFlag(const PartyState &test_flag);

  /* Returns the party ID */
  int32_t getID() const;

  /* Returns the pointer to the current inventory of the Party */
  Inventory *getInventory();

  /* Returns the current maximum size of the party */
  uint32_t getMaxSize();

  /* Obtains a ptr to a member of a given index, if the index is valid */
  Person *getMember(const uint32_t &index);

  /* Returns the string name a party member at a given index, if valid */
  std::string getMemberName(const uint32_t &index);

  /* Get all members */
  std::vector<Person *> getMembers();

  /* Get the reserve members */
  std::vector<Person *> getReserveMembers();

  /* Returns the enumerated type of the Party */
  PartyType getPartyType();

  /* Return the size of the party */
  uint32_t getSize();

  /* Assigns a given PartyState flag a given value */
  void setFlag(const PartyState &flag, const bool &set_value = true);

  /* Assigns the party ID */
  void setID(int id);

  /* Assigns a new inventory to the Party */
  bool setInventory(Inventory *const new_inventory);

  /* Attempts to assign a new primary member of the Party */
  bool setMainMember(const uint32_t &new_main);

  /* Attempts to assign a new maximum size of the Party */
  bool setMaxSize(const uint32_t &new_max_size);

  /* Attempts to assign the new party type */
  bool setPartyType(const PartyType &type);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  Party& operator= (const Party &source);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the maximum size of the Bearacks */
  static uint32_t getMaxBearacks();

  /* Returns the maximum size of the Sleuth */
  static uint32_t getMaxSleuth();

  /* Returns the maximum size of the foes */
  static uint32_t getMaxFoes();
};

#endif // PARTY_H
