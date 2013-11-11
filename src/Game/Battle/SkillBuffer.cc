/*******************************************************************************
* Class Name: Skill Buffer [Header Declaration]
* Date Created: October 28th, 2013
* Inheritance: None
* Description: Skill Buffer is a helper class for Battle which contains the stack
*              of skills with users and targets.
*******************************************************************************/
#include "Game/Battle/SkillBuffer.h"

/*==============================================================================
 * CONSTANTS
 *============================================================================*/

const ushort SkillBuffer::kMAXIMUM_ELEMENTS  = 40;
const ushort SkillBuffer::kMAXIMUM_TARGETS   = 10;
const ushort SkillBuffer::kSTARTING_ELEMENT  = 0;

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Creates a SkillBuffer object
 *
 * Inputs: none
 */

SkillBuffer::SkillBuffer()
{
  curr_index = kSTARTING_ELEMENT;
}

/*
 * Description: Annihilates a SkillBuffero object
 *
 */
SkillBuffer::~SkillBuffer() {}

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Checks the validity of a SkillUseAction. Will check that the user
 *              of the skill is alive, in batttle, and has skill use enabled,
 *              will also check that the skill is single hit for single target
 *              or multi-hit for multi targets, and that the targets are all
 *              in battle.
 *
 * Inputs:  SkillUseAction - struct describing element of the SkillBuffer
 * Outputs: bool - validity of the given SkillUseAction
 */
bool SkillBuffer::checkValid(SkillUseAction skill_use_action)
{
  bool is_valid = true;

  /* If the user of the skill is both alive, in battle, and can use skills */
  is_valid &= skill_use_action.user->getPersonFlag(Person::ALIVE);
  is_valid &= skill_use_action.user->getPersonFlag(Person::IN_BATTLE);
  is_valid &= skill_use_action.user->getPersonFlag(Person::ITEM_USE_ENABLED);

  /* Iterate through targets and assert they are all in battle */
  QVector<Person*>::Iterator it = skill_use_action.targets.begin();

  for (; it < skill_use_action.targets.end(); ++it)
    is_valid &= (*it)->getPersonFlag(Person::IN_BATTLE);

  /* Assign the validity of the skill_use_action */
  skill_use_action.valid_skill_use = is_valid;

  return is_valid;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
bool SkillBuffer::decrementCooldown(int index)
{
  if (index < skill_buffer.size())
  {
    if (skill_buffer.at(index).cooldown > 0)
    {
      skill_buffer[index].cooldown--;

      return true;
    }
  }

  return false;
}

/*
 * Description: Clears the list of SkillUseActions. This does not delete the
 *              data of the person or skills contained in the struct.
 *
 * Inputs:
 * Outputs:
 */
void SkillBuffer::clearAll()
{
  skill_buffer.clear();
  curr_index = kSTARTING_ELEMENT;
}

 /*
 * Description: Returns a single element of the SkillBuffer given an index.
 *
 * Inputs: int index - the index of the item buffer to be checked.
 * Outputs: The SkillUseAction struct at that index, if one exists
 */
SkillUseAction SkillBuffer::getIndex(int index)
{
  if (index < skill_buffer.size())
    return skill_buffer.at(index);

  return skill_buffer.at(kSTARTING_ELEMENT);
}

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add a Skill to the Skill Use Buffer given a user, a
 *              skill to be used, and targets that the skill will hit. This may
 *              fail if the person is not in battle or dead, the skill is
 *              invalid or if the target vector is empty.
 *
 * Inputs: Person* user - the potential user of the skill
 *         Skill* skill_used - the potential skill to be used
 *         QVector<Person*> targets - the vector of targets the skill may hit
 * Outputs: bool - the validity of the new SkillUseAction
 */
 bool SkillBuffer::addSkillUse(Person *user, Skill *skill_used,
                               QVector<Person *> targets, int cooldown)
{
  bool add_to_buffer = true;

  /* Assert the user and skill to be used exist */
  add_to_buffer &= !(user == 0);
  add_to_buffer &= !(skill_used == 0);

  /* Assert the new size of the SkillBuffer will be within bounds */
  add_to_buffer &= (skill_buffer.size() < kMAXIMUM_ELEMENTS);

  /* Assert the cooldown is within range */
  add_to_buffer &= (cooldown >= 0);

  if (add_to_buffer)
  {
    struct SkillUseAction skill_use;

    skill_use.user = user;
    skill_use.skill_used = skill_used;
    skill_use.targets = targets;
    skill_use.cooldown =  skill_used->getCooldown();

    add_to_buffer &= checkValid(skill_use);

    if (add_to_buffer)
      skill_buffer.push_back(skill_use);
  }

  return add_to_buffer;
}

/*
 * Description: Clears only the invalid elements in the skill buffer by creating
 *              a temporary buffer and only adding in the valid elements.
 *
 * Inputs:
 * Outputs:
 */
void SkillBuffer::clearInvalid()
{
  /* Create an empty temp buffer of SkillUseAction structs */
  QList<SkillUseAction> temp;

  /* Iterate through the original skill buffer */
  QList<SkillUseAction>::Iterator it = skill_buffer.begin();

  for (; it < skill_buffer.end(); ++it)
  {
    /* Add the valid elements to the temp buffer */
    if ((*it).valid_skill_use)
      temp.push_back(*it);
  }

  /* Re-assign the actual skill buffer to the temp, now containing valid elems. */
  skill_buffer = temp;
  curr_index = kSTARTING_ELEMENT;
}
 
/*
 * Description: Re-Evaluates the next element (if one exists) in the item buffer,
 *              and return the state of that validity.
 *
 * Inputs: none
 * Outputs: bool - evaluation of the validity of the next SkillUseAction
 */
bool SkillBuffer::isNextValid()
{
  if (curr_index < skill_buffer.size())
    return checkValid(getIndex(curr_index + 1));
  return false;
}

/*
 * Description: Method for calling all print functions for the SkillBuffer
 *
 * Inputs: none
 * Outputs: none
 */
void SkillBuffer::printAll()
{
  qDebug() << "--- Skill Buffer --- ";
  printInfo();

  for (int i = 0; i < skill_buffer.size(); i++)
    printElement(i);

  qDebug() << "--- /Skill Buffer ---";
}

/*
 * Description: Method for printing out basic information describing the
 *              SkillBuffer
 *
 * Inputs: none
 * Outputs: none
 */
void SkillBuffer::printInfo()
{
  qDebug() << "Current Skill Buffer Index: " << curr_index;
  qDebug() << "Current Skill Buffer Size: " << skill_buffer.size();
}

/*
 * Description: Prints out the data describing an element in the SkillBuffer
 *              including the index, user name, skill name, the size of the
 *              target vector, and whether it is a valid skill use.
 *
 * Inputs: int index - the index of the skill buffer to print info for
 * Outputs: bool - true if the element exists
 */
bool SkillBuffer::printElement(int index)
{
  if (index >= 0 && index < skill_buffer.size())
  {
    qDebug() << "Element Index: " << index;
    qDebug() << "User Name: " << skill_buffer.at(index).user->getName();
    qDebug() << "Skill Name: " << skill_buffer.at(index).skill_used->getName();
    qDebug() << "Target Size: " << skill_buffer.at(index).targets.size();
    qDebug() << "Cooldown: " << skill_buffer.at(index).cooldown;
    qDebug() << "Valid Skill Use: " << skill_buffer.at(index).valid_skill_use;
    return true;
  }
  qDebug() << "INVALID ELEMENT INDEX";
  return false;
}
 
/*
 * Description: Attempts to remove a SkillUseAction from the skill buffer being
 *              given a user. In the case of more than one skill use, this
 *              function will remove the first occurence of a SkillUseAction by
 *              the user.
 *
 * Inputs: Person* user - the user of the SkillUseAction to be checked for
 * Outputs: bool - true if a SkillUseAction element was found and removed
 */
bool SkillBuffer::removeSkillUse(Person* user)
{
  /* Iterate through the SkillBuffer and check for user equivalencies */
  QList<SkillUseAction>::Iterator it = skill_buffer.begin();

  for (; it < skill_buffer.end(); ++it)
  {
    /* If the user is found, delete the SkillBuffer at that point */
    if ((*it).user == user)
    {
      skill_buffer.erase(it);
      return true;
    }
  }

  return true;
}
 
/*
 * Description: Attempts to remove a SkillUseAction element from the skill buffer
 *              given an index of an element to be removed.
 *
 * Inputs: int index - index of element to be removed
 * Output: bool - true if the element exists and was removed
 */
bool SkillBuffer::removeSkillUse(int index)
{
  if (index >= 0 && index < skill_buffer.size())
  {
    skill_buffer.removeAt(index);
    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void SkillBuffer::update(bool clear)
{
  clearInvalid();

  if (clear)
  {
    clearAll();
  }
  else
  {
    for (int i = 0; i < skill_buffer.size(); i++)
    {
      decrementCooldown(i);

      if (skill_buffer.at(i).cooldown == 0)
        removeSkillUse(i);
    }
  }
}
 
/*
 * Description: Returns a pointer to the user of the current element if the
 *              current index on the skill buffer is valid.
 *
 * Inputs: none
 * Output: Person* - ptr to the person on the curr_index of the SkillBuffer
 */
Person* SkillBuffer::getUser()
{
  if (checkValid(getIndex(curr_index)))
    return skill_buffer.at(curr_index).user;

  return 0;
}
 
/*
 * Description: Returns a pointer to the skill of the current element if the
 *              next index on the SkillBuffer is valid.
 *
 * Inputs: none
 * Output: Skill* - ptr to the skill on the curr_index of the SkillBuffer
  */
Skill* SkillBuffer::getSkill()
{
  if (checkValid(getIndex(curr_index)))
    return skill_buffer.at(curr_index).skill_used;

  return 0;
}
 
/*
 * Description: Returns the vector of targets of the current element if the
 *              curr_index of the SkillBuffer is valid
 *
 * Inputs: none
 * Outputs: QVector<Person*> - the vector of targets on the curr_index
 */
QVector<Person*> SkillBuffer::getTargets()
{
  if (checkValid(getIndex(curr_index)))
    return skill_buffer.at(curr_index).targets;

  QVector<Person*> empty_targets;
  return empty_targets;
}
 
/*
 * Description: Sets the curr_index of the SkillBuffer to the next value, unless
 *              the index is at the end, in which case the SkillBuffer is cleared
 *
 * Inputs: none
 * Outputs: bool - true if the next index was set, false if the buffer is cleared
 */
bool SkillBuffer::setNextIndex()
{
  /* If the next current_index is less than the last elem of the skill buffer */
  if (curr_index + 1 < skill_buffer.size())
  {
    curr_index++;
    return true;
  }

  /* Otherwise, the next index does not exist, clear the SkillBuffer */
  clearAll();
  return false;
}
