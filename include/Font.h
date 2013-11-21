/*******************************************************************************
 * Class Name: Font
 * Date Created: November 20, 2013
 * Inheritance: none
 * Description: This class handles the font and appropriate rendering. It is
 *              used for creating the font and storing it with render box
 *              sizing.
 ******************************************************************************/
#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Font
{
public:
  /* Constructor functions */
  Font();
  Font(std::string font_path, int font_size, int font_style = TTF_STYLE_NORMAL);

  /* Destructor function */
  ~Font();

private:
  /* The height of the texture */
  int height;
  
  /* The font to render the text to */
  TTF_Font* render_font;

  /* The texture that the font is rendered on */
  SDL_Texture* texture;
  
  /* The width of the texture */
  int width;

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Gets the font used for creating textures */
  TTF_Font* getFont();
  
  /* Gets the height of the stored texture */
  int getHeight();
  
  /* Gets the texture that has the rendered text */
  SDL_Texture* getTexture();
  
  /* Gets the width of the stored texture */
  int getWidth();
  
  /* Render the texture to the given renderer with the given parameters */
  bool render(SDL_Renderer* renderer, int x = 0, int y = 0);

  /* Sets up the font, that is used for rendering the text */
  bool setFont(std::string font_path, int font_size, 
                                      int font_style = TTF_STYLE_NORMAL);
  
  /* Sets the text, stored in the internal texture */
  bool setText(SDL_Renderer* renderer, std::string text, SDL_Color text_color);

  /* Clean up functions for the data stored in the class */
  void unsetFont();
  void unsetTexture();
};

#endif // FONT_H
