/*******************************************************************************
 * Class Name: SoundHandler
 * Date Created: September 14, 2015
 * Inheritance: none
 * Description: Is the handler for all sound resources in the application.
 *              Controls the queue and the channels and when sounds play, when
 *              they don't, etc.
 ******************************************************************************/
#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include "Sound.h"
#include "XmlData.h"

/* Sound handler class */
class SoundHandler
{
public:
  /* Constructor: Sets up a blank template, nothing will play */
  SoundHandler();

  /* Destructor function */
  ~SoundHandler();

private:
  /* Audio Sets */
  std::vector<Sound*> audio_music;
  std::vector<Sound*> audio_sound;

  /* Queue */
  std::vector<SoundQueue> queue;

  /* --------------------- CONSTANTS --------------------- */
  //const static short kINFINITE_LOOP; /* Infinite loop special character */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Create new sound files, based on ID */
  void createAudioMusic(int id);
  void createAudioSound(int id);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Add sound files */
  bool addAudioMusic(Sound* chunk);
  bool addAudioSound(Sound* chunk);

  /* Add to queue */
  void addToQueue(const SoundQueue& entry);
  void addToQueue(std::vector<SoundQueue> entries);

  /* Clear */
  void clear();

  /* Getters for sound files */
  Sound* getAudioMusic(int id);
  Sound* getAudioSound(int id);

  /* Load data from file */
  bool loadData(XmlData data, int index, std::string base_path);

  /* Process the queue */
  void process();

  /* Remove sound files */
  bool removeAudioMusic(int id);
  bool removeAudioSound(int id);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Pause all channels or select channels */
  //static void pauseAllChannels();
};

#endif // SOUNDHANDLER_H
