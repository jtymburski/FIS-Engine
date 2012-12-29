/******************************************************************************
* Class Name: Battle Implementation
* Date Created: December 2nd, 2012
* Inheritance:
* Description: 
*
*  TODO [12-28-12]: Write algorithm to determine enemy placement
*  TODO [12-28-12]: Pass in Parties to determine if enemies/allies exist first
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
* changeStats(Action *), changeStats(Action *) alters each Person's stats 
* based off its parameter Action.  UpdateScene() is then called.
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

#include "Game/Battle/Battle.h"

/* Creates a battle object */
Battle::Battle(QWidget* pointer)
{
   int max_width = 1216;
   int max_height = 704;

   setFixedSize(1216,704);
   battle_bg = new QPixmap();
   battle_bg->load(":/temp_bg.png");

   /* Temporary placement of enemy bounding boxes
    * Should be determined with algorithm as per
    * Jordan (also enemies should include
    * only if they exist)
    */

   enemy1_bound = new QRect(100,170,180,230);
   enemy2_bound = new QRect(300,165,180,230);
   enemy3_bound = new QRect(500,160,180,230);
   enemy4_bound = new QRect(700,155,180,230);
   enemy5_bound = new QRect(900,150,180,230);

   int top_d  = floor(0.7 * max_height);
   int ally_w = floor(0.12 * max_width);
   int ally_h = floor(0.3 * max_height);

   /* Ally bounding boxes */
   ally1_bound = new QRect(0,top_d,ally_w,ally_h);
   ally2_bound = new QRect(ally_w,top_d,ally_w,ally_h);
   ally3_bound = new QRect(ally_w * 2,top_d,ally_w,ally_h);
   ally4_bound = new QRect(ally_w * 3,top_d,ally_w,ally_h);
   ally5_bound = new QRect(ally_w * 4,top_d,ally_w,ally_h);


   /* Call paint event */
   update();
   show();
}


/* Annihilates a battle object */
Battle::~Battle()
{
}

void Battle::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setPen(QColor(Qt::black));

  int max_width = 1216;
  int max_height = 704;

  painter.drawPixmap(0,0,1216,704,*battle_bg);

  /* Temp paint drawings for battleinfo bar */
  int info_height = 0.05 * max_height;
  painter.drawRect(0,0,max_width,info_height);

  /* Temp paint for battle menu */
  int menu_height = floor(0.21 * max_height);
  int menu_width = max_width - floor(5 * 0.12 * max_width);
  painter.drawRect(729, 704-menu_height, menu_width, menu_height);

  /* Temp paint for battle status bars */
  menu_height = floor(0.18 * max_height);
  painter.drawRect(729, 540-menu_height, menu_width, menu_height);

  /* Temp paint drawings for allies/foes */
  painter.drawRect(*ally1_bound);
  painter.drawRect(*ally2_bound);
  painter.drawRect(*ally3_bound);
  painter.drawRect(*ally4_bound);
  painter.drawRect(*ally5_bound);
  painter.drawRect(*enemy1_bound);
  painter.drawRect(*enemy2_bound);
  painter.drawRect(*enemy3_bound);
  painter.drawRect(*enemy4_bound);
  painter.drawRect(*enemy5_bound);

  // if (foes->getMember(0))
  // painter.drawPixmap(enemy1_bound,foes->getMember(0)->getThirdPerson());
  // if (foes->getMember(1))
  // painter.drawPixmap(enemy2_bound,foes->getMember(1)->getThirdPerson());
  // if (foes->getMember(2))
  // painter.drawPixmap(enemy3_bound,foes->getMember(2)->getThirdPerson());
  // if (foes->getMember(3))
  // painter.drawPixmap(enemy4_bound,foes->getMember(3)->getThirdPerson());
  // if (foes->getMember(4))
  // painter.drawPixmap(enemy5_bound,foes->getMember(4)->getThirdPerson());
  // if (friends->getMember(0))
  // painter.drawPixmap(ally1_bound,foes->getMember(0)->getThirdPerson());
  // if (friends->getMember(1))
  // painter.drawPixmap(ally2_bound,foes->getMember(1)->getThirdPerson());
  // if (friends->getMember(2))
  // painter.drawPixmap(ally3_bound,foes->getMember(2)->getThirdPerson());
  // if (friends->getMember(3))
  // painter.drawPixmap(ally4_bound,foes->getMember(3)->getThirdPerson());
  // if (friends->getMember(4))
  // painter.drawPixmap(ally5_bound,foes->getMember(4)->getThirdPerson());
}

/* Checks for deaths, pops current action off stack, calls performAction();*/
void Battle::actionOutcome()
{
}

/* Ends battle with win message */
void Battle::battleWon()
{
}

/* Ends Battle with loss message, and reverts game to last save point */
void Battle::battleLost()
{
}

/* Arranges and paints static image */
void Battle::buildScene()
{
}

/* Sets temporary stats */
void Battle::buildStats()
{
}

/* Alters stats, updates scene */
void Battle::changeStats(Action* battle_action)
{
}

/* Non-character events */
void Battle::generalUpkeep()
{
}

/* Reorders stack (speed based)*/
void Battle::orderActions()
{
}

/* Performs current action animation */
void Battle::performAction()
{
}

/* Character events */
void Battle::personalUpkeep()
{
}

/* Increment party index for next Battle Menu */
void Battle::processAction()
{
}

/* Gets all enemy actions, adds to stack */
void Battle::processEnemyActions()
{
}

/* Paints dynamic images */
void Battle::updateScene()
{
}

/* Sets the targeting mode (slot) */
void Battle::setTargetMode(bool targeting)
{
   target_mode = targeting;
}
