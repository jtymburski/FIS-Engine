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

namespace StringDB
{
/* ------------ System Strings --------------- */
static const std::string kDEFAULT_PLAYER = "Berran";
static const std::string kDEFAULT_CURRENCY = "Joules";

/* ------------ Battle Strings --------------- */
static const std::string kACTOR_ALIVE = "Alive";
static const std::string kACTOR_DEAD = "KOd";
static const std::string kACTOR_REVIVE = "regained consciousness";
static const std::string kRUN = "Flee";

/* ------------ Equip Strings --------------- */
static const std::string kHEAD_SLOT = "Head";
static const std::string kARM_SLOT = "Limb";
static const std::string kBODY_SLOT = "Torso";
static const std::string kLEGS_SLOT = "Lower Body";

/* ------------ Object Strings --------------- */
static const std::string kDEFAULT_ITEM_DESC    = "Unknown item";
static const std::string kDEFAULT_ITEM_NAME    = "Unknown";
static const std::string kDEFAULT_ITEM_PREFIX  = "items";
static const std::string kDEFAULT_ITEM_USE_MSG = "has been used";

static const size_t kMAX_BRIEF_DESC  = 70;
static const size_t kMAX_DESC        = 500;
static const size_t kMAX_PREFIX      = 15;
static const size_t kMAX_USE_MSG     = 40;

};

#endif //STRINGDB_H