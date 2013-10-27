/******************************************************************************
 * Class Name: Grammar Helper
 * Date Created: August 10th, 2013
 * Inheritance: none
 * Description: Helper functions for English grammar [messages, etc.]
 *****************************************************************************/
#include "GrammarHelper.h"

/*============================================================================
 * NOUNAGE
 *===========================================================================*/

/*
 * Description: Returns: "a [noun]" or "an [noun]"
 *
 * Inputs: QString noun - the noun to be examined
 * Output: Either "a [noun]" or "an [noun]" where appropriate
 */
QString a_noun(QString noun)
{
  if (noun == "")
    return "";
  return a_an(noun) + " " + noun;
}

/*
 * Description: Returns "a" or "an" based on the first syllable of a noun.
 *
 * Inputs: QString noun - the noun to be examined.
 * Output: QString - "a" or "an", whichever is appropriate
 */
QString a_an(QString noun)
{
  QString a_an = "a";
  QString lower = noun.toLower();
  QChar initial = noun.at(0);

  /* General vowel vs. consonant rule ("u" is usually consonant) */
  if (lower.length() == 1)
  {
    if (isVowel(initial) && initial != 'u')
      a_an = "an";
    a_an = "a";
  }

  /* Generally words that start with vowels are "an" */
  if (isVowel(initial))
    a_an = "an";

  if ((initial == 'y') && !isVowel(lower.at(1)))
    a_an = "a";

  /* Some exceptions for consonant and vowel sounds (e.g. "a eulogy") */
  if (lower.left(2) == "eu" || lower.left(3) == "one" || lower.left(3) == "uni")
    a_an = "a";
  else if (lower.left(3) == "hon")
    a_an = "an";

  return a_an;
}

/*
 * Description: Prefixes a noun with an article
 *
 * Inputs: QString noun - the noun to be prefixed.
 *         bool definite - true for "the", false for a/an
 * Output: QString - the noun with an article
 */
QString articleNoun(QString noun, bool definite)
{
  if (definite)
    return "the " + noun;
  return a_noun(noun);
}

/*
 * Description: Prefixes a noun with an expression such as "piece of"
 *
 * Inputs: Item* object - pointer to the object to be full formed.
 *         int quantity - the number of items to be prefixed.
 *
 * Output: QString - the full form of the Item, ex. "pieces of X"
 */
QString fullFormItem(Item* object, int quantity)
{
  if (object == 0)
      return "";

  if (object->getPrefix() != "")
  {
    if (quantity > 1)
      return plural(object->getPrefix()) + " of " + object->getName();
    return object->getPrefix() + " of " + object->getName();
  }

  return object->getName();
}

/*
 * Description: Determines whether a char is a vowel
 *
 * Inputs: QChar x - the character to be checked
 * Output: Returns true if x is a vowel (A, E, I, O, U)
 */
bool isVowel(QChar x)
{
  x = x.toLower();

  if (x == 'a' || x == 'e' || x == 'i' || x == 'o' || x == 'u')
    return true;
  return false;
}

//TODO
QString plural(QString noun)
{
  noun;//WARNING
  return "";//WARNING
}

//TODO
QString singular(QString enoun)
{
  enoun;//WARNING
  return "";//WARNING
}

/*
 * Description: Appends the proper plural to a noun, ex. Arcadius' or Elephants.
 *
 * Inputs: QString - the noun to have a plural appended.
 * Output: QString - the noun with a plural suffix appended.
 */
QString suffixS(QString noun)
{
  QChar last = noun.right(1).at(0);

  if (last == 's')
      return noun + "'";
  return noun + "'s";
}

/*============================================================================
 * VERBAGE
 *===========================================================================*/

/*============================================================================
 * PLURAL QUANTITY FUNCTIONS
 *===========================================================================*/

/*
 * Description: Determines whether appropriate to use "is" or "are"
 *
 * Inputs: int - quantity of a noun
 * Output: QString - "is" or "are", whichever is appropriate
 */
QString isAre(int quantity)
{
  if (quantity)
    return "are";
  return "is";
}

/*
 * Description: Determines whether appropriate to use "it" or "they"
 *
 * Inputs: int - quantity of a noun
 * Output: QString - "it" or "they", whichever is appropriate
 */
QString itThey(int quantity)
{
  if (quantity)
    return "they";
  return "it";
}

/*
 * Description: Determines whether appropriate to use "it" or "them"
 *
 * Inputs: int - quantity of a noun
 * Output: QString - "it" or "them", whichever is appropriate
 */
QString itThem(int quantity)
{
  if (quantity)
    return "them";
  return "it";
}

/*
 * Description: Determines whether appropriate to use "has" or "have"
 *
 * Inputs: int - quantity of a noun
 * Output: QString - "have" or "has", whichever is appropriate
 */
QString hasHave(int quantity)
{
  if (quantity)
    return "have";
  return "has";
}

/*
 * Description: Determines whether to find the singular or plural version of
 *              a given noun.
 *
 * Inputs: QString - the noun to be examined.
 *         int - quantity of the noun
 * Output: QString - the singular or plural version of a noun
 */
QString pluralNoun(QString noun, int quantity)
{
  if (quantity == 1)
    return singular(noun);
  else
    return plural(noun);
}

/*
 * Description: Returns the singular or plural version of a given name, prefixed
 *              by the English equivalent of the quantity.
 *
 * Inputs: QString - the noun to be examined.
 *         int - the quantity of the noun
 * Output: QString - singular or plural version, with English quantity prefixed
 */
QString quantifiedPluralNoun(QString noun, int quantity)
{
  QString end = pluralNoun(noun, quantity);
  return intToEnglish((qint64)quantity) + " " + end;
}

/*
 * Description: Determines whether appropriate to use "that" or "those"
 *
 * Inputs: int - quantity of a noun
 * Output: QString - "that" or "those", whichever is appropriate
 */
QString thatThose(int quantity)
{
  if (quantity)
    return "those";
  return "that";
}

/*
 * Description: Determines whether appropriate to use "this" or "these"
 *
 * Inputs: int - quantity of a noun
 * Output: QString - "this" or "these", whichever is appropriate
 */
QString thisThese(int quantity)
{
  if (quantity)
    return "these";
  return "this";
}

/*============================================================================
 * English Number Functions
 *===========================================================================*/

/*
 * Description: Converts a 64-bit integer to the English name.
 *
 * Inputs: qint64 - a 64 bit integer
 * Output: QString - the english name of the integer
 */
QString intToEnglish(qint64 n)
{
  bool negative = false;
  QString number = "";
  int magnitude = 0;

  if (n < 0)
  {
    negative = true;
    n *= -1;
  }

  while (n > 0)
  {
    if (n % 1000 != 0)
      number = thousandChunk(n % 1000)
               + padIfNeeded(magnitudeName(magnitude)
               + padIfNeeded(number));
    n /= 1000;
    magnitude++;
  }

  if (number == "")
    number = "zero";
  if (negative)
    number = "negative " + number;

  return number;
}

/*
 * Description: Converts a digit (1-20) to the English equivalent
 *
 * Inputs: uint n - the digit to be converted
 * Output: QString - the English equivalent of the digit
 */
QString digitName(uint n)
{
  if (n == 0)  return  "";
  if (n == 1)  return  "one";
  if (n == 2)  return  "two";
  if (n == 3)  return  "three";
  if (n == 4)  return  "four";
  if (n == 5)  return  "five";
  if (n == 6)  return  "six";
  if (n == 7)  return  "seven";
  if (n == 8)  return  "eight";
  if (n == 9)  return  "nine";
  if (n == 10) return "ten";
  if (n == 11) return "eleven";
  if (n == 12) return "twelve";
  if (n == 13) return "thirteen";
  if (n == 14) return "fourteen";
  if (n == 15) return "fifteen";
  if (n == 16) return "sixteen";
  if (n == 17) return "seventeen";
  if (n == 18) return "eighteen";
  if (n == 19) return "nineteen";
  return "";
}

/*
 * Description: Converts the tens place value of a number to its English
 *              equivalent.
 *
 * Inputs: uint - the value of the tens place
 * Output: QString - the name of the tens value of the digit (ex. 2 = "twenty")
 */
QString tensName(uint n)
{
  if (n < 2)  return "";
  if (n == 2) return "twenty";
  if (n == 3) return "thirty";
  if (n == 4) return "forty";
  if (n == 5) return "fifty";
  if (n == 6) return "sixty";
  if (n == 7) return "seventy";
  if (n == 8) return "eighty";
  if (n == 9) return "ninety";
  return "";
}

/*
 * Description: Converts the thousand chunk of a number to its English
 *              equivalent.
 *
 * Inputs: uint - the thousand chunk of a number to be converted.
 * Output: QString - English ver of a thousand chunk (ex. for [486] of [1486])
 */
QString thousandChunk(uint n)
{
  if (n < 100)
    return hundredChunk(n);

  int hundreds = n / 100;
  int hundred_chunk = n % 100;

  return digitName(hundreds) + " " + kHUNDRED_STRING +
          padIfNeeded(hundredChunk(hundred_chunk));
}

/*
 * Description: Converts the hundred chunk of a number to its English equivalent
 *
 * Inputs: uint - the hundred chunk to be converted
 * Output: QString - the English ver of a hundred chunk (ex. for [87] of [187])
 */
QString hundredChunk(uint n)
{
  /* Special Cases */
  if (n < 20)
    return digitName(n);

  int tens = n / 10;
  int ones = n % 10;

  return tensName(tens) + padIfNeeded(digitName(ones));
}

/*
 * Description: Determines the English string of the magnitude (ex. million)
 *
 * Inputs: uint - the order of magnitude of the number (10^3X)
 * Output: QString - the name of the magnitude
 */
QString magnitudeName(uint magnitude)
{
  if (magnitude >= 7)
    return "unknown magnitude";
  if (magnitude >= 6)
    return kQUINTILLION_STRING;
  if (magnitude >= 5)
    return kQUADRILLION_STRING;
  if (magnitude >= 4)
    return kTRILLION_STRING;
  if (magnitude >= 3)
    return kBILLION_STRING;
  if (magnitude >= 2)
    return kMILLION_STRING;
  if (magnitude >= 1)
    return kTHOUSAND_STRING;
  return "";
}

/*
 * Description: Determines the ordered English version of an integer (ex. first
 * or twenty third or fifty forth)
 *
 * Inputs: int - the integer to be converted
 * Output: QString - the English ordered version of the Integer
 */
QString orderedInt(int n)
{
  bool negative = false;
  QString ordered = "";
  int last_digit = n % 10;

  if (n < 0)
  {
    negative = true;
    n *= -1;
  }

  if (n < 20)
  {
    ordered = orderedDigit(n);
  }
  else
  {
    ordered = intToEnglish((qint64)n);

    /* If the number ends in a '0', append an "ieth". Else, convert the
     * number to its English representation, then chop off the last word and
     * convert that to English. ex. four hundred twelve -> four hundred twelth
     */
    if (n % 10 == 0)
    {
        ordered.chop(1);
      ordered += "ieth";
    }
    else
    {
      ordered.section(" ", -1);

      while (!ordered.at(ordered.size() - 1).isSpace())
        ordered.chop(1);

      ordered = ordered + orderedDigit(last_digit);
    }
  }

  if (negative)
      ordered = "negative" + ordered;
  return ordered;
}

/*
 * Description: Determines the English ordered version of a given integer.
 *
 * Inputs: uint - the integer to be converted to its English ordered form.
 * Output: QString - the ordered equivalent of the int (ex. first, second, etc.)
 */
QString orderedDigit(uint n)
{
  if (n == 1)  return "first";
  if (n == 2)  return "second";
  if (n == 3)  return "third";
  if (n == 4)  return "fourth";
  if (n == 5)  return "fifth";
  if (n == 6)  return "sixth";
  if (n == 7)  return "seventh";
  if (n == 8)  return "eighth";
  if (n > 8 && n != 12 && n != 20)  return digitName(n) + "th";
  if (n == 12) return "twelth";
  return "";
}

/*
 * Description: Pads a string if needed.
 *
 * Inputs: QString - the string to be examined
 * Output: QString - padded version of the string (if not blank)
 */
QString padIfNeeded(QString str)
{
  if (str == "")
    return "";
  return " " + str;
}
