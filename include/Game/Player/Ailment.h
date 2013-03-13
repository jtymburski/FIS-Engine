/*******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: N/A
* Description: The ailment class represents a Status Ailment or Debuff. Ailments
*              use the Infliction enumerated type to store their type as well
*              as the EnumString class to conver the enumerations to strings
*              instead of doing string compare (note that these are std::strings
*
* Note [1]: To add, remove or change a status ailment:
*              1 - (Add) an enumerated value to Infliction enum EnumDatabase.h
*              2 - (Add) a registered string to BEGIN_ENUM_STRING( Infliction )
*              3 - [(Add)] const static values to be used if necessary to class
*              4 - (Add) the Ailment's effect to apply(), and other flags
*              5 - (Add) the Ailment's immunities based off Person flags
*              6 - (Add) any effects or checks in Battle or subclasses
*******************************************************************************/
#ifndef AILMENT_H
#define AILMENT_H

#include <QtGui/QWidget>
#include "Game/Player/Person.h"
#include "EnumDatabase.h"

class Ailment : public QWidget
{
  Q_OBJECT

public:
  /* Constructor: Sets up ailment with type, turn and chance durations */
  Ailment(Person* victim, Infliction type, short max_turns = 0, double chance = 0,
          QWidget* parent = NULL);

  /* Constructor: Sets up an ailment with a QString instead of an enum */
  Ailment(Person* victim, QString name, short max_turns = 0, double chance = 0,
          QWidget* parent = NULL);

  /* Default constructor: Sets up a blank NOAILMENT type */
  Ailment(Person* victim, QWidget* parent = NULL);

  /* Annihilates an AttributeSet object */
  ~Ailment();

  enum AilmentFlag
  {
    INFINITE    = 1 << 0, /* Ailment does not alleviate by time? >kMAX_TURNS */
    CURABLE     = 1 << 1, /* Ailment cannot be cured except by time? */
    TOBECURED   = 1 << 1, /* The ailment is to be cured immediately */
    TOBEUPDATED = 1 << 2, /* Ailment set to be updated on new turn */
    TOBEAPPLIED = 1 << 3, /* Ailment effects set to be applied on new turn */
    FAVORABLE   = 1 << 4, /* Is this ailment a favorable ailment? */
    ADVERSE     = 1 << 5, /* Is this ailment an adverse ailment? */
    NEUTRAL     = 1 << 6, /* (!FAVORABLE && !ADVERSE) */
    IMMUNITY    = 1 << 7  /* Is the inflicted person immune to this ailment? */
  };
  Q_DECLARE_FLAGS(AilmentFlags, AilmentFlag)

private:
  /*------------------- Constants -----------------------*/
  static const ushort kMAX_TURNS = 25; /* Maximum # turns ailments will last */
  static const ushort kMIN_TURNS =  1; /* The minimum # turns ailments last */

  /* Inflinction of the Ailment */
  Infliction ailment_type;

  /* Chance the status effect will wear off per turn (>1 = 100%, 0 = 0%) */
  float chance;

  /* Set of flags for the current ailment */
  AilmentFlags flag_set;

  /* Durations of the status_ailment (>KMAX_TURNS = INFINITY) */
  short max_turns_left;
  short turns_occured;

  /* The victim (owner) of the ailment. */
  Person* victim;

  /* Applies the effect of the ailment */
  const bool apply();

  /* Checks the immunity of the ailment */
  const bool checkImmunity(Person* new_victim);

  /* Updates the ailment by decrementing the turn counter if necessary */
  const bool updateTurns();

  /* Sets the Inflinction of the Status Ailment */
  void setType(Infliction t);

  /* Assigns the victim of the object */
  void setVictim(Person* set_victim);

signals:

public slots:

public:
  /* Updates the ailment (will call apply and updateTurns() */
  void update();

  /* Undoes the effect (if exits) to the victim before curing */
  void unapply();

  /* Evaluates an ailment flag or flags */
  const bool getFlag(AilmentFlag flags);

  /* Returns the number of turns left (assuming 0%) */
  ushort getTurnsLeft();

  /* Returns the Inflinction of the status ailment */
  Infliction getType();

  /* Gets a QString of the current ailment's enumerated value */
  QString getName();

  /* Gets an infliction from a QString */
  static QString getAilmentStr(Infliction type);
  static Infliction getInfliction(QString name);

  /* Obtains the victim of the Status Ailment */
  Person* getVictim();

  /* Sets the duration of the ailment */
  void setDuration(ushort max_turns, double chance = 0);

  /* Sets the value of an AilmentFlag to a set_value, defaulting to TRUE */
  void setFlag(AilmentFlag flags, const bool set_value = TRUE);

  /* Public function to assign a new victom for the status ailment */
  const bool setNewVictim(Person* new_victim, const bool refresh_turns = FALSE);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Ailment::AilmentFlags)

#endif // AILMENT_H
