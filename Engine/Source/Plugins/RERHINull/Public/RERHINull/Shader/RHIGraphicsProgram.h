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
#include "RERHINull/RERHINull.h"
#include <RERHI/Shader/RHIGraphicsProgram.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class VertexShader;
class TessellationControlShader;
class TessellationEvaluationShader;
class GeometryShader;
class TaskShader;
class MeshShader;
class FragmentShader;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null graphics program class
*/
class GraphicsProgram final : public RERHI::RHIGraphicsProgram
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
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
  GraphicsProgram(RHIDynamicRHI& nullRhi, VertexShader* vertexShader, TessellationControlShader* tessellationControlShader, TessellationEvaluationShader* tessellationEvaluationShader, GeometryShader* geometryShader, FragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

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
  GraphicsProgram(RHIDynamicRHI& nullRhi, TaskShader* taskShader, MeshShader& meshShader, FragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~GraphicsProgram() override;


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIGraphicsProgram methods          ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual RECore::handle getUniformHandle(const char*) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void selfDestruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit GraphicsProgram(const GraphicsProgram& source) = delete;
  GraphicsProgram& operator =(const GraphicsProgram& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
