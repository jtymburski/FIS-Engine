/******************************************************************************
 * Class Name: XmlData
 * Date Created: March 29, 2013
 * Inheritance: none
 * Description: Handles a line of XML data. This is used when data is read 
 *              from the XML file reader and returned to the calling party.
 *****************************************************************************/
#ifndef XMLDATA_H
#define XMLDATA_H

#include <QDebug>
#include <QString>
#include <QVector>

class XmlData
{
public:
  /* Constructor: Sets up a blank template with no data in the line set */
  XmlData();
  
  /* Constructor: Sets up a template with one data item */
  XmlData(bool data);
  XmlData(double data);
  XmlData(int data);
  XmlData(QString data);

  /* Destructor function */
  ~XmlData();

  /* Public Enumerators */
  enum DataType {BOOLEAN, INTEGER, FLOAT, STRING, NONE};

private:
  /* Element, key, and value stacks for data */
  QVector<QString> element;
  QVector<QString> key;
  QVector<QString> value;

  /* The data from the XML */
  DataType data_type;
  bool bool_data;
  double double_data;
  int int_data;
  QString string_data;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* If the start element has been added that indicates the data, call this
   * with a string read of the data and it will be converted */
  bool addData(QString data);

  /* Add data to encapsulate in this class */
  bool addDataOfType(bool data);
  bool addDataOfType(double data);
  bool addDataOfType(int data);
  bool addDataOfType(QString data);

  /* Add element to XML array */
  bool addElement(QString element, QString key = "", QString value = "");

  /* Delete all elements in the stack for XML - clears data */
  bool clearElements();

  /* Removes the stored data in the class, doubles as an initializer */
  bool clearData();

  /* Get data calls - success holds if the data is actually set in the class */
  bool getDataBool(bool* success = 0);
  double getDataFloat(bool* success = 0);
  int getDataInteger(bool* success = 0);
  QString getDataString(bool* success = 0);

  /* Element handling */
  QVector<QString> getAllElements();
  QVector<QString> getAllKeys();
  QVector<QString> getAllKeyValues();
  QString getElement(int index);
  QString getKey(int index);
  QString getKeyValue(int index);
  int getNumElements();

  /* Determine the type of the data */
  bool isDataBool();
  bool isDataFloat();
  bool isDataInteger();
  bool isDataString();
  bool isDataUnset();

  /* Delete last element - clears data */
  bool removeLastElement();
};

#endif // XMLDATA_H
