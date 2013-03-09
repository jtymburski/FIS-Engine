/*******************************************************************************
* Class Name: Battle
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: Game
* Description: 
*
* Notes: Turn Progression:
*
* 1. generalUpkeep() adjusts all values based on Weather.  BattleInfoBar
* displays info on these adjustments * after X seconds, personalUpkeep() is 
* called.
*
* 2. personalUpkeep() adjusts all values of the Person represented by 
* party_index. PersonalUpkeep() then, after X seconds, activates and 
* initializes BattleMenu with party_index's relevant options.
*
* 3. Once an option from BattleMenu is selected processAction() is
* called and party_index is incremented, then if it is less
* than 5, personalUpkeep is called (Step 2).  Else, BattleMenu 
* deactivates and processEnemyActions() is called (Step 4).
*
* 4. processEnemyActions() uses the enemies scripts to add actions to the
* stack, after which orderActions() is called.
*
* 5. orderActions() reorders the stack based on speed and status ailments,
* performActions() is then called.
*
* 6. performAction() takes the first Action on the stack and loads it into 
* action_animate which then shows the Actions animation.  The Action is then 
* removed from the stack.
*
* 7. action_animate emits a finished(Action *) signal which is connected to
* changeStats(Action* battle_action), changeStats(Action* battle_action) 
* alters each Person's stats based off its parameter Action.  UpdateScene() 
* is then called.
*
* 8. updateScene() emits a finished() signal which is connected to 
* actionOutcome(). ActionOutcome() checks for deaths and animates 
* appropriately, if a whole party is eliminated, then all the memory
* is cleared, and battleWon() or battleLost() is called.  If the stack is not
* empty, (Step 6) else reset party_index and (Step 1).
*   
* Note: Animation has changed since this design and requires some alteration 
*     in terms of these steps.
*  
* In Battle Action Animations
* ----------------------------
* Action Timer is removed
* For actions that occur during battle, the animations will only be targets
* of the player and centered on them. For example, there will be a sequence 
* of sprites where it will animate (based on a timer) and flash on the target 
* and possibly flash on the caster itself. These will be attached to the action
* class and will occur during the generation of the action in the step 6? This
* could be tied into the action paint event to draw on the battle screen These 
* would utilize the sprite class which already has a linked list to generate 
* the sequences of the animation. How would this be programmed in? Attached 
* to the database since we generated a sequence within action to program in 
* actions and commands that can be used in the battle sequence. An addition 
* to that sequence to include frames or generate generic ones that everyone 
* has to share.
******************************************************************************/
#ifndef BATTLE_H
#define BATTLE_H

#include <QKeyEvent>
#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <cmath>
#include "Game/Battle/BattleInfoBar.h"
#include "Game/Battle/BattleMenu.h"
#include "Game/Battle/BattleStatusBar.h"
#include "Game/Player/SkillSet.h"
#include "Game/Player/Party.h"
#include "Game/Weather.h"

class Battle: public QWidget
{
  Q_OBJECT

public:
  /* Constructor for a Battle class */
  Battle(Party* p_friends, Party* p_foes, QWidget* pointer = 0);

  /* Annihilates a battle object */
  ~Battle();

  /* Enumerated flags for battle class */
  enum BattleState
  {
    BOSS     = 1ul << 0, /* Is this a boss battle? */
    MINIBOSS = 1ul << 1  /* Is this a mini-boss battle? */
  };
  Q_DECLARE_FLAGS(BattleFlags, BattleState)
  BattleFlags flag_set;

private:
  /* Constant max limits */
  static const int kMAX_PARTY_SIZE = 5;

  /* Sets the allies pointer */
  void setFriends(Party* p_friends = NULL);

  /* Sets the foes pointer */
  void setFoes(Party* p_foes = NULL);

  /* Sets up bounding boxes */
  void setUpBoxes();

  /* Sets the maximum x-length of the battle window */
  void setMaxWidth(int value);

  /* Sets the maximum y-length of the battle window */
  void setMaxHeight(int value);

  /* Pointer to the battle info bar */
  BattleInfoBar* info_bar;

  /* Battle Status bar */
  BattleStatusBar* status_bar;
  QVector<EnemyStatusBar*> enemy_status_bar;

  /* The Battle menu pointer (for selecting actions), off by default */
  // BattleMenu* menu;

  /* Checks if targeting mode is active */
  bool target_mode; //Checks if targeting mode is active

  /* Maximum size of the battle window */
  uint max_width;
  uint max_height;

  /* Bounding box dimensions */
  uint ally_width;
  uint ally_height;

  /* Which party is currently selected */
  uint party_index;

  /* Current target selected */
  uint target_index;

  /* Current turn count */
  uint turn_count;

  /* The enemy party */
  Party* foes; 

  /* Allied party */
  Party* friends;

  /* Backdrop for the battle */
  QPixmap* battle_bg;

  /* Battle Status Bar Image */
  QPixmap* battle_status_bar_image;

  /* Ally Bounding Boxes */
  QVector<QRect*> ally_box;

  /* Enemy Bounding Boxes & StatusBar Boxes */
  QVector<QRect*> enemy_box;
  QVector<QRect*> enemy_status_boxes;

  /* Bounding boxes */
  // QRect* target_box;
  QRect* status_box;
  QRect* info_box;
  QRect* extra_box;

  /* The skill buffer */
  // QVector<Skill*> skill_buffer;

  /* The items use buffer */
  // QVector<Item*> items_buffer;

  /* Weather condition during battle */
  // Weather* weather_cond;
   
protected:
  /* Handles all key entries */
  void keyPressEvent(QKeyEvent*);

  /* Paint event for the class */
  void paintEvent(QPaintEvent*);
  void paintAll();
  void paintMenu();

  /* Checks for deaths, pops current action off stack, calls performAction();*/
  void actionOutcome();

public slots:
  void closeBattle();

signals:
  void closingBattle(int index);
  void finished();

public: 

  /* Ends battle with win message */
  void battleWon(); 

  /* Ends Battle with loss message, and reverts game to last save point */
  void battleLost(); 

  /* Arranges and paints static image */
  void buildScene(); 

  /* Sets temporary stats */
  void buildStats(); 

  /* Alters stats, updates scene */
  void changeStats(Action* battle_action); 

  /* Non-character events */
  void generalUpkeep(); 

  /* Reorders stack (speed based) */
  void orderActions(); 

  /* Performs current action animation */
  void performAction(); 

  /* Character events */
  void personalUpkeep();

  /* Increment party index for next Battle Menu */
  void processAction(); 

  /* Gets all enemy actions, adds to stack */
  void processEnemyActions(); 

  /* Paints dynamic images */
  void updateScene();

  /* Gets the maximum x-length of the battle window */
  int getMaxWidth();

  /* Gets the maximum y-length of the battle window */
  int getMaxHeight();

  /* Sets the targeting mode (slot) */
  void setTargetMode(bool targeting);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Battle::BattleFlags)

#endif // BATTLE_H
