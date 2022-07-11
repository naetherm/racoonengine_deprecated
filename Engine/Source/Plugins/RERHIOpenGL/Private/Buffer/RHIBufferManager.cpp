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
#include "RERHIOpenGL/Buffer/RHIBufferManager.h"
#include "RERHIOpenGL/Buffer/RHIIndexBufferBind.h"
#include "RERHIOpenGL/Buffer/RHIIndexBufferDsa.h"
#include "RERHIOpenGL/Buffer/RHIIndirectBufferBind.h"
#include "RERHIOpenGL/Buffer/RHIIndirectBufferDsa.h"
#include "RERHIOpenGL/Buffer/RHIStructuredBufferBind.h"
#include "RERHIOpenGL/Buffer/RHIStructuredBufferDsa.h"
#include "RERHIOpenGL/Buffer/RHITextureBufferBind.h"
#include "RERHIOpenGL/Buffer/RHITextureBufferDsa.h"
#include "RERHIOpenGL/Buffer/RHIUniformBufferBind.h"
#include "RERHIOpenGL/Buffer/RHIUniformBufferDsa.h"
#include "RERHIOpenGL/Buffer/RHIVertexArrayNoVao.h"
#include "RERHIOpenGL/Buffer/RHIVertexArrayVaoBind.h"
#include "RERHIOpenGL/Buffer/RHIVertexArrayVaoDsa.h"
#include "RERHIOpenGL/Buffer/RHIVertexBufferBind.h"
#include "RERHIOpenGL/Buffer/RHIVertexBufferDsa.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
BufferManager::BufferManager(RHIDynamicRHI& openGLRhi) :
RHIBufferManager(openGLRhi),
mExtensions(&openGLRhi.getExtensions())
{}

RERHI::RHIVertexBuffer* BufferManager::createVertexBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // "GL_ARB_vertex_buffer_object" required
  if (mExtensions->isGL_ARB_vertex_buffer_object())
  {
    // Is "GL_EXT_direct_state_access" there?
    RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), VertexBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), VertexBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    // Error!
    return nullptr;
  }
}

RERHI::RHIIndexBuffer* BufferManager::createIndexBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage, RERHI::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // "GL_ARB_vertex_buffer_object" required
  if (mExtensions->isGL_ARB_vertex_buffer_object())
  {
    // Is "GL_EXT_direct_state_access" there?
    RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), IndexBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), IndexBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    // Error!
    return nullptr;
  }
}

RERHI::RHIVertexArray* BufferManager::createVertexArray(const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, RERHI::RHIIndexBuffer* indexBuffer RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
#ifdef DEBUG
  {
				const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
				for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
				{
					RHI_ASSERT(&openGLRhi == &vertexBuffer->vertexBuffer->getRhi(), "OpenGL error: The given vertex buffer resource is owned by another RHI instance")
				}
			}
#endif
  RHI_ASSERT(nullptr == indexBuffer || &openGLRhi == &indexBuffer->getRhi(), "OpenGL error: The given index buffer resource is owned by another RHI instance")

  // Create vertex array
  RECore::uint16 id;
  if (openGLRhi.VertexArrayMakeId.createID(id))
  {
    // Is "GL_ARB_vertex_array_object" there?
    if (mExtensions->isGL_ARB_vertex_array_object())
    {
      // Effective vertex array object (VAO)

      // Is "GL_EXT_direct_state_access" there?
      if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
      {
        // Effective direct state access (DSA)
        return RHI_NEW(openGLRhi.getContext(), VertexArrayVaoDsa)(openGLRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
      }
      else
      {
        // Traditional bind version
        return RHI_NEW(openGLRhi.getContext(), VertexArrayVaoBind)(openGLRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
      }
    }
    else
    {
      // Traditional version
      return RHI_NEW(openGLRhi.getContext(), VertexArrayNoVao)(openGLRhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
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

RERHI::RHITextureBuffer* BufferManager::createTextureBuffer(RECore::uint32 numberOfBytes, const void* data, RECore::uint32 , RERHI::BufferUsage bufferUsage, RERHI::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT((numberOfBytes % RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat)) == 0, "The OpenGL texture buffer size must be a multiple of the selected texture format bytes per texel")

  // "GL_ARB_texture_buffer_object" required
  if (mExtensions->isGL_ARB_texture_buffer_object())
  {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), TextureBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), TextureBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    // Error!
    return nullptr;
  }
}

RERHI::RHIStructuredBuffer* BufferManager::createStructuredBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage, [[maybe_unused]] RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  RHI_ASSERT((numberOfBytes % numberOfStructureBytes) == 0, "The OpenGL structured buffer size must be a multiple of the given number of structure bytes")
  RHI_ASSERT((numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The OpenGL structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

  // "GL_ARB_shader_storage_buffer_object" required
  if (mExtensions->isGL_ARB_shader_storage_buffer_object())
  {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), StructuredBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), StructuredBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    // Error!
    return nullptr;
  }
}

RERHI::RHIIndirectBuffer* BufferManager::createIndirectBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 indirectBufferFlags, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  RHI_ASSERT((indirectBufferFlags & RERHI::RHIndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (indirectBufferFlags & RERHI::RHIndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid OpenGL flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
  RHI_ASSERT(!((indirectBufferFlags & RERHI::RHIndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (indirectBufferFlags & RERHI::RHIndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid OpenGL flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
  RHI_ASSERT((indirectBufferFlags & RERHI::RHIndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (numberOfBytes % sizeof(RERHI::DrawArguments)) == 0, "OpenGL indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
  RHI_ASSERT((indirectBufferFlags & RERHI::RHIndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (numberOfBytes % sizeof(RERHI::DrawIndexedArguments)) == 0, "OpenGL indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

  // "GL_ARB_draw_indirect" required
  if (mExtensions->isGL_ARB_draw_indirect())
  {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), IndirectBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), IndirectBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    // Error!
    return nullptr;
  }
}

RERHI::RHIUniformBuffer* BufferManager::createUniformBuffer(RECore::uint32 numberOfBytes, const void* data, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // "GL_ARB_uniform_buffer_object" required
  if (mExtensions->isGL_ARB_uniform_buffer_object())
  {
    RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

    // Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
    // -> Inside GLSL "layout(binding, std140) writeonly uniform OutputUniformBuffer" will result in the GLSL compiler error "Failed to parse the GLSL shader source code: ERROR: 0:85: 'assign' :  l-value required "anon@6" (can't modify a uniform)"
    // -> Inside GLSL "layout(binding, std430) writeonly buffer  OutputUniformBuffer" will work in OpenGL but will fail in Vulkan with "Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT" Object: "0" Location: "0" Message code: "13" Layer prefix: "Validation" Message: "Object: VK_NULL_HANDLE (Type) | Type mismatch on descriptor slot 0.0 (used as type `ptr to uniform struct of (vec4 of float32)`) but descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER""
    // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid OpenGL buffer flags, uniform buffer can't be used for unordered access")
    // RHI_ASSERT((bufferFlags & RERHI::BufferFlag::SHADER_RESOURCE) != 0, "Invalid OpenGL buffer flags, uniform buffer must be used as shader resource")

    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), UniformBufferDsa)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), UniformBufferBind)(openGLRhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    // Error!
    return nullptr;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
