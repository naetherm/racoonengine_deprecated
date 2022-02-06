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
#include "RECore/RECore.h"
#include "RECore/Frontend/FrontendImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get frontend context
*/
inline const FrontendContext &Frontend::getFrontendContext() const
{
	return m_cFrontendContext;
}

/**
*  @brief
*    Get frontend implementation
*/
inline FrontendImpl *Frontend::getImpl() const
{
	// Return implementation
	return m_pFrontendImpl;
}

/**
*  @brief
*    Get native window handle
*/
inline handle Frontend::getNativeWindowHandle() const
{
	// Call backend
	return m_pFrontendImpl->getNativeWindowHandle();
}

/**
*  @brief
*    Redraw frontend
*/
inline void Frontend::redraw()
{
	// Call backend
	m_pFrontendImpl->redraw();
}

/**
*  @brief
*    Give the frontend a chance to process OS messages
*/
inline void Frontend::ping() const
{
	// Call backend
	m_pFrontendImpl->ping();
}

/**
*  @brief
*    Redraw frontend and give the frontend a chance to process OS messages
*/
inline void Frontend::redrawAndPing()
{
	// Call backend methods
	m_pFrontendImpl->redraw();
	m_pFrontendImpl->ping();
}

/**
*  @brief
*    Get frontend title
*/
inline String Frontend::getTitle() const
{
	// Call backend
	return m_pFrontendImpl->getTitle();
}

/**
*  @brief
*    Set frontend title
*/
inline void Frontend::setTitle(const String &sTitle)
{
	// Call backend
	return m_pFrontendImpl->setTitle(sTitle);
}

/**
*  @brief
*    Get the x position of the frontend (in screen coordinates)
*/
inline int Frontend::getX() const
{
	// Call backend
	return m_pFrontendImpl->getX();
}

/**
*  @brief
*    Get the y position of the frontend (in screen coordinates)
*/
inline int Frontend::getY() const
{
	// Call backend
	return m_pFrontendImpl->getY();
}

/**
*  @brief
*    Get frontend width
*/
inline uint32 Frontend::getWidth() const
{
	// Call backend
	return m_pFrontendImpl->getWidth();
}

/**
*  @brief
*    Get frontend height
*/
inline uint32 Frontend::getHeight() const
{
	// Call backend
	return m_pFrontendImpl->getHeight();
}

/**
*  @brief
*    Returns frontend window position and size
*/
inline void Frontend::getWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const
{
	// Call backend
	m_pFrontendImpl->getWindowPositionSize(nX, nY, nWidth, nHeight);
}

/**
*  @brief
*    Set frontend window position and size
*/
inline void Frontend::setWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
	// Call backend
	m_pFrontendImpl->setWindowPositionSize(nX, nY, nWidth, nHeight);
}

/**
*  @brief
*    Gets whether it's allowed to toggle the fullscreen mode using hotkeys
*/
inline bool Frontend::getToggleFullscreenMode() const
{
	// Call backend
	return m_pFrontendImpl->getToggleFullscreenMode();
}

/**
*  @brief
*    Sets whether it's allowed to toggle the fullscreen mode using hotkeys
*/
inline void Frontend::setToggleFullscreenMode(bool bToggleFullscreenMode)
{
	// Call backend
	m_pFrontendImpl->setToggleFullscreenMode(bToggleFullscreenMode);
}

/**
*  @brief
*    Gets whether it's allowed to use Alt-Tab if fullscreen mode is used
*/
inline bool Frontend::getFullscreenAltTab() const
{
	// Call backend
	return m_pFrontendImpl->getFullscreenAltTab();
}

/**
*  @brief
*    Sets whether it's allowed to use Alt-Tab if fullscreen mode is used
*/
inline void Frontend::setFullscreenAltTab(bool bAllowed)
{
	// Call backend
	m_pFrontendImpl->setFullscreenAltTab(bAllowed);
}

/**
*  @brief
*    Returns whether the frontend is in fullscreen mode or not
*/
inline bool Frontend::isFullscreen() const
{
	// Call backend
	return m_pFrontendImpl->isFullscreen();
}

/**
*  @brief
*    Sets the frontend's fullscreen mode
*/
inline void Frontend::setFullscreen(bool bFullscreen)
{
	// Call backend
	m_pFrontendImpl->setFullscreen(bFullscreen);
}

/**
*  @brief
*    Something related to fullscreen mode has been changed (e.g. the display resolution)
*/
inline void Frontend::refreshFullscreen()
{
	// Call backend
	m_pFrontendImpl->refreshFullscreen();
}

/**
*  @brief
*    Check if the mouse is currently over the frontend
*/
inline bool Frontend::isMouseOver() const
{
	// Call backend
	return m_pFrontendImpl->isMouseOver();
}

/**
*  @brief
*    Get current mouse cursor X position inside the frontend
*/
inline int Frontend::getMousePositionX() const
{
	// Call backend
	return m_pFrontendImpl->getMousePositionX();
}

/**
*  @brief
*    Get current mouse cursor Y position inside the frontend
*/
inline int Frontend::getMousePositionY() const
{
	// Call backend
	return m_pFrontendImpl->getMousePositionY();
}

/**
*  @brief
*    Check if the mouse cursor is visible
*/
inline bool Frontend::isMouseVisible() const
{
	// Call backend
	return m_pFrontendImpl->isMouseVisible();
}

/**
*  @brief
*    Set mouse cursor visibility
*/
inline void Frontend::setMouseVisible(bool bVisible)
{
	// Call backend
	m_pFrontendImpl->setMouseVisible(bVisible);
}

/**
*  @brief
*    Trap mouse inside the frontend
*/
inline void Frontend::setTrapMouse(bool bTrap)
{
	// Call backend
	m_pFrontendImpl->setTrapMouse(bTrap);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
