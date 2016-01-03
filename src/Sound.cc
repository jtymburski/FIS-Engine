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
const int Sound::kSTOP_FADE = 1500;
/* Public Constant Implementation */
const int Sound::kDEFAULT_FREQUENCY = 22050;

/* -- */
const uint32_t Sound::kID_MUSIC_TITLE = 0;
const uint32_t Sound::kID_MUSIC_LOADING = 1;
const uint32_t Sound::kID_MUSIC_BATTLE = 2;
/* -- */


/* Reserved IDS
 *    System Sounds  0 - 49
 *    Map Sounds    50 - 99
 *   Battle Sounds 100 - 199
 *   Custom Sounds 1000+ */

/* System Sounds */
const uint32_t Sound::kID_SOUND_MENU_CHG = 0;
const uint32_t Sound::kID_SOUND_MENU_NEXT = 1;
const uint32_t Sound::kID_SOUND_MENU_PREV = 2;

/* Map Sounds*/
const uint32_t Sound::kID_SOUND_PICK_COIN = 12;
const uint32_t Sound::kID_SOUND_PICK_ITEM = 13;

/* Battle Sounds */
const uint32_t Sound::kID_SOUND_BTL_CONFUSE = 111;
const uint32_t Sound::kID_SOUND_BTL_DEATH = 112;
const uint32_t Sound::kID_SOUND_BTL_FIRE = 113;
const uint32_t Sound::kID_SOUND_BTL_HIBERNATE = 114;
const uint32_t Sound::kID_SOUND_BTL_LOWER = 115;
const uint32_t Sound::kID_SOUND_BTL_PARALYSIS = 116;
const uint32_t Sound::kID_SOUND_BTL_PLEP = 117;
const uint32_t Sound::kID_SOUND_BTL_RAISE = 118;
const uint32_t Sound::kID_SOUND_BTL_SILENCE = 119;

/* -- */
const int Sound::kUNSET_ID = -1;

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
  id = kUNSET_ID;
  length = 0;
  loop_count = 0;
  raw_data = NULL;
  volume = MIX_MAX_VOLUME - MIX_MAX_VOLUME / 4;
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

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void Sound::copySelf(const Sound &source)
{
  /* Core data */
  channel = source.channel;
  fade_time = source.fade_time;
  id = source.id;
  loop_count = source.loop_count;
  volume = source.volume;

  /* Sound data - unable to be copied */
  raw_data = nullptr;
  length = 0;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: This function handles a rudimentary cross-fading between two
 *              channels. This takes a channel ID and tries to play this sound
 *              first and if successful, begins to stop the given channel ID.
 *              This uses the fade time from the new class fading in to fade out
 *              the other class. Only executes if the channel is different than
 *              the set channel in this class.
 *
 * Inputs: int channel - the channel ID, used for sound mixing
 * Output: none
 */
void Sound::crossFade(int channel)
{
  if(channel != getChannelInt() && channel >= 0 && getChannelInt() >= 0)
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
  if(channel != SoundChannels::UNASSIGNED)
    crossFade(getChannelInt(channel));
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
  return getChannelInt(channel);
}

/* Returns the fade status */
Mix_Fading Sound::getFadeStatus()
{
  if(isPlaying())
    return Mix_FadingChannel(getChannelInt());
  return MIX_NO_FADING;
}

/*
 * Description: Returns the time to fade in or out this sound class when play
 *              or stop is called. The time is stored in milliseconds.
 *
 * Inputs: none
 * Output: uint32_t - the fade time, in milliseconds
 */
uint32_t Sound::getFadeTime() const
{
  return fade_time;
}

/*
 * Description: Returns the ID of the sound chunk.
 *
 * Inputs: none
 * Output: int - the ID. If less than 0, unset
 */
int Sound::getID()
{
  return id;
}

/*
 * Description: Returns the length in milliseconds of the chunk within the sound
 *              file. If unset, will be 0.
 *
 * Inputs: none
 * Output: int - the length in milliseconds
 */
int Sound::getLength()
{
  return length;
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
uint8_t Sound::getVolume() const
{
  return volume;
}

/*
 * Description: Returns if the chunk is playing on its assigned channel.
 *
 * Inputs: none
 * Output: bool - true if the sound chunk is playing
 */
bool Sound::isPlaying()
{
  if(channel != SoundChannels::UNASSIGNED)
  {
    int channel_id = getChannelInt();

    if(Mix_Playing(channel_id) > 0 &&
       Mix_GetChunk(channel_id) == raw_data)
    {
      return true;
    }
  }
  return false;
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
 *                             is playing. Default false. If false, only stops
 *                             this channel if this chunk is playing on it.
 *         bool skip_fade - true to skip stop and start fade on play trigger
 * Output: bool - if the play call was successful
 */
bool Sound::play(bool stop_channel, bool skip_fade)
{
  bool success = false;

  /* Only proceed if the sound chunk is set and if the stop was successful */
  if(raw_data != NULL && channel != SoundChannels::UNASSIGNED
                      && stop(skip_fade))
  {
    int channel_int = getChannelInt();

    /* If stop channel, clean out all other playing */
    if(stop_channel)
      stopChannel(channel);

    /* Try to play on the channel */
    int play_channel = -1;
    if(fade_time > 0 && !skip_fade)
      play_channel =
            Mix_FadeInChannel(channel_int, raw_data, loop_count, fade_time);
    else
      play_channel = Mix_PlayChannel(channel_int, raw_data, loop_count);

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
  if(stop())
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
 * Description: Sets the ID of the sound chunk. If less than 0, unsets the ID.
 *
 * Inputs: int id - the new ID for the chunk
 * Output: none
 */
void Sound::setID(int id)
{
  if(id < 0)
    this->id = kUNSET_ID;
  else
    this->id = id;
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
      std::cerr << "[WARNING] Unable to load WAV file: " << path << std::endl;
                //<< Mix_GetError() << std::endl;
      return false;
    }

    /* Unset the old and set the new data */
    unsetSoundFile();
    raw_data = sound;
    Mix_VolumeChunk(raw_data, volume);

    /* Calculate length: raw / ((hz * 2 bytes * 1 (1mono, 2stereo)) / 1000) */
    length = raw_data->alen / ((Sound::kDEFAULT_FREQUENCY * 2 * 1) / 1000);

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
  else
    this->volume = volume;

  /* Change the chunk volume related to this file */
  if(raw_data != NULL)
    Mix_VolumeChunk(raw_data, volume);
}

/*
 * Description: Stop function. This handles stopping the sound file that was
 *              configured in this class. It will fade out the sound file if
 *              the fade time is greater than 0. If the input is true, it will
 *              halt the channel instantly instead of waiting for the set fade
 *              out time. This will only stop the channel if the chunk is
 *              playing.
 *
 * Inputs: bool skip_fade - true if the fade time should be negated. Default
 *                          to false.
 * Output: bool - status if the current chunk is stopped or stopping
 */
bool Sound::stop(bool skip_fade)
{
  if(isPlaying())
  {
    int channel_id = getChannelInt();

    /* Stop the chunk, if relevant */
    if(fade_time > 0 && !skip_fade && getFadeStatus() != MIX_FADING_OUT)
      Mix_FadeOutChannel(channel_id, fade_time);
    if(skip_fade || fade_time <= 0)
      Mix_HaltChannel(channel_id);

    /* Check the status */
    if(Mix_Playing(channel_id) == 0 ||
       Mix_FadingChannel(channel_id) == MIX_FADING_OUT)
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
  stop(true);

  /* Free the sound chunk */
  if(raw_data != NULL)
    Mix_FreeChunk(raw_data);
  raw_data = NULL;
  length = 0;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another Sound.
 *
 * Inputs: const Sound &source - the source class constructor
 * Output: Sound& - pointer to the copied class
 */
Sound& Sound::operator= (const Sound &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the corresponding integer for the channel enumerator. If
 *              less than 0, invalid.
 *
 * Inputs: SoundChannels channel - the channel enumerator
 * Output: int - the integer representation of the enum
 */
int Sound::getChannelInt(SoundChannels channel)
{
  return static_cast<int>(channel);
}

/*
 * Description: Returns if the given channel integer is playing.
 *
 * Inputs: int channel - the channel number to check
 * Output: bool - true if the channel is playing
 */
bool Sound::isChannelPlaying(int channel)
{
  return (Mix_Playing(channel) > 0);
}

/*
 * Description: Returns if the given channel enum is playing.
 *
 * Inputs: SoundChannels channel - the channel enum to check
 * Output: bool - true if the channel is playing
 */
bool Sound::isChannelPlaying(SoundChannels channel)
{
  if(channel != SoundChannels::UNASSIGNED)
    return isChannelPlaying(getChannelInt(channel));
  return false;
}

/*
 * Description: Pause all channels of sound.
 *
 * Inputs: none
 * Output: none
 */
void Sound::pauseAllChannels()
{
  Mix_Pause(-1);
}

/*
 * Description: Pause a single channel of sound, based on the enumerator.
 *
 * Inputs: SoundChannels channel - the enumerated channel to pause
 * Output: none
 */
void Sound::pauseChannel(SoundChannels channel)
{
  if(channel != SoundChannels::UNASSIGNED)
    Mix_Pause(getChannelInt(channel));
}

/*
 * Description: Resumes all channels. This only does something if they were
 *              paused previously.
 *
 * Inputs: none
 * Output: none
 */
void Sound::resumeAllChannels()
{
  Mix_Resume(-1);
}

/*
 * Description: Resumes a single channel of sound, based on the enumerator.
 *              This only does something if they were paused previously.
 *
 * Inputs: SoundChannels channel - the enumerated channel to pause
 * Output: none
 */
void Sound::resumeChannel(SoundChannels channel)
{
  if(channel != SoundChannels::UNASSIGNED)
    Mix_Resume(getChannelInt(channel));
}

/*
 * Description: Sets the audio volume channels to the designated volume. If less
 *              then 0, set to 0. If greater than max, set to max (128). The
 *              audio channels include all channels that aren't music (e.g.
 *              weather, menus, tiles, etc)
 *
 * Inputs: int new_volume - the new volume level to set all audio channels.
 * Output: none
 */
int Sound::setAudioVolumes(int new_volume)
{
  /* Error checking on input */
  if(new_volume < 0)
    new_volume = 0;
  if(new_volume > MIX_MAX_VOLUME)
    new_volume = MIX_MAX_VOLUME;

  /* Set volumes */
  Mix_Volume((int)SoundChannels::WEATHER1, new_volume);
  Mix_Volume((int)SoundChannels::WEATHER2, new_volume);
  Mix_Volume((int)SoundChannels::MENUS, new_volume);
  Mix_Volume((int)SoundChannels::TILES, new_volume);
  Mix_Volume((int)SoundChannels::THINGS, new_volume);
  Mix_Volume((int)SoundChannels::SECTORS, new_volume);
  Mix_Volume((int)SoundChannels::TRIGGERS, new_volume);

  /* Return volume */
  return new_volume;
}

/*
 * Description: Sets the volume of all channels to the designated volume. If
 *              less then 0, set to 0. If greater than max, set to max (128).
 *
 * Inputs: int new_volume - the new volume level to set all audio channels.
 * Output: none
 */
int Sound::setMasterVolume(int new_volume)
{
    /* Error checking on input */
  if(new_volume < 0)
    new_volume = 0;
  if(new_volume > MIX_MAX_VOLUME)
    new_volume = MIX_MAX_VOLUME;

  /* Set volumes */
  Mix_Volume(-1, new_volume);

  /* Return volume */
  return new_volume;
}

/*
 * Description: Sets the music volume channels to the designated volume. If less
 *              then 0, set to 0. If greater than max, set to max (128). The
 *              music channels only include music related channels (der)
 *
 * Inputs: int new_volume - the new volume level to set all music channels.
 * Output: none
 */
int Sound::setMusicVolumes(int new_volume)
{
  /* Error checking on input */
  if(new_volume < 0)
    new_volume = 0;
  if(new_volume > MIX_MAX_VOLUME)
    new_volume = MIX_MAX_VOLUME;

  /* Set volumes */
  Mix_Volume((int)SoundChannels::MUSIC1, new_volume);
  Mix_Volume((int)SoundChannels::MUSIC2, new_volume);

  /* Return volume */
  return new_volume;
}

/*
 * Description: Stops all channels with no fade out time.
 *
 * Inputs: int fade - the ms to fade the channel. Default value 1500
 * Output: none
 */
void Sound::stopAllChannels(int fade)
{
  if(fade > 0)
    Mix_FadeOutChannel(-1, fade);
  else
    Mix_HaltChannel(-1);
}

/*
 * Description: Stops the given channel with no fade time.
 *
 * Inputs: SoundChannels channel - the channel to halt
 *         int fade - the ms to fade the channel. Default value 1500
 * Output: none
 */
void Sound::stopChannel(SoundChannels channel, int fade)
{
  if(channel != SoundChannels::UNASSIGNED)
  {
    if(fade > 0)
      Mix_FadeOutChannel(getChannelInt(channel), fade);
    else
      Mix_HaltChannel(getChannelInt(channel));
  }
}
