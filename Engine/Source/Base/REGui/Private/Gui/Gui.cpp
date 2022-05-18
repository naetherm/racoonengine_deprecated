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
#include "REGui/Gui/Gui.h"
#include "REGui/Gui/NativeWindow.h"
#include "REGui/Gui/Screen.h"
#if defined(LINUX)
#include "REGui/Backend/Linux/GuiLinux.h"
#elif defined(WIN32)
#endif
#include <imgui.h>
#include <imgui_internal.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


Gui &Gui::instance() {
  static Gui SInstance;
  return SInstance;
}


Gui::Gui()
: mpImpl(nullptr)
, mGuiContext(nullptr)
, mDefaultScreen(nullptr)
, mMainWindow(nullptr) {
#if defined(LINUX)
  mpImpl = new GuiLinux(this);
#endif
}

Gui::~Gui() {
  { // Clear screens
    for (auto iter = mScreens.begin(); iter != mScreens.end(); ++iter) {
      delete *iter;
    }
    mScreens.clear();
  }
  // Do not delete the gui context, we will manage this

  // Clear all screens
  delete mpImpl;
}


void Gui::initialize(GuiContext *guiContext) {
  // Sync the gui context
  mGuiContext = guiContext;
}


GuiImpl *Gui::getImpl() const {
  return mpImpl;
}

bool Gui::isActive() const {
  // Check if the platform dependent gui implementation is active and whether there is a window to work on
  return ((mpImpl != nullptr) && !mWindows.empty());
}


void Gui::update() {

}

void Gui::processMessages() {
  mpImpl->processMessage();
}

void Gui::addWindow(NativeWindow* nativeWindow) {
  // TODO(naetherm): This will always be the main window for now
  mMainWindow = nativeWindow;
  mWindows.emplace(nativeWindow->getWindowHandle(), nativeWindow);
}

void Gui::removeWindow(NativeWindow* nativeWindow) {
  // Find native window by its handle
}

NativeWindow* Gui::getWindow(RECore::handle nativeWindowHandle) const {
  auto iter = mWindows.find(nativeWindowHandle);
  if (iter != mWindows.end()) {
    return iter->second;
  }
  return nullptr;
}

NativeWindow* Gui::getMainWindow() const {
  return mMainWindow;
}


const std::vector<Screen*> &Gui::getScreens() const {
  return mScreens;
}

Screen *Gui::getScreen(const RECore::String &name) const {
  for (auto iter = mScreens.begin(); iter != mScreens.end(); ++iter) {
    if ((*iter)->getName() == name) {
      return *iter;
    }
  }

  return nullptr;
}

Screen *Gui::getDefaultScreen() const {
  return mDefaultScreen;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
