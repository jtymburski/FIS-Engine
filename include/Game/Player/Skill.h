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
* - Possible OFENSIVE/DEFENSIVE/NEUTRAL categories (set in flags)? [11-23-13]
*******************************************************************************/

#ifndef SKILL_H
#define SKILL_H

#include <algorithm> /* std::sort */

#include "Game/Player/Action.h"
#include "Sound.h"
#include "Sprite.h"

/* SkillFlags enumerated flags for classification of skill types */
ENUM_FLAGS(SkillFlags)
enum class SkillFlags
{
  ALTERING   = 1 << 0,
  HEALING    = 1 << 1,
  INFLICTING = 1 << 2,
  RELIEVING  = 1 << 3,
  REVIVING   = 1 << 4,
  ASSIGNING  = 1 << 5,
  ABSORBING  = 1 << 6,
  VALID      = 1 << 7
};

class Skill
{
public:
  /* Constructs a default Skill object */
  Skill();

  /* Basic Skill constructor given a name */
  Skill(const std::string &name);

  /* General Skill construction with one effect */
  Skill(const int &id, const std::string &name, const ActionScope &scope,
        Action* effect, const float &chance, const uint32_t &cost = 0);

  /* General skill construction with multiple effects */
  Skill(const int &id, const std::string &name, const ActionScope &scope, 
  	    const std::vector<Action*> &effects, const std::vector<float> &chances,
        const uint32_t &cost = 0);

  /* Annihilates a Skill object */
  ~Skill() = default;

private:
  /* Pointer to the animation played by the Skill during Battle */
  Sprite* animation;

  /* Amount of turns the Skill takes to use */
  uint32_t cooldown;

  /* Cost of the Skill measured in QD */
  uint32_t cost;

  /* Description of the Skill */
  std::string description;

  /* Vector of effects the Skill does during use */
  std::vector<Action*> effects;

  /* Vector of chances correlating to the chance each effect has */
  std::vector<float> chances;

  /* Set of SkillFlags for skill categorization */
  SkillFlags flags;

  /* The ID of the Skill */
  int32_t id;

  /* String name of the Skill */
  std::string name;

  /* Primary elemental strength of the Skill */
  Element primary;

  /* Secondary elemental strength of the Skill */
  Element secondary;

  /* Pointer to the sound effect on use of the Skill */
  Sound* sound_effect;

  /* Scope of use of the action (one enemey, one ally, etc.) */
  ActionScope scope;

  /* Pointer to the thumbnail of the Skill */
  Frame* thumbnail;

  /* String message displayed upon use of the Skill */
  std::string message;

  /* Arbitrary point value of the Skill (for AI-use) */
  uint32_t value;

  /* ------------ Constants --------------- */
  static const size_t   kMAX_ACTIONS;     /* Maximum # of actions in a skill */
  static const uint32_t kMAX_COOLDOWN;    /* Maximum turn cooldown time */
  static const uint32_t kMAX_COST;        /* Highest possible cost for a skill */
  static const size_t   kMAX_MESG_LENGTH; /* Maximum length for using message */
  static const size_t   kMAX_NAME_LENGTH; /* Maximum length for a valid name */
  static const size_t   kMAX_DESC_LENGTH; /* Maximum length for a valid desc */
  static const uint32_t kMAX_VALUE;       /* Maximum assigned point value */
  static const int      kUNSET_ID;        /* ID for an unset Skill */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Determines the Skill classification based on contained effects */
  void flagSetup();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to add an action given a chance to the effect list */
  bool addAction(Action* new_action, const float &new_chance, 
                 const bool &single = true);

  /* Attempts to add a vector of effects to the Skill */
  bool addActions(const std::vector<Action*> &new_actions, 
                  const std::vector<float> &new_chances);

  /* Determines if the Skill is valid */
  bool isValid();

  /* Prints out the information about the current Skill state */
  void print();

  /* Attempts to remove an action from the vector of effects by index */
  bool removeAction(const uint32_t &index);

  /* Returns a pointer to the animation sprite */
  Sprite* getAnimation();
 
  /* Returns the cooldown of the Skill */
  uint32_t getCooldown();

  /* Returns the cost of the Skill */
  uint32_t getCost();

  /* Returns the chance of a given effect */
  float getChance(const uint32_t &index);

  /* Returns the vector of chances for the effects */
  std::vector<float> getChances();

  /* Returns the string description */
  std::string getDescription();

  /* Returns the effect of a given index */
  Action* getEffect(const uint32_t &index);

  /* Returns the vector of effects of the Skill */
  std::vector<Action*> getEffects();

  /* Returns the evaluation of a given SkillFlags flag */
  bool getFlag(SkillFlags test_flag);

  /* Returns the ID of the Skill */
  int getID();

  /* Returns the string name */
  std::string getName();

  /* Returns the primary element */
  Element getPrimary();

  /* Returns the secondary element */
  Element getSecondary();

  /* Returns the sound effect */
  Sound* getSoundEffect();

  /* Returns the enumerated scope */
  ActionScope getScope();

  /* Returns the thumbnail */
  Frame* getThumbnail();

  /* Returns the using message */
  std::string getMessage();

  /* Returns the point value */
  uint32_t getValue();

  /* Assigns a new using animation */
  bool setAnimation(Sprite* new_animation);

  /* Assigns a new cooldown */
  bool setCooldown(const uint32_t &new_value);

  /* Assigns a new cost to the skill */
  bool setCost(const uint32_t &new_value);

  /* Assigns a new description */
  bool setDescription(const std::string &new_description);

  /* Assigns a given flag to a value */
  void setFlag(const SkillFlags &flag, const bool &set_value = true);

  /* Assigns an ID to the Skill */
  bool setID(const int &new_id);
  
  /* Assigns a string name to the skill */
  bool setName(const std::string &new_name);

  /* Assigns a new primary elemental strength */
  void setPrimary(const Element &new_element);

  /* Assigns a new secondary elemental strength */
  void setSecondary(const Element &new_element);

  /* Assigns a new sound effect */
  bool setSoundEffect(Sound* new_sound_effect);

  /* Assigns a new action scope */
  void setScope(const ActionScope &new_scope);

  /* Assigns a new thumbnail */
  void setThumbnail(Frame* new_thumbnail);

  /* Assigns a new using message */
  bool setMessage(const std::string &new_message);

  /* Assigns a new point value */
  bool setValue(const uint32_t &new_value);
};

#endif //SKILL_H