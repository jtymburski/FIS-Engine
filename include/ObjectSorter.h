/******************************************************************************
 * File Name: ObjectSorter.h
 * Date Created: July 7th, 2013
 * Description: This object sorter file contains all the functors for sorting
 *              objects in the game
\****************************************************************************/

#include "Game/Player/Item.h"
#include "Game/Player/Bubby.h"
#include "Game/Player/Equipment.h"
#include "Game/Player/Person.h"

/* ============================================================================
 * BUBBY CLASS FUNCTORS
 * ===========================================================================*/

/*
 * BubbyPtrSortByFlavour Functor
 *
 * Creates a functor for sorting bubby pointers by their flavour
 */
class BubbyPtrSortByFlavour
{
  public:
    bool operator()(Bubby* a, Bubby* b) const
    {
      return a->getType()->getName() < b->getType()->getName();
    }
};

/*
 * BubbyPtrSortByLevel Functor
 *
 * Creates a functor for sorting bubby pointers by their level.
 */
class BubbyPtrSortByLevel
{
  public:
    bool operator()(Bubby* a, Bubby* b) const
    {
        return a->getExp() < b->getExp();
    }
};

/* ============================================================================
 * EQUIPMENT CLASS FUNCTORS
 * ===========================================================================*/

/*
 * PersonPtrSortByLevel
 *
 * Functor for sorting person class pointers by their level
 */
class EquipmentPtrSortByName
{
  public:
    bool operator()(Equipment* a, Equipment* b) const
    {
        return a->getName() < b->getName();
    }
};


/* ============================================================================
 *  ITEM CLASS FUNCTORS
 * ===========================================================================*/

/*
 * ItemPtrSortByID Functor
 *
 * Creates a functor for sorting item pointers by their ID
 */
class ItemPtrSortByID
{
  public:
    bool operator()(Item* a, Item* b) const
    {
      return a->getId() < b->getId();
    }
};

/*
 * ItemPtrSortByName Functor
 *
 * Creates a functor for sorting item pointers by name
 */
class ItemPtrSortByName
{
  public:
    bool operator()(Item* a, Item* b) const
    {
      return a->getName() < b->getName();
    }
};

/*
 * ItemPtrSortByValue Functor
 *
 * Creates a functor for sorting item pointers by their value (monetary)
 */
class ItemPtrSortByValue
{
  public:
    bool operator()(Item* a, Item* b) const
    {
      return a->getValue() < b->getValue();
    }
};

/*
 * ItemPtrSortByMass Functor
 *
 * Creates a functor for sorting item pointers by their mass
 */
class ItemPtrSortByMass
{
  public:
    bool operator()(Item* a, Item* b) const
    {
      return a->getMass() < b->getMass();
    }
};

/*
 * ItemPtrSortByValuePerWeight Functor
 *
 * Creates a functor for sorting item pointers by their mass
 */
class ItemPtrSortByVPerM
{
  public:
    bool operator()(Item* a, Item* b) const
    {
      double value_per_weight_a = a->getValue() / a->getMass();
      double value_per_weight_b = b->getValue() / b->getMass();
      return value_per_weight_a < value_per_weight_b;
    }
};

/* ============================================================================
 * PERSON CLASS FUNCTORS
 * ===========================================================================*/

/*
 * PersonPtrSortByName
 *
 * Functor for sorting person class pointers by their name
 */
class PersonPtrSortByName
{
  public:

    bool operator()(Person* a, Person* b) const
    {
      return a->getName() < b->getName();
    }
};

/*
 * PersonPtrSortByLevel
 *
 * Functor for sorting person class pointers by their level
 */
class PersonPtrSortByLevel
{
  public:

    bool operator()(Person* a, Person* b) const
    {
      return a->getLevel() < b->getLevel();
    }
};

/*
 * PersonPtrSortBy
 *
 * Functor for sorting person class pointers by their race's name.
 */
class PersonPtrSortByRace
{
  public:

    bool operator()(Person* a, Person* b) const
    {
      return a->getRace()->getName() < b->getRace()->getName();
    }
};

/*
 * PersonPtrSortByCat
 *
 * Functor for sorting person class pointers by their category's name
 */
class PersonPtrSortByCat
{
  public:

    bool operator()(Person* a, Person* b) const
    {
      return a->getCategory()->getName() < b->getCategory()->getName();
    }
};

/* ============================================================================
 * SKILL CLASS FUNCTORS
 * ===========================================================================*/

/*
 * SkillPtrSortByName
 *
 * Functor for sorting skill class pointers by their name values.
 */
class SkillPtrSortByName
{
  public:

    bool operator()(Skill* a, Skill* b) const
    {
        return a->getName() < b->getName();
    }
};

/*
 * SkillPtrSortByQDValue
 *
 * Functor for sorting skill class pointers by their required qd cost.
 */
class SkillPtrSortByQdValue
{
  public:

    bool operator()(Skill* a, Skill* b) const
    {
        return a->getQdValue() < b->getQdValue();
    }
};
