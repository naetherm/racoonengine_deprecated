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
#include "RERHIOpenGLES3/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIOpenGLES3/Shader/RHIFragmentShaderGLSL.h"
#include "RERHIOpenGLES3/Shader/RHIVertexShaderGLSL.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Extensions.h"
#include "RERHIOpenGLES3/Helper.h"
#include "RERHIOpenGLES3/Mapping.h"
#include "RERHIOpenGLES3/RHIRootSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

GraphicsProgramGlsl::GraphicsProgramGlsl(RHIDynamicRHI &openGLES3Rhi, const RERHI::RHIRootSignature &rootSignature,
                                         const RERHI::VertexAttributes &vertexAttributes,
                                         VertexShaderGlsl *vertexShaderGlsl, FragmentShaderGlsl *fragmentShaderGlsl
                                         RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIGraphicsProgram(openGLES3Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mNumberOfRootSignatureParameters(0),
  mOpenGLES3Program(glCreateProgram()),
  mDrawIdUniformLocation(-1) {
  { // Define the vertex array attribute binding locations ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
    const uint32_t numberOfVertexAttributes = vertexAttributes.numberOfAttributes;
    for (uint32_t vertexAttribute = 0; vertexAttribute < numberOfVertexAttributes; ++vertexAttribute) {
      glBindAttribLocation(mOpenGLES3Program, vertexAttribute, vertexAttributes.attributes[vertexAttribute].name);
    }
  }

  // Attach the shaders to the program
  // -> We don't need to keep a reference to the shader, to add and release at once to ensure a nice behaviour
  if (nullptr != vertexShaderGlsl) {
    vertexShaderGlsl->AddReference();
    glAttachShader(mOpenGLES3Program, vertexShaderGlsl->getOpenGLES3Shader());
    vertexShaderGlsl->Release();
  }
  if (nullptr != fragmentShaderGlsl) {
    fragmentShaderGlsl->AddReference();
    glAttachShader(mOpenGLES3Program, fragmentShaderGlsl->getOpenGLES3Shader());
    fragmentShaderGlsl->Release();
  }

  // Link the program
  glLinkProgram(mOpenGLES3Program);

  // Check the link status
  GLint linked = GL_FALSE;
  glGetProgramiv(mOpenGLES3Program, GL_LINK_STATUS, &linked);
  if (GL_TRUE == linked) {
    // Get draw ID uniform location
    if (!openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_EXT_base_instance()) {
      mDrawIdUniformLocation = glGetUniformLocation(mOpenGLES3Program, "drawIdUniform");
    }

    // The actual locations assigned to uniform variables are not known until the program object is linked successfully
    // -> So we have to build a root signature parameter index -> uniform location mapping here
    const RERHI::RootSignature &rootSignatureData = static_cast<const RootSignature &>(rootSignature).getRootSignature();
    const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
    if (numberOfRootParameters > 0) {
      uint32_t uniformBlockBindingIndex = 0;
      const bool isGL_EXT_texture_buffer = openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_EXT_texture_buffer();
      for (uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex) {
        const RERHI::RootParameter &rootParameter = rootSignatureData.parameters[rootParameterIndex];
        if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
          RHI_ASSERT(
            nullptr != reinterpret_cast<const RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges),
            "Invalid OpenGL ES 3 descriptor ranges")
          const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
          for (uint32_t descriptorRangeIndex = 0;
               descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex) {
            const RERHI::DescriptorRange &descriptorRange = reinterpret_cast<const RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

            // Ignore sampler range types in here (OpenGL ES 3 handles samplers in a different way then Direct3D 10>=)
            if (RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType) {
              // Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
              // for backward compatibility, ask for the uniform block index
              const GLuint uniformBlockIndex = glGetUniformBlockIndex(mOpenGLES3Program,
                                                                      descriptorRange.baseShaderRegisterName);
              if (GL_INVALID_INDEX != uniformBlockIndex) {
                // Associate the uniform block with the given binding point
                glUniformBlockBinding(mOpenGLES3Program, uniformBlockIndex, uniformBlockBindingIndex);
                ++uniformBlockBindingIndex;
              }
            } else if (RERHI::DescriptorRangeType::SAMPLER != descriptorRange.rangeType) {
              // We can only emulate the "RERHI::TextureFormat::R32G32B32A32F" texture format using an uniform buffer
              // -> Check for something like "InstanceTextureBuffer". Yes, this only works when one sticks to the naming convention.
              if (!isGL_EXT_texture_buffer &&
                  nullptr != strstr(descriptorRange.baseShaderRegisterName, "TextureBuffer")) {
                // Texture buffer emulation using uniform buffer

                // Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
                // for backward compatibility, ask for the uniform block index
                const GLuint uniformBlockIndex = glGetUniformBlockIndex(mOpenGLES3Program,
                                                                        descriptorRange.baseShaderRegisterName);
                if (GL_INVALID_INDEX != uniformBlockIndex) {
                  // Associate the uniform block with the given binding point
                  glUniformBlockBinding(mOpenGLES3Program, uniformBlockIndex, uniformBlockBindingIndex);
                  ++uniformBlockBindingIndex;
                }
              } else {
                const GLint uniformLocation = glGetUniformLocation(mOpenGLES3Program,
                                                                   descriptorRange.baseShaderRegisterName);
                if (uniformLocation >= 0) {
                  // OpenGL ES 3/GLSL is not automatically assigning texture units to samplers, so, we have to take over this job
                  // -> When using OpenGL or OpenGL ES 3 this is required
                  // -> OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension supports explicit binding points ("layout(binding = 0)"
                  //    in GLSL shader) , for backward compatibility we don't use it in here
                  // -> When using Direct3D 9, Direct3D 10 or Direct3D 11, the texture unit
                  //    to use is usually defined directly within the shader by using the "register"-keyword
                  // TODO(naetherm) There's room for binding API call related optimization in here (will certainly be no huge overall efficiency gain)
#ifdef RHI_OPENGLES3_STATE_CLEANUP
                  // Backup the currently used OpenGL ES 3 program
                        GLint openGLES3ProgramBackup = 0;
                        glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
                        if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
                        {
                          // Set uniform, please note that for this our program must be the currently used one
                          glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
                        }
                        else
                        {
                          // Set uniform, please note that for this our program must be the currently used one
                          glUseProgram(mOpenGLES3Program);
                          glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));

                          // Be polite and restore the previous used OpenGL ES 3 program
                          glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
                        }
#else
                  // Set uniform, please note that for this our program must be the currently used one
                  glUseProgram(mOpenGLES3Program);
                  glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
#endif
                }
              }
            }
          }
        }
      }
    }
  } else {
    // Get the length of the information (including a null termination)
    GLint informationLength = 0;
    glGetProgramiv(mOpenGLES3Program, GL_INFO_LOG_LENGTH, &informationLength);
    if (informationLength > 1) {
      // Allocate memory for the information
      const RERHI::RHIContext &context = openGLES3Rhi.getContext();
      char *informationLog = RHI_MALLOC_TYPED(context, char, informationLength);

      // Get the information
      glGetProgramInfoLog(mOpenGLES3Program, informationLength, nullptr, informationLog);

      // Output the debug string
      RE_LOG(Critical, informationLog)

      // Cleanup information memory
      RHI_FREE(context, informationLog);
    }
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLES3Program && openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
          glObjectLabelKHR(GL_SHADER_KHR, mOpenGLES3Program, -1, detailedDebugName);
        }
#endif
}

/**
*  @brief
*    Destructor
*/
GraphicsProgramGlsl::~GraphicsProgramGlsl() {
// Destroy the OpenGL ES 3 program
// -> A value of 0 for program will be silently ignored
  glDeleteProgram(mOpenGLES3Program);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
