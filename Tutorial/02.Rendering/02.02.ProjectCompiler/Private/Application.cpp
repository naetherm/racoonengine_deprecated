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
#include <RERendererToolkit/RendererToolkitImpl.h>
#include <RERendererToolkit/Project/IProject.h>
#include <RERendererToolkit/Context.h>

#include <RECore/File/FileSystemHelper.h>
#include <RECore/File/IFileManager.h>
#include <RECore/File/DefaultFileManager.h>

#include <RECore/Core/DefaultAssert.h>
#ifdef EXAMPLES_MIMALLOC
#include <RECore/Memory/MimallocAllocator.h>
#else
#include <RECore/Memory/DefaultAllocator.h>
#endif

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Application::Application()
: RECore::CoreApplication() {
  // Command line
  m_cCommandLine.addParameter("Target", "-t", "--target", "The render target to compile", "");
}

Application::~Application() {

}

void Application::main() {
  RECore::DefaultAssert assert;
#ifdef EXAMPLES_MIMALLOC
  RECore::MimallocAllocator allocator;
#else
  RECore::DefaultAllocator allocator;
#endif
  RECore::DefaultFileManager defaultFileManager(std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string());
  RERendererToolkit::Context rendererToolkitContext(defaultFileManager);
  //RERendererToolkit::RendererToolkitInstance rendererToolkitInstance(rendererToolkitContext);
  //RERendererToolkit::IRendererToolkit* rendererToolkit = rendererToolkitInstance.getRendererToolkit();
  RERendererToolkit::IRendererToolkit* rendererToolkit = new RERendererToolkit::RendererToolkitImpl(rendererToolkitContext);
  if (nullptr != rendererToolkit)
  {
    RERendererToolkit::IProject* project = rendererToolkit->createProject();
    try
    {
      // Load project: Shippable executable binaries are inside e.g. "unrimp/Binary/Windows_x64_Shared" while development data source is located
      // at "unrimp/Example/DataSource/Example" and the resulting compiled/baked data ends up inside e.g. "unrimp/Binary/DataPc/Example"
      project->load("../Example/DataSource/Example");

      if (m_cCommandLine.getNumOfOptions() == 0)
      {
        //	project->compileAllAssets("Direct3D9_30");
        //	project->compileAllAssets("Direct3D11_50");
        //	project->compileAllAssets("Direct3D12_50");
        //	project->compileAllAssets("OpenGLES3_300");
        project->compileAllAssets("OpenGL_440");
      }
      else
      {
        // For now all given arguments are interpreted as render target
        RECore::String renderTargetName = m_cCommandLine.getValue("Target");
        //for (const std::string_view& renderTarget : commandLineArguments.getArguments())
        {
          //const std::string renderTargetString(renderTarget);
          RE_LOG(Info, RECore::String("Compiling for target: ") + renderTargetName)
          project->compileAllAssets(renderTargetName.cstr());
          RE_LOG(Info, "Compilation done")
        }
      }
    }
    catch (const std::exception& e)
    {
      RE_LOG(Critical, RECore::String("Project compilation failed: ") + e.what())
      RE_LOG(Info, "Press any key to continue")
      getchar();
    }
    delete project;
  }
}
