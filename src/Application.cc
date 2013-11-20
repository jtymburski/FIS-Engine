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
  SDL_Rect rect;
  SDL_Texture* target_texture = NULL;
  int test; // TODO - Remove: Warning test
  int angle = 0;
  double brightness = 1.0;
  double brightness2 = 1.0;
  bool increasing = true;
  
  if(isInitialized())
  {
    quit = false;

    // TESTING - frame setup
    Frame f("sprites/bg.png", renderer);
    Frame f2("sprites/arcadius.png", renderer);
    Frame f3("sprites/white_mask.png", renderer);

    // TESTING - Do some setup for the target painted texture
    SDL_SetTextureBlendMode(f3.getTexture(), SDL_BLENDMODE_ADD);
    SDL_SetTextureAlphaMod(f3.getTexture(), 128);
    target_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                 SDL_TEXTUREACCESS_TARGET, 64, 64);
    SDL_SetTextureBlendMode(target_texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, target_texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    f2.render(renderer);
    SDL_RenderCopy(renderer, f3.getTexture(), NULL, NULL);
    rect.x = 128;
    rect.y = 128;
    rect.w = 64;
    rect.h = 64;
    
    SDL_SetRenderTarget(renderer, NULL);
    
    // TESTING - sprite setup
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
      
      handleEvents();
      
      /* Clear screen */
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);
      
      // TODO: Put rendering code here for entire application
      
      // TESTING
      SDL_RenderCopy(renderer, f.getTexture(), NULL, NULL);
      
      SDL_SetTextureAlphaMod(f2.getTexture(), 255);
      f2.render(renderer, 64, 64);
      
      SDL_SetTextureColorMod(f2.getTexture(), 128, 128, 128);
      f2.render(renderer, 256, 256);
      
      SDL_SetTextureColorMod(f2.getTexture(), 255, 255, 255);
      SDL_SetTextureAlphaMod(f2.getTexture(), 128);
      f2.render(renderer, 192, 192);
      
      SDL_RenderCopy(renderer, target_texture, NULL, &rect);
      
      /* Render Sprite */
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
