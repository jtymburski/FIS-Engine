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

class Sound
{
public:
  /* Constructor: Sets up a blank template, nothing will play */
  Sound();

  /* Constructor: Sets up all the pertinent data to create a sound */
  Sound(std::string path, int loop_count = 0);

  /* Destructor function */
  ~Sound();

private:
  /* The channel number the sound will be played on */
  int channel;

  /* The number of times to play the sound. It will be loop_count+1 
   * except for -1, which is infinite loop */
  int loop_count;

  /* The path for the played sound file */
  std::string path;

  /* The raw data of the sound in RAM */
  Mix_Chunk* sound;

  /* If the sound has been set */
  bool sound_set;

  /* --------------------- CONSTANTS --------------------- */
private:
  const static short kINFINITE_LOOP; /* Infinite loop special character */
  const static short kUNSET_CHANNEL; /* Unset channel definition */
public:
  const static int kINIT_BUFFERS; /* Size of audio buffers in memory */
  const static int kINIT_CHANNELS; /* 1 channel = mono, 2 = stereo */
  const static uint16_t kINIT_FORMAT; /* Format of the audio being played */
  const static int kINIT_RATE; /* Frequency of Audio Playback */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the loop count, the number of times it will play for */
  int getPlayCount();

  /* Play slot. If sound is set, it will play the sound for the given number
   * of loops */
  void play();
  
  /* Trys to set the sound file to the given path */
  bool setSoundFile(std::string path);
  
  /* Set the number of times to play the song for. If 0, defaults to 1 */
  bool setPlayCount(int play_count);

  /* Set the sound to loop infinitely, until stop() is called */
  void setPlayForever();

  /* Stop slot. Will stop the sound, if playing */
  void stop();
  
  /* Unset the sound file and frees the memory */
  bool unsetSoundFile(bool clear_path = true);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Plays a sound, only once. More than that? create a sound handle */
  //bool playSound(); // TODO
};

#endif // SOUND_H
