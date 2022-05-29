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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RECore/Windows/Windows.h>
#include "RECore/Frontend/WindowOSImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Frontend;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OS Window frontend implementation class using standard Win32-functions
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'System' abstraction
*/
class WindowsWindowOS : public WindowOS {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class Frontend;


  //[-------------------------------------------------------]
  //[ Private static functions                              ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Static window callback function
  */
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] cFrontendOS
  *    Owner frontend implementation instance
  */
  WindowsWindowOS(Frontend &cFrontendOS);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~WindowsWindowOS();

  /**
  *  @brief
  *    Register a hotkey
  *
  *  @param[in] nModKey
  *    Modification key (for example VK_MENU (alt-key))
  *  @param[in] nKey
  *    Key (for example VK_RETURN)
  *
  *  @return
  *    Hotkey ID, 0 on error
  */
  int registerHotkey(UINT nModKey, UINT nKey);

  /**
  *  @brief
  *    Unregister a hotkey
  *
  *  @param[in] nID
  *    Hotkey ID
  */
  void unregisterHotkey(int nID);

  /**
  *  @brief
  *    Update trap mouse if required
  *
  *  @note
  *    - Should be called after window position or size was changed
  */
  void updateTrapMouse();

  /**
  *  @brief
  *    If the window is not visible yet, make it visible right now
  */
  void makeVisible();


  //[-------------------------------------------------------]
  //[ Private virtual OSWindow functions                    ]
  //[-------------------------------------------------------]
private:
  RECore::handle getNativeWindowHandle() const override;
  void redraw() override;
  bool ping() override;
  RECore::String getTitle() const override;
  void setTitle(const RECore::String &sTitle) override;
  int getX() const override;
  int getY() const override;
  RECore::uint32 getWidth() const override;
  RECore::uint32 getHeight() const override;
  void setWindowPositionSize(int nX, int nY, RECore::uint32 nWidth, RECore::uint32 nHeight) override;
  void setFullscreenAltTab(bool bAllowed) override;
  void setFullscreen(bool bFullscreen) override;
  void refreshFullscreen() override;
  bool isMouseOver() const override;
  int getMousePositionX() const override;
  int getMousePositionY() const override;
  bool isMouseVisible() const override;
  void setMouseVisible(bool bVisible) override;
  void setTrapMouse(bool bTrap) override;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  Frontend   *m_pFrontendOS;				/**< Owner frontend implementation instance, always valid! */
  HINSTANCE	m_hInstance;				/**< OS main process handle */
  DWORD		m_nThreadID;				/**< OS main thread ID */
  WNDCLASS	m_WndClass;					/**< OS window class */
  HICON		m_hIcon;					/**< Extracted default icon of the process (don't forget to call "DestroyIcon()" on it), can be a null pointer */
  HWND		m_hWnd;						/**< OS window handle, can be a null pointer */
  bool		m_bInitialized;				/**< Initialization done? */
  bool		m_bBackgroundInitialized;	/**< Background initialization done? (if false, WM_ERASEBKGND won't be caught) */
  bool		m_bVisible;					/**< Was the window made visible? (independent of the real OS window visibility state) */
  bool		m_bDestroyed;				/**< 'true' if the window has already been destroyed */
  int			m_nHotkeyIDAltTab;			/**< Alt-Tab hotkey */
  bool		m_bWindowRectBackup;		/**< Is there a window position & size backup? */
  RECT		m_sWindowRectBackup;		/**< Window position & size backup */
  bool		m_bMouseVisible;			/**< Is the mouse cursor visible? */
  bool		m_bTrapMouse;				/**< Trap mouse? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore