/*******************************************************************************
 * Class Name: BattleDisplay [Implementation]
 * Date Created: December 16, 2014
 * Inheritance: None
 * Description: 
 *
 * Notes
 * -----
 *
 * [1]:
 *
 * See .h file for TODOs
 ******************************************************************************/
#include "Game/Battle/BattleDisplay.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t BattleDisplay::kALLY_HEALTH_H = 18;
const uint8_t BattleDisplay::kALLY_HEALTH_TRIANGLE = 9;
const uint8_t BattleDisplay::kALLY_HEALTH_W = 132;
const uint16_t BattleDisplay::kALLY_HEIGHT = 70;
const uint8_t BattleDisplay::kALLY_QD_H = 12;
const uint8_t BattleDisplay::kALLY_QD_OFFSET = 3;
const uint8_t BattleDisplay::kALLY_QD_TRIANGLE = 6;
const uint8_t BattleDisplay::kALLY_QD_W = 94;
const uint16_t BattleDisplay::kBIGBAR_OFFSET = 88;
const uint8_t BattleDisplay::kCOLOR_BASE = 150;
const uint8_t BattleDisplay::kINFO_BORDER = 2;
const uint8_t BattleDisplay::kINFO_GREY = 200;
const uint16_t BattleDisplay::kINFO_H = 50;
const uint8_t BattleDisplay::kINFO_OPACITY = 166;
const uint8_t BattleDisplay::kINFO_TRIANGLE = 6;
const uint16_t BattleDisplay::kINFO_W = 180;
const uint8_t BattleDisplay::kFOE_BAR_H = 8;
const uint8_t BattleDisplay::kFOE_BAR_OFFSET = 2;
const uint8_t BattleDisplay::kFOE_BAR_TRIANGLE = 5;
const uint16_t BattleDisplay::kFOE_BAR_W = 126;
const uint16_t BattleDisplay::kFOES_BAR_GAP = 12;
const uint16_t BattleDisplay::kFOES_OFFSET = 94;
const uint16_t BattleDisplay::kFRIENDS_OFFSET = 328;
const uint8_t BattleDisplay::kMAX_CHARS = 5;
const uint8_t BattleDisplay::kMAX_LAYERS = 10;
const uint16_t BattleDisplay::kPERSON_SPREAD = 200;
const uint16_t BattleDisplay::kPERSON_WIDTH = 256;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
// TODO: Comment
BattleDisplay::BattleDisplay(Options* running_config)
{
  /* Blank out variables */
  background = NULL;
  bar_offset = 0;
  battle = NULL;
  battle_bar = NULL;
  foes_backdrop = NULL;
  font_header = NULL;
  font_subheader = NULL;
  offset = 0; // TODO: Delete
  offset_2 = 0; // TODO: Delete
  system_options = NULL;

  /* Set up variables */
  setConfiguration(running_config);
}

/*
 * Description: Destructor function
 */
BattleDisplay::~BattleDisplay()
{
  deleteFonts();
  clear();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

// TODO: Comment
void BattleDisplay::createFoeBackdrop(SDL_Renderer* renderer)
{
  /* Sizing variables */
  uint16_t width = kINFO_W;
  uint16_t height = kINFO_H;
  uint8_t black_opacity = kINFO_OPACITY;
  uint8_t triangle_width = kINFO_TRIANGLE;
  uint8_t border = kINFO_BORDER;
  uint8_t grey_color = kINFO_GREY;

  /* Create rendering texture */
  Frame* rendered_frame = new Frame();
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, 
                                           width, height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  /* Draw center rectangle */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, black_opacity);
  SDL_Rect rect;
  rect.x = border + triangle_width - 1;
  rect.y = border - 1;
  rect.w = width - triangle_width * 2 - border;
  rect.h = height - border;
  SDL_RenderFillRect(renderer, &rect);

  /* Draw top right triangle */
  uint16_t x_top = rect.x + rect.w;
  uint16_t y_top = rect.y;
  Frame::renderTriangle(x_top, y_top, x_top, y_top + triangle_width, 
                        x_top + triangle_width, y_top + triangle_width, 
                        renderer);

  /* Draw right rectangle */
  SDL_Rect rect2;
  rect2.x = rect.x + rect.w;
  rect2.y = y_top + triangle_width + 1;
  rect2.w = triangle_width;
  rect2.h = rect.h - triangle_width * 2 - border;
  SDL_RenderFillRect(renderer, &rect2);

  /* Draw right bottom triangle */
  y_top = rect2.y + rect2.h;
  Frame::renderTriangle(x_top, y_top, x_top + triangle_width, y_top, x_top, 
                        y_top + triangle_width, renderer);

  /* Draw left top triangle */
  x_top = rect.x;
  y_top = rect.y;
  Frame::renderTriangle(x_top, y_top, x_top, y_top + triangle_width, 
                        x_top - triangle_width, y_top + triangle_width, 
                        renderer);

  /* Draw left rectangle */
  rect2.x = x_top - triangle_width;
  SDL_RenderFillRect(renderer, &rect2);

  /* Draw left bottom triangle */
  y_top = rect2.y + rect2.h;
  Frame::renderTriangle(x_top, y_top, x_top - triangle_width, y_top, x_top, 
                        y_top + triangle_width, renderer);

  /* First single pixel border */
  SDL_SetRenderDrawColor(renderer, grey_color, grey_color, grey_color, 255);
  SDL_Point points[9];
  points[0].x = rect.x;
  points[0].y = rect.y;
  points[1].x = points[0].x + rect.w - 1;
  points[1].y = points[0].y;
  points[2].x = points[1].x + triangle_width;
  points[2].y = points[1].y + triangle_width;
  points[3].x = points[2].x;
  points[3].y = points[2].y + rect2.h + 1;
  points[4].x = points[3].x - triangle_width;
  points[4].y = points[3].y + triangle_width;
  points[5].x = points[4].x - rect.w + 1;
  points[5].y = points[4].y;
  points[6].x = points[5].x - triangle_width;
  points[6].y = points[5].y - triangle_width;
  points[7].x = points[6].x;
  points[7].y = points[6].y - rect2.h - 1;
  points[8].x = points[7].x + triangle_width;
  points[8].y = points[7].y - triangle_width;
  SDL_RenderDrawLines(renderer, points, 9);

  /* Second single pixel border */
  points[0].y -= 1;
  points[1].y -= 1;
  points[2].x += 1;
  points[3].x += 1;
  points[4].y += 1;
  points[5].y += 1;
  points[6].x -= 1;
  points[7].x -= 1;
  points[8].y -= 1;
  SDL_RenderDrawLines(renderer, points, 9);

  /* Finalize the frame */
  SDL_SetRenderTarget(renderer, NULL);
  rendered_frame->setTexture(texture);
  foes_backdrop = rendered_frame;
}

// TODO: Comment
bool BattleDisplay::createFoeInfo(Person* foe, uint8_t index, 
                                  SDL_Renderer* renderer)
{
  /* Sizing variables */
  uint16_t bar_width = kFOE_BAR_W;
  uint8_t bar_triangle_width = kFOE_BAR_TRIANGLE;
  uint8_t bar_height = kFOE_BAR_H;
  uint8_t bar_offset = kFOE_BAR_OFFSET;

  /* Only can be added if within proper battle range */
  if(index < kMAX_CHARS)
  {
    /* Wipe out the existing data, if rendered */
    if(foes_info.size() > index && foes_info[index] != NULL)
    {
      delete foes_info[index];
      foes_info[index] = NULL;
    }

    /* Create rendering texture */
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET, 
                                             kINFO_W, kINFO_H);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* Render the health bar border */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Point border[5];
    border[0].x = (kINFO_W - bar_width) / 2;
    border[0].y = (kINFO_H - bar_height) / 2 + bar_offset;
    border[1].x = border[0].x + bar_width + bar_triangle_width;
    border[1].y = border[0].y;
    border[2].x = border[1].x - bar_triangle_width;
    border[2].y = border[1].y + bar_height;
    border[3].x = border[2].x - bar_width - bar_triangle_width;
    border[3].y = border[2].y;
    border[4].x = border[0].x;
    border[4].y = border[0].y;
    SDL_RenderDrawLines(renderer, border, 5);

    /* Render the enemy name */
    SDL_Color color = {255, 255, 255, 255};
    Text* t = new Text(font_header);
    t->setText(renderer, foe->getName(), color);
    t->render(renderer, (kINFO_W - t->getWidth()) / 2, 
                        (border[0].y - t->getHeight()) / 2);
    delete t;

    /* Render the enemy level */
    t = new Text(font_subheader);
    t->setText(renderer, "Level " + std::to_string(foe->getLevel()), color);
    t->render(renderer, (kINFO_W - t->getWidth()) / 2, 
              border[0].y + bar_height + 
              (kINFO_H - border[0].y - bar_height - t->getHeight()) / 2);
    delete t;

    /* Set the new frame */
    Frame* null_frame = NULL;
    while(foes_info.size() <= index)
      foes_info.push_back(null_frame);
    foes_info[index] = new Frame();
    foes_info[index]->setTexture(texture);

    /* Clear render connection */
    SDL_SetRenderTarget(renderer, NULL);

    return true;
  }
  return false;
}

/*
 * Description: Attempts to create the main font and the title font that is used
 *              throughout dialog representation. This is acquired from the
 *              configuration of the application. If successful, set the new
 *              fonts.
 *
 * Inputs: none
 * Output: bool - status if creation was successful
 */
bool BattleDisplay::createFonts()
{
  bool success = false;

  if(system_options != NULL)
  {
    /* Try and create the new fonts */
    TTF_Font* subheader_font = Text::createFont(system_options->getBasePath() + 
                                                system_options->getFont(), 13,
                                                TTF_STYLE_BOLD);
    TTF_Font* header_font = Text::createFont(system_options->getBasePath() + 
                                             system_options->getFont(), 
                                             16, TTF_STYLE_BOLD);

    /* If successful, insert the new fonts. Otherwise, delete if any were
     * created */
    if(subheader_font != NULL && header_font != NULL)
    {
      deleteFonts();
      font_header = header_font;
      font_subheader = subheader_font;
      success = true;
    }
    else
    {
      TTF_CloseFont(header_font);
      header_font = NULL;

      TTF_CloseFont(subheader_font);
      subheader_font = NULL;
    }
  }

  return success;
}

/* Generates info for friends in battle */
// TODO: Comment
bool BattleDisplay::createFriendInfo(Person* ally, uint8_t index, 
                                     SDL_Renderer* renderer)
{
  /* Sizing variables */
  uint8_t health_height = kALLY_HEALTH_H;
  uint8_t health_triangle = kALLY_HEALTH_TRIANGLE;
  uint8_t health_width = kALLY_HEALTH_W;
  uint16_t height = kALLY_HEIGHT;
  uint8_t qd_height = kALLY_QD_H;
  uint8_t qd_offset = kALLY_QD_OFFSET;
  uint8_t qd_triangle = kALLY_QD_TRIANGLE;
  uint8_t qd_width = kALLY_QD_W;
  uint16_t width = kINFO_W;

  /* Only can be added if within proper battle range */
  if(index < kMAX_CHARS)
  {
    /* Wipe out the existing data, if rendered */
    if(friends_info.size() > index && friends_info[index] != NULL)
    {
      delete friends_info[index];
      friends_info[index] = NULL;
    }

    /* Create rendering texture */
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET, 
                                             width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    /* Render background of health Bar */
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//    uint8_t qd_x = border[0].x + health_width - qd_offset - 
//                   qd_triangle - qd_width;
//    uint8_t qd_y = border[0].y + health_height - (qd_height / 2);
//    Frame::renderBar(qd_x, qd_y, qd_width + qd_triangle, qd_height, 
//                    (float)qd_triangle / qd_height, renderer);
    
    /* Render the health bar border */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Point border[5];
    border[0].x = (width - health_width) / 2;
    border[0].y = (height - health_height) / 2;
    border[1].x = border[0].x + health_width + health_triangle;
    border[1].y = border[0].y;
    border[2].x = border[1].x - health_triangle;
    border[2].y = border[1].y + health_height;
    border[3].x = border[2].x - health_width - health_triangle;
    border[3].y = border[2].y;
    border[4].x = border[0].x;
    border[4].y = border[0].y;
    SDL_RenderDrawLines(renderer, border, 5);

    /* Render background of QD Bar */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    uint8_t qd_x = border[0].x + health_width - qd_offset - 
                   qd_triangle - qd_width;
    uint8_t qd_y = border[0].y + health_height - (qd_height / 2);
    Frame::renderBar(qd_x, qd_y, qd_width + qd_triangle, qd_height, 
                    (float)qd_triangle / qd_height, renderer);

    /* Render the ally name */
    SDL_Color color = {255, 255, 255, 255};
    Text* t = new Text(font_header);
    t->setText(renderer, ally->getName(), color);
    t->render(renderer, (width - t->getWidth()) / 2, 
              (border[0].y - t->getHeight()) / 2);
    delete t;

    /* Render the ally level */
    t = new Text(font_subheader);
    t->setText(renderer, "Level " + std::to_string(ally->getLevel()), color);
    t->render(renderer, (width - t->getWidth()) / 2, 
              border[0].y + health_height + qd_height / 2 + 
              (height - border[0].y - health_height - qd_height / 2 - 
               t->getHeight()) / 2);
    delete t;

    /* Set the new frame */
    Frame* null_frame = NULL;
    while(friends_info.size() <= index)
      friends_info.push_back(null_frame);
    friends_info[index] = new Frame();
    friends_info[index]->setTexture(texture);

    /* Clear render connection */
    SDL_SetRenderTarget(renderer, NULL);

    return true;
  }
  return false;
}

// TODO: Comment
void BattleDisplay::deleteBattleInfo()
{
  /* Foes bar info */
  for(uint16_t i = 0; i < foes_info.size(); i++)
    if(foes_info[i] != NULL)
      delete foes_info[i];
  foes_info.clear();

  /* Friends bar info */
  for(uint16_t i = 0; i < friends_info.size(); i++)
    if(friends_info[i] != NULL)
      delete friends_info[i];
  friends_info.clear();
}

/*
 * Description: Delete the rendering fonts stored in the class. Do not try and
 *              render again since it will try to use the NULL fonts and fail.
 *
 * Inputs: none
 * Ouptut: none
 */
void BattleDisplay::deleteFonts()
{
  TTF_CloseFont(font_header);
  font_header = NULL;

  TTF_CloseFont(font_subheader);
  font_subheader = NULL;
}

// TODO: Comment
std::vector<Person*> BattleDisplay::getFoes()
{
  /* Check to see if the friends are set */
  if(battle->getFoes() != NULL)
    return battle->getFoes()->getMembers();

  /* Otherwise, return empty stack */
  std::vector<Person*> empty_stack;
  return empty_stack;
}

// TODO: Comment
std::vector<Frame*> BattleDisplay::getFoesFrames()
{
  std::vector<Person*> list = getFoes();

  /* Get the third person frames */
  std::vector<Frame*> frame_list;
  for(uint8_t i = 0; i < list.size(); i++)
  {
    if(list[i] != NULL)
      frame_list.push_back(list[i]->getThirdPerson());
    else
      frame_list.push_back(NULL);
  }

  return frame_list;
}

/* Get foes in battle */
// TODO: Comment
Party* BattleDisplay::getFoesParty()
{
  return battle->getFoes();
}

// TODO: Comment
std::vector<Person*> BattleDisplay::getFriends()
{
  /* Check to see if the friends are set */
  if(battle->getFriends() != NULL)
    return battle->getFriends()->getMembers();

  /* Otherwise, return empty stack */
  std::vector<Person*> empty_stack;
  return empty_stack;
}
  
// TODO: Comment
std::vector<Frame*> BattleDisplay::getFriendsFrames()
{
  std::vector<Person*> list = getFriends();

  /* Get the first person frames */
  std::vector<Frame*> frame_list;
  for(uint8_t i = 0; i < list.size(); i++)
  {
    if(list[i] != NULL)
      frame_list.push_back(list[i]->getFirstPerson());
    else
      frame_list.push_back(NULL);
  }

  return frame_list;
}

/* Get friends in battle */
// TODO: Comment
Party* BattleDisplay::getFriendsParty()
{
  return battle->getFriends();
}
  
/* Renders the battle bar */
// TODO: Comment
bool BattleDisplay::renderBar(SDL_Renderer* renderer, uint16_t screen_width, 
                                                      uint16_t screen_height)
{
  bool success = true;

  if(battle_bar != NULL)
  {
    success &= battle_bar->render(renderer, 0, 
                                  screen_height - kBIGBAR_OFFSET - bar_offset,
                                  screen_width);

    // TODO: Implementation - the actual battle information

    return success;
  }

  return false;
}

/* Renders the foes */
// TODO: Comment
bool BattleDisplay::renderFoes(SDL_Renderer* renderer, uint16_t screen_width)
{
  /* Get frames */
  std::vector<Frame*> frame_list = getFoesFrames();
  bool success = true;

  /* Render the foes */
  uint8_t index = 0;
  for(uint8_t i = 0; i < frame_list.size(); i++)
  {
    if(frame_list[i] != NULL)
    {
      /* Modify index for 1 and 2 flip around */
      if(i == 0)
        index = 1;
      else if(i == 1)
        index = 0;
      else
        index = i;

      /* Render the frame */
      success &= frame_list[i]->render(renderer, screen_width - 
                        kPERSON_WIDTH - (index) * kPERSON_SPREAD, kFOES_OFFSET);
    }
  }
  
  return success;
}

// TODO: Comment
bool BattleDisplay::renderFoesInfo(SDL_Renderer* renderer, 
                                   uint16_t screen_width)
{
  std::vector<Person*> foes_list = getFoes();
  bool success = true;

  /* Render the box */
  uint16_t y = kFOES_OFFSET - kFOES_BAR_GAP - foes_backdrop->getHeight();
  uint8_t index = 0;
  for(uint8_t i = 0; i < foes_list.size(); i++)
  {
    if(foes_list[i] != NULL && foes_info.size() > i && foes_info[i] != NULL)
    {
      /* Modify index for 1 and 2 flip around */
      if(i == 0)
        index = 1;
      else if(i == 1)
        index = 0;
      else
        index = i;
      
      /* Render the frame */
      uint16_t x = screen_width - kPERSON_WIDTH - (index) * kPERSON_SPREAD;
      success &= foes_backdrop->render(renderer, x, y);

      /* Render the health bar */
      float health_percent = foes_list[i]->getVitaPercent() / 100.0;
      if(health_percent >= 0.5)
        SDL_SetRenderDrawColor(renderer, kCOLOR_BASE * ((1-health_percent) * 2), 
                               kCOLOR_BASE, 0, 255);
      else
        SDL_SetRenderDrawColor(renderer, kCOLOR_BASE, 
                               kCOLOR_BASE * health_percent * 2, 0, 255);
      Frame::renderBar(x + (kINFO_W - kFOE_BAR_W) / 2 + 1, 
                       y + (kINFO_H - kFOE_BAR_H) / 2 + kFOE_BAR_OFFSET,
                       (kFOE_BAR_W + kFOE_BAR_TRIANGLE) * health_percent, 
                       kFOE_BAR_H, 
                       (float)kFOE_BAR_TRIANGLE / kFOE_BAR_H, renderer);

      /* Render foe info */
      foes_info[i]->render(renderer, x, y);
    }
  }

  return success;
}

/* Renders the friends */
// TODO: Comment
bool BattleDisplay::renderFriends(SDL_Renderer* renderer, 
                                  uint16_t screen_height)
{
  /* Get frames */
  std::vector<Frame*> frame_list = getFriendsFrames();
  bool success = true;

  /* Render the foes */
  uint8_t index = 0;
  for(uint8_t i = 0; i < frame_list.size(); i++)
  {
    if(frame_list[i] != NULL)
    {
      /* Modify index for 1 and 2 flip around */
      if(i == 0)
        index = 1;
      else if(i == 1)
        index = 0;
      else
        index = i;

      /* Render the frame */
      success &= frame_list[i]->render(renderer, index * kPERSON_SPREAD, 
                                       screen_height - kFRIENDS_OFFSET);
    }
  }
  
  return success;
}

// TODO: Comment
bool BattleDisplay::renderFriendsInfo(SDL_Renderer* renderer, 
                                      uint16_t screen_height)
{
  /* Get people */
  std::vector<Person*> friends_list = getFriends();
  bool success = true;

  /* Render the box */
  uint16_t y = screen_height - kALLY_HEIGHT;
  uint8_t index = 0;
  for(uint8_t i = 0; i < friends_list.size(); i++)
  {
    if(friends_list[i] != NULL && friends_info.size() > i && 
       friends_info[i] != NULL)
    {
      /* Modify index for 1 and 2 flip around */
      if(i == 0)
        index = 1;
      else if(i == 1)
        index = 0;
      else
        index = i;

      /* Render the frame */
      uint16_t x = (index * kPERSON_SPREAD) + (kPERSON_WIDTH - kINFO_W) / 2;

      /* Render the health bar */
      float health_percent = friends_list[i]->getVitaPercent() / 100.0;
      if(health_percent >= 0.5)
        SDL_SetRenderDrawColor(renderer, kCOLOR_BASE * ((1-health_percent) * 2), 
                               kCOLOR_BASE, 0, 255);
      else
        SDL_SetRenderDrawColor(renderer, kCOLOR_BASE, 
                               kCOLOR_BASE * health_percent * 2, 0, 255);
      uint16_t health_x = x + (kINFO_W - kALLY_HEALTH_W) / 2;
      uint16_t health_y = y + (kALLY_HEIGHT - kALLY_HEALTH_H) / 2;
      Frame::renderBar(health_x, health_y, 
                       (kALLY_HEALTH_W + kALLY_HEALTH_TRIANGLE) * 
                        health_percent, kALLY_HEALTH_H, 
                       (float)kALLY_HEALTH_TRIANGLE / kALLY_HEALTH_H, renderer);

      /* Render friends info */
      friends_info[i]->render(renderer, x, y);
    
      /* Render the qd bar */
      float qd_percent = friends_list[i]->getQDPercent() / 100.0;
      SDL_SetRenderDrawColor(renderer, 58, 170, 198, 255);
      uint16_t qd_x = health_x + kALLY_HEALTH_W - kALLY_QD_OFFSET -  
                      kALLY_QD_TRIANGLE - kALLY_QD_W;
      uint16_t qd_y = health_y + kALLY_HEALTH_H - (kALLY_QD_H / 2);
      Frame::renderBar(qd_x, qd_y, 
                      (kALLY_QD_W + kALLY_QD_TRIANGLE) * qd_percent, kALLY_QD_H, 
                      (float)kALLY_QD_TRIANGLE / kALLY_QD_H, renderer);

      /* Render the qd bar border */
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_Point border[5];
      border[0].x = qd_x;
      border[0].y = qd_y;
      border[1].x = border[0].x + kALLY_QD_W + kALLY_QD_TRIANGLE;
      border[1].y = border[0].y;
      border[2].x = border[1].x - kALLY_QD_TRIANGLE;
      border[2].y = border[1].y + kALLY_QD_H;
      border[3].x = border[2].x - kALLY_QD_W - kALLY_QD_TRIANGLE;
      border[3].y = border[2].y;
      border[4].x = border[0].x;
      border[4].y = border[0].y;
      SDL_RenderDrawLines(renderer, border, 5);

      // QTDR

      /* Health Text Amount */
      AttributeSet set = friends_list[i]->getCurr();
      SDL_Color color = {255, 255, 255, 255};
      Text* t = new Text(font_subheader);
      t->setText(renderer, std::to_string(set.getStat("VITA")), color);
      t->render(renderer, health_x + (kALLY_HEALTH_W - t->getWidth()) / 2,  
                health_y);

      /* QD Text Amount */
      t->setText(renderer, std::to_string(set.getStat("QTDR")), color);
      t->render(renderer, qd_x + (kALLY_QD_W - t->getWidth()) / 2, qd_y);
      delete t;
    }
  }

  return success;
}

/* Trims the midlay vector of NULL sprite pointers */
// TODO: Comment
void BattleDisplay::trimMidlays()
{
  std::vector<Sprite*> non_null;

  /* Find all non-null sprites */
  for(uint8_t i = 0; i < midlays.size(); i++)
    if(midlays[i] != NULL)
      non_null.push_back(midlays[i]);

  /* If the vectors are different size, make the new one the non-null version */
  if(non_null.size() < midlays.size())
    midlays = non_null;
}

/* Trims the overlay vector of NULL sprite pointers */
// TODO: Comment
void BattleDisplay::trimOverlays()
{
  std::vector<Sprite*> non_null;

  /* Find all non-null sprites */
  for(uint8_t i = 0; i < overlays.size(); i++)
    if(overlays[i] != NULL)
      non_null.push_back(overlays[i]);

  /* If the vectors are different size, make the new one the non-null version */
  if(non_null.size() < overlays.size())
    overlays = non_null;
}

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
  
/* Adds a midlay sprite - appends to end of stack */
// TODO: Comment
bool BattleDisplay::addMidlay(Sprite* midlay)
{
  if(midlay != NULL && midlay->isFramesSet() && midlays.size() < kMAX_LAYERS)
  {
    midlays.push_back(midlay);
    return true;
  }
  return false;
}

/* Adds an overlay sprite - appends to end of stack */
// TODO: Comment
bool BattleDisplay::addOverlay(Sprite* overlay)
{
  if(overlay != NULL && overlay->isFramesSet() && overlays.size() < kMAX_LAYERS)
  {
    overlays.push_back(overlay);
    return true;
  }
  return false;
}

/* Clears the class data */
// TODO: Comment
void BattleDisplay::clear()
{
  /* Clears variables */
  system_options = NULL;

  /* Unsets the flat rendering sprites */
  unsetBackground();
  unsetBattle();
  unsetBattleBar();
  unsetMidlays();
  unsetOverlays();

  /* Deletes internal pointers, if set */
  if(foes_backdrop != NULL)
    delete foes_backdrop;
  foes_backdrop = NULL;
}

/* Get the background */
// TODO: Comment
Sprite* BattleDisplay::getBackground()
{
  return background;
}
  
/* Gets the battle corresponding to the display */
// TODO: Comment
Battle* BattleDisplay::getBattle()
{
  return battle;
}

/* Get the midlay(s) */
// TODO: Comment
Sprite* BattleDisplay::getMidlay(uint8_t index)
{
  if(midlays.size() > index)
    return midlays[index];

  Sprite* null_sprite = NULL;
  return null_sprite;
}
  
// TODO: Comment
std::vector<Sprite*> BattleDisplay::getMidlays()
{
  return midlays;
}

/* Get the overlay(s) */
// TODO: Comment
Sprite* BattleDisplay::getOverlay(uint8_t index)
{
  if(overlays.size() > index)
    return overlays[index];

  Sprite* null_sprite = NULL;
  return null_sprite;
}
  
// TODO: Comment
std::vector<Sprite*> BattleDisplay::getOverlays()
{
  return overlays;
}

/* Renders the battle display */
// TODO: Comment
bool BattleDisplay::render(SDL_Renderer* renderer)
{
  uint16_t height = 0;
  bool success = true;
  uint16_t width = 0;

  /* Check the options for screen height and width */
  if(system_options != NULL)
  {
    height = system_options->getScreenHeight();
    width = system_options->getScreenWidth();
  }

  if(battle != NULL)
  {
    /* Render background */
    if(background != NULL)
      success &= background->render(renderer, 0, 0, width, height);

    /* Render overlays - over background modifiers */
    for(uint8_t i = 0; i < overlays.size(); i++)
      success &= overlays[i]->render(renderer, 0, 0, width, height);

    /* Render enemies */
    success &= renderFoes(renderer, width);

    /* Render midlays (between enemies and players */
    for(uint8_t i = 0; i < midlays.size(); i++)
    {
      // TODO: Restore
      //success &= midlays[i]->render(renderer, 0, 0, width, height);

      if(i == 0)
      {
        midlays[i]->render(renderer, offset, 0, width, height);
        midlays[i]->render(renderer, offset - width, 0, width, height);
      }
      else if(i == 1)
      {
        midlays[i]->render(renderer, 0, 0, width, height);

        midlays.front()->render(renderer, offset_2, 0, width, height);
        midlays.front()->render(renderer, offset_2 - width, 0, width, height);
      }
    }

    /* Render player team */
    success &= renderFriends(renderer, height);
    
    /* Render enemy information */
    success &= renderFoesInfo(renderer, width);

    /* Render battle bar (on bottom) */
    renderBar(renderer, width, height);

    /* Render friend information */
    success &= renderFriendsInfo(renderer, height);

    // TODO: Render extra battle flair

    return success;
  }

  return false;
}

/* Sets the background sprite */
// TODO: Comment
bool BattleDisplay::setBackground(Sprite* background)
{
  if(background != NULL && background->isFramesSet())
  {
    unsetBackground();
    this->background = background;
    return true;
  }

  return false;
}

/* Sets the battle connected to the display */
// TODO: Comment
bool BattleDisplay::setBattle(Battle* battle, SDL_Renderer* renderer)
{
  bool success = true;

  if(battle != NULL && renderer != NULL && system_options != NULL)
  {
    this->battle = battle;

    /* Generate foes bar if it doesn't exist */
    if(foes_backdrop == NULL)
      createFoeBackdrop(renderer);

    /* Generate foes info */
    std::vector<Person*> foes_list = getFoes();
    for(uint8_t i = 0; i < foes_list.size(); i++)
      if(foes_list[i] != NULL)
        success &= createFoeInfo(foes_list[i], i, renderer);
   
    /* Generate friends info */
    std::vector<Person*> friends_list = getFriends();
    for(uint8_t i = 0; i < friends_list.size(); i++)
      if(friends_list[i] != NULL)
        success &= createFriendInfo(friends_list[i], i, renderer);

    return success;
  }

  return false;
}

/* Sets the rendering battle bar */
// TODO: Comment
bool BattleDisplay::setBattleBar(Frame* bar)
{
  if(bar != NULL && bar->isTextureSet())
  {
    battle_bar = bar;
    return true;
  }
  return false;
}

/* Sets the running configuration, from the options class */
// TODO: Comment
bool BattleDisplay::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    createFonts();

    return true;
  }
  
  return false;
}

/* Sets the midlay sprite - main one, removes all others */
// TODO: Comment
bool BattleDisplay::setMidlay(Sprite* midlay)
{
  if(midlay != NULL && midlay->isFramesSet())
  {
    unsetMidlays();
    midlays.push_back(midlay);
    return true;
  }

  return false;
}

/* Sets the overlay sprite - main one, removes all others */
// TODO: Comment
bool BattleDisplay::setOverlay(Sprite* overlay)
{
  if(overlay != NULL && overlay->isFramesSet())
  {
    unsetOverlays();
    overlays.push_back(overlay);
    return true;
  }

  return false;
}

/* Unsets the background sprite */
// TODO: Comment
void BattleDisplay::unsetBackground()
{
  if(background != NULL)
    delete background;
  background = NULL;
}

/* Unsets the battle connected to the display */
// TODO: Comment
void BattleDisplay::unsetBattle()
{
  /* Clean up of class set information */
  if(battle != NULL)
    deleteBattleInfo();

  battle = NULL;
}
  
/* Unsets the rendering battle bar */
void BattleDisplay::unsetBattleBar()
{
  if(battle_bar != NULL)
    delete battle_bar;
  battle_bar = NULL;

  /* Clear rendering variables */
  bar_offset = 0;
}

/* Unsets the midlay sprite(s) */
// TODO: Comment
void BattleDisplay::unsetMidlay(uint8_t index)
{
  if(midlays.size() > index && midlays[index] != NULL)
  {
    delete midlays[index];
    midlays[index] = NULL;
    trimMidlays();
  }
}

// TODO: Comment
void BattleDisplay::unsetMidlays()
{
  for(uint8_t i = 0; i < midlays.size(); i++)
    unsetMidlay(i);
  midlays.clear();
}

/* Unsets the overlay sprite(s) */
// TODO: Comment
void BattleDisplay::unsetOverlay(uint8_t index)
{
  if(overlays.size() > index && overlays[index] != NULL)
  {
    delete overlays[index];
    overlays[index] = NULL;
    trimOverlays();
  }
}

// TODO: Comment
void BattleDisplay::unsetOverlays()
{
  for(uint8_t i = 0; i < overlays.size(); i++)
    unsetOverlay(i);
  overlays.clear();
}

/* Updates the battle display */
// TODO: Comment
bool BattleDisplay::update(int cycle_time)
{
  /* Only proceed if there is a set battle */
  if(battle != NULL)
  {
    /* Update the background */
    if(background != NULL)
      background->update(cycle_time);

    /* Update the midlays */
    for(uint8_t i = 0; i < midlays.size(); i++)
      midlays[i]->update(cycle_time);

    /* Update the overlays */
    for(uint8_t i = 0; i < overlays.size(); i++)
      overlays[i]->update(cycle_time);
  }

  // TODO: Delete
  if(midlays.size() > 0)
  {
    offset += cycle_time / 8;
    if(offset >= 1216)
      offset -= 1216;
    offset_2 += cycle_time / 4;
    if(offset_2 >= 1216)
      offset_2 -= 1216;
    //midlays.front()->setOpacity(midlays.front()->getOpacity() - 1);
  }

  return true;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/
