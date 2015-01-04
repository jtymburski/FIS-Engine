/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: February 23rd, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/
#include "Game/Battle/Battle.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/* ------------ Menu Constants --------------- */
const uint16_t Battle::kGENERAL_UPKEEP_DELAY = 500; /* Time prior info bar */
const uint16_t Battle::kBATTLE_MENU_DELAY    = 400; /* Personal menu delay */

/* ------------ Battle Damage Calculation Modifiers ---------------
 *
 * Maximum Ailments (Total)
 * Maximum Each Ailments (Per Person)
 * Minimum Damage (Possible)
 * Maximum Damage (Possible)
 *
 * Offensive Physical Modifier
 * Defensive Physical Modifier
 * Offensive Primary Elemental Match Modifier
 * Defensive Primary Elemental Match Modifier
 * Offensive Secondary Elemental Match Modifier
 * Defensive Secondary Elemental Match Modifier
 *
 * Offensive Primary Element Modifier
 * Defensive Primary Element Modifier
 * Offensive Secondary Element Modifier
 * Defensive Secondary Element Modifier
 *
 * Primary Elemental Advantage Modifier
 * Primary Elemental Disadvantage Modifier
 * Secondary Elemental Advantage Modifier
 * Secondary Elemental Disadvantage Modifier
 * Double Elemental Advantage Modifier
 * Double Elemental Disadvantage Modifier
 *
 * Manna Power Modifier
 * Manna Defense Modifier
 * User Power Modifier
 * Target Defense Modifier
 * 
 * Base Critical Hit Chance
 * Offensive Critical Factor
 * Critical Modifier (Base)
 * Critical Lvl Modifier (Per Level)
 * Critical Defending Modifier (While User Defending)
 * Crtical Guarded Modifier (While User Being Guarded)
 * Critical Shielded Modifier (While User Being Shielded)
 * 
 * Base Run Chance (in %)
 * User Run Modifier (Modify user value per point of momentum)
 * Ally Run Modifier (Modify user value per point of momentum)
 * Enemy Run Modifier (Modify user value per point of momentum)
 * Run PC Per Poitn (% to alter run chance by per point of momentum)
 *
 * Dodge Chance [Limbertude] Modifier
 * Dodge Chance [Limbertude] Per Level Modifier
 *
 * Defend Modifier (Base Damage Mod While Defending)
 * Guard Modifier (Base Damage Mod While Being Guarded)
 */
const size_t   Battle::kMAX_AILMENTS                =    50;
const size_t   Battle::kMAX_EACH_AILMENTS           =     5;
const uint16_t Battle::kMINIMUM_DAMAGE              =     1;
const uint16_t Battle::kMAXIMUM_DAMAGE              = 29999;
 
const float    Battle::kOFF_PHYS_MODIFIER           = 1.00;
const float    Battle::kDEF_PHYS_MODIFIER           = 1.10;
const float    Battle::kOFF_PRIM_ELM_MATCH_MODIFIER = 1.05;
const float    Battle::kDEF_PRIM_ELM_MATCH_MODIFIER = 1.04;
const float    Battle::kOFF_SECD_ELM_MATCH_MODIFIER = 1.03;
const float    Battle::kDEF_SECD_ELM_MATCH_MODIFIER = 1.02;

const float    Battle::kOFF_PRIM_ELM_MODIFIER       =  1.00;
const float    Battle::kDEF_PRIM_ELM_MODIFIER       =  1.00;
const float    Battle::kOFF_SECD_ELM_MODIFIER       =  1.00;
const float    Battle::kDEF_SECD_ELM_MODIFIER       =  1.00;

const float    Battle::kPRIM_ELM_ADV_MODIFIER       =  1.15;
const float    Battle::kPRIM_ELM_DIS_MODIFIER       =  0.87;
const float    Battle::kSECD_ELM_ADV_MODIFIER       =  1.10;
const float    Battle::kSECD_ELM_DIS_MODIFIER       =  0.93;
const float    Battle::kDOUBLE_ELM_ADV_MODIFIER     =  1.30;
const float    Battle::kDOUBLE_ELM_DIS_MODIFIER     =  0.74;

const float    Battle::kMANNA_POW_MODIFIER          =  1.00;
const float    Battle::kMANNA_DEF_MODIFIER          =  1.20;
const float    Battle::kUSER_POW_MODIFIER           =  3.00;
const float    Battle::kTARG_DEF_MODIFIER           =  2.90;

const float    Battle::kBASE_CRIT_CHANCE            =   0.10;
const float    Battle::kOFF_CRIT_FACTOR             =   1.25;
const float    Battle::kCRIT_MODIFIER               =   0.0008;
const float    Battle::kCRIT_LVL_MODIFIER           =   0.012;
const float    Battle::kCRIT_DEFENDING_MODIFIER     =   0.70;
const float    Battle::kCRIT_GUARDED_MODIFIER       =   0.65;

const float    Battle::kBASE_RUN_CHANCE             =   0.25;
const float    Battle::kUSER_RUN_MODIFIER           =   2.00;
const float    Battle::kALLY_RUN_MODIFIER           =   1.00;
const float    Battle::kENEMY_RUN_MODIFIER          =   1.00;
const float    Battle::kRUN_PC_PER_POINT            =   0.003;
const int16_t  Battle::kRUN_PC_EXP_PENALTY          =   5;

const float    Battle::kDODGE_MODIFIER              =   0.10;
const float    Battle::kDODGE_HIGHEST_RATE_PC       =   50.0;
const float    Battle::kDODGE_PER_LEVEL_MODIFIER    =   2.50;

const float    Battle::kDEFEND_MODIFIER             =   0.50;
const float    Battle::kGUARD_MODIFIER              =   1.10;
const float    Battle::kSHIELDED_MODIFIER           =   0.00;

const int16_t  Battle::kREGEN_RATE_ZERO_PC           =      0;
const int16_t  Battle::kREGEN_RATE_WEAK_PC           =      2; 
const int16_t  Battle::kREGEN_RATE_NORMAL_PC         =      4;
const int16_t  Battle::kREGEN_RATE_STRONG_PC         =      6;
const int16_t  Battle::kREGEN_RATE_GRAND_PC          =      8;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a Battle, given a pointer to Options and two Parties.
 *
 * Inputs: Options* running_config - running config assignment
 *         Party* const friends - pointer to friends party
 *         Party* const foes - pointer to foes party
 */
Battle::Battle(Options* running_config, Party* const friends, Party* const foes,
    SkillSet* const bubbified_skills)
      : bubbified_skills{bubbified_skills}
      , friends{friends}
      , foes{foes}
{
  if (!checkAIModules())
    std::cerr << "[Error] Crtical Error: AI Moudles for enemies bad!\n";
  if (friends == nullptr || foes == nullptr || bubbified_skills == nullptr)
    std::cerr << "[Error] Critical error: Invalid battle initialization!\n";

  setupClass();
  determineTurnMode();
  loadBattleStateFlags();
  setConfiguration(running_config);

  action_buffer = new Buffer();
  menu          = new BattleMenu(running_config);

  setBattleFlag(CombatState::PHASE_DONE, true);
}

/*
 * Description: Annihilates the Battle.
 */
Battle::~Battle()
{
  for (auto& ailment : ailments)
    delete ailment;
  ailments.clear();

  if (menu != nullptr)
    delete menu;
  menu = nullptr;

  if (action_buffer != nullptr)
    delete action_buffer;
  action_buffer = nullptr;

  clearEvents();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add an ailment to the vector of ailments
 *
 * Inputs: Ailment* const new_ailment - pointer to a new ailment object.
 * Output: bool true if the ailment infliction was kosher
 */
bool Battle::addAilment(Infliction infliction_type, Person* inflictor,
                        uint16_t min_turns, uint16_t max_turns, int32_t chance)
{
  auto success     = false;
  auto new_ailment = new Ailment(curr_target, infliction_type, inflictor,
                         min_turns, max_turns, static_cast<float>(chance));

  if (new_ailment->getFlag(AilState::INFLICTOR_SET) && 
      new_ailment->getFlag(AilState::VICTIM_SET))
  {
    ailments.push_back(new_ailment);

    if (new_ailment->getFlag(AilState::TO_APPLY))
      new_ailment->apply();

    success = true;
  }
  else
  {
    delete new_ailment;
    new_ailment = nullptr;
    success     = false;
  }

  if (success && getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{INFLICTION} " << curr_target->getName() << " has been "
              << "inflicted with " << Helpers::ailmentToStr(infliction_type)
              << " lasting " << min_turns << " to " << max_turns << ".\n";
  }

  return success;
}

/*
 * Description:
 *
 * Inputs: 
 * Output:
 */
void Battle::clearEvents()
{
  std::cout << "--- Clear Events ---" << std::endl;
  for (auto& event : curr_events)
  {
    delete event;
    event = nullptr;
  }

  curr_events.clear();
}

/*
 * Description:
 *
 * Inputs: 
 * Output:
 */
BattleEvent* Battle::createEvent(BattleEventType type, Person* user, 
    Person* target)
{
  std::cout << "Creating event!" << std::endl;
  BattleEvent* new_event = new BattleEvent();
  new_event->type = type;
  new_event->user = user;
  std::vector<Person*> target_vector{target};
  new_event->targets = target_vector;

  return new_event;
}

/*
 * Description:
 *
 * Inputs: 
 * Output:
 */
BattleEvent* Battle::createEvent(BattleEventType type, Person* user,
    std::vector<Person*> targets)
{
  std::cout << "Creating event!" << std::endl;
  BattleEvent* new_event = new BattleEvent();
  new_event->type = type;
  new_event->user = user;
  new_event->targets = targets;

  return new_event;
}

/*
 * Description:
 *
 * Inputs: 
 * Output:
 */
BattleEvent* Battle::createEvent(BattleEventType type, Skill* skill_use,
    bool happens)
{
  std::cout << "Creating event!" << std::endl;
  BattleEvent* new_event = new BattleEvent();
  new_event->type = type;
  new_event->skill_use = skill_use;
  new_event->happens = happens;

  return new_event;
}

/*
 * Description:
 *
 * Inputs: 
 * Output:
 */
bool Battle::reCalcAilmentFlags(Person* target, Ailment* ail)
{
  if (target != nullptr &&  ail != nullptr && ail->getVictim() == target)
  {
    /* Unapply the ailment (by the default effect) set to be removed */
    if (ail->getFlag(AilState::TO_UNAPPLY))
      ail->unapply();

    /* For all other ailments inflicted upon the given target, iterate through
     * and recalculate the necessary flag and stats values */
    auto person_ailments = getPersonAilments(target);
    std::vector<std::tuple<Ailment*, bool>> reapply_values;

    for (auto ill : person_ailments)
    {
      if (ill != ail)
      {
        auto new_tuple = std::make_tuple(ill, ill->toReapplyFlags());
        reapply_values.push_back(new_tuple);
      }
    }

    /* Find a base flag state */
    target->resetActionFlags();

    /* Reapply each ailment needed */
    for (auto& ill_tuple : reapply_values)
    {
      if (std::get<0>(ill_tuple) != nullptr && std::get<1>(ill_tuple))
      {
        std::get<0>(ill_tuple)->setFlag(AilState::TO_APPLY);
        std::get<0>(ill_tuple)->update(false);
      }
    }

    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs: 
 * Output:
 */
bool Battle::removeAilment(Ailment* remove_ailment)
{
  auto found   = false;
  auto success = false;

  for (auto& ailment : ailments)
  {
    if (ailment == remove_ailment)
    {
      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{RELIEVING} -- The ailment on " << curr_target->getName() 
            << " called " << Helpers::ailmentToStr(remove_ailment->getType()) 
            << " is being relieved." << std::endl;
      }

      /* Some ailments will alter flags that need to be recalculated */
      success = reCalcAilmentFlags(curr_target, ailment);

      found = true;
      delete ailment;
      ailment = nullptr;
    }
  }

  ailments.erase(std::remove(begin(ailments), end(ailments), nullptr), 
      end(ailments));

  reCalcAilments(curr_target);

  return found && success;
}

/*
 * Description: Called when the Battle has been lost. Battle lost generally 
 *              results in a return to title (event handler?)
 *
 * Inputs: none
 * Output: none
 * //TODO [08-24-14]: Finish battle lost functionality
 */
void Battle::battleLost()
{
  if (getBattleMode() == BattleMode::TEXT)
  {
    printPartyState();
    std::cout << "Battle lost! :-(\n";  
  }

  setBattleFlag(CombatState::OUTCOME_DONE);
  setNextTurnState();

  //TODO: [11-14-14] Return to title after battle lost.
}

/*
 * Description: Called when the Battle is being run from. Runners from the
 *              battle will occur a penalty against their experience towards
 *              the next level. (This does not matter for enemies)
 *
 * Inputs: none
 * Output: none
 * //TODO [08-24-14]: Finish battle run functionality
 */
void Battle::battleRun()
{
  if (getBattleFlag(CombatState::ALLIES_RUN))
  {
    /* For each person on the friends team, incur a % penalty against the
     * experience to the next level */
    for (uint32_t i = 0; i < friends->getSize(); i++)
    {
      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << friends->getMember(i)->getName() << " has lost " 
                  << kRUN_PC_EXP_PENALTY << "% EXP towards next level.\n";
      }

      friends->getMember(i)->loseExpPercent(kRUN_PC_EXP_PENALTY);
      // TODO [11-06-14] Update personal record run from battle count
    }

  if (getBattleMode() == BattleMode::TEXT)
    std::cout << "\n{ALLIES RUN} The allied team has ran from Battle! :-/\n";

  }
  else if (getBattleFlag(CombatState::ENEMIES_RUN))
  {
    if (getBattleMode() == BattleMode::TEXT)
      std::cout << "{ENEMIES RUN} The foes team has ran from Battle! :-/\n";
  }

  setBattleFlag(CombatState::OUTCOME_DONE);
  setNextTurnState();
}

/*
 * Description: Called when the Battle has been won
 *
 * Inputs: none
 * Output: none
 * //TODO: [08-24-14] Finish victory functions
 */
void Battle::battleWon()
{
  //call victory
    // for each person on friends
      // increase exp by enemyTotalExp
      // levelUp() --> show update info
      // for each equipment
        // for each bubby
          // increase exp by enemyTotalExp
          // levelUp() --> show update info
   // process loot
     // money
     // items

  if (getBattleMode() == BattleMode::TEXT)
  {
    printPartyState();
    std::cout << "Battle victorious! :-) " << std::endl;
  }

  setBattleFlag(CombatState::OUTCOME_DONE);
  setNextTurnState();
}

/*
 * Description: Buffers an enemy action based on the current Person Index.
 *              This function will use the AI Module of the Person Index to
 *              get the enemy to select an action. From the AI Module, this
 *              function will grab the necessary information and compile it
 *              into a proper Buffer addition and determine whether the
 *              Enemy should select another action, or to increment the 
 *              person index, or assign the Buffer Enemy Actions phase complete.
 *
 * Inputs: none
 * Output: bool - true if an action was successfully buffered
 */
bool Battle::bufferEnemyAction()
{
  auto buffered      = false;

  curr_user = getPerson(person_index);

  auto action_type    = curr_module->getActionType();
  auto action_targets = curr_module->getChosenTargets();

  if (action_type == ActionType::SKILL)
  {
    curr_skill = curr_module->getSelectedSkill();
    buffered = action_buffer->add(curr_user, curr_skill, action_targets, 0);

    /* Pay the required QTDR cost for the Skill */
    auto true_cost = curr_user->getTrueCost(curr_skill);
    curr_user->getCurr().alterStat(Attribute::QTDR, -true_cost);
  }
  else if (action_type == ActionType::ITEM)
  {
    curr_item = curr_module->getSelectedItem();
    buffered = action_buffer->add(curr_user, curr_item, action_targets, 0);

    if (buffered)
    {
      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "Removing " << curr_item->getName() << " from "
                  << "inventory and implementing to buffer." << std::endl;
      }

      /* Remove the item from the inventory, update module with current items */
      foes->getInventory()->removeItemID(curr_item->getGameID());

      /* Grab the vector of pairs of item/amts and build battl item vector
       * and inject back into the AI Module for selection */
      auto items = foes->getInventory()->getBattleItems();
      curr_module->setItems(buildBattleItems(person_index, items));
    }
  }
  else if (action_type == ActionType::DEFEND  ||
           action_type == ActionType::GUARD   ||
           action_type == ActionType::IMPLODE ||
           action_type == ActionType::RUN     ||
           action_type == ActionType::PASS)
  {
    buffered = action_buffer->add(curr_user, action_type, action_targets, 0);
  }
  else
  {
    std::cerr << "[Error]: Enemy action selection has invalid action type\n";
  }

  if (buffered)
  {
    if (curr_user->getBFlag(BState::SELECTED_2ND_ACTION))
      curr_user->setBFlag(BState::SELECTED_3RD_ACTION);
    else if (curr_user->getBFlag(BState::SELECTED_ACTION))
      curr_user->setBFlag(BState::SELECTED_2ND_ACTION);
    else
      curr_user->setBFlag(BState::SELECTED_ACTION);

    if (canIncrementIndex(curr_user))
      return setNextPersonIndex();
  }
  else
  {
    std::cerr << "[Error]: Action buffer addition failure!" << std::endl;   
  }

  return false;
}

/*
 * Description: This function is similar to bufferEnemyAction, but uses the
 *              BattleMenu class to determine a selected action instead of
 *              an AI Module for the person. This function ofnly grabs
 *              info from the menu, the Menu is actually UPDATED FOR NEW PEOPLE
 *              in Battle::update (or called from there). This function will
 *              grab the menu selected information and add it to the buffer if
 *              possible, and update the person index as necessary.
 *
 * Inputs: none
 * Output: bool - true if a Buffer was successful
 */
bool Battle::bufferUserAction()
{
  auto buffered       = false;
  auto action_type    = menu->getActionType();
  auto action_targets = menu->getActionTargets();

  curr_user     = getPerson(person_index);
  auto is_confused = hasInfliction(Infliction::CONFUSE, curr_user);

  if (is_confused)
   action_targets = menu->getRandomTargets();

  /* Build the vector Person ptrs from the target index vector */
  std::vector<Person*> person_targets;

  for (auto it = begin(action_targets); it != end(action_targets); ++it)
    person_targets.push_back(getPerson(*it));
        
  /* Push the actions on to the Buffer */
  if (action_type == ActionType::SKILL)
  {
    //TODO: Buffering skill cooldowns [11-15-14]

    curr_skill = menu->getSelectedSkill().skill;
    buffered = action_buffer->add(curr_user, curr_skill, person_targets, 0);
  }
  else if (action_type == ActionType::ITEM)
  {
    curr_item = menu->getSelectedItem();
    buffered = action_buffer->add(curr_user, curr_item, person_targets, 0);

    if (buffered)
    {
      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{USING ITEM} " << curr_item->getName() << " from "
                  << "inventory and adding to buffer." << std::endl;
      }
      
      friends->getInventory()->removeItemID(curr_item->getGameID());

      auto items = friends->getInventory()->getBattleItems();
      menu->setSelectableItems(buildBattleItems(person_index, items));
    }
  }
  else if (action_type == ActionType::DEFEND  || 
           action_type == ActionType::GUARD   ||
           action_type == ActionType::IMPLODE ||
           action_type == ActionType::RUN     ||
           action_type == ActionType::PASS)
  {
    buffered = action_buffer->add(curr_user, action_type, person_targets, 0);
  }
  else
  {
    std::cerr << "[Error]: Invalid action selected\n";
  }

  if (buffered)
  {
    if (curr_user->getBFlag(BState::SELECTED_2ND_ACTION))
      curr_user->setBFlag(BState::SELECTED_3RD_ACTION);
    else if (curr_user->getBFlag(BState::SELECTED_ACTION))
      curr_user->setBFlag(BState::SELECTED_2ND_ACTION);
    else
      curr_user->setBFlag(BState::SELECTED_ACTION);

    if (canIncrementIndex(curr_user))
      return setNextPersonIndex();
  }
  else
  {
    std::cerr << "[Error]: Action buffer addition failure!" << std::endl;   
  }

  return false;
}

/*
 * Description: Builds the vector of BattleItem objects. Given a person index
 *              to build the item for, and a pair of Item, and amount objects,
 *              this method will build the BattleItems to be selected from in
 *              the BattleMenu and AI Module.
 *
 * Inputs: const int32_t p_index - the person index to build the items for.
 *         pair<Item, uint16_t> - the items and amounts of items selectable
 * Output: std::vector<BattleItem> - the construced vector of BattleItems
 */
std::vector<BattleItem> Battle::buildBattleItems(const int32_t &p_index,
    std::vector<std::pair<Item*, uint16_t>> items)
{
  curr_user = getPerson(p_index);
  std::vector<BattleItem> battle_items;

  if (curr_user != nullptr)
  {
    for (auto it = begin(items); it != end(items); ++it)
    {

      auto targets = getValidTargets(p_index, 
                                     (*it).first->getUseSkill()->getScope());
      
      auto all_targets = getPersonsFromIndexes(targets);
      std::vector<Person*> friends_targets;
      std::vector<Person*> foes_targets;
      
      for (auto target: all_targets)
      {
        if (friends->isInParty(target))
          friends_targets.push_back(target);
        else
          foes_targets.push_back(target);
      }

      BattleItem new_battle_item;
      new_battle_item.item = (*it).first;
      new_battle_item.item_skill = (*it).first->getUseSkill();
      new_battle_item.amount = (*it).second;
      new_battle_item.all_targets  = all_targets;
      new_battle_item.ally_targets = friends_targets;
      new_battle_item.foe_targets  = foes_targets;

      battle_items.push_back(new_battle_item);
    }
  }

  return battle_items;
}

/*
 * Description: Constructs the vector of BattleSkill objects given a person 
 *              index, and a SkillSet of useable skills.
 *
 * Inputs: int32_t p_index - the person index to the select the skills for.
 *         SkillSet* useable_skills - set of useable skills.
 * Output: std::vetor<BattleSkill> - constructed vector of BattleSkills
 */
std::vector<BattleSkill> Battle::buildBattleSkills(const int32_t &p_index,
    SkillSet* const useable_skills)
{
  curr_user = getPerson(p_index);
  std::vector<BattleSkill> battle_skills;

  if (curr_user != nullptr)
  {
    auto skill_elements = useable_skills->getElements(curr_user->getLevel());

    for (auto it = begin(skill_elements); it != end(skill_elements); ++it)
    {
      auto targets = getValidTargets(p_index, (*it).skill->getScope());
      auto all_targets = getPersonsFromIndexes(targets);

      std::vector<Person*> friends_targets;

      std::vector<Person*> foes_targets;
      
      for (auto target: all_targets)
      {
        if (friends->isInParty(target)) 
          friends_targets.push_back(target);
        else
          foes_targets.push_back(target);
      }

      BattleSkill new_battle_skill;
      new_battle_skill.skill          = (*it).skill;
      new_battle_skill.all_targets    = all_targets;
      new_battle_skill.ally_targets   = friends_targets;
      new_battle_skill.foe_targets    = foes_targets;

      battle_skills.push_back(new_battle_skill);
    }
  }

  return battle_skills;
}

/*
 * Description: This function calculates the base damage of an action during 
 *              battle. To do this, this function will also call functions like
 *              calcIgnoreFlags and calcElementalMods (elemental modifications
 *              have a huge impact on the actual damage resultant).
 *
 * Inputs: float crit_factor - the already determined crit factor to use
 * Output: int32_t - the base damage of the current action
 */
int32_t Battle::calcBaseDamage(const float &crit_factor, 
    const DamageType &damage_type)
{
  auto targ_attrs = temp_target_stats.at(pro_index);

  int32_t base_user_pow = 0; /* Base user power value */
  int32_t base_targ_def = 0; /* Base target defense value */
  int32_t phys_pow_val  = 0; /* Physical towards attack */
  int32_t phys_def_val  = 0; /* Physical towards defense */
  int32_t elm1_pow_val  = 0; /* Primary elemental towards attacking */
  int32_t elm1_def_val  = 0; /* Primary elemental towards defending */
  int32_t elm2_pow_val  = 0; /* Secondary elemental towards defending */
  int32_t elm2_def_val  = 0; /* Secondary elemental towards defending */
  int32_t luck_pow_val  = 0; /* Luck value towards attacking */
  int32_t luck_def_val  = 0; /* Luck value towards defending */
  int32_t var_val       = 0; /* Determined variance value */

  calcIgnoreState();
  calcElementalMods();

  /* Always calculate physical power into the equation */
  if (!getIgnoreFlag(IgnoreState::IGNORE_PHYS_ATK))
  {
    phys_pow_val = temp_user_stats.getStat(Attribute::PHAG);
    phys_pow_val *= kOFF_PHYS_MODIFIER;
  }

  if (!getIgnoreFlag(IgnoreState::IGNORE_PHYS_DEF))
  {
    phys_def_val = targ_attrs.getStat(Attribute::PHFD);
    phys_def_val *= kDEF_PHYS_MODIFIER;
  }

  /* Primary elemental affiliation bonuses */
  if (curr_skill->getPrimary() != Element::NONE &&
      curr_skill->getPrimary() != Element::PHYSICAL)
  {
    if (!getIgnoreFlag(IgnoreState::IGNORE_PRIM_ATK) &&
        (curr_user->getPrimary() == curr_skill->getPrimary() ||
        curr_user->getPrimary() == curr_skill->getSecondary()))
    {
      std::cout << "{Elements} -- Skill matches users prim elmmt!" << std::endl;
      elm1_pow_val  = temp_user_stats.getStat(prim_off);
      elm1_pow_val *= kOFF_PRIM_ELM_MATCH_MODIFIER;
    }
    
    if (!getIgnoreFlag(IgnoreState::IGNORE_PRIM_DEF) &&
        (curr_target->getPrimary() == curr_skill->getPrimary() ||
        curr_target->getPrimary() == curr_skill->getSecondary()))
    {
      std::cout <<"{Elements} -- Skill matches targets prim elmmt!"<< std::endl;
      elm1_def_val  = targ_attrs.getStat(prim_def);
      elm1_def_val *= kDEF_PRIM_ELM_MATCH_MODIFIER;
    }
  }
 
  /* Secondary elemental affiliation bonuses */
  if (!getIgnoreFlag(IgnoreState::IGNORE_SECD_ATK) &&
      (curr_skill->getSecondary() != Element::NONE &&
      curr_skill->getSecondary() != Element::PHYSICAL))
  {
    if (curr_user->getSecondary() == curr_skill->getPrimary() ||
        curr_user->getSecondary() == curr_skill->getSecondary())
    {
      std::cout << "{Elements} -- Skill matches user's secd elmmt!"<< std::endl;
      elm2_pow_val  = temp_user_stats.getStat(secd_off);
      elm2_pow_val *= kOFF_SECD_ELM_MATCH_MODIFIER;
    }

    if (!getIgnoreFlag(IgnoreState::IGNORE_SECD_DEF) &&
        (curr_target->getSecondary() == curr_skill->getPrimary() ||
        curr_target->getSecondary() == curr_skill->getSecondary()))
    {
      std::cout <<"{Elements} -- Skill matches targets secd elmmt!"<< std::endl;
      elm2_def_val  = targ_attrs.getStat(secd_def);
      elm2_def_val *= kDEF_SECD_ELM_MATCH_MODIFIER;
    }
  }

  /* Additional bonuses - luck power/defense values */
  if (!getIgnoreFlag(IgnoreState::IGNORE_LUCK_ATK))
  {
    luck_pow_val  = temp_user_stats.getStat(Attribute::MANN);
    luck_pow_val *= kMANNA_POW_MODIFIER;
  }

  if (!getIgnoreFlag(IgnoreState::IGNORE_LUCK_DEF))
  {
    luck_def_val  = targ_attrs.getStat(Attribute::MANN);
    luck_def_val *= kMANNA_DEF_MODIFIER;
  }

  /* Summation of base power / defense */
  base_user_pow  = phys_pow_val + elm1_pow_val + elm2_pow_val + luck_pow_val;
  base_user_pow *= kUSER_POW_MODIFIER;

  base_targ_def  = phys_def_val + elm1_def_val + elm2_def_val + luck_def_val;
  base_targ_def *= kTARG_DEF_MODIFIER;

  /* Addition of the power of the action */
  auto action_power = curr_action->getBase();

  /* If the action power is a percentage, add a pc of total user power */
  if (curr_action->actionFlag(ActionFlags::BASE_PC))
  {
    auto base_pc = static_cast<float>(action_power) / 100;
    action_power = base_pc * base_user_pow;
  }

  /* Addition of the variance of the action */
  auto base_var   = curr_action->getVariance();

  if (curr_action->actionFlag(ActionFlags::VARI_PC))
    var_val   = static_cast<float>(base_var) / 100 * action_power;
  else
    var_val = base_var;

  action_power = Helpers::randU(action_power - var_val, action_power + var_val);

  int32_t base_damage = 0;

  if (base_user_pow > base_targ_def)
    base_damage = base_user_pow + action_power - base_targ_def;
  else if (base_user_pow <= base_targ_def)
    base_damage = action_power;

  /* If the user is defending, shielded or guarded  decrease the damage taken by
   * the respective modifier values */
  if (curr_target->getBFlag(BState::DEFENDING))
    base_damage *= kDEFEND_MODIFIER;
  if (curr_target->getBFlag(BState::SHIELDED))
    base_damage *= kSHIELDED_MODIFIER;

  /* For guarding users, they will take kGUARD_MODIFIER factor of their dmg */
  if (damage_type == DamageType::GUARD)
    base_damage *= kGUARD_MODIFIER;

  base_damage *= crit_factor;
  base_damage = Helpers::setInRange(base_damage, kMINIMUM_DAMAGE, 
                    kMAXIMUM_DAMAGE);

  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "User Power: ----- " << base_user_pow << std::endl;
    std::cout << "Action Power: --- " << action_power << std::endl;
    std::cout << "Target Def: ----- " << base_targ_def << std::endl;
    std::cout << "Crit Factor: ---- " << crit_factor <<std::endl;
    std::cout << "Base Damage: ---- " << base_damage << std::endl <<std::endl;
  }

  return base_damage;
}

/*
 * Description: This function calculates elemental modifiers for the current
 *              action/person/target outcome processing. Damage is modified
 *              if the 
 *
 * Inputs:
 * Outputs:
 */
void Battle::calcElementalMods()
{
  /* Grab the temp attribute set for the curent processing target index */
  auto targ_attrs = temp_target_stats.at(pro_index);

  /* Determine the correct stats to apply the modifier to */
  auto prim_user_stat = Attribute::NONE;
  auto secd_user_stat = Attribute::NONE;
  auto prim_targ_stat = Attribute::NONE;
  auto secd_targ_stat = Attribute::NONE;

  if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
  {
    prim_user_stat = prim_off;
    secd_user_stat = secd_off;
    prim_targ_stat = prim_def;
    secd_targ_stat = secd_def;
  }

  auto prim_user_mod = temp_user_stats.getStat(prim_user_stat);
  auto secd_user_mod = temp_user_stats.getStat(secd_user_stat);

  auto prim_targ_mod = targ_attrs.getStat(prim_targ_stat);
  auto secd_targ_mod = targ_attrs.getStat(secd_targ_stat);

  auto prim_strength = false; /* If the opponent's prim element is str */
  auto secd_strength = false; /* If the opponent's secd element is str */
  auto prim_weakness = false; /* If the opponent's prim element is weak */
  auto secd_weakness = false; /* If the opponent's secd element is weak */

  if (curr_skill->getPrimary() != Element::NONE)
  {
  /* If the user's prim element is weak against the target's */
    if (curr_skill->getPrimary() == 
        Helpers::getStrength(curr_target->getPrimary()))
      prim_strength = true;

    /* If the user's prim element is strong against the target's */
    if (curr_skill->getPrimary() == 
        Helpers::getWeakness(curr_target->getPrimary()))
      prim_weakness = true;
  }

  if (curr_skill->getSecondary() != Element::NONE)
  {
    /* If the user's secd element is weak against the target's */
    if (curr_skill->getSecondary() == 
        Helpers::getStrength(curr_target->getSecondary()))
      secd_strength = true;    

    /* If the user's secd element is strong against the target's */
    if (curr_skill->getSecondary() == 
        Helpers::getWeakness(curr_target->getSecondary()))
      secd_weakness = true;
  }

  if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
  {
    /* User is strong in primary elemental case */
    if (prim_weakness && !secd_weakness)
    {
      std::cout << "{Elements} -- Prim Weakness" << std::endl;
      prim_user_mod *= kPRIM_ELM_ADV_MODIFIER;
    }
    /* User is strong in secondary elemental case */
    else if (!prim_weakness && secd_weakness)
    {
      std::cout << "{Elements} -- Secondary Weakness" << std::endl;
      secd_user_mod *= kSECD_ELM_ADV_MODIFIER;
    }
    /* User is strong in both elemental cases */
    else if (prim_weakness && secd_weakness)
    {
      std::cout << "{Elements} -- Double Weakness" << std::endl;
      prim_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in primary elemental case */
    else if (prim_strength && !secd_strength)
    {
      std::cout << "{Elements} -- Prim Strength" << std::endl;
      prim_targ_mod *= kPRIM_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in secondary elemental case */
    else if (!prim_strength && secd_strength)
    {
      std::cout << "{Elements} -- Secondary Strength" << std::endl;
      secd_targ_mod *= kSECD_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in both elemental cases */
    else if (prim_strength && secd_strength)
    {
      std::cout << "{Elements} -- Double Strength" << std::endl;
      prim_targ_mod *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_targ_mod *= kDOUBLE_ELM_ADV_MODIFIER;
   }
  }

  temp_user_stats.setStat(prim_user_stat, prim_user_mod);
  temp_user_stats.setStat(secd_user_stat, secd_user_mod);
  targ_attrs.setStat(prim_targ_stat, prim_targ_mod);
  targ_attrs.setStat(secd_targ_stat, secd_targ_mod);
}

/*
 * Description: Calculates the crit factor for the Battle. 
 *
 * Inputs:
 * Outputs:
 */
float Battle::calcCritFactor()
{
  /* Base crit factor */
  auto crit_factor = kOFF_CRIT_FACTOR;

  /* Unbearability modifier */
  auto unbr = curr_user->getTemp().getStat(Attribute::UNBR);

  crit_factor += unbr * kCRIT_MODIFIER;

  /* Level difference modifier */
  std::vector<Person*> target_vec = {curr_target};
  crit_factor += calcLevelDifference(target_vec) * kCRIT_LVL_MODIFIER;

  if (curr_target->getBFlag(BState::DEFENDING))
    crit_factor *= kCRIT_DEFENDING_MODIFIER;
  if (curr_target->getBFlag(BState::GUARDED))
    crit_factor *= kCRIT_GUARDED_MODIFIER;

  /* Crits should be between 100 and 1000% always */
  crit_factor = Helpers::setInRange(crit_factor, 1, 10);

  return crit_factor;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
bool Battle::calcIgnoreState()
{
  auto success = false;

  if (curr_action != nullptr && curr_skill != nullptr)
  {
    /* Offensive Action */
    if (curr_skill->getFlag(SkillFlags::OFFENSIVE) ||
        curr_skill->getFlag(SkillFlags::DEFENSIVE))
    {
      auto IG_PHYS_ATK = IgnoreState::IGNORE_PHYS_ATK;
      auto IG_PHYS_DEF = IgnoreState::IGNORE_PHYS_DEF;
      auto IG_PRIM_ATK = IgnoreState::IGNORE_PRIM_ATK;
      auto IG_PRIM_DEF = IgnoreState::IGNORE_PRIM_DEF;
      auto IG_SECD_ATK = IgnoreState::IGNORE_SECD_ATK;
      auto IG_SECD_DEF = IgnoreState::IGNORE_SECD_DEF;

      if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
      {
        IG_PHYS_ATK = IgnoreState::IGNORE_PHYS_DEF;
        IG_PHYS_DEF = IgnoreState::IGNORE_PHYS_ATK;
        IG_PRIM_ATK = IgnoreState::IGNORE_PRIM_DEF;
        IG_PRIM_DEF = IgnoreState::IGNORE_PRIM_ATK;
        IG_SECD_ATK = IgnoreState::IGNORE_SECD_DEF;
        IG_SECD_DEF = IgnoreState::IGNORE_SECD_ATK;
      }

      if (curr_action->atkFlag(IgnoreFlags::PHYSICAL))
        setIgnoreFlag(IG_PHYS_ATK);
      if (curr_action->defFlag(IgnoreFlags::PHYSICAL))
        setIgnoreFlag(IG_PHYS_DEF);

      if (curr_action->atkFlag(IgnoreFlags::LUCK))
        setIgnoreFlag(IgnoreState::IGNORE_LUCK_ATK);
      if (curr_action->defFlag(IgnoreFlags::LUCK))
        setIgnoreFlag(IgnoreState::IGNORE_LUCK_DEF);

      if (prim_off == Attribute::THAG)
        setIgnoreFlag(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::THERMAL));
      else if (prim_off == Attribute::POAG)
        setIgnoreFlag(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::POLAR));
      else if (prim_off == Attribute::PRAG)
        setIgnoreFlag(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::PRIMAL));
      else if (prim_off == Attribute::CHAG)
        setIgnoreFlag(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::CHARGED));
      else if (prim_off == Attribute::CYAG)
       setIgnoreFlag(IG_PRIM_ATK,curr_action->atkFlag(IgnoreFlags::CYBERNETIC));
      else if (prim_off == Attribute::NIAG)
        setIgnoreFlag(IG_PRIM_ATK, curr_action->atkFlag(IgnoreFlags::NIHIL));

      if (prim_def == Attribute::THFD)
        setIgnoreFlag(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::THERMAL));
      else if (prim_def == Attribute::POFD)
        setIgnoreFlag(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::POLAR));
      else if (prim_def == Attribute::PRFD)
        setIgnoreFlag(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::PRIMAL));
      else if (prim_def == Attribute::CHFD)
        setIgnoreFlag(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::CHARGED));
      else if (prim_def == Attribute::CYFD)
       setIgnoreFlag(IG_PRIM_DEF,curr_action->defFlag(IgnoreFlags::CYBERNETIC));
      else if (prim_def == Attribute::NIFD)
        setIgnoreFlag(IG_PRIM_DEF, curr_action->defFlag(IgnoreFlags::NIHIL));

      if (secd_off == Attribute::THAG)
        setIgnoreFlag(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::THERMAL));
      else if (secd_off == Attribute::POAG)
        setIgnoreFlag(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::POLAR));
      else if (secd_def == Attribute::PRAG)
        setIgnoreFlag(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::PRIMAL));
      else if (secd_def == Attribute::CHAG)
        setIgnoreFlag(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::CHARGED));
      else if (secd_off == Attribute::CYAG)
       setIgnoreFlag(IG_SECD_ATK,curr_action->atkFlag(IgnoreFlags::CYBERNETIC));
      else if (secd_off == Attribute::NIAG)
        setIgnoreFlag(IG_SECD_ATK, curr_action->atkFlag(IgnoreFlags::NIHIL));

      if (secd_def == Attribute::THFD)
        setIgnoreFlag(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::THERMAL));
      else if (secd_def == Attribute::POFD)
        setIgnoreFlag(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::POLAR));
      else if (secd_def == Attribute::PRFD)
        setIgnoreFlag(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::PRIMAL));
      else if (secd_def == Attribute::CHFD)
        setIgnoreFlag(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::CHARGED));
      else if (secd_def == Attribute::CYFD)
       setIgnoreFlag(IG_SECD_DEF,curr_action->defFlag(IgnoreFlags::CYBERNETIC));
      else if (secd_def == Attribute::NIAG)
        setIgnoreFlag(IG_SECD_DEF, curr_action->defFlag(IgnoreFlags::NIHIL));
    }

    success = true;
  }

  return success;
}

/*
 * Description: Determines the average level difference among targets
 *
 * Inputs: none
 * Output: int16_t - the [average] difference in level value
 */
int16_t Battle::calcLevelDifference(std::vector<Person*> targets)
{
  if (curr_user != nullptr)
  {
    auto total_lvl = 0;

    for (uint16_t i = 0; i < targets.size(); i++)
    {
      if (targets.at(i) != nullptr)
        total_lvl += targets.at(i)->getLevel();
    }
    
    if (total_lvl != 0)
      return curr_user->getLevel() - (total_lvl / targets.size());
  }
  else
    std::cerr << "[Error] Cannot compute level difference." << std::endl;

  return 0;
}

/*
 * Description:
 *
 * Inputs:
 * Output: 
 */
int32_t Battle::calcTurnRegen(Person* const target, const Attribute& attr)
{
  auto regen_amt = 0;
  auto can_process = (target != nullptr);
  can_process &= (attr == Attribute::VITA || attr == Attribute::QTDR);

  if (can_process)
  {
    auto one_pc = static_cast<float>(target->getTemp().getStat(attr)) / 100.0;
    auto alt_factor = getRegenFactor(target->getQDRegenRate());

    if (attr == Attribute::VITA)
      alt_factor = getRegenFactor(target->getVitaRegenRate());

    regen_amt = one_pc * static_cast<float>(alt_factor);
    auto max_amt = target->getTemp().getStat(attr);

    if (target->getCurr().getStat(attr) + regen_amt >= max_amt)
      regen_amt = max_amt - target->getCurr().getStat(attr);

    target->getCurr().alterStat(attr, regen_amt);
  }
  else
  {
    std::cerr << "[Error] Bad turn regeneration calculation" << std::endl;
  }

  return regen_amt;
}

/*
 * Description: Asserts that every member in the Foes party has a valid AI
 *              Module (assigned with a parent matching each member) before
 *              larger problems occur.
 *
 * Inputs: none
 * Output: bool - the validity of all AI Modules on the foes party
 */
bool Battle::checkAIModules()
{
  auto modules_good = true;

  for (size_t i = 0; i < foes->getSize(); i++)
  {
    auto member = foes->getMember(i);

    if (member != nullptr)
    {
      modules_good &= (member->getAI() != nullptr);
      modules_good &= (member == (member->getAI()->getParent()));
    }
  }

  return modules_good;
}

/*
 * Description: Determines whether the battle processing can be incremented or
 *              whether the current person index needs to select another action
 *              based on the person's battle flags.
 *
 * Inputs: Person* - person to check whether to increment action selection for
 * Output: bool - true if action selection should be incremented
 */
bool Battle::canIncrementIndex(Person* check_person)
{
  if ((check_person->getBFlag(BState::SELECTED_ACTION) && 
      !check_person->getAilFlag(PersonAilState::TWO_SKILLS) && 
      !check_person->getAilFlag(PersonAilState::THREE_SKILLS)) ||
      (check_person->getBFlag(BState::SELECTED_2ND_ACTION) &&
       check_person->getAilFlag(PersonAilState::TWO_SKILLS) &&
      !check_person->getAilFlag(PersonAilState::THREE_SKILLS)) ||
      (check_person->getBFlag(BState::SELECTED_3RD_ACTION) &&
       check_person->getAilFlag(PersonAilState::THREE_SKILLS)))
  {
    return true;
  }

  return false;
}

/*
 * Description: Clears action variables, prepping it for a new action/person/
 *              target combination for turn processing.
 *
 * Inputs: none
 * Output: none
 */
void Battle::clearActionVariables()
{
  prim_off  = Attribute::NONE;
  prim_def  = Attribute::NONE;
  secd_off  = Attribute::NONE;
  secd_def  = Attribute::NONE;
  user_attr = Attribute::NONE;
  targ_attr = Attribute::NONE;

  temp_user_stats     = AttributeSet();
  temp_user_max_stats = AttributeSet();

  temp_target_stats.clear();
  temp_target_max_stats.clear();

  curr_user   = nullptr;
  curr_target = nullptr;
  curr_action = nullptr;
  curr_skill  = nullptr;
  curr_item   = nullptr;

  ignore_flags = static_cast<IgnoreState>(0);

  pro_index = 0;
}

/*
 * Description: Returns enumeration of party death [None, Friends, Foes, Both]
 *
 * Inputs: Party* - party to check death for.
 * Output: bool - true if the size of the living members vector is zero
 */
bool Battle::checkPartyDeath(Party* const check_party)
{
  if (check_party != nullptr && check_party->getLivingMembers().size() == 0)
    return true;

  return false;
}

/*
 * Description: Cleanup before the end of a Battle turn. Clears the action
 *              buffer, resets person index, the action variables. Updates
 *              the turns elapsed and menu fora new turn.
 *
 * Inputs: none
 * Output: none
 */
void Battle::cleanUp()
{
  action_buffer->clearAll();
  person_index = 0;

  setBattleFlag(CombatState::BEGIN_PROCESSING, false);
  setBattleFlag(CombatState::BEGIN_ACTION_PROCESSING, false);
 
  /* Clean all action processing related variables */
  clearActionVariables();

  /* Increment the turn counter */
  turns_elapsed++;

  action_buffer->update();
  menu->unsetAll();
  curr_module = nullptr;

  /* Cleanup for each member of friends and persons */
  for (size_t i = 0; i < friends->getSize(); i++)
  {
    auto member = friends->getMember(i);
    member->resetDefend();
    member->resetGuard();
    member->resetGuardee();
    member->setAilFlag(PersonAilState::SKIP_NEXT_TURN, false);
  }

  for (size_t i = 0; i < foes->getSize(); i++)
  {
    auto member = foes->getMember(i);
    member->resetAI();
    member->resetDefend();
    member->resetGuard();
    member->resetGuardee();
    member->setAilFlag(PersonAilState::SKIP_NEXT_TURN, false);
  }

  setBattleFlag(CombatState::PHASE_DONE, true);
}

/*
 * Description: Determines the turn progression of the Battle. The party with
 *              the total greatest speed will generally go first. In the rare
 *              case that the party's have equal speed, the first party will
 *              randomly go first. Note that it really has little consequence
 *              in the outcome of Battle which parties go first since actions
 *              are ordered based upon user momentum.
 *
 * Inputs: none
 * Output: none
 */
void Battle::determineTurnMode()
{
  if (friends->getTotalSpeed() > foes->getTotalSpeed())
    setTurnMode(TurnMode::FRIENDS_FIRST);
  else if (friends->getTotalSpeed() < foes->getTotalSpeed())
    setTurnMode(TurnMode::ENEMIES_FIRST);
  else
  {
    if (Helpers::flipCoin())
      setTurnMode(TurnMode::FRIENDS_FIRST);
    else
      setTurnMode(TurnMode::ENEMIES_FIRST);
  }
}

/*
 * Description: Determines whether the current action will crit against the
 *              current target. This chance is determined by the base crit
 *              chance, a modifier based on the value of the action user's
 *              UNBR stat. In addition, bonus critical chance occurs
 *              proportional to the level difference of the action user's 
 *              level and the action target's level.
 *               
 * Inputs: none
 * Output: bool - true if the current action will crit against the curr target.
 */
bool Battle::doesActionCrit()
{
  if (curr_user == nullptr || curr_target == nullptr)
    return false;

  auto crit_possible = true;
  auto crit_happens  = false;

  crit_possible &= curr_user->getBFlag(BState::CAN_CRIT);
  crit_possible &= curr_target->getBFlag(BState::CAN_BE_CRIT);

  if (crit_possible)
  {
    auto crit_chance = kBASE_CRIT_CHANCE;
    auto crit_mod = temp_user_stats.getStat(Attribute::UNBR) * kCRIT_MODIFIER;
    auto crit_lvl_mod = calcLevelDifference(action_buffer->getTargets()) * 
                                            kCRIT_LVL_MODIFIER;

    crit_chance += crit_mod + crit_lvl_mod;

    if (curr_target->getBFlag(BState::DEFENDING))
      crit_chance *= kCRIT_DEFENDING_MODIFIER;
    if (curr_target->getBFlag(BState::GUARDED))
      crit_chance *= kCRIT_GUARDED_MODIFIER;

    if (crit_chance > 0)
    {
      uint32_t crit_pc_1000 = floor(crit_chance * 1000);
      if (Helpers::chanceHappens(crit_pc_1000, 1000))
        crit_happens = true;
    }
  }

  return crit_happens;
}

/*
 * Description: Determines whether the current skill will entirely miss or
 *              if the processing of each of its actions will have a chance of
 *              occuring.
 *
 * Inputs: none
 * Output: bool - true if the current skill will hit
 */
bool Battle::doesSkillHit(std::vector<Person*> targets)
{
  auto can_process = true;
  auto hits  = true;

  can_process &= curr_skill != nullptr;
  can_process &= curr_user != nullptr;

  if (can_process)
  {
    auto can_hit = true;

    can_hit &= curr_skill->isValid();
    can_hit &= curr_user->getBFlag(BState::SKL_ENABLED);
    can_hit &= !curr_user->getAilFlag(PersonAilState::MISS_NEXT_TARGET);

    if (curr_user->getAilFlag(PersonAilState::MISS_NEXT_TARGET))
    {
      auto miss = createEvent(BattleEventType::BLIND_MISS, curr_user, targets);
      curr_events.push_back(miss);
      std::cout << "{BLIND} - Blind miss" << std::endl;
    }

    if (can_hit)
    {
      if (curr_user->getAilFlag(PersonAilState::NEXT_ATK_NO_EFFECT))
      {
        auto miss = createEvent(BattleEventType::DREAMSNARE_MISS, curr_user,
                        targets);
        curr_events.push_back(miss);
        std::cout << "{DREAMSNARE} - No effect" << std::endl;
      }

      /* Obtain the base hit rate (in XX.X%) */
      auto hit_rate = curr_skill->getChance();

      /* Obtain the average level difference in #lvls, a positive value denoting
       * the user's level is higher -> modify it -> add to base hit chance */
      auto level_diff = calcLevelDifference(targets);

      /* Hit rate is not altered for user's who have higher levels */
      if (level_diff < 0)
      {
        /* Add the (negative) mod value to the hit rate */
        auto mod = static_cast<float>(level_diff * kDODGE_PER_LEVEL_MODIFIER);
        auto new_hit_rate = hit_rate + mod;
        auto lowest_hit_rate = hit_rate * (kDODGE_HIGHEST_RATE_PC / 100);

        /* Assert that the hit rate is above the minimum hit rate */
        hit_rate = Helpers::setInRange(new_hit_rate, lowest_hit_rate, hit_rate);
      }
 
      hits = Helpers::chanceHappens(static_cast<uint32_t>(hit_rate), 100);
    }
  }

  return hits;
}

/*
 * Description: Determines whether the current action will miss its current
 *              target. 
 *
 * Inputs: none
 * Output: bool - true if the current action will hit
 */
bool Battle::doesActionHit()
{
  auto can_process = true;
  auto hit = true;

  can_process &= curr_user    != nullptr;
  can_process &= curr_target  != nullptr;
  can_process &= curr_action  != nullptr;


  if (can_process)
  {
    /* Run probabilities of the action occuring */
    hit = Helpers::chanceHappens(
        static_cast<uint32_t>(curr_action->getChance()), 100);
  }
  else
  {
    std::cout << "Something ain't set right!" << std::endl;
    std::cerr << "[Warning] Cannot process action missing chances" << std::endl; 
  }

  return hit;
}

/*
 * Description: Determines whether the current person succeeds in running
 *              from the Battle.
 *
 * Inputs: none
 * Output: bool - true if the current person succeeds in running
 */
bool Battle::doesCurrPersonRun()
{
  auto can_run_happen = true;
  auto run_happens    = false;

  /* First, determine if a run is possible */
  can_run_happen &= !getBattleFlag(CombatState::BOSS);
  can_run_happen &= !getBattleFlag(CombatState::FINAL_BOSS);
  can_run_happen &= curr_user->getBFlag(BState::ALIVE);
  can_run_happen &= curr_user->getBFlag(BState::RUN_ENABLED);
  
  if (can_run_happen)
  {
    /* Next, determine the chance for a run to occur. Users with higher
     * momentum will have a higher chance of running. Parties with higher
     * momentum will have a higher chance of running. Enemies with higher
     * momentum will decrease the chance of a successful run */
    std::vector<Person*> other_allies;
    Party* other_enemies;
    double mmnt_points = 0;

    if (person_index < 0)
    {
      other_allies   = foes->findMembersExcept(curr_user, true);
      other_enemies  = friends;
    }
    else
    {
      other_allies  = friends->findMembersExcept(curr_user, true);
      other_enemies = foes;
    }

    mmnt_points += curr_user->getCurr().getStat(Attribute::MMNT) 
                   * kUSER_RUN_MODIFIER;
  
    /* Add each other allies momentum by the factor of ally run modifier */
    for (auto it = begin(other_allies); it != end(other_allies); ++it)
      mmnt_points += (*it)->getCurr().getStat(Attribute::MMNT)
                     * kALLY_RUN_MODIFIER;

    /* Add each enemies momentum by the factor of enemy run modifier */
    for (size_t i = 0; i < other_enemies->getSize(); i++)
    {
      auto enemy_stats = other_enemies->getMember(i)->getCurr();
      mmnt_points += enemy_stats.getStat(Attribute::MMNT) * kENEMY_RUN_MODIFIER;
    }

    /* Determine the percent change based on run mmnt pc modifier */
    auto pc_change = kRUN_PC_PER_POINT * mmnt_points;
    auto run_chance = static_cast<int32_t>((pc_change * 100) + 
                                          (kBASE_RUN_CHANCE * 100));

    run_happens = Helpers::chanceHappens(run_chance, 100);
  }

  return run_happens;
}

/*
 * Description: Deals with general upkeep including effects brought on by
 *              the current battle circumstances like weather that aren't
 *              or can't be dealt with in personal upkeep functions.
 *
 * Inputs: none
 * Output: none
 */
void Battle::generalUpkeep()
{
  /* Print out the party state if either in UDEBUG or in TEXT battle mode */
  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "\n=============\n";
    std::cout << "  TURN " << Helpers::numToRoman(turns_elapsed + 1)
              << "\n=============" << std::endl;
  }

  //TODO: Weather updates [03-01-14]

  /* General upkeep phase complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/*
 * Description: Sets the flags of BattleState at the beginning of the Battle.
 *              By default, all Battles will be a random encounter unless
 *              there exists a person in the foes party which is: a mini-boss,
 *              a boss, or final boss. Also sets other flags to their default
 *              (undone) state.
 *
 * Inputs: none
 * Output: none
 */
void Battle::loadBattleStateFlags()
{
  setBattleFlag(CombatState::RANDOM_ENCOUNTER, true);

  if (foes->hasMiniBoss())
  {
    setBattleFlag(CombatState::RANDOM_ENCOUNTER, false);
    setBattleFlag(CombatState::MINI_BOSS, true);
  }
  if (foes->hasBoss())
  {
    setBattleFlag(CombatState::RANDOM_ENCOUNTER, false);
    setBattleFlag(CombatState::MINI_BOSS, false);
    setBattleFlag(CombatState::BOSS, true);
  }
  if (foes->hasFinalBoss())
  {
    setBattleFlag(CombatState::RANDOM_ENCOUNTER, false);
    setBattleFlag(CombatState::MINI_BOSS, false);
    setBattleFlag(CombatState::BOSS, false);
    setBattleFlag(CombatState::FINAL_BOSS, true);
  }

  setBattleFlag(CombatState::PHASE_DONE, false);
  setBattleFlag(CombatState::LOSS, false);
  setBattleFlag(CombatState::VICTORY, false);
  setBattleFlag(CombatState::ALLIES_RUN, false);
  setBattleFlag(CombatState::ENEMIES_RUN, false);
  setBattleFlag(CombatState::OUTCOME_DONE, false);
  setBattleFlag(CombatState::ERROR_STATE, false);
}

/*
 * Description: Orders the actions on the buffer by speed of the aggressor. 
 *              By default, items will be take precedence over other actions
 *              (this is one advantage of items vs. skills), then the remaining
 *              actions will be sorted by the user of the action's momentum.
 *
 * Inputs: none
 * Output: none
 */
void Battle::orderActions()
{
  /* Re-order buffer itmes based on defined oredering */
  action_buffer->reorder();

  /* Order action state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Battle::performEvents()
{
  std::cout << "---- Performing Events ----" << std::endl;
  
  for (auto &event : curr_events)
  {
    if (event->type == BattleEventType::BEGIN_DEFEND)
    {
      event->user->setBFlag(BState::DEFENDING, true);

      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{DEFEND} " << curr_user->getName() << " is now defending "
                  << "themselves from damage." << std::endl;
      }  
    }
  }
}

/*
 * Description: Deals with personal-related upkeep (such as processing damaging
 *              ailments, stat recalculations), at the beginning of each turn.
 *
 * Inputs: Person* - target person to perform upkeep for
 * Output: none
 */
void Battle::personalUpkeep(Person* const target)
{
  curr_target   = target;
  auto ailments = getPersonAilments(target);

  for (auto& ailment : ailments)
  {
    if (ailment != nullptr && ailment->getFlag(AilState::TO_UPDATE))
    {
      ailment->update(true);

      if (ailment->getFlag(AilState::DEALS_DAMAGE) && 
          target->getBFlag(BState::ALIVE))
      {
        auto damage_amount = ailment->getDamageAmount();
        auto damage_type   = ailment->getDamageType();
        processDamageAmount(damage_amount, damage_type);
      }

      if (ailment->getType() == Infliction::DEATHTIMER)
      {
        if (ailment->getFlag(AilState::TO_KILL))
        {
          processDamageAmount(target->getCurr().getStat(0),
              DamageType::DEATHTIMER);
        }
        else if (getBattleMode() == BattleMode::TEXT)
        {
          std::cout << "{DEATHTIMER}" << ailment->getTurnsLeft() 
                    << " turns until death." << std::endl;
        }
      }

      /* If the target is dead, the ailment update must have killed them */
      if (!target->getBFlag(BState::ALIVE))
      {
        /* The ailment should be destroyed if it removes on death */
        if (ailment->getFlag(AilState::CURE_ON_DEATH))
          ailment->setFlag(AilState::TO_CURE);
      }
    }

    if (ailment != nullptr && ailment->getFlag(AilState::TO_CURE))
      removeAilment(ailment);
  }
  
  //TODO
  // clear flags for new turn (temp flags?)
  // recalulate ailment factor

  auto vita_regen = calcTurnRegen(target, Attribute::VITA);
  auto qtdr_regen = calcTurnRegen(target, Attribute::QTDR);

  if (getBattleMode() == BattleMode::TEXT)
  {
    if (vita_regen > 0) 
    {
      std::cout << "{REGEN} " << target->getName() << " has restored " 
                << vita_regen << " VITA.\n";
    }
    if (qtdr_regen > 0)
    {
      std::cout << "{REGEN} " << target->getName() << " has regained "
                << qtdr_regen << " QTDR.\n";
    }
  }

  target->battleTurnPrep();
}

/*
 * Description: Process the action selections (Items/Skills/Guard/etc.) in the
 *              buffer after they've been ordered. Will call functions like
 *              processSkill(std::vector<Person*> targets) to achieve this.
 *              Adds the processing effects as a vector events to the current
 *              events of Battle and waits for the rendering to be complete.
 *
 * Inputs: none
 * Output: none
 */
void Battle::processBuffer()
{
  std::cout << "--- Processing Battle Events ---" << std::endl;
  auto last_index = false;
  
  /* Update to the next action buffer index if action processing was completed
     and set it to a bool to checkf or the last buffer index */
  if (getBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE))
  {
    setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, false);
        last_index &= !action_buffer->setNext();
    //setBattleFlag(CombatState::BEGIN_PROCESSING, false);
  }
   
  auto curr_action_type = ActionType::NONE;
  auto curr_targets = action_buffer->getTargets();
  auto damage_types = action_buffer->getDamageTypes();
  curr_action_type  = action_buffer->getActionType();
  curr_user         = action_buffer->getUser();

  if (getBattleMode() == BattleMode::TEXT && curr_user != nullptr)
    std::cout << "\n{User} Processing: " << curr_user->getName() << std::endl;

  if (curr_action_type == ActionType::SKILL)
  {
    processSkill(curr_targets, damage_types);
  }
  else if (curr_action_type == ActionType::ITEM)
  {
    //TODO: Item processing
  }
  else if (curr_action_type == ActionType::DEFEND)
  {
    auto event = createEvent(BattleEventType::BEGIN_DEFEND, curr_user, nullptr);
    curr_events.push_back(event);
    setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
  }
  else if (curr_action_type == ActionType::GUARD)
  {
    auto event = createEvent(BattleEventType::BEGIN_GUARD, curr_user,
                   action_buffer->getTargets().at(0));
    curr_events.push_back(event);
    
    bool good_guard = processGuard();

      // if (good_guard)
      // {
      //   /* Update the buffer to swap out Guard <--> Guardee targets */
      //   action_buffer->injectGuardTargets();

      //   if (getBattleMode() == BattleMode::TEXT)
      //   {
      //     std::cout << "{GUARD} " << curr_target->getGuard()->getName() 
      //               << " is now guarding " << curr_target->getName() 
      //               << " from some damage.\n";
      //   }
      // }
      // else
      // {
      //   if (getBattleMode() == BattleMode::TEXT)
      //   {
      //     std::cout << "{FAIL} " << curr_user->getName() << "'s attempt to "
      //               << "guard " << curr_target->getName() << " has failed.\n";
      //   }
      // }

      // Guard the appropriate target
    }
    else if (curr_action_type == ActionType::IMPLODE)
    {
      // Annihilate self in catastrophic hit against opponents!
    }
    else if (curr_action_type == ActionType::RUN)
    {
      // if (getBattleMode() == BattleMode::TEXT)
      //   std::cout << "{RUNNING} Attempting to run." << std::endl;

      // if (doesCurrPersonRun())
      
      //   done = true;

      //   if (friends->isInParty(curr_user))
      //     setBattleFlag(CombatState::ALLIES_RUN, true);
      //   else
      //     setBattleFlag(CombatState::ENEMIES_RUN, true);

      //   setBattleFlag(CombatState::PHASE_DONE, true);
      // }
      // else
      // {
      //   if (getBattleMode() == BattleMode::TEXT)
      //   {
      //     std::cout << "{Failed} The run attempt has failed!" << std::endl;
      //   }
      // }
    }
    else if (curr_action_type == ActionType::PASS)
    {
      // Pass the turn, do nothing!
    }
    else
    {
      // if (getBattleMode() == BattleMode::TEXT)
      // {
      //   //action_buffer->print();
      //   std::cout << "[Error]: Attempting to process bad action!" << std::endl;
      // }
    }

    // if (!can_process && getBattleMode() == BattleMode::TEXT)
    // {
    //   action_buffer->print();
    //   std::cout << "[Error]: Couldn't process current action!" << std::endl;
    // }

  /* Complete the processing of the last of events and move to clean up */
  if (last_index && getBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE))
    setBattleFlag(CombatState::PHASE_DONE, true);

  setBattleFlag(CombatState::READY_TO_RENDER, true);
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
int16_t Battle::getRegenFactor(const RegenRate &regen_rate)
{
  if (regen_rate == RegenRate::WEAK)
    return kREGEN_RATE_WEAK_PC;
  else if (regen_rate == RegenRate::NORMAL)
    return kREGEN_RATE_NORMAL_PC;
  else if (regen_rate == RegenRate::STRONG)
    return kREGEN_RATE_STRONG_PC;
  else if (regen_rate == RegenRate::GRAND)
    return kREGEN_RATE_GRAND_PC;
  return kREGEN_RATE_ZERO_PC;
}

/*
 * Description: Attempts to assign a new guard/guardee pair for the curr_user/
 *              curr_target pair respectively, by first checking if a new 
 *              guard pair can be made and then assigning the proper pointers. 
 *
 * Inputs: none
 * Output: bool - return the outcome of the guard operation
 */
bool Battle::processGuard()
{
  auto can_guard  = true;
  auto good_guard = false;

  /* A guard/guardee combo can only be made if both the guard and guardee are
   * not being guarded by, or guarding any persons */
  can_guard &= curr_target->getGuard() == nullptr;
  can_guard &= curr_target->getGuardee() == nullptr;
  can_guard &= curr_user->getGuard() == nullptr;
  can_guard &= curr_user->getGuardee() == nullptr;

  /* If a guard ca be made, attempt to assign guardee and guarding persons.
   * This should assign the GUARDING/GUARDED flags properly */
  if (can_guard)
  {
    good_guard  = true;
    good_guard &= curr_user->setGuardee(curr_target);
    good_guard &= curr_target->setGuard(curr_user);
  }

  return good_guard;
}

bool Battle::performGuard()
{

}

/*
 * Description:
 *
 * Inputs: 
 * Output:
 */
bool Battle::processAction(BattleEvent* battle_event,
    std::vector<DamageType> damage_types)
{
  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{Target} Processing: " 
              << battle_event->targets.at(0)->getName() << std::endl;
  }

  auto can_process  = false;
  auto done         = false;  
  auto target_alive = curr_target->getBFlag(BState::ALIVE);
  auto damage_type  = damage_types.at(pro_index);

  /* Non-revive actions can only be processed against targets who are alive */
  if (!target_alive && curr_action->actionFlag(ActionFlags::REVIVE))
    can_process = true;
  else if (target_alive && !curr_action->actionFlag(ActionFlags::REVIVE))
    can_process = true;

  auto action_hits = false;

  if (can_process)
    action_hits = doesActionHit();

  if (can_process && action_hits)
  {
    //TODO -> move to perform action
    //curr_user->setAilFlag(PersonAilState::MISS_NEXT_TARGET, false);
    //curr_user->setAilFlag(PersonAilState::NEXT_ATK_NO_EFFECT, false);

    if (curr_action->actionFlag(ActionFlags::ALTER) ||
        curr_action->actionFlag(ActionFlags::ASSIGN))
    {
      auto action_target = curr_user;
      auto factor_target = curr_user;

      user_attr = curr_action->getUserAttribute();
      targ_attr = curr_action->getTargetAttribute();

      /* If the user's attribute is defined and the target's is not, the
       * alteration will be on the user's %/value up to their MAX attrs*/
      if (user_attr != Attribute::NONE && targ_attr == Attribute::NONE)
          targ_attr  = user_attr;
      /* If the target's attribute is defined and the user's is not, the
       * alteration or assignment will be on the target's %/value up to their 
       * MAX or CURR amount, respectively */
      else if (user_attr == Attribute::NONE && targ_attr != Attribute::NONE)
      {
        action_target = curr_target;
        factor_target = curr_target;
        user_attr     = targ_attr;
      }
      /* If both the user and target's attributes are defined, the alteration
       * will alter the target's value by a percentage of the user's stat */
      else if (user_attr != Attribute::NONE && targ_attr != Attribute::NONE)
        action_target = curr_target;
      else
        std::cerr << "[Error] - Critical error in Battle processing.\n";

      if (curr_action->actionFlag(ActionFlags::FLIP_ATTR))
      {
        std::cout << "Flipping!" << std::endl;
        std::swap(action_target, factor_target);
        std::swap(user_attr, targ_attr);
        std::cout << "Action Target:" << action_target->getName()<< std::endl;
        std::cout << "Factor Target: " << factor_target->getName()<< std::endl;
      }


      std::vector<Person*> target_vec{action_target};
      battle_event->targets = target_vec;

      // if (curr_action->actionFlag(ActionFlags::ALTER))
      //   done = processAlterAction(damage_type, action_target, factor_target);
      // else
      //   done = processAssignAction(damage_type, action_target, factor_target);
      // }
    }
    else if (curr_action->actionFlag(ActionFlags::DAMAGE))
      done = processDamageAction(battle_event, damage_type);

      // else if (curr_action->actionFlag(ActionFlags::INFLICT))
      //   done = processInflictAction();
      // else if (curr_action->actionFlag(ActionFlags::RELIEVE))
      //   done = processRelieveAction();
      // else if (curr_action->actionFlag(ActionFlags::REVIVE))
      //   done = processReviveAction();
  }
  else if (!can_process)
  {
    auto fizzle = createEvent(BattleEventType::FIZZLE, curr_user, nullptr);
    fizzle->action_use = curr_action;
    curr_events.push_back(fizzle);
    setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);

    if (getBattleMode() == BattleMode::TEXT)
      std::cout << "{Fizzle} The action has fizzled!" << std::endl;
  }
  else if (can_process && !action_hits)
  {
    auto miss = createEvent(BattleEventType::ACTION_MISS, curr_user, nullptr);
    miss->action_use = curr_action;
    curr_events.push_back(miss);
    setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);

    if (getBattleMode() == BattleMode::TEXT)
      std::cout << "{MISS} The action has missed. " << std::endl;
  }

  return done;
}

/*
 * Description: Processes an alteration keywoard action against a vector of 
 *              targets also given a vector of corresponding DamageTypes
 *
 * Inputs: std::vector<Person*> - vector of targets in the form of Person ptrs
 *         std::vector<DamageType> - vector of corresponding damage types
 * Output: bool - true if a party death (vic. cond.) occured during operation
 */
bool Battle::processAlterAction(const DamageType &damage_type,
    Person* action_target, Person* factor_target)
{  
  auto base_pc   = curr_action->actionFlag(ActionFlags::BASE_PC);
  auto vari_pc   = curr_action->actionFlag(ActionFlags::VARI_PC);

  int32_t base      = curr_action->getBase();
  int32_t vari      = curr_action->getVariance();
  int32_t cur_value = 0;
  int32_t set_value = 0;
  int32_t var_value = 0;
  float one_pc      = 0.0;
  auto death = false;
  auto done  = false;

  auto max_value = action_target->getTemp().getStat(targ_attr);
  cur_value      = action_target->getCurr().getStat(targ_attr);
    
  one_pc    = static_cast<float>(factor_target->getTemp().getStat(user_attr));
  one_pc   /= 100;

  set_value = (base_pc) ? (std::floor(static_cast<int32_t>(one_pc * base)))
                        : (base);
  var_value = (vari_pc) ? (std::floor(static_cast<int32_t>(one_pc * vari)))
                        : (vari);

  var_value = Helpers::randU(-var_value, var_value);
  auto alt_value = set_value + var_value;

  /* The altered amount cannot be > the dif between max and current */
  if (alt_value > 0 && (alt_value + cur_value) > max_value)
  {
    alt_value = max_value - cur_value;
  }
  /* The altered amount cannot be such that it would decrease a stat to neg */
  else if (alt_value < 0 && alt_value < -cur_value)
  {
    alt_value = -cur_value;
    death = true;
  }

  /* Actually perform the alteration */
  action_target->getCurr().alterStat(targ_attr, alt_value);

  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{ALTER} " << action_target->getName() << "'s " 
              << AttributeSet::getName(targ_attr) << " has been altered by "
              << alt_value << "." << std::endl;
  }

  if (death)
    done = updatePersonDeath(damage_type);

  return done;
}

/*
 * Description: 
 *
 * Inputs:
 * Output:
 */
bool Battle::processAssignAction(const DamageType &damage_type,
    Person* action_target, Person* factor_target)
{
  auto base_pc   = curr_action->actionFlag(ActionFlags::BASE_PC);
  auto vari_pc   = curr_action->actionFlag(ActionFlags::VARI_PC);
  int32_t base      = curr_action->getBase();
  int32_t vari      = curr_action->getVariance();
  int32_t set_value = 0;
  int32_t var_value = 0;
  float one_pc      = 0.0;
  auto done         = false;
  auto max_value    = factor_target->getTemp().getStat(targ_attr);

  one_pc    = static_cast<float>(factor_target->getCurr().getStat(user_attr));
  one_pc   /= 100;

  set_value = (base_pc) ? (std::floor(static_cast<int32_t>(one_pc * base)))
                        : (base);
  var_value = (vari_pc) ? (std::floor(static_cast<int32_t>(one_pc * vari)))
                        : (vari);

  set_value += Helpers::randU(-var_value, var_value);
  set_value  = Helpers::setInRange(set_value, 0, max_value);

  /* Actually perform the assignment operation */
  if (base_pc)
    action_target->getCurr().setStat(targ_attr, set_value);
  else
    action_target->getTemp().setStat(targ_attr, set_value);
  
  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{ASSIGN} " << action_target->getName() << "'s " 
              << AttributeSet::getName(targ_attr) << " has been assigned to "
              << set_value << "." << std::endl;
  }

  if (set_value == 0)
    done = updatePersonDeath(damage_type);

  return done;
}

/*
 * Description: 
 *
 * Inputs:
 * Output:
 */
bool Battle::processDamageAction(BattleEvent* damage_event,
    const DamageType &damage_type)
{
  auto done = false; //TODO
  auto actual_crit_factor = 1.00;
  bool crit_happens = doesActionCrit();
        
  if (crit_happens)
    actual_crit_factor = calcCritFactor();

  std::cout << "CALCULATING BASE DAMAGE " << std::endl;
  auto base_damage = calcBaseDamage(actual_crit_factor, damage_type);
  auto damage_mod   = curr_user->getDmgMod();
  auto damage = static_cast<int32_t>(base_damage * damage_mod);

  damage = Helpers::setInRange(damage, kMINIMUM_DAMAGE, kMAXIMUM_DAMAGE);
        
  if (crit_happens && getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{CRITICAL} - The strike is critical!" << std::endl;
  }

  damage_event->type    = BattleEventType::STANDARD_DAMAGE;
  damage_event->amount  = damage;
  curr_events.push_back(damage_event);

  //TODO - Check for death event
  // auto done = processDamageAmount(damage, DamageType::BASE);
  
  if (hasInfliction(Infliction::BERSERK, curr_user))
  {
    auto hitback = base_damage * Ailment::getBerserkHitbackPC();
   
    auto hitback_event = createEvent(BattleEventType::HITBACK_DAMAGE, nullptr, 
                           curr_target);
    hitback_event->happens = true;
    hitback_event->amount = hitback;
    curr_events.push_back(hitback_event);

    //TODO - Check for death event
    // done &= processDamageAmount(hitback, DamageType::HITBACK);
  }

  return done;
}

/*
 * Description: 
 *
 * Inputs:
 * Output:
 */
bool Battle::processDamageAmount(int32_t amount, DamageType damage_type)
{
  auto party_death = false;

  /* If doDmg returns true, the actor has died. Update guarding and other
   * corner cases and check for party death. Else, an actor has not died
   * but guard and defending flags, etc. may need to be recalcuted */
  auto death = curr_target->doDmg(amount, damage_type);

  auto str_dmg = "";
  if (damage_type == DamageType::BASE)
    str_dmg = "Damage";
  else if (damage_type == DamageType::HITBACK)
    str_dmg = "Hitback";

  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{" << str_dmg << "}" << curr_target->getName() 
        << " struck with " << amount << " damage.\n";

    if (death)
      std::cout << "{FALLEN} " << curr_target->getName() << " has fallen.\n\n";
  }

  if (death)
    party_death = updatePersonDeath(damage_type);
  else
    updateTargetDefense();

  return party_death;
}

/*
 * Description: 
 *
 * Inputs:
 * Output:
 */
bool Battle::processRelieveAction()
{
  //TODO
  return false;
}

/*
 * Description: 
 *
 * Inputs:
 * Output:
 */
bool Battle::processReviveAction()
{
  auto one_pc = curr_target->getTemp().getStat(Attribute::VITA);
  auto base_pc = curr_action->actionFlag(ActionFlags::BASE_PC);
  auto vari_pc = curr_action->actionFlag(ActionFlags::VARI_PC);
  int32_t base_val = 0;
  int32_t vari_val = 0;

  base_val = (base_pc) ? (one_pc * curr_action->getBase()) 
                       : (curr_action->getBase());
  vari_val = (vari_pc) ? (one_pc * curr_action->getVariance()) 
                        : (curr_action->getVariance());

  vari_val = Helpers::randU(-vari_val, +vari_val);

  curr_target->getCurr().setStat(Attribute::VITA, base_val + vari_val);

  if (curr_target->getCurr().getStat(Attribute::VITA) > 0)
    curr_target->setBFlag(BState::ALIVE, true);

  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{REVIVE} " << curr_target->getName() << " has been brought "
              << " back from KO with " 
              << curr_target->getCurr().getStat(Attribute::VITA) << " VITA.\n";
  }

  //TODO: All party target attacks should now hit the alive person.
  return false;
}

/*
 * Description: 
 *
 * Inputs:
 * Output:
 */
bool Battle::processInflictAction()
{
  if (canInflict(curr_action->getAilment()))
  {
    /* If a person is about to be bubbified, their current ailments must be
     * removed */
    if (curr_action->getAilment() == Infliction::BUBBIFY)
    {
      for (auto ill : ailments)
        if (ill->getVictim() == curr_target)
          removeAilment(ill);
    }

    addAilment(curr_action->getAilment(), curr_user, 
      curr_action->getMin(), curr_action->getMax(), curr_action->getBase());
  }
  else
  {
    if (getBattleMode() == BattleMode::TEXT)
    {
      std::cout << "{FAILED} " << curr_target->getName() << " cannot be "
          << "inflicted with infliction." << std::endl;
    }
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Battle::canInflict(Infliction test_infliction)
{
  auto can_add = ailments.size() < kMAX_AILMENTS;
  auto person_ailments = getPersonAilments(curr_target);
  
  can_add &= (person_ailments.size() < kMAX_EACH_AILMENTS);

  //TODO: Instead of not being able to inflict, reset current ill? [12-20-14]
  can_add &= !(hasInfliction(test_infliction, curr_target));

  if (test_infliction != Infliction::BUBBIFY)
    can_add &= !(curr_target->getAilFlag(PersonAilState::IS_BUBBY));

  if (can_add)
  {
    return !hasInfliction(test_infliction, curr_target);
  }

  return false;
}

bool Battle::hasInfliction(Infliction type, Person* const check)
{
  bool has_infliction = false;

  if (check == nullptr)
    return false;

  for (auto& ailment : ailments)
    if (ailment->getType() == type && ailment->getVictim() == check)
      has_infliction = true;

  return has_infliction;
}

/*
 * Description: Processes the current Skill (or Item->Skill) action selected
 *              for each action in the Skill against a vector of targets which
 *              were selected as targets of the Skill
 *
 * Inputs: std::vector<Person*> - vector of targets for processing of curr Skill
 * Output: bool - whether processing to end (win cond.) after this skill
 */
void Battle::processSkill(std::vector<Person*> targets, std::vector<DamageType>
     damage_types)
{
  if (getBattleMode() == BattleMode::TEXT)
    std::cout << "{Skill} Processing: " << curr_skill->getName() << std::endl;

  curr_skill = action_buffer->getSkill();

  /* If processing the first action, append begin skill use event */
  if (!getBattleFlag(CombatState::BEGIN_ACTION_PROCESSING))
  {
    setBattleFlag(CombatState::BEGIN_ACTION_PROCESSING, true);
    
    auto event = createEvent(BattleEventType::SKILL_USE, curr_skill, false);
    auto to_process_skill = true;

    event->user = curr_user;
    event->targets = targets;
    curr_events.push_back(event);
    
    if (curr_skill->getCooldown() > 0)
    {
      event->type = BattleEventType::SKILL_COOLDOWN;
      to_process_skill = false;
    }

    if (to_process_skill)
    {
      auto skill_hits = doesSkillHit(targets);
        
      if (skill_hits)
      {
        event->happens = true;
            
        if (hasInfliction(Infliction::BERSERK, curr_user) &&
            !curr_skill->isBerserkSkill())
        {
          auto fizzle = createEvent(BattleEventType::SKILL_USE_FIZZLE,
                                curr_user, targets);
          fizzle->skill_use = curr_skill;
          curr_events.push_back(fizzle);
          setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
        }
      }
      else if (curr_events.size() < 1)
      {
        auto miss = createEvent(BattleEventType::SKILL_MISS, curr_user, 
                            targets);
        curr_events.push_back(miss);
      }

      /* If the skill misses, go to the next action processing after render */
      if (!skill_hits)
        setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);

    }
    else
    {
      setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
    }

    std::cout << "Setting curr action index: " << curr_action_index << std::endl;
    curr_action_index = 0;
  }

  /* Else, process action index and increment the action index */
  auto effects = curr_skill->getEffects();

  /* Grab the enumerated attribute types related to the elements of the Skill */
  auto prim_stats = Helpers::elementToStats(curr_skill->getPrimary());
  auto secd_stats = Helpers::elementToStats(curr_skill->getSecondary());

  prim_off = prim_stats.first;
  prim_def = prim_stats.second;
  secd_off = secd_stats.first;
  secd_def = secd_stats.second;

  /* Update the temporary copy of the User's current stats */
  temp_user_stats     = AttributeSet(curr_user->getCurr());
  temp_user_max_stats = AttributeSet(curr_user->getTemp());

  /* Build vectors of curr and curr_max stas for each target */
  for (auto jt = begin(targets); jt != end(targets); ++jt)
  {
    temp_target_stats.push_back(AttributeSet((*jt)->getCurr()));
    temp_target_max_stats.push_back(AttributeSet((*jt)->getTemp()));
  }
 
  /* User ref. vars related to prim/secd skill attributes, -1 if Attr:NONE */
  auto prim_user_off = temp_user_stats.getStat(prim_off);
  auto prim_user_def = temp_user_stats.getStat(prim_def);
  auto secd_user_off = temp_user_stats.getStat(secd_off);
  auto secd_user_def = temp_user_stats.getStat(secd_def);

  if (curr_user->getPrimary() == curr_skill->getPrimary())
  {
    prim_user_off *= kOFF_PRIM_ELM_MODIFIER;
    prim_user_def *= kDEF_PRIM_ELM_MODIFIER;
  }
  else if (curr_user->getSecondary() == curr_skill->getSecondary())
  {
    secd_user_off *= kOFF_SECD_ELM_MODIFIER;
    secd_user_def *= kDEF_SECD_ELM_MODIFIER;
  }

  /* Assert the current action index is within effects range, then
   * compute the outcome for the effect for every target, and prepare
   * the outcomes for rendering */
  if (curr_action_index < effects.size())
  {
    curr_action = effects.at(curr_action_index);

    for (auto it = begin(targets); it != end(targets); ++it)
    {
      curr_target = *it;
      auto action = createEvent(BattleEventType::SKILL_USE,
                        curr_user, curr_target);
      action->action_use = curr_action;
      action->skill_use  = curr_skill;
        
      std::cout << "!!! Processing action !!!" << std::endl;
      processAction(action, damage_types);   
    }
  }
  else
  {
    setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
  }

        curr_action_index++;
}

/*
 * Description: Recalculates the ailments modification factors for a given
 *              Person* inflcited by all ailments. For proper reversible stat
 *              buffs, both the multiplicative effects and additive effects
 *              must be separately calculated and re-applied carefully to base
 *              stats.
 *
 * Inputs: Person* - person to recalculate ailment modification factors for 
 * Output: none
 */
void Battle::reCalcAilments(Person* const target)
{
  auto temp_vita = target->getCurr().getStat(0);
  auto temp_qtdr = target->getCurr().getStat(1);

  auto base_max_stats = target->getCurrMax();

  target->setCurr(base_max_stats);
  target->getCurr().setStat(0, temp_vita);
  target->getCurr().setStat(1, temp_qtdr);
  target->setTemp(base_max_stats);

  auto ills = getPersonAilments(target);

  for (auto ill : ills)
    if (ill->getFlag(AilState::BUFF))
      ill->apply();
}

/*
 * Description: Sets up enemy AI Modules for a new person index to call
 *              bufferEnemyAction() to actually grab the data.
 *
 * Inputs: none
 * Output: none
 */
void Battle::selectEnemyActions()
{
  auto update_module = false;

  /* If the current module selection is complete, attempt to add it into the 
   * buffer. bufferEnemyAction() will update the person index if needed */
  if (curr_module == nullptr)
  {
    if (!testPersonIndex(person_index))
      setNextPersonIndex();

    update_module = true;
  }
  else if (curr_module->getFlag(AIState::SELECTION_COMPLETE))
  {
    update_module = bufferEnemyAction();
  }

  /* Assert the person index exists in the Foes scope (-5 to -1) */
  if (update_module)
  {
    curr_user = getPerson(person_index);
    curr_module = curr_user->getAI();

    /* Reset the AI Module for a new turn decision, assign data */
    curr_user->resetAI();

    auto allies = getPersonsFromIndexes(getFriendsTargets());
    auto foes   = getPersonsFromIndexes(getFoesTargets());

    curr_module->setFriendTargets(allies);
    curr_module->setFoeTargets(foes);

    auto items = friends->getInventory()->getBattleItems();
    curr_module->setItems(buildBattleItems(person_index, items));

    auto skills = curr_user->getUseableSkills();

    auto battle_skills = buildBattleSkills(person_index, skills);
    
    if (hasInfliction(Infliction::BUBBIFY, curr_user))
      battle_skills = buildBattleSkills(person_index, bubbified_skills);

    curr_module->setSkills(battle_skills);
    curr_module->calculateAction();
  }
  else
  {
    /* Mark the enemy selection phase as complete on the max index */
    setBattleFlag(CombatState::PHASE_DONE);
  }
}

/*
 * Description: Iterates through the person indexes for users to call 
 *              bufferUserActions() for each index to actually get data.
 *
 * Inputs: none
 * Output: none
 */
void Battle::selectUserActions()
{
  auto update_menu = false;

  /* If an action has been selected for a valid person index, grab the info.
      and load it into the buffer */
  if (person_index == 1)
  {
    if (!testPersonIndex(person_index))
      setNextPersonIndex();

    update_menu = true;
  }
  if (person_index > 0 && 
      menu->getMenuFlag(MenuState::SELECTION_VERIFIED))
  {
    update_menu = bufferUserAction();
  }

  /* If a menu action has been selected, update to the next person index. If 
     the index is at the highest level, set the select user action phase done */
  if (update_menu)
  {
    curr_user = getPerson(person_index);

    if (curr_user != nullptr)
    {
      /* Construct useable battle skills or useable battle item structures */
      auto skills = curr_user->getUseableSkills();
      auto items  = friends->getInventory()->getBattleItems();

      auto battle_skills = buildBattleSkills(person_index, skills);

      if (hasInfliction(Infliction::BUBBIFY, curr_user))
        battle_skills = buildBattleSkills(person_index, bubbified_skills);

      auto battle_items  = buildBattleItems(person_index, items);

      /* Reload the menu information for the next person */
      menu->reset(curr_user, person_index);
      menu->setNumAllies(friends->getLivingMembers().size());
      menu->setSelectableSkills(battle_skills);
      menu->setSelectableItems(battle_items);
    
      if (getBattleMode() == BattleMode::TEXT && 
          hasInfliction(Infliction::CONFUSE, curr_user))
      {
        menu->printMenuState();
      }
    }
    else
    { 
      std::cerr << "[Error]: Selection action for invalid person\n";
    }
  }
  else 
  {
    /* Set the phase complete on the max person index */
    setBattleFlag(CombatState::PHASE_DONE);
  }
}

/*
 * Description: Load default configuration of the battle 
 *
 * Inputs: none
 * Output: bool - true
 */
bool Battle::setupClass()
{
  action_buffer = nullptr;
  curr_module   = nullptr;
  config        = nullptr;
  menu          = nullptr;

  ailment_update_mode = BattleOptions::FOREST_WALK;
  hud_display_mode    = BattleOptions::FOREST_WALK;
  battle_mode         = BattleMode::TEXT;
  turn_mode           = TurnMode::FRIENDS_FIRST;
  turn_state = TurnState::BEGIN;

  flags = static_cast<CombatState>(0);

  person_index           = 0;
  screen_height          = 0;
  screen_width           = 0;
  time_elapsed           = 0;
  time_elapsed_this_turn = 0;
  turns_elapsed          = 0;

  clearActionVariables();

  return true;
}

/*
 * Description: Tests whether a person index is valid. If the person index is
 *              an invalid person, if the person is alive and if the person
 *              is no set to skip their next turn, this function returns true.
 *
 * Inputs: int32_t - the person index to test if it is valid for action sel.
 * Output: bool - the validity of the person index for action selection
 */
bool Battle::testPersonIndex(const int32_t &test_index)
{
  auto test_person = getPerson(test_index);

  if (test_person->getBFlag(BState::ALIVE))
  {
    if (test_person->getAilFlag(PersonAilState::SKIP_NEXT_TURN))
    {
      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{SKIP} " << test_person->getName() << " skips their turn"
            << "." << std::endl;
      }
      
      return false;
    }

    return true;
  }
 
  return false;
}

/*
 * Description: Upkeep function which calls each personal upkeep for each 
 *              member in both friends and foes party.
 *
 * Inputs: none
 * Output: none
 */
void Battle::upkeep()
{
  if (turns_elapsed > 0)
  {
    /* Friends update */
    for (uint32_t i = 0; i < friends->getSize(); i++)
      if (friends->getMember(i)->getBFlag(BState::ALIVE))
        personalUpkeep(friends->getMember(i));

    /* Foes update */
    for (uint32_t i = 0; i < foes->getSize(); i++)
      if (foes->getMember(i)->getBFlag(BState::ALIVE))
        personalUpkeep(foes->getMember(i));
  }

  printPartyState();

  /* Personal upkeep state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/*
 * Description: Sub-function for update which holds data related to updating
 *              the battle during the ally selection phase.
 *
 * Inputs: none
 * Output: none
 */
void Battle::updateAllySelection()
{
  auto is_confused = hasInfliction(Infliction::CONFUSE, curr_user);

  if (menu->getMenuFlag(MenuState::SELECTION_VERIFIED))
  {
    selectUserActions();
  }
  else
  {
    if (is_confused && !menu->getMenuFlag(MenuState::ACTION_SELECTED))
    {
      bool valid = false;

      while (!valid)
      {
        menu->selectRandomAction();
        auto check_scope = menu->getSelectedSkill().skill->getScope();
        auto check_targets = getValidTargets(person_index, check_scope);

        std::cout << "bitch " << std::endl; 
        
        if (check_targets.size() > 0)
          valid = true;
      }
    }
    else if (is_confused && menu->getMenuFlag(MenuState::TARGETS_ASSIGNED))
      menu->setMenuFlag(MenuState::SELECTION_VERIFIED);

    auto action_type = ActionType::NONE;
     
    if (menu->isActionTypeSelected())
      action_type = menu->getActionType();

    /* If the action index has been assigned and targets have not been 
     * assigned yet (for that action index), find the scope of that action 
     * the user wishes to use and inject the valid targets into the menu */
    if ((menu->getMenuFlag(MenuState::ACTION_SELECTED)   || 
         menu->getActionType() == ActionType::GUARD      ||
         menu->getActionType() == ActionType::IMPLODE)   &&
         !(menu->getActionType() == ActionType::DEFEND)  &&
         !(menu->getActionType() == ActionType::PASS)    &&
         !(menu->getActionType() == ActionType::RUN)     &&
         !menu->getMenuFlag(MenuState::TARGETS_ASSIGNED))
    {
      auto scope = ActionScope::NO_SCOPE;
      std::vector<int32_t> targets;

      if (action_type == ActionType::SKILL)
        scope = menu->getSelectedSkill().skill->getScope();
      else if (action_type == ActionType::ITEM)
        scope = menu->getSelectedItem()->getUseSkill()->getScope();
      else if (action_type == ActionType::IMPLODE)
        scope = ActionScope::USER;

      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "Finding selectable targets for action with scope: "
                  << Helpers::actionScopeToStr(scope) << std::endl;
      }
 
      if (action_type == ActionType::SKILL)
      {
        auto selected_skill = menu->getSelectedSkill();

        std::cout << "Skill Nameb"<<selected_skill.skill->getName()<<std::endl;

        targets = getIndexesOfPersons(selected_skill.all_targets);                 
      }
      else if (action_type == ActionType::ITEM)
      {
        //auto battle_item;
      }
      else if (action_type == ActionType::GUARD)
      {
         targets = getValidTargets(person_index, ActionScope::ONE_ALLY_NOT_USER);
      }

      if (!menu->setSelectableTargets(targets))
      {
        if (getBattleMode() == BattleMode::TEXT)
        {
          std::cout << "No selectable targets found! Select another action"
                    << " index!" << std::endl;
        }
      }      
      else
      {
        menu->setMenuFlag(MenuState::TARGETS_ASSIGNED);
        menu->setActionScope(scope);
        menu->setMenuFlag(MenuState::SCOPE_ASSIGNED);
        menu->printMenuState();
      }
    }
  }
}

/*
 * Description: 
 *
 * Inputs:
 * Output: 
 */
void Battle::updateEnemySelection()
{
  if (curr_module != nullptr) 
  {
    auto action_type = curr_module->getActionType();

    if ((curr_module->getFlag(AIState::ACTION_INDEX_CHOSEN) ||
         curr_module->getActionType() == ActionType::DEFEND ||
         curr_module->getActionType() == ActionType::GUARD ||
         curr_module->getActionType() == ActionType::IMPLODE) &&
         !curr_module->getFlag(AIState::TARGETS_ASSIGNED))
    {
      auto scope = ActionScope::NO_SCOPE;
   
      if (action_type == ActionType::SKILL)
        scope = curr_module->getSelectedSkill()->getScope();
      else if (action_type == ActionType::ITEM)
        scope = curr_module->getSelectedItem()->getUseSkill()->getScope();
      else if (action_type == ActionType::DEFEND || 
               action_type == ActionType::IMPLODE ||
               action_type == ActionType::RUN ||
               action_type == ActionType::PASS)
      {
        std::cout << "Setting action type:: USER" << std::endl;
        scope = ActionScope::USER;
      }
      else if (action_type == ActionType::GUARD)
      {
        scope = ActionScope::ONE_ALLY_NOT_USER;
      }

      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "Finding selectable targets for enemy action w/ scope: "
                  << Helpers::actionScopeToStr(scope) << std::endl;
      }

      auto valid_targets = getValidTargets(person_index, scope);
      std::vector<Person*> friends_persons;
      std::vector<Person*> foes_persons;

      for (auto target : valid_targets)
      {
        if (target < 0)
          friends_persons.push_back(getPerson(target));
        else
          foes_persons.push_back(getPerson(target));
      }

      curr_module->setActionScope(scope);
      curr_module->setFriendTargets(friends_persons);
      curr_module->setFoeTargets(foes_persons);
      curr_module->setFlag(AIState::TARGETS_ASSIGNED, true);
      curr_module->calculateTargets();
    }
    else if (curr_module->getFlag(AIState::SELECTION_COMPLETE))
    {
      /* Set to the next enemy selection or finish enemy selections */
      selectEnemyActions();
    }
  }
}

/*
 * Description:
 *
 * Inputs: 
 * Output: bool - true if a party death has occured
 */
bool Battle::updatePartyDeaths()
{
  if (checkPartyDeath(friends))
  {
    setBattleFlag(CombatState::LOSS, true);
    return true;
  } 
  else if (checkPartyDeath(foes))
  {
    setBattleFlag(CombatState::VICTORY, true);
    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
bool Battle::updatePersonDeath(const DamageType &damage_type)
{
  if (getBattleMode() == BattleMode::TEXT)
  {
    std::cout << "{FALLEN} " << curr_target->getName() << " has fallen!"
              << std::endl;
  }

  /* Extra assert that the person has died */
  curr_target->setBFlag(BState::ALIVE, false);

  /* Every action performed by the recently deceased must be removed */
  action_buffer->removeAllByUser(curr_target);

  /* If a guard dies, the remaining targets must revert back to the guardee */
  if (damage_type == DamageType::GUARD)
    action_buffer->rejectGuardTargets(curr_target);

  /* Remove ailments which are cured upon death */
  for (auto& ailment : ailments)
    if (ailment->getVictim() == curr_target)
      removeAilment(ailment);

  /* Enable all skills corner case (silence cure on death) */
  auto skills =  curr_target->getCurrSkills();
  for (size_t i = 0; i < skills->getSize(); i++)
    skills->setState(i, true);

  return updatePartyDeaths();
}

/*
 * Description: Updates the state of a defending target. If they are a power
 *              defender, this means they are able to persist through further
 *              hits, but a non power defender may not.
 *
 * Inputs: 
 * Output: 
 */
bool Battle::updateTargetDefense()
{
  auto can_process = true;

  can_process &= curr_target != nullptr;
  can_process &= curr_user != nullptr;

  if (can_process)
  {
    auto defending = curr_target->getBFlag(BState::DEFENDING);

    /* If the person was defending, unless they'r pow. def., reset def status */
    if (defending && !curr_target->isPowerDefender())
    {
      /* Reset the defense information in person class */
      curr_target->resetDefend();

      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{BREAK DEFEND} " << curr_target->getName()
                  << " is no longer defending from damage.\n\n";
      }
    }
    else if (defending && curr_target->isPowerDefender())
    {
      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{PERSIST DEFEND} " << curr_target->getName() 
                  << " continues to defend themselves from damage.\n\n";
      }
    }

    auto guarding = curr_target->getBFlag(BState::GUARDING);

    if (guarding && !curr_target->isPowerGuarder())
    {
      action_buffer->rejectGuardTargets(curr_target);

      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{BREAK GUARD} " << curr_target->getName() << " is no "
                  << "longer protecting " 
                  << curr_target->getGuardee()->getName() << " from damage\n";
      }

      curr_target->getGuardee()->resetGuard();
      curr_target->resetGuardee();
    }
    else if (guarding && curr_target->isPowerGuarder())
    {
      if (getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "{PERSIST GUARD " << curr_target->getName() << " continues"
                  << " to protect " << curr_target->getGuardee()->getName()
                  << " from damage" << std::endl;
      }
    }
  }

  return can_process;
}

/*
 * Description: Assigns a new value to the ailment update mode
 *
 * Inputs: BattleOptions - new enumerated options for Battle
 * Output: none
 */
void Battle::setAilmentUpdateMode(const BattleOptions &new_value)
{
  ailment_update_mode = new_value;
}

/*
 * Description: Assigns a new value to the battle output mode
 *
 * Inputs: BattleMode - new enumerated BattleMode (TEXT/GUI)
 * Output: none
 */
void Battle::setBattleMode(const BattleMode &new_value)
{
  battle_mode = new_value;
}

/*
 * Description: Assigns the friends party of the Battle
 *
 * Inputs: Party* - pointer to friends party
 * Output: bool - true if the new pointer was not null
 */
bool Battle::setFriends(Party* const new_friends)
{
  if (new_friends != nullptr)
  {
    friends = new_friends;

    return true;
  }

  return false;
}

/*
 * Description: Assigns the foes party of the Battle
 *
 * Inputs: Party* foes - new foes party 
 * Output: bool - true if the foes party was not nullptr
 */
bool Battle::setFoes(Party* const new_foes)
{
  if (new_foes != nullptr)
  {
    foes = new_foes;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a new value to the hud display mode
 *
 * Inputs: BattleOptions - enumerated BattleOptions for hud display mode
 * Output: non
 */
void Battle::setHudDisplayMode(const BattleOptions &new_value)
{
  hud_display_mode = new_value;
}

/*
 * Description: Updates the Battle to the next state based on its present state.
 *              Refreshes the phase and action done flags.
 *
 * Inputs: none
 * Output: none
 */
void Battle::setNextTurnState()
{
 /* Set the CURRENT_STATE to incomplete */
  setBattleFlag(CombatState::PHASE_DONE, false);
  setBattleFlag(CombatState::ACTION_DONE, false);

  /* If the Battle victory/loss has been complete, go to Destruct */
  if (getBattleFlag(CombatState::OUTCOME_DONE))
  {
    setTurnState(TurnState::DESTRUCT);
    menu->unsetAll(true);
    //TODO [11-0614]: Eventhandler battle finish signal?
  }

  if (getTurnState() != TurnState::DESTRUCT)
  {
    /* If the Battle has been won, go to victory */
    if (getBattleFlag(CombatState::VICTORY))
    {
      setTurnState(TurnState::VICTORY);
      battleWon();
    }

    /* If the Battle has been lost, GAME OVER */
    if (getBattleFlag(CombatState::LOSS))
    {
      setTurnState(TurnState::LOSS);
      battleLost();
    }

    if (getBattleFlag(CombatState::ALLIES_RUN) || 
        getBattleFlag(CombatState::ENEMIES_RUN))
    {
      setTurnState(TurnState::RUNNING);
      battleRun();
    }

    /* After the Battle Begins, the general turn loop begins at General Upkeep*/
    if (turn_state == TurnState::BEGIN)
    {
      setTurnState(TurnState::GENERAL_UPKEEP);
      generalUpkeep();
    }

    /* After general upkeep, each personal upkeep takes place */
    else if (turn_state == TurnState::GENERAL_UPKEEP)
    {
      setTurnState(TurnState::UPKEEP);
      upkeep();
    }

    /* After presonal upkeeps, the first turn order party selects actions */
    else if (turn_state == TurnState::UPKEEP)
    {
      if (turn_mode == TurnMode::FRIENDS_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ALLY);

        person_index = 1;
        selectUserActions();
      }
      else if (turn_mode == TurnMode::ENEMIES_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ENEMY);

        person_index = -1;
        selectEnemyActions();
      }
    }

    /* After the user selects actions, the enemy party may still need to
       select actions, or if not, order actions is called  */
    else if (turn_state == TurnState::SELECT_ACTION_ALLY)
    {
      if (turn_mode == TurnMode::FRIENDS_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ENEMY);

        person_index = -1;
        selectEnemyActions();
      }
      else if (turn_mode == TurnMode::ENEMIES_FIRST)
      {
        setTurnState(TurnState::ORDER_ACTIONS);
        orderActions();
      }
    }

    /* After enemies select actions, the users may still need to select actios,
       or if not, order actions is called  */
    else if (turn_state == TurnState::SELECT_ACTION_ENEMY)
    {
      if (turn_mode == TurnMode::FRIENDS_FIRST)
      {
        setTurnState(TurnState::ORDER_ACTIONS);
        orderActions();
      }
      else if (turn_mode == TurnMode::ENEMIES_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ALLY);
        
        person_index = 1;
        selectUserActions();
      }
    }

    /* After the actions are ordered, the actions are processed */
    else if (turn_state == TurnState::ORDER_ACTIONS)
    {
      setTurnState(TurnState::PROCESS_ACTIONS);
      action_buffer->print(false);
    }

    /* After the actions are processed, Battle turn clean up occurs */
    else if (turn_state == TurnState::PROCESS_ACTIONS)
    {
      setTurnState(TurnState::CLEAN_UP);
      cleanUp();
    }

    /* After the end of the turn, loop restarts at general upkeep */
    else if (turn_state == TurnState::CLEAN_UP)
    {
      setTurnState(TurnState::GENERAL_UPKEEP);
      generalUpkeep();
    }
  }
}

/*
 * Description: Assigns the next person index. If In selection action enemy,
 *              finds the next person index which needs action selected. Same
 *              for selection of user actions. If no next person is needed,
 *              this function returns false.
 *
 * Inputs: none
 * Output: bool - whether the next person index was set and is valid
 */
bool Battle::setNextPersonIndex()
{
  auto valid_person = false;

  if (turn_state == TurnState::SELECT_ACTION_ENEMY)
  {
    for (int32_t i = person_index;
         (i >= (-1 * static_cast<int32_t>(foes->getSize()))) && !valid_person; 
         i--)
    {
      if ((i < person_index) && testPersonIndex(i))
      {
        person_index = i;
        valid_person = true;
      }
    }
  }
  else if (turn_state == TurnState::SELECT_ACTION_ALLY)
  {
    for (int32_t i = person_index; 
         (i <= static_cast<int32_t>(friends->getSize())) && !valid_person; 
         i++)
    {
      if ((i > person_index) && testPersonIndex(i))
      {
        person_index = i;
        valid_person = true;
      }
    }
  }

  return valid_person;
}

/*
 * Description: Assigns a new value to the elapsed time
 *
 * Inputs: int32-t new_value - new value for total time elapsed in battle
 * Output: none
 */
void Battle::setTimeElapsed(const int32_t &new_value)
{
  time_elapsed = new_value;
}

/*
 * Description: Assigns thee time elapsed this turn
 *
 * Inputs: int32_t - new value for time elapsed this turn
 * Output: none
 */
void Battle::setTimeElapsedThisTurn(const int32_t &new_value)
{
  time_elapsed_this_turn = new_value;
}

/*
 * Description: Assigns a new value to the turns elapsed
 *
 * Inputs: uint16_t new_value - the new value for the turns elapsed
 * Output: none
 */
void Battle::setTurnsElapsed(const uint16_t &new_value)
{
  turns_elapsed = new_value;
}

/*
 * Description: Assigns a new turn mode to the Battle
 *
 * Inputs: TurnMode - new enumerated turn mode (FRIENDS_FIRST, etc.)
 * Output: none
 */
void Battle::setTurnMode(const TurnMode &new_turn_mode)
{
  turn_mode = new_turn_mode;
}

/*
 * Description: Updates the turn state of the Battle. Prints it if in UDEBUG
 *
 * Inputs: TurnState - new enumerated turn state for the Battle (UPKEEP, etc.)
 * Output: none
 */
void Battle::setTurnState(const TurnState &new_turn_state)
{
  turn_state = new_turn_state;

#ifdef UDEBUG
  printTurnState();
#endif

}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Process key down events for user selection of actions/outcome
 *              of battle.
 *
 * Inputs: SDL_KeyboardEvent - the key down event
 * Output: bool - false
 */
bool Battle::keyDownEvent(SDL_KeyboardEvent event)
{
#ifdef UDEBUG
  if (!getBattleFlag(CombatState::OUTCOME_DONE))
  {
    //Helpers::flushConsole();
    
    if (event.keysym.sym == SDLK_PAUSE)
      printPartyState();
    else if (event.keysym.sym == SDLK_PRINTSCREEN)
      printTurnState();
    else if (event.keysym.sym == SDLK_HOME)
      printAll(false, true, false);
    else if (event.keysym.sym == SDLK_END)
      action_buffer->print(true);
    // else if (event.keysym.sym == SDLK_PAGEUP)
    else if (event.keysym.sym == SDLK_DELETE)
      setBattleFlag(CombatState::RENDERING_COMPLETE, true);
    else if (event.keysym.sym == SDLK_PAGEDOWN)
      printInventory(foes);
  }
  else
    std::cout << "The battle is complete!" << std::endl;
#endif

  if (menu->getWindowStatus() == WindowStatus::ON)
  {
    return menu->keyDownEvent(event);
  }

  return false;
}

/*
 * Description: Method to print information about the Battle.
 *
 * Inputs: bool simple - true if to show a simple output of Battle information
 *         bool flags - true if to print out state of all flags
 *         bool party - true if to print out party state
 * Output: none
 */
void Battle::printAll(const bool &simple, const bool &flags, const bool &party)
{
  std::cout << "==== Battle ====\n";

  if (simple)
  {

  }
  else
  {
    if (ailment_update_mode == BattleOptions::FOREST_WALK)
      std::cout << "Ailment update mode: FOREST_WALK\n";
    else if (ailment_update_mode == BattleOptions::BEARLY_DIFFICULT)
      std::cout << "Ailment update mode: BEARLY_DIFFICULT\n";
    else if (ailment_update_mode == BattleOptions::GRIZZLY)
      std::cout << "Ailment update mode: GRIZZLY\n";

    if (hud_display_mode == BattleOptions::FOREST_WALK)
      std::cout << "Hud display mode: FOREST_WALK\n";
    else if (hud_display_mode == BattleOptions::BEARLY_DIFFICULT)
      std::cout << "Hud display mode: BEARLY_DIFFICULT\n";
    else if (hud_display_mode == BattleOptions::GRIZZLY)
      std::cout << "Hud display mode: GRIZZLY\n";

    std::cout << "Friends Size: " << friends->getSize();
    std::cout << "\nFoes Size: " << foes->getSize();
    std::cout << "\nScreen Height: " << screen_height;
    std::cout << "\nScreen Width: " << screen_width;
    std::cout << "\nTime Elapsed: " << time_elapsed;
    std::cout << "\nTurns Elapsed: " << turns_elapsed;
    //std::cout << "\n# Enemy Bars: " << enemy_bars.size();
    std::cout << "\n\n";

    if (flags)
    {
      std::cout << "CONFIGURED: " << getBattleFlag(CombatState::CONFIGURED);
      std::cout << "\nFLAGS_CONFIGURED: " 
                << getBattleFlag(CombatState::FLAGS_CONFIGURED);
      std::cout << "\nPHASE_DONE: " << getBattleFlag(CombatState::PHASE_DONE);
      std::cout << "\nACTION_DONE: " << getBattleFlag(CombatState::ACTION_DONE);
      std::cout << "\nVICTORY: " << getBattleFlag(CombatState::VICTORY);
      std::cout << "\nLOSS: " << getBattleFlag(CombatState::LOSS);
      std::cout << "\nALLIES_RUN: " << getBattleFlag(CombatState::ALLIES_RUN);
      std::cout << "\nENEMIES_RUN: " << getBattleFlag(CombatState::ENEMIES_RUN);
      std::cout << "\nOUTCOME_DONE: " 
                << getBattleFlag(CombatState::OUTCOME_DONE);
      std::cout << "\nERROR_STATE: " << getBattleFlag(CombatState::ERROR_STATE);
      std::cout << "\nRANDOM_ENCOUNTER: " << getBattleFlag(CombatState::RANDOM_ENCOUNTER);
      std::cout << "\nMINI_BOSS: " << getBattleFlag(CombatState::MINI_BOSS);
      std::cout << "\nBOSS: " << getBattleFlag(CombatState::BOSS);
      std::cout << "\nFINAL_BOSS: " << getBattleFlag(CombatState::FINAL_BOSS);
      std::cout << "\nPROCESSING_SKILL: " 
                << getBattleFlag(CombatState::PROCESSING_SKILL);
      std::cout << "\nPROCESSING_ITEM: " 
                << getBattleFlag(CombatState::PROCESSING_ITEM);
      std::cout << "\nREADY_TO_RENDER: "
                << getBattleFlag(CombatState::READY_TO_RENDER);
      std::cout << "\nRENDERING_COMPLETE: " 
                << getBattleFlag(CombatState::RENDERING_COMPLETE);
      std::cout << "\nPERFORMING_COMPLETE: " 
                << getBattleFlag(CombatState::PERFORMING_COMPLETE);
      std::cout << "\nBEGIN_PROCESSING: " 
                << getBattleFlag(CombatState::BEGIN_PROCESSING);
      std::cout << "\nBEGIN ACTION PROCESSING: " 
                << getBattleFlag(CombatState::BEGIN_ACTION_PROCESSING);
      std::cout << "\nACTION PROCESSING COMPLETE: " 
                << getBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE);
      std::cout << "\n\n";
    }

    if (party)
      printPartyState();
  }

  std::cout << "==== / Battle ====\n\n";
}

void Battle::printEvents()
{
  for (const auto& event : curr_events)
  {
    std::cout << "Event Type: " << static_cast<int>(event->type) << std::endl;
    if (event->action_use != nullptr)
      std::cout << "Action Use: " << event->action_use << std::endl;
    if (event->skill_use != nullptr)
    std::cout << "Skill Use: " << event->skill_use->getName() << std::endl;
    if (event->user != nullptr)
    std::cout << "User: " << event->user->getName() << std::endl;
    
    for (const auto& target : event->targets)
      if (target != nullptr)
        std::cout << "Target: " << target->getName() << std::endl;

    std::cout << "Amount: " << event->amount << std::endl;
    std::cout << "Happens? " << event->happens << std::endl;
  }
}

/*
 * Description: Print out the etailed information for each person in friends
 *              and foes to the console.
 *
 * Inputs: none
 * Output: none
 */
void Battle::printPartyState()
{
  std::cout << "\n---- Friends ----\n";
  for (uint32_t i = 0; i < friends->getSize(); i++)
  {
    printPersonState(friends->getMember(i), 
        getIndexOfPerson(friends->getMember(i)));
  }

  std::cout << "---- Foes ----\n";
  for (uint32_t i = 0; i < foes->getSize(); i++)
    printPersonState(foes->getMember(i), getIndexOfPerson(foes->getMember(i)));
}

/*
 * Description: Prints out the state of a given person and their corresponding
 *              person index.
 *
 * Inputs: Person* - pointer to the member which to print out VITA/QTDR for
 *         int32_t - the person index of the person (only for display purposes)
 * Outputs: none
 */
void Battle::printPersonState(Person* const member, 
                              const int32_t &person_index)
{
  if (member != nullptr)
  {
    std::cout << "[" << person_index << "] - " << member->getName()
              << " [ Lv. " << member->getLevel() << " ] << \n" 
              << "VITA: " << member->getCurr().getStat(0) << "/" 
              << member->getTemp().getStat(0) << " [" 
              << std::floor(member->getVitaPercent() * 100) << "%]\n" << "QTDR: " 
              << member->getCurr().getStat(1) << "/"
              << member->getTemp().getStat(1) << " [" << std::floor(member->getQDPercent() * 100)
              << "%]\nILLS: ";
    auto person_ailments = getPersonAilments(member);

    for (auto& ailment : person_ailments)
      std::cout << Helpers::ailmentToStr(ailment->getType()) << " ";
    std::cout << "\n\n";
  }
}

/*
 * Description: Prints out the inventory information for a given Party*
 *
 * Inputs: Party* - pointer to print the inventory information for
 * Output: none
 */
void Battle::printInventory(Party* const target_party)
{
  if (target_party != nullptr && target_party->getInventory() != nullptr)
  { 
    target_party->getInventory()->print();
  }
  else
  {
    std::cout << "[Warning]: Attempting to print null party or null inventory."
              << std::endl;
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Battle::printTurnState()
{
  std::cout << "Current battle state: ";
  if (turn_state == TurnState::BEGIN)
    std::cout << "BEGIN";
  else if (turn_state == TurnState::GENERAL_UPKEEP) 
    std::cout << "GENERAL_UPKEEP";
  else if (turn_state == TurnState::UPKEEP)
    std::cout << "UPKEEP";
  else if (turn_state == TurnState::SELECT_ACTION_ALLY)
    std::cout << "SELECT_ACTION_ALLY";
  else if (turn_state == TurnState::SELECT_ACTION_ENEMY)
    std::cout << "SELECT_ACTION_ENEMY";
  else if (turn_state == TurnState::ORDER_ACTIONS)
    std::cout << "ORDER_ACTIONS";
  else if (turn_state == TurnState::PROCESS_ACTIONS)
    std::cout << "PROCESS_ACTIONS";
  else if (turn_state == TurnState::CLEAN_UP)
    std::cout << "CLEAN_UP";
  else if (turn_state == TurnState::LOSS)
    std::cout << "LOSS";
  else if (turn_state == TurnState::VICTORY)
    std::cout << "VICTORY";
  else if (turn_state == TurnState::RUNNING)
    std::cout << "RUNNING";
  else if (turn_state == TurnState::DESTRUCT)
    std::cout << "DESTRUCT";
  std::cout << std::endl;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Battle::update(int32_t cycle_time)
{
  setTimeElapsed(cycle_time);

  // GUIupdate(); //TODO [11-06-14] Update the battle interface

  if (getBattleFlag(CombatState::PHASE_DONE) && 
      !getBattleFlag(CombatState::OUTCOME_DONE))
  {
    setNextTurnState();
  }

  /* During menu, if an action type has been chosen, inject valid targets for
   * the scope of the particular action type [if required] */
  else if (turn_state == TurnState::SELECT_ACTION_ALLY)
  {
    updateAllySelection();
  }
  else if (turn_state == TurnState::SELECT_ACTION_ENEMY)
  {
    updateEnemySelection();
  }
  else if (turn_state == TurnState::PROCESS_ACTIONS)
  {
    /* If rendering is complete -> perform events on the stack and then
     * continue processing battle events */
    if (getBattleFlag(CombatState::RENDERING_COMPLETE))
    {
      setBattleFlag(CombatState::RENDERING_COMPLETE, false);

      processBuffer();
      printEvents();
      performEvents();
      clearEvents();
    }
  }
  else if (turn_state == TurnState::RUNNING)
  {

  }
  else if (turn_state == TurnState::DESTRUCT)
  {
    //TODO [11-19-14] EventHandlers - return state to map or wherever nicely
    return false;
  }
  
  return false;
}

/*
 * Description: Returns the ailment update mode currently set
 *
 * Inputs: none
 * Output: BattleOptions - enumerated BattleOptions for ailment update mode
 */
BattleOptions Battle::getAilmentUpdateMode()
{
  return ailment_update_mode;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
BattleMenu* Battle::getBattleMenu()
{
  return menu;
}

/*
 * Description: Returns the assigned Battle display mode
 *
 * Inputs: none
 * Output: BattleMode - enumerated mode of Battle (TEXT/GUI)
 */
BattleMode Battle::getBattleMode()
{
  return battle_mode;
}

/*
 * Description: Return the value of a given CombatState flag
 *
 * Inputs: CombatState test_flag - flag which to find the value for
 * Output: bool - boolean value of the given flag
 */
bool Battle::getBattleFlag(const CombatState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Return the value of a given IgnoreState flag
 *
 * Inputs: IgnoreState - flag which to find the value for
 * Output: bool - boolean value of the given flag
 */
bool Battle::getIgnoreFlag(const IgnoreState &test_flag)
{
  return static_cast<bool>((ignore_flags & test_flag) == test_flag);
}

/*
 * Description: Returns the friends pointer of the Battle
 *
 * Inputs: none
 * Output: Party* - pointer to the Friends party
 */
Party* Battle::getFriends()
{
  return friends;
} 

/*
 * Description: Returns the foes pointer of the Battle
 *
 * Inputs: none
 * Output: Party* - pointer to the Foes party
 */
Party* Battle::getFoes()
{
  return foes;
}

/*
 * Description: Returns the hud display mode currently set
 *
 * Inputs: none
 * Output: BattleOptions - enumerated Battle options set by running config
 */
BattleOptions Battle::getHudDisplayMode()
{
  return hud_display_mode;
}

/*
 * Description: Returns the value of the screen height 
 *
 * Inputs: none
 * Output: uint32_t - the set height of the screen
 */
uint32_t Battle::getScreenHeight()
{
  return screen_height;
}

/*
 * Description: Returns the value of the screen width.
 *
 * Inputs: none
 * Output: uint32_t - the set width of the screen
 */
uint32_t Battle::getScreenWidth()
{
  return screen_width;
}

/*
 * Description: Evaluates and returns a vector of ailments for a given person.
 *
 * Inputs: Person* - person pointer to find all the ailment pointers for
 * Output: std::vector<Ailment*> - vector of ailments inflicting the Person
 */
std::vector<Ailment*> Battle::getPersonAilments(const Person* const target)
{
  std::vector<Ailment*> person_ailments;

  for (const auto& ailment : ailments)
    if (ailment->getVictim() == target && target!= nullptr)
      person_ailments.push_back(ailment);

  return person_ailments;
}

/*
 * Description: Returns the value of the turns elapsed. Notice that the current
 *              turn number is turns elapsed + 1.
 *
 * Inputs: none
 * Output: uint32_t - the total number of turns elapsed completely
 */
uint32_t Battle::getTurnsElapsed()
{
  return turns_elapsed;
}

/*
 * Description: Returns the elapsed time of the Battle.
 *
 * Inputs: none
 * Output: uint32_t - the amount of time that has been elapsed
 */
uint32_t Battle::getTimeElapsed()
{
  return time_elapsed;
}

/*
 * Description: Returns the enumerated turn state of the Battle
 *
 * Inputs: none
 * Output: TurnState - enumerated turn state of the Battle
 */
TurnState Battle::getTurnState()
{
  return turn_state;
}

/*
 * Description: Returns the index integer of a a given Person ptr, or 0 if no
 *              pointer 
 *
 * Inputs: Person* - returns the corresponding index of a given Person pointer
 * Output: int32_t - the corresponding index of the given pointer
 */
int32_t Battle::getTarget(Person* battle_member)
{
  for (uint32_t i = 0; i < friends->getSize(); i++)
    if (friends->getMember(i) == battle_member)
      return static_cast<int32_t>(i) + 1;

  for (uint32_t i = 0; i < foes->getSize(); i++)
    if (foes->getMember(i) == battle_member)
      return static_cast<int32_t>((i * -1) -1);

  return 0;
}

/*
 * Description: Converts an index for a person to the corresponding Person*,
 *              or returns nullptr if there is no matching Person*
 *
 * Inputs: index - the index given for wanting the corresponding Person ptr
 * Output: Person* - pointer for the given index
 */
Person* Battle::getPerson(const int32_t &index)
{
  if (index < 0)
  {
    if ((index + static_cast<int32_t>(foes->getSize())) > -1)
      return foes->getMember((index * -1) - 1);
  }
  else if (index > 0)
  {
    if (index - 1 < static_cast<int32_t>(friends->getSize()))
      return friends->getMember(index - 1);
  }

  return nullptr;
}

/*
 * Description: Compiles a vector of all possible targets in the current Battle
 *              from both the friends and foes party.
 *
 * Inputs: none
 * Output: std::vector<int32_t> - indexes of all possible targets
 */
std::vector<int32_t> Battle::getAllTargets()
{
  auto all_targets =  getFriendsTargets();
  auto foes_targets = getFoesTargets();

  all_targets.insert(end(all_targets), begin(foes_targets), end(foes_targets));

  return all_targets;
}

/*
 * Description: Obtains all friendly battle member indexes. Will grab all living
 *              members or all dead members of the Foes party.
 *
 * Inputs: ko - whether to grab dead members. If false, will grab living members
 * Output: std::vector<int32_t> - indexes of needed persons in the Friends party
 */
std::vector<int32_t> Battle::getFriendsTargets(const bool &ko)
{
  std::vector<int32_t> friends_targets;

  for (uint32_t i = 0; i < friends->getSize(); i++)
  {
    auto member       = friends->getMember(i);
    auto valid_target = false;

    valid_target |= (ko  && !member->getBFlag(BState::ALIVE));
    valid_target |= (!ko && member->getBFlag(BState::ALIVE));
    
    if (valid_target)
      friends_targets.push_back(getTarget(friends->getMember(i)));
  }

  return friends_targets;
}

/*
 * Description: Obtains all unfriendly battle member indexes. Will grab all
 *              living members or all dead members in the Foes party.
 *
 * Inputs: ko - whether to grab dead members. If false, will grab living members
 * Output: std::vector<int32_t> - indexes of needed persons in the Foes party
 */
std::vector<int32_t> Battle::getFoesTargets(const bool &ko)
{
  std::vector<int32_t> foes_targets;

  for (uint32_t i = 0; i < foes->getSize(); i++)
  {
    auto member       = foes->getMember(i);
    auto valid_target = false;

    valid_target |= (ko && !member->getBFlag(BState::ALIVE));
    valid_target |= (!ko && member->getBFlag(BState::ALIVE));

    if (valid_target)
      foes_targets.push_back(getTarget(foes->getMember(i)));
  }

  return foes_targets;
}

/*
 * Description: Obtains a vector of targets indexes for all members of a party
 *              given the index of one member of the party. This is trivial
 *              because all members of the same party will have the same signage
 *              (positive or negtaive).
 *
 * Inputs: int32_t check_index - a member of the party to find all targets for
 * Output: std::vector<int32_t> - constructed vector of all party indexes
 */
std::vector<int32_t> Battle::getPartyTargets(int32_t check_index)
{
  if (check_index < 0 )
    return getFoesTargets();
  else
    return getFriendsTargets();

  return getFriendsTargets();
}

/*
 * Description: Function for converting integer person indexes into their
 *              respective Person pointers.
 *
 * Inputs: std::vector<int32_t> - vector of integer person indexes
 * Output: std::vector<Person*> - vector of corresponding Person pointers
 */
std::vector<Person*> Battle::getPersonsFromIndexes(std::vector<int32_t> indexes)
{
  std::vector<Person*> persons;

  for (auto it = begin(indexes); it != end(indexes); ++it)
    if (getPerson(*it) != nullptr)
      persons.push_back(getPerson(*it));

  return persons;
}

/*
 * Description: Function for converting Person pointers to their respective
 *              person indexes.
 *
 * Inputs: std::vector<Person*> - vector of Person pointers to find indexes of
 * Output: std::vector<int32_t> - vector of corresponding integer indexes
 */
std::vector<int32_t> Battle::getIndexesOfPersons(std::vector<Person*> persons)
{
  std::vector<int32_t> indexes;

  for (auto it = begin(persons); it != end(persons); ++it)
  {
    auto p_index = getIndexOfPerson(*it);

    if (p_index != 0)
      indexes.push_back(p_index);
  }

  return indexes;
}

/*
 * Description: Determines a person index value given a Person* to find it for.
 *
 * Inputs: Person* check_person - the person pointer to find the index of
 * Output: int32_t - the corresponding integer person index
 */
int32_t Battle::getIndexOfPerson(Person* check_person)
{
  /* For Friends members, their person index is party_index + 1  (1 - 5) */
  for (uint32_t i = 0; i < friends->getSize(); i++)
    if (check_person == friends->getMember(i))
      return static_cast<int32_t>(i + 1);
  
  /* For Foes members, their index is (-1 * party_index) - 1) (-1 to -5) */
  for (uint32_t i = 0; i < foes->getSize(); i++)
    if (check_person == foes->getMember(i))
      return (static_cast<int32_t>(i) * -1) - 1;

  return 0;
}

/*
 * Description: Obtains a vector of battle member indexes for a given user and 
 *              scope. This checks all targets which the user can target
 *              based on the scope. (ex. NOT_USER is everybody but the user).
 *
 * Inputs: int32_t index - the person index for the user to check targets for
 *         ActionScope action_scope - the scope relating to targets
 * Output: std::vector<int32_t> - constructed vector of valid person targets
 */
std::vector<int32_t> Battle::getValidTargets(int32_t index, 
    ActionScope action_scope)
{
  std::vector<int32_t> valid_targets;

  if (action_scope == ActionScope::USER)
  {
    std::cout << "Finding valid targets for: " << index << std::endl;
    valid_targets.push_back(index);
  }
  else if (action_scope == ActionScope::ONE_TARGET  ||
           action_scope == ActionScope::ALL_TARGETS ||
           action_scope == ActionScope::ONE_PARTY)
  {
    valid_targets = getAllTargets();
  }
  else if (action_scope == ActionScope::NOT_USER ||
           action_scope == ActionScope::ALL_NOT_USER)
  {
    valid_targets = getAllTargets();
    std::remove(begin(valid_targets), end(valid_targets), index);
    valid_targets.pop_back();
  }
  else if (action_scope == ActionScope::ONE_ALLY_NOT_USER)
  {
    if (index > 0)
      valid_targets = getFriendsTargets();
    else if (index < 0)
      valid_targets = getFoesTargets();

    std::remove(begin(valid_targets), end(valid_targets), index);
    valid_targets.pop_back();
  }
  else if (action_scope == ActionScope::ONE_ENEMY   ||
           action_scope == ActionScope::TWO_ENEMIES ||
           action_scope == ActionScope::ALL_ENEMIES)
  {
    if (index > 0)
      valid_targets = getFoesTargets();
    else if (index < 0)
      valid_targets = getFriendsTargets();
  } 
  else if (action_scope == ActionScope::ONE_ALLY   ||
           action_scope == ActionScope::TWO_ALLIES ||
           action_scope == ActionScope::ALL_ALLIES)
  {
    if (index > 0)
      valid_targets = getFriendsTargets();
    else if (index < 0)
      valid_targets = getFoesTargets();
  }
  else if (action_scope == ActionScope::ONE_ALLY_KO ||
           action_scope == ActionScope::ALL_ALLIES_KO)
  {
    if (index > 0)
      valid_targets = getFriendsTargets(true);
    else if (index < 0)
      valid_targets = getFoesTargets(true);
  }
  
  return valid_targets;
}

/*
 * Description: Assigns the running config. If the new config is not null, 
 *              will call functions to assign screen width, enumerated display
 *              modes based upon the option and turns the configured flag
 *              to true.
 *
 * Inputs: Options* - pointer to running config to be assigned
 * Outputs: bool - true if the new configuration was asssigned
 */
bool Battle::setConfiguration(Options* const new_config)
{
  if (new_config != nullptr)
  {
    config = new_config;

    if (menu != nullptr)
      menu->setConfiguration(new_config);

    setAilmentUpdateMode(config->getAilmentUpdateState());
    setHudDisplayMode(config->getBattleHudState());
    setBattleMode(config->getBattleMode());

    setBattleFlag(CombatState::CONFIGURED, true);
    return true;
  }

  return false;
}

/*
 * Description: Assign a value to a CombatState flag
 *
 * Inputs: CombatState flag - flag to assign value to
 *         set_value - boolean value to assign to given flag
 * Output: noneproc
 */
void Battle::setBattleFlag(CombatState flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description: Assign a value to an IgnoreState flag
 *
 * Inputs: IgnoreState flag - flag to assign value to
 *         set_value - boolean value to assign to given flag
 * Output: none
 */
void Battle::setIgnoreFlag(IgnoreState flag, const bool &set_value)
{
  (set_value) ? (ignore_flags |= flag) : (ignore_flags &= ~flag);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/* Public static gets for menu constant values */
uint32_t Battle::getGenUpkeepDelay()
{
  return kGENERAL_UPKEEP_DELAY;
}

uint32_t Battle::getBattleMenuDelay()
{
  return kBATTLE_MENU_DELAY;
}

uint32_t Battle::getMaxAilments()
{
  return kMAX_AILMENTS;  
}

uint32_t Battle::getMaxEachAilments()
{
  return kMAX_EACH_AILMENTS;
}

uint32_t Battle::getMaxDamage()
{
  return kMAXIMUM_DAMAGE;
}

uint32_t Battle::getMinDamage()
{
  return kMINIMUM_DAMAGE;
}

float Battle::getOffPrimElmMod()
{
  return kOFF_PRIM_ELM_MODIFIER;
}

float Battle::getDefPrimElmMod()
{
  return kDEF_PRIM_ELM_MODIFIER;
}

float Battle::getOffSecdElmMod()
{
  return kOFF_SECD_ELM_MODIFIER;
}

float Battle::getDefSecdElmMod()
{
  return kDEF_SECD_ELM_MODIFIER;
}

float Battle::getDodgeMod()
{
  return kDODGE_MODIFIER;
}

float Battle::getDogePerLvlMod()
{
  return kDODGE_PER_LEVEL_MODIFIER;
}

float Battle::getPrimElmAdvMod()
{
  return kPRIM_ELM_ADV_MODIFIER;
}

float Battle::getPrimElmDisMod()
{
  return kPRIM_ELM_DIS_MODIFIER;
}

float Battle::getSecdElmAdvMod()
{
  return kSECD_ELM_ADV_MODIFIER;
}

float Battle::getSecdElmDisMod()
{
  return kSECD_ELM_DIS_MODIFIER;
}

float Battle::getDoubleElmAdvMod()
{
  return kDOUBLE_ELM_ADV_MODIFIER;
}

float Battle::getDoubleElmDisMod()
{
  return kDOUBLE_ELM_DIS_MODIFIER;
}
