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
  // TODO: TEMP - REMOVE: create test sound files
  char* directory = SDL_GetBasePath();
  std::string base_path(directory);
  SDL_free(directory);

  /* ---- MUSIC ---- */
  Sound* title_music = new Sound();
  title_music->setID(Sound::kID_MUSIC_TITLE);
  title_music->setFadeTime(2000);
  title_music->setSoundFile(base_path + "sound/unlicensed/ag_theme.ogg");
  addMusic(title_music);

  /* ---- CUSTOM MUSIC ---- */
  Sound* game_music = new Sound();
  game_music->setID(1000);
  game_music->setFadeTime(2500);
  game_music->setSoundFile(base_path +
                           "sound/ambience/background_menu_sound.wav");
  addMusic(game_music);

  /* ---- SOUND ---- */
  Sound* menu_sound = new Sound();
  menu_sound->setID(Sound::kID_SOUND_MENU_CHG);
  menu_sound->setSoundFile(base_path + "sound/functional/menu_click.wav");
  addSound(menu_sound);

  Sound* menu_nxt_sound = new Sound();
  menu_nxt_sound->setID(Sound::kID_SOUND_MENU_NEXT);
  menu_nxt_sound->setSoundFile(base_path + "sound/functional/menu_next.wav");
  addSound(menu_nxt_sound);

  Sound* menu_prv_sound = new Sound();
  menu_prv_sound->setID(Sound::kID_SOUND_MENU_PREV);
  menu_prv_sound->setSoundFile(base_path + "sound/functional/menu_prev.wav");
  addSound(menu_prv_sound);

  /* ---- CUSTOM SOUND ---- */
  Sound* grass_sound = new Sound();
  grass_sound->setID(1000);
  grass_sound->setSoundFile(base_path + "sound/map/grass01.wav");
  addSound(grass_sound);

  Sound* metal_sound = new Sound();
  metal_sound->setID(1001);
  metal_sound->setSoundFile(base_path + "sound/map/metal01.wav");
  addSound(metal_sound);

  Sound* wood_sound = new Sound();
  wood_sound->setID(1002);
  wood_sound->setSoundFile(base_path + "sound/map/wood01.wav");
  addSound(wood_sound);

  Sound* bush_sound = new Sound();
  bush_sound->setID(1003);
  bush_sound->setSoundFile(base_path + "sound/map/bush01.wav");
  addSound(bush_sound);
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
void SoundHandler::addToQueue(uint32_t id, SoundChannels channel,
                              bool process_force)
{
  addToQueue({id, channel}, process_force);
}

/* Add to queue */
void SoundHandler::addToQueue(const SoundQueue& entry,
                              bool process_force)
{
  queue.push_back(entry);
  if(process_force)
    process();
}

/* Add to queue */
void SoundHandler::addToQueue(std::vector<SoundQueue> entries,
                              bool process_force)
{
  queue.insert(queue.end(), entries.begin(), entries.end());
  if(process_force)
    process();
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
      if(data.getDataInteger() <= 255)
        edit_chunk->setVolume(data.getDataInteger());
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
    /* -- MUSIC QUEUE ITEM -- */
    if(queue[i].channel == SoundChannels::MUSIC1 ||
       queue[i].channel == SoundChannels::MUSIC2)
    {
      bool playing_mus1 = Sound::isChannelPlaying(SoundChannels::MUSIC1);
      bool playing_mus2 = Sound::isChannelPlaying(SoundChannels::MUSIC2);

      /* State both playing - stop one */
      if(playing_mus1 && playing_mus2)
      {
        Sound::stopChannel(SoundChannels::MUSIC2);
        playing_mus2 = Sound::isChannelPlaying(SoundChannels::MUSIC2);
      }

      /* Only proceed if at least one channel isn't active anymore */
      if(!playing_mus1 || !playing_mus2)
      {
        /* Get sound file */
        Sound* found_chunk = getAudioMusic(queue[i].id);
        if(found_chunk != nullptr && !found_chunk->isPlaying())
        {
          /* Channel 1 is active */
          if(playing_mus1)
          {
            found_chunk->setChannel(SoundChannels::MUSIC2);
            found_chunk->crossFade(SoundChannels::MUSIC1);
          }
          /* Channel 2 is active */
          else if(playing_mus2)
          {
            found_chunk->setChannel(SoundChannels::MUSIC1);
            found_chunk->crossFade(SoundChannels::MUSIC2);
          }
          /* No channels are active */
          else
          {
            found_chunk->setChannel(SoundChannels::MUSIC1);
            found_chunk->play();
          }
        }
      }
    }
    /* -- SOUND QUEUE ITEM -- */
    else if(queue[i].channel != SoundChannels::UNASSIGNED)
    {
      /* Try and stop the channel prior - only where required */
      if(queue[i].channel == SoundChannels::MENUS)
        Sound::stopChannel(queue[i].channel);

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
