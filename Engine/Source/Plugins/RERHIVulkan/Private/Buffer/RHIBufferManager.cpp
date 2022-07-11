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
#include "RERHIVulkan/Buffer/RHIBufferManager.h"
#include "RERHIVulkan/Buffer/RHIIndexBuffer.h"
#include "RERHIVulkan/Buffer/RHIIndirectBuffer.h"
#include "RERHIVulkan/Buffer/RHIStructuredBuffer.h"
#include "RERHIVulkan/Buffer/RHITextureBuffer.h"
#include "RERHIVulkan/Buffer/RHIUniformBuffer.h"
#include "RERHIVulkan/Buffer/RHIVertexArray.h"
#include "RERHIVulkan/Buffer/RHIVertexBuffer.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

BufferManager::BufferManager(RHIDynamicRHI& vulkanRhi) :
RHIBufferManager(vulkanRhi)
  {}

/**
*  @brief
*    Destructor
*/
BufferManager::~BufferManager()
{}


RERHI::RHIVertexBuffer* BufferManager::createVertexBuffer(RECore::uint32 numberOfBytes, const void* data, RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), VertexBuffer)(vulkanRhi, numberOfBytes, data, bufferFlags, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIIndexBuffer* BufferManager::createIndexBuffer(RECore::uint32 numberOfBytes, const void* data, RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage, RERHI::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), IndexBuffer)(vulkanRhi, numberOfBytes, data, bufferFlags, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIVertexArray* BufferManager::createVertexArray(const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, RERHI::RHIIndexBuffer* indexBuffer RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
#ifdef DEBUG
  {
				const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
				for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
				{
					RHI_ASSERT(&vulkanRhi == &vertexBuffer->vertexBuffer->getRhi(), "Vulkan error: The given vertex buffer resource is owned by another RHI instance")
				}
			}
#endif
  RHI_ASSERT(nullptr == indexBuffer || &vulkanRhi == &indexBuffer->getRhi(), "Vulkan error: The given index buffer resource is owned by another RHI instance")

  // Create vertex array
  RECore::uint16 id;
  if (vulkanRhi.VertexArrayMakeId.createID(id))
  {
    return RHI_NEW(vulkanRhi.getContext(), VertexArray)(vulkanRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error: Ensure a correct reference counter behaviour
  const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
  for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
  {
    vertexBuffer->vertexBuffer->AddReference();
    vertexBuffer->vertexBuffer->Release();
  }
  if (nullptr != indexBuffer)
  {
    indexBuffer->AddReference();
    indexBuffer->Release();
  }
  return nullptr;
}

RERHI::RHITextureBuffer* BufferManager::createTextureBuffer(RECore::uint32 numberOfBytes, const void* data, RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage, RERHI::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), TextureBuffer)(vulkanRhi, numberOfBytes, data, bufferFlags, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIStructuredBuffer* BufferManager::createStructuredBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage, RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), StructuredBuffer)(vulkanRhi, numberOfBytes, data, bufferUsage, numberOfStructureBytes RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIIndirectBuffer* BufferManager::createIndirectBuffer(RECore::uint32 numberOfBytes, const void* data, RECore::uint32 indirectBufferFlags, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), IndirectBuffer)(vulkanRhi, numberOfBytes, data, indirectBufferFlags, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIUniformBuffer* BufferManager::createUniformBuffer(RECore::uint32 numberOfBytes, const void* data, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
  // -> Inside GLSL "layout(binding, std140) writeonly uniform OutputUniformBuffer" will result in the GLSL compiler error "Failed to parse the GLSL shader source code: ERROR: 0:85: 'assign' :  l-value required "anon@6" (can't modify a uniform)"
  // -> Inside GLSL "layout(binding, std430) writeonly buffer  OutputUniformBuffer" will work in OpenGL but will fail in Vulkan with "Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT" Object: "0" Location: "0" Message code: "13" Layer prefix: "Validation" Message: "Object: VK_NULL_HANDLE (Type) | Type mismatch on descriptor slot 0.0 (used as type `ptr to uniform struct of (vec4 of float32)`) but descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER""
  // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid Vulkan buffer flags, uniform buffer can't be used for unordered access")
  // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::SHADER_RESOURCE) != 0, "Invalid Vulkan buffer flags, uniform buffer must be used as shader resource")

  // Create the uniform buffer
  return RHI_NEW(vulkanRhi.getContext(), UniformBuffer)(vulkanRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
