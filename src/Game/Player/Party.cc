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

const uint32_t Party::kID_BEARACKS{1};
const uint32_t Party::kID_SLEUTH{0};
const uint16_t Party::kMAX_MEMBERS_BEARACKS{50};
const uint16_t Party::kMAX_MEMBERS_SLEUTH{5};
const uint16_t Party::kMAX_MEMBERS_FOES{5};
const int32_t Party::kUNSET_ID{-1};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: The blank constructor.
 *
 * Inputs: none
 */
Party::Party() : id{kUNSET_ID}
{
  loadDefaults();
  createInventory();
}

/*
 * Description: General party construction. Constructs a Party with a given
 *              ID, main person pointer, an enumerated type, a maximum size
 *              and a pointer to an inventory pouch.
 *
 * Inputs: int32_t my_id - ID of the party to be created.
 *         Person* main - member member to be added to the inventory.
 *         PartyType type - enumerated type of the party.
 *         uint16_t max - maximum size of the party.
 */
Party::Party(const int32_t& my_id, Person* const main, const PartyType& type,
             const uint16_t& max)
    : id{my_id}
{
  assert(main);
  loadDefaults();

  members.push_back(main);

  party_type = type;
  setMaxSize(max);
  createInventory();
}

/*
 * Description: Genertal party construction which constructs a party given
 *an
 *              ID, a vector of Persons, a maximum size and an enumerated
 *              Party type with a pointer to an inventory.
 *
 * Inputs: int32_t my_id - ID of the party to be created.
 *         std::vector<Person*> - pointers to the members of the Party.
 *         uint16_t max - the maximum size of the party.
 */
Party::Party(const int32_t& my_id, std::vector<Person*> members,
             const uint16_t& max, const PartyType& type)
    : id{my_id}
{
  assert(members.size() < max_size);
  assert(members.size() > 0);
  assert(members.at(0));

  loadDefaults();

  for(const auto& member : members)
    members.push_back(member);

  party_type = type;
  setMaxSize(max);
  createInventory();
}

Party::~Party()
{
  if(pouch != nullptr)
    delete pouch;

  pouch = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Copies all data from source object to this object.
 * Notes: NOT FULLY COMPLETE. Just enough data required for Editor. TODO?
 *
 * Inputs: Party &source - the source to copy from
 * Output: none
 */
void Party::copySelf(const Party& source)
{
  flags = source.flags;

  members = source.members;
  reserve_members = source.reserve_members;

  max_size = source.max_size;

  party_type = source.party_type;

  //*pouch = *source.pouch;
}

void Party::createInventory()
{
  if(pouch != nullptr)
    delete pouch;

  // TODO: Method to determine name of inventory? [08-17-15]
  // TODO: Thumbnail for inventory. [08-17-15]
  pouch = new Inventory(id);
}

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
  setFlag(PartyState::HAS_BOND, false);
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
  if(getFlag(PartyState::CAN_ADD_MEMBERS) && members.size() < max_size &&
     new_member)
  {
    members.push_back(new_member);

    /* Ensure member is set up properly */
    setPartyType(party_type);

    return true;
  }

  return false;
}

/*
 * Description: Clears all members of the party except for the primary
 *leader.
 *
 * Inputs: none
 * Output: bool - true if the party was cleared succesfully.
 */
bool Party::clearParty()
{
  if(members.at(0))
  {
    members.clear();
    members.push_back(members.at(0));

    return true;
  }

  return false;
}

//TODO Reserve members?
void Party::restorePartyVita()
{
  for(auto& member : members)
    if(member)
      member->restoreHealth();
}

//TODO Reserve members?
void Party::restorePartyQtdr()
{
  for(auto& member : members)
    if(member)
      member->restoreQtdr();
}

/*
 * Description: Determines whether a given person is in the Party.
 *
 * Inputs: Person* - person to be checked whether they are in the party.
 * Output: bool - true if the person is in the party.
 */
bool Party::isInParty(Person* const check_person)
{
  for(const auto& member : members)
    if(member == check_person)
      return true;

  return false;
}

/*
 * Description: Loads the data from file associated with the category.
 *
 * Inputs: XmlData data - the xml data structure
 *         int index - the element reference index
 *         SDL_Renderer* renderer - the rendering engine
 * Output: bool - true if load was successful
 */
bool Party::loadData(XmlData data, int index, SDL_Renderer* renderer,
                     std::string base_path)
{
  bool success = true;

  /* ---- INVENTORY ---- */
  if(data.getElement(index) == "inventory")
  {
    if(pouch != nullptr)
      success &= pouch->loadData(data, index + 1, renderer, base_path);
    else
      success = false;
  }
  /* ---- PERSON: MAIN OR RESERVE ---- */
  else if(data.getElement(index) == "person" ||
          data.getElement(index) == "reserve")
  {
    uint16_t ref_index = 0;

    /* Update the index if valid */
    if(!data.getKeyValue(index).empty())
    {
      try
      {
        int new_index = std::stoi(data.getKeyValue(index));
        if(new_index >= 0)
          ref_index = new_index;
      }
      catch (std::exception&)
      {}
    }

    /* Parse index: if valid */
    Person* ref_person = nullptr;
    if(data.getElement(index) == "person")
    {
      if(ref_index < static_cast<int>(members.size()))
        ref_person = members[ref_index];
    }
    else
    {
      if(ref_index < static_cast<int>(reserve_members.size()))
        ref_person = reserve_members[ref_index];
    }

    /* If reference person found, load data */
    if(ref_person != nullptr)
    {
      success &= ref_person->loadData(data, index + 1, renderer, base_path);
    }
  }
  /* ---- TYPE ---- */
  else if(data.getElement(index) == "type")
  {
    success &=
        setPartyType(Helpers::partyTypeFromStr(data.getDataString(&success)));
  }
  // TODO: FUTURE - PartyState flags editable?

  return success;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Party::moveMemberToReserve(Person* test_member)
{
  auto success = false;

  if(test_member)
  {
    for(auto it = begin(members); it != end(members); ++it)
    {
      if((*it) == test_member)
      {
        members.erase(it);
        reserve_members.push_back(test_member);
        success = true;
      }
    }
  }

  return success;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Party::moveReserveMember(Person* test_member)
{
  auto success = false;

  if(test_member)
  {
    for(auto it = begin(reserve_members); it != end(members); ++it)
    {
      if((*it) == test_member)
      {
        reserve_members.erase(it);
        members.push_back(test_member);
        success = true;
      }
    }
  }

  return success;
}

/*
 * Description: Prints out the state of the Party.
 *
 * Inputs: const bool simple - whether to print out just simple info
 *         const bool flags  - whether to print out the value of the flags.
 * Output: none
 */
void Party::print(const bool& simple, const bool& flags)
{
  std::cout << "=== Party ===\n";
  std::cout << "ID: " << id << "\n";
  std::cout << "# Members: " << members.size() << "\n";
  std::cout << "Max Size: " << (int)max_size << "\n";
  std::cout << "Pouch Assigned? " << (pouch != nullptr) << "\n";

  if(!simple)
  {
    std::cout << "----- Members -----\n";

    for(const auto& member : members)
      std::cout << "Member: " << member->getMyID() << " - " << member->getName()
                << ":" << member->getLevel() << "\n";
    std::cout << "----- Reserve Members -----\n";
    for(const auto& member : reserve_members)
      std::cout << "Reserve Member: " << member->getMyID() << " - "
                << member->getName() << ":" << member->getLevel() << "\n";

    std::cout << "--------" << std::endl;

    if(pouch != nullptr)
      pouch->print(false);
  }

  if(flags)
  {
    std::cout << "ADD: " << getFlag(PartyState::CAN_ADD_MEMBERS);
    std::cout << "\nREMOVE: " << getFlag(PartyState::CAN_REMOVE_MEMBERS);
    std::cout << "\nITEM USE: " << getFlag(PartyState::ITEM_USE_ENABLED);
    std::cout << "\nADD ITEMS: " << getFlag(PartyState::CAN_ADD_ITEMS);
    std::cout << "\nREMOVE ITEMS: " << getFlag(PartyState::CAN_REMOVE_ITEMS);
    std::cout << "\nENCOUNTERS: " << getFlag(PartyState::ENCOUNTERS_ENABLED);
    std::cout << "\n";
  }
}

/*
 * Description: Attempts to remove a mmber of the party by a given Index.
 *
 * Inputs: uint8_t - index of party member to be removed
 * Output: bool - true if the removal was successful.
 */
bool Party::removeMember(const uint8_t& index)
{
  if(index < members.size() && members.size() > 1)
  {
    members.erase(begin(members) + index);

    return true;
  }

  return false;
}

/*
 * Description: Attempts to remove a member of the party by a given string
 *name.
 *
 * Inputs: const std::string - name of the party member to be removed.
 * Output: bool - true if the removal was successful.
 */
bool Party::removeMember(const std::string& name)
{
  if(members.size() == 1)
    return false;

  for(auto it = begin(members); it != end(members); ++it)
  {
    if((*it)->getName() == name)
    {
      members.erase(it);

      return true;
    }
  }

  return false;
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
 * Description: Evaluates and returns a given PartyState flag
 *
 * Inputs: PartyState test_flag - eumerated flag to grab the value for
 * Output: bool - the value of the flag.
 */
bool Party::getFlag(const PartyState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Returns the ID for the party.
 *
 * Inputs: none
 * Output: int32_t - the party id
 */
int32_t Party::getID() const
{
  return id;
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
 * Description: Obtains a pointer to the member person at a given index in
 *the
 *              party.
 *
 * Inputs: uint8_t - index to check member for.
 * Output: Person* - pointer to a the person at the given index (or nullptr)
 */
Person* Party::getMember(const uint32_t& index)
{
  if(index < members.size())
    return members.at(index);

  return nullptr;
}

/*
 * Description: Returns the string name of a party member at a given index,
 *if
 *              it is valid.
 *
 * Inputs: uint32_t - index of the party member to find the name of.
 * Output: std::string - string name of hte member.
 */
std::string Party::getMemberName(const uint32_t& index)
{
  if(index < members.size())
    return members.at(index)->getName();

  return "";
}

/*
 * Description: Returns the stack of all persons within the party.
 *
 * Inputs: none
 * Output: std::vector<Person*> - stack of persons
 */
std::vector<Person*> Party::getMembers()
{
  return members;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::vector<Person*> Party::getReserveMembers()
{
  return reserve_members;
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
 * Description: Saves the data of this party to the file handler pointer.
 *
 * Inputs: FileHandler* fh - the saving file handler
 *         std::string wrapper - the text to wrap the xml data in. default is
 *                               blank and unused
 * Output: bool - true if successful
 */
bool Party::saveData(FileHandler* fh, std::string wrapper)
{
  if(fh != nullptr)
  {
    /* Start wrapper */
    if(!wrapper.empty())
      fh->writeXmlElement(wrapper);

    /* Write data - persons */
    for(uint16_t i = 0; i < members.size(); i++)
    {
      fh->writeXmlElement("person", "index", i);
      members[i]->saveData(fh);
      fh->writeXmlElementEnd();
    }

    /* Write data - reserve persons */
    for(uint16_t i = 0; i < reserve_members.size(); i++)
    {
      fh->writeXmlElement("reserve", "index", i);
      reserve_members[i]->saveData(fh);
      fh->writeXmlElementEnd();
    }

    /* Write data - inventory */
    if(pouch != nullptr)
    {
      pouch->saveData(fh);
    }

    /* End wrapper */
    if(!wrapper.empty())
      fh->writeXmlElementEnd();

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
void Party::setFlag(const PartyState& flag, const bool& set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description: Sets the party ID
 *
 * Inputs: int id - the id
 * Output: none
 */
void Party::setID(int id)
{
  if(id < 0)
    this->id = kUNSET_ID;
  else
    this->id = id;
}

/*
 * Description: Attempts to assign a new inventory for the Party.
 *
 * Inputs: Inventory* - new inventory for the Party.
 * Output: bool- true if the inventory assignment was successful.
 */
bool Party::setInventory(Inventory* const new_inventory)
{
  if(pouch != nullptr)
    delete pouch;

  pouch = new_inventory;

  return (pouch != nullptr);
}

/*
 * Description: Attempts to assign a new main (leader) of the Party.
 *
 * Inputs: uin8_t - index of party member to be assigned as the leader.
 * Output: bool - true if a main member was successfully assigned
 */
bool Party::setMainMember(const uint32_t& new_main)
{
  if(new_main != 0 && new_main < members.size())
  {
    auto old_main = members.at(0);

    members[0] = members.at(new_main);
    members[new_main] = old_main;

    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new maximum size of the Party.
 *
 * Inputs: const uin8_t - new maximum size for the Party.
 * Output: bool - true if the maximum size was assigned succesfully
 */
bool Party::setMaxSize(const uint32_t& new_max_size)
{
  uint32_t limit = 0;

  if(party_type == PartyType::SLEUTH)
    limit = kMAX_MEMBERS_SLEUTH;
  else if(party_type == PartyType::BEARACKS)
    limit = kMAX_MEMBERS_BEARACKS;
  else
    limit = kMAX_MEMBERS_FOES;

  if(new_max_size <= limit && members.size() <= new_max_size)
  {
    max_size = new_max_size;

    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new type of the Party.
 *
 * Inputs: PartyType type - the new type of the party
 * Output: bool - true if the type was set successfully
 */
bool Party::setPartyType(const PartyType& type)
{
  bool success = true;

  /* Set type */
  party_type = type;

  /* Loop through all persons and set the type */
  for(uint32_t i = 0; i < members.size(); i++)
    members[i]->setPFlag(type);
  for(uint32_t i = 0; i < reserve_members.size(); i++)
    reserve_members[i]->setPFlag(type);

  /* Clear inventory */
  pouch->setFlag(InvState::PLAYER_STORAGE | InvState::SHIP_STORAGE |
                     InvState::ENEMY_STORAGE | InvState::SHOP_STORAGE,
                 false);

  /* Set size based on type */
  if(type == PartyType::SLEUTH)
  {
    success &= setMaxSize(kMAX_MEMBERS_SLEUTH);
    pouch->setFlag(InvState::PLAYER_STORAGE, true);
    pouch->setFlag(InvState::UPGRADEABLE, true);
  }
  else if(type == PartyType::BEARACKS)
  {
    success &= setMaxSize(kMAX_MEMBERS_BEARACKS);
    pouch->setFlag(InvState::SHIP_STORAGE, true);
    pouch->setFlag(InvState::UPGRADEABLE, true);
  }
  else
  {
    success &= setMaxSize(kMAX_MEMBERS_FOES);
    pouch->setFlag(InvState::ENEMY_STORAGE, true);
    pouch->setFlag(InvState::UPGRADEABLE, false);
  }

  return success;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              Party.
 *
 * Inputs: const Party &source - the source class constructor
 * Output: Party& - pointer to the copied class
 */
Party& Party::operator=(const Party& source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the static maximum size of the Bearacks.
 *
 * Inputs: none
 * Output: uint32_t - the static maximum size of the Bearacks
 */
/* Returns the maximum size of the Bearacks */
uint32_t Party::getMaxBearacks()
{
  return kMAX_MEMBERS_BEARACKS;
}

/*
 * Description: Retuns the static maximum size of the sleuth.
 *
 * Inputs: none
 * Output: uint32_t - the static maximum size of the sleuth.
 */
uint32_t Party::getMaxSleuth()
{
  return kMAX_MEMBERS_SLEUTH;
}

/*
 * Description: Retunrs the static maximum size of a foes party.
 *
 * Inputs: none
 * Output: uint32_t - the static maximum size of the foes.
 */
uint32_t Party::getMaxFoes()
{
  return kMAX_MEMBERS_FOES;
}
