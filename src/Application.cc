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
const std::string Application::kLOGO_ICON =
    "sprites/General/univ-logo-small.png";
const std::string Application::kPATH = "maps/Univursa.ugv";
const bool Application::kPATH_ENCRYPTED = false;
const int Application::kPATH_MAP = 0;
const uint8_t Application::kUPDATE_CHANGE_LIMIT = 5;
const uint8_t Application::kUPDATE_RATE = 32;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Application::Application(std::string base_path, std::string app_path,
                         int app_map)
{
  /* Start game with a default state */
  game_handler = new Game();

  /* Initialize the variables */
  if(app_path.empty())
  {
    this->app_path = kPATH;
    this->app_map = kPATH_MAP;
  }
  else
  {
    this->app_path = app_path;
    this->app_map = app_map;
  }

  this->base_path = base_path;
  initialized = false;
  renderer = NULL;
  window = NULL;

  /* System Options */
  system_options = new Options(base_path);
  system_options->setSoundHandler(&sound_handler);

  /* Initialize update variables */
  update_rate = kUPDATE_RATE;
  update_sync = 0;

  /* Game Handler */
  game_handler->setConfiguration(system_options);
  game_handler->setSoundHandler(&sound_handler);

  /* Title Screen */
  title_screen.setConfig(system_options);
  title_screen.setSoundHandler(&sound_handler);

  /* Sets the current mode - paused mode */
  // changeMode(PAUSED);
  mode = NONE;
  mode_next = NONE;
  changeMode(PAUSED);
}

Application::~Application()
{
  /* Clear the options class */
  delete system_options;
  system_options = NULL;

  /* Uninitialize */
  uninitialize();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Change the mode that the application is running */
bool Application::changeMode(AppMode mode)
{
  /* Run logic to determine if mode switch is allowed - currently empty */
  bool allow = true;

  /* If allowed, make change */
  if(allow && this->mode != mode)
  {
    /* Changes to execute on the views closing */
    // if(this->mode == TITLESCREEN)
    //   title_screen.enableView(false);
    if(this->mode == GAME)
      game_handler->enableView(false);
    else if(this->mode == PAUSED)
      Sound::resumeAllChannels();

    mode_next = mode;
    // this->mode = mode;

    /* Changes to execute on the views opening */
    // if(mode == TITLESCREEN)
    //   title_screen.enableView(true);
    if(mode == GAME)
      game_handler->enableView(true);
    else if(mode == PAUSED)
      Sound::pauseAllChannels();
    // else if(mode == LOADING)
    //  displayLoadingFrame();

    /* Update views */
    updateViews(0);
  }

  return allow;
}

/* Display loading frame */
void Application::displayLoadingFrame()
{
  /* Render draw color and clear */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  /* Render loading frame */
  load_frame.render(renderer, 0, 0, system_options->getScreenWidth(),
                    system_options->getScreenHeight());
  SDL_RenderPresent(renderer);
}

/* Goes through all available events that are currently on the stack */
void Application::handleEvents()
{
  SDL_Event event;

  /* Pump events in the key handler to the get the current state of Keyboard */
  key_handler.update(0);

  while(SDL_PollEvent(&event) != 0)
  {
    /* If quit initialized, end the game loop */
    if(event.type == SDL_QUIT)
    {
      changeMode(EXIT);
    }

    /* Otherwise, pass the key down events on to the active view */
    else if(event.type == SDL_KEYDOWN)
    {
#ifdef UDEBUG
      SDL_KeyboardEvent press_event = event.key;

      /* -- Refresh config: cycle maps -- */
      if(press_event.keysym.sym == SDLK_F10)
      {
        /* Decide on which reload */
        if(app_path != "maps/design_map.ugv")
        {
          setPath("maps/design_map.ugv");
        }
        else
        {
          int new_map = app_map + 1;
          if(new_map > 2)
            new_map = 0;
          setPath(app_path, new_map);
        }
      }
      /* -- Full Screen toggle -- */
      else if(press_event.keysym.sym == SDLK_F11)
      {
        uint32_t full_flag = SDL_WINDOW_FULLSCREEN;
        uint32_t flags = SDL_GetWindowFlags(window);
        bool is_fullscreen = (flags & full_flag) > 0;

        /* If full screen, disable */
        if(is_fullscreen)
        {
          SDL_SetWindowFullscreen(window, 0);
        }
        /* otherwise, enable */
        else
        {
          SDL_SetWindowFullscreen(window, full_flag);
        }
      }
      else if(mode == GAME)
      {
        game_handler->keyTestDownEvent(press_event);
      }
      else if(mode == TITLESCREEN)
      {
        title_screen.keyTestDownEvent(press_event);
      }
#endif

      /* -- Pause toggle -- */
      if(key_handler.isDepressed(GameKey::PAUSE))
      {
        if(mode == PAUSED)
        {
          revertMode();
        }
        else
        {
          mode_temp = mode;
          changeMode(PAUSED);
        }
      }
      /* Send the key to the relevant view */
      if(mode == TITLESCREEN)
      {
        title_screen.keyDownEvent(key_handler);
      }
      else if(mode == GAME)
      {
        game_handler->keyDownEvent(key_handler);
      }
    }
    else if(event.type == SDL_KEYUP)
    {
      /* Send the key to the relevant view */
      if(mode == GAME)
        game_handler->keyUpEvent(key_handler);
    }
  }
}

/* Load */
bool Application::load()
{
  bool done = false;
  int index = 0;
  bool read_success = true;
  bool success = true;

  /* Create the file handler */
  FileHandler fh(app_path, false, true, kPATH_ENCRYPTED);
  XmlData data;

  /* Start the file read */
  success &= fh.start();

  /* If file open was successful, move forward */
  if(success)
  {
    std::cout << "--" << std::endl
              << "Application Load: " << fh.getDate() << std::endl
              << "--" << std::endl;

    /* Display loading frame */
    displayLoadingFrame();

    /* First unload */
    unload();

    /* Declare timer for application load time */
    Timer t;

    do
    {
      /* Read set of XML data */
      data = fh.readXmlData(&done, &read_success);
      success &= read_success;

      /* Only proceed if defined for core application */
      if(data.getElement(index) == "app")
      {
        /* Sounds */
        if(data.getElement(index + 1) == "music" ||
           data.getElement(index + 1) == "sound")
        {
          sound_handler.load(data, index + 1, system_options->getBasePath());
        }
      }
    } while(!done && success);

    /* Print out the time to create all the music/sounds */
    std::cout << "App Load Time: " << t.elapsed() << "s" << std::endl;
  }

  /* Stop the file read */
  success &= fh.stop();

  /* If success, load into game handler with noted map */
  if(success)
    game_handler->setPath(app_path, app_map, false);

  /* Change mode back to title screen */
  changeMode(TITLESCREEN);

  return success;
}

/* Renders the current view and all relevant visual data */
void Application::render(uint32_t cycle_time)
{
  /* Handle the individual action items, depending on whats running */
  if(mode == TITLESCREEN)
  {
    title_screen.render(renderer);
  }
  else if(mode == GAME)
  {
    if(game_handler->getMode() == Game::LOADING)
      displayLoadingFrame();
    game_handler->render(renderer);
  }
  else if(mode == LOADING)
  {
    displayLoadingFrame();
  }
  else if(mode == OPTIONS)
  {
    cycle_time = cycle_time; // TODO: DO OPTIONS EXECUTION
  }
  else if(mode == PAUSED)
  {
    cycle_time = cycle_time;
  }
}

/* Revert to temporary mode */
bool Application::revertMode()
{
  changeMode(mode_temp);

  return false;
}

/* Uninitializes all set functions in the application. Used to wind down
 * and no rendering will take place after this. */
void Application::uninitialize()
{
  /* Unloads application data */
  unload();

  /* Delete game */
  delete game_handler;
  game_handler = nullptr;

  /* Clean up the renderer */
  if(renderer != NULL)
  {
    Helpers::deleteMasks();
    SDL_DestroyRenderer(renderer);
  }
  renderer = NULL;

  /* Clean up the window */
  if(window != NULL)
    SDL_DestroyWindow(window);
  window = NULL;

  initialized = false;
}

/* Unloads all loaded application data */
void Application::unload()
{
  /* Change mode back to title screen */
  // changeMode(TITLESCREEN);

  /* Clean up game */
  game_handler->unload();

  /* Clean up sounds */
  sound_handler.removeAll();
}

/* Returns the latched cycle time */
int Application::updateCycleTime(int cycle_time)
{
  // uint8_t update_step = kUPDATE_RATE / 2;
  uint8_t update_time = 0;
  // std::cout << "Cycle time: " << cycle_time << std::endl;

  /* Parse the cycle time and find the category */
  if(cycle_time < 0)
  {
    update_time = 0;
  }
  else if(cycle_time <= kUPDATE_RATE)
  {
    update_time = kUPDATE_RATE;
  }
  else
  {
    update_time =
        ((cycle_time + kUPDATE_RATE / 2) / kUPDATE_RATE) * kUPDATE_RATE;
  }

  // /* Check if the update time is different */
  // if(update_time == update_rate)
  // {
  // if(update_sync > 0)
  // update_sync--;
  // }
  // else
  // {
  // update_sync++;
  // }

  // /* Determine if an update is required */
  // if(update_sync > kUPDATE_CHANGE_LIMIT)
  // {
  // update_rate = update_time;
  // update_sync = 0;
  // }

  return update_time;
}

/* Handles actions in views, depending on what's active */
bool Application::updateViews(int cycle_time)
{
  bool quit = false;

  /* Update the key handler */
  key_handler.update(cycle_time);

  /* Mode next handling */
  if(mode_next != NONE)
  {
    if(mode == GAME)
    {
      if(game_handler->isModeDisabled())
      {
        mode = mode_next;
        mode_next = NONE;
      }
    }
    else
    {
      mode = mode_next;
      mode_next = NONE;
    }
  }

  /* Handle any appropriate actions of the individual views */
  if(mode == TITLESCREEN)
  {

    /* Update the title screen, which returns if an action is available */
    if(title_screen.update(cycle_time))
    {
      /* If action is available, get it, and parse it to change the mode */
      MenuType menu_type = title_screen.getActiveTitleMenu();

      // if(menu_type == MenuType::TITLE_QUIT)
      //   changeMode(EXIT);
      // else if(menu_type == MenuType::TITLE_NEW_GAME)
        changeMode(GAME);
    }
  }
  /* Otherwise, update the game and check if the game is finished */
  else if(mode == GAME)
  {
    if(game_handler->update(cycle_time))
    {
      if(mode_next == NONE)
        changeMode(TITLESCREEN);
    }
  }
  /* Loading */
  else if(mode == LOADING)
  {
    load();
  }
  /* If exit, return true to notify the running thread the application is
   * done */
  else if(mode == EXIT)
  {
    quit = true;
  }

  if(system_options)
    system_options->update();

  return quit;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Getter access functions */
SDL_Renderer* Application::getRenderer()
{
  return renderer;
}

/* Getter access functions */
SDL_Window* Application::getWindow()
{
  return window;
}

/* Provides initialization of the SDL engine. Required for running. */
bool Application::initialize()
{
  bool success = !isInitialized();

  /* Only proceed if successful */
  if(success)
  {
    /* Create window for display */
    uint32_t flags = SDL_WINDOW_SHOWN;
    if(system_options->isFullScreen())
      flags |= SDL_WINDOW_FULLSCREEN;
    window = SDL_CreateWindow("Univursa", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              system_options->getScreenWidth(),
                              system_options->getScreenHeight(), flags);

    /* If failed, create error */
    if(window == NULL)
    {
      std::cerr << "[ERROR] Window could not be created. SDL error: "
                << SDL_GetError() << std::endl;
      success = false;
    }
    /* Otherwise, create window icon */
    else
    {
      std::string icon_path = base_path + kLOGO_ICON;
      SDL_Surface* surface = IMG_Load(icon_path.c_str());
      SDL_SetWindowIcon(window, surface);
    }
  }

  /* Create the renderer for the window */
  if(success)
  {
    /* Set up the renderer flags */
    uint32_t flags = SDL_RENDERER_ACCELERATED;
    if(system_options->isVsyncEnabled())
      flags |= SDL_RENDERER_PRESENTVSYNC;

    auto driver_index = -1;

#ifdef _WIN32_OPENGL
    /* Force OpenGL to be used as the rendering driver if there are more than
       one rendering drivers available */
    if(SDL_GetNumRenderDrivers() > 1)
    {
      std::string open_gl = "opengl";

      /* Loop through each driver, comparing its value to "opengl" */
      for(int32_t i = 0; i < SDL_GetNumRenderDrivers() && driver_index == -1;
          i++)
      {
        auto renderer_info = new SDL_RendererInfo;
        auto error_index = SDL_GetRenderDriverInfo(i, renderer_info);

        if(error_index > -1 && renderer_info != nullptr)
        {
          std::cout << "Checking Driver: " << renderer_info->name << std::endl;
          if(std::strcmp(open_gl.c_str(), renderer_info->name) == 0)
            driver_index = i;
        }
        else
        {
          std::cerr << "[ERROR]: Error attempting to discern ren. driver"
                    << std::endl;
        }

        delete renderer_info;
      }

      /* Create the renderer */
      renderer = SDL_CreateRenderer(window, driver_index, flags);

      /* Print out the chosen rendering driver info */
      if(renderer != nullptr)
      {
        auto renderer_info = new SDL_RendererInfo;
        SDL_GetRenderDriverInfo(0, renderer_info);
        auto error_index = SDL_GetRendererInfo(renderer, renderer_info);

        if(error_index > -1 && renderer_info != nullptr)
          std::cout << "Rendering Driver: " << renderer_info->name << std::endl;
        else
          std::cerr << "[ERROR] Unable to get rendering driver info."
                    << std::endl;

        delete renderer_info;
      }
    }
    else
    {
      /* If only one renderer exists, let SDL 'choose' */
      renderer = SDL_CreateRenderer(window, -1, flags);
    }
#else // _WIN32_OPENGL
    renderer = SDL_CreateRenderer(window, -1, flags);
#endif

    if(renderer == NULL)
    {
      std::cerr << "[ERROR] Renderer could not be created. SDL error: "
                << SDL_GetError() << std::endl;
      success = false;
    }

    else
    {
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

      /* Set game handler renderer */
      game_handler->setRenderer(renderer);

      /* Create helper graphical portions */
      Helpers::createMaskBlack(renderer);
      Helpers::createMaskWhite(renderer);
      load_frame.setTexture(system_options->getBasePath() +
                                "sprites/General/loading.png",
                            renderer);

      /* Set render color */
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    }
  }

  /* Test the font engine */
  if(!system_options->confirmFontSetup())
  {
    std::cerr << "[ERROR] Could not create font. This is either a library "
              << "issue or the font files are missing or invalid." << std::endl;
    success = false;
  }

  /* If successful, attempt final load sequence */
  if(success)
  {
    // success &= load();

    // /* Set the title screen background - TODO: Encapsulate in load?? */
    // title_screen.setBackground("sprites/Title/old_title.png", renderer);

    if(success)
    {
      initialized = true;
      changeMode(LOADING);
      // game_handler.setPath(app_path, app_map, false);
    }
  }

  /* Uninitialize everything, if the init sequence failed */
  if(!isInitialized())
    uninitialize();

  return success;
}

/* Returns the status if the application libraries have been initialized */
bool Application::isInitialized()
{
  return initialized;
}

/* Runs the application */
bool Application::run(bool skip_title)
{
  uint32_t count = 1;
  uint32_t cycle_time = kUPDATE_RATE;
  bool quit = false;
  uint32_t ticks = 0;

  /* Font testing - TODO: Remove */
  // Text text1;
  // text1.setFont("fonts/colab_light.otf", 16, TTF_STYLE_BOLD);
  // text1.setText(renderer, "341", {255, 255, 255, 255});
  // Text text2;
  // text2.setFont("fonts/colab_light.otf", 16, TTF_STYLE_BOLD);
  // TTF_SetFontOutline(text2.getFont(), 2);
  // text2.setText(renderer, "341", {0, 0, 0, 255});

  if(isInitialized())
  {
    /* If skip title triggered, jump straight to map */
    if(skip_title)
      changeMode(GAME);

    /* Main application loop */
    while(!quit)
    {
      /* Determine the previous cycle time for using throughout the update
       * sequence for rendering */
      uint32_t new_ticks = SDL_GetTicks();
      if(new_ticks > ticks + 200)
      {
        cycle_time = 200 / count;

        // // TODO: Cycle fix hack!
        // if(cycle_time > 33)
        //   cycle_time = 16;

        count = 0;
        ticks = new_ticks;
      }

      // std::cout << cycle_time << std::endl;
      // cycle_time = new_ticks - ticks;
      // ticks = new_ticks;
      // if(system_options->isVsyncEnabled())
      //  cycle_time = updateCycleTime(cycle_time);

      /* Handle events - key press, window events, and such */
      handleEvents();

      /* Update the view control (moving sprites, players, etc.)
       * This returns true if the application should shut down */
      if(updateViews(cycle_time))
        quit = true;

      /* Play through sound queue */
      sound_handler.process();

      /* Clear screen */
      if(mode != PAUSED)
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Render the application view */
        render(cycle_time);

        // Font testing - TODO: Remove
        // text2.render(renderer, 48, 48);
        // text1.render(renderer, 50, 50);

        /* Update screen */
        SDL_RenderPresent(renderer);
      }

      count++;

      /* Delay if VSync is not enabled */
      if(!system_options->isVsyncEnabled())
        SDL_Delay(12);
    }

    return true;
  }
  return false;
}

/* Sets the application path */
void Application::setPath(std::string path, int level, bool skip_title)
{
  if(!path.empty() && level >= 0 && isInitialized())
  {
    /* If path is different then app path, full re-load */
    if(app_path != path)
    {
      /* Unload internal resources */
      // unload();

      /* Set new path */
      app_path = path;
      app_map = level;
      changeMode(LOADING);

      /* Load */
      // load();
    }
    /* Otherwise, only level update */
    else if(app_map != level)
    {
      /* Unload game sub data */
      // game_handler.unloadSub();

      /* Set the new level */
      app_map = level;
      game_handler->setPath(app_path, app_map, false);

      /* If mode is already in game, update the map accordingly */
      // if(mode == GAME)
      //{
      //  if(!game_handler.isLoadedCore())
      //    game_handler.load(renderer, true);
      //  else if(!game_handler.isLoadedSub())
      //    game_handler.load(renderer, false);
      //}
    }

    /* Go to game mode, if skip title is enabled */
    if(skip_title && mode != GAME)
      changeMode(GAME);
  }
}
