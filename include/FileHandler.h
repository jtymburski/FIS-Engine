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
 *****************************************************************************/
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <QByteArray>
#include <QDebug>
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
  enum FileType {REGULAR, XML};
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
  std::fstream file_stream; // is_open()
  FileType file_type;
  bool file_write;

  /* XML data that offers as a buffer between XML parser and encryption */
  QByteArray xml_data;

  /* XML handlers for reading/writing */
  QXmlStreamReader* xml_reader;
  QXmlStreamWriter* xml_writer;

  /* Close the file using fstream in the class */
  bool fileClose();

  /* Open the file using fstream in the class */
  bool fileOpen();

public:
  /* Returns the filename that's used for reading from and writing to */
  QString getFilename();

  /* Returns the file type that's used for reading from and writing to */
  FileType getFileType();

  /* Determines if the class is available for using (ie. no open streams) */
  bool isAvailable();

  /* Determines if encryption is enabled for reading and writing */
  bool isEncryptionEnabled();

  /* Determines if the class is read or write (TRUE if write) */
  bool isWriteEnabled();

  /* Reads the following line as a string. Only valid for REGULAR files */
  QString readLine(bool* fail = 0);

  /* Sets if encryption is enabled for reading and writing */
  void setEncryptionEnabled(bool enable);

  /* Sets the filename for reading from and writing to */
  void setFilename(QString path);

  /* Sets the type that the file is that will be read */
  void setFileType(FileType type);

  /* Sets if the class is read or write (TRUE if write) */
  void setWriteEnabled(bool enable);

  /* Starts the whole process, to be able to access the file stream */
  bool start();

  /* Stops the whole process, to end access to the file stream */
  bool stop();

  /* Writes the following line to the file. Only valid for REGULAR files */
  bool writeLine(QString line);
};

#endif // FILEHANDLER_H
