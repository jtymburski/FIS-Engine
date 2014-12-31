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
  int buffers = 1024; /* Size of audio buffers in memory */
  int channels = 2;   /* 1 channel = mono, 2 = stereo */
  int rate = 22050;   /* Frequency of Audio Playback */
  if(!Mix_Init(MIX_INIT_OGG) || 
     Mix_OpenAudio(rate, AUDIO_S16SYS, channels, buffers) != 0)
  {
    std::cerr << "[WARNING] Unable to initialize audio: " 
              << Mix_GetError() << std::endl;
  }

  return success;
}

int main(int argc, char** argv)
{
  (void)argc;
  (void)argv;

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
    Application* game_app = new Application(dir_string);
    if(game_app->initialize())
      game_app->run();
  
    /* Clean up the application, after the run loop is finished */
    game_app->uninitialize();
    delete game_app;
    game_app = NULL;

  }
  
  /* Clean up SDL libraries */
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  
	return 0;
}
