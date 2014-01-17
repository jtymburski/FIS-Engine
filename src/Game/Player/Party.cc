/*******************************************************************************
* Class Name: Party [Implementation]
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
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Party.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

// const uint8_t Party::kMAX_MEMBERS{5};
// const uint8_t Party::kMAX_MEMBERS_BEARACKS{50};
// const uint8_t Party::kMAX_MEMBERS_SLEUTH{5};
// const uint8_t Party::kMAX_MEMBERS_FOES{5};

// /*=============================================================================
//  * CONSTRUCTORS / DESTRUCTORS
//  *============================================================================*/

// /*
//  * Description:
//  *
//  * Inputs:
//  */
// Party::Party(Person* const main, const PartyType &type, const uint8_t &max,
//              Inventory* const inv)
//   : max_size(max)
//   , party_type(type)
//   , pouch(inv)
// {
//   if (main == nullptr)
//     std::cerr << "Error: Creating party with null main member\n";

//   members.push_back(main);
// }

// Party::Party(std::vector<Person*> members, const PartyType &type, 
//              const uint8_t &max, Inventory* const inv)
//   : max_size(max)
//   , party_type(type)
//   , pouch(inv)
// {
//   for (auto member : members)
//   {
//     if (member == nullptr)
//       std::cerr << "Error: Creating party with null members\n";
//     if (members.size() < max_size)
//       members.push_back(member);
//     else
//       std::cerr << "Error: Creating party with too large of size\n";
//   }
// }

/*
 * Description:
 */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
// /* Implements the battle use effect of a given item (by game_id) */
// void Party::battleUseItem(const uint32_t &game_id, const uint8_t &index)
// {

// }

// /* Implements the menu use effect of a given item (by game_id) */
// void Party::menuUseItem(const uint32_t &game_id, const uint8_t &index)
// {

// }

// /*=============================================================================
//  * PUBLIC FUNCTIONS
//  *============================================================================*/

// /* Attempts to add a person to the party */
// bool Party::addMember(Person* const new_member)
// {

// }

// /* Clears all members of the party except the primary member */
// bool Party::clearParty()
// {

// }

// /* Evaluates whether the current party contains a boss */
// bool Party::hasBoss()
// {

// }

//  Evaluates whether the current party contains a final boss 
// bool Party::hasFinalBoss()
// {

// }

// /* Evaluates whether the current party contains a mini boss */
// bool Party::hasMiniBoss()
// {

// }

// /* Prints out the state of the Party */
// void Party::print(bool simple)
// {

// }

// /* Attempts to a remove a member of the party by a given index */
// bool Party::removeMember(const uint8_t &index)
// {

// }

// /* Attempts to remove a member of the party by a given string name */
// bool Party::removeMember(const std::string &name)
// {

// }

// /* Uses an item (given by game_id) on a given member (index) of a use type */
// bool Party::useItem(const uint32_t &game_id, const uint8_t &index, 
//                     const ActionOccasion &use_type)
// {

// }

// /* Calculates and returns the average speed of the Party */
// int32_t Party::getAverageSpeed()
// {

// }

// /* Returns a vector of indexes of all KO'd party members */
// std::vector<uint32_t> Party::getDeadMembers()
// {

// }

// /* Evaluates and returns a given PartyState flag */
// bool Party::getFlag(const PartyState &test_flag)
// {

// }

// /* Returns the pointer to the current inventory of the Party */
// Inventory* Party::getInventory()
// {

// }

// /* Returns a vector of the indexes of all non-KO'd party members */
// std::vector<uint32_t> Party::getLivingMembers()
// {

// }

// /* Returns the current maximum size of the party */
// uint32_t Party::getMaxSize()
// {

// }

// /* Obtains a ptr to a member of a given index, if the index is valid */
// Person* Party::getMember(const uint8_t &index)
// {

// }

// /* Returns the string name a party member at a given index, if valid */
// std::string Party::getMemberName(const uint8_t &index)
// {

// }

// /* Returns the enumerated type of the Party */
// PartyType Party::getPartyType()
// {

// }

// /* Calculates and returns the total speed of the Party */
// int32_t Party::getTotalSpeed()
// {

// }

// /* Attempts to assign a new maximum size of the Party */
// bool Party::setMaxSize(const uint8_t &new_max_size)
// {

// }

// /* Assigns a new inventory to the Party */
// bool Party::setInventory(Inventory* const new_inventory)
// {

// }

// /* Attempts to assign a new primary member of the Party */
// bool Party::setMainMember(const uint8_t &new_main)
// {

// }

// /* Assigns a given PartyState flag a given value */
// void Party::setFlag(const PartyState &flag, const bool &set_value)
// {

// }

// /*=============================================================================
//  * PUBLIC STATIC FUNCTIONS
//  *============================================================================*/


// /* Returns the total maximum size of members */
// uint8_t Party::getMaxMembers()
// {
//   return kMAX_MEMBERS;
// }

// /* Returns the maximum size of the Bearacks */
// uint8_t Party::getMaxBearacks()
// {
//   return kMAX_MEMBERS_BEARACKS;
// }

// /* Returns the maximum size of the Sleuth */
// uint8_t Party::getMaxSleuth()
// {
//   return kMAX_MEMBERS_SLEUTH;
// }

// /* Returns the maximum size of the foes */
// uint8_t Party::getMaxFoes()
// {
//   return kMAX_MEMBERS_FOES;
// }