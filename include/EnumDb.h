/******************************************************************************
* File Name: EnumDb [Declaration]
* Date Created: November 12th, 2013
* 
* Description: EnumDatabase  contains the database of global enumerations and
*              functions for Enum -> String
******************************************************************************/
#ifndef ENUMDB_H
#define ENUMDB_H

//#include <boost/assign/list_of.hpp>
//#include <boost/unordered_map.hpp>
#include <cstdint>

/*============================================================================
 * GLOBAL ENUMS
 *===========================================================================*/

/* Possible occasions for an action */
enum class ActionOccasion : std::uint8_t;

/* Possible target scopes actions may have in Battle */
enum class ActionScope : std::uint16_t;

/* Possible types of actions for an actor in Battle */
enum class ActionType : std::uint8_t;

/* Possible causes for ActorDeath in Battle */
enum class ActorDeath : std::uint16_t;

/* All statistics used by actors for Battle-related purposes */
enum class Attribute : std::uint32_t;

/* Dialog category for the talking in map */
enum class DialogCategory;

/* Handler for all direction related calls for movement passability, etc. */
enum class Direction;

/* Elements of the Game. Elements have weaknesses and strengths */
enum class Element : std::uint8_t;

/*  Slots which equipment can fit into */
enum class EquipmentSlots : std::uint8_t;

/* The event classification - for what the event will do */
enum class EventClassifier;

/* Describes the current state of the inventory (how it is currently sorted) */
enum class ItemSorts : std::uint8_t;

/* Possible inflictions (ailments/status buffs) on an actor */
enum class Infliction : std::uint64_t;

/* The various types of parties which occur in the game */
enum class PartyType : std::uint8_t;

/* Description: The ranks a Person may hold */
enum class Ranks : std::uint16_t;

#endif // ENUMDB_H
