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
#include "RERHIOpenGL/State/RHIComputePipelineState.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class ComputeShaderSeparate;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Separate compute pipeline state class
*/
class ComputePipelineStateSeparate : public ComputePipelineState
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] rootSignature
  *    Root signature
  *  @param[in] computeShaderSeparate
  *    Compute shader the compute pipeline state is using
  *  @param[in] id
  *    The unique compact compute pipeline state ID
  *
  *  @note
  *    - The compute pipeline state keeps a reference to the provided compute shader and releases it when no longer required
  */
  ComputePipelineStateSeparate(RHIDynamicRHI& openGLRhi, const RERHI::RHIRootSignature& rootSignature, ComputeShaderSeparate& computeShaderSeparate, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~ComputePipelineStateSeparate() override;

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


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), ComputePipelineStateSeparate, this);
  }


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint				   mOpenGLProgramPipeline;	///< OpenGL program pipeline ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
  ComputeShaderSeparate& mComputeShaderSeparate;	///< Compute shader the compute pipeline state is using (we keep a reference to it)


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit ComputePipelineStateSeparate(const ComputePipelineStateSeparate& source) = delete;
  ComputePipelineStateSeparate& operator =(const ComputePipelineStateSeparate& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
