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
#include "REGui/Application/GuiContext.h"
#include "REGui/Gui/GuiMessage.h"
#include "REGui/Gui/GuiRenderer.h"
#include "REGui/Widget/Window/MainWindow.h"
#include "REGui/Gui/NativeWindow.h"
#include <REGui/Theme/ThemeDark.h>
#if defined(LINUX)
#include "REGui/Backend/Linux/GuiLinux.h"
#endif
#include <RECore/Color/Color4.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


Gui& Gui::instance() {
  static Gui SInstance;
  return SInstance;
}


Gui::Gui()
: mGuiImpl(nullptr)
, mCoreContext(nullptr)
, mImGuiContext(nullptr)
, mGuiContext(nullptr)
, mGuiRenderer(nullptr)
, mMainWindow(nullptr)
, mMainSwapChain(nullptr)
, mTheme(nullptr) {
#if defined(LINUX)
  mGuiImpl = new GuiLinux(this);
#endif
}

Gui::~Gui() {
  if (mTheme) {
    delete mTheme;
  }
  ImGui::DestroyContext(mImGuiContext);
  delete mGuiImpl;
}


void Gui::initialize(RECore::CoreContext* coreContext, GuiContext *guiContext) {
  // We can just set this here
  mImGuiContext = ImGui::CreateContext();
  mCoreContext = coreContext;
  if (mGuiContext == nullptr) {
    mGuiContext = guiContext;
  }
}


GuiImpl* Gui::getImpl() const {
  return mGuiImpl;
}

RECore::CoreContext& Gui::getCoreContext() const {
  return *mCoreContext;
}

GuiContext& Gui::getGuiContext() const {
  return *mGuiContext;
}

bool Gui::isActive() const {
  return (mGuiImpl != nullptr) && (!mMainWindow->isDestroyed());
}

bool Gui::hasWindows() const {
  return (mMainWindow != nullptr);
}

void Gui::setMainWindow(MainWindow *mainWindow) {
  // Check if there is already a main window, if this is the first one we first have to initialize the whole
  // gui renderer too
  if (mMainWindow == nullptr) {
    mGuiRenderer = new GuiRenderer(this);
    mGuiRenderer->startup(mainWindow->getWindowHandle());

    mTheme = new ThemeDark();
    mTheme->initialize();
  }

  mMainWindow = mainWindow;

  // Create swapchain interface
  {
    // Create render pass using the preferred swap chain texture format
    const RERHI::Capabilities& capabilities = mGuiRenderer->getRhi()->getCapabilities();
    RERHI::RHIRenderPass* renderPass = mGuiRenderer->getRhi()->createRenderPass(
      1,
      &capabilities.preferredSwapChainColorTextureFormat,
      capabilities.preferredSwapChainDepthStencilTextureFormat,
      1 RHI_RESOURCE_DEBUG_NAME("Main"));

    // Create a main swap chain instance
    mMainWindow->setSwapChain(mGuiRenderer->getRhi()->createSwapChain(
      *renderPass,
      RERHI::WindowHandle{mMainWindow->getWindowHandle(), nullptr, nullptr},
      mGuiRenderer->getRhi()->getContext().isUsingExternalContext() RHI_RESOURCE_DEBUG_NAME("MainWindow")));
  }
}

MainWindow* Gui::getMainWindow() const {
  return mMainWindow;
}


void Gui::update() {
  mGuiRenderer->update();
}

void Gui::processMessages() {
  mGuiImpl->processMessage();
}

void Gui::sendMessage(const GuiMessage& guiMessage) {
  if (guiMessage.getType() == MessageOnDraw) {
    // new frame
    mGuiRenderer->startFrame(mMainWindow);
    RERHI::Command::SetGraphicsRenderTarget::create(mCommandBuffer, mMainWindow->getSwapChain());

    { // Since Direct3D 12 is command list based, the viewport and scissor rectangle must be set in every draw call to work with all supported RHI implementations
      // Get the window size
      RECore::uint32 width  = 1;
      RECore::uint32 height = 1;
      mMainWindow->getSwapChain()->getWidthAndHeight(width, height);

      // Set the graphics viewport and scissor rectangle
      RERHI::Command::SetGraphicsViewportAndScissorRectangle::create(mCommandBuffer, 0, 0, width, height);
    }

    RERHI::Command::ClearGraphics::create(mCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);
  }
  // TODO(naetherm): Send the message to the correct window
  mMainWindow->onMessage(guiMessage);

  if (guiMessage.getType() == MessageOnDraw) {
    // end frame and draw
    mGuiRenderer->fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(mCommandBuffer);

    mCommandBuffer.dispatchToRhiAndClear(*mGuiRenderer->getRhi());

    mMainWindow->getSwapChain()->present();
  }
}


void Gui::dummy() {
  if (mMainWindow != nullptr && mMainWindow->getSwapChain() != nullptr) {
    printf("Draw\n");
    RERHI::Command::SetGraphicsRenderTarget::create(mCommandBuffer, mMainWindow->getSwapChain());

    { // Since Direct3D 12 is command list based, the viewport and scissor rectangle must be set in every draw call to work with all supported RHI implementations
      // Get the window size
      RECore::uint32 width  = 1;
      RECore::uint32 height = 1;
      mMainWindow->getSwapChain()->getWidthAndHeight(width, height);

      // Set the graphics viewport and scissor rectangle
      RERHI::Command::SetGraphicsViewportAndScissorRectangle::create(mCommandBuffer, 0, 0, width, height);
    }

    RERHI::Command::ClearGraphics::create(mCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);

    mCommandBuffer.dispatchToRhiAndClear(*mGuiRenderer->getRhi());

    mMainWindow->getSwapChain()->present();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
