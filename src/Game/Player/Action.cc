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
Action::Action()
    : action_flags{static_cast<ActionFlags>(0)}
    , target_attribute{Attribute::NONE}
    , user_attribute{Attribute::NONE}
    , ailment{Infliction::INVALID}
    , base{0}
    , chance{0}
    , id{kUNSET_ID}
    , ignore_atk{static_cast<IgnoreFlags>(0)}
    , ignore_def{static_cast<IgnoreFlags>(0)}
    , min_duration(0)
    , max_duration(0)
    , variance(0)
{}

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

    /* Parse Target attribute -- ALTER/ASSIGN keywords relate to it */
    if (actionFlag(ActionFlags::DAMAGE)  || actionFlag(ActionFlags::ALTER) || 
        actionFlag(ActionFlags::ASSIGN))
    {
      parseAttribute(sub_strings.at(5), true);
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

    /* Parse USER attribute -- ALTER/ASSIGN keywords relate to it */
    if (actionFlag(ActionFlags::DAMAGE) || actionFlag(ActionFlags::ALTER) || 
        actionFlag(ActionFlags::ASSIGN))
    {
      parseAttribute(sub_strings.at(8), false);
    }

    /* Parse the chance occuring of the action */
    if (sub_strings.size() == 10 && sub_strings.at(9) != "")
    {
      auto parse_chance = std::stoi(sub_strings.at(9));
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
  else if (ailm == "MOMBUFF")     ailment = Infliction::MOMBUFF; 
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
  else if (ailm == "STUBULATE")   ailment = Infliction::STUBULATE;

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
  else if (action_keyword == "ALTER")
    action_flags |= ActionFlags::ALTER;
  else if (action_keyword == "INFLICT")
    action_flags |= ActionFlags::INFLICT;
  else if (action_keyword == "RELIEVE")
    action_flags |= ActionFlags::RELIEVE;
  else if (action_keyword == "ASSIGN")
    action_flags |= ActionFlags::ASSIGN;
  else if (action_keyword == "REVIVE")
    action_flags |= ActionFlags::REVIVE;
  else
  {
    parseWarning("attempting to parse keyword", action_keyword);
    return false;
  }

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

  parseWarning("attempting to parse attribute", attr_parse);
  return false;
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
      flag_set &= ~IgnoreFlags::PHYSICAL;
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
    std::cout << "VALID: "   << actionFlag(ActionFlags::VALID)   << std::endl;
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
  return static_cast<bool>((action_flags & test_flag) == test_flag);
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
  return static_cast<bool>((ignore_atk & test_flag) == test_flag);
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
  return static_cast<bool>((ignore_def & test_flag) == test_flag);
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
