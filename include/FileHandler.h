/******************************************************************************
* Class Name: FileHandler
* Date Created: February 16, 2012
* Inheritance: none
* Description: Handles all files that enter and leave the game. Will be 
*              writing everything into a specific XML format and will allow
*              for encrypting and decrypting the file using an implementation 
*              of XXTEA. 
*
* Template: <map>
*             <row>
*               <tile>
*                 <base>file/path</base>
*                 <enhancer>file/path</enhancer>
*               </tile>
*             </row>
*           </map>
******************************************************************************/
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <QByteArray>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class FileHandler
{
public:
  /* Constructor: Sets up a blank template, no read/write file set */
  FileHandler();

  /* Destructor function */
  ~FileHandler();

  /* Public Enumerators */
  enum VarType {BOOLEAN, INTEGER, FLOAT, STRING};

private:
  /* Flag for if the class is available for usage */
  bool available;

  /* The depth that the XML file reader/writer is currently at */
  int depth;

  /* Set if the encryption system is enabled */
  bool encryption_enabled;

  /* The filename information */
  QString file_name;
  std::fstream* file_stream; // is_open()
  bool file_write;

  /* XML data that offers as a buffer between XML parser and encryption */
  QByteArray xml_data;

  /* XML handlers for reading/writing */
  QXmlStreamReader* xml_reader;
  QXmlStreamWriter* xml_writer;

public:
  /* Returns the filename that's used for reading from and writing to */
  QString getFilename();

  /* Determines if the class is available for using (ie. no open streams) */
  bool isAvailable();

  /* Determines if encryption is enabled for reading and writing */
  bool isEncryptionEnabled();

  /* Determines if the class is read or write (TRUE if write) */
  bool isWriteEnabled();

  /* Sets if encryption is enabled for reading and writing */
  void setEncryptionEnabled(bool enable);

  /* Sets the filename for reading from and writing to */
  void setFilename(QString path);

  /* Sets if the class is read or write (TRUE if write) */
  void setWriteEnabled(bool enable);
};

#endif // FILEHANDLER_H
