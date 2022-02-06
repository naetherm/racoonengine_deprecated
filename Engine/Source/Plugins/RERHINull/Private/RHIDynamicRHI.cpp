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
#include "RERHINull/RHIDynamicRHI.h"
#include "RERHINull/RHIRootSignature.h"
#include "RERHINull/RHIResourceGroup.h"
#include "RERHINull/Buffer/RHIBufferManager.h"
#include "RERHINull/RenderTarget/RHIFramebuffer.h"
#include "RERHINull/RenderTarget/RHIRenderPass.h"
#include "RERHINull/RenderTarget/RHISwapChain.h"
#include "RERHINull/Shader/RHIShaderLanguage.h"
#include "RERHINull/State/RHIComputePipelineState.h"
#include "RERHINull/State/RHIGraphicsPipelineState.h"
#include "RERHINull/State/RHISamplerState.h"
#include "RERHINull/Texture/RHITextureManager.h"


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
namespace ImplementationDispatch
{


//[-------------------------------------------------------]
//[ Command buffer                                        ]
//[-------------------------------------------------------]
void DispatchCommandBuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DispatchCommandBuffer* realData = static_cast<const RERHI::Command::DispatchCommandBuffer*>(data);
  RHI_ASSERT(nullptr != realData->commandBufferToDispatch, "The null command buffer to dispatch must be valid")
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).dispatchCommandBufferInternal(*realData->commandBufferToDispatch);
}

//[-------------------------------------------------------]
//[ Graphics                                              ]
//[-------------------------------------------------------]
void SetGraphicsRootSignature(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsRootSignature* realData = static_cast<const RERHI::Command::SetGraphicsRootSignature*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setGraphicsRootSignature(realData->rootSignature);
}

void SetGraphicsPipelineState(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsPipelineState* realData = static_cast<const RERHI::Command::SetGraphicsPipelineState*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setGraphicsPipelineState(realData->graphicsPipelineState);
}

void SetGraphicsResourceGroup(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetGraphicsResourceGroup* realData = static_cast<const RERHI::Command::SetGraphicsResourceGroup*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setGraphicsResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
}

void SetGraphicsVertexArray(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Input-assembler (IA) stage
  const RERHI::Command::SetGraphicsVertexArray* realData = static_cast<const RERHI::Command::SetGraphicsVertexArray*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setGraphicsVertexArray(realData->vertexArray);
}

void SetGraphicsViewports(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsViewports* realData = static_cast<const RERHI::Command::SetGraphicsViewports*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setGraphicsViewports(realData->numberOfViewports, (nullptr != realData->viewports) ? realData->viewports : reinterpret_cast<const RERHI::Viewport*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsScissorRectangles(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Rasterizer (RS) stage
  const RERHI::Command::SetGraphicsScissorRectangles* realData = static_cast<const RERHI::Command::SetGraphicsScissorRectangles*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setGraphicsScissorRectangles(realData->numberOfScissorRectangles, (nullptr != realData->scissorRectangles) ? realData->scissorRectangles : reinterpret_cast<const RERHI::ScissorRectangle*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData)));
}

void SetGraphicsRenderTarget(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  // Output-merger (OM) stage
  const RERHI::Command::SetGraphicsRenderTarget* realData = static_cast<const RERHI::Command::SetGraphicsRenderTarget*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setGraphicsRenderTarget(realData->renderTarget);
}

void ClearGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ClearGraphics* realData = static_cast<const RERHI::Command::ClearGraphics*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).clearGraphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
}

void DrawGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawGraphics* realData = static_cast<const RERHI::Command::DrawGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    // No resource owner security check in here, we only support emulated indirect buffer
    static_cast<RERHINull::RHIDynamicRHI&>(rhi).drawGraphicsEmulated(realData->indirectBuffer->getEmulationData(), realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHINull::RHIDynamicRHI&>(rhi).drawGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawIndexedGraphics(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawIndexedGraphics* realData = static_cast<const RERHI::Command::DrawIndexedGraphics*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    // No resource owner security check in here, we only support emulated indirect buffer
    static_cast<RERHINull::RHIDynamicRHI&>(rhi).drawIndexedGraphicsEmulated(realData->indirectBuffer->getEmulationData(), realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHINull::RHIDynamicRHI&>(rhi).drawIndexedGraphicsEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

void DrawMeshTasks(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DrawMeshTasks* realData = static_cast<const RERHI::Command::DrawMeshTasks*>(data);
  if (nullptr != realData->indirectBuffer)
  {
    // No resource owner security check in here, we only support emulated indirect buffer
    static_cast<RERHINull::RHIDynamicRHI&>(rhi).drawMeshTasksEmulated(realData->indirectBuffer->getEmulationData(), realData->indirectBufferOffset, realData->numberOfDraws);
  }
  else
  {
    static_cast<RERHINull::RHIDynamicRHI&>(rhi).drawMeshTasksEmulated(RERHI::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
  }
}

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
void SetComputeRootSignature(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputeRootSignature* realData = static_cast<const RERHI::Command::SetComputeRootSignature*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setComputeRootSignature(realData->rootSignature);
}

void SetComputePipelineState(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputePipelineState* realData = static_cast<const RERHI::Command::SetComputePipelineState*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setComputePipelineState(realData->computePipelineState);
}

void SetComputeResourceGroup(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::SetComputeResourceGroup* realData = static_cast<const RERHI::Command::SetComputeResourceGroup*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).setComputeResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
}

void DispatchCompute(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::DispatchCompute* realData = static_cast<const RERHI::Command::DispatchCompute*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).dispatchCompute(realData->groupCountX, realData->groupCountY, realData->groupCountZ);
}

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void SetTextureMinimumMaximumMipmapIndex(const void*, RERHI::RHIDynamicRHI&)
{}

void ResolveMultisampleFramebuffer(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResolveMultisampleFramebuffer* realData = static_cast<const RERHI::Command::ResolveMultisampleFramebuffer*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).resolveMultisampleFramebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
}

void CopyResource(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::CopyResource* realData = static_cast<const RERHI::Command::CopyResource*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).copyResource(*realData->destinationResource, *realData->sourceResource);
}

void GenerateMipmaps(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::GenerateMipmaps* realData = static_cast<const RERHI::Command::GenerateMipmaps*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).generateMipmaps(*realData->resource);
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
  RHI_ASSERT(false, "The set uniform command isn't supported by the null RHI implementation")
}

//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void ResetQueryPool(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::ResetQueryPool* realData = static_cast<const RERHI::Command::ResetQueryPool*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).resetQueryPool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
}

void BeginQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::BeginQuery* realData = static_cast<const RERHI::Command::BeginQuery*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).beginQuery(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
}

void EndQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::EndQuery* realData = static_cast<const RERHI::Command::EndQuery*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).endQuery(*realData->queryPool, realData->queryIndex);
}

void WriteTimestampQuery(const void* data, RERHI::RHIDynamicRHI& rhi)
{
  const RERHI::Command::WriteTimestampQuery* realData = static_cast<const RERHI::Command::WriteTimestampQuery*>(data);
  static_cast<RERHINull::RHIDynamicRHI&>(rhi).writeTimestampQuery(*realData->queryPool, realData->queryIndex);
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void SetDebugMarker(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::SetDebugMarker* realData = static_cast<const RERHI::Command::SetDebugMarker*>(data);
					static_cast<RERHINull::RHIDynamicRHI&>(rhi).setDebugMarker(realData->name);
				}

				void BeginDebugEvent(const void* data, RERHI::RHIDynamicRHI& rhi)
				{
					const RERHI::Command::BeginDebugEvent* realData = static_cast<const RERHI::Command::BeginDebugEvent*>(data);
					static_cast<RERHINull::RHIDynamicRHI&>(rhi).beginDebugEvent(realData->name);
				}

				void EndDebugEvent(const void*, RERHI::RHIDynamicRHI& rhi)
				{
					static_cast<RERHINull::RHIDynamicRHI&>(rhi).endDebugEvent();
				}
#else
void SetDebugMarker(const void*, RERHI::RHIDynamicRHI&)
{}

void BeginDebugEvent(const void*, RERHI::RHIDynamicRHI&)
{}

void EndDebugEvent(const void*, RERHI::RHIDynamicRHI&)
{}
#endif


}


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
static constexpr RERHI::ImplementationDispatchFunction DISPATCH_FUNCTIONS[static_cast<uint8_t>(RERHI::CommandDispatchFunctionIndex::NUMBER_OF_FUNCTIONS)] =
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
namespace RERHINull {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
RHIDynamicRHI::RHIDynamicRHI(const RERHI::RHIContext& context) :
  RERHI::RHIDynamicRHI(RERHI::NameId::NULL_DUMMY, context),
  VertexArrayMakeId(),
  GraphicsPipelineStateMakeId(),
  ComputePipelineStateMakeId(),
  mShaderLanguage(nullptr),
  mRenderTarget(nullptr),
  mGraphicsRootSignature(nullptr),
  mComputeRootSignature(nullptr)
{
  // Initialize the capabilities
  initializeCapabilities();
}

RHIDynamicRHI::~RHIDynamicRHI()
{
  // Release instances
  if (nullptr != mRenderTarget)
  {
    mRenderTarget->Release();
    mRenderTarget = nullptr;
  }
  if (nullptr != mGraphicsRootSignature)
  {
    mGraphicsRootSignature->Release();
    mGraphicsRootSignature = nullptr;
  }
  if (nullptr != mComputeRootSignature)
  {
    mComputeRootSignature->Release();
    mComputeRootSignature = nullptr;
  }

#ifdef RHI_STATISTICS
  { // For debugging: At this point there should be no resource instances left, validate this!
			// -> Are the currently any resource instances?
			const uint32_t numberOfCurrentResources = getStatistics().getNumberOfCurrentResources();
			if (numberOfCurrentResources > 0)
			{
				// Error!
				if (numberOfCurrentResources > 1)
				{
					RHI_ASSERT(false, "The null RHI implementation is going to be destroyed, but there are still %u resource instances left (memory leak)", numberOfCurrentResources)
				}
				else
				{
					RHI_ASSERT(false, "The null RHI implementation is going to be destroyed, but there is still one resource instance left (memory leak)")
				}

				// Use debug output to show the current number of resource instances
				getStatistics().debugOutputCurrentResouces(mContext);
			}
		}
#endif

  // Release the null shader language instance, in case we have one
  if (nullptr != mShaderLanguage)
  {
    mShaderLanguage->Release();
  }
}

void RHIDynamicRHI::dispatchCommandBufferInternal(const RERHI::RHICommandBuffer& commandBuffer)
{
  // Loop through all commands
  const uint8_t* commandPacketBuffer = commandBuffer.getCommandPacketBuffer();
  RERHI::ConstCommandPacket constCommandPacket = commandPacketBuffer;
  while (nullptr != constCommandPacket)
  {
    { // Dispatch command packet
      const RERHI::CommandDispatchFunctionIndex commandDispatchFunctionIndex = RERHI::CommandPacketHelper::loadCommandDispatchFunctionIndex(constCommandPacket);
      const void* command = RERHI::CommandPacketHelper::loadCommand(constCommandPacket);
      detail::DISPATCH_FUNCTIONS[static_cast<uint32_t>(commandDispatchFunctionIndex)](command, *this);
    }

    { // Next command
      const uint32_t nextCommandPacketByteIndex = RERHI::CommandPacketHelper::getNextCommandPacketByteIndex(constCommandPacket);
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
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::setGraphicsResourceGroup([[maybe_unused]] uint32_t rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  // Security checks
#ifdef DEBUG
  {
			RHI_ASSERT(nullptr != mGraphicsRootSignature, "No null RHI implementation graphics root signature set")
			const RERHI::RootSignature& rootSignature = mGraphicsRootSignature->getRootSignature();
			RHI_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The null RHI implementation root parameter index is out of bounds")
			const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			RHI_ASSERT(RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The null RHI implementation root parameter index doesn't reference a descriptor table")
			RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The null RHI implementation descriptor ranges is a null pointer")
		}
#endif

  if (nullptr != resourceGroup)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *resourceGroup)

    // TODO(naetherm) Some additional resource type root signature security checks in debug build?
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::setGraphicsVertexArray(RERHI::RHIVertexArray* vertexArray)
{
  // Input-assembler (IA) stage

  // Nothing here, the following is just for debugging
  if (nullptr != vertexArray)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *vertexArray)
  }
}

void RHIDynamicRHI::setGraphicsViewports([[maybe_unused]] uint32_t numberOfViewports, [[maybe_unused]] const RERHI::Viewport* viewports)
{
  // Rasterizer (RS) stage

  // Sanity check
  RHI_ASSERT(numberOfViewports > 0 && nullptr != viewports, "Invalid null rasterizer state viewports")
}

void RHIDynamicRHI::setGraphicsScissorRectangles([[maybe_unused]] uint32_t numberOfScissorRectangles, [[maybe_unused]] const RERHI::ScissorRectangle* scissorRectangles)
{
  // Rasterizer (RS) stage

  // Sanity check
  RHI_ASSERT(numberOfScissorRectangles > 0 && nullptr != scissorRectangles, "Invalid null rasterizer state scissor rectangles")
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
        mRenderTarget->Release();
      }

      // Set new render target and add a reference to it
      mRenderTarget = renderTarget;
      mRenderTarget->AddReference();

      // That's all folks!
    }
    else
    {
      // Release the render target reference, in case we have one
      if (nullptr != mRenderTarget)
      {
        mRenderTarget->Release();
        mRenderTarget = nullptr;
      }

      // That's all folks!
    }
  }
}

void RHIDynamicRHI::clearGraphics(uint32_t, const float [4], [[maybe_unused]] float z, uint32_t)
{
  // Sanity check
  RHI_ASSERT(z >= 0.0f && z <= 1.0f, "The null clear graphics z value must be between [0, 1] (inclusive)")
}

void RHIDynamicRHI::drawGraphicsEmulated([[maybe_unused]] const uint8_t* emulationData, uint32_t, [[maybe_unused]] uint32_t numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The null emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")
}

void RHIDynamicRHI::drawIndexedGraphicsEmulated([[maybe_unused]] const uint8_t* emulationData, uint32_t, [[maybe_unused]] uint32_t numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The null emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")
}

void RHIDynamicRHI::drawMeshTasksEmulated([[maybe_unused]] const uint8_t* emulationData, uint32_t, [[maybe_unused]] uint32_t numberOfDraws)
{
  // Sanity checks
  RHI_ASSERT(nullptr != emulationData, "The null emulation data must be valid")
  RHI_ASSERT(numberOfDraws > 0, "The number of null draws must not be zero")
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
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::setComputeResourceGroup([[maybe_unused]] uint32_t rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup)
{
  // Security checks
#ifdef DEBUG
  {
			RHI_ASSERT(nullptr != mComputeRootSignature, "No null RHI implementation compute root signature set")
			const RERHI::RootSignature& rootSignature = mComputeRootSignature->getRootSignature();
			RHI_ASSERT(rootParameterIndex < rootSignature.numberOfParameters, "The null RHI implementation root parameter index is out of bounds")
			const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			RHI_ASSERT(RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType, "The null RHI implementation root parameter index doesn't reference a descriptor table")
			RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "The null RHI implementation descriptor ranges is a null pointer")
		}
#endif

  if (nullptr != resourceGroup)
  {
    // Sanity check
    RHI_MATCH_CHECK(*this, *resourceGroup)

    // TODO(naetherm) Some additional resource type root signature security checks in debug build?
  }
  else
  {
    // TODO(naetherm) Handle this situation?
  }
}

void RHIDynamicRHI::dispatchCompute(uint32_t, uint32_t, uint32_t)
{}


//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resolveMultisampleFramebuffer(RERHI::RHIRenderTarget&, RERHI::RHIFramebuffer&)
{}

void RHIDynamicRHI::copyResource(RERHI::RHIResource&, RERHI::RHIResource&)
{}

void RHIDynamicRHI::generateMipmaps(RERHI::RHIResource&)
{}


//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
void RHIDynamicRHI::resetQueryPool(RERHI::RHIQueryPool&, uint32_t, uint32_t)
{}

void RHIDynamicRHI::beginQuery(RERHI::RHIQueryPool&, uint32_t, uint32_t)
{}

void RHIDynamicRHI::endQuery(RERHI::RHIQueryPool&, uint32_t)
{}

void RHIDynamicRHI::writeTimestampQuery(RERHI::RHIQueryPool&, uint32_t)
{}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void RHIDynamicRHI::setDebugMarker(const char*)
		{}

		void RHIDynamicRHI::beginDebugEvent(const char*)
		{}

		void RHIDynamicRHI::endDebugEvent()
		{}
#endif


//[-------------------------------------------------------]
//[ Public virtual RERHI::RHIDynamicRHI methods                      ]
//[-------------------------------------------------------]
const char* RHIDynamicRHI::getName() const
{
  return "Null";
}

bool RHIDynamicRHI::isInitialized() const
{
  return true;
}

bool RHIDynamicRHI::isDebugEnabled()
{
  // Nothing here

  // Debug disabled
  return false;
}


//[-------------------------------------------------------]
//[ Shader language                                       ]
//[-------------------------------------------------------]
uint32_t RHIDynamicRHI::getNumberOfShaderLanguages() const
{
  // Only one shader language supported in here
  return 1;
}

const char* RHIDynamicRHI::getShaderLanguageName([[maybe_unused]] uint32_t index) const
{
  RHI_ASSERT(index < getNumberOfShaderLanguages(), "Null: Shader language index is out-of-bounds")
  return ::detail::NULL_NAME;
}

RERHI::RHIShaderLanguage* RHIDynamicRHI::getShaderLanguage(const char* shaderLanguageName)
{
  // In case "shaderLanguage" is a null pointer, use the default shader language
  if (nullptr != shaderLanguageName)
  {
    // In case "shaderLanguage" is a null pointer, use the default shader language
    // -> Only one shader language supported in here
    if (nullptr == shaderLanguageName || !stricmp(shaderLanguageName, ::detail::NULL_NAME))
    {
      // If required, create the null shader language instance right now
      if (nullptr == mShaderLanguage)
      {
        mShaderLanguage = RHI_NEW(mContext, ShaderLanguage)(*this);
        mShaderLanguage->AddReference();	// Internal RHI reference
      }

      // Return the shader language instance
      return mShaderLanguage;
    }

    // Error!
    return nullptr;
  }

  // Return the null shader language instance as default
  return getShaderLanguage(::detail::NULL_NAME);
}


//[-------------------------------------------------------]
//[ Resource creation                                     ]
//[-------------------------------------------------------]
RERHI::RHIRenderPass* RHIDynamicRHI::createRenderPass(uint32_t numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  return RHI_NEW(mContext, RenderPass)(*this, numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIQueryPool* RHIDynamicRHI::createQueryPool([[maybe_unused]] RERHI::QueryType queryType, [[maybe_unused]] uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER_NO_DEFAULT)
{
  // TODO(naetherm) Implement me
  return nullptr;
}

RERHI::RHISwapChain* RHIDynamicRHI::createSwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle, bool RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity checks
  RHI_MATCH_CHECK(*this, renderPass)
  RHI_ASSERT(NULL_HANDLE != windowHandle.nativeWindowHandle, "Null: The provided native window handle must not be a null handle")

  // Create the swap chain
  return RHI_NEW(mContext, SwapChain)(renderPass, windowHandle RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIFramebuffer* RHIDynamicRHI::createFramebuffer(RERHI::RHIRenderPass& renderPass, const RERHI::FramebufferAttachment* colorFramebufferAttachments, const RERHI::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Sanity check
  RHI_MATCH_CHECK(*this, renderPass)

  // We don't keep a reference to the provided textures in here
  // -> Ensure a correct reference counter behaviour

  // Are there any color textures?
  const uint32_t numberOfColorAttachments = static_cast<RenderPass&>(renderPass).getNumberOfColorAttachments();
  if (numberOfColorAttachments > 0)
  {
    // Loop through all color textures
    const RERHI::FramebufferAttachment* colorFramebufferAttachmentsEnd = colorFramebufferAttachments + numberOfColorAttachments;
    for (const RERHI::FramebufferAttachment* colorFramebufferAttachment = colorFramebufferAttachments; colorFramebufferAttachment < colorFramebufferAttachmentsEnd; ++colorFramebufferAttachment)
    {
      // Valid entry?
      if (nullptr != colorFramebufferAttachment->texture)
      {
        // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
        colorFramebufferAttachment->texture->AddReference();
        colorFramebufferAttachment->texture->Release();
      }
    }
  }

  // Add a reference to the used depth stencil texture
  if (nullptr != depthStencilFramebufferAttachment)
  {
    depthStencilFramebufferAttachment->texture->AddReference();
    depthStencilFramebufferAttachment->texture->Release();
  }

  // Create the framebuffer instance
  return RHI_NEW(mContext, Framebuffer)(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER);
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
  RHI_ASSERT(nullptr != graphicsPipelineState.rootSignature, "Null: Invalid graphics pipeline state root signature")
  RHI_ASSERT(nullptr != graphicsPipelineState.graphicsProgram, "Null: Invalid graphics pipeline state graphics program")
  RHI_ASSERT(nullptr != graphicsPipelineState.renderPass, "Null: Invalid graphics pipeline state render pass")

  // Create graphics pipeline state
  uint16_t id = 0;
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
  uint16_t id = 0;
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

RERHI::RHISamplerState* RHIDynamicRHI::createSamplerState(const RERHI::SamplerState& RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  return RHI_NEW(mContext, SamplerState)(*this RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Resource handling                                     ]
//[-------------------------------------------------------]
bool RHIDynamicRHI::map(RERHI::RHIResource&, uint32_t, RERHI::MapType, uint32_t, RERHI::MappedSubresource&)
{
  // Not supported by the null RHI
  return false;
}

void RHIDynamicRHI::unmap(RERHI::RHIResource&, uint32_t)
{}

bool RHIDynamicRHI::getQueryPoolResults(RERHI::RHIQueryPool&, uint32_t, uint8_t*, uint32_t, uint32_t, uint32_t, uint32_t)
{
  return true;
}


//[-------------------------------------------------------]
//[ Operation                                             ]
//[-------------------------------------------------------]
void RHIDynamicRHI::dispatchCommandBuffer(const RERHI::RHICommandBuffer& commandBuffer)
{
  // Sanity check
  RHI_ASSERT(!commandBuffer.isEmpty(), "The null command buffer to dispatch mustn't be empty")

  // Dispatch command buffer
  dispatchCommandBufferInternal(commandBuffer);
}


//[-------------------------------------------------------]
//[ Protected virtual RERHI::RefCount methods               ]
//[-------------------------------------------------------]
void RHIDynamicRHI::selfDestruct()
{
  RHI_DELETE(mContext, RHIDynamicRHI, this);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void RHIDynamicRHI::initializeCapabilities()
{
  strcpy(mCapabilities.deviceName, "Null");

  // Preferred swap chain texture format
  mCapabilities.preferredSwapChainColorTextureFormat		  = RERHI::TextureFormat::Enum::R8G8B8A8;
  mCapabilities.preferredSwapChainDepthStencilTextureFormat = RERHI::TextureFormat::Enum::D32_FLOAT;

  // Maximum number of viewports (always at least 1)
  mCapabilities.maximumNumberOfViewports = 1;

  // Maximum number of simultaneous render targets (if <1 render to texture is not supported)
  mCapabilities.maximumNumberOfSimultaneousRenderTargets = 8;

  // Maximum texture dimension
  mCapabilities.maximumTextureDimension = 42;

  // Maximum number of 1D texture array slices (usually 512, in case there's no support for 1D texture arrays it's 0)
  mCapabilities.maximumNumberOf1DTextureArraySlices = 42;

  // Maximum number of 2D texture array slices (usually 512, in case there's no support for 2D texture arrays it's 0)
  mCapabilities.maximumNumberOf2DTextureArraySlices = 42;

  // Maximum number of cube texture array slices (usually 512, in case there's no support for cube texture arrays it's 0)
  mCapabilities.maximumNumberOfCubeTextureArraySlices = 42;

  // Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
  mCapabilities.maximumTextureBufferSize = mCapabilities.maximumStructuredBufferSize = 42;

  // Maximum indirect buffer size in bytes
  mCapabilities.maximumIndirectBufferSize = 128 * 1024;	// 128 KiB

  // Maximum uniform buffer (UBO) size in bytes (usually at least 4096 * 16 bytes, in case there's no support for uniform buffer it's 0)
  // -> Let's use the DirectX 11 value: See https://msdn.microsoft.com/en-us/library/windows/desktop/ff819065(v=vs.85).aspx - "Resource Limits (Direct3D 11)" - "Number of elements in a constant buffer D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096)"
  // -> One element = float4 = 16 bytes
  mCapabilities.maximumUniformBufferSize = 4096 * 16;

  // Maximum number of multisamples (always at least 1, usually 8)
  mCapabilities.maximumNumberOfMultisamples = 1;

  // Maximum anisotropy (always at least 1, usually 16)
  mCapabilities.maximumAnisotropy = 16;

  // Left-handed coordinate system with clip space depth value range 0..1
  mCapabilities.upperLeftOrigin = mCapabilities.zeroToOneClipZ = true;

  // Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
  mCapabilities.individualUniforms = true;

  // Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
  mCapabilities.instancedArrays = true;

  // Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
  mCapabilities.drawInstanced = true;

  // Base vertex supported for draw calls?
  mCapabilities.baseVertex = true;

  // The null RHI has native multithreading
  mCapabilities.nativeMultithreading = true;

  // The null RHI has no shader bytecode support
  mCapabilities.shaderBytecode = false;

  // Is there support for vertex shaders (VS)?
  mCapabilities.vertexShader = true;

  // Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
  mCapabilities.maximumNumberOfPatchVertices = 32;

  // Maximum number of vertices a geometry shader can emit (usually 0 for no geometry shader support or 1024)
  mCapabilities.maximumNumberOfGsOutputVertices = 1024;

  // Is there support for fragment shaders (FS)?
  mCapabilities.fragmentShader = true;

  // Is there support for task shaders (TS) and mesh shaders (MS)?
  mCapabilities.meshShader = true;

  // Is there support for compute shaders (CS)?
  mCapabilities.computeShader = true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull

RERHINULL_API RERHI::RHIDynamicRHI* createNullRhiInstance(const RERHI::RHIContext& context)
{
	return RHI_NEW(context, RERHINull::RHIDynamicRHI)(context);
}
RERHI::RHIDynamicRHI* createRhiInstance(const RERHI::RHIContext& context)
{
  return RHI_NEW(context, RERHINull::RHIDynamicRHI)(context);
}