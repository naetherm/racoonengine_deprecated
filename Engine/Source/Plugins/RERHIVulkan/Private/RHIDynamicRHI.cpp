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
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/VulkanRuntimeLinking.h"
#include "RERHIVulkan/RHIResourceGroup.h"
#include "RERHIVulkan/RHIRootSignature.h"
#include "RERHIVulkan/Buffer/RHIIndexBuffer.h"
#include "RERHIVulkan/Buffer/RHIVertexBuffer.h"
#include "RERHIVulkan/Buffer/RHIBufferManager.h"
#include "RERHIVulkan/Buffer/RHIIndirectBuffer.h"
#include "RERHIVulkan/Buffer/RHIStructuredBuffer.h"
#include "RERHIVulkan/Buffer/RHITextureBuffer.h"
#include "RERHIVulkan/Buffer/RHIUniformBuffer.h"
#include "RERHIVulkan/Buffer/RHIVertexArray.h"
#include "RERHIVulkan/Query/RHIQueryPool.h"
#include "RERHIVulkan/RenderTarget/RHIFramebuffer.h"
#include "RERHIVulkan/RenderTarget/RHIRenderPass.h"
#include "RERHIVulkan/RenderTarget/RHISwapChain.h"
#include "RERHIVulkan/Shader/RHIComputeShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIFragmentShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIGeometryShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIVulkan/Shader/RHIMeshShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIShaderLanguageGLSL.h"
#include "RERHIVulkan/Shader/RHITaskShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITessellationControlShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITessellationEvaluationShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIVertexShaderGLSL.h"
#include "RERHIVulkan/Texture/RHITexture1D.h"
#include "RERHIVulkan/Texture/RHITexture1DArray.h"
#include "RERHIVulkan/Texture/RHITexture2D.h"
#include "RERHIVulkan/Texture/RHITexture2DArray.h"
#include "RERHIVulkan/Texture/RHITexture3D.h"
#include "RERHIVulkan/Texture/RHITextureCube.h"
#include "RERHIVulkan/Texture/RHITextureCubeArray.h"
#include "RERHIVulkan/Texture/RHITextureManager.h"
#include "RERHIVulkan/State/RHIComputePipelineState.h"
#include "RERHIVulkan/State/RHIGraphicsPipelineState.h"
#include "RERHIVulkan/State/RHISamplerState.h"


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

[[nodiscard]] VKAPI_ATTR void* VKAPI_CALL vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope)
{
  return RECore::Memory::reallocate(nullptr, 0, size, alignment);
}

[[nodiscard]] VKAPI_ATTR void* VKAPI_CALL vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope)
{
  return RECore::Memory::reallocate(pOriginal, 0, size, alignment);
}

VKAPI_ATTR void VKAPI_CALL vkFreeFunction(void* pUserData, void* pMemory)
{
  RECore::Memory::reallocate(pMemory, 0, 0, 1);
}

namespace ImplementationDispatch
{


//[-------------------------------------------------------]
//[ Command buffer                                        ]
//[-------------------------------------------------------]
void DispatchCommandBuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DispatchCommandBuffer* realData = static_cast<const RERHI::Command::DispatchCommandBuffer*>(data);
  RHI_ASSERT(nullptr != realData->commandBufferToDispatch, "The Vulkan command buffer to dispatch must be valid")
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).dispatchCommandBufferInternal(*realData->commandBufferToDispatch);
}

//[-------------------------------------------------------]
//[ Graphics states                                       ]
//[-------------------------------------------------------]
void SetGraphicsRootSignature(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsRootSignature* realData = static_cast<const RERHI::Command::SetGraphicsRootSignature*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setGraphicsRootSignature(realData->rootSignature);
}

void SetGraphicsPipelineState(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsPipelineState* realData = static_cast<const RERHI::Command::SetGraphicsPipelineState*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setGraphicsPipelineState(realData->graphicsPipelineState);
}

void SetGraphicsResourceGroup(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsResourceGroup* realData = static_cast<const RERHI::Command::SetGraphicsResourceGroup*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setGraphicsResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
}

void SetGraphicsVertexArray(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Input-assembler (IA) stage
  const RERHI::Command::SetGraphicsVertexArray* realData = static_cast<const RERHI::Command::SetGraphicsVertexArray*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setGraphicsVertexArray(realData->vertexArray);
}

void SetGraphicsViewports(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsViewports* realData = static_cast<const RERHI::Command::SetGraphicsViewports*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setGraphicsViewports(realData->numberOfViewports, (nullptr != realData->viewports) ? realData->viewports : reinterpret_cast<const RERHI::Viewport*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsScissorRectangles(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsScissorRectangles* realData = static_cast<const RERHI::Command::SetGraphicsScissorRectangles*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setGraphicsScissorRectangles(realData->numberOfScissorRectangles, (nullptr != realData->scissorRectangles) ? realData->scissorRectangles : reinterpret_cast<const RERHI::ScissorRectangle*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsRenderTarget(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Output-merger (OM) stage
  const RERHI::Command::SetGraphicsRenderTarget* realData = static_cast<const RERHI::Command::SetGraphicsRenderTarget*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setGraphicsRenderTarget(realData->renderTarget);
}

void ClearGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ClearGraphics* realData = static_cast<const RERHI::Command::ClearGraphics*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).clearGraphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
}

void DrawGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawGraphics* realData = static_cast<const RERHI::Command::DrawGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).drawGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).drawGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawIndexedGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawIndexedGraphics* realData = static_cast<const RERHI::Command::DrawIndexedGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).drawIndexedGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).drawIndexedGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawMeshTasks(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawMeshTasks* realData = static_cast<const RERHI::Command::DrawMeshTasks*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).drawMeshTasks(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).drawMeshTasksEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void SetComputeRootSignature(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputeRootSignature* realData = static_cast<const RERHI::Command::SetComputeRootSignature*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setComputeRootSignature(realData->rootSignature);
}

void SetComputePipelineState(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputePipelineState* realData = static_cast<const RERHI::Command::SetComputePipelineState*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setComputePipelineState(realData->computePipelineState);
}

void SetComputeResourceGroup(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputeResourceGroup* realData = static_cast<const RERHI::Command::SetComputeResourceGroup*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setComputeResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
}

void DispatchCompute(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DispatchCompute* realData = static_cast<const RERHI::Command::DispatchCompute*>(data);
  vkCmdDispatch(static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).getVulkanContext().getVkCommandBuffer(), realData->groupCountX, realData->groupCountY, realData->groupCountZ);
}

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void SetTextureMinimumMaximumMipmapIndex(const void* data, [[maybe_unused]] RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const RERHI::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
  RHI_ASSERT(static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).getContext(), realData->texture->getResourceType() == RERHI::ResourceType::TEXTURE_2D, "Unsupported Vulkan texture resource type")
  static_cast<RERHIVulkan::Texture2D*>(realData->texture)->setMinimumMaximumMipmapIndex(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
}

void ResolveMultisampleFramebuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResolveMultisampleFramebuffer* realData = static_cast<const RERHI::Command::ResolveMultisampleFramebuffer*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).resolveMultisampleFramebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
}

void CopyResource(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::CopyResource* realData = static_cast<const RERHI::Command::CopyResource*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).copyResource(*realData->destinationResource, *realData->sourceResource);
}

void GenerateMipmaps(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::GenerateMipmaps* realData = static_cast<const RERHI::Command::GenerateMipmaps*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).generateMipmaps(*realData->resource);
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
  RHI_ASSERT(false, "The set uniform command isn't supported by the Vulkan RHI implementation")
}

//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void ResetQueryPool(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResetQueryPool* realData = static_cast<const RERHI::Command::ResetQueryPool*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).resetQueryPool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
}

void BeginQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::BeginQuery* realData = static_cast<const RERHI::Command::BeginQuery*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).beginQuery(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
}

void EndQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::EndQuery* realData = static_cast<const RERHI::Command::EndQuery*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).endQuery(*realData->queryPool, realData->queryIndex);
}

void WriteTimestampQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::WriteTimestampQuery* realData = static_cast<const RERHI::Command::WriteTimestampQuery*>(data);
  static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).writeTimestampQuery(*realData->queryPool, realData->queryIndex);
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void SetDebugMarker(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::SetDebugMarker* realData = static_cast<const RERHI::Command::SetDebugMarker*>(data);
					static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).setDebugMarker(realData->name);
				}

				void BeginDebugEvent(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::BeginDebugEvent* realData = static_cast<const RERHI::Command::BeginDebugEvent*>(data);
					static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).beginDebugEvent(realData->name);
				}

				void EndDebugEvent(const void*, RERHI::RHIDynamicRHI& rhi)
				{
					static_cast<RERHIVulkan::RHIDynamicRHI&>(rhi).endDebugEvent();
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

void beginVulkanRenderPass(const RERHI::RHIRenderTarget& renderTarget, VkRenderPass vkRenderPass, VkFramebuffer vkFramebuffer, RECore::uint32 numberOfAttachments, const RERHIVulkan::RHIDynamicRHI::VkClearValues& vkClearValues, VkCommandBuffer vkCommandBuffer)
{
  // Get render target dimension
  RECore::uint32 width = 1;
  RECore::uint32 height = 1;
  renderTarget.getWidthAndHeight(width, height);

  // Begin Vulkan render pass
  const VkRenderPassBeginInfo vkRenderPassBeginInfo =
    {
      VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,	// sType (VkStructureType)
      nullptr,									// pNext (const void*)
      vkRenderPass,								// renderPass (VkRenderPass)
      vkFramebuffer,								// framebuffer (VkFramebuffer)
      { // renderArea (VkRect2D)
        { 0, 0 },								// offset (VkOffset2D)
        { width, height }						// extent (VkExtent2D)
      },
      numberOfAttachments,						// clearValueCount (RECore::uint32)
      vkClearValues.data()						// pClearValues (const VkClearValue*)
    };
  vkCmdBeginRenderPass(vkCommandBuffer, &vkRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
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
namespace RERHIVulkan {

//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIDynamicRHI::RHIDynamicRHI(const RERHI::RHIContext& context) :
  RERHI::RHIDynamicRHI(RERHI::NameId::VULKAN, context),
  VertexArrayMakeId(),
  GraphicsPipelineStateMakeId(),
  ComputePipelineStateMakeId(),
  mVkAllocationCallbacks{nullptr, &::detail::vkAllocationFunction, &::detail::vkReallocationFunction, &::detail::vkFreeFunction, nullptr, nullptr},
  mVulkanRuntimeLinking(nullptr),
  mVulkanContext(nullptr),
  mShaderLanguageGlsl(nullptr),
  mGraphicsRootSignature(nullptr),
  mComputeRootSignature(nullptr),
  mDefaultSamplerState(nullptr),
  mInsideVulkanRenderPass(false),
  mVkClearValues{},
  mVertexArray(nullptr),
  mRenderTarget(nullptr)
{
  // TODO(naetherm) Make it possible to enable/disable validation from the outside?
#ifdef DEBUG
  const bool enableValidation = true;
#else
  const bool enableValidation = false;
#endif

  // Is Vulkan available?
  mVulkanRuntimeLinking = RHI_NEW(mContext, VulkanRuntimeLinking)(*this, enableValidation);
  if (mVulkanRuntimeLinking->isVulkanAvailable())
  {
    // TODO(naetherm) Add external Vulkan context support
    mVulkanContext = RHI_NEW(mContext, VulkanContext)(*this);

    // Is the Vulkan context initialized?
    if (mVulkanContext->isInitialized())
    {
      // Initialize the capabilities
      initializeCapabilities();

      // Create the default sampler state
      mDefaultSamplerState = createSamplerState(RERHI::RHISamplerState::getDefaultSamplerState());

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
  // Set no vertex array reference, in case we have one
  setGraphicsVertexArray(nullptr);

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
					RHI_ASSERT(false, "The Vulkan RHI implementation is going to be destroyed, but there are still %u resource instances left (memory leak)", numberOfCurrentResources)
				}
				else
				{
					RHI_ASSERT(false, "The Vulkan RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
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

  // Destroy the Vulkan context instance
  RHI_DELETE(mContext, VulkanContext, mVulkanContext);

  // Destroy the Vulkan runtime linking instance
  RHI_DELETE(mContext, VulkanRuntimeLinking, mVulkanRuntimeLinking);
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
  if (nullptr != graphicsPipelineState)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *graphicsPipelineState)

    // Bind Vulkan graphics pipeline
    vkCmdBindPipeline(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<GraphicsPipelineState*>(graphicsPipelineState)->getVkPipeline());
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::setGraphicsResourceGroup(RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  // Security checks
#ifdef DEBUG
  {
			RHI_ASSERT(nullptr != mGraphicsRootSignature, "No Vulkan RHI implementation graphics root signature set")
			const RERHI::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
			RHI_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The Vulkan RHI implementation root parameter index is out of bounds")
			const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			RHI_ASSERT(RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The Vulkan RHI implementation root parameter index doesn't reference a descriptor table")
			RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The Vulkan RHI implementation descriptor ranges is a null pointer")
		}
#endif

  if (nullptr != resourceGroup)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *resourceGroup)

    // Bind Vulkan descriptor set
    const VkDescriptorSet vkDescriptorSet = static_cast<ResourceGroup*>(resourceGroup)->getVkDescriptorSet();
    if (VK_NULL_HANDLE != vkDescriptorSet)
    {
      vkCmdBindDescriptorSets(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsRootSignature->getVkPipelineLayout(), rootParameterIndex, 1, &vkDescriptorSet, 0, nullptr);
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

      // Unset the currently used vertex array
      unsetGraphicsVertexArray();

      // Set new vertex array and add a reference to it
      mVertexArray = static_cast<VertexArray*>(vertexArray);
      mVertexArray->AddReference();

      // Bind Vulkan buffers
      mVertexArray->bindVulkanBuffers(getVulkanContext().getVkCommandBuffer());
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
  RHI_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid Vulkan rasterizer state viewports")

  // Set Vulkan viewport
  // -> We're using the "VK_KHR_maintenance1"-extension ("VK_KHR_MAINTENANCE1_EXTENSION_NAME"-definition) to be able to specify a negative viewport height,
  //    this way we don't have to apply "<output position>.y = -<output position>.y" inside vertex shaders to compensate for the Vulkan coordinate system
  // TODO(naetherm) Add support for multiple viewports
  VkViewport vkViewport = reinterpret_cast<const VkViewport*>(viewports)[0];
  vkViewport.y += vkViewport.height;
  vkViewport.height = -vkViewport.height;
  vkCmdSetViewport(getVulkanContext().getVkCommandBuffer(), 0, 1, &vkViewport);
}

void RHIDynamicRHI::setGraphicsScissorRectangles([[maybe_unused]] RECore::uint32 numberOfScissorRectangles, const RERHI::ScissorRectangle* scissorRectangles)
{
  // Rasterizer (RS) stage

  // Sanity check
  RHI_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid Vulkan rasterizer state scissor rectangles")

  // Set Vulkan scissor
  // TODO(naetherm) Add support for multiple scissor rectangles. Change "RERHI::ScissorRectangle" to Vulkan style to make it the primary API on the long run?
  const VkRect2D vkRect2D =
    {
      { static_cast<RECore::int32>(scissorRectangles[0].topLeftX), static_cast<RECore::int32>(scissorRectangles[0].topLeftY) },
      { static_cast<RECore::uint32>(scissorRectangles[0].bottomRightX - scissorRectangles[0].topLeftX), static_cast<RECore::uint32>(scissorRectangles[0].bottomRightY - scissorRectangles[0].topLeftY) }
    };
  vkCmdSetScissor(getVulkanContext().getVkCommandBuffer(), 0, 1, &vkRect2D);
}

void RHIDynamicRHI::setGraphicsRenderTarget(RERHI::RHIRenderTarget* renderTarget)
{
  // Output-merger (OM) stage

  // New render target?
  if (mRenderTarget != renderTarget)
  {
    // Release the render target reference, in case we have one
    if (nullptr != mRenderTarget)
    {
      // Start Vulkan render pass, if necessary (for e.g. clearing)
      if (!mInsideVulkanRenderPass && ((mRenderTarget->getResourceType() == RERHI::ResourceType::SWAP_CHAIN && nullptr == renderTarget) || mRenderTarget->getResourceType() == RERHI::ResourceType::FRAMEBUFFER))
      {
        beginVulkanRenderPass();
      }

      // End Vulkan render pass, if necessary
      if (mInsideVulkanRenderPass)
      {
        vkCmdEndRenderPass(getVulkanContext().getVkCommandBuffer());
        mInsideVulkanRenderPass = false;
      }

      // Release
      mRenderTarget->Release();
      mRenderTarget = nullptr;
    }

    // Set a render target?
    if (nullptr != renderTarget)
    {
      // Sanity check
      RHI_MATCH_CHECK(*this, *renderTarget)

      // Set new render target and add a reference to it
      mRenderTarget = renderTarget;
      mRenderTarget->AddReference();

      // Set clear color and clear depth stencil values
      const RECore::uint32 numberOfColorAttachments = static_cast<const RenderPass&>(mRenderTarget->getRenderPass()).getNumberOfColorAttachments();
      RHI_ASSERT(numberOfColorAttachments < 8, "Vulkan only supports 7 render pass color attachments")
      for (RECore::uint32 i = 0; i < numberOfColorAttachments; ++i)
      {
        mVkClearValues[i] = VkClearValue{0.0f, 0.0f, 0.0f, 1.0f};
      }
      mVkClearValues[numberOfColorAttachments] = VkClearValue{ 1.0f, 0 };
    }
  }
}

void RHIDynamicRHI::clearGraphics(RECore::uint32 clearFlags, const float color[4], float z, RECore::uint32 stencil)
{
  // Sanity checks
  RHI_ASSERT(nullptr != mRenderTarget, "Can't execute Vulkan clear command without a render target set")
  RHI_ASSERT(!mInsideVulkanRenderPass, "Can't execute clear command inside a Vulkan render pass")
  RHI_ASSERT(z >= 0.0f && z <= 1.0f, "The Vulkan clear graphics z value must be between [0, 1] (inclusive)")

  // Clear color
  const RECore::uint32 numberOfColorAttachments = static_cast<const RenderPass&>(mRenderTarget->getRenderPass()).getNumberOfColorAttachments();
  RHI_ASSERT(numberOfColorAttachments < 8, "Vulkan only supports 7 render pass color attachments")
  if (clearFlags & RERHI::ClearFlag::COLOR)
  {
    for (RECore::uint32 i = 0; i < numberOfColorAttachments; ++i)
    {
      memcpy(mVkClearValues[i].color.float32, &color[0], sizeof(float) * 4);
    }
  }

  // Clear depth stencil
  if ((clearFlags & RERHI::ClearFlag::DEPTH) || (clearFlags & RERHI::ClearFlag::STENCIL))
  {
    mVkClearValues[numberOfColorAttachments].depthStencil.depth = z;
    mVkClearValues[numberOfColorAttachments].depthStencil.stencil = stencil;
  }
}

void RHIDynamicRHI::drawGraphics(const RERHI::RHIIndirectBuffer& indirectBuffer, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, indirectBuffer)
  RHI_ASSERT(numberOfDraws > 0, "Number of Vulkan draws must not be zero")
  // It's possible to draw without "mVertexArray"

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass)
  {
    beginVulkanRenderPass();
  }

  // Vulkan draw indirect command
  vkCmdDrawIndirect(getVulkanContext().getVkCommandBuffer(), static_cast<const IndirectBuffer&>(indirectBuffer).getVkBuffer(), indirectBufferOffset, numberOfDraws, sizeof(VkDrawIndirectCommand));
}

void RHIDynamicRHI::drawGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")
  // It's possible to draw without "mVertexArray"

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass)
  {
    beginVulkanRenderPass();
  }

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-draw-indirect emulation");
			}
#endif
  const VkCommandBuffer vkCommandBuffer = getVulkanContext().getVkCommandBuffer();
  for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
  {
    // Draw and advance
    const RERHI::DrawArguments& drawArguments = *reinterpret_cast<const RERHI::DrawArguments*>(emulationData);
    vkCmdDraw(vkCommandBuffer, drawArguments.vertexCountPerInstance, drawArguments.instanceCount, drawArguments.startVertexLocation, drawArguments.startInstanceLocation);
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
  RHI_ASSERT(numberOfDraws > 0, "Number of Vulkan draws must not be zero")
  RHI_ASSERT(nullptr != mVertexArray, "Vulkan draw indexed needs a set vertex array")
  RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "Vulkan draw indexed needs a set vertex array which contains an index buffer")

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass)
  {
    beginVulkanRenderPass();
  }

  // Vulkan draw indexed indirect command
  vkCmdDrawIndexedIndirect(getVulkanContext().getVkCommandBuffer(), static_cast<const IndirectBuffer&>(indirectBuffer).getVkBuffer(), indirectBufferOffset, numberOfDraws, sizeof(VkDrawIndexedIndirectCommand));
}

void RHIDynamicRHI::drawIndexedGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")
  RHI_ASSERT(nullptr != mVertexArray, "Vulkan draw indexed needs a set vertex array")
  RHI_ASSERT(nullptr != mVertexArray->getIndexBuffer(), "Vulkan draw indexed needs a set vertex array which contains an index buffer")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Start Vulkan render pass, if necessary
  if (!mInsideVulkanRenderPass)
  {
    beginVulkanRenderPass();
  }

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
#endif
  const VkCommandBuffer vkCommandBuffer = getVulkanContext().getVkCommandBuffer();
  for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
  {
    // Draw and advance
    const RERHI::DrawIndexedArguments& drawIndexedArguments = *reinterpret_cast<const RERHI::DrawIndexedArguments*>(emulationData);
    vkCmdDrawIndexed(vkCommandBuffer, drawIndexedArguments.indexCountPerInstance, drawIndexedArguments.instanceCount, drawIndexedArguments.startIndexLocation, drawIndexedArguments.baseVertexLocation, drawIndexedArguments.startInstanceLocation);
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
  // vkCmdDrawMeshTasksIndirectNV
  // vkCmdDrawMeshTasksIndirectCountNV
}

void RHIDynamicRHI::drawMeshTasksEmulated([[maybe_unused]] const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset, RECore::uint32 numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The Vulkan emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of Vulkan draws must not be zero")

  // TODO(naetherm) Currently no buffer overflow check due to lack of interface provided data
  emulationData += indirectBufferOffset;

  // Emit the draw calls
#ifdef DEBUG
  if (numberOfDraws > 1)
			{
				beginDebugEvent("Multi-indexed-draw-indirect emulation");
			}
#endif
  const VkCommandBuffer vkCommandBuffer = getVulkanContext().getVkCommandBuffer();
  for (RECore::uint32 i = 0; i < numberOfDraws; ++i)
  {
    const RERHI::DrawMeshTasksArguments& drawMeshTasksArguments = *reinterpret_cast<const RERHI::DrawMeshTasksArguments*>(emulationData);

    // Draw and advance
    vkCmdDrawMeshTasksNV(vkCommandBuffer, drawMeshTasksArguments.numberOfTasks, drawMeshTasksArguments.firstTask);
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
  if (nullptr != computePipelineState)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *computePipelineState)

    // Bind Vulkan compute pipeline
    vkCmdBindPipeline(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, static_cast<ComputePipelineState*>(computePipelineState)->getVkPipeline());
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::setComputeResourceGroup(RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  // Security checks
#ifdef DEBUG
  {
			RHI_ASSERT(nullptr != mComputeRootSignature, "No Vulkan RHI implementation compute root signature set")
			const RERHI::RootSignature& rootSignature = mComputeRootSignature->getRootSignature();
			RHI_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The Vulkan RHI implementation root parameter index is out of bounds")
			const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			RHI_ASSERT(RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The Vulkan RHI implementation root parameter index doesn't reference a descriptor table")
			RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The Vulkan RHI implementation descriptor ranges is a null pointer")
		}
#endif

  if (nullptr != resourceGroup)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *resourceGroup)

    // Bind Vulkan descriptor set
    const VkDescriptorSet vkDescriptorSet = static_cast<ResourceGroup*>(resourceGroup)->getVkDescriptorSet();
    if (VK_NULL_HANDLE != vkDescriptorSet)
    {
      vkCmdBindDescriptorSets(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, mComputeRootSignature->getVkPipelineLayout(), rootParameterIndex, 1, &vkDescriptorSet, 0, nullptr);
    }
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}


//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resolveMultisampleFramebuffer(RERHI::RHIRenderTarget&, RERHI::RHIFramebuffer&)
{
  // TODO(naetherm) Implement me
}

void RHIDynamicRHI::copyResource(RERHI::RHIResource&, RERHI::RHIResource&)
{
  // TODO(naetherm) Implement me
}

void RHIDynamicRHI::generateMipmaps(RERHI::RHIResource&)
{
  // TODO(naetherm) Implement me: Search for https://github.com/SaschaWillems/Vulkan/tree/master/texturemipmapgen inside this cpp file and unify the code to be able to reuse it in here
}


//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resetQueryPool(RERHI::RHIQueryPool& queryPool, RECore::uint32 firstQueryIndex, RECore::uint32 numberOfQueries)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Reset Vulkan query pool
  vkCmdResetQueryPool(getVulkanContext().getVkCommandBuffer(), static_cast<const QueryPool&>(queryPool).getVkQueryPool(), firstQueryIndex, numberOfQueries);
}

void RHIDynamicRHI::beginQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex, RECore::uint32 queryControlFlags)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Begin Vulkan query
  vkCmdBeginQuery(getVulkanContext().getVkCommandBuffer(), static_cast<const QueryPool&>(queryPool).getVkQueryPool(), queryIndex, ((queryControlFlags & RERHI::QueryControlFlags::PRECISE) != 0) ? VK_QUERY_CONTROL_PRECISE_BIT : 0u);
}

void RHIDynamicRHI::endQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // End Vulkan query
  vkCmdEndQuery(getVulkanContext().getVkCommandBuffer(), static_cast<const QueryPool&>(queryPool).getVkQueryPool(), queryIndex);
}

void RHIDynamicRHI::writeTimestampQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Write Vulkan timestamp query
  vkCmdWriteTimestamp(getVulkanContext().getVkCommandBuffer(), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, static_cast<const QueryPool&>(queryPool).getVkQueryPool(), queryIndex);
}


//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDynamicRHI::setDebugMarker(const char* name)
		{
			if (nullptr != vkCmdDebugMarkerInsertEXT)
			{
				RHI_ASSERT(nullptr != name, "Vulkan debug marker names must not be a null pointer")
				const VkDebugMarkerMarkerInfoEXT vkDebugMarkerMarkerInfoEXT =
				{
					VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,	// sType (VkStructureType)
					nullptr,										// pNext (const void*)
					name,											// pMarkerName (const char*)
					{ // color[4] (float)
						0.0f,
						0.0f,
						1.0f,	// Blue
						1.0f
					}
				};
				vkCmdDebugMarkerInsertEXT(getVulkanContext().getVkCommandBuffer(), &vkDebugMarkerMarkerInfoEXT);
			}
		}

		void RHIDynamicRHI::beginDebugEvent(const char* name)
		{
			if (nullptr != vkCmdDebugMarkerBeginEXT)
			{
				RHI_ASSERT(nullptr != name, "Vulkan debug event names must not be a null pointer")
				const VkDebugMarkerMarkerInfoEXT vkDebugMarkerMarkerInfoEXT =
				{
					VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,	// sType (VkStructureType)
					nullptr,										// pNext (const void*)
					name,											// pMarkerName (const char*)
					{ // color[4] (float)
						0.0f,
						1.0f,	// Green
						0.0f,
						1.0f
					}
				};
				vkCmdDebugMarkerBeginEXT(getVulkanContext().getVkCommandBuffer(), &vkDebugMarkerMarkerInfoEXT);
			}
		}

		void RHIDynamicRHI::endDebugEvent()
		{
			if (nullptr != vkCmdDebugMarkerEndEXT)
			{
				vkCmdDebugMarkerEndEXT(getVulkanContext().getVkCommandBuffer());
			}
		}
#endif


//[-------------------------------------------------------]
//[ Public virtual RERHI::RHIDynamicRHI methods                      ]
//[-------------------------------------------------------]
const char* RHIDynamicRHI::getName() const
{
  return "Vulkan";
}

bool RHIDynamicRHI::isInitialized() const
{
  // Is the Vulkan context initialized?
  return (nullptr != mVulkanContext && mVulkanContext->isInitialized());
}

bool RHIDynamicRHI::isDebugEnabled()
{
  // Check for any "VK_EXT_debug_marker"-extension function pointer
  return (nullptr != vkDebugMarkerSetObjectTagEXT || nullptr != vkDebugMarkerSetObjectNameEXT || nullptr != vkCmdDebugMarkerBeginEXT || nullptr != vkCmdDebugMarkerEndEXT || nullptr != vkCmdDebugMarkerInsertEXT);
}


//[-------------------------------------------------------]
//[ Shader language                                       ]
//[-------------------------------------------------------]
RECore::uint32 RHIDynamicRHI::getNumberOfShaderLanguages() const
{
  // Done, return the number of supported shader languages
  return 1;
}

const char* RHIDynamicRHI::getShaderLanguageName([[maybe_unused]] RECore::uint32 index) const
{
  RHI_ASSERT(index < getNumberOfShaderLanguages(), "Vulkan: Shader language index is out-of-bounds")
  return ::detail::GLSL_NAME;
}

RERHI::RHIShaderLanguage* RHIDynamicRHI::getShaderLanguage(const char* shaderLanguageName)
{
  // In case "shaderLanguage" is a null pointer, use the default shader language
  if (nullptr != shaderLanguageName)
  {
    // Optimization: Check for shader language name pointer match, first
    // -> "ShaderLanguageSeparate::NAME" has the same value
    if (::detail::GLSL_NAME == shaderLanguageName || !stricmp(shaderLanguageName, ::detail::GLSL_NAME))
    {
      // If required, create the GLSL shader language instance right now
      if (nullptr == mShaderLanguageGlsl)
      {
        mShaderLanguageGlsl = RHI_NEW(mContext, ShaderLanguageGlsl(*this));
        mShaderLanguageGlsl->AddReference();	// Internal RHI reference
      }
      return mShaderLanguageGlsl;
    }
  }
  else
  {
    // Return the shader language instance as default
    return getShaderLanguage(::detail::GLSL_NAME);
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
  RHI_ASSERT(numberOfQueries > 0, "Vulkan: Number of queries mustn't be zero")
  return RHI_NEW(mContext, QueryPool)(*this, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHISwapChain* RHIDynamicRHI::createSwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle, bool RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, renderPass)
  RHI_ASSERT(NULL_HANDLE != windowHandle.nativeWindowHandle || nullptr != windowHandle.renderWindow, "Vulkan: The provided native window handle or render window must not be a null handle / null pointer")

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
  RHI_ASSERT(nullptr != graphicsPipelineState.rootSignature, "Vulkan: Invalid graphics pipeline state root signature")
  RHI_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "Vulkan: Invalid graphics pipeline state graphics program")
  RHI_ASSERT(nullptr != graphicsPipelineState.renderPass, "Vulkan: Invalid graphics pipeline state render pass")

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
  if (ComputePipelineStateMakeId.createID(id))
  {
    return RHI_NEW(mContext, ComputePipelineState)(*this, rootSignature, computeShader, id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
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
  return RHI_NEW(mContext, SamplerState)(*this, samplerState RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Resource handling                                     ]
//[-------------------------------------------------------]
bool RHIDynamicRHI::map(RERHI::RHIResource& resource, RECore::uint32, RERHI::MapType, RECore::uint32, RERHI::MappedSubresource& mappedSubresource)
{
  // Evaluate the resource type
  switch (resource.getResourceType())
  {
    case RERHI::ResourceType::VERTEX_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<VertexBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case RERHI::ResourceType::INDEX_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<IndexBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case RERHI::ResourceType::TEXTURE_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<TextureBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case RERHI::ResourceType::STRUCTURED_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<StructuredBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case RERHI::ResourceType::INDIRECT_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<IndirectBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
    }

    case RERHI::ResourceType::UNIFORM_BUFFER:
    {
      mappedSubresource.rowPitch   = 0;
      mappedSubresource.depthPitch = 0;
      return (vkMapMemory(getVulkanContext().getVkDevice(), static_cast<UniformBuffer&>(resource).getVkDeviceMemory(), 0, VK_WHOLE_SIZE, 0, &mappedSubresource.data) == VK_SUCCESS);
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
      // TODO(naetherm) Implement me
      return false;
    }

    case RERHI::ResourceType::TEXTURE_2D_ARRAY:
    {
      // TODO(naetherm) Implement me
      return false;
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
    {
      vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<VertexBuffer&>(resource).getVkDeviceMemory());
      break;
    }

    case RERHI::ResourceType::INDEX_BUFFER:
    {
      vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<IndexBuffer&>(resource).getVkDeviceMemory());
      break;
    }

    case RERHI::ResourceType::TEXTURE_BUFFER:
    {
      vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<TextureBuffer&>(resource).getVkDeviceMemory());
      break;
    }

    case RERHI::ResourceType::STRUCTURED_BUFFER:
    {
      vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<StructuredBuffer&>(resource).getVkDeviceMemory());
      break;
    }

    case RERHI::ResourceType::INDIRECT_BUFFER:
    {
      vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<IndirectBuffer&>(resource).getVkDeviceMemory());
      break;
    }

    case RERHI::ResourceType::UNIFORM_BUFFER:
    {
      vkUnmapMemory(getVulkanContext().getVkDevice(), static_cast<UniformBuffer&>(resource).getVkDeviceMemory());
      break;
    }

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

bool RHIDynamicRHI::getQueryPoolResults(RERHI::RHIQueryPool& queryPool, RECore::uint32 numberOfDataBytes, RECore::uint8* data, RECore::uint32 firstQueryIndex, RECore::uint32 numberOfQueries, RECore::uint32 strideInBytes, [[maybe_unused]] RECore::uint32 queryResultFlags)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, queryPool)

  // Query pool type dependent processing
  const QueryPool& vulkanQueryPool = static_cast<const QueryPool&>(queryPool);
  switch (vulkanQueryPool.getQueryType())
  {
    case RERHI::QueryType::OCCLUSION:
    case RERHI::QueryType::TIMESTAMP:	// TODO(naetherm) Convert time to nanoseconds, see e.g. http://vulkan-spec-chunked.ahcox.com/ch16s05.html - VkPhysicalDeviceLimits::timestampPeriod - The number of nanoseconds it takes for a timestamp value to be incremented by 1
    {
      // Get Vulkan query pool results
      const VkQueryResultFlags vkQueryResultFlags = 0u;
      // const VkQueryResultFlags vkQueryResultFlags = ((queryResultFlags & RERHI::QueryResultFlags::WAIT) != 0) ? VK_QUERY_RESULT_WAIT_BIT : 0u;	// TODO(naetherm)
      return (vkGetQueryPoolResults(getVulkanContext().getVkDevice(), vulkanQueryPool.getVkQueryPool(), firstQueryIndex, numberOfQueries, numberOfDataBytes, data, strideInBytes, VK_QUERY_RESULT_64_BIT | vkQueryResultFlags) == VK_SUCCESS);
    }

    case RERHI::QueryType::PIPELINE_STATISTICS:
    {
      // Our setup results in the same structure layout as used by "D3D11_QUERY_DATA_PIPELINE_STATISTICS" which we use for "RERHI::PipelineStatisticsQueryResult"
      const VkQueryResultFlags vkQueryResultFlags = 0u;
      // const VkQueryResultFlags vkQueryResultFlags = ((queryResultFlags & RERHI::QueryResultFlags::WAIT) != 0) ? VK_QUERY_RESULT_WAIT_BIT : 0u;	// TODO(naetherm)
      return (vkGetQueryPoolResults(getVulkanContext().getVkDevice(), vulkanQueryPool.getVkQueryPool(), firstQueryIndex, numberOfQueries, numberOfDataBytes, data, strideInBytes, VK_QUERY_RESULT_64_BIT | vkQueryResultFlags) == VK_SUCCESS);
    }
  }

  // Result not ready
  return false;
}


//[-------------------------------------------------------]
//[ Operation                                             ]
//[-------------------------------------------------------]
void RHIDynamicRHI::dispatchCommandBuffer(const RERHI::RHICommandBuffer& commandBuffer)
{
  // Sanity check
  RHI_ASSERT(!commandBuffer.isEmpty(), "The Vulkan command buffer to dispatch mustn't be empty")

  // Begin Vulkan command buffer
  // -> This automatically resets the Vulkan command buffer in case it was previously already recorded
  static constexpr VkCommandBufferBeginInfo vkCommandBufferBeginInfo =
    {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	// sType (VkStructureType)
      nullptr,										// pNext (const void*)
      0,												// flags (VkCommandBufferUsageFlags)
      nullptr											// pInheritanceInfo (const VkCommandBufferInheritanceInfo*)
    };
  if (vkBeginCommandBuffer(getVulkanContext().getVkCommandBuffer(), &vkCommandBufferBeginInfo) == VK_SUCCESS)
  {
    // Dispatch command buffer
    dispatchCommandBufferInternal(commandBuffer);

    // We need to forget about the currently set render target
    // -> "Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT" Object: "54336828" Location: "0" Message code: "0" Layer prefix: "Validation" Message: " [ VUID-vkEndCommandBuffer-commandBuffer-00060 ] Object: 0x33d1d3c (Type = 6) | vkEndCommandBuffer(): It is invalid to issue this call inside an active render pass (0x20). The Vulkan spec states: If commandBuffer is a primary command buffer, there must not be an active render pass instance (https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-vkEndCommandBuffer-commandBuffer-00060)" "
    setGraphicsRenderTarget(nullptr);

    // We need to forget about the currently set vertex array
    // -> "Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT" Object: "217049444" Location: "0" Message code: "0" Layer prefix: "Validation" Message: " [ UNASSIGNED-CoreValidation-DrawState-VtxIndexOutOfBounds ] Object: 0xcefe964 (Type = 6) | The Pipeline State Object (0x1b) expects that this Command Buffer's vertex binding Index 0 should be set via vkCmdBindVertexBuffers. This is because VkVertexInputBindingDescription struct at index 0 of pVertexBindingDescriptions has a binding value of 0." "
    unsetGraphicsVertexArray();

    // End Vulkan command buffer
    if (vkEndCommandBuffer(getVulkanContext().getVkCommandBuffer()) != VK_SUCCESS)
    {
      // Error!
      RE_LOG(Critical, "Failed to end Vulkan command buffer instance")
    }
  }
  else
  {
    // Error!
    RE_LOG(Critical, "Failed to begin Vulkan command buffer instance")
  }
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void RHIDynamicRHI::initializeCapabilities()
{
  { // Get device name
    VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
    vkGetPhysicalDeviceProperties(mVulkanContext->getVkPhysicalDevice(), &vkPhysicalDeviceProperties);
    const size_t numberOfCharacters = ::detail::countof(mCapabilities.deviceName) - 1;
    strncpy(mCapabilities.deviceName, vkPhysicalDeviceProperties.deviceName, numberOfCharacters);
    mCapabilities.deviceName[numberOfCharacters] = '\0';
  }

  // Preferred swap chain texture format
  mCapabilities.preferredSwapChainColorTextureFormat = (SwapChain::findColorVkFormat(getContext(), mVulkanRuntimeLinking->getVkInstance(), *mVulkanContext) == VK_FORMAT_R8G8B8A8_UNORM) ? RERHI::TextureFormat::Enum::R8G8B8A8 : RERHI::TextureFormat::Enum::B8G8R8A8;

  { // Preferred swap chain depth stencil texture format
    const VkFormat depthVkFormat = SwapChain::findDepthVkFormat(mVulkanContext->getVkPhysicalDevice());
    if (VK_FORMAT_D32_SFLOAT == depthVkFormat)
    {
      mCapabilities.preferredSwapChainDepthStencilTextureFormat = RERHI::TextureFormat::Enum::D32_FLOAT;
    }
    else
    {
      // TODO(naetherm) Add support for "VK_FORMAT_D32_SFLOAT_S8_UINT" and "VK_FORMAT_D24_UNORM_S8_UINT"
      mCapabilities.preferredSwapChainDepthStencilTextureFormat = RERHI::TextureFormat::Enum::D32_FLOAT;
    }
  }

  // TODO(naetherm) Implement me, this in here is just a placeholder implementation

  { // "D3D_FEATURE_LEVEL_11_0"
    // Maximum number of viewports (always at least 1)
    mCapabilities.maximumNumberOfViewports = 16;

    // Maximum number of simultaneous render targets (if <1 render to texture is not supported)
    mCapabilities.maximumNumberOfSimultaneousRenderTargets = 8;

    // Maximum texture dimension
    mCapabilities.maximumTextureDimension = 16384;

    // Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
    mCapabilities.maximumNumberOf1DTextureArraySlices = 512;

    // Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
    mCapabilities.maximumNumberOf2DTextureArraySlices = 512;

    // Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
    mCapabilities.maximumNumberOfCubeTextureArraySlices = 512;

    // Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
    mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 128 * 1024 * 1024;	// TODO(naetherm) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention the texture buffer? Currently the OpenGL 3 minimum is used: 128 MiB.

    // Maximum indirect buffer size in bytes
    mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

    // Maximum number of multisamples (always at least 1, usually 8)
    mCapabilities.maximumNumberOfMultisamples = 1;	// TODO(naetherm) Add multisample support, when setting "VkPhysicalDeviceFeatures" -> "sampleRateShading" -> VK_TRUE don't forget to check whether or not the device sup pots the feature

    // Maximum anisotropy (always at least 1, usually 16)
    mCapabilities.maximumAnisotropy = 16;

    // Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
    mCapabilities.instancedArrays = true;

    // Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
    mCapabilities.drawInstanced = true;

    // Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
    mCapabilities.maximumNumberOfPatchVertices = 32;

    // Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
    mCapabilities.maximumNumberOfGsOutputVertices = 1024;	// TODO(naetherm) http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx does not mention it, so I assume it's 1024
  }

  // The rest is the same for all feature levels

  // Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
  // -> See https://msdn.microsoft.com/en-us/library/windows/desktop/ff819065(v=vs.85).aspx - "Resource Limits (Direct3D 11)" - "Number of elements in a constant buffer D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096)"
  // -> One element = float4 = 16 bytes
  mCapabilities.maximumUniformBufferSize = 4096 * 16;

  // Left-handed coordinate system with clip space depth value range 0..1
  mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = true;

  // Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
  mCapabilities.individualUniforms = false;

  // Base vertex supported for draw calls?
  mCapabilities.baseVertex = true;

  // Vulkan has native multithreading
  mCapabilities.nativeMultithreading = false;	// TODO(naetherm) Enable native multithreading when done

  // Vulkan has shader bytecode support
  mCapabilities.shaderBytecode = false;	// TODO(naetherm) Vulkan has shader bytecode support, set "mCapabilities.shaderBytecode" to true later on

  // Is there support for vertex shaders (VS)?
  mCapabilities.vertexShader = true;

  // Is there support for fragment shaders (FS)?
  mCapabilities.fragmentShader = true;

  // Is there support for task shaders (TS) and mesh shaders (MS)?
  mCapabilities.meshShader = (nullptr != vkCmdDrawMeshTasksNV);

  // Is there support for compute shaders (CS)?
  mCapabilities.computeShader = true;
}

void RHIDynamicRHI::unsetGraphicsVertexArray()
{
  // Release the currently used vertex array reference, in case we have one
  if (nullptr != mVertexArray)
  {
    // Do nothing since the Vulkan specification says "bindingCount must be greater than 0"
    // vkCmdBindVertexBuffers(getVulkanContext().getVkCommandBuffer(), 0, 0, nullptr, nullptr);

    // Release reference
    mVertexArray->Release();
    mVertexArray = nullptr;
  }
}

void RHIDynamicRHI::beginVulkanRenderPass()
{
  // Sanity checks
  RHI_ASSERT(!mInsideVulkanRenderPass, "We're already inside a Vulkan render pass")
  RHI_ASSERT(nullptr != mRenderTarget, "Can't begin a Vulkan render pass without a render target set")

  // Start Vulkan render pass
  const RECore::uint32 numberOfAttachments = static_cast<const RenderPass&>(mRenderTarget->getRenderPass()).getNumberOfAttachments();
  RHI_ASSERT(numberOfAttachments < 9, "Vulkan only supports 8 render pass attachments")
  switch (mRenderTarget->getResourceType())
  {
    case RERHI::ResourceType::SWAP_CHAIN:
    {
      const SwapChain* swapChain = static_cast<SwapChain*>(mRenderTarget);
      ::detail::beginVulkanRenderPass(*mRenderTarget, swapChain->getVkRenderPass(), swapChain->getCurrentVkFramebuffer(), numberOfAttachments, mVkClearValues, getVulkanContext().getVkCommandBuffer());
      break;
    }

    case RERHI::ResourceType::FRAMEBUFFER:
    {
      const Framebuffer* framebuffer = static_cast<Framebuffer*>(mRenderTarget);
      ::detail::beginVulkanRenderPass(*mRenderTarget, framebuffer->getVkRenderPass(), framebuffer->getVkFramebuffer(), numberOfAttachments, mVkClearValues, getVulkanContext().getVkCommandBuffer());
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
  mInsideVulkanRenderPass = true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan



//RERHIVULKAN_API
RERHI::RHIDynamicRHI* createVulkanRhiInstance(const RERHI::RHIContext& context)
{
  return RHI_NEW(context, RERHIVulkan::RHIDynamicRHI)(context);
}
RERHI::RHIDynamicRHI* createRhiInstance(const RERHI::RHIContext& context)
{
  return RHI_NEW(context, RERHIVulkan::RHIDynamicRHI)(context);
}