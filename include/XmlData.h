/******************************************************************************
 * Class Name: XmlData
 * Date Created: March 29, 2013
 * Inheritance: none
 * Description: Handles a line of XML data. This is used when data is read
 *              from the XML file reader and returned to the calling party.
 *****************************************************************************/
#ifndef XMLDATA_H
#define XMLDATA_H

#include <string>
#include <vector>

class XmlData
{
public:
  /* Constructor: Sets up a blank template with no data in the line set */
  XmlData();

  /* Constructor: Sets up a template with one data item */
  XmlData(bool data);
  XmlData(float data);
  XmlData(int data);
  XmlData(std::string data);

  /* Destructor function */
  ~XmlData();

  /* Public Enumerators */
  enum DataType {BOOLEAN = 1,
                 INTEGER = 2,
                 FLOAT   = 3,
                 STRING  = 4,
                 NONE    = 5};

private:
  /* Element, key, and value stacks for data */
  std::vector<std::string> element;
  std::vector<std::string> key;
  std::vector<std::string> value;

  /* The data from the XML */
  DataType data_type;
  bool bool_data;
  float float_data;
  int int_data;
  std::string string_data;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* If the start element has been added that indicates the data, call this
   * with a string read of the data and it will be converted */
  bool addData(std::string data);

  /* Add data to encapsulate in this class */
  bool addDataOfType(bool data);
  bool addDataOfType(float data);
  bool addDataOfType(int data);
  bool addDataOfType(std::string data);

  /* Add element to XML array */
  bool addElement(std::string element,
                  std::string key = "", std::string value = "");

  /* Delete all elements in the stack for XML - clears data */
  bool clearElements();

  /* Removes the stored data in the class, doubles as an initializer */
  bool clearData();

  /* Flips all element arrays, such that the first is the last, and last is
   * first. */
  void flipElements();

  /* Get data calls - success holds if the data is actually set in the class */
  std::string getData(bool* success = nullptr);
  bool getDataBool(bool* success = nullptr);
  float getDataFloat(bool* success = nullptr);
  int getDataInteger(bool* success = nullptr);
  std::string getDataString(bool* success = nullptr);

  /* Returns the data type */
  DataType getDataType();

  /* Element handling */
  std::vector<std::string> getAllElements();
  std::vector<std::string> getAllKeys();
  std::vector<std::string> getAllKeyValues();
  std::string getElement(uint16_t index);
  std::string getKey(uint16_t index);
  std::string getKeyValue(uint16_t index);
  int getNumElements();
  std::vector<std::string> getTailElements(uint16_t index);

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
