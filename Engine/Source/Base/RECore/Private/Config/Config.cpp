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

}


void Config::load(const String& filename) {

}

void Config::save(const String& filename) {

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
