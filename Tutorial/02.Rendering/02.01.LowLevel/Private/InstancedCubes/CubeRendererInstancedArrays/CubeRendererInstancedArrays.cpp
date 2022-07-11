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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.h"
#include "InstancedCubes/CubeRendererInstancedArrays/BatchInstancedArrays.h"
#include <RECore/Log/Log.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <glm/glm.hpp>
PRAGMA_WARNING_POP

#include <math.h>
#include <float.h>	// For "FLT_MAX"
#include <stdlib.h>	// For "rand()"


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		// Vertex input layout
		static constexpr RERHI::VertexAttribute CubeRendererInstancedArraysVertexAttributesLayout[] =
		{
			// Mesh data
			{ // Attribute 0
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"Position",								// name[32] (char)
				"POSITION",								// semanticName[32] (char)
				0,										// semanticIndex (RECore::uint32)
				// Data source
				0,										// inputSlot (RECore::uint32)
				0,										// alignedByteOffset (RECore::uint32)
				sizeof(float) * 8,						// strideInBytes (RECore::uint32)
				0										// instancesPerElement (RECore::uint32)
			},
			{ // Attribute 1
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"TexCoord",								// name[32] (char)
				"TEXCOORD",								// semanticName[32] (char)
				0,										// semanticIndex (RECore::uint32)
				// Data source
				0,										// inputSlot (RECore::uint32)
				sizeof(float) * 3,						// alignedByteOffset (RECore::uint32)
				sizeof(float) * 8,						// strideInBytes (RECore::uint32)
				0										// instancesPerElement (RECore::uint32)
			},
			{ // Attribute 2
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"Normal",								// name[32] (char)
				"NORMAL",								// semanticName[32] (char)
				0,										// semanticIndex (RECore::uint32)
				// Data source
				0,										// inputSlot (RECore::uint32)
				sizeof(float) * 5,						// alignedByteOffset (RECore::uint32)
				sizeof(float) * 8,						// strideInBytes (RECore::uint32)
				0										// instancesPerElement (RECore::uint32)
			},

			// Per-instance data
			{ // Attribute 3
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_4,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"PerInstancePositionTexture",			// name[32] (char)
				"TEXCOORD",								// semanticName[32] (char)
				1,										// semanticIndex (RECore::uint32)
				// Data source
				1,										// inputSlot (RECore::uint32)
				0,										// alignedByteOffset (RECore::uint32)
				sizeof(float) * 8,						// strideInBytes (RECore::uint32)
				1										// instancesPerElement (RECore::uint32)
			},
			{ // Attribute 4
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_4,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"PerInstanceRotationScale",				// name[32] (char)
				"TEXCOORD",								// semanticName[32] (char)
				2,										// semanticIndex (RECore::uint32)
				// Data source
				1,										// inputSlot (RECore::uint32)
				sizeof(float) * 4,						// alignedByteOffset (RECore::uint32)
				sizeof(float) * 8,						// strideInBytes (RECore::uint32)
				1										// instancesPerElement (RECore::uint32)
			}
		};
		const RERHI::VertexAttributes CubeRendererInstancedArraysVertexAttributes(static_cast<RECore::uint32>(GLM_COUNTOF(CubeRendererInstancedArraysVertexAttributesLayout)), CubeRendererInstancedArraysVertexAttributesLayout);


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
CubeRendererInstancedArrays::CubeRendererInstancedArrays(RERHI::RHIDynamicRHI& rhi, RERHI::RHIRenderPass& renderPass, RECore::uint32 numberOfTextures, RECore::uint32 sceneRadius) :
	mRhi(&rhi),
	mRenderPass(renderPass),
	mNumberOfTextures(numberOfTextures),
	mSceneRadius(sceneRadius),
	mMaximumNumberOfInstancesPerBatch(0),
	mNumberOfBatches(0),
	mBatches(nullptr)
{
	// Create the buffer and texture manager
	mBufferManager = mRhi->createBufferManager();
	mTextureManager = mRhi->createTextureManager();

	// Check number of textures (limit of this implementation and RHI limit)
	if (mNumberOfTextures > MAXIMUM_NUMBER_OF_TEXTURES)
	{
		mNumberOfTextures = MAXIMUM_NUMBER_OF_TEXTURES;
	}

	// Get the maximum number of instances per batch
	// -> When using instanced arrays, the limit is the available memory for a vertex buffer
	// -> To be on the safe side and not bumping into a limitation of less capable cards we set a decent maximum number of instances per batch
	mMaximumNumberOfInstancesPerBatch = 65536;

	{ // Create the root signature
		RERHI::DescriptorRangeBuilder ranges[5];
		ranges[0].initialize(RERHI::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticVs",  RERHI::ShaderVisibility::VERTEX);
		ranges[1].initialize(RERHI::ResourceType::UNIFORM_BUFFER, 1, "UniformBlockDynamicVs", RERHI::ShaderVisibility::VERTEX);
		ranges[2].initialize(RERHI::ResourceType::TEXTURE_2D,		0, "AlbedoMap",				RERHI::ShaderVisibility::FRAGMENT);
		ranges[3].initialize(RERHI::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicFs", RERHI::ShaderVisibility::FRAGMENT);
		ranges[4].initializeSampler(0, RERHI::ShaderVisibility::FRAGMENT);

		RERHI::RootParameterBuilder rootParameters[3];
		rootParameters[0].initializeAsDescriptorTable(2, &ranges[0]);
		rootParameters[1].initializeAsDescriptorTable(2, &ranges[2]);
		rootParameters[2].initializeAsDescriptorTable(1, &ranges[4]);

		// Setup
		RERHI::RootSignatureBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(static_cast<RECore::uint32>(GLM_COUNTOF(rootParameters)), rootParameters, 0, nullptr, RERHI::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		mRootSignature = mRhi->createRootSignature(rootSignatureBuilder);
	}

	{ // Create the textures
		static constexpr RECore::uint32 TEXTURE_WIDTH   = 128;
		static constexpr RECore::uint32 TEXTURE_HEIGHT  = 128;
		static constexpr RECore::uint32 NUMBER_OF_BYTES = TEXTURE_WIDTH * TEXTURE_HEIGHT * 4;

		// Allocate memory for the 2D texture
		RECore::uint8* data = new RECore::uint8[NUMBER_OF_BYTES * mNumberOfTextures];

		{ // Fill the texture content
			// TODO(naetherm) Be a little bit more creative while filling the texture data
			RECore::uint8* RESTRICT dataCurrent = data;
			const float colors[][MAXIMUM_NUMBER_OF_TEXTURES] =
			{
				{ 1.0f, 0.0f, 0.0f},
				{ 0.0f, 0.1f, 0.0f},
				{ 0.0f, 0.0f, 0.1f},
				{ 0.5f, 0.5f, 0.5f},
				{ 1.0f, 1.0f, 1.0f},
				{ 0.1f, 0.2f, 0.2f},
				{ 0.2f, 0.5f, 0.5f},
				{ 0.1f, 0.8f, 0.2f}
			};
			for (RECore::uint32 j = 0; j < mNumberOfTextures; ++j)
			{
				// Random content
				for (RECore::uint32 i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; ++i)
				{
					*dataCurrent = static_cast<RECore::uint8>(static_cast<float>(rand() % 255) * colors[j][0]);
					++dataCurrent;
					*dataCurrent = static_cast<RECore::uint8>(static_cast<float>(rand() % 255) * colors[j][1]);
					++dataCurrent;
					*dataCurrent = static_cast<RECore::uint8>(static_cast<float>(rand() % 255) * colors[j][2]);
					++dataCurrent;
					*dataCurrent = 255;
					++dataCurrent;
				}
			}
		}

		// Create the texture instance
		// -> This implementation has to support Direct3D 9 which has no 2D array textures
		// -> We simply create a single simple 2D texture atlas with the textures aligned along the vertical axis
		mTexture2D = mTextureManager->createTexture2D(TEXTURE_WIDTH, TEXTURE_HEIGHT * mNumberOfTextures, RERHI::TextureFormat::R8G8B8A8, data, RERHI::TextureFlag::GENERATE_MIPMAPS | RERHI::TextureFlag::SHADER_RESOURCE);

		// Free texture memory
		delete [] data;
	}

	// Create sampler state instance and wrap it into a resource group instance
	RERHI::RHIResource* samplerStateResource = mRhi->createSamplerState(RERHI::RHISamplerState::getDefaultSamplerState());
	mSamplerStateGroup = mRootSignature->createResourceGroup(1, 1, &samplerStateResource);

	// Uniform buffer object (UBO, "constant buffer" in Direct3D terminology) supported?
	// -> If they are there, we really want to use them (performance and ease of use)
	if (mRhi->getCapabilities().maximumUniformBufferSize > 0)
	{
		{ // Create and set constant graphics program uniform buffer at once
			// TODO(naetherm) Ugly fixed hacked in model-view-projection matrix
			// TODO(naetherm) OpenGL matrix, Direct3D has minor differences within the projection matrix we have to compensate
			static constexpr float MVP[] =
			{
				 1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				 0.0f,			 1.9776078f,	-0.57472473f,	-0.573576453f,
				-1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				 0.0f,			 0.0f,			 9.8198195f,	 10.0f
			};
			mUniformBufferStaticVs = mBufferManager->createUniformBuffer(sizeof(MVP), MVP);
		}

		// Create dynamic uniform buffers
		mUniformBufferDynamicVs = mBufferManager->createUniformBuffer(sizeof(float) * 2, nullptr, RERHI::BufferUsage::DYNAMIC_DRAW);
		mUniformBufferDynamicFs = mBufferManager->createUniformBuffer(sizeof(float) * 3, nullptr, RERHI::BufferUsage::DYNAMIC_DRAW);
	}

	{ // Create resource group with vertex shader visibility
		RERHI::RHIResource* resources[2] = { mUniformBufferStaticVs, mUniformBufferDynamicVs };
		mResourceGroupVs = mRootSignature->createResourceGroup(0, static_cast<RECore::uint32>(GLM_COUNTOF(resources)), resources);
	}

	{ // Create resource group with fragment shader visibility
		RERHI::RHIResource* resources[2] = { mTexture2D, mUniformBufferDynamicFs };
		RERHI::RHISamplerState* samplerStates[2] = { static_cast<RERHI::RHISamplerState*>(samplerStateResource), nullptr };
		mResourceGroupFs = mRootSignature->createResourceGroup(1, static_cast<RECore::uint32>(GLM_COUNTOF(resources)), resources, samplerStates);
	}

	{
		// Get the shader source code (outsourced to keep an overview)
		const char* vertexShaderSourceCode = nullptr;
		const char* fragmentShaderSourceCode = nullptr;
		#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays_GLSL_450.h"	// For Vulkan
		#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays_GLSL_140.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
		#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays_GLSL_130.h"
		#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays_GLSL_ES3.h"
		#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays_HLSL_D3D10_D3D11_D3D12.h"
		#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays_HLSL_D3D9.h"
		#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays_Null.h"

		// Create the graphics program
		RERHI::RHIShaderLanguage& shaderLanguage = mRhi->getDefaultShaderLanguage();
		mGraphicsProgram = shaderLanguage.createGraphicsProgram(
			*mRootSignature,
			detail::CubeRendererInstancedArraysVertexAttributes,
			shaderLanguage.createVertexShaderFromSourceCode(detail::CubeRendererInstancedArraysVertexAttributes, vertexShaderSourceCode),
			shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
	}

	{ // Create the vertex buffer object (VBO)
		static constexpr float VERTEX_POSITION[] =
		{
			// Front face
			// Position					TexCoord		 Normal				// Vertex ID
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 0.0f, 0.0f, 1.0f,	// 0
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,	// 1
			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		 0.0f, 0.0f, 1.0f,	// 2
			-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		 0.0f, 0.0f, 1.0f,	// 3
			// Back face
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		 0.0f, 0.0f,-1.0f,	// 4
			-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 0.0f, 0.0f,-1.0f,	// 5
			 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		 0.0f, 0.0f,-1.0f, 	// 6
			 0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		 0.0f, 0.0f,-1.0f,	// 7
			// Top face
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		 0.0f, 1.0f, 0.0f,	// 8
			-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,		 0.0f, 1.0f, 0.0f,	// 9
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		 0.0f, 1.0f, 0.0f,	// 10
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 0.0f, 1.0f, 0.0f,	// 11
			// Bottom face
			-0.5f, -0.5f, -0.5f,		1.0f, 1.0f,		 0.0f,-1.0f, 0.0f,	// 12
			 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		 0.0f,-1.0f, 0.0f,	// 13
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 0.0f,-1.0f, 0.0f,	// 14
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		 0.0f,-1.0f, 0.0f,	// 15
			// Right face
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		 1.0f, 0.0f, 0.0f,	// 16
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 1.0f, 0.0f, 0.0f,	// 17
			 0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		 1.0f, 0.0f, 0.0f,	// 18
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 1.0f, 0.0f, 0.0f,	// 19
			// Left face
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	// 20
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	// 21
			-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	// 22
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f	// 23
		};
		mVertexBuffer = mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION);
	}

	{ // Create the index buffer object (IBO)
		static constexpr RECore::uint16 INDICES[] =
		{
			// Front face	Triangle ID
			 1,  0,  2,		// 0
			 3,  2,  0,		// 1
			// Back face
			 6,  5,  4,		// 2
			 4,  7,  6,		// 3
			// Top face
			 9,  8, 10,		// 4
			11, 10,  8,		// 5
			// Bottom face
			13, 12, 14,		// 6
			15, 14, 12,		// 7
			// Right face
			17, 16, 18,		// 8
			19, 18, 16,		// 9
			// Left face
			21, 20, 22,		// 10
			23, 22, 20		// 11
		};
		mIndexBuffer = mBufferManager->createIndexBuffer(sizeof(INDICES), INDICES);
	}
}

CubeRendererInstancedArrays::~CubeRendererInstancedArrays()
{
	// The RHI resource pointers are released automatically

	// Destroy the batches, if needed
	delete [] mBatches;
}


//[-------------------------------------------------------]
//[ Public virtual ICubeRenderer methods                  ]
//[-------------------------------------------------------]
void CubeRendererInstancedArrays::setNumberOfCubes(RECore::uint32 numberOfCubes)
{
	// Destroy previous batches, in case there are any
	if (nullptr != mBatches)
	{
		delete [] mBatches;
		mNumberOfBatches = 0;
		mBatches = nullptr;
	}

	// A third of the cubes should be rendered using alpha blending
	const RECore::uint32 numberOfTransparentCubes = numberOfCubes / 3;
	const RECore::uint32 numberOfSolidCubes       = numberOfCubes - numberOfTransparentCubes;

	// There's a limitation how many instances can be created per draw call
	// -> If required, create multiple batches
	const RECore::uint32 numberOfSolidBatches       = static_cast<RECore::uint32>(ceil(static_cast<float>(numberOfSolidCubes)       / static_cast<float>(mMaximumNumberOfInstancesPerBatch)));
	const RECore::uint32 numberOfTransparentBatches = static_cast<RECore::uint32>(ceil(static_cast<float>(numberOfTransparentCubes) / static_cast<float>(mMaximumNumberOfInstancesPerBatch)));

	// Create a batch instances
	mNumberOfBatches = numberOfSolidBatches + numberOfTransparentBatches;
	mBatches = new BatchInstancedArrays[mNumberOfBatches];

	// Initialize the solid batches
	BatchInstancedArrays* batch     = mBatches;
	BatchInstancedArrays* lastBatch = mBatches + numberOfSolidBatches;
	for (int remaningNumberOfCubes = static_cast<int>(numberOfSolidCubes); batch < lastBatch; ++batch, remaningNumberOfCubes -= mMaximumNumberOfInstancesPerBatch)
	{
		const RECore::uint32 currentNumberOfCubes = (remaningNumberOfCubes > static_cast<int>(mMaximumNumberOfInstancesPerBatch)) ? mMaximumNumberOfInstancesPerBatch : remaningNumberOfCubes;
		batch->initialize(*mBufferManager, *mRootSignature, detail::CubeRendererInstancedArraysVertexAttributes, *mVertexBuffer, *mIndexBuffer, *mGraphicsProgram, mRenderPass, currentNumberOfCubes, false, mNumberOfTextures, mSceneRadius);
	}

	// Initialize the transparent batches
	// -> TODO(naetherm) For correct alpha blending, the transparent instances should be sorted from back to front.
	lastBatch = batch + numberOfTransparentBatches;
	for (int remaningNumberOfCubes = static_cast<int>(numberOfTransparentCubes); batch < lastBatch; ++batch, remaningNumberOfCubes -= mMaximumNumberOfInstancesPerBatch)
	{
		const RECore::uint32 currentNumberOfCubes = (remaningNumberOfCubes > static_cast<int>(mMaximumNumberOfInstancesPerBatch)) ? mMaximumNumberOfInstancesPerBatch : remaningNumberOfCubes;
		batch->initialize(*mBufferManager, *mRootSignature, detail::CubeRendererInstancedArraysVertexAttributes, *mVertexBuffer, *mIndexBuffer, *mGraphicsProgram, mRenderPass, currentNumberOfCubes, true, mNumberOfTextures, mSceneRadius);
	}

	// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	mCommandBuffer.clear();
	fillReusableCommandBuffer();
}

void CubeRendererInstancedArrays::fillCommandBuffer(float globalTimer, float globalScale, float lightPositionX, float lightPositionY, float lightPositionZ, RERHI::RHICommandBuffer& commandBuffer)
{
	// Sanity check
	RHI_ASSERT(mRhi->getContext(), nullptr != mGraphicsProgram, "Invalid graphics program")

	{ // Update graphics program uniform data
		// Some counting timer, we don't want to touch the buffers on the GPU
		const float timerAndGlobalScale[] = { globalTimer, globalScale };

		// Animate the point light world space position
		const float lightPosition[] = { lightPositionX, lightPositionY, lightPositionZ };

		// Use uniform buffer?
		if (nullptr != mUniformBufferDynamicVs)
		{
			// Copy data into the uniform buffer
			RERHI::Command::CopyUniformBufferData::create(commandBuffer, *mUniformBufferDynamicVs, timerAndGlobalScale, sizeof(timerAndGlobalScale));
			if (nullptr != mUniformBufferDynamicFs)
			{
				RERHI::Command::CopyUniformBufferData::create(commandBuffer, *mUniformBufferDynamicFs, lightPosition, sizeof(lightPosition));
			}
		}
		else
		{
			// Set individual graphics program uniforms
			// -> Using uniform buffers (aka constant buffers in Direct3D) would be more efficient, but Direct3D 9 doesn't support it (neither does e.g. OpenGL ES 3.0)
			// -> To keep it simple in here, I just use a less performant string to identity the uniform (does not really hurt in here)
			RERHI::Command::SetUniform::create2fv(commandBuffer, *mGraphicsProgram, mGraphicsProgram->getUniformHandle("TimerAndGlobalScale"), timerAndGlobalScale);
			RERHI::Command::SetUniform::create3fv(commandBuffer, *mGraphicsProgram, mGraphicsProgram->getUniformHandle("LightPosition"), lightPosition);
		}
	}

	// Set constant graphics program uniform
	if (nullptr == mUniformBufferStaticVs)
	{
		// TODO(naetherm) Ugly fixed hacked in model-view-projection matrix
		// TODO(naetherm) OpenGL matrix, Direct3D has minor differences within the projection matrix we have to compensate
		static constexpr float MVP[] =
		{
			 1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
			 0.0f,			 1.9776078f,	-0.57472473f,	-0.573576453f,
			-1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
			 0.0f,			 0.0f,			 9.8198195f,	 10.0f
		};

		// There's no uniform buffer: We have to set individual uniforms
		RERHI::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mGraphicsProgram->getUniformHandle("MVP"), MVP);
	}

	// Dispatch pre-recorded command buffer
	RERHI::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void CubeRendererInstancedArrays::fillReusableCommandBuffer()
{
	// Sanity checks
	ASSERT(nullptr != mRhi, "Invalid RHI instance")
	RHI_ASSERT(mRhi->getContext(), mCommandBuffer.isEmpty(), "The command buffer is already filled")
	RHI_ASSERT(mRhi->getContext(), nullptr != mRootSignature, "Invalid root signature")
	RHI_ASSERT(mRhi->getContext(), nullptr != mTexture2D, "Invalid texture 2D")
	RHI_ASSERT(mRhi->getContext(), 0 == mRhi->getCapabilities().maximumUniformBufferSize || nullptr != mUniformBufferStaticVs, "Invalid uniform buffer static VS")
	RHI_ASSERT(mRhi->getContext(), 0 == mRhi->getCapabilities().maximumUniformBufferSize || nullptr != mUniformBufferDynamicVs, "Invalid uniform buffer dynamic VS")
	RHI_ASSERT(mRhi->getContext(), 0 == mRhi->getCapabilities().maximumUniformBufferSize || nullptr != mUniformBufferDynamicFs, "Invalid uniform buffer dynamic FS")
	RHI_ASSERT(mRhi->getContext(), nullptr != mResourceGroupVs && nullptr != mResourceGroupFs, "Invalid resource group")
	RHI_ASSERT(mRhi->getContext(), nullptr != mSamplerStateGroup, "Invalid sampler state group")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Set the used graphics root signature
	RERHI::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set resource groups
	RERHI::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mResourceGroupVs);
	RERHI::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mResourceGroupFs);
	RERHI::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 2, mSamplerStateGroup);

	// Draw the batches
	if (nullptr != mBatches)
	{
		// Loop though all batches
		BatchInstancedArrays* batch     = mBatches;
		BatchInstancedArrays* lastBatch = mBatches + mNumberOfBatches;
		for (; batch < lastBatch; ++batch)
		{
			// Draw this batch
			batch->fillCommandBuffer(mCommandBuffer);
		}
	}
}
