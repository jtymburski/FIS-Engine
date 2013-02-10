/******************************************************************************
* Class Name: Sound
* Date Created: February 9, 2012
* Inheritance: MediaObject
* Description: Handles all sound used within the game. Must use this instead
*              of QSound since Linux doesn't cleanly support QSound except for
*              a specific library requirement for the sound drivers.
******************************************************************************/
#include "Sound.h"

Sound::Sound(QString path, int loop_count)
{
  setup();
  setSourceFile(path);
  setLoopCount(loop_count);
}

Sound::Sound()
{
  setup();
  setLoopCount(0);
}

Sound::~Sound()
{
  delete audio_ctrl;
}

void Sound::queueSong()
{
  if(loop_count != 0)
  {
    enqueue(Phonon::MediaSource(source_path));
    setLoopCount(loop_count-1);
  }
}

void Sound::stopSong()
{
  if(loop_count == 0)
  {
    emit finishedSequence();
  }
}

void Sound::setup()
{
  audio_ctrl = new Phonon::AudioOutput();
  Phonon::createPath(this, audio_ctrl);

  source_path = "";
  source_set = FALSE;

  QObject::connect(this, SIGNAL(aboutToFinish()), 
                   this, SLOT(queueSong()));
  QObject::connect(this, SIGNAL(currentSourceChanged(Phonon::MediaSource)), 
                   this, SLOT(stopSong()));
  QObject::connect(this, SIGNAL(finishedSequence()), 
                   this, SLOT(clear()));
}

int Sound::getLoopCount()
{
  return loop_count;
}

bool Sound::setSourceFile(QString path)
{
  if(!path.isEmpty())
  {
    setCurrentSource(Phonon::MediaSource(path));
    source_path = path;
    source_set = TRUE;

    return TRUE;
  }
  return FALSE;
}

void Sound::setLoopCount(int loop_count)
{
  if(loop_count < 0)
    this->loop_count = kINFINITE_LOOP;
  else
    this->loop_count = loop_count;
}
