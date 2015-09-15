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

}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Create new sound files, based on ID */
void SoundHandler::createAudioMusic(int id)
{

}

/* Create new sound files, based on ID */
void SoundHandler::createAudioSound(int id)
{

}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
  
/* Add sound files */
bool SoundHandler::addAudioMusic(Sound* chunk)
{

}

/* Add sound files */
bool SoundHandler::addAudioSound(Sound* chunk)
{

}

/* Add to queue */
void SoundHandler::addToQueue(const SoundQueue& entry)
{

}

/* Add to queue */
void SoundHandler::addToQueue(std::vector<SoundQueue> entries)
{

}

/* Clear */
void SoundHandler::clear()
{

}

/* Getters for sound files */
Sound* SoundHandler::getAudioMusic(int id)
{

}

/* Getters for sound files */
Sound* SoundHandler::getAudioSound(int id)
{

}

/* Load data from file */
bool SoundHandler::loadData(XmlData data, int index, std::string base_path)
{

}

/* Process the queue */
void SoundHandler::process()
{

}

/* Remove sound files */
bool SoundHandler::removeAudioMusic(int id)
{

}

/* Remvoe sound files */
bool SoundHandler::removeAudioSound(int id)
{

}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
  
/* Pause all channels or select channels */
//static void pauseAllChannels();
