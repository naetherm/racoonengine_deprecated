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
#include "Examples/Framework/PlatformTypes.h"
#include "Examples/Framework/ExampleBase.h"

#include <RECore/Time/Stopwatch.h>
#include <RECore/Resource/IResourceListener.h>


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
namespace RERenderer
{
	typedef RECore::uint32 MeshResourceId;	///< POD mesh resource identifier
	typedef RECore::uint32 TextureResourceId;	///< POD texture resource identifier
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mesh example
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Texture buffer object (TBO)
*    - Uniform buffer object (UBO)
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Blinn-Phong shading
*    - Albedo, normal, roughness and emissive mapping
*    - Optimization: Cache data to not bother the RHI to much
*    - Compact vertex format (32 bit texture coordinate, QTangent, 56 bytes vs. 28 bytes per vertex)
*/
class Mesh final : public ExampleBase, public RECore::IResourceListener
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	Mesh();

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~Mesh() override
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
	virtual void onUpdate() override;
	virtual void onDraw(RERHI::RHICommandBuffer& commandBuffer) override;


//[-------------------------------------------------------]
//[ Protected virtual RECore::IResourceListener methods ]
//[-------------------------------------------------------]
protected:
	inline virtual void onLoadingStateChange(const RECore::IResource&) override
	{
		// Forget about the resource group so it's rebuild
		mResourceGroup = nullptr;
		mCommandBuffer.clear();
		fillCommandBuffer();
	}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	explicit Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	void fillCommandBuffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	RERHI::RHICommandBuffer			   mCommandBuffer;				///< Command buffer
	RERHI::RHIRootSignaturePtr		   mRootSignature;				///< Root signature, can be a null pointer
	RERHI::RHIUniformBufferPtr		   mUniformBuffer;				///< Uniform buffer object (UBO), can be a null pointer
	RERHI::RHIGraphicsPipelineStatePtr mGraphicsPipelineState;		///< Graphics pipeline state object (PSO), can be a null pointer
	RERHI::RHIGraphicsProgramPtr	   mGraphicsProgram;			///< Graphics program, can be a null pointer
	RERenderer::MeshResourceId	   mMeshResourceId;				///< Mesh resource ID, can be set to invalid value
	RERenderer::TextureResourceId	   m_argb_nxaTextureResourceId;
	RERenderer::TextureResourceId    m_hr_rg_mb_nyaTextureResourceId;
	RERenderer::TextureResourceId    mEmissiveTextureResourceId;
	RERHI::RHIResourceGroupPtr		   mResourceGroup;				///< Resource group, can be a null pointer
	RERHI::RHISamplerStatePtr		   mSamplerStatePtr;			///< Sampler state, can be a null pointer
	RERHI::RHIResourceGroupPtr		   mSamplerStateGroup;			///< Sampler state resource group, can be a null pointer
	// Optimization: Cache data to not bother the RHI implementation to much
	handle mObjectSpaceToClipSpaceMatrixUniformHandle;	///< Object space to clip space matrix uniform handle, can be "NULL_HANDLE"
	handle mObjectSpaceToViewSpaceMatrixUniformHandle;	///< Object space to view space matrix uniform handle, can be "NULL_HANDLE"
	// For timing
	RECore::Stopwatch mStopwatch;		///< Stopwatch instance
	float				mGlobalTimer;	///< Global timer


};
