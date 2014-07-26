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
const uint16_t Battle::kGENERAL_UPKEEP_DELAY = 500; /* Time prior info bar msg */
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
const float    Battle::kOFF_PRIM_ELM_MODIFIER    =  1.07;
const float    Battle::kDEF_PRIM_ELM_MODIFIER    =  1.04;
const float    Battle::kOFF_SECD_ELM_MODIFIER    =  1.05;
const float    Battle::kDEF_SECD_ELM_MODIFIER    =  1.03;
const float    Battle::kBASE_CRIT_CHANCE         =  0.10;
const float    Battle::kOFF_CRIT_FACTOR          =  1.25;
const float    Battle::kCRIT_MODIFIER            =  0.0008;
const float    Battle::kCRIT_LVL_MODIFIER        =  0.012;
const float    Battle::kDODGE_MODIFIER           =  1.10;
const float    Battle::kDODGE_PER_LEVEL_MODIFIER =  1.04;
const float    Battle::kPRIM_ELM_ADV_MODIFIER    =  1.15;
const float    Battle::kPRIM_ELM_DIS_MODIFIER    =  0.87;
const float    Battle::kSECD_ELM_ADV_MODIFIER    =  1.10;
const float    Battle::kSECD_ELM_DIS_MODIFIER    =  0.93;
const float    Battle::kDOUBLE_ELM_ADV_MODIFIER  =  1.30;
const float    Battle::kDOUBLE_ELM_DIS_MODIFIER  =  0.74;

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
 * Description:
 *
 * Inputs:
 * Output:
 */
/* Attempts to add an ailment to the vector of ailments */
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Called when the Battle has been won */
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Called when the Battle has been lost */
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

  std::cout << "ASSIGNING CURR USER" << std::endl;
  curr_user   = nullptr;
  curr_target = nullptr;
  curr_action = nullptr;
  curr_skill   = nullptr;

  crit_happens   = false;
  action_happens = false;
  prim_strength = false;
  secd_strength = false;
  prim_weakness = false;
  secd_weakness = false;

  p_target_index = 0;

  prim_user_off = 0;
  prim_user_def = 0;
  secd_user_off = 0;
  secd_user_def = 0;

  float critical_chance = 0.00;
  float miss_chance     = 0.00;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Returns enumeration of party death [None, Friends, Foes, Both] */
bool Battle::checkPartyDeath(Party* const check_party)
{
  if (check_party->getLivingMembers().size() == 0)
    return true;

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Cleanup before the end of a Battle turn */
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Determines the turn progression of the Battle (based on speed) */
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Deals with general upkeep (i.e. weather) */
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Sets the flags of BattleState at the beginning of the Battle */
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
 * Description:
 *
 * Inputs:
 * Outputs:
 */
/* Orders the actions on the buffer by speed of the aggressor */
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

/*
 * Description: Actually performs the actions in the buffer
 *
 * Inputs:
 * Outputs:
 */
void Battle::performAction()
{
  //TODO: Perform actions [03-01-14]
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

void Battle::buildTargetVariables(Skill* curr_skill)
{
  /* Build variables for the current target */
  auto targ_attrs = temp_target_stats.at(p_target_index);
  auto prim_targ_off = temp_target_stats.at(p_target_index).getStat(prim_off);
  auto prim_targ_def = temp_target_stats.at(p_target_index).getStat(prim_def);
  auto secd_targ_off = temp_target_stats.at(p_target_index).getStat(secd_off);
  auto secd_targ_def = temp_target_stats.at(p_target_index).getStat(secd_def);
 
  prim_strength = false; /* If the opponent's prim element is str */
  secd_strength = false; /* If the opponent's secd element is str */
  prim_weakness = false; /* If the opponent's prim element is weak */
  secd_weakness = false; /* If the opponent's secd element is weak */

  /* If the user's prim element is weak against the target's */
  if (curr_user->getPrimary() == Helpers::getStrength(curr_target->getPrimary()))
    prim_strength = true;

  /* If the user's secd element is weak against the target's */
  if (curr_user->getSecondary() == Helpers::getStrength(curr_target->getSecondary()))
    secd_strength = true;

  /* If the user's prim element is strong against the target's */
  if (curr_user->getPrimary() == Helpers::getWeakness(curr_target->getPrimary()))
    prim_weakness = true;
  
  /* If the user's secd element is strong against the target's */
  if (curr_user->getSecondary() == Helpers::getWeakness(curr_target->getSecondary()))
    secd_weakness = true;

  if (prim_weakness && !secd_weakness)
  {
    if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
    {
      prim_user_off *= kPRIM_ELM_ADV_MODIFIER;
      secd_user_off *= kPRIM_ELM_ADV_MODIFIER;
    }
    else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
    {
      prim_user_def *= kPRIM_ELM_ADV_MODIFIER;
      secd_user_def *= kPRIM_ELM_ADV_MODIFIER;
    }
  }
  else if (!prim_weakness && secd_weakness)
  {
    if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
    {
      prim_user_off *= kSECD_ELM_ADV_MODIFIER;
      secd_user_off *= kSECD_ELM_ADV_MODIFIER;
    }
    else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
    {
      prim_user_off *= kSECD_ELM_ADV_MODIFIER;
      secd_user_def *= kSECD_ELM_ADV_MODIFIER;
    }
  }
  else if (prim_weakness && secd_weakness)
  {
    if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
    {
      prim_user_off *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_user_off *= kDOUBLE_ELM_ADV_MODIFIER;
    }
    else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
    {
      prim_user_off *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_user_def *= kDOUBLE_ELM_ADV_MODIFIER;
    }
  }
  else if (prim_strength && !secd_strength)
  {
    if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
    {
      prim_targ_off *= kPRIM_ELM_ADV_MODIFIER;
      secd_targ_off *= kPRIM_ELM_ADV_MODIFIER;
    }
    else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
    {
      prim_targ_def *= kPRIM_ELM_ADV_MODIFIER;
      secd_targ_def *= kPRIM_ELM_ADV_MODIFIER;
    }
  }
  else if (!prim_strength && secd_strength)
  {
    if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
    {
      prim_targ_off *= kSECD_ELM_ADV_MODIFIER;
      secd_targ_off *= kSECD_ELM_ADV_MODIFIER;
    }
    else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
    {
      prim_targ_off *= kSECD_ELM_ADV_MODIFIER;
      secd_targ_def *= kSECD_ELM_ADV_MODIFIER;
    }
  }
  else if (prim_strength && secd_strength)
  {
    if (curr_skill->getFlag(SkillFlags::OFFENSIVE))
    {
      prim_targ_off *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_targ_off *= kDOUBLE_ELM_ADV_MODIFIER;
    }
    else if (curr_skill->getFlag(SkillFlags::DEFENSIVE))
    {
      prim_targ_off *= kDOUBLE_ELM_ADV_MODIFIER;
      secd_targ_def *= kDOUBLE_ELM_ADV_MODIFIER;
    }
  }

  crit_happens  = false;
  action_happens = true;

  critical_chance = kBASE_CRIT_CHANCE;

  auto crit_mod = (temp_user_stats.getStat(Attribute::UNBR) * kCRIT_MODIFIER);
  auto lvl_diff = (curr_user->getLevel() - curr_target->getLevel());
  auto crit_lvl_mod = lvl_diff * kCRIT_LVL_MODIFIER;

  critical_chance += crit_mod + crit_lvl_mod;

  if (critical_chance > 0)
  {
    uint32_t crit_pc_1000 = floor(critical_chance * 1000);
    if (Helpers::chanceHappens(crit_pc_1000, 1000))
      crit_happens = true;
  }

  if (crit_happens)
  {
    targ_attrs.setStat(Attribute::PHAG, targ_attrs.getStat(Attribute::PHAG) * 
                                        kOFF_CRIT_FACTOR);
    prim_user_off *= kOFF_CRIT_FACTOR;
    secd_user_off *= kOFF_CRIT_FACTOR;
  }

  miss_chance = temp_target_stats.at(p_target_index).getStat(Attribute::LIMB);
  miss_chance *= kDODGE_MODIFIER;
  miss_chance += (-lvl_diff) * kDODGE_PER_LEVEL_MODIFIER;

  if (miss_chance > 0)
  {
    uint32_t miss_pc_1000 = floor(miss_chance * 1000);
    if (Helpers::chanceHappens(miss_pc_1000, 1000))
      action_happens = false;
  }
}

void Battle::processSkill(Person* user, std::vector<Person*> targets, 
                          Skill* curr_skill)
{
  /* Assign the current user and grab the list of effects of the Skill */
  curr_user = user;
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

    /* Create a temporary copy of the User's current stats */
    temp_user_stats = AttributeSet(user->getCurr());
    temp_user_max_stats = AttributeSet(user->getCurrMax());

    /* Build vectors of curr and curr_max stas for each target */
    for (auto jt = begin(targets); jt != end(targets); ++jt)
    {
      temp_target_stats.push_back(AttributeSet((*jt)->getCurr()));
      temp_target_max_stats.push_back(AttributeSet((*jt)->getCurrMax()));
    }

    std::cout << "location 1" << std::endl;
    /* Variables related to action and to skill elements */
    auto user_attr = (*it)->getUserAttribute();
    auto targ_attr = (*it)->getTargetAttribute();

    prim_user_off = temp_user_stats.getStat(prim_off);
    prim_user_def = temp_user_stats.getStat(prim_def);
    secd_user_off = temp_user_stats.getStat(secd_off);
    secd_user_def = temp_user_stats.getStat(secd_def);

        std::cout << "location 2" << std::endl;
    if (user->getPrimary() == curr_skill->getPrimary())
    {
      prim_user_off *= kOFF_PRIM_ELM_MODIFIER;
      prim_user_def *= kDEF_PRIM_ELM_MODIFIER;
    }
    else if (user->getSecondary() == curr_skill->getSecondary())
    {
      secd_user_off *= kOFF_SECD_ELM_MODIFIER;
      secd_user_def *= kDEF_SECD_ELM_MODIFIER;
    }

    auto test = (*it)->actionFlag(ActionFlags::ALTER);
    std::cout << "location 3" << std::endl;
    if ((*it)->actionFlag(ActionFlags::ALTER))
    {
      p_target_index = 0;

      for (auto jt = begin(targets); jt != end(targets); ++jt, p_target_index++)
      {
        /* Build the variables and calculate factors for the current target */
        curr_target = *jt;
        std::cout << "building target variabels" << std::endl;
        buildTargetVariables(curr_skill);
        std::cout << "built target variabels" << std::endl;

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
    std::cout << "location 4" << std::endl;
  }
  std::cout << "location 5" << std::endl;
}

/* Process the actions (Items & Skills) in the buffer */
void Battle::processActions()
{
#ifdef UDEBUG
  std::cout << "Processing actions on buffer." << std::endl;
#endif

  auto curr_action_type = ActionType::NONE;

  do
  {
    curr_action_type = action_buffer->getActionType();
    auto can_process = true;

    if (curr_action_type == ActionType::SKILL)
    {
      if (action_buffer->getSkill() != nullptr)
      {
        if (action_buffer->getSkill()->getCooldown() == 0)
        {
#ifdef UDEBUG
          std::cout << "Processing skill action!" << std::endl;
#endif
          processSkill(action_buffer->getUser(), action_buffer->getTargets(), 
                       action_buffer->getSkill());
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
#ifdef UDEBUG
          std::cout << "Processing item skill action!" << std::endl;
#endif
          processSkill(action_buffer->getUser(), action_buffer->getTargets(),
                       action_buffer->getItem()->getUseSkill());
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
    else
      std::cout << "Processed current action!" << std::endl;
#endif

  } while (action_buffer->setNext());

#ifdef UDEBUG
  std::cout << "Finished processing actions, phase complete." << std::endl;
#endif

  /* Process Action stae complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Recalculates the ailments after they have been altered */
void Battle::recalculateAilments(Person* const target)
{
  (void)target; //TODO: [Warning]
  // find base stats of person
  // find all buff factors
  // OR find bubbify factor (bubbify == NO BUFFS)
  // disenstubulate factor
}

/* Calculates enemy actions and add them to the buffer */
void Battle::selectEnemyActions()
{
  if (person_index < 0 && curr_module != nullptr &&
      curr_module->getFlag(AIState::SELECTION_COMPLETE))
  {
#ifdef UDEBUG
  std::cout << "Attempting to Add Enemy Actions: " << person_index << std::endl;
#endif 
    auto buffer_addition = false;
    auto person_user     = getPerson(person_index);
    auto action_type     = curr_module->getActionType();
    auto action_targets  = curr_module->getChosenTargets();

    if (action_type == ActionType::SKILL)
    {
      auto selected_skill = curr_module->getSelectedSkill();

      if (action_buffer->add(person_user, selected_skill, action_targets, 0))
        buffer_addition = true;
    }
    else if (action_type == ActionType::ITEM)
    {
      auto selected_item = curr_module->getSelectedItem();

      std::cout << "Attempting to add" << selected_item->getName() << "to buffer. " << std::endl;
      if (action_buffer->add(person_user, selected_item, action_targets, 0))
        buffer_addition = true;
    
      if (buffer_addition)
      {
        std::cout << "Added " << selected_item->getName() << "to buffer. " << std::endl;
        if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
        {
          std::cout << "Removing " << selected_item->getName() << " from "
                    << "inventory and implementing to buffer." << std::endl;
        }

        foes->getInventory()->removeItemID(selected_item->getGameID());
        curr_module->setItems(foes->getInventory()->getBattleItems());

        std::cout << "Does this delete the pointer? " << selected_item->getName() << "to buffer. " << std::endl;
      }
    }
    else if (action_type == ActionType::DEFEND  ||
             action_type == ActionType::GUARD   ||
             action_type == ActionType::IMPLODE ||
             action_type == ActionType::RUN     ||
             action_type == ActionType::PASS)
    {
      if (action_buffer->add(person_user, action_type, action_targets, 0))
        buffer_addition = true;
    }
    else
    {
      std::cerr << "[Error]: Enemy selection fo invalid action type\n";
    }

    if (buffer_addition)
    {
      if (person_user->getBFlag(BState::SELECTED_2ND_ACTION))
        person_user->setBFlag(BState::SELECTED_3RD_ACTION);
      else if (person_user->getBFlag(BState::SELECTED_ACTION))
        person_user->setBFlag(BState::SELECTED_2ND_ACTION);
      else
        person_user->setBFlag(BState::SELECTED_ACTION);

      if (canIncrementIndex(person_user))
        person_index--;

    }
    else
    {
      std::cout << "[Error]: Action buffer addition failure!" << std::endl;   
    }
  }

  /* Assert the person index exists in the Foes scope (-5 to -1) */
  if (person_index < 0 && 
      person_index >= (-1 * static_cast<int32_t>(foes->getSize())))
  {
    std::cout << "Preparing ai module for next person index" << std::endl;
    auto person_user = getPerson(person_index);

    /* If no curr module or IF the current selection is complete */
    if (curr_module == nullptr || !person_user->getBFlag(BState::SKIP_NEXT_TURN))
    {
      /* Obtain the AI Module for the current person */
      auto curr_person = getPerson(person_index);
      curr_module = curr_person->getAI();

      /* If the module exists, get it ready for a new turn */
      if (curr_module != nullptr)
      {
        /* Reset the AI Module for a new turn decision, assign data */
        curr_person->resetAI();
        curr_module->setItems(foes->getInventory()->getBattleItems());
        curr_module->calculateAction();
      }
    }
    else if (person_user->getBFlag(BState::SKIP_NEXT_TURN))
    {
      /* */
      person_index--;
    }
    else if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
    {
      std::cout << "[Error] Null AI module for Selecting Enemy Action!"
                << std::endl;
    }

  }
  else
  {
#ifdef UDEBUG
    std::cout << "Enemy action selection complete!" << std::endl;
#endif

    /* Mark the enemy selection phase as complete on the max index */
    setBattleFlag(CombatState::PHASE_DONE);
  }
}

/* Calculates user actions and add them to the buffer */
void Battle::selectUserActions()
{
#ifdef UDEBUG
  std::cout << "Selecting User Actions: " << person_index <<  std::endl;
#endif

  /* If an action has been selected for a valid person index, grab the info.
      and load it into the buffer */
  if (person_index > 0 &&
      menu->getMenuFlag(BattleMenuState::SELECTION_VERIFIED))
  {
    auto person_user    = getPerson(person_index);
    auto action_type    = menu->getActionType();
    auto action_targets = menu->getActionTargets();
    auto buffer_addition = false;

    /* Build the vector Person ptrs from the target index vector */
    std::vector<Person*> person_targets;

    for (auto it = begin(action_targets); it != end(action_targets); ++it)
      person_targets.push_back(getPerson(*it));
        
    /* Push the actions on to the Buffer */
    if (action_type == ActionType::SKILL)
    {
      auto selected_skill = menu->getSelectedSkill();

      if (action_buffer->add(person_user, selected_skill, person_targets, 0))
        buffer_addition = true;
      
    }
    else if (action_type == ActionType::ITEM)
    {
      auto selected_item = menu->getSelectedItem();
      
      if (action_buffer->add(person_user, selected_item, person_targets, 0))
        buffer_addition = true;

      if (buffer_addition)
      {
        if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
        {
          std::cout << "Removing " << selected_item->getName() << " from "
                    << "inventory and implementing to buffer." << std::endl;
        }

        friends->getInventory()->removeItemID(selected_item->getGameID());
        menu->setSelectableItems(friends->getInventory()->getBattleItems());
      }
    }
    else if (action_type == ActionType::DEFEND  || 
             action_type == ActionType::GUARD   ||
             action_type == ActionType::IMPLODE ||
             action_type == ActionType::RUN     ||
             action_type == ActionType::PASS)
    {
      if (action_buffer->add(person_user, action_type, person_targets, 0))
        buffer_addition = true;
    }
    else
    {
      std::cerr << "[Error]: Invalid action selected\n";
    }

    if (buffer_addition)
    {
      if (person_user->getBFlag(BState::SELECTED_2ND_ACTION))
        person_user->setBFlag(BState::SELECTED_3RD_ACTION);
      else if (person_user->getBFlag(BState::SELECTED_ACTION))
        person_user->setBFlag(BState::SELECTED_2ND_ACTION);
      else
        person_user->setBFlag(BState::SELECTED_ACTION);
    }
    else
    {
      std::cerr << "[Error]: Action buffer addition failure!" << std::endl;   
    }

    if (canIncrementIndex(person_user))
        person_index++;
  }

  /* If a menu action has been selected, update to the next person index. If 
     the index is at the highest level, set the select user action phase done */
  if ((static_cast<uint32_t>(person_index) <= 
      friends->getSize()) && person_index > -1)
  {
    auto person = getPerson(person_index);

    if (person != nullptr)
    {
      if (!person->getBFlag(BState::SKIP_NEXT_TURN))
      {
        /* Reload the menu information for the next person */
        menu->reset(getPerson(person_index), person_index);

        if (friends->getInventory() != nullptr)
          menu->setSelectableItems(friends->getInventory()->getBattleItems());
    
        if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
          menu->printMenuState();
      }
      else
      {
 
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
    std::cout << "Ally menu selection complete!" << std::endl;
#endif

    /* Set the phase complete on the max person index */
    setBattleFlag(CombatState::PHASE_DONE);
  }
}

/* Load default configuration of the battle */
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

/* Method which calls personal upkeeps */
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

/* Assigns a new value to the ailment update mode */
void Battle::setAilmentUpdateMode(const BattleOptions &new_value)
{
  ailment_update_mode = new_value;
}

/* Assigns a new value to the battle output mode */
void Battle::setBattleMode(const BattleMode &new_value)
{
  battle_mode = new_value;
}

/* Assigns the friends party of the Battle */
bool Battle::setFriends(Party* const new_friends)
{
  if (new_friends != nullptr)
  {
    friends = new_friends;

    return true;
  }

  return false;
}

/* Assigns the foes party of the Battle */
bool Battle::setFoes(Party* const new_foes)
{
  if (new_foes != nullptr)
  {
    foes = new_foes;

    return true;
  }

  return false;
}

/* Assigns a new value to the hud display mode */
void Battle::setHudDisplayMode(const BattleOptions &new_value)
{
  hud_display_mode = new_value;
}

/* Updates the Battle to the next state */
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

/* Assigns a new value for the screen width */
void Battle::setScreenHeight(const uint16_t &new_value)
{
  screen_height = new_value;
}

/* Assigns a new value for the screen width */
void Battle::setScreenWidth(const uint16_t &new_value)
{
  screen_width = new_value;
}

/* Assigns a new value to the elapsed time */
void Battle::setTimeElapsed(const int32_t &new_value)
{
  time_elapsed = new_value;
}

/* Assigns thee time elapsed this turn */
void Battle::setTimeElapsedThisTurn(const int32_t &new_value)
{
  time_elapsed_this_turn = new_value;
}

/* Assigns a new value to the turns elapsed */
void Battle::setTurnsElapsed(const uint16_t &new_value)
{
  turns_elapsed = new_value;
}

/* Assigns a new turn mode to the Battle */
void Battle::setTurnMode(const TurnMode &new_turn_mode)
{
  turn_mode = new_turn_mode;
}

/* Updates the turn state of the Battle */
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

/* */
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

/* */
bool Battle::isPartyDead()
{
  return (friends->getLivingMembers().size() == 0);
}

/* Methods to print information about the Battle */
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


void Battle::printPartyState()
{
  std::cout << "---- Friends ----\n";
  for (uint32_t i = 0; i < friends->getSize(); i++)
    printPersonState(friends->getMember(i), i + 1);

  std::cout << "---- Foes ----\n";
  for (uint32_t i = 0; i < foes->getSize(); i++)
    printPersonState(foes->getMember(i), i - foes->getSize());
}

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

void Battle::printTargetVariables(const bool &print_target_stats)
{
  std::cout << "---- Current User/Action/Target Variables ----\n";
  std::cout << "Primary Off Attr:     " << AttributeSet::getName(prim_off);
  std::cout << "\nPrimary Def Attr:   " << AttributeSet::getName(prim_def);
  std::cout << "\nSecondary Off Attr: " << AttributeSet::getName(secd_off);
  std::cout << "\nSecondary Def Attr: " << AttributeSet::getName(secd_def);

  std::cout << "\n--- Temp User Stats: ---\n";
  temp_user_stats.print();
  //temp_user_max_stats.print();

  if (print_target_stats)
  {
    for (auto it = begin(temp_target_stats); it != end(temp_target_stats); ++it)
      (*it).print();
    for (auto it = begin(temp_target_max_stats)
         ; it != end(temp_target_max_stats)
         ; ++it)
    {
      (*it).print();
    }
  }

  if (curr_user != nullptr)
    std::cout << "Curr User: " << curr_user->getName();
  if (curr_target != nullptr)
    std::cout << "\nCurr Target: " << curr_target->getName();
  if (curr_action != nullptr)
    std::cout << "\nCurr Action: " << curr_action->getID();
  if (curr_skill != nullptr)
    std::cout << "\nCurr Skill: " << curr_skill->getName();

  std::cout << "\nCrit Happens? "    << crit_happens;
  std::cout << "\nAction Happens? "  << action_happens;
  std::cout << "\nPrim Strength? "   << prim_strength;
  std::cout << "\nSecd Strength? "   << secd_strength;
  std::cout << "\nPrim Weakness? "   << prim_weakness;
  std::cout << "\nSecd Weakness? "   << secd_weakness;
  std::cout << "\nP Target Index: "  << p_target_index;
  std::cout << "\n\nPrim User Off: " << prim_user_off;
  std::cout << "\nPrim User Def: "   << prim_user_def;
  std::cout << "\nSecd User Off: "   << secd_user_off;
  std::cout << "\nSecd User Def:"    << secd_user_def;
  std::cout << "\nCritical Chance: " << critical_chance;
  std::cout << "\nMiss Chance: "     << miss_chance << "\n";
}

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

/* Update the cycle time of Battle */
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
      {
        action_type = menu->getActionType();
      }

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

        if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
        {
          std::cout << "Finding selectable targets for action with scope: "
                    << Helpers::actionScopeToStr(scope) << std::endl;
        }

        auto valid_targets = getValidTargets(person_index, scope);
        menu->setMenuFlag(BattleMenuState::TARGETS_ASSIGNED);
      
        if (!menu->setSelectableTargets(valid_targets))
        {
          if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
          {
            std::cout << "No selectable targets found! Select another action index!"
                      << std::endl;
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

/* Returns the ailment update mode currently set */
BattleOptions Battle::getAilmentUpdateMode()
{
  return ailment_update_mode;
}

/* Returns the assigned Battle display mode */
BattleMode Battle::getBattleMode()
{
  return battle_mode;
}

/* Return the value of a given CombatState flag */
bool Battle::getBattleFlag(const CombatState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/* Returns the friends pointer of the Battle */
Party* Battle::getFriends()
{
  return friends;
} 

/* Returns the foes pointer of the Battle */
Party* Battle::getFoes()
{
  return foes;
}

/* Returns the hud display mode currently set */
BattleOptions Battle::getHudDisplayMode()
{
  return hud_display_mode;
}

/* Returns the value of the screen height */
uint32_t Battle::getScreenHeight()
{
  return screen_height;
}

/* Returns the value of the screen width */
uint32_t Battle::getScreenWidth()
{
  return screen_width;
}

/* Evaluates and returns a vector of ailments for a given person */
std::vector<Ailment*> Battle::getPersonAilments(Person* const target)
{
  std::vector<Ailment*> person_ailments;

  if (target != nullptr)
    for (auto it = begin(ailments); it != end(ailments); ++it)
      if ((*it).second->getVictim() == target)
        person_ailments.push_back((*it).second);

  return person_ailments;
}

/* Returns the value of the turns elapsed */
uint32_t Battle::getTurnsElapsed()
{
  return turns_elapsed;
}

/* Returns the elapsed time of the Battle */
uint32_t Battle::getTimeElapsed()
{
  return time_elapsed;
}

/* Returns the enumerated turn state of the Battle */
TurnState Battle::getTurnState()
{
  return turn_state;
}

/* Returns the index integer of a a given Person ptr */
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

/*  */
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

/* Calculate and return all BattleMember indexes */
std::vector<int32_t> Battle::getAllTargets()
{
  auto all_targets =  getFriendsTargets();
  auto foes_targets = getFoesTargets();

  all_targets.insert(end(all_targets), begin(foes_targets), end(foes_targets));

  return all_targets;
}

/* Obtains all friendly battle member indexes */
std::vector<int32_t> Battle::getFriendsTargets(const bool &only_ko)
{
  std::vector<int32_t> friends_targets;

  for (uint32_t i = 0; i < friends->getSize(); i++)
  {
    auto member = foes->getMember(i);

    if (!only_ko || !member->getBFlag(BState::ALIVE))
      friends_targets.push_back(getTarget(friends->getMember(i)));
  }

  return friends_targets;
}

/* Obtains all unfriendly battle member indexes */
std::vector<int32_t> Battle::getFoesTargets(const bool &only_ko)
{
  std::vector<int32_t> foes_targets;

  for (uint32_t i = 0; i < foes->getSize(); i++)
  {
    auto member = foes->getMember(i);

    if (!only_ko || !member->getBFlag(BState::ALIVE))
      foes_targets.push_back(getTarget(foes->getMember(i)));
  }

  return foes_targets;
}

/* Obtains a vector of targets matching the signage */
std::vector<int32_t> Battle::getPartyTargets(int32_t check_index)
{
  if (check_index < 0 )
    return getFoesTargets();
  else
    return getFriendsTargets();

  return getFriendsTargets();
}

/* Obtains a vector of battle member indexes for a given user and scope */
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

/* Assigns the running config */
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

/* Assign a value to a CombatState flag */
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