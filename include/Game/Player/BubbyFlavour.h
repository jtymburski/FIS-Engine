/*******************************************************************************
* Class Name: BubbyFlavour
* Date Created: March 6th, 2013
* Inheritance: None
* Description: Describes the flavour of a Bubby. Every Bubby of certain flavours
*              will have the same SkillSet and AttributeSet as others.
*******************************************************************************/
#ifndef BUBBYFLAVOUR_H
#define BUBBYFLAVOUR_H

#include <QtGui/QWidget>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"

class BubbyFlavour
{
public:
  /* Constructs a BubbyFlavour object */
  BubbyFlavour(QString name, AttributeSet stats, SkillSet* skills = NULL);

  /* Annihilates a BubbyFlavour object */
 ~BubbyFlavour();

private:
  /* Stat set for the Bubby (bonuses) */
  AttributeSet stats;

  /* Name and description of the BubbyFlavour */
  QString name;
  QString description;

  /* The list of bubby flavours */

  /* The list of actions offered by the bubby */
  SkillSet* skill_list;

  /* The sprite image for each tier */
  QVector<Sprite*> sprites;

  /* ------------ Constants --------------- */
  const static ushort kTIER_CAP;        /* Max # of Bubby Tiers */
  static QVector<QString> flavour_list; /* Vector of Flavour names */

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Adding, checking flavour functions */
  static bool addFlavour(QString new_flavour);

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Gets the set of attributes for the Bubby */
  AttributeSet* getAttr();

  /* Returns the list of instantiated flavours */
  static QVector<QString> getFlavourList();

  /* Gets the name and description */
  QString getName();
  QString getDescription();

  /* Gets the list of bubby actions (Used for total action lists in battle)*/
  SkillSet* getSkillSet();

  /* Gets the sprites for the bubby */
  QVector<Sprite*> getSprites();

  /* Sets the AttributeSet for the Bubby */
  void setAttr(AttributeSet new_stats);

  /* Set the name and description of the BubbyFlavour */
  void setName(QString new_name);
  void setDescription(QString new_description);

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
};

#endif //BUBBYFLAVOUR_H
