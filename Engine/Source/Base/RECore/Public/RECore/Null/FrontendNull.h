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
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Frontend/FrontendImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null frontend implementation class
*/
class RECORE_API FrontendNull : public FrontendImpl {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_def(RECORE_API)

re_class_def_end


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
public:
/**
*  @brief
*    Constructor
*/
  FrontendNull();

/**
*  @brief
*    Destructor
*/
  virtual ~FrontendNull();


//[-------------------------------------------------------]
//[ Private virtual FrontendImpl functions        ]
//[-------------------------------------------------------]
private:
  virtual int run(const String &sExecutableFilename, const std::vector<String> &lstArguments) override;

  virtual handle getNativeWindowHandle() const override;

  virtual void redraw() override;

  virtual void ping() override;

  virtual String getTitle() const override;

  virtual void setTitle(const String &sTitle) override;

  virtual int getX() const override;

  virtual int getY() const override;

  virtual uint32 getWidth() const override;

  virtual uint32 getHeight() const override;

  virtual void getWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const override;

  virtual void setWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight) override;

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
  uint64 m_nTimeToWait;  /**< Time to wait */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore