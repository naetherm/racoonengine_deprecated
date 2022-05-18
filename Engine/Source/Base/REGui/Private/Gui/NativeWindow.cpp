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
#include "REGui/Gui/NativeWindow.h"
#include "REGui/Gui/Gui.h"
#if defined(LINUX)
#include "REGui/Backend/Linux/NativeWindowLinux.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


NativeWindow::NativeWindow(Gui *gui)
: mGui(gui)
, mImpl(nullptr) {
#if defined(LINUX)
  mImpl = new NativeWindowLinux(this);
#endif

  // Do the final step for creation
  mImpl->createWindow();

  // TODO(naetherm): Should we self-register ourself?
  gui->addWindow(this);
}

NativeWindow::~NativeWindow() {
  delete mImpl;
}

Gui *NativeWindow::getGui() const {
  return mGui;
}

RECore::handle NativeWindow::getWindowHandle() const {
  return mImpl->getNativeWindowHandle();
}

RERHI::RHISwapChainPtr NativeWindow::getSwapChain() const {
  return mSwapChain;
}

void NativeWindow::setSwapChain(RERHI::RHISwapChainPtr swapChainPtr) {
  mSwapChain = swapChainPtr;
}

void NativeWindow::setPosition(const RECore::Vec2i &position) {
  mImpl->setPosition(position);
}

RECore::Vec2i NativeWindow::getPosition() const {
  return mImpl->getPosition();
}

void NativeWindow::setSize(const RECore::Vec2i &size) {
  mImpl->setSize(size);
}

RECore::Vec2i NativeWindow::getSize() const {
  return mImpl->getSize();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
