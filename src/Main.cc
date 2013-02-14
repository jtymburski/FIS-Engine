#include <QtGui/QApplication>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "MainWindow.h"

#undef main

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  /* Frequency of Audio Playback */
	int audio_rate = 22050;
  /* Format of the audio we're playing */
	Uint16 audio_format = AUDIO_S16SYS;
  /* 1 channel = mono, 2 channels = stereo */
	int audio_channels = 2;			//2 channels = stereo
  /* Size of the audio buffers in memory */
	int audio_buffers = 4096;
	
	/* Initialize the SDL audio */
	if (SDL_Init(SDL_INIT_AUDIO) != 0) 
  {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Initialize SDL_mixer with our chosen audio settings */
	if(Mix_OpenAudio(audio_rate, audio_format, 
                   audio_channels, audio_buffers) != 0) 
  {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}

  /* Setup QT */
  MainWindow* mw = new MainWindow();
  QObject::connect(mw, SIGNAL(closing()), &app, SLOT(quit()));

  /* Run QT App */
  int qt_result = app.exec();
  
  /* Clean up QT */
  delete mw;

	/* Clean up SDL */
	Mix_CloseAudio();
	SDL_Quit();	

  return qt_result;
}
