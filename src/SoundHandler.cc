/*******************************************************************************
 * Class Name: SoundHandler
 * Date Created: September 14, 2015
 * Inheritance: none
 * Description: Is the handler for all sound resources in the application.
 *              Controls the queue and the channels and when sounds play, when
 *              they don't, etc.
 ******************************************************************************/
#include "SoundHandler.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Sets up a blank sound handler database. Nothing will play on
 *              initial trigger.
 *
 * Inputs: none
 */
SoundHandler::SoundHandler()
{
}

/*
 * Description: Destructor function
 */
SoundHandler::~SoundHandler()
{
  removeAll();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Creates an audio music chunk with the appropriate properties
 *              and the given ID and adds it to the music list in the database.
 *
 * Inputs: uint32_t id - the new ID for the music chunk
 * Output: Sound* - the new music sound chunk
 */
Sound* SoundHandler::createAudioMusic(uint32_t id)
{
  Sound* found_chunk = getAudioMusic(id);

  /* Process */
  if(found_chunk == nullptr)
  {
    Sound* new_chunk = new Sound();
    new_chunk->setID(id);
    if(!addMusic(new_chunk))
    {
      delete new_chunk;
      new_chunk = nullptr;
    }
    found_chunk = new_chunk;
  }
  return found_chunk;
}

/*
 * Description: Creates an audio sound chunk with the appropriate properties
 *              and the given ID and adds it to the sound list in the database.
 *
 * Inputs: uint32_t id - the new ID for the sound chunk
 * Output: Sound* - the new one shot sound chunk
 */
Sound* SoundHandler::createAudioSound(uint32_t id)
{
  Sound* found_chunk = getAudioSound(id);

  /* Process */
  if(found_chunk == nullptr)
  {
    Sound* new_chunk = new Sound();
    new_chunk->setID(id);
    if(!addSound(new_chunk))
    {
      delete new_chunk;
      new_chunk = nullptr;
      return nullptr;
    }
    found_chunk = new_chunk;
  }
  return found_chunk;
}

/*
 * Description: Takes the queue before processing and provides any clean-up
 *              if required. Current state just removes play triggers on similar
 *              channels if stop triggers follow in the same queue.
 *
 * Inputs: none
 * Output: none
 */
void SoundHandler::queueCleanUp()
{
  /* Go through all entries */
  for(int i = 0; i < (int)queue.size(); i++)
  {
    bool valid = true;

    /* Check all next entries vs. current entry */
    for(int j = i + 1; valid && j < (int)queue.size(); j++)
    {
      /* If a later entry is stopping an entry in the same queue that's playing,
       * remove play entry */
      if(queue[i].channel == queue[j].channel &&
         !queue[i].stop && queue[j].stop)
      {
        valid = false;
      }
    }

    /* Therefore, if not valid - delete */
    if(!valid)
    {
      queue.erase(queue.begin() + i);
      i--;
    }
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds the passed in audio chunk to the music queue, assuming the
 *              ID is valid and the chunk is valid. Once added, this class takes
 *              control of the memory.
 *
 * Inputs: Sound* chunk - the new chunk to add to the list
 * Output: bool - true if the chunk was added to the music list
 */
bool SoundHandler::addMusic(Sound* chunk)
{
  /* Check to make sure the chunk is valid */
  if(chunk != nullptr && chunk->getID() >= 0)
  {
    /* Erase existing, if relevant */
    int id = chunk->getID();
    removeMusic(id);

    /* Modify chunk settings */
    chunk->setChannel(SoundChannels::MUSIC1);
    chunk->setLoopForever();

    /* Add new */
    auto ret = audio_music.insert(std::pair<int, Sound*>(id, chunk));
    if(ret.second == false)
      std::cerr << "[WARNING] Chunk ID: " << id << " still existing on insert. "
                << "Previous delete failed." << std::endl;
    else
      return true;
  }
  return false;
}

/*
 * Description: Adds the passed in audio chunk to the sound queue, assuming the
 *              ID is valid and the chunk is valid. Once added, this class takes
 *              control of the memory.
 *
 * Inputs: Sound* chunk - the new chunk to add to the list
 * Output: bool - true if the chunk was added to the sound list
 */
bool SoundHandler::addSound(Sound* chunk)
{
  /* Check to make sure the chunk is valid */
  if(chunk != nullptr && chunk->getID() >= 0)
  {
    /* Erase existing, if relevant */
    int id = chunk->getID();
    removeSound(id);

    /* Modify chunk settings */
    chunk->setChannel(SoundChannels::TILES);
    chunk->setLoopCount(0);

    /* Add new */
    auto ret = audio_sound.insert(std::pair<int, Sound*>(id, chunk));
    if(ret.second == false)
      std::cerr << "[WARNING] Chunk ID: " << id << " still existing on insert."
                << "Previous delete failed." << std::endl;
    else
      return true;
  }
  return false;
}

/*
 * Description: Add play trigger to queue for sound ID on the designated
 *              channel. If force is enabled, it will immediately process the
 *              queue.
 *
 * Inputs: uint32_t id - the sound ID to trigger
 *         SoundChannels channel - the channel to trigger on
 *         bool process_force - true to force a process call
 * Output: none
 */
void SoundHandler::addPlayToQueue(uint32_t id, SoundChannels channel,
                                  bool process_force)
{
  if(channel != SoundChannels::UNASSIGNED)
  {
    SoundQueue entry = {id, channel, false};
    queue.push_back(entry);
    if(process_force)
      process();
  }
}

/*
 * Description: Add play trigger stack to queue for sound ID on the designated
 *              channels. If force is enabled, it will immediately process the
 *              queue.
 *
 * Inputs: std::vector<SoundQueue> entries - a stack of entries to add to queue
 *         bool process_force - true to force a process call
 * Output: none
 */
void SoundHandler::addPlayToQueue(std::vector<SoundQueue> entries,
                                  bool process_force)
{
  if(entries.size() > 0)
  {
    /* Process all entries - no stop allowed */
    for(uint32_t i = 0; i < entries.size(); i++)
      entries[i].stop = false;

    /* Append entries */
    queue.insert(queue.end(), entries.begin(), entries.end());
    if(process_force)
      process();
  }
}

/*
 * Description: Add stop trigger to queue for the designated channel. If force
 *              is enabled, it will immediately process the queue.
 *
 * Inputs: SoundChannels channel - the channel to stop playing
 *         bool process_force - true to force a process call
 * Output: none
 */
void SoundHandler::addStopToQueue(SoundChannels channel, bool process_force)
{
  if(channel != SoundChannels::UNASSIGNED)
  {
    SoundQueue entry = {0, channel, true};
    queue.push_back(entry);
    if(process_force)
      process();
  }
}

/*
 * Description: Returns audio chunk from the music stack with the connected ID.
 *              If the ID is not found, returns null.
 *
 * Inputs: uint32_t id - the ID of the music chunk to find
 * Output: Sound* - the found chunk
 */
Sound* SoundHandler::getAudioMusic(uint32_t id)
{
  auto iter = audio_music.find(id);
  if(iter != audio_music.end())
    return iter->second;
  return nullptr;
}

/*
 * Description: Returns audio chunk from the sound stack with the connected ID.
 *              If the ID is not found, returns null.
 *
 * Inputs: uint32_t id - the ID of the sound chunk to find
 * Output: Sound* - the found chunk
 */
Sound* SoundHandler::getAudioSound(uint32_t id)
{
  auto iter = audio_sound.find(id);
  if(iter != audio_sound.end())
    return iter->second;
  return nullptr;
}

/*
 * Description: Returns if there is an audio chunk in the music stack with the
 *              given ID.
 *
 * Inputs: uint32_t id - the ID to find if valid in the music stack
 * Output: bool - true if an audio chunk was found with the ID
 */
bool SoundHandler::isMusicSet(uint32_t id)
{
  Sound* chunk = getAudioMusic(id);
  if(chunk != nullptr && chunk->getRawData() != nullptr)
    return true;
  return false;
}

/*
 * Description: Returns if there is an audio chunk in the sound stack with the
 *              given ID.
 *
 * Inputs: uint32_t id - the ID to find if valid in the sound stack
 * Output: bool - true if an audio chunk was found with the ID
 */
bool SoundHandler::isSoundSet(uint32_t id)
{
  Sound* chunk = getAudioSound(id);
  if(chunk != nullptr && chunk->getRawData() != nullptr)
    return true;
  return false;
}

/*
 * Description: Loads the data from file associated with the sound database.
 *
 * Inputs: XmlData data - the xml data structure
 *         int index - the element reference index
 *         std::string base_path - base path to project directory
 * Output: bool - true if load was successful
 */
bool SoundHandler::load(XmlData data, int index, std::string base_path)
{
  bool success = true;
  Sound* edit_chunk = nullptr;

  /* Get chunk pointer */
  if(data.getElement(index) == "music" && !data.getKeyValue(index).empty())
  {
    int id = stoi(data.getKeyValue(index));
    edit_chunk = createAudioMusic(id);
  }
  else if(data.getElement(index) == "sound" && !data.getKeyValue(index).empty())
  {
    int id = stoi(data.getKeyValue(index));
    edit_chunk = createAudioSound(id);
  }

  /* Process changes */
  if(edit_chunk != nullptr)
  {
    if(data.getElement(index + 1) == "fade")
    {
      edit_chunk->setFadeTime(data.getDataInteger());
    }
    else if(data.getElement(index + 1) == "path")
    {
      success &= edit_chunk->setSoundFile(base_path + data.getDataString());
    }
    else if(data.getElement(index + 1) == "vol")
    {
      int sound_vol = data.getDataInteger();
      if(sound_vol >= 0 && sound_vol <= 255)
        edit_chunk->setVolume(sound_vol);
      else
        success = false;
    }
  }
  else
  {
    success = false;
  }

  return success;
}

/*
 * Description: Process the queue of sound and music triggers.
 *
 * Inputs: none
 * Output: none
 */
void SoundHandler::process()
{
  /* Pre-Processing */
  queueCleanUp();

  /* Loop through queue */
  for(uint32_t i = 0; i < queue.size(); i++)
  {
    /* -- MUSIC OR WEATHER QUEUE ITEM -- */
    if(queue[i].channel == SoundChannels::MUSIC1 ||
       queue[i].channel == SoundChannels::MUSIC2 ||
       queue[i].channel == SoundChannels::WEATHER1 ||
       queue[i].channel == SoundChannels::WEATHER2)
    {
      /* Channel selector - music or weather */
      SoundChannels channel1 = SoundChannels::MUSIC1;
      SoundChannels channel2 = SoundChannels::MUSIC2;
      if(queue[i].channel == SoundChannels::WEATHER1 ||
         queue[i].channel == SoundChannels::WEATHER2)
      {
        channel1 = SoundChannels::WEATHER1;
        channel2 = SoundChannels::WEATHER2;
      }

      /* Play processing */
      if(!queue[i].stop)
      {
        /* Check current channel status */
        bool playing_mus1 = Sound::isChannelPlaying(channel1);
        bool playing_mus2 = Sound::isChannelPlaying(channel2);

        /* State both playing - stop one */
        if(playing_mus1 && playing_mus2)
        {
          Sound::stopChannel(channel2, 0);
          playing_mus2 = Sound::isChannelPlaying(channel2);
        }

        /* Get the relevant sound chunk */
        Sound* found_chunk = getAudioMusic(queue[i].id);
        if(found_chunk != nullptr)
        {
          /* Only proceed if at least one channel isn't active anymore */
          if(!playing_mus1 || !playing_mus2)
          {
            /* If chunk is playing and not on either channels, kill it */
            if(found_chunk->isPlaying() &&
               found_chunk->getChannel() != channel1 &&
               found_chunk->getChannel() != channel2)
            {
              found_chunk->stop(true);
            }

            /* Channel 1 is active */
            if(playing_mus1)
            {
              found_chunk->setChannel(channel2);
              found_chunk->crossFade(channel1);
            }
            /* Channel 2 is active */
            else if(playing_mus2)
            {
              found_chunk->setChannel(channel1);
              found_chunk->crossFade(channel2);
            }
            /* No channels are active */
            else
            {
              found_chunk->setChannel(channel1);
              found_chunk->play();
            }
          }
        }
      }
      /* Stop processing */
      else
      {
        Sound::stopChannel(channel1);
        Sound::stopChannel(channel2);
      }
    }
    /* -- SOUND QUEUE ITEM -- */
    else if(queue[i].channel != SoundChannels::UNASSIGNED)
    {
      /* Play processing */
      if(!queue[i].stop)
      {
        /* Try and stop the channel prior - only where required */
        if(queue[i].channel == SoundChannels::MENUS)
          Sound::stopChannel(queue[i].channel, 0);

        /* Only process if the channel is not playing */
        if(!Sound::isChannelPlaying(queue[i].channel))
        {
          /* Try and find the sound chunk - and ensure its not playing */
          Sound* found_chunk = getAudioSound(queue[i].id);
          if(found_chunk != nullptr && !found_chunk->isPlaying())
          {
            /* Update channel and play */
            found_chunk->setChannel(queue[i].channel);
            found_chunk->play();
          }
        }
      }
      /* Stop processing */
      else
      {
        Sound::stopChannel(queue[i].channel, 0);
      }
    }
  }

  /* Clear the queue */
  queue.clear();
}

/*
 * Description: Removes all music and sound files from the database. If they
 *              are playing, they will be stopped upon deletion.
 *
 * Inputs: none
 * Output: none
 */
void SoundHandler::removeAll()
{
  /* Go through music files */
  for(auto iter = audio_music.begin(); iter != audio_music.end(); iter++)
    delete iter->second;
  audio_music.clear();

  /* Go through sound files */
  for(auto iter = audio_sound.begin(); iter != audio_sound.end(); iter++)
    delete iter->second;
  audio_sound.clear();

  /* Delete queue */
  queue.clear();
}

/*
 * description: Removes the music chunk from the database with the given id.
 *              if there is no music chunk with the given id, it returns false.
 *
 * inputs: uint32_t id - the music chunk id to try and find and delete
 * output: bool - true if the music chunk was found and removed
 */
bool SoundHandler::removeMusic(uint32_t id)
{
  Sound* found = getAudioMusic(id);
  if(found != nullptr)
  {
    delete found;
    audio_music.erase(id);
    return true;
  }
  return false;
}

/*
 * description: Removes the sound chunk from the database with the given id.
 *              if there is no sound chunk with the given id, it returns false.
 *
 * inputs: uint32_t id - the sound chunk id to try and find and delete
 * output: bool - true if the sound chunk was found and removed
 */
bool SoundHandler::removeSound(uint32_t id)
{
  Sound* found = getAudioSound(id);
  if(found != nullptr)
  {
    delete found;
    audio_sound.erase(id);
    return true;
  }
  return false;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/* Pause all channels or select channels */
//static void pauseAllChannels();
