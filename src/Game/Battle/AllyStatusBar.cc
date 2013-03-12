/*******************************************************************************
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
* // TODO: Redo status ailment checks.
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
  if (health_bar != NULL)
  {
    delete health_bar;
    health_bar = NULL;
  }
  if (health_label != NULL)
  {
    delete health_label;
    health_label = NULL;
  }
  if (name_label != NULL)
  {
    delete name_label;
    name_label = NULL;
  }
  if (qd_label != NULL)
  {
    delete qd_label;
    qd_label = NULL;
  }
  if (qd_outline != NULL)
  {
    delete qd_outline;
    qd_outline = NULL;
  }
  if (qd_bar != NULL)
  {
    delete qd_bar;
    qd_bar = NULL;
  }
  if (qd_grad != NULL)
  {
    delete qd_grad;
    qd_grad = NULL;
  }

  cleanUp();
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
  QPalette pal(palette());

  painter.setPen(QColor(Qt::black));

  // Status thumbs TODO: Get Status from Person [02-03-31]
  painter.setOpacity(0.80);
  painter.setBrush(QColor(Qt::red));

  /* Rebuild and paint the status thumbnail boxes / images */
  rebuildStatusBoxes();
  for (int i = 0; i < status_thumbs.size(); i++)
    painter.drawRect(*status_thumbs.at(i));

  /* Draw HP & QD Bars */
  painter.setPen(QColor(Qt::black));
  painter.setBrush(Qt::transparent);
  painter.drawRect(*health_outline);
  painter.setPen(Qt::transparent);
  painter.setBrush(*health_grad);

  short num = character->tempStats()->getStat("VITA");
  short den = character->tempStats()->getMax("VITA");
  health_bar->setWidth(health_outline->width() * 1.0 * num / den);

  painter.drawRect(*health_bar);
  painter.setPen(QColor(Qt::black));
  painter.setBrush(Qt::transparent);
  painter.drawRect(*qd_outline);
  painter.setPen(Qt::transparent);
  painter.setBrush(*qd_grad);
  num = character->tempStats()->getStat("QTDR");
  den = character->tempStats()->getMax("QTDR");
  qd_bar->setWidth(qd_outline->width() * 1.0 * num / den);
  painter.drawRect(*qd_bar);
  health_label->setText(getDisplayHP());
  qd_label->setText(getDisplayQD());
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
  QPalette pal(palette());
  pal.setColor(QPalette::Foreground, QColor(10,10,10,255));
  setPalette(pal);


  name_label = new QLabel(character->getName(), this);
  name_label->move(5,1);
  name_label->setPalette(pal);


  level_label->move(getWidth() - (getDisplayLevel().size() * 10),1);

  /* HP & QD */
  uint length  = (getHeight() / 4) - 6; /* Length of the status boxes */
  uint pad = 5;
  uint left_d = pad;
  uint top_d  = 2 * pad + length + font_size;
  uint width  = getWidth() - 2 * pad;
  uint height = getHeight() / 4 - 6;

  health_bar = new QRect(left_d, top_d, width, height);
  health_outline = new QRect(left_d, top_d, width, height);
  pal.setColor(QPalette::Foreground, QColor(100,125,25,255));
  health_label = new QLabel(getDisplayHP(), this);
  health_label->move(health_bar->left() + 2, health_bar->top() - 3);
  health_label->setPalette(pal);
  health_grad = new QLinearGradient(QPointF(0,0), QPointF(width,height));
  health_grad->setColorAt(0, QColor("#E30004"));
  health_grad->setColorAt(1, QColor("#6E0002"));
  top_d += pad + height;
  qd_bar     = new QRect(left_d, top_d, width, height);
  qd_outline = new QRect(left_d, top_d, width, height);
  pal.setColor(QPalette::Foreground, QColor(100,125,25,255));
  qd_label = new QLabel(getDisplayQD(), this);
  qd_label->move(qd_bar->left() + 2, qd_bar->top() - 3);
  qd_label->setPalette(pal);
  qd_grad = new QLinearGradient(QPointF(0,0), QPointF(width,height));
  qd_grad->setColorAt(0, QColor("#0C2B36"));
  qd_grad->setColorAt(1, QColor("#2D9FC4"));
}

/*
 * Description: Rebuilds the status bar bounding boxes before painting
 *
 * Inputs: none
 * Output: none
 */
void AllyStatusBar::rebuildStatusBoxes()
{
  clearStatusBoxes();
  uint length  = (getHeight() / 4) - 6;
  // uint left_d  = i * (length + 4) + 5;
  //  status_thumbs.push_back(new QRect(left_d, 5 + font_size, length, length));

  // calcAilments();
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
  return display_qd.setNum(character->tempStats()->getStat("QTDR"));
}


