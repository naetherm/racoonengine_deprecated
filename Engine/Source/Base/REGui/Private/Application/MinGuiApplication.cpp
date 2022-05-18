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
#include "REGui/Application/MinGuiApplication.h"
#include "REGui/Gui/AppWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(MinGuiApplication, "REGui", RERenderer::RendererApplication, "Renderer application class")
  // Constructors
  re_constructor_1_metadata(ParameterConstructor, RECore::Frontend &,
                            "Parameter constructor. Frontend this application instance is running in as first parameter.",
                            "")
re_class_metadata_end(MinGuiApplication)


MinGuiApplication::MinGuiApplication(RECore::Frontend& frontend)
: RERenderer::RendererApplication(frontend)
, mMainWindow(nullptr) {

}

MinGuiApplication::~MinGuiApplication() {
  if (mMainWindow) {
    delete mMainWindow;
  }
}


bool MinGuiApplication::onStart() {

  if (RERenderer::RendererApplication::onStart()) {
    // Create example


    return true;
  }

  // Error
  return false;
}


void MinGuiApplication::onStop() {

  RERenderer::RendererApplication::onStop();
}

void MinGuiApplication::onSize() {
  RERenderer::RendererApplication::onSize();
}

void MinGuiApplication::onUpdate() {
  RERenderer::RendererApplication::onUpdate();
}


void MinGuiApplication::main() {

}

void MinGuiApplication::createMainWindow() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
