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
#include "RECore/Localization/LocalizationGroup.h"
#include "RECore/Localization/LocalizationText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
LocalizationGroup::LocalizationGroup(const String &name)
: mName(name) {

}

LocalizationGroup::~LocalizationGroup() {
  // clear all
  removeAllTexts();
}

String LocalizationGroup::getName() const {
  return mName;
}

uint32 LocalizationGroup::getNumOfTexts() const {
  return mlstTexts.size();
}

LocalizationText *LocalizationGroup::getText(uint32 index) const {
  return mlstTexts[index];
}

LocalizationText *LocalizationGroup::getText(const String &key) const {
  auto iter = mKeyToTranslation.find(key);

  if (iter != mKeyToTranslation.end()) {
    return iter->second;
  }

  return nullptr;
}

LocalizationText *LocalizationGroup::addText(const String &key, const String &translation) {
  auto iter = mKeyToTranslation.find(key);

  if (iter == mKeyToTranslation.end()) {
    LocalizationText* text = new LocalizationText(*this, key, translation);

    mlstTexts.push_back(text);
    mKeyToTranslation.emplace(key, text);

    return text;
  } else {
    return iter->second;
  }
}

bool LocalizationGroup::removeText(uint32 index) {
  auto iter = mlstTexts.begin();

  if (iter != mlstTexts.end()) {
    mKeyToTranslation.erase((*iter)->getKey());
    std::advance(iter, index);
    mlstTexts.erase(iter);
    return true;
  }
  return false;
}

bool LocalizationGroup::removeText(const String &name) {
  auto iter = mKeyToTranslation.find(name);

  if (iter != mKeyToTranslation.end()) {
    mKeyToTranslation.erase(name);

    auto liter = std::find(mlstTexts.begin(), mlstTexts.end(), iter->second);
    mlstTexts.erase(liter);

    return true;
  }

  return false;
}

void LocalizationGroup::removeAllTexts() {
  for (auto iter = mlstTexts.begin(); iter != mlstTexts.end(); ++iter) {
    delete *iter;
  }
  mlstTexts.clear();
  mKeyToTranslation.clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
