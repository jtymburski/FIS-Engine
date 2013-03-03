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
  return FALSE;
}

QString FileHandler::decryptLine(QString line, bool* success)
{
  uint32_t* compressed_data;
  int compressed_length;
  uint32_t decrypt_data [4];
  int* line_data;
  int line_length;
  QString decrypted_line;

  /* Set the success status if bool pointer is set */
  if(success != 0)
    *success = TRUE;

  /* Convert line data into long to be decrypted */
  line_length = stringToInt(line, &line_data);
  compressed_data = intToLong(line_data, line_length);
  compressed_length = line_length / 4;

  /* Begin to decrypt the data, reverse access (opposite of encrypt) */
  for(int i = compressed_length - 1; i >= 0; i--)
  {
    for(int j = 0; j < 4; j++)
      decrypt_data[j] = compressed_data[wrapNumber(i+j, compressed_length)];

    decryptData(decrypt_data); // TODO: Has return
   
    for(int j = 0; j < 4; j++)
      compressed_data[wrapNumber(i+j, compressed_length)] = decrypt_data[j];
  }

  /* Convert the line back to a string */
  delete[] line_data;
  line_data = longToInt(compressed_data, compressed_length);
  decrypted_line = intToString(line_data, line_length, TRUE);

  /* Clean Up */
  delete[] compressed_data;
  delete[] line_data;

  return decrypted_line;
}

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
  return FALSE;
}

QString FileHandler::encryptLine(QString line, bool* success)
{
  uint32_t* compressed_data;
  int compressed_length;
  uint32_t encrypt_data [4];
  int* line_data;
  int line_length;
  QString encrypted_line;

  /* Set the success status if bool pointer is set */
  if(success != 0)
    *success = TRUE;

  /* Convert line data into long to be encrypted */
  line_length = stringToInt(line, &line_data, TRUE);
  compressed_data = intToLong(line_data, line_length);
  compressed_length = line_length / 4;

  /* Begin to encrypt the data, reverse access (opposite of encrypt) */
  for(int i = 0; i < compressed_length; i++)
  {
    for(int j = 0; j < 4; j++)
      encrypt_data[j] = compressed_data[wrapNumber(i+j, compressed_length)];

    encryptData(encrypt_data); // TODO: Has return
   
    for(int j = 0; j < 4; j++)
      compressed_data[wrapNumber(i+j, compressed_length)] = encrypt_data[j];
  }

  /* Convert the line back to a string */
  delete[] line_data;
  line_data = longToInt(compressed_data, compressed_length);
  encrypted_line = intToString(line_data, line_length);

  /* Clean Up */
  delete[] compressed_data;
  delete[] line_data;

  return encrypted_line;
}

bool FileHandler::fileClose()
{
  file_stream.close();
  return !file_stream.is_open();
}

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
  return 0;
}

QString FileHandler::intToString(int* line_data, int length, bool decrypting)
{
  bool end_reached = FALSE;
  QString line = "";
  int line_count = length - 1;

  /* Check for padding at the end, only if the data was encrypted */
  if(encryption_enabled && decrypting)
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
  for(int i = 0; i < length; i++)
  {
    if(line_data[i] <= kMAX_ASCII)
    {
      char c = (char)line_data[i];
      line.append(QChar(c));
    }
  }

  return line;
}

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
        int_data[i*kASCII_IN_LONG + j] = temp_long & 0xFF;
        temp_long = temp_long >> 8;
      }
    }

    return int_data;
  }
  return 0;
}

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

  /* Allocate appropriate space */
  *line_data = new int[final_length];

  /* Convert the line */
  for(int i = 0; i < final_length; i++)
    (*line_data)[i] = line.at(i).unicode();

  return final_length;
}

int FileHandler::wrapNumber(int value, int limit)
{
  if(value >= limit)
    return value - (value / limit) * limit;
  return value;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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

  if(available && file_write && file_type == REGULAR)
  {
    if(encryption_enabled)
      file_stream << encryptLine(line, &success).toStdString() << std::endl;
    else
      file_stream << line.toStdString() << std::endl;

    return success;
  }

  return FALSE;
}
