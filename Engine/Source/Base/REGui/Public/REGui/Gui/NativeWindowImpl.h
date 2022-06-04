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
#include "RECore/Math/Vec2i.h"
#include <RECore/String/String.h>


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
 * Abstract platform dependent native window implementation.
 * For each supported platform we must create an implementation of this class.
 */
class NativeWindowImpl {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] nativeWindow
   * Pointer to the platform independent native window implementation.
   */
  NativeWindowImpl(NativeWindow* nativeWindow);

  /**
   * @brief
   * Destructor.
   */
  virtual ~NativeWindowImpl();

public:

  /**
   * @brief
   * Returns the window handle to this window.
   *
   * @return
   * The window handle to the window.
   */
  virtual RECore::handle getWindowHandle() const = 0;

  /**
   * @brief
   * Responsible for redrawing the window.
   */
  virtual void redraw() = 0;

  /// TODO(naetherm): Deprecated?
  virtual void ping() = 0;

  /**
   * @brief
   * Sets the title of the window.
   *
   * @param[in] title
   * The window title.
   */
  virtual void setTitle(const RECore::String& title) = 0;

  /**
   * @brief
   * Get the window and height of the window itself.
   *
   * @param[out] width
   * The width of the window.
   * @param[out] height
   * The height of the window.
   */
  virtual void getWindowSize(RECore::int32& width, RECore::int32& height) = 0;

  /**
   * @brief
   * Resizes the window.
   *
   * @param[in] width
   * The new width of the window.
   * @param[in] height
   * The new height of the window.
   */
  virtual void setWindowSize(RECore::int32 width, RECore::int32 height) = 0;

  /**
   * @brief
   * Returns whether the windows was destroyed.
   *
   * @return
   * True if window was destroyed, false otherwise.
   */
  [[nodiscard]] virtual bool isDestroyed() const = 0;

protected:
  /** Pointer to the platform independent native window implementation */
  NativeWindow* mNativeWindow;
  /** The position of the window */
  RECore::Vec2i mPosition;
  /** The size of the window */
  RECore::Vec2i mSize;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
