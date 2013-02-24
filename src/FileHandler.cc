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

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
FileHandler::FileHandler()
{
  available = TRUE;
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

QString FileHandler::readLine(bool* fail)
{

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

  /* Open the file stream */
  success &= fileOpen();

  /* Determine the class availablility based on the success status */
  if(success)
    available = FALSE;

  return success;
}

bool FileHandler::stop()
{
  bool success = TRUE;

  /* Close the file stream */
  success &= fileClose();

  /* If success, reopen the class availability */
  if(success)
    available = TRUE;

  return success;
}

bool FileHandler::writeLine(QString line)
{

}
