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
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include "RECore/Null/FrontendNull.h"
#include "RECore/Time/TimeManager.h"
#include "RECore/Platform/Platform.h"
#include "RECore/Frontend/Frontend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(FrontendNull, "RECore", RECore::FrontendImpl, "Null frontend implementation class")
  // Constructors
  re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
re_class_metadata_end(FrontendNull)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendNull::FrontendNull() :
  m_nTimeToWait(0)
{
}

/**
*  @brief
*    Destructor
*/
FrontendNull::~FrontendNull()
{
}


//[-------------------------------------------------------]
//[ Private virtual RECore::FrontendImpl functions        ]
//[-------------------------------------------------------]
int FrontendNull::run(const String &sExecutableFilename, const std::vector<String> &lstArguments)
{
  // Do the frontend life cycle thing - initialize
  if (onStart()) {
    onResume();

    // The frontend main loop
    while (m_pFrontend && m_pFrontend->isRunning()) {
      // Time to wait?
      if (m_nTimeToWait) {
        // Let the system some time to process other system tasks etc.
        // If this isn't done the CPU usage is always up to 100%!!
        // Please note that there's no guaranty that the resulting FPS always reaches
        // exactly the maximum FPS limit.
        Platform::instance().sleep(m_nTimeToWait);
      }

      // Redraw & ping
      redraw();
      ping();
    }

    // Do the frontend life cycle thing - de-initialize
    onPause();
    onStop();

    // Done
    return 0;
  } else {
    // Error!
    return -1;
  }
}

handle FrontendNull::getNativeWindowHandle() const
{
  // There's no window handle!
  return NULL_HANDLE;
}

void FrontendNull::redraw()
{
  // Let the frontend draw into it's window
  onDraw();
}

void FrontendNull::ping()
{
  // MS Windows: Get and dispatch messages, an application using this frontend may need it for e.g. proper input processing
#ifdef WIN32
  MSG sMsg;
		while (PeekMessage(&sMsg, nullptr, 0, 0, FALSE)) {
			GetMessage(&sMsg, nullptr, 0, 0);
			TranslateMessage(&sMsg);
			DispatchMessage(&sMsg);
		}
#endif

  // Check if we're allowed to perform an update right now
  ///if (Timing::getInstance()->Update(&m_nTimeToWait))
  {
    // Let the frontend update it's states
    onUpdate();
  }
}

String FrontendNull::getTitle() const
{
  // There's no window
  return "";
}

void FrontendNull::setTitle(const String &sTitle)
{
  // There's no window
}

int FrontendNull::getX() const
{
  // There's no window
  return 0;
}

int FrontendNull::getY() const
{
  // There's no window
  return 0;
}

uint32 FrontendNull::getWidth() const
{
  // There's no window
  return 0;
}

uint32 FrontendNull::getHeight() const
{
  // There's still no window
  return 0;
}

void FrontendNull::getWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const
{
  // There's no window

  // Set known default values
  nX = nY = nWidth = nHeight = 0;
}

void FrontendNull::setWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight)
{
  // There's no window
}

bool FrontendNull::getToggleFullscreenMode() const
{
  // Exactly, no window
  return false;
}

void FrontendNull::setToggleFullscreenMode(bool bToggleFullscreenMode)
{
  // No window, again
}

bool FrontendNull::getFullscreenAltTab() const
{
  // Return whether or not it's allowed to use Alt-Tab - if there would be a window, but there's no
  return false;
}

void FrontendNull::setFullscreenAltTab(bool bAllowed)
{
  // Sorry, this frontend really has no window...
}

bool FrontendNull::isFullscreen() const
{
  // ... no window at all...
  return false;
}

void FrontendNull::setFullscreen(bool bFullscreen)
{
  // No window = nothing to do in here
}

void FrontendNull::refreshFullscreen()
{
  // No window = nothing to do in here
}

bool FrontendNull::isMouseOver() const
{
  // We're not interested in GUI stuff
  return false;
}

int FrontendNull::getMousePositionX() const
{
  // We're not interested in GUI stuff
  return -1;
}

int FrontendNull::getMousePositionY() const
{
  // We're not interested in GUI stuff
  return -1;
}

bool FrontendNull::isMouseVisible() const
{
  // We're not interested in GUI stuff
  return false;
}

void FrontendNull::setMouseVisible(bool bVisible)
{
  // We're not interested in GUI stuff
}

void FrontendNull::setTrapMouse(bool bTrap)
{
  // We're not interested in GUI stuff
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore