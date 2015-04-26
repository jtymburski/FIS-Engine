/*******************************************************************************
 * Class Name: Sound
 * Date Created: February 9, 2012
 * Inheritance: none
 * Description: Handles all sound used within the game. This individual class
 *              only handles playing a single sound at one time. If you click
 *              play while it's still playing, it will stop the previous
 *              execution. However, if you create multiple sound files, SDL 
 *              allows up to x files being mixed together at once before
 *              returning the channel full error.
 *
 * TODO:
 ******************************************************************************/
#include "Sound.h"

/* Constant Implementation - see header file for descriptions */
const short Sound::kINFINITE_LOOP = -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructor function - Sets up a shell for a sound. It will
 *              play nothing though, since no sound file is setup.
 *
 * Inputs: none
 */
Sound::Sound()
{
  channel = SoundChannels::UNASSIGNED;
  fade_time = 0;
  loop_count = 0;
  raw_data = NULL;
  volume = MIX_MAX_VOLUME;
}

/*
 * Description: Constructor function - Sets up a sound with the path given
 *              and with a loop count to play it for and the channel to play on.
 *
 * Inputs: SoundChannels channel - the channel reference to play on
 *         std::string path - the path to the audio file
 *         int loop_count - the number of time you want the audio to loop.
 *                          Default is 1 time repeat.
 */
Sound::Sound(SoundChannels channel, std::string path, int loop_count) : Sound()
{
  setChannel(channel);
  setSoundFile(path);
  setLoopCount(loop_count);
}

/* 
 * Description: Destructor function
 */
Sound::~Sound()
{
  unsetSoundFile();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: This function handles a rudimentary cross-fading between two
 *              channels. This takes a channel ID and tries to play this sound
 *              first and if successful, begins to stop the given channel ID. 
 *              This uses the fade time from the new class fading in to fade out
 *              the other class.
 *
 * Inputs: int channel - the channel ID, used for sound mixing
 * Output: none
 */
void Sound::crossFade(int channel)
{
  /* Attempt to play the current audio file */
  bool success = play();
  
  /* If successful, begin to transition the passed in function out */
  if(success)
  {
    if(Mix_FadingChannel(channel) == MIX_FADING_IN)
      Mix_HaltChannel(channel);
    else if(Mix_FadingChannel(channel) == MIX_NO_FADING)
    {
      if(fade_time > 0)
        Mix_FadeOutChannel(channel, fade_time);
      else
        Mix_HaltChannel(channel);
    }
  }
}

/*
 * Description: This function handles a rudimentary cross-fading between two
 *              channels. This takes a channel and tries to play this sound
 *              first and if successful, begins to stop the given channel. 
 *              This uses the fade time from the new class fading in to fade out
 *              the other class.
 *
 * Inputs: SoundChannels channel - the channel, used for sound mixing
 * Output: none
 */ 
void Sound::crossFade(SoundChannels channel)
{
  crossFade(static_cast<int>(channel));
}

/*
 * Description: Returns the channel that the sound file is currently playing on.
 *              This will be the UNASSIGNED mode if it's unset.
 *
 * Inputs: none
 * Output: SoundChannels - the channel enum indicator
 */
SoundChannels Sound::getChannel()
{
  return channel;
}

/*
 * Description: Returns the channel that the sound file is currently playing on.
 *              This will be the unset channel value (-1) if it's unset.
 *
 * Inputs: none
 * Output: int - the channel integer
 */
int Sound::getChannelInt()
{
  return static_cast<int>(channel);
}

/*
 * Description: Returns the time to fade in or out this sound class when play
 *              or stop is called. The time is stored in milliseconds.
 *
 * Inputs: none
 * Output: uint32_t - the fade time, in milliseconds
 */
uint32_t Sound::getFadeTime()
{
  return fade_time;
}

/* 
 * Description: Gets the number of times that the audio will loop for when
 *              the play() function is called. If it returns -1, this
 *              represents it will loop infinitely until stopped using stop().
 *
 * Inputs: none
 * Output: int - returns the number of times the audio will play for
 */
int Sound::getLoopCount()
{
  if(loop_count == kINFINITE_LOOP)
    return loop_count;
  return loop_count + 1;
}

/*
 * Description: Returns the raw chunk data of the loaded audio file. This
 *              returns NULL if no audio file is set.
 *
 * Inputs: none
 * Output: Mix_Chunk* - the chunk struct for the audio file
 */
Mix_Chunk* Sound::getRawData()
{
  return raw_data;
}

/*
 * Description: Returns the volume that the chunk is currently set at.
 *              This is a number from 0 -> 128, as defined by SDL.
 *
 * Inputs: none
 * Output: uint8_t - the volume integer.
 */
uint8_t Sound::getVolume()
{
  return volume;
}

/* 
 * Description: Play function. This handles playing the sound file that was
 *              configured in this class. It will use the number of loops
 *              setup as well as the file. If the file was playing, it
 *              will stop it before. Therefore, it will act as a reset as
 *              well. If errors occur, they are pushed through stderr. If fade
 *              time is greater than 0, it will execute a fade on starting and
 *              stopping.
 *
 * Inputs: bool stop_channel - should the channel be stopped regardless of what
 *                             is playing.
 * Output: bool - if the play call was successful
 */
bool Sound::play(bool stop_channel)
{
  bool success = false;
  
  /* Only proceed if the sound chunk is set and if the stop was successful */
  if(raw_data != NULL && channel != SoundChannels::UNASSIGNED 
                      && stop(stop_channel))
  {
    /* Try to play on a new channel */
    int play_channel = -1;
    if(fade_time > 0)
      play_channel = 
            Mix_FadeInChannel(getChannelInt(), raw_data, loop_count, fade_time);
    else
      play_channel = Mix_PlayChannel(getChannelInt(), raw_data, loop_count);
      
    /* Check the status of the played channel */
    if(play_channel < 0)
      std::cerr << "[WARNING] Unable to play sound file: " << Mix_GetError()
                << std::endl;
    else
      success = true;
  }
  
  return success;
}

/*
 * Description: Sets the channel that the audio file should be played on. If 
 *              the sound is currently playing, stop it first.
 *
 * Inputs: SoundChannels channel - the channel enumerator for the allocated 
 *                                 channels.
 * Output: none
 */
void Sound::setChannel(SoundChannels channel)
{
  if(stop(false))
    this->channel = channel;
}

/*
 * Description: Sets the fade time for starting and stopping the sound. This is
 *              a value in milliseconds. If set to 0, there will be no fade
 *              effect.
 *
 * Inputs: uint32_t time - the time in milliseconds
 * Output: none
 */
void Sound::setFadeTime(uint32_t time)
{
  fade_time = time;
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
void Sound::setLoopCount(int loop_count)
{
  if(loop_count < 0)
    this->loop_count = kINFINITE_LOOP;
  else if(loop_count > 0)
    this->loop_count = loop_count - 1;
  else
    loop_count = 0;
}

/*
 * Description: Loop the stored sound file infinitely, once play is called.
 *              This will only cease once the stop() function is called.
 *
 * Inputs: none
 * Output: none
 */
void Sound::setLoopForever()
{
  loop_count = kINFINITE_LOOP;
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
  /* Attempt and load the file */
  if(!path.empty())
  {
    /* First, try wav loader */
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());

    /* Determine if the setting of the sound was valid */
    if(sound == NULL)
    {
      std::cerr << "[WARNING] Unable to load WAV file: "
                << Mix_GetError() << std::endl;
      return false;
    }
    
    /* Unset the old and set the new data */
    unsetSoundFile();
    raw_data = sound;
    Mix_VolumeChunk(raw_data, volume);
    return true;
  }

  std::cerr << "[WARNING] Unable to load empty WAV file path." << std::endl;
  return false;
}

/*
 * Description: Sets the volume to play the mixed sound file at. This is a
 *              constant integer between 0 and 128. This can be set while the
 *              file is being played without stopping it.
 *
 * Inputs: uint8_t volume - the volume integer to set it to
 * Output: none
 */
void Sound::setVolume(uint8_t volume)
{
  if(volume > MIX_MAX_VOLUME)
    this->volume = MIX_MAX_VOLUME;
  this->volume = volume;
  
  /* Change the chunk volume related to this file */
  if(raw_data != NULL)
    Mix_VolumeChunk(raw_data, volume);
}

/* 
 * Description: Stop function. This handles stopping the sound file that was
 *              configured in this class. It will fade out the sound file if
 *              the fade time is greater than 0. If the input is true, it will
 *              halt the channel regardless of if this class has the chunk
 *              that is playing on it.
 *
 * Inputs: bool stop_channel - if the channel should be stopped regardless if 
 *                             this class is the chunk that's playing
 * Output: bool - status if the stop occurred
 */
bool Sound::stop(bool stop_channel)
{
  if(channel != SoundChannels::UNASSIGNED)
  {
    int channel_id = getChannelInt();
    
    /* Stop the playback, if relevant */
    if(stop_channel || Mix_GetChunk(channel_id) == raw_data)
    {
      if(fade_time > 0)
        Mix_FadeOutChannel(channel_id, fade_time);
      else
        Mix_HaltChannel(channel_id);
    }
    
    /* Check the status of the playing */
    if(!Mix_Playing(channel_id) || 
       Mix_FadingChannel(channel_id) == MIX_FADING_OUT || 
       Mix_FadingChannel(channel_id) == MIX_FADING_IN)
      return true;
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
 * Inputs: bool clear_path - clear the path stored in the class as well
 * Output: none
 */
void Sound::unsetSoundFile()
{
  /* Just stop this class, if it's the playing one */
  stop(false);
  
  /* Free the sound chunk */
  if(raw_data != NULL)
    Mix_FreeChunk(raw_data);
  raw_data = NULL;
}
