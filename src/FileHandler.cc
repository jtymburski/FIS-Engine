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
  int length = line.length();
  int* line_data;

  /* Set the success status if bool pointer is set */
  if(success != 0)
    *success = TRUE;

  /* Get the individual character data in the string */ 
  line_data = StringToInt(line);

  uint32_t* data = new uint32_t[4];
  data[0] = 12;
  data[1] = 13;
  data[2] = 14;
  data[3] = 15;

  qDebug() << encryptData(data);
  qDebug() << data[0] << " " << data[1] << " " << data[2] << " " << data[3];

  qDebug() << decryptData(data);
  qDebug() << data[0] << " " << data[1] << " " << data[2] << " " << data[3];

  /* Test code */
  delete[] data;

  /* Clean Up */
  delete[] line_data;

  return line.replace('b', "o");
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
  if(success != 0)
    *success = FALSE;

  /* Get the line raw data in individual integers */
  int* line_data = StringToInt(line);
  // TODO

  encryptData(0);

  return line.replace('o', "b");
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

QString FileHandler::IntToString(int* data, int length)
{
  QString line = "";

  for(int i = 0; i < length; i++)
  {
    char c = (char)data[i];
    line.append(QChar(c));
  }

  return line;
}

int* FileHandler::StringToInt(QString line)
{
  int* line_data = new int[line.length()];

  for(int i = 0; i < line.length(); i++)
  {
    line_data[i] = line.at(i).unicode();
  }

  return line_data;
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
