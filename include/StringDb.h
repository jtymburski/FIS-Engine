/*******************************************************************************
* File Name: StringDB
* Date Created: December 11th, 2013
* Inheritance: none
* Description: Namespace for important game strings. Central location for easy
*              to change values.
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*
*******************************************************************************/

#ifndef STRINGDB_H
#define STRINGDB_H

#include <string>

namespace StringDb
{

/* ------------ System Strings --------------- */
static const std::string kDEFAULT_PLAYER = "Berran";
static const std::string kDEFAULT_CURRENCY = "Joules";

/* ------------ Battle Strings --------------- */
static const std::string kACTOR_ALIVE = "Alive";
static const std::string kACTOR_DEAD = "KO";
static const std::string kACTOR_REVIVE = "regained consciousness";
static const std::string kRUN = "Flee";

/* ------------ Equip Strings --------------- */
static const std::string kHEAD_SLOT = "Head";
static const std::string kARM_SLOT = "Limb";
static const std::string kBODY_SLOT = "Torso";
static const std::string kLEGS_SLOT = "Lower";

/* ============= Object Strings ============= */

/* ------------ Default Names --------------- */
static const std::string kDEFAULT_BUBBY_PREFIX = "bubbies";
static const std::string kDEFAULT_ITEM_DESC = "Unknown item";
static const std::string kDEFAULT_ITEM_NAME = "Unknown";
static const std::string kDEFAULT_ITEM_PREFIX = "items";
static const std::string kDEFAULT_ITEM_USE_MSG = "has been used";

/* ------------ Alloy Names --------------- */
static const std::string kPHYSICAL_ALLOY_STR = "bartite";
static const std::string kNONE_PHYSICAL_ALLOY_STR = "cyllion";
static const std::string kFIRE_ALLOY_STR = "effretus";
static const std::string kFOREST_ALLOY_STR = "floriver";
static const std::string kICE_ALLOY_STR = "avalonium";
static const std::string kELECTRIC_ALLOY_STR = "niobium";
static const std::string kDIGITAL_ALLOY_STR = "ravizer";
static const std::string kNIHIL_ALLOY_STR = "vacantus";

/* ------------ Cat/Race Strings --------------- */
static const std::string kDEFAULT_CAT_NAME = "Battle Class";
static const std::string kDEFAULT_DENONYM = "Battle Classian";
static const std::string kDEFAULT_CAT_DESC = "A generic Battle Class";

/* ------------ Limits --------------- */
static const size_t kMAX_BRIEF_DESC = 80;
static const size_t kMAX_DESC = 500;
static const size_t kMAX_PREFIX = 15;
static const size_t kMAX_ITEM_NAME = 20;
static const size_t kMAX_TITLE_NAME = 13;
static const size_t kMAX_USE_MSG = 40;
};

#endif // STRINGDB_H
