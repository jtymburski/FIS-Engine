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
* [1]:
*
* TODO
* ----
* 
*******************************************************************************/

#ifndef ITEM_H
#define ITEM_H

#include <iostream>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/Flavour.h"
#include "Game/Player/Skill.h"
#include "EnumDb.h"
#include "EnumFlags.h"
#include "Helpers.h"
#include "Sprite.h"
#include "StringDB.h"
#include "Sound.h"

/* Item flags describing the type of an Item */
ENUM_FLAGS(ItemFlags)
enum class ItemFlags
{
  CONSUMED       = 1 << 0,
  OFFENSIVE      = 1 << 1,
  DEFENSIVE      = 1 << 2,
  EQUIPMENT      = 1 << 3,
  BUBBY          = 1 << 4,
  KEY_ITEM       = 1 << 5,
  MATERIAL       = 1 << 6,
  GENE_COMP      = 1 << 7,
  STAT_ALTERING  = 1 << 8,
  SKILL_LEARNING = 1 << 9,
  HEALING_ITEM   = 1 << 10,
  RELIEVING_ITEM = 1 << 11,
  SET_BASE_ITEM  = 1 << 12
};

/* Material flags describing the composition of an Item */
ENUM_FLAGS(Material)
enum class Material
{
  METALLIC    = 1 << 0,
  WOODEN      = 1 << 1,
  INSULATED   = 1 << 2,
  ANTIMATTER  = 1 << 3,
  DARKMATTER  = 1 << 4,
  ENERGY      = 1 << 5,
  DARK_ENERGY = 1 << 6,
  ICE         = 1 << 7,
  PLASMA      = 1 << 8,
  CHARGED     = 1 << 9,
};

class Item
{
public:
  /* Default Item constructor - constructs a basic Item object */
  Item();

  /* Copy constructor - constructs copy of base Item with unique ID */
  Item(const Item &source);

  /* Mvoe constructor - constructs copy of Item with r.value ref */
  Item(Item&& source);

  Item(Flavour* const source);

  /* Base Item constructor - constructs an Item given a game ID and basics */
  Item(const uint32_t &game_id, const std::string &name, const uint32_t &value,
       Frame* thumbnail = nullptr, const double &mass = 0.0);

  /* Annihilates an Item object */
  ~Item();

private:
  /* Static ID counter for the Item class */
  static int id;

protected:
  /* Pointer to the Base Item, null if the Item has changed in any way */
  Item* base_item;

  /* AttrSet by which the Item will alter stats */
  AttributeSet buff_set;
  
  /* Brief and Full-length descriptions */
  std::string brief_description;
  std::string description;

  /* Material composition of the Item */
  Material composition;

  /* Flag set for the type of the current item */
  ItemFlags flags;

  /* Game ID (Base Item ID#) of the Item */
  const int game_id;

  /* Mass of the Item */
  double mass;

  /* The item-only ID of the current Item object */
  int my_id;

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

  /* ------------ Constants --------------- */
  static const double   kMAX_MASS;       /* Maximum mass of an Item */
  static const double   kMIN_MASS;       /* Minimum mass */
  static const uint32_t kMAX_VALUE;      /* Maximum value of an Item */
  static const int      kUNSET_ID;       /* Game_ID unset value */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Sets up the Item class with basic values */
  void classSetup();

  /* Copies a const ref source Item object to this object */
  void copySelf(const Item& source);

  /* Static function for swappin two item object's data (ex. move construct) */
  static void swap(Item& object, Item& source);

  /* Unsets all the Item data for deletion */
  static void unsetAll(Item* object);

  /* Assigns a unique ID to the Item */
  void setID(const uint32_t &value);

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/
public:
  /* Prints out the Item information */
  virtual void print();

  /* Returns the buff_set */
  virtual AttributeSet getStats();

  /* Returns the mass of the Item */
  virtual double getMass();

  /* Returns the value of the Item */
  virtual uint32_t getValue();

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
  /* Assigns the base item pointer */
  bool setBase(Item* item_base);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Evaluates and returns whether the cur. object is a base item */
  bool isBaseItem();

  /* Prints the value of the flags of the Item */
  void printFlags();

  /* Prints out the basic info of the Item, in simple or complex formats */
  void printInfo(const bool &basic = false);

  /* Returns the brief description */
  std::string getBriefDescription();

  /* Returns the description */
  std::string getDescription();

  /* Evaluates and returns a given Item flag value */
  bool getFlag(ItemFlags test_flag);

  /* Returns the game_id (base item ID) of the object or kUNSET_ID */
  int getGameID();

  /* Returns the unique ID of the Item */
  int getID();

  /* Evaluates and returns a given material composition flag */
  bool getMaterial(Material composition);

  /* Returns the string name of the Item */
  std::string getName();

  /* Returns the prefix of the Item */
  std::string getPrefix();

  /* Returns the occasion [conditions of use] of the Item */
  ActionOccasion getOccasion();

  /* Returns the ptr to the thumbnail */
  Frame* getThumb();

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

  /* Assigns a given ItemFlags flag to a given value */
  void setFlag(ItemFlags flag, const bool &set_value = true);

  /* Assigns a given material composition flag a given value */
  void setMaterial(Material flag, const bool &set_value = true);

  /* Attempts to assign a new string name */
  bool setName(const std::string &new_name);

  /* Attempts to assign a new string prefix */
  bool setPrefix(const std::string &new_prefix);

  /* Assigns a new occasion [conditions of use] */
  void setOccasion(const ActionOccasion &new_occasion);

  /* Attempts to assign a new mass */
  bool setMass(const double &new_mass);

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

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/
public:
  /* Overloaded = operator for copy construction */
  Item& operator=(const Item &source);

  /* Overloaded move assignment operator */
  Item& operator=(Item&& source);
};

#endif //ITEM_H
