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
#include "FileHandler.h"

FileHandler::FileHandler()
{
  available = TRUE;
  depth = 0;

  setEncryptionEnabled(FALSE);
  setFilename("");
  setWriteEnabled(FALSE);
}

FileHandler::~FileHandler()
{
}

QString FileHandler::getFilename()
{
  return file_name;
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

void FileHandler::setEncryptionEnabled(bool enable)
{
  encryption_enabled = enable;
}

void FileHandler::setFilename(QString path)
{
  file_name = path;
}

void FileHandler::setWriteEnabled(bool enable)
{
  file_write = enable;
}
