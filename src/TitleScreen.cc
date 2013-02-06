/******************************************************************************
* Class Name: TitleScreen
* Date Created: Dec 02 2012
* Inheritance: QWidget
* Description: The TitleScreen class
******************************************************************************/
#include "TitleScreen.h"

/* Constructor function */
TitleScreen::TitleScreen(int width, int height, QWidget* parent)
{
  setFixedSize(width, height);

  /* Window Colors Setup */
  QPalette pal(palette());
  pal.setColor(QPalette::Background, Qt::black);
  pal.setColor(QPalette::Foreground, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);

  /* Font setup */
  QFont current_font = font();
  current_font.setPixelSize(30);
  current_font.setWeight(QFont::Bold);
  setFont(current_font);

  /* Title Options */
  title_options.append("Map Test");
  title_options.append("Battle Test");
  title_options.append("Exit");

  /* Setup the internals of the screen */
  setup();

  show();
}

/* Destructor function */
TitleScreen::~TitleScreen()
{
}

void TitleScreen::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  QPixmap image(":/FBS_Logo");
  painter.drawPixmap((this->width() - image.width()) / 2, 100, 
                      QPixmap(":/FBS_Logo"));
}

void TitleScreen::keyPressEvent(QKeyEvent* event)
{
  //if(event->key == Qt::Key_Enter)
  //{
  //  // Enter was hit
  //}
}

void TitleScreen::keyReleaseEvent(QKeyEvent* event)
{
  //if(event->key == Qt::Key_Enter)
  //{
  //  // Enter was released
  //}
}

/* Changes the menu to state s and the given index */
void TitleScreen::iterate(State titlestate, int index)
{
}

void TitleScreen::setup()
{
  int marginWidth = 10;

  for(int i = 0; i < title_options.size(); i++)
  {
    QLabel* new_label = new QLabel(title_options[i], this);
    int pixelWidth = new_label->fontMetrics().boundingRect(
                                                    new_label->text()).width();

    QPalette pal(new_label->palette());
    pal.setColor(QPalette::Foreground, Qt::white);
    pal.setColor(QPalette::Background, QColor(28,76,46,64)); // R,G,B,A
    new_label->setPalette(pal);

    new_label->setMargin(marginWidth);
    new_label->move((this->width() - pixelWidth) / 2 - marginWidth, 
                    400 + i*75);
    new_label->setAutoFillBackground(true);

    option_labels.append(new_label);
  }
}
