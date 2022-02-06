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
#include "RERHIOpenGL/Shader/Separate/RHIGraphicsProgramSeparate.h"


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
/**
*  @brief
*    Separate graphics program class, effective direct state access (DSA)
*/
class GraphicsProgramSeparateDsa final : public GraphicsProgramSeparate
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
  *  @param[in] vertexShaderSeparate
  *    Vertex shader the graphics program is using, can be a null pointer
  *  @param[in] tessellationControlShaderSeparate
  *    Tessellation control shader the graphics program is using, can be a null pointer
  *  @param[in] tessellationEvaluationShaderSeparate
  *    Tessellation evaluation shader the graphics program is using, can be a null pointer
  *  @param[in] geometryShaderSeparate
  *    Geometry shader the graphics program is using, can be a null pointer
  *  @param[in] fragmentShaderSeparate
  *    Fragment shader the graphics program is using, can be a null pointer
  *
  *  @note
  *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
  */
  GraphicsProgramSeparateDsa(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, VertexShaderSeparate* vertexShaderSeparate, TessellationControlShaderSeparate* tessellationControlShaderSeparate, TessellationEvaluationShaderSeparate* tessellationEvaluationShaderSeparate, GeometryShaderSeparate* geometryShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Constructor for task and mesh shader based graphics program
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] rootSignature
  *    Root signature
  *  @param[in] taskShaderSeparate
  *    Task shader the graphics program is using, can be a null pointer
  *  @param[in] meshShaderSeparate
  *    Mesh shader the graphics program is using
  *  @param[in] fragmentShaderSeparate
  *    Fragment shader the graphics program is using, can be a null pointer
  *
  *  @note
  *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
  */
  GraphicsProgramSeparateDsa(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, TaskShaderSeparate* taskShaderSeparate, MeshShaderSeparate& meshShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~GraphicsProgramSeparateDsa() override = default;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit GraphicsProgramSeparateDsa(const GraphicsProgramSeparateDsa& source) = delete;
  GraphicsProgramSeparateDsa& operator =(const GraphicsProgramSeparateDsa& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
