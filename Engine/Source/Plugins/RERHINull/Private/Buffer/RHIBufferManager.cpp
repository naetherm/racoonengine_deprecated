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
#include "RERHINull/Buffer/RHIBufferManager.h"
#include "RERHINull/Buffer/RHIIndexBuffer.h"
#include "RERHINull/Buffer/RHIIndirectBuffer.h"
#include "RERHINull/Buffer/RHIStructuredBuffer.h"
#include "RERHINull/Buffer/RHITextureBuffer.h"
#include "RERHINull/Buffer/RHIUniformBuffer.h"
#include "RERHINull/Buffer/RHIVertexArray.h"
#include "RERHINull/Buffer/RHIVertexBuffer.h"
#include "RERHINull/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

/**
*  @brief
*    Constructor
*
*  @param[in] nullRhi
*    Owner null RHI instance
*/
BufferManager::BufferManager(RHIDynamicRHI& nullRhi) :
RHIBufferManager(nullRhi)
  {}

/**
*  @brief
*    Destructor
*/
BufferManager::~BufferManager()
{}


RERHI::RHIVertexBuffer* BufferManager::createVertexBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), VertexBuffer)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIIndexBuffer* BufferManager::createIndexBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage, [[maybe_unused]] RERHI::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), IndexBuffer)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIVertexArray* BufferManager::createVertexArray([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, RERHI::RHIIndexBuffer* indexBuffer RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
#ifdef DEBUG
  {
				const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
				for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
				{
					RHI_ASSERT(&nullRhi == &vertexBuffer->vertexBuffer->getRhi(), "Null error: The given vertex buffer resource is owned by another RHI instance")
				}
			}
#endif
  RHI_ASSERT(nullptr == indexBuffer || &nullRhi == &indexBuffer->getRhi(), "Null error: The given index buffer resource is owned by another RHI instance")

  // We don't keep a reference to the vertex buffers used by the vertex array attributes in here
  // -> Ensure a correct reference counter behaviour
  const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
  for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
  {
    vertexBuffer->vertexBuffer->AddReference();
    vertexBuffer->vertexBuffer->Release();
  }

  // We don't keep a reference to the index buffer in here
  // -> Ensure a correct reference counter behaviour
  if (nullptr != indexBuffer)
  {
    indexBuffer->AddReference();
    indexBuffer->Release();
  }

  // Create the vertex array instance
  RECore::uint16 id;
  return nullRhi.VertexArrayMakeId.createID(id) ? RHI_NEW(nullRhi.getContext(), VertexArray)(nullRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER) : nullptr;
}

RERHI::RHITextureBuffer* BufferManager::createTextureBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT((numberOfBytes % RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat)) == 0, "The null texture buffer size must be a multiple of the selected texture format bytes per texel")

  // Create the texture buffer
  return RHI_NEW(nullRhi.getContext(), TextureBuffer)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIStructuredBuffer* BufferManager::createStructuredBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage, [[maybe_unused]] RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  RHI_ASSERT((numberOfBytes % numberOfStructureBytes) == 0, "The null structured buffer size must be a multiple of the given number of structure bytes")
  RHI_ASSERT((numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The null structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

  // Create the structured buffer
  return RHI_NEW(nullRhi.getContext(), StructuredBuffer)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIIndirectBuffer* BufferManager::createIndirectBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 indirectBufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  RHI_ASSERT((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid null flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
  RHI_ASSERT(!((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid null flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
  RHI_ASSERT((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (numberOfBytes % sizeof(RERHI::DrawArguments)) == 0, "Null indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
  RHI_ASSERT((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (numberOfBytes % sizeof(RERHI::DrawIndexedArguments)) == 0, "Null indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

  // Create indirect buffer
  return RHI_NEW(nullRhi.getContext(), IndirectBuffer)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIUniformBuffer* BufferManager::createUniformBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
  // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid null buffer flags, uniform buffer can't be used for unordered access")
  // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::SHADER_RESOURCE) != 0, "Invalid null buffer flags, uniform buffer must be used as shader resource")

  // Create the uniform buffer
  return RHI_NEW(nullRhi.getContext(), UniformBuffer)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


void BufferManager::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), BufferManager, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
