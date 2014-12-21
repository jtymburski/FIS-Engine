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
* - Menu use Item function [01-18-14]
* - Battle use Item function [01-18-14]
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
  CAN_ADD_MEMBERS    = 1 << 0,
  CAN_REMOVE_MEMBERS = 1 << 1,
  ITEM_USE_ENABLED   = 1 << 2,
  CAN_ADD_ITEMS      = 1 << 3,
  CAN_REMOVE_ITEMS   = 1 << 4,
  ENCOUNTERS_ENABLED = 1 << 5,
  HAS_BOND           = 1 << 6
};

class Party
{
public:
  /* Constructs a Party given a main character and a type of Party */
  Party(const int32_t &my_id, Person* const main, const PartyType &type, 
        const uint8_t &max, Inventory* const inv = nullptr);

  /* Constructs a Party given a vector of Person ptrs as members */
  Party(const int32_t &my_id, std::vector<Person*> members, const uint8_t &max, 
        const PartyType &type, Inventory* const inv = nullptr);

private:
  /* Party flag set */
  PartyState flags;

  /* The members who are currently in the Party */
  std::vector<Person*> members;
 
  /* The maximum size of the party */
  uint8_t max_size;

  /* The type of the Party */
  PartyType party_type;
 
  /* The ID of the party */
  const int32_t id;

  /* The inventory for the party */
  Inventory* pouch;
  
  /* ------------ Constants --------------- */
  static const uint8_t kMAX_MEMBERS_BEARACKS;
  static const uint8_t kMAX_MEMBERS_SLEUTH;
  static const uint8_t kMAX_MEMBERS_FOES;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

  /* Loads the default settings for a Party */
  void loadDefaults();

  /* Implements the battle use effect of a given item (by game_id) */
  void battleUseItem(const uint32_t &game_id, const uint8_t &index);

  /* Implements the menu use effect of a given item (by game_id) */
  void menuUseItem(const uint32_t &game_id, const uint8_t &index);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to add a person to the party */
  bool addMember(Person* const new_member);
 
  /* Clears all members of the party except the primary member */
  bool clearParty();

  /* Returns all the persons in the party except a given user */
  std::vector<Person*> findMembersExcept(Person* const member, 
      const bool &only_living = false);
  
  /* Evaluates whether the current party contains a boss */
  bool hasBoss();

  /* Evaluates whether the current party contains a final boss */
  bool hasFinalBoss();

  /* Evaluates whether the current party contains a mini boss */
  bool hasMiniBoss();

  /* Checks if a given person is in the party */
  bool isInParty(Person* const check_person);

  /* Prints out the state of the Party */
  void print(const bool &simple = false, const bool &flags = false);

  /* Attempts to a remove a member of the party by a given index */
  bool removeMember(const uint8_t &index);

  /* Attempts to remove a member of the party by a given string name */
  bool removeMember(const std::string &name);

  /* Uses an item (given by game_id) on a given member (index) of a use type */
  bool useItem(const uint32_t &game_id, const uint8_t &index, 
               const ActionOccasion &use_type);

  /* Calculates and returns the average speed of the Party */
  int32_t getAverageSpeed();

  /* Returns a vector of indexes of all KO'd party members */
  std::vector<uint32_t> getDeadMembers();

  /* Evaluates and returns a given PartyState flag */
  bool getFlag(const PartyState &test_flag);

  /* Returns the pointer to the current inventory of the Party */
  Inventory* getInventory();

  /* Returns a vector of the indexes of all non-KO'd party members */
  std::vector<uint32_t> getLivingMembers();

  /* Returns the current maximum size of the party */
  uint32_t getMaxSize();

  /* Obtains a ptr to a member of a given index, if the index is valid */
  Person* getMember(const uint8_t &index);

  /* Returns the string name a party member at a given index, if valid */
  std::string getMemberName(const uint8_t &index);

  /* Get all members */
  std::vector<Person*> getMembers();

  /* Returns the enumerated type of the Party */
  PartyType getPartyType();

  /* Return the size of the party */
  uint32_t getSize();

  /* Calculates and returns the total speed of the Party */
  int64_t getTotalSpeed();

  /* Attempts to assign a new maximum size of the Party */
  bool setMaxSize(const uint8_t &new_max_size);

  /* Assigns a new inventory to the Party */
  bool setInventory(Inventory* const new_inventory);

  /* Attempts to assign a new primary member of the Party */
  bool setMainMember(const uint8_t &new_main);

  /* Assigns a given PartyState flag a given value */
  void setFlag(const PartyState &flag, const bool &set_value = true);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the maximum size of the Bearacks */
  static uint8_t getMaxBearacks();

  /* Returns the maximum size of the Sleuth */
  static uint8_t getMaxSleuth();

  /* Returns the maximum size of the foes */
  static uint8_t getMaxFoes();
};

#endif //PARTY_H
