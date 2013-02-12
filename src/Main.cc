#include <QtGui/QApplication>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "MainWindow.h"

#undef main

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  //app.setApplicationName("Univursa Game");

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

/*
	//Load our WAV file from disk
	sound = Mix_LoadWAV("alarm.wav");
  sound2 = Mix_LoadWAV("background_menu_sound.wav");
	if(sound == NULL) {
		printf("Unable to load WAV file: %s\n", Mix_GetError());
	}
	
	//Play our sound file, and capture the channel on which it is played
	channel = Mix_PlayChannel(-1, sound, 0);
  channel2 = Mix_PlayChannel(-1, sound2, 0);
	if(channel == -1) {
		printf("Unable to play WAV file: %s\n", Mix_GetError());
	}
*/
  MainWindow mw;
  QObject::connect(&mw, SIGNAL(closing()), &app, SLOT(quit()));

  int qt_result = app.exec();

/*
	//Release the memory allocated to our sound
	Mix_FreeChunk(sound);
*/
	//Need to make sure that SDL_mixer and SDL have a chance to clean up
	Mix_CloseAudio();
	SDL_Quit();	

  return qt_result;
}
