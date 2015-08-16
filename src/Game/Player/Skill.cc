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

/*==============================================================================
 * CONSTANTS
 *============================================================================*/
const uint16_t Skill::kDEFAULT_VALUE     =      1;
const size_t   Skill::kMAX_ACTIONS       =   1000;
const float    Skill::kMAX_CHANCE        =    100;
const uint16_t Skill::kMAX_COOLDOWN      =     10;
const uint32_t Skill::kMAX_COST          =   5000;
const size_t   Skill::kMAX_MESG_LENGTH   =     70;
const size_t   Skill::kMAX_NAME_LENGTH   =     60;
const size_t   Skill::kMAX_DESC_LENGTH   =    500;
const uint16_t Skill::kMAX_VALUE         =     10;
const int32_t  Skill::kUNSET_ID          =     -1;

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
    , chance{0}
    , cooldown{0}
    , cost{0}
    , description{0}
    , flags{static_cast<SkillFlags>(0)}
    , id{kUNSET_ID}
    , message{""}
    , primary{Element::NONE}
    , scope{ActionScope::NO_SCOPE}
    , secondary{Element::NONE}
    , sound_effect{nullptr}
    , thumbnail{nullptr}
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
 *         cost - the cost of the skill in QD
 */
Skill::Skill(const int &id, const std::string &name, const ActionScope &scope,
    Action* effect, const float &chance, const uint32_t &cost)
      : Skill::Skill()
{
  setID(id);
  setName(name);
  setScope(scope);
  setCost(cost);
  setChance(chance);
  setValue(kDEFAULT_VALUE);

  /* The skill will be valid of the action is able to be added */
  addAction(effect);
}

/*
 * Description: General construction of a Skill given an ID, name, scope and
 *              chance of occuring.
 *
 * Inputs: id - the ID number for the Skill
 *         name - string name for the Skill
 *         scope - enumerated conditions of use for the effects of the Skill
 *         effects - vector of effects the action does
 *         chance - chance of all the effects happening
 *         cost - the cost of the skill in QD
 */
Skill::Skill(const int &id, const std::string &name, const ActionScope &scope,
    const std::vector<Action*> &effects,
    const float &chance, const uint32_t &cost)
      : Skill::Skill()
{
  setID(id);
  setName(name);
  setScope(scope);
  setChance(chance);
  setCost(cost);

  /* The skill will be valid if all the actions are able to be added */
  addActions(effects);

  /* Flags need to be determined since a single addition did not take place */
  flagSetup();
}

/*
 * Description: Destructor function
 */
Skill::~Skill()
{
  unsetAnimation();
  unsetThumbnail();
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

      if ((*it)->getUserAttribute() == Attribute::VITA && (*it)->getBase() > 0)
        setFlag(SkillFlags::HEALING);
    }

    else if ((*it)->actionFlag(ActionFlags::DAMAGE))
      setFlag(SkillFlags::DAMAGING);

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

      if ((*it)->getUserAttribute() == Attribute::VITA && (*it)->getBase() > 0)
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
 *         single - boolean whether to call flagSetup (only needs one call)
 * Output: bool - true if the effect can and was added
 */
bool Skill::addAction(Action* new_action, const bool &single)
{
  if (new_action != nullptr && effects.size() < kMAX_ACTIONS)
  {
    effects.push_back(new_action);

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
 * Output: bool - true if all the actions were successfully added
 */
bool Skill::addActions(const std::vector<Action*> &new_actions)
{
  bool valid = true;

  if (new_actions.size() > 0)
  {
    /* Temporarily store the effects */
    auto temp_effects = effects;

    for (auto it_e = begin(new_actions); it_e != end(new_actions); ++it_e)
      if (!addAction((*it_e), false))
        valid = false;

    /* Undo the additions if they were not valid */
    if (!valid)
      effects = temp_effects;
  }

  return valid;
}

/*
 * Description: Determines the validity of a skill used for person while
 *              Berserked.
 *
 * Inputs: none
 * Output: bool - true if the skill is valid for a Berserked person
 */
bool Skill::isBerserkSkill()
{
  auto has_bad_effect = true;

  has_bad_effect &= getFlag(SkillFlags::ALTERING);
  has_bad_effect &= getFlag(SkillFlags::HEALING);
  has_bad_effect &= getFlag(SkillFlags::INFLICTING);
  has_bad_effect &= getFlag(SkillFlags::RELIEVING);
  has_bad_effect &= getFlag(SkillFlags::REVIVING);
  has_bad_effect &= getFlag(SkillFlags::ASSIGNING);

  if (has_bad_effect)
    return false;

  return getFlag(SkillFlags::DAMAGING);
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
void Skill::print(bool flags)
{
  std::cout << "--- Skill ----\n";
  std::cout << "Skill ID: " << id << std::endl;
  std::cout << "\nAction Scope: " << Helpers::actionScopeToStr(scope);
  std::cout << "\nName: " << name << std::endl;
  std::cout << "Animation: " << animation << std::endl;
  std::cout << "Chance: " << chance << std::endl;
  std::cout << "Cooldown: " << cooldown << std::endl;
  std::cout << "Cost: " << cost << std::endl;
  std::cout << "Description: " << description << std::endl;
  std::cout << "Point Value: " << value << std::endl;
  std::cout << "Primary Element " << Helpers::elementToString(primary);
  std::cout << "\nSecondary Element " << Helpers::elementToString(secondary);
  std::cout << "\nThumbnail: " << thumbnail << std::endl;
  std::cout << "Effect Chances: " << std::endl;
  std::cout << "\nSound effect set? " << sound_effect << std::endl;

  for (const auto& effect : effects)
    std::cout << "Action Chance: " << effect->getChance() << "\n";

  if (flags)
  {
    std::cout << "OFFENSIVE: " << getFlag(SkillFlags::OFFENSIVE) << std::endl;
    std::cout << "DEFENSIVE: " << getFlag(SkillFlags::DEFENSIVE) << std::endl;
    std::cout << "NEUTRAL: " << getFlag(SkillFlags::NEUTRAL) << std::endl;
    std::cout << "ALTERING: " << getFlag(SkillFlags::ALTERING) << std::endl;
    std::cout << "DAMAGING: " << getFlag(SkillFlags::DAMAGING) << std::endl;
    std::cout << "HEALING: " << getFlag(SkillFlags::HEALING) << std::endl;
    std::cout << "INFLICTING: " << getFlag(SkillFlags::INFLICTING) << std::endl;
    std::cout << "RELIEVING: " << getFlag(SkillFlags::RELIEVING) << std::endl;
    std::cout << "REVIVING: " << getFlag(SkillFlags::REVIVING) << std::endl;
    std::cout << "ASSIGNING: " << getFlag(SkillFlags::ASSIGNING) << std::endl;
    std::cout << "VALID: " << getFlag(SkillFlags::VALID) << std::endl;
  }
  std::cout << "------" << std::endl;
}

/*
 * Description: Attempts to remove an action from the Skill being give an index.
 *
 * Inputs: index - the index of the skill to be removed
 * Output: bool - true if the skill was removed
 */
bool Skill::removeAction(const uint32_t &index)
{
  if (index < effects.size())
  {
    effects.erase(effects.begin() + index);

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
 * Description: Returns the chance of an effect of a given index, if it exists
 *
 * Inputs: index - the index to be checked for a chance
 * Output: float - the chance of the given effect index, or 0.
 */
float Skill::getChance()
{
  return chance;
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
 * Description: Assigns a new animation to the Skill. This class takes control
 *              of memory manipulation upon set.
 *
 * Inputs: Sprite* - pointer to the new animation
 * Output: bool - true if the new animation is not nullptr
 */
bool Skill::setAnimation(Sprite* new_animation)
{
  if(new_animation != nullptr)
  {
    unsetAnimation();
    animation = new_animation;
    return true;
  }
  return false;
}

/*
 * Description: Assign a new chance for occuring to the skill. Return true
 *              if the chance is under the max_value and above zero.
 *
 * Inputs: new_chance - new chance for the Skill to occur
 * Output: bool - true if the chance was assigned without altering.
 */
bool Skill::setChance(const float &new_chance)
{
  if (new_chance <= kMAX_CHANCE && new_chance > 0)
  {
    chance = new_chance;

    return true;
  }
  else if (new_chance > 0)
  {
    chance = kMAX_CHANCE;
  }

  chance = 0;

  return false;
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
 * Description: Attempts to assign the cost of the Skill. If the cost is above
 *              the maximum allowable, assign the highest possible cost.
 *
 * Inputs: new_value - the new cost for the Skill
 * Output: bool - true if the cost was assigned without altering
 */
bool Skill::setCost(const uint32_t &new_cost)
{
  if (new_cost <= kMAX_COST)
  {
    cost = new_cost;

    return true;
  }

  cost = kMAX_COST;

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
bool Skill::setID(const int32_t &new_id)
{
  if (new_id == kUNSET_ID || id != kUNSET_ID)
    return false;

  id = new_id;

  return true;
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
 * Description: Assigns a new primary element.
 *
 * Inputs: Element new_element - enumerated element to assign
 * Output: none
 */
void Skill::setPrimary(const Element &new_element)
{
  primary = new_element;
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
 * Description: Assigns a new secondary element.
 *
 * Inputs: Element new_element - enumerated element to assign
 * Output: none
 */
void Skill::setSecondary(const Element & new_element)
{
  secondary = new_element;
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
 * Description: Assigns a new thumbnail to the Skill. The class takes control
 *              of the memory of the frame.
 *
 * Inputs: new_thumbnail - thumbnail to be assigned to the Skill
 * Output: none
 */
void Skill::setThumbnail(Frame* new_thumbnail)
{
  if (new_thumbnail != nullptr)
  {
    unsetThumbnail();
    thumbnail = new_thumbnail;
  }
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

void Skill::unsetAnimation()
{
  if(animation != nullptr)
    delete animation;
  animation = nullptr;
}

/*
 * Description: Unsets the thumbnail and deletes it from memory.
 *
 * Inputs: none
 * Output: none
 */
void Skill::unsetThumbnail()
{
  if(thumbnail != nullptr)
    delete thumbnail;
  thumbnail = nullptr;
}
