/*******************************************************************************
 * Class Name: N/A - int main implementation
 * Date Created: November 17, 2013
 * Inheritance: none
 * Description: The main call implementation. Essentially just sets up
 *              necessary subsystems and starts up the application.
 ******************************************************************************/
#include "Application.h"

int main(int argc, char** argv)
{
  /* Set up the game application */
  Application game_app;
  game_app.initialize();
  
  /* Run the game (game loop) */
  game_app.run();
  
  /* Clean up the game */
  game_app.uninitialize();
  
	return 0;
}