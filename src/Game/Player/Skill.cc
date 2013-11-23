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

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
const size_t Skill::kMAX_ACTIONS =   10;
const uint Skill::kMAX_COOLDOWN  =   10;
const uint Skill::kMAX_COST      = 5000;
const size_t kMAX_NAME_LENGTH    =   70;
const size_t kMAX_DESC_LENGTH    =  500;
const uint Skill::kMAX_VALUE     =   10;

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
Skill::Skill(const std::string &name, const ActionScope &skill_scope, 
  	         const std::vector<Action*> &effects, 
  	         const std::vector<float> &chances)
{
  classSetup();
  setName(name);
  setScope(skill_scope);
  addActions(effects, chances);
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
  description = "":
  flags = static_cast<SkillFlags>(0));
  cost = 0;
  name = "";
  sound_effect = nullptr;
  scope = ActionScope::NONE;
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
void Skill::flagSetup()
{

}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Skill::addAction(const Action* new_action, const float &new_chance)
{
  if ((effects.size() && chances.size()) < kMAX_ACTIONS)
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
  valid = true;

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
  valid = true;

  for (auto it = effects.begin(); it != effects.end(); ++it)
    if (((*it)->getFlags() & Action::VALID) != Action::VALID)
      valid = false;

  valid  &= (effects.size() == chances.size());

  return valid;
}

bool Skill::removeAction(const uint &index)
{
  if ((effects.size() == chances.size()) && index < effects.size())
  {
    effects.removeAt(index);
    chances.removeAt(index);

    return true;
  }

  return false;
}

Sprite* Skill::getAnimation()
{
  return animation;
}
 
uint Skill::getCooldown()
{
  return cooldown;
}

float Skill::getChance(const uint &index)
{
  if (index < effects.size())
    return chances[index];

  return 0;
}

std::vector<float> getChances()
{
  return chances;
}

std::string getDescription()
{
  return description;
}

Action* getEffect(const uint &index)
{
  if (index < effects.size())
    return effects[index];

  return nullptr;
}

std::vector<Action*> getEffects()
{
  return effects;
}

SkillFlags getFlags()
{
  return flags;
}

std::string getName()
{
  return name;
}

Sound* getSoundEffect()
{
  return sound_effect;
}

ActionScope getScope()
{
  return scope;
}

Frame* getThumbnail()
{
  return thumbnail;
}

std::string getMessage()
{
  return message;
}

uint getValue()
{
  return value;
}

bool setAnimation(const Sprite* new_animation)
{
  if (new_animation )
}

bool setCooldown(const uint &new_value)
{

}

void setChance(const float &new_chance)
{

}

bool setDescription(const std::string &new_description)
{

}

bool setName(const std::string &new_name)
{

}

void setSoundEffect(const Sound* new_sound_effect)
{

}

void setScope(const ActionScope &new_scope)
{

}

void setThumbnail(const Frame* new_thumbnail)
{

}

bool setMessage(const std::string &message)
{

}

bool setValue(const uint &new_value)
{

}