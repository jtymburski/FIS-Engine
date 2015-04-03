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
* [1]: A Skill is a possible selection for a BattleAction. When a Skill is 
*      chosen, its cost in QD is removed from the user (unless the Skill is 
*      a skill from an Item). When the action is performed, each Action (effect)
*      in the skill performs an effect on the selected targets.
*
* TODO
* ----
* [08-13-14] Skill construction from XML Data
*******************************************************************************/
#ifndef SKILL_H
#define SKILL_H

#include "Game/Player/Action.h"
#include "Sound.h"
#include "Sprite.h"

/* SkillFlags enumerated flags for classification of skill types */
ENUM_FLAGS(SkillFlags)
enum class SkillFlags
{
  OFFENSIVE  = 1 << 0, /* Is this skill OFFENSIVE against targets (does dmg)? */
  DEFENSIVE  = 1 << 1, /* Is this skill beneficial to its targets? */
  NEUTRAL    = 1 << 2, /* Is this skill not-offensive and not-defensive? */
  ALTERING   = 1 << 3, /* Does this skill contain an ALTERING effect? */
  DAMAGING   = 1 << 4, /* Does this skill contain a DAMAGING effect? */
  HEALING    = 1 << 5, /* Does this skill contain a HEALING effect? */
  INFLICTING = 1 << 6, /* Does this skill contain an INFLICTING effect? */
  RELIEVING  = 1 << 7, /* Does this skill contain a RELIEVING effect? */
  REVIVING   = 1 << 8, /* Does this skill contain a REVIVING effect? */
  ASSIGNING  = 1 << 9, /* Does this skill contain an ASSIGNING effect? */
  VALID      = 1 << 11  /* Is this a valid skill? */
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
  	    const std::vector<Action*> &effects, const float &chance,
        const uint32_t &cost = 0);

  /* Destructor function */
  ~Skill();

private:
  /* Pointer to the animation played by the Skill during Battle */
  Sprite* animation;

  /* Chance the skill has overall to hit (100 = always hit) */
  float chance;

  /* Amount of turns the Skill takes to use */
  uint16_t cooldown;

  /* Cost of the Skill measured in QD */
  uint32_t cost;

  /* Description of the Skill */
  std::string description;

  /* Vector of effects the Skill does during use */
  std::vector<Action*> effects;

  /* Set of SkillFlags for skill categorization */
  SkillFlags flags;

  /* The ID of the Skill */
  int32_t id;

  /* String message displayed upon use of the Skill */
  std::string message;

  /* String name of the Skill */
  std::string name;

  /* Primary elemental strength of the Skill */
  Element primary;

  /* Scope of use of the action (one enemey, one ally, etc.) */
  ActionScope scope;

  /* Secondary elemental strength of the Skill */
  Element secondary;

  /* Pointer to the sound effect on use of the Skill */
  Sound* sound_effect;

  /* Pointer to the thumbnail of the Skill */
  Frame* thumbnail;

  /* Arbitrary point value of the Skill (for AI-use) */
  uint16_t value;

  /* ------------ Constants --------------- */
  static const uint16_t kDEFAULT_VALUE;   /* Default value (points) */
  static const size_t   kMAX_ACTIONS;     /* Maximum # of actions in a skill */
  static const float    kMAX_CHANCE;      /* Maximum value of chance */
  static const uint16_t kMAX_COOLDOWN;    /* Maximum turn cooldown time */
  static const uint32_t kMAX_COST;        /* Highest possible cost for skill */
  static const size_t   kMAX_DESC_LENGTH; /* Maximum length for a valid desc */
  static const size_t   kMAX_MESG_LENGTH; /* Maximum length for using message */
  static const size_t   kMAX_NAME_LENGTH; /* Maximum length for a valid name */
  static const uint16_t kMAX_VALUE;       /* Maximum assigned point value */
  static const int32_t  kUNSET_ID;        /* ID for an unset Skill */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Add effect data */
  bool addEffectData();

  /* Determines the Skill classification based on contained effects */
  void flagSetup();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to add an action given a chance to the effect list */
  bool addAction(Action* new_action, const bool &single = true);

  /* Attempts to add a vector of effects to the Skill */
  bool addActions(const std::vector<Action*> &new_actions);

  /* Determines if a Skill is able to be used with Berserk */
  bool isBerserkSkill();

  /* Determines if the Skill is valid */
  bool isValid();

  /* Prints out the information about the current Skill state */
  void print(bool flags);

  /* Attempts to remove an action from the vector of effects by index */
  bool removeAction(const uint32_t &index);

  /* Returns a pointer to the animation sprite */
  Sprite* getAnimation();
 
  /* Returns the chance of a given effect */
  float getChance();

  /* Returns the cooldown of the Skill */
  uint32_t getCooldown();

  /* Returns the cost of the Skill */
  uint32_t getCost();

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

  /* Returns the using message */
  std::string getMessage();

  /* Returns the string name */
  std::string getName();

  /* Returns the primary element */
  Element getPrimary();

  /* Returns the enumerated scope */
  ActionScope getScope();
  
  /* Returns the secondary element */
  Element getSecondary();

  /* Returns the sound effect */
  Sound* getSoundEffect();

  /* Returns the thumbnail */
  Frame* getThumbnail();

  /* Returns the point value */
  uint32_t getValue();

  /* Assigns a new using animation */
  bool setAnimation(Sprite* new_animation);

  /* Assigns a new chance to the skill */
  bool setChance(const float &new_chance);

  /* Assigns a new cooldown */
  bool setCooldown(const uint32_t &new_value);

  /* Assigns a new cost to the skill */
  bool setCost(const uint32_t &new_value);

  /* Assigns a new description */
  bool setDescription(const std::string &new_description);

  /* Assigns a given flag to a value */
  void setFlag(const SkillFlags &flag, const bool &set_value = true);

  /* Assigns an ID to the Skill */
  bool setID(const int32_t &new_id);
  
  /* Assigns a new using message */
  bool setMessage(const std::string &new_message);

  /* Assigns a string name to the skill */
  bool setName(const std::string &new_name);

  /* Assigns a new primary elemental strength */
  void setPrimary(const Element &new_element);
  
  /* Assigns a new action scope */
  void setScope(const ActionScope &new_scope);
  
  /* Assigns a new secondary elemental strength */
  void setSecondary(const Element &new_element);

  /* Assigns a new sound effect */
  bool setSoundEffect(Sound* new_sound_effect);

  /* Assigns a new thumbnail */
  void setThumbnail(Frame* new_thumbnail);

  /* Assigns a new point value */
  bool setValue(const uint32_t &new_value);
 
  /* Unsets the animation - deletes from memory */
  void unsetAnimation();

  /* Unsets the thumbnail - deletes from memory */
  void unsetThumbnail();
};

#endif //SKILL_H
