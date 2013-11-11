/*******************************************************************************
* Class Name: Item Buffer
* Date Created: October 28th, 2013
* Inheritance: None
* Description: Item Buffer stores the stack of items which have been chosen by
*              players or enemies to be used. In addition it contains the user
*              of these items, and the targets thesee items are aimed at hitting.
*******************************************************************************/
#ifndef ITEMBUFFER_H
#define ITEMBUFFER_H

#include "Game/Player/Item.h"
#include "Game/Player/Person.h"

#include <QDebug>
#include <QList>
#include <QVector>

/* Structure containing the data of one element in the Item Buffer */
struct ItemUseAction
{
  /* The user of the ItemUseAction */
  Person* user;

  /* The item to be used */
  Item*   item_used;

  /* Vector of targets the item will hit */
  QVector<Person*> targets;

  /* Validity of the ItemUseAction element */
  bool valid_item_use;
};

class ItemBuffer
{
public:
  /* Creates an ItemBuffer object */
  ItemBuffer();

  /* Annihilates an Item Buffer object */
  ~ItemBuffer();

private:
  /* The current index of the ItemUse buffer */
  int curr_index;

  /* List of ItemUseAction structs describing the ItemBuffer */
  QList<ItemUseAction> item_buffer;

  /* ------------ Constants --------------- */
  static const ushort kMAXIMUM_ELEMENTS;
  static const ushort kMAXIMUM_TARGETS;
  static const ushort kSTARTING_ELEMENT;

/*==============================================================================
 * PRIVATE FUNCTIONS
 *=============================================================================*/
private:
  /* Asserts that a given ItemUseAction is a valid item use */
  bool checkValid(ItemUseAction item_use_action);

  /* Clears the vector of ItemUseActions -- Does not delete item/person data */
  void clearAll();

  /* Returns the entire structure of the an ItemBuffer element */
  ItemUseAction getIndex(int index);

/*==============================================================================
 * PUBLIC FUNCTIONS
 *=============================================================================*/
public:
  /* Attempts to add an Item to the Item Buffer */
  bool addItemUse(Person* user, Item* item_used, QVector<Person*> targets);

  /* Calculates and clears the invalid ItemUseActions from the ItemBuffer */
  void clearInvalid();

  /* Returns whether the next ItemUseAction is valid */
  bool isNextValid();

  /* Methods for printing out basic information describing Item Buffer */
  void printAll();
  void printInfo();
  bool printElement(int index);

  /* Removes an ItemUse from the Buffer by a given user of the item */
  bool removeItemUse(Person* user);

  /* Removes an ItemUse from the buffer by a given index */
  bool removeItemUse(int index);

  /* Returns a pointer to the current user of the item index */
  Person* getUser();

  /* Returns a pointer to the current item on the ItemUse buffer. */
  Item* getItemUse();

  /* Returns the vector containing the targets of the current ItemUse buffer */
  QVector<Person*> getTargets();

  /* Sets the next index of the ItemUse buffer--if one exists */
  bool setNextIndex();
};

#endif // ITEMBUFFER_H
