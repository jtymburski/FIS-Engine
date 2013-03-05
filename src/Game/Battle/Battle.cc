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

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

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

  for (uint i = 0; i < p_friends->getPartySize(); i++)
      p_friends->getMember(i)->setTemporaryStats();
  for (uint i = 0; i < p_foes->getPartySize(); i++)
      p_foes->getMember(i)->setTemporaryStats();

  /* Basic settings for battle window sizing and backdrops */
  setMaxWidth(1216);  // TODO: Obtain from options [02-24-13]
  setMaxHeight(704);  // TODO: Obtain from options [02-24-13]
  setFixedSize(getMaxWidth(), getMaxHeight());
  battle_bg = new QPixmap();
  battle_bg->load(":/bbd_sewers");
  battle_status_bar_image = new QPixmap();
  battle_status_bar_image->load(":/statusbar");

  /* Create and place enemy & ally bounding boxes */
  uint left_d   = floor(0.1290 * getMaxHeight());
  uint top_d    = floor(0.1464 * getMaxHeight());
  uint enemy_w  = floor(0.2105 * getMaxWidth());
  uint enemy_h  = enemy_w;
  uint spacing  = floor(0.0226 * getMaxWidth());
  uint atop_d   = floor(0.5200 * getMaxHeight());
  uint ally_w   = floor(0.2100 * getMaxWidth());
  uint ally_h   = floor(0.3636 * getMaxHeight());
  uint aspacing = floor(0.0263 * getMaxWidth());

  for (uint i = 0; i < kMAX_PARTY_SIZE; i++)
  {
    uint left_margin = left_d + (enemy_w * i) - spacing * i;
    enemy_box.push_back(new QRect(left_margin,top_d, enemy_w, enemy_h));
    left_margin = (ally_w * i) - aspacing * i;
    ally_box.push_back(new QRect(left_margin,atop_d,ally_w,ally_h));
  }

  /* Battle status bar setup */
  uint bar_width  = (ally_w * 5) - aspacing * 4;
  uint bar_height = getMaxHeight() * 0.1200;
  status_box = new QRect(0, getMaxHeight() - bar_height, bar_width, bar_height);
  status_bar = new BattleStatusBar(friends, bar_width, bar_height, this);
  status_bar->setGeometry(*status_box);

  /* Battle extra bar set up */
  bar_width  = 1 - (getMaxWidth() * bar_width);
  extra_box = new QRect(0, getMaxHeight() - bar_height, bar_width, bar_height);
  // TODO: Create an extra bar [02-24-13]

  /* Battle info bar set up */
  bar_width  = getMaxWidth()   * 1.0000;
  bar_height = getMaxHeight()  * 0.0500;
  info_box = new QRect(0, 0, bar_width, bar_height);
  info_bar = new BattleInfoBar();

  paintAll();
}

/*
 * Description: Annihilates a battle object
 */
Battle::~Battle()
{
  setFriends();
  setFoes();
}

/*============================================================================
 * PAINT EVENTS
 *===========================================================================*/

/*
 * Description: Paint event for the battle class
 */
void Battle::paintEvent(QPaintEvent*)
{
  /* Preparation */
  QPainter painter(this);
  painter.setPen(QColor(Qt::black));
  painter.setBrush(QColor(Qt::black));
  painter.setOpacity(1.00);

  /* Paint the current backdrop */
  painter.drawPixmap(0,0,getMaxWidth(),getMaxHeight(),*battle_bg);

  /* Draw sprites for allies and foes (if they exist) */
  painter.setOpacity(1.0);
  for (uint i = 0; i < kMAX_PARTY_SIZE; i++)
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
  painter.drawPixmap(0,getMaxHeight() * 0.8181,*battle_status_bar_image);


  /* Paint drawings for info,status,extra bars */
  painter.setOpacity(0.70); //TODO: Get opacity form somewhere [02-23-13]
  painter.drawRect(*info_box);

  /* Temp painting of status bar bounding box */
  painter.setOpacity(0.70);
}

/*
 * Description: Function which calls all the normally displayed elements
 */
void Battle::paintAll()
{
  /* Calls all objects paint events */
  update();

  /* Paints stats bar */
  status_bar->update();


  info_bar->update();
}

/*
 * Description: Paint event for the menu
 */
void Battle::paintMenu()
{

}

/*==============================================================================
 * PAINT EVENTS
 *============================================================================*/

/* TODO: Temporary change of battle background [02-20-13] */
void Battle::keyPressEvent(QKeyEvent* event)
{
  switch(event->key())
  {
    case Qt::Key_Escape:
      closeBattle();
      break;
    /* Temporary swapping for background image testing */
    case Qt::Key_1:
      battle_bg->load(":/bbd_sewers");
      break;
    case Qt::Key_2:
      battle_bg->load(":/bbd_sewers2");
      break;
    case Qt::Key_3:
      battle_bg->load(":/bbd_sewers3");
      break;
    case Qt::Key_4:
      battle_bg->load(":/bbd_sewers4");
      break;
    case Qt::Key_F1:
      friends->getMember(0)->setVitality(-5);
      break;
    case Qt::Key_F2:
      friends->getMember(1)->setVitality(-5);
      break;
    default:
      break;
  }
  update();
  paintAll();
  status_bar->update();
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

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

void Battle::closeBattle()
{
  emit closingBattle(2);
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
