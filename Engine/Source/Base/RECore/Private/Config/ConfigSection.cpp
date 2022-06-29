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
#include "RECore/Config/ConfigSection.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ConfigSection::ConfigSection(const String& name)
: mName(name) {

}

ConfigSection::ConfigSection(const ConfigSection& rhs)
: mName(rhs.mName)
, mKeyToValue(rhs.mKeyToValue) {

}

ConfigSection::~ConfigSection() {

}


ConfigSection& ConfigSection::operator=(const ConfigSection& rhs) {
  mName = rhs.mName;
  mKeyToValue = rhs.mKeyToValue;

  return *this;
}

bool ConfigSection::operator==(const ConfigSection& rhs) const {
  return ((mName == rhs.mName) && (mKeyToValue == rhs.mKeyToValue));
}

bool ConfigSection::operator!=(const ConfigSection& rhs) const {
  return !operator==(rhs);
}


const String& ConfigSection::getName() const {
  return mName;
}

const std::map<String, String>& ConfigSection::getAllKeyValues() const {
  return mKeyToValue;
}

std::map<String, String>& ConfigSection::getAllKeyValues() {
  return mKeyToValue;
}

void ConfigSection::addKeyValue(String key, String value) {
  auto iter = mKeyToValue.find(key);

  if (iter == mKeyToValue.end()) {
    mKeyToValue.emplace(key, value);
  }
}

String ConfigSection::getValue(const String& key) const {
  auto iter = mKeyToValue.find(key);

  if (iter != mKeyToValue.end()) {
    return iter->second;
  }

  return "";
}

void ConfigSection::clear() {
  mKeyToValue.clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
