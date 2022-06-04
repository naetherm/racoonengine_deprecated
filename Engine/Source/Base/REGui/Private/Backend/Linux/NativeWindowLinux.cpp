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
#include "REGui/Widget/Window/MainWindow.h"
#include "REGui/Gui/NativeWindow.h"
#include "REGui/Gui/Gui.h"
#include "REGui/Gui/GuiMessage.h"


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
NativeWindowLinux::NativeWindowLinux(NativeWindow *nativeWindow)
: NativeWindowImpl(nativeWindow)
, mDestroyed(false) {
  // Get Display from linux gui implementation
  mDisplay = reinterpret_cast<GuiLinux*>(nativeWindow->getGui()->getImpl())->getDisplay();

  WM_DELETE_WINDOW = XInternAtom(mDisplay, "WM_DELETE_WINDOW", True);
  UTF8_STRING = XInternAtom(mDisplay, "UTF8_STRING", False);
  WM_NAME = XInternAtom(mDisplay, "WM_NAME", False);
  _NET_WM_NAME = XInternAtom(mDisplay, "_NET_WM_NAME", False);
  _NET_WM_VISIBLE_NAME = XInternAtom(mDisplay, "_NET_WM_VISIBLE_NAME", False);

  // TODO(naetherm): This is just for now
  const RECore::uint32 width = 800;
  const RECore::uint32 height = 600;
  const int screen = DefaultScreen(mDisplay);
  Visual* visual = DefaultVisual(mDisplay, screen);
  const int depth = DefaultDepth(mDisplay, screen);

  // Create native OS window instance and switch background to black
  XSetWindowAttributes attrs;
  attrs.background_pixel = 0;
  attrs.event_mask = ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask | VisibilityChangeMask | KeyReleaseMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
  mWindowHandle = XCreateWindow(
    mDisplay,
    XRootWindow(mDisplay, screen),
    100, 100,
    width, height,
    0,
    depth,
    InputOutput,
    visual,
    CWBackPixel | CWEventMask,
    &attrs);
  XSetWMProtocols(mDisplay, mWindowHandle, &WM_DELETE_WINDOW, 1);

  setTitle("RacoonEngine - Gui");

  // Show window
  //XMapRaised(mDisplay, mWindowHandle);
  XMapWindow(mDisplay, mWindowHandle);

  // Do it!
  XSync(mDisplay, False);
}

NativeWindowLinux::~NativeWindowLinux() {
  if (mWindowHandle) {
    XDestroyWindow(
      mDisplay,
      mWindowHandle);
  }
}


::Window NativeWindowLinux::getWindow() const {
  return mWindowHandle;
}

void NativeWindowLinux::handleEvent(XEvent& event) {
  switch (event.type) {
    // draw window
    case Expose:
    {
      mNativeWindow->getGui()->sendMessage(GuiMessage::onDraw(mNativeWindow));

      break;
    }
    // window configuration changed (position or size)
    case ConfigureNotify:
    {
      // Get position and size
      int x = event.xconfigure.x;
      int y = event.xconfigure.y;
      int w = event.xconfigure.width;
      int h = event.xconfigure.height;

      if (mSize.getX() != w || mSize.getY() != h) {
        mSize.setX(w);
        mSize.setY(h);

        // Send message
        mNativeWindow->getGui()->sendMessage(GuiMessage::onSize(mNativeWindow, mSize));
      } else if (mPosition.getX() != x || mPosition.getY() != y) {
        mPosition.setX(x);
        mPosition.setY(y);

        // Send message
        mNativeWindow->getGui()->sendMessage(GuiMessage::onMove(mNativeWindow, mSize));
      }

      //mNativeWindow->getGui()->getMainWindow()->onResize();
    }
      // custom messages
    case ClientMessage:
    {
      if (WM_DELETE_WINDOW == static_cast<Atom>(event.xclient.data.l[0])) {
        XDestroyWindow(event.xany.display, mWindowHandle);
        mDestroyed = true;
      }
    }
    // show window
    case MapNotify:
    {
      mNativeWindow->getGui()->sendMessage(GuiMessage::onShow(mNativeWindow));
      break;
    }
    // hide window
    case UnmapNotify:
    {
      mNativeWindow->getGui()->sendMessage(GuiMessage::onHide(mNativeWindow));
      break;
    }
    // window property changed
    case PropertyNotify:
    // mouse entered window
    case EnterNotify:
    {
      mNativeWindow->getGui()->sendMessage(GuiMessage::onMouseEnter(mNativeWindow));
      break;
    }
    // mouse left the window
    case LeaveNotify:
    {
      mNativeWindow->getGui()->sendMessage(GuiMessage::onMouseLeave(mNativeWindow));
      break;
    }
    // mouse moved
    case MotionNotify:
    {
      mNativeWindow->getGui()->sendMessage(GuiMessage::onMouseMove(mNativeWindow, RECore::Vec2i(event.xmotion.x, event.xmotion.y)));
      break;
    }
    // Mouse button released
    case ButtonRelease:
    {
      mNativeWindow->getGui()->sendMessage(GuiMessage::onMouseButtonUp(mNativeWindow, static_cast<EMouseButton>(event.xbutton.button)));
      break;
    }
    // Mouse button pressed
    case ButtonPress:
    {
      if (event.xbutton.button == 4 || event.xbutton.button == 5) {
        mNativeWindow->getGui()->sendMessage(GuiMessage::onMouseWheel(mNativeWindow, event.xbutton.button == 4 ? 1 : -1));
      } else {
        mNativeWindow->getGui()->sendMessage(GuiMessage::onMouseButtonDown(mNativeWindow, static_cast<EMouseButton>(event.xbutton.button)));
      }
      break;
    }
    // key pressed
    case KeyPress:
    // key released
    case KeyRelease:
    {
      const int buffer_size = 2;
      char buffer[buffer_size + 1];
      KeySym keySym;
      int count = XLookupString(&event.xkey, buffer, buffer_size, &keySym, nullptr);
      buffer[count] = 0;

      if (event.type == KeyPress) {
        mNativeWindow->getGui()->sendMessage(GuiMessage::onKeyDown(mNativeWindow, buffer[0], 0));
      } else {
        mNativeWindow->getGui()->sendMessage(GuiMessage::onKeyUp(mNativeWindow, buffer[0], 0));
      }
      break;
    }
  }
}


RECore::handle NativeWindowLinux::getWindowHandle() const {
  return mWindowHandle;
}

void NativeWindowLinux::redraw() {
  {
    // Send expose event
    XEvent sEvent;
    sEvent.type			 = Expose;
    sEvent.xany.window	 = mWindowHandle;
    sEvent.xexpose.count = 0;
    XSendEvent(mDisplay, mWindowHandle, False, 0, &sEvent);

    // Do it!
    XSync(mDisplay, False);
  }

}

void NativeWindowLinux::ping() {

}

void NativeWindowLinux::setTitle(const RECore::String& title) {
  const unsigned char* windowTitle = reinterpret_cast<const unsigned char*>(title.cstr());
  XChangeProperty(mDisplay, mWindowHandle, WM_NAME, UTF8_STRING, 8, PropModeReplace, windowTitle, title.length());
  XChangeProperty(mDisplay, mWindowHandle, _NET_WM_NAME, UTF8_STRING, 8, PropModeReplace, windowTitle, title.length());
  XChangeProperty(mDisplay, mWindowHandle, _NET_WM_VISIBLE_NAME, UTF8_STRING, 8, PropModeReplace, windowTitle, title.length());
}

void NativeWindowLinux::getWindowSize(RECore::int32& width, RECore::int32& height) {
  if (mWindowHandle) {
    ::Window rootWindow = 0;
    int x = 0, y = 0;
    unsigned int w = 0, h = 0, b = 0, d = 0;
    XGetGeometry(
      mDisplay,
      mWindowHandle,
      &rootWindow,
      &x, &y,
      &w, &h,
      &b, &d);
    width = w;
    height = h;
  } else {
    width = 0;
    height = 0;
  }
}

void NativeWindowLinux::setWindowSize(RECore::int32 width, RECore::int32 height) {
  // Check if widget has been destroyed
  if (!mDestroyed) {
    printf("beep\n");
    // Set size
    XWindowChanges sChanges;
    sChanges.width  = mSize.getX();
    sChanges.height = mSize.getY();
    XConfigureWindow(mDisplay, mWindowHandle, CWWidth | CWHeight, &sChanges);

    // Do it!
    XSync(mDisplay, False);
  }
}

bool NativeWindowLinux::isDestroyed() const {
  return mDestroyed;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
