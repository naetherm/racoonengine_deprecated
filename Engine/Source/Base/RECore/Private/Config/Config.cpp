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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/Config/Config.h"
#include "RECore/Config/ConfigSection.h"
#include "RECore/File/IFileManager.h"
#include "RECore/File/IFile.h"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Config::Config() {

}

Config::~Config() {
  // Cleanup
  clear();
}


void Config::load(const IFileManager& fileManager, const String& filename) {
  mFilename = filename;

  if (fileManager.doesFileExist(filename)) {
    IFile* file = fileManager.openFile(IFileManager::FileMode::READ, filename);

    String contentAsString;

    if (nullptr != file) {
      // Load the whole file content as string
      const size_t numberOfBytes = file->getNumberOfBytes();
      contentAsString.resize(numberOfBytes);
      // Read
      file->read(const_cast<void*>(static_cast<const void*>(contentAsString.data())), numberOfBytes);

      // Close the file
      fileManager.closeFile(*file);
    }

    // Load the JSON document
    rapidjson::Document rapidJsonDocument;
    const rapidjson::ParseResult rapidJsonParseResult = rapidJsonDocument.Parse(contentAsString.cstr());
    if (rapidJsonParseResult.Code() != rapidjson::kParseErrorNone) {
      // Error
    }

    // Finally, we can read the file
    {
      // Loop through all members -> these are normally the section names
      for (rapidjson::Value::MemberIterator iter = rapidJsonDocument.MemberBegin(); rapidJsonDocument.MemberEnd() != iter; ++iter) {
        // Get name
        String sectionName = iter->name.GetString();
        ConfigSection* section = addSectionByName(sectionName);

        for (rapidjson::Value::MemberIterator vk_iter = iter->value.MemberBegin(); iter->value.MemberEnd() != vk_iter; ++vk_iter) {
          section->addKeyValue(
            vk_iter->name.GetString(),
            vk_iter->value.GetString());
        }
      }
    }
  } else {
    // Error
  }
}

void Config::save(const IFileManager& fileManager, const String& filename) {
  std::vector<ConfigSection*> configSections = getAllSections();

  for (auto iter = configSections.begin(); iter != configSections.end(); ++iter) {
    ConfigSection* section = *iter;
  }
}


const std::vector<ConfigSection*>& Config::getAllSections() const {
  return mlstSections;
}

const ConfigSection* Config::getSectionByName(const String& sectionName) const {
  auto iter = mSectionNameToSection.find(sectionName);

  if (iter != mSectionNameToSection.end()) {
    return iter->second;
  }

  return nullptr;
}

ConfigSection* Config::getSectionByName(const String& sectionName)  {
  auto iter = mSectionNameToSection.find(sectionName);

  ConfigSection* section = nullptr;

  if (iter == mSectionNameToSection.end()) {
    section = new ConfigSection(sectionName);

    mlstSections.push_back(section);
    mSectionNameToSection.emplace(sectionName, section);
  } else {
    section = iter->second;
  }

  return section;
}

ConfigSection* Config::addSectionByName(const String& sectionName) {
  auto iter = mSectionNameToSection.find(sectionName);

  ConfigSection* section = nullptr;

  if (iter == mSectionNameToSection.end()) {
    section = new ConfigSection(sectionName);

    mlstSections.push_back(section);
    mSectionNameToSection.emplace(sectionName, section);
  } else {
    section = iter->second;
  }

  return section;
}

void Config::clear() {
  for (auto iter = mlstSections.begin(); iter != mlstSections.end(); ++iter) {
    delete *iter;
  }
  mlstSections.clear();
  mSectionNameToSection.clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
