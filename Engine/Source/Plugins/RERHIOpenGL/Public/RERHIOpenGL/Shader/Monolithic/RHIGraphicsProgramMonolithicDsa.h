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
#include "RERHIOpenGL/RERHIOpenGL.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIGraphicsProgramMonolithic.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class VertexShaderMonolithic;
class TessellationControlShaderMonolithic;
class TessellationEvaluationShaderMonolithic;
class FragmentShaderMonolithic;
class GeometryShaderMonolithic;
class TaskShaderMonolithic;
class MeshShaderMonolithic;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Monolithic graphics program class, effective direct state access (DSA)
*/
class GraphicsProgramMonolithicDsa final : public GraphicsProgramMonolithic
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor for traditional graphics program
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] rootSignature
  *    Root signature
  *  @param[in] vertexAttributes
  *    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
  *  @param[in] vertexShaderMonolithic
  *    Vertex shader the graphics program is using, can be a null pointer
  *  @param[in] tessellationControlShaderMonolithic
  *    Tessellation control shader the graphics program is using, can be a null pointer
  *  @param[in] tessellationEvaluationShaderMonolithic
  *    Tessellation evaluation shader the graphics program is using, can be a null pointer
  *  @param[in] geometryShaderMonolithic
  *    Geometry shader the graphics program is using, can be a null pointer
  *  @param[in] fragmentShaderMonolithic
  *    Fragment shader the graphics program is using, can be a null pointer
  *
  *  @note
  *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
  */
  GraphicsProgramMonolithicDsa(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, const RERHI::VertexAttributes& vertexAttributes, VertexShaderMonolithic* vertexShaderMonolithic, TessellationControlShaderMonolithic* tessellationControlShaderMonolithic, TessellationEvaluationShaderMonolithic* tessellationEvaluationShaderMonolithic, GeometryShaderMonolithic* geometryShaderMonolithic, FragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Constructor for task and mesh shader based graphics program
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] rootSignature
  *    Root signature
  *  @param[in] taskShaderMonolithic
  *    Task shader the graphics program is using, can be a null pointer
  *  @param[in] meshShaderMonolithic
  *    Mesh shader the graphics program is using
  *  @param[in] fragmentShaderMonolithic
  *    Fragment shader the graphics program is using, can be a null pointer
  *
  *  @note
  *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
  */
  GraphicsProgramMonolithicDsa(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, TaskShaderMonolithic* taskShaderMonolithic, MeshShaderMonolithic& meshShaderMonolithic, FragmentShaderMonolithic* fragmentShaderMonolithic RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~GraphicsProgramMonolithicDsa() override = default;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit GraphicsProgramMonolithicDsa(const GraphicsProgramMonolithicDsa& source) = delete;
  GraphicsProgramMonolithicDsa& operator =(const GraphicsProgramMonolithicDsa& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
