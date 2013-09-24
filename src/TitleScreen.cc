/******************************************************************************
* Class Name: TitleScreen
* Date Created: Dec 02 2012
* Inheritance: QWidget
* Description: Is the widget for the main display of the game. Will mostly 
*              just be a menu but this allows for the safe passage between
*              classes as interactions with Application occur.
*
* TODO:
*  1. Set up the options connection -> resolution
*  2. Constants in setupClass() and setupMenu()??
******************************************************************************/
#include "TitleScreen.h"

/* Constant Implementation - see header file for descriptions */
const QString TitleScreen::kMENU_ITEMS[]   = {"Game - Map",
                                              "Game - Battle",
                                              "Options",
                                              "Exit"};
const short TitleScreen::kNUM_MENU_ITEMS = 4;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Default constructor */
TitleScreen::TitleScreen(QWidget* parent)
{
  setupClass();
}

/* Constructor function */
TitleScreen::TitleScreen(Options running_config, QWidget* parent) 
                                                          : QWidget(parent)
{
  setConfiguration(running_config);
  setupClass();
}

/* Destructor function */
TitleScreen::~TitleScreen()
{
  for(int i = 0; i < option_labels.size(); i++)
    delete option_labels[i];
  option_labels.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Decrements the selected menu item */
void TitleScreen::decrementSelected()
{
  if(cursor_index == 0)
    setSelected(kNUM_MENU_ITEMS - 1);
  else
    setSelected(cursor_index - 1);
}
  
/* Choose whether to highlight or not highlight the selected index */
bool TitleScreen::highlight(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[index]->palette());
    pal.setColor(QPalette::Background, QColor(28,76,46,64)); // R,G,B,A
    option_labels[index]->setPalette(pal);
    return true;
  }

  return false;
}

/* Increments the selected menu item */
void TitleScreen::incrementSelected()
{
  if((cursor_index + 1) == kNUM_MENU_ITEMS)
    setSelected(0);
  else
    setSelected(cursor_index + 1);
}

/* Sets the selected menu item - fails if out of range */
bool TitleScreen::setSelected(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    unhighlight(cursor_index);
    cursor_index = index;
    highlight(cursor_index);

    return true;
  }
  return false;
}

void TitleScreen::setupClass()
{
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
  background_sound.setSoundFile("sound/ambience/background_menu_sound.wav");
  background_sound.setPlayForever();
  background_sound.play();
  menu_click_sound.setSoundFile("sound/functional/menu_click.wav");
  menu_click_sound.setPlayCount(1);

  /* Setup the internals of the screen */
  setupMenu();
  cursor_index = 0;
}

/* Set up the menu display text, for painting */
void TitleScreen::setupMenu()
{
  int marginWidth = 10;

  for(int i = 0; i < kNUM_MENU_ITEMS; i++)
  {
    QLabel* new_label = new QLabel(kMENU_ITEMS[i], this);
    int pixelWidth = new_label->fontMetrics().boundingRect(
                                                    new_label->text()).width();

    /* Set the color */
    QPalette pal(new_label->palette());
    pal.setColor(QPalette::Foreground, Qt::white);
    pal.setColor(QPalette::Background, QColor(255,0,0,0));
    new_label->setPalette(pal);

    /* Paint the labels */
    new_label->setMargin(marginWidth);
    new_label->move((this->width() - pixelWidth) / 2 - marginWidth,
                    350 + i*75);
    new_label->setAutoFillBackground(true);

    option_labels.append(new_label);
  }

  setSelected(TESTMAP);
}

/* Un-Highlight the selected index by removing the border */
bool TitleScreen::unhighlight(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[index]->palette());
    pal.setColor(QPalette::Background, QColor(255,0,0,0));
    option_labels[index]->setPalette(pal);
    return true;
  }

  return false;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void TitleScreen::paintEvent(QPaintEvent* event)
{
  (void)event;
  QPainter painter(this);

  painter.drawPixmap(0, 0, system_options.getScreenWidth(), 
                           system_options.getScreenHeight(), 
                           QPixmap("pictures/univursatitle.png"));
}

void TitleScreen::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Up)
  {
    decrementSelected();
    menu_click_sound.play();
  }
  else if(event->key() == Qt::Key_Down)
  {
    incrementSelected();
    menu_click_sound.play();
  }
  else if(event->key() == Qt::Key_Escape)
  {
    setSelected(EXIT);
    menu_click_sound.play();
  }
  else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    background_sound.stop();

    if(cursor_index == TESTMAP)
      emit openMap();
    else if(cursor_index == TESTBATTLE)
      emit openBattle();
    else if(cursor_index == OPTIONS) // TODO
      background_sound.play();
    else if(cursor_index == EXIT)
      emit close();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

void TitleScreen::playBackground()
{
  background_sound.play();
}

void TitleScreen::setConfiguration(Options running_config)
{
  system_options = running_config;
  setFixedSize(system_options.getScreenWidth(), 
               system_options.getScreenHeight());
}
