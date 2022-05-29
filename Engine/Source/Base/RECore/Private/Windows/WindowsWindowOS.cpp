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
#include <RECore/Time/Time.h>
#include <RECore/Frontend/Frontend.h>
#include <RECore/Frontend/FrontendContext.h>
#include "RECore/Frontend/FrontendOS.h"
#include "RECore/Windows/WindowsWindowOS.h"
#include <Shellapi.h>	// For "ExtractIcon()"

#define IDI_PL	                       101

// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static window callback function
*/
LRESULT CALLBACK WindowsWindowOS::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
  // Get pointer to window
  WindowsWindowOS *pWindowsWindowOS = nullptr;
  if (nMsg == WM_CREATE) {
    pWindowsWindowOS = static_cast<WindowsWindowOS*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
  } else {
  if (hWnd)
    pWindowsWindowOS = reinterpret_cast<WindowsWindowOS*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
  }

  // Disable screen saver and monitor power management...
  if (nMsg == WM_SYSCOMMAND && (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER))
    return 0;

  // If we get a message for a dead window, something is wrong
  if (pWindowsWindowOS && pWindowsWindowOS->m_bDestroyed) {
    // There is always a WM_NCDESTROY coming after WM_DESTROY, so this will be called which
    // is OK. But we don't need the message, so just give back here to be sure that nothing can go wrong...
    return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
  }

  // We need a window now
  if (pWindowsWindowOS) {
    // Process message
    switch (nMsg) {
    // Initialize window
    case WM_CREATE:
      // Set window pointer and handle (SetWindowLongPtr is the 64bit equivalent to SetWindowLong)
      ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindowsWindowOS));

      // Backup the native window handle
      pWindowsWindowOS->m_hWnd = hWnd;

      // Doing the following frontend life cycle thing in here would be nice, but when done this way, the window border
      // will be potentially invisible until OnStart() returns... but this may take some seconds... not nice to look at...
      // pWindowsWindowOS->m_pFrontendOS->OnStart();
      return 0;

    // Destroy window
    case WM_DESTROY:
      // Do the frontend life cycle thing - stop
      pWindowsWindowOS->m_pFrontendOS->onStop();

      // Mark window destroyed
      pWindowsWindowOS->m_bDestroyed = true;
      pWindowsWindowOS->m_hWnd	   = nullptr;

      // Request application shut down
      PostQuitMessage(0);
      return 0;

    // Got focus
    case WM_SETFOCUS:
    case WM_EXITSIZEMOVE:	// Beside focus, do the same for moving/sizing a window - it just feels right this way
      // (e.g. no spinning around controlled camera while sizing a window)
      // Do the frontend life cycle thing - resume
      pWindowsWindowOS->m_pFrontendOS->onResume();
      return 0;

    // Lost focus
    case WM_KILLFOCUS:
    case WM_ENTERSIZEMOVE:	// Beside focus, do the same for moving/sizing a window - it just feels right this way
      // (e.g. no spinning around controlled camera while sizing a window)
      // Do the frontend life cycle thing - pause
      pWindowsWindowOS->m_pFrontendOS->onPause();
      return 0;

    case WM_SYSKEYDOWN:
      // Is it allowed to toggle the fullscreen mode using hotkeys? If so, toggle fullscreen right now? (Alt-Return)
      if (pWindowsWindowOS->m_pFrontendOS->getToggleFullscreenMode() && wParam == VK_RETURN && (lParam & (1 << 29))) {	// Bit 29 = the ALT-key
        // Toggle fullscreen mode
        pWindowsWindowOS->m_pFrontendOS->setFullscreen(!pWindowsWindowOS->m_pFrontendOS->isFullscreen());
        return 0;
      }
      break;

    // Keyboard key down
    case WM_KEYDOWN:
      // Is it allowed to toggle the fullscreen mode using hotkeys? If so, toggle fullscreen right now? (AltGr-Return)
      if (wParam == VK_RETURN && GetAsyncKeyState(VK_RMENU)) {
        // Toggle fullscreen mode
        pWindowsWindowOS->m_pFrontendOS->setFullscreen(!pWindowsWindowOS->m_pFrontendOS->isFullscreen());
      }
      break;

    // Process hotkey
    case WM_HOTKEY:
      // We caught the hotkey
      return 0;

    case WM_ERASEBKGND:
      // Catch WM_ERASEBKGND in order to avoid black flickering?
      if (pWindowsWindowOS->m_bBackgroundInitialized)
        return 0;	// Gotcha!

      // ... no catching, the system will now "erase" the background...
      break;

    // Window paint request
    case WM_PAINT:
      {
        // Begin paint
        ::PAINTSTRUCT sPaint;
        ::BeginPaint(hWnd, &sPaint);

        // Redraw, but only if the draw area isn't null
        if (!::IsRectEmpty(&sPaint.rcPaint)) {
          // Let the frontend draw into it's window
          pWindowsWindowOS->m_pFrontendOS->onDraw();
        }

        // End paint
        ::EndPaint(hWnd, &sPaint);
        return 0;
      }

    // Window was moved
    case WM_MOVE:
      // Update trap mouse if required
      pWindowsWindowOS->updateTrapMouse();
      return 0;

    // The size of the window has been changed
    case WM_SIZE:
      // Update trap mouse if required
      pWindowsWindowOS->updateTrapMouse();

      // Inform that the window size has been changed
      pWindowsWindowOS->m_pFrontendOS->onSize();
      return 0;

    // Drag and drop of files
    case WM_DROPFILES:
      {
      // Get dropped filenames. Because there's no way - without extreme overhead :) - to check whether
      // we really need to use Unicode or ASCII is quite enough, we always use Unicode just to be sure.
      const uint32 nNumOfFiles = DragQueryFileW(reinterpret_cast<HDROP>(wParam), 0xFFFFFFFF, static_cast<LPWSTR>(nullptr), 0);
      if (nNumOfFiles) {
        // Create the file list
        std::vector<String> lstFiles;
        lstFiles.Resize(nNumOfFiles);
        for (uint32 i=0; i<nNumOfFiles; i++) {
          // Get the length of the string (+1 for \0)
          const UINT nSize = DragQueryFileW(reinterpret_cast<HDROP>(wParam), i, nullptr, 0) + 1;

          // Create the string and fill it
          wchar_t *pszFile = new wchar_t[nSize];
          DragQueryFileW(reinterpret_cast<HDROP>(wParam), i, pszFile, nSize);

          // Store the string (the PL string takes over the control)
          lstFiles[i] = String(pszFile, false, nSize - 1);
        }

        // Inform the frontend
        pWindowsWindowOS->m_pFrontendOS->onDrop(lstFiles);
        return 0;
      }
      }
    }
  }

  // Let the OS handle this message...
  return hWnd ? ::DefWindowProc(hWnd, nMsg, wParam, lParam) : 0;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WindowsWindowOS::WindowsWindowOS(Frontend &cFrontendOS)
: m_pFrontendOS(&cFrontendOS)
, m_hInstance(::GetModuleHandle(nullptr))
, m_nThreadID(::GetCurrentThreadId())
, m_hWnd(nullptr)
, m_bInitialized(false)
, m_bBackgroundInitialized(false)
, m_bVisible(false)
, m_bDestroyed(false)
, m_nHotkeyIDAltTab(0)
, m_bWindowRectBackup(false)
, m_bMouseVisible(true)
, m_bTrapMouse(false) {
  MemoryManager::Set(&m_sWindowRectBackup, 0, sizeof(RECT));

  // Tell the frontend about this instance at once because it may already be required during frontend life cycle initialization
  m_pFrontendOS->m_pActiveTopLevelWindow = this;

  // Create window class
  m_WndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  m_WndClass.lpfnWndProc		= static_cast<WNDPROC>(WndProc);
  m_WndClass.cbClsExtra		= 0;
  m_WndClass.cbWndExtra		= 0;
  m_WndClass.hInstance		= m_hInstance;
  m_WndClass.hIcon			= nullptr;	// ... set below...
  m_WndClass.hCursor			= ::LoadCursor(nullptr, IDC_ARROW);
  m_WndClass.hbrBackground	= ::CreateSolidBrush(RGB(0, 0, 0));	// By using a brush, the system will emit WM_ERASEBKGND (nullptr would result in a nasty white window as soon as it's shown)
  m_WndClass.lpszMenuName		= nullptr;
  m_WndClass.lpszClassName	= TEXT("RECore_WindowsWindowOS");

  { // Use the default process icon... if there's one...
    // Get the filename of this process
    wchar_t szModule[MAX_PATH];
    ::GetModuleFileName(m_hInstance, szModule, MAX_PATH);

    // Extract the icon (don't forget to call "::DestroyIcon()" on it)
    m_WndClass.hIcon = m_hIcon = ::ExtractIcon(m_hInstance, szModule, 0);
  }

  // If there's no default process icon, we're using the standard RacoonEngine icon
  if (!m_WndClass.hIcon) {
    // "::GetModuleHandle(nullptr)" returns the instance of the calling process, but for the icon we need the one of this shared library

    // Get the filename of this shared library
    MEMORY_BASIC_INFORMATION sMemoryBasicInformation;
    static const int nDummy = 0;
    ::VirtualQuery(&nDummy, &sMemoryBasicInformation, sizeof(MEMORY_BASIC_INFORMATION));
    wchar_t szModule[MAX_PATH];
    ::GetModuleFileName(static_cast<HMODULE>(sMemoryBasicInformation.AllocationBase), szModule, MAX_PATH);

    // Finally, load the icon with the instance of this shared library
    m_WndClass.hIcon = ::LoadIcon(::GetModuleHandle(szModule), MAKEINTRESOURCE(IDI_PL));
  }

  // Register window class
  if (::RegisterClass(&m_WndClass)) {
    // Set window style
    const DWORD dwStyle    = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    const DWORD dwExtStyle = WS_EX_APPWINDOW | WS_EX_ACCEPTFILES;

    // Create the window
    m_hWnd = ::CreateWindowExA(dwExtStyle,
                               "RECore_WindowsWindowOS",
                               m_pFrontendOS->GetFrontend() ? m_pFrontendOS->GetFrontend()->GetContext().GetName() : "",
                               dwStyle,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               HWND_DESKTOP,
                               0,
                               ::GetModuleHandle(nullptr),
                               this);
    if (m_hWnd) {
      // Do the frontend life cycle thing - start
      m_pFrontendOS->OnStart();

      // Initialization is done
      m_bInitialized = m_bBackgroundInitialized = true;

      // If the window is not visible yet, make it visible right now
      MakeVisible();
    } else {
      // Could not create window
      m_bDestroyed = true;
    }
  } else {
    // Error registering window class
  }
}

/**
*  @brief
*    Destructor
*/
WindowsWindowOS::~WindowsWindowOS() {
  // Check if the window has already been destroyed
  if (m_hWnd) {
    // Send destroy message to window
    ::DestroyWindow(m_hWnd);
  }

  // Unregister window class
  if (!::UnregisterClass(L"RECore_WindowsWindowOS", m_hInstance)) {
    // Error unregistering window class
  }

  // Is there an extracted icon to destroy?
  if (m_hIcon)
    ::DestroyIcon(m_hIcon);
}

int WindowsWindowOS::RegisterHotkey(UINT nModKey, UINT nKey) {
  if (m_hWnd) {
    // Get new hotkey ID
    static int nHotkeyID = 0xC000;
    nHotkeyID++;

    // Register hotkey
    ::RegisterHotKey(m_hWnd, nHotkeyID, nModKey, nKey);
    return nHotkeyID;
  } else {
    // Error!
    return 0;
  }
}

void WindowsWindowOS::unregisterHotkey(int nID) {
  // Unregister hotkey
  if (m_hWnd)
    ::UnregisterHotKey(m_hWnd, nID);
}

/**
*  @brief
*    Update trap mouse if required
*/
void WindowsWindowOS::updateTrapMouse() {
  // Trap mouse?
  if (m_bTrapMouse) {
    // Get window rectangle (in screen coordinates)
    RECT sRect;
    ::GetWindowRect(m_hWnd, &sRect);

    // Trap mouse within up-to-date widget rectangle
    ::ClipCursor(&sRect);
  }
}

/**
*  @brief
*    If the window is not visible yet, make it visible right now
*/
void WindowsWindowOS::makeVisible() {
  if (!m_bVisible && m_hWnd) {
    // The window is now considered to be visible
    m_bVisible = true;

    // Show the window and activate it right now - this also sends WM_ERASEBKGND which now clears the content to black
    ::ShowWindow(m_hWnd, SW_SHOW);
  }
}


//[-------------------------------------------------------]
//[ Private virtual OSWindow functions                    ]
//[-------------------------------------------------------]
handle WindowsWindowOS::getNativeWindowHandle() const {
  return reinterpret_cast<handle>(m_hWnd);
}

void WindowsWindowOS::redraw() {
  // Initialization done?
  if (!m_bBackgroundInitialized) {
    // If the window is not visible yet, make it visible right now
    MakeVisible();

    // Ok, someone asked us to redraw, so we just assume that for this person
    // the initialization is done and everything is usable for drawing.
    // WM_ERASEBKGND will now be caught.
    m_bBackgroundInitialized = true;
  }

  // Redraw
  if (m_hWnd)
    ::RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE);
}

bool WindowsWindowOS::ping() {
  bool bQuit = false;

  // Check if we're allowed to perform an update right now, please note that an update is only allowed when the frontend is fully initialized
  if (m_bInitialized && Timing::GetInstance()->Update()) {
    // Let the frontend update it's states (do this before drawing else, e.g. the first frame may have an unwanted content)
    m_pFrontendOS->OnUpdate();
  }

  // Look if messages are waiting (non-blocking)
  MSG sMsg;
  while (::PeekMessage(&sMsg, nullptr, 0, 0, PM_NOREMOVE)) {
    // Get the waiting message
    ::GetMessage(&sMsg, nullptr, 0, 0);
    if (sMsg.message == WM_QUIT)
      bQuit = true;

    // Process message
    ::TranslateMessage(&sMsg);
    ::DispatchMessage(&sMsg);
  }

  // Done
  return bQuit;
}

String WindowsWindowOS::getTitle() const {
  // First of all, get the length of the title (excluding the terminating zero, so we need to add +1)
  const int nLength = ::GetWindowTextLengthW(m_hWnd) + 1;
  if (nLength) {
    // Get title
    wchar_t *pszTitle = new wchar_t[nLength];
    if (::GetWindowTextW(m_hWnd, pszTitle, nLength)) {
      return String(pszTitle, false, nLength-1); // Do not copy, please
    } else {
      // We no longer need the data, so free it
      delete [] pszTitle;
    }
  }

  // Error!
  return "";
}

void WindowsWindowOS::setTitle(const String &sTitle) {
  if (m_hWnd)
    ::SetWindowTextW(m_hWnd, sTitle);
}

int WindowsWindowOS::getX() const {
  if (m_hWnd) {
    // Get window rect (in screen coordinates)
    RECT sRect;
    ::GetWindowRect(m_hWnd, &sRect);
    return sRect.left;
  } else {
    // Error!
    return 0;
  }
}

int WindowsWindowOS::getY() const {
  if (m_hWnd) {
    // Get window rect (in screen coordinates)
    RECT sRect;
    ::GetWindowRect(m_hWnd, &sRect);
    return sRect.top;
  } else {
    // Error!
    return 0;
  }
}

uint32 WindowsWindowOS::getWidth() const {
  if (m_hWnd) {
    // Request a relative window position (always (0, 0)) and size (equal to (width, height))
    RECT sRect;
    ::GetClientRect(m_hWnd, &sRect);
    return sRect.right;
  } else {
    // Error!
    return 0;
  }
}

uint32 WindowsWindowOS::getHeight() const {
  if (m_hWnd) {
    // Request a relative window position (always (0, 0)) and size (equal to (width, height))
    RECT sRect;
    ::GetClientRect(m_hWnd, &sRect);
    return sRect.bottom;
  } else {
    // Error!
    return 0;
  }
}

void WindowsWindowOS::setWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight) {
  if (m_hWnd) {
    // Correct frontend position and size settings
    Frontend::CorrectPositionSize(nX, nY, nWidth, nHeight, GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN), GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));

    // Lookout! "MoveWindow()" sets the window size, not the window client area size
    // -> We have to calculate this window size basing on the given client area size
    RECT cRect = {nX, nY, nX+nWidth, nY+nHeight};
    if (::AdjustWindowRect(&cRect, GetWindowLong(m_hWnd, GWL_STYLE), FALSE)) {
      // Set OS window position and size
      ::MoveWindow(m_hWnd, nX, nY, cRect.right-cRect.left, cRect.bottom-cRect.top, TRUE);

      // If the window is not visible yet, make it visible right now
      MakeVisible();
    }
  }
}

void WindowsWindowOS::setFullscreenAltTab(bool bAllowed) {
  if (m_hWnd) {
    // Register/unregister hotkey
    if (bAllowed && m_pFrontendOS->IsFullscreen()) {
      // Catch Alt-Tab hotkey so it can't be used during fullscreen
      if (!m_nHotkeyIDAltTab)
        m_nHotkeyIDAltTab = RegisterHotkey(MOD_ALT, VK_TAB);
    } else {
      // Release Alt-Tab hotkey
      if (m_nHotkeyIDAltTab) {
        UnregisterHotkey(m_nHotkeyIDAltTab);
        m_nHotkeyIDAltTab = 0;
      }
    }
  }
}

void WindowsWindowOS::setFullscreen(bool bFullscreen) {
  if (m_hWnd) {
    // Update fullscreen Alt-Tab
    SetFullscreenAltTab(m_pFrontendOS->GetFullscreenAltTab());

    // Backup the current window position & size?
    if (bFullscreen && !m_bWindowRectBackup) {
      GetWindowRect(m_hWnd, &m_sWindowRectBackup);
      m_bWindowRectBackup = true;
    }

    { // Update window style
      // Get the current window style
      LONG nStyle = GetWindowLong(m_hWnd, GWL_STYLE);

      // Modify the current window style
      if (bFullscreen)
        nStyle &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);	// Hide decoration
      else
        nStyle |= (WS_BORDER | WS_CAPTION | WS_THICKFRAME);		// Show decoration

      // Set the new window style
      SetWindowLong(m_hWnd, GWL_STYLE, nStyle);
    }

    // Switch to fullscreen?
    if (bFullscreen) {
      // For fullscreen, place the window left/top and set new size - looks better if we do this "warm up" in here
      ::SetWindowPos(m_hWnd, nullptr, 0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN), SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    // Inform that the fullscreen mode was changed
    m_pFrontendOS->OnFullscreenMode();

    // Switch to fullscreen?
    if (bFullscreen) {
      // For fullscreen, place the window left/top and set new size - now the real size (we're a fullscreen window, we're important, bring us at the top of the Z order and ensure that we're in the focus)
      ::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
    } else {
      // Restore window position & size
      if (m_bWindowRectBackup) {
        ::SetWindowPos(m_hWnd, nullptr, m_sWindowRectBackup.left, m_sWindowRectBackup.top, m_sWindowRectBackup.right - m_sWindowRectBackup.left,
                       m_sWindowRectBackup.bottom - m_sWindowRectBackup.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
        m_bWindowRectBackup = false;
      }
    }
  }
}

void WindowsWindowOS::refreshFullscreen() {
  // This information is only interesting if we're currently in fullscreen mode, if not, just ignore this method call
  if (m_pFrontendOS->IsFullscreen()) {
    // Inform that the fullscreen mode was changed (in here, usually e.g. the display resolution is updated and so on)
    m_pFrontendOS->OnFullscreenMode();

    // Update the OS window, place the window left/top and set new size (we're a fullscreen window, we're important, bring us at the top of the Z order and ensure that we're in the focus)
    ::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
  }
}

bool WindowsWindowOS::isMouseOver() const {
  if (m_hWnd) {
    // Get the mouse cursor's position (in screen coordinates)
    POINT sPOINT;
    if (::GetCursorPos(&sPOINT)) {
      // Get window rectangle (in screen coordinates)
      RECT sRect;
      if (::GetWindowRect(m_hWnd, &sRect)) {
        // Is the mouse cursor within the window rectangle?
        return (::PtInRect(&sRect, sPOINT) == TRUE);
      }
    }
  }

  // Error!
  return false;
}

int WindowsWindowOS::getMousePositionX() const {
  if (m_hWnd) {
    // Get the mouse cursor's position (in screen coordinates)
    POINT sPoint;
    ::GetCursorPos(&sPoint);

    // Get the mouse cursor position inside this window
    if (::ScreenToClient(m_hWnd, &sPoint))
      return sPoint.x;
  }

  // Error!
  return -1;
}

int WindowsWindowOS::getMousePositionY() const {
  if (m_hWnd) {
    // Get the mouse cursor's position (in screen coordinates)
    POINT sPoint;
    ::GetCursorPos(&sPoint);

    // Get the mouse cursor position inside this window
    if (::ScreenToClient(m_hWnd, &sPoint))
      return sPoint.y;
  }

  // Error!
  return -1;
}

bool WindowsWindowOS::isMouseVisible() const {
  return m_bMouseVisible;
}

void WindowsWindowOS::setMouseVisible(bool bVisible)
{
  // Backup the state
  m_bMouseVisible = bVisible;

  // Set mouse cursor visibility
  if (bVisible) {
    // Show mouse cursor
    while (::ShowCursor(true) < 0)
      ; // Do nothing
  } else {
    // Hide mouse cursor
    while (::ShowCursor(false) >= 0)
      ; // Do nothing
  }
}

void WindowsWindowOS::setTrapMouse(bool bTrap) {
  if (m_hWnd) {
    // Trap mouse?
    if (bTrap) {
      // Get window rectangle (in screen coordinates)
      RECT sRect;
      ::GetWindowRect(m_hWnd, &sRect);

      // Trap mouse
      ::ClipCursor(&sRect);
    } else {
      // Untrap mouse
      ::ClipCursor(nullptr);
    }

    // Backup the state
    m_bTrapMouse = bTrap;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore