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
class ThemeManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Gui
 *
 * @brief
 * Platform independent gui implementation.
 */
class Gui {
public:

  /**
   * @brief
   * Returns reference to singleton gui instance.
   *
   * @return
   * Reference to singleton gui instance.
   */
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


  /**
   * @brief
   * Initializes the gui instance.
   *
   * @param[in] coreContext
   * Pointer to core context instance.
   * @param[in] guiContext
   * Pointer to gui context instance.
   */
  void initialize(RECore::CoreContext* coreContext, GuiContext* guiContext);


  /**
   * @brief
   * Returns pointer to platform dependent gui implementation.
   *
   * @return
   * Pointer to platform dependent gui implementation;
   */
  [[nodiscard]] GuiImpl* getImpl() const;

  /**
   * @brief
   * Returns reference to core context implementation.
   *
   * @return
   * Reference to core context implementation.
   */
  [[nodiscard]] RECore::CoreContext& getCoreContext() const;

  /**
   * @brief
   * Returns reference to gui context implementation.
   *
   * @return
   * Reference to gui context implementation.
   */
  [[nodiscard]] GuiContext& getGuiContext() const;

  /**
   * @brief
   * Returns whether the gui system is active.
   *
   * @return
   * True if gui system is active, false otherwise.
   */
  [[nodiscard]] bool isActive() const;

  /**
   * @brief
   * Returns whether the gui system has any window to work on.
   *
   * @return
   * True if there is at least one window, false otherwise.
   */
  [[nodiscard]] bool hasWindows() const;

  /**
   * @brief
   * Sets the main window of the gui system.
   *
   * @param[in] mainWindow
   * Pointer to main window.
   *
   * @remarks
   * Currently we are only supporting a single window mode for the gui system but in the
   * future we want to have multi-window support.
   */
  void setMainWindow(MainWindow* mainWindow);

  /**
   * @brief
   * Returns pointer to the main window.
   *
   * @return
   * Pointer to main window.
   */
  [[nodiscard]] MainWindow* getMainWindow() const;


  /**
   * @brief
   * Updates the gui system.
   */
  void update();


  //[-------------------------------------------------------]
  //[ Gui Messages                                          ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Process all outstanding messages.
   */
  void processMessages();

  /**
   * @brief
   * Sets the given message to the correct system or window.
   *
   * @param[in] guiMessage
   * The gui message to process.
   */
  void sendMessage(const GuiMessage& guiMessage);

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
  /** The main swap chain */
  RERHI::RHISwapChain* mMainSwapChain;
  /** The command buffer, used for drawing */
  RERHI::RHICommandBuffer mCommandBuffer;
  /** Pointer to the theme manager */
  ThemeManager* mThemeManager;
  /** Pointer to the currently active theme */
  Theme* mTheme;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
