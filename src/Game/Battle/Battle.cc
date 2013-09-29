/******************************************************************************
* Class Name: Battle Implementation
* Date Created: December 2nd, 2012
* Inheritance:
* Description: 
*
*  FUTURE [12-28-12]: Write algorithm to determine enemy placement
*  TODO [01-27-13]: Write battle progression steps
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

/*=============================================================================
 * CONSTANTS
 *===========================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for the battle class
 *
 * Inputs: friends - pointer to allied party
 *         foes - pointer to foes party
 */
Battle::Battle(Party* friends, Party* foes, QWidget* parent) : QWidget(parent)
{
  /* Pointer setup */
  setFriends(friends);
  setFoes(foes);

  info_bar = NULL;
  status_bar = NULL;
  menu = NULL;
  battle_bg = NULL;
  battle_status_bar_image = NULL;
  target_box = NULL;
  status_box = NULL;
  info_box = NULL;
  extra_box = NULL;
  weather_cond = NULL;

  /* Basic settings for battle window sizing and backdrops */
  setMaxWidth(1216);
  setMaxHeight(704);
  setFixedSize(getMaxWidth(), getMaxHeight());
  battle_bg = new QPixmap();
  battle_bg->load("");
  battle_status_bar_image = new QPixmap();

  /* Set up each friend's and each foe's temporary statistics */
  for (uint i = 0; i < friends->getPartySize(); i++)
    friends->getMember((int)i)->battlePrep();
  for (uint i = 0; i < foes->getPartySize(); i++)
    foes->getMember((int)i)->battlePrep();

  /* Create and place enemy & ally bounding boxes */
  uint left_d   = floor(0.1290 * getMaxHeight());
  uint top_d    = floor(0.1464 * getMaxHeight());
  uint enemy_w  = 256;//floor(0.2105 * getMaxWidth());
  uint enemy_h  = 256;//enemy_w;
  uint spacing  = floor(0.0226 * getMaxWidth());
  uint atop_d   = floor(0.5200 * getMaxHeight());
  uint ally_w   = floor(0.2100 * getMaxWidth());
  uint ally_h   = floor(0.3636 * getMaxHeight());
  uint aspacing = floor(0.0263 * getMaxWidth());

  for (int i = 0; i < 5; i++)
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

  /* Battle info bar set up */
  bar_width  = getMaxWidth()   * 1.0000;
  bar_height = getMaxHeight()  * 0.0500;
  info_box = new QRect(0, 0, bar_width, bar_height);
  info_bar = new BattleInfoBar();

  /* Enemy status bars setup */
  bar_width  = enemy_w        * 0.75000;
  bar_height = getMaxHeight() * 0.07500;
  top_d      = top_d - bar_height;
  for (int i = 0; i < 5; i++)
  {
    uint left_m = left_d + (enemy_w * 1) - spacing * i;
    enemy_status_boxes.push_back(new QRect(left_m,top_d,bar_width,bar_height));
  }

  /* Battle extra bar set up */
  bar_width  = 1 - (getMaxWidth() * bar_width);
  extra_box = new QRect(0, getMaxHeight() - bar_height, bar_width, bar_height);
  paintAll();
}

/*
 * Description: Annihilates a battle object
 */
Battle::~Battle()
{
  if (info_bar != NULL)
  {
    delete info_bar;
    info_bar = NULL;
  }
  if (status_bar != NULL)
  {
    delete status_bar;
    status_bar = NULL;
  }
  qDeleteAll(enemy_status_bar);
  if (menu != NULL)
  {
    delete menu;
    menu = NULL;
  }
  if (battle_bg != NULL)
  {
    delete battle_bg;
    battle_bg = NULL;
  }
  if  (battle_status_bar_image != NULL)
  {
    delete battle_status_bar_image;
    battle_status_bar_image = NULL;
  }
  qDeleteAll(ally_box);
  qDeleteAll(enemy_box);
  qDeleteAll(enemy_status_boxes);
  if (target_box != NULL)
  {
    delete target_box;
    target_box = NULL;
  }
  if (status_box != NULL)
  {
    delete status_box;
    status_box = NULL;
  }
  if (info_box != NULL)
  {
    delete info_box;
    info_box = NULL;
  }
  if (extra_box != NULL)
  {
    delete extra_box;
    extra_box = NULL;
  }
  qDeleteAll(skill_buffer);
  qDeleteAll(items_buffer);
  if (weather_cond != NULL)
  {
    delete weather_cond;
    weather_cond = NULL;
  }
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Paint event for the battle class
 */
void Battle::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setPen(QColor(Qt::black));
  painter.setBrush(QColor(Qt::blue));
  painter.setOpacity(1.00);

  painter.drawPixmap(0,0,getMaxWidth(),getMaxHeight(),*battle_bg);
  painter.setOpacity(1.0);
  Person* p = friends->getMember(0);

  //painter.drawPixmap(*ally_box[0],p->getFirstPerson()->getCurrent());


  p = foes->getMember(0);
  //painter.drawPixmap(*enemy_box[3],p->getThirdPerson()->getCurrent());

  if (friends->getPartySize() > 1)
  {
    p = friends->getMember(1);
    //painter.drawPixmap(*ally_box[0],p->getFirstPerson()->getCurrent());
  }

  if (friends->getPartySize() > 1)
  {
    p = foes->getMember(1);
    //painter.drawPixmap(*enemy_box[4],p->getFirstPerson()->getCurrent());
  }

  for (uint i = 2; i < friends->getPartySize(); i++)
  {
    p = friends->getMember(i);
    //painter.drawPixmap(*ally_box[i],p->getFirstPerson()->getCurrent());
  }

  painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

  p = foes->getMember(1);
  // painter.drawPixmap(*enemy_box[1], p->getThirdPerson()->getCurrent());
  p = foes->getMember(2);
  // painter.drawPixmap(*enemy_box[2],p->getThirdPerson()->getCurrent());

  //painter.drawPixmap(0,getMaxHeight() * 0.8181, *battle_status_bar_image);
  painter.setOpacity(0.70); //TODO: Get opacity from somewhere [02-23-13]
  //painter.drawRect(*info_box);
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
  //status_bar->update();

  //info_bar->update();
}

/*
 * Description: Paint event for the menu
 */
void Battle::paintMenu()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* TODO: Temporary change of battle background [02-20-13] */
void Battle::keyPressEvent(QKeyEvent* event)
{
  switch(event->key())
  {
    case Qt::Key_Escape:
      emit closeBattle();
      break;
    /* Temporary swapping for background image testing */
    case Qt::Key_1:
      battle_bg->load(":/bbd_sewers");
      break;
    default:
      break;
  }
  paintAll();
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
    (void)battle_action;
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
