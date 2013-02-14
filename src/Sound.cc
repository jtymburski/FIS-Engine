/******************************************************************************
* Class Name: Sound
* Date Created: February 9, 2012
* Inheritance: MediaObject
* Description: Handles all sound used within the game. Must use this instead
*              of QSound since Linux doesn't cleanly support QSound except for
*              a specific library requirement for the sound drivers. This 
*              individual class only handles playing a single sound at one
*              time. If you click play while it's still playing, it will stop
*              the previous execution. However, if you create multiple sound
*              files, SDL allows up to 8 files being mixed together at
*              once before returning the channel full error.
* Notes: This requires that SDL has been setup already and set to run. This
*        is typically done with SDL_Init(*) and Mix_OpenAudio(*).
******************************************************************************/
#include "Sound.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Sets up a sound with the path given
 *              and with a loop count to play it for.
 *
 * Inputs: QString path - the path to the audio file (in WAV)
 *         int loop_count: the number of time you want the audio to loop.
 *                         Default is 0. If a 0 is used, it will automatically
 *                         default to 1 loop.
 */
Sound::Sound(QString path, int loop_count)
{
  sound = NULL;
  sound_set = FALSE;
  channel = kUNSET_CHANNEL;

  setSoundFile(path);
  setPlayCount(loop_count);
}

/*
 * Description: Constructor function - Sets up a shell for a sound. It will
 *              play nothing though, since no sound file is setup.
 *
 * Inputs: none
 */
Sound::Sound()
{
  sound = NULL;
  sound_set = FALSE;
  channel = kUNSET_CHANNEL;

  setPlayCount(0);
}

/* 
 * Description: Destructor function
 */
Sound::~Sound()
{
  unsetSoundFile();
}

/*============================================================================
 * SIGNALS / SLOTS 
 *===========================================================================*/

/* 
 * Description: Play slot. This handles playing the sound file that was
 *              configured in this class. It will use the number of loops
 *              setup as well as the file. If the file was playing, it
 *              will stop it before. Therefore, it will act as a reset as
 *              well. If errors occur, they are pushed through qDebug()
 *              to the terminal.
 *
 * Inputs: none
 * Output: none
 */
void Sound::play()
{
  /* Ensure the channel is stopped if it was in use */
  stop();

  /* Try to play on a new channel */
  channel = Mix_PlayChannel(-1, sound, loop_count);
  if(channel == kUNSET_CHANNEL)
    qDebug() << "[ERROR]Unable to play WAV file " << Mix_GetError();
}

/* 
 * Description: Stop slot. This handles stopping the sound file that was
 *              configured in this class. It will first check if the channel
 *              integer is set and if it isn't, it will halt the channel.
 *              Note: if the channel is already stopped, due to the number
 *              of loops expiring, this function won't fail and it will
 *              just notify the class that the audio has been stopped.
 *
 * Inputs: none
 * Output: none
 */
void Sound::stop()
{
  if(channel != kUNSET_CHANNEL)
    Mix_HaltChannel(channel);
  channel = kUNSET_CHANNEL;
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Gets the number of times that the audio will loop for when
 *              the play() function is called. If it returns -1, this
 *              represents it will loop infinitely until stopped using stop().
 *
 * Inputs: none
 * Output: int - returns the number of times the audio will play for
 */
int Sound::getPlayCount()
{
  if(loop_count == kINFINITE_LOOP)
    return loop_count;
  return loop_count + 1;
}

/* 
 * Description: Sets the sound to be played the next time the play() 
 *              function is called. This tries to open it and if it
 *              fails, it will not be set and the class will be notified
 *              as well as the terminal. If the sound file was set and this
 *              set was unsuccessful, there will be no sound file set anymore.
 *
 * Inputs: QString path - the path to the sound to add
 * Output: bool - status if the setting of the sound file was successful
 */
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

/* 
 * Description: Sets the number of times that the audio file should loop for
 *              before halting to repeat when the play() function is called.
 *              If a 0 is used, it will default to only playing once and the
 *              number represents the number of times it will play (1=once, 
 *              etc.). If -1 is used, it will loop forever.
 *              position.
 *
 * Inputs: int play_count - the number of times the file should play for
 * Output: bool - status if play count set was successful. If it isn't, it 
 *                will default to 1 play.
 */
bool Sound::setPlayCount(int play_count)
{
  if(play_count < 0)
    loop_count = kINFINITE_LOOP;
  else if(play_count > 0)
    loop_count = play_count - 1;
  else
  {
    loop_count = 0;
    return FALSE;
  }
  return TRUE;
}

/* 
 * Description: Unsets the sound file in this class. If it's set, the memory 
 *              is freed, the pointer is nulled and the class is notified of
 *              the change. Otherwise, it just cleans up the pointer logic
 *              to ensure that the sound hasn't been set. Will be called on
 *              cleanup and destruction as well.
 *
 * Inputs: none
 * Output: bool - returns if the sound file was set before calling this 
 *                function.
 */
bool Sound::unsetSoundFile()
{
  if(sound_set)
  {
    Mix_FreeChunk(sound);
    sound = NULL;
    sound_set = FALSE;
    return TRUE;
  }

  sound = NULL;
  return FALSE;
}
