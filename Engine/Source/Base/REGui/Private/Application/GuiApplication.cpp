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
#include <RECore/Log/Log.h>
#include <RECore/Platform/Platform.h>
#include <RERenderer/RendererImpl.h>
#include <RERenderer/Context.h>
#include <RECore/File/PhysicsFSFileManager.h>
#include "REGui/Widget/Window/MainWindow.h"
#if defined(LINUX)
#include "REGui/Backend/Linux/GuiLinux.h"
#include <RERHI/Linux/X11Context.h>
#endif


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
, mMainWindow(nullptr)
, EventHandlerOnDestroy(&GuiApplication::onDestroyMainWindow, this) {
  // TODO(naetherm): This is temporarily and will late on be replaced by a configuration file
  m_cCommandLine.addParameter("RHI", "-r", "--rhi", "RHI Interface", "");
}

GuiApplication::~GuiApplication() {
  if (mGuiContext) {
    delete mGuiContext;
  }
}

MainWindow *GuiApplication::getMainWindow() const {
  return mMainWindow;
}

void GuiApplication::setMainWindow(MainWindow *nativeWindow) {
  mMainWindow = nativeWindow;
}

bool GuiApplication::onStart() {
  mFileManager = new RECore::PhysicsFSFileManager(std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string());
  mcCoreContext.initialize(*mFileManager);
  // Get gui instance here
  Gui& gui = Gui::instance();
  if (CoreApplication::onStart()) {
    // Before we start create the gui context first
    createGuiContext();

    // Initialize the gui (and the gui context
    gui.initialize(mGuiContext);

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
  while (gui.isActive() && m_bRunning) {
    gui.update();

    gui.processMessages();
  }
}

void GuiApplication::createGuiContext() {
  Gui& gui = Gui::instance();
  // We will need the gui early so we can access platform dependent information
  mGuiContext = new GuiContext();
  mGuiContext->setRhiName(m_cCommandLine.getValue("RHI"));
  mGuiContext->mSharedLibraryName = RECore::Platform::instance().getSharedLibraryPrefix() + "RERHI" + mGuiContext->getRhiName() + "." +
                             RECore::Platform::instance().getSharedLibraryExtension();

  RE_LOG(Info, std::string(("Starting Gui with " + mGuiContext->getRhiName() + " rhi").cstr()))

#if defined(LINUX)
  mGuiContext->mRhiContext = new RERHI::X11Context(
    reinterpret_cast<GuiLinux*>(gui.getImpl())->getDisplay(),
    reinterpret_cast<GuiLinux*>(gui.getImpl())->getWindowHandle());
#endif

  // Initialize library
  auto *pLib = new RECore::DynLib();
  if (pLib->load(mGuiContext->mSharedLibraryName)) {
    typedef RERHI::RHIDynamicRHI *(*RHI_INSTANCER)(const RERHI::RHIContext &);
    RHI_INSTANCER _Creator = reinterpret_cast<RHI_INSTANCER>(pLib->getSymbol("createRhiInstance"));
    if (_Creator) {
      mGuiContext->mRhi = _Creator(*mGuiContext->mRhiContext);

      if (mGuiContext->mRhi) {
        mGuiContext->mRhi->AddReference();
        RE_LOG(Info, std::string("Successfully created the RHI backend of ") + mGuiContext->mSharedLibraryName.cstr())
      } else {
        // Error
        RE_LOG(Error, "Creation of dynamic RHI failed.")
      }
    } else {
      // Error
      RE_LOG(Error, "Unable to find the symbol 'createRHIInstance'")
    }
  } else {
    // Error
    RE_LOG(Error, std::string("Unable to load the library ") + mGuiContext->mSharedLibraryName.cstr())
  }

  // Create renderer instance
  {
    mGuiContext->mRendererContext = new RERenderer::Context(*mGuiContext->mRhi, mcCoreContext);
    mGuiContext->mRenderer = new RERenderer::RendererImpl(*mGuiContext->mRendererContext);
    RE_LOG(Info,"Created renderer context")
  }
}

void GuiApplication::onDestroyMainWindow() {

}

void GuiApplication::onCreateMainWindow() {
  mMainWindow = new MainWindow();
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
