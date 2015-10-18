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

/* 
 * Description: Constructor function - Set up a no data XML class
 *
 * Input: none
 */
XmlData::XmlData()
{
  /* Ensure the XML vectors are clear */
  clearElements();

  /* Clear data set */
  clearData();
}

/* 
 * Description: Constructor function - Sets up the class with just one BOOL
 *              data element in it.
 *
 * Input: bool data - the data to add to the class, as a boolean
 */
XmlData::XmlData(bool data)
{
  /* Start with the data cleared */
  clearElements();
  clearData();
  
  /* Add the data */
  addDataOfType(data);
}

/* 
 * Description: Constructor function - Set up the class with just one FLOAT
 *
 * Input: float data - the data to add to the class, as a float
 */
XmlData::XmlData(float data)
{
  /* Start with the data cleared */
  clearElements();
  clearData();
  
  /* Add the data */
  addDataOfType(data);
}

/* 
 * Description: Constructor function - Set up the class with just one INTEGER
 *
 * Input: int data - the data to add to the class, as an integer
 */
XmlData::XmlData(int data)
{
  /* Start with the data cleared */
  clearElements();
  clearData();
  
  /* Add the data */
  addDataOfType(data);
}

/* 
 * Description: Constructor function - Set up the class with just one STRING
 *
 * Input: std::string data - the data to add to the class, as a string
 */
XmlData::XmlData(std::string data)
{
  /* Start with the data cleared */
  clearElements();
  clearData();
  
  /* Add the data */
  addDataOfType(data);
}
 
/* 
 * Description: Destructor function
 */
XmlData::~XmlData()
{
  /* Cleanup by clearing the vectors */
  clearElements();
  clearData();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Add data to the class. It utilizes the last entered element
 *              to determine what the data should be, as per the XML standard
 *              of this product. It then pops off that identifier since it 
 *              isn't needed anymore.
 *
 * Inputs: std::string data - the data to add, of any of the 4 types
 * Output: bool - status if insert is successful
 */
bool XmlData::addData(std::string data)
{
  bool success = true;

  /* Determine what the data is */
  if(element.size() > 0)
  {
    DataType type = static_cast<DataType>(std::stoi(value.back()));

    if(type == BOOLEAN)
      success &= addDataOfType(data == "true");
    else if(type == INTEGER)
      success &= addDataOfType(std::stoi(data));
    else if(type == FLOAT)
      success &= addDataOfType(std::stof(data));
    else if(type == STRING)
      success &= addDataOfType(data);
    else
      success = false;
  }
  else
  {
    success = false;
  }

  /* Remove the type counter */
  if(success)
  {
    key.back() = "";
    value.back() = "";
  }

  return success;
}

/* 
 * Description: Add data to the class, of the type bool. It will attempt to 
 *              clear out previous data before setting the new.
 *
 * Inputs: bool data - the data to add, of the type bool
 * Output: bool - status if insert is successful
 */
bool XmlData::addDataOfType(bool data)
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

/* 
 * Description: Add data to the class, of the type float. It will attempt to
 *              clear out previous data before setting the new.
 *
 * Inputs: float data - the data to add, of the type float
 * Output: bool - status if insert is successful
 */
bool XmlData::addDataOfType(float data)
{
  /* Clear the data before doing anything */
  bool status = clearData();

  /* Set the data */
  if(status)
  {
    data_type = FLOAT;
    float_data = data;
  }

  return status;
}

/* 
 * Description: Add data to the class, of the type integer. It will attempt to
 *              clear out previous data before setting the new.
 *
 * Inputs: int data - the data to add, of the type integer
 * Output: bool - status if insert is successful
 */
bool XmlData::addDataOfType(int data)
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

/* 
 * Description: Add data to the class. of the type QString. It will attempt to
 *              clear out previous data before setting the new.
 *
 * Inputs: std::string data - the data to add, of the type string
 * Output: bool - status if insert is successful
 */
bool XmlData::addDataOfType(std::string data)
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

/* 
 * Description: Add an element to the class. It adds it to the back of the
 *              stack. The key and value strings are left to default to empty
 *              and unused. If the key is not empty, then it is set in XML
 *              as an indicator for the element.
 *
 * Inputs: std::string element - the element name (front of the tag)
 *         std::string key - the key for the tag identifier (default "")
 *         std::string value - the value corresponding to the key (default "")
 * Output: bool - status if insert is successful
 */
bool XmlData::addElement(std::string element, 
                         std::string key, std::string value)
{
  /* Add only if the element entry is not empty */
  if(!element.empty())
  {
    this->element.push_back(element);

    /* Only add key-value pair if key is not empty (aka default) */
    if(!key.empty())
    {
      this->key.push_back(key);
      this->value.push_back(value);
    }
    else
    {
      this->key.push_back("");
      this->value.push_back("");
    }

    return true;
  }

  return false;
}

/* 
 * Description: Clear all the elements in the class, including the 
 *              corresponding keys and values.
 *
 * Inputs: none
 * Output: bool - status if clear was successful
 */
bool XmlData::clearElements()
{
  element.clear();
  key.clear();
  value.clear();

  return true;
}

/* 
 * Description: Clears out the data that is stored in this class. Set the data
 *              type to NONE .
 *
 * Inputs: none
 * Output: bool - status if clear was successful
 */
bool XmlData::clearData()
{
  data_type = NONE;
  bool_data = false;
  float_data = 0.0;
  int_data = 0;
  string_data.clear();
  
  return true;
}

/*
 * Description: Flips all elements that are currently in the stack to the other
 *              order. This is a convenience function, since prepending stuff
 *              onto an array multiple times will be slower than just flipping
 *              all the elements once at the end of the call. Relevant for how
 *              parsing is done with the TinyXML2 library.
 *
 * Inputs: none
 * Output: none
 */
void XmlData::flipElements()
{
  std::vector<std::string> flipped_elements;
  std::vector<std::string> flipped_keys;
  std::vector<std::string> flipped_values;
  
  /* Flip all the elements */
  for(int i = element.size() - 1; i >= 0; i--)
  {
    flipped_elements.push_back(element[i]);
    flipped_keys.push_back(key[i]);
    flipped_values.push_back(value[i]);
  }
  
  /* Swap the vectors */
  element = flipped_elements;
  key = flipped_keys;
  value =flipped_values;
}
  
/* 
 * Description: Returns the data stored in the class, if it's a bool. If it's 
 *              not, success is set to false.
 *
 * Inputs: bool* success - status if the data in the class is bool
 * Output: bool - the bool data available in the class
 */
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

/* 
 * Description: Returns the data stored in the class, if it's a float. If it's 
 *              not, success is set to false.
 *
 * Inputs: bool* success - status if the data in the class is bool
 * Output: float - the float data available in the class
 */
float XmlData::getDataFloat(bool* success)
{
  /* Only return data if the data stored is a float */
  if(data_type == FLOAT)
  {
    if(success != 0)
      *success = true;
    return float_data;
  }

  /* Otherwise, return fail status */
  if(success != 0)
    *success = false;
  return 0.0;
}

/* 
 * Description: Returns the data stored in the class, if it's an integer. If 
 *              it's not, success is set to false.
 *
 * Inputs: bool* success - status if the data in the class is bool
 * Output: int - the integer data available in the class
 */
int XmlData::getDataInteger(bool* success)
{
  /* Only return data if the data stored is an integer */
  if(data_type == INTEGER)
  {
    if(success != 0)
      *success = true;
    return int_data;
  }

  /* Otherwise, return fail status and negative integer */
  if(success != 0)
    *success = false;
  return -1;
}

/* 
 * Description: Returns the data stored in the class, if it's a string. If it's 
 *              not, success is set to false.
 *
 * Inputs: bool* success - status if the data in the class is bool
 * Output: std::string - the string data available in the class
 */
std::string XmlData::getDataString(bool* success)
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

/* 
 * Description: Returns a vector list of all elements stored within the class.
 *
 * Inputs: none
 * Output: std::vector<std::string> - the list of all elements in the class
 */
std::vector<std::string> XmlData::getAllElements()
{
  return element;
}

/* 
 * Description: Returns a vector list of all keys stored within the class.
 *              This corresponds directly to the list of elements.
 *
 * Inputs: none
 * Output: std::vector<std::string> - the list of all keys in the class
 */
std::vector<std::string> XmlData::getAllKeys()
{
  return key;
}

/* 
 * Description: Returns a vector list of all values stored within the class.
 *              This corresponds directly to the list of keys.
 *
 * Inputs: none
 * Output: std::vector<std::string> - the list of all values (of keys) in the 
 *                                    class
 */
std::vector<std::string> XmlData::getAllKeyValues()
{
  return value;
}

/* 
 * Description: Returns a single element of the given index, if it's within
 *              range. Otherwise, it's returns a blank string.
 *
 * Inputs: uint16_t index - the index of the element to return
 * Output: std::string - the element to be returned
 */
std::string XmlData::getElement(uint16_t index)
{
  if(index < element.size())
    return element[index];
  return "";
}

/* 
 * Description: Returns a single key of the given index, if it's within
 *              range. Otherwise, it's returns a blank string. This key
 *              corresponds directly to the element, at the same index.
 *
 * Inputs: uint16_t index - the index of the key to return
 * Output: std::string - the key to be returned
 */
std::string XmlData::getKey(uint16_t index)
{
  if(index < key.size())
    return key[index];
  return "";
}

/* 
 * Description: Returns a single value (of the key) of the given index, if 
 *              it's within range. Otherwise, it's returns a blank string.
 *
 * Inputs: uint16_t index - the index of the value (of the key) to return
 * Output: std::string - the value (of the key) to be returned
 */
std::string XmlData::getKeyValue(uint16_t index)
{
  if(index < value.size())
    return value[index];
  return "";
}

/* 
 * Description: Returns the number of elements that are stored. This number
 *              also indicates the number of keys and values, since these
 *              directly correlate.
 *
 * Inputs: none
 * Output: int - the number of elements in the class
 */
int XmlData::getNumElements()
{
  return element.size();
}

/*
 * Description: Returns the tail elements from the selected index to the end.
 * 
 * Inputs: uint16_t index - the index for the starting element
 * Output: std::vector<std::string> - the vector stack of elements
 */
std::vector<std::string> XmlData::getTailElements(uint16_t index)
{
  std::vector<std::string> tail_elements;
  
  if(index < element.size())
  {
    for(uint16_t i = index; i < element.size(); i++)
      tail_elements.push_back(element[i]);
  }
  
  return tail_elements;
}

/* 
 * Description: Check if the data stored in the class is a boolean.
 *
 * Inputs: none
 * Output: bool - returns if the data is a boolean.
 */
bool XmlData::isDataBool()
{
  return (data_type == BOOLEAN);
}

/* 
 * Description: Check if the data stored in the class is a float.
 *
 * Inputs: none
 * Output: bool - returns if the data is a float.
 */
bool XmlData::isDataFloat()
{
  return (data_type == FLOAT);
}

/* 
 * Description: Check if the data stored in the class is an integer.
 *
 * Inputs: none
 * Output: bool - returns if the data is an integer.
 */
bool XmlData::isDataInteger()
{
  return (data_type == INTEGER);
}

/* 
 * Description: Check if the data stored in the class is a string.
 *
 * Inputs: none
 * Output: bool - returns if the data is a string.
 */
bool XmlData::isDataString()
{
  return (data_type == STRING);
}

/* 
 * Description: Check if the data stored in the class is unset.
 *
 * Inputs: none
 * Output: bool - returns if the data is unset.
 */
bool XmlData::isDataUnset()
{
  return (data_type == NONE);
}

/* 
 * Description: Removes the last element on the top of the stack from the 
 *              list of elements. This also includes the corresponding key
 *              and value. This is a pop removal system, which corresponds
 *              to the way that XML acts.
 *
 * Inputs: none
 * Output: bool - returns if there is an element that can be popped off
 */
bool XmlData::removeLastElement()
{
  if(!element.empty())
  {
    element.pop_back();
    key.pop_back();
    value.pop_back();
    return true;
  }
  return false;
};
