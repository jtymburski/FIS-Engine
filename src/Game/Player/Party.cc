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
 * CONSTANTS - See header declaration for details.
 *============================================================================*/

const uint8_t Party::kMAX_MEMBERS_BEARACKS{50};
const uint8_t Party::kMAX_MEMBERS_SLEUTH{5};
const uint8_t Party::kMAX_MEMBERS_FOES{10};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: General party construction. Constructs a Party with a given
 *              ID, main person pointer, an enumerated type, a maximum size
 *              and a pointer to an inventory pouch.
 *
 * Inputs: int32_t my_id - ID of the party to be created.
 *         Person* main - member member to be added to the inventory.
 *         PartyType type - enumerated type of the party.
 *         uint8_t max - maximum size of the party.
 *         Inventory* - pointer to the inventory pouch.
 */
Party::Party(const int32_t &my_id, Person* const main, const PartyType &type, 
    const uint8_t &max, Inventory* const inv)
      : id{my_id}
{
  loadDefaults();

  if (main == nullptr)
    std::cerr << "[Error] Creating party with null main member" << std::endl;
  else
  {
    members.push_back(main);
    party_type = type;
    setMaxSize(max);
    pouch = inv;
  }
}

/*
 * Description: Genertal party construction which constructs a party given an
 *              ID, a vector of Persons, a maximum size and an enumerated
 *              Party type with a pointer to an inventory.
 *
 * Inputs: int32_t my_id - ID of the party to be created.
 *         std::vector<Person*> - pointers to the members of the Party.
 *         uint8_t max - the maximum size of the party.
 *         Inventory* - pointer to the Inventory object.
 */
Party::Party(const int32_t &my_id, std::vector<Person*> members, 
    const uint8_t &max, const PartyType &type, Inventory* const inv)
      : id{my_id}
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

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Loads the default state of the Party, including the default
 *              flags, size, and type, and a null inventory.
 *
 * Inputs: none
 * Output: none
 */
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
 * TODO: Do we need this function? [08-23-14]
 */
/* Implements the battle use effect of a given item (by game_id) */
void Party::battleUseItem(const uint32_t &game_id, const uint8_t &index)
{
  //TODO: Battle use Item function [01-18-14]
  (void)game_id;//warning
  (void)index;//warning
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
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

/*
 * Description: Attempts to add a person to the party.
 *
 * Inputs: Person* - member to be added to the party.
 * Output: bool - true if the person was added successfully.
 */
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

/*
 * Description: Clears all members of the party except for the primary leader.
 *
 * Inputs: none
 * Output: bool- true if the party was cleared succesfully.
 */
bool Party::clearParty()
{
  auto main_member = members.at(0);

  members.clear();
  members.push_back(main_member);

  if (main_member != nullptr)
    return true;

  return false;
}

/*
 * Description: Evaluates whether the current party contains a boss.
 *
 * Inputs: none
 * Output: bool - true if the current party contains a boss.
 */
bool Party::hasBoss()
{
  for (auto it = begin(members); it != end(members); ++it)
    if ((*it)->getPFlag(PState::BOSS))
      return true;

  return false;
}

/*
 * Description: Evaluates whether the current party contains a final boss.
 *
 * Inputs: none
 * Output: bool - true if the Party contains a final boss.
 */
bool Party::hasFinalBoss()
{
  for (auto it = begin(members); it != end(members); ++it)
    if ((*it)->getPFlag(PState::FINAL))
      return true;

  return false;
}

/*
 * Description: Evaluates whether the current party contains a mini-boss.
 *
 * Inputs: none
 * Output: bool - true if the party has a mini-boss.
 */
bool Party::hasMiniBoss()
{
  for (auto it = begin(members); it != end(members); ++it)
    if ((*it)->getPFlag(PState::MINI_BOSS))
      return true;

  return false;
}

/*
 * Description: Determines whether a given person is in the Party.
 *
 * Inputs: Person* - person to be checked whether they are in the party.
 * Output: bool - true if the person is in the party.
 */
bool Party::isInParty(Person* const check_person)
{
  for (auto it = begin(members); it != end(members); ++it)
    if ((*it) == check_person)
      return true;

  return false;
}

/*
 * Description: Prints out the state of the Party.
 *
 * Inputs: const bool simple - whether to print out just simple info 
 *         const bool flags  - whether to print out the value of the flags.
 * Output: none
 */
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

/*
 * Description: Attempts to remove a mmber of the party by a given Index.
 *
 * Inputs: uint8_t - index of party member to be removed
 * Output: bool - true if the removal was successful.
 */
bool Party::removeMember(const uint8_t &index)
{
  if (index < members.size() && members.size() > 1)
  {
    members.erase(begin(members) + index);

    return true;
  }

  return false;
}

/*
 * Description: Attempts to remove a member of the party by a given string name.
 *
 * Inputs: const std::string - name of the party member to be removed.
 * Output: bool - true if the removal was successful.
 */
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

/*
 * Description: Uses an itme (given by a game ID) on a given Index (of party
 *              members) of an enumerated ActionOccasion use type.
 *
 * Inputs: uint32_t - game ID of the Item to be used.
 *         uint8_t  - index of party member to have the item used on them
 * Output: bool - true if the item use was successful.
 * //TODO: Wtf with this function yo. [08-23-14]
 */
bool Party::useItem(const uint32_t &game_id, const uint8_t &index, 
                    const ActionOccasion &use_type)
{
  //TODO: Use item function [01-18-13]
  (void)game_id;//warning
  (void)index;//warning
  (void)use_type;//warning
  return true;//warning
}

/*
 * Description: Returns the size of the party
 *
 * Inputs: none
 * Output: uint32_t - the size of the party
 */
uint32_t Party::getSize()
{
  return members.size();
}

/*
 * Description: Calculates and returns the average speed among members of the
 *              party.
 *
 * Inputs: none
 * Output: int32_t - the average speed of members in the party.
 */
int32_t Party::getAverageSpeed()
{
  return std::floor(getTotalSpeed() / members.size());
}

/*
 * Description: Returns a vector of indexes of all KO'd party members 
 *
 * Inputs: none
 * Output: std::vector<uint32_t> - the indexes of all dead members
 */
std::vector<uint32_t> Party::getDeadMembers()
{
  std::vector<uint32_t> dead_members;

  auto index = 0;
  for (auto it = begin(members); it != end(members); ++it, index++)
    if (!(*it)->getBFlag(BState::ALIVE))
      dead_members.push_back(index);

  return dead_members;
}

/*
 * Description: Evaluates and returns a given PartyState flag
 *
 * Inputs: PartyState test_flag - eumerated flag to grab the value for
 * Output: bool - the value of the flag.
 */
bool Party::getFlag(const PartyState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Returns the pointer to the current inventory object.
 *
 * Inputs: none
 * Output: Inventory* - pointer to the Inventory object.
 */
Inventory* Party::getInventory()
{
  return pouch;
}

/*
 * Description: Returns the vector of indexes along the party which are
 *              living members.
 *
 * Inputs: none
 * Output: std::vector <uint32_t> - living party member indexes.
 */
std::vector<uint32_t> Party::getLivingMembers()
{
  std::vector<uint32_t> living_members;

  auto index = 0;
  for (auto it = begin(members); it != end(members); ++it, index++)
    if ((*it)->getBFlag(BState::ALIVE))
      living_members.push_back(index);

  return living_members;
}

/*
 * Description: Returns the current maximum size of the Party.
 *
 * Inputs: none
 * Output: int32_t - the maximum size for the party.
 */
uint32_t Party::getMaxSize()
{
  return max_size;
}

/*
 * Description: Obtains a pointer to the member person at a given index in the
 *              party.
 *
 * Inputs: uint8_t - index to check member for.
 * Output: Person* - pointer to a the person at the given index (or nullptr)
 */
Person* Party::getMember(const uint8_t &index)
{
  if (index < members.size())
    return members.at(index);

  return nullptr;
}

/*
 * Description: Attempts to return a vector of all members in the party except
 *              a given member. If this member is not found, a vector of all
 *              the members of the party will be found.
 *
 * Inputs: Person* member - the member to be excluded from all members
 * Output: std::vector<Person*> - a vector of members with all but given member
 */
std::vector<Person*> Party::findMembersExcept(Person* const member,
    const bool &only_living)
{
  std::vector<Person*> temp_members;

  for (auto it = begin(members); it != end(members); ++it)
  {
    if (*it != member)
    {
      /* Add the member if they are dead only if only_living is false */
      if (only_living && (*it)->getBFlag(BState::ALIVE))
        temp_members.push_back(*it);
      else if (!only_living)
        temp_members.push_back(*it);
    }
  }

  return temp_members;
}

/*
 * Description: Returns the string name of a party member at a given index, if
 *              it is valid.
 *
 * Inputs: uint8_t - index of the party member to find the name of.
 * Output: std::string - string name of hte member.
 */
std::string Party::getMemberName(const uint8_t &index)
{
  if (index < members.size())
    return members.at(index)->getName();

  return "";
}

/*
 * Description: Returns the enumerated PartyType of the Party (ex. sleuth,
  *             bearacks, etc.)
 *
 * Inputs: none
 * Output: PartyType - enumerated party type.
 */
PartyType Party::getPartyType()
{
  return party_type;
}

/*
 * Description: Calculates and returns the total speed of all the party members.
 *
 * Inputs: none
 * Output: int64_t the calculated total speed of the Party.
 */
/* Calculates and returns the total speed of the Party */
int64_t Party::getTotalSpeed()
{
  int64_t total_speed{0};

  for (auto it = begin(members); it != end(members); ++it)
    total_speed += (*it)->getCurr().getStat(Attribute::MMNT);

  return total_speed;
}

/*
 * Description: Attempts to assign a new maximum size of the Party.
 *
 * Inputs: const uin8_t - new maximum size for the Party.
 * Output: bool - true if the maximum size was assigned succesfully
 */
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

/*
 * Description: Attempts to assign a new inventory for the Party.
 *
 * Inputs: Inventory* - new inventory for the Party.
 * Output: bool- true if the inventory assignment was successful.
 */
bool Party::setInventory(Inventory* const new_inventory)
{
  if (new_inventory != nullptr)
  {
    pouch = new_inventory;
    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new main (leader) of the Party.
 *
 * Inputs: uin8_t - index of party member to be assigned as the leader.
 * Output: bool - true if a main member was successfully assigned
 */
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

/*
 * Description: Assigns a given PartyState flag a given Boolean vale.
 *
 * Inputs: PartyState flag - flag to be assigned to a value.
 *         const bool set_value - value to be assigned to the flag.
 * Output: none
 */
void Party::setFlag(const PartyState &flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the static maximum size of the Bearacks.
 *
 * Inputs: none
 * Output: uint8_t - the static maximum size of the Bearacks
 */
/* Returns the maximum size of the Bearacks */
uint8_t Party::getMaxBearacks()
{
  return kMAX_MEMBERS_BEARACKS;
}

/*
 * Description: Retuns the static maximum size of the sleuth.
 *
 * Inputs: none
 * Output: uint8_t - the static maximum size of the sleuth.
 */
uint8_t Party::getMaxSleuth()
{
  return kMAX_MEMBERS_SLEUTH;
}

/*
 * Description: Retunrs the static maximum size of a foes party.
 *
 * Inputs: none
 * Output: uint8_t - the static maximum size of the foes.
 */
uint8_t Party::getMaxFoes()
{
  return kMAX_MEMBERS_FOES;
}