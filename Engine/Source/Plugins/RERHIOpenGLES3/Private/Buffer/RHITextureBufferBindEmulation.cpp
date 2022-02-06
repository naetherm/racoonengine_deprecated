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
#include "RERHIOpenGLES3/Buffer/RHITextureBufferBindEmulation.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


TextureBufferBindEmulation::TextureBufferBindEmulation(RHIDynamicRHI &openGLES3Rhi, uint32_t numberOfBytes,
                                                       const void *data, RERHI::BufferUsage bufferUsage,
                                                       [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat
                                                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  TextureBuffer(openGLES3Rhi, numberOfBytes RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently bound OpenGL ES 3 uniform buffer
          GLint openGLES3UniformBufferBackup = 0;
          glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &openGLES3UniformBufferBackup);
#endif

// TODO(naetherm) Review OpenGL ES 3 uniform buffer alignment topic

// Bind this OpenGL ES 3 uniform buffer and upload the data
  glBindBuffer(GL_UNIFORM_BUFFER, mOpenGLES3TextureBuffer);
// -> Usage: These constants directly map to GL_EXT_vertex_buffer_object and OpenGL ES 3 constants, do not change them
  glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 uniform buffer
          glBindBuffer(GL_UNIFORM_BUFFER, static_cast<GLuint>(openGLES3UniformBufferBackup));
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
            glObjectLabelKHR(GL_BUFFER_KHR, mOpenGLES3TextureBuffer, -1, detailedDebugName);
          }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
