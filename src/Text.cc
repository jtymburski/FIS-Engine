/*******************************************************************************
 * Class Name: Text
 * Date Created: November 20, 2013
 * Inheritance: none
 * Description: This class handles the font and appropriate rendering. It is
 *              used for creating the font and storing it with render box
 *              sizing.
 ******************************************************************************/
#include "Text.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t Text::kDEFAULT_ALPHA = 255;

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
  alpha = kDEFAULT_ALPHA;
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
 * Inputs: string font_path - the path to the font
 *         int font_size - the DPI size of the font
 *         int font_style - the TTF styles, such as BOLD, ITALIC, etc.
 */
Text::Text(string font_path, int font_size, int font_style) : Text()
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
 * Description: Returns the alpha rating for the generated text. Sets
 *              automatically for any new lines of text.
 *
 * Inputs: none
 * Output: uint8_t - alpha opacity rating (0=invisible, 255=fully opaque)
 */
uint8_t Text::getAlpha()
{
  return alpha;
}

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
 * Description: Sets the alpha for the text generated texture. This will be set
 *              for the current generated texture and all new ones.
 *
 * Inputs: uint8_t alpha - the alpha rating (0=invisible, 255=fully opaque)
 * Output: none
 */
void Text::setAlpha(uint8_t alpha)
{
  this->alpha = alpha;
  SDL_SetTextureAlphaMod(getTexture(), alpha);
}

/*
 * Description: Sets the font to use for creating the text texture that will
 *              inevitably used for rendering. Fails if the font_path isn't
 *              relevant. The font created from this path will be deleted by
 *              this class when it's destroyed or when unsetFont() is called.
 *
 * Inputs: string font_path - the path to the font
 *         int font_size - the DPI size of font
 *         int font_style - the TTF styles, such as BOLD, ITALIC, etc.
 *                        - these are from SDL TTF. See documentation
 * Output: bool - returns if the font was created
 */
bool Text::setFont(string font_path, int font_size, int font_style)
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
 *         string text - the text to create the texture from
 *         SDL_Color text_color - the color of the text
 * Output: bool - returns if the text is created
 */
bool Text::setText(SDL_Renderer* renderer, string text, SDL_Color text_color)
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
        alpha = text_color.a;
        texture = text_texture;
        height = text_surface->h;
        width = text_surface->w;
        setAlpha(alpha);
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

/*
 * Description: Creates a font given a font path string and size. Will add
 *              styles as well if they are passed in by the user (OR multiple
 *              styles if you want). The font styles are part of the
 *              TTF_STYLE_* enumerators.
 *
 * Inputs: string font_path - the path to the font to open
 *         int font_size - the point size, based on 72DPI
 *         int font_style - the set of styles for the font
 * Output: TTF_Font* - a new font pointer (deleted by caller). NULL if fails
 */
TTF_Font* Text::createFont(string font_path, int font_size, int font_style)
{
  TTF_Font* new_font = TTF_OpenFont(font_path.c_str(), font_size);

  /* If the font creation is successful, set the style */
  if(new_font != NULL)
    TTF_SetFontStyle(new_font, font_style);

  return new_font;
}

/*
 * Description: Defines the property structure with the input data.
 *
 * Inputs: bool bold, italic, underline - text style properties
 *         uint8_t red, green, blue - the RGB values to define the color
 * Output: TextProperty - the returned property structure
 */
TextProperty Text::createProperty(bool bold, bool italic, bool underline,
                                  uint8_t red, uint8_t green, uint8_t blue)
{
  /* Font Style */
  int text_style = TTF_STYLE_NORMAL;
  if(bold)
    text_style |= TTF_STYLE_BOLD;
  if(italic)
    text_style |= TTF_STYLE_ITALIC;
  if(underline)
    text_style |= TTF_STYLE_UNDERLINE;

  /* Color */
  SDL_Color text_color = {red, green, blue, kDEFAULT_ALPHA};

  /* Create structure */
  TextProperty text_property;
  text_property.color = text_color;
  text_property.style = text_style;

  return text_property;
}

/*
 * Description: Accesses the data from the property structure.
 *
 * Inputs: TextProperty property - the property structure to access
 *         bool bold, italic, underline - text style properties
 *         uint8_t red, green, blue - the RGB values of the rendering text
 * Output: none
 */
void Text::dataProperty(TextProperty property, bool& bold, bool& italic,
                        bool& underline, uint8_t& red, uint8_t& green,
                        uint8_t& blue)
{
  bold = ((property.style & TTF_STYLE_BOLD) > 0);
  italic = ((property.style & TTF_STYLE_ITALIC) > 0);
  underline = ((property.style & TTF_STYLE_UNDERLINE) > 0);

  red = property.color.r;
  green = property.color.g;
  blue = property.color.b;
}
/*
 * Description: Takes an integer and properly formats it with thousand
 *              delimiters as per the north american standard.
 *
 * Inputs: int32_t number - the integer value to format
 * Output: string - the resulting string in the delimited format
 */
string Text::formatNum(int32_t number)
{
  string str_num;
  int index = 0;

  /* Handle 0 case */
  if(number == 0)
    return "0";

  /* If negative, remove negative sign and note */
  string prefix = "";
  if(number < 0)
  {
    prefix = "-";
    number *= -1;
  }

  /* Loop through number */
  while(number > 0)
  {
    /* Add comma to front if another sequence is about to be parsed */
    if(index != 0 && index % 3 == 0)
      str_num = "," + str_num;

    int digit = number % 10;
    str_num = to_string(digit) + str_num;
    number /= 10;
    index++;
  }

  /* If was negative, add to final string */
  str_num = prefix + str_num;

  return str_num;
}

/*
 * Description: Parse the html information within passed in text string and
 *              returns the vector set of strings and the properties. This will
 *              separate based on adjacent unique properties.
 * Notes: Input - <b>Test Text</b> Normal Text <i>Italic T<u>e</u>xt</i>
 *        Returns - "Test Text", Bold
 *                  " Normal Text ", No mods
 *                  "Italic T", Italic
 *                  "e", Italic, Underline
 *                  "xt", Italic
 *
 * Inputs: string text - the input string of text to parse
 * Output: vector<pair<string, TextProperty>> - the stack. See Notes above
 */
vector<pair<string, TextProperty>> Text::parseHtml(string text)
{
  vector<pair<string, TextProperty>> result;
  vector<string> first = Helpers::split(text, '<');
  TextProperty prop = createProperty();

  /* Create list of property working data */
  int bold = 0;
  int italic = 0;
  int underline = 0;
  vector<SDL_Color> color_set;

  /* If size is 1, do nothing and just return result */
  if(first.size() <= 1)
  {
    result.push_back(pair<string, TextProperty>(text, prop));
  }
  /* Size is greater than 1, thus a split occurred */
  else
  {
    /* Loop through first split set */
    for(uint32_t i = 0; i < first.size(); i++)
    {
      vector<string> second = Helpers::split(first[i], '>');

      /* If size is 2, this is a valid case for an XML element */
      if(second.size() == 2)
      {
        /* Parse property */ // TODO: Review QTextEdit prior...
        if(second.front() == "b")
        {
          bold++;
        }
        else if(second.front() == "i")
        {
          italic++;
        }
        else if(second.front() == "u")
        {
          underline++;
        }
        else if(second.front() == "/b")
        {
          bold--;
          if(bold < 0)
            bold = 0;
        }
        else if(second.front() == "/i")
        {
          italic--;
          if(italic < 0)
            italic = 0;
        }
        else if(second.front() == "/u")
        {
          underline--;
          if(underline < 0)
            underline = 0;
        }
        else if(second.front() == "/font")
        {
          if(color_set.size() > 0)
            color_set.erase(color_set.begin() + color_set.size() - 1);
        }
        /* Remaining must be font which is handled differently */
        else
        {
          vector<string> font_split = Helpers::split(second.front(), ' ');
          if(font_split.size() == 2 && font_split.front() == "font")
          {
            vector<string> color_split = Helpers::split(font_split.back(), '=');
            if(color_split.size() == 2 && color_split.front() == "color")
            {
              /* Confirm color in question */
              //string new_color = color_split.back();
              //if(new_color.size() == 
              // TODO: color_split.back() is "#ff0000". Process and add stack
            }
          }
        }

        // TODO: Process results
      }
      /* Otherwise, invalid: just return the original string */
      else
      {
        result.push_back(pair<string, TextProperty>(first[i], prop));
      }
    }
  }

  return result;
}

/*
 * Description: Splits a line into the appropriate widths, based on the
 *              rendering font. This will seg fault if the font is NULL or
 *              unset. For non-elided split, if the width is less than a single
 *              word size, it will still include the one word. For elided,
 *              if the width is too short, it will just be "...". The returned
 *              vector of lines will be of the appropriate length when rendered
 *              with the same font.
 *
 * Inputs: TTF_Font* font - the rendering font (must be non-NULL)
 *         string text - the sequence of characters to split
 *         int line_width - the limited length of line to delimit to
 *         bool elided - if the line should be cut off at a single line. If
 *                       cut shorter then text, add ...
 * Output: vector<string> - stack of lines, as per line_width
 */
vector<string> Text::splitLine(TTF_Font* font, string text,
                               int line_width, bool elided)
{
  int dot_width = 0;
  vector<string> line_stack;
  int space_width = 0;
  bool success = true;
  int width = 0;
  vector<string> words = Helpers::split(text, ' ');
  vector<int> word_widths;

  /* Get the widths of all words */
  for(uint16_t i = 0; i < words.size(); i++)
  {
    success &= (TTF_SizeText(font, words[i].c_str(), &width, NULL) == 0);
    word_widths.push_back(width);
  }
  success &= (TTF_SizeText(font, " ", &space_width, NULL) == 0);
  success &= (TTF_SizeText(font, ".", &dot_width, NULL) == 0);

  /* Proceed if font sizing was successful */
  if(success)
  {
    bool done = false;
    uint16_t index = 0;
    string line = "";
    width = 0;

    /* If elided, it sets the line to the under line length. If it's greater,
     * it adds ... after the last word */
    if(elided)
    {
      /* Modify the line width to expect the final 3 dots */
      line_width -= (dot_width * 3);

      while(index < words.size() && !done)
      {
        /* Calculate the check width, to compare against line width */
        int check_width = width + word_widths[index];
        if(index != 0)
          check_width += space_width;

        /* Run the check, if greater, end line fill sequence */
        if(check_width > line_width)
          done = true;
        /* Otherwise, append the word */
        else
        {
          if(index != 0)
          {
            line += " ";
            width += space_width;
          }
          line += words[index];
          width += word_widths[index];
          index++;
        }
      }

      /* Add the final ... and push word to vector */
      if(done)
        line += "...";
      line_stack.push_back(line);
    }
    /* Otherwise, this creates a sequence of lines, where each line is less
     * than the limit of line length unless one word is longer than length */
    else
    {
      bool first_word = true;

      /* Loop through all the words */
      while(index < words.size())
      {
        /* If it's the first word, force it onto the stack */
        if(first_word)
        {
          line += words[index];
          width += word_widths[index];
          first_word = false;
          index++;
        }
        /* Otherwise, check if the new word will make the line too long
         * If so, push the previous line and clear to make way for the new
         * word */
        else if((width + space_width + word_widths[index]) > line_width)
        {
          line_stack.push_back(line);
          line.clear();
          width = 0;
          first_word = true;
        }
        /* Otherwise, the word can be appended. Do so, and shift the index */
        else
        {
          line += " " + words[index];
          width += space_width + word_widths[index];
          index++;
        }
      }

      /* Append the final line if not null */
      if(!line.empty())
        line_stack.push_back(line);
    }
  }

  return line_stack;
}
