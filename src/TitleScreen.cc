/******************************************************************************
* Class Name: TitleScreen
* Date Created: Dec 02 2012
* Inheritance: QWidget
* Description: The TitleScreen class
******************************************************************************/
#include "TitleScreen.h"

#include <QLabel>

/* Constructor function */
TitleScreen::TitleScreen(QWidget* parent)
{
  setFixedSize(1216, 704);

  QPalette pal(palette());
  pal.setColor(QPalette::Background, Qt::black);
  pal.setColor(QPalette::Foreground, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);

  QFont current_font = font();
  current_font.setPixelSize(30);
  current_font.setWeight(QFont::Bold);
  setFont(current_font);

  QLabel* label = new QLabel("My name is not fucking 'Sunshine' you li'l bitch.", this); /* OH FUCK THIS LINE IS MORE THAN 80 CHARACTRES LONG. WHAT IS GOING TO HAPPEN.                                 REMEMBER JESUS */
  QPalette pal2(label->palette());
  pal2.setColor(QPalette::Foreground, Qt::white);
  pal2.setColor(QPalette::Background, QColor(28,76,46,64)); // R,G,B,A
  label->setPalette(pal2);
  label->setMargin(5);
  label->setAutoFillBackground(true);

  show();
}

/* Destructor function */
TitleScreen::~TitleScreen()
{
}

void TitleScreen::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.drawPixmap(520, 100, QPixmap(":/FBS_Logo"));

  painter.drawText(250, 250, "BATTLE TEST");
  painter.drawText(250, 300, "MAP TEST");
}

/* Changes the menu to state s and the given index */
void TitleScreen::iterate(State titlestate, int index)
{
}
