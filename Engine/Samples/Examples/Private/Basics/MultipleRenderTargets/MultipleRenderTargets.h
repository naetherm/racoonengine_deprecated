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
*    An example showing how to render into multiple render targets (MRT)
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - 2D texture
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Framebuffer object (FBO) used for render to texture
*    - Multiple render targets (MRT)
*/
class MultipleRenderTargets final : public ExampleBase
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	inline MultipleRenderTargets()
	{
		// Nothing here
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~MultipleRenderTargets() override
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
//[ Private definitions                                   ]
//[-------------------------------------------------------]
private:
	static constexpr RECore::uint32 TEXTURE_SIZE		 = 16;	///< Texture size
	static constexpr RECore::uint32 NUMBER_OF_TEXTURES = 2;	///< Number of textures


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	void fillCommandBuffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	RERHI::RHIBufferManagerPtr			mBufferManager;									///< Buffer manager, can be a null pointer
	RERHI::RHITextureManagerPtr			mTextureManager;								///< Texture manager, can be a null pointer
	RERHI::RHICommandBuffer				mCommandBuffer;									///< Command buffer
	RERHI::RHIRootSignaturePtr			mRootSignature;									///< Root signature, can be a null pointer
	RERHI::RHIFramebufferPtr			mFramebuffer;									///< Framebuffer object (FBO), can be a null pointer
	RERHI::RHIResourceGroupPtr			mTextureGroup;									///< Texture group, can be a null pointer
	RERHI::RHIResourceGroupPtr			mSamplerStateGroup;								///< Sampler state resource group, can be a null pointer
	RERHI::RHIGraphicsPipelineStatePtr	mGraphicsPipelineStateMultipleRenderTargets;	///< Graphics pipeline state object (PSO) multiple render targets, can be a null pointer
	RERHI::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;							///< Graphics pipeline state object (PSO), can be a null pointer
	RERHI::RHIVertexArrayPtr			mVertexArray;									///< Vertex array object (VAO), can be a null pointer


};
