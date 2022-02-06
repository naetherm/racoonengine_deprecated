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
#include "RERHIVulkan/Buffer/RHIVertexArray.h"
#include "RERHIVulkan/Buffer/RHIIndexBuffer.h"
#include "RERHIVulkan/Buffer/RHIVertexBuffer.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

VertexArray::VertexArray(RHIDynamicRHI &vulkanRhi, const RERHI::VertexAttributes &vertexAttributes,
                         uint32_t numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer *vertexBuffers,
                         IndexBuffer *indexBuffer, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIVertexArray(static_cast<RERHI::RHIDynamicRHI &>(vulkanRhi), id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mIndexBuffer(indexBuffer),
  mNumberOfSlots(numberOfVertexBuffers),
  mVertexVkBuffers(nullptr),
  mStrides(nullptr),
  mOffsets(nullptr),
  mVertexBuffers(nullptr) {
  // Add a reference to the given index buffer
  if (nullptr != mIndexBuffer) {
    mIndexBuffer->AddReference();
  }

  // Add a reference to the used vertex buffers
  if (mNumberOfSlots > 0) {
    const RERHI::RHIContext &context = vulkanRhi.getContext();
    mVertexVkBuffers = RHI_MALLOC_TYPED(context, VkBuffer, mNumberOfSlots);
    mStrides = RHI_MALLOC_TYPED(context, uint32_t, mNumberOfSlots);
    mOffsets = RHI_MALLOC_TYPED(context, VkDeviceSize, mNumberOfSlots);
    memset(mOffsets, 0, sizeof(VkDeviceSize) *
                        mNumberOfSlots);  // Vertex buffer offset is not supported by OpenGL, so our RHI implementation doesn't support it either, set everything to zero
    mVertexBuffers = RHI_MALLOC_TYPED(context, VertexBuffer*, mNumberOfSlots);

    { // Loop through all vertex buffers
      VkBuffer *currentVertexVkBuffer = mVertexVkBuffers;
      VertexBuffer **currentVertexBuffer = mVertexBuffers;
      const RERHI::VertexArrayVertexBuffer *vertexBufferEnd = vertexBuffers + mNumberOfSlots;
      for (const RERHI::VertexArrayVertexBuffer *vertexBuffer = vertexBuffers;
           vertexBuffer < vertexBufferEnd; ++vertexBuffer, ++currentVertexVkBuffer, ++currentVertexBuffer) {
        // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
        *currentVertexBuffer = static_cast<VertexBuffer *>(vertexBuffer->vertexBuffer);
        *currentVertexVkBuffer = (*currentVertexBuffer)->getVkBuffer();
        (*currentVertexBuffer)->AddReference();
      }
    }

    { // Gather slot related data
      const RERHI::VertexAttribute *attribute = vertexAttributes.attributes;
      const RERHI::VertexAttribute *attributesEnd = attribute + vertexAttributes.numberOfAttributes;
      for (; attribute < attributesEnd; ++attribute) {
        mStrides[attribute->inputSlot] = attribute->strideInBytes;
      }
    }
  }
}

VertexArray::~VertexArray() {
// Release the index buffer reference
  if (nullptr != mIndexBuffer) {
    mIndexBuffer->Release();
  }

// Cleanup Vulkan input slot data
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  const RERHI::RHIContext &context = vulkanRhi.getContext();
  if (mNumberOfSlots > 0) {
    RHI_FREE(context, mVertexVkBuffers);
    RHI_FREE(context, mStrides);
    RHI_FREE(context, mOffsets);
  }

// Release the reference to the used vertex buffers
  if (nullptr != mVertexBuffers) {
// Release references
    VertexBuffer **vertexBuffersEnd = mVertexBuffers + mNumberOfSlots;
    for (VertexBuffer **vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer) {
      (*vertexBuffer)->Release();
    }

// Cleanup
    RHI_FREE(context, mVertexBuffers);
  }

// Free the unique compact vertex array ID
  vulkanRhi.VertexArrayMakeId.destroyID(getId());
}

void VertexArray::bindVulkanBuffers(VkCommandBuffer vkCommandBuffer) const {
  // Set the Vulkan vertex buffers
  if (nullptr != mVertexVkBuffers) {
    vkCmdBindVertexBuffers(vkCommandBuffer, 0, mNumberOfSlots, mVertexVkBuffers, mOffsets);
  } else {
    // Do nothing since the Vulkan specification says "bindingCount must be greater than 0"
    // vkCmdBindVertexBuffers(vkCommandBuffer, 0, 0, nullptr, nullptr);
  }

  // Set the used index buffer
  // -> In case of no index buffer we don't set null indices, there's not really a point in it
  if (nullptr != mIndexBuffer) {
    vkCmdBindIndexBuffer(vkCommandBuffer, mIndexBuffer->getVkBuffer(), 0, mIndexBuffer->getVkIndexType());
  }
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
