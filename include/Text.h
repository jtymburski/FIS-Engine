/*******************************************************************************
 * Class Name: Text
 * Date Created: November 20, 2013
 * Inheritance: none
 * Description: This class handles the font and appropriate rendering. It is
 *              used for creating the font and storing it with render box
 *              sizing.
 ******************************************************************************/
#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Helpers.h"

using std::vector;
using std::string;
using std::pair;
using std::to_string;

/*
 * Description: Text property. Defines the pertinent information for rendering
 *              a set of text based on the core text properties.
 */
struct TextProperty
{
  SDL_Color color;
  int style;

  bool operator==(const TextProperty& prop) const
  {
    return (prop.color.r == color.r && prop.color.g == color.g &&
            prop.color.b == color.b && prop.color.a == color.a &&
            prop.style == style);
  }
};

/* Text Class */
class Text
{
public:
  /* Constructor functions */
  Text();
  Text(TTF_Font* font);
  Text(string font_path, int font_size, int font_style = TTF_STYLE_NORMAL);

  /* Destructor function */
  ~Text();

private:
  /* The texture alpha rating */
  uint8_t alpha;

  /* The height of the texture */
  int height;

  /* The font to render the text to, and if it needs to be deleted */
  TTF_Font* render_font;
  bool delete_font;

  /* The texture that the font is rendered on */
  SDL_Texture* texture;

  /* The width of the texture */
  int width;

  /*------------------- Constants -----------------------*/
  const static uint8_t kDEFAULT_ALPHA; /* The default alpha rating */

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the alpha rating of the texture */
  uint8_t getAlpha();

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
  bool render(SDL_Renderer* renderer, int x, int y, int src_w, int src_h);

  /* Sets the alpha rating of the texture */
  void setAlpha(uint8_t alpha);

  /* Sets up the font, that is used for rendering the text */
  bool setFont(std::string font_path, int font_size,
                                      int font_style = TTF_STYLE_NORMAL);

  /* Sets the using font to this one - does not delete once class is done */
  bool setFont(TTF_Font* font);

  /* Sets the text, stored in the internal texture */
  bool setText(SDL_Renderer* renderer, string text, SDL_Color text_color);
  bool setText(SDL_Renderer* renderer,
               vector<vector<pair<string, TextProperty>>> text,
               int length = -1);

  /* Clean up functions for the data stored in the class */
  void unsetFont();
  void unsetTexture();

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the length of the text passed in */
  static int countLength(vector<vector<pair<string, TextProperty>>> text);

  /* Create a font, based on the font parameters. Null if fails */
  static TTF_Font* createFont(string font_path, int font_size,
                              int font_style = TTF_STYLE_NORMAL);

  /* Defines the property structure */
  static TextProperty createProperty(bool bold = false, bool italic = false,
                                     bool underline = false, uint8_t red = 255,
                                     uint8_t green = 255, uint8_t blue = 255);

  /* Accesses the data from the property structure */
  static void dataProperty(TextProperty property, bool& bold, bool& italic,
                           bool& underline, uint8_t& red, uint8_t& green,
                           uint8_t& blue);

  /* Takes an integer and properly formats it with thousands separators */
  static string formatNum(int32_t number);

  /* Parse the html information within the text set and return the property */
  static vector<pair<string, TextProperty>> parseHtml(string text);

  /* Takes a string of characters and splits it to fit a line after it is
   * rendered by the given font. */
  static vector<string> splitLine(TTF_Font* font, std::string text,
                                  int line_width, int elide_count = 0);

  /* Takes a string of characters processed by parseHtml() and splits it to fit
   * a line after it is rendered by the given font */
  static vector<vector<vector<pair<string, TextProperty>>>> splitLineProperty(
                                   TTF_Font* font, string text, int line_width,
                                   int elide_count = 0);
  static vector<vector<vector<pair<string, TextProperty>>>> splitLineProperty(
                                   TTF_Font* font, int line_width,
                                   vector<pair<string, TextProperty>> text_set,
                                   int elide_count = 0);
};

#endif // TEXT_H
