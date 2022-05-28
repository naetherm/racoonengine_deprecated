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
#include "RECore/Frontend/FrontendImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class WindowOS;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OS frontend implementation class
*/
class FrontendOS : public RECore::FrontendImpl {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  // New
  friend class WindowOS;
  friend class LinuxWindowOS;
  friend class WindowOSWindows;


  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
re_class_def()
re_class_def_end


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  FrontendOS();

  /**
  *  @brief
  *    Destructor
  */
  virtual ~FrontendOS();

  inline WindowOS* getWindow() const {
    return this->m_pActiveTopLevelWindow;
  }

private:

  //[-------------------------------------------------------]
  //[ Private virtual RECore::FrontendImpl functions        ]
  //[-------------------------------------------------------]
private:
  virtual int run(const RECore::String &sExecutableFilename, const std::vector<RECore::String> &lstArguments) override;

  virtual RECore::handle getNativeWindowHandle() const override;

  virtual void redraw() override;

  virtual void ping() override;

  virtual RECore::String getTitle() const override;

  virtual void setTitle(const RECore::String &sTitle) override;

  virtual int getX() const override;

  virtual int getY() const override;

  virtual RECore::uint32 getWidth() const override;

  virtual RECore::uint32 getHeight() const override;

  virtual void getWindowPositionSize(int &nX, int &nY, RECore::uint32 &nWidth, RECore::uint32 &nHeight) const override;

  virtual void setWindowPositionSize(int nX, int nY, RECore::uint32 nWidth, RECore::uint32 nHeight) override;

  virtual bool getToggleFullscreenMode() const override;

  virtual void setToggleFullscreenMode(bool bToggleFullscreenMode) override;

  virtual bool getFullscreenAltTab() const override;

  virtual void setFullscreenAltTab(bool bAllowed) override;

  virtual bool isFullscreen() const override;

  virtual void setFullscreen(bool bFullscreen) override;

  virtual void refreshFullscreen() override;

  virtual bool isMouseOver() const override;

  virtual int getMousePositionX() const override;

  virtual int getMousePositionY() const override;

  virtual bool isMouseVisible() const override;

  virtual void setMouseVisible(bool bVisible) override;

  virtual void setTrapMouse(bool bTrap) override;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  WindowOS* m_pActiveTopLevelWindow; /**< The OS specific window implementation, can be a null pointer */
  bool m_bQuit;          /**< Quit frontend? */
  bool m_bToggleFullscreenMode;  /**< Is it allowed to toggle the fullscreen mode using hotkeys? */
  bool m_bFullscreenAltTab;    /**< Is it allowed to use Alt-Tab within fullscreen mode? */
  bool m_bIsFullscreen;      /**< 'true' if the window is in fullscreen mode, else 'false' */

  bool m_bEarlyStartup;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
