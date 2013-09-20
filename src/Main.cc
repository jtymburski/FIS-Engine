/******************************************************************************
* Name: Main function call
* Date Created: February 9, 2012
* Inheritance: none
* Description: This is the first call of the entire game and where it all
*              begins. Merely sets up the sound and QT and runs until the 
*              application close sequence is called. Built to create joy for 
*              all between the age of 17.7-17.9
******************************************************************************/
#include <QApplication>

#include "Application.h"
#include "Sound.h"

/* DELETE: Only for testing */
#include "FileHandler.h"

#undef main

/*============================================================================
 * MAIN FUNCTION
 *===========================================================================*/

int main(int argc, char *argv[])
{
  /* Seed the Random # Generator */
  seed();

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

  /*------------------ Actual Code Start ----------------------*/

  /* Setup the sound */
  Sound::initiateSDL();
  qDebug() << "[DEBUG] Sound configured: " << Sound::statusSDL();

  /* Setup QT */
  QApplication qt_app(argc, argv);
  //QApplication::setGraphicsSystem("opengl"); /* Removed in QT5 */
  //QGL::setPreferredPaintEngine(QPaintEngine::OpenGL); /* Removed in QT5 */
  Application* app = new Application();
  QObject::connect(app, SIGNAL(closing()), &qt_app, SLOT(quit()));
  app->show();

  /* Run QT App */
  int qt_result = qt_app.exec();
  
  /* Clean up QT and the game */
  app->hide();
  delete app;
	
  /* Clean up SDL */
  Sound::cleanupSDL();

  return qt_result;
}
