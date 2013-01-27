/******************************************************************************
* Class Name: Battle Implementation
* Date Created: December 2nd, 2012
* Inheritance:
* Description: 
*
*  FUTURE [12-28-12]: Write algorithm to determine enemy placement
*  TODO [01-27-13]: Write battle progression steps
*  TODO [01-27-13]: Menu designs, displaying information, etc.
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
#include <QDebug>

/*
 * Description: Constructor for the battle class
 *
 * Inputs: p_friends - pointer to allied party
 *         p_foes - pointer to foes party
 */
Battle::Battle(Party* p_friends, Party* p_foes, QWidget* pointer)
{
  /* Setup for pointers */
  setFriends(p_friends);
  setFoes(p_foes);

  /* Basic settings for battle window sizing and backdrop */
  setMaxWidth(1216);
  setMaxHeight(704);
  setFixedSize(getMaxWidth(), getMaxHeight());
  battle_bg = new QPixmap();
  battle_bg->load(":/bbd_sewers");

  /* Create and place enemy bounding boxes */
  for (int i = 0; i < 5; i++)
    enemy_box.push_back(new QRect(100 + 200 * i,170 - 5 * i, 200, 200));

  /* Create and place ally bounding boxes */
  int top_d  = floor(0.7 * getMaxHeight());
  int ally_w = floor(0.13 * getMaxWidth());
  int ally_h = floor(0.3 * getMaxHeight());

  for (int i = 0; i < 5; i++)
    ally_box.push_back(new QRect((ally_w * i) - 10 * i,top_d,ally_w,ally_h));

  /* Call paint event and show screen */
  update();
  show();
}


/*
 * Description: Annihilates a battle object
 */
Battle::~Battle()
{
  setFriends();
  setFoes();
}

/*
 * Description: Paint event for the battle class
 */
void Battle::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setPen(QColor(Qt::blue));
  painter.setOpacity(1);

  painter.drawPixmap(0,0,getMaxWidth(),getMaxHeight(),*battle_bg);

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

  /* Draw sprites for allies and foes (if they exist) */
  for (int i = 0; i < 5; i++)
  {
    Person* p = NULL;
    if (i < friends->getPartySize() && friends->getMember(i))
    {
      p = friends->getMember(i);
      painter.drawPixmap(*ally_box[i],p->getFirstPerson()->getCurrent());
    }
    if (i < foes->getPartySize() && foes->getMember(i))
    {
      p = foes->getMember(i);
      painter.drawPixmap(*enemy_box[i],p->getThirdPerson()->getCurrent());
    }
  }


}

/*
 * Description: Checks for deaths, pops current action off stack, calls
 *              performAction()
 *
 * Inputs:
 * Output:
 */
void Battle::actionOutcome()
{
}

/*
 * Description: Set the friends pointer
 *
 * Inputs: Party* - party to set friends pointer to
 * Output: none
 */
void Battle::setFriends(Party* p_friends)
{
  friends = p_friends;
}

/*
 * Description: Sets the foes pointer
 *
 * Inputs: Party* - party to set foes pointer to
 * Output: none
 */
void Battle::setFoes(Party* p_foes)
{
  foes = p_foes;
}

/*
 * Description: Ends battle with win message
 *
 * Inputs:
 * Output:
 */
void Battle::battleWon()
{
}

/*
 * Description: Ends battle with loss message, and reverts game to last save
 *              point.
 *
 * Inputs:
 * Output:
 */
void Battle::battleLost()
{
}
/*
 * Description: Arranges and paints static image
 *
 * Inputs:
 * Output:
 */
void Battle::buildScene()
{
}

/*
 * Description: Sets temporary stats
 *
 * Inputs:
 * Output:
 */
void Battle::buildStats()
{
}

/*
 * Description: Alters stats, updates scene
 *
 * Inputs:
 * Output:
 */
void Battle::changeStats(Action* battle_action)
{
}

/*
 * Description: Non-charactr events
 *
 * Inputs:
 * Output:
 */
void Battle::generalUpkeep()
{
}
/*
 * Description: Reorders stack (speed based)
 *
 * Inputs:
 * Output:
 */
void Battle::orderActions()
{
}
/*
 * Description: Performs current action animation
 *
 * Inputs:
 * Output:
 */
void Battle::performAction()
{
}

/*
 * Description: Character events
 *
 * Inputs:
 * Output:
 */
void Battle::personalUpkeep()
{
}

/*
 * Description: Increments party index for the next battle menu
 *
 * Inputs:
 * Output:
 */
void Battle::processAction()
{
}

/*
 * Description: Gets all enemy actions, adds to stack
 *
 * Inputs:
 * Output:
 */
void Battle::processEnemyActions()
{
}

/*
 * Description: Paints dynamic images
 *
 * Inputs:
 * Output:
 */
void Battle::updateScene()
{
}

/*
 * Description: Returns the max_width of the battle window
 *
 * Inputs: none
 * Output: int - max_width
 */
int Battle::getMaxWidth()
{
  return max_width;
}

/*
 * Description: Returns the max_height of the battle window
 *
 * Inputs: none
 * Output: int - max_height
 */
int Battle::getMaxHeight()
{
  return max_height;
}

/*
 * Description: Sets the maximum width of the battle window
 *
 * Inputs: int - new value of width
 * Output: none
 */
void Battle::setMaxWidth(int value)
{
  max_width = value;
}

/*
 * Description: Sets the maximum height of the battle window
 *
 * Inputs: int - new value of height
 * Output: none
 */
void Battle::setMaxHeight(int value)
{
  max_height = value;
}

/*
 * Description: Sets the targeting mode (slot)
 *
 * Inputs: bool -
 * Output: none
 */
void Battle::setTargetMode(bool targeting)
{
  target_mode = targeting;
}
