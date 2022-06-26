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
#include "RECore/Localization/Localization.h"
#include "RECore/Localization/LocalizationGroup.h"
#include "RECore/Localization/LocalizationText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Localization &Localization::instance() {
  static Localization SSingleton;
  return SSingleton;
}

Localization::Localization() {

}

Localization::~Localization() {
  // clear all
  removeAllGroups();
}

String Localization::getLanguage() const {
  return mLanguage;
}

void Localization::setLanguage(const String &language) {
  mLanguage = language;
}

String Localization::get(const String &text) const {
  return mLanguageToGroup.find(mLanguage)->second->getText(text)->getValue();
}

uint32 Localization::getNumOfGroups() const {
  return mlstGroups.size();
}

LocalizationGroup *Localization::getGroup(uint32 index) const {
  return mlstGroups[index];
}

LocalizationGroup *Localization::addGroup(const String &name) {
  auto iter = mLanguageToGroup.find(name);

  if (iter == mLanguageToGroup.end()) {
    LocalizationGroup* group = new LocalizationGroup(name);

    mlstGroups.push_back(group);
    mLanguageToGroup.emplace(name, group);

    return group;
  }

  return iter->second;
}

bool Localization::removeGroup(uint32 index) {
  auto iter = mlstGroups.begin();

  if (iter != mlstGroups.end()) {
    mLanguageToGroup.erase((*iter)->getName());
    std::advance(iter, index);
    mlstGroups.erase(iter);
    return true;
  }
  return false;
}

bool Localization::removeGroup(const String &name) {
  auto iter = mLanguageToGroup.find(name);

  if (iter != mLanguageToGroup.end()) {
    mLanguageToGroup.erase(name);

    auto liter = std::find(mlstGroups.begin(), mlstGroups.end(), iter->second);
    mlstGroups.erase(liter);

    return true;
  }

  return false;
}

void Localization::removeAllGroups() {
  for (auto iter = mlstGroups.begin(); iter != mlstGroups.end(); ++iter) {
    delete *iter;
  }
  mlstGroups.clear();
  mLanguageToGroup.clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
