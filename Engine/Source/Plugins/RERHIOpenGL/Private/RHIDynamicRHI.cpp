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
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/OpenGLRuntimeLinking.h"
#include "RERHIOpenGL/RHIOpenGLContext.h"
#include "RERHIOpenGL/RHIRootSignature.h"
#include "RERHIOpenGL/RHIResourceGroup.h"
#include "RERHIOpenGL/Query/PipelineStatisticsQueryPool.h"
#include "RERHIOpenGL/Query/RHIOcclusionTimestampQueryPool.h"
#include "RERHIOpenGL/Buffer/RHIBufferManager.h"
#include "RERHIOpenGL/Buffer/RHIIndexBuffer.h"
#include "RERHIOpenGL/Buffer/RHIIndirectBuffer.h"
#include "RERHIOpenGL/Buffer/RHIStructuredBuffer.h"
#include "RERHIOpenGL/Buffer/RHITextureBuffer.h"
#include "RERHIOpenGL/Buffer/RHIUniformBuffer.h"
#include "RERHIOpenGL/Buffer/RHIVertexArray.h"
#include "RERHIOpenGL/Buffer/RHIVertexArrayNoVao.h"
#include "RERHIOpenGL/Buffer/RHIVertexArrayVao.h"
#include "RERHIOpenGL/Buffer/RHIVertexBuffer.h"
#include "RERHIOpenGL/RenderTarget/RHIFramebufferBind.h"
#include "RERHIOpenGL/RenderTarget/RHIFramebufferDsa.h"
#include "RERHIOpenGL/RenderTarget/RHIRenderPass.h"
#include "RERHIOpenGL/RenderTarget/RHISwapChain.h"
#include "RERHIOpenGL/Texture/RHITextureManager.h"
#include "RERHIOpenGL/Texture/RHITexture1D.h"
#include "RERHIOpenGL/Texture/RHITexture1DBind.h"
#include "RERHIOpenGL/Texture/RHITexture1DDsa.h"
#include "RERHIOpenGL/Texture/RHITexture1DArray.h"
#include "RERHIOpenGL/Texture/RHITexture1DArrayBind.h"
#include "RERHIOpenGL/Texture/RHITexture1DArrayDsa.h"
#include "RERHIOpenGL/Texture/RHITexture2D.h"
#include "RERHIOpenGL/Texture/RHITexture2DBind.h"
#include "RERHIOpenGL/Texture/RHITexture2DDsa.h"
#include "RERHIOpenGL/Texture/RHITexture2DArray.h"
#include "RERHIOpenGL/Texture/RHITexture2DArrayBind.h"
#include "RERHIOpenGL/Texture/RHITexture2DArrayDsa.h"
#include "RERHIOpenGL/Texture/RHITexture3D.h"
#include "RERHIOpenGL/Texture/RHITexture3DBind.h"
#include "RERHIOpenGL/Texture/RHITexture3DDsa.h"
#include "RERHIOpenGL/Texture/RHITextureCubeBind.h"
#include "RERHIOpenGL/Texture/RHITextureCubeDsa.h"
#include "RERHIOpenGL/Texture/RHITextureCubeArray.h"
#include "RERHIOpenGL/Texture/RHITextureCubeArrayBind.h"
#include "RERHIOpenGL/Texture/RHITextureCubeArrayDsa.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIComputeShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIShaderLanguageMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIGraphicsProgramMonolithicDsa.h"
#include "RERHIOpenGL/Shader/Separate/RHIComputeShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIShaderLanguageSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIGraphicsProgramSeparateDsa.h"
#include "RERHIOpenGL/Shader/Separate/RHIVertexShaderSeparate.h"
#include "RERHIOpenGL/State/RHIDepthStencilState.h"
#include "RERHIOpenGL/State/RHIRasterizerState.h"
#include "RERHIOpenGL/State/RHIBlendState.h"
#include "RERHIOpenGL/State/RHIComputePipelineStateMonolithic.h"
#include "RERHIOpenGL/State/RHIComputePipelineStateSeparate.h"
#include "RERHIOpenGL/State/RHIGraphicsPipelineState.h"
#include "RERHIOpenGL/State/RHISamplerState.h"
#include "RERHIOpenGL/State/RHISamplerStateBind.h"
#include "RERHIOpenGL/State/RHISamplerStateDsa.h"
#include "RERHIOpenGL/State/RHISamplerStateSo.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Helper.h"
#include "RERHIOpenGL/Mapping.h"
#if defined(LINUX)
#include "RERHIOpenGL/Linux/RHIOpenGLContextLinux.h"
#endif


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

[[nodiscard]] bool mapBuffer([[maybe_unused]] const RERHI::RHIContext& context, const RERHIOpenGL::Extensions& extensions, GLenum target, [[maybe_unused]] GLenum bindingTarget, GLuint openGLBuffer, RERHI::MapType mapType, RERHI::MappedSubresource& mappedSubresource)
{
  // TODO(naetherm) This buffer update isn't efficient, use e.g. persistent buffer mapping

  // Is "GL_ARB_direct_state_access" there?
  if (extensions.isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    mappedSubresource.data		 = RERHIOpenGL::glMapNamedBuffer(openGLBuffer, RERHIOpenGL::Mapping::getOpenGLMapType(mapType));
    mappedSubresource.rowPitch   = 0;
    mappedSubresource.depthPitch = 0;
  }
    // Is "GL_EXT_direct_state_access" there?
  else if (extensions.isGL_EXT_direct_state_access())
  {
    // Effective direct state access (DSA)
    mappedSubresource.data		 = RERHIOpenGL::glMapNamedBufferEXT(openGLBuffer, RERHIOpenGL::Mapping::getOpenGLMapType(mapType));
    mappedSubresource.rowPitch   = 0;
    mappedSubresource.depthPitch = 0;
  }
  else
  {
    // Traditional bind version

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL buffer
					GLint openGLBufferBackup = 0;
					glGetIntegerv(bindingTarget, &openGLBufferBackup);
#endif

    // Bind this OpenGL buffer
    RERHIOpenGL::glBindBufferARB(target, openGLBuffer);

    // Map
    mappedSubresource.data		 = RERHIOpenGL::glMapBufferARB(target, RERHIOpenGL::Mapping::getOpenGLMapType(mapType));
    mappedSubresource.rowPitch   = 0;
    mappedSubresource.depthPitch = 0;

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL buffer
					glBindBufferARB(target, static_cast<GLuint>(openGLBufferBackup));
#endif
  }

  // Done
  RHI_ASSERT(nullptr != mappedSubresource.data, "Mapping of OpenGL buffer failed")
  return (nullptr != mappedSubresource.data);
}

void unmapBuffer(const RERHIOpenGL::Extensions& extensions, GLenum target, [[maybe_unused]] GLenum bindingTarget, GLuint openGLBuffer)
{
  // Is "GL_ARB_direct_state_access" there?
  if (extensions.isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    RERHIOpenGL::glUnmapNamedBuffer(openGLBuffer);
  }
    // Is "GL_EXT_direct_state_access" there?
  else if (extensions.isGL_EXT_direct_state_access())
  {
    // Effective direct state access (DSA)
    RERHIOpenGL::glUnmapNamedBufferEXT(openGLBuffer);
  }
  else
  {
    // Traditional bind version

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL buffer
					GLint openGLBufferBackup = 0;
					glGetIntegerv(bindingTarget, &openGLBufferBackup);
#endif

    // Bind this OpenGL buffer
    RERHIOpenGL::glBindBufferARB(target, openGLBuffer);

    // Unmap
    RERHIOpenGL::glUnmapBufferARB(target);

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL buffer
					glBindBufferARB(target, static_cast<GLuint>(openGLBufferBackup));
#endif
  }
}

namespace ImplementationDispatch
{


//[-------------------------------------------------------]
//[ Command buffer                                        ]
//[-------------------------------------------------------]
void DispatchCommandBuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DispatchCommandBuffer* realData = static_cast<const RERHI::Command::DispatchCommandBuffer*>(data);
  RHI_ASSERT(nullptr != realData->commandBufferToDispatch, "The OpenGL command buffer to dispatch must be valid")
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).dispatchCommandBufferInternal(*realData->commandBufferToDispatch);
}

//[-------------------------------------------------------]
//[ Graphics                                              ]
//[-------------------------------------------------------]
void SetGraphicsRootSignature(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsRootSignature* realData = static_cast<const RERHI::Command::SetGraphicsRootSignature*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setGraphicsRootSignature(realData->rootSignature);
}

void SetGraphicsPipelineState(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsPipelineState* realData = static_cast<const RERHI::Command::SetGraphicsPipelineState*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setGraphicsPipelineState(realData->graphicsPipelineState);
}

void SetGraphicsResourceGroup(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsResourceGroup* realData = static_cast<const RERHI::Command::SetGraphicsResourceGroup*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setGraphicsResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
}

void SetGraphicsVertexArray(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Input-assembler (IA) stage
  const RERHI::Command::SetGraphicsVertexArray* realData = static_cast<const RERHI::Command::SetGraphicsVertexArray*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setGraphicsVertexArray(realData->vertexArray);
}

void SetGraphicsViewports(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsViewports* realData = static_cast<const RERHI::Command::SetGraphicsViewports*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setGraphicsViewports(realData->numberOfViewports, (nullptr != realData->viewports) ? realData->viewports : reinterpret_cast<const RERHI::Viewport*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsScissorRectangles(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsScissorRectangles* realData = static_cast<const RERHI::Command::SetGraphicsScissorRectangles*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setGraphicsScissorRectangles(realData->numberOfScissorRectangles, (nullptr != realData->scissorRectangles) ? realData->scissorRectangles : reinterpret_cast<const RERHI::ScissorRectangle*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsRenderTarget(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Output-merger (OM) stage
  const RERHI::Command::SetGraphicsRenderTarget* realData = static_cast<const RERHI::Command::SetGraphicsRenderTarget*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setGraphicsRenderTarget(realData->renderTarget);
}

void ClearGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ClearGraphics* realData = static_cast<const RERHI::Command::ClearGraphics*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).clearGraphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
}

void DrawGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawGraphics* realData = static_cast<const RERHI::Command::DrawGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).drawGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).drawGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawIndexedGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawIndexedGraphics* realData = static_cast<const RERHI::Command::DrawIndexedGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).drawIndexedGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).drawIndexedGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawMeshTasks(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawMeshTasks* realData = static_cast<const RERHI::Command::DrawMeshTasks*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).drawMeshTasks(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).drawMeshTasksEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void SetComputeRootSignature(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputeRootSignature* realData = static_cast<const RERHI::Command::SetComputeRootSignature*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setComputeRootSignature(realData->rootSignature);
}

void SetComputePipelineState(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputePipelineState* realData = static_cast<const RERHI::Command::SetComputePipelineState*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setComputePipelineState(realData->computePipelineState);
}

void SetComputeResourceGroup(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputeResourceGroup* realData = static_cast<const RERHI::Command::SetComputeResourceGroup*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setComputeResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
}

void DispatchCompute(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DispatchCompute* realData = static_cast<const RERHI::Command::DispatchCompute*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).dispatchCompute(realData->groupCountX, realData->groupCountY, realData->groupCountZ);
}

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void SetTextureMinimumMaximumMipmapIndex(const void* data, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const RERHI::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
  RHI_ASSERT(static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).getContext(), realData->texture->getResourceType() == RERHI::ResourceType::TEXTURE_2D, "Unsupported OpenGL texture resource type")
  static_cast<RERHIOpenGL::Texture2D*>(realData->texture)->setMinimumMaximumMipmapIndex(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
}

void ResolveMultisampleFramebuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResolveMultisampleFramebuffer* realData = static_cast<const RERHI::Command::ResolveMultisampleFramebuffer*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).resolveMultisampleFramebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
}

void CopyResource(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::CopyResource* realData = static_cast<const RERHI::Command::CopyResource*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).copyResource(*realData->destinationResource, *realData->sourceResource);
}

void GenerateMipmaps(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::GenerateMipmaps* realData = static_cast<const RERHI::Command::GenerateMipmaps*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).generateMipmaps(*realData->resource);
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

void SetUniform(const void*, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  RHI_ASSERT(false, "The set uniform command isn't supported by the OpenGL RHI implementation")
}

//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void ResetQueryPool(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResetQueryPool* realData = static_cast<const RERHI::Command::ResetQueryPool*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).resetQueryPool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
}

void BeginQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::BeginQuery* realData = static_cast<const RERHI::Command::BeginQuery*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).beginQuery(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
}

void EndQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::EndQuery* realData = static_cast<const RERHI::Command::EndQuery*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).endQuery(*realData->queryPool, realData->queryIndex);
}

void WriteTimestampQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::WriteTimestampQuery* realData = static_cast<const RERHI::Command::WriteTimestampQuery*>(data);
  static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).writeTimestampQuery(*realData->queryPool, realData->queryIndex);
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void SetDebugMarker(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::SetDebugMarker* realData = static_cast<const RERHI::Command::SetDebugMarker*>(data);
					static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).setDebugMarker(realData->name);
				}

				void BeginDebugEvent(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::BeginDebugEvent* realData = static_cast<const RERHI::Command::BeginDebugEvent*>(data);
					static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).beginDebugEvent(realData->name);
				}

				void EndDebugEvent(const void*, RERHI::RHIDynamicRHI& rhi)
				{
					static_cast<RERHIOpenGL::RHIDynamicRHI&>(rhi).endDebugEvent();
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
namespace RERHIOpenGL {



//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIDynamicRHI::RHIDynamicRHI(const RERHI::RHIContext& context) :
  RERHI::RHIDynamicRHI(RERHI::NameId::OPENGL, context),
  VertexArrayMakeId(),
  GraphicsPipelineStateMakeId(),
  ComputePipelineStateMakeId(),
  mOpenGLRuntimeLinking(nullptr),
  mOpenGLContext(nullptr),
  mExtensions(nullptr),
  mShaderLanguage(nullptr),
  mGraphicsRootSignature(nullptr),
  mComputeRootSignature(nullptr),
  mDefaultSamplerState(nullptr),
  mOpenGLCopyResourceFramebuffer(0),
  mDefaultOpenGLVertexArray(0),
  // States
  mGraphicsPipelineState(nullptr),
  mComputePipelineState(nullptr),
  // Input-assembler (IA) stage
  mVertexArray(nullptr),
  mOpenGLPrimitiveTopology(0xFFFF),	// Unknown default setting
  mNumberOfVerticesPerPatch(0),
  // Output-merger (OM) stage
  mRenderTarget(nullptr),
  // State cache to avoid making redundant OpenGL calls
  mOpenGLClipControlOrigin(GL_INVALID_ENUM),
  mOpenGLProgramPipeline(0),
  mOpenGLProgram(0),
  mOpenGLIndirectBuffer(0),
  // Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
  mOpenGLVertexProgram(0),
  mDrawIdUniformLocation(-1),
  mCurrentStartInstanceLocation(~0u)
{
  // Is OpenGL available?
  mOpenGLRuntimeLinking = RHI_NEW(mContext, OpenGLRuntimeLinking)(*this);
  if (mOpenGLRuntimeLinking->isOpenGLAvaiable())
  {
    const RECore::handle nativeWindowHandle = mContext.getNativeWindowHandle();
    const RERHI::TextureFormat::Enum textureFormat = RERHI::TextureFormat::Enum::R8G8B8A8;
    const RenderPass renderPass(*this, 1, &textureFormat, RERHI::TextureFormat::Enum::UNKNOWN, 1 RHI_RESOURCE_DEBUG_NAME("OpenGL Unknown"));
#ifdef _WIN32
    {
				// TODO(naetherm) Add external OpenGL context support
				mOpenGLContext = RHI_NEW(mContext, OpenGLContextWindows)(mOpenGLRuntimeLinking, renderPass.getDepthStencilAttachmentTextureFormat(), nativeWindowHandle);
			}
#elif defined LINUX
    mOpenGLContext = RHI_NEW(mContext, OpenGLContextLinux)(*this, mOpenGLRuntimeLinking, renderPass.getDepthStencilAttachmentTextureFormat(), nativeWindowHandle, mContext.isUsingExternalContext());
#else
#error "Unsupported platform"
#endif

    // We're using "this" in here, so we are not allowed to write the following within the initializer list
    mExtensions = RHI_NEW(mContext, Extensions)(*this, *mOpenGLContext);

    // Is the OpenGL context and extensions initialized?
    if (mOpenGLContext->isInitialized() && mExtensions->initialize())
    {
#ifdef DEBUG
      // "GL_ARB_debug_output"-extension available?
					if (mExtensions->isGL_ARB_debug_output())
					{
						// Synchronous debug output, please
						// -> Makes it easier to find the place causing the issue
						glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

						// Disable severity notifications, most drivers print many things with this severity
						glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, false);

						// Set the debug message callback function
						glDebugMessageCallbackARB(&RHIDynamicRHI::debugMessageCallback, this);
					}
#endif

      // Globally enable seamless cube map texture, e.g. Direct3D 11 has this enabled by default so do the same for OpenGL
      // -> The following is just for the sake of completeness: It's the year 2020 and OpenGL on Mac is officially dead. But if someone would still
      //    want to support it in a productive way, one has to take care of the situation that enabling seamless cube map texture can result on
      //    slow software rendering on Mac. For checking whether or not this is the case, see "GL_TEXTURE_CUBE_MAP_SEAMLESS on OS X" published at April 26, 2012 on http://distrustsimplicity.net/articles/gl_texture_cube_map_seamless-on-os-x/
      //    "
      //    GLint gpuVertex, gpuFragment;
      //    CGLGetParameter(CGLGetCurrentContext(), kCGLCPGPUVertexProcessing, &gpuVertex);
      //    CGLGetParameter(CGLGetCurrentContext(), kCGLCPGPUFragmentProcessing, &gpuFragment);
      //    "
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

      // Initialize the capabilities
      initializeCapabilities();

      // Create the default sampler state
      mDefaultSamplerState = createSamplerState(RERHI::RHISamplerState::getDefaultSamplerState());

      // Create default OpenGL vertex array
      if (mExtensions->isGL_ARB_vertex_array_object())
      {
        glGenVertexArrays(1, &mDefaultOpenGLVertexArray);
        glBindVertexArray(mDefaultOpenGLVertexArray);
      }

      // Add references to the default sampler state and set it
      if (nullptr != mDefaultSamplerState)
      {
        mDefaultSamplerState->AddReference();
        // TODO(naetherm) Set default sampler states
      }
    }
  }
}

RHIDynamicRHI::~RHIDynamicRHI()
{
  // Set no graphics and compute pipeline state reference, in case we have one
  if (nullptr != mGraphicsPipelineState)
  {
    setGraphicsPipelineState(nullptr);
  }
  if (nullptr != mComputePipelineState)
  {
    setComputePipelineState(nullptr);
  }

  // Set no vertex array reference, in case we have one
  if (nullptr != mVertexArray)
  {
    setGraphicsVertexArray(nullptr);
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

  // Destroy the OpenGL framebuffer used by "RHIDynamicRHI::RHIDynamicRHI::copyResource()" if the "GL_ARB_copy_image"-extension isn't available
  // -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteFramebuffers(1, &mOpenGLCopyResourceFramebuffer);

  // Destroy the OpenGL default vertex array
  // -> Silently ignores 0's and names that do not correspond to existing vertex array objects
  glDeleteVertexArrays(1, &mDefaultOpenGLVertexArray);

  // Release the graphics and compute root signature instance, in case we have one
  if (nullptr != mGraphicsRootSignature)
  {
    mGraphicsRootSignature->Release();
  }
  if (nullptr != mComputeRootSignature)
  {
    mComputeRootSignature->Release();
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
					RHI_ASSERT(false, "The OpenGL RHI implementation is going to be destroyed, but there are still %lu resource instances left (memory leak)", numberOfCurrentResources)
				}
				else
				{
					RHI_ASSERT(false, "The OpenGL RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
				}

				// Use debug output to show the current number of resource instances
				getStatistics().debugOutputCurrentResouces(mContext);
			}
		}
#endif

  // Release the shader language instance, in case we have one
  if (nullptr != mShaderLanguage)
  {
    mShaderLanguage->Release();
  }

  // Destroy the extensions instance
  RHI_DELETE(mContext, Extensions, mExtensions);

  // Destroy the OpenGL context instance
  RHI_DELETE(mContext, RHIOpenGLContext, mOpenGLContext);

  // Destroy the OpenGL runtime linking instance
  RHI_DELETE(mContext, OpenGLRuntimeLinking, mOpenGLRuntimeLinking);
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
//[ Graphics                                              ]
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

      // Set OpenGL primitive topology
      mOpenGLPrimitiveTopology = mGraphicsPipelineState->getOpenGLPrimitiveTopology();
      const int newNumberOfVerticesPerPatch = mGraphicsPipelineState->getNumberOfVerticesPerPatch();
      if (0 != newNumberOfVerticesPerPatch && mNumberOfVerticesPerPatch != newNumberOfVerticesPerPatch)
      {
        mNumberOfVerticesPerPatch = newNumberOfVerticesPerPatch;
        glPatchParameteri(GL_PATCH_VERTICES, mNumberOfVerticesPerPatch);
      }

      // Set graphics pipeline state
      mGraphicsPipelineState->bindGraphicsPipelineState();
    }
    else if (nullptr != mGraphicsPipelineState)
    {
      // TODO(naetherm) Handle this situation by resetting OpenGL states?
      mGraphicsPipelineState->Release();
      mGraphicsPipelineState = nullptr;
    }
  }

    // Set graphics pipeline state
  else if (nullptr != mGraphicsPipelineState)
  {
    // Set OpenGL graphics pipeline state
    // -> This is necessary since OpenGL is using just a single current program, for graphics as well as compute
    setOpenGLGraphicsProgram(mGraphicsPipelineState->getGraphicsProgram());
  }
}

void RHIDynamicRHI::setGraphicsResourceGroup(RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  // Security checks
#ifdef DEBUG
  {
			RHI_ASSERT(nullptr != mGraphicsRootSignature, "No OpenGL RHI implementation graphics root signature set")
			const RERHI::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
			RHI_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The OpenGL RHI implementation root parameter index is out of bounds")
			const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			RHI_ASSERT(RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The OpenGL RHI implementation root parameter index doesn't reference a descriptor table")
			RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The OpenGL RHI implementation descriptor ranges is a null pointer")
		}
#endif

  // Set graphics resource group
  setResourceGroup(*mGraphicsRootSignature, rootParameterIndex, resourceGroup);
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

      // Unset the currently used vertex array
      unsetGraphicsVertexArray();

      // Set new vertex array and add a reference to it
      mVertexArray = static_cast<VertexArray*>(vertexArray);
      mVertexArray->AddReference();

      // Evaluate the internal array type of the new vertex array to set
      switch (static_cast<VertexArray*>(mVertexArray)->getInternalResourceType())
      {
        case VertexArray::InternalResourceType::NO_VAO:
          // Enable OpenGL vertex attribute arrays
          static_cast<VertexArrayNoVao*>(mVertexArray)->enableOpenGLVertexAttribArrays();
          break;

        case VertexArray::InternalResourceType::VAO:
          // Bind OpenGL vertex array
          glBindVertexArray(static_cast<VertexArrayVao*>(mVertexArray)->getOpenGLVertexArray());
          break;
      }
    }
    else
    {
      // Unset the currently used vertex array
      unsetGraphicsVertexArray();
    }
  }
}

void RHIDynamicRHI::setGraphicsViewports([[maybe_unused]] RECore::uint32 numberOfViewports, const RERHI::Viewport* viewports)
{
  // Rasterizer (RS) stage

  // Sanity check
  RHI_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid OpenGL rasterizer state viewports")

  // In OpenGL, the origin of the viewport is left bottom while Direct3D is using a left top origin. To make the
  // Direct3D 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
  // -> This isn't influenced by the "GL_ARB_clip_control"-extension

  // Get the width and height of the current render target
  RECore::uint32 renderTargetHeight = 1;
  if (nullptr != mRenderTarget)
  {
    RECore::uint32 renderTargetWidth = 1;
    mRenderTarget->getWidthAndHeight(renderTargetWidth, renderTargetHeight);
  }

  // Set the OpenGL viewport
  // TODO(naetherm) "GL_ARB_viewport_array" support ("RHIDynamicRHI::setGraphicsViewports()")
  // TODO(naetherm) Check for "numberOfViewports" out of range or are the debug events good enough?
  RHI_ASSERT(numberOfViewports <= 1, "OpenGL supports only one viewport")
  glViewport(static_cast<GLint>(viewports->topLeftX), static_cast<GLint>(static_cast<float>(renderTargetHeight) - viewports->topLeftY - viewports->height), static_cast<GLsizei>(viewports->width), static_cast<GLsizei>(viewports->height));
  glDepthRange(static_cast<GLclampd>(viewports->minDepth), static_cast<GLclampd>(viewports->maxDepth));
}

void RHIDynamicRHI::setGraphicsScissorRectangles([[maybe_unused]] RECore::uint32 numberOfScissorRectangles, const RERHI::ScissorRectangle* scissorRectangles)
{
  // Rasterizer (RS) stage

  // Sanity check
  RHI_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid OpenGL rasterizer state scissor rectangles")

  // In OpenGL, the origin of the scissor rectangle is left bottom while Direct3D is using a left top origin. To make the
  // Direct3D 9 & 10 & 11 implementation as efficient as possible the Direct3D convention is used and we have to convert in here.
  // -> This isn't influenced by the "GL_ARB_clip_control"-extension

  // Get the width and height of the current render target
  RECore::uint32 renderTargetHeight = 1;
  if (nullptr != mRenderTarget)
  {
    RECore::uint32 renderTargetWidth = 1;
    mRenderTarget->getWidthAndHeight(renderTargetWidth, renderTargetHeight);
  }

  // Set the OpenGL scissor rectangle
  // TODO(naetherm) "GL_ARB_viewport_array" support ("RHIDynamicRHI::setGraphicsViewports()")
  // TODO(naetherm) Check for "numberOfViewports" out of range or are the debug events good enough?
  RHI_ASSERT(numberOfScissorRectangles <= 1, "OpenGL supports only one scissor rectangle")
  const GLsizei width  = scissorRectangles->bottomRightX - scissorRectangles->topLeftX;
  const GLsizei height = scissorRectangles->bottomRightY - scissorRectangles->topLeftY;
  glScissor(static_cast<GLint>(scissorRectangles->topLeftX), static_cast<GLint>(renderTargetHeight - static_cast<RECore::uint32>(scissorRectangles->topLeftY) - height), width, height);
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
        // Unbind OpenGL framebuffer?
        if (RERHI::ResourceType::FRAMEBUFFER == mRenderTarget->getResourceType() && RERHI::ResourceType::FRAMEBUFFER != renderTarget->getResourceType())
        {
          // Do we need to disable multisample?
          if (static_cast<Framebuffer*>(mRenderTarget)->isMultisampleRenderTarget())
          {
            glDisable(GL_MULTISAMPLE);
          }

          // We do not render into a OpenGL framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // Release
        mRenderTarget->Release();
      }

      // Set new render target and add a reference to it
      mRenderTarget = renderTarget;
      mRenderTarget->AddReference();

      // Evaluate the render target type
      GLenum clipControlOrigin = GL_UPPER_LEFT;
      switch (mRenderTarget->getResourceType())
      {
        case RERHI::ResourceType::SWAP_CHAIN:
        {
          static_cast<SwapChain*>(mRenderTarget)->getOpenGLContext().makeCurrent();
          clipControlOrigin = GL_LOWER_LEFT;	// Compensate OS window coordinate system y-flip
          break;
        }

        case RERHI::ResourceType::FRAMEBUFFER:
        {
          // Get the OpenGL framebuffer instance
          Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);

          // Bind the OpenGL framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getOpenGLFramebuffer());

          // Define the OpenGL buffers to draw into, "GL_ARB_draw_buffers"-extension required
          if (mExtensions->isGL_ARB_draw_buffers())
          {
            // https://www.opengl.org/registry/specs/ARB/draw_buffers.txt - "The draw buffer for output colors beyond <n> is set to NONE."
            // -> Meaning depth only rendering which has no color textures at all will work as well, no need for "glDrawBuffer(GL_NONE)"
            static constexpr GLenum OPENGL_DRAW_BUFFER[16] =
              {
                GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,  GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,
                GL_COLOR_ATTACHMENT8,  GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
                GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15
              };
            glDrawBuffersARB(static_cast<GLsizei>(framebuffer->getNumberOfColorTextures()), OPENGL_DRAW_BUFFER);
          }

          // Do we need to enable multisample?
          if (framebuffer->isMultisampleRenderTarget())
          {
            glEnable(GL_MULTISAMPLE);
          }
          else
          {
            glDisable(GL_MULTISAMPLE);
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
      if (mOpenGLClipControlOrigin != clipControlOrigin && mExtensions->isGL_ARB_clip_control())
      {
        // OpenGL default is "GL_LOWER_LEFT" and "GL_NEGATIVE_ONE_TO_ONE", change it to match Vulkan and Direct3D
        mOpenGLClipControlOrigin = clipControlOrigin;
        glClipControl(mOpenGLClipControlOrigin, GL_ZERO_TO_ONE);
      }
    }
    else if (nullptr != mRenderTarget)
    {
      // Evaluate the render target type
      if (RERHI::ResourceType::FRAMEBUFFER == mRenderTarget->getResourceType())
      {
        // We do not render into a OpenGL framebuffer
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
  RHI_ASSERT(z >= 0.0f && z <= 1.0f, "The OpenGL clear graphics z value must be between [0, 1] (inclusive)")

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
      glClearDepth(static_cast<GLclampd>(z));
      if (nullptr != mGraphicsPipelineState && RERHI::DepthWriteMask::ALL != mGraphicsPipelineState->getDepthStencilState().depthWriteMask)
      {
        glDepthMask(GL_TRUE);
      }
    }
    if (clearFlags & RERHI::ClearFlag::STENCIL)
    {
      glClearStencil(static_cast<GLint>(stencil));
    }

    // Unlike OpenGL, when using Direct3D 10 & 11 the scissor rectangle(s) do not affect the clear operation
    // -> We have to compensate the OpenGL behaviour in here

    // Disable OpenGL scissor test, in case it's not disabled, yet
    if (nullptr != mGraphicsPipelineState && mGraphicsPipelineState->getRasterizerState().scissorEnable)
    {
      glDisable(GL_SCISSOR_TEST);
    }

    // Clear
    glClear(flagsApi);

    // Restore the previously set OpenGL states
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

void RHIDynamicRHI::drawGraphics(const RERHI::RHIIndirectBuffer& indirectBuffer, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, indirectBuffer)
  RHI_ASSERT(numberOfDraws > 0, "Number of OpenGL draws must not be zero")
  RHI_ASSERT(mExtensions->isGL_ARB_draw_indirect(), "The GL_ARB_draw_indirect OpenGL extension isn't supported")
  // It's possible to draw without "mVertexArray"

  // Tessellation support: "glPatchParameteri()" is called within "RHIDynamicRHI::iaSetPrimitiveTopology()"

  { // Bind indirect buffer
    const GLuint openGLIndirectBuffer = static_cast<const IndirectBuffer&>(indirectBuffer).getOpenGLIndirectBuffer();
    if (openGLIndirectBuffer != mOpenGLIndirectBuffer)
    {
      mOpenGLIndirectBuffer = openGLIndirectBuffer;
      glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, mOpenGLIndirectBuffer);
    }
  }

  // Draw indirect
  if (1 == numberOfDraws)
  {
    glDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
  }
  else if (numberOfDraws > 1)
  {
    if (mExtensions->isGL_ARB_multi_draw_indirect())
    {
      glMultiDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)), static_cast<GLsizei>(numberOfDraws), 0);	// 0 = tightly packed
    }
    else
    {
      // Emulate multi-draw-indirect
#ifdef DEBUG
      beginDebugEvent("Multi-draw-indirect emulation");
#endif
      for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
      {
        glDrawArraysIndirect(mOpenGLPrimitiveTopology, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
        indirectBufferOffset += sizeof(RERHI::DrawArguments);
      }
#ifdef DEBUG
      endDebugEvent();
#endif
    }
  }
}

void RHIDynamicRHI::drawGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")
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
    updateGL_ARB_base_instanceEmulation(drawArguments.startInstanceLocation);

    // Draw and advance
    if ((drawArguments.instanceCount > 1 && mExtensions->isGL_ARB_draw_instanced()) || (drawArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance()))
    {
      // With instancing
      if (drawArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())
      {
        glDrawArraysInstancedBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount), drawArguments.startInstanceLocation);
      }
      else
      {
        glDrawArraysInstancedARB(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance), static_cast<GLsizei>(drawArguments.instanceCount));
      }
    }
    else
    {
      // Without instancing
      RHI_ASSERT(drawArguments.instanceCount <= 1, "Invalid OpenGL instance count")
      glDrawArrays(mOpenGLPrimitiveTopology, static_cast<GLint>(drawArguments.startVertexLocation), static_cast<GLsizei>(drawArguments.vertexCountPerInstance));
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

void RHIDynamicRHI::drawIndexedGraphics(const RERHI::RHIIndirectBuffer& indirectBuffer, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, indirectBuffer)
  RHI_ASSERT(numberOfDraws > 0, "Number of OpenGL draws must not be zero")
  RHI_ASSERT(nullptr != mVertexArray, "OpenGL draw indexed needs a set vertex array")
  RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "OpenGL draw indexed needs a set vertex array which contains an index buffer")
  RHI_ASSERT(mExtensions->isGL_ARB_draw_indirect(), "The GL_ARB_draw_indirect OpenGL extension isn't supported")

  // Tessellation support: "glPatchParameteri()" is called within "RHIDynamicRHI::iaSetPrimitiveTopology()"

  { // Bind indirect buffer
    const GLuint openGLIndirectBuffer = static_cast<const IndirectBuffer&>(indirectBuffer).getOpenGLIndirectBuffer();
    if (openGLIndirectBuffer != mOpenGLIndirectBuffer)
    {
      mOpenGLIndirectBuffer = openGLIndirectBuffer;
      glBindBufferARB(GL_DRAW_INDIRECT_BUFFER, mOpenGLIndirectBuffer);
    }
  }

  // Draw indirect
  if (1 == numberOfDraws)
  {
    glDrawElementsIndirect(mOpenGLPrimitiveTopology, mVertexArray->getIndexBuffer()->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
  }
  else if (numberOfDraws > 1)
  {
    if (mExtensions->isGL_ARB_multi_draw_indirect())
    {
      glMultiDrawElementsIndirect(mOpenGLPrimitiveTopology, mVertexArray->getIndexBuffer()->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)), static_cast<GLsizei>(numberOfDraws), 0);	// 0 = tightly packed
    }
    else
    {
      // Emulate multi-indexed-draw-indirect
#ifdef DEBUG
      beginDebugEvent("Multi-indexed-draw-indirect emulation");
#endif
      const RECore::uint32 openGLType = mVertexArray->getIndexBuffer()->getOpenGLType();
      for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
      {
        glDrawElementsIndirect(mOpenGLPrimitiveTopology, openGLType, reinterpret_cast<void*>(static_cast<uintptr_t>(indirectBufferOffset)));
        indirectBufferOffset += sizeof(RERHI::DrawIndexedArguments);
      }
#ifdef DEBUG
      endDebugEvent();
#endif
    }
  }
}

void RHIDynamicRHI::drawIndexedGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")
  RHI_ASSERT(nullptr != mVertexArray, "OpenGL draw indexed needs a set vertex array")
  RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "OpenGL draw indexed needs a set vertex array which contains an index buffer")

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
    updateGL_ARB_base_instanceEmulation(drawIndexedArguments.startInstanceLocation);

    // Draw and advance
    if ((drawIndexedArguments.instanceCount > 1 && mExtensions->isGL_ARB_draw_instanced()) || (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance()))
    {
      // With instancing
      if (drawIndexedArguments.baseVertexLocation > 0)
      {
        // Use start instance location?
        if (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())
        {
          // Draw with base vertex location and start instance location
          glDrawElementsInstancedBaseVertexBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation), drawIndexedArguments.startInstanceLocation);
        }

          // Is the "GL_ARB_draw_elements_base_vertex" extension there?
        else if (mExtensions->isGL_ARB_draw_elements_base_vertex())
        {
          // Draw with base vertex location
          glDrawElementsInstancedBaseVertex(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
        }
        else
        {
          // Error!
          RHI_ASSERT(false, "Failed to OpenGL draw indexed emulated")
        }
      }
      else if (drawIndexedArguments.startInstanceLocation > 0 && mExtensions->isGL_ARB_base_instance())
      {
        // Draw without base vertex location and with start instance location
        glDrawElementsInstancedBaseInstance(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount), drawIndexedArguments.startInstanceLocation);
      }
      else
      {
        // Draw without base vertex location
        glDrawElementsInstancedARB(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLsizei>(drawIndexedArguments.instanceCount));
      }
    }
    else
    {
      // Without instancing
      RHI_ASSERT(drawIndexedArguments.instanceCount <= 1, "Invalid OpenGL instance count")

      // Use base vertex location?
      if (drawIndexedArguments.baseVertexLocation > 0)
      {
        // Is the "GL_ARB_draw_elements_base_vertex" extension there?
        if (mExtensions->isGL_ARB_draw_elements_base_vertex())
        {
          // Draw with base vertex location
          glDrawElementsBaseVertex(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())), static_cast<GLint>(drawIndexedArguments.baseVertexLocation));
        }
        else
        {
          // Error!
          RHI_ASSERT(false, "Failed to OpenGL draw indexed emulated")
        }
      }
      else
      {
        // Draw without base vertex location
        glDrawElements(mOpenGLPrimitiveTopology, static_cast<GLsizei>(drawIndexedArguments.indexCountPerInstance), indexBuffer->getOpenGLType(), reinterpret_cast<void*>(static_cast<uintptr_t>(drawIndexedArguments.startIndexLocation * indexBuffer->getIndexSizeInBytes())));
      }
    }
    emulationData += sizeof(RERHI::DrawIndexedArguments);
  }
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
#endif
}

void RHIDynamicRHI::drawMeshTasks([[maybe_unused]] const RERHI::RHIIndirectBuffer& indirectBuffer, [[maybe_unused]] RECore::uint32 indirectBufferOffset, [[maybe_unused]] RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")

  // TODO(naetherm) Implement me
  /*
  void MultiDrawMeshTasksIndirectNV(intptr indirect,
                  sizei drawcount,
                  sizei stride);

  void MultiDrawMeshTasksIndirectCountNV( intptr indirect,
                      intptr drawcount,
                      sizei maxdrawcount,
                      sizei stride);
  */
}

void RHIDynamicRHI::drawMeshTasksEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The OpenGL emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of OpenGL draws must not be zero")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
#endif
  for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
  {
    const RERHI::DrawMeshTasksArguments& drawMeshTasksArguments = *reinterpret_cast<const RERHI::DrawMeshTasksArguments*>(emulationData);

    // Draw and advance
    glDrawMeshTasksNV(drawMeshTasksArguments.firstTask, drawMeshTasksArguments.numberOfTasks);
    emulationData += sizeof(RERHI::DrawMeshTasksArguments);
  }
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				endDebugEvent();
			}
#endif
}


//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void RHIDynamicRHI::setComputeRootSignature(RERHI::RHIRootSignature* rootSignature)
{
  if (nullptr != mComputeRootSignature)
  {
    mComputeRootSignature->Release();
  }
  mComputeRootSignature = static_cast<RootSignature*>(rootSignature);
  if (nullptr != mComputeRootSignature)
  {
    mComputeRootSignature->AddReference();

    // Sanity check
    RHI_MATCH_CHECK(*this, *rootSignature)
  }
}

void RHIDynamicRHI::setComputePipelineState(RERHI::RHIComputePipelineState* computePipelineState)
{
  if (mComputePipelineState != computePipelineState)
  {
    if (nullptr != computePipelineState)
    {
      // Sanity check
      RHI_MATCH_CHECK(*this, *computePipelineState)

      // Set new compute pipeline state and add a reference to it
      if (nullptr != mComputePipelineState)
      {
        mComputePipelineState->Release();
      }
      mComputePipelineState = static_cast<ComputePipelineState*>(computePipelineState);
      mComputePipelineState->AddReference();

      // Set OpenGL compute pipeline state
      setOpenGLComputePipelineState(mComputePipelineState);
    }
    else if (nullptr != mComputePipelineState)
    {
      // TODO(naetherm) Handle this situation by resetting OpenGL states?
      mComputePipelineState->Release();
      mComputePipelineState = nullptr;
    }
  }

    // Set compute pipeline state
  else if (nullptr != mComputePipelineState)
  {
    // Set OpenGL compute pipeline state
    // -> This is necessary since OpenGL is using just a single current program, for graphics as well as compute
    setOpenGLComputePipelineState(mComputePipelineState);
  }
}

void RHIDynamicRHI::setComputeResourceGroup(RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  // Security checks
#ifdef DEBUG
  {
			RHI_ASSERT(nullptr != mComputeRootSignature, "No OpenGL RHI implementation compute root signature set")
			const RERHI::RootSignature& rootSignature = mComputeRootSignature->getRootSignature();
			RHI_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The OpenGL RHI implementation root parameter index is out of bounds")
			const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			RHI_ASSERT(RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The OpenGL RHI implementation root parameter index doesn't reference a descriptor table")
			RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The OpenGL RHI implementation descriptor ranges is a null pointer")
		}
#endif

  // Set compute resource group
  setResourceGroup(*mComputeRootSignature, rootParameterIndex, resourceGroup);
}

void RHIDynamicRHI::dispatchCompute(RECore::uint32 groupCountX, RECore::uint32 groupCountY, RECore::uint32 groupCountZ)
{
  // "GL_ARB_compute_shader"-extension required
  if (mExtensions->isGL_ARB_compute_shader())
  {
    glDispatchCompute(groupCountX, groupCountY, groupCountZ);

    // TODO(naetherm) Compute shader: Memory barrier currently fixed build in: Make sure writing to image has finished before read
    glMemoryBarrierEXT(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glMemoryBarrierEXT(GL_SHADER_STORAGE_BARRIER_BIT);
  }
}


//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resolveMultisampleFramebuffer(RERHI::RHIRenderTarget& destinationRenderTarget, RERHI::RHIFramebuffer& sourceMultisampleFramebuffer)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, destinationRenderTarget)
  RHI_MATCH_CHECK(*this, sourceMultisampleFramebuffer)

  // Evaluate the render target type
  switch (destinationRenderTarget.getResourceType())
  {
    case RERHI::ResourceType::SWAP_CHAIN:
    {
      // Get the OpenGL swap chain instance
      // TODO(naetherm) Implement me, not that important in practice so not directly implemented
      // SwapChain& swapChain = static_cast<SwapChain&>(destinationRenderTarget);
      break;
    }

    case RERHI::ResourceType::FRAMEBUFFER:
    {
      // Get the OpenGL framebuffer instances
      const Framebuffer& openGLDestinationFramebuffer = static_cast<const Framebuffer&>(destinationRenderTarget);
      const Framebuffer& openGLSourceMultisampleFramebuffer = static_cast<const Framebuffer&>(sourceMultisampleFramebuffer);

      // Get the width and height of the destination and source framebuffer
      RECore::uint32 destinationWidth = 1;
      RECore::uint32 destinationHeight = 1;
      openGLDestinationFramebuffer.getWidthAndHeight(destinationWidth, destinationHeight);
      RECore::uint32 sourceWidth = 1;
      RECore::uint32 sourceHeight = 1;
      openGLSourceMultisampleFramebuffer.getWidthAndHeight(sourceWidth, sourceHeight);

      // Resolve multisample
      glBindFramebuffer(GL_READ_FRAMEBUFFER, openGLSourceMultisampleFramebuffer.getOpenGLFramebuffer());
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, openGLDestinationFramebuffer.getOpenGLFramebuffer());
      glBlitFramebuffer(
        0, 0, static_cast<GLint>(sourceWidth), static_cast<GLint>(sourceHeight),			// Source
        0, 0, static_cast<GLint>(destinationWidth), static_cast<GLint>(destinationHeight),	// Destination
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
        // Get the OpenGL texture 2D instances
        const Texture2D& openGlDestinationTexture2D = static_cast<const Texture2D&>(destinationResource);
        const Texture2D& openGlSourceTexture2D = static_cast<const Texture2D&>(sourceResource);
        RHI_ASSERT(openGlDestinationTexture2D.getWidth() == openGlSourceTexture2D.getWidth(), "OpenGL source and destination width must be identical for resource copy")
        RHI_ASSERT(openGlDestinationTexture2D.getHeight() == openGlSourceTexture2D.getHeight(), "OpenGL source and destination height must be identical for resource copy")

        // Copy resource, but only the top-level mipmap
        const GLsizei width = static_cast<GLsizei>(openGlDestinationTexture2D.getWidth());
        const GLsizei height = static_cast<GLsizei>(openGlDestinationTexture2D.getHeight());
        if (mExtensions->isGL_ARB_copy_image())
        {
          glCopyImageSubData(openGlSourceTexture2D.getOpenGLTexture(), GL_TEXTURE_2D, 0, 0, 0, 0,
                             openGlDestinationTexture2D.getOpenGLTexture(), GL_TEXTURE_2D, 0, 0, 0, 0,
                             width, height, 1);
        }
        else
        {
#ifdef RHI_OPENGL_STATE_CLEANUP
          // Backup the currently bound OpenGL framebuffer
							GLint openGLFramebufferBackup = 0;
							glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLFramebufferBackup);
#endif

          // Copy resource by using a framebuffer, but only the top-level mipmap
          if (0 == mOpenGLCopyResourceFramebuffer)
          {
            glGenFramebuffers(1, &mOpenGLCopyResourceFramebuffer);
          }
          glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLCopyResourceFramebuffer);
          glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, openGlSourceTexture2D.getOpenGLTexture(), 0);
          glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, openGlDestinationTexture2D.getOpenGLTexture(), 0);
          static constexpr GLenum OPENGL_DRAW_BUFFER[1] =
            {
              GL_COLOR_ATTACHMENT1
            };
          glDrawBuffersARB(1, OPENGL_DRAW_BUFFER);	// We could use "glDrawBuffer(GL_COLOR_ATTACHMENT1);", but then we would also have to get the "glDrawBuffer()" function pointer, avoid OpenGL function overkill
          glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

#ifdef RHI_OPENGL_STATE_CLEANUP
          // Be polite and restore the previous bound OpenGL framebuffer
							glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLFramebufferBackup));
#endif
        }
      }
      else
      {
        // Error!
        RHI_ASSERT(false, "Failed to copy OpenGL resource")
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
  RHI_ASSERT(resource.getResourceType() == RERHI::ResourceType::TEXTURE_2D, "TODO(naetherm) Mipmaps can only be generated for OpenGL 2D texture resources")

  Texture2D& texture2D = static_cast<Texture2D&>(resource);

  // Is "GL_EXT_direct_state_access" there?
  if (mExtensions->isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    glGenerateTextureMipmap(texture2D.getOpenGLTexture());
  }
  else if (mExtensions->isGL_EXT_direct_state_access())
  {
    // Effective direct state access (DSA)
    glGenerateTextureMipmapEXT(texture2D.getOpenGLTexture(), GL_TEXTURE_2D);
  }
  else
  {
    // Traditional bind version

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL texture
				// TODO(naetherm) It's possible to avoid calling this multiple times
				GLint openGLTextureBackup = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLTextureBackup);
#endif

    // Generate mipmaps
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, texture2D.getOpenGLTexture());
    glGenerateMipmap(GL_TEXTURE_2D);

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL texture
				glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLTextureBackup));
#endif
  }
}


//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resetQueryPool([[maybe_unused]] RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 firstQueryIndex, [[maybe_unused]] RECore::uint32 numberOfQueries)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, queryPool)
  RHI_ASSERT(firstQueryIndex < static_cast<const QueryPool&>(queryPool).getNumberOfQueries(), "OpenGL out-of-bounds query index")
  RHI_ASSERT((firstQueryIndex + numberOfQueries) <= static_cast<const QueryPool&>(queryPool).getNumberOfQueries(), "OpenGL out-of-bounds query index")

  // Nothing to do in here for OpenGL
}

void RHIDynamicRHI::beginQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex, RECore::uint32)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Query pool type dependent processing
  const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
  RHI_ASSERT(queryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
  switch (openGLQueryPool.getQueryType())
  {
    case RERHI::QueryType::OCCLUSION:
      // At this point in time we know that the "GL_ARB_occlusion_query"-extension is supported
      glBeginQueryARB(GL_SAMPLES_PASSED_ARB, static_cast<const OcclusionTimestampQueryPool&>(openGLQueryPool).getOpenGLQueries()[queryIndex]);
      break;

    case RERHI::QueryType::PIPELINE_STATISTICS:
      // At this point in time we know that the "GL_ARB_pipeline_statistics_query"-extension is supported
      static_cast<const PipelineStatisticsQueryPool&>(openGLQueryPool).beginQuery(queryIndex);
      break;

    case RERHI::QueryType::TIMESTAMP:
      RHI_ASSERT(false, "OpenGL begin query isn't allowed for timestamp queries, use \"Rhi::Command::WriteTimestampQuery\" instead")
      break;
  }
}

void RHIDynamicRHI::endQuery(RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 queryIndex)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Query pool type dependent processing
  const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
  RHI_ASSERT(queryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
  switch (openGLQueryPool.getQueryType())
  {
    case RERHI::QueryType::OCCLUSION:
      // At this point in time we know that the "GL_ARB_occlusion_query"-extension is supported
      glEndQueryARB(GL_SAMPLES_PASSED_ARB);
      break;

    case RERHI::QueryType::PIPELINE_STATISTICS:
      // At this point in time we know that the "GL_ARB_pipeline_statistics_query"-extension is supported
      static_cast<const PipelineStatisticsQueryPool&>(openGLQueryPool).endQuery();
      break;

    case RERHI::QueryType::TIMESTAMP:
      RHI_ASSERT(false, "OpenGL end query isn't allowed for timestamp queries, use \"Rhi::Command::WriteTimestampQuery\" instead")
      break;
  }
}

void RHIDynamicRHI::writeTimestampQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Query pool type dependent processing
  const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
  RHI_ASSERT(queryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
  switch (openGLQueryPool.getQueryType())
  {
    case RERHI::QueryType::OCCLUSION:
      RHI_ASSERT(false, "OpenGL write timestamp query isn't allowed for occlusion queries, use \"Rhi::Command::BeginQuery\" and \"Rhi::Command::EndQuery\" instead")
      break;

    case RERHI::QueryType::PIPELINE_STATISTICS:
      RHI_ASSERT(false, "OpenGL write timestamp query isn't allowed for pipeline statistics queries, use \"Rhi::Command::BeginQuery\" and \"Rhi::Command::EndQuery\" instead")
      break;

    case RERHI::QueryType::TIMESTAMP:
      // At this point in time we know that the "GL_ARB_timer_query"-extension is supported
      glQueryCounter(static_cast<const OcclusionTimestampQueryPool&>(openGLQueryPool).getOpenGLQueries()[queryIndex], GL_TIMESTAMP);
      break;
  }
}


//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDynamicRHI::setDebugMarker(const char* name)
		{
			// "GL_KHR_debug"-extension required
			if (mExtensions->isGL_KHR_debug())
			{
				RHI_ASSERT(nullptr != name, "OpenGL debug marker names must not be a null pointer")
				glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, -1, name);
			}
		}

		void RHIDynamicRHI::beginDebugEvent(const char* name)
		{
			// "GL_KHR_debug"-extension required
			if (mExtensions->isGL_KHR_debug())
			{
				RHI_ASSERT(nullptr != name, "OpenGL debug event names must not be a null pointer")
				glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, name);
			}
		}

		void RHIDynamicRHI::endDebugEvent()
		{
			// "GL_KHR_debug"-extension required
			if (mExtensions->isGL_KHR_debug())
			{
				glPopDebugGroup();
			}
		}
#endif


//[-------------------------------------------------------]
//[ Public virtual RERHI::RHIDynamicRHI methods                      ]
//[-------------------------------------------------------]
const char* RHIDynamicRHI::getName() const
{
  return "OpenGL";
}

bool RHIDynamicRHI::isInitialized() const
{
  // Is the OpenGL context initialized?
  return (nullptr != mOpenGLContext && mOpenGLContext->isInitialized());
}

bool RHIDynamicRHI::isDebugEnabled()
{
  // OpenGL has nothing that is similar to the Direct3D 9 PIX functions (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)

  // Debug disabled
  return false;
}


//[-------------------------------------------------------]
//[ Shader language                                       ]
//[-------------------------------------------------------]
RECore::uint32 RHIDynamicRHI::getNumberOfShaderLanguages() const
{
  RECore::uint32 numberOfShaderLanguages = 0;

  // "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
  if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects())
  {
    // GLSL supported
    ++numberOfShaderLanguages;
  }

  // Done, return the number of supported shader languages
  return numberOfShaderLanguages;
}

const char* RHIDynamicRHI::getShaderLanguageName(RECore::uint32 index) const
{
  RHI_ASSERT(index < getNumberOfShaderLanguages(), "OpenGL: Shader language index is out-of-bounds")

  // "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
  if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects())
  {
    // GLSL supported
    if (0 == index)
    {
      return ::detail::GLSL_NAME;
    }
  }

  // Error!
  return nullptr;
}

RERHI::RHIShaderLanguage* RHIDynamicRHI::getShaderLanguage(const char* shaderLanguageName)
{
  // "GL_ARB_shader_objects" or "GL_ARB_separate_shader_objects" required
  if (mExtensions->isGL_ARB_shader_objects() || mExtensions->isGL_ARB_separate_shader_objects())
  {
    // In case "shaderLanguage" is a null pointer, use the default shader language
    if (nullptr != shaderLanguageName)
    {
      // Optimization: Check for shader language name pointer match, first
      if (::detail::GLSL_NAME == shaderLanguageName || !stricmp(shaderLanguageName, ::detail::GLSL_NAME))
      {
        // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
        if (mExtensions->isGL_ARB_separate_shader_objects())
        {
          // If required, create the separate shader language instance right now
          if (nullptr == mShaderLanguage)
          {
            mShaderLanguage = RHI_NEW(mContext, ShaderLanguageSeparate)(*this);
            mShaderLanguage->AddReference();	// Internal RHI reference
          }

          // Return the shader language instance
          return mShaderLanguage;
        }
        else if (mExtensions->isGL_ARB_shader_objects())
        {
          // If required, create the monolithic shader language instance right now
          if (nullptr == mShaderLanguage)
          {
            mShaderLanguage = RHI_NEW(mContext, ShaderLanguageMonolithic)(*this);
            mShaderLanguage->AddReference();	// Internal RHI reference
          }

          // Return the shader language instance
          return mShaderLanguage;
        }
      }
    }
    else
    {
      // Return the shader language instance as default
      return getShaderLanguage(::detail::GLSL_NAME);
    }
  }

  // Error!
  return nullptr;
}


//[-------------------------------------------------------]
//[ Resource creation                                     ]
//[-------------------------------------------------------]
RERHI::RHIRenderPass* RHIDynamicRHI::createRenderPass(RECore::uint32 numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  return RHI_NEW(mContext, RenderPass)(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIQueryPool* RHIDynamicRHI::createQueryPool(RERHI::QueryType queryType, RECore::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  RHI_ASSERT(numberOfQueries > 0, "OpenGL: Number of queries mustn't be zero")
  switch (queryType)
  {
    case RERHI::QueryType::OCCLUSION:
      if (!mExtensions->isGL_ARB_occlusion_query())
      {
        RE_LOG(Critical, "OpenGL extension \"GL_ARB_occlusion_query\" isn't supported")
        return nullptr;
      }
      return RHI_NEW(mContext, OcclusionTimestampQueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);

    case RERHI::QueryType::PIPELINE_STATISTICS:
      if (!mExtensions->isGL_ARB_pipeline_statistics_query())
      {
        RE_LOG(Critical, "OpenGL extension \"GL_ARB_pipeline_statistics_query\" isn't supported")
        return nullptr;
      }
      return RHI_NEW(mContext, PipelineStatisticsQueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);

    case RERHI::QueryType::TIMESTAMP:
      if (!mExtensions->isGL_ARB_timer_query())
      {
        RE_LOG(Critical, "OpenGL extension \"GL_ARB_timer_query\" isn't supported")
        return nullptr;
      }
      return RHI_NEW(mContext, OcclusionTimestampQueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  return nullptr;
}

RERHI::RHISwapChain* RHIDynamicRHI::createSwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle, bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, renderPass)
  RHI_ASSERT(NULL_HANDLE != windowHandle.nativeWindowHandle || nullptr != windowHandle.renderWindow, "OpenGL: The provided native window handle or render window must not be a null handle / null pointer")

  // Create the swap chain
  return RHI_NEW(mContext, SwapChain)(renderPass, windowHandle, useExternalContext RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIFramebuffer* RHIDynamicRHI::createFramebuffer(RERHI::RHIRenderPass& renderPass, const RERHI::FramebufferAttachment* colorFramebufferAttachments, const RERHI::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, renderPass)

  // "GL_ARB_framebuffer_object" required
  if (mExtensions->isGL_ARB_framebuffer_object())
  {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      // -> Validation is done inside the framebuffer implementation
      return RHI_NEW(mContext, FramebufferDsa)(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      // -> Validation is done inside the framebuffer implementation
      return RHI_NEW(mContext, FramebufferBind)(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    // Error!
    return nullptr;
  }
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
  RHI_ASSERT(nullptr != graphicsPipelineState.rootSignature, "OpenGL: Invalid graphics pipeline state root signature")
  RHI_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "OpenGL: Invalid graphics pipeline state graphics program")
  RHI_ASSERT(nullptr != graphicsPipelineState.renderPass, "OpenGL: Invalid graphics pipeline state render pass")

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

RERHI::RHIComputePipelineState* RHIDynamicRHI::createComputePipelineState(RERHI::RHIRootSignature& rootSignature, RERHI::RHIComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, rootSignature)
  RHI_MATCH_CHECK(*this, computeShader)

  // Create the compute pipeline state
  RECore::uint16 id = 0;
  if ((mExtensions->isGL_ARB_separate_shader_objects() || mExtensions->isGL_ARB_shader_objects()) && ComputePipelineStateMakeId.createID(id))
  {
    // Create the compute pipeline state
    // -> Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects())
    {
      return RHI_NEW(mContext, ComputePipelineStateSeparate)(*this, rootSignature, static_cast<ComputeShaderSeparate&>(computeShader), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else if (mExtensions->isGL_ARB_shader_objects())
    {
      return RHI_NEW(mContext, ComputePipelineStateMonolithic)(*this, rootSignature, static_cast<ComputeShaderMonolithic&>(computeShader), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }

  // Error: Ensure a correct reference counter behaviour
  rootSignature.AddReference();
  rootSignature.Release();
  computeShader.AddReference();
  computeShader.Release();
  return nullptr;
}

RERHI::RHISamplerState* RHIDynamicRHI::createSamplerState(const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Is "GL_ARB_sampler_objects" there?
  if (mExtensions->isGL_ARB_sampler_objects())
  {
    // Effective sampler object (SO)
    return RHI_NEW(mContext, SamplerStateSo)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

    // Is "GL_EXT_direct_state_access" there?
  else if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
  {
    // Direct state access (DSA) version to emulate a sampler object
    return RHI_NEW(mContext, SamplerStateDsa)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version to emulate a sampler object
    return RHI_NEW(mContext, SamplerStateBind)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
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
      return ::detail::mapBuffer(mContext, *mExtensions, GL_ARRAY_BUFFER_ARB, GL_ARRAY_BUFFER_BINDING_ARB, static_cast<VertexBuffer&>(resource).getOpenGLArrayBuffer(), mapType, mappedSubresource);

    case RERHI::ResourceType::INDEX_BUFFER:
      return ::detail::mapBuffer(mContext, *mExtensions, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, static_cast<IndexBuffer&>(resource).getOpenGLElementArrayBuffer(), mapType, mappedSubresource);

    case RERHI::ResourceType::TEXTURE_BUFFER:
      return ::detail::mapBuffer(mContext, *mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<TextureBuffer&>(resource).getOpenGLTextureBuffer(), mapType, mappedSubresource);

    case RERHI::ResourceType::STRUCTURED_BUFFER:
      return ::detail::mapBuffer(mContext, *mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<StructuredBuffer&>(resource).getOpenGLStructuredBuffer(), mapType, mappedSubresource);

    case RERHI::ResourceType::INDIRECT_BUFFER:
      return ::detail::mapBuffer(mContext, *mExtensions, GL_DRAW_INDIRECT_BUFFER, GL_DRAW_INDIRECT_BUFFER_BINDING, static_cast<IndirectBuffer&>(resource).getOpenGLIndirectBuffer(), mapType, mappedSubresource);

    case RERHI::ResourceType::UNIFORM_BUFFER:
      return ::detail::mapBuffer(mContext, *mExtensions, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING, static_cast<UniformBuffer&>(resource).getOpenGLUniformBuffer(), mapType, mappedSubresource);

    case RERHI::ResourceType::TEXTURE_1D:
    {
      glBindTexture(GL_TEXTURE_1D, static_cast<Texture1D&>(resource).getOpenGLTexture());
      // TODO(naetherm) Implement me
      return true;
    }

    case RERHI::ResourceType::TEXTURE_1D_ARRAY:
    {
      glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, static_cast<Texture1DArray&>(resource).getOpenGLTexture());
      // TODO(naetherm) Implement me
      return true;
    }

    case RERHI::ResourceType::TEXTURE_2D:
    {
      bool result = true;

      glBindTexture(GL_TEXTURE_2D, static_cast<Texture2D&>(resource).getOpenGLTexture());

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
      bool result = true;

      glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, static_cast<Texture2DArray&>(resource).getOpenGLTexture());

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
      return ::detail::mapBuffer(mContext, *mExtensions, GL_PIXEL_UNPACK_BUFFER_ARB, GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, static_cast<Texture3D&>(resource).getOpenGLPixelUnpackBuffer(), mapType, mappedSubresource);

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
      ::detail::unmapBuffer(*mExtensions, GL_ARRAY_BUFFER_ARB, GL_ARRAY_BUFFER_BINDING_ARB, static_cast<VertexBuffer&>(resource).getOpenGLArrayBuffer());
      break;

    case RERHI::ResourceType::INDEX_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, static_cast<IndexBuffer&>(resource).getOpenGLElementArrayBuffer());
      break;

    case RERHI::ResourceType::TEXTURE_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<TextureBuffer&>(resource).getOpenGLTextureBuffer());
      break;

    case RERHI::ResourceType::STRUCTURED_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_TEXTURE_BUFFER_ARB, GL_TEXTURE_BINDING_BUFFER_ARB, static_cast<StructuredBuffer&>(resource).getOpenGLStructuredBuffer());
      break;

    case RERHI::ResourceType::INDIRECT_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_DRAW_INDIRECT_BUFFER, GL_DRAW_INDIRECT_BUFFER_BINDING, static_cast<IndirectBuffer&>(resource).getOpenGLIndirectBuffer());
      break;

    case RERHI::ResourceType::UNIFORM_BUFFER:
      ::detail::unmapBuffer(*mExtensions, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING, static_cast<UniformBuffer&>(resource).getOpenGLUniformBuffer());
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
      // Unmap pixel unpack buffer
      const Texture3D& texture3D = static_cast<Texture3D&>(resource);
      const RERHI::TextureFormat::Enum textureFormat = texture3D.getTextureFormat();
      const RECore::uint32 openGLPixelUnpackBuffer = texture3D.getOpenGLPixelUnpackBuffer();
      ::detail::unmapBuffer(*mExtensions, GL_PIXEL_UNPACK_BUFFER_ARB, GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, openGLPixelUnpackBuffer);

      // Backup the currently set alignment and currently bound OpenGL pixel unpack buffer
#ifdef RHI_OPENGL_STATE_CLEANUP
      GLint openGLAlignmentBackup = 0;
					glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
					GLint openGLUnpackBufferBackup = 0;
					glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, &openGLUnpackBufferBackup);
#endif
      glPixelStorei(GL_UNPACK_ALIGNMENT, (RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

      // Copy pixel unpack buffer to texture
      glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, openGLPixelUnpackBuffer);
      if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
      {
        // Effective direct state access (DSA)
        if (mExtensions->isGL_ARB_direct_state_access())
        {
          glTextureSubImage3D(texture3D.getOpenGLTexture(), 0, 0, 0, 0, static_cast<GLsizei>(texture3D.getWidth()), static_cast<GLsizei>(texture3D.getHeight()), static_cast<GLsizei>(texture3D.getDepth()), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), 0);
        }
        else
        {
          glTextureSubImage3DEXT(texture3D.getOpenGLTexture(), GL_TEXTURE_3D, 0, 0, 0, 0, static_cast<GLsizei>(texture3D.getWidth()), static_cast<GLsizei>(texture3D.getHeight()), static_cast<GLsizei>(texture3D.getDepth()), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), 0);
        }
      }
      else
      {
        // Traditional bind version

        // Backup the currently bound OpenGL texture
#ifdef RHI_OPENGL_STATE_CLEANUP
        GLint openGLTextureBackup = 0;
						glGetIntegerv(GL_TEXTURE_BINDING_3D, &openGLTextureBackup);
#endif

        // Copy pixel unpack buffer to texture
        glBindTexture(GL_TEXTURE_3D, texture3D.getOpenGLTexture());
        glTexSubImage3DEXT(GL_TEXTURE_3D, 0, 0, 0, 0, static_cast<GLsizei>(texture3D.getWidth()), static_cast<GLsizei>(texture3D.getHeight()), static_cast<GLsizei>(texture3D.getDepth()), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), 0);

        // Be polite and restore the previous bound OpenGL texture
#ifdef RHI_OPENGL_STATE_CLEANUP
        glBindTexture(GL_TEXTURE_3D, static_cast<GLuint>(openGLTextureBackup));
#endif
      }

      // Restore previous alignment and pixel unpack buffer
#ifdef RHI_OPENGL_STATE_CLEANUP
      glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
					glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, static_cast<GLuint>(openGLUnpackBufferBackup));
#else
      glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
#endif
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

bool RHIDynamicRHI::getQueryPoolResults(RERHI::RHIQueryPool& queryPool, [[maybe_unused]] RECore::uint32 numberOfDataBytes, RECore::uint8* data, RECore::uint32 firstQueryIndex, RECore::uint32 numberOfQueries, RECore::uint32 strideInBytes, RECore::uint32 queryResultFlags)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, queryPool)
  RHI_ASSERT(numberOfDataBytes >= sizeof(std::RECore::uint64), "OpenGL out-of-memory query access")
  RHI_ASSERT(1 == numberOfQueries || strideInBytes > 0, "OpenGL invalid stride in bytes")
  RHI_ASSERT(numberOfDataBytes >= strideInBytes * numberOfQueries, "OpenGL out-of-memory query access")
  RHI_ASSERT(nullptr != data, "OpenGL out-of-memory query access")
  RHI_ASSERT(numberOfQueries > 0, "OpenGL number of queries mustn't be zero")

  // Query pool type dependent processing
  bool resultAvailable = true;
  const QueryPool& openGLQueryPool = static_cast<const QueryPool&>(queryPool);
  RHI_ASSERT(firstQueryIndex < openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
  RHI_ASSERT((firstQueryIndex + numberOfQueries) <= openGLQueryPool.getNumberOfQueries(), "OpenGL out-of-bounds query index")
  const bool waitForResult = ((queryResultFlags & RERHI::QueryResultFlags::WAIT) != 0);
  switch (openGLQueryPool.getQueryType())
  {
    case RERHI::QueryType::OCCLUSION:
    case RERHI::QueryType::TIMESTAMP:	// OpenGL return the time in nanoseconds
    {
      RECore::uint8* currentData = data;
      const GLuint* openGLQueries = static_cast<const OcclusionTimestampQueryPool&>(openGLQueryPool).getOpenGLQueries();
      for (RECore::uint32 i = 0; i  < numberOfQueries; ++i)
      {
        const GLuint openGLQuery = openGLQueries[firstQueryIndex + i];
        GLuint openGLQueryResult = GL_FALSE;
        do
        {
          glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_AVAILABLE_ARB, &openGLQueryResult);
        }
        while (waitForResult && GL_TRUE != openGLQueryResult);
        if (GL_TRUE == openGLQueryResult)
        {
          glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_ARB, &openGLQueryResult);
          *reinterpret_cast<RECore::uint64*>(currentData) = openGLQueryResult;
        }
        else
        {
          // Result not ready
          resultAvailable = false;
          break;
        }
        currentData += strideInBytes;
      }
      break;
    }

    case RERHI::QueryType::PIPELINE_STATISTICS:
      RHI_ASSERT(numberOfDataBytes >= sizeof(RERHI::PipelineStatisticsQueryResult), "OpenGL out-of-memory query access")
      RHI_ASSERT(1 == numberOfQueries || strideInBytes >= sizeof(RERHI::PipelineStatisticsQueryResult), "OpenGL out-of-memory query access")
      resultAvailable = static_cast<const PipelineStatisticsQueryPool&>(openGLQueryPool).getQueryPoolResults(data, firstQueryIndex, numberOfQueries, strideInBytes, waitForResult);
      break;
  }

  // Done
  return resultAvailable;
}


//[-------------------------------------------------------]
//[ Operation                                             ]
//[-------------------------------------------------------]
void RHIDynamicRHI::dispatchCommandBuffer(const RERHI::RHICommandBuffer& commandBuffer)
{
  // Sanity check
  RHI_ASSERT(!commandBuffer.isEmpty(), "The OpenGL command buffer to dispatch mustn't be empty")

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
				case GL_DEBUG_SOURCE_API_ARB:
					strncpy(debugSource, "OpenGL", 20);
					break;

				case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
					strncpy(debugSource, "Windows", 20);
					break;

				case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
					strncpy(debugSource, "Shader compiler", 20);
					break;

				case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
					strncpy(debugSource, "Third party", 20);
					break;

				case GL_DEBUG_SOURCE_APPLICATION_ARB:
					strncpy(debugSource, "Application", 20);
					break;

				case GL_DEBUG_SOURCE_OTHER_ARB:
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
				case GL_DEBUG_TYPE_ERROR_ARB:
					strncpy(debugType, "Error", 25);
					break;

				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
					logType = RECore::ILog::Type::COMPATIBILITY_WARNING;
					strncpy(debugType, "Deprecated behavior", 25);
					break;

				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
					strncpy(debugType, "Undefined behavior", 25);
					break;

				case GL_DEBUG_TYPE_PORTABILITY_ARB:
					logType = RECore::ILog::Type::COMPATIBILITY_WARNING;
					strncpy(debugType, "Portability", 25);
					break;

				case GL_DEBUG_TYPE_PERFORMANCE_ARB:
					logType = RECore::ILog::Type::PERFORMANCE_WARNING;
					strncpy(debugType, "Performance", 25);
					break;

				case GL_DEBUG_TYPE_OTHER_ARB:
					strncpy(debugType, "Other", 25);
					break;

				case GL_DEBUG_TYPE_MARKER:
					strncpy(debugType, "Marker", 25);
					break;

				case GL_DEBUG_TYPE_PUSH_GROUP:
					strncpy(debugType, "Push group", 25);
					break;

				case GL_DEBUG_TYPE_POP_GROUP:
					strncpy(debugType, "Pop group", 25);
					break;

				default:
					strncpy(debugType, "?", 25);
					break;
			}

			// Debug severity to string
			char debugSeverity[20 + 1]{0};	// +1 for terminating zero
			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH_ARB:
					strncpy(debugSeverity, "High", 20);
					break;

				case GL_DEBUG_SEVERITY_MEDIUM_ARB:
					strncpy(debugSeverity, "Medium", 20);
					break;

				case GL_DEBUG_SEVERITY_LOW_ARB:
					strncpy(debugSeverity, "Low", 20);
					break;

				case GL_DEBUG_SEVERITY_NOTIFICATION:
					strncpy(debugSeverity, "Notification", 20);
					break;

				default:
					strncpy(debugSeverity, "?", 20);
					break;
			}

			// Print into log
      RE_LOG(Critical, RECore::String(message))
			//if (static_cast<const OpenGLRhi*>(userParam)->getContext().getLog().print(logType, nullptr, __FILE__, static_cast<RECore::uint32>(__LINE__), "OpenGL debug message\tSource:\"%s\"\tType:\"%s\"\tID:\"%u\"\tSeverity:\"%s\"\tMessage:\"%s\"", debugSource, debugType, id, debugSeverity, message))
			{
			//	DEBUG_BREAK;
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
    // -> OpenGL 4.3 Compatibility Profile Specification, section 22.2, page 627: "String queries return pointers to UTF-8 encoded, null-terminated static strings describing properties of the current GL context."
    // -> For example "AMD Radeon R9 200 Series"
    const size_t numberOfCharacters = ::detail::countof(mCapabilities.deviceName) - 1;
    strncpy(mCapabilities.deviceName, reinterpret_cast<const char*>(glGetString(GL_RENDERER)), numberOfCharacters);
    mCapabilities.deviceName[numberOfCharacters] = '\0';
  }

  // Preferred swap chain texture format
  mCapabilities.preferredSwapChainColorTextureFormat		  = RERHI::TextureFormat::Enum::R8G8B8A8;
  mCapabilities.preferredSwapChainDepthStencilTextureFormat = RERHI::TextureFormat::Enum::D32_FLOAT;

  // Maximum number of viewports (always at least 1)
  // TODO(naetherm) "GL_ARB_viewport_array" support ("RHIDynamicRHI::setGraphicsViewports()")
  mCapabilities.maximumNumberOfViewports = 1;	// TODO(naetherm) GL_ARB_viewport_array

  // Maximum number of simultaneous render targets (if <1 render to texture is not supported, "GL_ARB_draw_buffers" required)
  if (mExtensions->isGL_ARB_draw_buffers())
  {
    glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &openGLValue);
    mCapabilities.maximumNumberOfSimultaneousRenderTargets = static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    // "GL_ARB_framebuffer_object"-extension for render to texture required
    mCapabilities.maximumNumberOfSimultaneousRenderTargets = static_cast<RECore::uint32>(mExtensions->isGL_ARB_framebuffer_object());
  }

  // Maximum texture dimension
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &openGLValue);
  mCapabilities.maximumTextureDimension = static_cast<RECore::uint32>(openGLValue);

  // Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
  // Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
  // Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
  if (mExtensions->isGL_EXT_texture_array())
  {
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, &openGLValue);
    mCapabilities.maximumNumberOf1DTextureArraySlices = static_cast<RECore::uint32>(openGLValue);
    mCapabilities.maximumNumberOf2DTextureArraySlices = static_cast<RECore::uint32>(openGLValue);
    mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;	// TODO(naetherm) Implement me		 static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    mCapabilities.maximumNumberOf1DTextureArraySlices = 0;
    mCapabilities.maximumNumberOf2DTextureArraySlices = 0;
    mCapabilities.maximumNumberOfCubeTextureArraySlices = 0;
  }

  // Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
  if (mExtensions->isGL_ARB_texture_buffer_object())
  {
    glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE_EXT, &openGLValue);
    mCapabilities.maximumTextureBufferSize = static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    mCapabilities.maximumTextureBufferSize = 0;
  }

  // Maximum structured buffer size in bytes
  if (mExtensions->isGL_ARB_shader_storage_buffer_object())
  {
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &openGLValue);
    mCapabilities.maximumStructuredBufferSize = static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    mCapabilities.maximumStructuredBufferSize = 0;
  }

  // Maximum indirect buffer size in bytes
  if (mExtensions->isGL_ARB_draw_indirect())
  {
    // TODO(naetherm) How to get the indirect buffer maximum size? Didn't find any information about this.
    mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB
  }
  else
  {
    mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB
  }

  // Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
  if (mExtensions->isGL_ARB_uniform_buffer_object())
  {
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &openGLValue);
    mCapabilities.maximumUniformBufferSize = static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    mCapabilities.maximumUniformBufferSize = 0;
  }

  // Maximum number of multisamples (always at least 1, usually 8)
  if (mExtensions->isGL_ARB_texture_multisample())
  {
    glGetIntegerv(GL_MAX_SAMPLES, &openGLValue);
    if (openGLValue > 8)
    {
      // Limit to known maximum we can test, even if e.g. GeForce 980m reports 32 here
      openGLValue = 8;
    }
    mCapabilities.maximumNumberOfMultisamples = static_cast<RECore::uint8>(openGLValue);
  }
  else
  {
    mCapabilities.maximumNumberOfMultisamples = 1;
  }

  // Maximum anisotropy (always at least 1, usually 16)
  // -> "GL_EXT_texture_filter_anisotropic"-extension
  glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &openGLValue);
  mCapabilities.maximumAnisotropy = static_cast<RECore::uint8>(openGLValue);

  // Coordinate system
  // -> If the "GL_ARB_clip_control"-extension is available: Left-handed coordinate system with clip space depth value range 0..1
  // -> If the "GL_ARB_clip_control"-extension isn't available: Right-handed coordinate system with clip space depth value range -1..1
  // -> For background theory see "Depth Precision Visualized" by Nathan Reed - https://developer.nvidia.com/content/depth-precision-visualized
  // -> For practical information see "Reversed-Z in OpenGL" by Nicolas Guillemot - https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
  // -> Shaders might want to take the following into account: "Mac computers that use OpenCL and OpenGL graphics" - https://support.apple.com/en-us/HT202823 - "iMac (Retina 5K, 27-inch, 2017)" - OpenGL 4.1
  mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = mExtensions->isGL_ARB_clip_control();

  // Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
  mCapabilities.individualUniforms = true;

  // Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex, "GL_ARB_instanced_arrays" required)
  mCapabilities.instancedArrays = mExtensions->isGL_ARB_instanced_arrays();

  // Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID, "GL_ARB_draw_instanced" required)
  mCapabilities.drawInstanced = mExtensions->isGL_ARB_draw_instanced();

  // Base vertex supported for draw calls?
  mCapabilities.baseVertex = mExtensions->isGL_ARB_draw_elements_base_vertex();

  // OpenGL has no native multithreading
  mCapabilities.nativeMultithreading = false;

  // We don't support the OpenGL program binaries since those are operation system and graphics driver version dependent, which renders them useless for pre-compiled shaders shipping
  mCapabilities.shaderBytecode = mExtensions->isGL_ARB_gl_spirv();

  // Is there support for vertex shaders (VS)?
  mCapabilities.vertexShader = mExtensions->isGL_ARB_vertex_shader();

  // Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
  if (mExtensions->isGL_ARB_tessellation_shader())
  {
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &openGLValue);
    mCapabilities.maximumNumberOfPatchVertices = static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    mCapabilities.maximumNumberOfPatchVertices = 0;
  }

  // Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
  if (mExtensions->isGL_ARB_geometry_shader4())
  {
    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB, &openGLValue);
    mCapabilities.maximumNumberOfGsOutputVertices = static_cast<RECore::uint32>(openGLValue);
  }
  else
  {
    mCapabilities.maximumNumberOfGsOutputVertices = 0;
  }

  // Is there support for fragment shaders (FS)?
  mCapabilities.fragmentShader = mExtensions->isGL_ARB_fragment_shader();

  // Is there support for task shaders (TS) and mesh shaders (MS)?
  mCapabilities.meshShader = mExtensions->isGL_NV_mesh_shader();

  // Is there support for compute shaders (CS)?
  mCapabilities.computeShader = mExtensions->isGL_ARB_compute_shader();
}

void RHIDynamicRHI::unsetGraphicsVertexArray()
{
  // Release the currently used vertex array reference, in case we have one
  if (nullptr != mVertexArray)
  {
    // Evaluate the internal array type of the currently set vertex array
    switch (static_cast<VertexArray*>(mVertexArray)->getInternalResourceType())
    {
      case VertexArray::InternalResourceType::NO_VAO:
        // Disable OpenGL vertex attribute arrays
        static_cast<VertexArrayNoVao*>(mVertexArray)->disableOpenGLVertexAttribArrays();
        break;

      case VertexArray::InternalResourceType::VAO:
        // Unbind OpenGL vertex array
        // -> No need to check for "GL_ARB_vertex_array_object", in case were in here we know it must exist
        glBindVertexArray(mDefaultOpenGLVertexArray);
        break;
    }

    // Release reference
    mVertexArray->Release();
    mVertexArray = nullptr;
  }
}

void RHIDynamicRHI::setResourceGroup(const RootSignature& rootSignature, RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  if (nullptr != resourceGroup)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *resourceGroup)

    // Set resource group
    const ResourceGroup* openGLResourceGroup = static_cast<ResourceGroup*>(resourceGroup);
    const RECore::uint32 numberOfResources = openGLResourceGroup->getNumberOfResources();
    RERHI::RHIResource** resources = openGLResourceGroup->getResources();
    const RERHI::RootParameter& rootParameter = rootSignature.getRootSignature().parameters[rootParameterIndex];
    for (RECore::uint32 resourceIndex = 0; resourceIndex < numberOfResources; ++resourceIndex, ++resources)
    {
      RERHI::RHIResource* resource = *resources;
      RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
      const RERHI::DescriptorRange& descriptorRange = reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];

      // Check the type of resource to set
      // TODO(naetherm) Some additional resource type root signature security checks in debug build?
      const RERHI::ResourceType resourceType = resource->getResourceType();
      switch (resourceType)
      {
        case RERHI::ResourceType::TEXTURE_BUFFER:
        case RERHI::ResourceType::TEXTURE_1D:
        case RERHI::ResourceType::TEXTURE_1D_ARRAY:
        case RERHI::ResourceType::TEXTURE_2D:
        case RERHI::ResourceType::TEXTURE_2D_ARRAY:
        case RERHI::ResourceType::TEXTURE_3D:
        case RERHI::ResourceType::TEXTURE_CUBE:
        case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
        {
          // In OpenGL, all shaders share the same texture units (= "RERHI::RootParameter::shaderVisibility" stays unused)
          switch (descriptorRange.rangeType)
          {
            case RERHI::DescriptorRangeType::SRV:
            {
              // Is "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" there?
              if (mExtensions->isGL_ARB_direct_state_access() || mExtensions->isGL_EXT_direct_state_access())
              {
                // Effective direct state access (DSA)
                const bool isArbDsa = mExtensions->isGL_ARB_direct_state_access();

                // "glBindTextureUnit()" unit parameter is zero based so we can simply use the value we received
                const GLuint unit = descriptorRange.baseShaderRegister;

                // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                // Evaluate the texture type
                switch (resourceType)
                {
                  case RERHI::ResourceType::TEXTURE_BUFFER:
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<TextureBuffer*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_BUFFER_ARB, static_cast<TextureBuffer*>(resource)->getOpenGLTexture());
                    }
                    break;

                  case RERHI::ResourceType::TEXTURE_1D:
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<Texture1D*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_1D, static_cast<Texture1D*>(resource)->getOpenGLTexture());
                    }
                    break;

                  case RERHI::ResourceType::TEXTURE_1D_ARRAY:
                    // No texture 1D array extension check required, if we in here we already know it must exist
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<Texture1DArray*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_1D_ARRAY_EXT, static_cast<Texture1DArray*>(resource)->getOpenGLTexture());
                    }
                    break;

                  case RERHI::ResourceType::TEXTURE_2D:
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<Texture2D*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      const Texture2D* texture2D = static_cast<Texture2D*>(resource);
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture());
                    }
                    break;

                  case RERHI::ResourceType::TEXTURE_2D_ARRAY:
                    // No texture 2D array extension check required, if we in here we already know it must exist
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<Texture2DArray*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_2D_ARRAY_EXT, static_cast<Texture2DArray*>(resource)->getOpenGLTexture());
                    }
                    break;

                  case RERHI::ResourceType::TEXTURE_3D:
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<Texture3D*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_3D, static_cast<Texture3D*>(resource)->getOpenGLTexture());
                    }
                    break;

                  case RERHI::ResourceType::TEXTURE_CUBE:
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<TextureCube*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_CUBE_MAP, static_cast<TextureCube*>(resource)->getOpenGLTexture());
                    }
                    break;

                  case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
                    // TODO(naetherm) Implement me
                    /*
                    if (isArbDsa)
                    {
                      glBindTextureUnit(unit, static_cast<TextureCubeArray*>(resource)->getOpenGLTexture());
                    }
                    else
                    {
                      // "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glBindMultiTextureEXT(GL_TEXTURE0_ARB + unit, GL_TEXTURE_CUBE_MAP, static_cast<TextureCubeArray*>(resource)->getOpenGLTexture());
                    }
                    */
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
                  case RERHI::ResourceType::STRUCTURED_BUFFER:
                  case RERHI::ResourceType::INDIRECT_BUFFER:
                  case RERHI::ResourceType::UNIFORM_BUFFER:
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
                    RHI_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
                    break;
                }

                // Set the OpenGL sampler states, if required (texture buffer has no sampler state), it's valid that there's no sampler state (e.g. texel fetch instead of sampling might be used)
                if (RERHI::ResourceType::TEXTURE_BUFFER != resourceType && nullptr != openGLResourceGroup->getSamplerState())
                {
                  const SamplerState* samplerState = static_cast<const SamplerState*>(openGLResourceGroup->getSamplerState()[resourceIndex]);
                  if (nullptr != samplerState)
                  {
                    // Is "GL_ARB_sampler_objects" there?
                    if (mExtensions->isGL_ARB_sampler_objects())
                    {
                      // Effective sampler object (SO)
                      glBindSampler(descriptorRange.baseShaderRegister, static_cast<const SamplerStateSo*>(samplerState)->getOpenGLSampler());
                    }
                    else
                    {
#ifdef RHI_OPENGL_STATE_CLEANUP
                      // Backup the currently active OpenGL texture
													GLint openGLActiveTextureBackup = 0;
													glGetIntegerv(GL_ACTIVE_TEXTURE, &openGLActiveTextureBackup);
#endif

                      // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                      // Activate the texture unit we want to manipulate
                      // -> "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                      glActiveTextureARB(GL_TEXTURE0_ARB + unit);

                      // Is "GL_EXT_direct_state_access" there?
                      if (mExtensions->isGL_EXT_direct_state_access())
                      {
                        // Direct state access (DSA) version to emulate a sampler object
                        static_cast<const SamplerStateDsa*>(samplerState)->setOpenGLSamplerStates();
                      }
                      else
                      {
                        // Traditional bind version to emulate a sampler object
                        static_cast<const SamplerStateBind*>(samplerState)->setOpenGLSamplerStates();
                      }

#ifdef RHI_OPENGL_STATE_CLEANUP
                      // Be polite and restore the previous active OpenGL texture
													glActiveTextureARB(static_cast<GLenum>(openGLActiveTextureBackup));
#endif
                    }
                  }
                }
              }
              else
              {
                // Traditional bind version

                // "GL_ARB_multitexture" required
                if (mExtensions->isGL_ARB_multitexture())
                {
#ifdef RHI_OPENGL_STATE_CLEANUP
                  // Backup the currently active OpenGL texture
											GLint openGLActiveTextureBackup = 0;
											glGetIntegerv(GL_ACTIVE_TEXTURE, &openGLActiveTextureBackup);
#endif

                  // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                  // Activate the texture unit we want to manipulate
                  // -> "GL_TEXTURE0_ARB" is the first texture unit, while the unit we received is zero based
                  glActiveTextureARB(GL_TEXTURE0_ARB + descriptorRange.baseShaderRegister);

                  // Evaluate the resource type
                  switch (resourceType)
                  {
                    case RERHI::ResourceType::TEXTURE_BUFFER:
                      glBindTexture(GL_TEXTURE_BUFFER_ARB, static_cast<TextureBuffer*>(resource)->getOpenGLTexture());
                      break;

                    case RERHI::ResourceType::TEXTURE_1D:
                      glBindTexture(GL_TEXTURE_1D, static_cast<Texture1D*>(resource)->getOpenGLTexture());
                      break;

                    case RERHI::ResourceType::TEXTURE_1D_ARRAY:
                      // No extension check required, if we in here we already know it must exist
                      glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, static_cast<Texture1DArray*>(resource)->getOpenGLTexture());
                      break;

                    case RERHI::ResourceType::TEXTURE_2D:
                    {
                      const Texture2D* texture2D = static_cast<Texture2D*>(resource);
                      glBindTexture(static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D), texture2D->getOpenGLTexture());
                      break;
                    }

                    case RERHI::ResourceType::TEXTURE_2D_ARRAY:
                      // No extension check required, if we in here we already know it must exist
                      glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, static_cast<Texture2DArray*>(resource)->getOpenGLTexture());
                      break;

                    case RERHI::ResourceType::TEXTURE_3D:
                      glBindTexture(GL_TEXTURE_3D, static_cast<Texture3D*>(resource)->getOpenGLTexture());
                      break;

                    case RERHI::ResourceType::TEXTURE_CUBE:
                      glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<TextureCube*>(resource)->getOpenGLTexture());
                      break;

                    case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
                      // TODO(naetherm) Implement me
                      // glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<TextureCubeArray*>(resource)->getOpenGLTexture());
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
                    case RERHI::ResourceType::STRUCTURED_BUFFER:
                    case RERHI::ResourceType::INDIRECT_BUFFER:
                    case RERHI::ResourceType::UNIFORM_BUFFER:
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
                      RHI_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
                      break;
                  }

                  // Set the OpenGL sampler states, if required (texture buffer has no sampler state), it's valid that there's no sampler state (e.g. texel fetch instead of sampling might be used)
                  if (RERHI::ResourceType::TEXTURE_BUFFER != resourceType)
                  {
                    RHI_ASSERT(nullptr != openGLResourceGroup->getSamplerState(), "Invalid OpenGL sampler state")
                    const SamplerState* samplerState = static_cast<const SamplerState*>(openGLResourceGroup->getSamplerState()[resourceIndex]);
                    if (nullptr != samplerState)
                    {
                      // Is "GL_ARB_sampler_objects" there?
                      if (mExtensions->isGL_ARB_sampler_objects())
                      {
                        // Effective sampler object (SO)
                        glBindSampler(descriptorRange.baseShaderRegister, static_cast<const SamplerStateSo*>(samplerState)->getOpenGLSampler());
                      }
                        // Is "GL_EXT_direct_state_access" there?
                      else if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
                      {
                        // Direct state access (DSA) version to emulate a sampler object
                        static_cast<const SamplerStateDsa*>(samplerState)->setOpenGLSamplerStates();
                      }
                      else
                      {
                        // Traditional bind version to emulate a sampler object
                        static_cast<const SamplerStateBind*>(samplerState)->setOpenGLSamplerStates();
                      }
                    }
                  }

#ifdef RHI_OPENGL_STATE_CLEANUP
                  // Be polite and restore the previous active OpenGL texture
											glActiveTextureARB(static_cast<GLenum>(openGLActiveTextureBackup));
#endif
                }
              }
              break;
            }

            case RERHI::DescriptorRangeType::UAV:
            {
              // Is "GL_EXT_shader_image_load_store" there?
              if (mExtensions->isGL_EXT_shader_image_load_store())
              {
                // "glBindImageTextureEXT()" unit parameter is zero based so we can simply use the value we received
                const GLuint unit = descriptorRange.baseShaderRegister;

                // TODO(naetherm) Some security checks might be wise *maximum number of texture units*
                // Evaluate the texture type
                switch (resourceType)
                {
                  case RERHI::ResourceType::TEXTURE_BUFFER:
                  {
                    const TextureBuffer* textureBuffer = static_cast<TextureBuffer*>(resource);
                    glBindImageTextureEXT(unit, textureBuffer->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureBuffer->getOpenGLInternalFormat()));
                    break;
                  }

                  case RERHI::ResourceType::TEXTURE_1D:
                  {
                    const Texture1D* texture1D = static_cast<Texture1D*>(resource);
                    glBindImageTextureEXT(unit, texture1D->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture1D->getOpenGLInternalFormat()));
                    break;
                  }

                  case RERHI::ResourceType::TEXTURE_1D_ARRAY:
                  {
                    const Texture1DArray* texture1DArray = static_cast<Texture1DArray*>(resource);
                    glBindImageTextureEXT(unit, texture1DArray->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture1DArray->getOpenGLInternalFormat()));
                    break;
                  }

                  case RERHI::ResourceType::TEXTURE_2D:
                  {
                    const Texture2D* texture2D = static_cast<Texture2D*>(resource);
                    glBindImageTextureEXT(unit, texture2D->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture2D->getOpenGLInternalFormat()));
                    break;
                  }

                  case RERHI::ResourceType::TEXTURE_2D_ARRAY:
                  {
                    const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(resource);
                    glBindImageTextureEXT(unit, texture2DArray->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture2DArray->getOpenGLInternalFormat()));
                    break;
                  }

                  case RERHI::ResourceType::TEXTURE_3D:
                  {
                    const Texture3D* texture3D = static_cast<Texture3D*>(resource);
                    glBindImageTextureEXT(unit, texture3D->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(texture3D->getOpenGLInternalFormat()));
                    break;
                  }

                  case RERHI::ResourceType::TEXTURE_CUBE:
                  {
                    const TextureCube* textureCube = static_cast<TextureCube*>(resource);
                    glBindImageTextureEXT(unit, textureCube->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureCube->getOpenGLInternalFormat()));
                    break;
                  }

                  case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
                  {
                    // TODO(naetherm) Implement me
                    // const TextureCubeArray* textureCubeArray = static_cast<TextureCubeArray*>(resource);
                    // glBindImageTextureEXT(unit, textureCubeArray->getOpenGLTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLint>(textureCubeArray->getOpenGLInternalFormat()));
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
                  case RERHI::ResourceType::VERTEX_BUFFER:
                  case RERHI::ResourceType::INDEX_BUFFER:
                  case RERHI::ResourceType::STRUCTURED_BUFFER:
                  case RERHI::ResourceType::INDIRECT_BUFFER:
                  case RERHI::ResourceType::UNIFORM_BUFFER:
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
                    RHI_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
                    break;
                }
              }
              break;
            }

            case RERHI::DescriptorRangeType::UBV:
            case RERHI::DescriptorRangeType::SAMPLER:
            case RERHI::DescriptorRangeType::NUMBER_OF_RANGE_TYPES:
              RHI_ASSERT(false, "Invalid OpenGL descriptor range type")
              break;
          }
          break;
        }

        case RERHI::ResourceType::VERTEX_BUFFER:
        {
          RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange.rangeType || RERHI::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL vertex buffer must bound at SRV or UAV descriptor range type")
          RHI_ASSERT(RERHI::ShaderVisibility::ALL == descriptorRange.shaderVisibility || RERHI::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object())
          {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<VertexBuffer*>(resource)->getOpenGLArrayBuffer());
          }
          break;
        }

        case RERHI::ResourceType::INDEX_BUFFER:
        {
          RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange.rangeType || RERHI::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL index buffer must bound at SRV or UAV descriptor range type")
          RHI_ASSERT(RERHI::ShaderVisibility::ALL == descriptorRange.shaderVisibility || RERHI::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object())
          {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<IndexBuffer*>(resource)->getOpenGLElementArrayBuffer());
          }
          break;
        }

        case RERHI::ResourceType::STRUCTURED_BUFFER:
        {
          RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange.rangeType || RERHI::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL structured buffer must bound at SRV or UAV descriptor range type")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object())
          {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<StructuredBuffer*>(resource)->getOpenGLStructuredBuffer());
          }
          break;
        }

        case RERHI::ResourceType::INDIRECT_BUFFER:
        {
          RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange.rangeType || RERHI::DescriptorRangeType::UAV == descriptorRange.rangeType, "OpenGL indirect buffer must bound at SRV or UAV descriptor range type")
          RHI_ASSERT(RERHI::ShaderVisibility::ALL == descriptorRange.shaderVisibility || RERHI::ShaderVisibility::COMPUTE == descriptorRange.shaderVisibility, "OpenGL descriptor range shader visibility must be \"ALL\" or \"COMPUTE\"")

          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object())
          {
            // "glBindBufferBase()" unit parameter is zero based so we can simply use the value we received
            const GLuint index = descriptorRange.baseShaderRegister;

            // Attach the buffer to the given SSBO binding point
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<IndirectBuffer*>(resource)->getOpenGLIndirectBuffer());
          }
          break;
        }

        case RERHI::ResourceType::UNIFORM_BUFFER:
          // "GL_ARB_uniform_buffer_object" required
          if (mExtensions->isGL_ARB_uniform_buffer_object())
          {
            // Attach the buffer to the given UBO binding point
            // -> Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension
            // -> Direct3D 10 and Direct3D 11 have explicit binding points
            RHI_ASSERT(RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType, "OpenGL uniform buffer must bound at UBV descriptor range type")
            RHI_ASSERT(nullptr != openGLResourceGroup->getResourceIndexToUniformBlockBindingIndex(), "Invalid OpenGL resource index to uniform block binding index")
            glBindBufferBase(GL_UNIFORM_BUFFER, openGLResourceGroup->getResourceIndexToUniformBlockBindingIndex()[resourceIndex], static_cast<UniformBuffer*>(resource)->getOpenGLUniformBuffer());
          }
          break;

        case RERHI::ResourceType::SAMPLER_STATE:
          // Unlike Direct3D >=10, OpenGL directly attaches the sampler settings to the texture (unless the sampler object extension is used)
          break;

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
        case RERHI::ResourceType::VERTEX_SHADER:
        case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
        case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case RERHI::ResourceType::GEOMETRY_SHADER:
        case RERHI::ResourceType::FRAGMENT_SHADER:
        case RERHI::ResourceType::TASK_SHADER:
        case RERHI::ResourceType::MESH_SHADER:
        case RERHI::ResourceType::COMPUTE_SHADER:
          RHI_ASSERT(false, "Invalid OpenGL RHI implementation resource type")
          break;
      }
    }
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::setOpenGLGraphicsProgram(RERHI::RHIGraphicsProgram* graphicsProgram)
{
  if (nullptr != graphicsProgram)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *graphicsProgram)

    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects())
    {
      // Bind the graphics program pipeline, if required
      GraphicsProgramSeparate* graphicsProgramSeparate = static_cast<GraphicsProgramSeparate*>(graphicsProgram);
      const RECore::uint32 openGLProgramPipeline = graphicsProgramSeparate->getOpenGLProgramPipeline();
      if (openGLProgramPipeline != mOpenGLProgramPipeline)
      {
        mOpenGLProgramPipeline = openGLProgramPipeline;
        { // Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
          const VertexShaderSeparate* vertexShaderSeparate = graphicsProgramSeparate->getVertexShaderSeparate();
          if (nullptr != vertexShaderSeparate)
          {
            mOpenGLVertexProgram = vertexShaderSeparate->getOpenGLShaderProgram();
            mDrawIdUniformLocation = vertexShaderSeparate->getDrawIdUniformLocation();
          }
          else
          {
            mOpenGLVertexProgram = 0;
            mDrawIdUniformLocation = -1;
          }
          mCurrentStartInstanceLocation = ~0u;
        }
        glBindProgramPipeline(mOpenGLProgramPipeline);
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects())
    {
      // Bind the graphics program, if required
      const GraphicsProgramMonolithic* graphicsProgramMonolithic = static_cast<GraphicsProgramMonolithic*>(graphicsProgram);
      const RECore::uint32 openGLProgram = graphicsProgramMonolithic->getOpenGLProgram();
      if (openGLProgram != mOpenGLProgram)
      {
        mOpenGLProgram = mOpenGLVertexProgram = openGLProgram;
        mDrawIdUniformLocation = graphicsProgramMonolithic->getDrawIdUniformLocation();
        mCurrentStartInstanceLocation = ~0u;
        glUseProgram(mOpenGLProgram);
      }
    }
  }
  else
  {
    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects())
    {
      // Unbind the program pipeline, if required
      if (0 != mOpenGLProgramPipeline)
      {
        glBindProgramPipeline(0);
        mOpenGLProgramPipeline = 0;
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects())
    {
      // Unbind the program, if required
      if (0 != mOpenGLProgram)
      {
        glUseProgram(0);
        mOpenGLProgram = 0;
      }
    }
    mOpenGLVertexProgram = 0;
    mDrawIdUniformLocation = -1;
    mCurrentStartInstanceLocation = ~0u;
  }
}

void RHIDynamicRHI::setOpenGLComputePipelineState(ComputePipelineState* computePipelineState)
{
  if (nullptr != computePipelineState)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *computePipelineState)

    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects())
    {
      // Bind the program pipeline, if required
      const RECore::uint32 openGLProgramPipeline = static_cast<ComputePipelineStateSeparate*>(computePipelineState)->getOpenGLProgramPipeline();
      if (openGLProgramPipeline != mOpenGLProgramPipeline)
      {
        mOpenGLProgramPipeline = openGLProgramPipeline;
        glBindProgramPipeline(mOpenGLProgramPipeline);
        mOpenGLVertexProgram = 0;
        mDrawIdUniformLocation = -1;
        mCurrentStartInstanceLocation = ~0u;
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects())
    {
      // Bind the program, if required
      const RECore::uint32 openGLProgram = static_cast<ComputePipelineStateMonolithic*>(computePipelineState)->getOpenGLProgram();
      if (openGLProgram != mOpenGLProgram)
      {
        mOpenGLProgram = openGLProgram;
        glUseProgram(mOpenGLProgram);
        mOpenGLVertexProgram = 0;
        mDrawIdUniformLocation = -1;
        mCurrentStartInstanceLocation = ~0u;
      }
    }
  }
  else
  {
    // Prefer "GL_ARB_separate_shader_objects" over "GL_ARB_shader_objects"
    if (mExtensions->isGL_ARB_separate_shader_objects())
    {
      // Unbind the program pipeline, if required
      if (0 != mOpenGLProgramPipeline)
      {
        glBindProgramPipeline(0);
        mOpenGLProgramPipeline = 0;
      }
    }
    else if (mExtensions->isGL_ARB_shader_objects())
    {
      // Unbind the program, if required
      if (0 != mOpenGLProgram)
      {
        glUseProgram(0);
        mOpenGLProgram = 0;
      }
    }
    mOpenGLVertexProgram = 0;
    mDrawIdUniformLocation = -1;
    mCurrentStartInstanceLocation = ~0u;
  }
}

void RHIDynamicRHI::updateGL_ARB_base_instanceEmulation(RECore::uint32 startInstanceLocation)
{
  if (mDrawIdUniformLocation != -1 && 0 != mOpenGLVertexProgram && mCurrentStartInstanceLocation != startInstanceLocation)
  {
    glProgramUniform1ui(mOpenGLVertexProgram, mDrawIdUniformLocation, startInstanceLocation);
    mCurrentStartInstanceLocation = startInstanceLocation;
  }
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL


RERHIOPENGL_API RERHI::RHIDynamicRHI* createOpenGLRhiInstance(const RERHI::RHIContext& context)
{
  return RHI_NEW(context, RERHIOpenGL::RHIDynamicRHI)(context);
}
RERHIOPENGL_API RERHI::RHIDynamicRHI* createRhiInstance(const RERHI::RHIContext& context)
{
  return RHI_NEW(context, RERHIOpenGL::RHIDynamicRHI)(context);
}