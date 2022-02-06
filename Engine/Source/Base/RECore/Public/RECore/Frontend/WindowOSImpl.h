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
#include "RECore/RECore.h"
#include "RECore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

class FrontendOS;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract OS window base class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class RECORE_API WindowOSImpl {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class FrontendOS;

  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  WindowOSImpl(FrontendOS& cFrontendOS);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~WindowOSImpl();


  //[-------------------------------------------------------]
  //[ Protected virtual WindowOSImpl functions              ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Get native window handle
  *
  *  @return
  *    Native window handle for the frontend window, can be a null pointer
  */
  virtual RECore::handle getNativeWindowHandle() const = 0;

  /**
  *  @brief
  *    Redraw window
  */
  virtual void redraw() = 0;

  /**
  *  @brief
  *    Ping window
  *
  *  @return
  *    'true' when to shut down the frontend, else 'false'
  */
  virtual bool ping() = 0;

  /**
  *  @brief
  *    Get frontend title
  *
  *  @return
  *    Frontend title
  *
  *  @remarks
  *    When the frontend has a window, this title can be seen within the window
  *    title bar. Please note that there's no guarantee that there's a window
  *    title bar or even a window. By default, the title is set to the frontend
  *    context name ("GetContext().GetName()") which is usually sufficient. So,
  *    unless you have a good reason to explicitly set an individual frontend
  *    title, just use the default setting and don't touch the frontend.
  */
  virtual RECore::String getTitle() const = 0;

  /**
  *  @brief
  *    Set frontend title
  *
  *  @param[in] sTitle
  *    Frontend title
  *
  *  @see
  *    - getTitle()
  */
  virtual void setTitle(const RECore::String &sTitle) = 0;

  //[-------------------------------------------------------]
  //[ Position and size                                     ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Get the x position of the frontend (in screen coordinates)
  *
  *  @return
  *    X position of the frontend
  */
  virtual int getX() const = 0;

  /**
  *  @brief
  *    Get the y position of the frontend (in screen coordinates)
  *
  *  @return
  *    Y position of the frontend
  */
  virtual int getY() const = 0;

  /**
  *  @brief
  *    Get window width
  *
  *  @return
  *    Width
  */
  virtual RECore::uint32 getWidth() const = 0;

  /**
  *  @brief
  *    Get window height
  *
  *  @return
  *    Height
  */
  virtual RECore::uint32 getHeight() const = 0;

  /**
  *  @brief
  *    Set frontend position and size
  *
  *  @param[in] nX
  *    X position of the frontend (in screen coordinates)
  *  @param[in] nY
  *    Y position of the frontend (in screen coordinates)
  *  @param[in] nWidth
  *    Width of the frontend
  *  @param[in] nHeight
  *    Height of the frontend
  *
  *  @remarks
  *    The primary argument to allow the user to request a frontend position and size change is,
  *    that it should be possible to restore the frontend position and size of a previous session
  *    (may be important for the usability). Do not misuse this method to frequently manipulate
  *    the frontend appearance. Please note that, as for all other frontend methods, this is only
  *    considered to be a request. A frontend implementation may deny the request in general or
  *    just improper settings (e.g. a too small size, position outside the visible screen etc.).
  */
  virtual void setWindowPositionSize(int nX, int nY, RECore::uint32 nWidth, RECore::uint32 nHeight) = 0;

  //[-------------------------------------------------------]
  //[ Fullscreen                                            ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Sets whether it's allowed to use Alt-Tab if fullscreen mode is used
  *
  *  @param[in] bAllowed
  *    Is it allowed to use Alt-Tab within fullscreen mode?
  */
  virtual void setFullscreenAltTab(bool bAllowed) = 0;

  /**
  *  @brief
  *    Sets the window's fullscreen mode
  *
  *  @param[in] bFullscreen
  *    'true' if the window should be in fullscreen mode, else 'false'
  */
  virtual void setFullscreen(bool bFullscreen) = 0;

  /**
  *  @brief
  *    Something related to fullscreen mode has been changed (e.g. the display resolution)
  */
  virtual void refreshFullscreen() = 0;

  //[-------------------------------------------------------]
  //[ Mouse                                                 ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Check if the mouse is currently over the window
  *
  *  @return
  *    'true' if mouse-over, else 'false'
  */
  virtual bool isMouseOver() const = 0;

  /**
  *  @brief
  *    Get current mouse cursor X position inside the window
  *
  *  @return
  *    Current mouse cursor X position inside the window, negative value if the mouse cursor isn't currently over the frontend
  */
  virtual int getMousePositionX() const = 0;

  /**
  *  @brief
  *    Get current mouse cursor Y position inside the window
  *
  *  @return
  *    Current mouse cursor Y position inside the window, negative value if the mouse cursor isn't currently over the frontend
  */
  virtual int getMousePositionY() const = 0;

  /**
  *  @brief
  *    Check if the mouse cursor is visible
  *
  *  @return
  *    'true' if the mouse cursor is visible, else 'false'
  *
  *  @note
  *    - If the mouse cursor is visible in general, it's still possible that it's
  *      invisible over some special widgets.
  *    - If the mouse cursor is invisible in general, it will NEVER be visible!
  */
  virtual bool isMouseVisible() const = 0;

  /**
  *  @brief
  *    Set mouse cursor visibility
  *
  *  @param[in] bVisible
  *    Shall the mouse cursor be visible?
  *
  *  @see
  *    - IsMouseVisible()
  */
  virtual void setMouseVisible(bool bVisible) = 0;

  /**
  *  @brief
  *    Trap mouse inside the frontend window
  *
  *  @param[in] bTrap
  *    'true' if the mouse should be trapped inside the frontend window, else 'false'
  */
  virtual void setTrapMouse(bool bTrap) = 0;

protected:

  FrontendOS* mpFrontendOS;

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore

