/*******************************************************************************
* Class Name: Fonts [Implementation]
* Date Created: September 7, 2015
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
#include "Fonts.h"
#include "Options.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
*/
Fonts::Fonts(Options* config)
{
  createFonts(config);
}

/*
 * Description:
 */
Fonts::~Fonts()
{
  deleteFonts();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Fonts::createFonts(Options* config)
{
  bool success = false;

  if(config)
  {
    success = true;

    /* Create the fonts */
    fonts.emplace(FontName::BATTLE_ACTION,
                  Text::createFont(config->getBasePath() + config->getFont(1),
                                   60, TTF_STYLE_NORMAL));
    fonts.emplace(FontName::BATTLE_DAMAGE,
                  Text::createFont(config->getBasePath() + config->getFont(),
                                   28, TTF_STYLE_BOLD));
    fonts.emplace(FontName::BATTLE_HEADER,
                  Text::createFont(config->getBasePath() + config->getFont(),
                                   16, TTF_STYLE_BOLD));
    fonts.emplace(FontName::BATTLE_SUBHEADER,
                  Text::createFont(config->getBasePath() + config->getFont(),
                                   13, TTF_STYLE_BOLD));
    fonts.emplace(FontName::BATTLE_TURN,
                  Text::createFont(config->getBasePath() + config->getFont(1),
                                   80, TTF_STYLE_NORMAL));
    fonts.emplace(FontName::REGULAR_FONT,
                  Text::createFont(config->getBasePath() + config->getFont(),
                                   18, TTF_STYLE_NORMAL));

   std::cout << "Built da fonts" << std::endl;
    /* Assert all fonts are not nullptr */
    for(auto& map_font : fonts)
      success &= (map_font.second != nullptr);

    /* If the font creation failed, close all fonts */
    if(!success)
    {
#ifdef UDEBUG
      std::cout << "[ERROR] DELETING FONTS!" << std::endl;
#endif
      deleteFonts();
    }
  }

  return success;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void Fonts::deleteFonts()
{
  for(auto& map_font : fonts)
  {
    TTF_CloseFont(map_font.second);
    map_font.second = nullptr;
  }

  fonts.clear();
}

TTF_Font* Fonts::getFont(FontName font_name)
{
  auto it = fonts.find(font_name);

  if(it != end(fonts))
    return it->second;

  return nullptr;
}

/*=============================================================================
* PUBLIC STATIC FUNCTIONS
*============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/
