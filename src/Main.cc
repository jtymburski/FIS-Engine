/*******************************************************************************
 * Class Name: N/A - int main implementation
 * Date Created: November 17, 2013
 * Inheritance: none
 * Description: The main call implementation. Essentially just sets up
 *              necessary subsystems and starts up the application.
 ******************************************************************************/
#include "Application.h"
#include "Helpers.h"

#include <unistd.h>

bool initSDL()
{
  bool success = true;

  /* Initialize SDL */
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    std::cerr << "[ERROR] SDL could not initialize. SDL error: "
              << SDL_GetError() << std::endl;
    success = false;
  }

  /* Initialize SDL Image library */
  if(!IMG_Init(IMG_INIT_PNG))
  {
    std::cerr << "[ERROR] SDL_Image could not initialize. SDL_image error: "
              << IMG_GetError() << std::endl;
    success = false;
  }

  /* Initialize SDL TTF library */
  if(!TTF_WasInit() && TTF_Init() == -1)
  {
    std::cerr << "[ERROR] SDL_ttf could not initialize. SDL_ttf error: "
              << TTF_GetError() << std::endl;
    success = false;
  }

  /* Init SDL mixer, and open the audio with the chosen settings */
  int buffers = 1024; /* Size of audio buffers in memory - possibly to 4096? */
  int channels = 1;   /* 1 channel = mono, 2 = stereo */
  int rate = Sound::kDEFAULT_FREQUENCY;   /* Frequency of Audio Playback */
  /* 8 mixing channels are chosen by default. Currently have 9. Once sectors
   * are active, will need to enable more (or if any more are added) */
  if(Mix_OpenAudio(rate, AUDIO_S16SYS, channels, buffers) != 0 || !Mix_Init(MIX_INIT_OGG))
  {
    std::cerr << "[WARNING] Unable to initialize audio: "
              << Mix_GetError() << std::endl;
  }

  return success;
}

int main(int argc, char** argv)
{
  /* See if there is a map to skip all proceedings for */
  std::string init_app = "";
  if(argc > 1)
    init_app += argv[1];
  int map_lvl = 0;
  bool map_requested = false;
  if(argc > 2)
  {
    map_lvl = std::stoi(argv[2]);
    map_requested = true;
  }

  /* Get the base directory to the executable, which will be the location of
   * all applicable resources */
  char* directory = SDL_GetBasePath();
  std::string dir_string(directory);
  SDL_free(directory);

  /* Initialize SDL libraries */
  bool success = initSDL();

  if(success)
  {
    /* Create the application and start the run loop */
    Application* game_app = new Application(dir_string, init_app, map_lvl);
    if(game_app->initialize())
      game_app->run(map_requested);

    /* Clean up the application, after the run loop is finished */
    //game_app->uninitialize();
    delete game_app;
    game_app = nullptr;
  }

  /* Clean up SDL libraries */
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
