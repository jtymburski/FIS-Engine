/*******************************************************************************
* Class Name: Skill [Implementation]
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
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Skill.h"

/*===============================f==============================================
 * CONSTANTS
 *============================================================================*/
const size_t   Skill::kMAX_ACTIONS       =   10;
const uint32_t Skill::kMAX_COOLDOWN      =   10;
const uint32_t Skill::kMAX_COST          = 5000;
const size_t   Skill::kMAX_MESG_LENGTH   =   70;
const size_t   Skill::kMAX_NAME_LENGTH   =   60;
const size_t   Skill::kMAX_DESC_LENGTH   =  500;
const uint32_t Skill::kMAX_VALUE         =   10;
const int32_t  Skill::kUNSET_ID          =   -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a default (invalid) Skill object.
 *
 * Inputs: none
 */
Skill::Skill()
  : animation{nullptr}
  , cooldown{0}
  , cost{0}
  , description{0}
  , flags{static_cast<SkillFlags>(0)}
  , id{kUNSET_ID}
  , primary{Element::NONE}
  , secondary{Element::NONE}
  , sound_effect{nullptr}
  , scope{ActionScope::NO_SCOPE}
  , thumbnail{nullptr}
  , message{""}
  , value{0}
{}

/*
 * Description: Constructs a basic empty Skill given a name.
 *
 * Inputs: name - string name for the Skill
 */
Skill::Skill(const std::string &name)
  : Skill::Skill()
{
  setName(name);
}

/*
 * Description: General construction of a Skill given an ID, name, scope and 
 *              a single effect with a chance for it occuring.
 *
 * Inputs: id - the ID number for the Skill
 *         name - string name for the Skill
 *         scope - enumerated conditions of use for the effects of the Skill
 *         effect - pointer to an action which the Skill will do
 *         chance - float chance for the action to take place.
 */
Skill::Skill(const int &id, const std::string &name, const ActionScope &scope,
	           Action* effect, const float &chance)
  : Skill::Skill()
{
  setID(id);
  setName(name);
  setScope(scope);

  /* The skill will be valid of the action is able to be added */
  addAction(effect, chance);
}

/*
 * Description: General construction of a Skill given an ID, name, scope and
 *              a vector of effects with corresponding vector of chances.
 *
 * Inputs: id - the ID number for the Skill
 *         name - string name for the Skill
 *         scope - enumerated conditions of use for the effects of the Skill
 *         effects - vector of effects the action does
 *         chances - vector of chances relating to the effects
 */
Skill::Skill(const int &id, const std::string &name, const ActionScope &scope, 
	           const std::vector<Action*> &effects, 
	           const std::vector<float> &chances)
  : Skill::Skill()
{
  setID(id);
  setName(name);
  setScope(scope);

  /* The skill will be valid if all the actions are able to be added */
  addActions(effects, chances);

  /* Flags need to be determined since a single addition did not take place */
  flagSetup();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Assigns the SkillFlag (categorization of Skill types) based
 *              on the ActionKeywords and related of each action effect
 *              contained in the Skill.
 *
 * Inputs: none
 * Output: none
 */
void Skill::flagSetup()
{
  for (auto it = effects.begin(); it != effects.end(); ++it)
  {
    if ((*it)->actionFlag(ActionFlags::ALTER))
    {
      setFlag(SkillFlags::ALTERING);

      if ((*it)->getAttribute() == Attribute::VITA && (*it)->getBase() > 0)
        setFlag(SkillFlags::HEALING);
    }

    else if ((*it)->actionFlag(ActionFlags::INFLICT))
      setFlag(SkillFlags::INFLICTING);

    else if ((*it)->actionFlag(ActionFlags::RELIEVE))
      setFlag(SkillFlags::RELIEVING);

    else if ((*it)->actionFlag(ActionFlags::REVIVE))
    {
      setFlag(SkillFlags::REVIVING);
      setFlag(SkillFlags::HEALING);
    }

    else if ((*it)->actionFlag(ActionFlags::ASSIGN))
    {
      setFlag(SkillFlags::ASSIGNING);

      if ((*it)->getAttribute() == Attribute::VITA && (*it)->getBase() > 0)
        setFlag(SkillFlags::HEALING);
    }
  }

  setFlag(SkillFlags::VALID, this->isValid());
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add an action with a chance for it occuring
 *              to those already included in the Skill.
 *
 * Inputs: new_action - pointer to the action to be added to the Skill
 *         new_chance - chance for the action to occur
 *         single - boolean whether to call flagSetup (only needs one call)
 * Output: bool - true if the effect can and was added
 */
bool Skill::addAction(Action* new_action, const float &new_chance, 
                      const bool &single)
{
  if (new_action != nullptr && effects.size() < kMAX_ACTIONS)
  {
    effects.push_back(new_action);
    chances.push_back(new_chance);

    if (single)
      flagSetup();

    return true;
  }
 
  return false;
}

/*
 * Description: Attempts to add a number of effects to the Skill given a vector
 *              of Action ptrs and a vector of chances to go with them.
 *
 * Inputs: new_actions - vector of pointers to effects to be added
 *         new_chances - vector of chances corresponding to the effects
 * Output: bool - true if all the actions were successfully added
 */
bool Skill::addActions(const std::vector<Action*> &new_actions, 
	                     const std::vector<float> &new_chances)
{
  bool valid = true;

  if (new_actions.size() == new_chances.size())
  {
    /* Temporarily store the effects and chances */
    auto temp_effects = effects;
    auto temp_chances = chances;

    auto it_e = new_actions.begin();
    auto it_c = new_chances.begin();

    for (; it_e != new_actions.end(), it_c != new_chances.end(); ++it_e, ++it_c)
      if (!addAction((*it_e), (*it_c)), false)
        valid = false;

    /* Undo the additions if they were not valid */
    if (!valid)
    {
      effects = temp_effects;
      chances = temp_chances;
    }

  }

  return valid;
}

/*
 * Description: Evaluates and returns  the validity of the current Skill object.
 *
 * Inputs: none
 * Output: bool - validity of the current Skill object
 */
bool Skill::isValid()
{
  bool valid = true;

  for (auto it = effects.begin(); it != effects.end(); ++it)
    valid &= (*it)->actionFlag(ActionFlags::VALID);

  valid &= (effects.size() == chances.size());
  valid &= (id != kUNSET_ID);
  valid &= (scope != ActionScope::NO_SCOPE);

  return valid;
}

/*
 * Description: Outputs [std::cout] the current state of the Skill
 *
 * Inputs: none
 * Output: none
 */
void Skill::print()
{
  std::cout << "--- Skill ----\n";
  std::cout << "Skill ID: " << id << std::endl;
  std::cout << "Name: " << name << std::endl;
  std::cout << "Animation set? " << !(animation == nullptr) << std::endl;
  std::cout << "Cooldown: " << cooldown << std::endl; 
  std::cout << "Cost: " << cost << std::endl;
  std::cout << "Description: " << description << std::endl;

  std::cout << "# Effects: " << effects.size() << std::endl;
  std::cout << "# Chances: " << chances.size() << std::endl;

  std::cout << "Primary Element" << Helpers::elementToString(primary) << std::endl;
  std::cout << "Secondary Element" << Helpers::elementToString(secondary) << std::endl;
  std::cout << "Sound effect set? " << !(sound_effect == nullptr) << std::endl;
  std::cout << "[void]Action scope: " << std::endl;
  std::cout << "Thumb Set? " << !(thumbnail == nullptr) << std::endl;
  std::cout << "Point Value: " << value << std::endl;
  std::cout << "----" << std::endl;
  std::cout << "ALTERING: " << getFlag(SkillFlags::ALTERING) << std::endl;
  std::cout << "HEALING: " << getFlag(SkillFlags::HEALING) << std::endl;
  std::cout << "INFLICTING: " << getFlag(SkillFlags::INFLICTING) << std::endl;
  std::cout << "RELIEVING: " << getFlag(SkillFlags::RELIEVING) << std::endl;
  std::cout << "REVIVING: " << getFlag(SkillFlags::REVIVING) << std::endl;
  std::cout << "ASSIGNING: " << getFlag(SkillFlags::ASSIGNING) << std::endl;
  std::cout << "VALID: " << getFlag(SkillFlags::VALID) << std::endl;
}

/*
 * Description: Attempts to remove an action from the Skill being give an index.
 *
 * Inputs: index - the index of the skill to be removed
 * Output: bool - true if the skill was removed
 */
bool Skill::removeAction(const uint32_t &index)
{
  if ((effects.size() == chances.size()) && index < effects.size())
  {
    effects.erase(effects.begin() + index);
    chances.erase(chances.begin() + index);

    return true;
  }

  return false;
}

/*
 * Description: Returns a pointer to the using animation of the Skill.
 *
 * Inputs: none
 * Output: Sprite* 
 */
Sprite* Skill::getAnimation()
{
  return animation;
}
 
/*
 * Description: Returns the cooldown of the Skill
 *
 * Inputs: none
 * Output: uint32_t - the Skill's cooldown
 */
uint32_t Skill::getCooldown()
{
  return cooldown;
}

/*
 * Description: Returns the cost of the Skill
 *
 * Inputs: none
 * Output: uint32_t - the Skill's cost
 */
uint32_t Skill::getCost()
{
  return cost;
}

/*
 * Description: Returns the chance of an effect of a given index, if it exists
 *
 * Inputs: index - the index to be checked for a chance
 * Output: float - the chance of the given effect index, or 0.
 */
float Skill::getChance(const uint32_t &index)
{
  if (index < effects.size())
    return chances[index];

  return 0;
}

/*
 * Description: Returns the complete vector of chances
 *
 * Inputs: none
 * Output: std::vector<float> - the complete vector of chances of the Skill
 */
std::vector<float> Skill::getChances()
{
  return chances;
}

/*
 * Description: Returns the string description of the Skill
 *
 * Inputs: none
 * Output: std::string - the string description
 */
std::string Skill::getDescription()
{
  return description;
}

/*
 * Description: Returns the effect of a given index.
 *
 * Inputs: uint32_t - index of effect to be checked
 * Output: Action* - ptr to the effect at the index, or nullptr
 */
Action* Skill::getEffect(const uint32_t &index)
{
  if (index < effects.size())
    return effects[index];

  return nullptr;
}

/*
 * Description: Returns the complete vector of effects
 *
 * Inputs: none
 * Output: std::vector<Action*> - the vector of effects of the Skill
 */
std::vector<Action*> Skill::getEffects()
{
  return effects;
}

/*
 * Description: Evaluates and returns a given SkillFlags by comparing it to
 *              the current flag set to a bit-wise of itself.
 *
 * Inputs: test_flag - the flag to be evaluated
 * Output: bool - the evaluation of the given flag
 */
bool Skill::getFlag(SkillFlags test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Returns the ID of the Skill.
 *
 * Inputs: none
 * Output: int - the ID of the Skill.
 */
int Skill::getID()
{
  return id;
}

/*
 * Description: Returns the string name of the Skill
 *
 * Inputs: none
 * Output: std::string - the name of the Skill
 */
std::string Skill::getName()
{
  return name;
}

/*
 * Description: Returns the primary elemental strength
 *
 * Inputs: none
 * Output: Element - enumerated element [primary Skill strength]
 */
Element Skill::getPrimary()
{
  return primary;
}

/*
 * Description: Returns the secondary elemental strength
 *
 * Inputs: none
 * Output: Element - enumerated element [secondary Skill strength]
 */
Element Skill::getSecondary()
{
  return secondary;
}

/*
 * Description: Returns the pointer to the sound effect
 *
 * Inputs: none
 * Output: Sound* - pointer to the sound effect
 */
Sound* Skill::getSoundEffect()
{
  return sound_effect;
}

/*
 * Description: Returns the ActionScope (usage condition)
 *
 * Inputs: none
 * Output: ActionScope - enumerated usage condition
 */
ActionScope Skill::getScope()
{
  return scope;
}

/*
 * Description: Returns the Skill's thumbnail
 *
 * Inputs: none
 * Output: Frame* - ptr to the thumbnail
 */
Frame* Skill::getThumbnail()
{
  return thumbnail;
}

/*
 * Description: Returns the string message
 *
 * Inputs: none
 * Output: std::string - the message displayed upon Skill use
 */
std::string Skill::getMessage()
{
  return message;
}

/*
 * Description: Returns the assigned point value of the Skill
 *
 * Inputs: none
 * Output: uint32_t - the assigned point value of the Skill
 */
uint32_t Skill::getValue()
{
  return value;
}

/*
 * Description: Assigns a new animation to the Skill
 *
 * Inputs: Sprite* - pointer to the new animation
 * Output: bool - true if the new animation is nullptr
 */
bool Skill::setAnimation(Sprite* new_animation)
{
  animation = new_animation;

  return (animation != nullptr);
}

/*
 * Description: Assigns a new cooldown to the Skill.
 *
 * Inputs: uint32_t - new value for the cooldown.
 * Output: bool - true if the new cooldown value is within range.
 */
bool Skill::setCooldown(const uint32_t &new_value)
{
  if (new_value < kMAX_COOLDOWN)
  {
    cooldown = new_value;

    return true;
  }

  cooldown = 0;

  return false;
}

/*
 * Description: Assigns a new string description for the Skill.
 *
 * Inputs: new_description - new string description for the Skill
 * Output: bool - true if the new description was in range
 */
bool Skill::setDescription(const std::string &new_description)
{
  if (new_description.size() < kMAX_DESC_LENGTH)
  {
    description = new_description;

    return true;
  }

  description = "";

  return false;
}

/*
 * Description: Assigns a given SkillFlags flag to a value
 *
 * Inputs: flag - const ref to a flag object to be changed
 *         set_value - value to change the flag to
 * Output: none
 */
void Skill::setFlag(const SkillFlags &flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description: Assigs a new string name to the Skill
 *
 * Inputs: new_name - new name to be assigned
 * Output: bool - true if the new name size was within range
 */
bool Skill::setName(const std::string &new_name)
{
  if (new_name.size() < kMAX_NAME_LENGTH)
  {
    name = new_name;

    return true;
  }

  name = "";

  return false;
}

/*
 * Description: Assigns a new id to the skill.
 *
 * Inputs: new_id - the ID to assign the skill to
 * Output: bool - true if the ID was assigned (can't reassign a set ID)
 */
bool Skill::setID(const int &new_id)
{
  if (new_id == kUNSET_ID || id != kUNSET_ID)
    return false;

  id = new_id;

  return true;
}

/*
 * Description: Assigns a new sound effect to the Skill
 *
 * Inputs: new_sound_effect - new sound effect for the use of the Skill
 * Output: bool - true if the new sound effect is not nullptr
 */
bool Skill::setSoundEffect(Sound* new_sound_effect)
{
  sound_effect = new_sound_effect;

  return (sound_effect != nullptr);
}

/*
 * Description: Assigns a new ActionScope (conditions of usage)
 *
 * Inputs: new_scope - new enumerated scope of use for the Skill
 * Output: none
 */
void Skill::setScope(const ActionScope &new_scope)
{
  scope = new_scope;
}

/*
 * Description: Assigns a new thumbnail to the Skill
 *
 * Inputs: new_thumbnail - thumbnail to be assigned to the Skill
 * Output: none
 */
void Skill::setThumbnail(Frame* new_thumbnail)
{
  thumbnail = new_thumbnail;
}

/*
 * Description: Assigns a new using message.
 *
 * Inputs: new_message - new using message string
 * Output: bool - true if the new message was within range
 */
bool Skill::setMessage(const std::string &new_message)
{
  if (new_message.size() < kMAX_MESG_LENGTH)
  {
    message = new_message;
    
    return true;
  }

  return false;
}

/*
 * Description: Assigns a new point value for use with AI decisions.
 *
 * Inputs: new_value - new point value for the skill
 * Output: bool - true if the point value is within range
 */
bool Skill::setValue(const uint32_t &new_value)
{
  if (new_value < kMAX_VALUE)
  {
    value = new_value;

    return true;
  }

  return false;
}