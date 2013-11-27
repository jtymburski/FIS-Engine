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

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
const size_t   Skill::kMAX_ACTIONS       =   10;
const uint8_t  Skill::kMAX_COOLDOWN      =   10;
const uint16_t Skill::kMAX_COST          = 5000;
const size_t   Skill::kMAX_MESG_LENGTH   =   70;
const size_t   Skill::kMAX_NAME_LENGTH   =   60;
const size_t   Skill::kMAX_DESC_LENGTH   =  500;
const uint8_t  Skill::kMAX_VALUE         =   10;
const int      Skill::kUNSET_ID          =   -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Skill::Skill()
{
  classSetup();
}

/*
 * Description:
 *
 * Inputs:
 */
Skill::Skill(const Skill &source)
{
  copySelf(source);
}

/*
 * Description:
 *
 * Inputs:
 */
Skill::Skill(const std::string &name)
{
  classSetup();
  setName(name);
}

/*
 * Description:
 *
 * Inputs:
 */
Skill::Skill(const int &id, const std::string &name, const ActionScope &scope,
	           Action* effect, const float &chance)
{
  classSetup();
  setID(id);
  setName(name);
  setScope(scope);

  if (addAction(effect, chance))
    setFlag(SkillFlags::VALID);

  flagSetup();
}

/*
 * Description:
 *
 * Inputs:
 */
Skill::Skill(const int &id, const std::string &name, const ActionScope &scope, 
	           const std::vector<Action*> &effects, 
	           const std::vector<float> &chances)
{
  classSetup();
  setID(id);
  setName(name);
  setScope(scope);

  if (addActions(effects, chances))
    setFlag(SkillFlags::VALID);

  flagSetup();
}

/*
 * Description:
 */
Skill::~Skill() {}


/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Skill::classSetup()
{
  animation = nullptr;
  cooldown = 0;
  cost = 0;
  description = "";
  flags = static_cast<SkillFlags>(0);
  name = "";
  id = kUNSET_ID;
  primary = Element::NONE;
  secondary = Element::NONE;
  sound_effect = nullptr;
  scope = ActionScope::NO_SCOPE;
  thumbnail = nullptr;
  message = "";
  value = 0;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Skill::copySelf(const Skill &source)
{
  animation    = source.animation;
  cooldown     = source.cooldown;
  cost         = source.cost;
  description  = source.description;
  flags        = source.flags;
  id           = source.id;  //TODO? [11-23-13]
  name         = source.name;
  primary      = source.primary;
  secondary    = source.secondary;
  sound_effect = source.sound_effect;
  scope        = source.scope;
  thumbnail    = source.thumbnail;
  message      = source.message;
  value        = source.value;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Skill::flagSetup()
{
  //TODO: Offensive/Defensive/Neutral categories? [11-25-13]

  for (auto it = effects.begin(); it != effects.end(); ++it)
  {
    if ((*it)->actionFlag(ActionFlags::ALTER))
      setFlag(SkillFlags::ALTERING);

    else if ((*it)->actionFlag(ActionFlags::INFLICT))
      setFlag(SkillFlags::INFLICTING);

    else if ((*it)->actionFlag(ActionFlags::RELIEVE))
      setFlag(SkillFlags::RELIEVING);

    else if ((*it)->actionFlag(ActionFlags::REVIVE))
      setFlag(SkillFlags::REVIVING);

    else if ((*it)->actionFlag(ActionFlags::ASSIGN))
      setFlag(SkillFlags::ASSIGNING);
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Skill::addAction(Action* new_action, const float &new_chance)
{
  if (effects.size() < kMAX_ACTIONS)
  {
    if (new_action != nullptr)
    {
      effects.push_back(new_action);
      chances.push_back(new_chance);

      return true;
    }
  }

  return false;
}

bool Skill::addActions(const std::vector<Action*> &new_actions, 
	                     const std::vector<float> &new_chances)
{
  bool valid = true;

  if (new_actions.size() == new_chances.size())
  {
    auto it_e = effects.begin();
    auto it_c = chances.begin();

    for ( ; it_e != effects.end(), it_c != chances.end(); ++it_e, ++it_c)
      if (!addAction((*it_e), (*it_c)))
        valid = false;
  }

  return valid;
}

bool Skill::isValid()
{
  bool valid = true;

  for (auto it = effects.begin(); it != effects.end(); ++it)
    if ((*it)->actionFlag(ActionFlags::VALID))
      valid = false;

  valid  &= (effects.size() == chances.size());

  return valid;
}

void Skill::print()
{
  std::cout << "--- Skill ----\n";
  std::cout << "Skill ID: " << id;
  std::cout << "Name: " << name << std::endl;
  std::cout << "Animation set? " << !(animation == nullptr) << std::endl;
  std::cout << "Cooldown: " << cooldown << std::endl; 
  std::cout << "Cost: " << cost << std::endl;
  std::cout << "Description: " << description << std::endl;
  std::cout << "# Effects: " << effects.size() << std::endl;
  std::cout << "# Chances: " << chances.size() << std::endl;
  std::cout << "Primary Element" << Helpers::elementToString(primary) << std::endl;
  std::cout << "Secondary Element" << Helpers::elementToString(secondary) << std::endl;
  std::cout << "Sound effect set?" << !(sound_effect == nullptr) << std::endl;
  std::cout << "[void]Action scope: " << std::endl;
  std::cout << "Thumb Set?" << !(thumbnail == nullptr) << std::endl;
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

bool Skill::removeAction(const uint8_t &index)
{
  if ((effects.size() == chances.size()) && index < effects.size())
  {
    effects.erase(effects.begin() + index);
    chances.erase(chances.begin() + index);

    return true;
  }

  return false;
}

Sprite* Skill::getAnimation()
{
  return animation;
}
 
uint8_t Skill::getCooldown()
{
  return cooldown;
}

float Skill::getChance(const uint8_t &index)
{
  if (index < effects.size())
    return chances[index];

  return 0;
}

std::vector<float> Skill::getChances()
{
  return chances;
}

std::string Skill::getDescription()
{
  return description;
}

Action* Skill::getEffect(const uint8_t &index)
{
  if (index < effects.size())
    return effects[index];

  return nullptr;
}

std::vector<Action*> Skill::getEffects()
{
  return effects;
}

bool Skill::getFlag(SkillFlags test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

int Skill::getID()
{
  return id;
}

std::string Skill::getName()
{
  return name;
}

Element Skill::getPrimary()
{
  return primary;
}

Element Skill::getSecondary()
{
  return secondary;
}

Sound* Skill::getSoundEffect()
{
  return sound_effect;
}

ActionScope Skill::getScope()
{
  return scope;
}

Frame* Skill::getThumbnail()
{
  return thumbnail;
}

std::string Skill::getMessage()
{
  return message;
}

uint8_t Skill::getValue()
{
  return value;
}

bool Skill::setAnimation(Sprite* const new_animation)
{
  if (new_animation != nullptr)
  {
    animation = new_animation;

    return true;
  }

  animation = nullptr;

  return false;
}

bool Skill::setCooldown(const uint8_t &new_value)
{
  if (new_value < kMAX_COOLDOWN)
  {
    cooldown = new_value;

    return true;
  }

  return false;
}

bool Skill::setDescription(const std::string &new_description)
{
  if (new_description.size() < kMAX_DESC_LENGTH)
  {
    description = new_description;

    return true;
  }

  return false;
}

void Skill::setFlag(const SkillFlags &flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= flag);
}

bool Skill::setName(const std::string &new_name)
{
  if (new_name.size() < kMAX_NAME_LENGTH)
  {
    name = new_name;
    return true;
  }

  return false;
}

bool Skill::setID(const unsigned int &new_id)
{
  if(static_cast<int>(new_id) == kUNSET_ID || static_cast<int>(id) != kUNSET_ID)
    return false;

  id = new_id;

  return true;
}

bool Skill::setSoundEffect(Sound* const new_sound_effect)
{
  if (new_sound_effect != nullptr)
  {
    sound_effect = new_sound_effect;

    return true;
  }
  
  return false;
}

void Skill::setScope(const ActionScope &new_scope)
{
  scope = new_scope;
}

void Skill::setThumbnail(Frame* const new_thumbnail)
{
  thumbnail = new_thumbnail;
}

bool Skill::setMessage(const std::string &new_message)
{
  if (new_message.size() < kMAX_MESG_LENGTH)
  {
    message = new_message;
    
    return true;
  }

  return false;
}

bool Skill::setValue(const uint8_t &new_value)
{
  if (new_value < kMAX_VALUE)
  {
    value = new_value;

    return true;
  }

  return false;
}

Skill& Skill::operator=(const Skill &source)
{
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}