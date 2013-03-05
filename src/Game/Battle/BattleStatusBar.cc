/******************************************************************************
* Class Name: BattleStatusBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: QWidet
* Description: BattleStatusBar is a class which represents a box holding
*              all of the friend Party's individual status boxes (PStautsBar)
******************************************************************************/

#include "Game/Battle/BattleStatusBar.h"
#include <QDebug>

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: BattleStatusBar object
 *
 * Inputs: Party* - pointer to a party to create the bar for
 *         uint width - full useable width of the bar
 *         uint height - full useable height of the bar
 */
BattleStatusBar::BattleStatusBar(Party* persons, uint width,
                                 uint height, QWidget* parent)
{
  setParent(parent);
  setWidth(width);
  setHeight(height);
  for (uint i = 0; i < persons->getPartySize(); i++)
    addPerson(persons->getMember(i), i);
}

/*
 * Description: Annihilates a BattleStatusBar object
 */
BattleStatusBar::~BattleStatusBar()
{
  for (int i = 0; i < bars.size(); i++)
  {
    delete bars.at(i);
    bars[i] = NULL;
  }
  for (int i = 0; i < boxes.size(); i++)
  {
    delete boxes.at(i);
    boxes[i] = NULL;
  }
}

/*
 * Description: Adds a person to the BattlStatusBar
 *
 * Inputs: Person* - pointer to the person to be added
 *            uint - index of the person to be added
 * Output: none
 */
void BattleStatusBar::addPerson(Person* character, int person_index)
{
  if (person_index < 0)
  {
    bars.push_back(new EnemyStatusBar(character,width,height,this));
  }
  else
  {
    qDebug() << "Person Index:" << person_index;
    uint left_d = (getWidth() / 5) * person_index;
    uint width  = getWidth() / 5;
    uint height = getHeight();
    boxes.push_back(new QRect(left_d, 0, width, height));
    bars.push_back(new AllyStatusBar(character,width,height,this));
    bars.at(person_index)->setGeometry(*boxes.at(person_index));
  }
}

/*
 * Description: Gets the height of the BattleStatusBar
 *
 * Inputs: none
 * Output:
 */
uint BattleStatusBar::getHeight()
{
  return height;
}

uint BattleStatusBar::getWidth()
{
  return width;
}

void BattleStatusBar::setSize(QRect* box)
{
  setLeftMargin(box->left());
  setTopDistance(box->top());
  setWidth(box->width());
  setHeight(box->height());
}

void BattleStatusBar::setLeftMargin(uint left_margin)
{
  this->left_margin = left_margin;
}

void BattleStatusBar::setTopDistance(uint top_distance)
{
  this->top_distance = top_distance;
}

void BattleStatusBar::setWidth(uint new_width)
{
  (new_width < kMAX_WIDTH) ? (width = new_width) : (width = kMAX_WIDTH);
}

void BattleStatusBar::setHeight(uint new_height)
{
  (new_height < kMAX_HEIGHT) ? (height = new_height) : (height = kMAX_HEIGHT);
}
