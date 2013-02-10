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

#include <QCoreApplication>
#include <phonon/AudioOutput>
#include <phonon/MediaObject>
#include <phonon/MediaSource>

class Sound : public Phonon::MediaObject
{
  Q_OBJECT

public:
  Sound(QString path, int loop_count = 0);
  Sound();
  ~Sound();

private:
  Phonon::AudioOutput* audio_ctrl;
  int loop_count;
  QString source_path;
  bool source_set;

  /* --------------------- CONSTANTS --------------------- */
  static const int kINFINITE_LOOP = -1;

public slots:
  void queueSong();
  void stopSong();

signals:
  void finishedSequence();

private:
  void setup();

public:
  int getLoopCount();
  bool setSourceFile(QString path);
  void setLoopCount(int loop_count);
};

#endif // SOUND_H
