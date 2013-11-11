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
*
* Notes: main needed to be undefined because of how SDL forces control of
*        Winmain for initialization. Since I do initialization myself,
*        this call wasn't needed.
******************************************************************************/
#undef main

#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Sound : public QObject
{
  Q_OBJECT

public:
  /* Constructor: Sets up a blank template, nothing will play */
  Sound();

  /* Constructor: Sets up all the pertinent data to create a sound */
  Sound(QString path, int loop_count = 0);

  /* Destructor function */
  ~Sound();

private:
  /* The channel number the sound will be played on */
  int channel;

  /* The number of times to play the sound. It will be loop_count+1 
   * except for -1, which is infinite loop */
  int loop_count;

  /* The path for the played sound file */
  QString path;

  /* The raw data of the sound in RAM */
  Mix_Chunk* sound;

  /* If the sound has been set */
  bool sound_set;

  /* --------------------- CONSTANTS --------------------- */
  const static short kINIT_BUFFERS; /* Size of audio buffers in memory */
  const static short kINIT_CHANNELS; /* 1 channel = mono, 2 = stereo */
  const static Uint16 kINIT_FORMAT; /* Format of the audio being played */
  const static short kINIT_RATE; /* Frequency of Audio Playback */
  const static short kINFINITE_LOOP; /* Infinite loop special character */
  const static short kUNSET_CHANNEL; /* Unset channel definition */

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  /* Play slot. If sound is set, it will play the sound for the given number
   * of loops */
  void play();

  /* Stop slot. Will stop the sound, if playing */
  void stop();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the loop count, the number of times it will play for */
  int getPlayCount();

  /* Trys to set the sound file to the given path */
  bool setSoundFile(QString path);
  
  /* Set the number of times to play the song for. If 0, defaults to 1 */
  bool setPlayCount(int play_count);

  /* Set the sound to loop infinitely, until stop() is called */
  void setPlayForever();

  /* Unset the sound file and frees the memory */
  bool unsetSoundFile(bool clear_path = true);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Cleans up the initialization protocol which ends all sound */
  static void cleanupSDL();

  /* Initiates the sound protocol using SDL */
  static bool initiateSDL();

  /* Plays a sound, only once. More than that? create a sound handle */
  //bool playSound(); // TODO
  
  /* Status of the running SDL system */
  static bool statusSDL();
};

#endif // SOUND_H
