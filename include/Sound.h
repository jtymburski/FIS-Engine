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

#include <QDebug>
#include <QObject>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Sound : public QObject
{
  Q_OBJECT

public:
  Sound(QString path, int loop_count = 0);
  Sound();
  ~Sound();

private:
  int channel;
  int loop_count;
  Mix_Chunk* sound;
  bool sound_set;

  /* --------------------- CONSTANTS --------------------- */
  static const int kINFINITE_LOOP = -1;
  static const int kUNSET_CHANNEL = -1;

public slots:
  void play();
  void stop();

public:
  int getLoopCount();
  bool setSoundFile(QString path);
  void setLoopCount(int loop_count);
  bool unsetSoundFile();
};

#endif // SOUND_H
