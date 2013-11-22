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
const uint8_t TitleScreen::kFONT_SIZE = 28;
const std::string TitleScreen::kMENU_ITEMS[]   = {"Play Game",
                                                  "Options",
                                                  "Exit"};
const short TitleScreen::kNUM_MENU_ITEMS = 3;
const uint16_t TitleScreen::kTEXT_GAP = 65;
const uint16_t TitleScreen::kTEXT_MARGIN = 75;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Default constructor */
TitleScreen::TitleScreen()
{
  /* Initial parameter setup */
  action = NONE;
  cursor_index = 0;
  font = NULL;
  render_index = 0;
  system_options = NULL;
  
  /* Window Colors Setup */
  //QPalette pal(palette());
  //pal.setColor(QPalette::Background, Qt::black);
  //pal.setColor(QPalette::Foreground, Qt::white);
  //setAutoFillBackground(true);
  //setPalette(pal);

  /* Font setup */
  //QFont current_font = font();
  //current_font.setPixelSize(30);
  //current_font.setWeight(QFont::Bold);
  //setFont(current_font);

  /* Sound setup */
  //background_sound.setSoundFile("sound/ambience/background_menu_sound.wav");
  //background_sound.setPlayForever();
  //background_sound.play();
  //menu_click_sound.setSoundFile("sound/functional/menu_click.wav");
  //menu_click_sound.setPlayCount(1);

  /* Setup the internals of the screen */
  //setupMenu();
}

/* Constructor function */
TitleScreen::TitleScreen(Options* running_config) : TitleScreen()
{
  setConfiguration(running_config);
}

/* Destructor function */
TitleScreen::~TitleScreen()
{
  system_options = NULL;
  unsetMenu();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Decrements the selected menu item */
/*void TitleScreen::decrementSelected()
{
  if(cursor_index == 0)
    setSelected(kNUM_MENU_ITEMS - 1);
  else
    setSelected(cursor_index - 1);
}*/
  
/* Choose whether to highlight or not highlight the selected index */
/*bool TitleScreen::highlight(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[index]->palette());
    pal.setColor(QPalette::Background, QColor(28,76,46,64)); // R,G,B,A
    option_labels[index]->setPalette(pal);
    return true;
  }

  return false;
}*/

/* Increments the selected menu item */
/*void TitleScreen::incrementSelected()
{
  if((cursor_index + 1) == kNUM_MENU_ITEMS)
    setSelected(0);
  else
    setSelected(cursor_index + 1);
}*/

/* Sets the selected menu item - fails if out of range */
/*bool TitleScreen::setSelected(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    unhighlight(cursor_index);
    cursor_index = index;
    highlight(cursor_index);

    return true;
  }
  return false;
}*/

/* Sets the selected item. This gets polled by another class */
void TitleScreen::setAction()
{
  if(action == NONE)
    action = static_cast<MenuItems>(cursor_index);
}

/* Set up the menu display text, for painting */
bool TitleScreen::setMenu(SDL_Renderer* renderer)
{
  if(system_options != NULL)
  {
    SDL_Color tinted_color = {20, 153, 78};
    SDL_Color plain_color = {255, 255, 255};
    TTF_Font* new_font = Text::createFont(system_options->getFont(), 
                                          kFONT_SIZE, TTF_STYLE_BOLD);
    if(new_font != NULL)
    {
      unsetMenu();
      
      /* Set the class font */
      font = new_font;
      
      /* Set up the labels */
      for(int i = 0; i < kNUM_MENU_ITEMS; i++)
      {
        /* The selected text */
        Text* selected = new Text(font);
        selected->setText(renderer, kMENU_ITEMS[i], tinted_color);
        selected_options.push_back(selected);
        
        /* The unselected text */
        Text* unselected = new Text(font);
        unselected->setText(renderer, kMENU_ITEMS[i], plain_color);
        unselected_options.push_back(unselected);
      }
      
      /* Proceed to update the render index */
      render_index = kTEXT_MARGIN + kTEXT_GAP*(kNUM_MENU_ITEMS - 1) +
                     selected_options[kNUM_MENU_ITEMS - 1]->getHeight();
      render_index = system_options->getScreenHeight() - render_index;
      
      return true;
    }
  }
  
  return false;
}
  
/* Un-Highlight the selected index by removing the border */
/*bool TitleScreen::unhighlight(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[index]->palette());
    pal.setColor(QPalette::Background, QColor(255,0,0,0));
    option_labels[index]->setPalette(pal);
    return true;
  }

  return false;
}*/

void TitleScreen::unsetMenu()
{
  /* Clears selected labels */
  for(uint8_t i = 0; i < selected_options.size(); i++)
    delete selected_options[i];
  selected_options.clear();
  
  /* Clears unselected labels */
  for(uint8_t i = 0; i < unselected_options.size(); i++)
    delete unselected_options[i];
  unselected_options.clear();
  
  /* Destroy the font */
  TTF_CloseFont(font);
  font = NULL;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*void TitleScreen::paintEvent(QPaintEvent* event)
{
  (void)event;
  QPainter painter(this);

  painter.drawPixmap(0, 0, system_options.getScreenWidth(), 
                           system_options.getScreenHeight(), 
                           QPixmap("pictures/univursatitle.png"));
}*/

//void TitleScreen::keyPressEvent(QKeyEvent* event)
//{
//  if(event->key() == Qt::Key_Up)
//  {
//    decrementSelected();
//    menu_click_sound.play();
//  }
//  else if(event->key() == Qt::Key_Down)
//  {
//    incrementSelected();
//    menu_click_sound.play();
//  }
//  else if(event->key() == Qt::Key_Escape)
//  {
//    setSelected(EXIT);
//    menu_click_sound.play();
/*  }
  else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    background_sound.stop();

    if(cursor_index == TESTMAP)
      emit openMap();
    else if(cursor_index == TESTBATTLE)
      emit openBattle();
    else if(cursor_index == OPTIONS) // TODO
    {
      emit openOptions();
      background_sound.play();
    }
    else if(cursor_index == EXIT)
      emit exit();
  }
}*/

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the active action */
TitleScreen::MenuItems TitleScreen::getAction()
{
  if(action != NONE)
  {
    MenuItems triggered_action = action;
    action = NONE;
    return triggered_action;
  }
  
  return action;
}
  
/* Check if there's an active action */
bool TitleScreen::isActionOnQueue()
{
  return (action != NONE);
}
  
/* The key up and down events to be handled by the class */
/* The key down event handler */
void TitleScreen::keyDownEvent(SDL_Keysym symbol)
{
  if(symbol.sym == SDLK_DOWN)
  {
    cursor_index++;
    if(cursor_index == kNUM_MENU_ITEMS)
      cursor_index = 0;
  }
  else if(symbol.sym == SDLK_UP)
  {
    if(cursor_index == 0)
      cursor_index = kNUM_MENU_ITEMS;
    cursor_index--;
  }
  else if(symbol.sym == SDLK_RETURN)
  {
    setAction();
  }
  else if(symbol.sym == SDLK_ESCAPE)
  {
    cursor_index = kNUM_MENU_ITEMS - 1;
  }
}

/* The key up event handler */
void TitleScreen::keyUpEvent(SDL_Keysym symbol)
{
}
  
//void TitleScreen::playBackground()
//{
//  background_sound.play();
//}

/* Renders the title screen */
bool TitleScreen::render(SDL_Renderer* renderer)
{
  if(renderer != NULL)
  {
    /* Create font and labels, if NULL. Typically, this will only be called
     * once the first time */
    if(font == NULL)
      setMenu(renderer);
    
    /* Render the background */
    SDL_RenderCopy(renderer, background.getTexture(), NULL, NULL);
    
    /* Paint the selected options on the screen */
    for(uint8_t i = 0; i < selected_options.size(); i++)
    {
      if(i == cursor_index)
        selected_options[i]->render(renderer, kTEXT_MARGIN, 
                                    render_index + kTEXT_GAP*i);
      else
        unselected_options[i]->render(renderer, kTEXT_MARGIN, 
                                      render_index + kTEXT_GAP*i);
    }
    
    return true;
  }
  
  return false;
}

bool TitleScreen::setBackground(std::string path, SDL_Renderer* renderer)
{
  return background.setTexture(path, renderer);
}

/* Sets the running configuration, from the options class */
bool TitleScreen::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    return true;
  }
  
  return false;
}
