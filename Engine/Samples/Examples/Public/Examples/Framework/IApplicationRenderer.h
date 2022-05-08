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
#include "Examples/Framework/IApplicationRhi.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: '=': conversion from 'int' to '::size_t', signed/unsigned mismatch
	PRAGMA_WARNING_DISABLE_MSVC(4371)	// warning C4371: 'std::_Tuple_val<_This>': layout of class may have changed from a previous version of the compiler due to better packing of member 'std::_Tuple_val<_This>::_Val'
	PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	PRAGMA_WARNING_DISABLE_MSVC(4623)	// warning C4623: 'std::_UInt_is_zero': default constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::codecvt_base': copy constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::codecvt_base': assignment operator was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::_Generic_error_category': move constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Generic_error_category': move assignment operator was implicitly defined as deleted
	#include <mutex>
	#include <thread>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RECore {
class IFileManager;
class CoreContext;
}
namespace RERenderer
{
	class Context;
	class IRenderer;
	class RendererInstance;
	#ifdef RENDERER_PROFILER
		class IProfiler;
	#endif
}
namespace RERendererToolkit
{
	class IRendererToolkit;
	#ifdef RENDERER_TOOLKIT
		class Context;
		class IProject;
		class RendererToolkitInstance;
	#endif
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Renderer application interface
*/
class IApplicationRenderer final : public IApplicationRhi
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Constructor
	*
	*  @param[in] rhiName
	*    Case sensitive ASCII name of the RHI to instance, if null pointer or unknown RHI no RHI will be used.
	*    Example RHI names: "Null", "Vulkan", "OpenGL", "OpenGLES3", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"
	*  @param[in] exampleBase
	*    Reference to an example which should be used
	*/
	inline IApplicationRenderer(const char* rhiName, ExampleBase& exampleBase) :
		IApplicationRhi(rhiName, exampleBase),
		mFileManager(nullptr),
    mCoreContext(nullptr),
		#ifdef RENDERER_PROFILER
			mProfiler(nullptr),
		#endif
		mRendererContext(nullptr),
		mRenderer(nullptr)
		#ifdef RENDERER_TOOLKIT
			, mRendererToolkitFileManager(nullptr)
			, mRendererToolkitContext(nullptr)
			, mRendererToolkitInstance(nullptr)
			, mProject(nullptr)
		#endif
	{
		// Nothing here
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~IApplicationRenderer() override
	{
		// Nothing here

		// "mFileManager", "mProfiler" and "mRendererInstance" is destroyed within "onDeinitialization()"
	}


//[-------------------------------------------------------]
//[ Public virtual IApplicationFrontend methods           ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Return the renderer instance
	*
	*  @return
	*    The renderer instance, can be a null pointer
	*/
	[[nodiscard]] virtual RERenderer::IRenderer* getRenderer() const override;

	/**
	*  @brief
	*    Return the renderer toolkit instance
	*
	*  @return
	*    The renderer toolkit instance, can be a null pointer
	*
	*  @remarks
	*    During runtime, the renderer toolkit can optionally be used to enable asset hot-reloading. Meaning,
	*    as soon as an source asset gets changed, the asset is recompiled in a background thread and the compiled
	*    runtime-ready asset is reloaded. One can see the change in realtime without the need to restart the application.
	*
	*    This feature links during runtime the renderer toolkit as soon as this method is accessed the first time. If
	*    the renderer toolkit shared library is not there, this method will return a null pointer. This is a developer-feature
	*    and as such, it's not available in static builds which are meant for the end-user who e.g. just want to "play the game".
	*/
	[[nodiscard]] virtual RERendererToolkit::IRendererToolkit* getRendererToolkit() override;


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
public:
	[[nodiscard]] virtual bool onInitialization() override;
	virtual void onDeinitialization() override;
	virtual void onUpdate() override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	explicit IApplicationRenderer(const IApplicationRenderer& source) = delete;
	IApplicationRenderer& operator =(const IApplicationRenderer& source) = delete;
	void deinitialization();
	#ifdef RENDERER_TOOLKIT
		void rendererToolkitProjectStartupThreadWorker(RERenderer::IRenderer* renderer, RERendererToolkit::IRendererToolkit* rendererToolkit, bool rhiIsOpenGLES);
	#endif


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	RECore::IFileManager*		mFileManager;		///< File manager instance, can be a null pointer
	RECore::CoreContext* mCoreContext;
	#ifdef RENDERER_PROFILER
		RERenderer::IProfiler*	mProfiler;			///< Profiler instance, can be a null pointer
	#endif
	RERenderer::Context*			mRendererContext;	///< Renderer context instance, can be a null pointer
	//RERenderer::RendererInstance* mRendererInstance;	///< Renderer instance, can be a null pointer
  RERenderer::IRenderer* mRenderer;
	#ifdef RENDERER_TOOLKIT
		RECore::IFileManager*					  mRendererToolkitFileManager;	///< Renderer toolkit file manager instance to avoid intermixing virtual filenames with the renderer, can be a null pointer
		RERendererToolkit::Context*				  mRendererToolkitContext;		///< Renderer toolkit context instance, can be a null pointer
		RERendererToolkit::RendererToolkitInstance* mRendererToolkitInstance;		///< Renderer toolkit instance, can be a null pointer
		std::mutex								  mProjectMutex;
		std::thread								  mRendererToolkitProjectStartupThread;
		RERendererToolkit::IProject*				  mProject;						///< Use "mProjectMutex" when accessing it
	#endif


};
