/*******************************************************************************
 * Class Name: Music
 * Date Created: November 23, 2013
 * Inheritance: none
 * Description: Handles the backend music used within the game. This class
 *              contains the music that can be played at any time. Only one set
 *              of music can be played at a single time. Very similar to the
 *              control like the Sound class.
 ******************************************************************************/
#ifndef MUSIC_H
#define MUSIC_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class Music
{
public:
  /* Constructor: Sets up a blank template, nothing will play */
  Music();

  /* Constructor: Sets up all the pertinent data to create a sound */
  Music(std::string path, int loop_count = 0);

  /* Destructor function */
  ~Music();

private:
  /* The number of times to play the sound. It will be loop_count+1
   * except for -1, which is infinite loop */
  int loop_count;

  /* If the music has been set */
  bool music_set;

  /* The raw data of the music in RAM */
  Mix_Music* raw_data;

  /* --------------------- CONSTANTS --------------------- */
private:
  const static short kFADE_TIME; /* The time for the music to fade in or out */
  const static short kINFINITE_LOOP; /* Infinite loop special character */

  /*======================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Returns the loop count, the number of times it will play for */
  int getPlayCount();

  /* Play slot. If music is set, it will play for the given number of loops */
  void play(bool fade_in = true);

  /* Trys to set the music file to the given path */
  bool setMusicFile(std::string path);

  /* Set the number of times to play the song for. If 0, defaults to 1 */
  bool setPlayCount(int play_count);

  /* Set the music to loop infinitely, until stop() is called */
  void setPlayForever();

  /* Unset the music file and frees the memory */
  void unsetMusicFile();

  /*===================== PUBLIC STATIC  FUNCTIONS ===========================*/
public:
  /* Stops the music from playing */
  static void stop(bool fade_out = true);
};

#endif // MUSIC_H
