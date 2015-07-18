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
const uint16_t TitleScreen::kNAV_TIME_LIMIT = 200;
const uint8_t TitleScreen::kNUM_MENU_ITEMS = 3;
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
  base_path = "";
  cursor_index = 0;
  font = NULL;
  nav_down = false;
  nav_time = 0;
  nav_up = false;
  render_index = 0;
  system_options = NULL;
 
  // TODO: TESTING - FIX
  rotate1 = 0;
  rotate2 = 0;
  rotate3 = 0;
  rotate6 = 0;
  delay = 0;
  delay2 = 0;

  /* Set up the configuration, if applicable */
  setConfiguration(running_config);
}

/* Destructor function */
TitleScreen::~TitleScreen()
{
  background_music.stop(false);
  system_options = NULL;
  unsetMenu();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Decrements the selected option */
void TitleScreen::decrementSelected()
{
  if(cursor_index == 0)
    cursor_index = kNUM_MENU_ITEMS;
  cursor_index--;
  
  if (system_options->isAudioEnabled())
    menu_click_sound.play();
}
  
/* Increments the selected option */
void TitleScreen::incrementSelected()
{
  cursor_index++;
  if(cursor_index == kNUM_MENU_ITEMS)
    cursor_index = 0;
  
  if (system_options->isAudioEnabled())
    menu_click_sound.play();
}
  
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
    SDL_Color tinted_color = {20, 153, 78, 255};
    SDL_Color plain_color = {255, 255, 255, 255};
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

/* Enables or disables the view. This includes any initialization for before
 * or after it was visible */
void TitleScreen::enableView(bool enable)
{
  /* Enables all relevant control for the view */
  // TODO: Temporarily make music permanent
  if(enable)
  {
    background_music.crossFade(game_music.getChannelInt());
    //background_music.play();
  }
  /* Disables all relevant control for the view */
  else
  {
    game_music.crossFade(background_music.getChannelInt());
    //background_music.stop();
  }
}
 
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
  
/* The key up and down events to be handled by the class */
/* The key down event handler */
void TitleScreen::keyDownEvent(SDL_KeyboardEvent event)
{
  if(system_options != NULL)
  {
    if(event.keysym.sym == SDLK_DOWN && event.repeat == 0)
    {
      incrementSelected();
      nav_down = true;
    }
    else if(event.keysym.sym == SDLK_UP && event.repeat == 0)
    {
      decrementSelected();
      nav_up = true;
    }
    else if(event.keysym.sym == SDLK_RETURN)
    {
      setAction();
    }
    else if(event.keysym.sym == SDLK_ESCAPE)
    {
      if(cursor_index != (kNUM_MENU_ITEMS - 1))
      {
        cursor_index = kNUM_MENU_ITEMS - 1;

        if (system_options->isAudioEnabled())
          menu_click_sound.play();
      }
    }
  }
}

/* The key up event handler */
void TitleScreen::keyUpEvent(SDL_KeyboardEvent event)
{
  if(system_options != NULL)
  {
    if(event.keysym.sym == SDLK_DOWN)
    {
      nav_down = false;
    }
    else if(event.keysym.sym == SDLK_UP)
    {
      nav_up = false;
    }
  }
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

#ifdef TITLE_SKIP
    /* Render the background */
    background.render(renderer);
#else
    /* Render the background */
    background.render(renderer, 0, 0);
    background6.render(renderer, -220, -48);
    background2.render(renderer, 153, 314);

    /* Waldo show - TODO: Only certain hours? */
    if(true)
    {
      background7.render(renderer, 153, 314);
    }

    /* The flash and bangs */
    background4.render(renderer, 153, 314);
    background5.render(renderer, 153, 314);
	
    /* Render atmosphere */
    background3.render(renderer, 153, 314);

    /* Render title */
    title.render(renderer, 50, 50);

#endif
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
#ifdef TITLE_SKIP
  return background.insertFirst(base_path + path, renderer);
#else
  (void)path;
  
  background.insertFirst(base_path + "sprites/Title/title-backdrop.png", 
                         renderer);
  background2.insertFirst(base_path + "sprites/Title/title-dynaton.png", renderer);
  background4.insertFirst(base_path + "sprites/Title/title-dynatonbooms1.png", renderer);
  background4.setOpacity(0);
  background5.insertFirst(base_path + "sprites/Title/title-dynatonbooms2.png", renderer);
  background5.setOpacity(0);
  background3.insertFirst(base_path + "sprites/Title/title-dynatonatmosphere.png", renderer);
  background3.setOpacity(196);
  background3.setColorBalance(255, 170, 170);
  background6.insertFirst(base_path + "sprites/Title/title-moon.png", renderer);
  background7.insertFirst(base_path + "sprites/Title/title-waldo.png", renderer);
  title.setTexture(base_path + "sprites/Title/title.png", renderer);

  return true;
#endif
}

/* Sets the running configuration, from the options class */
bool TitleScreen::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    base_path = system_options->getBasePath();
    return true;
  }
  
  return false;
}

/* Sets the music in the title screen - TODO: Encapsulate in file load? */
void TitleScreen::setMusic()
{
  /* Sound setup */
  background_music.setChannel(SoundChannels::MUSIC1);
  background_music.setSoundFile(base_path + 
                                "sound/unlicensed/ag_theme.ogg");
  background_music.setLoopForever();
  background_music.setFadeTime(2000);
  background_music.setVolume(system_options->getMusicLevel());
  

  game_music.setChannel(SoundChannels::MUSIC2);
  game_music.setSoundFile(base_path + "sound/unlicensed/space_cowboy.ogg");
  game_music.setFadeTime(2000);
  game_music.setLoopForever();
  game_music.setVolume(system_options->getMusicLevel());
  
  menu_click_sound.setChannel(SoundChannels::MENUS);
  menu_click_sound.setSoundFile(base_path + "sound/functional/menu_click.wav");
  menu_click_sound.setVolume(system_options->getAudioLevel());
}

/* Updates the title screen. Necessary for visual updates */
bool TitleScreen::update(int cycle_time)
{
  /* Increment the nav time */
  nav_time += cycle_time;
 
  /* Rotation testing */
  //rotate1 += 0.03125;//0.0625;//0.001;
  rotate2 += 0.03125;
  rotate3 += 0.05;
  rotate6 -= 0.075;

  //background.setRotation(rotate1);
  background2.setRotation(rotate2);
  background3.setRotation(rotate3);
  background4.setRotation(rotate2);
  background5.setRotation(rotate2);
  background6.setRotation(rotate6);
  background7.setRotation(rotate2);

  /* Poof Number 1 */
  if(delay < 5000)
    delay += cycle_time;
  else if(delay < 10000)
  {
    delay = 10000;
    background4.setOpacity(255);
  }
  if(delay == 10000)
  {
    background4.setBrightness(background4.getBrightness() - 0.03125);

    if(background4.getBrightness() == 0.0)
    {
      uint8_t opacity = background4.getOpacity();

      if(opacity >= 2)
        background4.setOpacity(opacity - 2);
      else
      {
        background4.setBrightness(1.0);
        background4.setOpacity(0);
        delay = 0;
      }
    }
  }

  /* Poof Number 2 */
  if(delay2 < 7500)
    delay2 += cycle_time;
  else if(delay2 < 10000)
  {
    delay2 = 10000;
    background5.setOpacity(255);
  }
  if(delay2 == 10000)
  {
    background5.setBrightness(background5.getBrightness() - 0.03125);

    if(background5.getBrightness() == 0.0)
    {
      uint8_t opacity = background5.getOpacity();

      if(opacity >= 2)
        background5.setOpacity(opacity - 2);
      else
      {
        background5.setBrightness(1.0);
        background5.setOpacity(0);
        delay2 = 0;
      }
    }
  }

  /* Brightness (flicker) concept */
  //if(Helpers::randInt(10) < 2)
  //  background.setBrightness(background.getBrightness() - 0.01);
  //if(background.getBrightness() < 0.6)
  //  background.setBrightness(1.0);

  /* If the down key is pressed and not up */
  if(nav_down && !nav_up)
  {
    if(nav_time >= kNAV_TIME_LIMIT)
    {
      incrementSelected();
      nav_time -= kNAV_TIME_LIMIT;
    }
  }
  /* If the up key is pressed but not down */
  else if(nav_up && !nav_down)
  {
    if(nav_time >= kNAV_TIME_LIMIT)
    {
      decrementSelected();
      nav_time -= kNAV_TIME_LIMIT;
    }
  }
  /* Otherwise, just clear the buffers */
  else
  {
    nav_time = 0;
  }

  return (action != NONE);
}
 
