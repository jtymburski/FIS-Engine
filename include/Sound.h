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
#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

/* Enumerator class for sound */
enum class SoundChannels : int
{
  UNASSIGNED = -1,
  MUSIC1 = 0,
  MUSIC2 = 1,
  WEATHER1 = 2,
  WEATHER2 = 3,
  MENUS = 4,
  TILES = 5,
  THINGS = 6,
  TRIGGERS = 7,
  SECTORS = 8
};

/*
 * Structure which handles a sound queue entry
 */
struct SoundQueue
{
  uint32_t id;
  SoundChannels channel;

  bool stop;
};

/* Sound class */
class Sound
{
public:
  /* Constructor: Sets up a blank template, nothing will play */
  Sound();

  /* Constructor: Sets up all the pertinent data to create a sound */
  Sound(SoundChannels channel, std::string path, int loop_count = 0);

  /* Destructor function */
  ~Sound();

private:
  /* The channel number the sound will be played on */
  SoundChannels channel;

  /* The time to fade the sound in. If 0, no fade */
  uint32_t fade_time;

  /* Sound ID */
  int id;

  /* Length, in milliseconds - calculated on creation */
  int length;

  /* The number of times to play the sound. It will be loop_count+1
   * except for -1, which is infinite loop */
  int loop_count;

  /* The raw data of the sound in RAM */
  Mix_Chunk* raw_data;

  /* The volume that the chunk will play at */
  //uint8_t default_volume; // TODO: Remove? See cc.
  uint8_t volume;

  /* --------------------- CONSTANTS --------------------- */
private:
  const static float kDEFAULT_RATIO; /* Default percentage of max */
  const static short kINFINITE_LOOP; /* Infinite loop special character */
  const static int kSTOP_FADE;       /* Stop fade time on channel */

public:
  const static int kDEFAULT_FREQUENCY; /* Sound frequency - DO NOT CHANGE */

  /* Reserved Music IDs */
  const static uint32_t kID_MUSIC_BATTLE;
  const static uint32_t kID_MUSIC_LOADING;
  const static uint32_t kID_MUSIC_TITLE;
  const static uint32_t kID_MUSIC_VICTORY;
  const static uint32_t kID_MUSIC_DEFEAT;

  /* Reserved Sound Menu IDs */
  const static uint32_t kID_SOUND_MENU_CHG;
  const static uint32_t kID_SOUND_MENU_NEXT;
  const static uint32_t kID_SOUND_MENU_PREV;

  /* Reserved Sound Map IDs */
  const static uint32_t kID_SOUND_PICK_COIN;
  const static uint32_t kID_SOUND_PICK_ITEM;
  const static uint32_t kID_SOUND_SPOTTED;

  /* Reserved Sound Battle IDs */
  const static uint32_t kID_SOUND_BTL_CONFUSE;
  const static uint32_t kID_SOUND_BTL_DEATH;
  const static uint32_t kID_SOUND_BTL_FIRE;
  const static uint32_t kID_SOUND_BTL_HIBERNATE;
  const static uint32_t kID_SOUND_BTL_LOWER;
  const static uint32_t kID_SOUND_BTL_PARALYSIS;
  const static uint32_t kID_SOUND_BTL_PLEP;
  const static uint32_t kID_SOUND_BTL_RAISE;
  const static uint32_t kID_SOUND_BTL_SILENCE;
  const static uint32_t kID_SOUND_BTL_POISON;

  const static int kUNSET_ID; /* The placeholder unset ID */

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const Sound& source);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Cross fade the given channel out with the current class in */
  void crossFade(int channel);
  void crossFade(SoundChannels channel);

  /* Returns the channel that the music is playing on. Useful for calling mix
   * calls to get feedback on the sound */
  SoundChannels getChannel();
  int getChannelInt();

  /* Returns the fade status */
  Mix_Fading getFadeStatus();

  /* Returns the time that the chunk will be faded in or out */
  uint32_t getFadeTime() const;

  /* Returns the ID */
  int getID();

  /* Returns the length, in milliseconds, of the chunk */
  int getLength();

  /* Returns the loop count, the number of times it will play for */
  int getLoopCount();

  /* Returns the raw chunk data, of the loaded sound. NULL if inactive */
  Mix_Chunk* getRawData();

  /* Returns the volume of the sound chunk */
  uint8_t getVolume() const;

  /* Returns if the chunk is playing */
  bool isPlaying();

  /* Play function. If sound is set, it will play the sound for the given number
   * of loops */
  bool play(bool stop_channel = false, bool skip_fade = false);

  /* Sets the channel integer to be used for playing the chunk */
  void setChannel(SoundChannels channel);

  /* Sets the fade time for the sound chunk sequence (in msec) */
  void setFadeTime(uint32_t time);

  /* Sets the ID */
  void setID(int id);

  /* Set the number of times to play the song for. If 0, defaults to 1 */
  void setLoopCount(int loop_count);

  /* Set the sound to loop infinitely, until stop() is called */
  void setLoopForever();

  /* Trys to set the sound file to the given path */
  bool setSoundFile(std::string path);

  /* Sets the defauilt volume that the chunk will be played at. */
  // TODO: Remove? See cc.
  //void setDefaultVolume(uint8_t default_volume);

  /* Sets the volume that the chunk will be played at */
  void setVolume(uint8_t volume);

  /* Stop function. Will stop the sound, if playing */
  bool stop(bool skip_fade = false);

  /* Unset the sound file and frees the memory */
  void unsetSoundFile();

  /*===================== PUBLIC STATIC FUNCTIONS ===========================*/
public:
  /* Returns channel int corresponding to the channel enum */
  static int getChannelInt(SoundChannels channel);

  /* Status if given channel input is playing */
  static bool isChannelPlaying(int channel);
  static bool isChannelPlaying(SoundChannels channel);

  /* Pause all channels or select channels */
  static void pauseAllChannels();
  static void pauseChannel(SoundChannels channel);

  /* Resume all channels or select channels */
  static void resumeAllChannels();
  static void resumeChannel(SoundChannels channel);

  /* Set volumes */
  static int setAudioVolumes(int new_volume);
  static int setMasterVolume(int new_volume);
  static int setMusicVolumes(int new_volume);

  /* Stop all channels or select channels */
  static void stopAllChannels(int fade = kSTOP_FADE);
  static void stopChannel(SoundChannels channel, int fade = kSTOP_FADE);

  /*======================== OPERATOR FUNCTIONS ==============================*/
public:
  Sound& operator=(const Sound& source);
};

#endif // SOUND_H
