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
 * CONSTANT
 *============================================================================*/

/* ------------ Menu Constants --------------- */
const uint16_t Battle::kGENERAL_UPKEEP_DELAY = 500; /* Time prior info bar */
const uint16_t Battle::kBATTLE_MENU_DELAY    = 400; /* Personal menu delay */

/* ------------ Battle Damage Calculation Modifiers ---------------
 *
 * Offensive Primary Element Modifier
 * Defensive Primary Element Modifier
 * Offensive Secondary Element Modifier
 * Defensive Secondary Element Modifier

 * Base Critical Hit Chance [Unbearability] Modifier
 * Dodge Chance [Limbertude] Modifier
 * Dodge Chance [Limbertude] Per Level Modifier
 * Primary Elemental Advantage Modifier
 * Primary Elemental Disadvantage Modifier
 * Secondary Elemental Advantage Modifier
 * Secondary Elemental Disadvantage Modifier
 * Double Elemental Advantage Modifier
 * Double Elemental Disadvantage Modifier
 */
const uint16_t Battle::kMAX_AILMENTS             =    50;
const uint16_t Battle::kMAX_EACH_AILMENTS        =     5;
const uint16_t Battle::kMINIMUM_DAMAGE           =     1;
const uint16_t Battle::kMAXIMUM_DAMAGE           = 29999;

const float    Battle::kOFF_PHYS_MODIFIER           = 1.00;
const float    Battle::kDEF_PHYS_MODIFIER           = 1.00;
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
const float    Battle::kMANNA_DEF_MODIFIER          =  1.00;
const float    Battle::kUSER_POW_MODIFIER           =  4.00;
const float    Battle::kTARG_DEF_MODIFIER           =  2.90;

const float    Battle::kBASE_CRIT_CHANCE            =   0.10;
const float    Battle::kOFF_CRIT_FACTOR             =   1.25;
const float    Battle::kCRIT_MODIFIER               = 0.0008;
const float    Battle::kCRIT_LVL_MODIFIER           =  0.012;
const float    Battle::kDODGE_MODIFIER              =   1.10;
const float    Battle::kDODGE_PER_LEVEL_MODIFIER    =   1.04;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
/* Constructs a party given two parties and configured options */
Battle::Battle(Options* running_config, Party* const friends, Party* const foes)
    : friends(friends)
    , foes(foes)
{
  if (checkAIModules())
  {
#ifdef UDEBUG
    std::cout << "AI Modules configuration good!" << std::endl;
#endif
  }
  else
  {
    std::cerr << "[Error] Crtical Error: AI Moudles for enemies bad!"
              << std::endl;
  }

  setupClass();
  determineTurnMode();
  loadBattleStateFlags();

  // info_bar = new BattleInfg, this);
  
  // status_bar = new BattleStatusBar(getFriends(), getScreenWidth(), 
  //                                  getScreenHeight(), this);
  action_buffer = new Buffer();
  menu          = new BattleMenu(running_config);

  setConfiguration(running_config);
  setBattleFlag(CombatState::PHASE_DONE, true);
}

/*
 * Description:
 */
Battle::~Battle()
{
  for (auto it = begin(ailments); it != end(ailments); ++it)
    if ((*it).second != nullptr)
      delete (*it).second;
  ailments.clear();

  // if (info_bar != nullptr)
  //   delete info_bar;
  // info_bar = nullptr;

  if (menu != nullptr)
    delete menu;
  menu = nullptr;

  // if (status_bar != nullptr)
  //   delete status_bar;
  // status_bar = nullptr;

  if (action_buffer != nullptr)
    delete action_buffer;
  action_buffer = nullptr;

  // for (auto it = begin(enemy_bars); it != end(enemy_bars); ++it)
  //   if ((*it) != nullptr)
  //     delete (*it);
  // enemy_bars.clear();

  // if (status_bar_bg != nullptr)
  //   delete status_bar_bg;
  // status_bar_bg = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add an ailment to the vector of ailments
 *
 * Inputs:
 * Output:
 */
bool Battle::addAilment(Ailment* const new_ailment)
{
  auto can_add = ailments.size() < kMAX_AILMENTS;
  auto person_ailments = getPersonAilments(new_ailment->getVictim()).size();
  can_add &= person_ailments < kMAX_EACH_AILMENTS;

  if (can_add)
  {
    auto vic_name = new_ailment->getVictim()->getName();
    auto ail_name = static_cast<int32_t>(new_ailment->getType());

#ifdef UDEBUG
    std::cout << "Inflicting ailment: " << ail_name + " on " + vic_name 
              << "\n";
#else
  if (battle_mode == BattleMode::TEXT)
    std::cout << "Inflicting ailment: " << ail_name << " on " << vic_name 
              << "\n";
#endif
  
    //TODO: Add ailment infliction to Info Bar [03-16-14]
  }

  return can_add;
}

/*
 * Description: Called when the Battle has been won
 *
 * Inputs:
 * Outputs:
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

#ifdef UDEBUG
  std::cout << "Battle victorious! :-)\n";
#endif

  setBattleFlag(CombatState::OUTCOME_DONE);
  setNextTurnState();
}

/*
 * Description: Called when the Battle has been lost
 *
 * Inputs:
 * Outputs:
 */
void Battle::battleLost()
{
  // return to title?

#ifdef UDEBUG
  std::cout << "Battle lost! :-(\n";
#endif
  setBattleFlag(CombatState::OUTCOME_DONE);
  setNextTurnState();
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
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

    if (config->getBattleMode() == BattleMode::GUI)
    {
      //TODO: Battle Front End [08-01-14]
    }
  }
  else if (action_type == ActionType::ITEM)
  {
    curr_item = curr_module->getSelectedItem();
    buffered = action_buffer->add(curr_user, curr_item, action_targets, 0);

    if (buffered)
    {
      if (config->getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "Removing " << curr_item->getName() << " from "
                  << "inventory and implementing to buffer." << std::endl;
      }
      else if (buffered && config->getBattleMode() == BattleMode::GUI)
      {
        //TODO: Battle Front End [08-01-14]
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
    std::cout << "[Error]: Action buffer addition failure!" << std::endl;   
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
bool Battle::bufferUserAction()
{
  auto buffered       = false;
  auto action_type    = menu->getActionType();
  auto action_targets = menu->getActionTargets();

  curr_user     = getPerson(person_index);

  /* Build the vector Person ptrs from the target index vector */
  std::vector<Person*> person_targets;

  for (auto it = begin(action_targets); it != end(action_targets); ++it)
    person_targets.push_back(getPerson(*it));
        
  /* Push the actions on to the Buffer */
  if (action_type == ActionType::SKILL)
  {
    curr_skill = menu->getSelectedSkill();
    buffered = action_buffer->add(curr_user, curr_skill, person_targets, 0);
  }
  else if (action_type == ActionType::ITEM)
  {
    curr_item = menu->getSelectedItem();
    buffered = action_buffer->add(curr_user, curr_item, person_targets, 0);

    if (buffered)
    {
      if (config->getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "Removing " << curr_item->getName() << " from "
                  << "inventory and adding to buffer." << std::endl;
      }
      else if (config->getBattleMode() == BattleMode::GUI)
      {
        //TODO: Battle Front End [08-01-14]
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
    std::cout << "[Error]: Invalid action selected\n";
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
    std::cout << "[Error]: Action buffer addition failure!" << std::endl;   
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
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
 * Description:
 *
 * Inputs:
 * Outputs:
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
      new_battle_skill.skill           = (*it).skill;
      new_battle_skill.all_targets     = all_targets;
      new_battle_skill.ally_targets = friends_targets;
      new_battle_skill.foe_targets    = foes_targets;

      battle_skills.push_back(new_battle_skill);
    }
  }

  return battle_skills;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
int32_t Battle::calcBaseDamage(const float &crit_factor)
{
  auto targ_attrs = temp_target_stats.at(p_target_index);

  int32_t base_user_pow = 0;
  int32_t base_targ_def = 0;

  int32_t phys_pow_val  = 0;
  int32_t phys_def_val  = 0;
  int32_t elm1_pow_val  = 0;
  int32_t elm1_def_val  = 0;
  int32_t elm2_pow_val  = 0;
  int32_t elm2_def_val  = 0;
  int32_t luck_pow_val  = 0;
  int32_t luck_def_val  = 0;
  
  calcElementalMods();

  /* Always calculate physical power into the equation */
  phys_pow_val = temp_user_stats.getStat(Attribute::PHAG);
  phys_pow_val *= kOFF_PHYS_MODIFIER;

  phys_def_val = temp_user_stats.getStat(Attribute::PHFD);
  phys_def_val *= kDEF_PHYS_MODIFIER;

  /* Primary elemental affiliation bonuses */
  if (curr_skill->getPrimary() != Element::NONE)
  {
    if (curr_user->getPrimary() == curr_skill->getPrimary() ||
       curr_user->getPrimary() == curr_skill->getSecondary())
    {
      elm1_pow_val  = temp_user_stats.getStat(prim_off);
      elm1_pow_val *= kOFF_PRIM_ELM_MATCH_MODIFIER;
    }
    
    if (curr_target->getPrimary() == curr_skill->getPrimary() ||
        curr_target->getPrimary() == curr_skill->getSecondary())
    {
      elm1_pow_val  = targ_attrs.getStat(prim_def);
      elm2_def_val *= kDEF_PRIM_ELM_MATCH_MODIFIER;
    }
  }
 
  /* Secondary elemental affiliation bonuses */
  if (curr_skill->getSecondary() != Element::NONE)
  {
    if (curr_user->getSecondary() == curr_skill->getPrimary() ||
        curr_user->getSecondary() == curr_skill->getSecondary())
    {
      elm1_pow_val  = temp_user_stats.getStat(secd_off);
      elm2_pow_val *= kOFF_SECD_ELM_MATCH_MODIFIER;
    }

    if (curr_target->getSecondary() == curr_skill->getPrimary() ||
        curr_target->getSecondary() == curr_skill->getSecondary())
    {
      elm2_pow_val  = targ_attrs.getStat(secd_def);
      elm2_def_val *= kDEF_SECD_ELM_MATCH_MODIFIER;
    }
  }

  /* Additional bonuses - luck power/defense values */
  luck_pow_val  = temp_user_stats.getStat(Attribute::MANN);
  luck_pow_val *= kMANNA_POW_MODIFIER;

  luck_def_val  = targ_attrs.getStat(Attribute::MANN);
  luck_def_val *= kMANNA_DEF_MODIFIER;

  /* Summation of base power / defense */
  base_user_pow  = phys_pow_val + elm1_pow_val + elm2_pow_val + luck_pow_val;
  base_user_pow *= kUSER_POW_MODIFIER;

  base_targ_def  = phys_def_val + elm1_def_val + elm2_def_val + luck_def_val;
  base_targ_def *= kTARG_DEF_MODIFIER;

  auto base_damage = base_user_pow - base_targ_def;
  base_damage     *= crit_factor;

// #ifdef UDEBUG
//   std::cout << "Base damage calculation: " << base_damage << std::endl;
// #endif

  /* Addition of the power of the action */
  auto action_damage = curr_action->getBase();

  if (curr_action->actionFlag(ActionFlags::BASE_PC))
  {
    auto base_pc = static_cast<float>(action_damage) / 100;
    action_damage = base_pc * temp_user_stats.getStat(Attribute::VITA);
  }

  base_damage += action_damage;

  /* Addition of the variance of the action */
  auto base_var   = curr_action->getVariance();
  int32_t var_val = 0;

  if (curr_action->actionFlag(ActionFlags::VARI_PC))
  {
    auto var_pc = static_cast<float>(base_var) / 100;
    var_val   = var_pc * base_damage;
  }
  else
  {
    var_val = base_var;
  }

  base_damage = Helpers::randU(base_damage - var_val, base_damage + var_val);
  
// #ifdef UDEBUG
//   std::cout << "Final damage calculation: " << base_damage << std::endl;
// #endif

  return base_damage;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
void Battle::calcElementalMods()
{
  /* Grab the temp attribute set for the curent processing target index */
  auto targ_attrs = temp_target_stats.at(p_target_index);

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
  else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
  {
    //TODO: Elemental modifiers for defensive skills?
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
      prim_user_mod *= kPRIM_ELM_ADV_MODIFIER;
    }
    /* User is strong in secondary elemental case */
    else if (!prim_weakness && secd_weakness)
    {
      secd_user_mod *= kSECD_ELM_ADV_MODIFIER;
    }
    /* User is strong in both elemental cases */
    else if (prim_weakness && secd_weakness)
    {
      prim_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_user_mod *= kDOUBLE_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in primary elemental case */
    else if (prim_strength && !secd_strength)
    {
      prim_targ_mod *= kPRIM_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in secondary elemental case */
    else if (!prim_strength && secd_strength)
    {
      secd_targ_mod *= kSECD_ELM_ADV_MODIFIER;
    }
    /* Opponent is strong in both elemental cases */
    else if (prim_strength && secd_strength)
    {
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
float Battle::calcCritFactor()
{
  //TODO: Crit factor calculation
  return 1.00;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
int16_t Battle::calcLevelDifference()
{
  return curr_user->getLevel() - curr_target->getLevel();
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
bool Battle::canIncrementIndex(Person* check_person)
{
  if ((check_person->getBFlag(BState::SELECTED_ACTION) && 
      !check_person->getBFlag(BState::TWO_SKILLS) && 
      !check_person->getBFlag(BState::THREE_SKILLS)) ||
      (check_person->getBFlag(BState::SELECTED_2ND_ACTION) &&
       check_person->getBFlag(BState::TWO_SKILLS) &&
      !check_person->getBFlag(BState::THREE_SKILLS)) ||
      (check_person->getBFlag(BState::SELECTED_3RD_ACTION) &&
       check_person->getBFlag(BState::THREE_SKILLS)))
  {
    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
void Battle::clearActionVariables()
{
  prim_off = Attribute::NONE;
  prim_def = Attribute::NONE;
  secd_off = Attribute::NONE;
  secd_def = Attribute::NONE;

  temp_user_stats     = AttributeSet();
  temp_user_max_stats = AttributeSet();

  temp_target_stats.clear();
  temp_target_max_stats.clear();

  curr_user   = nullptr;
  curr_target = nullptr;
  curr_action = nullptr;
  curr_skill  = nullptr;
  curr_item   = nullptr;

  p_target_index = 0;
}

/*
 * Description: Returns enumeration of party death [None, Friends, Foes, Both]
 *
 * Inputs:
 * Outputs:
 */
bool Battle::checkPartyDeath(Party* const check_party)
{
  if (check_party->getLivingMembers().size() == 0)
    return true;

  return false;
}

/*
 * Description: Cleanup before the end of a Battle turn
 *
 * Inputs:
 * Outputs:
 */
void Battle::cleanUp()
{
  action_buffer->clearAll();
  person_index = 0;
 
  /* Clean all action processing related variables */
  clearActionVariables();

  /* Increment the turn counter */
  turns_elapsed++;

  action_buffer->update();
  menu->unsetAll();

  setBattleFlag(CombatState::PHASE_DONE, true);

  // TODO: Auto win turns elapsed [03-01-14]
  if (turns_elapsed == 7)
    setBattleFlag(CombatState::VICTORY);
}

/*
 * Description: Determines the turn progression of the Battle (based on speed)
 *
 * Inputs:
 * Outputs:
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
 *              current target.
 *
 * Inputs:  none
 * Outputs: bool - true if the current action will crit against the curr target.
 */
bool Battle::doesActionCrit()
{
  auto crit_possible = true;
  auto crit_happens  = false;

  crit_possible &= curr_user->getBFlag(BState::CAN_CRIT);
  crit_possible &= curr_target->getBFlag(BState::CAN_BE_CRIT);

  if (crit_possible)
  {
    auto critical_chance = kBASE_CRIT_CHANCE;
    auto crit_mod = (temp_user_stats.getStat(Attribute::UNBR) * kCRIT_MODIFIER);
    auto crit_lvl_mod = calcLevelDifference() * kCRIT_LVL_MODIFIER;

    critical_chance += crit_mod + crit_lvl_mod;

    if (critical_chance > 0)
    {
      uint32_t crit_pc_1000 = floor(critical_chance * 1000);
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
 * Inputs:  none
 * Outputs: bool - true if the current skill will entirely miss
 */
bool Battle::doesSkillMiss()
{
  auto skill_hits = true;

  skill_hits &= curr_skill->isValid();
  skill_hits &= curr_user->getBFlag(BState::SKL_ENABLED);
  skill_hits &= curr_user->getBFlag(BState::MISS_NEXT_TARGET);

  return skill_hits;
}

/*
 * Description: Determines whether the current action will miss its current
 *              target. 
 *
 * Inputs:  none
 * Outputs: bool - true if the current action will miss the current target
 */
bool Battle::doesActionMiss()
{
  auto action_happens = true;

  auto miss_ch = temp_target_stats.at(p_target_index).getStat(Attribute::LIMB);
  miss_ch *= kDODGE_MODIFIER;

  miss_ch += (-calcLevelDifference()) * kDODGE_PER_LEVEL_MODIFIER;

  if (miss_ch > 0)
  {
    uint32_t miss_pc_1000 = floor(miss_ch * 1000);
    if (Helpers::chanceHappens(miss_pc_1000, 1000))
      action_happens = false;
  }

  return action_happens;
}

/*
 * Description: Deals with general upkeep (i.e. weather)
 *
 * Inputs:
 * Outputs:
 */
void Battle::generalUpkeep()
{
#ifdef UDEBUG
  printPartyState();
#else 
  if (battle_mode == BattleMode::TEXT)
    printPartyState();
#endif

  //TODO: Weather updates [03-01-14]

  /* General upkeep phase complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/*
 * Description: Sets the flags of BattleState at the beginning of the Battle
 *
 * Inputs:
 * Outputs:
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
  setBattleFlag(CombatState::OUTCOME_DONE, false);
  setBattleFlag(CombatState::ERROR_STATE, false);
}

/*
 * Description: Orders the actions on the buffer by speed of the aggressor
 *
 * Inputs:
 * Outputs:
 */
void Battle::orderActions()
{
  std::cout << "Action buffer state prior to sorting: " << std::endl;
  action_buffer->print();
  std::cout << std::endl;

  /* Re-order item actions first, and skills by momentum of the user */
  action_buffer->reorder(BufferSorts::ITEM_FIRST, BufferSorts::MOMENTUM);

  std::cout << "Action state after sorting: " << std::endl;
  action_buffer->print();
  std::cout << std::endl;

  /* Order action state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Deals with character related upkeep */
void Battle::personalUpkeep(Person* const target)
{
  target->battleTurnPrep();
  // clear flags for new turn (temp flags?)
  // process ailments
    // damage ailments
    // flag setting ailments
    // recalulate ailment factors
}

/*
 * Description: 
 *
 * Inputs:
 * Outputs:
 */
void Battle::processSkill(std::vector<Person*> targets)
{
  /* Assign the current user and grab the list of effects of the Skill */
  auto effects = curr_skill->getEffects();
  
  /* Grab the enumerated attribute types related to the elements of the Skill */
  auto prim_stats = Helpers::elementToStats(curr_skill->getPrimary());
  auto secd_stats = Helpers::elementToStats(curr_skill->getSecondary());
  prim_off = prim_stats.first;
  prim_def = prim_stats.second;
  secd_off = secd_stats.first;
  secd_def = secd_stats.second;

  /* Perform each action on the skill */
  for (auto it = begin(effects); it != end(effects); ++it)
  {
    curr_action = *it;

    /* Update the temporary copy of the User's current stats */
    temp_user_stats     = AttributeSet(curr_user->getCurr());
    temp_user_max_stats = AttributeSet(curr_user->getCurrMax());

    /* Build vectors of curr and curr_max stas for each target */
    for (auto jt = begin(targets); jt != end(targets); ++jt)
    {
      temp_target_stats.push_back(AttributeSet((*jt)->getCurr()));
      temp_target_max_stats.push_back(AttributeSet((*jt)->getCurrMax()));
    }

    /* User and target attribute for the current action */
    // auto user_attr = (*it)->getUserAttribute();
    // auto targ_attr = (*it)->getTargetAttribute();
 
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

    if ((*it)->actionFlag(ActionFlags::ALTER))
    {
      p_target_index = 0;

      for (auto jt = begin(targets); jt != end(targets); ++jt, p_target_index++)
      {
        /* Build the variables and calculate factors for the current target */
        curr_target = *jt;
        
        if (!doesActionMiss())
        {
          auto crit_factor = 1.00;

          calcElementalMods();

          //TODO
          if (doesActionCrit())
            crit_factor = calcCritFactor();

          auto base_damage = calcBaseDamage(crit_factor);
          
          if (curr_target->doDmg(base_damage))
          {
            // Person has died
          }

          if (checkPartyDeath(friends))
            setBattleFlag(CombatState::LOSS, true);
          else if (checkPartyDeath(foes))
            setBattleFlag(CombatState::VICTORY, true);
        }

        if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
        {
          // NOTES: CHECK FOR IGNORE FLAGS ALONG THE WAY
          // deal damage based on computed values
          // check party death upon each damage dealt
          // output info to BIB
        }
        else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
        {

        }          
      }
    }
    else if ((*it)->actionFlag(ActionFlags::INFLICT))
    {
    // if infliction
      // check for immunities
        // if not immune, chance for infliction
        // if successful, recalculate ailments
        // if failed, output message
    }
    else if ((*it)->actionFlag(ActionFlags::RELIEVE))
    {
    // if relieving, if chance occurs,
      // remove ailment if exists
      // update ailments
    }
    else if ((*it)->actionFlag(ActionFlags::ASSIGN))
    {
    // stat changing? TBD
      // find each stat to change
      // find by amount or by factor
        // incr. stats by amt. or factor
    }
    else if ((*it)->actionFlag(ActionFlags::REVIVE))
    {

    }
    else if ((*it)->actionFlag(ActionFlags::ABSORB))
    {

    }
  }
}

/*
 * Description: Process the actions (Items & Skills) in the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::processActions()
{
#ifdef UDEBUG
  std::cout << "Begin processing actions on buffer." << std::endl;
#endif

  auto curr_action_type = ActionType::NONE;

  do
  {
    curr_action_type = action_buffer->getActionType();
    curr_user        = action_buffer->getUser();

    auto can_process = true;

    if (curr_action_type == ActionType::SKILL)
    {
      if (action_buffer->getSkill() != nullptr)
      {
        if (action_buffer->getSkill()->getCooldown() == 0)
        {
          curr_skill = action_buffer->getSkill();

          if (!doesSkillMiss())
            processSkill(action_buffer->getTargets());
        }
      }
      else
      {
        can_process = false;
      }
    }
    else if (curr_action_type == ActionType::ITEM)
    {
      if (action_buffer->getItem() != nullptr && 
          action_buffer->getItem()->getUseSkill() != nullptr)
      {
        if (action_buffer->getItem()->getUseSkill()->getCooldown() == 0)
        {
          curr_skill = action_buffer->getItem()->getUseSkill();

          if (!doesSkillMiss())
            processSkill(action_buffer->getTargets());
        }
      }
      else
      {
        can_process = false;
      }
    }
    else if (curr_action_type == ActionType::DEFEND)
    {
      // Defend oneself (increase defenses by some factor)
    }
    else if (curr_action_type == ActionType::GUARD)
    {
      // Guard the appropriate target
    }
    else if (curr_action_type == ActionType::IMPLODE)
    {
      // Annihilate self in catastrophic hit against opponents!
    }
    else if (curr_action_type == ActionType::RUN)
    {
      // Attempt to run!
    }
    else if (curr_action_type == ActionType::PASS)
    {
      // Pass the turn, do nothing!
    }
    else
    {
      std::cout << "[Error]: Attempting to process invalid action type!" 
                << std::endl;
    }

#ifdef UDEBUG
    if (!can_process)
      std::cout << "[Error]: Couldn't process current action!" << std::endl;
#endif

  } while (action_buffer->setNext());

#ifdef UDEBUG
  std::cout << "End processing actions." << std::endl;
#endif

  /* Process Action stae complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/*
 * Description: Recalculates the ailments after they have been altered
 *
 * Inputs:
 * Outputs:
 */
void Battle::recalculateAilments(Person* const target)
{
  (void)target; //TODO: [Warning]
  // find base stats of person
  // find all buff factors
  // OR find bubbify factor (bubbify == NO BUFFS)
  // disenstubulate factor
}

/*
 * Description: Calculates enemy actions and add them to the buffer
 *
 * Inputs:
 * Outputs:
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
    update_module = bufferEnemyAction();

  /* Assert the person index exists in the Foes scope (-5 to -1) */
  if (update_module)
  {
    std::cout << "Preparing AIModule for person index" << person_index << std::endl;
    curr_user = getPerson(person_index);
    curr_module = curr_user->getAI();

    /* Reset the AI Module for a new turn decision, assign data */
    curr_user->resetAI();

    auto items = friends->getInventory()->getBattleItems();
    curr_module->setItems(buildBattleItems(person_index, items));
    curr_module->calculateAction();
  }
  else
  {
#ifdef UDEBUG
    std::cout << "Enemy selection of actions complete" << std::endl;
#endif

    /* Mark the enemy selection phase as complete on the max index */
    setBattleFlag(CombatState::PHASE_DONE);
  }
}

/*
 * Description: Calculates user actions and add them to the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::selectUserActions()
{
#ifdef UDEBUG
  std::cout << "Selecting User Actions: " << person_index <<  std::endl;
#endif

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
      menu->getMenuFlag(BattleMenuState::SELECTION_VERIFIED))
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
      /* Reload the menu information for the next person */
      menu->reset(curr_user, person_index);
      
      auto skills = curr_user->getUseableSkills();
      menu->setSelectableSkills(buildBattleSkills(person_index, skills));

      if (friends->getInventory() != nullptr)
      {
        auto items = friends->getInventory()->getBattleItems();
        menu->setSelectableItems(buildBattleItems(person_index, items));
      }
    
      if (config->getBattleMode() == BattleMode::TEXT)
      {
        menu->printMenuState();
      }
      else if (config->getBattleMode() == BattleMode::GUI)
      {
        (void)menu;//TODO: Battle Front End [08-01-14]
      }
    }
    else
    {
      std::cerr << "[Error]: Selection action for invalid person\n";
    }
  }
  else 
  {
#ifdef UDEBUG
    std::cout << "User selection of actions complete" << std::endl;
#endif
    /* Set the phase complete on the max person index */
    setBattleFlag(CombatState::PHASE_DONE);
  }
}

/*
 * Description: Load default configuration of the battle 
 *
 * Inputs:
 * Outputs:
 */
bool Battle::setupClass()
{
  curr_module = nullptr;

  // info_bar = nullptr;
  // menu = nullptr;
  // status_bar = nullptr;

  action_buffer = nullptr;
  bg            = nullptr;
  config        = nullptr;
  status_bar_bg = nullptr;

  ailment_update_mode = BattleOptions::FOREST_WALK;
  hud_display_mode    = BattleOptions::FOREST_WALK;
  battle_mode         = BattleMode::TEXT;
  turn_mode           = TurnMode::FRIENDS_FIRST;
  flags = static_cast<CombatState>(0);

  person_index           = 0;
  screen_height          = 0;
  screen_width           = 0;
  time_elapsed           = 0;
  time_elapsed_this_turn = 0;
  turns_elapsed          = 0;

  turn_state = TurnState::BEGIN;

  clearActionVariables();

  return true;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
bool Battle::testPersonIndex(const int32_t &test_index)
{
  auto test_person = getPerson(test_index);

  if (test_person->getBFlag(BState::ALIVE) && 
      !test_person->getBFlag(BState::SKIP_NEXT_TURN))
  {
    return true;
  }
 
  return false;
}

/*
 * Description: Method which calls personal upkeeps
 *
 * Inputs:
 * Outputs:
 */
void Battle::upkeep()
{
  /* Friends update */
  for (uint32_t i = 0; i < friends->getSize(); i++)
    personalUpkeep(friends->getMember(i));

  /* Foes update */
  for (uint32_t i = 0; i < foes->getSize(); i++)
    personalUpkeep(foes->getMember(i));

  /* Personal upkeep state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/*
 * Description: Assigns a new value to the ailment update mode
 *
 * Inputs:
 * Outputs:
 */
void Battle::setAilmentUpdateMode(const BattleOptions &new_value)
{
  ailment_update_mode = new_value;
}

/*
 * Description: Assigns a new value to the battle output mode
 *
 * Inputs:
 * Outputs:
 */
void Battle::setBattleMode(const BattleMode &new_value)
{
  battle_mode = new_value;
}

/*
 * Description: Assigns the friends party of the Battle
 *
 * Inputs:
 * Outputs:
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
 * Inputs:
 * Outputs:
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
 * Inputs:
 * Outputs:
 */
void Battle::setHudDisplayMode(const BattleOptions &new_value)
{
  hud_display_mode = new_value;
}

/*
 * Description: Updates the Battle to the next state
 *
 * Inputs:
 * Outputs:
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
    //Todo: Eventhandler battle finish signal?
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

    /* After the Battle Begins, the general turn loop begins at General Upkeep */
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
      processActions();
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
 * Description:
 *
 * Inputs:
 * Outputs:
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
 * Description: Assigns a new value for the screen width
 *
 * Inputs:
 * Outputs:
 */
void Battle::setScreenHeight(const uint16_t &new_value)
{
  screen_height = new_value;
}

/*
 * Description: Assigns a new value for the screen width
 *
 * Inputs:
 * Outputs:
 */
void Battle::setScreenWidth(const uint16_t &new_value)
{
  screen_width = new_value;
}

/*
 * Description: Assigns a new value to the elapsed time
 *
 * Inputs:
 * Outputs:
 */
void Battle::setTimeElapsed(const int32_t &new_value)
{
  time_elapsed = new_value;
}

/*
 * Description: Assigns thee time elapsed this turn
 *
 * Inputs:
 * Outputs:
 */
void Battle::setTimeElapsedThisTurn(const int32_t &new_value)
{
  time_elapsed_this_turn = new_value;
}

/*
 * Description: Assigns a new value to the turns elapsed
 *
 * Inputs:
 * Outputs:
 */
void Battle::setTurnsElapsed(const uint16_t &new_value)
{
  turns_elapsed = new_value;
}

/*
 * Description: Assigns a new turn mode to the Battle
 *
 * Inputs:
 * Outputs:
 */
void Battle::setTurnMode(const TurnMode &new_turn_mode)
{
  turn_mode = new_turn_mode;
}

/*
 * Description: Updates the turn state of the Battle
 *
 * Inputs:
 * Outputs:
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
 * Description: 
 *
 * Inputs:
 * Outputs:
 */
bool Battle::keyDownEvent(SDL_KeyboardEvent event)
{
  Helpers::flushConsole();

#ifdef UDEBUG
  if (event.keysym.sym == SDLK_PAUSE)
    printPartyState();
  else if (event.keysym.sym == SDLK_PRINTSCREEN)
    printTurnState();
  else if (event.keysym.sym == SDLK_HOME)
    action_buffer->print(false);
  else if (event.keysym.sym == SDLK_END)
    action_buffer->print(true);
  else if (event.keysym.sym == SDLK_PAGEUP)
    printInventory(friends);
  else if (event.keysym.sym == SDLK_PAGEDOWN)
    printInventory(foes);
  else if (event.keysym.sym == SDLK_DELETE)
    printTargetVariables(false);
#endif

  if (menu->getWindowStatus() == WindowStatus::ON)
    return menu->keyDownEvent(event);

  return false;
}

/*
 * Description: 
 *
 * Inputs:
 * Outputs:
 */
bool Battle::isPartyDead()
{
  return (friends->getLivingMembers().size() == 0);
}

/*
 * Description: Methods to print information about the Battle
 *
 * Inputs:
 * Outputs:
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
      std::cout << "\nOUTCOME_DONE: " 
                << getBattleFlag(CombatState::OUTCOME_DONE);
      std::cout << "\nERROR_STATE: " << getBattleFlag(CombatState::ERROR_STATE);
      std::cout << "\n\n";
    }

    if (party)
      printPartyState();

    // menu->printInfo() TODO: [03-01-14]
  }

  std::cout << "==== / Battle ====\n\n";
}

/*
 * Description: Actually performs the actions in the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::printPartyState()
{
  std::cout << "---- Friends ----\n";
  for (uint32_t i = 0; i < friends->getSize(); i++)
    printPersonState(friends->getMember(i), i + 1);

  std::cout << "---- Foes ----\n";
  for (uint32_t i = 0; i < foes->getSize(); i++)
    printPersonState(foes->getMember(i), i - foes->getSize());
}

/*
 * Description: Actually performs the actions in the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::printPersonState(Person* const member, 
                              const int32_t &person_index)
{
  if (member != nullptr)
  {
    std::cout << "[" << person_index << "] - " << member->getName();
    std::cout << " [ Lv. " << member->getLevel() << " ] << \n" 
              << "VITA: " << member->getCurr().getStat(0) << "\n"
              << "QTDR: " << member->getCurr().getStat(1) << "\n\n";
  }
}

/*
 * Description: Actually performs the actions in the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::printInventory(Party* const target_party)
{
  if (target_party != nullptr && target_party->getInventory() != nullptr)
  { 
    target_party->getInventory()->print();
  }
  else
  {
    std::cout << "[Warning]: Attempting to print ynull party or null inventory."
              << std::endl;
  }
}

/*
 * Description: Actually performs the actions in the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::printTargetVariables(const bool &print_target_stats)
{
  (void)print_target_stats;//WARNING
  // std::cout << "---- Current User/Action/Target Variables ----\n";
  // std::cout << "Primary Off Attr:     " << AttributeSet::getName(prim_off);
  // std::cout << "\nPrimary Def Attr:   " << AttributeSet::getName(prim_def);
  // std::cout << "\nSecondary Off Attr: " << AttributeSet::getName(secd_off);
  // std::cout << "\nSecondary Def Attr: " << AttributeSet::getName(secd_def);

  // std::cout << "\n--- Temp User Stats: ---\n";
  // temp_user_stats.print();
  // //temp_user_max_stats.print();

  // if (print_target_stats)
  // {
  //   for (auto it = begin(temp_target_stats); it != end(temp_target_stats); ++it)
  //     (*it).print();
  //   for (auto it = begin(temp_target_max_stats)
  //        ; it != end(temp_target_max_stats)
  //        ; ++it)
  //   {
  //     (*it).print();
  //   }
  // }

  // if (curr_user != nullptr)
  //   std::cout << "Curr User: " << curr_user->getName();
  // if (curr_target != nullptr)
  //   std::cout << "\nCurr Target: " << curr_target->getName();
  // if (curr_action != nullptr)
  //   std::cout << "\nCurr Action: " << curr_action->getID();
  // if (curr_skill != nullptr)
  //   std::cout << "\nCurr Skill: " << curr_skill->getName();

  // std::cout << "\nCrit Happens? "    << crit_happens;
  // std::cout << "\nAction Happens? "  << action_happens;
  // std::cout << "\nPrim Strength? "   << prim_strength;
  // std::cout << "\nSecd Strength? "   << secd_strength;
  // std::cout << "\nPrim Weakness? "   << prim_weakness;
  // std::cout << "\nSecd Weakness? "   << secd_weakness;
  // std::cout << "\nP Target Index: "  << p_target_index;
  // std::cout << "\n\nPrim User Off: " << prim_user_off;
  // std::cout << "\nPrim User Def: "   << prim_user_def;
  // std::cout << "\nSecd User Off: "   << secd_user_off;
  // std::cout << "\nSecd User Def:"    << secd_user_def;
  // std::cout << "\nCritical Chance: " << critical_chance;
  // std::cout << "\nMiss Chance: "     << miss_chance << "\n";
}

/*
 * Description: Actually performs the actions in the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::printTurnState()
{
  std::cout << "Current battle state: ";
  if (turn_state == TurnState::BEGIN)
    std::cout << "BEGIN\n";
  else if (turn_state == TurnState::GENERAL_UPKEEP) 
    std::cout << "GENERAL_UPKEEP\n";
  else if (turn_state == TurnState::UPKEEP)
    std::cout << "UPKEEP\n";
  else if (turn_state == TurnState::SELECT_ACTION_ALLY)
    std::cout << "SELECT_ACTION_ALLY\n";
  else if (turn_state == TurnState::SELECT_ACTION_ENEMY)
    std::cout << "SELECT_ACTION_ENEMY\n";
  else if (turn_state == TurnState::ORDER_ACTIONS)
    std::cout << "ORDER_ACTIONS\n";
  else if (turn_state == TurnState::PROCESS_ACTIONS)
    std::cout << "PROCESS_ACTIONS\n";
  else if (turn_state == TurnState::CLEAN_UP)
    std::cout << "CLEAN_UP\n";
  else if (turn_state == TurnState::LOSS)
    std::cout << "LOSS\n";
  else if (turn_state == TurnState::VICTORY)
    std::cout << "VICTORY\n";
  else if (turn_state == TurnState::DESTRUCT)
    std::cout << "DESTRUCT\n";
}

/*
 * Description: Update the cycle time of Battle
 *
 * Inputs:
 * Outputs:
 */
bool Battle::update(int32_t cycle_time)
{
  setTimeElapsed(cycle_time);

  // update(); TODO: Update the battle interface

  if (getBattleFlag(CombatState::PHASE_DONE))
  {
#ifdef UDEBUG
    std::cout << "Phase done, setting next turn state." << std::endl;
#endif
    setNextTurnState();
  }

  /* During menu, if an action type has been chosen, inject valid targets for
   * the scope of the particular action type [if required] */
  else if (turn_state == TurnState::SELECT_ACTION_ALLY)
  {
    if (menu->getMenuFlag(BattleMenuState::SELECTION_VERIFIED))
    {
      selectUserActions();
    }
    else
    {
      ActionType action_type = ActionType::NONE;
     
      if (menu->isActionTypeSelected())
        action_type = menu->getActionType();

      /* If the action index has been assigned and targets have not been assigned
       * yet (for that action index), find the scope of that action the user
       * wishes to use and inject the valid targets into the menu */
        if ((menu->getMenuFlag(BattleMenuState::ACTION_SELECTED) || 
             menu->getActionType() == ActionType::DEFEND ||
             menu->getActionType() == ActionType::GUARD  ||
             menu->getActionType() == ActionType::IMPLODE) && 
            !menu->getMenuFlag(BattleMenuState::TARGETS_ASSIGNED))
        {
          auto scope = ActionScope::NO_SCOPE;

          if (action_type == ActionType::SKILL)
          {
            scope = menu->getSelectedSkill()->getScope();
          }
          else if (action_type == ActionType::ITEM)
          {
            scope = menu->getSelectedItem()->getUseSkill()->getScope();
          }
          else if (action_type == ActionType::DEFEND || 
                   action_type == ActionType::IMPLODE)
          {
            scope = ActionScope::USER;
          }
          else if (action_type == ActionType::GUARD)
          {
            scope = ActionScope::ONE_ALLY_NOT_USER;
          }

          if (config->getBattleMode() == BattleMode::TEXT)
          {
            std::cout << "Finding selectable targets for action with scope: "
                      << Helpers::actionScopeToStr(scope) << std::endl;
          }

          auto battle_skill = menu->getMenuSkills().at(menu->getActionIndex());
          auto skill_targets = getIndexesOfPersons(battle_skill.all_targets);
          
          menu->setMenuFlag(BattleMenuState::TARGETS_ASSIGNED);
      
          if (!menu->setSelectableTargets(skill_targets))
          {
            if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
            {
              std::cout << "No selectable targets found! Select another action"
                        << " index!" << std::endl;
            }
          }
          else
          {
            menu->setActionScope(scope);
            menu->setMenuFlag(BattleMenuState::SCOPE_ASSIGNED);
            menu->printMenuState();
          }
        }
      }
    }
  else if (turn_state == TurnState::SELECT_ACTION_ENEMY)
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
        {
          scope = curr_module->getSelectedSkill()->getScope();
        }
        else if (action_type == ActionType::ITEM)
        {
          scope = curr_module->getSelectedItem()->getUseSkill()->getScope();
        }
        else if (action_type == ActionType::DEFEND || 
                 action_type == ActionType::IMPLODE ||
                 action_type == ActionType::RUN ||
                 action_type == ActionType::PASS)
        {
          scope = ActionScope::USER;
        }
        else if (action_type == ActionType::GUARD)
        {
          scope = ActionScope::ONE_ALLY_NOT_USER;
        }

        if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
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
  
  return false;
}

/*
 * Description: Returns the ailment update mode currently set
 *
 * Inputs:
 * Outputs:
 */
BattleOptions Battle::getAilmentUpdateMode()
{
  return ailment_update_mode;
}

/*
 * Description: Returns the assigned Battle display mode
 *
 * Inputs:
 * Outputs:
 */
BattleMode Battle::getBattleMode()
{
  return battle_mode;
}

/*
 * Description: Return the value of a given CombatState flag
 *
 * Inputs:
 * Outputs:
 */
bool Battle::getBattleFlag(const CombatState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Returns the friends pointer of the Battle
 *
 * Inputs:
 * Outputs:
 */
Party* Battle::getFriends()
{
  return friends;
} 

/*
 * Description: Returns the foes pointer of the Battle
 *
 * Inputs:
 * Outputs:
 */
Party* Battle::getFoes()
{
  return foes;
}

/*
 * Description: Returns the hud display mode currently set
 *
 * Inputs:
 * Outputs:
 */
BattleOptions Battle::getHudDisplayMode()
{
  return hud_display_mode;
}

/*
 * Description: Returns the value of the screen height 
 *
 * Inputs:
 * Outputs:
 */
uint32_t Battle::getScreenHeight()
{
  return screen_height;
}

/*
 * Description: Returns the value of the screen width
 *
 * Inputs:
 * Outputs:
 */
uint32_t Battle::getScreenWidth()
{
  return screen_width;
}

/*
 * Description: Evaluates and returns a vector of ailments for a given person
 *
 * Inputs:
 * Outputs:
 */
std::vector<Ailment*> Battle::getPersonAilments(Person* const target)
{
  std::vector<Ailment*> person_ailments;

  if (target != nullptr)
    for (auto it = begin(ailments); it != end(ailments); ++it)
      if ((*it).second->getVictim() == target)
        person_ailments.push_back((*it).second);

  return person_ailments;
}

/*
 * Description: Returns the value of the turns elapsed
 *
 * Inputs:
 * Outputs:
 */
uint32_t Battle::getTurnsElapsed()
{
  return turns_elapsed;
}

/*
 * Description: Returns the elapsed time of the Battle
 *
 * Inputs:
 * Outputs:
 */
uint32_t Battle::getTimeElapsed()
{
  return time_elapsed;
}

/*
 * Description: Returns the enumerated turn state of the Battle
 *
 * Inputs:
 * Outputs:
 */
TurnState Battle::getTurnState()
{
  return turn_state;
}

/*
 * Description: Returns the index integer of a a given Person ptr
 *
 * Inputs:
 * Outputs:
 */
/*  */
int32_t Battle::getTarget(Person* battle_member)
{
  for (uint32_t i = 0; i < friends->getSize(); i++)
    if (friends->getMember(i) == battle_member)
      return static_cast<int32_t>(i) + 1;

  for (uint32_t i = 0; i < foes->getSize(); i++)
    if (foes->getMember(i) == battle_member)
      return static_cast<int32_t>(i) - foes->getSize();

  return 0;
}

/*
 * Description: Actually performs the actions in the buffer
 *
 * Inputs:
 * Outputs:
 */
Person* Battle::getPerson(const int32_t &index)
{
  if (index < 0)
  {
    if ((index + static_cast<int32_t>(foes->getSize())) > -1)
      return foes->getMember(index + foes->getSize());
  }
  else if (index > 0)
  {
    if (index - 1 < static_cast<int32_t>(friends->getSize()))
      return friends->getMember(index - 1);
  }

  return nullptr;
}

/*
 * Description: Calculate and return all BattleMember indexes
 *
 * Inputs:
 * Outputs:
 */
std::vector<int32_t> Battle::getAllTargets()
{
  auto all_targets =  getFriendsTargets();
  auto foes_targets = getFoesTargets();

  all_targets.insert(end(all_targets), begin(foes_targets), end(foes_targets));

  return all_targets;
}

/*
 * Description: Obtains all friendly battle member indexes
 *
 * Inputs:
 * Outputs:
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
 * Description: Obtains all unfriendly battle member indexes
 *
 * Inputs:
 * Outputs:
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
 * Description: Obtains a vector of targets matching the signage
 *
 * Inputs:
 * Outputs:
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
 * Description: 
 *
 * Inputs:
 * Outputs:
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
 * Description: 
 *
 * Inputs:
 * Outputs:
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
 * Description: 
 *
 * Inputs:
 * Outputs:
 */
int32_t Battle::getIndexOfPerson(Person* check_person)
{
  for (uint32_t i = 0; i < friends->getSize(); i++)
    if (check_person == friends->getMember(i))
      return static_cast<int32_t>(i + 1);

  for (uint32_t i = 0; i < foes->getSize(); i++)
    if (check_person == foes->getMember(i))
      return (static_cast<int32_t>(i) * -1) - 1;

  return 0;
}

/*
 * Description: Obtains a vector of battle member indexes for a given user and 
 *              scope
 *
 * Inputs:
 * Outputs:
 */
std::vector<int32_t> Battle::getValidTargets(int32_t index, 
    ActionScope action_scope)
{
  std::vector<int32_t> valid_targets;

  if (action_scope == ActionScope::USER)
  {
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
 * Description: Assigns the running config
 *
 * Inputs:
 * Outputs:
 */
bool Battle::setConfiguration(Options* const new_config)
{
  if (new_config != nullptr)
  {
#ifdef UDEBUG
    std::cout << "Assigning Battle Configuration!" << std::endl;
#endif
    
    config = new_config;

    if (menu != nullptr)
      menu->setConfiguration(new_config);

    setScreenHeight(config->getScreenHeight());
    setScreenWidth(config->getScreenWidth());

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
 * Inputs:
 * Outputs:
 */

void Battle::setBattleFlag(CombatState flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
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

/* Public static gets for battle modifier values */
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