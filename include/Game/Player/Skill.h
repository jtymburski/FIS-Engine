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

#include <QWidget>

#include "Game/Player/Action.h"
#include "Game/Sprite.h"
#include "Sound.h"

class Skill
{
public:
  /* Constructor for a skill object */
  Skill(QString name);

  Skill(QString name, EnumDb::ActionScope skill_scope,
        QVector<Action*> effect_list, QVector<float> chance_list);

  /* Annihilates a skill object */
  ~Skill();

  /* SkillType Flags */
  enum SkillType
  {
    OFFENSIVE      = 1 << 2,
    DEFENSIVE      = 1 << 3,
    INFLICTING     = 1 << 4,
    ALLEVIATING    = 1 << 5,
    HEALING        = 1 << 6,
    REVIVING       = 1 << 7,
    PHYSICAL       = 1 << 8,
    THERMAL        = 1 << 9,
    POLAR          = 1 << 10,
    PRIMAL         = 1 << 11,
    CHARGED        = 1 << 12,
    CYBERNETIC     = 1 << 13,
    NIHIL          = 1 << 14,
    VALID_SKILL    = 1 << 15
  };
  Q_DECLARE_FLAGS(SkillFlags, SkillType)

private:
  /* ------------ Constants --------------- */
  static const int kMAX_ACTIONS;
  static const uint kMAX_QD_COST;
  static const int kMAX_NAME_LENGTH;
  static const ushort kMAX_COOLDOWN;

  /* Cooldown of the skill */
  uint cooldown;

  /* Int value for Quantum Drive cost */
  uint qd_cost;

  /* Point value for the Skill */
  uint point_value;

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

  /* Sprite animation of the skill */
  Sprite* animation;

  /* Sound effect that the skill uses */
  Sound* sound_effect;

  /* SKill flags variable */
  SkillFlags skill_flags;

  /* The scope of the skill */
  EnumDb::ActionScope skill_scope;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Sets up the flags during construction */
  void calculateFlags();

  /* Sets up the class during construction */
  void classSetup();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Appends an effect chance to the vector */
  bool addEffectChance(float new_value);

  /* Asserts the current skill is valid */
  bool isValid();

  /* Removes an effect chance from the vector */
  bool removeEffectChance(uint index);

  /* Appends an effect to the vector */
  bool addEffect(Action* new_action);

  /* Methods for printing ou the info of a Skill */
  void printAll();
  void printInfo();
  void printFlags();

  /* Removes an effect from the vector */
  bool removeEffect(uint index);

  /* Toggle a SkillType flag */
  void toggleFlag(SkillType flags);

  /* Returns the cooldown of the skill */
  uint getCooldown();

  /* Get a skill flag */
  bool getFlag(SkillType flags);

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

  /* Returns the point value of the skill */
  uint getPointValue();

  /* Gets the QD Cost Value */
  uint getQdValue();

  /* Returns the scope of the skill */
  EnumDb::ActionScope getSkillScope();

  /* Sets the sprite pointer for animation */
  void setAnimation(Sprite* new_sprite);

  /* Assigns a cooldown to the skill */
  void setCooldown(int new_cooldown);

  /* Sets the effect_cance vector */
  void setEffectChances(QVector<float> new_value);

  /* Sets the description of the flag */
  void setDescription(QString new_value);

  /* Set a skill flag */
  void setFlag(SkillFlags flags, bool set_value = true);

  /* Sets the name of the skill */
  void setName(QString new_value);

  /* Assigns a new point value to the skill */
  void setPointValue(uint new_point_value);

  /* Sets the qd cost value */
  void setQdCost(uint new_value);

  /* Sets the sound effect pointer */
  void setSoundEffect(Sound* new_sound);

  /* Sets the using message of the skill */
  void setUsingMessage(QString new_value);

  /* Assigns the scope of the skill */
  void setSkillScope(EnumDb::ActionScope new_skill_scope);

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Skill::SkillFlags)

#endif // SKILL_H
