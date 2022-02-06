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


/**
*  @brief
*    Amalgamated/unity build rendering hardware interface (RHI)
*
*  @remarks
*    == Description ==
*    Please note that this is a 100% interface project resulting in no binary at all. The one and only goal of this project is to offer an unified RHI for multiple graphics APIs like Vulkan, Direct3D 12 or OpenGL. Features like resource loading, font rendering or even rendering of complex scenes is out of the scope of this project.
*
*    In order to make it easier to use the RHI, the header "Rhi.h" puts everything together within a single header without any additional third party dependencies.
*
*    == Preprocessor Definitions ==
*    - Set "_WIN32" as preprocessor definition when building for Microsoft Windows
*    - Set "LINUX" as preprocessor definition when building for Linux or similar platforms
*        - For Linux or similar platforms: Set "HAVE_VISIBILITY_ATTR" as preprocessor definition to use the visibility attribute (the used compiler must support it)
*    - Set "__ANDROID__" as preprocessor definition when building for Android
*    - Set "ARCHITECTURE_X64" as preprocessor definition when building for x64 instead of x86
*    - Set "RHI_STATISTICS" as preprocessor definition in order to enable the gathering of statistics (tiny binary size and tiny negative performance impact)
*    - Set "DEBUG" as preprocessor definition in order to enable e.g. Direct3D 9 PIX functions (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box) debug features (disabling support just reduces the binary size slightly but makes debugging more difficult)
*    - Set 0-n of the following to tell the system which RHI implementations should be available during runtime: "RHI_NULL", "RHI_VULKAN", "RHI_OPENGL", "RHI_OPENGLES3", "RHI_DIRECT3D9", "RHI_DIRECT3D10", "RHI_DIRECT3D11", "RHI_DIRECT3D12"
*/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


#include <RECore/RECore.h>
#include "RERHI/RERHI.h"
#include "RERHI/RHIDynamicRHI.h"
#include "RERHI/RHIDynamicRHITypes.h"
#include "RERHI/RHIResourceGroup.h"
#include "RERHI/RHIRootSignature.h"
#include "RERHI/RHIRootSignatureTypes.h"
#include "RERHI/RHICapabilities.h"
#include "RERHI/RHIContext.h"
#include "RERHI/RHIResource.h"
#include "RERHI/RHIStatistics.h"
#include "RERHI/Buffer/RHICommandBuffer.h"
#include "RERHI/Buffer/RHIBuffer.h"
#include "RERHI/Buffer/RHIBufferTypes.h"
#include "RERHI/Buffer/RHIStructuredBuffer.h"
#include "RERHI/Buffer/RHIBufferManager.h"
#include "RERHI/Buffer/RHIIndexBuffer.h"
#include "RERHI/Buffer/RHIIndexBufferTypes.h"
#include "RERHI/Buffer/RHIUniformBuffer.h"
#include "RERHI/Buffer/RHITextureBuffer.h"
#include "RERHI/Buffer/RHIIndirectBuffer.h"
#include "RERHI/Buffer/RHIIndirectBufferTypes.h"
#include "RERHI/Buffer/RHIVertexArray.h"
#include "RERHI/Buffer/RHIVertexArrayTypes.h"
#include "RERHI/Buffer/RHIVertexBuffer.h"
#include "RERHI/RenderTarget/RHIRenderTarget.h"
#include "RERHI/RenderTarget/RHISwapChain.h"
#include "RERHI/RenderTarget/RHIFramebuffer.h"
#include "RERHI/RenderTarget/RHIRenderPass.h"
#include "RERHI/Query/RHIQueryPool.h"
#include "RERHI/Query/RHIQueryTypes.h"
#include "RERHI/Shader/RHIComputeShader.h"
#include "RERHI/Shader/RHIMeshShader.h"
#include "RERHI/Shader/RHITaskShader.h"
#include "RERHI/Shader/RHIFragmentShader.h"
#include "RERHI/Shader/RHITessellationControlShader.h"
#include "RERHI/Shader/RHITessellationEvaluationShader.h"
#include "RERHI/Shader/RHIVertexShader.h"
#include "RERHI/Shader/RHIGeometryShader.h"
#include "RERHI/Shader/RHIShaderLanguage.h"
#include "RERHI/Shader/RHIGraphicsProgram.h"
#include "RERHI/Shader/RHIShader.h"
#include "RERHI/Shader/RHIShaderTypes.h"
#include "RERHI/State/RHIGraphicsPipelineState.h"
#include "RERHI/State/RHIComputePipelineState.h"
#include "RERHI/State/RHISamplerState.h"
#include "RERHI/State/RHIPipelineState.h"
#include "RERHI/State/RHIState.h"
#include "RERHI/Texture/RHITextureCubeArray.h"
#include "RERHI/Texture/RHITextureCube.h"
#include "RERHI/Texture/RHITexture2DArray.h"
#include "RERHI/Texture/RHITexture2D.h"
#include "RERHI/Texture/RHITexture1DArray.h"
#include "RERHI/Texture/RHITexture1D.h"
#include "RERHI/Texture/RHITexture3D.h"
#include "RERHI/Texture/RHITextureManager.h"
#include "RERHI/Texture/RHITexture.h"


namespace RERHI {

	// Debug macros
	#ifdef DEBUG
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name) Rhi::Command::SetDebugMarker::create(commandBuffer, name);

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer) Rhi::Command::SetDebugMarker::create(commandBuffer, __FUNCTION__);

		/**
		*  @brief
		*    Begin debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name) Rhi::Command::BeginDebugEvent::create(commandBuffer, name);

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer) Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__);

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(commandBuffer) Rhi::Command::EndDebugEvent::create(commandBuffer);

		/**
		*  @brief
		*    Scoped debug event class
		*/
		class CommandScopedDebugEventOnExit
		{
		// Public methods
		public:
			inline explicit CommandScopedDebugEventOnExit(CommandBuffer& commandBuffer) :
				mCommandBuffer(commandBuffer)
			{}

			inline ~CommandScopedDebugEventOnExit()
			{
				Rhi::Command::EndDebugEvent::create(mCommandBuffer);
			}

		// Private methods
		private:
			explicit CommandScopedDebugEventOnExit(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;
			CommandScopedDebugEventOnExit& operator =(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;

		// Private data
		private:
			CommandBuffer& mCommandBuffer;
		};

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, name); \
			PRAGMA_WARNING_PUSH \
				PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			PRAGMA_WARNING_POP

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__); \
			PRAGMA_WARNING_PUSH \
				PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			PRAGMA_WARNING_POP
	#else
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name)

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer)

		/**
		*  @brief
		*    Begin debug event, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name)

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer)

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(commandBuffer)

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name)

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer)
	#endif




//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Rhi




//[-------------------------------------------------------]
//[ Debug macros                                          ]
//[-------------------------------------------------------]
#ifdef DEBUG
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name) if (nullptr != rhi) { (rhi)->setDebugMarker(name); }

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->setDebugMarker(__FUNCTION__); }

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name) if (nullptr != rhi) { (rhi)->beginDebugEvent(name); }

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->beginDebugEvent(__FUNCTION__); }

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi) if (nullptr != rhi) { (rhi)->endDebugEvent(); }

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name) , name

	/**
	*  @brief
	*    Decorate the debug name to make it easier to see the semantic of the resource
	*
	*  @param[in] name
	*    Debug name provided from the outside
	*  @param[in] decoration
	*    Decoration to append in front (e.g. "IBO", will result in appended "IBO: " in front if the provided name isn't empty)
	*  @param[in] numberOfDecorationCharacters
	*    Number of decoration characters
	*
	*  @note
	*    - The result is in local string variable "detailedName"
	*    - Traditional C-string on the runtime stack used for efficiency reasons (just for debugging, but must still be some kind of usable)
	*/
	#define RHI_DECORATED_DEBUG_NAME(name, detailedName, decoration, numberOfDecorationCharacters) \
		RE_LOG(strlen(name) < 256, "Name is not allowed to exceed 255 characters") \
		char detailedName[256 + numberOfDecorationCharacters] = decoration; \
		if (name[0] != '\0') \
		{ \
			strcat(detailedName, ": "); \
			strncat(detailedName, name, 256); \
		}
#else
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name)

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi)

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name)

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi)

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi)

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name)
#endif
