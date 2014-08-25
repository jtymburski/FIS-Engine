/*******************************************************************************
 * Class Name: Item [Implementation]
 * Date Created: December 9th, 2013
 * Inheritance: none
 * Description: An Item is an object contained within the inventory of a party. 
 *              An Item may be a key item which is important for the progression
 *              through the game, some sort of material or component, an item
 *              that has a use in Battle or Menu (does some skill, etc.), or
 *              one of the Item sub-classes: Equipment or Bubby.
 *
 * Notes
 * -----
 *
 * [1]:
 *
 * See .h file for TODOs
 ******************************************************************************/
#include "Game/Player/Item.h"

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/

/* Static ID Counter */
int32_t Item::id{0};

/* Static Class Constants */
const uint32_t Item::kMAX_DURABILITY{100000};
const double   Item::kMAX_MASS{5000};
const uint32_t Item::kMAX_VALUE{100000000};
const uint32_t Item::kMIN_DURABILITY{0};
const double   Item::kMIN_MASS{-1000};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs an Item (technically a Bubby) given a Flavour object.
 *
 * Inputs: Flavour* - pointer to a flavour object for the item's likeness
 */
Item::Item(Flavour* const source)
    : game_id{source->game_id}
    , my_id{++id}
    , base_item{nullptr}
    , thumbnail{nullptr}
{
  setupClass();

  buff_set = source->base_stats;
  mass = source->base_mass;
  value = source->base_value;
  description = source->description;
  name = source->name;
  prefix = StringDb::kDEFAULT_BUBBY_PREFIX;
  occasion = ActionOccasion::NONE;

  this->setFlag(ItemFlags::BUBBY, true);
}

/*
 * Description: Constructs an item given an Item pointer to be copied from
 *
 * Inputs: Item* - pointer to an item object to be copied
 */
Item::Item(Item* const source)
    : game_id{source->game_id}
    , my_id{++id}
    , base_item{source}
{
  setupClass();
}

/*
 * Description: Base Item construction - constructs an Item with a game_id, 
 *              name, value, icon and mass.
 *
 * Inputs: game_id - the game ID (base ID of the item)
 *         name - the name to be assigned to the Item
 *         value - value (currency) for the Item
 *         thumbnail - icon for the Item
 *         mass - base mass for the Item
 */
Item::Item(const int32_t &game_id, const std::string &name, 
    const uint32_t &value, Frame* thumbnail, const double &mass, 
    const uint32_t &dura)
      : game_id{game_id}
      , my_id{++id}
      , base_item{nullptr}
      , max_durability{dura}
      , mass{mass}
      , name{name}
      , thumbnail{thumbnail}
      , value{value}
{
  setupClass();
}

/*
 * Description: Key (Also Base Item) constructor - constructs a Key item
 *
 * Inputs: game_id - the game ID (base ID of the item)
 *         name - the name to be assigned to the Item
 *         thumbnail - icon for the Item
 */
Item::Item(const int32_t &game_id, const std::string &name, Frame* thumbnail)
    : game_id{game_id}
    , my_id{++id}
    , base_item{nullptr}
    , max_durability{0}
    , mass{0.0}
    , name{name}
    , thumbnail{thumbnail}
    , value{0}
{
  setupClass();
  setFlag(ItemFlags::KEY_ITEM, true);
}

/*
 * Descripption: Annihilates an Item object
 *
 */
Item::~Item()
{
  unsetAll();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

 /*
 * Description: Sets the Item class to default values. Private function that
 *              should NEVER be called outside of construction or assignment
 *              operations.
 *
 * Inputs: none
 * Output: none
 */
void Item::setupClass()
{
  /* Clear some variables that are the same between both */
  value_modifier = 0;
  
  /* Setup the class as a standalone Item */
  if (base_item == nullptr)
  {
    buff_set = AttributeSet();
    
    brief_description = StringDb::kDEFAULT_ITEM_DESC;
    description = StringDb::kDEFAULT_ITEM_DESC;
    composition = static_cast<Material>(0);
    durability = max_durability;
    flags = static_cast<ItemFlags>(0);
    prefix = StringDb::kDEFAULT_ITEM_PREFIX;
    occasion = ActionOccasion::NONE;
    
    using_skill     = nullptr;
    using_animation = nullptr;
    using_message   = "";
    using_sound     = nullptr;  
  }

  /* Setup the class as a copy of the Base Item */
  else
  {
    buff_set = base_item->buff_set;
    brief_description = base_item->brief_description;
    description = base_item->description;
    max_durability = base_item->max_durability;
    durability = max_durability;
    composition = base_item->composition;
    flags = base_item->flags;
    mass = base_item->mass;
    name = base_item->name;
    prefix = base_item->prefix;
    occasion = base_item->occasion;
    thumbnail = base_item->thumbnail;
    using_skill = base_item->using_skill;
    using_animation = base_item->using_animation;
    using_message = base_item->using_message;
    using_sound = base_item->using_sound;
    value = base_item->value;
    value_modifier = base_item->value_modifier;
  }
}

 /*
 * Description: Unsets all the values of a given Item object
 *              to destrucable ready-values.
 *
 * Inputs: none
 * Output: none
 */
void Item::unsetAll()
{
  /* Unset the thumbnail, only if it's a base */
  if (base_item == nullptr && thumbnail != nullptr)
    delete thumbnail;
  
  /* Clear variables in the class */
  id = 0;
  base_item = nullptr;
  buff_set = AttributeSet();
  brief_description = "";
  description = "";
  durability = kMIN_DURABILITY;
  max_durability = kMIN_DURABILITY;
  composition = static_cast<Material>(0);
  flags = static_cast<ItemFlags>(0);
  mass = 0;
  name = "";
  prefix = "";
  occasion = ActionOccasion::NONE;
  thumbnail = nullptr;
  using_skill = nullptr;
  using_animation = nullptr;
  using_message = "";
  using_sound = nullptr;
  value = 0;
}

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Item::print()
{
  std::cout << "--- Item ---\n";
  printInfo();
  printFlags();
  std::cout << "------------\n";
}

 /*
 * Description: Virtual method which returns the current stats of the Item.
 *
 * Inputs: none
 * Output: AttributeSet - the stats of the 
 */
AttributeSet Item::getStats()
{
  return buff_set;
}

 /*
 * Description: Virtual method which returns the mass of the Item.
 *
 * Inputs: none
 * Output: double - the mass of the Item
 */
double Item::getMass()
{
  return mass;
}

 /*
 * Description: Virtual method which returns the value of the Item.
 *
 * Inputs: none
 * Output: uint32_t - the value of the Item
 */
uint32_t Item::getValue()
{
  int32_t calc_value = value + value_modifier;
  
  /* Check bounds */
  if(calc_value < 0)
    return 0;
  else if(static_cast<uint32_t>(calc_value) > kMAX_VALUE)
    return kMAX_VALUE;
  return calc_value;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: 
 *
 * Inputs:
 * Output:
 */
bool Item::isBaseItem()
{
  return (base_item == nullptr);
}

/*
 * Description: Prints out the state of the both flags (regular and material)
 *              of the Item object.
 *
 * Inputs: none
 * Output: none
 */
void Item::printFlags()
{
  /* Print ItemState flags */
  std::cout << "CONSUMED: " << getFlag(ItemFlags::CONSUMED) << "\n";
  std::cout << "OFFENSIVE: " << getFlag(ItemFlags::OFFENSIVE) << "\n";
  std::cout << "DEFENSIVE: " << getFlag(ItemFlags::DEFENSIVE) << "\n";
  std::cout << "EQUIPMENT: " << getFlag(ItemFlags::EQUIPMENT) << "\n";
  std::cout << "BUBBY: " << getFlag(ItemFlags::BUBBY) << "\n";
  std::cout << "KEY_ITEM: " << getFlag(ItemFlags::KEY_ITEM) << "\n";
  std::cout << "MATERIAL: " << getFlag(ItemFlags::MATERIAL) << "\n";
  std::cout << "GENE_COMP: " << getFlag(ItemFlags::GENE_COMP) << "\n";
  std::cout << "STAT_ALTERING: " << getFlag(ItemFlags::STAT_ALTERING) << "\n";
  std::cout << "SKILL_LEARNING: " << getFlag(ItemFlags::SKILL_LEARNING) << "\n";
  std::cout << "HEALING_ITEM: " << getFlag(ItemFlags::HEALING_ITEM) << "\n";
  std::cout << "RELIEVING_ITEM: " << getFlag(ItemFlags::RELIEVING_ITEM) << "\n";

  /* Print Material (composition) flags -- alloys etc. */
  std::cout << "WOODEN: "    << getMaterial(Material::WOODEN)        << "\n";
  std::cout << "OFFENSIVE: " << getMaterial(Material::STEEL)         << "\n";
  std::cout << "BRASS: "     << getMaterial(Material::BRASS)         << "\n";
  std::cout << "TITANIUM: "  << getMaterial(Material::TITANIUM)      << "\n";
  std::cout << "GRAPHENE: "  << getMaterial(Material::GRAPHENE)      << "\n";

  /* Material alloys -- see StringDb.h for the actual name */
  std::cout << "PHYSICAL: "     << getMaterial(Material::PHYSICAL)     << "\n";
  std::cout << "NON_PHYSICAL: " << getMaterial(Material::NON_PHYSICAL) << "\n";
  std::cout << "FIRE: "         << getMaterial(Material::FIRE)         << "\n";
  std::cout << "FOREST: "       << getMaterial(Material::FOREST)       << "\n";
  std::cout << "ICE: "          << getMaterial(Material::ICE)          << "\n";
  std::cout << "ELECTRIC: "     << getMaterial(Material::ELECTRIC)     << "\n";
  std::cout << "DIGITAL: "      << getMaterial(Material::DIGITAL)      << "\n";
  std::cout << "NIHIL: "        << getMaterial(Material::NIHIL)        << "\n";
}

 /*
 * Description: Prints out the information of the Item object [std::cout]
 *
 * Inputs: basic - boolean whether to display a simple format or not
 * Output: none
 */
void Item::printInfo(const bool &basic)
{
  if (basic)
  {
    std::cout << "ID: " << my_id << "Game ID: " << game_id << " N: " << name 
              << " M: " << mass << " V: " << value << "\n";
  }
  else
  {
    std::cout << "ID: " << my_id << "\n";
    std::cout << "Base Item? " << isBaseItem() << "\n";
    std::cout << "Brief Description: " << brief_description << "\n";
    std::cout << "Description: " << description << "\n";
    std::cout << "Game ID: " << game_id << "\n";
    std::cout << "Mass: " << mass << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Prefix: " << prefix << "\n";
    std::cout << "[void]Occasion: " << "\n";
    std::cout << "Thumbnail? " << !(thumbnail == nullptr) << "\n";
    std::cout << "Use Skill? " << !(using_skill == nullptr) << "\n";
    std::cout << "Use Animation? " << !(using_animation == nullptr) << "\n";
    std::cout << "Use Message: " << using_message << "\n";
    std::cout << "Use Sound? " << !(using_sound == nullptr) << "\n";
    std::cout << "Value: " << value << "\n";

    std::cout << "Buff_Set: " << "\n"; 
    buff_set.print(basic);
    std::cout << "\n";
  }
}

/*
 * Description: Returns the string brief description of the Item
 *
 * Inputs: none
 * Output: std::string - the brief description of the Item
 */
std::string Item::getBriefDescription()
{
  return brief_description;
}

/*
 * Description: Returns the string description of the Item
 *
 * Inputs: none
 * Output: std::string - description of the Item
 */
std::string Item::getDescription()
{
  return description;
}

/*
 * Description: Returns the current durability value of the Item
 *
 * Inputs: none
 * Output: uint32_t - the durability value
 */
uint32_t Item::getDurability()
{
  return durability;
}

/*
 * Description: Returns the game id (base item ID) of the current Item
 *
 * Inputs: none
 * Output: int32_t - the game_id of the Item
 */
int32_t Item::getGameID()
{
  return game_id;
}

/*
 * Description: Returns the actual unique ID (my_id) of the current Item.
 * 
 * Inputs: none
 * Output: int32_t - the actual unique ID of the current item
 */
int32_t Item::getID()
{
  return my_id;
}

/*
 * Description: Evaluates and returns the value of a given ItemFlags flag
 *
 * Inputs: test_flag - ItemFlags flag to check the value for
 * Output: bool - value of the given ItemFlags flag
 */
bool Item::getFlag(ItemFlags test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Evaluates and returns the value of a given Material flag.
 *
 * Inputs: test_flag - Material composition flag to check the value for
 * Output: bool - value of the given material composition flag
 */
bool Item::getMaterial(Material test_flag)
{
  return static_cast<bool>((composition & test_flag) == test_flag);
}

/*
 * Description: Returns the maximum durability value
 *
 * Inputs: none
 * Output: uint32_t - the maximum value of durability
 */
uint32_t Item::getMaxDurability()
{
  return max_durability;
}

/*
 * Description: Returns the string name of the Item (ex. Sword of Space)
 *
 * Inputs: none
 * Output: std::string - the string name of the Item
 */
std::string Item::getName()
{
  return name;
}

/*
 * Description: Returns the string prefix of the Item (ex. "bottles")
 *
 * Inputs: none
 * Output: std::string - prefix of the Item
 */
std::string Item::getPrefix()
{
  return prefix;
}

/*
 * Description: Returns the occasion (conditions of use) of the Item (B vs. M)
 *
 * Inputs: none
 * Output: ActionOccasion - enumerated use conditions of the Item
 */
ActionOccasion Item::getOccasion()
{
  return occasion;
}

 /*
 * Description: Returns a ptr to the thumbnail of the Item.
 *
 * Inputs: none
 * Output: Frame* - ptr to the thumbnail of the Item
 */
Frame* Item::getThumb()
{
  return thumbnail;
}

 /*
 * Description: Returns a ptr to the using animation of the Item
 *
 * Inputs: none
 * Output: Sprite* - ptr to the animation of the use effect of the Item
 */
Sprite* Item::getUseAnimation()
{
  return using_animation;
}

 /*
 * Description: Returns the using message of the Item
 *
 * Inputs: none
 * Output: std::string - the message the Item displays upon use
 */
std::string Item::getUseMessage()
{
  return using_message;
}

 /*
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
Skill* Item::getUseSkill()
{
  return using_skill;
}

 /*
 * Description: Returns a ptr to the sound effect of the Item.
 *
 * Inputs: none
 * Output: Sound* - ptr to the sound effect of the Item
 */
Sound* Item::getUseSound()
{
  return using_sound;
}

 /*
 * Description: Assigns a new AttributeSet for the Item.
 *
 * Inputs: new_buff_set - new AttributeSet to assign to the Item
 * Output: none
 */
void Item::setBuffSet(const AttributeSet &new_buff_set)
{
  buff_set = new_buff_set;
}

 /*
 * Description: Attempts to assign a new brief description to the Item and
 *              returns the outcome of the assignment.
 *
 * Inputs: new_brief_description - new string to assign br. desc for
 * Output: bool - outcome of the assignment
 */
bool Item::setBriefDescription(const std::string &new_brief_description)
{
  if (new_brief_description.size() <= StringDb::kMAX_BRIEF_DESC)
  {
    brief_description = new_brief_description;

    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new string description to the Item and
 *              returns the outcome of the assignment
 *  
 * Inputs: new_description - descripion of the Item
 * Output: bool - outcome of the description assignment
 */
bool Item::setDescription(const std::string &new_description)
{
  if (new_description.size() <= StringDb::kMAX_DESC)
  {
    description = new_description;
    
    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new current durability value to the Item,
 *              but will fail if the new durability value is beyond the Item's
 *              maximum durability.
 *  
 * Inputs: new_durability - new durability value for the Item
 * Output: bool- outcome of the description assignment
 */
bool Item::setDurability(const uint32_t &new_durability)
{
  if (new_durability <= max_durability)
  {
    durability = new_durability;
    return true;
  }
  
  return false;
}

 /*
 * Description: Assigns a given ItemFlags flag a given boolean value.
 *
 * Inputs: flag - ItemFlags flag to be set
 *         set_value - value to set the given flag to
 * Output: none
 */
void Item::setFlag(ItemFlags flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

 /*
 * Description: Assigns a given Material composition a given value.
 *
 * Inputs: flag - Material composition flag to be assigned a value
 *         set_value - value to assign to the flag
 * Output: none
 */
void Item::setMaterial(Material flag, const bool &set_value)
{
  (set_value) ? (composition |= flag) : (composition &= ~flag);
}

 /*
 * Description: Attempts to assign a new string name to the Item
 *
 * Inputs: new_name - new string name to assign to the Item
 * Output: bool - the outcome of the name assignment
 */
bool Item::setName(const std::string &new_name)
{
  if (new_name.size() <= StringDb::kMAX_NAME)
  {
    name = new_name;

    return true;
  }

  return false;
}

 /*
 * Description: Attempts to assign a new prefix to the Item and returns the 
 *              outcome of the assignment
 *
 * Inputs: new_prefix - string prefix for the Item
 * Output: bool - the outcome of the assignment
 */
bool Item::setPrefix(const std::string &new_prefix)
{
  if (new_prefix.size() <= StringDb::kMAX_PREFIX)
  {
    prefix = new_prefix;

    return true;
  }

  return false;
}

 /*
 * Description: Assigns a new occasion to the Item.
 *
 * Inputs: new_occasion - action occasion (conditions of use) of the Item
 * Output: none
 */
void Item::setOccasion(const ActionOccasion &new_occasion)
{
  occasion = new_occasion;
}

 /*
 * Description: Attempts to assign a new mass for the Item and returns the
 *              outcome of the assignment.
 *
 * Inputs: new_mass - the new mass to be assigned
 * Output: bool - outcome of the assignment
 */
bool Item::setMass(const double &new_mass)
{
  if (new_mass >= kMIN_MASS && new_mass <= kMAX_MASS)
  {
    mass = new_mass;
    
    return true;
  }
  
  return false;
}

 /*
 * Description: Attempts to assign a new thumbnail ptr (icon image) to the Item
 *              and return true if the new thumbnail is not null.
 *
 * Inputs: new_thumbnail - ptr to a new thumb frame for the Item
 * Output: bool - true if the new Frame ptr is not null
 */
bool Item::setThumbnail(Frame* new_thumbnail)
{
  thumbnail = new_thumbnail;

  return (new_thumbnail != nullptr);
}

 /*
 * Description: Attempts to assign a new usage animation to the Item and returns
 *              true if the new animation is not null.
 *
 * Inputs: new_animation - new animation to be assigned
 * Output: bool - true if the new animation is not null
 */
bool Item::setUseAnimation(Sprite* new_animation)
{
  using_animation = new_animation;

  return (new_animation != nullptr);
}

 /*
 * Description: Attempts to assign a new usage message to the Item and 
 *              returns the outcome of the assignment.
 *
 * Inputs: new_message - new message to attempted to be assigned
 * Output: bool - true if the new message was assigned
 */
bool Item::setUseMessage(const std::string &new_message)
{
  if (new_message.size() <= StringDb::kMAX_USE_MSG)
  {
    using_message = new_message;

    return true;
  }

  return false;
}

 /*
 * Description: Attempts to assign a new skill use effect to the Item and 
 *              returns true if the new skill effect is not null.
 *
 * Inputs: new_skill - ptr to the new usage skill for the Item
 * Output: bool - true if the new usage skill is not null
 */
bool Item::setUseSkill(Skill* new_skill)
{
  using_skill = new_skill;

  return (new_skill != nullptr);
}

 /*
 * Description: Attempts to assign a new sound effect to the Item and returns
 *              true if the new assignment is not null.
 *
 * Inputs: new_sound - ptr to a new sound effect for the Item
 * Output: bool = true if the new soun ptr is not null
 */
bool Item::setUseSound(Sound* new_sound)
{
  using_sound = new_sound;

  return (new_sound != nullptr);
}

/*
 * Description: Attempts to assign a new value to the value of the Item
 *              and returns the outcome of the assignment.
 *
 * Inputs: new_value - new value to assign for the Item
 * Output: bool - true if the value was assigned
 */
bool Item::setValue(const uint32_t &new_value)
{
  if (new_value <= kMAX_VALUE)
  {
    value = new_value;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a modifier value which is computed against the value
 *              when getValue() is called. 
 *              and returns the outcome of the assignment.
 *
 * Inputs: new_value - new value to assign for the Item
 * Output: bool - true if the value was assigned
 */
void Item::setValueModifier(const int32_t &new_value)
{
  value_modifier = new_value;
}
