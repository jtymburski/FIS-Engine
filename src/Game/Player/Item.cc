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
 * CONSTANTS - See implementation for details
 *============================================================================*/

const double   Item::kMAX_MASS       =      5000;
const double   Item::kMIN_MASS       =     -1000;
const uint32_t Item::kMAX_VALUE      = 100000000;
const int      Item::kUNSET_ID       =        -1;

int Item::id = 0;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default constructor - constructs a default Item object.
 *
 * Inputs: none
 */
Item::Item()
  : game_id(kUNSET_ID)
  , my_id(++id)
{
  classSetup();
}

/*
 * Description: Copy constructor - copies a source Item object. Constructs
 *              the new Item object as a copy of the base item with a UNIQUE
 *              ID my_id, but equivalent game_id.
 *
 * Inputs: source - const ref. of an Item object to be copied
 */
Item::Item(const Item &source)
  : game_id(source.game_id)
  , my_id(++id) /* Assign the game ID of the base object */
{
  copySelf(source);
}

/*
 * Description: Move constructor - constructs an Item from an r.value ref.
 *
 * Inputs: source - r.value ref to an Item
 */
Item::Item(Item&& source)
  : game_id(source.game_id)
{
  unsetAll(this);
  swap(*this, source);
  setID(++id);
}

Item::Item(Flavour* const source)
  : game_id(source->getGameID())
  , my_id(++id)
{
  classSetup();
  setName(source->getName());
  setBuffSet(source->getStats());
  // setBriefDescription(source->getBriefDescription());
  setDescription(source->getDescription());
  setName(source->getName());
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
Item::Item(const uint32_t &game_id, const std::string &name, 
	       const uint32_t &value, Frame* thumbnail, const double &mass)
  : game_id(game_id)
  , my_id(++id) /* Assign the game ID */
{
  classSetup();
  setName(name);
  setValue(value);
  setThumbnail(thumbnail);
  setMass(mass);
  
  /* Assign the Base Item ptr and flag */
  //setBase(this);
}

/*
 * Description: Annihilates an Item object.
 */
Item::~Item()
{
  unsetAll(this);
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
void Item::classSetup()
{
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
 * Description: Copies a const source Item to the current object. Should NEVER
 *              be called outside of construction or assignment operators.
 *
 * Inputs: source - const ref to Item object to be copied.
 * Output: none
 */
void Item::copySelf(const Item &source)
{ 
  base_item = source.base_item;
  buff_set = source.buff_set;
  brief_description = source.brief_description;
  description = source.description;
  composition = source.composition;
  flags = source.flags;
  mass = source.mass;
  name = source.name;
  prefix = source.prefix;
  occasion = source.occasion;
  thumbnail = source.thumbnail;
  using_skill = source.using_skill;
  using_animation = source.using_animation;
  using_message = source.using_message;
  using_sound = source.using_sound;
  value = source.value;
}

/*
 * Description: Swaps the data of two Item objects using std::swap. If called
 *              during a move, the ID should be updated after calling.
 *
 * Inputs: object - lhs Item to be swapped
 *         source - rhs Item to be swapped
 * Output: none
 */
void Item::swap(Item& object, Item& source)
{
  std::swap(object.my_id, source.my_id);
  std::swap(object.base_item, source.base_item);
  std::swap(object.buff_set, source.buff_set);
  std::swap(object.brief_description, source.brief_description);
  std::swap(object.description, source.description);
  std::swap(object.composition, source.composition);
  std::swap(object.flags, source.flags);
  std::swap(object.mass, source.mass);
  std::swap(object.name, source.name);
  std::swap(object.prefix, source.prefix);
  std::swap(object.occasion, source.occasion);
  std::swap(object.thumbnail, source.thumbnail);
  std::swap(object.using_skill, source.using_skill);
  std::swap(object.using_animation, source.using_animation);
  std::swap(object.using_message, source.using_message);
  std::swap(object.using_sound, source.using_sound);
  std::swap(object.value, source.value);
}

 /*
 * Description: Unsets all the values of a given Item object
 *              to destrucable ready-values.
 *
 * Inputs: object - ptr to an Item object to be unset
 * Output:
 */
void Item::unsetAll(Item* object)
{
  object->id = 0;
  object->base_item = nullptr;
  object->buff_set = AttributeSet();
  object->brief_description = "";
  object->description = "";
  object->composition = static_cast<Material>(0);
  object->flags = static_cast<ItemFlags>(0);
  object->mass = 0;
  object->name = "";
  object->prefix = "";
  object->occasion = ActionOccasion::NONE;
  if(object->thumbnail != nullptr)
    delete object->thumbnail;
  object->thumbnail = nullptr;
  object->using_skill = nullptr;
  object->using_animation = nullptr;
  object->using_message = "";
  object->using_sound = nullptr;
  object->value = 0;
}

 /*
 * Description: Private method for assigning the my_id (not game_id) of the Item
 *              (generally should be called with ++id for incrementing the
 *               static id counter of the Item class)
 *
 * Inputs: value - the value to be assigned for the ID
 * Output: none
 */
void Item::setID(const uint32_t &value)
{
  my_id = value;
}

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*
 * Description: Virtual method for printing out the Item. Calls sub-methods
 *              for printing out the Item info and flags
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
  return value;
}

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to assign a Base item to the Item. This function
 *              can only be called ONCE per Item since once a base Item is
 *              set, the set base flag is set (which cannot be unset)
 *
 * Inputs: item_base - base item to be assigned to the item
 * Output: bool - true if base_item was unset before and is now not null
 */
bool Item::setBase(Item* item_base)
{
  if (getFlag(ItemFlags::SET_BASE_ITEM))
    return false;

  if (item_base != nullptr)
  {
    base_item = item_base;
    setFlag(ItemFlags::SET_BASE_ITEM, true);

    return true;
  }

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Evalates and returns whether the current Item is a base item.
 *              The current Item is a base item if the following conditions are
 *              met:
 * 
 *              I] A game_id [base item ID] is set to other than KUNSET_ID
 *             II] The Flag for SET_BASE_ITEM is set to true
 *            III] The base_item ptr is null
 *
 * Inputs: none
 * Output: bool - true if the current object is a base item
 */
bool Item::isBaseItem()
{
  if (getFlag(ItemFlags::SET_BASE_ITEM))
  {
    std::cout << "Checking base Item ptr" << std::endl;
    return (base_item == nullptr);
  }
  
  return true;
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
  std::cout << "SET_BASE_ITEM: " << getFlag(ItemFlags::SET_BASE_ITEM) << "\n";
  std::cout << "METALLIC: " << getMaterial(Material::METALLIC) << "\n";
  std::cout << "INSULATED: " << getMaterial(Material::INSULATED) << "\n";
  std::cout << "ANTIMATTER: " << getMaterial(Material::ANTIMATTER) << "\n";
  std::cout << "DARKMATTER: " << getMaterial(Material::DARKMATTER) << "\n";
  std::cout << "ENERGY: " << getMaterial(Material::ENERGY) << "\n";
  std::cout << "DARK_ENERGY: " << getMaterial(Material::DARK_ENERGY) << "\n";
  std::cout << "ICE: " << getMaterial(Material::ICE) << "\n";
  std::cout << "PLASMA: " << getMaterial(Material::PLASMA) << "\n";
  std::cout << "CHARGED: " << getMaterial(Material::CHARGED) << "\n";
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
 * Description: Returns the game id (base item ID) of the current Item, if
 *              the Item is still a base Item.
 *
 * Inputs: none
 * Output: int - the game_id of the Item or kUNSET_ID, if it's not a base item
 */
int Item::getGameID()
{
  if (isBaseItem())
    return game_id;

  return kUNSET_ID;
}

/*
 * Description: Returns the actual unique ID (my_id) of the current Item.
 * 
 * Inputs: none
 * Output: int - the actual unique ID of the current item
 */
int Item::getID()
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
  if (!(buff_set == new_buff_set))
    setBase(nullptr);

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
  if (new_brief_description.size() <= StringDB::kMAX_BRIEF_DESC)
  {
  	if (new_brief_description != brief_description)
      setBase(nullptr);

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
  if (new_description.size() <= StringDB::kMAX_DESC)
  {
  	if (new_description != description)
      setBase(nullptr);

    description = new_description;
    
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
  auto temp_flags = flags;

  (set_value) ? (flags |= flag) : (flags &= ~flag);

  if ((temp_flags & ItemFlags::SET_BASE_ITEM) == ItemFlags::SET_BASE_ITEM)
    if (temp_flags != flags)
      setBase(nullptr);
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
  auto temp_composition = composition;

  (set_value) ? (composition |= flag) : (composition &= ~flag);

  if (composition != temp_composition)
    setBase(nullptr);
}

 /*
 * Description: Attempts to assign a new string name to the Item
 *
 * Inputs: new_name - new string name to assign to the Item
 * Output: bool - the outcome of the name assignment
 */
bool Item::setName(const std::string &new_name)
{
  if (new_name.size() <= StringDB::kMAX_NAME)
  {
    if (new_name != name)
      setBase(nullptr);

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
  if (new_prefix.size() <= StringDB::kMAX_PREFIX)
  {
    if (new_prefix != prefix)
      setBase(nullptr);

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
  if (new_occasion != occasion)
    setBase(nullptr);

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
    if (new_mass != mass)
      setBase(nullptr);

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
  if (new_thumbnail != thumbnail)
     setBase(nullptr);

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
  if (new_animation != using_animation)
    setBase(nullptr);

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
  if (new_message.size() <= StringDB::kMAX_USE_MSG)
  {
    if (new_message != using_message)
      setBase(nullptr);

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
  if (new_skill != using_skill)
    setBase(nullptr);

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
  if (new_sound != using_sound)
    setBase(nullptr);

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
  	if (new_value != value)
  	  setBase(nullptr);

    value = new_value;

    return true;
  }

  return false;
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

 /*
 * Description: Overloaded assignment operator - copies source object into
 *              this using common copySelf() function after self-assignment 
 *              check and returns the copied object.
 *
 * Inputs: source - the object to be copied.
 * Output: Item& - return the copied object by ref.
 */
 Item& Item::operator=(const Item &source)
 {
  /* Copy if not self-assignment */
  if (this != &source)
  {
    copySelf(source);
    setID(++id);
  }

  return *this;
 }

 /*
 * Description: Move assignment operator, unsets the current object then
 *              uses std::swap to swap the data of the object with the source
 *              object without calling copy constructor, then unsets the
 *              source object and returns the copied object by ref.
 *
 * Inputs: source - r.value ref of an Item
 * Output: Item& - copy of the r.value ref returned by ref
 */
Item& Item::operator=(Item&& source)
{
  unsetAll(this);

  /* Move-copy if not self-assignment */
  if (this != &source)
  {
    swap(*this, source);
    setID(++id);
    unsetAll(&source);
  }

  return *this;
}