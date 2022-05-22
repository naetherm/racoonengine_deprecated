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
#include "RERHIOpenGLES3/Buffer/RHIBufferManager.h"
#include "RERHIOpenGLES3/Buffer/RHIIndexBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHIIndirectBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHITextureBufferBind.h"
#include "RERHIOpenGLES3/Buffer/RHITextureBufferBindEmulation.h"
#include "RERHIOpenGLES3/Buffer/RHIUniformBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHIVertexArray.h"
#include "RERHIOpenGLES3/Buffer/RHIVertexBuffer.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Extensions.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

BufferManager::BufferManager(RHIDynamicRHI& openGLES3Rhi) :
RHIBufferManager(openGLES3Rhi),
mExtensions(&openGLES3Rhi.getOpenGLES3Context().getExtensions())
{}


RERHI::RHIVertexBuffer* BufferManager::createVertexBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(openGLES3Rhi.getContext(), VertexBuffer)(openGLES3Rhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIIndexBuffer* BufferManager::createIndexBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage, RERHI::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(openGLES3Rhi.getContext(), IndexBuffer)(openGLES3Rhi, numberOfBytes, data, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIVertexArray* BufferManager::createVertexArray(const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, RERHI::RHIIndexBuffer* indexBuffer RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
#ifdef DEBUG
  {
				const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
				for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
				{
					RHI_ASSERT(&openGLES3Rhi == &vertexBuffer->vertexBuffer->getRhi(), "OpenGL ES 3 error: The given vertex buffer resource is owned by another RHI instance")
				}
			}
#endif
  RHI_ASSERT(nullptr == indexBuffer || &openGLES3Rhi == &indexBuffer->getRhi(), "OpenGL ES 3 error: The given index buffer resource is owned by another RHI instance")

  // Create vertex array
  RECore::uint16 id;
  if (openGLES3Rhi.VertexArrayMakeId.createID(id))
  {
    return RHI_NEW(openGLES3Rhi.getContext(), VertexArray)(openGLES3Rhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
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

RERHI::RHITextureBuffer* BufferManager::createTextureBuffer(RECore::uint32 numberOfBytes, const void* data, RECore::uint32, RERHI::BufferUsage bufferUsage, RERHI::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT((numberOfBytes % RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat)) == 0, "The OpenGL ES 3 texture buffer size must be a multiple of the selected texture format bytes per texel")

  // Is "GL_EXT_texture_buffer" there?
  if (mExtensions->isGL_EXT_texture_buffer())
  {
    // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
    return RHI_NEW(openGLES3Rhi.getContext(), TextureBufferBind)(openGLES3Rhi, numberOfBytes, data, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

    // We can only emulate the "RERHI::TextureFormat::R32G32B32A32F" texture format using an uniform buffer
  else if (RERHI::TextureFormat::R32G32B32A32F == textureFormat)
  {
    // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
    return RHI_NEW(openGLES3Rhi.getContext(), TextureBufferBindEmulation)(openGLES3Rhi, numberOfBytes, data, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error!
  return nullptr;
}

RERHI::RHIStructuredBuffer* BufferManager::createStructuredBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage, [[maybe_unused]] RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Sanity checks
  RHI_ASSERT((numberOfBytes % numberOfStructureBytes) == 0, "The OpenGL ES 3 structured buffer size must be a multiple of the given number of structure bytes")
  RHI_ASSERT((numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The OpenGL ES 3 structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

  // TODO(naetherm) Add OpenGL ES structured buffer support ("GL_EXT_buffer_storage"-extension)
  return nullptr;
}

RERHI::RHIIndirectBuffer* BufferManager::createIndirectBuffer(RECore::uint32 numberOfBytes, const void* data, RECore::uint32 indirectBufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(openGLES3Rhi.getContext(), IndirectBuffer)(openGLES3Rhi, numberOfBytes, data, indirectBufferFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIUniformBuffer* BufferManager::createUniformBuffer(RECore::uint32 numberOfBytes, const void* data, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
  // -> Inside GLSL "layout(binding, std140) writeonly uniform OutputUniformBuffer" will result in the GLSL compiler error "Failed to parse the GLSL shader source code: ERROR: 0:85: 'assign' :  l-value required "anon@6" (can't modify a uniform)"
  // -> Inside GLSL "layout(binding, std430) writeonly buffer  OutputUniformBuffer" will work in OpenGL but will fail in Vulkan with "Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT" Object: "0" Location: "0" Message code: "13" Layer prefix: "Validation" Message: "Object: VK_NULL_HANDLE (Type) | Type mismatch on descriptor slot 0.0 (used as type `ptr to uniform struct of (vec4 of float32)`) but descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER""
  // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid OpenGL ES 3 buffer flags, uniform buffer can't be used for unordered access")
  // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::SHADER_RESOURCE) != 0, "Invalid OpenGL ES 3 buffer flags, uniform buffer must be used as shader resource")

  // Create the uniform buffer
  return RHI_NEW(openGLES3Rhi.getContext(), UniformBuffer)(openGLES3Rhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
