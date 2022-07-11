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
#include "RERHIOpenGLES3/Buffer/RHIIndirectBuffer.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


IndirectBuffer::IndirectBuffer(RHIDynamicRHI& openGLES3Rhi, RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 indirectBufferFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHIIndirectBuffer(openGLES3Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mNumberOfBytes(numberOfBytes),
mData(nullptr)
{
  // Sanity checks
  RHI_ASSERT((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid OpenGL ES 3 flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
  RHI_ASSERT(!((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid OpenGL ES 3 flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
  RHI_ASSERT((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (numberOfBytes % sizeof(RERHI::DrawArguments)) == 0, "OpenGL ES 3 indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
  RHI_ASSERT((indirectBufferFlags & RERHI::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (numberOfBytes % sizeof(RERHI::DrawIndexedArguments)) == 0, "OpenGL ES 3 indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

  // Copy data
  if (mNumberOfBytes > 0)
  {
    mData = RHI_MALLOC_TYPED(openGLES3Rhi.getContext(), RECore::uint8, mNumberOfBytes);
    if (nullptr != data)
    {
      memcpy(mData, data, mNumberOfBytes);
    }
  }
  else
  {
    RHI_ASSERT(nullptr == data, "Invalid OpenGL ES 3 indirect buffer data")
  }
}

/**
*  @brief
*    Destructor
*/
IndirectBuffer::~IndirectBuffer()
{
RHI_FREE(getRhi().getContext(), mData);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
