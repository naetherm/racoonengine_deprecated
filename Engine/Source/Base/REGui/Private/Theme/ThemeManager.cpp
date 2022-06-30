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
#include "REGui/Theme/ThemeManager.h"
#include "REGui/Theme/Theme.h"
#include <RECore/Log/Log.h>
#include <RECore/Reflect/ClassManager.h>
#include <RECore/Reflect/Class.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ThemeManager& ThemeManager::instance() {
  static ThemeManager SInstance;
  return SInstance;
}

ThemeManager::ThemeManager()
: SlotClassLoaded(&ThemeManager::onClassLoaded, this)
, SlotClassUnloaded(&ThemeManager::onClassUnloaded, this) {
  // The theme manager must be informed if new classes registered in order to register new theme types!
  RECore::ClassManager::instance().EventClassLoaded.connect(SlotClassLoaded);
  RECore::ClassManager::instance().EventClassUnloaded.connect(SlotClassUnloaded);
}

ThemeManager::~ThemeManager() {
  // Clean up
  for (RECore::uint32 i = 0; i < mlstThemes.size(); ++i) {
    delete mlstThemes[i];
  }

  mlstThemes.clear();
  mmapThemes.clear();
}


RECore::uint32 ThemeManager::getNumOfThemes() {
  registerClasses();

  return mlstThemes.size();
}

Theme *ThemeManager::getThemeByIndex(RECore::uint32 index) {
  registerClasses();

  return mlstThemes.at(index);
}

Theme *ThemeManager::getThemeByName(const RECore::String &name) {
  registerClasses();

  auto iter = mmapThemes.find(name);
  if (iter != mmapThemes.end()) {
    return iter->second;
  }

  return nullptr;
}


void ThemeManager::onClassLoaded(const RECore::Class* c) {
  printf("puished\n");
  mNewClasses.push_back(c);
}

void ThemeManager::onClassUnloaded(const RECore::Class* c) {
  /// TODO(naetherm): Loop through all themes and destroy them

}

void ThemeManager::registerClasses() {
  printf("ffff\n");
  if (mNewClasses.size()) {
    auto iter = mNewClasses.begin();
    while (iter != mNewClasses.end()) {

      const RECore::Class* thisClass = *iter;

      static const RECore::String BaseClassName = "REGui::Theme";

      if (thisClass->isDerivedFrom(BaseClassName) && thisClass->hasConstructor()) {
        // Create instance of class
        Theme* theme = reinterpret_cast<REGui::Theme*>(thisClass->create());
        RE_LOG(Info, "Registered Theme with name " + theme->getName())

        mlstThemes.push_back(theme);
        mmapThemes.emplace(theme->getName(), theme);
      }

      ++iter;
    }
  }
  // Clear
  mNewClasses.clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui