/*******************************************************************************
* Class Name: x [Implementation]
* Date Created: 
* Inheritance: 
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Item.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const size_t   Item::kMAX_BRIEF_DESC =        70;
const size_t   Item::kMAX_DESC       =       500;
const size_t   Item::kMAX_USE_MSG    =        40;
const double   Item::kMAX_MASS       =      5000;
const double   Item::kMIN_MASS       =     -1000;
const uint32_t Item::kMAX_VALUE      = 100000000;

int Item::id = 0;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Item::Item()
{
  classSetup();
}

Item::Item(const Item &source)
{
  classSetup();
  copySelf(source);
}

Item::Item(const std::string &name, const uint32_t &value, Frame* thumbnail,
	       const double &mass)
{
  classSetup();
  setName(name);
  setValue(value);
  setThumbnail(thumbnail);
  setMass(mass);
}

/*
 * Description: Annihilates an Item object
 */
Item::~Item() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Item::classSetup()
{
  setID(++id);

  base_item = nullptr;
  buff_set = AttributeSet();
  brief_description = StringDB::kDEFAULT_ITEM_DESC;
  description = StringDB::kDEFAULT_ITEM_DESC;
  composition = static_cast<Material>(0);
  flags = static_cast<ItemFlags>(0);
  mass = 0;
  name = StringDB::kDEFAULT_ITEM_NAME;
  prefix = StringDB::kDEFAULT_ITEM_PREFIX;
  occasion = ActionOccasion::NONE;
  thumbnail = nullptr;
  using_skill = nullptr;
  using_animation = nullptr;
  using_message = StringDB::kDEFAULT_ITEM_USE_MSG;
  using_sound = nullptr;
  value = 0;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Item::copySelf(const Item &source)
{  
  //TODO [12-11-13]
  name = source.name;
}

void Item::setID(const uint32_t &value)
{
  my_id = value;
}

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Item::print()
{

}

AttributeSet Item::getBuffSet()
{
  return buff_set;
}

double Item::getMass()
{
  return mass;
}

uint32_t Item::getValue()
{
  return value;
}

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

void Item::printFlags()
{
  std::cout << "CONSUMED: " << getFlag(ItemFlags::CONSUMED) << std::endl;
  std::cout << "OFFENSIVE: " << getFlag(ItemFlags::OFFENSIVE) << std::endl;
  std::cout << "DEFENSIVE: " << getFlag(ItemFlags::DEFENSIVE) << std::endl;
  std::cout << "EQUIPMENT: " << getFlag(ItemFlags::EQUIPMENT) << std::endl;
  std::cout << "BUBBY: " << getFlag(ItemFlags::BUBBY) << std::endl;
  std::cout << "KEY_ITEM: " << getFlag(ItemFlags::KEY_ITEM) << std::endl;
  std::cout << "MATERIAL: " << getFlag(ItemFlags::MATERIAL) << std::endl;
  std::cout << "GENE_COMP: " << getFlag(ItemFlags::GENE_COMP) << std::endl;
  std::cout << "STAT_ALTERING: " << getFlag(ItemFlags::STAT_ALTERING) 
            << std::endl;
  std::cout << "SKILL_LEARNING: " << getFlag(ItemFlags::SKILL_LEARNING) 
            << std::endl;
  std::cout << "HEALING_ITEM: " << getFlag(ItemFlags::HEALING_ITEM) 
            << std::endl;
  std::cout << "RELIEVING_ITEM: " << getFlag(ItemFlags::RELIEVING_ITEM) 
            << std::endl;
  std::cout << "SET_BASE_ITEM: " << getFlag(ItemFlags::SET_BASE_ITEM) 
            << std::endl;
  std::cout << "METALLIC: " << getMaterial(Material::METALLIC) << std::endl;
  std::cout << "INSULATED: " << getMaterial(Material::INSULATED) << std::endl;
  std::cout << "ANTIMATTER: " << getMaterial(Material::ANTIMATTER) << std::endl;
  std::cout << "DARKMATTER: " << getMaterial(Material::DARKMATTER) << std::endl;
  std::cout << "ENERGY: " << getMaterial(Material::ENERGY) << std::endl;
  std::cout << "DARK_ENERGY: " << getMaterial(Material::DARK_ENERGY) 
            << std::endl;
  std::cout << "ICE: " << getMaterial(Material::ICE) << std::endl;
  std::cout << "PLASMA: " << getMaterial(Material::PLASMA) << std::endl;
  std::cout << "CHARGED: " << getMaterial(Material::CHARGED) << std::endl;
}

void Item::printInfo(const bool &basic)
{
  if (basic)
  {
    std::cout << my_id << " N: " << name << " M: " << mass << " V: " << value 
              << std::endl;
  }
  else
  {
    std::cout << "ID: " << my_id << std::endl;
    std::cout << "Base Item? " << !(base_item == nullptr) << std::endl;
    std::cout << "Buff_Set: " << std::endl; 
    buff_set.print(basic);
    std::cout << std::endl;
    std::cout << "Brief Description: " << brief_description << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Mass: " << mass << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Prefix: " << prefix << std::endl;
    std::cout << "[void]Occasion: " << std::endl;
    std::cout << "Thumbnail? " << !(thumbnail == nullptr) << std::endl;
    std::cout << "Use Skill? " << !(using_skill == nullptr) << std::endl;
    std::cout << "Use Animation? " << !(using_animation == nullptr) << std::endl;
    std::cout << "Use Message: " << using_message << std::endl;
    std::cout << "Use Sound? " << !(using_sound == nullptr) << std::endl;
    std::cout << "Value: " << value << std::endl;
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Item::setBase(Item* item_base)
{
  if (getFlag(ItemFlags::SET_BASE_ITEM))
    return false;

  base_item = item_base;
  setFlag(ItemFlags::SET_BASE_ITEM, true);

  return true;
}

void Item::setBuffSet(const AttributeSet &new_buff_set)
{
  buff_set = new_buff_set;
  setBase(nullptr);
}

bool Item::setBriefDescription(const std::string &new_brief_description)
{
  if (new_brief_description.size() <= kMAX_BRIEF_DESC)
  {
    setBase(nullptr);
    brief_description = new_brief_description;
    

    return true;
  }

  return false;
}

bool Item::setDescription(const std::string &new_description)
{
  if (new_description.size() <= kMAX_DESC)
  {
    setBase(nullptr);
    description = new_description;
    

    return true;
  }

  return false;
}

void Item::setFlag(ItemFlags flag, const bool &set_value)
{
  if (getFlag(ItemFlags::SET_BASE_ITEM))
    setBase(nullptr);

  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

void Item::setMaterial(Material flag, const bool &set_value)
{
  setBase(nullptr);
  (set_value) ? (composition |= flag) : (composition &= ~flag);
}

void Item::setName(const std::string &new_name)
{
  setBase(nullptr);
  name = new_name;
}

void Item::setPrefix(const std::string &new_prefix)
{
  setBase(nullptr);
  prefix = new_prefix;
}

void Item::setOccasion(const ActionOccasion &new_occasion)
{
  setBase(nullptr);
  occasion = new_occasion;
}

bool Item::setMass(const double &new_mass)
{
  if (new_mass >= kMIN_MASS && new_mass <= kMAX_MASS)
  {
    setBase(nullptr);
    mass = new_mass;
    
    return true;
  }
  
  return false;
}

bool Item::setThumbnail(Frame* new_thumbnail)
{
  setBase(nullptr);
  thumbnail = new_thumbnail;

  return (new_thumbnail != nullptr);
}

bool Item::setUseAnimation(Sprite* new_animation)
{
  setBase(nullptr);
  using_animation = new_animation;

  return (new_animation != nullptr);
}

bool Item::setUseMessage(const std::string &new_message)
{
  if (new_message.size() <= kMAX_USE_MSG)
  {
    using_message = new_message;

    return true;
  }

  return false;
}

bool Item::setUseSkill(Skill* new_skill)
{
  using_skill = new_skill;

  return (new_skill != nullptr);
}

bool Item::setUseSound(Sound* new_sound)
{
  using_sound = new_sound;

  return (new_sound != nullptr);
}

bool Item::setValue(const uint32_t &new_value)
{
  if (new_value <= kMAX_VALUE)
  {
    value = new_value;

    return true;
  }

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Item::isBaseItem()
{
  return (base_item != nullptr);
}

std::string Item::getBriefDescription()
{
  return brief_description;
}

std::string Item::getDescription()
{
  return description;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Item::getFlag(ItemFlags test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Item::getMaterial(Material test_flag)
{
  return static_cast<bool>((composition & test_flag) == test_flag);
}

std::string Item::getName()
{
  return name;
}

std::string Item::getPrefix()
{
  return prefix;
}

ActionOccasion Item::getOccasion()
{
  return occasion;
}

Frame* Item::getThumb()
{
  return thumbnail;
}

Sprite* Item::getUseAnimation()
{
  return using_animation;
}

std::string Item::getUseMessage()
{
  return using_message;
}

Sound* Item::getUseSound()
{
  return using_sound;
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

 /*
 * Description:
 *
 * Inputs:
 * Output:
 */
 Item& Item::operator=(const Item &source)
 {
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
 }