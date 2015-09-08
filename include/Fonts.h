/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#ifndef FONTS_H
#define FONTS_H

class Options;

#include "Fonts.h"
#include "Text.h"

#include <map>
#include <vector>

enum class FontName
{
  BATTLE_ACTION,
  BATTLE_DAMAGE,
  BATTLE_HEADER,
  BATTLE_SUBHEADER,
  BATTLE_TURN
};

class Fonts
{
public:
  Fonts(Options* system_options);

  ~Fonts();

private:
  /* The fonts */
  std::map<FontName, TTF_Font*> fonts;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  bool createFonts(Options* config);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  void deleteFonts();

  /* Return the pointer to the font of a given FontName enumerated key */
  TTF_Font* getFont(FontName font_name);
};

#endif // FONTS_H
