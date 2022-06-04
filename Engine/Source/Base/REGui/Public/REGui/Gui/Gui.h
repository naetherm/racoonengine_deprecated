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
#include <RECore/Application/CoreContext.h>
#include <imgui.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiImpl;
class GuiMessage;
class MainWindow;
class GuiRenderer;
class GuiContext;
// Theming
class Theme;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class Gui {
public:

  static Gui& instance();

public:

  /**
   * @brief
   * Constructor.
   */
  Gui();
  /**
   * @brief
   * Destructor.
   */
  virtual ~Gui();


  void initialize(RECore::CoreContext* coreContext, GuiContext* guiContext);


  /**
   * @brief
   * Returns pointer to platform dependent gui implementation.
   *
   * @return
   * Pointer to platform dependent gui implementation;
   */
  [[nodiscard]] GuiImpl* getImpl() const;

  [[nodiscard]] RECore::CoreContext& getCoreContext() const;

  [[nodiscard]] GuiContext& getGuiContext() const;

  [[nodiscard]] bool isActive() const;

  [[nodiscard]] bool hasWindows() const;

  void setMainWindow(MainWindow* mainWindow);

  [[nodiscard]] MainWindow* getMainWindow() const;


  void update();

  void dummy();


  //[-------------------------------------------------------]
  //[ Gui Messages                                          ]
  //[-------------------------------------------------------]
  void processMessages();

  void sendMessage(const GuiMessage& guiMessage);

private:

protected:
  /** Pointer to platform dependent gui implementation */
  GuiImpl* mGuiImpl;
  /** Pointer to core context */
  RECore::CoreContext* mCoreContext;
  /** Pointer to imgui context instance, must be valid! */
  ImGuiContext*	   mImGuiContext;
  /** Pointer to gui context */
  GuiContext* mGuiContext;
  /** Pointer to gui renderer implementation */
  GuiRenderer* mGuiRenderer;
  /** Pointer to main window */
  MainWindow* mMainWindow;

  RERHI::RHISwapChain* mMainSwapChain;

  RERHI::RHICommandBuffer mCommandBuffer;

  // TODO(naetherm):
  Theme* mTheme;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
