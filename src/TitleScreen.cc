/*******************************************************************************
 * Class Name: TitleScreen
 * Date Created: Dec 02 2012
 * Inheritance: none
 * Description: Is the view for the main control display of the game. Will 
 *              mostly just be a menu but this allows for the safe passage 
 *              between classes as interactions with Application occur.
 *
 * TODO:
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

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* Constructor function */
TitleScreen::TitleScreen(Options* running_config)
{
  /* Initial parameter setup */
  action = NONE;
  cursor_index = 0;
  font = NULL;
  render_index = 0;
  system_options = NULL;

  /* Sound setup */
  background_sound.setSoundFile("sound/ambience/background_menu_sound.wav");
  background_sound.setPlayForever();
  background_sound.play();
  menu_click_sound.setSoundFile("sound/functional/menu_click.wav");
  menu_click_sound.setPlayCount(1);
  
  /* Set up the configuration, if applicable */
  setConfiguration(running_config);
}

/* Destructor function */
TitleScreen::~TitleScreen()
{
  system_options = NULL;
  unsetMenu();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

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

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

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
    menu_click_sound.play();
  }
  else if(symbol.sym == SDLK_UP)
  {
    if(cursor_index == 0)
      cursor_index = kNUM_MENU_ITEMS;
    cursor_index--;
    menu_click_sound.play();
  }
  else if(symbol.sym == SDLK_RETURN)
  {
    setAction();
  }
  else if(symbol.sym == SDLK_ESCAPE)
  {
    cursor_index = kNUM_MENU_ITEMS - 1;
    menu_click_sound.play();
  }
}

/* The key up event handler */
void TitleScreen::keyUpEvent(SDL_Keysym symbol)
{
}

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
