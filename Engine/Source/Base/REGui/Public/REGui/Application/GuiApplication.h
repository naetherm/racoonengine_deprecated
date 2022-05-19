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
#include "REGui/Application/GuiContext.h"
#include <RECore/Application/CoreApplication.h>
#include <RECore/Reflect/Event/EventHandler.h>


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
 * Gui application class
 *
 * @remarks
 * An application class for a typical GUI application. This application runs a main loop
 * that does the necessary message processing and provides methods to manage the windows
 * associated with this application.
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
   * Default constructor.
   */
  REGUI_API GuiApplication();

  /**
   * @brief
   * Destructor
   */
  REGUI_API virtual ~GuiApplication() override;


  /**
   * @brief
   * Gets pointer to the main window.
   *
   * @return
   * Main window, can be a nullptr.
   */
  REGUI_API MainWindow* getMainWindow() const;

  /**
   * @brief
   * Sets the main window.
   *
   * @param[in] nativeWindow
   * Pointer to the main window to set.
   */
  REGUI_API void setMainWindow(MainWindow* nativeWindow);

  //[-------------------------------------------------------]
  //[ Protected virtual PLCore::AbstractLifecycle functions ]
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

private:

  /**
   * @brief
   * Creates the gui context.
   */
  void createGuiContext();

  /**
   * @brief
   * Called when the main window was destroyed.
   */
  void onDestroyMainWindow();

protected:

  /**
   * @brief
   * Called when application should open it's main window.
   */
  REGUI_API virtual void onCreateMainWindow();
  
protected:
  /** Pointer to the gui context */
  GuiContext* mGuiContext;
  RECore::IFileManager*		mFileManager;		///< File manager instance, can be a null pointer

  /** Pointer to the main window */
  MainWindow* mMainWindow;
  /** Event handler */
  RECore::EventHandler<> EventHandlerOnDestroy;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
