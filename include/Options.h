/******************************************************************************
* Class Name: Options
* Date Created: Sunday, November 4th, 2012
* Inheritance: QWidget
* Description: Options menu 
*  
* TODO: See cc file
******************************************************************************/
#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

class Options : public QWidget
{
public:
  /* Constructor(s) */
  Options(QWidget* parent = 0);

  /* Copy constructor */
  Options(const Options &source, QWidget* parent = 0);

  /* Destructor */
  ~Options();

  /* Battle Options */
  enum BattleOptions {    BEARWALK,
                          BEARLY_DIFFICULT,
                          GRIZZLY };

  enum BattleMode {  DEBUG,
                     GUI    };

private:
  /* Amount of information to display during ailment updates */
  BattleOptions ailment_update_state;

  /* Amount of information to display on the Battle HUD */
  BattleOptions battle_hud_state;

  /* Output mode the Battle is operating in */
  BattleMode battle_mode;

  /* Resolution of the screen currently being used */
  short resolution_x;
  short resolution_y;

  /* Vertical refresh syncing enable */
  bool vsync_enabled;

  /* Horizontal resolution options */
  //QVector<int> x_options; 

  /* Vertical resolution options */
  //QVector<int> y_options; 

  /* Primary options menu */
  //QList<QString> main_options;

  /* Secondary options menu */
  /* Notes: other options to be added as need (audio, animations, font size) */
  //QList<QString> secondary_Options;

  /* Flag for battle animations */
  //bool ANIMATIONS; 

  /*------------------- Constants -----------------------*/
  const static short kNUM_RESOLUTIONS;  /* Number of resolutions available */
  const static short kRESOLUTIONS_X[]; /* X Resolution sizes available */
  const static short kRESOLUTIONS_Y[]; /* Y Resolution sizes available */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy all parameters from the other to the current if called */
  void copySelf(const Options &source);

  /* Sets all parameters to default */
  void setAllToDefault();

  /* Assign a new ailment update state to BattleOptions */
  void setAilmentUpdateState(BattleOptions new_state);

  /* Assign a new hud update state to BattleOptions */
  void setBattleHudState(BattleOptions new_state);

  /* Assigns a new battle mode */
  void setBattleMode(BattleMode new_state);

  /* Set the screen dimensions to be painted, based on the array index */
  void setScreenHeight(int index);
  void setScreenWidth(int index);

  /* Enable or disable vertical refresh syncing */
  void setVsync(bool enabled);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the value of the given option */
  //int getOption(QString s); 

  /* Returns the current state of ailment updates */
  BattleOptions getAilmentUpdateState();

  /* Returns the current state of battle hud display */
  BattleOptions getBattleHudState();

  /* Returns the current state of Battle update mode */
  BattleMode getBattleMode();

  /* Returns the screen dimensions to be painted */
  short getScreenHeight();
  short getScreenWidth();

  /* Returns if vertical refresh syncing is enabled */
  bool isVsyncEnabled();

  /* Sets the given option ot the give value (may require reimplementation) */
  //void setOption(QString s, int value); 

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  Options& operator= (const Options &source);
};

#endif // OPTIONS_H
