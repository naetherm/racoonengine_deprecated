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
#include "RERenderer/Application/RendererApplication.h"
#include "RERHI/Linux/X11Context.h"
#include <RECore/Log/Log.h>
#include <RECore/File/PhysicsFSFileManager.h>
#include <RECore/Asset/AssetManager.h>
#include <RECore/Platform/Platform.h>
#include <RECore/Frontend/FrontendOS.h>
#include <RERHI/Rhi.h>
#if defined(LINUX)
#include <RECore/Linux/WindowOSLinux.h>
#endif
#include <RERenderer/Context.h>
#include <RERenderer/RendererImpl.h>

namespace RERenderer {

//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(RendererApplication, "RERenderer", RECore::FrontendApplication, "Renderer application class")
  // Constructors
  re_constructor_1_metadata(ParameterConstructor, RECore::Frontend &,
                            "Parameter constructor. Frontend this application instance is running in as first parameter.",
                            "")
re_class_metadata_end(RendererApplication)


RendererApplication::RendererApplication(RECore::Frontend &frontend)
  : RECore::FrontendApplication(frontend), mRhi(nullptr), mMainSwapChain(nullptr), mRendererContext(nullptr),
    mRenderer(nullptr) {
  // Register command line options:
  // [-r/--rhi] The rendering hardware interface
  // [-e/--example] The example to execute
}

RendererApplication::~RendererApplication() {
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


RERHI::RHIDynamicRHI* RendererApplication::getRhi() const {
  return mRhi;
}

RERHI::RHIRenderTarget* RendererApplication::getMainRenderTarget() const {
  return mMainSwapChain;
}

RERenderer::IRenderer* RendererApplication::getRenderer() const {
  return mRenderer;
}

RERenderer::IRenderer& RendererApplication::getRendererSafe() const {
  return *mRenderer;
}


bool RendererApplication::onStart() {
  mFileManager = new RECore::PhysicsFSFileManager(std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string());
  mcCoreContext.initialize(*mFileManager);
  if (RECore::FrontendApplication::onStart()) {
    // Command line
    mRhiName = m_cCommandLine.getValue("RHI");
    // Generate library name
    this->mSharedLibraryName = RECore::Platform::instance().getSharedLibraryPrefix() + "RERHI" + mRhiName + "." +
                               RECore::Platform::instance().getSharedLibraryExtension();

    this->onCreateRendererContext();

    RECore::AssetManager& assetManager = this->mRenderer->getAssetManager();
    bool mountAssetPackageResult = (assetManager.mountAssetPackage("../DataPc/Example/Content", "Example") != nullptr);

    if (!mountAssetPackageResult) {
      RE_LOG(Critical, "Unable to mount asset package")
    }

    mRenderer->loadPipelineStateObjectCache();

    // Create swap chain
    // Create the swapchain
    const RERHI::Capabilities &cCapabilities = this->mRhi->getCapabilities();
    RERHI::RHIRenderPass *pRenderPass = this->mRhi->createRenderPass(1,
                                                                     &cCapabilities.preferredSwapChainColorTextureFormat,
                                                                     cCapabilities.preferredSwapChainDepthStencilTextureFormat,
                                                                     1);
    this->mMainSwapChain = this->mRhi->createSwapChain(*pRenderPass,
                                                       RERHI::WindowHandle{this->getFrontend().getNativeWindowHandle(),
                                                                           nullptr,
                                                                           nullptr},
                                                       mRhi->getContext().isUsingExternalContext()
                                                       RHI_RESOURCE_DEBUG_NAME("Main"));
    this->mMainSwapChain->AddReference();

    return true;
  }

  return false;
}

void RendererApplication::onStop() {
  FrontendApplication::onStop();
}


void RendererApplication::onSize() {
  RECore::FrontendApplication::onSize();
}

void RendererApplication::onFullscreenMode() {

}

void RendererApplication::onInitConfig() {

}

void RendererApplication::onInitData() {

}

void RendererApplication::onUpdate() {
  FrontendApplication::onUpdate();

  RERenderer::IRenderer* renderer = getRenderer();
  if (nullptr != renderer)
  {
    renderer->update();
  }

  RERHI::Command::SetGraphicsRenderTarget::create(this->mCommandBuffer, this->mMainSwapChain);
}

void RendererApplication::onDraw() {

  this->drawInternal(this->mCommandBuffer);
  //this->mCommandBuffer.dispatchToRhiAndClear(*this->mRhi);
  //this->mMainSwapChain->present();

  FrontendApplication::onDraw();
}


void RendererApplication::drawInternal(RERHI::RHICommandBuffer &commandBuffer) {
  // Implement this!
}


void RendererApplication::onCreateRendererContext() {
  // Load and initialize the rhi backend
  // Create the RHI context
#if defined(LINUX)
  this->mRhiContext = new RERHI::X11Context(
    reinterpret_cast<RECore::WindowOSLinux *>(reinterpret_cast<RECore::FrontendOS *>(this->getFrontend().getImpl())->getWindow())->getDisplay(),
    this->getFrontend().getNativeWindowHandle()
  );
#endif

  // Initialize library
  auto *pLib = new RECore::DynLib();
  if (pLib->load(this->mSharedLibraryName)) {
    typedef RERHI::RHIDynamicRHI *(*RHI_INSTANCER)(const RERHI::RHIContext &);
    RHI_INSTANCER _Creator = reinterpret_cast<RHI_INSTANCER>(pLib->getSymbol("createRhiInstance"));
    if (_Creator) {
      this->mRhi = _Creator(*this->mRhiContext);

      if (this->mRhi) {
        this->mRhi->AddReference();
        RE_LOG(Info, std::string("Successfully created the RHI backend of ") + this->mSharedLibraryName.cstr())
      } else {
        // Error
        RE_LOG(Error, "Creation of dynamic RHI failed.")
      }
    } else {
      // Error
      RE_LOG(Error, "Unable to find the symbol 'createRHIInstance'")
    }
  } else {
    // Error
    RE_LOG(Error, std::string("Unable to load the library ") + this->mSharedLibraryName.cstr())
  }

  // Create renderer instance
  {
    mRendererContext = new RERenderer::Context(*mRhi, mcCoreContext);
    mRenderer = new RERenderer::RendererImpl(*mRendererContext);
    RE_LOG(Info,"Created renderer context")
  }
}

}