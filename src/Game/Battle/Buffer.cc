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

void Buffer::clearAll()
{
  action_buffer.clear();
  index = 0;
}

BufferAction& Buffer::getIndex(const uint32_t &index)
{
  if (index < action_buffer.size())
    return action_buffer.at(index);;
 

  return action_buffer.at(0);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

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

  auto add = checkValid(new_elm);

  if (add)
  {
    action_buffer.push_back(new_elm);

    return true;
  }

  return false;
}

void Buffer::clearInvalid()
{
  std::vector<BufferAction> temp;

  for (auto it = begin(action_buffer); it != end(action_buffer); ++it)
    if ((*it).valid)
      temp.push_back(*it);

  action_buffer = temp;
  index  = 0;
}

bool Buffer::isNextValid()
{
  if (index + 1 < action_buffer.size())
    return checkValid(getIndex(index + 1));

  return false;
}

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

bool Buffer::remove(const uint32_t &index)
{
  if (index < action_buffer.size())
  {
    action_buffer.erase(action_buffer.begin() + index);

    return true;
  }

  return false;
}

bool Buffer::removeAllByUser(Person* user)
{
  if (user == nullptr)
    return false;

  for (auto it = begin(action_buffer); it != end(action_buffer); ++it)
  {
    if ((*it).user == user)
    {
      action_buffer.erase(it);

      return true;
    }
  }

  return false;
}

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

Person* Buffer::getUser()
{
  if (checkValid(getIndex(index)))
    return getIndex(index).user;

  return nullptr;
}

Skill* Buffer::getSkill()
{
  if (checkValid(getIndex(index)))
    return getIndex(index).skill_used;

  return nullptr;
}

std::vector<Person*> Buffer::getTargets()
{
  if (checkValid(getIndex(index)))
    return getIndex(index).targets;

  return {};
}

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

uint16_t Buffer::getMaxSize()
{
  return kMAXIMUM_ELEMENTS;
}