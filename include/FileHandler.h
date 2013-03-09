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
#include <stdint.h>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

/* Macros */
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (kKEY[(p&3)^e] ^ z)))

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
  QByteArray file_data;
  QString file_name;
  std::fstream file_stream; // is_open()
  FileType file_type;
  bool file_write;

  /* XML data that offers as a buffer between XML parser and encryption */
  QByteArray xml_data;

  /* XML handlers for reading/writing */
  QXmlStreamReader* xml_reader;
  QXmlStreamWriter* xml_writer;

  /*------------------- Constants -----------------------*/
  const static int kASCII_IN_LONG = 4; /* # of ascii's that will fit in long */
  const static int kDELTA = 2654435769; /* Sum bias for encryption */
  const static int kENCRYPTION_MIN = 4; /* Min line length for encryption */
  const static uint32_t kKEY[]; /* Key array for encryption */
  const static int kLONG_BIT_SHIFT = 8; /* Number of bits to shift long to 
                                           next spot */
  const static int kLONG_BUFFER = 0xFF; /* Only use most significant long */
  const static int kMAX_ASCII = 255; /* Max ascii out of bounds */
  const static int kMIN_LINE = 16; /* Minimum line length for encryption */
  const static int kPADDING_ASCII = 200; /* Start of padding characters */
  const static int kXXTEA_ROUNDS = 19; /* Number of rounds for encryption */

private:
  /* Decrypt raw data in an array of ints */
  bool decryptData(uint32_t* data);

  /* Decrypt line of data */
  QString decryptLine(QString line, bool* success = 0);

  /* Encrypt raw data in an array of ints */
  bool encryptData(uint32_t* data);

  /* Encrypt line of data */
  QString encryptLine(QString line, bool* success = 0);

  /* Close the file using fstream in the class */
  bool fileClose();

  /* Open the file using fstream in the class */
  bool fileOpen();

  /* Converts an array of ints into an array of longs -> 4 ints in one long */
  uint32_t* intToLong(int* line_data, int length);

  /* Converts an array of ints to a string line, representing the characters */
  QString intToString(int* line_data, int length, bool decrypting = 0);

  /* Converts an array of longs into an array of ints -> one long -> 4 ints */
  int* longToInt(uint32_t* line_data, int length);

  /* Convert a string to an array of ints, each representing a character */
  int stringToInt(QString line, int** line_data, bool encrypting = 0);

  /* Takes a number and wraps it around, if it exceeds the limit */
  int wrapNumber(int value, int limit);

public:
  /* Returns the hash for the data that has currently been written. Returns
   * nothing if the function is uninitialized or set on read */
  QByteArray computeMd5();

  /* Returns the filename that's used for reading from and writing to */
  QString getFilename();

  /* Returns the file type that's used for reading from and writing to */
  FileType getFileType();

  /* Determines if the class is available for using (ie. stream opened) */
  bool isAvailable();

  /* Determines if encryption is enabled for reading and writing */
  bool isEncryptionEnabled();

  /* Determines if the class is read or write (TRUE if write) */
  bool isWriteEnabled();

  /* Reads the following line as a string. Only valid for REGULAR files */
  QString readLine(bool* done = 0, bool* success = 0);

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
