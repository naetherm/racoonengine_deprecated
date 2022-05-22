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
#include "Examples/Framework/IApplicationRhi.h"
#include "Examples/Framework/ExampleBase.h"
#include "Examples/ExampleRunner.h"

#include <RECore/Log/DefaultLog.h>
#include <RECore/Core/DefaultAssert.h>
#ifdef EXAMPLES_MIMALLOC
	#include <RECore/Memory/MimallocAllocator.h>
#else
	#include <RECore/Memory//DefaultAllocator.h>
#endif
#include <RERHI/RhiInstance.h>

#if defined(RENDERER) && defined(RENDERER_GRAPHICS_DEBUGGER)
	#include <RERenderer/Core/RenderDocGraphicsDebugger.h>
#endif
#if defined(LINUX)
#include <RERHI/Linux/X11Context.h>
#endif


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
#ifdef EXAMPLES_MIMALLOC
	RECore::MimallocAllocator g_MimallocAllocator;
	RECore::IAllocator* g_Allocator = &g_MimallocAllocator;
#else
	RECore::DefaultAllocator g_DefaultAllocator;
	RECore::IAllocator* g_Allocator = &g_DefaultAllocator;
#endif


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		RECore::DefaultLog	g_DefaultLog;
		RECore::DefaultAssert g_DefaultAssert;


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
IApplicationRhi::IApplicationRhi(const char* rhiName, ExampleBase& exampleBase) :
	IApplication((std::string("RHI: \"") + std::string(rhiName) + "\" - Example: \"" + std::string(exampleBase.getExampleName()) + "\"").c_str()),
	#if defined(RENDERER) && defined(RENDERER_GRAPHICS_DEBUGGER)
		mGraphicsDebugger(nullptr),
	#endif
	mExampleBase(exampleBase),
	mRhiContext(nullptr),
	mRhiInstance(nullptr),
	mRhi(nullptr),
	mMainSwapChain(nullptr)
{
	mExampleBase.setApplicationFrontend(this);
	
	// Copy the given RHI name
	if (nullptr != rhiName)
	{
		strncpy(mRhiName, rhiName, 32);

		// In case the source string is longer then 32 bytes (including null terminator) make sure that the string is null terminated
		mRhiName[31] = '\0';
	}
	else
	{
		mRhiName[0] = '\0';
	}
}


//[-------------------------------------------------------]
//[ Public virtual IApplicationFrontend methods           ]
//[-------------------------------------------------------]
void IApplicationRhi::switchExample(const char* exampleName, const char* rhiName)
{
	ASSERT(nullptr != exampleName, "Invalid example name")
	mExampleBase.getExampleRunner().switchExample(exampleName, rhiName);
	exit();
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
bool IApplicationRhi::onInitialization()
{
	createRhi();
	mExampleBase.onInitialization();

	// Done
	return true;
}

void IApplicationRhi::onDeinitialization()
{
	mExampleBase.onDeinitialization();
	destroyRhi();
}

void IApplicationRhi::onUpdate()
{
	mExampleBase.onUpdate();
}

void IApplicationRhi::onResize()
{
	// Is there a RHI and main swap chain instance?
	if (nullptr != mRhi && nullptr != mMainSwapChain)
	{
		// Inform the swap chain that the size of the native window was changed
		// -> Required for Direct3D 9, Direct3D 10, Direct3D 11
		// -> Not required for OpenGL and OpenGL ES 3
		mMainSwapChain->resizeBuffers();
	}
}

void IApplicationRhi::onToggleFullscreenState()
{
	// Is there a RHI and main swap chain instance?
	if (nullptr != mRhi && nullptr != mMainSwapChain)
	{
		// Toggle the fullscreen state
		mMainSwapChain->setFullscreenState(!mMainSwapChain->getFullscreenState());
	}
}

void IApplicationRhi::onDrawRequest()
{
	if (mExampleBase.doesCompleteOwnDrawing())
	{
		// The example does the drawing completely on its own
		mExampleBase.draw(mCommandBuffer);
	}

	// Is there a RHI and main swap chain instance?
	else if (nullptr != mRhi && nullptr != mMainSwapChain)
	{
		{ // Scene rendering
			// Scoped debug event
			COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

			// Make the graphics main swap chain to the current render target
			RERHI::Command::SetGraphicsRenderTarget::create(mCommandBuffer, mMainSwapChain);

			{ // Since Direct3D 12 is command list based, the viewport and scissor rectangle must be set in every draw call to work with all supported RHI implementations
				// Get the window size
				RECore::uint32 width  = 1;
				RECore::uint32 height = 1;
				mMainSwapChain->getWidthAndHeight(width, height);

				// Set the graphics viewport and scissor rectangle
				RERHI::Command::SetGraphicsViewportAndScissorRectangle::create(mCommandBuffer, 0, 0, width, height);
			}

			// Call the draw method
			mExampleBase.draw(mCommandBuffer);
		}

		// Dispatch command buffer to the RHI implementation
		mCommandBuffer.dispatchToRhiAndClear(*mRhi);

		// Present the content of the current back buffer
		mMainSwapChain->present();
	}
}

void IApplicationRhi::onEscapeKey()
{
	if (mExampleBase.getExampleRunner().getCurrentExampleName() == "ImGuiExampleSelector")
	{
		exit();
	}
	else
	{
		switchExample("ImGuiExampleSelector");
	}
}


//[-------------------------------------------------------]
//[ Protected methods                                     ]
//[-------------------------------------------------------]
void IApplicationRhi::createRhi()
{
	ASSERT(nullptr == mRhi, "Invalid RHI instance")

	// Create the RHI instance
	mRhi = createRhiInstance(mRhiName);
	if (nullptr != mRhi)
	{
		// Create render pass using the preferred swap chain texture format
		const RERHI::Capabilities& capabilities = mRhi->getCapabilities();
		RERHI::RHIRenderPass* renderPass = mRhi->createRenderPass(1, &capabilities.preferredSwapChainColorTextureFormat, capabilities.preferredSwapChainDepthStencilTextureFormat, 1 RHI_RESOURCE_DEBUG_NAME("Main"));

		// Create a main swap chain instance
		mMainSwapChain = mRhi->createSwapChain(*renderPass, RERHI::WindowHandle{getNativeWindowHandle(), nullptr, nullptr}, mRhi->getContext().isUsingExternalContext() RHI_RESOURCE_DEBUG_NAME("Main"));
		mMainSwapChain->AddReference();	// Internal RHI reference
	}
}

void IApplicationRhi::destroyRhi()
{
	// Delete the RHI instance
	if (nullptr != mMainSwapChain)
	{
		mMainSwapChain->Release();
		mMainSwapChain = nullptr;
	}
	mRhi = nullptr;
	if (nullptr != mRhiInstance)
	{
		mRhiInstance->destroyRhi();
	}

	// Call base implementation after RHI was destroyed, needed at least under Linux see comments in private method "RHIInstance::loadRhiApiSharedLibrary()" for more details
	// TODO(naetherm) Try to find another solution which doesn't change the application flow which results in deinitialization been called twice
	#ifdef LINUX
		IApplication::onDeinitialization();
	#endif

	// Delete the RHI instance
	delete mRhiInstance;
	mRhiInstance = nullptr;
	delete mRhiContext;
	mRhiContext = nullptr;
	#ifdef RENDERER_GRAPHICS_DEBUGGER
		delete static_cast<RERenderer::RenderDocGraphicsDebugger*>(mGraphicsDebugger);
	#endif
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
RERHI::RHIDynamicRHI* IApplicationRhi::createRhiInstance(const char* rhiName)
{
	// Is the given pointer valid?
	if (nullptr != rhiName)
	{
		bool loadRhiApiSharedLibrary = false;
		RECore::ILog& log = (nullptr != mExampleBase.getCustomLog()) ? *mExampleBase.getCustomLog() : ::detail::g_DefaultLog;
		#ifdef _WIN32
			mRhiContext = new RERHI::RHIContext(::detail::g_DefaultAssert, g_Allocator, getNativeWindowHandle());
		#elif LINUX
			// Under Linux the OpenGL library interacts with the library from X11 so we need to load the library ourself instead letting it be loaded by the RHI instance
			// -> See http://dri.sourceforge.net/doc/DRIuserguide.html "11.5 libGL.so and dlopen()"
			loadRhiApiSharedLibrary = true;
			mRhiContext = new RERHI::X11Context(getX11Display(), getNativeWindowHandle());
		#endif
		#if defined(RENDERER) && defined(RENDERER_GRAPHICS_DEBUGGER)
			mGraphicsDebugger = new RERenderer::RenderDocGraphicsDebugger(*mRhiContext);
		#endif
		mRhiInstance = new RERHI::RhiInstance(rhiName, *mRhiContext, loadRhiApiSharedLibrary);
	}
	RERHI::RHIDynamicRHI* rhi = (nullptr != mRhiInstance) ? mRhiInstance->getRhi() : nullptr;

	// Is the RHI instance is properly initialized?
	if (nullptr != rhi && !rhi->isInitialized())
	{
		// We are not interested in not properly initialized RHI instances, so get rid of the broken thing
		rhi = nullptr;
		delete mRhiInstance;
		mRhiInstance = nullptr;
		delete mRhiContext;
		mRhiContext = nullptr;
	}

	#ifndef DEBUG
		// By using
		//   "RERHI::RHIRERHI::isDebugEnabled()"
		// in here its possible to check whether or not your application is currently running
		// within a known debug/profile tool like e.g. Direct3D PIX (also works directly within VisualStudio
		// 2017 out-of-the-box). In case you want at least try to protect your asset, you might want to stop
		// the execution of your application when a debug/profile tool is used which can e.g. record your data.
		// Please be aware that this will only make it a little bit harder to debug and e.g. while doing so
		// reading out your asset data. Public articles like
		// "PIX: How to circumvent D3DPERF_SetOptions" at
		//   http://www.gamedev.net/blog/1323/entry-2250952-pix-how-to-circumvent-d3dperf-setoptions/
		// describe how to "hack around" this security measurement, so, don't rely on it. Those debug
		// methods work fine when using a Direct3D RHI implementation. OpenGL on the other hand
		// has no Direct3D PIX like functions or extensions, use for instance "gDEBugger" (http://www.gremedy.com/)
		// instead.
		if (nullptr != rhi && rhi->isDebugEnabled())
		{
			// We don't allow debugging in case debugging is disabled
			RE_LOG(Critical, "Debugging with debug/profile tools like e.g. Direct3D PIX is disabled within this application")
			delete rhi;
			rhi = nullptr;
		}
	#endif

	// Done
	return rhi;
}
