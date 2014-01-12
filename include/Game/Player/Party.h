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
*******************************************************************************/

#ifndef PARTY_H
#define PARTY_H

#include "EnumDB.h"
#include "EnumFlags.h"
#include "Game/Player/Inventory.h"
#include "Game/Player/Person.h"

ENUM_FLAGS(PartyState)
enum class PartyState
{
  CAN_ADD_MEMBERS    = 1 << 0,
  CAN_REMOVE_MEMBERS = 1 << 1,
  ITEM_USE_ENABLED   = 1 << 2,
  CAN_ADD_ITEMS      = 1 << 3,
  CAN_REMOVE_ITEMS   = 1 << 4,
  ENCOUNTERS_ENABLED = 1 << 5,
};

class Party
{
public:
  Party(Person* main, PartyType party_type, Inventory* inv = nullptr);

  ~Party();

private:
  /* The members who are currently in the Party */
  std::vector<Person*> members;
 
  /* The maximum size of the party */
  uint8_t max_size;

  /* The type of the Party */
  PartyType party_type;

  /* The inventory for the party */
  Inventory* pouch;

  /* ------------ Constants --------------- */
  static const uint8_t kMAX_MEMBERS;
  static const uint8_t kMAX_MEMBERS_BEARACKS;
  static const uint8_t kMAX_MEMBERS_SLEUTH;
  static const uint8_t kMAX_MEMBERS_FOES;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  bool addMember(Person* new_member);

  bool removeMember(const uint32_t &index);

  Inventory* getInventory();
  
  uint32_t getMaxSize();

  Person* getMember(const uint32_t &index);

  PartyType getPartyType();



/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the total maximum size of members */
  static uint8_t getMaxMembers();

  /* Returns the maximum size of the Bearacks */
  static uint8_t getMaxBearacks();

  /* Returns the maximum size of the Sleuth */
  static uint8_t getMaxSleuth();

  /* Returns the maximum size of the foes */
  static uint8_t getMaxFoes();

};
#endif //PARTY_H
