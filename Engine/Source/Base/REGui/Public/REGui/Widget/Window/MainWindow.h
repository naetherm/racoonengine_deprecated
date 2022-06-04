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
#include <RECore/String/String.h>
#include <imgui.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;
class GuiMessage;
class MenuBar;
class NativeWindow;
class Widget;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class MainWindow {
public:

  MainWindow(Gui* gui);

  virtual ~MainWindow();


  void setSwapChain(RERHI::RHISwapChain* swapChain);

  void setTitle(const RECore::String& title);

  [[nodiscard]] NativeWindow* getNativeWindow() const;

  [[nodiscard]] RECore::handle getWindowHandle() const;

  [[nodiscard]] RERHI::RHISwapChain* getSwapChain() const;

  void redraw();

  [[nodiscard]] bool isDestroyed() const;


//[-------------------------------------------------------]
//[ Main Menu Bar                                         ]
//[-------------------------------------------------------]

  [[nodiscard]] bool showMainMenuBar() const;

  void makeMainMenuBarVisible(bool visible);

  void setMainMenuBar(MenuBar* menuBar);

public:

  void onMessage(const GuiMessage& guiMessage);

public:

protected:

  virtual void onDraw();

  void onMouseMove(const RECore::Vec2i& position);

  void onResize();

  void onMove(const RECore::Vec2i& position);

  void onSize(const RECore::Vec2i& size);

protected:

  void onDrawMainMenuBar();

protected:
  /** Pointer to gui implementation */
  Gui* mGui;
  /** Pointer to native window implementation */
  NativeWindow* mNativeWindow;
  /** Pointer to swap chain */
  RERHI::RHISwapChain* mSwapChain;

  /** List of all widgets of this main window */
  std::vector<Widget*> mChildren;
  /** Pointer to main menu bar, this can be a nullptr */
  MenuBar* mMainMenuBar;
  /** Determines whether to show the main menu bar */
  bool mShowMainMenuBar;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
