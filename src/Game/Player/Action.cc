/*******************************************************************************
* Class Name: Action [Implementation]
* Date Created: November 19th, 2013
* Inheritance: None
* Description: An action is an element of a skill used buy a person in a Battle.
*              A skill may have a number of actions which have various effects:
*              altering stats, flipping flags, inflict ailments/buffs. An action
*              is constructed from parsing a string from a file. Actions have
*              unique IDs.
*
* See .h file for detailed explanations.
* See .h file for TODOs
*******************************************************************************/
#include "Game/Player/Action.h"
#include "Game/Player/AttributeSet.h" /* Forward declaration in .h */

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/

const int32_t  Action::kDEFAULT_MIN     = 1;
const int32_t  Action::kDEFAULT_MAX     = 2;
const char     Action::kDELIMITER       = ',';
const char     Action::kDELIMITER_2     = '.';
const int32_t  Action::kMAX_BASE_PC     = 100;
const uint32_t Action::kMAX_VARIANCE_PC = 1000;
const int32_t  Action::kUNSET_ID        = -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a default (invalid) Action object
 *
 * Inputs: none
 */
Action::Action() : id{kUNSET_ID}
{
  clear();
}

/*
 * Description: Constructs a default Action object given a raw string to parse
 *
 * Inputs: std::string raw - the raw string to be parsed
 */
Action::Action(const std::string &raw)
    : Action::Action()
{
  parse(raw);
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Does the check against the indicated ignore flag */
bool Action::atkDefFlag(IgnoreFlags test_flag, bool attack)
{
  if(attack)
    return atkFlag(test_flag);
  return defFlag(test_flag);
}

/*
 * Description: Converts the ignore flag bits to a period delimited
 *              string.
 *
 * Inputs: bool attack - true if attack flag. Else defense
 * Output: std::string - output string
 */
std::string Action::ignoreToStr(bool attack)
{
  std::string output = "";
  bool elemental = false;
  bool all = false;

  /* Space saving setters */
  if(atkDefFlag(IgnoreFlags::THERMAL | IgnoreFlags::POLAR |
                IgnoreFlags::PRIMAL | IgnoreFlags::CHARGED |
                IgnoreFlags::NIHIL | IgnoreFlags::CYBERNETIC, attack))
  {
    if(atkDefFlag(IgnoreFlags::PHYSICAL | IgnoreFlags::LUCK, attack))
               //IgnoreFlags::ARMOR))
    {
      output += Helpers::combine("ALL", kDELIMITER_2);
      all = true;
    }
    else
    {
      output += Helpers::combine("ELEMENTAL", kDELIMITER_2);
    }
    elemental = true;
  }

  /* No need to check if all is set */
  if(!all)
  {
    if(atkDefFlag(IgnoreFlags::PHYSICAL, attack))
      output += Helpers::combine("PHYSICAL", kDELIMITER_2);
    if(atkDefFlag(IgnoreFlags::LUCK, attack))
      output += Helpers::combine("LUCK", kDELIMITER_2);
    if(atkDefFlag(IgnoreFlags::ARMOR, attack))
      output += Helpers::combine("ARMOR", kDELIMITER_2);

    /* No need to check if elemental is set */
    if(!elemental)
    {
      if(atkDefFlag(IgnoreFlags::THERMAL, attack))
        output += Helpers::combine("THERMAL", kDELIMITER_2);
      if(atkDefFlag(IgnoreFlags::POLAR, attack))
        output += Helpers::combine("POLAR", kDELIMITER_2);
      if(atkDefFlag(IgnoreFlags::PRIMAL, attack))
        output += Helpers::combine("PRIMAL", kDELIMITER_2);
      if(atkDefFlag(IgnoreFlags::CHARGED, attack))
        output += Helpers::combine("CHARGED", kDELIMITER_2);
      if(atkDefFlag(IgnoreFlags::NIHIL, attack))
        output += Helpers::combine("NIHIL", kDELIMITER_2);
      if(atkDefFlag(IgnoreFlags::CYBERNETIC, attack))
        output += Helpers::combine("CYBERNETIC", kDELIMITER_2);
    }
  }

  /* Remove the last ., if it exists */
  if(output.size() > 0)
    output.pop_back();

  return output;
}

/*
 * Description: Sub-method for parsing the sub-string containing the infliction
 *              the action may inflict or relieve. Uses string compare to assign
 *              an enumerated value. If the infliction was not found, a warning
 *              is emitted and the parse fails.
 *
 * Inputs: ailm - string containing the ailment inflicted/relieved
 * Output: bool - validity of the ailment parse
 */
bool Action::parseAilment(const std::string &ailm)
{
  if      (ailm == "POISON")      ailment = Infliction::POISON;
  else if (ailm == "BURN")        ailment = Infliction::BURN;
  else if (ailm == "SCALD")       ailment = Infliction::SCALD;
  else if (ailm == "CHARR")       ailment = Infliction::CHARR;
  else if (ailm == "BERSERK")     ailment = Infliction::BERSERK;
  else if (ailm == "CONFUSE")     ailment = Infliction::CONFUSE;
  else if (ailm == "SILENCE")     ailment = Infliction::SILENCE;
  else if (ailm == "BUBBIFY")     ailment = Infliction::BUBBIFY;
  else if (ailm == "DEATHTIMER")  ailment = Infliction::DEATHTIMER;
  else if (ailm == "PARALYSIS")   ailment = Infliction::PARALYSIS;
  else if (ailm == "BLINDNESS")   ailment = Infliction::BLINDNESS;
  else if (ailm == "DREADSTRUCK") ailment = Infliction::DREADSTRUCK;
  else if (ailm == "DREAMSNARE")  ailment = Infliction::DREAMSNARE;
  else if (ailm == "HELLBOUND")   ailment = Infliction::HELLBOUND;
  else if (ailm == "BOND")        ailment = Infliction::BOND;
  else if (ailm == "BONDED")      ailment = Infliction::BONDED;
  else if (ailm == "ALLATKBUFF")  ailment = Infliction::ALLATKBUFF;
  else if (ailm == "ALLDEFBUFF")  ailment = Infliction::ALLDEFBUFF;
  else if (ailm == "PHYBUFF")     ailment = Infliction::PHYBUFF;
  else if (ailm == "THRBUFF")     ailment = Infliction::THRBUFF;
  else if (ailm == "POLBUFF")     ailment = Infliction::POLBUFF;
  else if (ailm == "PRIBUFF")     ailment = Infliction::PRIBUFF;
  else if (ailm == "CHGBUFF")     ailment = Infliction::CHGBUFF;
  else if (ailm == "CYBBUFF")     ailment = Infliction::CYBBUFF;
  else if (ailm == "NIHBUFF")     ailment = Infliction::NIHBUFF;
  else if (ailm == "LIMBUFF")     ailment = Infliction::LIMBUFF;
  else if (ailm == "UNBBUFF")     ailment = Infliction::UNBBUFF;
  else if (ailm == "VITBUFF")     ailment = Infliction::VITBUFF;
  else if (ailm == "QDBUFF")      ailment = Infliction::QDBUFF;
  else if (ailm == "ROOTBOUND")   ailment = Infliction::ROOTBOUND;
  else if (ailm == "DOUBLECAST")  ailment = Infliction::DOUBLECAST;
  else if (ailm == "TRIPLECAST")  ailment = Infliction::TRIPLECAST;
  else if (ailm == "HALFCOST")    ailment = Infliction::HALFCOST;
  else if (ailm == "REFLECT")     ailment = Infliction::REFLECT;
  else if (ailm == "HIBERNATION") ailment = Infliction::HIBERNATION;
  else if (ailm == "CURSE")       ailment = Infliction::CURSE;
  else if (ailm == "METATETHER")  ailment = Infliction::METATETHER;
  else if (ailm == "MODULATE")   ailment = Infliction::MODULATE;

  if (ailment != Infliction::INVALID)
    return true;

  parseWarning("attempting to parse ailment", ailm);
  return false;
}

/*
 * Description: Sub-method for parsing the sub-string containing the action
 *              keyword. Uses a string compare to determine the keyword, else
 *              a warning is emitted and the parse fails.
 *
 * Inputs: action_keyword - string containing the action keyword
 * Output: bool - validity of the keyword parse
 */
bool Action::parseActionKeyword(const std::string &action_keyword)
{
  if (action_keyword == "DAMAGE")
    action_flags |= ActionFlags::DAMAGE;
  else if (action_keyword == "ALTER" || action_keyword == "ALTER-FLIP")
    action_flags |= ActionFlags::ALTER;
  else if (action_keyword == "INFLICT")
    action_flags |= ActionFlags::INFLICT;
  else if (action_keyword == "RELIEVE")
    action_flags |= ActionFlags::RELIEVE;
  else if (action_keyword == "ASSIGN" || action_keyword == "ASSIGN-FLIP")
    action_flags |= ActionFlags::ASSIGN;
  else if (action_keyword == "REVIVE")
    action_flags |= ActionFlags::REVIVE;
  else
  {
    parseWarning("attempting to parse keyword", action_keyword);
    return false;
  }

  if (action_keyword == "ALTER-FLIP" || action_keyword == "ASSIGN-FLIP")
    action_flags |= ActionFlags::FLIP_ATTR;

  return true;
}

/*
 * Description: Sub-method for parsing the sub-string containing the attribute
 *              which the action may affect (via alteration or assigmment).
 *              Determination via string compare, else the method emits a
 *              warning and the parse fails.
 *
 * Inputs: attr_parse - string containing the affected attribute
 *         target     - are we parsing a target attribute?
 * Output: bool - validity o the attribute parse
 */
bool Action::parseAttribute(const std::string &attr_parse, const bool &target)
{
  auto attr = Attribute::NONE;

  if (attr_parse == "VITA")      attr = Attribute::VITA;
  else if (attr_parse == "QTDR") attr = Attribute::QTDR;
  else if (attr_parse == "PHAG") attr = Attribute::PHAG;
  else if (attr_parse == "PHFD") attr = Attribute::PHFD;
  else if (attr_parse == "THAG") attr = Attribute::THAG;
  else if (attr_parse == "THFD") attr = Attribute::THFD;
  else if (attr_parse == "PRAG") attr = Attribute::PRAG;
  else if (attr_parse == "PRFD") attr = Attribute::PRFD;
  else if (attr_parse == "POAG") attr = Attribute::POAG;
  else if (attr_parse == "POFD") attr = Attribute::POFD;
  else if (attr_parse == "CHAG") attr = Attribute::CHAG;
  else if (attr_parse == "CHFD") attr = Attribute::CHFD;
  else if (attr_parse == "CYAG") attr = Attribute::CYAG;
  else if (attr_parse == "CYFD") attr = Attribute::CYFD;
  else if (attr_parse == "NIAG") attr = Attribute::NIAG;
  else if (attr_parse == "NIFD") attr = Attribute::NIFD;
  else if (attr_parse == "MMNT") attr = Attribute::MMNT;
  else if (attr_parse == "UNBR") attr = Attribute::UNBR;
  else if (attr_parse == "MANN") attr = Attribute::MANN;

  if (attr != Attribute::NONE || (!target && attr_parse == ""))
  {
    (target) ? (target_attribute = attr) : (user_attribute = attr);

    return true;
  }

  //parseWarning("attempting to parse attribute", attr_parse);

  return true;
}

void Action::parseChance(const float &parse_chance)
{
  if (parse_chance > 0)
    (parse_chance > 100) ? (chance = 100) : (chance = parse_chance);
  else
    chance = 0;
}

/*
 * Description: Sub-method for parsing the sub-string containing the ignore
 *              flags. Passes in an IgnoreFlags parameter to be able to
 *              use the function for both atk and def flag sets.
 *
 * Inputs: flag_sets - the particular set of flags (atk or def) to assign to
 *         flags - the sub-string containing the elemental ignore flags
 * Output: none
 */
void Action::parseIgnoreFlags(IgnoreFlags& flag_set, const std::string &flags)
{
  std::vector<std::string> sub_strings = Helpers::split(flags, kDELIMITER_2);

  for (std::string s : sub_strings)
  {
    if (s == "ALL" || s == "ELEMENTAL")
    {
      flag_set |= IgnoreFlags::PHYSICAL | IgnoreFlags::THERMAL    |
                  IgnoreFlags::POLAR    | IgnoreFlags::PRIMAL     |
                  IgnoreFlags::CHARGED  | IgnoreFlags::CYBERNETIC |
                  IgnoreFlags::NIHIL    | IgnoreFlags::LUCK;
    }
    if (s == "ELEMENTAL")
    {
      flag_set &= ~IgnoreFlags::PHYSICAL;
      flag_set &= ~IgnoreFlags::LUCK;
    }
    else if (s == "PHYSICAL")
      flag_set |= IgnoreFlags::PHYSICAL;
    else if (s == "THERMAL")
      flag_set |= IgnoreFlags::THERMAL;
    else if (s == "POLAR")
      flag_set |= IgnoreFlags::POLAR;
    else if (s == "PRIMAL")
      flag_set |= IgnoreFlags::PRIMAL;
    else if (s == "CHARGED")
      flag_set |= IgnoreFlags::CHARGED;
    else if (s == "CYBERNETIC")
      flag_set |= IgnoreFlags::CYBERNETIC;
    else if (s == "NIHIL")
      flag_set |= IgnoreFlags::NIHIL;
    else if (s == "LUCK")
      flag_set |= IgnoreFlags::LUCK;
  }

  if (static_cast<int>(ignore_atk) == 0)
    parseWarning("attempting to parse ignore flags", flags);
}

/*
 * Description: Method for printing out a standardized warning emitted by
 *              the parsing functions.  The warning is only emitted if the
 *              debug mode for attribute (constant) is enabled. Also turns off
 *              the flag representing the validity of the action.
 *
 * Inputs: warning - string containing the specific warning text
 *         raw - string containing the raw info dump related to the warning
 * Output: none
 */
void Action::parseWarning(const std::string &warning, const std::string &raw)
{
#ifdef UDEBUG
    std::cerr << "Action Error: " << warning << " on: " << raw << std::endl;
#endif

  action_flags &= ~ActionFlags::VALID;
}

/*
 * Description: Method for assigning the duration the infliction of the Action
 *              may last for given min and max values. Fails if the values are
 *              not valid.
 *
 * Inputs: min_value - minimum # of turns the infliction can last for
 *         max_value - maximum # of turns the infliction can last for
 * Output: bool - the validity of the duration assignment
 */
bool Action::setDuration(const int &min_value, const int &max_value)
{
  if (max_value >= min_value && min_value >= 0 && max_value >= 0)
  {
    min_duration = min_value;
    max_duration = max_value;
    return true;
  }

  min_duration = -1;
  max_duration = -1;

  parseWarning("attempting to parse min, max value", "");
  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Method for printing out all the info describing the state of the
 *              current action in an easy to view format. [std::cout display]
 *
 * Inputs: print_action - whether to print the action flags
 *         print_ignore - whether to print the ignore flags
 * Output: none
 */
void Action::print(const bool &print_action, const bool& print_ignore)
{
  std::cout << "=== Action: " << id << " === ";
  std::cout << "\nUser Attr: " << AttributeSet::getName(user_attribute);
  std::cout << "\nTarg Attr: " << AttributeSet::getName(target_attribute);
  std::cout << "\nAttribute: " << Helpers::ailmentToStr(ailment);
  std::cout << "\nMin Duration: " << min_duration << std::endl;
  std::cout << "Max Duration: " << max_duration << std::endl;
  std::cout << "Base Change: "  << base         << std::endl;
  std::cout << "Variance: "     << variance     << std::endl;
  std::cout << "Chance: " << chance             << std::endl;

  if (print_action)
  {
    std::cout << "--- Action Flags --- " << std::endl;
    std::cout << "DAMAGE: "  << actionFlag(ActionFlags::DAMAGE)  << std::endl;
    std::cout << "ALTER: "   << actionFlag(ActionFlags::ALTER)   << std::endl;
    std::cout << "INFLICT: " << actionFlag(ActionFlags::INFLICT) << std::endl;
    std::cout << "RELIEVE: " << actionFlag(ActionFlags::RELIEVE) << std::endl;
    std::cout << "ASSIGN: "  << actionFlag(ActionFlags::ASSIGN)  << std::endl;
    std::cout << "REVIVE: "  << actionFlag(ActionFlags::REVIVE)  << std::endl;
    std::cout << "BASE_PC: " << actionFlag(ActionFlags::BASE_PC) << std::endl;
    std::cout << "VARI_PC: " << actionFlag(ActionFlags::VARI_PC) << std::endl;
    std::cout << "FLIP_ATTR: " << actionFlag(ActionFlags::FLIP_ATTR);
    std::cout << "\nVALID: "   << actionFlag(ActionFlags::VALID)   << std::endl;
  }

  if (print_ignore)
  {
    std::cout << "--- Igore Atk Flags --- " << std::endl;
    std::cout << "PHYS: " << atkFlag(IgnoreFlags::PHYSICAL)   << std::endl;
    std::cout << "THER: " << atkFlag(IgnoreFlags::THERMAL)    << std::endl;
    std::cout << "POLA: " << atkFlag(IgnoreFlags::POLAR)      << std::endl;
    std::cout << "PRIM: " << atkFlag(IgnoreFlags::PRIMAL)     << std::endl;
    std::cout << "CHAR: " << atkFlag(IgnoreFlags::CHARGED)    << std::endl;
    std::cout << "CYBR: " << atkFlag(IgnoreFlags::CYBERNETIC) << std::endl;
    std::cout << "NIHI: " << atkFlag(IgnoreFlags::NIHIL)      << std::endl;

    std::cout << "--- Igore Def Flags --- " << std::endl;
    std::cout << "PHYS: " << defFlag(IgnoreFlags::PHYSICAL)   << std::endl;
    std::cout << "THER: " << defFlag(IgnoreFlags::THERMAL)    << std::endl;
    std::cout << "POLA: " << defFlag(IgnoreFlags::POLAR)      << std::endl;
    std::cout << "PRIM: " << defFlag(IgnoreFlags::PRIMAL)     << std::endl;
    std::cout << "CHAR: " << defFlag(IgnoreFlags::CHARGED)    << std::endl;
    std::cout << "CYBR: " << defFlag(IgnoreFlags::CYBERNETIC) << std::endl;
    std::cout << "NIHI: " << defFlag(IgnoreFlags::NIHIL)      << std::endl;
  }

  std::cout << "=== / Action === " << std::endl;
}

/*
 * Description: Returns the evaluation of a given ActionFlag flag or set of
 *              flags by comparing them after a bit-wise and to the current set.
 *
 * Inputs: none
 * Output: bool - evaluation of the given flag
 */
bool Action::actionFlag(ActionFlags test_flag)
{
  return ((action_flags & test_flag) == test_flag);
}

/*
 * Description: Returns the evaluation of a given ignore atk flag or set of
 *              flags by comparing them after a bit-wise and to the current set.
 *
 * Inputs: none
 * Output: bool - evaluation of the given flag
 */
bool Action::atkFlag(IgnoreFlags test_flag)
{
  return ((ignore_atk & test_flag) == test_flag);
}

/*
 * Description: Returns the evaluation of a given ignore def flag or set of
 *              flags by comparing them after a bit-wise and to the current set.
 *
 * Inputs: none
 * Output: bool - evaluation of the givenflag
 */
bool Action::defFlag(IgnoreFlags test_flag)
{
  return ((ignore_def & test_flag) == test_flag);
}

/* Clears the class info */
void Action::clear()
{
  action_flags = static_cast<ActionFlags>(0);
  ailment = Infliction::INVALID;
  base = 0;
  chance = 0;
  ignore_atk = static_cast<IgnoreFlags>(0);
  ignore_def = static_cast<IgnoreFlags>(0);
  min_duration = 0;
  max_duration = 0;
  target_attribute = Attribute::NONE;
  user_attribute = Attribute::NONE;
  variance = 0;
}

/*
 * Description: Returns the enumerated value of the attribute the action affects
 *
 * Inputs: none
 * Output: Attribute - enumerated atribute the action affects
 */
Attribute Action::getUserAttribute() const
{
  return user_attribute;
}

Attribute Action::getTargetAttribute() const
{
  return target_attribute;
}

/*
 * Description: Returns the enumerated infliction the action affects
 *
 * Inputs: none
 * Output: Infliction - enumerated attribute the action affects
 */
Infliction Action::getAilment() const
{
  return ailment;
}

/*
 * Description: Returns the base value by which the Action alters an attribute
 *
 * Inputs: none
 * Output: int - the base value of the Action
 */
int Action::getBase() const
{
  return base;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float Action::getChance() const
{
  return chance;
}

/*
 * Description: Returns the ID of the Action
 *
 * Inputs: none
 * Output: int - the ID of the Action
 */
int Action::getID() const
{
  return id;
}

/*
 * Description: Returns the minimum duration of the Action
 *
 * Inputs: none
 * Output: int - the minimum duration of the Action
 */
int Action::getMin() const
{
  return min_duration;
}

/*
 * Description: Returns the maximum duration of the Action
 *
 * Inputs: none
 * Output: int - the maximum duration of the Action
 */
int Action::getMax() const
{
  return max_duration;
}

/*
 * Description: Returns the variance of the Action
 *
 * Inputs: none
 * Output: uint32_t - the variance on the Base value
 */
uint32_t Action::getVariance() const
{
  return variance;
}

/*
 * Description: Generates the output string that matches the file storage
 *              of the action.
 *
 * Inputs: none
 * Output: std::string - the output string
 */
std::string Action::outputString()
{
  bool failed = false;
  std::string output = "";

  /* Pre-checks */
  if(id >= 0)
  {
    /* Add the ID */
    output += std::to_string(id) + kDELIMITER;

    /* Add the action keyword */
    if(actionFlag(ActionFlags::DAMAGE))
      output += "DAMAGE";
    else if(actionFlag(ActionFlags::ALTER))
    {
      output += "ALTER";
      if(actionFlag(ActionFlags::FLIP_ATTR))
        output += "-FLIP";
    }
    else if(actionFlag(ActionFlags::INFLICT))
      output += "INFLICT";
    else if(actionFlag(ActionFlags::RELIEVE))
      output += "RELIEVE";
    else if(actionFlag(ActionFlags::ASSIGN))
    {
      output += "ASSIGN";
      if(actionFlag(ActionFlags::FLIP_ATTR))
        output += "-FLIP";
    }
    else if(actionFlag(ActionFlags::REVIVE))
      output += "REVIVE";
    else
      failed = true;
    output += kDELIMITER;

    /* If it's an INFLICT, add the min and max turn values */
    if(actionFlag(ActionFlags::INFLICT))
    {
      output += std::to_string(getMin()) + kDELIMITER_2;
      output += std::to_string(getMax());
    }
    output += kDELIMITER;

    /* Loop through the attack elements that are protected from */
    if(actionFlag(ActionFlags::DAMAGE))
      output += ignoreToStr(true);
    output += kDELIMITER;
    if(actionFlag(ActionFlags::DAMAGE))
      output += ignoreToStr(false);
    output += kDELIMITER;

    /* Check the user attribute / ailment */
    if(actionFlag(ActionFlags::ASSIGN) || actionFlag(ActionFlags::ALTER))
      output += Helpers::attributeToStr(user_attribute);
    else if(actionFlag(ActionFlags::INFLICT) ||
            actionFlag(ActionFlags::RELIEVE))
    {
      output += Helpers::ailmentToStr(ailment);
    }
    output += kDELIMITER;

    /* Add the base damage / healing */
    if(!(actionFlag(ActionFlags::INFLICT) || actionFlag(ActionFlags::RELIEVE)))
    {
      if(actionFlag(ActionFlags::BASE_PC))
        output += "PC";
      else
        output += "AMOUNT";
      output += kDELIMITER_2;

      output += std::to_string(base);
    }
    else if(actionFlag(ActionFlags::INFLICT))
    {
      output += "AMOUNT";
      output += kDELIMITER_2;
      output += std::to_string(base);
    }
    output += kDELIMITER;

    /* Add the variance to the base amount */
    if(!(actionFlag(ActionFlags::INFLICT) || actionFlag(ActionFlags::RELIEVE)))
    {
      if(actionFlag(ActionFlags::VARI_PC))
        output += "PC";
      else
        output += "AMOUNT";
      output += kDELIMITER_2;

      output += std::to_string(variance);
    }
    output += kDELIMITER;

    /* Check the target attribute / ailment */
    if(actionFlag(ActionFlags::ASSIGN) || actionFlag(ActionFlags::ALTER))
      output += Helpers::attributeToStr(target_attribute);
    output += kDELIMITER;

    /* Add the percent chance at the end */
    output += std::to_string(chance);

    /* Proceed to return output, if successful */
    if(!failed)
      return output;
  }
  return "";
}

/*
 * Description: Primary method for parsing the raw string. Divides the string up
 *              into workable sections, calling sub-methods on the sub-string
 *              portions--checking for errors and along the way.
 *
 * Inputs: raw - the raw string to be parsed into an action
 * Output: bool - the final validity of the Action
 */
bool Action::parse(const std::string &raw)
{
  /* Clear existing info */
  clear();

  action_flags |= ActionFlags::VALID;
  std::vector<std::string> sub_strings = Helpers::split(raw, kDELIMITER);

  if (sub_strings.size() == 9 || sub_strings.size() == 10)
  {
  	/* Parse the ID */
    id = std::stoi(sub_strings.at(0));

  	/* Parse the initial action keyword */
    parseActionKeyword(sub_strings.at(1));

    /* Parse min & max durations */
    if (sub_strings.at(2) != "")
    {
      std::vector<std::string> turns =
          Helpers::split(sub_strings.at(2), kDELIMITER_2);

      if (turns.size() == 2)
        setDuration(std::stoi(turns.at(0)), std::stoi(turns.at(1)));
      else
        parseWarning("invalid duration size", raw);
    }
    else if (sub_strings.at(2) == "")
      if (actionFlag(ActionFlags::INFLICT) || actionFlag(ActionFlags::RELIEVE))
        setDuration(kDEFAULT_MIN, kDEFAULT_MAX);

    /* Parse ignore_atk flags */
    if (sub_strings.at(3) != "")
      parseIgnoreFlags(ignore_atk, sub_strings.at(3));

    /* Parse ignore def flags */
    if (sub_strings.at(4) != "")
      parseIgnoreFlags(ignore_def, sub_strings.at(4));

    /* Parse User attribute -- ALTER/ASSIGN keywords relate to it */
    if (actionFlag(ActionFlags::DAMAGE)  || actionFlag(ActionFlags::ALTER) ||
        actionFlag(ActionFlags::ASSIGN))
    {
      parseAttribute(sub_strings.at(5), false);
    }

    /* INFLICT and RELIEVE keywords relate to ailments */
    else if(actionFlag(ActionFlags::INFLICT)||actionFlag(ActionFlags::RELIEVE))
      parseAilment(sub_strings.at(5));

    /* Parse base change */
    if (sub_strings.at(6) != "")
    {
      std::vector<std::string> base_values =
          Helpers::split(sub_strings.at(6), kDELIMITER_2);

      if (base_values.size() == 2)
      {
        if (base_values.at(0) == "PC")
          action_flags |= ActionFlags::BASE_PC;
        else if (base_values.at(0) != "AMOUNT")
          parseWarning("invalid base keyword", raw);

        base = std::stoi(base_values.at(1));
      }
      else
        parseWarning("wrong # arguments in base parse", raw);
    }

    /* Parse variance [check if size is right - no empty end on split() ] */
    if (sub_strings.at(7) != "")
    {
      std::vector<std::string> variance_values =
          Helpers::split(sub_strings.at(7), kDELIMITER_2);

      if (variance_values.size() == 2)
      {
        if (variance_values.at(0) == "PC")
          action_flags |= ActionFlags::VARI_PC;
        else if (variance_values.at(0) != "AMOUNT")
          parseWarning("invalid variance keyword", raw);

        variance = std::stoi(variance_values.at(1));
      }
      else
        parseWarning("wrong # arguments in variance parse", raw);
    }

    /* Warning Checking */
    if (actionFlag(ActionFlags::BASE_PC) && base > kMAX_BASE_PC)
      parseWarning("base percent value higher than permitted", raw);

    if (actionFlag(ActionFlags::VARI_PC) && variance > kMAX_VARIANCE_PC)
      parseWarning("variance percent value higher than permitted", raw);

    /* Parse TARGET attribute -- ALTER/ASSIGN keywords relate to it */
    if (actionFlag(ActionFlags::DAMAGE) || actionFlag(ActionFlags::ALTER) ||
        actionFlag(ActionFlags::ASSIGN))
    {
      parseAttribute(sub_strings.at(8), true);
    }

    /* Parse the chance occuring of the action */
    if (sub_strings.size() == 10 && sub_strings.at(9) != "")
    {
      auto parse_chance = std::stof(sub_strings.at(9));
      parseChance(parse_chance);
    }
    else
      parseWarning("attempting to parse action chance", raw);
  }
  else
    parseWarning("invalid sub string size", raw);

  return actionFlag(ActionFlags::VALID);
}

/*
 * Description: Sets an action flag to a true or false value
 *
 * Inputs: ActionFlags set_flag - the flag to set
 *         bool set - true or false set value
 * Output: none
 */
void Action::setActionFlag(ActionFlags set_flag, bool set)
{
  set ? (action_flags |= set_flag) : (action_flags &= ~set_flag);

  /* If it's a percent change, fix the value */
  if(actionFlag(ActionFlags::BASE_PC))
    setBaseValue(base, true);
  if(actionFlag(ActionFlags::VARI_PC))
    setBaseVariance(variance, true);
}

/*
 * Description: Sets the infliction ailment for the action
 *
 * Inputs: Infliction ailment - the infliction to set
 * Output: none
 */
void Action::setAilment(Infliction ailment)
{
  this->ailment = ailment;
}

/*
 * Description: Sets the ailment duration minimum and maximum ranges for turn
 *              counts.
 *
 * Inputs: int min - the minimum number of turns
 *         int max - the maximum number of turns
 * Output: bool - true if its set. Min/Max greater than 0 and min >= max
 */
bool Action::setAilmentDuration(int min, int max)
{
  if(min > 0 && max > 0 && max >= min)
  {
    min_duration = min;
    max_duration = max;
    return true;
  }
  return false;
}

/*
 * Description: Sets the attribute that is affected of the target.
 *
 * Inputs: Attribute target - the attribute of the target
 * Output: none
 */
void Action::setAttributeTarget(Attribute target)
{
  target_attribute = target;
}

/*
 * Description: Sets the attribute that is affected of the user.
 *
 * Inputs: Attribute user - the attribute of the user
 * Output: none
 */
void Action::setAttributeUser(Attribute user)
{
  user_attribute = user;
}

/*
 * Description: Sets the base value that is modified of the attribute. It can
 *              be a percentage or a constant.
 *
 * Inputs: int32_t value - the modification value
 *         bool percent - is the value a percentage?
 * Output: none
 */
void Action::setBaseValue(int32_t value, bool percent)
{
  if(percent)
  {
    if(value > kMAX_BASE_PC)
      base = kMAX_BASE_PC;
    else
      base = value;

    action_flags |= ActionFlags::BASE_PC;
  }
  else
  {
    base = value;
    action_flags &= ~ActionFlags::BASE_PC;
  }
}

/*
 * Description: Sets the base variance that modifies the base value based on
 *              random chance. It can be a percentage or a constant.
 *
 * Inputs: uint32_t variance - the variance amount
 *         bool percent - is the value a percentage?
 * Output: none
 */
void Action::setBaseVariance(uint32_t variance, bool percent)
{
  if(percent)
  {
    if(variance > kMAX_VARIANCE_PC)
      this->variance = kMAX_VARIANCE_PC;
    else
      this->variance = variance;

    action_flags |= ActionFlags::VARI_PC;
  }
  else
  {
    this->variance = variance;
    action_flags &= ~ActionFlags::VARI_PC;
  }
}

/*
 * Description: Sets the chance the action is executed.
 *
 * Inputs: float chance - the percent chance of success
 * Output: bool - true if percent chance is between 0 and max (100)
 */
bool Action::setChance(float chance)
{
  if(chance >= 0.0 && chance <= kMAX_BASE_PC)
  {
    this->chance = chance;
    return true;
  }
  return false;
}

/*
 * Description: Sets the ID of the action. If out of range, it's set to invalid
 *              value.
 *
 * Inputs: int32_t id - the id value
 * Output: none
 */
void Action::setID(int32_t id)
{
  if(id < 0)
    this->id = kUNSET_ID;
  else
    this->id = id;
}

/*
 * Description: Sets the ignore attack element flag (true or false)
 *
 * Inputs: IgnoreFlags flag - the flag to set
 *         bool set - the value to set the flag to
 * Output: none
 */
void Action::setIgnoreAttack(IgnoreFlags flag, bool set)
{
  set ? (ignore_atk |= flag) : (ignore_atk &= ~flag);
}

/*
 * Description: Sets the ignore defense element flag (true or false)
 *
 * Inputs: IgnoreFlags flag - the flag to set
 *         bool set - the value to set the flag to
 * Output: none
 */
void Action::setIgnoreDefense(IgnoreFlags flag, bool set)
{
  set ? (ignore_def |= flag) : (ignore_def &= ~flag);
}
