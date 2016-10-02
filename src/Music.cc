/*******************************************************************************
 * Class Name: Music
 * Date Created: November 23, 2013
 * Inheritance: none
 * Description: Handles the backend music used within the game. This class
 *              contains the music that can be played at any time. Only one set
 *              of music can be played at a single time. Very similar to the
 *              control like the Sound class.
 *
 * TODO:
 *  - Add music adjustments for modifying the volume this individual music
 *    plays at.
 ******************************************************************************/
#include "Music.h"

/* Constant Implementation - see header file for descriptions */
const short Music::kFADE_TIME = 5000;
const short Music::kINFINITE_LOOP = -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* Constructor: Sets up a blank template, nothing will play */
Music::Music()
{
  music_set = false;
  raw_data = NULL;

  setPlayCount(0);
}

/* Constructor: Sets up all the pertinent data to create a sound */
Music::Music(std::string path, int loop_count) : Music()
{
  setMusicFile(path);
  setPlayCount(loop_count);
}

/* Destructor function */
Music::~Music()
{
  unsetMusicFile();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* 
 * Description: Gets the number of times that the audio will loop for when
 *              the play() function is called. If it returns -1, this
 *              represents it will loop infinitely until stopped using stop().
 *
 * Inputs: none
 * Output: int - returns the number of times the audio will play for
 */
int Music::getPlayCount()
{
  if(loop_count == kINFINITE_LOOP)
    return loop_count;
  return loop_count + 1;
}

/* 
 * Description: Play function. This handles playing the sound file that was
 *              configured in this class. It will use the number of loops
 *              setup as well as the file. If the file was playing, it
 *              will stop it before. Therefore, it will act as a reset as
 *              well. If errors occur, they are pushed through qDebug()
 *              to the terminal.
 *
 * Inputs: none
 * Output: none
 */
void Music::play(bool fade_in)
{
  /* Only proceed if the sound SDL layer is ready to run */
  if(music_set)
  {
    /* Ensure the channel is stopped if it was in use */
    stop(fade_in);
  
    /* Try to play the music file */
    if((!fade_in && Mix_PlayMusic(raw_data, loop_count) == -1) ||
       (fade_in && Mix_FadeInMusic(raw_data, loop_count, kFADE_TIME) == -1))
    {
      std::cerr << "[WARNING] Unable to play music file: " << Mix_GetError()
                << std::endl;
    }
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
bool Music::setMusicFile(std::string path)
{
  /* Attempt and load the file */
  if(!path.empty())
  {
    unsetMusicFile();
    raw_data = Mix_LoadMUS(path.c_str());

    /* Determine if the setting of the sound was valid */
    if(raw_data == NULL)
    {
      std::cerr << "[WARNING] Unable to load music file: "
                << Mix_GetError() << std::endl;
      return false;
    }

    music_set = true;
    return true;
  }

  std::cerr << "[WARNING] Unable to load empty music file path." << std::endl;
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
bool Music::setPlayCount(int play_count)
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
void Music::setPlayForever()
{
  loop_count = kINFINITE_LOOP;
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
void Music::unsetMusicFile()
{
  /* Free the music chunk */
  if(music_set)
  {
    Mix_FreeMusic(raw_data);
    music_set = false;
  }
  
  raw_data = NULL;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

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
void Music::stop(bool fade_out)
{
  if(fade_out)
    Mix_FadeOutMusic(kFADE_TIME);
  else
    Mix_HaltMusic();
}
