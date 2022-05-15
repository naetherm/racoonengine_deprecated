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
#include <RECore/String/String.h>
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiImpl;
class Screen;
class GuiContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Gui
 *
 * @brief
 * Platform independent Gui implementation.
 */
class Gui {

  friend class GuiApplication;

public:

  static Gui& instance();

public:

  /**
   * @brief
   * Constructor.
   */
  REGUI_API Gui();

  /**
   * @brief
   * Destructor.
   */
  REGUI_API ~Gui();


  /**
   * @brief
   * Get platform dependent gui implementation.
   *
   * @return
   * Platform dependent gui implementation.
   */
  [[nodiscard]] GuiImpl* getImpl() const;

  [[nodiscard]] bool isActive() const;


  //[-------------------------------------------------------]
  //[ Message processing                                    ]
  //[-------------------------------------------------------]
  void processMessages();


  //[-------------------------------------------------------]
  //[ Screens                                               ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Gets vector of all screens/monitors.
   *
   * @return
   * Vector of screens.
   */
  REGUI_API const std::vector<Screen*>& getScreens() const;

  /**
   * @brief
   * Gets pointer to screen by its name.
   *
   * @param[in] name
   * Name of the screen.
   *
   * @return
   * Pointer to screen, this can be a nullptr.
   */
  REGUI_API Screen* getScreen(const RECore::String& name) const;

  /**
   * @brief
   * Gets pointer to the default screen.
   *
   * @return
   * Pointer to the default screen.
   */
  REGUI_API Screen* getDefaultScreen() const;

protected:

  /**
   * @brief
   * Responsible for initializing the gui.
   *
   * @param[in] guiContext
   * Pointer to the gui context.
   */
  void initialize(GuiContext* guiContext);

protected:
  /** Platform dependent gui implementation */
  GuiImpl* mpImpl;
  /** Pointer to the gui context, always valid! */
  GuiContext* mGuiContext;
  /** List of available screens/monitors */
  std::vector<Screen*> mScreens;
  /** Pointer to the default screen, always valid! */
  Screen* mDefaultScreen;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
