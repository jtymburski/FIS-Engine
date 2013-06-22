#include <QtGui/QApplication>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "Application.h"

/* DELETE: Only for testing */
#include "FileHandler.h"

#undef main

/*============================================================================
 * MAIN FUNCTION
 *===========================================================================*/

int main(int argc, char *argv[])
{
  /* Testing code for file handler */
  bool done = false;
  bool success = true;
  FileHandler fh("TEST.log", false, true, false);

  /* Read */
  if(fh.start())
  {
    qDebug() << "Reading: " << fh.getDate();

    /* REGULAR */
    /*while(!done) 
      qDebug() << fh.readRegularLine(&done);*/

    /* XML */
    while(!done && success)
    {
      XmlData data = fh.readXmlData(&done, &success);

      if(!done)
      {
        for(int i = 0; i < data.getNumElements(); i++)
          qDebug() << data.getElement(i) + ": " + data.getKey(i) + " - " 
                                         + data.getKeyValue(i);

        if(data.isDataBool())
        {
          qDebug() << "  " << data.getDataBool();
        }
        else if(data.isDataFloat())
        {
          qDebug() << "  " << data.getDataFloat();
        }
        else if(data.isDataInteger())
        {
          qDebug() << "  " << data.getDataInteger();
        }
        else if(data.isDataString())
        {
          qDebug() << "  " << data.getDataString();
        }
      }
    }

    if(!success)
      qDebug() << "Error reading XML.";

    fh.stop();
  }
  else
  {
    qDebug() << "Reading file fail.";
  }

  /* Write */
  fh.setWriteEnabled(true);
  success = true;
  if(fh.start())
  {
    qDebug() << "Writing: " << fh.getDate();

    /* REGULAR */
    /*fh.writeRegularLine("Let's write a little story today.");
    fh.writeRegularLine("What's it going to be about you say?");
    fh.writeRegularLine("Well it will be interesting, I tell you.");
    fh.writeRegularLine("Let's begin?");*/

    /* XML */
    success &= fh.writeXmlElement("persons");
    success &= fh.writeXmlElement("person", "index", "0");
    success &= fh.writeXmlData("name", FileHandler::STRING, "john");
    success &= fh.writeXmlData("gender", FileHandler::STRING, "male");
    success &= fh.writeXmlData("available", FileHandler::BOOLEAN, "true");
    success &= fh.writeXmlElementEnd();
    success &= fh.writeXmlData("test", FileHandler::FLOAT, "0.25");

    fh.stop(!success);
  }
  else
  {
    qDebug() << "Writing file fail.";
  }

  /* End Test */

  QApplication::setGraphicsSystem("raster");
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
  QApplication::setGraphicsSystem(QLatin1String("opengl"));
  Application* game = new Application();
  QObject::connect(game, SIGNAL(closing()), &app, SLOT(quit()));
  game->show();

  /* Run QT App */
  int qt_result = app.exec();
  
  /* Clean up QT and the game */
  game->hide();
  delete game;
	
  /* Clean up SDL */
	Mix_CloseAudio();
	SDL_Quit();	

  return qt_result;
}
