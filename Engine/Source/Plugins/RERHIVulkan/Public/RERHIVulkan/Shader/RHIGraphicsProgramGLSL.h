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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIVulkan/RERHIVulkan.h"
#include "RERHIVulkan/Shader/RHIComputeShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIVertexShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIFragmentShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIMeshShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITaskShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITessellationControlShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITessellationEvaluationShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIVulkan/Shader/RHIGeometryShaderGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    GLSL graphics program class
*/
class GraphicsProgramGlsl final : public RERHI::RHIGraphicsProgram
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor for traditional graphics program
  *
  *  @param[in] vulkanRhi
  *    Owner Vulkan RHI instance
  *  @param[in] vertexAttributes
  *    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
  *  @param[in] vertexShaderGlsl
  *    Vertex shader the graphics program is using, can be a null pointer
  *  @param[in] tessellationControlShaderGlsl
  *    Tessellation control shader the graphics program is using, can be a null pointer
  *  @param[in] tessellationEvaluationShaderGlsl
  *    Tessellation evaluation shader the graphics program is using, can be a null pointer
  *  @param[in] geometryShaderGlsl
  *    Geometry shader the graphics program is using, can be a null pointer
  *  @param[in] fragmentShaderGlsl
  *    Fragment shader the graphics program is using, can be a null pointer
  *
  *  @note
  *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
  */
  GraphicsProgramGlsl(RHIDynamicRHI& vulkanRhi, VertexShaderGlsl* vertexShaderGlsl, TessellationControlShaderGlsl* tessellationControlShaderGlsl, TessellationEvaluationShaderGlsl* tessellationEvaluationShaderGlsl, GeometryShaderGlsl* geometryShaderGlsl, FragmentShaderGlsl* fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

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
  GraphicsProgramGlsl(RHIDynamicRHI& vulkanRhi, TaskShaderGlsl* taskShaderGlsl, MeshShaderGlsl& meshShaderGlsl, FragmentShaderGlsl* fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~GraphicsProgramGlsl() override;

  //[-------------------------------------------------------]
  //[ Traditional graphics program                          ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Return the GLSL vertex shader the graphics program is using
  *
  *  @return
  *    The GLSL vertex shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline VertexShaderGlsl* getVertexShaderGlsl() const
  {
    return mVertexShaderGlsl;
  }

  /**
  *  @brief
  *    Return the GLSL tessellation control shader the graphics program is using
  *
  *  @return
  *    The GLSL tessellation control shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline TessellationControlShaderGlsl* getTessellationControlShaderGlsl() const
  {
    return mTessellationControlShaderGlsl;
  }

  /**
  *  @brief
  *    Return the GLSL tessellation evaluation shader the graphics program is using
  *
  *  @return
  *    The GLSL tessellation evaluation shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline TessellationEvaluationShaderGlsl* getTessellationEvaluationShaderGlsl() const
  {
    return mTessellationEvaluationShaderGlsl;
  }

  /**
  *  @brief
  *    Return the GLSL geometry shader the graphics program is using
  *
  *  @return
  *    The GLSL geometry shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline GeometryShaderGlsl* getGeometryShaderGlsl() const
  {
    return mGeometryShaderGlsl;
  }

  //[-------------------------------------------------------]
  //[ Both graphics programs                                ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Return the GLSL fragment shader the graphics program is using
  *
  *  @return
  *    The GLSL fragment shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline FragmentShaderGlsl* getFragmentShaderGlsl() const
  {
    return mFragmentShaderGlsl;
  }

  //[-------------------------------------------------------]
  //[ Task and mesh shader based graphics program           ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Return the GLSL task shader the graphics program is using
  *
  *  @return
  *    The GLSL task shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline TaskShaderGlsl* getTaskShaderGlsl() const
  {
    return mTaskShaderGlsl;
  }

  /**
  *  @brief
  *    Return the GLSL mesh shader the graphics program is using
  *
  *  @return
  *    The GLSL mesh shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline MeshShaderGlsl* getMeshShaderGlsl() const
  {
    return mMeshShaderGlsl;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIGraphicsProgram methods          ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual RECore::handle getUniformHandle(const char*) override
  {
    RHI_ASSERT(false, "The Vulkan RHI graphics program GLSL implementation doesn't have legacy uniform support")
    return NULL_HANDLE;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), GraphicsProgramGlsl, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit GraphicsProgramGlsl(const GraphicsProgramGlsl& source) = delete;
  GraphicsProgramGlsl& operator =(const GraphicsProgramGlsl& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Traditional graphics program
  VertexShaderGlsl*				  mVertexShaderGlsl;					///< Vertex shader the graphics program is using (we keep a reference to it), can be a null pointer
  TessellationControlShaderGlsl*	  mTessellationControlShaderGlsl;		///< Tessellation control shader the graphics program is using (we keep a reference to it), can be a null pointer
  TessellationEvaluationShaderGlsl* mTessellationEvaluationShaderGlsl;	///< Tessellation evaluation shader the graphics program is using (we keep a reference to it), can be a null pointer
  GeometryShaderGlsl*				  mGeometryShaderGlsl;					///< Geometry shader the graphics program is using (we keep a reference to it), can be a null pointer
  // Both graphics programs
  FragmentShaderGlsl* mFragmentShaderGlsl;	///< Fragment shader the graphics program is using (we keep a reference to it), can be a null pointer
  // Task and mesh shader based graphics program
  TaskShaderGlsl* mTaskShaderGlsl;	///< Task shader the graphics program is using (we keep a reference to it), can be a null pointer
  MeshShaderGlsl* mMeshShaderGlsl;	///< Mesh shader the graphics program is using (we keep a reference to it), can be a null pointer


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
