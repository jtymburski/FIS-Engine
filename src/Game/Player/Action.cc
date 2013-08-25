/*******************************************************************************
* Class Name: Action
* Date Created: Sunday, October 28th, 2012
* Inheritance:
* Description: Action is an element of a skill (skill can have up to 10 actions
* that will do alter stats, flip flags of a person, etc., or do damage to them.
*******************************************************************************/
#include "Game/Player/Action.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default action constructor
 *
 * Inputs: none
 */
Action::Action()
    : ailment(EnumDb::NOAILMENT),
      min_duration(0),
      max_duration(0),
      base_change(0),
      variance(0.00)
{
  ailment = EnumDb::NOAILMENT;
}

/*
 * Description: Action constructor object
 *
 * Inputs: QString raw - the raw language to be parsed
 */
Action::Action(QString raw)
{
  parse(raw);
}

/*
 * Description: Annihilates an action object
 */
Action::~Action() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Sets the string of the ailment to be inflicted
 *
 * Inputs: QString - string to set the infliction to
 * Output: none
 */
void Action::setAilment(EnumDb::Infliction new_ailment)
{
  ailment = new_ailment;
}

/*
 * Description: Parses the raw language into something we can have skills use
 *
 * Notes: Language Syntax Below:
 *
 * [ACTION ID],[LOWER/UPPER/GIVE/TAKE],[STATISTIC/AILMENT],
 * [MIN DURATION].[MAX DURATION],[IGNORE ATK],[IGNORE ATK ELM 1]...,
 * [IGNORE DEF],[IGNORE DEF ELM 1]...,[BASECHANGE],[VARIANCE];
 *
 * [int]:[string][string][uint].[uint],[bool],[string]...,
 * [bool],[string]...,[uint],[float];
 *
 * Inputs: QString - the raw string of the input
 * Output: none
 */
void Action::parse(QString raw)
{
  QStringList split = raw.split(',');

  /* Parse ID */
  setId(split.at(0).toInt());

  /* Parse LOWER/UPPER/GIVE/TAKE --> BRANCH */
  if (split.at(1) == "LOWER")
    setActionFlag(Action::LOWER);
  else if (split.at(1) == "RAISE")
    setActionFlag(Action::RAISE);
  else if (split.at(1) == "GIVE")
    setActionFlag(Action::GIVE);
  else if (split.at(1) == "TAKE")
    setActionFlag(Action::TAKE);

  /* Parse STATISTIC or AILMENT */
  if (getActionFlag(Action::LOWER) || getActionFlag(Action::RAISE))
  {
    QStringList stat_split = split.at(2).split(' ');
    /* Check which stat is to be affected */
    if (stat_split.at(0) == "THERMAL")
      setActionFlag(Action::THERMAL);
    else if (stat_split.at(0) == "POLAR")
      setActionFlag(Action::POLAR);
    else if (stat_split.at(0) == "PRIMAL")
      setActionFlag(Action::PRIMAL);
    else if (stat_split.at(0) == "CHARGED")
      setActionFlag(Action::CHARGED);
    else if (stat_split.at(0) == "CYBERNETIC")
      setActionFlag(Action::CYBERNETIC);
    else if (stat_split.at(0) == "NIHIL")
      setActionFlag(Action::NIHIL);
    else if (stat_split.at(0) == "VITALITY")
      setActionFlag(Action::VITALITY);
    else if (stat_split.at(0) == "QD")
      setActionFlag(Action::QUANTUM_DRIVE);
    else if (stat_split.at(0) == "MOMENTUM")
      setActionFlag(Action::MOMENTUM);
    else if (stat_split.at(0) == "LIMBERTUDE")
      setActionFlag(Action::LIMBERTUDE);
    else if (stat_split.at(0) == "UNBEARABILITY")
      setActionFlag(Action::UNBEARABILITY);

    /* Check whether the stat to be affected is offensive or defensive */
    if (stat_split.size() > 1)
    {
      if (stat_split.at(1) == "AGGRESSION")
        setActionFlag(Action::OFFENSIVE);
      else if (stat_split.at(1) == "DEFENSIVE")
        setActionFlag(Action::DEFENSIVE);
    }
  }
  else if (getActionFlag(Action::GIVE) || getActionFlag(Action::TAKE))
    setAilment(getInfliction(split.at(2)));

  /* Parse Duration */
  QStringList duration_split = split.at(3).split('.');
  setDuration(duration_split.at(0).toInt(),duration_split.at(1).toInt());

  /* Parse Ignore Atk */
  setIgnoreFlag(Action::IGNORE_ATK, split.at(4).toInt());

  /* Parse Ignore Atk Elements */
  QStringList atk_split = split.at(5).split('.');
  for (int i = 0; i < atk_split.size(); i++)
  {
    if (atk_split.at(i) == "PHYSICAL")
      setIgnoreFlag(Action::IGNORE_PHYS_ATK);
    else if (atk_split.at(i) == "THERMAL")
      setIgnoreFlag(Action::IGNORE_THER_ATK);
    else if (atk_split.at(i) == "POLAR")
      setIgnoreFlag(Action::IGNORE_POLA_ATK);
    else if (atk_split.at(i) == "PRIMAL")
      setIgnoreFlag(Action::IGNORE_PRIM_ATK);
    else if (atk_split.at(i) == "CHARGED")
      setIgnoreFlag(Action::IGNORE_CHAR_ATK);
    else if (atk_split.at(i) == "CYBERNETIC")
      setIgnoreFlag(Action::IGNORE_CYBE_ATK);
    else if (atk_split.at(i) == "NIHIL")
      setIgnoreFlag(Action::IGNORE_NIHI_ATK);
  }

  /* Parse Ignore Def */
  setIgnoreFlag(Action::IGNORE_DEF, split.at(6).toInt());

  /* Parse Ignore Def Elements */
  QStringList def_split = split.at(7).split('.');
  for (int i = 0; i < def_split.size(); i++)
  {
    if (def_split.at(i) == "PHYSICAL")
      setIgnoreFlag(Action::IGNORE_PHYS_DEF);
    else if (def_split.at(i) == "THERMAL")
      setIgnoreFlag(Action::IGNORE_THER_DEF);
    else if (def_split.at(i) == "POLAR")
      setIgnoreFlag(Action::IGNORE_POLA_DEF);
    else if (def_split.at(i) == "PRIMAL")
      setIgnoreFlag(Action::IGNORE_PRIM_DEF);
    else if (def_split.at(i) == "CHARGED")
      setIgnoreFlag(Action::IGNORE_CHAR_DEF);
    else if (def_split.at(i) == "CYBERNETIC")
      setIgnoreFlag(Action::IGNORE_CYBE_DEF);
    else if (def_split.at(i) == "NIHIL")
      setIgnoreFlag(Action::IGNORE_NIHI_DEF);
  }

  /* Parse Base Change & Variance */
  setBaseChange(split.at(8).toInt());
  split[9].chop(1); /* Remove trailing ; */
  setVariance(split.at(9).toFloat());
}

/*
 * Description: Sets the id of the action
 *
 * Inputs: int - the id to be set (negative for normal, positive for status)
 * Output: none
 */
void Action::setId(int new_id)
{
  id = new_id;
}

/*
 * Description: Sets the duration of the action
 *
 * Inputs: min - minimum # of turns the action takes place (> 0)
 *         max - maximum # of turns the action takes place
 * Output: none
 */
void Action::setDuration(uint min, uint max)
{
  min_duration = min;
  max_duration = max;
}

/*
 * Description: Sets the base change describing the specified stat
 *
 * Inputs: uint - new value of base change
 * Output: none
 */
void Action::setBaseChange(uint new_value)
{
  base_change = new_value;
}

/*
 * Description: Sets the variance of the action
 *
 * Inputs: float - new value of the variance
 * Output: none
 */
void Action::setVariance(float new_value)
{
  if (new_value <= 1)
    variance = new_value;
  else
    variance = 1;
}

/*
 * Description: Sets an IgnoreAttack flag to a boolean
 *
 * Inputs: IgnoreAttack flag to be set
 *         Boolean value to set the flag to
 * Output: none
 */
void Action::setIgnoreFlag(IgnoreFlag flags, bool set_value)
{
  (set_value) ? (ignore_flags |= flags) : (ignore_flags ^= flags);
}

/*
 * Description: Sets an ActionType flag to a boolean
 *
 * Inputs: ActionType flag to be set
 *         Boolean value to set the flag to
 * Output: none
 */
void Action::setActionFlag(ActionType flags, bool set_value)
{
  (set_value) ? (action_flags |= flags) : (action_flags &= flags);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Prints all the info describing an action by calling the sub
 *              print functions.
 *
 * Inputs: none
 * Output: none
 */
void Action::printAll()
{
  printInfo();
  printFlags();
  qDebug() << " --- / Action ---";
}

/*
 * Description: Prints the basic info describing an action.
 *
 * Inputs: none
 * Output: none
 */
void Action::printInfo()
{
  qDebug() << "Action ID #: " << id;
  qDebug() << "Min Duration: " << min_duration;
  qDebug() << "Max Duration: " << max_duration;
  qDebug() << "Base Change: " << base_change;
  qDebug() << "Variance: " << variance;
}

/*
 * Description: Prints out the stored flag values
 *
 * Inputs: none
 * Output: none
 */
void Action::printFlags()
{
  qDebug() << "RAISE: " << getActionFlag(Action::RAISE);
  qDebug() << "LOWER: " << getActionFlag(Action::LOWER);
  qDebug() << "GIVE: " << getActionFlag(Action::GIVE);
  qDebug() << "TAKE: " << getActionFlag(Action::TAKE);
  qDebug() << "OFFENSIVE: " << getActionFlag(Action::OFFENSIVE);
  qDebug() << "DEFENSIVE: " << getActionFlag(Action::DEFENSIVE);
  qDebug() << "PHYSICAL: " << getActionFlag(Action::PHYSICAL);
  qDebug() << "THERMAL: " << getActionFlag(Action::THERMAL);
  qDebug() << "POLAR: " << getActionFlag(Action::POLAR);
  qDebug() << "PRIMAL: " << getActionFlag(Action::PRIMAL);
  qDebug() << "CHARGED: " << getActionFlag(Action::CHARGED);
  qDebug() << "CYBERNETIC: " << getActionFlag(Action::CYBERNETIC);
  qDebug() << "NIHIL: " << getActionFlag(Action::NIHIL);
  qDebug() << "VITALITY: " << getActionFlag(Action::VITALITY);
  qDebug() << "QTMN DRIVEL: " << getActionFlag(Action::QUANTUM_DRIVE);
  qDebug() << "MOMENTUM: " << getActionFlag(Action::MOMENTUM);
  qDebug() << "LIMBERTUDE: " << getActionFlag(Action::LIMBERTUDE);
  qDebug() << "UNBEARABILITY: " << getActionFlag(Action::UNBEARABILITY);
  qDebug() << "---";
  qDebug() << "IG ELMN ATK: " << getIgnoreFlag(Action::IGNORE_ELMN_ATK);
  qDebug() << "IG PHYS ATK: " << getIgnoreFlag(Action::IGNORE_PHYS_ATK);
  qDebug() << "IG THER ATK: " << getIgnoreFlag(Action::IGNORE_THER_ATK);
  qDebug() << "IG POLA ATK: " << getIgnoreFlag(Action::IGNORE_POLA_ATK);
  qDebug() << "IG PRIM ATK: " << getIgnoreFlag(Action::IGNORE_PRIM_ATK);
  qDebug() << "IG CHAR ATK: " << getIgnoreFlag(Action::IGNORE_CHAR_ATK);
  qDebug() << "IG CYBE ATK: " << getIgnoreFlag(Action::IGNORE_CYBE_ATK);
  qDebug() << "IG NIHI ATK: " << getIgnoreFlag(Action::IGNORE_NIHI_ATK);
  qDebug() << "IG ATK: " << getIgnoreFlag(Action::IGNORE_ATK);
  qDebug() << "IGNORE ELMN DEF: " << getIgnoreFlag(Action::IGNORE_ELMN_DEF);
  qDebug() << "IGNORE PHYS DEF: " << getIgnoreFlag(Action::IGNORE_PHYS_DEF);
  qDebug() << "IGNORE THER DEF: " << getIgnoreFlag(Action::IGNORE_THER_DEF);
  qDebug() << "IGNORE POLA DEF: " << getIgnoreFlag(Action::IGNORE_POLA_DEF);
  qDebug() << "IGNORE PRIM DEF: " << getIgnoreFlag(Action::IGNORE_PRIM_DEF);
  qDebug() << "IGNORE CHAR DEF: " << getIgnoreFlag(Action::IGNORE_CHAR_DEF);
  qDebug() << "IGNORE CYBE DEF: " << getIgnoreFlag(Action::IGNORE_CYBE_DEF);
  qDebug() << "IGNORE NIHI DEF: " << getIgnoreFlag(Action::IGNORE_NIHI_DEF);
  qDebug() << "IGNORE DEF: " << getIgnoreFlag(Action::IGNORE_DEF);
}

/*
 * Description: Returns the base change of the action
 *
 * Inputs: None
 * Output: uint - base change value
 */
uint Action::getBaseChange()
{
  return base_change;
}

/*
 * Description: Returns the ID of the action
 *
 * Inputs: None
 * Output: int - Action ID
 */
int Action::getId()
{
  return id;
}

/*
 * Description: Returns the string of the ailment
 *
 * Inputs: None
 * Output: QString - string of the ailment to be set/checked
 */
EnumDb::Infliction Action::getAilment()
{
  return ailment;
}

/*
 * Description: Evaluates an IgnoreAttack flag
 *
 * Inputs: IgnoreAttack flag to be evaluated
 * Output: Evaluation of the flag
 */
bool Action::getIgnoreFlag(IgnoreFlag flags)
{
  return ignore_flags.testFlag(flags);
}

/*
 * Description: Evaluates an ActionType flag
 *
 * Inputs: ActionType flag to be evaluated
 * Output: Evaluation of the flag
 */
bool Action::getActionFlag(ActionType flags)
{
  return action_flags.testFlag(flags);
}

/*
 * Description: Returns the maximum duration of an infliction
 *
 * Inputs: None
 * Output: uint - maximum duration of infliction
 */
uint Action::getMaximum()
{
  return max_duration;
}

/*
 * Description: Returns the minimum duration of an infliction
 *
 * Inputs: None
 * Output: uint - minimum duration of infliction
 */
uint Action::getMinimum()
{
  return min_duration;
}

/*
 * Description: Returns the variance of the action
 *
 * Inputs: None
 * Output: float - variance of the action
 */
double Action::getVariance()
{
  return variance;
}

/*
 * Description: Returns the Infliction of a given string (if one exists)
 *
 * Inputs: QString - name of Infliction to be checked for
 * Output: Infliction - the corresponding Infliction (NOAILMENT for default)
 */
EnumDb::Infliction Action::getInfliction(QString name)
{
  const std::string &ailment_string = name.toUtf8().constData();
  EnumDb::Infliction ailment_type = EnumDb::NOAILMENT;
  EnumString<EnumDb::Infliction>::To(ailment_type, ailment_string);
  return ailment_type;
}
