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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"



//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
namespace detail
{


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
static constexpr const char* GLSLES_NAME = "GLSLES";	///< ASCII name of this shader language, always valid (do not free the memory the returned pointer is pointing to)


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
void updateWidthHeight(RECore::uint32 mipmapIndex, RECore::uint32 textureWidth, RECore::uint32 textureHeight, RECore::uint32& width, RECore::uint32& height)
{
  RERHI::RHITexture::getMipmapSize(mipmapIndex, textureWidth, textureHeight);
  if (width > textureWidth)
  {
    width = textureWidth;
  }
  if (height > textureHeight)
  {
    height = textureHeight;
  }
}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;

class Helper {
public:
//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates, loads and compiles a shader from source code
*
*  @param[in] openGLES3Rhi
*    Owner OpenGL ES 3 RHI instance
*  @param[in] shaderType
*    Shader type (for example "GL_VERTEX_SHADER"
*  @param[in] sourceCode
*    Shader ASCII source code, must be a valid pointer
*
*  @return
*    The OpenGL ES 3 shader, 0 on error, destroy the returned resource if you no longer need it
*/
  [[nodiscard]] static GLuint
  loadShaderFromSourcecode(RHIDynamicRHI &openGLES3Rhi, GLenum shaderType, const GLchar *sourceCode) {
    // Create the shader object
    const GLuint openGLES3Shader = glCreateShader(shaderType);

    // Load the shader source
    glShaderSource(openGLES3Shader, 1, &sourceCode, nullptr);

    // Compile the shader
    glCompileShader(openGLES3Shader);

    // Check the compile status
    GLint compiled = GL_FALSE;
    glGetShaderiv(openGLES3Shader, GL_COMPILE_STATUS, &compiled);
    if (GL_TRUE == compiled) {
      // All went fine, return the shader
      return openGLES3Shader;
    } else {
      // Error, failed to compile the shader!

      { // Get the length of the information
        GLint informationLength = 0;
        glGetShaderiv(openGLES3Shader, GL_INFO_LOG_LENGTH, &informationLength);
        if (informationLength > 1) {
          // Allocate memory for the information
          const RERHI::RHIContext &context = openGLES3Rhi.getContext();
          GLchar *informationLog = RHI_MALLOC_TYPED(context, GLchar, informationLength);

          // Get the information
          glGetShaderInfoLog(openGLES3Shader, informationLength, nullptr, informationLog);

          // Output the debug string
          RE_LOG(Critical, informationLog)
          //if (openGLES3Rhi.getContext().getLog().print(RECore::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<RECore::uint32>(__LINE__), informationLog))
          {
            //	DEBUG_BREAK;
          }

          // Cleanup information memory
          RHI_FREE(context, informationLog);
        }
      }

      // Destroy the shader
      // -> A value of 0 for shader will be silently ignored
      glDeleteShader(openGLES3Shader);

      // Error!
      return 0;
    }
  }

};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
