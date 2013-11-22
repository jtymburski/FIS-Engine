/*******************************************************************************
 * Class Name: Application
 * Date Created: November 17, 2013
 * Inheritance: none
 * Description: The Main Application that handles all the displays for the 
 *              entire game. This includes the primary switching mechanism to 
 *              switch between views as needed according to the selection on 
 *              the screen.
 ******************************************************************************/
#include "Application.h"

/* Constant Implementation - see header file for descriptions */
//const short Application::kTICK_DELAY = 10;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Application::Application()//QWidget* parent)
{
  /* Initialize the variables */
  initialized = false;
  quit = false;
  renderer = NULL;
  running = TITLESCREEN;
  window = NULL;
  
  /* Set the title screen parameters */
  title_screen.setConfiguration(&system_options);

  //setParent(parent);

  // TODO: make this dependent on focus of the game. If it loses focus, 
  // return the normal cursor (Qt::ArrowCursor);
  //setCursor(Qt::BlankCursor);
  //setCursor(Qt::ArrowCursor);

  /* General class variables */
  //close_command = false;
  
  /* Options information */
  //short screen_height = system_options.getScreenHeight();
  //short screen_width = system_options.getScreenWidth();

  /* Set up the title screen */
  //title_screen = new TitleScreen(system_options);
  //title_screen->show();

  /* Set up the game */
  //game_handler = new Game(system_options);

  /* Add widgets to the stack */
  //addWidget(title_screen);
  //addWidget(game_handler);
  //switchWidget(TITLESCREEN);

  /* Widget information for handling the game */
  //setMaximumWidth(screen_width);
  //setMaximumHeight(screen_height);
  //setMinimumWidth(screen_width);
  //setMinimumHeight(screen_height);

  /* Object function call connections */
  //QObject::connect(title_screen, SIGNAL(exit()),
  //                 this,         SLOT(exit()));
  //QObject::connect(title_screen, SIGNAL(openBattle()), 
  //                 this,         SLOT(openBattle()));
  //QObject::connect(title_screen, SIGNAL(openMap()), 
  //                 this,         SLOT(openMap()));
  //QObject::connect(game_handler, SIGNAL(closeGame()),
  //                 this,         SLOT(closeGame()));

  /* Set the widget location (center of the screen) */
  //QDesktopWidget desktopWidget;
  //QRect desktopRect(desktopWidget
  //                    .availableGeometry(desktopWidget.primaryScreen()));
  //setGeometry((desktopRect.width() - screen_width) / 2, 
  //            (desktopRect.height() - screen_height) / 2, 
  //            screen_width, screen_height);

  /* Set up the tick and start the update sequence */
  //QObject::connect(&tick, SIGNAL(timeout()), this, SLOT(updateApp()));
  //update_time.start();
  //tick.setSingleShot(true);
  //tick.start(kTICK_DELAY);
  //updateApp();

  /* Do the final show once everything is set up */
  //showFullScreen();
}

Application::~Application()
{
  uninitialize();

  /* Clear out the widgets in the stack */
  //while(count() > 0)
  //  removeWidget(widget(0));

  /* Delete the game */
  //if(game_handler != 0)
  //{
  //  delete game_handler;
  //  game_handler = 0;
  //}
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Application::handleActions()
{
  /* Handle any appropriate actions of the individual views */
  if(running == TITLESCREEN)
  {
    /* Check if there is a title screen action */
    if(title_screen.isActionOnQueue())
    {
      TitleScreen::MenuItems action_item = title_screen.getAction();
      if(action_item == TitleScreen::EXIT)
        running = EXIT;
    }
  }
}

/* Goes through all available events that are currently on the stack */
void Application::handleEvents()
{
  SDL_Event event;
  
  while(SDL_PollEvent(&event) != 0)
  {
    /* If quit initialized, end the game loop */
    if(event.type == SDL_QUIT)
    {
      quit = true;
    }
    /* Otherwise, pass the key down events on to the active view */
    else if(event.type == SDL_KEYDOWN)
    {
      SDL_Keysym symbol = event.key.keysym;
      
      /* Send the key to the relevant view */
      if(running == TITLESCREEN)
        title_screen.keyDownEvent(symbol);
    }
    else if(event.type == SDL_KEYUP)
    {
      SDL_Keysym symbol = event.key.keysym;
      
      /* Send the key to the relevant view */
      if(running == TITLESCREEN)
        title_screen.keyUpEvent(symbol);
    }
  }
}
  
void Application::render(uint32_t cycle_time)
{
  //printf("Cycle Time: %d\n", cycle_time);
  
  /* Handle the individual action items, depending on whats running */
  if(running == TITLESCREEN)
    title_screen.render(renderer);
  else if(running == GAME)
    quit = quit; // DO GAME EXECUTION
  else if(running == OPTIONS)
    quit = quit; // DO OPTIONS EXECUTION
  else if(running == EXIT)
    quit = true;
}

/*=============================================================================
 * SLOTS
 *============================================================================*/

/* TEMP */
//void Application::openBattle()
//{
//  if(game_handler != 0)
//  {
//    game_handler->switchGameMode(Game::BATTLE);
//    switchWidget(GAME);
//  }
//}

/* TEMP */
//void Application::openMap()
//{
//  if(game_handler != 0)
//  {
//    game_handler->switchGameMode(Game::MAP);
//    switchWidget(GAME);
//  }
//}

//void Application::updateApp()
//{
//  /* Implement the update, where necessary */
//  int cycle_time = update_time.restart();
//  if(currentIndex() == TITLESCREEN && title_screen != 0)
//    title_screen->update(); // TODO: Change to custom function??
//  else if(currentIndex() == GAME && game_handler != 0)
//    game_handler->updateGame(cycle_time);
//
//  /* Restart the timer */
//  if(!close_command)
//    tick.start(kTICK_DELAY);
//}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Provides initialization of the SDL engine. Required for running. */
bool Application::initialize()
{
	bool success = !isInitialized();

  /* Initialize SDL */
  if(success && SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("[ERROR] SDL could not initialize. SDL error: %s\n", SDL_GetError());
    success = false;
  }

  /* Only proceed if successful */
  if(success)
  {
    /* Enable vertical syncing */
    //if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
    //  printf("[WARNING] Vsync not enabled.");
    
    /* Set texture filtering to linear */
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
      printf("[WARNING] Linear texture filtering not enabled.");
    
    /* Create window for display */
    window = SDL_CreateWindow("Univursa", SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 1216, 704, 
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == NULL)
    {
      printf("[ERROR] Window could not be created. SDL error: %s\n", 
             SDL_GetError());
      success = false;
    }
  }
  
  /* Create the renderer for the window */
  if(success)
  {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
      printf("[ERROR] Renderer could not be created. SDL error: %s\n", 
             SDL_GetError());
      success = false;
    }
    else
    {
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
  }
  
  /* Initialize SDL Image library */
  if(success)
  {
    if(!IMG_Init(IMG_INIT_PNG))
    {
      printf("[ERROR] SDL_Image could not initialize. SDL_image error: %s\n", 
             IMG_GetError());
      success = false;
    }
  }
  
  /* Initialize SDL TTF library */
  if(success)
  {
    if(TTF_Init() == -1)
    {
      printf("[ERROR] SDL_ttf could not initialize. SDL_ttf error: %s\n",
             TTF_GetError());
      success = false;
    }
    
    success = system_options.confirmFontSetup();
  }

  /* If successful, confirm the initialization */
  if(success)
  {
    initialized = true;
    
    /* Set the title screen background */
    title_screen.setBackground("pictures/univursatitle.png", renderer);
  }
  /* Uninitialize everything, if the init sequence failed */
  else if(!isInitialized())
    uninitialize();
  
	return success;
}

/* Returns the status if the application libraries have been initialized */
bool Application::isInitialized()
{
  return initialized;
}

/* Runs the application */
bool Application::run()
{
  uint32_t ticks = 0;
  
  // TODO: Remove - testing variables
  int angle = 0;
  double brightness = 1.0;
  double brightness2 = 1.0;
  bool increasing = true;
  
  if(isInitialized())
  {
    // TODO: Remove - sprite setup
    Frame white_mask("sprites/white_mask.png", renderer);
    SDL_SetTextureBlendMode(white_mask.getTexture(), SDL_BLENDMODE_ADD);
    
    Sprite image_sprite("sprites/raven_AA_D|3|.png", renderer);
    image_sprite.setWhiteMask(white_mask.getTexture());
    image_sprite.setBrightness(1.2f);
    image_sprite.setColorBalance(128, 128, 255);
    //image_sprite.flipHorizontal();
    //image_sprite.flipVertical(true);
    image_sprite.updateSprite(0, renderer);
    
    Sprite bubby_sprite("sprites/bubby_AA_A00.png", renderer);
    bubby_sprite.setWhiteMask(white_mask.getTexture());
    bubby_sprite.setOpacity(128);
    bubby_sprite.updateSprite(0, renderer);
    
    Sprite coin_sprite("sprites/coins_AA_A00.png", renderer);
    coin_sprite.setWhiteMask(white_mask.getTexture());
    coin_sprite.updateSprite(0, renderer);
    
    /* Main application loop */
    while(!quit)
    {
      // TODO: Remove - Sprite logic during rendering
      angle++;
      if(angle == 180)
        image_sprite.flipVertical(true);
      else if(angle == 360)
      {
        image_sprite.flipVertical(false);
        image_sprite.removeTail();
        angle = 0;
      }
      
      if(increasing)
      {
        brightness += 0.01;
        if(brightness > 1.3)
          increasing = false;
      }
      else
      {
        brightness -= 0.01;
        if(brightness <= 1.0)
          increasing = true;
      }
      brightness2 += 0.01;
      if(brightness2 >= 2.0)
        brightness2 = 0.0;
      
      /* Handle events and actions */
      handleEvents();
      handleActions();
      
      /* Determine the previous cycle time for using throughout the update
       * sequence for rendering */
      uint32_t cycle_time = SDL_GetTicks() - ticks;
      ticks = SDL_GetTicks();
      
      /* Clear screen */
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);
      
      /* Render the application view */
      render(cycle_time);
      
      // TODO: Remove - Sprite rendering
      image_sprite.setBrightness(brightness2);
      image_sprite.setRotation(angle);
      image_sprite.updateSprite(10, renderer);
      image_sprite.render(renderer, 512, 512);
      
      bubby_sprite.setBrightness(brightness);
      bubby_sprite.updateSprite(10, renderer);
      bubby_sprite.render(renderer, 512, 525);
      
      coin_sprite.setBrightness(brightness);
      coin_sprite.updateSprite(10, renderer);
      coin_sprite.render(renderer, 700, 300);
      
      /* Update screen */
      SDL_RenderPresent(renderer);
    }
        
    return true;
  }
  
  return false;
}
  
void Application::uninitialize()
{
  /* Clean up the renderer */
  if(renderer != NULL)
    SDL_DestroyRenderer(renderer);
  renderer = NULL;
  
  /* Clean up the window */
  if(window != NULL)
    SDL_DestroyWindow(window);
  window = NULL;

	/* Quit SDL sub-systems */
	IMG_Quit();
	SDL_Quit();
}
