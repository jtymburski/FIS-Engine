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

#include <map>

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
  std::map<int, Sound*> audio_music;
  std::map<int, Sound*> audio_sound;

  /* Queue */
  std::vector<SoundQueue> queue;

  /* --------------------- CONSTANTS --------------------- */
  //const static short kINFINITE_LOOP; /* Infinite loop special character */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Create new sound files, based on ID */
  Sound* createAudioMusic(uint32_t id);
  Sound* createAudioSound(uint32_t id);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Add sound files */
  bool addMusic(Sound* chunk);
  bool addSound(Sound* chunk);

  /* Add to queue */
  void addPlayToQueue(uint32_t id, SoundChannels channel,
                      bool process_force = false);
  void addPlayToQueue(std::vector<SoundQueue> entries, 
                      bool process_force = false);
  void addStopToQueue(SoundChannels channel, bool process_force = false);

  /* Getters for sound files */
  Sound* getAudioMusic(uint32_t id);
  Sound* getAudioSound(uint32_t id);

  /* Is the given ID music or sound file valid and set */
  bool isMusicSet(uint32_t id);
  bool isSoundSet(uint32_t id);

  /* Load data from file */
  bool load(XmlData data, int index, std::string base_path);

  /* Process the queue */
  void process();

  /* Remove sound files */
  void removeAll();
  bool removeMusic(uint32_t id);
  bool removeSound(uint32_t id);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Pause all channels or select channels */
  //static void pauseAllChannels();
};

#endif // SOUNDHANDLER_H
