/*******************************************************************************
 * Class Name: N/A - int main implementation
 * Date Created: November 17, 2013
 * Inheritance: none
 * Description: The main call implementation. Essentially just sets up
 *              necessary subsystems and starts up the application.
 ******************************************************************************/
#include "Application.h"
#include "Helpers.h"

// TODO - Remove testing headers
#include "Game/Sprite.h"

int main(int argc, char** argv)
{
  /* The initial game seeding - for random number generation */
  Helpers::seed();
  
  // TODO: Remove - split testing
  std::string test = "Hello,sunshine,what,a,beautiful,day";
  std::vector<std::string> temp_list;
  Helpers::split(test, ',', temp_list);
  printf("%d\n", temp_list.size());
  for(int i = 0; i < temp_list.size(); i++)
    printf("%s\n", temp_list[i].c_str());
  
  // TODO: Remove - sprite testing
  Sprite test_sprite("HALO|2|.png", NULL);
  
  // TODO: Remove - testing static cast on enum additives
  //SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
  //printf("%d %d %d\n", flip, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL);
  //flip &= ~SDL_FLIP_HORIZONTAL;
  //printf("%d\n", flip);
  
  /* Set up the game application and then run */
  Application game_app;
  if(game_app.initialize())
    game_app.run();
  
  /* Clean up the game */
  game_app.uninitialize();
  
	return 0;
}
