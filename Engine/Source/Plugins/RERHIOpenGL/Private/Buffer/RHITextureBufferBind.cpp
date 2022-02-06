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
#include "RERHIOpenGL/Buffer/RHITextureBufferBind.h"
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
TextureBufferBind::TextureBufferBind(RHIDynamicRHI &openGLRhi, uint32_t numberOfBytes, const void *data,
                                     RERHI::BufferUsage bufferUsage, RERHI::TextureFormat::Enum textureFormat
                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  TextureBuffer(openGLRhi, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
  { // Buffer part
#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL texture buffer
              GLint openGLTextureBufferBackup = 0;
              glGetIntegerv(GL_TEXTURE_BINDING_BUFFER_ARB, &openGLTextureBufferBackup);
#endif

// Create the OpenGL texture buffer
    glGenBuffersARB(1, &mOpenGLTextureBuffer);

// Bind this OpenGL texture buffer and upload the data
// -> Usage: These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
    glBindBufferARB(GL_TEXTURE_BUFFER_ARB, mOpenGLTextureBuffer);
    glBufferDataARB(GL_TEXTURE_BUFFER_ARB, static_cast<GLsizeiptrARB>(numberOfBytes), data,
                    static_cast<GLenum>(bufferUsage));

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL texture buffer
              glBindBufferARB(GL_TEXTURE_BUFFER_ARB, static_cast<GLuint>(openGLTextureBufferBackup));
#endif
  }

  { // Texture part
#ifdef RHI_OPENGL_STATE_CLEANUP
    // Backup the currently bound OpenGL texture
              GLint openGLTextureBackup = 0;
              glGetIntegerv(GL_TEXTURE_BINDING_BUFFER_ARB, &openGLTextureBackup);
#endif

// Create the OpenGL texture instance
    glGenTextures(1, &mOpenGLTexture);

// Make this OpenGL texture instance to the currently used one
    glBindTexture(GL_TEXTURE_BUFFER_ARB, mOpenGLTexture);

// Attaches the storage for the buffer object to the active buffer texture
    glTexBufferARB(GL_TEXTURE_BUFFER_ARB, mOpenGLInternalFormat, mOpenGLTextureBuffer);

#ifdef RHI_OPENGL_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL texture
              glBindTexture(GL_TEXTURE_BUFFER_ARB, static_cast<GLuint>(openGLTextureBackup));
#endif
  }

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
            glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
            glObjectLabel(GL_BUFFER, mOpenGLTextureBuffer, -1, detailedDebugName);
          }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
