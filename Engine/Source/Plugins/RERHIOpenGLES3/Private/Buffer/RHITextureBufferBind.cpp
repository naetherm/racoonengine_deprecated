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
#include "RERHIOpenGLES3/Buffer/RHITextureBufferBind.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


TextureBufferBind::TextureBufferBind(RHIDynamicRHI &openGLES3Rhi, RECore::uint32 numberOfBytes, const void *data,
                                     RERHI::BufferUsage bufferUsage, RERHI::TextureFormat::Enum textureFormat
                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  TextureBuffer(openGLES3Rhi, numberOfBytes RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
  { // Buffer part
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently bound OpenGL ES 3 texture buffer
              GLint openGLES3TextureBufferBackup = 0;
              glGetIntegerv(GL_TEXTURE_BINDING_BUFFER_EXT, &openGLES3TextureBufferBackup);
#endif

// Bind this OpenGL ES 3 texture buffer and upload the data
    glBindBuffer(GL_TEXTURE_BUFFER_EXT, mOpenGLES3TextureBuffer);
// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
    glBufferData(GL_TEXTURE_BUFFER_EXT, static_cast<GLsizeiptr>(numberOfBytes), data, static_cast<GLenum>(bufferUsage));

#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL ES 3 texture buffer
              glBindBuffer(GL_TEXTURE_BUFFER_EXT, static_cast<GLuint>(openGLES3TextureBufferBackup));
#endif
  }

  { // Texture part
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently bound OpenGL ES 3 texture
              GLint openGLESTextureBackup = 0;
              glGetIntegerv(GL_TEXTURE_BUFFER_BINDING_EXT, &openGLESTextureBackup);
#endif

// Make this OpenGL ES 3 texture instance to the currently used one
    glBindTexture(GL_TEXTURE_BUFFER_EXT, mOpenGLES3Texture);

// Attaches the storage for the buffer object to the active buffer texture
    glTexBufferEXT(GL_TEXTURE_BUFFER_EXT, Mapping::getOpenGLES3InternalFormat(textureFormat), mOpenGLES3TextureBuffer);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL ES 3 texture
              glBindTexture(GL_TEXTURE_BUFFER_EXT, static_cast<GLuint>(openGLESTextureBackup));
#endif
  }

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
            if (0 != mOpenGLES3Texture)
            {
              glObjectLabelKHR(GL_TEXTURE, mOpenGLES3Texture, -1, detailedDebugName);
            }
            if (0 != mOpenGLES3TextureBuffer)
            {
              glObjectLabelKHR(GL_BUFFER_KHR, mOpenGLES3TextureBuffer, -1, detailedDebugName);
            }
          }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
