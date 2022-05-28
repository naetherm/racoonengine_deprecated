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
#include "RECore/Frontend/FrontendOS.h"
#include "RECore/Frontend/Frontend.h"
#include "RECore/Frontend/WindowOS.h"
#if defined(WIN32)
	#include "RECore/Windows/WindowOSWindows.h"
#elif defined(APPLE)
	#include "RECore/MacOSX/WindowOSMacOSX.h"	// This is what we really want to use on Mac OS X to stay out of compatibility trouble
	// #include "RECore/Linux&OSWindowLinux.h"	// We can also reuse the X11 implementation from the Linux port
#elif defined(LINUX)
  #include "RECore/Linux/FrontendOSLinux.h"
	#include "RECore/Linux/LinuxWindowOS.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(FrontendOS, "RECore", RECore::FrontendImpl, "OS frontend implementation class")
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
re_class_metadata_end(FrontendOS)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendOS::FrontendOS()
: m_pActiveTopLevelWindow(nullptr)
, m_bQuit(false)
,	m_bToggleFullscreenMode(true)
,	m_bFullscreenAltTab(true)
,	m_bIsFullscreen(false){
}

/**
*  @brief
*    Destructor
*/
FrontendOS::~FrontendOS() {

}


//[-------------------------------------------------------]
//[ Private virtual RECore::FrontendImpl functions        ]
//[-------------------------------------------------------]
int FrontendOS::run(const String &sExecutableFilename, const std::vector<String> &lstArguments)
{
  // Create system implementation for the right platform
#if defined(WIN32)

#elif defined(LINUX)
  this->m_pActiveTopLevelWindow = new LinuxWindowOS(*this);
#endif

  // The frontend message loop
  if (m_pActiveTopLevelWindow->getNativeWindowHandle()) {
    m_bQuit = false;

    while (!m_bQuit && m_pActiveTopLevelWindow && m_pActiveTopLevelWindow->getNativeWindowHandle() && m_pFrontend && m_pFrontend->isRunning()) {
      this->redraw();

      this->ping();
    }
  }

  // Destroy the OS specific window implementation
  if (this->m_pActiveTopLevelWindow) {
    delete this->m_pActiveTopLevelWindow;
    this->m_pActiveTopLevelWindow = nullptr;
  }

	// Done
	return 0;
}

handle FrontendOS::getNativeWindowHandle() const
{
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getNativeWindowHandle() : NULL_HANDLE;
}

void FrontendOS::redraw()
{
	// Ask the OS politely to update (and repaint) the widget
  if (m_pActiveTopLevelWindow)
	  this->m_pActiveTopLevelWindow->redraw();
}

void FrontendOS::ping()
{
	// Ping the OS window
	//this->onUpdate();

  if (m_pActiveTopLevelWindow)
    m_bQuit = this->m_pActiveTopLevelWindow->ping();
}

String FrontendOS::getTitle() const
{
	// Query the OS window implementation
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getTitle() : "";
}

void FrontendOS::setTitle(const String &sTitle)
{
	// Query the OS window implementation
	if (m_pActiveTopLevelWindow)
    m_pActiveTopLevelWindow->setTitle(sTitle);
}

int FrontendOS::getX() const
{
	// Query the OS window implementation
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getX() : 0;
}

int FrontendOS::getY() const
{
	// Query the OS window implementation
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getY() : 0;
}

uint32 FrontendOS::getWidth() const
{
	// Query the OS window implementation
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getWidth() : 0;
}

uint32 FrontendOS::getHeight() const
{
	// Query the OS window implementation
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getHeight() : 0;
}

void FrontendOS::getWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const
{
	// Nothing special in here
	nX      = getX();
	nY      = getY();
	nWidth  = getWidth();
	nHeight = getHeight();
}

void FrontendOS::setWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
	// Query the OS window implementation
	if (m_pActiveTopLevelWindow)
    m_pActiveTopLevelWindow->setWindowPositionSize(nX, nY, nWidth, nHeight);
}

bool FrontendOS::getToggleFullscreenMode() const
{
	return m_bToggleFullscreenMode;
}

void FrontendOS::setToggleFullscreenMode(bool bToggleFullscreenMode)
{
	m_bToggleFullscreenMode = bToggleFullscreenMode;
}

bool FrontendOS::getFullscreenAltTab() const
{
	return m_bFullscreenAltTab;
}

void FrontendOS::setFullscreenAltTab(bool bAllowed)
{
	// Is there a state change?
	if (m_bFullscreenAltTab != bAllowed) {
		// Set the new state
		m_bFullscreenAltTab = bAllowed;

		// Let the OS window do the rest
		if (m_pActiveTopLevelWindow)
      m_pActiveTopLevelWindow->setFullscreenAltTab(bAllowed);
	}
}

bool FrontendOS::isFullscreen() const
{
	return m_bIsFullscreen;
}

void FrontendOS::setFullscreen(bool bFullscreen)
{
	// Is there a state change?
	if (m_bIsFullscreen != bFullscreen) {
		// Set the new state
		m_bIsFullscreen = bFullscreen;

		// Let the OS window do the rest
		if (m_pActiveTopLevelWindow)
      m_pActiveTopLevelWindow->setFullscreen(bFullscreen);
	}
}

void FrontendOS::refreshFullscreen()
{
	// Let the OS window do the rest
	if (m_pActiveTopLevelWindow)
    m_pActiveTopLevelWindow->refreshFullscreen();
}

bool FrontendOS::isMouseOver() const
{
	// Let the OS window do the rest
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->isMouseOver() : false;
}

int FrontendOS::getMousePositionX() const
{
	// Let the OS window do the rest
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getMousePositionX() : -1;
}

int FrontendOS::getMousePositionY() const
{
	// Let the OS window do the rest
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->getMousePositionY() : -1;
}

bool FrontendOS::isMouseVisible() const
{
	// Let the OS window do the rest
	return m_pActiveTopLevelWindow ? m_pActiveTopLevelWindow->isMouseVisible() : true;
}

void FrontendOS::setMouseVisible(bool bVisible)
{
	// Let the OS window do the rest
	if (m_pActiveTopLevelWindow)
    m_pActiveTopLevelWindow->setMouseVisible(bVisible);
}

void FrontendOS::setTrapMouse(bool bTrap)
{
	// Let the OS window do the rest
	if (m_pActiveTopLevelWindow)
    m_pActiveTopLevelWindow->setTrapMouse(bTrap);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
