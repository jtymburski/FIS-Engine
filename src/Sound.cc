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
  sound = NULL;
  sound_set = FALSE;
  channel = kUNSET_CHANNEL;

  setSoundFile(path);
  setLoopCount(loop_count);
}

Sound::Sound()
{
  sound = NULL;
  sound_set = FALSE;
  channel = kUNSET_CHANNEL;

  setLoopCount(0);
}

Sound::~Sound()
{
  if(sound_set)
    Mix_FreeChunk(sound);
}

void Sound::play()
{
  channel = Mix_PlayChannel(-1, sound, loop_count);
  if(channel == kUNSET_CHANNEL)
  {
    qDebug() << "[ERROR]Unable to play WAV file " << Mix_GetError();
  }
}

void Sound::stop()
{
  if(channel != kUNSET_CHANNEL)
    Mix_HaltChannel(channel);
  channel = kUNSET_CHANNEL;
}

int Sound::getLoopCount()
{
  return loop_count;
}

bool Sound::setSoundFile(QString path)
{
  if(!path.isEmpty())
  {
    unsetSoundFile();
    sound = Mix_LoadWAV(path.toStdString().c_str());

    /* Determine if the setting of the sound was valid */
    if(sound == NULL)
    {
      qDebug() << "[ERROR]Unable to load WAV file: " << Mix_GetError();
      return FALSE;
    }

    sound_set = TRUE;
    return TRUE;
  }

  qDebug() << "[ERROR]Unable to load empty WAV file path";
  return FALSE;
}

void Sound::setLoopCount(int loop_count)
{
  if(loop_count < 0)
    this->loop_count = kINFINITE_LOOP;
  else
    this->loop_count = loop_count;
}

bool Sound::unsetSoundFile()
{
  Mix_FreeChunk(sound);
  sound = NULL;

  if(sound_set)
  {
    sound_set = FALSE;
    return TRUE;
  }
  return FALSE;
}
