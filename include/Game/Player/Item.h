/*******************************************************************************
 * Class Name: Item [Header]
 * Date Created: December 9th, 2013
 * Inheritance: None
 * Description: An Item is an object contained within the inventory of a party.
 *              An Item may be a key item which is important for the progression
 *              through the game, some sort of material or component, an item
 *              that has a use in Battle or Menu (does some skill, etc.), or
 *              one of the Item sub-classes: Equipment or Bubby.
 *
 * Notes
 * -----
 *
 * [1]: Items are constructed primarily in one of two ways. First, base items
 *      are one-ofs, constructed from the item list in XML. These are your
 *      game ID items. All normal items that will be used in the game are
 *      created as copies of these items. These items will share almost all
 *      the same data as their parents, but will have unique IDs assosciated
 *      with them. Subclasses like Bubbies will also have altering data like
 *      level, experience, etc.
 *
 * TODO
 * ----
 ******************************************************************************/
#ifndef ITEM_H
#define ITEM_H

#include <iostream>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/Flavour.h"
#include "Game/Player/Skill.h"
//#include "EnumDb.h"
//#include "EnumFlags.h"
#include "Helpers.h"
#include "Sprite.h"
#include "StringDb.h"
#include "Sound.h"

/* Item flags describing the type of an Item */
ENUM_FLAGS(ItemFlags)
enum class ItemFlags
{
  CONSUMED       = 1 << 0,
  EQUIPMENT      = 1 << 1,
  BUBBY          = 1 << 2,
  KEY_ITEM       = 1 << 3,
  MATERIAL       = 1 << 4,
  NO_CATEGORY    = 1 << 5,
  STAT_ALTERING  = 1 << 6,
  SKILL_LEARNING = 1 << 7,
  HEALING_ITEM   = 1 << 8,
  RELIEVING_ITEM = 1 << 9,
  MONEY          = 1 << 10
}; /* TODO: A non-stackable flag? Maybe. Post beta T#355 */

/* Material flags describing the composition of an Item */
ENUM_FLAGS(Material)
enum class Material
{
  WOODEN       = 1 << 0,
  STEEL        = 1 << 1,
  BRASS        = 1 << 2,
  TITANIUM     = 1 << 3,
  GRAPHENE     = 1 << 4,
  PHYSICAL     = 1 << 5,  /* Bartite */
  NON_PHYSICAL = 1 << 6,  /* Cyllion */
  FIRE         = 1 << 7,  /* Effretus */
  FOREST       = 1 << 8,  /* Floriver */
  ICE          = 1 << 9,  /* Avalonium */
  ELECTRIC     = 1 << 10, /* Niobium */
  DIGITAL      = 1 << 11, /* Ravizer */
  NIHIL        = 1 << 12  /* Vacantus */
};

class Item
{
  friend Flavour;

public:
  /* Blank constructor */
  Item();

  /* Constructs a T0-Bubby based on a flaour pointer */
  Item(Flavour* const source);

  /* Constructs an item given a source pointer */
  Item(Item* const source);

  /* Base Item constructor - constructs an Item given a game ID and basics */
  Item(const int32_t &game_id, const std::string &name, const uint32_t &value,
      Frame* thumbnail = nullptr, const uint32_t &mass = 0,
      const uint32_t &dura = kMIN_DURABILITY);

  /* Key Item constructor */
  Item(const int32_t &game_id, const std::string &name,
      Frame* thumbnail = nullptr);

  /* Annihilates an Item object */
  virtual ~Item();

private:
  /* Static ID counter for the Item class */
  static int32_t id;

protected:
  /* Unique and non-unique IDs for the Item */
  int32_t game_id;
  int32_t my_id;

  /* Pointer to the Base Item, null if the Item has changed in any way */
  Item* base_item;

  /* AttrSet by which the Item will alter stats */
  AttributeSet buff_set;

  /* Brief and Full-length descriptions */
  std::string brief_description;
  std::string description;

  /* Material composition of the Item */
  Material composition;

  /* Item Durability */
  uint32_t max_durability;
  uint32_t durability;

  /* Flag set for the type of the current item */
  ItemFlags flags;

  /* Tiered level of item (TRASH, UNCOMMON, etc.) */
  ItemTier item_tier;

  /* Mass of the Item */
  uint32_t mass;

  /* The name of the Item and an attached prefix (ex. Bottles) */
  std::string name;
  std::string prefix;

  /* The occasion of use for the Item */
  ActionOccasion occasion;

  /* Pointer to the thumbnail */
  Frame* thumbnail;

  /* Pointer to the skill the Item either uses or teaches */
  Skill* using_skill;

  /* Pointer to the animation during use of the Item */
  Sprite* using_animation;

  /* Message the item displays when used */
  std::string using_message;

  /* Pointer to the sound which happens during use of the Item */
  Sound* using_sound;

  /* Value of the item (in the shop) */
  uint32_t value;
  int32_t  value_modifier;

  /* ------------ Constants --------------- */
public:
  static const uint32_t kID_MONEY;       /* ID of money item */
  static const uint32_t kMAX_DURABILITY; /* Maximum durability */
  static const uint32_t kMAX_MASS;       /* Maximum mass of an Item */
  static const uint32_t kMAX_VALUE;      /* Maximum value of an Item */
  static const uint32_t kMIN_DURABILITY; /* Minimum durability */
  static const uint32_t kMIN_MASS;       /* Minimum mass */
  static const int32_t  kUNSET_ID;       /* The unset ID for the item */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Sets up the Item class with basic values */
  void setupClass();

  /* Unsets all the Item data for deletion */
  void unsetAll();

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/
public:
  /* Prints out the Item information */
  virtual void print();

  /* Returns the buff_set */
  virtual AttributeSet getStats();

  /* Returns the mass of the Item */
  virtual uint32_t getMass();

  /* Returns the value of the Item */
  virtual uint32_t getValue();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Evaluates and returns whether the cur. object is a base item */
  bool isBaseItem();

  /* Load data from file */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer,
                std::string base_path);

  /* Prints the value of the flags of the Item */
  void printFlags();

  /* Prints out the basic info of the Item, in simple or complex formats */
  void printInfo(const bool &basic = false);

  /* Returns the brief description */
  std::string getBriefDescription();

  /* Returns the description */
  std::string getDescription();

  /* Returns the current durability */
  uint32_t getDurability();

  /* Evaluates and returns a given Item flag value */
  bool getFlag(ItemFlags test_flag);

  /* Returns the game_id (base item ID) of the object or kUNSET_ID */
  int32_t getGameID() const;

  /* Returns the unique ID of the Item */
  int32_t getID();

  /* Returns the enumerated ItemTier of the Item */
  ItemTier getItemTier();

  /* Evaluates and returns a given material composition flag */
  bool getMaterial(Material composition);

  /* Returns the maximum durability of the Item */
  uint32_t getMaxDurability();

  /* Returns the string name of the Item */
  std::string getName() const;

  /* Returns the prefix of the Item */
  std::string getPrefix();

  /* Returns the occasion [conditions of use] of the Item */
  ActionOccasion getOccasion();

  /* Returns the ptr to the thumbnail */
  virtual Frame* getThumb();

  /* Returns the ptr to the using animation */
  Sprite* getUseAnimation();

  /* Returns the using message */
  std::string getUseMessage();

  /* Returns the using skill */
  Skill* getUseSkill();

  /* Returns the sound effect */
  Sound* getUseSound();

  /* Assigns a new buff set */
  void setBuffSet(const AttributeSet &new_buff_set);

  /* Attempts to assign a new brief description */
  bool setBriefDescription(const std::string &new_brief_description);

  /* Attempts to assign a new description */
  bool setDescription(const std::string &new_description);

  /* Assigns a new durability to the item */
  bool setDurability(const uint32_t &new_durability, const bool &max = false);

  /* Assigns a given ItemFlags flag to a given value */
  void setFlag(ItemFlags flag, const bool &set_value = true);

  /* Sets the game ID */
  void setGameID(int32_t id);

  /* Assigns a new item tier for the item */
  void setItemTier(const ItemTier &new_item_tier);

  /* Assigns a given material composition flag a given value */
  void setMaterial(Material flag, const bool &set_value = true);

  /* Attempts to assign a new string name */
  bool setName(const std::string &new_name);

  /* Attempts to assign a new string prefix */
  bool setPrefix(const std::string &new_prefix);

  /* Assigns a new occasion [conditions of use] */
  void setOccasion(const ActionOccasion &new_occasion);

  /* Attempts to assign a new mass */
  bool setMass(uint32_t new_mass);

  /* Assigns a new icon image, returns true if the new image is not null */
  bool setThumbnail(Frame* new_thumbnail);

  /* Assigns a new animation, returns true if the new animation is not null */
  bool setUseAnimation(Sprite* new_animation);

  /* Attempts to assign a new using message */
  bool setUseMessage(const std::string &new_message);

  /* Assigns a new use skill, returns true if the new skill is not null */
  bool setUseSkill(Skill* new_skill);

  /* Assigns a new sound effect, returns true if the new sound is not null */
  bool setUseSound(Sound* new_sound);

  /* Attempts to set a new value for the Item */
  bool setValue(const uint32_t &new_value);

  /* Sets the modifier value, to affect the set value */
  void setValueModifier(const int32_t &new_value);
};

#endif //ITEM_H
