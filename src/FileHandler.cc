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
#include "FileHandler.h"
/* Constant Declarations */

const uint32_t FileHandler::kKEY[] = {1073676287,27644437,2971215073,94418953};

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
FileHandler::FileHandler()
{
  available = FALSE;
  depth = 0;
  
  setEncryptionEnabled(FALSE);
  setFilename("");
  setFileType(REGULAR);
  setWriteEnabled(FALSE);
}

FileHandler::~FileHandler()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
/*
 * CHECKED:
 * Returns FALSE only if data is unset
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
    
    return TRUE;
  }

  qDebug() << "[ERROR] File block decryption failed on null data";
  return FALSE;
}

/* 
 * CHECKED:
 * Success status will determine if this call worked. If failed, it will
 *  typically be an empty string ("")
 */
QString FileHandler::decryptLine(QString line, bool* success)
{
  uint32_t* compressed_data;
  int compressed_length;
  uint32_t decrypt_data [kENCRYPTION_MIN];
  QString decrypted_line;
  int* line_data;
  int line_length;
  bool status = TRUE;

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
      decrypted_line = intToString(line_data, line_length, TRUE);
    }
    else
    {
      decrypted_line = "";
    }
  }
  else
  {
    status = FALSE;
    decrypted_line = "";
    qDebug() << "[ERROR] Invalid data into file decrypt line function.";
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
 * CHECKED:
 * Returns false only if data is unset
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
    
    return TRUE;
  }
  
  qDebug() << "[ERROR] File block encryption failed on null data";
  return FALSE;
}

/* 
 * CHECKED:
 * Returns success status based on encryption. TRUE is success.
 *  Often, the string will be empty if it fails
 */
QString FileHandler::encryptLine(QString line, bool* success)
{
  uint32_t* compressed_data;
  int compressed_length;
  uint32_t encrypt_data [kENCRYPTION_MIN];
  QString encrypted_line;
  int* line_data;
  int line_length;
  bool status = TRUE;

  /* Convert line data into long to be encrypted */
  line_length = stringToInt(line, &line_data, TRUE);
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
      qDebug() << "[ERROR] Invalid data into file encrypt line function.";
    }
  }
  else
  {
    status = FALSE;
    encrypted_line = "";
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
 * CHECKED:
 * Returns TRUE if file closed successfuly.
 */
bool FileHandler::fileClose()
{
  file_stream.close();

  /* Determine if close was successful */
  if(!file_stream.is_open())
    return TRUE;

  /* If the strema close fails, notify the user */
  qDebug() << "[ERROR] File close with \"" << file_name << "\" failed.";
  return FALSE;
}

/*
 * CHECKED:
 * Returns TRUE if open stream was successful and stream is good
 */
bool FileHandler::fileOpen()
{
  /* Attempt to open the file stream (for either open or close) */
  if(file_write)
    file_stream.open(file_name.toStdString().c_str(), 
                     std::ios::out | std::ios::binary | std::ios::trunc);
  else
    file_stream.open(file_name.toStdString().c_str(), 
                     std::ios::in | std::ios::binary);

  /* Determine if the open was successful */
  if(file_stream.good())
    return TRUE;

  /* If the stream isn't good, reclose and return FALSE */
  qDebug() << "[ERROR] File open with \"" << file_name << "\" failed.";
  fileClose();
  return FALSE;
}

/* 
 * CHECKED:
 * Requires the line_data set to a memory location, length > 0 on line_data
 *  and length to be a factor of 4. Otherwise, 0 is returned.
 *
 * Data uninitialized otherwise. Ie. no free required
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

  qDebug() << "[ERROR] IntToLong conversion in FileHandler failed.";
  return 0;
}

/* 
 * CHECKED:
 * Returns empty string if length is 0 or line_data is uninitialized
 */
QString FileHandler::intToString(int* line_data, int length, bool decrypting)
{
  bool end_reached = FALSE;
  QString line = "";
  int line_count = length - 1;

  /* Check for padding at the end, only if the data was encrypted */
  if(encryption_enabled && decrypting && length > 0)
  {
    do
    {
      if(line_data[line_count] >= kPADDING_ASCII)
        line_data[line_count] = kMAX_ASCII+1;
      else
        end_reached = TRUE;
    } while(!end_reached && --line_count >= 0);
  }

  /* Get the string from the integer data, remove padding if needed */
  if(line_data != 0)
  {
    for(int i = 0; i < length; i++)
    {
      if(line_data[i] <= kMAX_ASCII)
      {
        char c = (char)line_data[i];
        line.append(QChar(c));
      }
    }
  }

  return line;
}

/*
 * CHECKED:
 * Does not allocate memory if line_data unset or length is <= 0
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
  
  qDebug() << "[ERROR] LongToInt conversion in FileHandler failed.";
  return 0;
}

/*
 * CHECKED:
 * Does not allocate memory if the length of line is 0 and not encrypting
 */
int FileHandler::stringToInt(QString line, int** line_data, bool encrypting)
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
    line.append(QChar(kPADDING_ASCII + i));
  final_length += padding_length;

  /* Allocate appropriate space, if size is greater than 0 */
  if(final_length > 0)
  {
    *line_data = new int[final_length];

    /* Convert the line */
    for(int i = 0; i < final_length; i++)
      (*line_data)[i] = line.at(i).unicode();

    return final_length;
  }

  qDebug() << "[ERROR] StringToInt conversion in FileHandler failed.";
  return 0;
}

/*
 * CHECKED:
 * Only fails if value or limit is less than 0. Failure is indicated by a
 *  -1 in the return call.
 */
int FileHandler::wrapNumber(int value, int limit)
{
  if(limit <= 0)
  {
    qDebug() << "[ERROR] Invalid limit (" << limit << ") in wrapping call.";
    return -1;
  }
  else if(value < 0)
  {
    qDebug() << "[ERROR] Invalid value (" << value << ") in wrapping call.";
    return -1;
  }
  else if(value >= limit)
  {
    return value - (value / limit) * limit;
  }
  return value;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

QByteArray FileHandler::computeMd5()
{
  if(available && file_write)
  {
    return QCryptographicHash::hash(file_data, 
                                    QCryptographicHash::Md5).toHex();
  }

  return "";
}

QString FileHandler::getFilename()
{
  return file_name;
}

FileHandler::FileType FileHandler::getFileType()
{
  return file_type;
}

bool FileHandler::isAvailable()
{
  return available;
}

bool FileHandler::isEncryptionEnabled()
{
  return encryption_enabled;
}

bool FileHandler::isWriteEnabled()
{
  return file_write;
}

QString FileHandler::readLine(bool* done, bool* success)
{
  std::string line;
  
  if(done != 0)
    *done = FALSE;
  if(success != 0)
    *success = TRUE;

  if(available && !file_write && file_type == REGULAR)
  {
    if(getline(file_stream, line))
    {
      if(encryption_enabled)
      {
        return decryptLine(QString::fromStdString(line), success);
      }

      return QString::fromStdString(line);
    }

    if(done != 0)
      *done = TRUE;
    return "";
  }

  if(success != 0)
    *success = FALSE;
  return "";
}

void FileHandler::setEncryptionEnabled(bool enable)
{
  encryption_enabled = enable;
}

void FileHandler::setFilename(QString path)
{
  file_name = path;
}

void FileHandler::setFileType(FileType type)
{
  file_type = type;
}

void FileHandler::setWriteEnabled(bool enable)
{
  file_write = enable;
}

bool FileHandler::start()
{
  bool success = TRUE;

  /* Stop the system first if it's already running */
  if(available)
    success &= stop();

  /* Clear the file data array */
  file_data.clear();

  /* Open the file stream */
  success &= fileOpen();

  /* Determine the class availablility based on the success status */
  if(success)
    available = TRUE;

  return success;
}

bool FileHandler::stop()
{
  bool success = TRUE;

  /* MD5 Test */
  if(file_write && encryption_enabled)
  {
    qDebug() << file_data;
    qDebug() << computeMd5();
  }

  /* Close the file stream */
  success &= fileClose();

  /* If success, reopen the class availability */
  if(success)
    available = FALSE;

  return success;
}

bool FileHandler::writeLine(QString line)
{
  bool success = TRUE;
  QString new_line;

  if(available && file_write && file_type == REGULAR)
  {
    /* Add to the QByteArray of currently written data */
    file_data.append(line);

    /* Determine if the line should be encrypted or not */
    if(encryption_enabled)
      new_line = encryptLine(line, &success);
    else
      new_line = line;
   
    /* Write the line to the file */
    file_stream << new_line.toStdString() << std::endl;

    return success;
  }

  return FALSE;
}
