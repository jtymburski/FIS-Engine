/***********************************************************************(*******
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
* // TODO: Options such as viewable flags, Fonts [03-03-13]
* // TODO: Get Status ailment vectors from the Peron [03-03-13]
*******************************************************************************/

#include "Game/Battle/AllyStatusBar.h"

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Construction of an AllyStatusBar object
 *
 * Inputs: Person* character - character to construct the bar for
 *         uint width        - width of the bar to be constructed
 *         uint height       - height of the bar to be constructed
 */
AllyStatusBar::AllyStatusBar(Person* character, uint width, uint height,
                             QWidget* parent)
    : PersonStatusBar(character, width, height, parent)
{
  additionalSetup();
  update();
}

/*
 * Description: Annihilates a PersonStatusBar
 */
AllyStatusBar::~AllyStatusBar()
{
    delete level_label;
    delete health_label;
    delete health_grad;
    delete health_outline;
    delete health_bar;
    level_label = NULL;
    health_label = NULL;
    health_outline = NULL;
    health_outline = NULL;
    health_bar = NULL;
    for (int i = 0; i < status_thumb_sprites.size(); i++)
    {
        delete status_thumb_sprites.at(i);
        status_thumb_sprites[i] = NULL;
        delete status_thumbs.at(i);
        status_thumbs[i] = NULL;
    }
  delete name_label;
  delete qd_label;
  delete qd_outline;
  delete qd_bar;
  delete name_box;
  delete qd_grad;
  name_label = NULL;
  qd_label = NULL;
  qd_outline = NULL;
  qd_bar = NULL;
  name_box = NULL;
  qd_grad = NULL;
}

/*==============================================================================
 * PAINT EVENTS
 *============================================================================*/

/*
 * Description: Paint event for the AllyStatusBar
 */
void AllyStatusBar::paintEvent(QPaintEvent*)
{
  /* Initial paint preparation */
  QPainter painter(this);
  painter.setPen(QColor(Qt::black));

  // Status thumbs TODO: Get Status from Person [02-03-31]
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
 * Description: Additional setup code (extra to parent) for bounding boxes
 *              and placement, fonts, etc.
 *
 * Inputs: none
 * Output: none
 */
void AllyStatusBar::additionalSetup()
{
  uint font_size = 17;    // TODO: Font size determination
  uint pad = 5;                         /* L & T Padding */
  uint length  = (getHeight() / 4) - 6; /* Length of the status boxes */
  uint spacing = 4;                     /* Spacing of the Status Boxes */

  /* Set up the spacing on the status boxes */
  for (uint i = 0; i < 5; i++) //TODO: STATUS FROM PERSON [02-23-13]
  {
    uint left_d  = i * (length + spacing) + pad;
    status_thumbs.push_back(new QRect(left_d, pad + font_size, length, length));
  }

  QPalette pal(palette());
  pal.setColor(QPalette::Foreground, QColor(10,10,10,255));
  setPalette(pal);


  name_label = new QLabel(character->getName(), this);
  name_label->move(5,1);
  name_label->setPalette(pal);

  level_label->move(getWidth() - (getDisplayLevel().size() * 10),1);

  /* HP & QD */
  uint left_d = pad;
  uint top_d  = 2 * pad + length + font_size;
  uint width  = getWidth() - 2 * pad;
  uint height = getHeight() / 4 - 6;

  health_bar = new QRect(left_d, top_d, width, height);
  health_outline = new QRect(left_d, top_d, width, height);

  QFont current_font(font());
  current_font.setPixelSize(15);
  setFont(current_font);
  pal.setColor(QPalette::Foreground, QColor(200,100,100,255));
  health_label = new QLabel(getDisplayHP(), this);
  health_label->move(left_d + 2, top_d - 2);
  health_label->setPalette(pal);

  health_grad = new QLinearGradient(QPointF(0,0), QPointF(width,height));
  health_grad->setColorAt(0, QColor("#E30004"));
  health_grad->setColorAt(1, QColor("#6E0002"));

  top_d += pad + height;

  qd_bar     = new QRect(left_d, top_d, width, height);
  qd_outline = new QRect(left_d, top_d, width, height);

  pal.setColor(QPalette::Foreground, QColor(200,100,100,255));
  qd_label = new QLabel(getDisplayQD(), this);
  qd_label->move(left_d + 2, top_d - 2);
  qd_label->setPalette(pal);

  qd_grad = new QLinearGradient(QPointF(0,0), QPointF(width,height));
  qd_grad->setColorAt(0, QColor("#0C2B36"));
  qd_grad->setColorAt(1, QColor("#2D9FC4"));
}

/*
 * Description: Gets a QString of the displayed QD value
 *
 * Inputs: none
 * Output: QString - string of the qd uint
 */
QString AllyStatusBar::getDisplayQD()
{
  QString display_qd;
  return display_qd.setNum(character->getQuantumDrive());
}


