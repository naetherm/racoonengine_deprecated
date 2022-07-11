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
#include "ExampleBase.h"

#ifdef RENDERER
	#include <RECore/Time/Stopwatch.h>
#endif

#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    An example showing how to work with cube textures
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Vertex array object (VAO)
*    - Cube texture and cube texture array
*    - Uniform buffer object (UBO)
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*/
class CubeTexture final : public ExampleBase
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	inline CubeTexture(Application& cApplication, const RECore::String& name)
    : ExampleBase(cApplication, name),
		mObjectSpaceToClipSpaceMatrixUniformHandle(NULL_HANDLE),
		mGlobalTimer(0.0f)
	{
		// Nothing here
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~CubeTexture() override
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
	#ifdef RENDERER
		virtual void onUpdate() override;
	#endif
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
	RERHI::RHIBufferManagerPtr			mBufferManager;			///< Buffer manager, can be a null pointer
	RERHI::RHITextureManagerPtr			mTextureManager;		///< Texture manager, can be a null pointer
	RERHI::RHICommandBuffer				mCommandBuffer;			///< Command buffer
	RERHI::RHIRootSignaturePtr			mRootSignature;			///< Root signature, can be a null pointer
	RERHI::RHIUniformBufferPtr			mUniformBuffer;			///< Uniform buffer object (UBO), can be a null pointer
	RERHI::RHIResourceGroupPtr			mTextureGroup;			///< Texture group, can be a null pointer
	RERHI::RHIResourceGroupPtr			mSamplerStateGroup;		///< Sampler state resource group, can be a null pointer
	RERHI::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;	///< Graphics pipeline state object (PSO), can be a null pointer
	RERHI::RHIGraphicsProgramPtr	    mGraphicsProgram;		///< Graphics program, can be a null pointer
	RERHI::RHIVertexArrayPtr			mVertexArray;			///< Vertex array object (VAO), can be a null pointer
	// Optimization: Cache data to not bother the RHI implementation to much
	RECore::handle mObjectSpaceToClipSpaceMatrixUniformHandle;	///< Object space to clip space matrix uniform handle, can be "NULL_HANDLE"
	// For timing
	#ifdef RENDERER
		RECore::Stopwatch mStopwatch;		///< Stopwatch instance
	#endif
	float					mGlobalTimer;	///< Global timer


};
