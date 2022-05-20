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
#include <map>


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
 * WindowManager
 *
 * @brief
 * A collection of all windows currently existing within the gui system.
 */
class WindowManager {
public:

  /**
   * @brief
   * Destructor.
   */
  WindowManager();

  /**
   * @brief
   * Destructor.
   */
  ~WindowManager();


  /**
   * @brief
   * Returns whether there is at least one window.
   *
   * @return
   * True if no window is registered, false otherwise.
   */
  [[nodiscard]] bool hasWindows() const;


  /**
   * @brief
   * Adds mainWindow to the list of all windows.
   *
   * @param[in] mainWindow
   * Pointer to the main window.
   */
  void addMainWindow(MainWindow* mainWindow);

  /**
   * @brief
   * Removes mainWindow from the list of all windows.
   *
   * @param[in] mainWindow
   * Pointer to main window.
   */
  void removeMainWindow(MainWindow* mainWindow);

  /**
   * @brief
   * Returns whether there exists a window mainWindow.
   *
   * @param[in] mainWindow
   * Pointer to main window.
   *
   * @return
   * True if there is such a window, false otherwise.
   */
  [[nodiscard]] bool hasMainWindow(MainWindow* mainWindow) const;

  /**
   * @brief
   * Returns whether there exists a window window handle.
   *
   * @param[in] window handle
   * Pointer to window handle.
   *
   * @return
   * True if there is such a window, false otherwise.
   */
  [[nodiscard]] bool hasMainWindow(RECore::handle windowHandle) const;

  /**
   * @brief
   * Returns pointer to the main window associated with the provided windowHandle.
   *
   * @param[in] windowHandle
   * Window handle to search for.
   *
   * @return
   * Pointer to window if it exists, nullptr otherwise.
   */
  MainWindow* getMainWindow(RECore::handle windowHandle) const;
private:
  /** Map containing pointers to main windows */
  std::map<RECore::handle, MainWindow*> mWindows;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
