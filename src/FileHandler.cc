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
#include "FileHandler.h"

/* Constant Implementation - see header file for descriptions */
const int      FileHandler::kASCII_IN_LONG   = 4;
const uint32_t FileHandler::kDELTA           = 2654435769u;
const int      FileHandler::kENCRYPTION_MIN  = 4;
const int      FileHandler::kENCRYPTION_PAD  = 150;
const int      FileHandler::kFILE_NAME_LIMIT = 1000000;
const int      FileHandler::kFILE_START      = 5728;
const int      FileHandler::kINT_BIT_SHIFT   = 4;
const int      FileHandler::kINT_BUFFER      = 0xF;
const uint32_t FileHandler::kKEY[]           = {1073676287u,27644437u,
                                                2971215073u,94418953u};
const int      FileHandler::kLONG_BIT_SHIFT  = 8;
const int      FileHandler::kLONG_BUFFER     = 0xFF;
const int      FileHandler::kMAX_ASCII       = 255;
const int      FileHandler::kMIN_LINE        = 16;
const int      FileHandler::kPADDING_ASCII   = 200;
const int      FileHandler::kXXTEA_ROUNDS    = 19;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor function - Set up the file handler class with
 *              just initial values and empty initialization.
 *
 * Input: none
 */
FileHandler::FileHandler()
{
  /* Preps the variables */
  available = false;
  encryption_enabled = false;
  file_date = "";
  file_name = "";
  file_name_temp = "";
  file_type = REGULAR;
  file_write = false;
  //xml_data = "";
  //xml_depth = 0;
  xml_document = NULL;
  xml_node = NULL;
}

/* 
 * Description: Constructor function - Set up the file handler class with
 *              just initial values and the initialization of the variables
 *              outlined below. Allows for quick initialization of the 
 *              class.
 *
 * Input: std::string filename - the filename to read/write data to.
 *        bool write - set the class to write instead of read (default false)
 *        bool xml - set the file type to xml instead of regular line format
 *                   (default false)
 *        bool encryption - enables encryption for reading and writing 
 *                          (default false)
 */
FileHandler::FileHandler(std::string filename, bool write, 
                         bool xml, bool encryption) : FileHandler()
{
  setEncryptionEnabled(encryption);
  setFilename(filename);
  setWriteEnabled(write);
  xml ? setFileType(XML) : setFileType(REGULAR);
}

/* 
 * Description: Destructor function
 */
FileHandler::~FileHandler()
{
  stop(true);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Cleans up the dynamic memory in the class.
 *
 * Inputs: none
 * Output: none
 */
void FileHandler::cleanUp()
{
  /* Delete XML pointer handler */
  if(xml_document != 0)
    delete xml_document;
  xml_document = NULL;

  xml_node = NULL;
}

/* 
 * Description: Takes a sequence of 32 bit data (4 in the array) and decrypts
 *              the data, as per XXTEA algorithm. The data is returned on the
 *              same path and returns a status.
 *
 * Inputs: uint32_t* data - an array of 4 32 bit values to decrypt
 * Output: bool - the returned success status
 */
bool FileHandler::decryptData(uint32_t* data)
{
  uint32_t y, z, sum;
  unsigned p, rounds, e;

  if(data != 0)
  {
    rounds = kXXTEA_ROUNDS;
    sum = rounds * kDELTA;
    y = data[0];

    do
    {
      e = (sum >> 2) & 3;

      for (p=3; p>0; p--) 
      {
        z = data[p-1];
        y = data[p] -= MX;
      }

      z = data[3];
      y = data[0] -= MX;

    } while((sum -= kDELTA) != 0);
    
    return true;
  }

  std::cerr << "[ERROR] File block decryption failed on null data" << std::endl;
  return false;
}

/* 
 * Description: Decrypts a line of data. This line must conform to the length
 *              requirements and must be greater than the minimum line length.
 *              The line needs to be a divisor of 4 in terms of the number
 *              of characters in order to ensure that the line can be
 *              converted to a 32bit number for use with the XXTEA algorithm.
 *              It also must have at least 16 characters (4 32-bit numbers)
 *              which is the minimum for the XXTEA algorithm.
 *
 * Inputs: std::string line - the line to be decrypted, from the file
 *         bool* success - a bool pointer that can be set to return the
 *                         success status.
 * Output: std::string - the decrypted version of the input line.
 */
std::string FileHandler::decryptLine(std::string line, bool* success)
{
  uint32_t* compressed_data;
  int compressed_length;
  uint32_t decrypt_data [kENCRYPTION_MIN];
  std::string decrypted_line;
  int* line_data;
  int line_length;
  bool status = true;

  /* Convert line data into long to be decrypted */
  line_length = stringToInt(line, &line_data);
  compressed_data = intToLong(line_data, line_length);
  compressed_length = line_length / kASCII_IN_LONG;

  if(compressed_data != 0 && compressed_length >= kENCRYPTION_MIN)
  {
    /* Begin to decrypt the data, reverse access (opposite of encrypt) */
    for(int i = compressed_length - 1; i >= 0; i--)
    {
      /* Assemble array to decrypt */
      for(int j = 0; j < kENCRYPTION_MIN; j++)
        decrypt_data[j] = compressed_data[wrapNumber(i+j, compressed_length)];

      /* Decrypt snapshot of data */
      status &= decryptData(decrypt_data);
   
      /* Dump the decrypted data back into the array */
      for(int j = 0; j < kENCRYPTION_MIN; j++)
        compressed_data[wrapNumber(i+j, compressed_length)] = decrypt_data[j];
    }

    /* Convert the line back to a string, as long as status was succesful */
    if(status)
    {
      delete[] line_data;
      line_data = longToInt(compressed_data, compressed_length);
      decrypted_line = intToString(line_data, line_length, true);
    }
    else
    {
      decrypted_line = "";
    }
  }
  else
  {
    status = false;
    decrypted_line = "";
    std::cerr << "[ERROR] Invalid data from file for line decrypt." 
              << std::endl;
  }

  /* Clean up compressed pointer, if set */
  if(compressed_data != 0 || compressed_length > 0)
  {
    delete[] compressed_data;
    compressed_data = NULL;
  }

  /* Delete final pointer only if line length was set */
  if(line_data != 0 || line_length > 0)
  {
    delete[] line_data;
    line_data = NULL;
  }

  /* Set the success status determined above */
  if(success != 0)
    *success = status;

  return decrypted_line;
}

/* 
 * Description: Takes a sequence of 32 bit data (4 in the array) and encrypts
 *              the data, as per XXTEA algorithm. The data is returned on the
 *              same path and returns a status.
 *
 * Inputs: uint32_t* data - an array of 4 32 bit values to encrypt
 * Output: bool - the returned success status
 */
bool FileHandler::encryptData(uint32_t* data)
{
  uint32_t y, z, sum;
  unsigned p, rounds, e;

  if(data != 0)
  {
    rounds = kXXTEA_ROUNDS;
    sum = 0;
    z = data[3];

    do
    {
      sum += kDELTA;
      e = (sum >> 2) & 3;

      for(p=0; p < 3; p++)
      {
        y = data[p+1];
        z = data[p] += MX;
      }
    
      y = data[0];
      z = data[3] += MX;

    } while(--rounds);
    
    return true;
  }
  
  std::cerr << "[ERROR] File block encryption failed on null data" << std::endl;
  return false;
}

/* 
 * Description: Encrypts a line of data. This line must conform to the length
 *              requirements and must be greater than the minimum line length.
 *              The line needs to be a divisor of 4 in terms of the number
 *              of characters in order to ensure that the line can be
 *              converted to a 32bit number for use with the XXTEA algorithm.
 *              It also must have at least 16 characters (4 32-bit numbers)
 *              which is the minimum for the XXTEA algorithm.
 *
 * Inputs: std::string line - the line to be encrypted, from the file
 *         bool* success - a bool pointer that can be set to return the
 *                         success status.
 * Output: std::string - the encrypted version of the input line.
 */
std::string FileHandler::encryptLine(std::string line, bool* success)
{
  uint32_t* compressed_data;
  int compressed_length;
  uint32_t encrypt_data [kENCRYPTION_MIN];
  std::string encrypted_line;
  int* line_data;
  int line_length;
  bool status = true;

  /* Convert line data into long to be encrypted */
  line_length = stringToInt(line, &line_data, true);
  compressed_data = intToLong(line_data, line_length);
  compressed_length = line_length / kASCII_IN_LONG;

  if(compressed_data != 0 && compressed_length >= kENCRYPTION_MIN)
  {
    /* Begin to encrypt the data, reverse access (opposite of encrypt) */
    for(int i = 0; i < compressed_length; i++)
    {
      /* Assemble array to encrypt */
      for(int j = 0; j < kENCRYPTION_MIN; j++)
        encrypt_data[j] = compressed_data[wrapNumber(i+j, compressed_length)];

      /* Encrypt snapshot of data */
      status &= encryptData(encrypt_data);

      /* Dump the encrypted data back into the array */
      for(int j = 0; j < kENCRYPTION_MIN; j++)
        compressed_data[wrapNumber(i+j, compressed_length)] = encrypt_data[j];
    }

    /* Convert the line back to a string, as long as status was succesful */
    if(status)
    {
      delete[] line_data;
      line_data = NULL;
      line_data = longToInt(compressed_data, compressed_length);
      encrypted_line = intToString(line_data, line_length);
    }
    else
    {
      encrypted_line = "";
      std::cerr << "[ERROR] Invalid data from file for line encrypt." 
                << std::endl;
    }
  }
  else
  {
    status = false;
    encrypted_line = "";
    std::cerr << "[ERROR] Data compression size encrypt computation failed." 
              << std::endl;
  }

  /* Clean up compressed data */
  if(compressed_data != 0 || compressed_length > 0)
  {
   delete[] compressed_data;
   compressed_data = NULL;
  }

  /* Clean up line pointer only if line length was set */
  if(line_data != 0 || line_length > 0)
  {
   delete[] line_data;
   line_data = NULL;
  }

  /* Set the success status determined above */
  if(success != 0)
   *success = status;

  return encrypted_line;
}

/* 
 * Description: Closes the file stream pointer, if it was open.
 *
 * Inputs: none
 * Output: bool - the status if the file stream is not open after this class
 *                tries to close it.
 */
bool FileHandler::fileClose()
{
  if(file_stream.is_open())
    file_stream.close();

  /* Determine if close was successful */
  if(!file_stream.is_open())
    return true;

  /* If the strema close fails, notify the user */
  std::cerr << "[ERROR] File close with \"" << file_name << "\" failed." 
                                                         << std::endl;
  return false;
}

/* 
 * Description: Opens the file stream, with the stored file name within the
 *              class. 
 *
 * Inputs: none
 * Output: bool - status is the file stream is open after the open procedure
 */
bool FileHandler::fileOpen()
{
  /* Attempt to open the file stream (for either open or close) */
  if(file_write)
    file_stream.open(file_name_temp.c_str(), 
                     std::ios::out | std::ios::binary | std::ios::trunc);
  else
    file_stream.open(file_name.c_str(), std::ios::in | std::ios::binary);

  /* Determine if the open was successful */
  if(file_stream.good())
    return true;

  /* If the stream isn't good, reclose and return false */
  std::cerr << "[ERROR] File open with \"" << file_name << "\" failed." 
                                                        << std::endl;
  fileClose();
  return false;
}

/* 
 * Description: Converts an array of integers to an array of longs. This 
 *              combines 4 ints into one 32 bit long. They're restricted to
 *              ASCII character integers which are 8 bits long. This class
 *              will fail if the length of the integer array is not a divisor
 *              of 4.
 *
 * Inputs: int* line_data - the individual characters in the line split up.
 *         int length - the number of integers in the array
 * Output: uint32_t* - the converted array of longs. Length is length % 4
 */
uint32_t* FileHandler::intToLong(int* line_data, int length)
{
  /* Only move forward if the data is legitimate */
  if(line_data != 0 && length > 0 && length % 4 == 0)
  {
    uint32_t* long_data = new uint32_t[length / kASCII_IN_LONG];

    /* Convert all ints into length / 4 longs */
    for(int i = 0; i < length / kASCII_IN_LONG; i++)
    {
      long_data[i] = 0;

      /* Run through the sets of 4 ASCII in the int array */
      for(int j = 0; j < kASCII_IN_LONG; j++)
      {
        long_data[i] = long_data[i] << 8;
        long_data[i] |= line_data[i*kASCII_IN_LONG + j] & kMAX_ASCII;
      }
    }

    return long_data;
  }

  std::cerr << "[ERROR] IntToLong conversion of file data line failed." 
            << std::endl;
  return 0;
}

/* 
 * Description: Converts an array of integers into a string. The integers 
 *              represent ASCII codes and are converted into the appropriate
 *              character and appended to the return result.
 *
 * Inputs: int* line_data - the array of integers that defines the ASCII line
 *         int length - the length of the integer array / future string line
 *         bool decrypting - Is this call decrypting the data
 * Output: std::string - the converted line of data
 */
std::string FileHandler::intToString(int* line_data, int length, 
                                                     bool decrypting)
{
  bool end_reached = false;
  std::string line = "";
  int line_count = length - 1;

  /* Check for padding at the end, only if the data was encrypted */
  if(encryption_enabled && decrypting && length > 0)
  {
    do
    {
      if(line_data[line_count] >= kPADDING_ASCII)
        line_data[line_count] = kMAX_ASCII+1;
      else
        end_reached = true;
    } while(!end_reached && --line_count >= 0);
  }

  /* Get the string from the integer data, remove padding if needed */
  if(line_data != 0)
  {
    if(encryption_enabled && decrypting)
    {
      for(int i = 0; i < length; i++)
      {
        if(line_data[i] <= kMAX_ASCII)
        {
          char c = line_data[i];
          line.push_back(c);
        }
      }
    }
    else
    {
      for(int i = 0; i < length; i++)
      {
        int val = ((line_data[i] >> kINT_BIT_SHIFT) & kINT_BUFFER) 
                                                    + kENCRYPTION_PAD;
        line.push_back(val);

        val = (line_data[i] & kINT_BUFFER) + kENCRYPTION_PAD;
        line.push_back(val);
      }
    }
  }


  return line;
}

/* 
 * Description: Converts an array of longs into an array of integers where
 *              it splits one long into 4 integers. The long will be 
 *              converted as follows: [int0][int1][int2][int3] = [long].
 *              The length corresponds to the long array.
 *
 * Inputs: uint32_t* line_data - the long data, stored in an array
 *         int length - the length of the line_data array
 * Output: int* - the array of the split longs. Length is length*4
 */
int* FileHandler::longToInt(uint32_t* line_data, int length)
{
  /* Only move forward if the data is legitimate */
  if(line_data != 0 && length > 0)
  {
    int* int_data = new int[length * kASCII_IN_LONG];

    /* Convert all longs into an array of ints */
    for(int i = 0; i < length; i++)
    {
      uint32_t temp_long = line_data[i];

      /* Run through the one long and convert to 4 ints */
      for(int j = kASCII_IN_LONG - 1; j >= 0; j--)
      {
        int_data[i*kASCII_IN_LONG + j] = temp_long & kLONG_BUFFER;
        temp_long = temp_long >> kLONG_BIT_SHIFT;
      }
    }

    return int_data;
  }
  
  std::cerr << "[ERROR] LongToInt conversion of file data line failed." 
            << std::endl;
  return 0;
}

/* 
 * Description: Reads the line from the file stream, if the file stream is 
 *              not complete. It takes said line, and decrypts it, if the
 *              current line is encrypted. Otherwise, it just returns the
 *              line.
 *
 * Inputs: bool* done - the pointer for a return value if the file has been
 *                      completed.
 *         bool* success - the pointer for a return value if the file read
 *                         was successful.
 * Output: std::string - the line that was read. Empty if file is finished or
 *                       failed.
 */
std::string FileHandler::readLine(bool* done, bool* success)
{
  std::string line;

  if(done != 0)
    *done = false;
  if(success != 0)
    *success = true;

  if(available && !file_write)
  {
    if(getline(file_stream, line))
    {
      if(encryption_enabled)
      {
        return decryptLine(line, success);
      }

      return line;
    }

    if(done != 0)
      *done = true;
    return "";
  }

  if(success != 0)
    *success = false;
  return "";
}

/* 
 * Description: This call determines if the file that is being read conforms
 *              to the MD5 value and if the file has been unchanged. Will be
 *              called before a read is allowed to check if the file is valid.
 *              Only used when the file is encrypted.
 *
 * Inputs: none
 * Output: bool - returns if the file is valid, and unchanged
 */
bool FileHandler::readMd5()
{
  bool complete = false;
  std::string md5_value = "";
  bool success = true;

  /* Only proceed if file is in read mode and encryption is enabled */
  if(!file_write && encryption_enabled)
  {
    /* Clear pertinent data storage */
    file_data.clear();

    /* If the file was open, close it */
    if(available)
    {
      success &= fileClose();
      available = false;
    }

    /* Proceed to open the file for parsing */
    if(success)
      success &= fileOpen();

    /* Parse the file */
    if(success)
    {
      available = true;
      md5_value = readLine(&complete, &success);
      
      while(!complete && success)
        file_data.append(readLine(&complete, &success));
    }

    /* Check the MD5 */
    if(success && (MD5::compute(file_data) == md5_value))
      success = true;
    else
      success = false;
    
    /* Close the file */
    success &= fileClose();
    available = false;
  }
  else
  {
    std::cerr << "[ERROR] Md5 read failed due to system not in read mode."
              << std::endl;
    return false;
  }

  return success;
}

/* 
 * Description: Sets a temp file name for writing to during the duration of
 *              a write procedure. The file will be deleted after it is 
 *              completed and is only used to not overwrite the primary
 *              file unless the write sequence was successful.
 *
 * Inputs: none
 * Output: bool - returns if the temp file name setting call was successful
 */
bool FileHandler::setTempFileName()
{
  if(!available)
  {
    bool complete = false;
    int i = 0;
    std::string name;
  
    while(!complete && i < kFILE_NAME_LIMIT)
    {
      name = std::to_string(kFILE_START) + 
             std::to_string(kFILE_NAME_LIMIT + i);

      if(!fileExists(name))
        complete = true;
      else
        i++;
    }

    if(i < kFILE_NAME_LIMIT)
    {
      file_name_temp = name;
      return true;
    }
  }
  return false;
}

/* 
 * Description: Converts a string line to an array of integers where each
 *              integer represents one character in the string line (via
 *              ASCII conversion). It returns the length of the array.
 *
 * Inputs: std::string line - the line to convert to an array of ints
 *         int** line_data - the returned data of the converted string line
 *         bool encrypting - Set to true if this call is called during the
 *                           encrypting procedure.
 * Output: int - Returns the length of the line_data array
 */
int FileHandler::stringToInt(std::string line, int** line_data, bool encrypting)
{
  int padding_length = 0;
  int final_length = line.length();

  /* Check if additional padding is needed, only if data will be encrypted */
  if(encryption_enabled && encrypting)
  {
    if(final_length < kMIN_LINE)
      padding_length = kMIN_LINE - final_length;
    else if(final_length % kASCII_IN_LONG != 0)
      padding_length = kASCII_IN_LONG - final_length % kASCII_IN_LONG;
  }

  /* Add the additional padding */
  for(int i = 0; i < padding_length; i++)
    line.push_back(kPADDING_ASCII + i);
  final_length += padding_length;

  /* Allocate appropriate space, if size is greater than 0 */
  if(final_length > 0)
  {
    /* Convert the line */
    if(encryption_enabled && !encrypting) 
    {
      final_length /= 2; /* Half length since combining two ints to one */

      *line_data = new int[final_length];
      
      for(int i = 0; i < final_length; i++)
      {
        (*line_data)[i] = ((line.at(i*2) - kENCRYPTION_PAD) 
                                               & kINT_BUFFER) << kINT_BIT_SHIFT;
        (*line_data)[i] |= (line.at(i*2+1) - kENCRYPTION_PAD) & kINT_BUFFER;
      }
    }
    else
    {
      *line_data = new int[final_length];

      for(int i = 0; i < final_length; i++)
        (*line_data)[i] = line.at(i);
    }

    return final_length;
  }

  std::cerr << "[ERROR] StringToInt conversion of file data line failed." 
            << std::endl;
  return 0;
}

/* 
 * Description: Returns the index in the file stream to the top of the file,
 *              for re-reading or writing the data.
 *
 * Inputs: none
 * Output: bool - returns if the file stream is available to shift the pointer
 */
bool FileHandler::topOfFile()
{
  if(available)
  {
    file_stream.seekg(0);
    return true;
  }
  return false;
}

/* 
 * Description: Writes the given line to the file. If the line is to be 
 *              decrypted, that procedure is done first. 
 *
 * Inputs: std::string line - the line to write to the file, if it exists
 *         bool md5_line - write indicates md5, which won't be saved
 * Output: bool - returns if the write sequence was successful
 */
bool FileHandler::writeLine(std::string line, bool md5_line)
{
  bool success = true;
  std::string new_line;

  if(available && file_write)
  {
    /* Add to the QByteArray of currently written data */
    if(!md5_line)
      file_data.append(line);

    /* Determine if the line should be encrypted or not */
    if(encryption_enabled)
      new_line = encryptLine(line, &success);
    else
     new_line = line;

    /* Write the line to the file */
    file_stream << new_line << std::endl;

    return success;
  }
  return false;
}

/* 
 * Description: A wrap number call. This takes a value and a limit and wraps
 *              it around to determine what the value should be. For example:
 *                value=15,limit=10 -> 5
 *                value=27,limit=10 -> 7
 *                value=27,limit=5 -> 2
 *
 * Inputs: int value - the value to be wrapped
 *         int limit - the maximum value that the value can be
 * Output: int - the value, after the limit was applied.
 */
int FileHandler::wrapNumber(int value, int limit)
{
  if(limit <= 0)
  {
    std::cerr << "[ERROR] Invalid limit (" << limit << ") in wrapping call." 
                                                    << std::endl;
    return -1;
  }
  else if(value < 0)
  {
    std::cerr << "[ERROR] Invalid value (" << value << ") in wrapping call." 
                                                    << std::endl;
    return -1;
  }
  else if(value >= limit)
  {
    return value - (value / limit) * limit;
  }
  return value;
}

/*
 * Description: Returns a node that contains a XMLText* node that is downstream
 *              of the current given node. This returns NULL if one cannot be
 *              found.
 *
 * Inputs: TinyXML2::XMLNode* starting_node - a starting search node
 * Output: TinyXML2::XMLNode* - the pointer to the found node
 */
TinyXML2::XMLNode* FileHandler::xmlNextData(TinyXML2::XMLNode* starting_node)
{
  TinyXML2::XMLNode* node = starting_node;
  bool search_done = false;
  
  if(node != NULL)
  {
    /* Search through until data node is found */
    while(!search_done)
    {
      /* Check to see if the node is data */
      if(node->ToText() != NULL)
        search_done = true;
      /* Check to see if the node has children to be parsed */
      else if(node->FirstChild() != NULL)
        node = node->FirstChild();
      /* Otherwise, check to see if it has siblings */
      else if(node->NextSibling() != NULL)
        node = node->NextSibling();
      /* Worst case, return to the parent (reversing) */
      else if(node->Parent() != NULL)
      {
        node = xmlNextNode(node);
        if(node == NULL)
          search_done = true;
      }
      /* If all fails, the call has finished */
      else
      {
        node = NULL;
        search_done = true;
      }
    }
  }
  
  return node;
}

/*
 * Description: Finds the next node that isn't a child but either a sibling
 *              or a parent's sibling to the starting node. Returns NULL if one
 *              cannot be found.
 *
 * Inputs: TinyXML2::XMLNode* starting_node - a starting search node
 * Output: TinyXML2::XMLNode* - the pointer to the found node
 */
TinyXML2::XMLNode* FileHandler::xmlNextNode(TinyXML2::XMLNode* starting_node)
{
  TinyXML2::XMLNode* node = starting_node;
  bool search_done = false;

  /* Only proceed if the node is non-NULL */
  if(node != NULL)
  {
    /* Search for the next sibling or parent sibling node */
    while(!search_done)
    {
      /* If the next sibling isn't null, a node has been found */
      if(node->NextSibling() != NULL)
      {
        node = node->NextSibling();
        search_done = true;
      }
      /* Otherwise, if the parent isn't null, move the search there */
      else if(node->Parent() != NULL)
      {
        node = node->Parent();
      }
      /* Otherwise, there are no more nodes. End search */
      else
      {
        node = NULL;
        search_done = true;
      }
    }
  }
  
  return node;
}

/* 
 * Description: Starts the XML sequence. For writing, it sets the formatting
 *              and appropriate header detail. For the reading, it parses
 *              all the data in the file to move it into the QXmlStreamReader.
 *
 * Inputs: none
 * Output: bool - returns if the call was successful.
 */
bool FileHandler::xmlReadStart()
{
  std::string data = "";
  bool done = false;
  bool success = true;

  if(xml_document != 0)
  {
    while(!done && success)
      data.append(readLine(&done, &success));
    
    /* Attempt to parse the document and then set the pointer to the head */
    success &= !xml_document->Parse(data.c_str());
    if(success)
      file_date = xmlToHead();
    
    return success;
  }
  
  return false;
}

/* 
 * Description: Wrap up the XML sequence before proceding to write the data to
 *              the file. This includes writing the end to all XML tags and 
 *              writing the data to the file.
 *
 * Inputs: none
 * Output: bool - Returns if the ending procedure was successful and ran.
 */
bool FileHandler::xmlWriteEnd()
{
  bool success = true;
  
  if(available)
  {
    /* Set up the printer and output the document */
    TinyXML2::XMLPrinter printer;
    xml_document->Print(&printer);
    std::string xml_output(printer.CStr());
    
    /* Split it based on the new line character */
    std::vector<std::string> output_lines = Helpers::split(xml_output, '\n');
    
    /* Write each line to the file */
    for(uint16_t i = 0; i < output_lines.size(); i++)
      success &= writeLine(output_lines[i]);

    return success;
  }
  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Returns the date stored in this class. It's in string format,
 *              like the following: [Month].[Day].[Year] [Hour]:[Minute]
 *              For example: 04.20.2013 09:56
 *
 * Inputs: none
 * Output: std::string - the date string
 */
std::string FileHandler::getDate()
{
  return file_date;
}

/* 
 * Description: Returns the file name that this class reads and writes from.
 *
 * Inputs: none
 * Output: std::string - the filename string
 */
std::string FileHandler::getFilename()
{
  return file_name;
}

/* 
 * Description: Returns the file type that this class is configured for.
 *
 * Inputs: none
 * Output: FileType - the file type classification (REGULAR, XML)
 */
FileHandler::FileType FileHandler::getFileType()
{
  return file_type;
}

/* 
 * Description: Returns if the class is available to read or write from.
 *              If false is returned, the read and write calls will not 
 *              work since the file has not been opened and configured.
 *
 * Inputs: none
 * Output: bool - if the class is available for reading and writing
 */
bool FileHandler::isAvailable()
{
  return available;
}

/* 
 * Description: Check if the class is configured for encryption to be enabled.
 *
 * Inputs: none
 * Output: bool - if the class is enabled for encryption
 */
bool FileHandler::isEncryptionEnabled()
{
  return encryption_enabled;
}

/* 
 * Description: Check if the class is configured for writing (or reading)
 *
 * Inputs: none
 * Output: bool - false = reading, true = writing
 */
bool FileHandler::isWriteEnabled()
{
  return file_write;
}

/* 
 * Description: Reads a regular line of data from the file that has been
 *              configured for this class. This method only works for REGULAR
 *              file configurations.
 *
 * Inputs: bool* done - the pointer to a bool variable to flag if the read
 *                      sequence is done and there is no more data to read
 *                      from the file.
 *         bool* success - returns if the call was successful
 * Output: std::string - the read data from the file
 */
std::string FileHandler::readRegularLine(bool* done, bool* success)
{
  if(done != 0)
    *done = false;

  if(file_type == REGULAR && !file_write)
    return readLine(done, success);

  if(success != 0)
    *success = false;
  return "";
}

/* 
 * Description: Reads the XML data from the appropriate file. The call will
 *              only succeed if it has been started and then if the class is
 *              configured for XML. The return will send back an XmlData class
 *              that will contain all the tags leading up the data as well as
 *              said data. See that class for more information on the data
 *              that can be accessed.
 *
 * Inputs: bool* done - returns if the file has been read and the sequence is
 *                      completed.
 *         bool* success - returns if the call was successful
 * Output: XmlData - the output data from the XML read. See this class for
 *                   more information on parsing the data
 */
XmlData FileHandler::readXmlData(bool* done, bool* success)
{
  XmlData data;
  bool failed = false;
  bool finished = false;
  
  /* Only pass through if the XML can proceed */
  if(available && file_type == XML && !file_write)
  {
    /* Try and find the next node of data */
    xml_node = xmlNextData(xml_node);
    
    /* Check to see if data was acquired */
    if(xml_node != NULL && xml_node->ToText() != NULL)
    {
      TinyXML2::XMLNode* temp_node = xml_node->Parent();

      /* Loop through all the parent nodes till the top is reached */
      while(temp_node != 0)
      {
        /* Convert the node to an element -> only elements matter */
        std::string attribute_name = "";
        std::string attribute_value = "";
        TinyXML2::XMLElement* element = temp_node->ToElement();
        
        if(element != NULL)
        {
          /* Get the attribute information, if relevant */
          const TinyXML2::XMLAttribute* attribute = element->FirstAttribute();
          if(attribute != NULL)
          {
            attribute_name = attribute->Name();
            attribute_value = attribute->Value();
          }
          
          /* Prepend the data */
          data.addElement(element->Name(), attribute_name, attribute_value);
        }
        
        /* Move to the next element node */
        temp_node = temp_node->Parent();
      }
      
      /* Once complete, add the data */
      data.flipElements();
      data.addData(xml_node->Value());
      
      /* Shift off of the data node. If the next node is done, call is 
       * finished */
      xml_node = xmlNextNode(xml_node);
      if(xml_node == NULL)
        finished = true;
    }
    else
    {
      failed = false;
      finished = true;
    }
  }
  else
  {
    failed = true;
  }
  
  /* Set status', if the pointers are available */
  if(done != NULL)
    *done = finished;
  if(success != NULL)
    *success = !failed;

  return data;
}

/* 
 * Description: Sets the encryption status in the class. If enabled, only 
 *              encrypted files can be read from and vice versa.
 *
 * Inputs: bool enabe - if encryption should be enabled
 * Output: bool - if the set was successful. Only fails if the class is
 *                currently off, which can be set after calling stop()
 */
bool FileHandler::setEncryptionEnabled(bool enable)
{
  if(!available)
  {
    encryption_enabled = enable;
    return true;
  }
  return false;
}

/* 
 * Description: Sets the filename string in the class. The read call will fail
 *              if the file doesn't exist or it doesn't conform to the 
 *              standard, as written from the write side of this class.
 *
 * Inputs: std::string path - the path to read or write from. Only works if the
 *                            path is not empty.
 * Output: bool - if the set was successful. Only fails if the class is 
 *                currently off, which can be set after calling stop()
 */
bool FileHandler::setFilename(std::string path)
{
  if(!available)
  {
    if(!path.empty())
    {
      file_name = path;
      return true;
    }
  }
  return false;
}

/* 
 * Description: Sets the file type that this class is configured for.
 *
 * Inputs: FileType type - the type of the file to use (REGULAR, XML)
 * Output: bool - if the set was successful. Only fails if the class is 
 *                currently off, which can be set after calling stop()
 */
bool FileHandler::setFileType(FileType type)
{
  if(!available)
  {
    file_type = type;
    return true;
  }
  return false;
}

/* 
 * Description: Sets if the class is being used for reading or writing.
 *
 * Inputs: bool enable - If true, write is enabled. Else, read is enabled.
 * Output: bool - if the set was successful. Only fails if the class is
 *                currently off, which can be set after calling stop()
 */
bool FileHandler::setWriteEnabled(bool enable)
{
  if(!available)
  {
    file_write = enable;
    return true;
  }
  return false;
}

/* 
 * Description: Start the class sequence to enable the set settings. After
 *              this call, the class can be accessed to write (or read) where
 *              appropriate (depending on the settings). This call will stop
 *              the class (with a fail) if it was running previously to wipe
 *              out the previous settings.
 *
 * Inputs: none
 * Output: bool - returns if the call was successful and if the file is now
 *                available to be read or written from. If it fails, the
 *                file will be unavailable.
 */
bool FileHandler::start()
{
  bool success = true;

  if(!file_name.empty())
  {
    /* Stop the system first if it's already running */
    if(available)
      success &= stop(true);

    /* Clear the file data array */
    file_data.clear();
   
    /* If file_write, determine temporary file name */
    if(file_write)
      success &= setTempFileName();

    /* If the system is in read and encryption, check validity of file */
    if(!file_write && encryption_enabled)
      success &= readMd5();
    
    /* Open the file stream */
    if(success)
      success &= fileOpen();

    /* Write the success status, if available */
    if(success)
    {
      available = true;

      /* For a readable file with encryption, first line is Md5 -> throw away */
      if(!file_write && encryption_enabled)
        readLine();
    }

    /* If the file type is XML, open the QXmlStreams and initialize XML */
    if(file_type == XML)
    {
      xml_document = new TinyXML2::XMLDocument();

      if(file_write)
      {
        xml_node = xml_document;
      }
      else
      {
        success &= xmlReadStart();
      }
    }

    /* Write a blank MD5 sequence, if applicable */
    if(success && file_write && encryption_enabled)
      success &= writeLine(MD5::compute(file_data), true);

    /* Write starting date, if applicable */
    if(success && file_write)
    {
      std::time_t current_time = time(0);
      file_date = std::ctime(&current_time);
      file_date.pop_back();

      if(file_type == REGULAR)
        success &= writeLine(file_date);
      else
        success &= writeXmlData("date", VarType::STRING, file_date);
    }

    /* Read off starting date, if applicable */
    if(success && !file_write && file_type == REGULAR)
      file_date = readRegularLine();

    /* Stop the program if there is any problems and halt operation */
    if(!success)
      stop(true);

    return success;
  }

  std::cerr << "[ERROR] Filename is empty and unset" << std::endl;
  return false;
}

/* 
 * Description: Stops the class, after it is was run. If the class was never
 *              started, this call does nothing except for a reclearing of 
 *              all data elements stored within the class. Once the class
 *              is stopped, no more reads and writes can take place until it
 *              is started again. The class must be stopped before settings
 *              can be changed, such as setEncryptionEnabled, etc.
 *
 * Inputs: bool failed - states if the calls failed and if the temporary
 *                       writing should be thrown out. If false, the temporary
 *                       write file is copied to overwrite the actual filename
 * Output: bool - returns if the stop procedure was successful
 */
bool FileHandler::stop(bool failed)
{
  bool success = true;

  // /* Handle ending of XML file if file in write and XML */
  if(file_write && file_type == XML)
    xmlWriteEnd();

  /* MD5 write - if encryption is enabled */
  if(file_write && encryption_enabled && available)
  {
    topOfFile();
    success &= writeLine(MD5::compute(file_data), true);
  }

  /* Close the file stream */
  success &= fileClose();
  file_date = "";
    
  /* If success, reopen the class availability */
  if(success)
  {
    available = false;

    /* If on write and successful, remove temporary file */
    if(file_write && !failed)
      fileRename(file_name_temp, file_name, true);
    else if(file_write)
      fileDelete(file_name_temp);
  }

  /* Do the final clean up once everything is stopped */
  cleanUp();

  return success;
}

/* 
 * Description: Writes a regular line of data. Handles if the data is
 *              encrypted or not. If the internal file type is not set to
 *              REGULAR, this call fails.
 *
 * Inputs: std::string line - the line to write to the file
 * Output: bool - returns if the procedure was successful
 */
bool FileHandler::writeRegularLine(std::string line)
{
  if(file_type == REGULAR && file_write)
    return writeLine(line);
  return false;
}

/* 
 * Description: Writes XML data to the class. This call only works if the 
 *              class has been configured for XML file types. It takes an
 *              element name, a Variable type for data (BOOLEAN, INTEGER,
 *              FLOAT, or STRING) and the appropriate data. 
 *
 * Inputs: std::string element - the element name indicator
 *         VarType type - the type of variable being inputted into XML
 *         std::string data - the data encapsulated in XML
 * Output: bool - returns if the writing of XML data was successful
 */
bool FileHandler::writeXmlData(std::string element, 
                               VarType type, std::string data)
{
  /* Call only works if the file type is XML */
  if(available && file_type == XML && file_write)
  {
    /* Only move foward if element string isn't empty */
    if(!element.empty() && !data.empty())
    {
      TinyXML2::XMLElement* data_node = 
                                      xml_document->NewElement(element.c_str());
      data_node->SetAttribute("type", static_cast<int>(type));
      data_node->InsertEndChild(xml_document->NewText(data.c_str()));
      
      xml_node->InsertEndChild(data_node);
      return true;
    }
  }
  
  return false;
}

/*
 * Description: Writes boolean XML data, using the internal class paradigm. Just
 *              recalls the 3 parameter writeXmlData() with the appropriate
 *              settings + casting.
 *
 * Inputs: std::string element - the string xml element
 *         bool data - the data, as a boolean
 * Output: bool - status if write was successful
 */
bool FileHandler::writeXmlData(std::string element, bool data)
{
  if(data)
    return writeXmlData(element, BOOLEAN, "true");
  return writeXmlData(element, BOOLEAN, "false");
}

/*
 * Description: Writes float XML data, using the internal class paradigm. Just
 *              recalls the 3 parameter writeXmlData() with the appropriate
 *              settings + casting.
 *
 * Inputs: std::string element - the string xml element
 *         float data - the data, as a floating point number
 * Output: bool - status if write was successful
 */
bool FileHandler::writeXmlData(std::string element, float data)
{
  return writeXmlData(element, FLOAT, std::to_string(data));
}

/*
 * Description: Writes integer XML data, using the internal class paradigm. Just
 *              recalls the 3 parameter writeXmlData() with the appropriate
 *              settings + casting.
 *
 * Inputs: std::string element - the string xml element
 *         int data - the data, as an integer number
 * Output: bool - status if write was successful
 */
bool FileHandler::writeXmlData(std::string element, int data)
{
  return writeXmlData(element, INTEGER, std::to_string(data));
}

/*
 * Description: Writes string XML data, using the internal class paradigm. Just
 *              recalls the 3 parameter writeXmlData() with the appropriate
 *              settings + casting.
 * Note: if called directly with a "string" as the parameter, this will cast
 *       to the boolean call since a c_str is compile time sent to the boolean
 *       call. It can be fixed by actually creating a string variable and using
 *       that as the input.
 *
 * Inputs: std::string element - the string xml element
 *         std::string data - the data, as a string
 * Output: bool - status if write was successful
 */
bool FileHandler::writeXmlData(std::string element, std::string data)
{
  return writeXmlData(element, STRING, data);
}
  
/* 
 * Description: Writes the XML starting element tag, to encapsulate data in
 *              the XML class. There are option key - value pairs to add to
 *              the end of the start element.
 *              For example: element="person", key="gender", value="male"
 *                           <person gender="male">
 *
 * Inputs: std::string element - the element name for the tag
 *         std::string key - the key identifier for the element (default "")
 *         std::string value - the value tied to said key (default "")
 * Output: bool - returns if the writing of the XML element was successful
 */
bool FileHandler::writeXmlElement(std::string element, 
                                  std::string key, std::string value)
{
  /* Call only works if the file type is XML */
  if(available && file_type == XML && file_write)
  {
    /* Only move forward if element string isn't empty */
    if(!element.empty())
    {
      TinyXML2::XMLElement* element_node = 
                                      xml_document->NewElement(element.c_str());
      
      /* Set the attribute if the key isn't empty */
      if(!key.empty())
        element_node->SetAttribute(key.c_str(), value.c_str());
        
      /* Append the element, and shift the node up */
      xml_node->InsertEndChild(element_node);
      xml_node = element_node;
      return true;
    }
  }
  return false;
}

/* 
 * Description: Writes the XML starting element tag, to encapsulate data in
 *              the XML class. This is the same as the string parameter one but
 *              allows for the key value to be an integer (convenience function)
 *
 * Inputs: std::string element - the element name for the tag
 *         std::string key - the key identifier for the element (default "")
 *         int value - the value tied to said key (default 0)
 * Output: bool - returns if the writing of the XML element was successful
 */
bool FileHandler::writeXmlElement(std::string element, 
                                  std::string key, int value)
{
  return writeXmlElement(element, key, std::to_string(value));
}

/* 
 * Description: Ends the element in the XML sequence, if there is an element
 *              to complete. If all is triggered, the XML class is closed
 *              up entirely. 
 *              For example: <person> -> writeXmlElementEnd() -> </person>
 *
 * Inputs: bool all - if all, all open XML elements are closed (except the 
 *                    overall identifier)
 * Output: bool - if the close sequence was successful
 */
bool FileHandler::writeXmlElementEnd(bool all)
{
  /* Call only works if the file type is XML */
  if(available && file_type == XML && file_write)
  {
    /* If root return requested, go all the way back */
    if(all)
    {
      xml_node = xml_document;
    }
    /* Otherwise, just go back one layer */
    else
    {
      TinyXML2::XMLNode* parent_node = xml_node->Parent();
      if(parent_node != 0)
        xml_node = parent_node;
    }
    
    return true;
  }
  return false;
}

/*
 * Description: If the file handler is currently in XML read mode, this will
 *              push the pointer to the head of the list where it can be parsed
 *              again, if needed. Returns the date from the top of the XML file.
 *
 * Inputs: none
 * Output: std::string - the string date from the top of the file, if exists
 */
std::string FileHandler::xmlToHead()
{
  std::string date = "";
  
  if(available && !file_write && file_type == XML)
  {
    /* Return the node to the root of the document */
    xml_node = xml_document->RootElement();

    /* If xml node is element, named date, and has a text, extract the date */
    if(xml_node != NULL && xml_node->ToElement() != NULL)
    {
      TinyXML2::XMLElement* element = xml_node->ToElement();
      std::string category = element->Value();
      if(category == "date" && element->GetText() != NULL)
      {
        date = element->GetText();
        xml_node = xml_node->NextSibling();
      }
    }
  }
  
  return date;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: A function to delete a given filename. Only works if the file
 *              already exists.
 *
 * Inputs: std::string filename - the filename to delete
 * Output: bool - status if deletion was successful
 */
bool FileHandler::fileDelete(std::string filename)
{
  if(fileExists(filename))
    return !std::remove(filename.c_str());
  return false;
}

/* 
 * Description: A function to check if the file exists on the file directory.
 *
 * Inputs: std::string filename - the filename to determine if it exists.
 * Output: bool - does the file exist status
 */
bool FileHandler::fileExists(std::string filename)
{
  std::ifstream test_file(filename.c_str());
  return test_file;
}

/* 
 * Description: A file rename call to take an old filename and change it to a
 *              new filename. If the new filename already exists, the
 *              the overwrite call must be set or this won't rename the file.
 *
 * Inputs: std::string old_filename - the filename to change
 *         std::string new_filename - the new filename name
 *         bool overwrite - overwrite the new_filename, if it exists
 * Output: bool - returns if the rename procedure was successful
 */
bool FileHandler::fileRename(std::string old_filename, std::string new_filename, 
                             bool overwrite)
{
  if(overwrite)
    fileDelete(new_filename);

  if(fileExists(old_filename) && !fileExists(new_filename))
    return !std::rename(old_filename.c_str(), new_filename.c_str());
  return false;
}
