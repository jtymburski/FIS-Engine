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
std::string kDEFAULT_PLAYER = "Berran";
std::string kDEFAULT_CURRENCY = "Joules";

/* ------------ Battle Strings --------------- */
std::string kACTOR_ALIVE = "Alive";
std::string kACTOR_DEAD = "KOd";
std::string kACTOR_REVIVE = "regained consciousness";
std::string kRUN = "Flee";

/* ------------ Equip Strings --------------- */
std::string kHEAD_SLOT = "Head";
std::string kARM_SLOT = "Limb";
std::string kBODY_SLOT = "Torso";
std::string kLEGS_SLOT = "Lower Body";

/* ------------ Item Strings --------------- */
std::string kDEFAULT_ITEM_DESC    = "Unknown item";
std::string kDEFAULT_ITEM_NAME    = "Unknown";
std::string kDEFAULT_ITEM_PREFIX  = "items";
std::string kDEFAULT_ITEM_USE_MSG = "has been used";

};

#endif //STRINGDB_H

