/******************************************************************************
* Class Name: Item
* Date Created: Oct 28 2012
* Inheritance: InventoryUnit
* Description: The Item Header that defines all elements within Inventory.
*              This includes Equipment and Bubby.
******************************************************************************/
#ifndef ITEM_H
#define ITEM_H

#include <QImage>
#include <QString>

class Item
{
public:
  /*Constructor function*/
  Item(QWidget *parent = 0);

  /*Destructor function*/
  ~Item();

private:
  /*Category unit belongs to*/
  QString category;

  /*Number of these that exist in inventory*/
  int count;

  /*Description of unit*/
  QString description;

  /*Name of unit*/
  QString name;

  /*Thumbnail of unit*/
  QImage thumbnail;

  /*Number of turns this will last in battle*/
  int turn_count;

  /*Stat alteration values*/
  int stamina, quantumdrive, momentum, limbertude, unbearability;
  int aggression, fortitude;
  int thermal_aggression, thermal_fortitude;
  int polar_aggression, polar_fortitude;
  int primal_aggression, primal_fortitude;
  int charged_aggression, charged_fortitude;
  int cybernetic_aggression, cybernetic_fortitude;
  int nihil_aggression, nihil_fortitude;

  /*Appears in battle as usable*/
  bool BATTLEREADY;

  /*In battle, lasts forever*/
  bool INDEFINITE;

  /*Can be grouped together with others*/
  bool STACKABLE;

public:
  /*Evaulates BATTLEREADY flag*/
  bool isBattleReady();

  /*Evaluates STACKABLE flag*/
  bool isStackable();

  /*Evaluates INDEFINITE flag*/
  bool isIndefinite();

  /*Gets category of unit*/
  QString getCategory();

  /*Gets unit count*/
  int getCount();

  /*Gets description of unit*/
  QString getDescription();

  /*Gets name of unit*/
  QString getName();

  /*Gets thumbnail of unit*/
  QImage getThumbnail();

  /*Gets the turn count*/
  int getTurnCount();

  /*Sets unit count*/
  void setCount(int i);
};

#endif // ITEM_H

