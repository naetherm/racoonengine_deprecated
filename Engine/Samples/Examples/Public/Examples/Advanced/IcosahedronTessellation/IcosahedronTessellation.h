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
#include "Examples/Framework/ExampleBase.h"

#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    More advanced tessellation example
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Uniform buffer object (UBO)
*    - Vertex shader (VS), tessellation control shader (TCS), tessellation evaluation shader (TES), geometry shader (GS) and fragment shader (FS)
*/
class IcosahedronTessellation final : public ExampleBase
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	inline IcosahedronTessellation() :
		mTessellationLevelOuter(2.0f),
		mTessellationLevelInner(3.0f)
	{
		// Nothing here
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~IcosahedronTessellation() override
	{
		// The resources are released within "onDeinitialization()"
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
public:
	virtual void onInitialization() override;
	virtual void onDeinitialization() override;
	virtual void onDraw(RERHI::RHICommandBuffer& commandBuffer) override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	void fillCommandBuffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	RERHI::RHIBufferManagerPtr			mBufferManager;				///< Buffer manager, can be a null pointer
	RERHI::RHICommandBuffer				mCommandBuffer;				///< Command buffer
	RERHI::RHIRootSignaturePtr			mRootSignature;				///< Root signature, can be a null pointer
	RERHI::RHIUniformBufferPtr			mUniformBufferDynamicTcs;	///< Dynamic tessellation control shader uniform buffer object (UBO), can be a null pointer
	RERHI::RHIResourceGroupPtr			mUniformBufferGroupTcs;		///< Uniform buffer group with tessellation control shader visibility, can be a null pointer
	RERHI::RHIResourceGroupPtr			mUniformBufferGroupTes;		///< Uniform buffer group with tessellation evaluation shader visibility, can be a null pointer
	RERHI::RHIResourceGroupPtr			mUniformBufferGroupGs;		///< Uniform buffer group with geometry visibility, can be a null pointer
	RERHI::RHIResourceGroupPtr			mUniformBufferGroupFs;		///< Uniform buffer group with fragment shader visibility, can be a null pointer
	RERHI::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;		///< Graphics pipeline state object (PSO), can be a null pointer
	RERHI::RHIVertexArrayPtr			mVertexArray;				///< Vertex array object (VAO), can be a null pointer
	float							mTessellationLevelOuter;	///< Outer tessellation level
	float							mTessellationLevelInner;	///< Inner tessellation level


};
