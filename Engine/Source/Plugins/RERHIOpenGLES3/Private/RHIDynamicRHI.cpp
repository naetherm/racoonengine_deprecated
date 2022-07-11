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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/RHIResourceGroup.h"
#include "RERHIOpenGLES3/RHIRootSignature.h"
#include "RERHIOpenGLES3/Buffer/RHIIndexBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHIVertexBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHITextureBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHIBufferManager.h"
#include "RERHIOpenGLES3/Buffer/RHIStructuredBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHIUniformBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHIVertexArray.h"
#include "RERHIOpenGLES3/Buffer/RHIIndirectBuffer.h"
#include "RERHIOpenGLES3/RenderTarget/RHIRenderPass.h"
#include "RERHIOpenGLES3/RenderTarget/RHIFramebuffer.h"
#include "RERHIOpenGLES3/RenderTarget/RHISwapChain.h"
#include "RERHIOpenGLES3/Shader/RHIShaderLanguageGLSL.h"
#include "RERHIOpenGLES3/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIOpenGLES3/Texture/RHITextureManager.h"
#include "RERHIOpenGLES3/Texture/RHITexture1D.h"
#include "RERHIOpenGLES3/Texture/RHITexture1DArray.h"
#include "RERHIOpenGLES3/Texture/RHITexture2D.h"
#include "RERHIOpenGLES3/Texture/RHITexture2DArray.h"
#include "RERHIOpenGLES3/Texture/RHITexture3D.h"
#include "RERHIOpenGLES3/Texture/RHITextureCube.h"
#include "RERHIOpenGLES3/Texture/RHITextureCubeArray.h"
#include "RERHIOpenGLES3/State/RHISamplerState.h"
#include "RERHIOpenGLES3/State/RHIGraphicsPipelineState.h"
#include "RERHIOpenGLES3/Extensions.h"
#include "RERHIOpenGLES3/ExtensionsRuntimeLinking.h"
#include "RERHIOpenGLES3/OpenGLES3Context.h"
#include "RERHIOpenGLES3/OpenGLES3RuntimeLinking.h"
#include "RERHIOpenGLES3/OpenGLES3ContextRuntimeLinking.h"
#include "RERHIOpenGLES3/Helper.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
namespace detail
{


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
// Implementation from "08/02/2015 Better array 'countof' implementation with C++ 11 (updated)" - https://www.g-truc.net/post-0708.html
template<typename T, std::size_t N>
[[nodiscard]] constexpr std::size_t countof(T const (&)[N])
{
  return N;
}

[[nodiscard]] bool mapBuffer([[maybe_unused]] const RERHI::RHIContext& context, GLenum target, GLenum bindingTarget, GLuint openGLES3Buffer, RECore::uint32 bufferSize, RERHI::MapType mapType, RERHI::MappedSubresource& mappedSubresource)
{
  // TODO(naetherm) This buffer update isn't efficient, use e.g. persistent buffer mapping

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently bound OpenGL ES 3 buffer
				GLint openGLES3BufferBackup = 0;
				glGetIntegerv(bindingTarget, &openGLES3BufferBackup);
#endif

  // Bind this OpenGL ES 3 buffer
  glBindBuffer(target, openGLES3Buffer);

  // Map
  mappedSubresource.data		 = glMapBufferRange(target, 0, static_cast<GLsizeiptr>(bufferSize), RERHIOpenGLES3::Mapping::getOpenGLES3MapRangeType(mapType));
  mappedSubresource.rowPitch   = 0;
  mappedSubresource.depthPitch = 0;

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 buffer
				glBindBuffer(target, static_cast<GLuint>(openGLES3BufferBackup));
#endif

  // Done
  RHI_ASSERT(nullptr != mappedSubresource.data, "Mapping of OpenGL ES 3 buffer failed")
  return (nullptr != mappedSubresource.data);
}

void unmapBuffer(GLenum target, GLenum bindingTarget, GLuint openGLES3Buffer)
{
#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently bound OpenGL ES 3 buffer
				GLint openGLES3BufferBackup = 0;
				glGetIntegerv(bindingTarget, &openGLES3BufferBackup);
#endif

  // Bind this OpenGL ES 3 buffer
  glBindBuffer(target, openGLES3Buffer);

  // Unmap
  glUnmapBuffer(target);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 buffer
				glBindBuffer(target, static_cast<GLuint>(openGLES3BufferBackup));
#endif
}

namespace ImplementationDispatch
{


//[-------------------------------------------------------]
//[ Command buffer                                        ]
//[-------------------------------------------------------]
void DispatchCommandBuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DispatchCommandBuffer* realData = static_cast<const RERHI::Command::DispatchCommandBuffer*>(data);
  RHI_ASSERT(nullptr != realData->commandBufferToDispatch, "The OpenGL ES 3 command buffer to dispatch must be valid")
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).dispatchCommandBufferInternal(*realData->commandBufferToDispatch);
}

//[-------------------------------------------------------]
//[ Graphics                                              ]
//[-------------------------------------------------------]
void SetGraphicsRootSignature(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsRootSignature* realData = static_cast<const RERHI::Command::SetGraphicsRootSignature*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setGraphicsRootSignature(realData->rootSignature);
}

void SetGraphicsPipelineState(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsPipelineState* realData = static_cast<const RERHI::Command::SetGraphicsPipelineState*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setGraphicsPipelineState(realData->graphicsPipelineState);
}

void SetGraphicsResourceGroup(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsResourceGroup* realData = static_cast<const RERHI::Command::SetGraphicsResourceGroup*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setGraphicsResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
}

void SetGraphicsVertexArray(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Input-assembler (IA) stage
  const RERHI::Command::SetGraphicsVertexArray* realData = static_cast<const RERHI::Command::SetGraphicsVertexArray*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setGraphicsVertexArray(realData->vertexArray);
}

void SetGraphicsViewports(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsViewports* realData = static_cast<const RERHI::Command::SetGraphicsViewports*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setGraphicsViewports(realData->numberOfViewports, (nullptr != realData->viewports) ? realData->viewports : reinterpret_cast<const RERHI::Viewport*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsScissorRectangles(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsScissorRectangles* realData = static_cast<const RERHI::Command::SetGraphicsScissorRectangles*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setGraphicsScissorRectangles(realData->numberOfScissorRectangles, (nullptr != realData->scissorRectangles) ? realData->scissorRectangles : reinterpret_cast<const RERHI::ScissorRectangle*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsRenderTarget(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Output-merger (OM) stage
  const RERHI::Command::SetGraphicsRenderTarget* realData = static_cast<const RERHI::Command::SetGraphicsRenderTarget*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setGraphicsRenderTarget(realData->renderTarget);
}

void ClearGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ClearGraphics* realData = static_cast<const RERHI::Command::ClearGraphics*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).clearGraphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
}

void DrawGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawGraphics* realData = static_cast<const RERHI::Command::DrawGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    // No resource owner security check in here, we only support emulated indirect buffer
    static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).drawGraphicsEmulated(realData->indirectBuffer->getEmulationData(), realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).drawGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawIndexedGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawIndexedGraphics* realData = static_cast<const RERHI::Command::DrawIndexedGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    // No resource owner security check in here, we only support emulated indirect buffer
    static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).drawIndexedGraphicsEmulated(realData->indirectBuffer->getEmulationData(), realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).drawIndexedGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawMeshTasks(const void*, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  RHI_ASSERT(static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).getContext(), false, "OpenGL ES 3 doesn't support mesh shaders")
}

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void SetComputeRootSignature(const void*, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  RHI_ASSERT(static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).getContext(), false, "OpenGL ES 3 doesn't support compute root signature")
}

void SetComputePipelineState(const void*, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  RHI_ASSERT(static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).getContext(), false, "OpenGL ES 3 doesn't support compute pipeline state")
}

void SetComputeResourceGroup(const void*, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  RHI_ASSERT(static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).getContext(), false, "OpenGL ES 3 doesn't support compute resource group")
}

void DispatchCompute(const void*, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  RHI_ASSERT(static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).getContext(), false, "OpenGL ES 3 doesn't support compute dispatch")
}

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void SetTextureMinimumMaximumMipmapIndex(const void* data, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const RERHI::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
  RHI_ASSERT(static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).getContext(), realData->texture->getResourceType() == RERHI::ResourceType::TEXTURE_2D, "Unsupported OpenGL ES 3 texture resource type")
  static_cast<RERHIOpenGLES3::Texture2D*>(realData->texture)->setMinimumMaximumMipmapIndex(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
}

void ResolveMultisampleFramebuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResolveMultisampleFramebuffer* realData = static_cast<const RERHI::Command::ResolveMultisampleFramebuffer*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).resolveMultisampleFramebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
}

void CopyResource(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::CopyResource* realData = static_cast<const RERHI::Command::CopyResource*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).copyResource(*realData->destinationResource, *realData->sourceResource);
}

void GenerateMipmaps(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::GenerateMipmaps* realData = static_cast<const RERHI::Command::GenerateMipmaps*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).generateMipmaps(*realData->resource);
}

void CopyUniformBufferData(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::CopyUniformBufferData* realData = static_cast<const RERHI::Command::CopyUniformBufferData*>(data);
  RERHI::MappedSubresource mappedSubresource;
  if (rhi.map(*realData->uniformBuffer, 0, RERHI::MapType::WRITE_DISCARD, 0, mappedSubresource))
  {
    memcpy(mappedSubresource.data, RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->numberOfBytes);
    rhi.unmap(*realData->uniformBuffer, 0);
  }
}

void SetUniform(const void* data, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetUniform* realData = static_cast<const RERHI::Command::SetUniform*>(data);
  switch (realData->type)
  {
    case RERHI::Command::SetUniform::Type::UNIFORM_1I:
      static_cast<RERHIOpenGLES3::GraphicsProgramGlsl*>(realData->graphicsProgram)->setUniform1i(realData->uniformHandle, *reinterpret_cast<const int*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
      break;

    case RERHI::Command::SetUniform::Type::UNIFORM_1F:
      static_cast<RERHIOpenGLES3::GraphicsProgramGlsl*>(realData->graphicsProgram)->setUniform1f(realData->uniformHandle, *reinterpret_cast<const float*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
      break;

    case RERHI::Command::SetUniform::Type::UNIFORM_2FV:
      static_cast<RERHIOpenGLES3::GraphicsProgramGlsl*>(realData->graphicsProgram)->setUniform2fv(realData->uniformHandle, reinterpret_cast<const float*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
      break;

    case RERHI::Command::SetUniform::Type::UNIFORM_3FV:
      static_cast<RERHIOpenGLES3::GraphicsProgramGlsl*>(realData->graphicsProgram)->setUniform3fv(realData->uniformHandle, reinterpret_cast<const float*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
      break;

    case RERHI::Command::SetUniform::Type::UNIFORM_4FV:
      static_cast<RERHIOpenGLES3::GraphicsProgramGlsl*>(realData->graphicsProgram)->setUniform4fv(realData->uniformHandle, reinterpret_cast<const float*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
      break;

    case RERHI::Command::SetUniform::Type::UNIFORM_MATRIX_3FV:
      static_cast<RERHIOpenGLES3::GraphicsProgramGlsl*>(realData->graphicsProgram)->setUniformMatrix3fv(realData->uniformHandle, reinterpret_cast<const float*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
      break;

    case RERHI::Command::SetUniform::Type::UNIFORM_MATRIX_4FV:
      static_cast<RERHIOpenGLES3::GraphicsProgramGlsl*>(realData->graphicsProgram)->setUniformMatrix4fv(realData->uniformHandle, reinterpret_cast<const float*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
      break;

    default:
      RHI_ASSERT(false, "Invalid set uniform type inside the OpenGLES 3 RHI implementation")
      break;
  }
}

//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void ResetQueryPool(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResetQueryPool* realData = static_cast<const RERHI::Command::ResetQueryPool*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).resetQueryPool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
}

void BeginQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::BeginQuery* realData = static_cast<const RERHI::Command::BeginQuery*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).beginQuery(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
}

void EndQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::EndQuery* realData = static_cast<const RERHI::Command::EndQuery*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).endQuery(*realData->queryPool, realData->queryIndex);
}

void WriteTimestampQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::WriteTimestampQuery* realData = static_cast<const RERHI::Command::WriteTimestampQuery*>(data);
  static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).writeTimestampQuery(*realData->queryPool, realData->queryIndex);
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void SetDebugMarker(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::SetDebugMarker* realData = static_cast<const RERHI::Command::SetDebugMarker*>(data);
					static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).setDebugMarker(realData->name);
				}

				void BeginDebugEvent(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::BeginDebugEvent* realData = static_cast<const RERHI::Command::BeginDebugEvent*>(data);
					static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).beginDebugEvent(realData->name);
				}

				void EndDebugEvent(const void*, RERHI::RHIDynamicRHI& rhi)
				{
					static_cast<RERHIOpenGLES3::RHIDynamicRHI&>(rhi).endDebugEvent();
				}
#else
void SetDebugMarker(const void*, RERHI::RHIDynamicRHI&)
{
  // Nothing here
}

void BeginDebugEvent(const void*, RERHI::RHIDynamicRHI&)
{
  // Nothing here
}

void EndDebugEvent(const void*, RERHI::RHIDynamicRHI&)
{
  // Nothing here
}
#endif


}


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
static constexpr RERHI::ImplementationDispatchFunction DISPATCH_FUNCTIONS[static_cast<RECore::uint8>(RERHI::CommandDispatchFunctionIndex::NUMBER_OF_FUNCTIONS)] =
  {
    // Command buffer
    &ImplementationDispatch::DispatchCommandBuffer,
    // Graphics
    &ImplementationDispatch::SetGraphicsRootSignature,
    &ImplementationDispatch::SetGraphicsPipelineState,
    &ImplementationDispatch::SetGraphicsResourceGroup,
    &ImplementationDispatch::SetGraphicsVertexArray,		// Input-assembler (IA) stage
    &ImplementationDispatch::SetGraphicsViewports,			// Rasterizer (RS) stage
    &ImplementationDispatch::SetGraphicsScissorRectangles,	// Rasterizer (RS) stage
    &ImplementationDispatch::SetGraphicsRenderTarget,		// Output-merger (OM) stage
    &ImplementationDispatch::ClearGraphics,
    &ImplementationDispatch::DrawGraphics,
    &ImplementationDispatch::DrawIndexedGraphics,
    &ImplementationDispatch::DrawMeshTasks,
    // Compute
    &ImplementationDispatch::SetComputeRootSignature,
    &ImplementationDispatch::SetComputePipelineState,
    &ImplementationDispatch::SetComputeResourceGroup,
    &ImplementationDispatch::DispatchCompute,
    // Resource
    &ImplementationDispatch::SetTextureMinimumMaximumMipmapIndex,
    &ImplementationDispatch::ResolveMultisampleFramebuffer,
    &ImplementationDispatch::CopyResource,
    &ImplementationDispatch::GenerateMipmaps,
    &ImplementationDispatch::CopyUniformBufferData,
    &ImplementationDispatch::SetUniform,
    // Query
    &ImplementationDispatch::ResetQueryPool,
    &ImplementationDispatch::BeginQuery,
    &ImplementationDispatch::EndQuery,
    &ImplementationDispatch::WriteTimestampQuery,
    // Debug
    &ImplementationDispatch::SetDebugMarker,
    &ImplementationDispatch::BeginDebugEvent,
    &ImplementationDispatch::EndDebugEvent
  };


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIDynamicRHI::RHIDynamicRHI(const RERHI::RHIContext& context) :
  RERHI::RHIDynamicRHI(RERHI::NameId::OPENGLES3, context),
  VertexArrayMakeId(),
  GraphicsPipelineStateMakeId(),
  mOpenGLES3Context(nullptr),
  mShaderLanguageGlsl(nullptr),
  mGraphicsRootSignature(nullptr),
  mDefaultSamplerState(nullptr),
  mOpenGLES3CopyResourceFramebuffer(0),
  mDefaultOpenGLES3VertexArray(0),
  // States
  mGraphicsPipelineState(nullptr),
  // Input-assembler (IA) stage
  mVertexArray(nullptr),
  mOpenGLES3PrimitiveTopology(0xFFFF),	// Unknown default setting
  // Output-merger (OM) stage
  mRenderTarget(nullptr),
  // State cache to avoid making redundant OpenGL ES 3 calls
  mOpenGLES3ClipControlOrigin(GL_INVALID_ENUM),
  mOpenGLES3Program(0),
  // Draw ID uniform location for "GL_EXT_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
  mDrawIdUniformLocation(-1),
  mCurrentStartInstanceLocation(~0u)
{
  // Initialize the OpenGL ES 3 context
  mOpenGLES3Context = RHI_NEW(mContext, OpenGLES3ContextRuntimeLinking)(*this, context.getNativeWindowHandle(), context.isUsingExternalContext());
  if (mOpenGLES3Context->initialize(0))
  {
#ifdef DEBUG
    // "GL_KHR_debug"-extension available?
				if (mOpenGLES3Context->getExtensions().isGL_KHR_debug())
				{
					// Synchronous debug output, please
					// -> Makes it easier to find the place causing the issue
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);

					// Disable severity notifications, most drivers print many things with this severity
					glDebugMessageControlKHR(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION_KHR, 0, 0, false);

					// Set the debug message callback function
					glDebugMessageCallbackKHR(&RHIDynamicRHI::debugMessageCallback, this);
				}
#endif

    // Initialize the capabilities
    initializeCapabilities();

    // Create the default sampler state
    mDefaultSamplerState = createSamplerState(RERHI::RHISamplerState::getDefaultSamplerState());

    // Create default OpenGL ES 3 vertex array
    glGenVertexArrays(1, &mDefaultOpenGLES3VertexArray);
    glBindVertexArray(mDefaultOpenGLES3VertexArray);

    // Add references to the default sampler state and set it
    if (nullptr != mDefaultSamplerState)
    {
      mDefaultSamplerState->AddReference();
      // TODO(naetherm) Set default sampler states
    }
  }
}

RHIDynamicRHI::~RHIDynamicRHI()
{
  // Set no graphics pipeline state reference, in case we have one
  if (nullptr != mGraphicsPipelineState)
  {
    setGraphicsPipelineState(nullptr);
  }

  // Release instances
  if (nullptr != mRenderTarget)
  {
    mRenderTarget->Release();
    mRenderTarget = nullptr;
  }
  if (nullptr != mDefaultSamplerState)
  {
    mDefaultSamplerState->Release();
    mDefaultSamplerState = nullptr;
  }

  // Destroy the OpenGL ES 3 framebuffer used by "RHIDynamicRHI::RHIDynamicRHI::copyResource()"
  // -> Silently ignores 0's and names that do not correspond to existing buffer objects
  // -> Null test in here only to handle the situation of OpenGL ES 3 initialization failure, meaning "glDeleteFramebuffers" itself is a null pointer
  if (0 != mOpenGLES3CopyResourceFramebuffer)
  {
    glDeleteFramebuffers(1, &mOpenGLES3CopyResourceFramebuffer);
  }

  // Set no vertex array reference, in case we have one
  if (nullptr != mVertexArray)
  {
    setGraphicsVertexArray(nullptr);
  }

  // Destroy the OpenGL ES 3 default vertex array
  // -> Silently ignores 0's and names that do not correspond to existing vertex array objects
  glDeleteVertexArrays(1, &mDefaultOpenGLES3VertexArray);

  // Release the graphics root signature instance, in case we have one
  if (nullptr != mGraphicsRootSignature)
  {
    mGraphicsRootSignature->Release();
  }

#ifdef RHI_STATISTICS
  { // For debugging: At this point there should be no resource instances left, validate this!
			// -> Are the currently any resource instances?
			const RECore::uint32 numberOfCurrentResources = getStatistics().getNumberOfCurrentResources();
			if (numberOfCurrentResources > 0)
			{
				// Error!
				if (numberOfCurrentResources > 1)
				{
					RHI_ASSERT(false, "The OpenGL ES 3 RHI implementation is going to be destroyed, but there are still %lu resource instances left (memory leak)", numberOfCurrentResources)
				}
				else
				{
					RHI_ASSERT(false, "The OpenGL ES 3 RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
				}

				// Use debug output to show the current number of resource instances
				getStatistics().debugOutputCurrentResouces(mContext);
			}
		}
#endif

  // Release the GLSL shader language instance, in case we have one
  if (nullptr != mShaderLanguageGlsl)
  {
    mShaderLanguageGlsl->Release();
  }

  // Destroy the OpenGL ES 3 context instance
  RHI_DELETE(mContext, IOpenGLES3Context, mOpenGLES3Context);
}

void RHIDynamicRHI::dispatchCommandBufferInternal(const RERHI::RHICommandBuffer& commandBuffer)
{
  // Loop through all commands
  const RECore::uint8* commandPacketBuffer = commandBuffer.getCommandPacketBuffer();
  RERHI::ConstCommandPacket constCommandPacket = commandPacketBuffer;
  while (nullptr != constCommandPacket)
  {
    { // Dispatch command packet
      const RERHI::CommandDispatchFunctionIndex commandDispatchFunctionIndex = RERHI::CommandPacketHelper::loadCommandDispatchFunctionIndex(constCommandPacket);
      const void* command = RERHI::CommandPacketHelper::loadCommand(constCommandPacket);
      detail::DISPATCH_FUNCTIONS[static_cast<RECore::uint32>(commandDispatchFunctionIndex)](command, *this);
    }

    { // Next command
      const RECore::uint32 nextCommandPacketByteIndex = RERHI::CommandPacketHelper::getNextCommandPacketByteIndex(constCommandPacket);
      constCommandPacket = (~0u != nextCommandPacketByteIndex) ? &commandPacketBuffer[nextCommandPacketByteIndex] : nullptr;
    }
  }
}


//[-------------------------------------------------------]
//[ Graphics states                                       ]
//[-------------------------------------------------------]
void RHIDynamicRHI::setGraphicsRootSignature(RERHI::RHIRootSignature* rootSignature)
{
  if (nullptr != mGraphicsRootSignature)
  {
    mGraphicsRootSignature->Release();
  }
  mGraphicsRootSignature = static_cast<RootSignature*>(rootSignature);
  if (nullptr != mGraphicsRootSignature)
  {
    mGraphicsRootSignature->AddReference();

    // Sanity check
    RHI_MATCH_CHECK(*this, *rootSignature)
  }
}

void RHIDynamicRHI::setGraphicsPipelineState(RERHI::RHIGraphicsPipelineState* graphicsPipelineState)
{
  if (mGraphicsPipelineState != graphicsPipelineState)
  {
    if (nullptr != graphicsPipelineState)
    {
      // Sanity check
      RHI_MATCH_CHECK(*this, *graphicsPipelineState)

      // Set new graphics pipeline state and add a reference to it
      if (nullptr != mGraphicsPipelineState)
      {
        mGraphicsPipelineState->Release();
      }
      mGraphicsPipelineState = static_cast<GraphicsPipelineState*>(graphicsPipelineState);
      mGraphicsPipelineState->AddReference();

      // Set graphics pipeline state
      mOpenGLES3PrimitiveTopology = mGraphicsPipelineState->getOpenGLES3PrimitiveTopology();
      mGraphicsPipelineState->bindGraphicsPipelineState();
    }
    else if (nullptr != mGraphicsPipelineState)
    {
      // TODO(naetherm) Handle this situation by resetting OpenGL states?
      mGraphicsPipelineState->Release();
      mGraphicsPipelineState = nullptr;
    }
  }
}

void RHIDynamicRHI::setGraphicsResourceGroup(RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  // Security checks
#ifdef DEBUG
  {
			RHI_ASSERT(nullptr != mGraphicsRootSignature, "No OpenGL ES 3 RHI implementation graphics root signature set")
			const RERHI::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
			RHI_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The OpenGL ES 3 RHI implementation root parameter index is out of bounds")
			const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			RHI_ASSERT(RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The OpenGL ES 3 RHI implementation root parameter index doesn't reference a descriptor table")
			RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The OpenGL ES 3 RHI implementation descriptor ranges is a null pointer")
		}
#endif

  if (nullptr != resourceGroup)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *resourceGroup)

    // Set graphics resource group
    const ResourceGroup* openGLES3ResourceGroup = static_cast<ResourceGroup*>(resourceGroup);
    const RECore::uint32 numberOfResources = openGLES3ResourceGroup->getNumberOfResources();
    RERHI::RHIResource** resources = openGLES3ResourceGroup->getResources();
    const RERHI::RootParameter& rootParameter = mGraphicsRootSignature->getRootSignature().parameters[rootParameterIndex];
    for (RECore::uint32 resourceIndex = 0; resourceIndex < numberOfResources; ++resourceIndex, ++resources)
    {
      RERHI::RHIResource* resource = *resources;
      RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL ES 3 descriptor ranges")
      const RERHI::DescriptorRange& descriptorRange = reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];

      // Check the type of resource to set
      // TODO(naetherm) Some additional resource type root signature security checks in debug build?
      const RERHI::ResourceType resourceType = resource->getResourceType();
      switch (resourceType)
      {
        case RERHI::ResourceType::TEXTURE_BUFFER:
          if (mOpenGLES3Context->getExtensions().isGL_EXT_texture_buffer())
          {
            // Fall through by intent
          }
          else
          {
            // We can only emulate the "RERHI::TextureFormat::R32G32B32A32F" texture format using an uniform buffer

            // Attach the buffer to the given UBO binding point
            // -> Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
            // -> Direct3D 10 and Direct3D 11 have explicit binding points
            RHI_ASSERT(nullptr != openGLES3ResourceGroup->getResourceIndexToUniformBlockBindingIndex(), "Invalid OpenGL ES 3 resource index to uniform block binding index")
            glBindBufferBase(GL_UNIFORM_BUFFER, openGLES3ResourceGroup->getResourceIndexToUniformBlockBindingIndex()[resourceIndex], static_cast<TextureBuffer*>(resource)->getOpenGLES3TextureBuffer());
            break;
          }

        case RERHI::ResourceType::STRUCTURED_BUFFER:
          // TODO(naetherm) Add OpenGL ES structured buffer support ("GL_EXT_buffer_storage"-extension)
          break;

        case RERHI::ResourceType::UNIFORM_BUFFER:
          // Attach the buffer to the given UBO binding point
          // -> Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
          // -> Direct3D 10 and Direct3D 11 have explicit binding points
          RHI_ASSERT(nullptr != openGLES3ResourceGroup->getResourceIndexToUniformBlockBindingIndex(), "Invalid OpenGL ES 3 resource index to uniform block binding index")
          glBindBufferBase(GL_UNIFORM_BUFFER, openGLES3ResourceGroup->getResourceIndexToUniformBlockBindingIndex()[resourceIndex], static_cast<UniformBuffer*>(resource)->getOpenGLES3UniformBuffer());
          break;

        case RERHI::ResourceType::TEXTURE_1D:
        case RERHI::ResourceType::TEXTURE_1D_ARRAY:
        case RERHI::ResourceType::TEXTURE_2D:
        case RERHI::ResourceType::TEXTURE_2D_ARRAY:
        case RERHI::ResourceType::TEXTURE_3D:
        case RERHI::ResourceType::TEXTURE_CUBE:
        case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
        {
          switch (descriptorRange.shaderVisibility)
          {
            // In OpenGL ES 3, all shaders share the same texture units
            case RERHI::ShaderVisibility::ALL:
            case RERHI::ShaderVisibility::ALL_GRAPHICS:
            case RERHI::ShaderVisibility::VERTEX:
            case RERHI::ShaderVisibility::FRAGMENT:
            {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
              // Backup the currently active OpenGL ES 3 texture
									GLint openGLES3ActiveTextureBackup = 0;
									glGetIntegerv(GL_ACTIVE_TEXTURE, &openGLES3ActiveTextureBackup);
#endif

              // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
              glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + descriptorRange.baseShaderRegister));

              // Bind texture or texture buffer
              if (RERHI::ResourceType::TEXTURE_BUFFER == resourceType)
              {
                glBindTexture(GL_TEXTURE_BUFFER_EXT, static_cast<TextureBuffer*>(resource)->getOpenGLES3Texture());
              }
              else if (RERHI::ResourceType::TEXTURE_1D == resourceType)
              {
                // OpenGL ES 3 has no 1D textures, just use a 2D texture with a height of one
                glBindTexture(GL_TEXTURE_2D, static_cast<Texture1D*>(resource)->getOpenGLES3Texture());
              }
              else if (RERHI::ResourceType::TEXTURE_1D_ARRAY == resourceType)
              {
                // OpenGL ES 3 has no 1D textures, just use a 2D texture with a height of one
                // No extension check required, if we in here we already know it must exist
                glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<Texture1DArray*>(resource)->getOpenGLES3Texture());
              }
              else if (RERHI::ResourceType::TEXTURE_2D_ARRAY == resourceType)
              {
                // No extension check required, if we in here we already know it must exist
                glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<Texture2DArray*>(resource)->getOpenGLES3Texture());
              }
              else if (RERHI::ResourceType::TEXTURE_3D == resourceType)
              {
                // No extension check required, if we in here we already know it must exist
                glBindTexture(GL_TEXTURE_3D, static_cast<Texture3D*>(resource)->getOpenGLES3Texture());
              }
              else if (RERHI::ResourceType::TEXTURE_CUBE == resourceType)
              {
                // No extension check required, if we in here we already know it must exist
                glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<TextureCube*>(resource)->getOpenGLES3Texture());
              }
              else if (RERHI::ResourceType::TEXTURE_CUBE_ARRAY == resourceType)
              {
                // No extension check required, if we in here we already know it must exist
                // TODO(naetherm) Implement me
                // glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<TextureCubeArray*>(resource)->getOpenGLES3Texture());
              }
              else
              {
                glBindTexture(GL_TEXTURE_2D, static_cast<Texture2D*>(resource)->getOpenGLES3Texture());
              }

              // Set the OpenGL ES 3 sampler states, if required (texture buffer has no sampler state), it's valid that there's no sampler state (e.g. texel fetch instead of sampling might be used)
              if (RERHI::ResourceType::TEXTURE_BUFFER != resourceType)
              {
                RHI_ASSERT(nullptr != openGLES3ResourceGroup->getSamplerState(), "Invalid OpenGL ES 3 sampler state")
                const SamplerState* samplerState = static_cast<const SamplerState*>(openGLES3ResourceGroup->getSamplerState()[resourceIndex]);
                if (nullptr != samplerState)
                {
                  // Traditional bind version to emulate a sampler object
                  samplerState->setOpenGLES3SamplerStates();
                }
              }

#ifdef RHI_OPENGLES3_STATE_CLEANUP
              // Be polite and restore the previous active OpenGL ES 3 texture
									glActiveTexture(static_cast<GLuint>(openGLES3ActiveTextureBackup));
#endif
              break;
            }

            case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
              RHI_ASSERT(false, "OpenGL ES 3 has no tessellation control shader support (hull shader in Direct3D terminology)")
              break;

            case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
              RHI_ASSERT(false, "OpenGL ES 3 has no tessellation evaluation shader support (domain shader in Direct3D terminology)")
              break;

            case RERHI::ShaderVisibility::GEOMETRY:
              RHI_ASSERT(false, "OpenGL ES 3 has no geometry shader support")
              break;

            case RERHI::ShaderVisibility::TASK:
              RHI_ASSERT(false, "OpenGL ES 3 has no task shader support")
              break;

            case RERHI::ShaderVisibility::MESH:
              RHI_ASSERT(false, "OpenGL ES 3 has no mesh shader support")
              break;

            case RERHI::ShaderVisibility::COMPUTE:
              RHI_ASSERT(false, "OpenGL ES 3 has no compute shader support")
              break;
          }
          break;
        }

        case RERHI::ResourceType::SAMPLER_STATE:
          // Unlike Direct3D >=10, OpenGL ES 3 directly attaches the sampler settings to the texture
          break;

        case RERHI::ResourceType::ROOT_SIGNATURE:
        case RERHI::ResourceType::RESOURCE_GROUP:
        case RERHI::ResourceType::GRAPHICS_PROGRAM:
        case RERHI::ResourceType::VERTEX_ARRAY:
        case RERHI::ResourceType::RENDER_PASS:
        case RERHI::ResourceType::QUERY_POOL:
        case RERHI::ResourceType::SWAP_CHAIN:
        case RERHI::ResourceType::FRAMEBUFFER:
        case RERHI::ResourceType::VERTEX_BUFFER:
        case RERHI::ResourceType::INDEX_BUFFER:
        case RERHI::ResourceType::INDIRECT_BUFFER:
        case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
        case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
        case RERHI::ResourceType::VERTEX_SHADER:
        case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
        case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case RERHI::ResourceType::GEOMETRY_SHADER:
        case RERHI::ResourceType::FRAGMENT_SHADER:
        case RERHI::ResourceType::TASK_SHADER:
        case RERHI::ResourceType::MESH_SHADER:
        case RERHI::ResourceType::COMPUTE_SHADER:
          RHI_ASSERT(false, "Invalid OpenGL ES 3 RHI implementation resource type")
          break;
      }
    }
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::setGraphicsVertexArray(RERHI::RHIVertexArray* vertexArray)
{
  // Input-assembler (IA) stage

  // New vertex array?
  if (mVertexArray != vertexArray)
  {
    // Set a vertex array?
    if (nullptr != vertexArray)
    {
      // Sanity check
      RHI_MATCH_CHECK(*this, *vertexArray)

      // Release the vertex array reference, in case we have one
      if (nullptr != mVertexArray)
      {
        // Release reference
        mVertexArray->Release();
      }

      // Set new vertex array and add a reference to it
      mVertexArray = static_cast<VertexArray*>(vertexArray);
      mVertexArray->AddReference();

      // Bind OpenGL ES 3 vertex array
      glBindVertexArray(static_cast<VertexArray*>(mVertexArray)->getOpenGLES3VertexArray());
    }

      // Release the vertex array reference, in case we have one
    else if (nullptr != mVertexArray)
    {
      // Unbind OpenGL ES 3 vertex array
      glBindVertexArray(mDefaultOpenGLES3VertexArray);

      // Release reference
      mVertexArray->Release();
      mVertexArray = nullptr;
    }
  }
}

void RHIDynamicRHI::setGraphicsViewports([[maybe_unused]] RECore::uint32 numberOfViewports, const RERHI::Viewport* viewports)
{
  // Rasterizer (RS) stage

  // Sanity check
  RHI_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid OpenGL ES 3 rasterizer state viewports")

  // In OpenGL ES 3, the origin of the viewport is left bottom while Direct3D is using a left top origin. To make the
  // Direct3D 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
  // -> This isn't influenced by the "GL_EXT_clip_control"-extension

  // Get the width and height of the current render target
  RECore::uint32 renderTargetHeight = 1;
  if (nullptr != mRenderTarget)
  {
    RECore::uint32 renderTargetWidth = 1;
    mRenderTarget->getWidthAndHeight(renderTargetWidth, renderTargetHeight);
  }

  // Set the OpenGL ES 3 viewport
  // -> OpenGL ES 3 supports only one viewport
  RHI_ASSERT(numberOfViewports <= 1, "OpenGL ES 3 supports only one viewport")
  glViewport(static_cast<GLint>(viewports->topLeftX), static_cast<GLint>(static_cast<float>(renderTargetHeight) - viewports->topLeftY - viewports->height), static_cast<GLsizei>(viewports->width), static_cast<GLsizei>(viewports->height));
  glDepthRangef(static_cast<GLclampf>(viewports->minDepth), static_cast<GLclampf>(viewports->maxDepth));
}

void RHIDynamicRHI::setGraphicsScissorRectangles([[maybe_unused]] RECore::uint32 numberOfScissorRectangles, const RERHI::ScissorRectangle* scissorRectangles)
{
  // Rasterizer (RS) stage

  // Sanity check
  RHI_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid OpenGL ES 3 rasterizer state scissor rectangles")

  // In OpenGL ES 3, the origin of the scissor rectangle is left bottom while Direct3D is using a left top origin. To make the
  // Direct3D 9 & 10 & 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
  // -> This isn't influenced by the "GL_EXT_clip_control"-extension

  // Get the width and height of the current render target
  RECore::uint32 renderTargetHeight = 1;
  if (nullptr != mRenderTarget)
  {
    RECore::uint32 renderTargetWidth = 1;
    mRenderTarget->getWidthAndHeight(renderTargetWidth, renderTargetHeight);
  }

  // Set the OpenGL ES 3 scissor rectangle
  RHI_ASSERT(numberOfScissorRectangles <= 1, "OpenGL ES 3 supports only one scissor rectangle")
  const GLsizei width  = scissorRectangles->bottomRightX - scissorRectangles->topLeftX;
  const GLsizei height = scissorRectangles->bottomRightY - scissorRectangles->topLeftY;
  glScissor(static_cast<GLint>(scissorRectangles->topLeftX), static_cast<GLint>(renderTargetHeight - scissorRectangles->topLeftY - height), width, height);
}

void RHIDynamicRHI::setGraphicsRenderTarget(RERHI::RHIRenderTarget* renderTarget)
{
  // Output-merger (OM) stage

  // New render target?
  if (mRenderTarget != renderTarget)
  {
    // Set a render target?
    if (nullptr != renderTarget)
    {
      // Sanity check
      RHI_MATCH_CHECK(*this, *renderTarget)

      // Release the render target reference, in case we have one
      if (nullptr != mRenderTarget)
      {
        // Unbind OpenGL ES 3 framebuffer?
        if (RERHI::ResourceType::FRAMEBUFFER == mRenderTarget->getResourceType() && RERHI::ResourceType::FRAMEBUFFER != renderTarget->getResourceType())
        {
          // We do not render into a OpenGL ES 3 framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // Release
        mRenderTarget->Release();
      }

      // Set new render target and add a reference to it
      mRenderTarget = renderTarget;
      mRenderTarget->AddReference();

      // Evaluate the render target type
      GLenum clipControlOrigin = GL_UPPER_LEFT_EXT;
      switch (mRenderTarget->getResourceType())
      {
        case RERHI::ResourceType::SWAP_CHAIN:
        {
          clipControlOrigin = GL_LOWER_LEFT_EXT;	// Compensate OS window coordinate system y-flip
          // TODO(naetherm) Implement me
          break;
        }

        case RERHI::ResourceType::FRAMEBUFFER:
        {
          // Get the OpenGL ES 3 framebuffer instance
          Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);

          // Bind the OpenGL ES 3 framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getOpenGLES3Framebuffer());

          // Define the OpenGL buffers to draw into
          {
            // https://www.opengl.org/registry/specs/ARB/draw_buffers.txt - "The draw buffer for output colors beyond <n> is set to NONE."
            // -> Meaning depth only rendering which has no color textures at all will work as well, no need for "glDrawBuffer(GL_NONE)"
            // -> https://www.khronos.org/opengles/sdk/docs/man3/html/glDrawBuffers.xhtml for OpenGL ES 3 specifies the same behaviour
            // -> "GL_COLOR_ATTACHMENT0" and "GL_COLOR_ATTACHMENT0_NV" have the same value
            static constexpr GLenum OPENGL_DRAW_BUFFER[16] =
              {
                GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,  GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,
                GL_COLOR_ATTACHMENT8,  GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
                GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15
              };
            glDrawBuffers(static_cast<GLsizei>(framebuffer->getNumberOfColorTextures()), OPENGL_DRAW_BUFFER);
          }
          break;
        }

        case RERHI::ResourceType::ROOT_SIGNATURE:
        case RERHI::ResourceType::RESOURCE_GROUP:
        case RERHI::ResourceType::GRAPHICS_PROGRAM:
        case RERHI::ResourceType::VERTEX_ARRAY:
        case RERHI::ResourceType::RENDER_PASS:
        case RERHI::ResourceType::QUERY_POOL:
        case RERHI::ResourceType::VERTEX_BUFFER:
        case RERHI::ResourceType::INDEX_BUFFER:
        case RERHI::ResourceType::TEXTURE_BUFFER:
        case RERHI::ResourceType::STRUCTURED_BUFFER:
        case RERHI::ResourceType::INDIRECT_BUFFER:
        case RERHI::ResourceType::UNIFORM_BUFFER:
        case RERHI::ResourceType::TEXTURE_1D:
        case RERHI::ResourceType::TEXTURE_1D_ARRAY:
        case RERHI::ResourceType::TEXTURE_2D:
        case RERHI::ResourceType::TEXTURE_2D_ARRAY:
        case RERHI::ResourceType::TEXTURE_3D:
        case RERHI::ResourceType::TEXTURE_CUBE:
        case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
        case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
        case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
        case RERHI::ResourceType::SAMPLER_STATE:
        case RERHI::ResourceType::VERTEX_SHADER:
        case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
        case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case RERHI::ResourceType::GEOMETRY_SHADER:
        case RERHI::ResourceType::FRAGMENT_SHADER:
        case RERHI::ResourceType::TASK_SHADER:
        case RERHI::ResourceType::MESH_SHADER:
        case RERHI::ResourceType::COMPUTE_SHADER:
        default:
          // Not handled in here
          break;
      }

      // Setup clip control
      if (mOpenGLES3ClipControlOrigin != clipControlOrigin && mOpenGLES3Context->getExtensions().isGL_EXT_clip_control())
      {
        // OpenGL ES 3 default is "GL_LOWER_LEFT_EXT" and "GL_NEGATIVE_ONE_TO_ONE_EXT", change it to match Vulkan and Direct3D
        mOpenGLES3ClipControlOrigin = clipControlOrigin;
        glClipControlEXT(mOpenGLES3ClipControlOrigin, GL_ZERO_TO_ONE_EXT);
      }
    }
    else if (nullptr != mRenderTarget)
    {
      // Evaluate the render target type
      if (RERHI::ResourceType::FRAMEBUFFER == mRenderTarget->getResourceType())
      {
        // We do not render into a OpenGL ES 3 framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }

      // TODO(naetherm) Set no active render target

      // Release the render target reference, in case we have one
      mRenderTarget->Release();
      mRenderTarget = nullptr;
    }
  }
}

void RHIDynamicRHI::clearGraphics(RECore::uint32 clearFlags, const float color[4], float z, RECore::uint32 stencil)
{
  // Sanity check
  RHI_ASSERT(z >= 0.0f && z <= 1.0f, "The OpenGL ES 3 clear graphics z value must be between [0, 1] (inclusive)")

  // Get API flags
  RECore::uint32 flagsApi = 0;
  if (clearFlags & RERHI::ClearFlag::COLOR)
  {
    flagsApi |= GL_COLOR_BUFFER_BIT;
  }
  if (clearFlags & RERHI::ClearFlag::DEPTH)
  {
    flagsApi |= GL_DEPTH_BUFFER_BIT;
  }
  if (clearFlags & RERHI::ClearFlag::STENCIL)
  {
    flagsApi |= GL_STENCIL_BUFFER_BIT;
  }

  // Are API flags set?
  if (0 != flagsApi)
  {
    // Set clear settings
    if (clearFlags & RERHI::ClearFlag::COLOR)
    {
      glClearColor(color[0], color[1], color[2], color[3]);
    }
    if (clearFlags & RERHI::ClearFlag::DEPTH)
    {
      glClearDepthf(z);
      if (nullptr != mGraphicsPipelineState && RERHI::DepthWriteMask::ALL != mGraphicsPipelineState->getDepthStencilState().depthWriteMask)
      {
        glDepthMask(GL_TRUE);
      }
    }
    if (clearFlags & RERHI::ClearFlag::STENCIL)
    {
      glClearStencil(static_cast<GLint>(stencil));
    }

    // Unlike OpenGL ES 3, when using Direct3D 10 & 11 the scissor rectangle(s) do not affect the clear operation
    // -> We have to compensate the OpenGL ES 3 behaviour in here

    // Disable OpenGL ES 3 scissor test, in case it's not disabled, yet
    if (nullptr != mGraphicsPipelineState && mGraphicsPipelineState->getRasterizerState().scissorEnable)
    {
      glDisable(GL_SCISSOR_TEST);
    }

    // Clear
    glClear(flagsApi);

    // Restore the previously set OpenGL ES 3 states
    if (nullptr != mGraphicsPipelineState && mGraphicsPipelineState->getRasterizerState().scissorEnable)
    {
      glEnable(GL_SCISSOR_TEST);
    }
    if ((clearFlags & RERHI::ClearFlag::DEPTH) && nullptr != mGraphicsPipelineState && RERHI::DepthWriteMask::ALL != mGraphicsPipelineState->getDepthStencilState().depthWriteMask)
    {
      glDepthMask(GL_FALSE);
    }
  }
}

void RHIDynamicRHI::drawGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The OpenGL ES 3 emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL ES 3 draws must not be zero")
  // It's possible to draw without "mVertexArray"

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-draw-indirect emulation");
			}
#endif
  for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
  {
    const RERHI::DrawArguments& drawArguments = *reinterpret_cast<const RERHI::DrawArguments*>(emulationData);
    updateGL_EXT_base_instanceEmulation(drawArguments.startInstanceLocation);

    // Draw and advance
    if (drawArguments.instanceCount > 1 || (drawArguments.startInstanceLocation > 0 && mOpenGLES3Context->getExtensions().isGL_EXT_base_instance()))
    {
      // With instancing
      if (drawArguments.startInstanceLocation > 0 && mOpenGLES3Context->getExtensions().isGL_EXT_base_instance())
      {
        glDrawArraysInstancedBaseInstanceEXT(mOpenGLES3PrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount), drawArguments.startInstanceLocation);
      }
      else
      {
        glDrawArraysInstanced(mOpenGLES3PrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount));
      }
    }
    else
    {
      // Without instancing
      RHI_ASSERT(drawArguments.instanceCount <= 1, "Invalid OpenGL ES 3 instance count")
      glDrawArrays(mOpenGLES3PrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance));
    }
    emulationData += sizeof(RERHI::DrawArguments);
  }
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
#endif
}

void RHIDynamicRHI::drawIndexedGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The OpenGL ES 3 emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL ES 3 draws must not be zero")
  RHI_ASSERT(nullptr != mVertexArray, "Draw OpenGL ES 3 indexed needs a set vertex array")
  RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "Draw OpenGL ES 3 indexed needs a set vertex array which contains an index buffer")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
#endif
  IndexBuffer* indexBuffer = mVertexArray->getIndexBuffer();
  for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
  {
    const RERHI::DrawIndexedArguments& drawIndexedArguments = *reinterpret_cast<const RERHI::DrawIndexedArguments*>(emulationData);
    updateGL_EXT_base_instanceEmulation(drawIndexedArguments.startInstanceLocation);

    // Draw and advance
    if (drawIndexedArguments.instanceCount > 1 || (drawIndexedArguments.startInstanceLocation > 0 && mOpenGLES3Context->getExtensions().isGL_EXT_base_instance()))
    {
      // With instancing
      if (drawIndexedArguments.baseVertexLocation > 0)
      {
        // Use start instance location?
        if (drawIndexedArguments.startInstanceLocation > 0 && mOpenGLES3Context->getExtensions().isGL_EXT_base_instance())
        {
          // Draw with base vertex location and start instance location
          glDrawElementsInstancedBaseVertexBaseInstanceEXT(mOpenGLES3PrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLES3Type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation), drawIndexedArguments.startInstanceLocation);
        }

          // Is the "GL_EXT_draw_elements_base_vertex" extension there?
        else if (mOpenGLES3Context->getExtensions().isGL_EXT_draw_elements_base_vertex())
        {
          // Draw with base vertex location
          glDrawElementsInstancedBaseVertexEXT(mOpenGLES3PrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLES3Type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
        }
        else
        {
          // Error!
          RHI_ASSERT(false, "Failed to OpenGL ES 3 draw indexed emulated")
        }
      }
      else if (drawIndexedArguments.startInstanceLocation > 0 && mOpenGLES3Context->getExtensions().isGL_EXT_base_instance())
      {
        // Draw without base vertex location and with start instance location
        glDrawElementsInstancedBaseInstanceEXT(mOpenGLES3PrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLES3Type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), drawIndexedArguments.startInstanceLocation);
      }
      else
      {
        // Draw without base vertex location
        glDrawElementsInstanced(mOpenGLES3PrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLES3Type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount));
      }
    }
    else
    {
      // Without instancing

      // Use base vertex location?
      if (drawIndexedArguments.baseVertexLocation > 0)
      {
        // Is the "GL_EXT_draw_elements_base_vertex" extension there?
        if (mOpenGLES3Context->getExtensions().isGL_EXT_draw_elements_base_vertex())
        {
          // Draw with base vertex location
          glDrawElementsBaseVertexEXT(mOpenGLES3PrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLES3Type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
        }
        else
        {
          // Error!
          RHI_ASSERT(false, "Failed to OpenGL ES 3 draw indexed emulated")
        }
      }
      else
      {
        // Draw and advance
        glDrawElements(mOpenGLES3PrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLES3Type(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())));
        emulationData += sizeof(RERHI::DrawIndexedArguments);
      }
    }
  }
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
#endif
}


//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resolveMultisampleFramebuffer(RERHI::RHIRenderTarget&, RERHI::RHIFramebuffer&)
{
  // TODO(naetherm) Implement me
}

void RHIDynamicRHI::copyResource(RERHI::RHIResource& destinationResource, RERHI::RHIResource& sourceResource)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, destinationResource)
  RHI_MATCH_CHECK(*this, sourceResource)

  // Evaluate the render target type
  switch (destinationResource.getResourceType())
  {
    case RERHI::ResourceType::TEXTURE_2D:
      if (sourceResource.getResourceType() == RERHI::ResourceType::TEXTURE_2D)
      {
        // Get the OpenGL ES 3 texture 2D instances
        const Texture2D& openGlEs3DestinationTexture2D = static_cast<const Texture2D&>(destinationResource);
        const Texture2D& openGlEs3SourceTexture2D = static_cast<const Texture2D&>(sourceResource);
        RHI_ASSERT(openGlEs3DestinationTexture2D.getWidth() == openGlEs3SourceTexture2D.getWidth(), "OpenGL source and destination width must be identical for resource copy")
        RHI_ASSERT(openGlEs3DestinationTexture2D.getHeight() == openGlEs3SourceTexture2D.getHeight(), "OpenGL source and destination height must be identical for resource copy")

#ifdef RHI_OPENGLES3_STATE_CLEANUP
        // Backup the currently bound OpenGL ES 3 framebuffer
						GLint openGLES3FramebufferBackup = 0;
						glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLES3FramebufferBackup);
#endif

        // Copy resource by using a framebuffer, but only the top-level mipmap
        const GLint width = static_cast<GLint>(openGlEs3DestinationTexture2D.getWidth());
        const GLint height = static_cast<GLint>(openGlEs3DestinationTexture2D.getHeight());
        if (0 == mOpenGLES3CopyResourceFramebuffer)
        {
          glGenFramebuffers(1, &mOpenGLES3CopyResourceFramebuffer);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLES3CopyResourceFramebuffer);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, openGlEs3SourceTexture2D.getOpenGLES3Texture(), 0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, openGlEs3DestinationTexture2D.getOpenGLES3Texture(), 0);
        static constexpr GLenum OPENGL_DRAW_BUFFER[1] =
          {
            GL_COLOR_ATTACHMENT1
          };
        glDrawBuffers(1, OPENGL_DRAW_BUFFER);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
        // Be polite and restore the previous bound OpenGL ES 3 framebuffer
						glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLES3FramebufferBackup));
#endif
      }
      else
      {
        // Error!
        RHI_ASSERT(false, "Failed to copy OpenGL ES 3 resource")
      }
      break;

    case RERHI::ResourceType::ROOT_SIGNATURE:
    case RERHI::ResourceType::RESOURCE_GROUP:
    case RERHI::ResourceType::GRAPHICS_PROGRAM:
    case RERHI::ResourceType::VERTEX_ARRAY:
    case RERHI::ResourceType::RENDER_PASS:
    case RERHI::ResourceType::QUERY_POOL:
    case RERHI::ResourceType::SWAP_CHAIN:
    case RERHI::ResourceType::FRAMEBUFFER:
    case RERHI::ResourceType::VERTEX_BUFFER:
    case RERHI::ResourceType::INDEX_BUFFER:
    case RERHI::ResourceType::TEXTURE_BUFFER:
    case RERHI::ResourceType::STRUCTURED_BUFFER:
    case RERHI::ResourceType::INDIRECT_BUFFER:
    case RERHI::ResourceType::UNIFORM_BUFFER:
    case RERHI::ResourceType::TEXTURE_1D:
    case RERHI::ResourceType::TEXTURE_1D_ARRAY:
    case RERHI::ResourceType::TEXTURE_2D_ARRAY:
    case RERHI::ResourceType::TEXTURE_3D:
    case RERHI::ResourceType::TEXTURE_CUBE:
    case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
    case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
    case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
    case RERHI::ResourceType::SAMPLER_STATE:
    case RERHI::ResourceType::VERTEX_SHADER:
    case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
    case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
    case RERHI::ResourceType::GEOMETRY_SHADER:
    case RERHI::ResourceType::FRAGMENT_SHADER:
    case RERHI::ResourceType::TASK_SHADER:
    case RERHI::ResourceType::MESH_SHADER:
    case RERHI::ResourceType::COMPUTE_SHADER:
    default:
      // Not handled in here
      break;
  }
}

void RHIDynamicRHI::generateMipmaps(RERHI::RHIResource& resource)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, resource)
  RHI_ASSERT(resource.getResourceType() == RERHI::ResourceType::TEXTURE_2D, "TODO(naetherm) Mipmaps can only be generated for OpenGL ES 3 2D texture resources")

  Texture2D& texture2D = static_cast<Texture2D&>(resource);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently bound OpenGL ES 3 texture
			// TODO(naetherm) It's possible to avoid calling this multiple times
			GLint openGLES3TextureBackup = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLES3TextureBackup);
#endif

  // Generate mipmaps
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture2D.getOpenGLES3Texture());
  glGenerateMipmap(GL_TEXTURE_2D);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 texture
			glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLES3TextureBackup));
#endif
}


//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resetQueryPool([[maybe_unused]] RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 firstQueryIndex, [[maybe_unused]] RECore::uint32 numberOfQueries)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // TODO(naetherm) Implement me
  NOP;
}

void RHIDynamicRHI::beginQuery([[maybe_unused]] RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 queryIndex, [[maybe_unused]] RECore::uint32 queryControlFlags)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // TODO(naetherm) Implement me
  NOP;
}

void RHIDynamicRHI::endQuery([[maybe_unused]] RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 queryIndex)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // TODO(naetherm) Implement me
  NOP;
}

void RHIDynamicRHI::writeTimestampQuery([[maybe_unused]] RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 queryIndex)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // TODO(naetherm) Implement me
  NOP;
}


//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDynamicRHI::setDebugMarker(const char* name)
		{
			// "GL_KHR_debug"-extension required
			if (mOpenGLES3Context->getExtensions().isGL_KHR_debug())
			{
				RHI_ASSERT(nullptr != name, "OpenGL ES 3 debug marker names must not be a null pointer")
				glDebugMessageInsertKHR(GL_DEBUG_SOURCE_APPLICATION_KHR, GL_DEBUG_TYPE_MARKER_KHR, 1, GL_DEBUG_SEVERITY_NOTIFICATION_KHR, -1, name);
			}
		}

		void RHIDynamicRHI::beginDebugEvent(const char* name)
		{
			// "GL_KHR_debug"-extension required
			if (mOpenGLES3Context->getExtensions().isGL_KHR_debug())
			{
				RHI_ASSERT(nullptr != name, "OpenGL ES 3 debug event names must not be a null pointer")
				glPushDebugGroupKHR(GL_DEBUG_SOURCE_APPLICATION_KHR, 1, -1, name);
			}
		}

		void RHIDynamicRHI::endDebugEvent()
		{
			// "GL_KHR_debug"-extension required
			if (mOpenGLES3Context->getExtensions().isGL_KHR_debug())
			{
				glPopDebugGroupKHR();
			}
		}
#endif


//[-------------------------------------------------------]
//[ Public virtual RERHI::RHIDynamicRHI methods                      ]
//[-------------------------------------------------------]
const char* RHIDynamicRHI::getName() const
{
  return "OpenGLES3";
}

bool RHIDynamicRHI::isInitialized() const
{
  // Is the OpenGL ES 3 context initialized?
  return mOpenGLES3Context->isInitialized();
}

bool RHIDynamicRHI::isDebugEnabled()
{
  // OpenGL ES 3 has nothing that is similar to the Direct3D 9 PIX functions (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)

  // Debug disabled
  return false;
}


//[-------------------------------------------------------]
//[ Shader language                                       ]
//[-------------------------------------------------------]
RECore::uint32 RHIDynamicRHI::getNumberOfShaderLanguages() const
{
  return 1;
}

const char* RHIDynamicRHI::getShaderLanguageName([[maybe_unused]] RECore::uint32 index) const
{
  RHI_ASSERT(index < getNumberOfShaderLanguages(), "OpenGL ES 3: Shader language index is out-of-bounds")
  return ::detail::GLSLES_NAME;
}

RERHI::RHIShaderLanguage* RHIDynamicRHI::getShaderLanguage(const char* shaderLanguageName)
{
  // In case "shaderLanguage" is a null pointer, use the default shader language
  if (nullptr != shaderLanguageName)
  {
    // Optimization: Check for shader language name pointer match, first
    if (::detail::GLSLES_NAME == shaderLanguageName || !stricmp(shaderLanguageName, ::detail::GLSLES_NAME))
    {
      // If required, create the GLSL shader language instance right now
      if (nullptr == mShaderLanguageGlsl)
      {
        mShaderLanguageGlsl = RHI_NEW(mContext, ShaderLanguageGlsl)(*this);
        mShaderLanguageGlsl->AddReference();	// Internal RHI reference
      }

      // Return the shader language instance
      return mShaderLanguageGlsl;
    }

    // Error!
    return nullptr;
  }

  // Return the GLSL shader language instance as default
  return getShaderLanguage(::detail::GLSLES_NAME);
}


//[-------------------------------------------------------]
//[ Resource creation                                     ]
//[-------------------------------------------------------]
RERHI::RHIRenderPass* RHIDynamicRHI::createRenderPass(RECore::uint32 numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  return RHI_NEW(mContext, RenderPass)(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIQueryPool* RHIDynamicRHI::createQueryPool([[maybe_unused]] RERHI::QueryType queryType, [[maybe_unused]] RECore::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER_NO_DEFAULT)
{
  // TODO(naetherm) Implement me
  return nullptr;
}

RERHI::RHISwapChain* RHIDynamicRHI::createSwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle, bool RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, renderPass)
  RHI_ASSERT(NULL_HANDLE != windowHandle.nativeWindowHandle || nullptr != windowHandle.renderWindow, "OpenGL ES 3: The provided native window handle or render window must not be a null handle / null pointer")

  // Create the swap chain
  return RHI_NEW(mContext, SwapChain)(renderPass, windowHandle RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIFramebuffer* RHIDynamicRHI::createFramebuffer(RERHI::RHIRenderPass& renderPass, const RERHI::FramebufferAttachment* colorFramebufferAttachments, const RERHI::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, renderPass)

  // Create the framebuffer
  return RHI_NEW(mContext, Framebuffer)(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIBufferManager* RHIDynamicRHI::createBufferManager()
{
  return RHI_NEW(mContext, BufferManager)(*this);
}

RERHI::RHITextureManager* RHIDynamicRHI::createTextureManager()
{
  return RHI_NEW(mContext, TextureManager)(*this);
}

RERHI::RHIRootSignature* RHIDynamicRHI::createRootSignature(const RERHI::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  return RHI_NEW(mContext, RootSignature)(*this, rootSignature RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGraphicsPipelineState* RHIDynamicRHI::createGraphicsPipelineState(const RERHI::GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity checks
  RHI_ASSERT(nullptr != graphicsPipelineState.rootSignature, "OpenGL ES 3: Invalid graphics pipeline state root signature")
  RHI_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "OpenGL ES 3: Invalid graphics pipeline state graphics program")
  RHI_ASSERT(nullptr != graphicsPipelineState.renderPass, "OpenGL ES 3: Invalid graphics pipeline state render pass")

  // Create graphics pipeline state
  RECore::uint16 id = 0;
  if (GraphicsPipelineStateMakeId.createID(id))
  {
    return RHI_NEW(mContext, GraphicsPipelineState)(*this, graphicsPipelineState, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error: Ensure a correct reference counter behaviour
  graphicsPipelineState.rootSignature->AddReference();
  graphicsPipelineState.rootSignature->Release();
  graphicsPipelineState.graphicsProgram->AddReference();
  graphicsPipelineState.graphicsProgram->Release();
  graphicsPipelineState.renderPass->AddReference();
  graphicsPipelineState.renderPass->Release();
  return nullptr;
}

RERHI::RHIComputePipelineState* RHIDynamicRHI::createComputePipelineState(RERHI::RHIRootSignature& rootSignature, RERHI::RHIComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER_NO_DEFAULT)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, rootSignature)
  RHI_MATCH_CHECK(*this, computeShader)

  // Error: Ensure a correct reference counter behaviour
  rootSignature.AddReference();
  rootSignature.Release();
  computeShader.AddReference();
  computeShader.Release();

  // Error! OpenGL ES 3 has no compute shader support.
  return nullptr;
}

RERHI::RHISamplerState* RHIDynamicRHI::createSamplerState(const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  return RHI_NEW(mContext, SamplerState)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Resource handling                                     ]
//[-------------------------------------------------------]
bool RHIDynamicRHI::map(RERHI::RHIResource& resource, RECore::uint32, RERHI::MapType mapType, RECore::uint32, RERHI::MappedSubresource& mappedSubresource)
{
  // Evaluate the resource type
  switch (resource.getResourceType())
  {
    case RERHI::ResourceType::VERTEX_BUFFER:
    {
      const VertexBuffer& vertexBuffer = static_cast<VertexBuffer&>(resource);
      return ::detail::mapBuffer(mContext, GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING, vertexBuffer.getOpenGLES3ArrayBuffer(), vertexBuffer.getBufferSize(), mapType, mappedSubresource);
    }

    case RERHI::ResourceType::INDEX_BUFFER:
    {
      const IndexBuffer& indexBuffer = static_cast<IndexBuffer&>(resource);
      return ::detail::mapBuffer(mContext, GL_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER_BINDING, indexBuffer.getOpenGLES3ElementArrayBuffer(), indexBuffer.getBufferSize(), mapType, mappedSubresource);
    }

    case RERHI::ResourceType::TEXTURE_BUFFER:
    {
      const TextureBuffer& textureBuffer = static_cast<TextureBuffer&>(resource);
      return ::detail::mapBuffer(mContext, GL_TEXTURE_BUFFER_EXT, GL_TEXTURE_BINDING_BUFFER_EXT, textureBuffer.getOpenGLES3TextureBuffer(), textureBuffer.getBufferSize(), mapType, mappedSubresource);
    }

    case RERHI::ResourceType::STRUCTURED_BUFFER:
    {
      // TODO(naetherm) Add OpenGL ES structured buffer support ("GL_EXT_buffer_storage"-extension)
      return false;
    }

    case RERHI::ResourceType::INDIRECT_BUFFER:
      mappedSubresource.data		 = static_cast<IndirectBuffer&>(resource).getWritableEmulationData();
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return true;

    case RERHI::ResourceType::UNIFORM_BUFFER:
    {
      const UniformBuffer& uniformBuffer = static_cast<UniformBuffer&>(resource);
      return ::detail::mapBuffer(mContext, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING, uniformBuffer.getOpenGLES3UniformBuffer(), uniformBuffer.getBufferSize(), mapType, mappedSubresource);
    }

    case RERHI::ResourceType::TEXTURE_1D:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case RERHI::ResourceType::TEXTURE_1D_ARRAY:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case RERHI::ResourceType::TEXTURE_2D:
    {
      bool result = false;

      // TODO(naetherm) Implement me
      /*
      // Begin debug event
      RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

      // Get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2D&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Map the Direct3D 11 resource
        result = (S_OK == mD3D11DeviceContext->Map(d3d11Resource, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, reinterpret_cast<D3D11_MAPPED_SUBRESOURCE*>(&mappedSubresource)));

        // Release the Direct3D 11 resource instance
        d3d11Resource->Release();
      }

      // End debug event
      RHI_END_DEBUG_EVENT(this)
      */

      // Done
      return result;
    }

    case RERHI::ResourceType::TEXTURE_2D_ARRAY:
    {
      bool result = false;

      // TODO(naetherm) Implement me
      /*
      // Begin debug event
      RHI_BEGIN_DEBUG_EVENT_FUNCTION(this)

      // Get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2DArray&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Map the Direct3D 11 resource
        result = (S_OK == mD3D11DeviceContext->Map(d3d11Resource, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, reinterpret_cast<D3D11_MAPPED_SUBRESOURCE*>(&mappedSubresource)));

        // Release the Direct3D 11 resource instance
        d3d11Resource->Release();
      }

      // End debug event
      RHI_END_DEBUG_EVENT(this)
      */

      // Done
      return result;
    }

    case RERHI::ResourceType::TEXTURE_3D:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case RERHI::ResourceType::TEXTURE_CUBE:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
    {
      // TODO(naetherm) Implement me
      return false;
    }

    case RERHI::ResourceType::ROOT_SIGNATURE:
    case RERHI::ResourceType::RESOURCE_GROUP:
    case RERHI::ResourceType::GRAPHICS_PROGRAM:
    case RERHI::ResourceType::VERTEX_ARRAY:
    case RERHI::ResourceType::RENDER_PASS:
    case RERHI::ResourceType::QUERY_POOL:
    case RERHI::ResourceType::SWAP_CHAIN:
    case RERHI::ResourceType::FRAMEBUFFER:
    case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
    case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
    case RERHI::ResourceType::SAMPLER_STATE:
    case RERHI::ResourceType::VERTEX_SHADER:
    case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
    case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
    case RERHI::ResourceType::GEOMETRY_SHADER:
    case RERHI::ResourceType::FRAGMENT_SHADER:
    case RERHI::ResourceType::TASK_SHADER:
    case RERHI::ResourceType::MESH_SHADER:
    case RERHI::ResourceType::COMPUTE_SHADER:
    default:
      // Nothing we can map, set known return values
      mappedSubresource.data		 = nullptr;
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;

      // Error!
      return false;
  }
}

void RHIDynamicRHI::unmap(RERHI::RHIResource& resource, RECore::uint32)
{
  // Evaluate the resource type
  switch (resource.getResourceType())
  {
    case RERHI::ResourceType::VERTEX_BUFFER:
      ::detail::unmapBuffer(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING, static_cast<VertexBuffer&>(resource).getOpenGLES3ArrayBuffer());
      break;

    case RERHI::ResourceType::INDEX_BUFFER:
      ::detail::unmapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER_BINDING, static_cast<IndexBuffer&>(resource).getOpenGLES3ElementArrayBuffer());
      break;

    case RERHI::ResourceType::TEXTURE_BUFFER:
      ::detail::unmapBuffer(GL_TEXTURE_BUFFER_EXT, GL_TEXTURE_BINDING_BUFFER_EXT, static_cast<TextureBuffer&>(resource).getOpenGLES3TextureBuffer());
      break;

    case RERHI::ResourceType::STRUCTURED_BUFFER:
      // TODO(naetherm) Add OpenGL ES structured buffer support ("GL_EXT_buffer_storage"-extension)
      break;

    case RERHI::ResourceType::INDIRECT_BUFFER:
      // Nothing here, it's a software emulated indirect buffer
      break;

    case RERHI::ResourceType::UNIFORM_BUFFER:
      ::detail::unmapBuffer(GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING, static_cast<UniformBuffer&>(resource).getOpenGLES3UniformBuffer());
      break;

    case RERHI::ResourceType::TEXTURE_1D:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case RERHI::ResourceType::TEXTURE_1D_ARRAY:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case RERHI::ResourceType::TEXTURE_2D:
    {
      // TODO(naetherm) Implement me
      /*
      // Get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2D&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Unmap the Direct3D 11 resource
        mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

        // Release the Direct3D 11 resource instance
        d3d11Resource->Release();
      }
      */
      break;
    }

    case RERHI::ResourceType::TEXTURE_2D_ARRAY:
    {
      // TODO(naetherm) Implement me
      /*
      // Get the Direct3D 11 resource instance
      ID3D11Resource* d3d11Resource = nullptr;
      static_cast<Texture2DArray&>(resource).getD3D11ShaderResourceView()->GetResource(&d3d11Resource);
      if (nullptr != d3d11Resource)
      {
        // Unmap the Direct3D 11 resource
        mD3D11DeviceContext->Unmap(d3d11Resource, subresource);

        // Release the Direct3D 11 resource instance
        d3d11Resource->Release();
      }
      */
      break;
    }

    case RERHI::ResourceType::TEXTURE_3D:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case RERHI::ResourceType::TEXTURE_CUBE:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
    {
      // TODO(naetherm) Implement me
      break;
    }

    case RERHI::ResourceType::ROOT_SIGNATURE:
    case RERHI::ResourceType::RESOURCE_GROUP:
    case RERHI::ResourceType::GRAPHICS_PROGRAM:
    case RERHI::ResourceType::VERTEX_ARRAY:
    case RERHI::ResourceType::RENDER_PASS:
    case RERHI::ResourceType::QUERY_POOL:
    case RERHI::ResourceType::SWAP_CHAIN:
    case RERHI::ResourceType::FRAMEBUFFER:
    case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
    case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
    case RERHI::ResourceType::SAMPLER_STATE:
    case RERHI::ResourceType::VERTEX_SHADER:
    case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
    case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
    case RERHI::ResourceType::GEOMETRY_SHADER:
    case RERHI::ResourceType::FRAGMENT_SHADER:
    case RERHI::ResourceType::TASK_SHADER:
    case RERHI::ResourceType::MESH_SHADER:
    case RERHI::ResourceType::COMPUTE_SHADER:
    default:
      // Nothing we can unmap
      break;
  }
}

bool RHIDynamicRHI::getQueryPoolResults([[maybe_unused]] RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 numberOfDataBytes, [[maybe_unused]] RECore::uint8* data, [[maybe_unused]] RECore::uint32 firstQueryIndex, [[maybe_unused]] RECore::uint32 numberOfQueries, [[maybe_unused]] RECore::uint32 strideInBytes, [[maybe_unused]] RECore::uint32 queryResultFlags)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // TODO(naetherm) Implement me
  return false;
}


//[-------------------------------------------------------]
//[ Operation                                             ]
//[-------------------------------------------------------]
void RHIDynamicRHI::dispatchCommandBuffer(const RERHI::RHICommandBuffer& commandBuffer)
{
  // Sanity check
  RHI_ASSERT(!commandBuffer.isEmpty(), "The OpenGL ES 3 command buffer to dispatch mustn't be empty")

  // Dispatch command buffer
  dispatchCommandBufferInternal(commandBuffer);
}


//[-------------------------------------------------------]
//[ Private static methods                                ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDynamicRHI::debugMessageCallback(RECore::uint32 source, RECore::uint32 type, RECore::uint32 id, RECore::uint32 severity, int, const char* message, const void* userParam)
		{
			// Source to string
			char debugSource[20 + 1]{0};	// +1 for terminating zero
			switch (source)
			{
				case GL_DEBUG_SOURCE_API_KHR:
					strncpy(debugSource, "OpenGL", 20);
					break;

				case GL_DEBUG_SOURCE_WINDOW_SYSTEM_KHR:
					strncpy(debugSource, "Windows", 20);
					break;

				case GL_DEBUG_SOURCE_SHADER_COMPILER_KHR:
					strncpy(debugSource, "Shader compiler", 20);
					break;

				case GL_DEBUG_SOURCE_THIRD_PARTY_KHR:
					strncpy(debugSource, "Third party", 20);
					break;

				case GL_DEBUG_SOURCE_APPLICATION_KHR:
					strncpy(debugSource, "Application", 20);
					break;

				case GL_DEBUG_SOURCE_OTHER_KHR:
					strncpy(debugSource, "Other", 20);
					break;

				default:
					strncpy(debugSource, "?", 20);
					break;
			}

			// Debug type to string
			RECore::ILog::Type logType = RECore::ILog::Type::CRITICAL;
			char debugType[25 + 1]{0};	// +1 for terminating zero
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR_KHR:
					strncpy(debugType, "Error", 25);
					break;

				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR:
					logType = RECore::ILog::Type::COMPATIBILITY_WARNING;
					strncpy(debugType, "Deprecated behavior", 25);
					break;

				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR:
					strncpy(debugType, "Undefined behavior", 25);
					break;

				case GL_DEBUG_TYPE_PORTABILITY_KHR:
					logType = RECore::ILog::Type::COMPATIBILITY_WARNING;
					strncpy(debugType, "Portability", 25);
					break;

				case GL_DEBUG_TYPE_PERFORMANCE_KHR:
					logType = RECore::ILog::Type::PERFORMANCE_WARNING;
					strncpy(debugType, "Performance", 25);
					break;

				case GL_DEBUG_TYPE_OTHER_KHR:
					strncpy(debugType, "Other", 25);
					break;

				case GL_DEBUG_TYPE_MARKER_KHR:
					strncpy(debugType, "Marker", 25);
					break;

				case GL_DEBUG_TYPE_PUSH_GROUP_KHR:
					// TODO(naetherm) How to ignore "glPushDebugGroupKHR()" via "glDebugMessageControlKHR()" by default in here to have the same behaviour as OpenGL "glPushDebugGroup()"?
					return;
					// strncpy(debugType, "Push group", 25);
					// break;

				case GL_DEBUG_TYPE_POP_GROUP_KHR:
					// TODO(naetherm) How to ignore "glPopDebugGroupKHR()" via "glDebugMessageControlKHR()" by default in here to have the same behaviour as OpenGL "glPopDebugGroup()"?
					return;
					// strncpy(debugType, "Pop group", 25);
					// break;

				default:
					strncpy(debugType, "?", 25);
					break;
			}

			// Debug severity to string
			char debugSeverity[20 + 1]{0};	// +1 for terminating zero
			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH_KHR:
					strncpy(debugSeverity, "High", 20);
					break;

				case GL_DEBUG_SEVERITY_MEDIUM_KHR:
					strncpy(debugSeverity, "Medium", 20);
					break;

				case GL_DEBUG_SEVERITY_LOW_KHR:
					strncpy(debugSeverity, "Low", 20);
					break;

				case GL_DEBUG_SEVERITY_NOTIFICATION_KHR:
					strncpy(debugSeverity, "Notification", 20);
					break;

				default:
					strncpy(debugSeverity, "?", 20);
					break;
			}

			// Print into log
			if (static_cast<const RHIDynamicRHI*>(userParam)->getContext().getLog().print(logType, nullptr, __FILE__, static_cast<RECore::uint32>(__LINE__), "OpenGL ES 3 debug message\tSource:\"%s\"\tType:\"%s\"\tID:\"%u\"\tSeverity:\"%s\"\tMessage:\"%s\"", debugSource, debugType, id, debugSeverity, message))
			{
				DEBUG_BREAK;
			}
		}
#else
void RHIDynamicRHI::debugMessageCallback(RECore::uint32, RECore::uint32, RECore::uint32, RECore::uint32, int, const char*, const void*)
{
  // Nothing here
}
#endif


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void RHIDynamicRHI::initializeCapabilities()
{
  GLint openGLValue = 0;

  { // Get device name
    // -> OpenGL ES Version 3.2 (November 3, 2016) Specification, section 20.2, page 440: "String queries return pointers to UTF-8 encoded, null-terminated static strings describing properties of the current GL context."
    // -> For example "PVRVFrame 10.6 - None (Host : AMD Radeon R9 200 Series) (SDK Build: 17.1@4673912)" (used PowerVR_SDK OpenGL ES emulator)
    const size_t numberOfCharacters = ::detail::countof(mCapabilities.deviceName) - 1;
    strncpy(mCapabilities.deviceName, reinterpret_cast<const char*>(glGetString(GL_RENDERER)), numberOfCharacters);
    mCapabilities.deviceName[numberOfCharacters] = '\0';
  }

  // Preferred swap chain texture format
  mCapabilities.preferredSwapChainColorTextureFormat		  = RERHI::TextureFormat::Enum::R8G8B8A8;
  mCapabilities.preferredSwapChainDepthStencilTextureFormat = RERHI::TextureFormat::Enum::D32_FLOAT;

  // Maximum number of viewports (always at least 1)
  mCapabilities.maximumNumberOfViewports = 1;	// OpenGL ES 3 only supports a single viewport

  // Maximum number of simultaneous render targets (if <1 render to texture is not supported)
  glGetIntegerv(GL_MAX_DRAW_BUFFERS, &openGLValue);
  mCapabilities.maximumNumberOfSimultaneousRenderTargets = static_cast<RECore::uint32>(openGLValue);

  // Maximum texture dimension
  openGLValue = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &openGLValue);
  mCapabilities.maximumTextureDimension = static_cast<RECore::uint32>(openGLValue);

  // Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
  glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &openGLValue);
  mCapabilities.maximumNumberOf1DTextureArraySlices = static_cast<RECore::uint32>(openGLValue);

  // Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
  mCapabilities.maximumNumberOf2DTextureArraySlices = static_cast<RECore::uint32>(openGLValue);

  // Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
  mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;	// TODO(naetherm) Implement me		static_cast<RECore::uint32>(openGLValue);

  // Maximum uniform buffer (UBO) size in bytes (usually at least 16384 bytes, in case there's no support for uniform buffer it's 0)
  openGLValue = 0;
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &openGLValue);
  mCapabilities.maximumUniformBufferSize = static_cast<RECore::uint32>(openGLValue);

  // Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
  if (mOpenGLES3Context->getExtensions().isGL_EXT_texture_buffer())
  {
    openGLValue = 0;
    glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE_EXT, &openGLValue);
    mCapabilities.maximumTextureBufferSize = static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    // We can only emulate the "RERHI::TextureFormat::R32G32B32A32F" texture format using an uniform buffer
    mCapabilities.maximumTextureBufferSize = sizeof(float) * 4 * 4096;	// 64 KiB
  }

  // TODO(naetherm) Add OpenGL ES structured buffer support ("GL_EXT_buffer_storage"-extension)
  mCapabilities.maximumStructuredBufferSize = 0;

  // Maximum indirect buffer size in bytes
  mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

  // Maximum number of multisamples (always at least 4 according to https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glGet.xhtml )
  glGetIntegerv(GL_MAX_SAMPLES, &openGLValue);
  if (openGLValue > 8)
  {
    // Limit to known maximum we can test
    openGLValue = 8;
  }
  mCapabilities.maximumNumberOfMultisamples = static_cast<RECore::uint8>(openGLValue);
  // TODO(naetherm) Implement multisample support
  mCapabilities.maximumNumberOfMultisamples = 1;

  // Maximum anisotropy (always at least 1, usually 16)
  // -> "GL_EXT_texture_filter_anisotropic"-extension
  glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &openGLValue);
  mCapabilities.maximumAnisotropy = static_cast<RECore::uint8>(openGLValue);

  // Coordinate system
  // -> If the "GL_EXT_clip_control"-extension is available: Left-handed coordinate system with clip space depth value range 0..1
  // -> If the "GL_EXT_clip_control"-extension isn't available: Right-handed coordinate system with clip space depth value range -1..1
  // -> For background theory see "Depth Precision Visualized" by Nathan Reed - https://developer.nvidia.com/content/depth-precision-visualized
  // -> For practical information see "Reversed-Z in OpenGL" by Nicolas Guillemot - https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
  // -> Shaders might want to take the following into account: "Mac computers that use OpenCL and OpenGL graphics" - https://support.apple.com/en-us/HT202823 - "iMac (Retina 5K, 27-inch, 2017)" - OpenGL 4.1
  mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = mOpenGLES3Context->getExtensions().isGL_EXT_clip_control();

  // Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
  mCapabilities.individualUniforms = true;

  // Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
  mCapabilities.instancedArrays = true;	// Is core feature in OpenGL ES 3.0

  // Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID, OpenGL ES 3 has no "GL_ARB_draw_instanced" extension)
  mCapabilities.drawInstanced = true;	// Is core feature in OpenGL ES 3.0

  // Base vertex supported for draw calls?
  mCapabilities.baseVertex = mOpenGLES3Context->getExtensions().isGL_EXT_draw_elements_base_vertex();

  // OpenGL ES 3 has no native multithreading
  mCapabilities.nativeMultithreading = false;

  // We don't support the OpenGL ES 3 program binaries since those are operation system and graphics driver version dependent, which renders them useless for pre-compiled shaders shipping
  mCapabilities.shaderBytecode = false;

  // Is there support for vertex shaders (VS)?
  mCapabilities.vertexShader = true;

  // Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
  mCapabilities.maximumNumberOfPatchVertices = 0;	// OpenGL ES 3 has no tessellation support

  // Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
  mCapabilities.maximumNumberOfGsOutputVertices = 0;	// OpenGL ES 3 has no support for geometry shaders

  // Is there support for fragment shaders (FS)?
  mCapabilities.fragmentShader = true;

  // Is there support for compute shaders (CS)?
  mCapabilities.computeShader = false;
}

void RHIDynamicRHI::setGraphicsProgram(RERHI::RHIGraphicsProgram* graphicsProgram)
{
  if (nullptr != graphicsProgram)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *graphicsProgram)

    // Bind the graphics program, if required
    const GraphicsProgramGlsl* graphicsProgramGlsl = static_cast<GraphicsProgramGlsl*>(graphicsProgram);
    const RECore::uint32 openGLES3Program = graphicsProgramGlsl->getOpenGLES3Program();
    if (openGLES3Program != mOpenGLES3Program)
    {
      mOpenGLES3Program = openGLES3Program;
      mDrawIdUniformLocation = graphicsProgramGlsl->getDrawIdUniformLocation();
      mCurrentStartInstanceLocation = ~0u;
      glUseProgram(mOpenGLES3Program);
    }
  }
  else if (0 != mOpenGLES3Program)
  {
    // Unbind the program
    glUseProgram(0);
    mOpenGLES3Program = 0;
    mDrawIdUniformLocation = -1;
    mCurrentStartInstanceLocation = ~0u;
  }
}

void RHIDynamicRHI::updateGL_EXT_base_instanceEmulation(RECore::uint32 startInstanceLocation)
{
  if (mDrawIdUniformLocation != -1 && 0 != mOpenGLES3Program && mCurrentStartInstanceLocation != startInstanceLocation)
  {
    glUniform1ui(mDrawIdUniformLocation, startInstanceLocation);
    mCurrentStartInstanceLocation = startInstanceLocation;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3



//RERHIVULKAN_API
RERHI::RHIDynamicRHI* createOpenGLES3RhiInstance(const RERHI::RHIContext& context)
{
  return RHI_NEW(context, RERHIOpenGLES3::RHIDynamicRHI)(context);
}