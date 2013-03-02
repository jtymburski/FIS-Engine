/******************************************************************************
* Class Name: PersonStatusBar Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Person status bar displays five thumbnails displaying the
*              ailments or buffs affecting the character, her health current
*              and maximum values, her quantum drive curretn and maximum values
*              and the character's name.
*
* Notes [1]: In Battle, Base Stats are the curent maximum values, and Temp
*            stats are current value, and the Normal stat set act as the Normal
*            Maximum values (without buffs and debuffs, etc.)
******************************************************************************/

#include "Game/Battle/PersonStatusBar.h"

/*
 * Description: Constructs a PersonStatusBar object
 *
 * Inputs: Person* - character to construct the object for
 *         width   - width of the status bar
 *         height  - height of the status bar
 */
PersonStatusBar::PersonStatusBar(Person* character, uint width, uint height, QWidget* parent)
{
  /* Initial Preparation */
  setParent(parent);
  setCharacter(character);
  setWidth(width);
  setHeight(height);

  /* Palette Set Up */
  QPalette pal(palette());
  pal.setColor(QPalette::Background, Qt::transparent);
  pal.setColor(QPalette::Foreground, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);

  /* Font setup */
  QFont current_font = font(); //TODO: Grab fonts from?
  current_font.setPixelSize(17);
  setFont(current_font);

  setup();
  update();
}

/*
 * Description: Annihilates a PersonStatusBar
 */
PersonStatusBar::~PersonStatusBar()
{
    // TODO FINISH DESTRUCTOR [02-23-13]
}

void PersonStatusBar::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setPen(QColor(Qt::black));
  painter.setBrush(QColor(Qt::black));
  painter.setOpacity(0.70);

  /* Temp draw person status bar */
  // painter.drawRect(0,0,bar_width,bar_height);

  /* Temp draw Status thumbs */
  painter.setOpacity(0.80);
  painter.setBrush(QColor(Qt::red));
  for (int i = 0; i < 5; i++)
      painter.drawRect(*status_thumbs.at(i));

  /* Draw HP & QD Bars */
  painter.setPen(QColor(Qt::black));
  painter.setBrush(Qt::transparent);
  painter.drawRect(*health_outline);
  painter.setPen(Qt::transparent);
  painter.setBrush(*health_grad);
  float pc = character->getVitality() * 1.0 / character->getTempVitality();
  health_bar->setWidth(health_outline->width() * pc);
  painter.drawRect(*health_bar);

  painter.setPen(QColor(Qt::black));
  painter.setBrush(Qt::transparent);
  painter.drawRect(*qd_outline);
  painter.setPen(Qt::transparent);
  painter.setBrush(*qd_grad);
  pc = character->getQuantumDrive() * 1.0 / character->getTempQuantumDrive();
  qd_bar->setWidth(qd_outline->width() * pc);
  painter.drawRect(*qd_bar);
}

/*
 * Description: Initial set up code for bounding boxes, etc.
 *
 * Inputs: none
 * Output: none
 */
void PersonStatusBar::setup()
{
  uint font_size = 17;   /* FONT SIZE DETERMINATION? */
  uint pad = 5;                         /* L & T Padding */
  uint length  = (getHeight() / 4) - 6; /* Length of the status boxes */
  uint spacing = 4;                     /* Spacing of the Status Boxes */

  /* Set up the spacing on the status boxes */
  for (uint i = 0; i < 5; i++) //TODO: STATUS FROM PERSON [02-23-13]
  {
    uint left_d  = i * (length + spacing) + pad;
    status_thumbs.push_back(new QRect(left_d, pad + font_size, length, length));
  }

  /* Initial Name box set up */
  name_box = new QRect(pad, pad, character->getName().size(), font_size);
  setDisplayMaxHP(character->getBaseVitality());
  setDisplayMaxQD(character->getBaseQuantumDrive());
  setDisplayHP(character->getTempVitality());
  setDisplayQD(character->getTempQuantumDrive());

  QPalette pal(palette());
  name_label = new QLabel(getName(), this);
  name_label->move(5,1);
  name_label->setPalette(pal);

  /* HP & QD */
  uint left_d = pad;
  uint top_d  = 2 * pad + length + font_size;
  uint width  = getWidth() - 2 * pad;
  uint height = getHeight() / 4 - 6;

  health_bar = new QRect(left_d, top_d, width, height);
  health_outline = new QRect(left_d, top_d, width, height);
  health_grad = new QLinearGradient(QPointF(0,0), QPointF(width,height));
  health_grad->setColorAt(0, QColor("#E30004"));
  health_grad->setColorAt(1, QColor("#6E0002"));

  top_d += pad + height;

  qd_bar = new QRect(left_d, top_d, width, height);
  qd_outline = new QRect(left_d, top_d, width, height);
  qd_grad = new QLinearGradient(QPointF(0,0), QPointF(width,height));
  qd_grad->setColorAt(0, QColor("#0C2B36"));
  qd_grad->setColorAt(1, QColor("#2D9FC4"));


}

/*
 * Description: Returs the name of the character for the status bar
 *
 * Inputs: none
 * Output: QString - name of the character
 */
QString PersonStatusBar::getName()
{
  return name;
}

/*
 * Description: Sets the character and name members in the class
 *
 * Inputs: Person* - character to set members to
 * Output: none
 */
void PersonStatusBar::setCharacter(Person* character)
{
  this->character = character;
  setName(character->getName());
}

/*
 * Description: Gets the current health displayed on the status bar
 *
 * Inputs: none
 * Output: uint - value of health
 */
uint PersonStatusBar::getDisplayHP()
{
  return health;
}

/*
 * Description: Gets the currently displayed quantum drive of the status bar
 *
 * Inputs: none
 * Output: uint - value of quantum drive
 */
uint PersonStatusBar::getDisplayQD()
{
  return qd;
}

/*
 * Description: Gets the currently displayed max HP of the status bar
 *
 * Inputs: none
 * Output: uint - value of max health
 */
uint PersonStatusBar::getDisplayMaxHP()
{
  return health_max;
}

/*
 * Description: Gets the currently displayed max quantum drive of the status bar
 *
 * Inputs: none
 * Output: uint - value of max quantum drive
 */
uint PersonStatusBar::getDisplayMaxQD()
{
  return qd_max;
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
 * Description: Sets the display HP of the status bar
 *
 * Inputs: uint - new displayed HP value
 * Output: none
 */
void PersonStatusBar::setDisplayHP(uint amount)
{
  health = amount;
  update();
}

/*
 * Description: Sets the display max HP of the status bar
 *
 * Inputs: uint - new displayed max HP value
 * Output: none
 */
void PersonStatusBar::setDisplayMaxHP(uint amount)
{
  health_max = amount;
}

/*
 * Description: Sets the display QD of the status bar
 *
 * Inputs: uint - new displayed QD value
 * Output: none
 */
void PersonStatusBar::setDisplayQD(uint amount)
{
  qd = amount;
  update();
}

/*
 * Description: Sets the display max QD of the status bar
 *
 * Inputs: uint - new displayed max QD value
 * Output: none
 */
void PersonStatusBar::setDisplayMaxQD(uint amount)
{
  qd_max = amount;
}

/*
 * Description: Sets the name to be displayed on the label
 *
 * Inputs: QString - name of the character displayed
 * Output: none
 */
void PersonStatusBar::setName(QString name)
{
  this->name = name;
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
  (new_value < kMAX_HEIGHT) ? (bar_height = new_value) : (bar_height = kMAX_HEIGHT);
}
