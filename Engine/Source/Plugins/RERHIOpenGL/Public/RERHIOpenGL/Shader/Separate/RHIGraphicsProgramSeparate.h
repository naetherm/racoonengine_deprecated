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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class VertexShaderSeparate;
class TessellationControlShaderSeparate;
class TessellationEvaluationShaderSeparate;
class GeometryShaderSeparate;
class FragmentShaderSeparate;
class ComputeShaderSeparate;
class TaskShaderSeparate;
class MeshShaderSeparate;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Separate graphics program class
*/
class GraphicsProgramSeparate : public RERHI::RHIGraphicsProgram
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
  GraphicsProgramSeparate(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, VertexShaderSeparate* vertexShaderSeparate, TessellationControlShaderSeparate* tessellationControlShaderSeparate, TessellationEvaluationShaderSeparate* tessellationEvaluationShaderSeparate, GeometryShaderSeparate* geometryShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER);

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
  GraphicsProgramSeparate(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, TaskShaderSeparate* taskShaderSeparate, MeshShaderSeparate& meshShaderSeparate, FragmentShaderSeparate* fragmentShaderSeparate RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~GraphicsProgramSeparate() override;

  /**
  *  @brief
  *    Return the OpenGL program pipeline
  *
  *  @return
  *    The OpenGL program pipeline, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLProgramPipeline() const
  {
    return mOpenGLProgramPipeline;
  }

  /**
  *  @brief
  *    Return the vertex shader the program is using
  *
  *  @return
  *    Vertex shader the program is using, can be a null pointer, don't destroy the instance
  */
  [[nodiscard]] inline VertexShaderSeparate* getVertexShaderSeparate() const
  {
    return mVertexShaderSeparate;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIGraphicsProgram methods          ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual RECore::handle getUniformHandle(const char*) override
  {
    RHI_ASSERT(false, "The OpenGL RHI graphics program separate implementation doesn't have legacy uniform support")
    return NULL_HANDLE;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), GraphicsProgramSeparate, this);
  }


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint mOpenGLProgramPipeline;	///< OpenGL program pipeline ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
  // Traditional graphics program
  VertexShaderSeparate*				  mVertexShaderSeparate;					///< Vertex shader the program is using (we keep a reference to it), can be a null pointer
  TessellationControlShaderSeparate*	  mTessellationControlShaderSeparate;		///< Tessellation control shader the program is using (we keep a reference to it), can be a null pointer
  TessellationEvaluationShaderSeparate* mTessellationEvaluationShaderSeparate;	///< Tessellation evaluation shader the program is using (we keep a reference to it), can be a null pointer
  GeometryShaderSeparate*				  mGeometryShaderSeparate;					///< Geometry shader the program is using (we keep a reference to it), can be a null pointer
  // Both graphics programs
  FragmentShaderSeparate* mFragmentShaderSeparate;	///< Fragment shader the program is using (we keep a reference to it), can be a null pointer
  // Task and mesh shader based graphics program
  TaskShaderSeparate* mTaskShaderSeparate;	///< Task shader the program is using (we keep a reference to it), can be a null pointer
  MeshShaderSeparate* mMeshShaderSeparate;	///< Mesh shader the program is using (we keep a reference to it), can be a null pointer


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit GraphicsProgramSeparate(const GraphicsProgramSeparate& source) = delete;
  GraphicsProgramSeparate& operator =(const GraphicsProgramSeparate& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
