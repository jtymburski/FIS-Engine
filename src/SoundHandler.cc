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

/* Constructor: Sets up a blank template, nothing will play */
SoundHandler::SoundHandler()
{
}

/* Destructor function */
SoundHandler::~SoundHandler()
{
  removeAll();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Create new sound files, based on ID */
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

/* Create new sound files, based on ID */
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

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Add sound files */
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

/* Add sound files */
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

/* Add to queue */
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

/* Add to queue */
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

/* Add stop channel to queue */
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

/* Getters for sound files */
Sound* SoundHandler::getAudioMusic(uint32_t id)
{
  auto iter = audio_music.find(id);
  if(iter != audio_music.end())
    return iter->second;
  return nullptr;
}

/* Getters for sound files */
Sound* SoundHandler::getAudioSound(uint32_t id)
{
  auto iter = audio_sound.find(id);
  if(iter != audio_sound.end())
    return iter->second;
  return nullptr;
}

/* Is the given ID music or sound file valid and set */
bool SoundHandler::isMusicSet(uint32_t id)
{
  Sound* chunk = getAudioMusic(id);
  if(chunk != nullptr && chunk->getRawData() != nullptr)
    return true;
  return false;
}

/* Is the given ID music or sound file valid and set */
bool SoundHandler::isSoundSet(uint32_t id)
{
  Sound* chunk = getAudioSound(id);
  if(chunk != nullptr && chunk->getRawData() != nullptr)
    return true;
  return false;
}

/* Load data from file */
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
  
/* Process the queue */
void SoundHandler::process()
{
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

/* Remove sound files */
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

/* Remove sound files */
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

/* Remvoe sound files */
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
