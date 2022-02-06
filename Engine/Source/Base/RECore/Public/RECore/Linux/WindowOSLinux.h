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


#ifndef __PLFRONTENDOS_OSWINDOW_LINUX_H__
#define __PLFRONTENDOS_OSWINDOW_LINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Frontend/WindowOS.h"
#include "RECore/Frontend/WindowDescription.h"
#include "RECore/Linux/X11Includes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FrontendOS;
class Linux_XDnDFileDropHelper;
class WindowOS;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OS Linux frontend implementation class using X11
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'System' abstraction
*/
class RECORE_API WindowOSLinux : public WindowOS {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class FrontendOS;
  friend class WindowOS;
  friend class Linux_XDnDFileDropHelper;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] cFrontendOS
  *    Owner frontend implementation instance
  */
  WindowOSLinux(FrontendOS &cFrontendOS);

  /**
  *  @brief
  *    Destructor
  */
  ~WindowOSLinux() override;


  /**
   * @brief
   * Returns a pointer to the X11 display.
   *
   * @return
   * Pointer to the X11 display.
   */
  ::Display* getDisplay() const;

  /**
  *  @brief
  *    If the window is not visible yet, make it visible right now
  */
  void makeVisible();

  /**
  *  @brief
  *    Creates an invisible cursor
  */
  void createInvisibleCursor();

  /**
  *  @brief
  *    Gets called from the Linux_XDnDFileDropHelper when a drop occurred
  *
  *  @param[in] lstFiles
  *    List of file names
  */
  void onDrop(const std::vector<RECore::String> &lstFiles) const;

  //[-------------------------------------------------------]
  //[ Private WindowOSImpl functions                        ]
  //[-------------------------------------------------------]
public:
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
  //[ Private static functions                              ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Signal handler callback
  *
  *  @param[in] nSignal
  *    Signal
  */
  static void signalHandler(int nSignal);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  FrontendOS  *m_pFrontendOS;			/**< Owner frontend implementation instance, always valid! */
  ::Display *m_pDisplay;				/**< System display, considered to be always valid */
  ::Window   m_nNativeWindowHandle;	/**< Native window handle, can be a null handle */
  bool	   m_bInitialized;			/**< Initialization done? */
  bool	   m_bVisible;				/**< Was the window made visible? (independent of the real OS window visibility state) */
  bool	   m_bIsMouseOver;			/**< Is the mouse cursor currently over the window? */
  bool	   m_bMouseVisible;			/**< Is the mouse cursor visible? */
  ::Cursor   m_nInvisibleCursor;		/**< The invisible cursor, can be null */
  bool	   m_bPause;				/**< Are we're currently within the pause-state? (used to avoid calling "OnPause()"-twice) */
  // Atoms
  ::Atom					 WM_DELETE_WINDOW;		/**< System atom for delete */
  ::Atom					 UTF8_STRING;			/**< Atom for the type of a window title */
  ::Atom 					 WM_NAME;				/**< Window title (old?) */
  ::Atom 					 _NET_WM_NAME;			/**< Window title */
  ::Atom 					 _NET_WM_VISIBLE_NAME;	/**< Window title (visible title, can be different) */
  Linux_XDnDFileDropHelper *m_pDropHelper;		/**< XDnD drop helper instance, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


#endif // __PLFRONTENDOS_OSWINDOW_LINUX_H__

