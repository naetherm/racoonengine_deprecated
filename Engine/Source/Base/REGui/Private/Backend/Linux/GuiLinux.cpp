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
#include "REGui/Gui/Screen.h"
#include <RECore/String/BasicRegEx.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


GuiLinux::GuiLinux(Gui *gui)
: GuiImpl(gui) {
  // Yes, we want to access Xlib from multiple threads, so we have to call
  XInitThreads();

  // Set X error handler
  XSetErrorHandler(&GuiLinux::errorHandler);

  // Get display
  mDisplay = XOpenDisplay(nullptr);


  // Create custom protocol extensions
  mClientProtocols.Close				= XInternAtom(mDisplay, "WM_DELETE_WINDOW",				False);
  mClientProtocols.Focus				= XInternAtom(mDisplay, "WM_TAKE_FOCUS",					False);
  mClientProtocols.Timer				= XInternAtom(mDisplay, "WM_TIMER",						False);
  mClientProtocols.Exit					= XInternAtom(mDisplay, "WM_EXIT",						False);
  mClientProtocols.Wakeup				= XInternAtom(mDisplay, "RE_WAKEUP",						False);
  mClientProtocols.DestroyWidget		= XInternAtom(mDisplay, "RE_DESTROY_WIDGET",				False);
  mClientProtocols.ThemeChanged			= XInternAtom(mDisplay, "RE_THEME_CHANGED",				False);

  // Create other atoms
  mAtoms.UTF8_STRING					= XInternAtom(mDisplay, "UTF8_STRING",					False);
  mAtoms.WM_NAME						= XInternAtom(mDisplay, "WM_NAME",						False);
  mAtoms._NET_WM_NAME					= XInternAtom(mDisplay, "_NET_WM_NAME",					False);
  mAtoms._NET_WM_VISIBLE_NAME			= XInternAtom(mDisplay, "_NET_WM_VISIBLE_NAME",			False);
  mAtoms._NET_WM_STATE					= XInternAtom(mDisplay, "_NET_WM_STATE",					False);
  mAtoms._NET_WM_STATE_FULLSCREEN		= XInternAtom(mDisplay, "_NET_WM_STATE_FULLSCREEN",		False);
  mAtoms._NET_WM_STATE_MAXIMIZED_HORZ	= XInternAtom(mDisplay, "_NET_WM_STATE_MAXIMIZED_HORZ",	False);
  mAtoms._NET_WM_STATE_MAXIMIZED_VERT	= XInternAtom(mDisplay, "_NET_WM_STATE_MAXIMIZED_VERT",	False);
  mAtoms._NET_WM_STATE_HIDDEN			= XInternAtom(mDisplay, "_NET_WM_STATE_HIDDEN",			False);
  mAtoms._NET_WM_STATE_SKIP_TASKBAR		= XInternAtom(mDisplay, "_NET_WM_STATE_SKIP_TASKBAR",		False);
  mAtoms._NET_WM_STATE_ABOVE			= XInternAtom(mDisplay, "_NET_WM_STATE_ABOVE",			False);
  mAtoms._XEMBED						= XInternAtom(mDisplay, "_XEMBED",						False);
  mAtoms._XEMBED_INFO					= XInternAtom(mDisplay, "_XEMBED_INFO",					False);
  mAtoms._NET_SYSTEM_TRAY_OPCODE		= XInternAtom(mDisplay, "_NET_SYSTEM_TRAY_OPCODE",		False);
  mAtoms._NET_SYSTEM_TRAY_MESSAGE_DATA	= XInternAtom(mDisplay, "_NET_SYSTEM_TRAY_MESSAGE_DATA",	False);

  // Create dummy window for internal messages
  XSetWindowAttributes sAttribs;
  mHiddenWindow = XCreateWindow(
    mDisplay,
    DefaultRootWindow(mDisplay),
    -100, -100,
    1, 1,
    0, CopyFromParent, InputOutput, CopyFromParent,
    0, &sAttribs);
  XSetWMProtocols(mDisplay, mHiddenWindow, reinterpret_cast<Atom*>(&mClientProtocols), 7);
}

GuiLinux::~GuiLinux() {

  // Destroy hidden window
  ::XDestroyWindow(mDisplay, mHiddenWindow);

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
  XNextEvent(mDisplay, &event);

  // Process event
  processXEvent(&event);
}

void GuiLinux::postMessage(const GuiMessage& guiMessage) {

}

void GuiLinux::enumerateScreens(std::vector<Screen *> &screens) {
  // Get display name
  RECore::String sDisplay = DisplayString(mDisplay);

  // Initialize display components
  RECore::String sServer = "";
  int    nNumber = 0;
//	int    nScreen = 0;	// Currently not used

  // Get display components (e.g.: ":0.0")
  RECore::CRegEx cDisplayRegEx("^(\\w*):(\\d+)\\.(\\d+)$");
  if (cDisplayRegEx.match(sDisplay)) {
    sServer = cDisplayRegEx.getResult(0);
    nNumber = cDisplayRegEx.getResult(1).getInt();
//		nScreen = cDisplayRegEx.GetResult(2).GetInt();	// Currently not used
  }

  // Get screens of display
  RECore::uint32 nScreens = ScreenCount(mDisplay);
  for (RECore::uint32 i=0; i<nScreens; i++) {
    // Get screen info
    ::Screen *pScreenInfo = XScreenOfDisplay(mDisplay, i);

    // Add screen
    Screen *pScreen = new Screen(mGui);
    pScreen->setName    (sServer + ':' + nNumber + "." + i);
    pScreen->setPosition(RECore::Vec2i(0, 0));
    pScreen->setSize    (RECore::Vec2i(XWidthOfScreen(pScreenInfo), XHeightOfScreen(pScreenInfo)));
    pScreen->setDefault(i == XDefaultScreen(mDisplay));
    screens.push_back(pScreen);
  }
}

RECore::Vec2i GuiLinux::getScreenSize() const {
  return RECore::Vec2i();
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

void GuiLinux::processXEvent(XEvent *event) {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
