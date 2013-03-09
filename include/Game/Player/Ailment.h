/******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: None
* Description: The ailment class represents a Status Ailment or Debuff
******************************************************************************/
#ifndef AILMENT_H
#define AILMENT_H

#include <QtGui/QWidget>
#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "EnumDatabase.h"

class Ailment
{
public:
  /* Constructs an AttributeSet object */
  Ailment(Infliction t, ushort max, double ch = 0);

  /* Default ailment constructor */
  Ailment();

  /* Annihilates an AttributeSet object */
  ~Ailment();

private:
  /* Absolute maximum number of turns */
  static const ushort kMAX_TURNS = 25;

  /* Chance the status effect will wear off per turn (>1 = 100%, 0 = 0%) */
  float chance;

  /* Inflinction of the Ailment */
  Infliction ailment_type;

  /* Durations of the status_ailment (>KMAX_TURNS = INFINITY) */
  ushort max_turns_left;

public:
  /* Updates the ailment only */
  const bool update();

  /* Updates the ailment and appllies the effect */
  const bool updateAndApply(AttributeSet* attr_set, SkillSet* skill_set = NULL);

  /* Returns the number of turns left (assuming 0%) */
  ushort getTurnsLeft();

  /* Returns the Inflinction of the status ailment */
  Infliction getType();

  /* Gets an ailment name */
  static QString getName(Infliction t);
  static Infliction getInfliction(QString name);

  /* Sets the duration of the ailment */
  void setDuration(ushort max, double ch = 0);

  /* Sets the Inflinction of the Status Ailment */
  void setType(Infliction t);
};

#endif // AILMENT_H
