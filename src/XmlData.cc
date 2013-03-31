/******************************************************************************
 * Class Name: XmlData
 * Date Created: March 29, 2013
 * Inheritance: none
 * Description: Handles a line of XML data. This is used when data is read 
 *              from the XML file reader and returned to the calling party.
 *****************************************************************************/
#include "XmlData.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
XmlData::XmlData()
{
  /* Ensure the XML vectors are clear */
  element.clear();
  key.clear();
  value.clear();

  /* Clear data set */
  clearData();
}

XmlData::~XmlData()
{
  /* Cleanup by clearing the vectors */
  element.clear();
  key.clear();
  value.clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
bool XmlData::addData(bool data)
{
  /* Clear the data before doing anything */
  bool status = clearData();

  /* Set the data */
  if(status)
  {
    data_type = BOOLEAN;
    bool_data = data;
  }

  return status;
}

bool XmlData::addData(double data)
{
  /* Clear the data before doing anything */
  bool status = clearData();

  /* Set the data */
  if(status)
  {
    data_type = FLOAT;
    double_data = data;
  }

  return status;
}

bool XmlData::addData(int data)
{
  /* Clear the data before doing anything */
  bool status = clearData();

  /* Set the data */
  if(status)
  {
    data_type = INTEGER;
    int_data = data;
  }

  return status;
}

bool XmlData::addData(QString data)
{
  /* Clear the data before doing anything */
  bool status = clearData();

  /* Set the data */
  if(status)
  {
    data_type = STRING;
    string_data = data;
  }

  return status;
}

bool XmlData::addElement(QString element, QString key, QString value)
{
  /* Add only if the element entry is not empty */
  if(!element.isEmpty())
  {
    this->element.append(element);

    /* Only add key-value pair if key is not empty (aka default) */
    if(!key.isEmpty())
    {
      this->key.append(key);
      this->value.append(value);
    }
    else
    {
      this->key.append("");
      this->value.append("");
    }

    return true;
  }

  return false;
}

bool XmlData::clearElements()
{
  if(!element.isEmpty())
  {
    element.clear();
    key.clear();
    value.clear();
    return true;
  }
  return false;
}

bool XmlData::clearData()
{
  data_type = NONE;
  bool_data = false;
  double_data = 0.0;
  int_data = 0;
  string_data.clear();
  
  return true;
}

bool XmlData::getDataBool(bool* success)
{
  /* Only return data if the data stored is a boolean */
  if(data_type == BOOLEAN)
  {
    if(success != 0)
      *success = true;
    return bool_data;
  }

  /* Otherwise, return fail status */
  if(success != 0)
    *success = false;
  return false;
}

double XmlData::getDataFloat(bool* success)
{
  /* Only return data if the data stored is a float */
  if(data_type == FLOAT)
  {
    if(success != 0)
      *success = true;
    return double_data;
  }

  /* Otherwise, return fail status */
  if(success != 0)
    *success = false;
  return 0.0;
}

int XmlData::getDataInteger(bool* success)
{
  /* Only return data if the data stored is an integer */
  if(data_type == INTEGER)
  {
    if(success != 0)
      *success = true;
    return int_data;
  }

  /* Otherwise, return fail status */
  if(success != 0)
    *success = false;
  return 0;
}

QString XmlData::getDataString(bool* success)
{
  /* Only return data if the data stored is a string */
  if(data_type == STRING)
  {
    if(success != 0)
      *success = true;
    return string_data;
  }

  /* Otherwise, return fail status */
  if(success != 0)
    *success = false;
  return "";
}
  
QVector<QString> XmlData::getAllElements()
{
  return element;
}

QVector<QString> XmlData::getAllKeys()
{
  return key;
}

QVector<QString> XmlData::getAllKeyValues()
{
  return value;
}

QString XmlData::getElement(int index)
{
  if(index < element.size() && index >= 0)
    return element.at(index);
  return "";
}

QString XmlData::getKey(int index)
{
  if(index < key.size() && index >= 0)
    return key.at(index);
  return "";
}

QString XmlData::getKeyValue(int index)
{
  if(index < value.size() && index >= 0)
    return value.at(index);
  return "";
}

int XmlData::getNumElements()
{
  return element.size();
}

bool XmlData::isDataBool()
{
  return (data_type == BOOLEAN);
}

bool XmlData::isDataFloat()
{
  return (data_type == FLOAT);
}

bool XmlData::isDataInteger()
{
  return (data_type == INTEGER);
}

bool XmlData::isDataString()
{
  return (data_type == STRING);
}

bool XmlData::isDataUnset()
{
  return (data_type == NONE);
}

bool XmlData::removeLastElement()
{
  if(!element.isEmpty())
  {
    element.remove(element.size() - 1);
    key.remove(key.size() - 1);
    value.remove(value.size() - 1);
    return true;
  }
  return false;
};
