/*******************************************************************************
* Class Name: Skill [Declaration]
* Date Created: November 21, 2013
* Inheritance: None
* Description: A Skill is a container for actions which are the actual choices
*              a person chooses to use in a Battle. Skills are generally 
*              contained in SkillSets which are used by Person and Items, etc.
*              A Skill can have a number of actions, each with different
*              effects.
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* - Testing [11-22-13]
* - Conventions [11-22-13]
*******************************************************************************/

#ifndef SKILL_H
#define SKILL_H

#include <vector>

#include "Game/Player/Action.h"
//#include "Game/Sound.h"
#include "Game/Sprite.h"
#include "EnumDb.h"
#include "EnumFlags.h"

ENUM_FLAGS(SkillFlags)
enum class SkillFlags
{
  ALTERING   = 1 << 0,
  HEALING    = 1 << 1,
  INFLICTING = 1 << 2,
  RELIEVING  = 1 << 3,
  REVIVING   = 1 << 4,
  ASSIGNING  = 1 << 5,
  VALID      = 1 << 6
};

class Skill
{
public:

  Skill();

  Skill(const std::string &name);

  Skill(const std::string &name, const ActionScope &skill_scope,
        const Action* effect, const float &chance);

  Skill(const std::string &name, const ActionScope &skill_scope, 
  	    const std::vector<Action*> &effects, const std::vector<float> &chances);

  ~Skill();

private:

  Sprite* animation;

  uint cooldown;

  std::string description;

  std::vector<Action*> effects;

  std::vector<float> effect_chance;

  SkillFlags flags;

  uint cost;

  std::string name;

  Element primary;

  Eleemtn secondary;

  Sound* sound_effect;

  ActionScope scope;

  Frame* thumbnail;

  std::string message;

  uint value;

  /* ------------ Constants --------------- */
  static const size_t kMAX_ACTIONS;     /* Maximum # of actions in a skill */
  static const uint   kMAX_COOLDOWN;    /* Maximum turn cooldown time */
  static const uint   kMAX_COST;        /* Highest possible cost for a skill */
  static const size_t kMAX_MESG_LENGTH; /* Maximum length for using message */
  static const size_t kMAX_NAME_LENGTH; /* Maximum length for a valid name */
  static const size_t kMAX_DESC_LENGTH; /* Maximum length for a valid desc */
  static const uint   kMAX_VALUE;       /* Maximum assigned point value */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

  void classSetup();

  void flagSetup();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  bool addAction(const Action* new_action, const float &new_chance);

  bool isValid();

  bool removeAction(const uint &index);

  Sprite* getAnimation();
 
  uint getCooldown();

  float getChance(const uint &index);

  std::vector<float> getChances();

  std::string getDescription();

  Action* getEffect(const uint &index);

  std::vector<Action*> getEffects();

  SkillFlags getFlags();

  std::string getName();

  Element getPrimary();

  Element getSecondary();

  Sound* getSoundEffect();

  ActionScope getScope();

  Frame* getThumbnail();

  std::string getMessage();

  uint getValue();

  bool setAnimation(const Sprite* new_animation);

  bool setCooldown(const uint &new_value);

  void setChance(const float &new_chance);

  bool setDescription(const std::string &new_description);

  bool setName(const std::string &new_name);

  void setPrimary(const Element &new_element);

  void setSecondary(const Element &new_element);

  bool setSoundEffect(const Sound* new_sound_effect);

  void setScope(const ActionScope &new_scope);

  void setThumbnail(const Frame* new_thumbnail);

  bool setMessage(const std::string &new_message);

  bool setValue(const uint &new_value);
};

#endif //SKILL_H