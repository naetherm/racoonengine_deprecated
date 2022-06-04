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
#include "REGui/Backend/Linux/GuiLinux.h"
#include "REGui/Gui/Gui.h"
#include "REGui/Widget/Window/MainWindow.h"
#include "REGui/Gui/NativeWindow.h"
#include "REGui/Backend/Linux/NativeWindowLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


GuiLinux::GuiLinux(Gui* gui)
: GuiImpl(gui)
, mDisplay(nullptr) {
  // Activate Xlib multi-thread support
  XInitThreads();

  // Set X error handler
  XSetErrorHandler(&GuiLinux::errorHandler);

  // Initialize x11 display
  mDisplay = XOpenDisplay(nullptr);
}

GuiLinux::~GuiLinux() {
  // Close display
  ::XCloseDisplay(mDisplay);
}


::Display* GuiLinux::getDisplay() const {
  return mDisplay;
}


bool GuiLinux::hasPendingMessages() {
  return (XPending(mDisplay) > 0);
}

void GuiLinux::processMessage() {
  // Wait and get event
  XEvent event;

  while (hasPendingMessages()) {
    XNextEvent(mDisplay, &event);

    // Process event
    processXEvent(&event);
  }
}


void GuiLinux::processXEvent(XEvent* event) {
  // TODO(naetherm): Wait for window implementation
  reinterpret_cast<NativeWindowLinux*>(mGui->getMainWindow()->getNativeWindow()->getImpl())->handleEvent(*event);
}

int GuiLinux::errorHandler(Display *display, XErrorEvent *error) {
  // Get error description
  char szError[256];
  XGetErrorText(display, error->error_code, szError, 255);
  printf("X Error: %s\n", szError);
  printf("  Serial number: %lu\n", error->serial);
  printf("  Major op-code: %d\n", error->request_code);
  printf("  Minor op-code: %d\n", error->minor_code);
  printf("\n");
  return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
