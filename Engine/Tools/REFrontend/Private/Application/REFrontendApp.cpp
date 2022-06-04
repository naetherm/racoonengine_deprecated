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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "REFrontend/Application/REFrontendApp.h"
#include "REFrontend/MainWindow/MainWindow.h"
#include <REGui/Gui/Gui.h>


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(REFrontendApp, "", REGui::GuiApplication, "Application class")
  // Constructors
  re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
re_class_metadata_end(REFrontendApp)


REFrontendApp::REFrontendApp()
  : REGui::GuiApplication() {

}

REFrontendApp::~REFrontendApp() {

}



/**
 * @brief
 * Helper method for creating the main window.
 *
 * @return
 * Returns pointer to created main window.
 */
REGui::MainWindow* REFrontendApp::createMainWindow() {
  MainWindow * mainWindow = new MainWindow(&REGui::Gui::instance());
  mainWindow->setTitle("REFrontend");

  return mainWindow;
}