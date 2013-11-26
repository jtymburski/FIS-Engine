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
* - Is copy constructor needed? (ID Problem) [11-23-13]
* - Possible OFENSIVE/DEFENSIVE/NEUTRAL categories (set in flags)? [11-23-13]
* - Testing [11-22-13]
* - Conventions [11-22-13]
*******************************************************************************/

#ifndef SKILL_H
#define SKILL_H

#include <algorithm> /* std::sort */
#include <vector>

#include "Game/Player/Action.h"
#include "EnumDb.h"
#include "EnumFlags.h"
#include "Sound.h"
#include "Sprite.h"

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

  Skill(const Skill &source);

  Skill(const std::string &name);

  Skill(const int &id, const std::string &name, const ActionScope &scope,
        Action* effect, const float &chance);

  Skill(const int &id, const std::string &name, const ActionScope &scope, 
  	    const std::vector<Action*> &effects, const std::vector<float> &chances);

  ~Skill();

private:

  Sprite* animation;

  uint8_t cooldown;

  uint16_t cost;

  std::string description;

  std::vector<Action*> effects;

  std::vector<float> chances;

  SkillFlags flags;

  unsigned int id;

  std::string name;

  Element primary;

  Element secondary;

  Sound* sound_effect;

  ActionScope scope;

  Frame* thumbnail;

  std::string message;

  uint8_t value;

  /* ------------ Constants --------------- */
  static const size_t   kMAX_ACTIONS;     /* Maximum # of actions in a skill */
  static const uint8_t  kMAX_COOLDOWN;    /* Maximum turn cooldown time */
  static const uint16_t kMAX_COST;        /* Highest possible cost for a skill */
  static const size_t   kMAX_MESG_LENGTH; /* Maximum length for using message */
  static const size_t   kMAX_NAME_LENGTH; /* Maximum length for a valid name */
  static const size_t   kMAX_DESC_LENGTH; /* Maximum length for a valid desc */
  static const uint8_t  kMAX_VALUE;       /* Maximum assigned point value */
  static const int      kUNSET_ID;        /* ID for an unset Skill */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

  void classSetup();

  void copySelf(const Skill &source);

  void flagSetup();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  bool addAction(Action* new_action, const float &new_chance);

  bool addActions(const std::vector<Action*> &new_actions, 
                  const std::vector<float> &new_chances);

  bool isValid();

  void print();

  bool removeAction(const uint8_t &index);

  Sprite* getAnimation();
 
  uint8_t getCooldown();

  float getChance(const uint8_t &index);

  std::vector<float> getChances();

  std::string getDescription();

  Action* getEffect(const uint8_t &index);

  std::vector<Action*> getEffects();

  bool getFlag(SkillFlags test_flag);

  int getID();

  std::string getName();

  Element getPrimary();

  Element getSecondary();

  Sound* getSoundEffect();

  ActionScope getScope();

  Frame* getThumbnail();

  std::string getMessage();

  uint8_t getValue();

  bool setAnimation(Sprite* const new_animation);

  bool setCooldown(const uint8_t &new_value);

  bool setDescription(const std::string &new_description);

  void setFlag(const SkillFlags &flag, const bool &set_value = true);

  bool setID(const unsigned int &new_id);
  
  bool setName(const std::string &new_name);

  void setPrimary(const Element &new_element);

  void setSecondary(const Element &new_element);

  bool setSoundEffect(Sound* const new_sound_effect);

  void setScope(const ActionScope &new_scope);

  void setThumbnail(Frame* const new_thumbnail);

  bool setMessage(const std::string &new_message);

  bool setValue(const uint8_t &new_value);

  Skill& operator=(const Skill &source);
};

#endif //SKILL_H