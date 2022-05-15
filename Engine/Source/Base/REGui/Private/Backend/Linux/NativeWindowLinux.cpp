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
#include "REGui/Backend/Linux/NativeWindowLinux.h"
#include "REGui/Backend/Linux/GuiLinux.h"
#include "REGui/Gui/Gui.h"
#include "REGui/Gui/NativeWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


NativeWindowLinux::NativeWindowLinux(NativeWindow *nativeWindow)
: NativeWindowImpl(nativeWindow)
, mNativeWindowHandle(0)
, mScreen(0)
, mDestroyed(false) {

}

NativeWindowLinux::~NativeWindowLinux() {
  destroy();
}

void NativeWindowLinux::createWindow(RECore::handle nativeWindowHandle) {
  // Save native window handle
  mNativeWindowHandle = static_cast<::Window>(nativeWindowHandle);
}

bool NativeWindowLinux::isDestroyed() const {
  return mDestroyed;
}

void NativeWindowLinux::destroy() {
  if (!mDestroyed) {
    GuiLinux* guiLinux = static_cast<GuiLinux*>(mNativeWindow->getGui()->getImpl());

    XEvent event;
    event.type = ClientMessage;
    event.xclient.window = mNativeWindowHandle;
    event.xclient.message_type = 0;
    event.xclient.format = 32;
    event.xclient.data.l[0] = guiLinux->mClientProtocols.DestroyWidget;
    event.xclient.data.l[1] = 0;
    event.xclient.data.l[2] = 0;
    XSendEvent(mDisplay, mNativeWindowHandle, False, NoEventMask, &event);

    XSync(mDisplay, False);
  }
}

RECore::handle NativeWindowLinux::getNativeWindowHandle() const {
  return mNativeWindowHandle;
}

void NativeWindowLinux::setPosition(const RECore::Vec2i &position) {
  if (mSize != position) {
    if (!mDestroyed) {
      XWindowChanges changes;
      changes.width = position.getX();
      changes.height = position.getY();
      XConfigureWindow(
        mDisplay,
        mNativeWindowHandle,
        CWX | CWY,
        &changes);

      XSync(mDisplay, False);

      mPosition = position;
    }
  }
}

RECore::Vec2i NativeWindowLinux::getPosition() const {
  return mPosition;
}

void NativeWindowLinux::setSize(const RECore::Vec2i &size) {
  if (mSize != size) {
    if (!mDestroyed) {
      XWindowChanges changes;
      changes.width = size.getX();
      changes.height = size.getY();
      XConfigureWindow(
        mDisplay,
        mNativeWindowHandle,
        CWWidth | CWHeight,
        &changes);

      XSync(mDisplay, False);

      mSize = size;
    }
  }
}

RECore::Vec2i NativeWindowLinux::getSize() const {
  return mSize;
}

void NativeWindowLinux::redraw() {
  if (!mDestroyed) {
    XEvent event;
    event.type = Expose;
    event.xany.window = mNativeWindowHandle;
    event.xexpose.count = 0;
    XSendEvent(mDisplay, mNativeWindowHandle, False, 0, &event);

    XSync(mDisplay, False);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
