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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class NativeWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * NativeWindowImpl
 *
 * @brief
 * Native window implementation.
 */
class NativeWindowImpl {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] nativeWindow
   * Pointer to native window.
   */
  REGUI_API NativeWindowImpl(NativeWindow* nativeWindow);

  /**
   * @brief
   * Destructor
   */
  REGUI_API virtual ~NativeWindowImpl();


  /**
   * @brief
   * Returns pointer to platform independent native window.
   *
   * @return
   * Platform independent native window.
   */
  REGUI_API NativeWindow* getNativeWindow() const;

public:

  /**
   * @brief
   * Create native window implementation.
   *
   * @param[in] nativeWindowHandle
   * The native window handle of the system.
   */
  virtual void createWindow(RECore::handle nativeWindowHandle) = 0;

  /**
   * @brief
   * Check if the window has been destroyed.
   *
   * @return
   * True if the windows is destroyed, false otherwise.
   */
  virtual bool isDestroyed() const = 0;

  /**
   * @brief
   * Destroy the window.
   */
  virtual void destroy() = 0;

  /**
   * @brief
   * Returns the native window handle.
   *
   * @return
   * Native window handle.
   */
  virtual RECore::handle getNativeWindowHandle() const = 0;

  /**
   * @brief
   * Sets the position of the window.
   *
   * @param[in] position
   * Position of the window.
   */
  virtual void setPosition(const RECore::Vec2i& position) = 0;

  /**
   * @brief
   * Returns the position of the window.
   *
   * @return
   * Position of the window.
   */
  virtual RECore::Vec2i getPosition() const = 0;

  /**
   * @brief
   * Sets the size of the window.
   *
   * @param[in] size
   * The size
   */
  virtual void setSize(const RECore::Vec2i& size) = 0;

  /**
   * @brief
   * Get the size of the window.
   *
   * @return
   * Size of window.
   */
  virtual RECore::Vec2i getSize() const = 0;

  /**
   * @brief
   * Redraw the window.
   */
  virtual void redraw() = 0;

protected:
  /** Pointer to platform independent native window object */
  NativeWindow* mNativeWindow;
private:
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
