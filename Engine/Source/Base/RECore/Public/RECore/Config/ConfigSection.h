////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/String/String.h"
#include <map>
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Config;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * ConfigSection
 *
 * @brief
 * A configuration section.
 */
class ConfigSection {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] name
   * The name of the section.
   */
  ConfigSection(const String& name);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * The object to copy.
   */
  ConfigSection(const ConfigSection& rhs);

  /**
   * @brief
   * Destructor.
   */
  ~ConfigSection();


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * The object to copy.
   *
   * @return
   * Reference to this.
   */
  ConfigSection& operator=(const ConfigSection& rhs);

  /**
   * @brief
   * Equality operator.
   *
   * @param[in] rhs
   * The object to compare to.
   *
   * @return
   * True if both are equal, false otherwise.
   */
  bool operator==(const ConfigSection& rhs) const;

  /**
   * @brief
   * Equality operator.
   *
   * @param[in] rhs
   * The object to compare to.
   *
   * @return
   * True if both are not equal, false otherwise.
   */
  bool operator!=(const ConfigSection& rhs) const;


  /**
   * @brief
   * Returns the name of the section.
   *
   * @return
   * The name of the section.
   */
  const String& getName() const;

  /**
   * @brief
   * Returns all key-value pairs of this configuration section.
   *
   * @return
   * Reference to key value storage.
   */
  const std::map<String, String>& getAllKeyValues() const;

  /**
   * @brief
   * Returns all key-value pairs of this configuration section.
   *
   * @return
   * Reference to key value storage.
   */
  std::map<String, String>& getAllKeyValues();

  /**
   * @brief
   * Adds a new key value pair to the section.
   *
   * @param[in] key
   * The key value.
   * @param[in] value
   * The value.
   */
  void addKeyValue(String key, String value);

  /**
   * @brief
   * Returns the value associated with the @p key.
   *
   * @param[in] key
   * The key to search from.
   *
   * @return
   * Value associated with key.
   */
  String getValue(const String& key) const;

  /**
   * @brief
   * Clears all values.
   */
  void clear();

private:
  /** The name of the section */
  String mName;
  /** The content of the section in the form of key value pairs */
  std::map<String, String> mKeyToValue;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
