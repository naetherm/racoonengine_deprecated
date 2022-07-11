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
#include "RERHINull/Shader/RHIGraphicsProgram.h"
#include "RERHINull/Shader/RHIComputeShader.h"
#include "RERHINull/Shader/RHIFragmentShader.h"
#include "RERHINull/Shader/RHIGeometryShader.h"
#include "RERHINull/Shader/RHIMeshShader.h"
#include "RERHINull/Shader/RHITaskShader.h"
#include "RERHINull/Shader/RHITessellationControlShader.h"
#include "RERHINull/Shader/RHITessellationEvaluationShader.h"
#include "RERHINull/Shader/RHIVertexShader.h"
#include "RERHINull/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

/**
*  @brief
*    Constructor for traditional graphics program
*
*  @param[in] nullRhi
*    Owner null RHI instance
*  @param[in] vertexShader
*    Vertex shader the graphics program is using, can be a null pointer
*  @param[in] tessellationControlShader
*    Tessellation control shader the graphics program is using, can be a null pointer
*  @param[in] tessellationEvaluationShader
*    Tessellation evaluation shader the graphics program is using, can be a null pointer
*  @param[in] geometryShader
*    Geometry shader the graphics program is using, can be a null pointer
*  @param[in] fragmentShader
*    Fragment shader the graphics program is using, can be a null pointer
*
*  @note
*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
*/
GraphicsProgram::GraphicsProgram(RHIDynamicRHI& nullRhi, VertexShader* vertexShader, TessellationControlShader* tessellationControlShader, TessellationEvaluationShader* tessellationEvaluationShader, GeometryShader* geometryShader, FragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIGraphicsProgram(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  {
    // We don't keep a reference to the shaders in here
    // -> Ensure a correct reference counter behaviour
    if (nullptr != vertexShader)
    {
      vertexShader->AddReference();
      vertexShader->Release();
    }
    if (nullptr != tessellationControlShader)
    {
      tessellationControlShader->AddReference();
      tessellationControlShader->Release();
    }
    if (nullptr != tessellationEvaluationShader)
    {
      tessellationEvaluationShader->AddReference();
      tessellationEvaluationShader->Release();
    }
    if (nullptr != geometryShader)
    {
      geometryShader->AddReference();
      geometryShader->Release();
    }
    if (nullptr != fragmentShader)
    {
      fragmentShader->AddReference();
      fragmentShader->Release();
    }
  }

/**
*  @brief
*    Constructor for task and mesh shader based graphics program
*
*  @param[in] nullRhi
*    Owner null RHI instance
*  @param[in] taskShader
*    Task shader the graphics program is using, can be a null pointer
*  @param[in] meshShader
*    Mesh shader the graphics program is using
*  @param[in] fragmentShader
*    Fragment shader the graphics program is using, can be a null pointer
*
*  @note
*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
*/
GraphicsProgram::GraphicsProgram(RHIDynamicRHI& nullRhi, TaskShader* taskShader, MeshShader& meshShader, FragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIGraphicsProgram(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  {
    // We don't keep a reference to the shaders in here
    // -> Ensure a correct reference counter behaviour
    if (nullptr != taskShader)
    {
      taskShader->AddReference();
      taskShader->Release();
    }
    meshShader.AddReference();
    meshShader.Release();
    if (nullptr != fragmentShader)
    {
      fragmentShader->AddReference();
      fragmentShader->Release();
    }
  }

/**
*  @brief
*    Destructor
*/
GraphicsProgram::~GraphicsProgram()
{}


RECore::handle GraphicsProgram::getUniformHandle(const char*)
{
  RHI_ASSERT(false, "The null RHI graphics program implementation doesn't have legacy uniform support")
  return NULL_HANDLE;
}


void GraphicsProgram::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), GraphicsProgram, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
