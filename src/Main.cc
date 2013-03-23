#include <QtGui/QApplication>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "Application.h"

/* Test - to delete */
#include "FileHandler.h"

#undef main

int main(int argc, char *argv[])
{
  /* Testing code for file handler */
  bool done = FALSE;
  FileHandler fh;
  fh.setEncryptionEnabled(FALSE);
  fh.setFilename("TEST.log");
  fh.setFileType(FileHandler::REGULAR);
  
  /* Read */
  fh.setWriteEnabled(FALSE);
  if(fh.start())
  {
    qDebug() << "Reading: " << fh.getDate();

    while(!done)
      qDebug() << fh.readLine(&done);

    fh.stop();
  }
  else
  {
    qDebug() << "Reading file fail.";
  }

  /* Public static functions */
  qDebug() << "File 'Test.log' exists: " << FileHandler::fileExists("Test.log");
  qDebug() << "File 'TEST.log' exists: " << FileHandler::fileExists("TEST.log");

  qDebug() << "File 'TEMP'->'TEST.log': " 
           << FileHandler::fileRename("TEMP", "TEST.log");
  qDebug() << "File 'BLAH'->'BLAH2': " 
           << FileHandler::fileRename("BLAH", "BLAH2");
  qDebug() << "File 'TEST.log'->'D.log': " 
           << FileHandler::fileRename("TEST.log", "D.log");

  qDebug() << "File 'Test.log' delete: " << FileHandler::fileDelete("Test.log");
  qDebug() << "File 'D.log' delete: " << FileHandler::fileDelete("D.log");
  qDebug() << "File 'D.log' delete: " << FileHandler::fileDelete("D.log");
  

  /* Write */
  fh.setWriteEnabled(TRUE);
  if(fh.start())
  {
    qDebug() << "Writing: " << fh.getDate();

    fh.writeLine("Let's write a little story today.");
    fh.writeLine("What's it going to be about you say?");
    fh.writeLine("Well it will be interesting, I tell you.");
    fh.writeLine("Let's begin?");

    fh.stop();
  }
  else
  {
    qDebug() << "Writing file fail.";
  }

  /* End Test */

  QApplication app(argc, argv);

  /* Frequency of Audio Playback */
	int audio_rate = 22050;
  /* Format of the audio we're playing */
	Uint16 audio_format = AUDIO_S16SYS;
  /* 1 channel = mono, 2 channels = stereo */
	int audio_channels = 2;			//2 channels = stereo
  /* Size of the audio buffers in memory */
	int audio_buffers = 1024;
	
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
  Application* game = new Application();
  QObject::connect(game, SIGNAL(closing()), &app, SLOT(quit()));

  /* Run QT App */
  int qt_result = app.exec();
  
  /* Clean up QT */
  delete game;

	/* Clean up SDL */
	Mix_CloseAudio();
	SDL_Quit();	

  return qt_result;
}
