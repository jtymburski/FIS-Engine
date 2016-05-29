/******************************************************************************
 * Class Name: FileHandler
 * Date Created: February 16, 2012
 * Inheritance: none
 * Description: Handles all files that enter and leave the game. Will be
 *              writing everything into a specific XML format and will allow
 *              for encrypting and decrypting the file using an implementation
 *              of XXTEA.
 *
 * Example: REGULAR file read
 *          -----------------
 *          setFilename("name");
 *          setFileType(FileType::REGULAR);
 *          setWriteEnabled(false); // Set to true for writing
 *          setEncryptionEnabled(false); // Set to true for encryption
 *          start();
 *          //writeRegularLine("line"); // For writing
 *          readRegularLine(); // For reading
 *          stop();
 *
 *          XML file read
 *          -------------
 *          setFilename("name");
 *          setFileType(FileType::XML);
 *          setWriteEnabled(false); // Set to true for writing
 *          setEncryptionEnabled(false); // Set to true for encryption
 *          start();
 *
 *          // For reading
 *          readXmlData();
 *
 *          // For writing
 *          //writeXmlElement("person", "id", "1");
 *          //writeXmlData("name", VarType::STRING, "john");
 *          //writeXmlElementEnd();
 *          // Result: <person id="1">
 *          //           <name type="3">john</name> // type 3 is string
 *          //         </person>
 *
 *          stop();
 *****************************************************************************/
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <cstdint>
#include <ctime>
#include <fstream>
#include <string>

#include "Helpers.h"
#include "Md5.h"
#include "TinyXML.h"
#include "XmlData.h"

/* Macros */
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (kKEY[(p&3)^e] ^ z)))

class FileHandler
{
public:
  /* Constructor: Sets up a blank template, no read/write file set */
  FileHandler();
  FileHandler(std::string filename, bool write = false,
              bool xml = false, bool encryption = false);

  /* Destructor function */
  ~FileHandler();

  /* Public Enumerators */
  enum FileType {REGULAR, XML};
  enum VarType
  {
    BOOLEAN = 1,
    INTEGER = 2,
    FLOAT   = 3,
    STRING  = 4
  };

private:
  /* Flag for if the class is available for usage */
  bool available;

  /* Element count - lines for regular, final elements for XML */
  int element_count;

  /* Set if the encryption system is enabled */
  bool encryption_enabled;

  /* The filename information */
  std::string file_data; /* The data that has been written */
  std::string file_date; /* The date of the file */
  std::string file_name; /* The name of the file */
  std::string file_name_temp; /* The temporary file name for writing */
  std::fstream file_stream; // is_open()
  FileType file_type;
  bool file_write;

  /* XML handlers for reading/writing */
  TinyXML2::XMLDocument* xml_document;
  TinyXML2::XMLNode* xml_node;

  /*------------------- Constants -----------------------*/
  const static int kASCII_IN_LONG;   /* # of ascii's that will fit in long */
  const static uint32_t kDELTA;      /* Sum bias for encryption */
  const static int kENCRYPTION_MIN;  /* Min line length for encryption */
  const static int kENCRYPTION_PAD;  /* Padding for encrypted values */
  const static int kFILE_NAME_LIMIT; /* File end number limit */
  const static int kFILE_START;      /* File start for temp data */
  const static int kINT_BIT_SHIFT;   /* Shift int to next spot */
  const static int kINT_BUFFER;      /* Only use most significant int */
  const static uint32_t kKEY[];      /* Key array for encryption */
  const static int kLONG_BIT_SHIFT;  /* Number of bits to shift long to
                                      * next spot */
  const static int kLONG_BUFFER;     /* Only use most significant long */
  const static int kMAX_ASCII;       /* Max ascii out of bounds */
  const static int kMIN_LINE;        /* Minimum line length for encryption */
  const static int kPADDING_ASCII;   /* Start of padding characters */
  const static int kXXTEA_ROUNDS;    /* Number of rounds for encryption */

/*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Clean up function, frees appropriate pointers */
  void cleanUp();

  /* Decrypt raw data in an array of ints */
  bool decryptData(uint32_t* data);

  /* Determines element XML count */
  void determineCount();

  /* Decrypt line of data */
  std::string decryptLine(std::string line, bool* success = 0);

  /* Encrypt raw data in an array of ints */
  bool encryptData(uint32_t* data);

  /* Encrypt line of data */
  std::string encryptLine(std::string line, bool* success = 0);

  /* Close the file using fstream in the class */
  bool fileClose();

  /* Open the file using fstream in the class */
  bool fileOpen();

  /* Converts an array of ints into an array of longs -> 4 ints in one long */
  uint32_t* intToLong(int* line_data, int length);

  /* Converts an array of ints to a string line, representing the characters */
  std::string intToString(int* line_data, int length, bool decrypting = 0);

  /* Converts an array of longs into an array of ints -> one long -> 4 ints */
  int* longToInt(uint32_t* line_data, int length);

  /* The base read line class, reads data from the file */
  std::string readLine(bool* done = nullptr, bool* success = nullptr,
                       std::fstream* file_stream = nullptr);

  /* Confirms if the MD5 matches the file */
  bool readMd5();

  /* Ascertains the temp file name to be used in the program */
  bool setTempFileName();

  /* Convert a string to an array of ints, each representing a character */
  int stringToInt(std::string line, int** line_data, bool encrypting = 0);

  /* Returns the control to the top of the file */
  bool topOfFile();

  /* The base write line class, pushes data to the file */
  bool writeLine(std::string line, bool md5_line = false);

  /* Takes a number and wraps it around, if it exceeds the limit */
  int wrapNumber(int value, int limit);

  /* Returns the next data node from the current pointer */
  TinyXML2::XMLNode* xmlNextData(TinyXML2::XMLNode* starting_node);

  /* Returns the next node, that isn't a child but either a sibling or a
   * parent's sibling */
  TinyXML2::XMLNode* xmlNextNode(TinyXML2::XMLNode* starting_node);

  /* Starts the XML reading procedure */
  bool xmlReadStart(bool read_before_write = false);

  /* Finishes the XML writing procedure */
  bool xmlWriteEnd();

/*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Finds the element sequence in the stack (not including data entry) and
   * puts the active node pointer at that location. */
  TinyXML2::XMLNode* findElement(XmlData data, bool save_location = false);

  /* Returns a count of the number of elements */
  int getCount();

  /* Returns the date that was acquired/set */
  std::string getDate();

  /* Returns the filename that's used for reading from and writing to */
  std::string getFilename();

  /* Returns the file type that's used for reading from and writing to */
  FileType getFileType();

  /* Determines if the class is available for using (ie. stream opened) */
  bool isAvailable();

  /* Determines if encryption is enabled for reading and writing */
  bool isEncryptionEnabled();

  /* Determines if the class is read or write (TRUE if write) */
  bool isWriteEnabled();

  /* Finds the element in the stack (not including data entry) and then purges
   * all children. */
  TinyXML2::XMLNode* purgeElement(XmlData data, bool save_location = false);

  /* Reads the following line as a string. Only valid for REGULAR files */
  std::string readRegularLine(bool* done = nullptr, bool* success = nullptr);

  /* Reads the next XML data element - returns an empty data element when
   * done */
  XmlData readXmlData(bool* done = nullptr, bool* success = nullptr);

  /* Save - triggers the write to file without closing the document */
  bool save();

  /* Sets if encryption is enabled for reading and writing */
  bool setEncryptionEnabled(bool enable);

  /* Sets the filename for reading from and writing to */
  bool setFilename(std::string path);

  /* Sets the type that the file is that will be read */
  bool setFileType(FileType type);

  /* Sets if the class is read or write (TRUE if write) */
  bool setWriteEnabled(bool enable);

  /* Starts the whole process, to be able to access the file stream */
  bool start(bool read_before_write = false);

  /* Stops the whole process, to end access to the file stream */
  bool stop(bool failed = false);

  /* Writes the following line to the file. Only valid for REGULAR files */
  bool writeRegularLine(std::string line);

  /* Writes the data encapsulated by element(s) */
  bool writeXmlData(std::string element, VarType type, std::string data);
  bool writeXmlData(std::string element, bool data);
  bool writeXmlData(std::string element, float data);
  bool writeXmlData(std::string element, int data);
  bool writeXmlData(std::string element, std::string data);
  bool writeXmlData(std::string element, uint32_t data);

  /* Writes the data as described in the xml data set */
  bool writeXmlDataSet(XmlData data, bool save_location = false);

  /* Writes a starting XML element */
  bool writeXmlElement(std::string element,
                       std::string key = "", std::string value = "");
  bool writeXmlElement(std::string element,
                        std::string key, int value);

  /* Writes an ending XML element or elements */
  bool writeXmlElementEnd(bool all = false);

  /* Puts the xml reader at the head of the XML document - returns date if
   * exists */
  std::string xmlToHead();

  /* Puts the xml reader at the end of the XML document */
  bool xmlToTail();

/*===================== PUBLIC STATIC  FUNCTIONS ===========================*/
public:
  /* Copies the file and based on the overwrite flag */
  static bool fileCopy(std::string old_filename, std::string new_filename,
                       bool overwrite = false);

  /* Deletes the said file, if it exists */
  static bool fileDelete(std::string filename);

  /* Check if the file exists */
  static bool fileExists(std::string filename);

  /* Rename the file, if it exists and the new name doesn't */
  static bool fileRename(std::string old_filename, std::string new_filename,
                         bool overwrite = false);

  /* Returns the current date in a string form as per std::strftime */
  static std::string getCurrentDate(std::string format = "%Y/%m/%d %H:%M:%S");
};

#endif // FILEHANDLER_H
