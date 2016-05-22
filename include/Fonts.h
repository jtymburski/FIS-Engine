
/*******************************************************************************
* Class Name: Fonts
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
  BATTLE_SMALL,
  BATTLE_SUBHEADER,
  BATTLE_TURN,
  BATTLE_VICTORY_HEADER,
  BATTLE_VICTORY_NAME,
  M_CURVE,
  M_ITEM_HEADER,
  M_ITEM_HEADER_14,
  M_MAIN_TITLE,
  M_TITLE_ELM,
  M_HEADER,
  M_SUBHEADER,
  M_STANDARD,
  M_OPTIONS,
  M_VALUE,
  M_SMALL_TITLE_ELM,
  REGULAR_FONT
};

class Fonts
{
public:
  /* Default fonts constructor */
  Fonts(Options* system_options = nullptr);

  /* Annihilate the fonts object */
  ~Fonts();

private:
  /* The fonts */
  std::map<FontName, TTF_Font*> fonts;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Create fonts, linking to the parent configuration */
  bool createFonts(Options* config);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Destroy the fonts */
  void deleteFonts();

  /* Return the pointer to the font of a given FontName enumerated key */
  TTF_Font* getFont(FontName font_name);
};

#endif // FONTS_H
