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
const size_t Skill::kMAX_ACTIONS       =   10;
const uint   Skill::kMAX_COOLDOWN      =   10;
const uint   Skill::kMAX_COST          = 5000;
const size_t Skill::kMAX_MESG_LENGTH   =   70;
const size_t Skill::kMAX_NAME_LENGTH   =   60;
const size_t Skill::kMAX_DESC_LENGTH   =  500;
const uint   Skill::kMAX_VALUE         =   10;

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
Skill::Skill(const std::string &name, const ActionScope &skill_scope,
	         const Action* effects, const float &chance)
{
  classSetup();
  setName(name);
  setScope(skill_scope);

  if (addAction(effect, chance))
    flags |= SkillFlags::VALID;

  flagSetup();
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

  if (addActions(effects, chances))
    flags |= SkillFlags::VALID;

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
  primary = EnumDb::NONE;
  secondary = EnumDb::NONE;
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
void Skill::copySelf(const Skill &source)
{
  animation    = source.animation;
  cooldown     = source.cooldown;
  description  = source.description;
  flags        = source.flags;
  cost         = source.cost;
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
  //TODO: Offensive/Defensive/Neutral categories?

  for (auto it = effects.begin(); it != effects.end(); ++it)
  {
    ActionFlags &action_flags = (*it)->getFlags();

    if ((action_flags & ActionFlags::ALTER) == ActionFlags::ALTER)
      flags |= SkillFlags::ALTERING;

    if ((action_flags & ActionFlags::INFLICT) == ActionFlags::INFLICT)
      flags |= SkillFlags::INFLICTING;

    if ((action_flags & ActionFlags::RELIEVE) == ActionFlags::RELIEVE)
      flags |= SkillFlags::RELIEVING;

    if ((action_flags & ActionFlags::REVIVE) == ActionFlags::REVIVE)
      flags |= SkillFlags::REVIVING;

    if ((action_flags & ActionFlags:ASSIGN) == ActionFlags::ASSIGN)
      flags |= SkillFlags::ASSIGNING;
  }
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

std::vector<float> Skill::getChances()
{
  return chances;
}

std::string Skill::getDescription()
{
  return description;
}

Action* Skill::getEffect(const uint &index)
{
  if (index < effects.size())
    return effects[index];

  return nullptr;
}

std::vector<Action*> Skill::getEffects()
{
  return effects;
}

SkillFlags Skill::getFlags()
{
  return flags;
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

uint Skill::getValue()
{
  return value;
}

bool Skill::setAnimation(const Sprite* new_animation)
{
  if (new_animation != nullptr)
  {
    animation = new_animation;

    return true;
  }

  animation = nullptr;

  return false;
}

bool Skill::setCooldown(const uint &new_value)
{
  if (new_value < kMAX_COOLDOWN)
  {
    cooldown = new_value;

    return true;
  }

  return false;
}

void Skill::setChance(const float &new_chance)
{
  chance = new_chance;

  if (chance > 1)
    chance = 1;
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

//TODO
bool Skill::setSoundEffect(const Sound* new_sound_effect)
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

void Skill::setThumbnail(const Frame* new_thumbnail)
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

bool Skill::setValue(const uint &new_value)
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