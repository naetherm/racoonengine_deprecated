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
#include "RERenderer/Context.h"

#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	#if defined(RENDERER_GRAPHICS_DEBUGGER) && defined(RENDERER_PROFILER)
		Context::Context(RERHI::RHIDynamicRHI& rhi, RECore::IFileManager& fileManager, IGraphicsDebugger& graphicsDebugger, IProfiler& profiler) :
	#elif defined RENDERER_GRAPHICS_DEBUGGER
		Context::Context(RERHI::RHIDynamicRHI& rhi, RECore::IFileManager& fileManager, IGraphicsDebugger& graphicsDebugger) :
	#elif defined RENDERER_PROFILER
		Context::Context(RERHI::RHIDynamicRHI& rhi, RECore::IFileManager& fileManager, IProfiler& profiler) :
	#else
		Context::Context(RERHI::RHIDynamicRHI& rhi, RECore::CoreContext& coreContext) :
	#endif
		mRhi(rhi),
		mCoreContext(coreContext)
		#ifdef RENDERER_GRAPHICS_DEBUGGER
			, mGraphicsDebugger(graphicsDebugger)
		#endif
		#ifdef RENDERER_PROFILER
			, mProfiler(profiler)
		#endif
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
