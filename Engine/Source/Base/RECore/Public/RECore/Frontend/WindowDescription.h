////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// PermIssion Is hereby granted, free of charge, to any person obtaining a copy of thIs
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publIsh, dIstribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software Is furnIshed to do so, subject to the following conditions:
//
// The above copyright notice and thIs permIssion notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RECore/RECore.h"
#include "RECore/String/String.h"

namespace RECore {

/**
 * @class
 * WindowDescription
 *
 * @brief
 * WindowDescription Is a simple description of a platform dependent window representation.
 *
 * It contains all necessary values for representing the platform independent part of a
 * window.
 */
class RECORE_API WindowDescription {
public:

  /**
   * @brief
   * Default constructor.
   */
  WindowDescription();

  /**
   * @brief
   * Destructor.
   */
  ~WindowDescription();

  /**
   * @brief
   * Returns the x position of the window.
   *
   * @return
   * The x position of the window.
   */
  float getPositionOnScreenX() const;

  /**
   * @brief
   * Sets the x position on screen.
   *
   * @param [in]setPositionOnScreenX
   * X position on screen.
   */
  void setPositionOnScreenX(float PositionOnScreenX);

  /**
   * @brief
   * Returns the y position of the window.
   *
   * @return
   * The y position of the window.
   */
  float getPositionOnScreenY() const;

  /**
   * @brief
   * Sets the y position on screen.
   *
   * @param [in]PositionOnScreenY
   * Y position on screen.
   */
  void setPositionOnScreenY(float PositionOnScreenY);

  /**
   * @brief
   * Returns the width on the screen.
   *
   * @return
   * The width on the screen.
   */
  float getWidthOnScreen() const;

  /**
   * @brief
   * Sets the width on the screen.
   *
   * @param [in]WidthOnScreen
   * Width on screen.
   */
  void setWidthOnScreen(float WidthOnScreen);

  /**
   * @brief
   * Returns the height on the screen.
   *
   * @return
   * The height on the screen.
   */
  float getHeightOnScreen() const;

  /**
   * @brief
   * Sets the height on the screen.
   *
   * @param [in]HeightOnScreen
   * Height on screen.
   */
  void setHeightOnScreen(float HeightOnScreen);

  /**
   * @brief
   * Returns whether the window should show the OS window border.
   *
   * @return
   * True if the window should show the OS window border.
   */
  bool isHasOSWindowBorder() const;

  /**
   * @brief
   * Sets whether the window uses a OS window border
   *
   * @param [in]HasOSWindowBorder
   * Use OS window border?
   */
  void setHasOSWindowBorder(bool HasOSWindowBorder);

  /**
   * @brief
   * Returns whether the window should be shown in the taskbar.
   *
   * @return
   * True if the window should be shown in the taskbar.
   */
  bool isInTaskbar() const;

  /**
   * @brief
   * Sets whether the window is visible within the taskbar.
   *
   * @param [in]InTaskbar
   * Is in taskbar?
   */
  void setInTaskbar(bool InTaskbar);

  /**
   * @brief
   * Returns whether the window is a topmost window.
   *
   * @return
   * True if window is a topmost window.
   */
  bool isTopmostWindow() const;

  /**
   * @brief
   * Sets whether this window is the topmost window.
   *
   * @param [in]IsTopmostWindow
   * Is topmost window?
   */
  void setTopmostWindow(bool IsTopmostWindow);

  /**
   * @brief
   * Returns whether the window should accept input.
   *
   * @return
   * True if the window should accept input.
   */
  bool isAcceptsInput() const;

  /**
   * @brief
   * Sets whether the window accepts any input.
   *
   * @param [in]AcceptsInput
   * Accept input?
   */
  void setAcceptsInput(bool AcceptsInput);

  /**
   * @brief
   * Returns whether the button supports a close button.
   *
   * @return
   * True if the window supports a close button.
   */
  bool isHasCloseButton() const;

  /**
   * @brief
   * Sets whether the close button should be visible.
   *
   * @param [in]HasCloseButton
   * Has a close button?
   */
  void setHasCloseButton(bool HasCloseButton);

  /**
   * @brief
   * Returns whether the window should be able to get minimized.
   *
   * @return
   * True if the window should support minimization, otherwise false.
   */
  bool isSupportsMinimize() const;

  /**
   * @brief
   * Sets whether the window should be able to get minimized.
   *
   * @param [in]SupportsMinimize
   * Should be miimizable?
   */
  void setSupportsMinimize(bool SupportsMinimize);

  /**
   * @brief
   * Returns whether the window should be able to get maximized.
   *
   * @return
   * True if the window should support meximization, otherwise false.
   */
  bool isSupportsMaximize() const;

  /**
   * @brief
   * Sets whether the window should be able to get maximized.
   *
   * @param [in]SupportsMaximize
   * Should be maximizable?
   */
  void setSupportsMaximize(bool SupportsMaximize);

  /**
   * @brief
   * Returns the title of the window
   *
   * @return
   * The title
   */
  RECore::String getTitle() const;

  /**
   * @brief
   * Sets the title of the window.
   *
   * @param [in]sTitle
   * The title of the window.
   */
  void setTitle(const RECore::String &sTitle);

  /**
   * @brief
   * Returns the transparency of the window.
   *
   * @return
   * The transparency of the window.
   */
  float getTransparency() const;

  /**
   * @brief
   * Sets the transparency of the window.
   *
   * @param [in]fTransparency
   * The transparency of the window.
   */
  void setTransparency(float fTransparency);

private:

  RECore::String Title;

  float PositionOnScreenX;
  float PositionOnScreenY;
  float WidthOnScreen;
  float HeightOnScreen;

  float Transparency;

  bool HasOSWindowBorder;
  bool InTaskbar;
  bool IsTopmostWindow;
  bool AcceptsInput;
  bool HasCloseButton;
  bool SupportsMinimize;
  bool SupportsMaximize;

};

}
