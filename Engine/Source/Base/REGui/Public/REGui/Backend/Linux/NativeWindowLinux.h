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
#include "REGui/Gui/NativeWindowImpl.h"
#include <RECore/Linux/X11Includes.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * NativeWindowLinux
 *
 * @brief
 * Linux native window implementation.
 */
class NativeWindowLinux : public NativeWindowImpl {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] nativeWindow
   * Pointer to native window.
   */
  REGUI_API NativeWindowLinux(NativeWindow* nativeWindow);

  /**
   * @brief
   * Destructor
   */
  REGUI_API ~NativeWindowLinux() override;

public:


  /**
   * @brief
   * Create native window implementation.
   *
   * @param[in] nativeWindowHandle
   * The native window handle of the system.
   */
  void createWindow(RECore::handle nativeWindowHandle) override;

  void createWindow() override;

  /**
   * @brief
   * Check if the window has been destroyed.
   *
   * @return
   * True if the windows is destroyed, false otherwise.
   */
  bool isDestroyed() const override;

  /**
   * @brief
   * Destroy the window.
   */
  void destroy() override;

  /**
   * @brief
   * Returns the native window handle.
   *
   * @return
   * Native window handle.
   */
  RECore::handle getNativeWindowHandle() const override;

  /**
   * @brief
   * Sets the position of the window.
   *
   * @param[in] position
   * Position of the window.
   */
  void setPosition(const RECore::Vec2i& position) override;

  /**
   * @brief
   * Returns the position of the window.
   *
   * @return
   * Position of the window.
   */
  RECore::Vec2i getPosition() const override;

  /**
   * @brief
   * Sets the size of the window.
   *
   * @param[in] size
   * The size
   */
  void setSize(const RECore::Vec2i& size) override;

  /**
   * @brief
   * Get the size of the window.
   *
   * @return
   * Size of window.
   */
  RECore::Vec2i getSize() const override;

  /**
   * @brief
   * Redraw the window.
   */
  void redraw() override;

private:

  ::Display* mDisplay;
  ::Window mNativeWindowHandle;
  int mScreen;
  RECore::Vec2i mPosition;
  RECore::Vec2i mSize;


  // Atoms
  ::Atom					 WM_DELETE_WINDOW;		/**< System atom for delete */
  ::Atom					 UTF8_STRING;			/**< Atom for the type of a window title */
  ::Atom 					 WM_NAME;				/**< Window title (old?) */
  ::Atom 					 _NET_WM_NAME;			/**< Window title */
  ::Atom 					 _NET_WM_VISIBLE_NAME;	/**< Window title (visible title, can be different) */

  bool mDestroyed;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
