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
#include "RERHIOpenGL/State/RHIComputePipelineStateSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIComputeShaderSeparate.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/RHIRootSignature.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Mapping.h"
#include "RERHIOpenGL/Helper.h"


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
ComputePipelineStateSeparate::ComputePipelineStateSeparate(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, ComputeShaderSeparate& computeShaderSeparate, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
ComputePipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLProgramPipeline(0),
mComputeShaderSeparate(computeShaderSeparate)
  {
    // Create the OpenGL program pipeline
    glGenProgramPipelines(1, &mOpenGLProgramPipeline);

  // If the "GL_ARB_direct_state_access" nor "GL_EXT_direct_state_access" extension is available, we need to change OpenGL states during resource creation (nasty thing)
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently used OpenGL program pipeline
				GLint openGLProgramPipelineBackup = 0;
#endif
  if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT)
  {
#ifdef RHI_OPENGL_STATE_CLEANUP
    glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &openGLProgramPipelineBackup);
#endif
    glBindProgramPipeline(mOpenGLProgramPipeline);
  }

  // Add reference to the provided compute shader
  computeShaderSeparate.AddReference();
  glUseProgramStages(mOpenGLProgramPipeline, GL_COMPUTE_SHADER_BIT, computeShaderSeparate.getOpenGLShaderProgram());

  // Validate program pipeline
  glValidateProgramPipeline(mOpenGLProgramPipeline);
  GLint validateStatus = 0;
  glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
  if (true)	// TODO(naetherm) Compute shader: Validate status always returns failure without log when using a compute shader? AMD 290X Radeon software version 18.7.1.
  //if (GL_TRUE == validateStatus)
  {
    // We're not using "glBindFragDataLocation()", else the user would have to provide us with additional OpenGL-only specific information
    // -> Use modern GLSL:
    //    "layout(location = 0) out vec4 ColorOutput0;"
    //    "layout(location = 1) out vec4 ColorOutput1;"
    // -> Use legacy GLSL if necessary:
    //    "gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);"
    //    "gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);"

    // The actual locations assigned to uniform variables are not known until the program object is linked successfully
    // -> So we have to build a root signature parameter index -> uniform location mapping here
    const RERHI::RootSignature& rootSignatureData = static_cast<const RootSignature&>(rootSignature).getRootSignature();
    const RECore::uint32 numberOfRootParameters = rootSignatureData.numberOfParameters;
    RECore::uint32 uniformBlockBindingIndex = 0;
    for (RECore::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex)
    {
      const RERHI::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
      if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType)
      {
        RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
        const RECore::uint32 numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
        for (RECore::uint32 descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex)
        {
          const RERHI::DescriptorRange& descriptorRange = reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

          // Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
          if (RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType)
          {
            switch (descriptorRange.shaderVisibility)
            {
              case RERHI::ShaderVisibility::ALL_GRAPHICS:
              case RERHI::ShaderVisibility::VERTEX:
              case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
              case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
              case RERHI::ShaderVisibility::GEOMETRY:
              case RERHI::ShaderVisibility::FRAGMENT:
              case RERHI::ShaderVisibility::TASK:
              case RERHI::ShaderVisibility::MESH:
                RHI_ASSERT(false, "Invalid OpenGL shader visibility")
                break;

              case RERHI::ShaderVisibility::ALL:
              case RERHI::ShaderVisibility::COMPUTE:
                Helper::bindUniformBlock(descriptorRange, mComputeShaderSeparate.getOpenGLShaderProgram(), uniformBlockBindingIndex);
                break;
            }
            ++uniformBlockBindingIndex;
          }
          else if (RERHI::DescriptorRangeType::SAMPLER != descriptorRange.rangeType)
          {
            switch (descriptorRange.shaderVisibility)
            {
              case RERHI::ShaderVisibility::ALL_GRAPHICS:
              case RERHI::ShaderVisibility::VERTEX:
              case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
              case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
              case RERHI::ShaderVisibility::GEOMETRY:
              case RERHI::ShaderVisibility::FRAGMENT:
              case RERHI::ShaderVisibility::TASK:
              case RERHI::ShaderVisibility::MESH:
                RHI_ASSERT(false, "Invalid OpenGL shader visibility")
                break;

              case RERHI::ShaderVisibility::ALL:
              case RERHI::ShaderVisibility::COMPUTE:
                Helper::bindUniformLocation(descriptorRange, mOpenGLProgramPipeline, mComputeShaderSeparate.getOpenGLShaderProgram());
                break;
            }
          }
        }
      }
    }
  }
  else
  {
    // Error, program pipeline validation failed!

    // Get the length of the information (including a null termination)
    GLint informationLength = 0;
    glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_INFO_LOG_LENGTH, &informationLength);
    if (informationLength > 1)
    {
      // Allocate memory for the information
      const RERHI::RHIContext& context = openGLRhi.getContext();
      char* informationLog = RHI_MALLOC_TYPED(context, char, informationLength);

      // Get the information
      glGetProgramPipelineInfoLog(mOpenGLProgramPipeline, informationLength, nullptr, informationLog);

      // Output the debug string
      RE_LOG(Critical, informationLog)

      // Cleanup information memory
      RHI_FREE(context, informationLog);
    }
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous used OpenGL program pipeline
				if (nullptr == glProgramUniform1i && nullptr == glProgramUniform1iEXT)
				{
					glBindProgramPipeline(static_cast<GLuint>(openGLProgramPipelineBackup));
				}
#endif

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
					glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
				}
#endif
  }

ComputePipelineStateSeparate::~ComputePipelineStateSeparate()
{
  // Destroy the OpenGL program pipeline
  glDeleteProgramPipelines(1, &mOpenGLProgramPipeline);

  // Release the compute shader reference
  mComputeShaderSeparate.Release();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
