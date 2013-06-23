/******************************************************************************
* Class Name: Item
* Date Created: October 28th 2012 (Rewritten May 26th 2013)
* Inheritance: None
* Description: The Item header defines all attributes describing an item.
*              This is the parent class of Equipment and Bubby, which are
*              Items that have additional functionality.
******************************************************************************/
#ifndef ITEM_H
#define ITEM_H

#include <QDebug>
#include <QString>
#include <QPair>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "Game/Sprite.h"
#include "Sound.h"

class Item
{
public:
  /* Item constructor function */
  Item(QString name, uint value = 0, Sprite* thumbnail = 0, double mass = 0);

  /* Annihilates an Item object */
  ~Item();

  /* Enumerated flags for item class */
  enum ItemState
  {
    BATTLEREADY   = 1ul << 0, /* Can the item be used in battle */
    MENUREADY     = 1ul << 1, /* Can the item be used in the menu? */
    HEALITEM      = 1ul << 2, /* Does the item heal vitality? */
    CURE          = 1ul << 3, /* Does the item cure ailments? */
    OFFENSIVE     = 1ul << 4, /* Does the item have an offensive battle use? */
    INDEFINITE    = 1ul << 5, /* If item has infinite uses */
    STACKABLE     = 1ul << 6, /* If item can be grouped with others */
    EQUIPMENT     = 1ul << 7, /* Is the item a piece of equipment? */
    BUBBY         = 1ul << 8, /* Is the item a Bubby? */
    KEYITEM       = 1ul << 9, /* is the item a unique quest item? */
    MULTIITEM     = 1ul << 10, /* Does the item hit more than one target? */
    PARTYITEM     = 1ul << 11 /* Does the item effect all members of a party? */
  };
  Q_DECLARE_FLAGS(ItemFlags, ItemState)
  ItemFlags iflag_set;

private:
  /*------------------- Constants -----------------------*/
  static const ushort kMAX_ACTION_SET; /* Max action set item can perform */
  static const ushort kMAX_BRIEF_DESCRIPTION; /* Max # of chars for desc. */
  static const ushort kMAX_DESCRIPTION; /* Max # of char for desc. */
  static const double kMAX_MASS; /* Max mass the item cna have */
  static const ushort kMAX_SKILL_SET; /* Max size of skill set */
  static const uint kMAX_VALUE;

protected:
  /* Buff Set which is factor by which the item can alter stats */
  AttributeSet* buff_set;

  /* Description of the item */
  QString brief_description;
  QString description;

  /* Duration the items effect will last (turns of Battle) */
  ushort duration;

  /* Static ID of the Item (current increasing value) */
  static int id;

  /* ID of the current Item object */
  int my_id;

  /* Mass of the item (not referenced to gravity) */
  double mass;

  /* Name of the item */
  QString name;

  /* SkillSet which can be gathered for the item */
  SkillSet* action_set;
  SkillSet* skill_set;

  /* Thumb sprite of the item */
  Sprite* thumbnail;

  /* Message and sound when consumed */
  Sprite* using_animation;
  QString using_message;
  Sound* using_sound;

  /* Value (in credits) to a shop */
  uint value;

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
  /* Increments the Item's ID */
  int setId();

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/
public:
  /* Methods for printing info of item */
  virtual void printAll();
  virtual void printFlags();
  virtual void printInfo();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the set of actions the item can perform */
  SkillSet* getActionSet();

  /* Returns the AttributeSet of the item */
  AttributeSet* getBuffSet();

  /* Returns the shorter, brief description of the item */
  QString getBriefDescription();

  /* Returns the description of the item */
  QString getDescription();

  /* Returns the duration (# of turns in Battle) of the item */
  ushort getDuration();

  /* Returns the current object's ID */
  int getId();

  /* Evaluates the value of a given ItemState flag */
  bool getItemFlag(ItemState flag);

  /* Returns the mass of the item */
  double getMass();

  /* Returns the name of the item */
  QString getName();

  /* Returns the SkillSet of the item */
  SkillSet* getSkillSet();

  /* Returns the thumbnail of the unit */
  Sprite* getThumb();

  /* Returns the animation when used or activated */
  Sprite* getUsingAnimation();

  /* Returns the using message of the item */
  QString getUsingMessage();

  /* Returns the using sound of the item */
  Sound* getUsingSound();

  /* Returns the value of an item */
  uint getValue();

  /* Assigns a set of actions that the item can perform */
  void setActionSet(SkillSet* new_action_set);

  /* Assigns an attribute set to the item */
  void setBuffSet(AttributeSet* new_buff_set);

  /* Assigns a brief description to the item */
  void setBriefDescription(QString new_brief_description);

  /* Assigns a description to the item */
  void setDescription(QString new_description);

  /* Assigns a duration to the item */
  void setDuration(ushort new_duration);

  /* Assigns a given value to a given ItemState flag */
  void setItemFlag(ItemState flag, bool set_value = true);

  /* Assigns a mass to the item */
  void setMass(double new_mass);

  /* Assigns the name of the item */
  void setName(QString new_name);

  /* Assigns a skill set to the item */
  void setSkillSet(SkillSet* new_skill_set);

  /* Assigns an icon thumbnail to the item */
  void setThumb(Sprite* new_thumb);

  /* Assigns a using animation */
  void setUsingAnimation(Sprite* new_using_animation);

  /* Asigns a using message to the item */
  void setUsingMessage(QString new_using_message);

  /* Assigns a using sound to the item */
  void setUsingSound(Sound* new_using_sound);

  /* Assigns the value of an item */
  void setValue(uint new_value);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Item::ItemFlags);

#endif // ITEM_H

