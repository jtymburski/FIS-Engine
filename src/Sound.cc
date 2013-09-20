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

/* Constant Implementation - see header file for descriptions */
const short Sound::kINIT_BUFFERS = 1024;
const short Sound::kINIT_CHANNELS = 2;
const Uint16 Sound::kINIT_FORMAT = AUDIO_S16SYS;
const short Sound::kINIT_RATE = 22050;
const short Sound::kINFINITE_LOOP = -1;
const short Sound::kUNSET_CHANNEL = -1;

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
  sound_set = false;
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
  sound_set = false;
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
 * SLOTS 
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

  /* Only proceed if the sound SDL layer is ready to run */
  if(sound_set && statusSDL())
  {
    /* Try to play on a new channel */
    channel = Mix_PlayChannel(-1, sound, loop_count);
    if(channel == kUNSET_CHANNEL)
      printf("[ERROR] Unable to play WAV file: %s\n", Mix_GetError());
  }
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
 * PUBLIC FUNCTIONS
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
  if(statusSDL())
  {
    if(!path.isEmpty())
    {
      unsetSoundFile();
      sound = Mix_LoadWAV(path.toStdString().c_str());

      /* Determine if the setting of the sound was valid */
      if(sound == NULL)
      {
        printf("[ERROR] Unable to load WAV file: %s\n", Mix_GetError());
        return false;
      }

      sound_set = true;
      return true;
    }

    printf("[ERROR] Unable to load empty WAV file path\n");
    return false;
  }
  return false;
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
    return false;
  }
  return true;
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
    sound_set = false;
    return true;
  }

  sound = NULL;
  return false;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Cleans up SDL sound information by closing the appropriate
 *              streams and calling close on the mixer and SDL.
 *
 * Inputs: none
 * Output: none
 */
void Sound::cleanupSDL()
{
  Mix_CloseAudio();
  SDL_Quit();
}

/* 
 * Description: Initializes the sound information to set up SDL in order for
 *              sound to work throughout the game. If the sound is not set up,
 *              no sound will be able to play.
 *
 * Inputs: none
 * Output: bool - status if the sound initiation was successful
 */
bool Sound::initiateSDL()
{
  /* Frequency of Audio Playback */
  int audio_rate = kINIT_RATE;
  /* Format of the audio we're playing */
  Uint16 audio_format = kINIT_FORMAT;
  /* 1 channel = mono, 2 channels = stereo */
  int audio_channels = kINIT_CHANNELS;
  /* Size of the audio buffers in memory */
  int audio_buffers = kINIT_BUFFERS;
  /* Success of the initialization */
  bool success = true;
  
  /* Initialize the SDL audio */
  if (SDL_Init(SDL_INIT_AUDIO) != 0) 
  {
    printf("[ERROR] Unable to initialize SDL: %s\n", SDL_GetError());
    success = false;
  }

  /* Initialize SDL_mixer with our chosen audio settings */
  if(Mix_OpenAudio(audio_rate, audio_format, 
                   audio_channels, audio_buffers) != 0) 
  {
    printf("[ERROR] Unable to initialize audio: %s\n", Mix_GetError());
    success = false;
  }

  //printf("%d\n", Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels));

  return success;
}

/*
 * Description: Returns if the SDL sound system has been configured and is up
 *              and running to allow for sounds to be played.
 *
 * Inputs: none
 * Output: bool - status if the SDL sound is configured and running
 */
bool Sound::statusSDL()
{
  /* Info on the configured audio */
  int audio_rate = kINIT_RATE;
  Uint16 audio_format = kINIT_FORMAT;
  int audio_channels = kINIT_CHANNELS;

  /* Return if the audio configuration is complete */
  return SDL_WasInit(SDL_INIT_AUDIO) > 0 && 
         Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels) > 0;
}
