 /*****************************************************************************
* Class Name: Battle
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: Game
* Description: 
*
*  TODO: CONSTRUCTORS TO BE FINISHED
*
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
* action_animate which then shows the Actions animation.  The Action is then removed from the stack.
*
* 7. action_animate emits a finished(Action *) signal which is connected to
* changeStats(Action *), changeStats(Action *) alters each Person's stats 
* based off its parameter Action.  UpdateScene() is then called.
*
* 8. updateScene() emits a finished() signal which is connected to 
* actionOutcome(). ActionOutcome() checks for deaths and animates 
* appropriately, if a whole party is eliminated, then all the memory
* is cleared, and battleWon() or battleLost() is called.  If the stack is not
* empty, (Step 6) else reset party_index and (Step 1).
*   
* Note: Animation has changed since this design and requires some alteration in terms of these steps
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

#include <QtGui/QWidget>
#include "BattleInfoBar.h"
#include "BattleStatusBar.h"
#include "BattleMenu.h"
#include "Party.h"
#include "Weather.h"
#include "Action.h"

class Battle: public QWidget

{
  public:

    Battle(QWidget *pointer = 0);
    ~Battle();

  private:

    /* Pointer to the battle info bar */
    BattleInfoBar* current_battle_info_bar; 

    /* Person 1's Status bar */
    BattleStatusBar* person1_status_bar; 

    /* Person 2's Status bar */
    BattleStatusBar* person2_status_bar; 

    /* Person 3's Status bar */
    BattleStatusBar* person3_status_bar; 

    /* Person 4's Status Bar */
    BattleStatusBar* person4_status_bar;

    /* Person 5's Status Bar */
    BattleStatusBar* person5_status_bar; 

    /* The Battle menu pointer (for selecting actions), off by default */
    BattleMenu* current_battle_menu;

    /* Checks if targeting mode is active */
    bool target_mode; //Checks if targeting mode is active

    /* Which party is currently selected */
    int party_index;

    /* Current target selected */
    int target_index; 

    /* Current turn count */
    int turn_count; 

    /* The enemy party */
    Party* foes; 

    /* Allied party */
    Party* friends;

    /* Backdrop for the battle */
    QImage* battlebg; 

    /* Enemy 1's sprite bounding box */
    QRect* enemy1_bound; 

    /* Enemy 2's sprite bounding box */
    QRect* enemy2_bound; 

    /* Enemy 3's sprite bounding box */
    QRect* enemy3_bound; 
 
    /* Enemy 4's sprite bounding box */
    QRect* enemy4_bound; 

    /* Enemy 5's sprite bounding box */
    QRect* enemy5_bound; 

    /* Ally 1's sprite bounding box */
    QRect* ally1_bound; 

    /* Ally 2's sprite bounding box */
    QRect* ally2_bound; 

    /* Ally 3's sprite bounding box */
    QRect* ally3_bound; 

    /* Ally 4's sprite bounding box */
    QRect* ally4_bound; 

    /* Ally 5's sprite bounding box */
    QRect* ally5_bound; 

    /* The targeting box for action/inventory target selection */
    QRect * target_box; 

    /* The action buffer */
    QVector<Action *> action_buffer; 

    /* The inventory use buffer */
    QVector<Action *> inventory_buffer; 

    /* Weather condition during battle */
    Weather* weather_conditions; 

    /* Flag for whether this is a boss battle (special code if it is)*/
    bool BOSS; 
   

  protected:


  signals:


  public slots:


  public:

    /* Checks for deaths, pops current action off stack, calls performAction();*/
    void actionOutcome(); 

    /* Ends battle with win message */
    void battleWon(); 

    /* Ends Battle with loss message, and reverts game to last save point */
    void battleLost(); 

    /* Arranges and paints static image */
    void buildScene(); 

    /* Sets temporary stats */
    void buildStats(); 

    /* Alters stats, updates scene */
    void changeStats(Action *); 

    /* Non-character events */
    void generalUpkeep(); 

    /* Reorders stack (speed based)*/
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

    /* Sets the targeting mode (slot) */
    void setTargetMode(bool);

};

#endif
