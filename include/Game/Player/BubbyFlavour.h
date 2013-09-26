/*******************************************************************************
* Class Name: BubbyFlavour
* Date Created: March 6th, 2013
* Inheritance: None
* Description: Describes the flavour of a Bubby. Every Bubby of certain flavours
*              will have the same SkillSet and AttributeSet as others.
*******************************************************************************/
#ifndef BUBBYFLAVOUR_H
#define BUBBYFLAVOUR_H

#include <QWidget>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"

class BubbyFlavour : QWidget
{
  Q_OBJECT

  friend class Bubby;

public:
  /* Constructs a BubbyFlavour object */
  BubbyFlavour(BubbyFlavour* flavour_parent, QString flavour_name,
               AttributeSet* stat_set = 0, SkillSet* skills = 0);

  /* Annihilates a BubbyFlavour object */
  ~BubbyFlavour();

  /* FlavourState enumerated flags */
  enum FlavourState
  {
    CREATED_WITH_PARENT = 1 << 0
  };
  Q_DECLARE_FLAGS(FlavourFlags, FlavourState)

private:
  /* Pointer to object's parent */
  BubbyFlavour* parent;

  /* Stat set for the Bubby */
  AttributeSet* stats;

  /* Flavour flags object */
  FlavourFlags flag_set;

  /* Mass of a Bubby of this particular flavour */
  double mass;

  /* Name and description of the BubbyFlavour */
  QString name;
  QString description;

  /* The list of actions offered by the bubby */
  SkillSet* skill_list;

  /* The sprite image for each tier */
  QVector<Sprite*> sprites;

  /* ------------ Constants --------------- */
  static const ushort    kTIER_CAP;     /* Max # of Bubby Tiers */
  static QVector<ushort> kTIER_LEVELS;  /* Max levels of skills */
  static QVector<QString> flavour_list; /* Vector of Flavour names */

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Adding, checking flavour functions */
  static bool addFlavour(QString new_flavour);

  /* Build the skill levels vector */
  static void buildLevels();

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  /* Methods for printing the state of the Class */
  void printInfo();

  /* FlavourFlag flag functions */
  bool getFlavourFlag(FlavourState flags);
  void setFlavourFlag(FlavourState flags, bool set_value = true);

  /* Gets the set of attributes for the Bubby */
  AttributeSet* getAttr();

  /* Returns the mass of a Bubby of this particular flavour */
  double getMass();

  /* Gets the name and description */
  QString getName();
  QString getDescription();

  /* Returns the parent of the object */
  BubbyFlavour* getParent();

  /* Gets the list of bubby actions (Used for total action lists in battle)*/
  SkillSet* getSkillSet();

  /* Gets the sprites for the bubby */
  QVector<Sprite*> getSprites();

  /* Sets the AttributeSet for the Bubby */
  void setAttr(AttributeSet* new_stats);

  /* Assigns a new mass to the Bubby flavour */
  void setMass(double new_mass);

  /* Set the name and description of the BubbyFlavour */
  void setName(QString new_name);
  void setDescription(QString new_description);

  /* Sets the parent of the Bubby flavour object */
  bool setParent(BubbyFlavour* new_parent);

  /* Sets the new SkillSet */
  void setSkillSet(SkillSet* new_skill_list);

  /* Sets the sprites */
  bool setSprites(QVector<Sprite*> new_sprites);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns true if a given flavour of Bubby already exists */
  static int isFlavour(QString flavour_name);

  /* Returns the list of instantiated flavours */
  static QVector<QString> getFlavourList();

  /* Gets the number of tiers for Bubby's */
  static ushort getNumTier();

  /* Returns the maximum level of skills a certain Bubby tier unlocks */
  static ushort getMaxSkillLevel(ushort tier_level);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(BubbyFlavour::FlavourFlags)

#endif //BUBBYFLAVOUR_H
