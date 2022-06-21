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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class NativeWindow;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * NativeWindowLinux
 *
 * @brief
 * Linux implementation of a native window implementation.
 */
class NativeWindowLinux : public NativeWindowImpl {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] nativeWindow
   * Pointer to the platform independent native window implementation.
   */
  NativeWindowLinux(NativeWindow* nativeWindow);

  /**
   * @brief
   * Destructor.
   */
  ~NativeWindowLinux() override;


  /**
   * @brief
   * REturns Window instance.
   *
   * @return
   * Window instance.
   */
  [[nodiscard]] ::Window getWindow() const;

  /**
   * @brief
   * Handles the provided event.
   *
   * @param[in] event
   * X event to handle.
   */
  void handleEvent(XEvent& event);

public:

  /**
   * @brief
   * Returns the window handle to this window.
   *
   * @return
   * The window handle to the window.
   */
  RECore::handle getWindowHandle() const override;

  /**
   * @brief
   * Responsible for redrawing the window.
   */
  void redraw() override;

  /// TODO(naetherm): Deprecated?
  void ping() override;

  /**
   * @brief
   * Sets the title of the window.
   *
   * @param[in] title
   * The window title.
   */
  void setTitle(const RECore::String& title) override;

  /**
   * @brief
   * Get the window and height of the window itself.
   *
   * @param[out] width
   * The width of the window.
   * @param[out] height
   * The height of the window.
   */
  void getWindowSize(RECore::int32& width, RECore::int32& height) override;

  /**
   * @brief
   * Resizes the window.
   *
   * @param[in] width
   * The new width of the window.
   * @param[in] height
   * The new height of the window.
   */
  void setWindowSize(RECore::int32 width, RECore::int32 height) override;

  /**
   * @brief
   * Returns whether the windows was destroyed.
   *
   * @return
   * True if window was destroyed, false otherwise.
   */
  [[nodiscard]] bool isDestroyed() const override;

protected:
  ::Display* mDisplay;
  ::Window mWindowHandle;
  ::Atom WM_DELETE_WINDOW;
  ::Atom UTF8_STRING;
  ::Atom WM_NAME;
  ::Atom _NET_WM_NAME;
  ::Atom _NET_WM_VISIBLE_NAME;
  bool mDestroyed;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
