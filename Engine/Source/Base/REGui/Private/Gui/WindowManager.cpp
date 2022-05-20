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
#include "REGui/Gui/WindowManager.h"
#include "REGui/Gui/NativeWindow.h"
#include "REGui/Widget/Window/MainWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


WindowManager::WindowManager() {

}

WindowManager::~WindowManager() {
  // Clear
  for (auto iter = mWindows.begin(); iter != mWindows.end(); ++iter) {
    delete iter->second;
  }
  mWindows.clear();
}


bool WindowManager::hasWindows() const {
  return mWindows.empty();
}

void WindowManager::addMainWindow(MainWindow *mainWindow) {
  auto iter = mWindows.find(mainWindow->getNativeWindow()->getWindowHandle());
  if (iter != mWindows.end()) {
    mWindows.emplace(mainWindow->getNativeWindow()->getWindowHandle(), mainWindow);
  }
}

void WindowManager::removeMainWindow(MainWindow *mainWindow) {
  auto iter = mWindows.find(mainWindow->getNativeWindow()->getWindowHandle());
  if (iter != mWindows.end()) {
    mWindows.erase(mainWindow->getNativeWindow()->getWindowHandle());
  }
}

bool WindowManager::hasMainWindow(MainWindow *mainWindow) const {
  auto iter = mWindows.find(mainWindow->getNativeWindow()->getWindowHandle());
  return (iter != mWindows.end());
}

bool WindowManager::hasMainWindow(RECore::handle windowHandle) const {
  auto iter = mWindows.find(windowHandle);
  return (iter != mWindows.end());
}

MainWindow *WindowManager::getMainWindow(RECore::handle windowHandle) const {
  auto iter = mWindows.find(windowHandle);
  return (iter != mWindows.end()) ? iter->second : nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
