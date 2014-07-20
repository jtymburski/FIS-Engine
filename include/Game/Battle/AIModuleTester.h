/*******************************************************************************
* Class Name: AI Module [Declaration]
* Date Created: July 20, 2014
* Inheritance: None
* Description: 
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*******************************************************************************/
#ifndef AIMODULE_TESTER
#define AIMODULE_TESTER

#include "EnumDb.h"
#include "Game/Battle/AIModule.h"
#include "Game/Player/Person.h"

#include <iostream>
#include <vector>

class AIModuleTester
{
public:
  AIModuleTester();

private:

  /* ------------ Constants --------------- */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Runs tests on calculating the determination of action type choices */
  bool aiActionTypeTests(const uint32_t &num_times, AIModule* const module,
                         std::vector<std::pair<Item*, uint16_t> > valid_items);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:

};

#endif //AIMODULE_H
