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
class ConfigSection;
class IFileManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Config
 *
 * @brief
 * Config class.
 */
class Config {
public:

  /**
   * @brief
   * Default constructor.
   */
  Config();

  /**
   * @brief
   * Destructor.
   */
  ~Config();


  /**
   * @brief
   * Loads the content of the file @p filename.
   *
   * @þaram[in] fileManager
   * Reference to the used file manager.
   * @param[in] filename
   * The filename to load.
   */
  void load(const IFileManager& fileManager, const String& filename);

  /**
   * @brief
   * Saves the content of the configuration within this file.
   *
   * @þaram[in] fileManager
   * Reference to the used file manager.
   * @param[in] filename
   * The filename to save to.
   */
  void save(const IFileManager& fileManager, const String& filename);


  /**
   * @brief
   * Returns all configuration sections.
   *
   * @return
   * All configuration sections.
   */
  const std::vector<ConfigSection*>& getAllSections() const;

  /**
   * @brief
   * REturns pointer to configuration section by name.
   *
   * @þaram[in] sectionName
   * The name of the section to return.
   *
   * @return
   * Pointer to configuration section, nullptr if not available.
   */
  const ConfigSection* getSectionByName(const String& sectionName) const;

  /**
   * @brief
   * REturns pointer to configuration section by name.
   *
   * @þaram[in] sectionName
   * The name of the section to return.
   *
   * @return
   * Pointer to configuration section, nullptr if not available.
   */
  ConfigSection* getSectionByName(const String& sectionName);

  /**
   * @brief
   * Adds a new configuration section with the section name @p sectionName.
   * If there is already a configuration section with the same name, the pointer to
   * this instance will be returned instead.
   *
   * @param[in] sectionName
   * The name of the section to add.
   *
   * @return
   * Pointer to configuration section.
   */
  ConfigSection* addSectionByName(const String& sectionName);

  void clear();

private:
  /** The filename of the file to load */
  String mFilename;
  /** Vector of all configuration sections */
  std::vector<ConfigSection*> mlstSections;
  /** Associative map from section name to configuration section */
  std::map<String, ConfigSection*> mSectionNameToSection;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
