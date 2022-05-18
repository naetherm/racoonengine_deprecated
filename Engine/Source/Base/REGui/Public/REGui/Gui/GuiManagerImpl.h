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
#include "REGui/REGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * GuiManagerImpl
 *
 * @brief
 * Interface implementation for a platform specific gui manager implementation.
 */
class GuiManagerImpl {
public:

  GuiManagerImpl(GuiManager* guiManager);

  virtual ~GuiManagerImpl();

public:

  virtual void onWindowResize(RECore::uint32 width, RECore::uint32 height) = 0;

  virtual void onKeyInput(RECore::uint32 keySym, char character, bool pressed) = 0;

  virtual void onMouseMoveInput(int x, int y) = 0;

  virtual void onMouseButtonInput(RECore::uint32 button, bool pressed) = 0;

  virtual void onMouseWheelInput(bool scrollUp) = 0;

  virtual void initializeImGuiKeyMap() = 0;

  virtual void onNewFrame(RERHI::RHIRenderTarget& renderTarget) = 0;

protected:
  /** Pointer to the platform independent gui manager implementation */
  GuiManager* mGuiManager;

  RECore::uint32 mWindowWidth;
  RECore::uint32 mWindowHeight;
  RECore::uint64 mTime;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
