/*******************************************************************************
 * Class Name: Text
 * Date Created: November 20, 2013
 * Inheritance: none
 * Description: This class handles the font and appropriate rendering. It is
 *              used for creating the font and storing it with render box
 *              sizing.
 ******************************************************************************/
#include "Text.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Sets up the text class but with blank parameters. Font is 
 *              required to be set first before the text can be sent and then
 *              rendering is viable.
 *
 * Inputs: none
 */
Text::Text()
{
  delete_font = false;
  height = 0;
  render_font = NULL;
  texture = NULL;
  width = 0;
}

/*
 * Description: Sets up the text class with an externally handled font. This
 *              allows the texture to be created right away, unless the font
 *              creation failed.
 *
 * Inputs: TTF_Font* font - the font pointer to use as a font
 */
Text::Text(TTF_Font* font) : Text()
{
  setFont(font);
}

/*
 * Description: Sets up the text class with parameters to create the font as
 *              well. This will allow the texture to be created right away, 
 *              unless the font creation failed.
 *
 * Inputs: std::string font_path - the path to the font
 *         int font_size - the DPI size of the font
 *         int font_style - the TTF styles, such as BOLD, ITALIC, etc.
 */
Text::Text(std::string font_path, int font_size, int font_style) : Text()
{
  setFont(font_path, font_size, font_style);
}

/*
 * Description: The deconstructor of the font class. Deletes texture and font.
 */
Text::~Text()
{
  unsetTexture();
  unsetFont();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the stored font. NULL if unset.
 *
 * Inputs: none
 * Output: TTF_Font* - the font reference pointer
 */
TTF_Font* Text::getFont()
{
  return render_font;
}

/*
 * Description: Returns the height of the rendered text that has been created.
 *              0 if the texture is unset.
 *
 * Inputs: none
 * Output: int - the height in pixels
 */
int Text::getHeight()
{
  return height;
}

/*
 * Description: Returns the texture that the class renders. NULL if unset.
 *
 * Inputs: none
 * Output: SDL_Texture* - the texture handle pointer
 */
SDL_Texture* Text::getTexture()
{
  return texture;
}
  
/*
 * Description: Returns the width of the rendered text that has been created.
 *              0 if the texture is unset.
 *
 * Inputs: none
 * Output: int - the width in pixels
 */
int Text::getWidth()
{
  return width;
}

/*
 * Description: Renders the texture, if it's set, to the given x and y
 *              coordinates (the top left corner). It is necessary that the
 *              renderer that the texture is set with is the same as the render
 *              texture.
 *
 * Inputs: SDL_Renderer* renderer - the rendering context
 *         int x - the x coordinate to be rendered to
 *         int y - the y coordinate to be rendered to
 * Output: bool - returns if a render sequence occurred and was successful
 */
bool Text::render(SDL_Renderer* renderer, int x, int y)
{
  if(texture != NULL && renderer != NULL)
  {
    /* Set size parameters */
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
    
    return (SDL_RenderCopy(renderer, texture, NULL, &rect) == 0);
  }
  
  return false;
}

/*
 * Description: Sets the font to use for creating the text texture that will 
 *              inevitably used for rendering. Fails if the font_path isn't
 *              relevant. The font created from this path will be deleted by
 *              this class when it's destroyed or when unsetFont() is called.
 *
 * Inputs: std::string font_path - the path to the font
 *         int font_size - the DPI size of font
 *         int font_style - the TTF styles, such as BOLD, ITALIC, etc.
 *                        - these are from SDL TTF. See documentation
 * Output: bool - returns if the font was created
 */
bool Text::setFont(std::string font_path, int font_size, int font_style)
{
  TTF_Font* new_font = createFont(font_path, font_size, font_style);
  bool success = setFont(new_font);
  
  /* If setting the font is successful, tell the class to delete the font when
   * done */
  if(success)
    delete_font = true;
  
  return success;
}

/*
 * Description: Sets the font pointer to use for rendering text in the class.
 *              This is merely a pointer to an external font so ensure that
 *              this class is destroyed before deleting the font. This class
 *              does not destroy the font and only sets it to NULL when unset.
 *
 * Inputs: TTF_Font* font - the font to set into the class
 * Output: bool - status if the set is successful
 */
bool Text::setFont(TTF_Font* font)
{
  if(font != NULL)
  {
    unsetFont();
    render_font = font;
    delete_font = false;
    return true;
  }
  
  return false;
}
  
/*
 * Description: Sets the text that is stored in the class and will be used for
 *              rendering. It is necessary that first the font is set up before
 *              creating the text.
 *
 * Inputs: SDL_Renderer* renderer - the rendering context
 *         std::string text - the text to create the texture from
 *         SDL_Color text_color - the color of the text
 * Output: bool - returns if the text is created
 */
bool Text::setText(SDL_Renderer* renderer, std::string text, 
                                           SDL_Color text_color)
{
  bool success = false;
  
  if(renderer != NULL && render_font != NULL)
  {
    /* Create the text surface */
    SDL_Surface* text_surface = 
                 TTF_RenderText_Blended(render_font, text.c_str(), text_color );
    if(text_surface != NULL)
    {
      /* Create the texture */
      SDL_Texture* text_texture = 
                          SDL_CreateTextureFromSurface(renderer, text_surface );
      if(text_texture != NULL)
      {
        /* Set the internal class texture */
        unsetTexture();
        texture = text_texture;
        height = text_surface->h;
        width = text_surface->w;
        success = true;
      }
      
      /* Free the surface */
      SDL_FreeSurface(text_surface);
    }
  }
  
  return success;
}

/*
 * Description: Unsets the font stored in the class.
 *
 * Inputs: none
 * Output: none
 */
void Text::unsetFont()
{
  if(delete_font)
    TTF_CloseFont(render_font);
  delete_font = false;
  render_font = NULL;
}

/*
 * Description: Unsets the texture stored in the class.
 *
 * Inputs: none
 * Output: none
 */
void Text::unsetTexture()
{
  SDL_DestroyTexture(texture);
  texture = NULL;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/* Create a font, based on the font parameters. Null if fails */
TTF_Font* Text::createFont(std::string font_path, int font_size,
                                                  int font_style)
{
  TTF_Font* new_font = TTF_OpenFont(font_path.c_str(), font_size);
  
  /* If the font creation is successful, set the style */
  if(new_font != NULL)
    TTF_SetFontStyle(new_font, font_style);

  return new_font;
}