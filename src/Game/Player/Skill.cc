/******************************************************************************
* Class Name: Skill
* Date Created: February 17th, 2013
* Inheritance: QWidget
* Description: Skill class is essentially a container for actions. Persons and
*              items should have vectors of skills, where a skill can be
*              one or more actions, such as a posion action or a damage action
*              for use on map, in battle, or in menu etc. The action class will
*              parse individual actions, and skills will be tied to each of
*              these actions. This way there is no need for skills with
*              different names but similar purposes to create more than one
*              action that does the same thing.
******************************************************************************/
#include "Game/Player/Skill.h"

/*============================================================================
 * CONSTANTS
 *============================================================================*/
const int  Skill::kMAX_ACTIONS = 10;
const uint Skill::kMAX_QD_COST = 1000;
const int  Skill::kMAX_NAME_LENGTH = 80;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructs a default skill object
 *
 * Inputs: QString - name of the skill
 */
Skill::Skill(QString name)
{
  /* Pointer setup */
  animation = 0;
  sound_effect = 0;

  setName(name);
  qd_cost = 0;
  description = "";
  using_message = "";
}

/*
 * Description: Constructs a skill object based off a list of actions
 *
 * Inputs: QString - name of the skill
 *         QVector<Action> - vector of actions to base the skill off
 *         QVector<float> - chance_list -
 */
Skill::Skill(QString name, QVector<Action*> effect_list,
             QVector<float> chance_list)
{
  /* Pointer setup */
  // assert(effect_list.size() == chance_list.size());
  for (int i = 0; i < effect_list.size(); i++)
  {
    addEffect(effect_list[i]);
    addEffectChance(chance_list[i]);
  }
  animation = 0;
  sound_effect = 0;

  setName(name);
  setQdCost(0);
  setDescription("");
  setUsingMessage("");
}

/*
 * Description: Annihilates a Skill object
 */
Skill::~Skill() {}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Appends an effect chance to the vector
 *
 * Inputs: float - new value of chance (1 = 100%)
 * Output: none
 */
bool Skill::addEffectChance(float new_value)
{
  if (effect_chance.size() + 1 < kMAX_ACTIONS)
  {
    effect_chance.push_back(new_value);
    return true;
  }
  return false;
}

/*
 * Description: Removes an effect chance from the vector
 *
 * Notes: The effect at the same index should also be removed!
 * Inputs: uint - index of effect to be removed.
 * Output: none
 */
bool Skill::removeEffectChance(uint index)
{
  if (effect_chance.at(index))
  {
    effect_chance.remove(index);
    return true;
  }
  return false;
}

/*
 * Description: Adds a skill effect to the vector
 *
 * Inputs: Action* - action to be added
 * Output: none
 */
bool Skill::addEffect(Action* new_action)
{
  if (effects.size() + 1 < kMAX_ACTIONS)
  {
    effects.push_back(new_action);
    return true;
  }
  return false;
}

/*
 * Description: Prints out all the info of a skill by calling the sub print
 *              functions.
 *
 * Inputs: none
 * Output: none
 */
void Skill::printAll()
{
  qDebug() << "--- Skill ---";
  printInfo();
  printFlags();
  qDebug() << "--- /SKill ---";
}

/*
 * Description: Prints out the basic info describing the skill
 *
 * Inputs: none
 * Output: none
 */
void Skill::printInfo()
{
  qDebug() << "Name: " << name;
  qDebug() << "Description: " << description;
  qDebug() << "QD Cost: " << qd_cost;
  qDebug() << "Using Message: " << using_message;

  for (int i = 0; i < effects.size(); i++)
  {
    qDebug() << "Action #" << i << ": " << effects.at(i)->getId();
    qDebug() << "Action Chance: " << effect_chance.at(i);
  }
}

/*
 * Description: Prints out the flags of the skill
 *
 * Inputs: none
 * Output: none
 */
void Skill::printFlags()
{
  qDebug() << "MULTIHIT: " << getFlag(Skill::MULTIHIT);
  qDebug() << "OFFENSIVE: " << getFlag(Skill::OFFENSIVE);
  qDebug() << "DEFENSIVE: " << getFlag(Skill::DEFENSIVE);
  qDebug() << "PHYSICAL: " << getFlag(Skill::PHYSICAL);
  qDebug() << "THERMAL: " << getFlag(Skill::THERMAL);
  qDebug() << "POLAR: " << getFlag(Skill::POLAR);
  qDebug() << "PRIMAL: " << getFlag(Skill::PRIMAL);
  qDebug() << "CHARGED: " << getFlag(Skill::CHARGED);
  qDebug() << "CYBERNETIC: " << getFlag(Skill::CYBERNETIC);
  qDebug() << "NIHIL: " << getFlag(Skill::NIHIL);
}

/*
 * Description: Removes an effect chance from the vector
 *
 * Inputs: uint - index of the effect to be removed
 * Output: none
 */
bool Skill::removeEffect(uint index)
{
  if (effects.at(index))
  {
    effects.remove(index);
    return true;
  }
  return false;
}

/*
 * Description: Toggles a SkillType flag
 *
 * Inputs: SkillType - flag to be toggled
 * Output: none
 */
void Skill::toggleFlag(SkillType flags)
{
  setFlag(flags, !getFlag(flags));
}

/*
 * Description: Gets a SkillType flag
 *
 * Inputs: SkillType - flag to be returned
 * Output: Boolean evaluation of the flag
 */
bool Skill::getFlag(SkillType flags)
{
  return skill_flags.testFlag(flags);
}

/*
 * Description: Returns the description of the string.
 *
 * Inputs: None
 * Output: QString - string of the description
 */
QString Skill::getDescription()
{
  return description;
}

/*
 * Description: Returns the name of the skill.
 *
 * Inputs: None
 * Output: QString - name of the skill
 */
QString Skill::getName()
{
  return name;
}

/*
 * Description: Returns the using message of the skill.
 *
 * Inputs: none
 * Output: QString - the using message
 */
QString Skill::getUsingMessage()
{
  return using_message;
}

/*
 * Description: Returns the effects vector of the skill
 *
 * Inputs: none
 * Output: Vector of action pointers - the actions of the skill
 */
QVector<Action*> Skill::getEffects()
{
  return effects;
}

/*
 * Description: Returns the effect chance vector of the skill
 *
 * Inputs: none
 * Output: Vector of float pointers - the chances of the skill
 */
QVector<float> Skill::getEffectChance()
{
  return effect_chance;
}

/*
 * Description: Returns a specific action pointer to an effect
 *
 * Inputs: uint - index of action to be returned
 * Output: none
 */
Action* Skill::getEffectIndex(uint index)
{
    return effects[index];
}

/*
 * Description: Returns a specific float to an effect chance
 *
 * Inputs: uint - index of the effect chance to be returned
 * Output: none
 */
float Skill::getEffectChance(uint index)
{
  if (effect_chance.at(index))
    return effect_chance[index];
  else
    return 0;
}

/*
 * Description: Returns the point value assosciated with the skill.
 *
 * Inputs: none
 * Output: uint - the point value of
 */
uint Skill::getPointValue()
{
  return point_value;
}

/*
 * Description: Returns the QD cost of the skill
 *
 * Inputs: none
 * Output: uint - QD cost of the skill
 */
uint Skill::getQdValue()
{
  return qd_cost;
}

/*
 * Description: Sets the animation sprite pointer
 *
 * Inputs: Sprite* - pointer to the new sprite
 * Output: none
 */
void Skill::setAnimation(Sprite* new_sprite)
{
    (void)new_sprite;//warning
  // animation = new_sprite;
}

/*
 * Description: Sets the effect chance vector
 *
 * Inputs: new_value - new vector of values.
 * Output: none
 */
void Skill::setEffectChances(QVector<float> new_value)
{
  effect_chance.clear();
  for (int i = 0; i < effect_chance.size(); i++)
    effect_chance.push_back(new_value[i]);
}

/*
 * Description: Sets the description of the skill
 *
 * Inputs: new_value - new description
 * Output: none
 */
void Skill::setDescription(QString new_value)
{
  description = new_value;
}

/*
 * Description: Sets a given skill flag
 *
 * Inputs: SkillFlag - flag to be set to
 *         set_value - value flag is to be set to
 * Output: none
 */
void Skill::setFlag(SkillFlags flags, bool set_value)
{
  (set_value) ? (skill_flags |= flags) : (skill_flags &= flags);
}

/*
 * Description: Sets the name of the skill
 *
 * Inputs: new_value - value to set the name to
 * Output: none
 */
void Skill::setName(QString new_value)
{
  if (new_value.length() <= kMAX_NAME_LENGTH)
    name = new_value;
  else
    name = "INVALID NAME LENGTH";
}

/*
 * Description: Assigns a new point value to the Skill.
 *
 * Inputs: new_point_value - new point value
 * Output: none
 */
void Skill::setPointValue(uint new_point_value)
{
  point_value = new_point_value;
}

/*
 * Description: Sets the QD cost value
 *
 * Inputs: new_value - qd cost value to be set
 * Output: none
 */
void Skill::setQdCost(uint new_value)
{
  if (qd_cost <= kMAX_QD_COST)
    qd_cost = new_value;
  else
    qd_cost = kMAX_QD_COST;
}

/*
 * Description: Sets the sound effect pointer
 *
 * Inputs: Sound* - new sound pointer
 * Output: none
 */
void Skill::setSoundEffect(Sound* new_sound)
{
    (void)new_sound;//warning
  // sound_effect = new_sound;
}

/*
 * Description: Sets the using message of the skill
 *
 * Inputs: new_value - using message of the skill
 * Output: none
 */
void Skill::setUsingMessage(QString new_value)
{
  using_message = new_value;
}


