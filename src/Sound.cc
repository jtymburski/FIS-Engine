/*******************************************************************************
 * Class Name: Sound
 * Date Created: February 9, 2012
 * Inheritance: none
 * Description: Handles all sound used within the game. This individual class
 *              only handles playing a single sound at one time. If you click
 *              play while it's still playing, it will stop the previous
 *              execution. However, if you create multiple sound files, SDL 
 *              allows up to 8 files being mixed together at once before
 *              returning the channel full error.
 ******************************************************************************/
#include "Sound.h"

/* Private Constant Implementation - see header file for descriptions */
const short Sound::kINFINITE_LOOP = -1;
const short Sound::kUNSET_CHANNEL = -1;

/* Public Constant Implementation - see header file for descriptions */
const int Sound::kINIT_BUFFERS = 1024;
const int Sound::kINIT_CHANNELS = 2;
const uint16_t Sound::kINIT_FORMAT = AUDIO_S16SYS;
const int Sound::kINIT_RATE = 22050;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Sets up a shell for a sound. It will
 *              play nothing though, since no sound file is setup.
 *
 * Inputs: none
 */
Sound::Sound()
{
  channel = kUNSET_CHANNEL;
  path.clear();
  sound = NULL;
  sound_set = false;

  setPlayCount(0);
}

/*
 * Description: Constructor function - Sets up a sound with the path given
 *              and with a loop count to play it for.
 *
 * Inputs: std::string path - the path to the audio file (in WAV)
 *         int loop_count: the number of time you want the audio to loop.
 *                         Default is 0. If a 0 is used, it will automatically
 *                         default to 1 loop.
 */
Sound::Sound(std::string path, int loop_count)
{
  channel = kUNSET_CHANNEL;
  path.clear();
  sound = NULL;
  sound_set = false;

  setSoundFile(path);
  setPlayCount(loop_count);
}

/* 
 * Description: Destructor function
 */
Sound::~Sound()
{
  unsetSoundFile();
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

  /* Attempt to initialize the sound file if it isn't initialized */
  if(!sound_set && !path.empty())
    setSoundFile(path);

  /* Only proceed if the sound SDL layer is ready to run */
  if(sound_set)
  {
    /* Try to play on a new channel */
    channel = Mix_PlayChannel(-1, sound, loop_count);
    if(channel == kUNSET_CHANNEL)
      std::cout << "[WARNING] Unable to play WAV file: " << Mix_GetError()
                << std::endl;
  }
}

/* 
 * Description: Sets the sound to be played the next time the play() 
 *              function is called. This tries to open it and if it
 *              fails, it will not be set and the class will be notified
 *              as well as the terminal. If the sound file was set and this
 *              set was unsuccessful, there will be no sound file set anymore.
 *
 * Inputs: std::string path - the path to the sound to add
 * Output: bool - status if the setting of the sound file was successful
 */
bool Sound::setSoundFile(std::string path)
{
  /* Set the path file first */
  if(!path.empty())
    this->path = path;

  /* Next, attempt and load the file */
  if(!path.empty())
  {
    unsetSoundFile(false);
    sound = Mix_LoadWAV(path.c_str());

    /* Determine if the setting of the sound was valid */
    if(sound == NULL)
    {
      std::cout << "[WARNING] Unable to load WAV file: "
                << Mix_GetError() << std::endl;
      return false;
    }

    sound_set = true;
    return true;
  }

  std::cout << "[WARNING] Unable to load empty WAV file path." << std::endl;
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
 * Description: Loop the stored sound file infinitely, once play is called.
 *              This will only cease once the stop() function is called.
 *
 * Inputs: none
 * Output: none
 */
void Sound::setPlayForever()
{
  loop_count = kINFINITE_LOOP;
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

/* 
 * Description: Unsets the sound file in this class. If it's set, the memory 
 *              is freed, the pointer is nulled and the class is notified of
 *              the change. Otherwise, it just cleans up the pointer logic
 *              to ensure that the sound hasn't been set. Will be called on
 *              cleanup and destruction as well.
 *
 * Inputs: bool clear_path - clear the path stored in the class as well
 * Output: bool - returns if the sound file was set before calling this 
 *                function.
 */
bool Sound::unsetSoundFile(bool clear_path)
{
  if(sound_set)
  {
    Mix_FreeChunk(sound);
    sound = NULL;
    sound_set = false;
    return true;
  }

  if(clear_path)
    path.clear();
  sound = NULL;
  return false;
}
