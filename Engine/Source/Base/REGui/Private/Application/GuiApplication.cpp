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
#include "REGui/Application/GuiApplication.h"
#include "REGui/Gui/Gui.h"
#include "REGui/Gui/NativeWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(GuiApplication, "RECore", RECore::CoreApplication, "Application class")
  // Constructors
  re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
re_class_metadata_end(GuiApplication)


GuiApplication::GuiApplication()
: mGuiContext(nullptr)
, mNativeWindow(nullptr)
, EventHandlerOnDestroy(&GuiApplication::onDestroyMainWindow, this) {
  // TODO(naetherm): This is temporarily
  m_cCommandLine.addParameter("RHI", "-r", "--rhi", "RHI Interface", "");
}

GuiApplication::~GuiApplication() {

}

NativeWindow *GuiApplication::getMainWindow() const {
  return mNativeWindow;
}

void GuiApplication::setMainWindow(NativeWindow *nativeWindow) {
  mNativeWindow = nativeWindow;
}

bool GuiApplication::onStart() {
  if (CoreApplication::onStart()) {
    // Before we start create the gui context first
    createGuiContext();
    // Create main window
    onCreateMainWindow();
    if (!m_bRunning) {
      return false;
    }

    // Done
    return true;
  }

  return false;
}

void GuiApplication::onStop() {
  CoreApplication::onStop();
}

void GuiApplication::main() {
  // Run main loop
  Gui& gui = Gui::instance();
  // Process gui message loop
  if (gui.isActive() && m_bRunning) {
    gui.processMessages();
  }
}

void GuiApplication::createGuiContext() {

}

void GuiApplication::onDestroyMainWindow() {

}

void GuiApplication::onCreateMainWindow() {

}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
