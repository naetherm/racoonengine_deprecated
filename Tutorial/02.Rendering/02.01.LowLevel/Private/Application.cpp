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
#include "RERHI/Linux/X11Context.h"
#include <RECore/Platform/Platform.h>
#include <RECore/Frontend/FrontendOS.h>
#include <RECore/Log/Log.h>
#include <RERHI/Rhi.h>
#if defined(LINUX)
#include <RECore/Linux/LinuxWindowOS.h>
#endif

#include "Empty/Empty.h"
#include "ComputeShader/ComputeShader.h"
#include "CubeTexture/CubeTexture.h"
#include "GeometryShader/GeometryShader.h"
#include "IndirectBuffer/IndirectBuffer.h"
#include "Instancing/Instancing.h"
#include "MeshShader/MeshShader.h"
#include "Queries/Queries.h"
#include "RenderToTexture/RenderToTexture.h"
#include "TessellationShader/TessellationShader.h"
#include "Texture/Texture.h"
#include "Triangle/Triangle.h"
#include "VertexBuffer/VertexBuffer.h"
#include "Gpgpu/Gpgpu.h"
#include "IcosahedronTessellation/IcosahedronTessellation.h"
#include "InstancedCubes/InstancedCubes.h"


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Application, "", RECore::FrontendApplication, "Renderer application class")
  // Constructors
  re_constructor_1_metadata(ParameterConstructor,	RECore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
re_class_metadata_end(Application)


Application::Application(RECore::Frontend &frontend)
: RECore::FrontendApplication(frontend)
, mRhi(nullptr)
, mMainSwapChain(nullptr)
, mRendererContext(nullptr)
, mRenderer(nullptr)
, mExampleBase(nullptr) {
  // Register command line options:
  // [-r/--rhi] The rendering hardware interface
  // [-e/--example] The example to execute
  m_cCommandLine.addParameter("RHI", "-r", "--rhi", "Low-Level RHI, one of: OpenGL", "", true);
  m_cCommandLine.addParameter("Example", "-e", "--example", "One of: Empty, ComputeShader, CubeTexture, GeometryShader, IndirectBuffer, Instancing, MeshShader, Queries, RenderToTexture, TessellationShader, Texture, Triangle VertexBuffer, Gpgpu, IcosahedronTessellation, InstancedCubes", "", true);
}

Application::~Application() {
  if (this->mExampleBase) {
    delete mExampleBase;
    mExampleBase = nullptr;
  }
  if (this->mRenderer) {
    delete mRenderer;
    mRenderer = nullptr;
  }
  if (this->mRendererContext) {
    delete mRendererContext;
    mRendererContext = nullptr;
  }
  if (this->mMainSwapChain) {
    delete mMainSwapChain;
    mMainSwapChain = nullptr;
  }
  if (this->mRhi) {
    delete mRhi;
    mRhi = nullptr;
  }
}


bool Application::onStart() {
  if (RECore::FrontendApplication::onStart()) {
    RECore::String sRHI = m_cCommandLine.getValue("RHI");
    RECore::String sExample = m_cCommandLine.getValue("Example");

    // Generate library name
    this->msLibraryName = RECore::Platform::instance().getSharedLibraryPrefix() + "RERHI" + sRHI + "." + RECore::Platform::instance().getSharedLibraryExtension();

    this->onCreateRendererContext();

    // Create swap chain
    // Create the swapchain
    const RERHI::Capabilities& cCapabilities = this->mRhi->getCapabilities();
    RERHI::RHIRenderPass* pRenderPass = this->mRhi->createRenderPass(1, &cCapabilities.preferredSwapChainColorTextureFormat, cCapabilities.preferredSwapChainDepthStencilTextureFormat, 1);
    this->mMainSwapChain = this->mRhi->createSwapChain(*pRenderPass, RERHI::WindowHandle{this->getFrontend().getNativeWindowHandle(), nullptr,
                                                                                         nullptr}, mRhi->getContext().isUsingExternalContext() RHI_RESOURCE_DEBUG_NAME("Main"));
    this->mMainSwapChain->AddReference();

    // Create and initialize the example
    if (sExample == "Empty") { this->mExampleBase = new Empty(*this, "Empty"); }
    if (sExample == "ComputeShader") { this->mExampleBase = new ComputeShader(*this, "ComputeShader"); }
    if (sExample == "CubeTexture") { this->mExampleBase = new CubeTexture(*this, "CubeTexture"); }
    if (sExample == "GeometryShader") { this->mExampleBase = new GeometryShader(*this, "GeometryShader"); }
    if (sExample == "IndirectBuffer") { this->mExampleBase = new IndirectBuffer(*this, "IndirectBuffer"); }
    if (sExample == "Instancing") { this->mExampleBase = new Instancing(*this, "Instancing"); }
    if (sExample == "MeshShader") { this->mExampleBase = new MeshShader(*this, "MeshShader"); }
    if (sExample == "Queries") { this->mExampleBase = new Queries(*this, "Queries"); }
    if (sExample == "RenderToTexture") { this->mExampleBase = new RenderToTexture(*this, "RenderToTexture"); }
    if (sExample == "TessellationShader") { this->mExampleBase = new TessellationShader(*this, "TessellationShader"); }
    if (sExample == "Texture") { this->mExampleBase = new Texture(*this, "Texture"); }
    if (sExample == "Triangle") { this->mExampleBase = new Triangle(*this, "Triangle"); }
    if (sExample == "VertexBuffer") { this->mExampleBase = new VertexBuffer(*this, "VertexBuffer"); }
    if (sExample == "Gpgpu") { this->mExampleBase = new Gpgpu(*this, "Gpgpu"); }
    if (sExample == "IcosahedronTessellation") { this->mExampleBase = new IcosahedronTessellation(*this, "IcosahedronTessellation"); }
    if (sExample == "InstancedCubes") { this->mExampleBase = new InstancedCubes(*this, "InstancedCubes"); }

    if (this->mExampleBase) {
      this->mExampleBase->onInitialization();
    }

    return true;
  }

  return false;
}

void Application::onStop() {
  this->mExampleBase->onDeinitialization();

  FrontendApplication::onStop();
}


void Application::onSize() {
  RECore::FrontendApplication::onSize();
}

void Application::onUpdate() {
  FrontendApplication::onUpdate();

  RERHI::Command::SetGraphicsRenderTarget::create(this->mCommandBuffer, this->mMainSwapChain);
  this->mExampleBase->onUpdate();
}

void Application::onDraw() {
  if (this->mExampleBase) {
    this->mExampleBase->onDraw(mCommandBuffer);
    this->mCommandBuffer.dispatchToRhiAndClear(*this->mRhi);
    this->mMainSwapChain->present();
  } else {
    RE_LOG(Info, "Invalid example")
  }
  FrontendApplication::onDraw();
}



void Application::main() {

}

void Application::onCreateRendererContext() {
  // Load and initialize the rhi backend
  // Create the RHI context
#if defined(LINUX)
  this->mpRHIContext = new RERHI::X11Context(
    reinterpret_cast<RECore::LinuxWindowOS*>(reinterpret_cast<RECore::FrontendOS*>(this->getFrontend().getImpl())->getWindow())->getDisplay(),
    this->getFrontend().getNativeWindowHandle()
  );
#endif

  // Initialize library
  auto* pLib = new RECore::DynLib();
  if (pLib->load(this->msLibraryName)) {
    typedef RERHI::RHIDynamicRHI* (*RHI_INSTANCER)(const RERHI::RHIContext&);
    RHI_INSTANCER _Creator = reinterpret_cast<RHI_INSTANCER>(pLib->getSymbol("createRhiInstance"));
    if (_Creator) {
      this->mRhi = _Creator(*this->mpRHIContext);

      if (this->mRhi) {
        this->mRhi->AddReference();
        RE_LOG(Info, std::string("Successfully created the RHI backend of ") + this->msLibraryName.cstr())
      } else {
        // Error
        RE_LOG(Error, "Creation of dynamic RHI failed.")
      }
    } else {
      // Error
      RE_LOG(Error, "Unable to find the symbol 'CreateRHIInstance'")
    }
  } else {
    // Error
    RE_LOG(Error, std::string("Unable to load the library ") + this->msLibraryName.cstr())
  }
}
