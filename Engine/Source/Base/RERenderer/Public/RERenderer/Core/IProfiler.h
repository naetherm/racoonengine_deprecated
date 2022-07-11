/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


#ifdef RENDERER_PROFILER


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include <inttypes.h>	// For RECore::uint32, RECore::uint64 etc.


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace RERenderer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract profiler interface
		*/
		class IProfiler
		{


		//[-------------------------------------------------------]
		//[ Public virtual RERenderer::IProfiler methods            ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Begin profiler CPU sample section
			*
			*  @param[in] name
			*    Section name
			*  @param[in] hashCache
			*    Hash cache, can be a null pointer (less efficient)
			*/
			virtual void beginCpuSample(const char* name, RECore::uint32* hashCache) = 0;

			/**
			*  @brief
			*    End profiler CPU sample section
			*/
			virtual void endCpuSample() = 0;

			/**
			*  @brief
			*    Begin profiler GPU sample section
			*
			*  @param[in] name
			*    Section name
			*  @param[in] hashCache
			*    Hash cache, can be a null pointer (less efficient)
			*/
			virtual void beginGpuSample(const char* name, RECore::uint32* hashCache) = 0;

			/**
			*  @brief
			*    End profiler GPU sample section
			*/
			virtual void endGpuSample() = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			inline IProfiler()
			{
				// Nothing here
			}

			inline virtual ~IProfiler()
			{
				// Nothing here
			}

			explicit IProfiler(const IProfiler&) = delete;
			IProfiler& operator=(const IProfiler&) = delete;


		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // RECore


	//[-------------------------------------------------------]
	//[ Macros & definitions                                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Begin profiler CPU sample section, must be ended by using "RENDERER_PROFILER_END_CPU_SAMPLE()"
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_BEGIN_CPU_SAMPLE(context, name) { static RECore::uint32 sampleHash_##__LINE__ = 0; (context).getProfiler().beginCpuSample(name, &sampleHash_##__LINE__); }

	/**
	*  @brief
	*    End profiler CPU sample section
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*/
	#define RENDERER_PROFILER_END_CPU_SAMPLE(context) (context).getProfiler().endCpuSample();

	namespace RERenderer
	{
		/**
		*  @brief
		*    Scoped profiler CPU sample section
		*/
		class RendererProfilerScopedCpuSampleOnExit
		{
		// Public methods
		public:
			inline explicit RendererProfilerScopedCpuSampleOnExit(IProfiler& profiler) :
				mProfiler(profiler)
			{}

			inline ~RendererProfilerScopedCpuSampleOnExit()
			{
				mProfiler.endCpuSample();
			}

		// Private methods
		private:
			explicit RendererProfilerScopedCpuSampleOnExit(const RendererProfilerScopedCpuSampleOnExit& rendererProfilerScopedCpuSampleOnExit) = delete;
			RendererProfilerScopedCpuSampleOnExit& operator =(const RendererProfilerScopedCpuSampleOnExit& rendererProfilerScopedCpuSampleOnExit) = delete;

		// Private data
		private:
			IProfiler& mProfiler;
		};
	}

	/**
	*  @brief
	*    Scoped profiler CPU sample section, minor internal overhead compared to manual begin/end
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_SCOPED_CPU_SAMPLE(context, name) \
		{ static RECore::uint32 sampleHash_##__LINE__ = 0; (context).getProfiler().beginCpuSample(name, &sampleHash_##__LINE__); } \
		PRAGMA_WARNING_PUSH \
			PRAGMA_WARNING_DISABLE_MSVC(4456) \
			RERenderer::RendererProfilerScopedCpuSampleOnExit rendererProfilerScopedCpuSampleOnExit##__FUNCTION__((context).getProfiler()); \
		PRAGMA_WARNING_POP

	/**
	*  @brief
	*    Begin profiler GPU sample section, must be ended by using "RENDERER_PROFILER_END_GPU_SAMPLE()"
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_BEGIN_GPU_SAMPLE(context, name) { static RECore::uint32 sampleHash_##__LINE__ = 0; (context).getProfiler().beginGpuSample(name, &sampleHash_##__LINE__); }

	/**
	*  @brief
	*    End profiler GPU sample section
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*/
	#define RENDERER_PROFILER_END_GPU_SAMPLE(context) (context).getProfiler().endGpuSample();

	namespace RERenderer
	{
		/**
		*  @brief
		*    Scoped profiler GPU sample section
		*/
		class RendererProfilerScopedGpuSampleOnExit
		{
		// Public methods
		public:
			inline explicit RendererProfilerScopedGpuSampleOnExit(IProfiler& profiler) :
				mProfiler(profiler)
			{}

			inline ~RendererProfilerScopedGpuSampleOnExit()
			{
				mProfiler.endGpuSample();
			}

		// Private methods
		private:
			explicit RendererProfilerScopedGpuSampleOnExit(const RendererProfilerScopedGpuSampleOnExit& rendererProfilerScopedGpuSampleOnExit) = delete;
			RendererProfilerScopedGpuSampleOnExit& operator =(const RendererProfilerScopedGpuSampleOnExit& rendererProfilerScopedGpuSampleOnExit) = delete;

		// Private data
		private:
			IProfiler& mProfiler;
		};
	}

	/**
	*  @brief
	*    Scoped profiler GPU sample section, minor internal overhead compared to manual begin/end
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_SCOPED_GPU_SAMPLE(context, name) \
		{ static RECore::uint32 sampleHash_##__LINE__ = 0; (context).getProfiler().beginGpuSample(name, &sampleHash_##__LINE__); } \
		PRAGMA_WARNING_PUSH \
			PRAGMA_WARNING_DISABLE_MSVC(4456) \
			RERenderer::RendererProfilerScopedGpuSampleOnExit rendererProfilerScopedGpuSampleOnExit##__FUNCTION__((context).getProfiler()); \
		PRAGMA_WARNING_POP

	/**
	*  @brief
	*    Combined scoped profiler CPU and GPU sample as well as renderer debug event command and a constant name (more efficient)
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] commandBuffer
	*    Reference to the RHI command buffer instance to use
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_SCOPED_PROFILER_EVENT(context, commandBuffer, name) \
		COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name) \
		{ static RECore::uint32 sampleHash_##__LINE__ = 0; (context).getProfiler().beginGpuSample(name, &sampleHash_##__LINE__); (context).getProfiler().beginCpuSample(name, &sampleHash_##__LINE__); } \
		PRAGMA_WARNING_PUSH \
			PRAGMA_WARNING_DISABLE_MSVC(4456) \
			RERenderer::RendererProfilerScopedCpuSampleOnExit rendererProfilerScopedCpuSampleOnExit##__FUNCTION__((context).getProfiler()); \
			RERenderer::RendererProfilerScopedGpuSampleOnExit rendererProfilerScopedGpuSampleOnExit##__FUNCTION__((context).getProfiler()); \
		PRAGMA_WARNING_POP

	/**
	*  @brief
	*    Combined scoped profiler CPU and GPU sample as well as renderer debug event command and a dynamic name (less efficient)
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] commandBuffer
	*    Reference to the RHI command buffer instance to use
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(context, commandBuffer, name) \
		COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name) \
		(context).getProfiler().beginGpuSample(name, nullptr); \
		(context).getProfiler().beginCpuSample(name, nullptr); \
		PRAGMA_WARNING_PUSH \
			PRAGMA_WARNING_DISABLE_MSVC(4456) \
			RERenderer::RendererProfilerScopedCpuSampleOnExit rendererProfilerScopedCpuSampleOnExit##__FUNCTION__((context).getProfiler()); \
			RERenderer::RendererProfilerScopedGpuSampleOnExit rendererProfilerScopedGpuSampleOnExit##__FUNCTION__((context).getProfiler()); \
		PRAGMA_WARNING_POP

	/**
	*  @brief
	*    Combined scoped profiler CPU and GPU sample as well as renderer debug event command, the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] commandBuffer
	*    Reference to the RHI command buffer instance to use
	*
	*  @note
	*    - Often using this macro results in too long names which make things confusing to read, you might want to use "RENDERER_SCOPED_PROFILER_EVENT()" instead for explicit names
	*/
	#define RENDERER_SCOPED_PROFILER_EVENT_FUNCTION(context, commandBuffer) RENDERER_SCOPED_PROFILER_EVENT(context, commandBuffer, __FUNCTION__)
#else
	//[-------------------------------------------------------]
	//[ Macros & definitions                                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Begin profiler CPU sample section, must be ended by using "RENDERER_PROFILER_END_CPU_SAMPLE()"
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_BEGIN_CPU_SAMPLE(context, name)

	/**
	*  @brief
	*    End profiler CPU sample section
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*/
	#define RENDERER_PROFILER_END_CPU_SAMPLE(context)

	/**
	*  @brief
	*    Scoped profiler CPU sample section, minor internal overhead compared to manual begin/end
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_SCOPED_CPU_SAMPLE(context, name)

	/**
	*  @brief
	*    Begin profiler GPU sample section, must be ended by using "RENDERER_PROFILER_END_GPU_SAMPLE()"
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_BEGIN_GPU_SAMPLE(context, name)

	/**
	*  @brief
	*    End profiler GPU sample section
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*/
	#define RENDERER_PROFILER_END_GPU_SAMPLE(context)

	/**
	*  @brief
	*    Scoped profiler GPU sample section, minor internal overhead compared to manual begin/end
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_PROFILER_SCOPED_GPU_SAMPLE(context, name)

	/**
	*  @brief
	*    Combined scoped profiler CPU and GPU sample as well as renderer debug event command and a constant name (more efficient)
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] commandBuffer
	*    Reference to the RHI command buffer instance to use
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_SCOPED_PROFILER_EVENT(context, commandBuffer, name) \
		COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name)

	/**
	*  @brief
	*    Combined scoped profiler CPU and GPU sample as well as renderer debug event command and a dynamic name (less efficient)
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] commandBuffer
	*    Reference to the RHI command buffer instance to use
	*  @param[in] name
	*    Section name
	*/
	#define RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(context, commandBuffer, name) \
		COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name)

	/**
	*  @brief
	*    Combined scoped profiler CPU and GPU sample as well as renderer debug event command, the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] context
	*    Renderer context to ask for the profiler interface
	*  @param[in] commandBuffer
	*    Reference to the RHI command buffer instance to use
	*
	*  @note
	*    - Often using this macro results in too long names which make things confusing to read, you might want to use "RENDERER_SCOPED_PROFILER_EVENT()" instead for explicit names
	*/
	#define RENDERER_SCOPED_PROFILER_EVENT_FUNCTION(context, commandBuffer)
#endif
