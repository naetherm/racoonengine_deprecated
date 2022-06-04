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
#if defined(LINUX)
#include "REGui/Backend/Linux/NativeWindowLinux.h"
#endif


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
NativeWindow::NativeWindow(Gui* gui)
: mImpl(nullptr)
, mGui(gui) {
#if defined(LINUX)
  mImpl = new NativeWindowLinux(this);
#endif
}

NativeWindow::~NativeWindow() {
  delete mImpl;
}

Gui* NativeWindow::getGui() const {
  return mGui;
}

NativeWindowImpl* NativeWindow::getImpl() const {
  return mImpl;
}

RECore::handle NativeWindow::getWindowHandle() const {
  return mImpl->getWindowHandle();
}

void NativeWindow::redraw() {
  mImpl->redraw();
}

void NativeWindow::ping() {
  mImpl->ping();
}

void NativeWindow::setTitle(const RECore::String &title) {
  mImpl->setTitle(title);
}

void NativeWindow::getWindowSize(RECore::int32& width, RECore::int32& height) {
  mImpl->getWindowSize(width, height);
}

void NativeWindow::setWindowSize(RECore::int32 width, RECore::int32 height) {
  mImpl->setWindowSize(width, height);
}


bool NativeWindow::isDestroyed() const {
  return mImpl->isDestroyed();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
