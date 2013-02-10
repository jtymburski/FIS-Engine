/******************************************************************************
* Class Name: TitleScreen
* Date Created: Dec 02 2012
* Inheritance: QWidget
* Description: The TitleScreen class
******************************************************************************/
#include "TitleScreen.h"

/* ------------------------- Constant Definitions -------------------------- */
const QString TitleScreen::kMENU_ITEMS[] = {"Map Test", "Battle Test", "Exit"};

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

  /* Sound setup */
  background_sound = new Sound(":/SOUND_titlescreen",-1);
  background_sound->play();

  /* Setup the internals of the screen */
  setup();
  cursor_index = 0;

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
  if(event->key() == Qt::Key_Up)
  {
    decrementState();
  }
  else if(event->key() == Qt::Key_Down)
  {
    incrementState();
  }
  else if(event->key() == Qt::Key_Escape)
  {
    setState(MAINEXIT);
  }
  else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    background_sound->stop();

    if(cursor_index == TESTMAP)
      openMap();
    else if(cursor_index == TESTBATTLE)
      openBattle();
    else
      close();
  }
}

void TitleScreen::close()
{
  emit closing();
}

void TitleScreen::openBattle()
{
  emit openingBattle(0);
}

void TitleScreen::openMap()
{
  emit openingMap(1);
}

void TitleScreen::decrementState()
{
  unsetSelectedMenu(cursor_index);

  if(cursor_index == 0)
    cursor_index = kNUM_MENU_ITEMS - 1;
  else
    cursor_index--;

  setSelectedMenu(cursor_index);
}

void TitleScreen::incrementState()
{
  unsetSelectedMenu(cursor_index);

  cursor_index++;
  if(cursor_index == kNUM_MENU_ITEMS)
    cursor_index = 0;

  setSelectedMenu(cursor_index);
}

/* Changes the menu to state s and the given index */
void TitleScreen::iterate(State titlestate, int index)
{
}

void TitleScreen::playBackground()
{
  background_sound->play();
}

bool TitleScreen::setSelectedMenu(int menu_count)
{
  if(menu_count < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[menu_count]->palette());
    pal.setColor(QPalette::Background, QColor(28,76,46,64)); // R,G,B,A
    option_labels[menu_count]->setPalette(pal);
    return TRUE;
  }

  return FALSE;
}

bool TitleScreen::setState(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    unsetSelectedMenu(cursor_index);
    cursor_index = index;
    setSelectedMenu(cursor_index);
    return TRUE;
  }
  return FALSE;
}

void TitleScreen::setup()
{
  int marginWidth = 10;

  for(int i = 0; i < kNUM_MENU_ITEMS; i++)
  {
    QLabel* new_label = new QLabel(kMENU_ITEMS[i], this);
    int pixelWidth = new_label->fontMetrics().boundingRect(
                                                    new_label->text()).width();

    QPalette pal(new_label->palette());
    pal.setColor(QPalette::Foreground, Qt::white);
    new_label->setPalette(pal);

    new_label->setMargin(marginWidth);
    new_label->move((this->width() - pixelWidth) / 2 - marginWidth, 
                    400 + i*75);
    new_label->setAutoFillBackground(true);

    option_labels.append(new_label);
  }

  setSelectedMenu(0);
}

bool TitleScreen::unsetSelectedMenu(int menu_count)
{
  if(menu_count < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[menu_count]->palette());
    pal.setColor(QPalette::Background, QColor(Qt::black));
    option_labels[menu_count]->setPalette(pal);
    return TRUE;
  }

  return FALSE; 
}
