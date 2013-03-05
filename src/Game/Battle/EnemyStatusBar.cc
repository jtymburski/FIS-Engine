#include "Game/Battle/EnemyStatusBar.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructs an EnemyStatusBar object
 *
 * Inputs: Person* character - character to construct the bar for
 *         uint width        - width of the bar to be constructed
 *         uint height       - height of the bar to be constructed
 */
EnemyStatusBar::EnemyStatusBar(Person* character, uint width, uint height,
                               QWidget* parent)
    : PersonStatusBar(character, width, height, parent)
{
    additionalSetup();
    update();
}

/*
 * Description: Annihilates an EnemyStatusBar object
 */
EnemyStatusBar::~EnemyStatusBar()
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
  clearStatusBoxes();
}

/*============================================================================
 * PAINT EVENTS
 *===========================================================================*/

/*
 * Description: Paint event for an EnemyStatusBar object
 */
void EnemyStatusBar::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setPen(QColor(Qt::black));
  painter.setBrush(QColor(Qt::black));
  painter.setOpacity(0.70);

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
}

/*
 * Description: Additional setup (extra to parent class) code for bounding boxes
 *              and fonts, etc.
 *
 * Inputs: none
 * Output: none
 */
void EnemyStatusBar::additionalSetup()
{
  uint font_size = 17;
  uint pad       = 5;
  uint length    = (getHeight() / 4) - 6;
  uint spacing   = 4;

  /* Setup the spacing on the status thumbnail boxes */
  for (uint i = 0; i < 5; i++) //TODO: STATUS FROM PERSON [03-02-13]
  {
    uint left_d = i * (length + spacing) + pad;
    status_thumbs.push_back(new QRect(left_d, pad + font_size, length, length));
  }

  /* Palette & Font Setup */
  QPalette pal(palette());
  pal.setColor(QPalette::Background, Qt::transparent);
  pal.setColor(QPalette::Foreground, Qt::blue);
  setAutoFillBackground(true);
  setPalette(pal);
  QFont current_font = font();
  current_font.setPixelSize(font_size);
  setFont(current_font);

  QString level;
  level.setNum(character->getLevel());
  level_label = new QLabel(level, this);
  level_label->move(getWidth() - (level.size() * 10),1);
  level_label->setPalette(pal);

  uint left_d = pad;
  uint top_d  = 2 * pad + length + font_size;
  uint width  = getWidth() - 2 * pad;
  uint height = getHeight() / 4 - 6;

  health_bar = new QRect(left_d, top_d, width, height);
  health_outline = new QRect(left_d, top_d, width, height);
  health_label = new QLabel(getDisplayHP());
  health_grad = new QLinearGradient(QPointF(0,0), QPointF(width,height));
  health_grad->setColorAt(0, QColor("#E30004"));
  health_grad->setColorAt(1, QColor("#6E0002"));
}

