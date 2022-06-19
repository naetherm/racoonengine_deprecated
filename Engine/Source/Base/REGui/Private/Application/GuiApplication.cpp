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
#include "REGui/Application/GuiContext.h"
#include "REGui/Gui/Gui.h"
#include "REGui/Widget/Window/MainWindow.h"
#include <RECore/Log/Log.h>
#include <RECore/File/PhysicsFSFileManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(GuiApplication, "REGui", RECore::CoreApplication, "Application class")
  // Constructors
  re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
re_class_metadata_end(GuiApplication)


GuiApplication::GuiApplication()
: RECore::CoreApplication()
, mFileManager(nullptr)
, mGuiContext(nullptr)
, mMainWindow(nullptr) {
  // TODO(naetherm): This is temporarily and will late on be replaced by a configuration file
  m_cCommandLine.addParameter("RHI", "-r", "--rhi", "RHI Interface", "");
}

GuiApplication::~GuiApplication() {
  delete mGuiContext;
  delete mFileManager;
}


bool GuiApplication::onStart() {
  // Before going into the main loop, initialize the file manager first
  mFileManager = new RECore::PhysicsFSFileManager(
    std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string());
  mcCoreContext.initialize(*mFileManager);

  // Call gui instance
  Gui& gui = Gui::instance();

  if (RECore::CoreApplication::onStart()) {
    // Create gui context
    createGuiContext();

    // Startup the gui
    gui.initialize(&mcCoreContext, mGuiContext);

    // Create main window
    mMainWindow = createMainWindow();
    gui.setMainWindow(mMainWindow);

    // Done
    return true;
  }

  // Error
  return false;
}

void GuiApplication::onStop() {
  RECore::CoreApplication::onStop();
}


void GuiApplication::main() {
  Gui& gui = Gui::instance();
  while (gui.isActive() && m_bRunning) {
    mMainWindow->redraw();
    gui.processMessages();
  }

  delete mMainWindow;
}

MainWindow* GuiApplication::createMainWindow() {
  return new MainWindow(&Gui::instance());
}

void GuiApplication::createGuiContext() {
  mGuiContext = new GuiContext();
  if (!mGuiContext->initialize(m_cCommandLine.getValue("RHI"))) {
    RE_LOG(Error, std::string("Unable to initialize the gui context with desired RHI '") + m_cCommandLine.getValue("RHI").cstr() + "'")
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
