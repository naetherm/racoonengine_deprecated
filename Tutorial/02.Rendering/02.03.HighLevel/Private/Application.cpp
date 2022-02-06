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
#include "Application.h"
#include "Scene/Scene.h"

//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Application, "", RERenderer::RendererApplication, "Renderer application class")
  // Constructors
  re_constructor_1_metadata(ParameterConstructor, RECore::Frontend &,
                            "Parameter constructor. Frontend this application instance is running in as first parameter.",
                            "")
re_class_metadata_end(Application)


Application::Application(RECore::Frontend& frontend):
RERenderer::RendererApplication(frontend)
, mExampleBase(nullptr) {

  m_cCommandLine.addParameter("RHI", "-r", "--rhi", "Low-Level RHI", "");
  m_cCommandLine.addParameter("Example", "-e", "--example", "The example to execute", "");
}

Application::~Application() {
  if (this->mExampleBase) {
    delete mExampleBase;
    mExampleBase = nullptr;
  }
}


bool Application::onStart() {

  if (RERenderer::RendererApplication::onStart()) {
    // Create example
    RECore::String sExample = m_cCommandLine.getValue("Example");

    if (sExample == "Scene") { this->mExampleBase = new Scene(*this, "Scene"); }

    if (this->mExampleBase) {
      this->mExampleBase->onInitialization();
    }

    return true;
  }

  // Error
  return false;
}


void Application::onStop() {
  this->mExampleBase->onDeinitialization();

  RERenderer::RendererApplication::onStop();
}

void Application::onSize() {
  RERenderer::RendererApplication::onSize();
}

void Application::onUpdate() {
  RERenderer::RendererApplication::onUpdate();

  this->mExampleBase->onUpdate();
}


void Application::drawInternal(RERHI::RHICommandBuffer &commandBuffer) {
  if (this->mExampleBase) {
    this->mExampleBase->draw(commandBuffer);
  }
}

void Application::main() {

}