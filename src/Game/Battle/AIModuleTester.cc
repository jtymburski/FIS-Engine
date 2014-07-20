/******************************************************************************
* Class Name: AI Module Tester [Implementation]
* Date Created: July 20, 2014
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
#include "Game/Battle/AIModuleTester.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
 AIModuleTester::AIModuleTester()
 {

 }

/*
 * Description:
 */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool AIModuleTester::aiActionTypeTests(const uint32_t &num_times,
                                       AIModule* const module,
                           std::vector<std::pair<Item*, uint16_t> > valid_items)
{
  if (module == nullptr)
    return false;

  auto skill_times   = 0;
  auto item_times    = 0;
  auto defend_times  = 0;
  auto guard_times   = 0;
  auto implode_times = 0;
  auto run_times     = 0;
  auto pass_times    = 0;
  auto discrepency   = 0;

  for (uint32_t i = 0; i < num_times; i++)
  {
    auto parent = module->getParent();
    parent->resetAI();
    module->setItems(valid_items);
    module->calculateAction();

   if (module->getActionType() == ActionType::SKILL)
     skill_times++;
    else if (module->getActionType() == ActionType::ITEM)
      item_times++;
    else if (module->getActionType() == ActionType::DEFEND)
      defend_times++;
    else if (module->getActionType() == ActionType::GUARD)
      guard_times++;
    else if (module->getActionType() == ActionType::IMPLODE)
      implode_times++;
    else if (module->getActionType() == ActionType::RUN)
      run_times++;
    else if (module->getActionType() == ActionType::PASS)
      pass_times++;
    else
      discrepency++;
  }
  
  std::cout << "Testing # of Action Type Choices" << std::endl;
  std::cout << "--------------------------------" << std::endl;
  std::cout << "Skill Times: " << skill_times << std::endl;
  std::cout << "Item Times:  " << item_times << std::endl;
  std::cout << "Defend Times: " << defend_times << std::endl;
  std::cout << "Guard Times: " << guard_times << std::endl;
  std::cout << "Implode Time: " << implode_times << std::endl;
  std::cout << "Run Times: " << run_times << std::endl;
  std::cout << "Pass Times: " << pass_times << std::endl;
  std::cout << "Discrepency: " << discrepency << std::endl;

  return true;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/