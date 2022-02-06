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
#include "Examples/Framework/IApplicationRenderer.h"
#include "Examples/Framework/ExampleBase.h"

#include <RERenderer/Context.h>
#include <RERenderer/RendererInstance.h>
#include <RERenderer/RendererImpl.h>
#include <RECore/Asset/AssetManager.h>
#ifdef RENDERER_PROFILER
	#include <RERenderer/Core/RemoteryProfiler.h>
#endif
#ifdef __ANDROID__
	#include <RECore/File/AndroidFileManager.h>

	#include <android_native_app_glue.h>
#else
	#include <RECore/File/PhysicsFSFileManager.h>
#endif

#ifdef RENDERER_TOOLKIT
	#include <RECore/File/DefaultFileManager.h>
	#include <RERenderer/Core/Platform/PlatformManager.h>

	#include <RERendererToolkit/RendererToolkitInstance.h>

	#include <exception>
#endif


//[-------------------------------------------------------]
//[ Public virtual IApplicationFrontend methods           ]
//[-------------------------------------------------------]
RERenderer::IRenderer* IApplicationRenderer::getRenderer() const
{
  return mRenderer;
	//return (nullptr != mRendererInstance) ? mRendererInstance->getRenderer() : nullptr;
}

RERendererToolkit::IRendererToolkit* IApplicationRenderer::getRendererToolkit()
{
	#ifdef RENDERER_TOOLKIT
		// Create the renderer toolkit instance, if required
		if (nullptr == mRendererToolkitInstance)
		{
			ASSERT(nullptr != mRendererInstance, "The renderer instance must be valid")
			const RERenderer::IRenderer* renderer = mRendererInstance->getRenderer();
			RECore::ILog& log = renderer->getRhi().getContext().getLog();
			RECore::IAssert& assert = renderer->getRhi().getContext().getAssert();
			RECore::IAllocator& allocator = renderer->getRhi().getContext().getAllocator();
			mRendererToolkitFileManager = new RERenderer::DefaultFileManager(log, assert, allocator, mFileManager->getAbsoluteRootDirectory());
			mRendererToolkitContext = new RERendererToolkit::Context(log, assert, allocator, *mRendererToolkitFileManager);
			mRendererToolkitInstance = new RERendererToolkit::RendererToolkitInstance(*mRendererToolkitContext);
		}
		return (nullptr != mRendererToolkitInstance) ? mRendererToolkitInstance->getRendererToolkit() : nullptr;
	#else
		return nullptr;
	#endif
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
bool IApplicationRenderer::onInitialization()
{
	// Create the RHI instance
	createRhi();

	// Is there a valid RHI instance?
	RERHI::RHIDynamicRHI* rhi = getRhi();
	if (nullptr != rhi)
	{
		// Create the renderer instance
		#ifdef __ANDROID__
			struct android_app androidApp;	// TODO(naetherm) Get Android app instance
			RHI_ASSERT(rhi->getContext(), nullptr != androidApp.activity->assetManager, "Invalid Android asset manager instance")
			mFileManager = new RERenderer::AndroidFileManager(rhi->getContext().getLog(), rhi->getContext().getAssert(), rhi->getContext().getAllocator(), std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string(), *androidApp.activity->assetManager);
		#else
			mFileManager = new RECore::PhysicsFSFileManager(std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string());
      mCoreContext = new RECore::CoreContext();
      mCoreContext->initialize(*mFileManager);
		#endif
		#if defined(RENDERER_GRAPHICS_DEBUGGER) && defined(RENDERER_PROFILER)
			mProfiler = new RERenderer::RemoteryProfiler(*rhi);
			mRendererContext = new RERenderer::Context(*rhi, *mFileManager, *mGraphicsDebugger, *mProfiler);
		#elif defined RENDERER_GRAPHICS_DEBUGGER
			mRendererContext = new RERenderer::Context(*rhi, *mFileManager, *mGraphicsDebugger);
		#elif defined RENDERER_PROFILER
			mProfiler = new RERenderer::RemoteryProfiler(*rhi);
			mRendererContext = new RERenderer::Context(*rhi, *mFileManager, *mProfiler);
		#else
			mRendererContext = new RERenderer::Context(*rhi, *mCoreContext);
		#endif
		//mRendererInstance = new RERenderer::RendererInstance(*mRendererContext);
    mRenderer = new RERenderer::RendererImpl(*mRendererContext);

		{
			RERenderer::IRenderer* renderer = getRenderer();
			if (nullptr != renderer)
			{
				// Mount asset package
				bool mountAssetPackageResult = false;
				RECore::AssetManager& assetManager = renderer->getAssetManager();
				bool rhiIsOpenGLES = (rhi->getNameId() == RERHI::NameId::OPENGLES3);
				if (rhiIsOpenGLES)
				{
					// Handy fallback for development: If the mobile data isn't there, use the PC data
					mountAssetPackageResult = (assetManager.mountAssetPackage("../DataMobile/Example/Content", "Example") != nullptr);
					if (!mountAssetPackageResult)
					{
						RE_LOG(Warning, "The examples application failed to find \"../DataMobile/Example/Content\", using \"../DataPc/Example/Content\" as fallback")
						mountAssetPackageResult = (assetManager.mountAssetPackage("../DataPc/Example/Content", "Example") != nullptr);
						rhiIsOpenGLES = false;
					}
				}
				else
				{
					mountAssetPackageResult = (assetManager.mountAssetPackage("../DataPc/Example/Content", "Example") != nullptr);
				}
				if (!mountAssetPackageResult)
				{
					// Error!
					showUrgentMessage("Please start \"ExampleProjectCompiler\" before starting \"Examples\" for the first time");
					deinitialization();
					return false;
				}
				renderer->loadPipelineStateObjectCache();

				// Load renderer toolkit project to enable hot-reloading in case of asset changes
				#ifdef RENDERER_TOOLKIT
				{
					RERendererToolkit::IRendererToolkit* rendererToolkit = getRendererToolkit();
					if (nullptr != rendererToolkit)
					{
						// The renderer toolkit project startup is done inside a background thread to not block the main thread
						mRendererToolkitProjectStartupThread = std::thread(&IApplicationRenderer::rendererToolkitProjectStartupThreadWorker, this, renderer, rendererToolkit, rhiIsOpenGLES);
					}
				}
				#endif
			}
		}
	}

	// Initialize the example now that the RHI instance should be created successfully
	mExampleBase.onInitialization();

	// Done
	return true;
}

void IApplicationRenderer::onDeinitialization()
{
	mExampleBase.onDeinitialization();
	deinitialization();
}

void IApplicationRenderer::onUpdate()
{
	RERenderer::IRenderer* renderer = getRenderer();
	if (nullptr != renderer)
	{
		renderer->update();
	}

	// Call base implementation
	IApplicationRhi::onUpdate();
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void IApplicationRenderer::deinitialization()
{
	//delete mRendererInstance;
	//mRendererInstance = nullptr;
  delete mRenderer;
  mRenderer = nullptr;
	delete mRendererContext;
	mRendererContext = nullptr;
	#ifdef RENDERER_PROFILER
		delete static_cast<RERenderer::RemoteryProfiler*>(mProfiler);
	#endif
	#ifdef __ANDROID__
		delete static_cast<RERenderer::AndroidFileManager*>(mFileManager);
	#else
		delete static_cast<RECore::PhysicsFSFileManager*>(mFileManager);
	#endif
	mFileManager = nullptr;
	#ifdef RENDERER_TOOLKIT
		{
			mRendererToolkitProjectStartupThread.join();
			std::lock_guard<std::mutex> projectMutexLock(mProjectMutex);
			if (nullptr != mProject)
			{
				delete mProject;
				mProject = nullptr;
			}
		}
		if (nullptr != mRendererToolkitInstance)
		{
			delete mRendererToolkitInstance;
			mRendererToolkitInstance = nullptr;
		}
		if (nullptr != mRendererToolkitContext)
		{
			delete mRendererToolkitContext;
			mRendererToolkitContext = nullptr;
		}
		if (nullptr != mRendererToolkitFileManager)
		{
			delete static_cast<RERenderer::DefaultFileManager*>(mRendererToolkitFileManager);
			mRendererToolkitFileManager = nullptr;
		}
	#endif
	destroyRhi();
}

#ifdef RENDERER_TOOLKIT
	void IApplicationRenderer::rendererToolkitProjectStartupThreadWorker(RERenderer::IRenderer* renderer, RERendererToolkit::IRendererToolkit* rendererToolkit, bool rhiIsOpenGLES)
	{
		RENDERER_SET_CURRENT_THREAD_DEBUG_NAME("Project startup", "Renderer toolkit: Project startup")
		std::lock_guard<std::mutex> projectMutexLock(mProjectMutex);
		mProject = rendererToolkit->createProject();
		if (nullptr != mProject)
		{
			try
			{
				// Load project: Shippable executable binaries are inside e.g. "racoonengine/Binary/Windows_x64_Shared" while development data source is located
				// at "racoonengine/Example/DataSource/Example" and the resulting compiled/baked data ends up inside e.g. "racoonengine/Binary/DataPc/Example"
				mProject->load("../../Example/DataSource/Example");
				mProject->startupAssetMonitor(*renderer, rhiIsOpenGLES ? "OpenGLES3_300" : "Direct3D11_50");
			}
			catch (const std::exception& e)
			{
				RE_LOG(Critical, "Failed to load renderer toolkit project: " + e.what())
			}
		}
	}
#endif
