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
  renderer = NULL;
  window = NULL;
  
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

/* Goes through all available events that are currently on the stack */
void Application::handleEvents()
{
  SDL_Event event;
  
  while(SDL_PollEvent(&event) != 0)
  {
    if(event.type == SDL_QUIT)
    {
      quit = true;
    }
  }
}
  
//void Application::switchWidget(int index)
//{
//  /* Sets the current displayed index */
//  setCurrentIndex(index);
//  if(index != GAME)
//    currentWidget()->setFocus();
//
//  /* Restart the background music if title screen is activated */
//  if(index == TITLESCREEN && title_screen != 0)
//    title_screen->playBackground();
//}

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

//void Application::closeEvent(QCloseEvent* event)
//{
//  exit();
//  event->ignore();
//}

/*=============================================================================
 * SLOTS
 *============================================================================*/

//void Application::closeGame()
//{
//  switchWidget(TITLESCREEN);
//}

//void Application::exit()
//{
//  close_command = true;
//  emit closing();
//}

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
    if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
      printf("[WARNING] Vsync not enabled.");
    
    /* Set texture filtering to linear */
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
      printf("[WARNING] Linear texture filtering not enabled.");
    
    /* Create window for display */
    window = SDL_CreateWindow("Univursa", SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 1216, 704, 
                              SDL_WINDOW_SHOWN);
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
      printf("[ERROR] SDL_Image could not initialize. SDL_Image error: %s\n", 
             IMG_GetError());
      success = false;
    }
  }

  /* If successful, confirm the initialization */
  if(success)
    initialized = true;
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
  if(isInitialized())
  {
    quit = false;
    
    /* Main application loop */
    while(!quit)
    {
      handleEvents();
      
      /* Clear screen */
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(renderer);
      
      // TODO: Put rendering code here for entire application
      
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
