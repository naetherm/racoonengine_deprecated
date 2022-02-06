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
#include "RECore/Linux/WindowOSLinux.h"
#include "RECore/Frontend/REIcon.h"
#include "RECore/Time/TimeManager.h"
#include "RECore/Frontend/FrontendOS.h"
#include "RECore/Frontend/Frontend.h"
#include "RECore/Frontend/FrontendContext.h"
#include "RECore/Log/Log.h"
#include "RECore/Linux/Linux_XDnDFileDropHelper.h"
#include "RECore/Linux/FrontendOSLinux.h"
#include <string.h>
#include <signal.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Ugly global variables                                 ]
//[-------------------------------------------------------]
bool g_bSignalSystemQuit = false;	/**< Does the OS asks us to shut down? */


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WindowOSLinux::WindowOSLinux(FrontendOS &cFrontendOS) :
  WindowOS(cFrontendOS),
  m_pFrontendOS(&cFrontendOS),
  m_pDisplay(XOpenDisplay(nullptr)),
  m_nNativeWindowHandle(NULL_HANDLE),
  m_bInitialized(false),
  m_bVisible(false),
  m_bIsMouseOver(false),
  m_bMouseVisible(true),
  m_nInvisibleCursor(0),
  m_bPause(true),
  // Atoms
  WM_DELETE_WINDOW	(XInternAtom(m_pDisplay, "WM_DELETE_WINDOW",	 True)),
  UTF8_STRING			(XInternAtom(m_pDisplay, "UTF8_STRING",			 False)),
  WM_NAME				(XInternAtom(m_pDisplay, "WM_NAME",				 False)),
  _NET_WM_NAME		(XInternAtom(m_pDisplay, "_NET_WM_NAME",		 False)),
  _NET_WM_VISIBLE_NAME(XInternAtom(m_pDisplay, "_NET_WM_VISIBLE_NAME", False)),
  m_pDropHelper(nullptr)
{
  // Tell the frontend about this instance at once because it may already be required during frontend life cycle initialization
  m_pFrontendOS->m_pActiveTopLevelWindow = this;

  // Connect Linux signals
  signal(SIGINT,  WindowOSLinux::signalHandler);
  signal(SIGTERM, WindowOSLinux::signalHandler);

  { // Create the native OS window based on information from WindowDescription
    const unsigned int  nWidth  = 800;
    const unsigned int  nHeight = 600;
    const int           nScreen = DefaultScreen(m_pDisplay);
    Visual             *pVisual = DefaultVisual(m_pDisplay, nScreen);
    const int           nDepth  = DefaultDepth(m_pDisplay, nScreen);

    // Create the native OS window instance with a black background (else we will see trash if nothing has been drawn, yet)
    XSetWindowAttributes sXSetWindowAttributes;
    sXSetWindowAttributes.background_pixel = 0;
    sXSetWindowAttributes.event_mask = ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask | VisibilityChangeMask | KeyPressMask | MotionNotify;
    m_nNativeWindowHandle = XCreateWindow(
      m_pDisplay,
      XRootWindow(m_pDisplay, nScreen),
      0,
      0,
      nWidth,
      nHeight,
      0,
      nDepth,
      InputOutput,
      pVisual,
      CWBackPixel | CWEventMask,
      &sXSetWindowAttributes);
    XSetWMProtocols(m_pDisplay, m_nNativeWindowHandle, &WM_DELETE_WINDOW, 1);

    // Set icon
    Atom wmIcon = XInternAtom(m_pDisplay, "_NET_WM_ICON", False);
    Atom wmCardinal = XInternAtom(m_pDisplay, "CARDINAL", False);
    XChangeProperty(m_pDisplay, m_nNativeWindowHandle, wmIcon, wmCardinal, 32,
                    PropModeReplace, reinterpret_cast<const unsigned char*>(re_icon), re_icon_length);

    // Set window title
    setTitle(m_pFrontendOS->getFrontend() ? m_pFrontendOS->getFrontend()->getFrontendContext().getName() : "");

    // Create the drag'n'drop helper instance
    m_pDropHelper = new Linux_XDnDFileDropHelper(*this);

    // Do it!
    XSync(m_pDisplay, False);

    // Create the invisible cursor instance
    createInvisibleCursor();
  }

  // Do the frontend life cycle thing - start
  m_pFrontendOS->onStart();

  // If the window is not visible yet, make it visible right now
  makeVisible();

  // Initialization is done
  m_bInitialized = true;
}

/**
*  @brief
*    Destructor
*/
WindowOSLinux::~WindowOSLinux()
{
  // Do the frontend life cycle thing - pause
  // -> "OnPause()" is already handled inside "WindowOSLinux::Ping()"
  // -> Life cycle methods should only be called from a single point in the program flow in order to ensure correctness
  // -> We could put the event processing inside "WindowOSLinux::Ping()" into a separate method and then calling it in
  //    here, but this would not bring many benefits. So, this comment block and a direct method call has to do the job
  //    to perform a clean life cycle.
  // -> Before calling "OnPause()", ensure that we're currently not within the pause-state, else we would call "OnPause()" twice
  if (!m_bPause) {
    m_pFrontendOS->onPause();
  }

  // Do the frontend life cycle thing - stop
  m_pFrontendOS->onStop();

  // Destroy the drag'n'drop helper instance
  if (m_pDropHelper)
    delete m_pDropHelper;

  // Check if the window has already been destroyed
  if (m_nNativeWindowHandle) {
    // Send destroy message to window
    XDestroyWindow(m_pDisplay, m_nNativeWindowHandle);
  }

  // Destroy the invisible cursor instance
  if (m_nInvisibleCursor)
    XFreeCursor(m_pDisplay, m_nInvisibleCursor);

  // Close the X11 display connection
  XCloseDisplay(m_pDisplay);
}

::Display *WindowOSLinux::getDisplay() const {
  return this->m_pDisplay;
}

/**
*  @brief
*    If the window is not visible yet, make it visible right now
*/
void WindowOSLinux::makeVisible()
{
  if (!m_bVisible && m_nNativeWindowHandle) {
    // The window is now considered to be visible
    m_bVisible = true;

    // Show window
    XMapRaised(m_pDisplay, m_nNativeWindowHandle);

    // Do it!
    XSync(m_pDisplay, False);
  }
}

/**
*  @brief
*    Creates an invisible cursor
*/
void WindowOSLinux::createInvisibleCursor()
{
  // Data of the "invisible" cursor
  XColor sXColorBlack;
  sXColorBlack.red = sXColorBlack.green = sXColorBlack.blue = 0;
  static char nNoData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  // Create the bitmap (it's our responsibility to free the bitmap using "XFreePixmap()" when finished)
  Pixmap sPixmapNoData = XCreateBitmapFromData(m_pDisplay, m_nNativeWindowHandle, nNoData, 8, 8);

  // Create the pixmap cursor
  m_nInvisibleCursor = XCreatePixmapCursor(m_pDisplay, sPixmapNoData, sPixmapNoData, &sXColorBlack, &sXColorBlack, 0, 0);

  // The "XCreatePixmapCursor"-documentation (http://tronche.com/gui/x/xlib/pixmap-and-cursor/XCreatePixmapCursor.html) states:
  // "The pixmaps can be freed immediately if no further explicit references to them are to be made"
  XFreePixmap(m_pDisplay, sPixmapNoData);
}

void WindowOSLinux::onDrop(const std::vector<String> &lstFiles) const
{
  m_pFrontendOS->onDrop(lstFiles);
}

//[-------------------------------------------------------]
//[ Private virtual OSWindow functions                    ]
//[-------------------------------------------------------]
handle WindowOSLinux::getNativeWindowHandle() const
{
  return static_cast<handle>(m_nNativeWindowHandle);
}

void WindowOSLinux::redraw()
{
  if (m_nNativeWindowHandle) {
    // If the window is not visible yet, make it visible right now
    makeVisible();

    // Send expose event
    XEvent sEvent;
    sEvent.type			 = Expose;
    sEvent.xany.window	 = m_nNativeWindowHandle;
    sEvent.xexpose.count = 0;
    XSendEvent(m_pDisplay, m_nNativeWindowHandle, False, 0, &sEvent);

    // Do it!
    XSync(m_pDisplay, False);
  }
}

bool WindowOSLinux::ping()
{
  bool bQuit = g_bSignalSystemQuit ? true : false;

  // Check if we're allowed to perform an update right now, please note that an update is only allowed when the frontend is fully initialized
  if (m_bInitialized) {/// && Timing::getInstance()->Update()) {
    // Let the frontend update it's states (do this before drawing else, e.g. the first frame may have an unwanted content)
    m_pFrontendOS->onUpdate();
  }

  // Look if messages are waiting (non-blocking)
  while (XPending(m_pDisplay) > 0) {
    // Get the waiting message
    XEvent sXEvent;
    XNextEvent(m_pDisplay, &sXEvent);

    // Process message
    switch (sXEvent.type) {
      case Expose:
        // There could be more then one Expose event currently in the event loop.
        // To avoid too many redraw calls, call OnDraw only when the current processed Expose event is the last one.
        if (!sXEvent.xexpose.count) {
          m_pFrontendOS->onDraw();
        }
        break;

      case DestroyNotify:
        // Mark window destroyed
        m_nNativeWindowHandle = NULL_HANDLE;
        bQuit = true;
        break;

        // Window configuration changed
      case ConfigureNotify:
        // Inform that the window size has been changed
        m_pFrontendOS->onSize();
        break;

      case UnmapNotify:
      case FocusOut:
        m_bPause = true;
        m_pFrontendOS->onPause();
        break;

      case MapNotify:
      case FocusIn:
        m_pFrontendOS->onResume();
        m_bPause = false;
        break;

      case ClientMessage:
        // Check if the client message is a XDnD message, otherwise check for WM_DELETE_WINDOW
        if (!m_pDropHelper || !m_pDropHelper->handleClientMessage(sXEvent.xclient)) {
          // When the "WM_DELETE_WINDOW" client message is send, no "DestroyNotify"-message is generated because the
          // application itself should destroy/close the window to which the "WM_DELETE_WINDOW" client message was send to.
          // In this case, we will leave the event loop after this message was processed and no other messages are in the queue.
          // -> No "DestroyNotify"-message can be received
          if (sXEvent.xclient.data.l[0] == WM_DELETE_WINDOW)
            bQuit = true;
        }
        break;

      case SelectionNotify:
        if (m_pDropHelper)
          m_pDropHelper->handleXdndSelection(sXEvent.xselection);
        break;

      case KeyPress:
        RE_LOG(Info, "KeyPress")
        // Is it allowed to toggle the fullscreen mode using hotkeys? (Alt-Return or AltGr-Return)
        if (m_pFrontendOS->getToggleFullscreenMode()) {
          // It's allowed, toggle fullscreen right now?
          const unsigned int nKey = XLookupKeysym(&sXEvent.xkey, 0);
          if (nKey == XK_Return && ((sXEvent.xkey.state & Mod1Mask) || (sXEvent.xkey.state & Mod2Mask))) {
            // Toggle fullscreen mode
            m_pFrontendOS->setFullscreen(!m_pFrontendOS->isFullscreen());
          }
        }
        break;
      case KeyRelease:
        RE_LOG(Info, "KeyRelease")
        break;

      case ButtonPress:
        RE_LOG(Info, "ButtonPress")
        break;
      case ButtonRelease:
        RE_LOG(Info, "ButtonRelease")
        break;

        // Mouse entered the window area
      case EnterNotify:
        m_bIsMouseOver = true;
        break;

        // Mouse left the window area
      case LeaveNotify:
        m_bIsMouseOver = false;
        break;
    }
  }

  // Done
  return bQuit;
}

String WindowOSLinux::getTitle() const
{
  if (m_nNativeWindowHandle) {
    // Request the window title from the OS
    Atom		   sPropertyType;
    int			   nDataUnit;
    unsigned long  nBytesLeft;
    unsigned long  nNumberOfUnits;
    unsigned char *pszName;

    // Try first getting as utf-8 string
    if (XGetWindowProperty(m_pDisplay, m_nNativeWindowHandle, WM_NAME, 0, 65536/sizeof(long), False, UTF8_STRING,
                           &sPropertyType, &nDataUnit, &nNumberOfUnits, &nBytesLeft, &pszName) == XLib::Success) {
      // Return the window title
      return String(reinterpret_cast<const char*>(pszName));

      // Try getting as plain text
    } else if (XGetWindowProperty(m_pDisplay, m_nNativeWindowHandle, WM_NAME, 0, 65536/sizeof(long), False, XA_STRING,
                                  &sPropertyType, &nDataUnit, &nNumberOfUnits, &nBytesLeft, &pszName) == XLib::Success) {
      // Return the window title
      return String(reinterpret_cast<const char*>(pszName));
    }
  }

  // Error!
  return "";
}

void WindowOSLinux::setTitle(const String &sTitle)
{
  if (m_nNativeWindowHandle) {
    // We need here the number of bytes of the string because the number of characters (returned by String::Length) and the used number of bytes can differ in an utf-8 string
    // String::getNumOfBytes can't be used here because the String class doesn't support UTF-8 as an internal format.
    // Even if the String class uses UNICODE as internal format this wouldn't work either.
    // Because the UNICODE String format uses wchar_t as datatype and the size of this type is greater than 1 byte.
    // Each character in a string needs the same amount of bytes.
    // In an utf-8 encoded string the byte-count per character can differ between 1 byte (ASCII char) and 4 bytes (UNICODE char in Unicode Code point range 	U+010000 to U+10FFFF)
    const unsigned char *pszWindowTitle = reinterpret_cast<const unsigned char*>(sTitle.cstr());
    const int nNumOfElements = strlen(reinterpret_cast<const char*>(pszWindowTitle));
    XChangeProperty(m_pDisplay, m_nNativeWindowHandle, WM_NAME,				 UTF8_STRING, 8, PropModeReplace, pszWindowTitle, nNumOfElements);
    XChangeProperty(m_pDisplay, m_nNativeWindowHandle, _NET_WM_NAME,		 UTF8_STRING, 8, PropModeReplace, pszWindowTitle, nNumOfElements);
    XChangeProperty(m_pDisplay, m_nNativeWindowHandle, _NET_WM_VISIBLE_NAME, UTF8_STRING, 8, PropModeReplace, pszWindowTitle, nNumOfElements);
  }
}

int WindowOSLinux::getX() const
{
  if (m_nNativeWindowHandle) {
    // Get X window geometry information
    ::Window nRootWindow = 0;
    int nPositionX = 0, nPositionY = 0;
    unsigned int nWidth = 0, nHeight = 0, nBorder = 0, nDepth = 0;
    XGetGeometry(m_pDisplay, m_nNativeWindowHandle, &nRootWindow, &nPositionX, &nPositionY, &nWidth, &nHeight, &nBorder, &nDepth);

    // Lookout! We need the position relative to the root window! (window decoration topic, don't do the horror below and you just get wrong position data)
    ::Window nChildWindow = 0;
    XTranslateCoordinates(m_pDisplay, m_nNativeWindowHandle, nRootWindow, nPositionX, nPositionY, &nPositionX, &nPositionY, &nChildWindow);
    XGetGeometry(m_pDisplay, nChildWindow, &nRootWindow, &nPositionX, &nPositionY, &nWidth, &nHeight, &nBorder, &nDepth);

    // Return the window x position (in screen coordinates)
    return nPositionX;
  } else {
    // Error!
    return 0;
  }
}

int WindowOSLinux::getY() const
{
  if (m_nNativeWindowHandle) {
    // Get X window geometry information
    ::Window nRootWindow = 0;
    int nPositionX = 0, nPositionY = 0;
    unsigned int nWidth = 0, nHeight = 0, nBorder = 0, nDepth = 0;
    XGetGeometry(m_pDisplay, m_nNativeWindowHandle, &nRootWindow, &nPositionX, &nPositionY, &nWidth, &nHeight, &nBorder, &nDepth);

    // Lookout! We need the position relative to the root window! (window decoration topic, don't do the horror below and you just get wrong position data)
    ::Window nChildWindow = 0;
    XTranslateCoordinates(m_pDisplay, m_nNativeWindowHandle, nRootWindow, nPositionX, nPositionY, &nPositionX, &nPositionY, &nChildWindow);
    XGetGeometry(m_pDisplay, nChildWindow, &nRootWindow, &nPositionX, &nPositionY, &nWidth, &nHeight, &nBorder, &nDepth);

    // Return the window y position (in screen coordinates)
    return nPositionY;
  } else {
    // Error!
    return 0;
  }
}

uint32 WindowOSLinux::getWidth() const
{
  if (m_nNativeWindowHandle) {
    // Get X window geometry information
    ::Window nRootWindow = 0;
    int nPositionX = 0, nPositionY = 0;
    unsigned int nWidth = 0, nHeight = 0, nBorder = 0, nDepth = 0;
    XGetGeometry(m_pDisplay, m_nNativeWindowHandle, &nRootWindow, &nPositionX, &nPositionY, &nWidth, &nHeight, &nBorder, &nDepth);

    // Return the window width
    return nWidth;
  } else {
    // Error!
    return 0;
  }
}

uint32 WindowOSLinux::getHeight() const
{
  if (m_nNativeWindowHandle) {
    // Get X window geometry information
    ::Window nRootWindow = 0;
    int nPositionX = 0, nPositionY = 0;
    unsigned int nWidth = 0, nHeight = 0, nBorder = 0, nDepth = 0;
    XGetGeometry(m_pDisplay, m_nNativeWindowHandle, &nRootWindow, &nPositionX, &nPositionY, &nWidth, &nHeight, &nBorder, &nDepth);

    // Return the window height
    return nHeight;
  } else {
    // Error!
    return 0;
  }
}

void WindowOSLinux::setWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
  if (m_nNativeWindowHandle) {
    { // Correct frontend position and size settings
      // Get screen info (the documentation doesn't state whether or not we need to free this screen when we're done, so we don't free it)
      Screen *pScreenInfo = XScreenOfDisplay(m_pDisplay, 0);

      // Correct frontend position and size settings
      FrontendOS::correctPositionSize(nX, nY, nWidth, nHeight, 0, 0, XWidthOfScreen(pScreenInfo), XHeightOfScreen(pScreenInfo));
    }

    // If the window is not visible yet, make it visible right now
    makeVisible();

    { // Set OS window position and size
      // Set position and size (only works correctly when the window is already visible, that's why the "MakeVisible()" call must be done first)
      XWindowChanges sChanges;
      sChanges.x		= nX;
      sChanges.y		= nY;
      sChanges.width	= nWidth;
      sChanges.height	= nHeight;
      XConfigureWindow(m_pDisplay, m_nNativeWindowHandle, CWX | CWY | CWWidth | CWHeight, &sChanges);

      // Do it!
      XSync(m_pDisplay, False);
    }
  }
}

void WindowOSLinux::setFullscreenAltTab(bool bAllowed)
{
  // Nothing to do in here
}

void WindowOSLinux::setFullscreen(bool bFullscreen)
{
  // Set/remove _NET_WM_STATE_FULLSCREEN to toggle fullscreen mode.
  // The window manger is responsible to restore the original position and size of the window when the fullscreen mode should be left.
  // This works only on window manger which are EWMH (v1.3 or greater) compatible (http://standards.freedesktop.org/wm-spec/wm-spec-1.3.html)
  Atom wmFullScreen = XInternAtom(m_pDisplay, "_NET_WM_STATE_FULLSCREEN", False);
  Atom wmState = XInternAtom(m_pDisplay, "_NET_WM_STATE", False);

  // Setup the X-event
  XEvent sXEvent;
  memset(&sXEvent, 0, sizeof(sXEvent));
  sXEvent.type                 = ClientMessage;
  sXEvent.xclient.window       = m_nNativeWindowHandle;
  sXEvent.xclient.message_type = wmState;
  sXEvent.xclient.format       = 32;
  sXEvent.xclient.data.l[0]    = bFullscreen ? 1 : 0;	// The atom should be added (= 1) or removed (= 0)
  sXEvent.xclient.data.l[1]    = wmFullScreen;

  // Send message to the root window to inform the window manager about the change
  XSendEvent(m_pDisplay, DefaultRootWindow(m_pDisplay), False, SubstructureNotifyMask, &sXEvent);

  // Do it!
  XSync(m_pDisplay, False);

  // Inform that the fullscreen mode was changed (in here, usually e.g. the display resolution is updated and so on)
  // -> This has really be done "after" the OS window has been set to fullscreen, else we don't see the window content
  // (looks like the OS takes care of situations like changing the display resolution and updating the window dimension automatically)
  m_pFrontendOS->onFullscreenMode();
}

void WindowOSLinux::refreshFullscreen()
{
  // This information is only interesting if we're currently in fullscreen mode, if not, just ignore this method call
  if (m_pFrontendOS->isFullscreen()) {
    // Set/remove _NET_WM_STATE_FULLSCREEN to toggle fullscreen mode.
    // The window manger is responsible to restore the original position and size of the window when the fullscreen mode should be left.
    // This works only on window manger which are EWMH (v1.3 or greater) compatible (http://standards.freedesktop.org/wm-spec/wm-spec-1.3.html)
    Atom wmFullScreen = XInternAtom(m_pDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    Atom wmState = XInternAtom(m_pDisplay, "_NET_WM_STATE", False);

    // Setup the X-event
    XEvent sXEvent;
    memset(&sXEvent, 0, sizeof(sXEvent));
    sXEvent.type                 = ClientMessage;
    sXEvent.xclient.window       = m_nNativeWindowHandle;
    sXEvent.xclient.message_type = wmState;
    sXEvent.xclient.format       = 32;
    sXEvent.xclient.data.l[1]    = wmFullScreen;

    // Leave fullscreen - Send message to the root window to inform the window manager about the change
    sXEvent.xclient.data.l[0] = 0;	// The atom should be added (= 1) or removed (= 0)
    XSendEvent(m_pDisplay, DefaultRootWindow(m_pDisplay), False, SubstructureNotifyMask, &sXEvent);
    XSync(m_pDisplay, False); // Do it!

    // Inform that the fullscreen mode was changed (in here, usually e.g. the display resolution is updated and so on)
    m_pFrontendOS->onFullscreenMode();

    // Reenter fullscreen - Send message to the root window to inform the window manager about the change
    sXEvent.xclient.data.l[0] = 1;	// The atom should be added (= 1) or removed (= 0)
    XSendEvent(m_pDisplay, DefaultRootWindow(m_pDisplay), False, SubstructureNotifyMask, &sXEvent);
    XSync(m_pDisplay, False); // Do it!
  }
}

bool WindowOSLinux::isMouseOver() const
{
  return m_bIsMouseOver;
}

int WindowOSLinux::getMousePositionX() const
{
  // Get the absolute mouse cursor position on the screen
  XEvent sXEvent;

  // Get the window directly below the current mouse cursor position - do only continue if this is our window
  if (XQueryPointer(m_pDisplay, m_nNativeWindowHandle, &sXEvent.xbutton.root, &sXEvent.xbutton.window,
                    &sXEvent.xbutton.x_root, &sXEvent.xbutton.y_root, &sXEvent.xbutton.x, &sXEvent.xbutton.y, &sXEvent.xbutton.state) == True) {
    // Done
    return sXEvent.xbutton.x;
  } else {
    // Error, the mouse cursor is currently not over this window
    return -1;
  }
}

int WindowOSLinux::getMousePositionY() const
{
  // Get the absolute mouse cursor position on the screen
  XEvent sXEvent;

  // Get the window directly below the current mouse cursor position - do only continue if this is our window
  if (XQueryPointer(m_pDisplay, m_nNativeWindowHandle, &sXEvent.xbutton.root, &sXEvent.xbutton.window,
                    &sXEvent.xbutton.x_root, &sXEvent.xbutton.y_root, &sXEvent.xbutton.x, &sXEvent.xbutton.y, &sXEvent.xbutton.state) == True) {
    // Done
    return sXEvent.xbutton.y;
  } else {
    // Error, the mouse cursor is currently not over this window
    return -1;
  }
}

bool WindowOSLinux::isMouseVisible() const
{
  return m_bMouseVisible;
}

void WindowOSLinux::setMouseVisible(bool bVisible)
{
  // Backup the state
  m_bMouseVisible = bVisible;

  // Set the mouse cursor state
  if (bVisible)
    XUndefineCursor(m_pDisplay, m_nNativeWindowHandle);
  else
    XDefineCursor(m_pDisplay, m_nNativeWindowHandle, m_nInvisibleCursor);
}

void WindowOSLinux::setTrapMouse(bool bTrap)
{
  // Check if the window has already been destroyed
  if (m_nNativeWindowHandle) {
    // Set or release mouse capture?
    if (bTrap) {
      // Grab mouse
      XGrabPointer(m_pDisplay, m_nNativeWindowHandle, True, 0, GrabModeAsync, GrabModeAsync, m_nNativeWindowHandle, XLib::None, CurrentTime);
    } else {
      // Release mouse grab
      XUngrabPointer(m_pDisplay, CurrentTime);
    }

    // Do it!
    XSync(m_pDisplay, False);
  }
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Signal handler callback
*/
void WindowOSLinux::signalHandler(int nSignal)
{
  // Catch signal
  switch (nSignal) {
    // Interrupt (exit application by ctrl-c)
    case SIGINT:
      // The OS asks us to shut down
      g_bSignalSystemQuit = true;

      // Signal handler has done it's job, re-raise signal
      signal(nSignal, SIG_DFL);
      raise(nSignal);
      break;

      // Terminate (exit application)
    case SIGTERM:
      // The OS asks us to shut down
      g_bSignalSystemQuit = true;

      // Signal handler has done it's job, re-raise signal
      signal(nSignal, SIG_DFL);
      raise(nSignal);
      break;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
