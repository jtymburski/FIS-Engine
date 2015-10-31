/*******************************************************************************
* Class Name: Buffer [Implementation]
* Date Created: February 23rd, 2014
* Date Redesigned: September 8th, 2015
* Inheritance: None
* Description:
*
* See .h file for TODOs
*******************************************************************************/
#include "Game/Battle/Buffer.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Buffer::Buffer() : index{0}, sorted{false}, action_buffer{}
{
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

BufferAction& Buffer::getIndex(const uint32_t& index)
{
  if(index < action_buffer.size())
    return action_buffer.at(index);

  std::cerr << "[ERROR} Invalid Buffer action selection" << std::endl;

  return action_buffer.at(0);
}

std::vector<BufferAction> Buffer::sort(std::vector<BufferAction> actions,
                                       BufferSorts buffer_sorts)
{
  if(buffer_sorts == BufferSorts::ITEM_FIRST)
    std::stable_sort(begin(actions), end(actions), Helpers::CompItemFirst());
  else if(buffer_sorts == BufferSorts::SKILL_FIRST)
    std::stable_sort(begin(actions), end(actions), Helpers::CompSkillFirst());
  else if(buffer_sorts == BufferSorts::MOMENTUM)
    std::stable_sort(begin(actions), end(actions), Helpers::CompMomentum());

  return actions;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void Buffer::addDefend(BattleActor* user)
{
  BufferAction defend_action;
  defend_action.user = user;
  defend_action.type = ActionType::DEFEND;
  action_buffer.push_back(defend_action);
}

/* Adds a guard element to the buffer */
void Buffer::addGuard(BattleActor* user, BattleActor* target)
{
  BufferAction guard_action;
  guard_action.user = user;
  guard_action.type = ActionType::GUARD;

  std::vector<BattleActor*> target_vec{target};
  guard_action.targets = target_vec;
  action_buffer.push_back(guard_action);
}

/* Add an imploding element to the buffer */
void Buffer::addImplode(BattleActor* user)
{
  BufferAction implode_action;
  implode_action.user = user;
  implode_action.type = ActionType::IMPLODE;
  action_buffer.push_back(implode_action);
}

/* Adds an Item element to the buffer */
void Buffer::addItem(BattleActor* user, Item* used_item,
                     std::vector<BattleActor*> targets)
{
  BufferAction item_action;
  item_action.user = user;
  item_action.used_item = used_item;
  item_action.type = ActionType::ITEM;

  std::vector<BattleActor*> target_vec{targets};
  item_action.targets = target_vec;
  action_buffer.push_back(item_action);
}

/* Adds a Pass event to the Battle */
void Buffer::addPass(BattleActor* user)
{
  BufferAction pass_action;
  pass_action.user = user;
  pass_action.type = ActionType::PASS;
  action_buffer.push_back(pass_action);
}

/* Adds a run element to the buffer */
void Buffer::addRun(BattleActor* user)
{
  BufferAction run_action;
  run_action.user = user;
  run_action.type = ActionType::RUN;
  action_buffer.push_back(run_action);
}

/* Adds a skill use to the buffer */
void Buffer::addSkill(BattleActor* user, Skill* used_skill,
                      std::vector<BattleActor*> targets, uint32_t cooldown,
                      uint32_t initial_turn)
{
  BufferAction skill_action;
  skill_action.user = user;
  skill_action.used_skill = used_skill;
  skill_action.targets = targets;
  skill_action.cooldown = cooldown;
  skill_action.initial_turn = initial_turn;
  skill_action.type = ActionType::SKILL;
  action_buffer.push_back(skill_action);
}

void Buffer::clear()
{
  action_buffer.clear();
}

void Buffer::clearForTurn(uint32_t turn_number)
{
  //TODO: This should remove all elements which have been performed?

  /* Erase remove for all elem's with initial turn matching the given turn # */
  action_buffer.erase(std::remove_if(begin(action_buffer), end(action_buffer),
                                     [&](const BufferAction a) -> bool
                                     {
                                       return (a.initial_turn == turn_number);
                                     }),
                      end(action_buffer));

  /* Resets the action buffer index to zero */
  index = 0;
  sorted = false;
}

bool Buffer::hasCoolingSkill(BattleActor* check_person)
{
  for(auto element : action_buffer)
    if(element.user == check_person &&
       element.used_skill) //&& element.cooldown >= 0)
      return true;

  return false;
}

bool Buffer::isIndexProcessed()
{
  if(index < action_buffer.size())
    return getIndex(index).processed;

  return false;
}

bool Buffer::isIndexStarted()
{
  if(index < action_buffer.size())
    return getIndex(index).started;

  return false;
}

bool Buffer::isSorted()
{
  return sorted;
}

void Buffer::print(bool simple)
{
  std::cout << "===== [ Action Buffer ] ======\n"
            << "Size: " << action_buffer.size() << "Curr. Index: " << index
            << "\n";

  if(!simple)
  {
    for(const auto& element : action_buffer)
    {
      std::cout << "  -- Element -- \n";
      std::cout << "Type: " << Helpers::actionTypeToStr(element.type) << "\n";

      if(element.type == ActionType::SKILL && element.used_skill)
      {
        std::cout << "Skill: " << element.used_skill->getName() << "\n";
        std::cout << "Scope: " << Helpers::actionScopeToStr(
                                      element.used_skill->getScope()) << "\n";
        std::cout << "Cooldown: " << element.cooldown << "\n";
        std::cout << "Initial Turn: " << element.initial_turn << "\n";
      }
      else if(element.type == ActionType::ITEM && element.used_item)
      {
        std::cout << "Item: " << element.used_item->getName() << "\n";

        if(element.used_item->getUseSkill())
        {
          std::cout << "Item Skill: "
                    << element.used_item->getUseSkill()->getName() << "\n";
          std::cout << "Scope: "
                    << Helpers::actionScopeToStr(
                           element.used_item->getUseSkill()->getScope())
                    << "\n";
        }
      }

      if(element.user && element.user->getBasePerson())
      {
        std::cout << "User: " << element.user->getBasePerson()->getName()
                  << "\n";
        std::cout << "User Speed: "
                  << element.user->getStats().getValue(Attribute::MMNT) << "\n";
      }

      for(const auto& target : element.targets)
        if(target && target->getBasePerson())
          std::cout << "Target: " << target->getBasePerson()->getName() << "\n";
    }
  }

  std::cout << std::endl;
}

void Buffer::removeAllByUser(BattleActor* user)
{
  action_buffer.erase(std::remove_if(begin(action_buffer), end(action_buffer),
                                     [&](BufferAction x) -> bool
                                     {
                                       return (x.user == user);
                                     }),
                      end(action_buffer));

  auto new_size = action_buffer.size();

  /* Set the index of the last possible if the index ends out of bounds */
  if(action_buffer.size() > 0 && index > new_size - 1)
    index = new_size - 1;
}

void Buffer::updateCooldowns()
{
  for(auto& element : action_buffer)
    if(element.cooldown != 0)
      --element.cooldown;
}

ActionType Buffer::getActionType()
{
  if(index < action_buffer.size())
    return getIndex(index).type;

  return ActionType::NONE;
}

int32_t Buffer::getCooldown()
{
  if(index < action_buffer.size())
    return getIndex(index).cooldown;

  return -1;
}

BattleActor* Buffer::getUser()
{
  if(index < action_buffer.size())
    return getIndex(index).user;

  return nullptr;
}

Skill* Buffer::getSkill()
{
  if(index < action_buffer.size())
    return getIndex(index).used_skill;

  return nullptr;
}

Item* Buffer::getItem()
{
  if(index < action_buffer.size())
    return getIndex(index).used_item;

  return nullptr;
}

int32_t Buffer::getInitialTurn()
{
  if(index < action_buffer.size())
    return getIndex(index).initial_turn;

  return -1;
}

std::vector<BattleActor*> Buffer::getTargets()
{
  if(index < action_buffer.size())
    return getIndex(index).targets;

  std::cerr << "[ERROR] - reaching invalid action buffer index" << std::endl;
  return getIndex(0).targets;
}

bool Buffer::setNext()
{
  if(index + 1 < action_buffer.size())
  {
    index++;

    if(index < action_buffer.size() - 1)
      return true;
  }

  return false;
}

void Buffer::setProcessed()
{
  if(index < action_buffer.size())
    getIndex(index).processed = true;
}

void Buffer::setStarted()
{
  if(index < action_buffer.size())
    getIndex(index).started = true;
}

void Buffer::reorder()
{
  /* Precedence of Action Types (each action type sorted by momentum)
   * ---- DEFEND, GUARD, ITEM, SKILL, OTHER ----
   * Sorting takes place by the value of the integer equiv. ActionType enum
   */
  std::sort(begin(action_buffer), end(action_buffer),
            [&](const BufferAction& a, const BufferAction& b) -> bool
            {
              return (static_cast<uint16_t>(a.type) <
                      static_cast<uint16_t>(b.type));
            });

  sorted = true;
}
