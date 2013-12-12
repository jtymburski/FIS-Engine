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
  CONSUMED       = 1 << 2,
  OFFENSIVE      = 1 << 3,
  DEFENSIVE      = 1 << 4,
  EQUIPMENT      = 1 << 5,
  BUBBY          = 1 << 6,
  KEY_ITEM       = 1 << 7,
  MATERIAL       = 1 << 8,
  GENE_COMP      = 1 << 9,
  STAT_ALTERING  = 1 << 10,
  SKILL_LEARNING = 1 << 11,
  HEALING_ITEM   = 1 << 12,
  RELIEVING_ITEM = 1 << 13,
  SET_BASE_ITEM  = 1 << 14
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

  Item();

  Item(const Item &source);

  Item(Item&& source);

  Item(const std::string &name, const uint32_t &value,
  	   Frame* thumbnail = nullptr, const double &mass = 0.0);

  ~Item();

private:

  static int id;

protected:

  int my_id;

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

  /* Mass of the Item */
  double mass;

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

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  
  void classSetup();

  void copySelf(const Item& source);

  static void swap(Item& object, Item& source);

  static void unsetAll(Item* object);

  void setID(const uint32_t &value);

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/
public:

  virtual void print();

  virtual AttributeSet getBuffSet();

  virtual double getMass();

  virtual uint32_t getValue();

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:

  void printFlags();

  void printInfo(const bool &basic = false);

  bool setBase(Item* item_base);

  void setBuffSet(const AttributeSet &new_buff_set);

  bool setBriefDescription(const std::string &new_brief_description);

  bool setDescription(const std::string &new_description);

  void setFlag(ItemFlags flag, const bool &set_value = true);

  void setMaterial(Material flag, const bool &set_value = true);

  void setName(const std::string &new_name);

  bool setPrefix(const std::string &new_prefix);

  void setOccasion(const ActionOccasion &new_occasion);

  bool setMass(const double &new_mass);

  bool setThumbnail(Frame* new_thumbnail);

  bool setUseAnimation(Sprite* new_animation);

  bool setUseMessage(const std::string &new_message);

  bool setUseSkill(Skill* new_skill);

  bool setUseSound(Sound* new_sound);

  bool setValue(const uint32_t &new_value);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  bool isBaseItem();

  std::string getBriefDescription();

  std::string getDescription();

  bool getFlag(ItemFlags test_flag);

  bool getMaterial(Material composition);

  std::string getName();

  std::string getPrefix();

  ActionOccasion getOccasion();

  Frame* getThumb();

  Sprite* getUseAnimation();

  std::string getUseMessage();

  Skill* getUseSkill();

  Sound* getUseSound();

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
