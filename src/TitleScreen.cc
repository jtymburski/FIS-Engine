/******************************************************************************
* Class Name: TitleScreen
* Date Created: Dec 02 2012
* Inheritance: QWidget
* Description: Is the widget for the main display of the game. Will mostly 
*              just be a menu but this allows for the safe passage between
*              classes as interactions with Application occur.
*
* TODO:
*  1. Set up the options connection -> resolution
*  2. Constants in setupClass() and setupMenu()??
******************************************************************************/
#include "TitleScreen.h"

/* Constant Implementation - see header file for descriptions */
const std::string TitleScreen::kMENU_ITEMS[]   = {"Play Game",
                                                  "Options",
                                                  "Exit"};
const short TitleScreen::kNUM_MENU_ITEMS = 3;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Default constructor */
TitleScreen::TitleScreen()
{
  /* Initial parameter setup */
  cursor_index = 0;
  //font = NULL;
  system_options = NULL;
  
  /* Window Colors Setup */
  //QPalette pal(palette());
  //pal.setColor(QPalette::Background, Qt::black);
  //pal.setColor(QPalette::Foreground, Qt::white);
  //setAutoFillBackground(true);
  //setPalette(pal);

  /* Font setup */
  //QFont current_font = font();
  //current_font.setPixelSize(30);
  //current_font.setWeight(QFont::Bold);
  //setFont(current_font);

  /* Sound setup */
  //background_sound.setSoundFile("sound/ambience/background_menu_sound.wav");
  //background_sound.setPlayForever();
  //background_sound.play();
  //menu_click_sound.setSoundFile("sound/functional/menu_click.wav");
  //menu_click_sound.setPlayCount(1);

  /* Setup the internals of the screen */
  //setupMenu();
}

/* Constructor function */
TitleScreen::TitleScreen(Options* running_config) : TitleScreen()
{
  setConfiguration(running_config);
}

/* Destructor function */
TitleScreen::~TitleScreen()
{
  system_options = NULL;
  
  //for(int i = 0; i < option_labels.size(); i++)
  //  delete option_labels[i];
  //option_labels.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Decrements the selected menu item */
/*void TitleScreen::decrementSelected()
{
  if(cursor_index == 0)
    setSelected(kNUM_MENU_ITEMS - 1);
  else
    setSelected(cursor_index - 1);
}*/
  
/* Choose whether to highlight or not highlight the selected index */
/*bool TitleScreen::highlight(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[index]->palette());
    pal.setColor(QPalette::Background, QColor(28,76,46,64)); // R,G,B,A
    option_labels[index]->setPalette(pal);
    return true;
  }

  return false;
}*/

/* Increments the selected menu item */
/*void TitleScreen::incrementSelected()
{
  if((cursor_index + 1) == kNUM_MENU_ITEMS)
    setSelected(0);
  else
    setSelected(cursor_index + 1);
}*/

/* Sets the selected menu item - fails if out of range */
/*bool TitleScreen::setSelected(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    unhighlight(cursor_index);
    cursor_index = index;
    highlight(cursor_index);

    return true;
  }
  return false;
}*/

/* Set up the menu display text, for painting */
bool TitleScreen::setupMenu(SDL_Renderer* renderer)
{
  bool success = true;
  SDL_Color color={255, 255, 255};
  TTF_Font* font = TTF_OpenFont(system_options->getFont().c_str(), 14);

  /* If the font setup is successful, proceed with creating the textures */
  /*if(font != NULL)
  {
    for(int i = 0; i < kNUM_MENU_ITEMS; i++)
    {
      SDL_Surface* text_surface = 
                    TTF_RenderText_Blended(font, kMENU_ITEMS[i].c_str(), color);
      if(text_surface != NULL)
        option_labels.
    }
    // Create textures here
    
    
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
  
  
  }
  else
  {
    success = false;
  }*/
  
  TTF_CloseFont(font);
  return success;
  
  /*int marginWidth = 10;

  for(int i = 0; i < kNUM_MENU_ITEMS; i++)
  {
    QLabel* new_label = new QLabel(kMENU_ITEMS[i], this);
    int pixelWidth = new_label->fontMetrics().boundingRect(
                                                    new_label->text()).width();

    /* Set the color */
    /*QPalette pal(new_label->palette());
    pal.setColor(QPalette::Foreground, Qt::white);
    pal.setColor(QPalette::Background, QColor(255,0,0,0));
    new_label->setPalette(pal);

    /* Paint the labels */
    /*new_label->setMargin(marginWidth);
    new_label->move((this->width() - pixelWidth) / 2 - marginWidth,
                    350 + i*75);
    new_label->setAutoFillBackground(true);

    option_labels.append(new_label);
  }

  setSelected(TESTMAP);*/
}

/* Un-Highlight the selected index by removing the border */
/*bool TitleScreen::unhighlight(int index)
{
  if(index >= 0 && index < kNUM_MENU_ITEMS)
  {
    QPalette pal(option_labels[index]->palette());
    pal.setColor(QPalette::Background, QColor(255,0,0,0));
    option_labels[index]->setPalette(pal);
    return true;
  }

  return false;
}*/

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*void TitleScreen::paintEvent(QPaintEvent* event)
{
  (void)event;
  QPainter painter(this);

  painter.drawPixmap(0, 0, system_options.getScreenWidth(), 
                           system_options.getScreenHeight(), 
                           QPixmap("pictures/univursatitle.png"));
}*/

//void TitleScreen::keyPressEvent(QKeyEvent* event)
//{
//  if(event->key() == Qt::Key_Up)
//  {
//    decrementSelected();
//    menu_click_sound.play();
//  }
//  else if(event->key() == Qt::Key_Down)
//  {
//    incrementSelected();
//    menu_click_sound.play();
//  }
//  else if(event->key() == Qt::Key_Escape)
//  {
//    setSelected(EXIT);
//    menu_click_sound.play();
/*  }
  else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    background_sound.stop();

    if(cursor_index == TESTMAP)
      emit openMap();
    else if(cursor_index == TESTBATTLE)
      emit openBattle();
    else if(cursor_index == OPTIONS) // TODO
    {
      emit openOptions();
      background_sound.play();
    }
    else if(cursor_index == EXIT)
      emit exit();
  }
}*/

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

//void TitleScreen::playBackground()
//{
//  background_sound.play();
//}

/* Renders the title screen */
bool TitleScreen::render(SDL_Renderer* renderer)
{
  if(renderer != NULL)
  {
    /* Render the background */
    SDL_RenderCopy(renderer, background.getTexture(), NULL, NULL);
    
    // TEST - Render font
    SDL_Color color = {255, 255, 255};
    std::string test = "Hello World, I know you're out there: " + 
                       std::to_string(SDL_GetTicks());
    Font font(system_options->getFont(), 25, TTF_STYLE_BOLD);
    font.setText(renderer, test, color);
    font.render(renderer, 200, 450);
    
    return true;
  }
  
  return false;
}

bool TitleScreen::setBackground(std::string path, SDL_Renderer* renderer)
{
  return background.setTexture(path, renderer);
}

/* Sets the running configuration, from the options class */
bool TitleScreen::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    return true;
  }
  
  return false;
}
