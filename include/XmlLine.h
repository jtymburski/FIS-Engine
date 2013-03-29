/******************************************************************************
 * Class Name: XmlLine
 * Date Created: March 29, 2013
 * Inheritance: none
 * Description: Handles a line of XML data. This is used when data is read 
 *              from the XML file reader and returned to the calling party.
 *****************************************************************************/
#ifndef XMLLINE_H
#define XMLLINE_H

class XmlLine
{
public:
  /* Constructor: Sets up a blank template with no data in the line set */
  XmlLine();

  /* Destructor function */
  ~XmlLine();

  /* Public Enumerators */
  //enum FileType {REGULAR, XML};
  //enum VarType {BOOLEAN, INTEGER, FLOAT, STRING};

private:
  /* Flag for if the class is available for usage */
  //bool available;

  /*------------------- Constants -----------------------*/
  //const static int kASCII_IN_LONG = 4; /* # of ascii's that will fit in */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Compute Md5 for byte array of data */
  //QByteArray computeMd5(QByteArray data);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the date that was acquired/set */
  //QString getDate();

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the said file, if it exists */
  //static bool fileDelete(QString filename);
};

#endif // XMLLINE_H
