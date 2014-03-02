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

const uint8_t Party::kMAX_MEMBERS_BEARACKS{50};
const uint8_t Party::kMAX_MEMBERS_SLEUTH{5};
const uint8_t Party::kMAX_MEMBERS_FOES{10};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Party::Party(Person* const main, const PartyType &type, const uint8_t &max,
             Inventory* const inv)
{
  loadDefaults();

  if (main == nullptr)
    std::cerr << "Error: Creating party with null main member\n";

  members.push_back(main);
  party_type = type;
  setMaxSize(max);
  pouch = inv;
}

/*
 * Description:
 *
 * Inputs:
 */
Party::Party(std::vector<Person*> members, const uint8_t &max, 
             const PartyType &type, Inventory* const inv)
{
  loadDefaults();

  for (auto member : members)
  {
    if (member == nullptr)
      std::cerr << "Error: Creating party with null members\n";
    if (members.size() < max_size)
      members.push_back(member);
    else
      std::cerr << "Error: Creating party with too large of size\n";
  }

  party_type = type;
  setMaxSize(max);
  pouch = inv;
}

/*
 * Description:
 */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Party::loadDefaults()
{
  flags = static_cast<PartyState>(0);
  max_size = kMAX_MEMBERS_SLEUTH;
  party_type = PartyType::REGULAR_FOE;
  pouch = nullptr;

  setFlag(PartyState::CAN_ADD_MEMBERS, true);
  setFlag(PartyState::CAN_REMOVE_MEMBERS, true);
  setFlag(PartyState::ITEM_USE_ENABLED, true);
  setFlag(PartyState::CAN_ADD_ITEMS, true);
  setFlag(PartyState::CAN_REMOVE_ITEMS, true);
  setFlag(PartyState::ENCOUNTERS_ENABLED, true);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
/* Implements the battle use effect of a given item (by game_id) */
void Party::battleUseItem(const uint32_t &game_id, const uint8_t &index)
{
  //TODO: Battle use Item function [01-18-14]
  (void)game_id;//warning
  (void)index;//warning
}

/* Implements the menu use effect of a given item (by game_id) */
void Party::menuUseItem(const uint32_t &game_id, const uint8_t &index)
{
  //TODO: Menu use Item function [01-18-14]
  (void)game_id;//warning
  (void)index;//warning
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Attempts to add a person to the party */
bool Party::addMember(Person* const new_member)
{
  if (getFlag(PartyState::CAN_ADD_MEMBERS))
  {
    if (members.size() < max_size)
    {
      if (new_member != nullptr)
      {
        members.push_back(new_member);

        return true;
      }
    }
  }

  return false;
}

/* Clears all members of the party except the primary member */
bool Party::clearParty()
{
  auto main_member = members.at(0);

  members.clear();
  members.push_back(main_member);

  if (main_member != nullptr)
    return true;

  return false;
}

/* Evaluates whether the current party contains a boss */
bool Party::hasBoss()
{
  for (auto it = begin(members); it != end(members); ++it)
    if ((*it)->getPFlag(PState::BOSS))
      return true;

  return false;
}

/* Evaluates whether the current party contains a final boss */
bool Party::hasFinalBoss()
{
  for (auto it = begin(members); it != end(members); ++it)
    if ((*it)->getPFlag(PState::FINAL))
      return true;

  return false;
}

/* Evaluates whether the current party contains a mini boss */
bool Party::hasMiniBoss()
{
  for (auto it = begin(members); it != end(members); ++it)
    if ((*it)->getPFlag(PState::MINI_BOSS))
      return true;

  return false;
}

/* Prints out the state of the Party */
void Party::print(const bool &simple, const bool &flags)
{
  std::cout << "=== Party ===\n";
  std::cout << "# Members: " << members.size() << "\n";
  std::cout << "Max Size: " << (int)max_size << "\n";
  std::cout << "Party State:[VOID]" << "\n";
  std::cout << "Pouch Assigned? " << (pouch != nullptr) << "\n";

  if (!simple)
  {
    std::cout << "----------\n";
    for (auto member : members)
      std::cout << "Member: " << member->getName() << "\n";

  
    std::cout << "Average Speed: " << getAverageSpeed() << "\n";
    std::cout << "Total Speed: " << getTotalSpeed() << "\n";
    std::cout << "Has Boss?: " << hasBoss() << "\n";
    std::cout << "Has Final Boss?: " << hasFinalBoss() << "\n";
    std::cout << "Has Mini Boss?: " << hasMiniBoss() << "\n";
    std::cout << "# Dead Members: " << getDeadMembers().size() << "\n";
    std::cout << "# Living Members: " << getLivingMembers().size() << "\n\n";
  }

  if (flags)
  {
    std::cout << "ADD: " << getFlag(PartyState::CAN_ADD_MEMBERS);
    std::cout << "\nREMOVE: " << getFlag(PartyState::CAN_REMOVE_MEMBERS);
    std::cout << "\nITEM USE: " << getFlag(PartyState::ITEM_USE_ENABLED);
    std::cout << "\nADD ITEMS: " << getFlag(PartyState::CAN_ADD_ITEMS);
    std::cout << "\nREMOVE ITEMS: " << getFlag(PartyState::CAN_REMOVE_ITEMS);
    std::cout << "ENCOUNTERS: " << getFlag(PartyState::ENCOUNTERS_ENABLED);
    std::cout << "\n";
  }
}

/* Attempts to a remove a member of the party by a given index */
bool Party::removeMember(const uint8_t &index)
{
  if (index < members.size() && members.size() > 1)
  {
    members.erase(begin(members) + index);

    return true;
  }

  return false;
}

/* Attempts to remove a member of the party by a given string name */
bool Party::removeMember(const std::string &name)
{
  if (members.size() == 1)
    return false;

  for (auto it = begin(members); it != end(members); ++it)
  {
    if ((*it)->getName() == name)
    {
      members.erase(it);

      return true;
    }
  }

  return false;
}

/* Uses an item (given by game_id) on a given member (index) of a use type */
bool Party::useItem(const uint32_t &game_id, const uint8_t &index, 
                    const ActionOccasion &use_type)
{
  //TODO: Use item function [01-18-13]
  (void)game_id;//warning
  (void)index;//warning
  (void)use_type;//warning
  return true;//warning
}

/* Return the size of the party */
uint32_t Party::getSize()
{
  return members.size();
}

/* Calculates and returns the average speed of the Party */
int32_t Party::getAverageSpeed()
{
  return (getTotalSpeed() / members.size());
}

/* Returns a vector of indexes of all KO'd party members */
std::vector<uint32_t> Party::getDeadMembers()
{
  std::vector<uint32_t> dead_members;

  auto index = 0;
  for (auto it = begin(members); it != end(members); ++it, index++)
    if (!(*it)->getBFlag(BState::ALIVE))
      dead_members.push_back(index);

  return dead_members;
}

/* Evaluates and returns a given PartyState flag */
bool Party::getFlag(const PartyState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/* Returns the pointer to the current inventory of the Party */
Inventory* Party::getInventory()
{
  return pouch;
}

/* Returns a vector of the indexes of all non-KO'd party members */
std::vector<uint32_t> Party::getLivingMembers()
{
  std::vector<uint32_t> living_members;

  auto index = 0;
  for (auto it = begin(members); it != end(members); ++it, index++)
    if ((*it)->getBFlag(BState::ALIVE))
      living_members.push_back(index);

  return living_members;
}

/* Returns the current maximum size of the party */
uint32_t Party::getMaxSize()
{
  return max_size;
}

/* Obtains a ptr to a member of a given index, if the index is valid */
Person* Party::getMember(const uint8_t &index)
{
  if (index < members.size())
    return members.at(index);

  return nullptr;
}

/* Returns the string name a party member at a given index, if valid */
std::string Party::getMemberName(const uint8_t &index)
{
  if (index < members.size())
    return members.at(index)->getName();

  return "";
}

/* Returns the enumerated type of the Party */
PartyType Party::getPartyType()
{
  return party_type;
}

/* Calculates and returns the total speed of the Party */
int64_t Party::getTotalSpeed()
{
  int64_t total_speed{0};

  for (auto it = begin(members); it != end(members); ++it)
    total_speed += (*it)->getCurr().getStat(Attribute::MMNT);

  return total_speed;
}

/* Attempts to assign a new maximum size of the Party */
bool Party::setMaxSize(const uint8_t &new_max_size)
{
  auto limit = 0;

  if (party_type == PartyType::SLEUTH)
    limit = kMAX_MEMBERS_SLEUTH;
  else if (party_type == PartyType::BEARACKS)
    limit = kMAX_MEMBERS_BEARACKS;
  else
    limit = kMAX_MEMBERS_FOES;

  if (new_max_size <= limit && members.size() <= new_max_size)
  {
    max_size = new_max_size;

    return true;
  }

  return false;
}

/* Assigns a new inventory to the Party */
bool Party::setInventory(Inventory* const new_inventory)
{
  if (new_inventory != nullptr)
  {
    pouch = new_inventory;
    return true;
  }

  return false;
}

/* Attempts to assign a new primary member of the Party */
bool Party::setMainMember(const uint8_t &new_main)
{
  if (new_main != 0 && new_main < members.size())
  {
    auto old_main = members.at(0);

    members[0]        =  members.at(new_main);
    members[new_main] = old_main;

    return true;
  }

  return false;
}

/* Assigns a given PartyState flag a given value */
void Party::setFlag(const PartyState &flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/* Returns the maximum size of the Bearacks */
uint8_t Party::getMaxBearacks()
{
  return kMAX_MEMBERS_BEARACKS;
}

/* Returns the maximum size of the Sleuth */
uint8_t Party::getMaxSleuth()
{
  return kMAX_MEMBERS_SLEUTH;
}

/* Returns the maximum size of the foes */
uint8_t Party::getMaxFoes()
{
  return kMAX_MEMBERS_FOES;
}