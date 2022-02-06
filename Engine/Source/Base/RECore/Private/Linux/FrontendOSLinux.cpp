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

#include "RECore/Linux/FrontendOSLinux.h"
#include "RECore/Frontend/WindowOS.h"
#include "RECore/Linux/WindowOSLinux.h"
#include <X11/Xlib.h>

namespace RECore {

FrontendOSLinux::FrontendOSLinux(FrontendOS &cFrontendOS)
: FrontendOSImpl(cFrontendOS)
, m_pDisplay(XOpenDisplay(nullptr)) {

}

FrontendOSLinux::~FrontendOSLinux() {
  XCloseDisplay(this->m_pDisplay);
}

::Display *FrontendOSLinux::getDisplay() const {
  return this->m_pDisplay;
}

void FrontendOSLinux::addWindowHandle(WindowOS *pWindow) {
  this->m_lstWindows.emplace(pWindow->getNativeWindowHandle(), pWindow);
}

void FrontendOSLinux::removeWindowHandle(WindowOS *pWindow) {
  if (pWindow != nullptr) {
    this->m_lstWindows.erase(pWindow->getNativeWindowHandle());
  }
}

void FrontendOSLinux::handlePendingMessages() {
  // Get next event if there is any and start handling it
  XEvent _event;

  while (!this->m_lstWindows.empty() && XPending(this->m_pDisplay)) {
    // There is at least one window and one event to process
    XNextEvent(this->m_pDisplay, &_event);

    // Handle the provided event
    this->handleEvent(_event);
  }
}

bool FrontendOSLinux::handleEvent(XEvent &cEvent) {
  handle _nWindowHandle = cEvent.xany.window;

  // Get the window with the correct window handle
  auto iter = this->m_lstWindows.find(_nWindowHandle);

  if (iter != this->m_lstWindows.end()) {
    WindowOS* _pWindow = iter->second;
    return reinterpret_cast<WindowOSLinux *>(_pWindow->getImpl())->handleEvent(cEvent);
  }

  return false;
}
}
