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
#include <RECore/Math/Vec2i.h>
#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;
class GuiMessage;
class NativeWindowImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * NativeWindow
 *
 * @brief
 * Platform independent native window implementation.
 */
class NativeWindow {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] gui
   * Pointer to platform independent gui implementation.
   */
  NativeWindow(Gui* gui);

  /**
   * @brief
   * Destructor.
   */
  ~NativeWindow();


  /**
   * @brief
   * Gets pointer to gui implementations.
   *
   * @return
   * Pointer to gui implementation.
   */
  [[nodiscard]] Gui* getGui() const;

  [[nodiscard]] RECore::handle getWindowHandle() const;

  void onMessage(const GuiMessage& guiMessage);

  /**
   * @brief
   * Gets pointer to the internally used swapchain implementation.
   *
   * @return
   * Pointer to swapchain implementation.
   */
  [[nodiscard]] RERHI::RHISwapChainPtr getSwapChain() const;

  void setSwapChain(RERHI::RHISwapChainPtr swapChainPtr);

  /**
   * @brief
   * Sets the position of the window.
   *
   * @param[in] position
   * Position of the window.
   */
  void setPosition(const RECore::Vec2i& position);

  /**
   * @brief
   * Returns the position of the window.
   *
   * @return
   * Position of the window.
   */
  RECore::Vec2i getPosition() const;

  /**
   * @brief
   * Sets the size of the window.
   *
   * @param[in] size
   * The size
   */
  void setSize(const RECore::Vec2i& size);

  /**
   * @brief
   * Get the size of the window.
   *
   * @return
   * Size of window.
   */
  RECore::Vec2i getSize() const;

private:

  void initializeSwapChain();

protected:
  /** Pointer to gui implementation, always valid! */
  Gui* mGui;
  /** Pointer to platform dependent native window implementation */
  NativeWindowImpl* mImpl;
  /** Pointer to swapchain implementation, always valid! */
  RERHI::RHISwapChainPtr mSwapChain;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
