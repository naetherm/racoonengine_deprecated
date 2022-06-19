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
#include "REGui/Widget/Window/MainWindow.h"
#include "REGui/Widget/Menu/MainMenuBar.h"
#include "REGui/Gui/Gui.h"
#include "REGui/Gui/GuiMessage.h"
#include "REGui/Gui/NativeWindow.h"
#include <imgui.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
MainWindow::MainWindow(Gui *gui)
: mGui(gui)
, mNativeWindow(nullptr)
, mSwapChain(nullptr)
, mMainMenuBar(nullptr) {
  // Create native window
  mNativeWindow = new NativeWindow(gui);
}

MainWindow::~MainWindow() {
  if (mMainMenuBar) {
    delete mMainMenuBar;
  }
  delete mSwapChain;
  delete mNativeWindow;
}


void MainWindow::setSwapChain(RERHI::RHISwapChain *swapChain) {
  mSwapChain = swapChain;
  mSwapChain->AddReference();
}

void MainWindow::setTitle(const RECore::String& title) {
  mNativeWindow->setTitle(title);
}

NativeWindow* MainWindow::getNativeWindow() const {
  return mNativeWindow;
}

RECore::handle MainWindow::getWindowHandle() const {
  return mNativeWindow->getWindowHandle();
}

RERHI::RHISwapChain* MainWindow::getSwapChain() const {
  return mSwapChain;
}

void MainWindow::onMessage(const GuiMessage& guiMessage) {
  switch(guiMessage.getType()) {
    case MessageOnDraw:
    {
      onDraw();
      break;
    }
    case MessageOnMove:
    {

      break;
    }
    case MessageOnSize:
    {
      //onSize(guiMessage.getPositionSize());
      break;
    }
    case MessageOnMouseMove:
    {
      onMouseMove(guiMessage.getPositionSize());
      break;
    }
    case MessageOnMouseButtonDown:
    case MessageOnMouseButtonUp:
    {
      {
        ImGui::GetIO().MouseDown[guiMessage.getMouseButton() - 1] = guiMessage.getType() == MessageOnMouseButtonDown;
      }
      break;
    }
    case MessageOnMouseWheel:
    {
      ImGui::GetIO().MouseWheel += guiMessage.getDelta();
      break;
    }
    case MessageOnKeyDown:
    case MessageOnKeyUp:
    {
      ImGui::GetIO().KeysDown[(guiMessage.getKey() & 0x1ff)] = MessageOnKeyDown == guiMessage.getType();
      if (MessageOnKeyDown == guiMessage.getType()) {
        ImGui::GetIO().AddInputCharacter(guiMessage.getKey());
      }
      break;
    }
  }
}

void MainWindow::redraw() {
  mNativeWindow->redraw();
}


void MainWindow::onDraw() {
  // If there is a main menu draw it
  if (mShowMainMenuBar && mMainMenuBar != nullptr) {
    mMainMenuBar->onDraw();
  }
}

void MainWindow::onMouseMove(const RECore::Vec2i& position) {
  RECore::int32 windowWidth  = 0;
  RECore::int32 windowHeight = 0;

  mNativeWindow->getWindowSize(windowWidth, windowHeight);

  {
    ImGuiIO& imGuiIo = ImGui::GetIO();
    imGuiIo.MousePos.x = static_cast<float>(position.getX()) * (imGuiIo.DisplaySize.x / (float)windowWidth);
    imGuiIo.MousePos.y = static_cast<float>(position.getY()) * (imGuiIo.DisplaySize.y / (float)windowHeight);
  }
}

void MainWindow::onResize() {
  mSwapChain->resizeBuffers();
}

void MainWindow::onMove(const RECore::Vec2i& position) {

}

void MainWindow::onSize(const RECore::Vec2i& size) {
  mNativeWindow->setWindowSize(size.getX(), size.getY());
}

bool MainWindow::isDestroyed() const {
  return mNativeWindow->isDestroyed();
}


bool MainWindow::showMainMenuBar() const {
  return mShowMainMenuBar;
}

void MainWindow::makeMainMenuBarVisible(bool visible) {
  mShowMainMenuBar = visible;
}

void MainWindow::setMainMenuBar(MenuBar* menuBar) {
  if (mMainMenuBar) {
    delete mMainMenuBar;
  }

  mMainMenuBar = menuBar;
}

void MainWindow::onDrawMainMenuBar() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
