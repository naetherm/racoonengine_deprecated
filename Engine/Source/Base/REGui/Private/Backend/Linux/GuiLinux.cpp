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
#include "REGui/Gui/Screen.h"
#include "REGui/Gui/NativeWindow.h"
#include "REGui/Widget/Window/MainWindow.h"
#include <RECore/Math/Vec2i.h>
#include <RECore/String/BasicRegEx.h>
#include <imgui.h>
#include <imgui_internal.h>


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

::Window GuiLinux::getWindowHandle() const {
  return mHiddenWindow;
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
  NativeWindow* nativeWindow = mGui->getWindow(event->xany.window)->getNativeWindow();

  if (nativeWindow) {
    switch(event->type) {
      case Expose:
        // Draw me!
        if (!event->xexpose.count) {
          //onDraw();
          mGui->sendMessage(GuiMessage::onDraw(nativeWindow));
        }
        break;
      case DestroyNotify:
        // Destroy me!
        break;
      case ConfigureNotify:
      {
        onResize(event->xconfigure.width, event->xconfigure.height);
        break;
      }
      case ClientMessage:
        break;
      case KeyPress:
      case KeyRelease:
      {
        const int buffer_size = 2;
        char buffer[buffer_size + 1];
        KeySym keySym;
        int count = XLookupString(&event->xkey, buffer, buffer_size, &keySym, nullptr);
        buffer[count] = 0;

        onKeyInput(keySym, buffer[0], event->type == KeyPress);
        break;
      }
      case ButtonPress:
      case ButtonRelease:
      {
        const bool isPressed = (ButtonPress == event->type);
        if (isPressed && (event->xbutton.button == 4 || event->xbutton.button == 5)) // Wheel buttons
        {
          onMouseWheelInput(event->xbutton.button == 4);
        }
        else
        {
          onMouseButtonInput(event->xbutton.button, isPressed);
        }
        break;
      }
      case MotionNotify: {
        {
          onMouseMoveInput(event->xmotion.x, event->xmotion.y);
          break;
        }
      }
    }
  }
}


void GuiLinux::onDraw() {

}

void GuiLinux::onResize(RECore::uint32 width, RECore::uint32 height) {

}

void GuiLinux::onKeyInput(uint32_t keySym, char character, bool pressed) {
  ImGuiIO& imGuiIo = ImGui::GetIO();
  if (keySym < 512)
  {
    imGuiIo.KeysDown[keySym] = pressed;
  }
  else if (XK_Alt_L == keySym)
  {
    imGuiIo.KeyAlt = pressed;
  }
  else if (XK_Shift_L == keySym)
  {
    imGuiIo.KeyShift = pressed;
  }
  else if (XK_Control_L == keySym)
  {
    imGuiIo.KeyCtrl = pressed;
  }
  else if (XK_Super_L == keySym)
  {
    imGuiIo.KeySuper = pressed;
  }
  else if ((keySym & 0xff00) == 0xff00)
  {
    // It is a special key (e.g. tab key) map the value to a range between 0x0ff and 0x1ff
    imGuiIo.KeysDown[(keySym & 0x1ff)] = pressed;
  }
  if (pressed && character > 0)
  {
    imGuiIo.AddInputCharacter(character);
  }
}

void GuiLinux::onMouseMoveInput(int x, int y) {
  float windowWidth  = 1.0f;
  float windowHeight = 1.0f;
  {
    RECore::Vec2i windowSize = mGui->getMainWindow()->getNativeWindow()->getSize();
    // Ensure that none of them is ever zero
    if (windowSize.getX() >= 1)
    {
      windowWidth = static_cast<float>(windowSize.getX());
    }
    if (windowSize.getY() >= 1)
    {
      windowHeight = static_cast<float>(windowSize.getY());
    }
  }

  {
    ImGuiIO& imGuiIo = ImGui::GetIO();
    imGuiIo.MousePos.x = static_cast<float>(x) * (imGuiIo.DisplaySize.x / windowWidth);
    imGuiIo.MousePos.y = static_cast<float>(y) * (imGuiIo.DisplaySize.y / windowHeight);
  }
}

void GuiLinux::onMouseButtonInput(uint32_t button, bool pressed) {
  // The mouse buttons on X11 starts at index 1 for the left mouse button. In ImGui the left mouse button is at index 0. Compensate it.
  if (button > 0 && button <= 5)
  {
    ImGui::GetIO().MouseDown[button - 1] = pressed;
  }
}

void GuiLinux::onMouseWheelInput(bool scrollUp) {
  ImGui::GetIO().MouseWheel += scrollUp ? -1.0f : 1.0f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
