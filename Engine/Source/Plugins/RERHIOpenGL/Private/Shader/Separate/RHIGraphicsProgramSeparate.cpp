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
#include "RERHIOpenGL/Shader/Separate/RHIGraphicsProgramSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIVertexShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHITessellationControlShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHITessellationEvaluationShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIGeometryShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIFragmentShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIComputeShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHITaskShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIMeshShaderSeparate.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/RHIRootSignature.h"
#include "RERHIOpenGL/Extensions.h"
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
GraphicsProgramSeparate::GraphicsProgramSeparate(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, VertexShaderSeparate* vertexShaderSeparate, TessellationControlShaderSeparate* tessellationControlShaderSeparate, TessellationEvaluationShaderSeparate* tessellationEvaluationShaderSeparate, GeometryShaderSeparate* geometryShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLProgramPipeline(0),
// Traditional graphics program
mVertexShaderSeparate(vertexShaderSeparate),
mTessellationControlShaderSeparate(tessellationControlShaderSeparate),
mTessellationEvaluationShaderSeparate(tessellationEvaluationShaderSeparate),
mGeometryShaderSeparate(geometryShaderSeparate),
// Both graphics programs
mFragmentShaderSeparate(fragmentShaderSeparate),
// Task and mesh shader based graphics program
mTaskShaderSeparate(nullptr),
mMeshShaderSeparate(nullptr)
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

    // Add references to the provided shaders
#define USE_PROGRAM_STAGES(ShaderBit, ShaderSeparate) if (nullptr != ShaderSeparate) { ShaderSeparate->AddReference(); glUseProgramStages(mOpenGLProgramPipeline, ShaderBit, ShaderSeparate->getOpenGLShaderProgram()); }
  USE_PROGRAM_STAGES(GL_VERTEX_SHADER_BIT,		  mVertexShaderSeparate)
  USE_PROGRAM_STAGES(GL_TESS_CONTROL_SHADER_BIT,	  mTessellationControlShaderSeparate)
  USE_PROGRAM_STAGES(GL_TESS_EVALUATION_SHADER_BIT, mTessellationEvaluationShaderSeparate)
  USE_PROGRAM_STAGES(GL_GEOMETRY_SHADER_BIT,		  mGeometryShaderSeparate)
  USE_PROGRAM_STAGES(GL_FRAGMENT_SHADER_BIT,		  mFragmentShaderSeparate)
#undef USE_PROGRAM_STAGES

  // Validate program pipeline
  glValidateProgramPipeline(mOpenGLProgramPipeline);
  GLint validateStatus = 0;
  glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
  if (GL_TRUE == validateStatus)
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
    const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
    uint32_t uniformBlockBindingIndex = 0;
    for (uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex)
    {
      const RERHI::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
      if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType)
      {
        RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
        const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
        for (uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex)
        {
          const RERHI::DescriptorRange& descriptorRange = reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

          // Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
          if (RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType)
          {
#define BIND_UNIFORM_BLOCK(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bindUniformBlock(descriptorRange, ShaderSeparate->getOpenGLShaderProgram(), uniformBlockBindingIndex);
            switch (descriptorRange.shaderVisibility)
            {
              case RERHI::ShaderVisibility::ALL:
              case RERHI::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_BLOCK(mVertexShaderSeparate)
                BIND_UNIFORM_BLOCK(mTessellationControlShaderSeparate)
                BIND_UNIFORM_BLOCK(mTessellationEvaluationShaderSeparate)
                BIND_UNIFORM_BLOCK(mGeometryShaderSeparate)
                BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
                break;

              case RERHI::ShaderVisibility::VERTEX:
                BIND_UNIFORM_BLOCK(mVertexShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
                BIND_UNIFORM_BLOCK(mTessellationControlShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
                BIND_UNIFORM_BLOCK(mTessellationEvaluationShaderSeparate)
                break;

              case RERHI::ShaderVisibility::GEOMETRY:
                BIND_UNIFORM_BLOCK(mGeometryShaderSeparate)
                break;

              case RERHI::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TASK:
              case RERHI::ShaderVisibility::MESH:
              case RERHI::ShaderVisibility::COMPUTE:
                RHI_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_BLOCK
            ++uniformBlockBindingIndex;
          }
          else if (RERHI::DescriptorRangeType::SAMPLER != descriptorRange.rangeType)
          {
#define BIND_UNIFORM_LOCATION(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bindUniformLocation(descriptorRange, mOpenGLProgramPipeline, ShaderSeparate->getOpenGLShaderProgram());
            switch (descriptorRange.shaderVisibility)
            {
              case RERHI::ShaderVisibility::ALL:
              case RERHI::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_LOCATION(mVertexShaderSeparate)
                BIND_UNIFORM_LOCATION(mTessellationControlShaderSeparate)
                BIND_UNIFORM_LOCATION(mTessellationEvaluationShaderSeparate)
                BIND_UNIFORM_LOCATION(mGeometryShaderSeparate)
                BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
                break;

              case RERHI::ShaderVisibility::VERTEX:
                BIND_UNIFORM_LOCATION(mVertexShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
                BIND_UNIFORM_LOCATION(mTessellationControlShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
                BIND_UNIFORM_LOCATION(mTessellationEvaluationShaderSeparate)
                break;

              case RERHI::ShaderVisibility::GEOMETRY:
                BIND_UNIFORM_LOCATION(mGeometryShaderSeparate)
                break;

              case RERHI::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TASK:
              case RERHI::ShaderVisibility::MESH:
              case RERHI::ShaderVisibility::COMPUTE:
                RHI_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_LOCATION
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
  if (0 != mOpenGLProgramPipeline && openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
					glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
				}
#endif
}

GraphicsProgramSeparate::GraphicsProgramSeparate(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, TaskShaderSeparate* taskShaderSeparate, MeshShaderSeparate& meshShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIGraphicsProgram(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLProgramPipeline(0),
// Traditional graphics program
mVertexShaderSeparate(nullptr),
mTessellationControlShaderSeparate(nullptr),
mTessellationEvaluationShaderSeparate(nullptr),
mGeometryShaderSeparate(nullptr),
// Both graphics programs
mFragmentShaderSeparate(fragmentShaderSeparate),
// Task and mesh shader based graphics program
mTaskShaderSeparate(taskShaderSeparate),
mMeshShaderSeparate(&meshShaderSeparate)
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

    // Add references to the provided shaders
#define USE_PROGRAM_STAGES(ShaderBit, ShaderSeparate) if (nullptr != ShaderSeparate) { ShaderSeparate->AddReference(); glUseProgramStages(mOpenGLProgramPipeline, ShaderBit, ShaderSeparate->getOpenGLShaderProgram()); }
  USE_PROGRAM_STAGES(GL_TASK_SHADER_BIT_NV,  mTaskShaderSeparate)
  USE_PROGRAM_STAGES(GL_MESH_SHADER_BIT_NV,  mMeshShaderSeparate)
  USE_PROGRAM_STAGES(GL_FRAGMENT_SHADER_BIT, mFragmentShaderSeparate)
#undef USE_PROGRAM_STAGES

  // Validate program pipeline
  glValidateProgramPipeline(mOpenGLProgramPipeline);
  GLint validateStatus = 0;
  glGetProgramPipelineiv(mOpenGLProgramPipeline, GL_VALIDATE_STATUS, &validateStatus);
  if (GL_TRUE == validateStatus)
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
    const uint32_t numberOfRootParameters = rootSignatureData.numberOfParameters;
    uint32_t uniformBlockBindingIndex = 0;
    for (uint32_t rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex)
    {
      const RERHI::RootParameter& rootParameter = rootSignatureData.parameters[rootParameterIndex];
      if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType)
      {
        RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
        const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
        for (uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex)
        {
          const RERHI::DescriptorRange& descriptorRange = reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];

          // Ignore sampler range types in here (OpenGL handles samplers in a different way then Direct3D 10>=)
          if (RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType)
          {
#define BIND_UNIFORM_BLOCK(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bindUniformBlock(descriptorRange, ShaderSeparate->getOpenGLShaderProgram(), uniformBlockBindingIndex);
            switch (descriptorRange.shaderVisibility)
            {
              case RERHI::ShaderVisibility::ALL:
              case RERHI::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_BLOCK(mTaskShaderSeparate)
                BIND_UNIFORM_BLOCK(mMeshShaderSeparate)
                break;

              case RERHI::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_BLOCK(mFragmentShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TASK:
                BIND_UNIFORM_BLOCK(mTaskShaderSeparate)
                break;

              case RERHI::ShaderVisibility::MESH:
                BIND_UNIFORM_BLOCK(mMeshShaderSeparate)
                break;

              case RERHI::ShaderVisibility::VERTEX:
              case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
              case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
              case RERHI::ShaderVisibility::GEOMETRY:
              case RERHI::ShaderVisibility::COMPUTE:
                RHI_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_BLOCK
            ++uniformBlockBindingIndex;
          }
          else if (RERHI::DescriptorRangeType::SAMPLER != descriptorRange.rangeType)
          {
#define BIND_UNIFORM_LOCATION(ShaderSeparate) if (nullptr != ShaderSeparate) Helper::bindUniformLocation(descriptorRange, mOpenGLProgramPipeline, ShaderSeparate->getOpenGLShaderProgram());
            switch (descriptorRange.shaderVisibility)
            {
              case RERHI::ShaderVisibility::ALL:
              case RERHI::ShaderVisibility::ALL_GRAPHICS:
                BIND_UNIFORM_LOCATION(mTaskShaderSeparate)
                BIND_UNIFORM_LOCATION(mMeshShaderSeparate)
                break;

              case RERHI::ShaderVisibility::FRAGMENT:
                BIND_UNIFORM_LOCATION(mFragmentShaderSeparate)
                break;

              case RERHI::ShaderVisibility::TASK:
                BIND_UNIFORM_LOCATION(mTaskShaderSeparate)
                break;

              case RERHI::ShaderVisibility::MESH:
                BIND_UNIFORM_LOCATION(mMeshShaderSeparate)
                break;

              case RERHI::ShaderVisibility::VERTEX:
              case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
              case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
              case RERHI::ShaderVisibility::GEOMETRY:
              case RERHI::ShaderVisibility::COMPUTE:
                RHI_ASSERT(false, "Invalid OpenGL shader visibility")
                break;
            }
#undef BIND_UNIFORM_LOCATION
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
  if (0 != mOpenGLProgramPipeline && openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics program", 19)	// 19 = "Graphics program: " including terminating zero
					glObjectLabel(GL_PROGRAM_PIPELINE, mOpenGLProgramPipeline, -1, detailedDebugName);
				}
#endif
}

GraphicsProgramSeparate::~GraphicsProgramSeparate()
{
  // Destroy the OpenGL program pipeline
  glDeleteProgramPipelines(1, &mOpenGLProgramPipeline);

  // Release the shader references
  if (nullptr != mVertexShaderSeparate)
  {
    mVertexShaderSeparate->Release();
  }
  if (nullptr != mTessellationControlShaderSeparate)
  {
    mTessellationControlShaderSeparate->Release();
  }
  if (nullptr != mTessellationEvaluationShaderSeparate)
  {
    mTessellationEvaluationShaderSeparate->Release();
  }
  if (nullptr != mGeometryShaderSeparate)
  {
    mGeometryShaderSeparate->Release();
  }
  if (nullptr != mFragmentShaderSeparate)
  {
    mFragmentShaderSeparate->Release();
  }
  if (nullptr != mTaskShaderSeparate)
  {
    mTaskShaderSeparate->Release();
  }
  if (nullptr != mMeshShaderSeparate)
  {
    mMeshShaderSeparate->Release();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
