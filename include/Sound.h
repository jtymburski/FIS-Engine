/******************************************************************************
* Class Name: Sound
* Date Created: February 9, 2012
* Inheritance: MediaObject
* Description: Handles all sound used within the game. Must use this instead
*              of QSound since Linux doesn't cleanly support QSound except for
*              a specific library requirement for the sound drivers.
******************************************************************************/
#ifndef SOUND_H
#define SOUND_H

#include <Phonon/MediaObject>

class Sound : public Phonon::MediaObject
{
public:
  Sound();
  ~Sound();
};

#endif // SOUND_H
