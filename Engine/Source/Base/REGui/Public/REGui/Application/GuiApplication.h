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
#include <RECore/Application/CoreApplication.h>
#include "REGui/Application/GuiContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class MainWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * GuiApplication
 *
 * @brief
 * Basic gui application, the starting point of all applications that will show a gui for interaction.
 */
class GuiApplication : public RECore::CoreApplication {

  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
  re_class_def(REGUI_API)
  re_class_def_end

public:

  /**
   * @brief
   * Constructor.
   */
  REGUI_API GuiApplication();

  /**
   * @brief
   * Destructor.
   */
  ~GuiApplication() override;

  //[-------------------------------------------------------]
  //[ Protected virtual RECore::AbstractLifecycle functions ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Initialization function that is called prior to onInit()
  *
  *  @return
  *    'true' if all went fine, else 'false' which will stop the application
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Everything that CoreApplication::onStart() does
  *    - Call OnCreateMainWindow()
  *    - Return and go on with onInit()
  */
  [[nodiscard]] REGUI_API bool onStart() override;

  /**
  *  @brief
  *    De-initialization function that is called after onDeInit()
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Everything that CoreApplication::onStop() does
  *    - De-initialize system GUI
  */
  REGUI_API void onStop() override;


  //[-------------------------------------------------------]
  //[ Protected virtual RECore::CoreApplication functions   ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Main function
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Run GUI main loop (processing GUI messages)
  *    - Exit loop when either the GUI or the application has been stopped
  */
  REGUI_API void main() override;

protected:

  /**
   * @brief
   * Helper method for creating the main window.
   *
   * @return
   * Returns pointer to created main window.
   */
  [[nodiscard]] virtual MainWindow* createMainWindow();

private:

  void createGuiContext();

protected:
  /** File manager instance, can be a null pointer */
  RECore::IFileManager*		mFileManager;
  /** Pointer to gui context, always valid! */
  GuiContext* mGuiContext;
  /** Pointer to main window */
  MainWindow* mMainWindow;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
