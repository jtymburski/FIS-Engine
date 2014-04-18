/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: February 23rd, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Battle/Buffer.h"

/*=============================================================================
 * CONSTANTS - See .h for details
 *============================================================================*/

const uint16_t Buffer::kMAXIMUM_ELEMENTS{40};
const uint16_t Buffer::kMAXIMUM_TARGETS{10};
const uint16_t Buffer::kMAXIMUM_COOLDOWN{10};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a Buffer object
 *
 * Inputs: none
 */
Buffer::Buffer()
  : index{0}
{
#ifdef UDEBUG
  std::cout << "hahaha we're debugging" << std::endl;
#endif
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Checks the validity of a given BufferAction and returns the 
 *              determination of the validity.
 *
 * Inputs: elm - ref. to a BufferAction to check the validity of
 * Output: bool - the validity of the element
 */
bool Buffer::checkValid(BufferAction& elm)
{
  auto is_valid = true;

  /* Assert the user is not null, the targets are not empty */
  is_valid &= !(elm.user == nullptr);
  is_valid &= !elm.targets.empty();
  is_valid &= elm.targets.size() < kMAXIMUM_TARGETS;

  /* Assign a valid value to the cooldown */
  Helpers::setInRange(elm.cooldown, static_cast<uint16_t>(0), 
                      kMAXIMUM_COOLDOWN);

  if (is_valid)
  {
    /* Assert the user of the action is in battle and is alive */
    is_valid &= elm.user->getBFlag(BState::ALIVE);
    is_valid &= elm.user->getBFlag(BState::IN_BATTLE);

    /* Iterate through each target asserting they are alive and in battle */
    for (auto it = begin(elm.targets); it != end(elm.targets); ++it)
    {
      is_valid &= (*it)->getBFlag(BState::ALIVE);
      is_valid &= (*it)->getBFlag(BState::IN_BATTLE);
    }

    if (elm.type == BufferType::SKILL)
    {
      /* Action skill must not be null and skill use must be enabled */
      is_valid &= !(elm.skill_used == nullptr);
      is_valid &= elm.user->getBFlag(BState::SKL_ENABLED);
    }
    else if (elm.type == BufferType::ITEM)
    {
      /* Action item must not be null and item use must be enabled */
      is_valid &= !(elm.item_used == nullptr);
      is_valid &= elm.user->getBFlag(BState::ITM_ENABLED);
    }
    else
    {
     is_valid = false;
    }
  }

  elm.valid = is_valid;

  return is_valid;
}

/*
 * Description: Decrements the cooldown of a BufferAction at a given element.
 *
 * Inputs: index - the index of the Buffer to cooldown.
 * Output: bool - true if the index was found and cooldown took place
 */
bool Buffer::decrementCooldown(const uint32_t &index)
{
  if (index < action_buffer.size())
  {
    if (getIndex(index).cooldown != 0)
    {
      getIndex(index).cooldown--;

      return true;
    }
  }

  return false;
}

/*
 * Description: Returns the ref to the element at a given index
 *
 * Inputs: index - the index of the Buffer to grab
 * Output: BufferAction& - the reference at the given index
 */
BufferAction& Buffer::getIndex(const uint32_t &index)
{
  if (index < action_buffer.size())
    return action_buffer.at(index);;
 
  return action_buffer.at(0);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Creates and adds a new Skill BufferAction element given params
 *
 * Inputs: user - Pointer to the user of the action
 *         skill_used - Pointer to the skill being used in the action
 *         targets - A vector of targets which the action will hit
 *         cooldown - The number of turns the action will take to perform
 * Output: bool - true if the addition takes places successfully
 */
bool Buffer::add(Person* const user, Skill* const skill_used, 
                 std::vector<Person*> targets, const uint32_t &cooldown)
{
  BufferAction new_elm;

  new_elm.cooldown   = cooldown;
  new_elm.user       = user;
  new_elm.item_used  = nullptr;
  new_elm.skill_used = skill_used;
  new_elm.targets    = targets;
  new_elm.type       = BufferType::SKILL;

  auto add = checkValid(new_elm);

  if (add)
  {
    action_buffer.push_back(new_elm);

    return true;
  }

  return false;
}

/*
 * Description: Creates and adds a new Skill BufferAction element given params
 *
 * Inputs: user - Pointer to the user of the action
 *         skill_used - Pointer to the skill being used in the action
 *         targets - A vector of targets which the action will hit
 *         cooldown - The number of turns the action will take to perform
 * Output: bool - true if the addition takes places successfully
 */
bool Buffer::add(Person* const user, Item* const item_used, 
                 std::vector<Person*> targets, const uint32_t &cooldown)
{  
  BufferAction new_elm;

  new_elm.cooldown   = cooldown;
  new_elm.user       = user;
  new_elm.item_used  = item_used;
  new_elm.skill_used = nullptr;
  new_elm.targets    = targets;
  new_elm.type       = BufferType::ITEM;

  return add(new_elm);
}

/*
 * Description: Adds a given BufferAction element into the Buffer
 *
 * Inputs: action - the BufferAction to be added
 * Output: bool - true if the BufferAction was added successfully
 */
bool Buffer::add(BufferAction &action)
{
  auto add = checkValid(action);
  add &= (action.cooldown <= kMAXIMUM_COOLDOWN);
  add &= (action.targets.size() <= kMAXIMUM_TARGETS);

  if (add)
  {
    action_buffer.push_back(action);

    return true;
  }
  else
  {
    std::cerr << "[Cerr] Adding invalid action buffer. \n";
  }

  return false;
}

/*
 * Description: Clears all the elements of the Buffer
 *
 * Inputs: none
 * Output: none
 */
void Buffer::clearAll()
{
  action_buffer.clear();
  index = 0;
}

/*
 * Description: Iterates through and removes all invalid BufferElements.
 *
 * Inputs: none
 * Output: none
 */
void Buffer::clearInvalid()
{
  std::vector<BufferAction> temp;

  for (auto it = begin(action_buffer); it != end(action_buffer); ++it)
    if (checkValid(*it))
      temp.push_back(*it);

  action_buffer = temp;
  index  = 0;
}

/*
 * Description: Checks if the next element of the Buffer is valid. 
 *
 * Inputs: none
 * Output: bool - true if the next index of the Buffer is valid
 */
bool Buffer::isNextValid()
{
  if (index + 1 < action_buffer.size())
    return checkValid(getIndex(index + 1));

  return false;
}

/*
 * Description: Prints out the information of the Buffer
 *
 * Inputs: simple - if a simple version of the Buffer is to be printed
 * Output: none
 */
void Buffer::print(const bool &simple)
{
  std::cout << " --- Buffer --- ";
  std::cout << "Size: " << action_buffer.size() << " I:" << index << "\n";

  if (!simple)
  {
    for (auto it = begin(action_buffer); it != end(action_buffer); ++it)
    {
      if ((*it).type == BufferType::SKILL)
      {
        if ((*it).skill_used != nullptr)
          std::cout << "Skill Name: " << (*it).skill_used->getName() << "\n";
        else
          std::cout << "[Warning]: Skill buffer has null skill\n";
      }
      else if ((*it).type == BufferType::ITEM)
      {
        if ((*it).item_used != nullptr)
          std::cout << "Item Name: " << (*it).item_used->getName() << "\n";
        else
          std::cout << "[Warning]: Item buffer has null skill\n";
      }
      else if ((*it).type  == BufferType::NONE)
        std::cout << "[Warning]: No buffer type set\n";

      std::cout << "Cooldown: " << (*it).cooldown << "\n";

      if ((*it).user != nullptr)
        std::cout << "User: " << (*it).user->getName() << "\n";
      else
        std::cout << "User is nullptr\n";

      for (auto jt = begin((*it).targets); jt != end((*it).targets); ++jt)
      {
        if ((*jt) != nullptr)
          std::cout << "Target: " << (*jt)->getName() << "\n";
        else
          std::cout << "Target is nullptr\n";
      }

      std::cout << "Valid: " << (*it).valid;
    }
  }
}

/*
 * Description: Removes a Buffer element at a given index if the index was found
 *
 * Inputs: index - the index of the Buffer to locate
 * Output: bool - true if the index was found and removed
 */
bool Buffer::remove(const uint32_t &index)
{
  if (index < action_buffer.size())
  {
    action_buffer.erase(action_buffer.begin() + index);

    return true;
  }

  return false;
}

/*
 * Description: Removes all the elements of the buffer of the given Person
 *
 * Inputs: user - the user to remove elements for
 * Output: bool - true if an element was found and removed
 */
bool Buffer::removeAllByUser(Person* user)
{
  auto removed = false;

  if (user == nullptr)
    return false;

  for (auto it = begin(action_buffer); it != end(action_buffer); ++it)
  {
    if ((*it).user == user)
    {
      action_buffer.erase(it);

      removed = true;
    }
  }

  return removed;
}

/*
 * Description: Updates the buffer for a turn by decrementing the cooldown
 *              of each buffer and removing each elemenet with a cooldown of 0.
 *
 * Inputs: bool - true if the buffer is to be cleared entirely
 * Output: none
 */
void Buffer::update(const bool &clear)
{
  clearInvalid();

  if (clear)
    clearAll();
  else
  {
    for (uint32_t i = 0; i < action_buffer.size(); i++)
    {
      decrementCooldown(i);

      if (getIndex(i).cooldown == 0)
        remove(i);
    }
  }
}

/*
 * Description: Returns the pointer to the person of the current index's user.
 *
 * Inputs: none 
 * Output: Person* - ptr to the element at current index's user
 */
Person* Buffer::getUser()
{
  if (checkValid(getIndex(index)))
    return getIndex(index).user;

  return nullptr;
}

/*
 * Description: Returns the pointer to the current index's skill 
 *
 * Inputs: none
 * Output: Skill* - ptr to the current index's skill
 */
Skill* Buffer::getSkill()
{
  if (checkValid(getIndex(index)))
    return getIndex(index).skill_used;

  return nullptr;
}

/*
 * Description: Returns the vector of targets for the current index's
 *
 * Inputs: none
 * Output: std::vector<Person*> - returns the vector of targets for the Buffer
 */
std::vector<Person*> Buffer::getTargets()
{
  if (checkValid(getIndex(index)))
    return getIndex(index).targets;

  return {};
}

/*
 * Description: Attempts to assign the next index of the buffer and returns
 *              true if a next index was found.
 *
 * Inputs: 
 * Output: 
 */
bool Buffer::setNext()
{
  if (index + 1 < action_buffer.size())
  {
    index++;

    return true;
  }

  clearAll();
  return false;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the static max size of the Buffer.
 *
 * Inputs: none
 * Output: uint16_t - the value of kMAXIMUM_ELEMENTS
 */
uint16_t Buffer::getMaxSize()
{
  return kMAXIMUM_ELEMENTS;
}

/*
 * Description: Sorts a given vector of BufferActions by a given enumerated
 *              sort type using std stable sort. Returns true if the sorting
 *              actually took place.
 *
 * Inputs: actions - the vector of actions to be sorted
 *         buffer_sorts - the enumerated sort to be performed
 * Output: bool - true if the sorting took place
 */
bool sort(std::vector<BufferAction> actions, BufferSorts buffer_sorts)
{
  auto sorted = false;

  if (buffer_sorts == BufferSorts::ITEM_FIRST)
  {
    std::stable_sort(begin(actions), end(actions), Helpers::CompItemFirst());
    sorted = true;
  }
  else if (buffer_sorts == BufferSorts::SKILL_FIRST)
  {
    std::stable_sort(begin(actions), end(actions), Helpers::CompSkillFirst());
    sorted = true;
  }
  else if (buffer_sorts == BufferSorts::MOMENTUM)
  {
    std::stable_sort(begin(actions), end(actions), Helpers::CompMomentum());
    sorted = true;
  }
  else
  {
    std::cerr << "[Error]: Unknown buffer sorting method \n";
  }

  return sorted;
}