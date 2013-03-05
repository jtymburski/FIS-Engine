/*******************************************************************************
* Class Name: Status Bar
* Date Created: Saturday, March 2nd, 2013
* Inheritance: Parent class: QWidget (Contained within BattleStatusBar)
* Description: Superclass of Enemy and Person Status Class
*******************************************************************************/

#include "Game/Battle/PersonStatusBar.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructs a PersonStatusBar object
 *
 * Inputs: Person* - character to construct the object for
 *         width   - width of the status bar
 *         height  - height of the status bar
 */
PersonStatusBar::PersonStatusBar(Person* character, uint width, uint height,
                           QWidget* parent)
{
  setParent(parent);
  setCharacter(character);
  setWidth(width);
  setHeight(height);
  setup();
}

/*
 * Description: Annihilates a PersonStatusBar object
 */
 PersonStatusBar::~PersonStatusBar()
{
  //delete character;
  //delete level_label;
  //delete health_label;
  //delete health_grad;
  //delete health_outline;
  //delete health_bar;
  //character = NULL;
  //level_label = NULL;
  //health_label = NULL;
  //health_grad = NULL;
  //health_outline = NULL;
  //health_bar = NULL;
  for (int i = 0; i < status_thumb_sprites.size(); i++)
  {
      delete status_thumb_sprites.at(i);
      status_thumb_sprites[i] = NULL;
      delete status_thumbs.at(i);
      status_thumbs[i] = NULL;
  }
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

void PersonStatusBar::setup()
{
  /* Palette & Font Setup */
  QPalette pal(palette());
  pal.setColor(QPalette::Background, Qt::transparent);
  pal.setColor(QPalette::Foreground, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);
  QFont current_font = font(); //TODO: Grab fonts from? [02-03-13]
  current_font.setPixelSize(17);
  setFont(current_font);

  // TODO -- IF show levels enabled - [03-03-13]
  current_font.setPixelSize(17);
  setFont(current_font);

  level_label = new QLabel(getDisplayLevel(), this);
  level_label->setPalette(pal);
  // END IF


}

/*
 * Description: Sets the character and name members in the class
 *
 * Inputs: Person* - character to set members to
 * Output: none
 */
void PersonStatusBar::setCharacter(Person* new_character)
{
  character = new_character;
}

/*
 * Description: Gets a QString of the displayed HP value.
 *
 * Inputs: none
 * Output: QString - string of the vitality uint.
 */
QString PersonStatusBar::getDisplayHP()
{
  QString display_hp;
  return display_hp.setNum(character->getVitality());
}

/*
 * Description: Gets a QString of the displayed level value.
 *
 * Inputs: none
 * Output: QString - string of the level uint.
 */
QString PersonStatusBar::getDisplayLevel()
{
  QString display_level;
  return display_level.setNum(character->getLevel());
}

/*
 * Description: Gets the current width of the status bar
 *
 * Inputs: none
 * Output: uint - width of the bar
 */
uint PersonStatusBar::getWidth()
{
  return bar_width;
}

/*
 * Description: Gets the height of the status bar
 *
 * Inputs: none
 * Output: uint - height of the bar
 */
uint PersonStatusBar::getHeight()
{
  return bar_height;
}

/*
 * Description: Sets the width of the status bar
 *
 * Inputs: uint - new width of the bar
 * Output: none
 */
void PersonStatusBar::setWidth(uint new_value)
{
  (new_value < kMAX_WIDTH) ? (bar_width = new_value) : (bar_width = kMAX_WIDTH);
}

/*
 * Description: Sets the height of the status bar
 *
 * Inputs: uint - new height of the bar
 * Output: none
 */
void PersonStatusBar::setHeight(uint new_value)
{
  (new_value < kMAX_HEIGHT) ? (bar_height = new_value) :
                              (bar_height = kMAX_HEIGHT);
}



