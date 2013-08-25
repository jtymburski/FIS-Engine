/*******************************************************************************
* Class Name: Status Bar
* Date Created: Saturday, March 2nd, 2013
* Inheritance: Parent class: QWidget (Contained within BattleStatusBar)
* Description: Superclass of Enemy and Person Status Class
*******************************************************************************/

#include "Game/Battle/PersonStatusBar.h"

/*============================================================================
 * CONSTANTS
 *===========================================================================*/
const uint PersonStatusBar::kMAX_FONT_SIZE = 25;
const uint PersonStatusBar::kMAX_WIDTH     = 1920 / 5;
const uint PersonStatusBar::kMAX_HEIGHT    = 200;

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
  setFontSize(17);
  setup();
}

/*
 * Description: Annihilates a Person status Bar.
 */
PersonStatusBar::~PersonStatusBar() {}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Constructs the vector of thumb sprites for status ailments that
 *              are currently inflicted . This should be called after
 *              clearStatusBoxes() so the vector is clear.
 *
 * Inputs: none
 * Output: none

void PersonStatusBar::calcAilments()
{

  QVector<Ailment> temp = character->getAilments();
  for (int i = 0; i < temp.size(); i++)
  {
    QString type = temp[i].getName(temp[i].getType());
    type = ":/" + type.toLower();
    // TODO: [03-08-13] QPixmap sprite(type);
    // Push_back type's status ailment  status_thumb_sprites.push_back(&sprite);
  }

}
*/

void PersonStatusBar::setup()
{
  /* Palette & Font Setup */
  QPalette pal(palette());
  pal.setColor(QPalette::Background, Qt::transparent);
  pal.setColor(QPalette::Foreground, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);
  QFont current_font = font();

  // TODO -- IF show levels enabled - [03-03-13]
  current_font.setPixelSize(font_size);
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
 * Description: Clears the status boxes and status sprites vectors
 *
 * Inputs: none
 * Output: none
 */
void PersonStatusBar::clearStatusBoxes()
{
  qDeleteAll(status_thumbs);
  qDeleteAll(status_thumb_sprites);
  status_thumbs.clear();
  status_thumb_sprites.clear();
}

/*
 * Description: Cleans up PersonStatusBar data
 *
 * Inputs: none
 * Output: QString - string of the vitality uint.
 */
void PersonStatusBar::cleanUp()
{
  if (level_label != NULL)
  {
    delete level_label;
    level_label = NULL;
  }
  if  (health_label != NULL)
  {
    delete health_label;
    health_label = NULL;
  }
  if (health_grad != NULL)
  {
    delete health_grad;
    health_grad = NULL;
  }
  if (health_outline != NULL)
  {
    delete health_outline;
    health_outline = NULL;
  }
  if (health_bar != NULL)
  {
    delete health_bar;
    health_bar = NULL;
  }

  clearStatusBoxes();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Gets a QString of the displayed HP value.
 *
 * Inputs: none
 * Output: QString - string of the vitality uint.
 */
QString PersonStatusBar::getDisplayHP()
{
  QString display_hp;
  return display_hp.setNum(character->getTemp()->getStat(EnumDb::VITA));
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
 * Description: Returns the current font size of the status bar
 *
 * Inputs: no
 * Output: uint - value of font_size
 */
uint PersonStatusBar::getFontSize()
{
  return font_size;
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
 * Description: Sets the size of the font for a PersonStatusBar
 *
 * Inputs: uint - value of the new font size
 * Output: none
 */
void PersonStatusBar::setFontSize(uint new_value)
{
  (new_value < kMAX_FONT_SIZE) ? (font_size = new_value) :
                                 (font_size = kMAX_FONT_SIZE);
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
