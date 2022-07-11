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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"
#include "RERHIOpenGLES3/State/RHIBlendState.h"
#include "RERHIOpenGLES3/State/RHIDepthStencilState.h"
#include "RERHIOpenGLES3/State/RHIRasterizerState.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    OpenGL ES 3 graphics pipeline state class
*/
class GraphicsPipelineState final : public RERHI::RHIGraphicsPipelineState
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  *  @param[in] graphicsPipelineState
  *    Graphics pipeline state to use
  *  @param[in] id
  *    The unique compact graphics pipeline state ID
  */
  GraphicsPipelineState(RHIDynamicRHI& openGLES3Rhi, const RERHI::GraphicsPipelineState& graphicsPipelineState, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~GraphicsPipelineState() override;

  /**
  *  @brief
  *    Return the OpenGL ES 3 primitive topology describing the type of primitive to render
  *
  *  @return
  *    OpenGL ES 3 primitive topology describing the type of primitive to render
  */
  [[nodiscard]] inline GLenum getOpenGLES3PrimitiveTopology() const
  {
    return mOpenGLES3PrimitiveTopology;
  }

  /**
  *  @brief
  *    Bind the graphics pipeline state
  */
  void bindGraphicsPipelineState() const;

  //[-------------------------------------------------------]
  //[ Detail state access                                   ]
  //[-------------------------------------------------------]
  [[nodiscard]] inline const RERHI::RasterizerState& getRasterizerState() const
  {
    return mRasterizerState.getRasterizerState();
  }

  [[nodiscard]] inline const RERHI::DepthStencilState& getDepthStencilState() const
  {
    return mDepthStencilState.getDepthStencilState();
  }

  [[nodiscard]] inline const RERHI::BlendState& getBlendState() const
  {
    return mBlendState.getBlendState();
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), GraphicsPipelineState, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit GraphicsPipelineState(const GraphicsPipelineState& source) = delete;
  GraphicsPipelineState& operator =(const GraphicsPipelineState& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLenum					mOpenGLES3PrimitiveTopology;	///< OpenGL ES 3 primitive topology describing the type of primitive to render
  RERHI::RHIGraphicsProgram*	mGraphicsProgram;
  RERHI::RHIRenderPass*		mRenderPass;
  RasterizerState			mRasterizerState;
  DepthStencilState		mDepthStencilState;
  BlendState				mBlendState;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
