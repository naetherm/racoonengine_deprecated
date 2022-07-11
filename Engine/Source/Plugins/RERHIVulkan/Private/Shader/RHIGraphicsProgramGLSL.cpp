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
#include "RERHIVulkan/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

GraphicsProgramGlsl::GraphicsProgramGlsl(RHIDynamicRHI &vulkanRhi, VertexShaderGlsl *vertexShaderGlsl,
                                         TessellationControlShaderGlsl *tessellationControlShaderGlsl,
                                         TessellationEvaluationShaderGlsl *tessellationEvaluationShaderGlsl,
                                         GeometryShaderGlsl *geometryShaderGlsl, FragmentShaderGlsl *fragmentShaderGlsl
                                         RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIGraphicsProgram(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
// Traditional graphics program
  mVertexShaderGlsl(vertexShaderGlsl),
  mTessellationControlShaderGlsl(tessellationControlShaderGlsl),
  mTessellationEvaluationShaderGlsl(tessellationEvaluationShaderGlsl),
  mGeometryShaderGlsl(geometryShaderGlsl),
// Both graphics programs
  mFragmentShaderGlsl(fragmentShaderGlsl),
// Task and mesh shader based graphics program
  mTaskShaderGlsl(nullptr),
  mMeshShaderGlsl(nullptr) {
  // Add references to the provided shaders
  if (nullptr != mVertexShaderGlsl) {
    mVertexShaderGlsl->AddReference();
  }
  if (nullptr != mTessellationControlShaderGlsl) {
    mTessellationControlShaderGlsl->AddReference();
  }
  if (nullptr != mTessellationEvaluationShaderGlsl) {
    mTessellationEvaluationShaderGlsl->AddReference();
  }
  if (nullptr != mGeometryShaderGlsl) {
    mGeometryShaderGlsl->AddReference();
  }
  if (nullptr != mFragmentShaderGlsl) {
    mFragmentShaderGlsl->AddReference();
  }
}

/**
*  @brief
*    Constructor for task and mesh shader based graphics program
*
*  @param[in] vulkanRhi
*    Owner Vulkan RHI instance
*  @param[in] taskShaderGlsl
*    Task shader the graphics program is using, can be a null pointer
*  @param[in] meshShaderGlsl
*    Mesh shader the graphics program is using
*  @param[in] fragmentShaderGlsl
*    Fragment shader the graphics program is using, can be a null pointer
*
*  @note
*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
*/
GraphicsProgramGlsl::GraphicsProgramGlsl(RHIDynamicRHI &vulkanRhi, TaskShaderGlsl *taskShaderGlsl,
                                         MeshShaderGlsl &meshShaderGlsl, FragmentShaderGlsl *fragmentShaderGlsl
                                         RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIGraphicsProgram(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
// Traditional graphics program
  mVertexShaderGlsl(nullptr),
  mTessellationControlShaderGlsl(nullptr),
  mTessellationEvaluationShaderGlsl(nullptr),
  mGeometryShaderGlsl(nullptr),
// Both graphics programs
  mFragmentShaderGlsl(fragmentShaderGlsl),
// Task and mesh shader based graphics program
  mTaskShaderGlsl(taskShaderGlsl),
  mMeshShaderGlsl(&meshShaderGlsl) {
  // Add references to the provided shaders
  if (nullptr != mTaskShaderGlsl) {
    mTaskShaderGlsl->AddReference();
  }
  mMeshShaderGlsl->AddReference();
  if (nullptr != mFragmentShaderGlsl) {
    mFragmentShaderGlsl->AddReference();
  }
}

/**
*  @brief
*    Destructor
*/
GraphicsProgramGlsl::~GraphicsProgramGlsl() {
// Release the shader references
  if (nullptr != mVertexShaderGlsl) {
    mVertexShaderGlsl->Release();
  }
  if (nullptr != mTessellationControlShaderGlsl) {
    mTessellationControlShaderGlsl->Release();
  }
  if (nullptr != mTessellationEvaluationShaderGlsl) {
    mTessellationEvaluationShaderGlsl->Release();
  }
  if (nullptr != mGeometryShaderGlsl) {
    mGeometryShaderGlsl->Release();
  }
  if (nullptr != mFragmentShaderGlsl) {
    mFragmentShaderGlsl->Release();
  }
  if (nullptr != mTaskShaderGlsl) {
    mTaskShaderGlsl->Release();
  }
  if (nullptr != mMeshShaderGlsl) {
    mMeshShaderGlsl->Release();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
