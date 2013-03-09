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

#ifndef SKILL_H
#define SKILL_H

#include <QtGui/QWidget>
#include "Game/Player/Action.h"
#include "Game/Sprite.h"
#include "Sound.h"

class Skill
{
public:
  /* Constructor for a skill object */
  Skill(QString name);

  Skill(QString name, QVector<Action*> effect_list, QVector<float> effect_chance_list);

  /* Annihilates a skill object */
  ~Skill();

  /* SkillType Flags */
  enum SkillType
  {
    MULTIHIT       = 1 << 0,
    OFFENSIVE      = 1 << 1,
    DEFENSIVE      = 1 << 2,
    PHYSICAL       = 1 << 3,
    THERMAL        = 1 << 4,
    POLAR          = 1 << 5,
    PRIMAL         = 1 << 6,
    CHARGED        = 1 << 7,
    CYBERNETIC     = 1 << 8,
    NIHIL          = 1 << 9
  };
  Q_DECLARE_FLAGS(SkillFlags, SkillType)

private:
  /* Constant skill class */
  static const int kMAX_ACTIONS = 10;
  static const uint kMAX_QD_COST = 1000;
  static const int kMAX_NAME_LENGTH = 80;

  /* Int value for Quantum Drive cost */
  uint qd_cost;

  /* Name of the skill */
  QString name;

  /* String describing what the skill does */
  QString description;

  /* Using message string (ex. X attacked with SKILL) */
  QString using_message;

  /* Vector of action pointers */
  QVector<Action*> effects;

  /* effect_chance[i] of effect[i] */
  QVector<float> effect_chance;

  // TODO: Discuss animations and sound effects [02-07-13] */
  // Sprite* animation;
  // Sound* sound_effect;

  SkillFlags skill_flags;

public:
  /* Appends an effect chance to the vector */
  const bool addEffectChance(float new_value);

  /* Removes an effect chance from the vector */
  const bool removeEffectChance(uint index);

  /* Appends an effect to the vector */
  const bool addEffect(Action* new_action);

  /* Removes an effect from the vector */
  const bool removeEffect(uint index);

  /* Toggle a SkillType flag */
  void toggleFlag(SkillType flags);

  /* Get a skill flag */
  const bool getFlag(SkillType flags);

  /* Return the description of the skill */
  QString getDescription();

  /* Return the name of the skill */
  QString getName();

  /* Return the using message of the skill */
  QString getUsingMessage();

  /* Return the effects pointer of the skill */
  QVector<Action*> getEffects();

  /* Return the chance vector of the skill */
  QVector<float> getEffectChance();

  /* Return an effect index of the skill */
  Action* getEffectIndex(uint index);

  /* Return a chance vector index of the skill */
  float getEffectChance(uint index);

  /* Gets the QD Cost Value */
  uint getQdValue();

  /* Sets the sprite pointer for animation */
  void setAnimation(Sprite* new_sprite);

  /* Sets the effect_cance vector */
  void setEffectChances(QVector<float> new_value);

  /* Sets the description of the flag */
  void setDescription(QString new_value);

  /* Set a skill flag */
  void setFlag(SkillFlags flags, const bool set_value);

  /* Sets the name of the skill */
  void setName(QString new_value);

  /* Sets the qd cost value */
  void setQdCost(uint new_value);

  /* Sets the sound effect pointer */
  void setSoundEffect(Sound* new_sound);

  /* Sets the using message of the skill */
  void setUsingMessage(QString new_value);

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Skill::SkillFlags)

#endif // SKILL_H
