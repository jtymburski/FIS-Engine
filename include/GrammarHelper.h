/******************************************************************************
 * Class Name: Grammar Helper
 * Date Created: August 10th, 2013
 * Inheritance: none
 * Description: Helper functions for English grammar [messages, etc.]
 *****************************************************************************/
#ifndef GRAMMARHELPER_H
#define GRAMMARHELPER_H

#include <QtGlobal>
#include <QString>
#include <QChar>
#include <QDebug>

#include "Game/Player/Item.h"

/* ------------- Constants ------------ */
const QString kQUINTILLION_STRING = "quintillion";
const QString kQUADRILLION_STRING = "quadrillion";
const QString kTRILLION_STRING    = "trillion";
const QString kBILLION_STRING     = "billion";
const QString kMILLION_STRING     = "million";
const QString kTHOUSAND_STRING    = "thousand";
const QString kHUNDRED_STRING     = "hundred";

/*============================================================================
 * NOUNAGE
 *===========================================================================*/

/* Returns "a [noun]" or "an [noun]", whichever is appropriate */
QString a_noun(QString noun);

/* Determines whether to use a or an based on the noun */
QString a_an(QString noun);

/* Determines the article noun of a noun based on wheter it's definite */
QString articleNoun(QString noun, bool definite);

/* Determines the full form of an Item */
QString fullFormItem(Item* object = 0, int quantity = 1);

/* Determines whether a character is a vowel */
bool isVowel(QChar x);

/* Determines the plural form of a noun */
QString plural(QString noun);

/* Determines the singular form of a noun */
QString singular(QString enoun);

/* Appends the appropriate plural suffix to a noun */
QString suffixS(QString noun);

/*============================================================================
 * VERBAGE
 *===========================================================================*/

/*============================================================================
 * PLURAL QUANTITY FUNCTIONS
 *===========================================================================*/

/* Determines whether to use is or are based on quantity */
QString isAre(int quantity);

/* Determines whether to use it or they based on quantity */
QString itThey(int quantity);

/* Determines whether to use it or them based on quantity */
QString itThem(int quantity);

/* Determines whether to use has or have based on quantity */
QString hasHave(int quantity);

/* Determines whether to use a plural or singular noun based on quantity */
QString pluralNoun(QString noun, int quantity);

/* Determines what to use as a quantified plural or singular noun */
QString quantifiedPluralNoun(QString noun, int quantity);

/* Determines whether to use that or those based on quantity */
QString thatThose(int quantity);

/* Determines whether to use this or those based on quantity */
QString thisThose(int quantity);

/*============================================================================
 * ENGLISH NUMBER FUNCTIONS
 *===========================================================================*/

/* Returns a QString of an English digit (1 - 20) */
QString digitName(uint n);

/* Returns a QString of the English version of a 64-bit integer */
QString intToEnglish(qint64 n);

/* Translates a hundred chunk of an English number */
QString hundredChunk(uint n);

/* Determines the string name of the magnitude of a number */
QString magnitudeName(uint magnitude);

/* Translates an integer to its English ordered form (ex. first, second) */
QString orderedInt(int n);

/* Translates a digit (1-20) to its ordered English form */
QString orderedDigit(uint n);

/* Pads a string with a space if needed */
QString padIfNeeded(QString str);

/* Translates the tens place of an English number */
QString tensName(uint n);

/* Translates the thousand chunk of an English number */
QString thousandChunk(uint n);

#endif // GRAMMARHELPER_H
