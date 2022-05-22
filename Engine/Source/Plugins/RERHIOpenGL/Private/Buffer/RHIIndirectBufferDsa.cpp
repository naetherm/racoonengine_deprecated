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
#include "RERHIOpenGL/Buffer/RHIIndirectBufferDsa.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
IndirectBufferDsa::IndirectBufferDsa(RHIDynamicRHI& openGLRhi, RECore::uint32 numberOfBytes, const void* data, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
IndirectBuffer(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  {
    if (openGLRhi.getExtensions().isGL_ARB_direct_state_access())
    {
      // Create the OpenGL indirect buffer
      glCreateBuffers(1, &mOpenGLIndirectBuffer);

      // Upload the data
      // -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
      glNamedBufferData(mOpenGLIndirectBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
    }
    else
    {
      // Create the OpenGL indirect buffer
      glGenBuffersARB(1, &mOpenGLIndirectBuffer);

      // Upload the data
      // -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
      glNamedBufferDataEXT(mOpenGLIndirectBuffer, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));
    }

    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IndirectBufferObject", 23)	// 23 = "IndirectBufferObject: " including terminating zero
					glObjectLabel(GL_BUFFER, mOpenGLIndirectBuffer, -1, detailedDebugName);
				}
#endif
  }


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
