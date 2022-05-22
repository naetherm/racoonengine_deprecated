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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Examples/Renderer/Mesh/Mesh.h"
#include <RECore/Color/Color4.h>

#include <RERenderer/Context.h>
#include <RERenderer/IRenderer.h>
#include <RERenderer/Core/IProfiler.h>
#include <RERenderer/Resource/Mesh/MeshResource.h>
#include <RERenderer/Resource/Mesh/MeshResourceManager.h>
#include <RERenderer/Resource/Texture/TextureResource.h>
#include <RERenderer/Resource/Texture/TextureResourceManager.h>
#include <RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <glm/gtc/type_ptr.hpp>
	#include <glm/gtc/matrix_transform.hpp>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Mesh::Mesh() :
	mMeshResourceId(RECore::getInvalid<RERenderer::MeshResourceId>()),
	m_argb_nxaTextureResourceId(RECore::getInvalid<RERenderer::TextureResourceId>()),
	m_hr_rg_mb_nyaTextureResourceId(RECore::getInvalid<RERenderer::TextureResourceId>()),
	mEmissiveTextureResourceId(RECore::getInvalid<RERenderer::TextureResourceId>()),
	mObjectSpaceToClipSpaceMatrixUniformHandle(NULL_HANDLE),
	mObjectSpaceToViewSpaceMatrixUniformHandle(NULL_HANDLE),
	mGlobalTimer(0.0f)
{
	// Nothing here
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Mesh::onInitialization()
{
	// Get and check the renderer instance
	RERenderer::IRenderer& renderer = getRendererSafe();
	RERHI::RHIDynamicRHIPtr rhi(getRhi());

	// Don't create initial pipeline state caches after a material blueprint has been loaded since this example isn't using the material blueprint system
	renderer.getMaterialBlueprintResourceManager().setCreateInitialPipelineStateCaches(false);

	{
		{ // Create the root signature
			RERHI::DescriptorRangeBuilder ranges[5];
			ranges[0].initialize(RERHI::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicVs", RERHI::ShaderVisibility::VERTEX);
			ranges[1].initialize(RERHI::ResourceType::TEXTURE_2D,		0, "_argb_nxa",				RERHI::ShaderVisibility::FRAGMENT);
			ranges[2].initialize(RERHI::ResourceType::TEXTURE_2D,		1, "_hr_rg_mb_nya",			RERHI::ShaderVisibility::FRAGMENT);
      ranges[3].initialize(RERHI::ResourceType::TEXTURE_2D,		2, "EmissiveMap",			RERHI::ShaderVisibility::FRAGMENT);
      ranges[4].initializeSampler(0, RERHI::ShaderVisibility::FRAGMENT);

			RERHI::RootParameterBuilder rootParameters[2];
			rootParameters[0].initializeAsDescriptorTable(4, &ranges[0]);
      rootParameters[1].initializeAsDescriptorTable(1, &ranges[4]);

			// Setup
			RERHI::RootSignatureBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(static_cast<RECore::uint32>(GLM_COUNTOF(rootParameters)), rootParameters, 0, nullptr, RERHI::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
		}

		// Create uniform buffer
		// -> Direct3D 9 does not support uniform buffers
		// -> Direct3D 10, 11 and 12 do not support individual uniforms
		// -> The RHI is just a light weight abstraction layer, so we need to handle the differences
		// -> Allocate enough memory for two 4x4 floating point matrices
		if (0 != rhi->getCapabilities().maximumUniformBufferSize)
		{
			mUniformBuffer = renderer.getBufferManager().createUniformBuffer(2 * 4 * 4 * sizeof(float), nullptr, RERHI::BufferUsage::DYNAMIC_DRAW);
		}

		// Vertex input layout
		static constexpr RERHI::VertexAttribute vertexAttributesLayout[] =
		{
			{ // Attribute 0
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_3,		// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"Position",									// name[32] (char)
				"POSITION",									// semanticName[32] (char)
				0,											// semanticIndex (RECore::uint32)
				// Data source
				0,											// inputSlot (RECore::uint32)
				0,											// alignedByteOffset (RECore::uint32)
				sizeof(float) * 5 + sizeof(short) * 4,		// strideInBytes (RECore::uint32)
				0											// instancesPerElement (RECore::uint32)
			},
			{ // Attribute 1
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_2,		// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"TexCoord",									// name[32] (char)
				"TEXCOORD",									// semanticName[32] (char)
				0,											// semanticIndex (RECore::uint32)
				// Data source
				0,											// inputSlot (RECore::uint32)
				sizeof(float) * 3,							// alignedByteOffset (RECore::uint32)
				sizeof(float) * 5 + sizeof(short) * 4,		// strideInBytes (RECore::uint32)
				0											// instancesPerElement (RECore::uint32)
			},
			{ // Attribute 2
				// Data destination
				RERHI::VertexAttributeFormat::SHORT_4,		// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"QTangent",									// name[32] (char)
				"TEXCOORD",									// semanticName[32] (char)
				1,											// semanticIndex (RECore::uint32)
				// Data source
				0,											// inputSlot (RECore::uint32)
				sizeof(float) * 5,							// alignedByteOffset (RECore::uint32)
				sizeof(float) * 5 + sizeof(short) * 4,		// strideInBytes (RECore::uint32)
				0											// instancesPerElement (RECore::uint32)
			}
		};
		const RERHI::VertexAttributes vertexAttributes(static_cast<RECore::uint32>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

		{ // Create sampler state and wrap it into a resource group instance
			RERHI::SamplerState samplerStateSettings = RERHI::RHISamplerState::getDefaultSamplerState();
			samplerStateSettings.addressU = RERHI::TextureAddressMode::WRAP;
			samplerStateSettings.addressV = RERHI::TextureAddressMode::WRAP;
			RERHI::RHIResource* samplerStateResource = mSamplerStatePtr = rhi->createSamplerState(samplerStateSettings);
			mSamplerStateGroup = mRootSignature->createResourceGroup(1, 1, &samplerStateResource);
		}

		{ // Create the graphics program
			// Get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
			#include "Examples/Renderer/Mesh/Mesh_GLSL_450.h"	// For Vulkan
			#include "Examples/Renderer/Mesh/Mesh_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
			#include "Examples/Renderer/Mesh/Mesh_GLSL_ES3.h"
			#include "Examples/Renderer/Mesh/Mesh_HLSL_D3D9.h"
			#include "Examples/Renderer/Mesh/Mesh_HLSL_D3D10_D3D11_D3D12.h"
			#include "Examples/Renderer/Mesh/Mesh_Null.h"

			// Create the graphics program
			RERHI::RHIShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
			mGraphicsProgram = shaderLanguage.createGraphicsProgram(
				*mRootSignature,
				vertexAttributes,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
		}

		// Is there a valid graphics program?
		if (nullptr != mGraphicsProgram)
		{
			// Create the graphics pipeline state object (PSO)
			mGraphicsPipelineState = rhi->createGraphicsPipelineState(RERHI::GraphicsPipelineStateBuilder(mRootSignature, mGraphicsProgram, vertexAttributes, getMainRenderTarget()->getRenderPass()));

			// Optimization: Cached data to not bother the RHI too much
			if (nullptr == mUniformBuffer)
			{
				mObjectSpaceToClipSpaceMatrixUniformHandle = mGraphicsProgram->getUniformHandle("ObjectSpaceToClipSpaceMatrix");
				mObjectSpaceToViewSpaceMatrixUniformHandle = mGraphicsProgram->getUniformHandle("ObjectSpaceToViewSpaceMatrix");
			}
		}

		// Create mesh instance
    renderer.getMeshResourceManager().loadMeshResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/SM_Imrod"), mMeshResourceId, this);

		{ // Load in the albedo, emissive, normal and roughness texture
			RERenderer::TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
      textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/T_Imrod_argb_nxa"),     ASSET_ID("RacoonEngine/Texture/DynamicByCode/Identity_argb_nxa2D"),     m_argb_nxaTextureResourceId, this);
      textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/T_Imrod_hr_rg_mb_nya"), ASSET_ID("RacoonEngine/Texture/DynamicByCode/Identity_hr_rg_mb_nya2D"), m_hr_rg_mb_nyaTextureResourceId, this);
      textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/T_Imrod_e"),            ASSET_ID("RacoonEngine/Texture/DynamicByCode/IdentityEmissiveMap2D"),   mEmissiveTextureResourceId, this);
		}
	}

	// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	fillCommandBuffer();
}

void Mesh::onDeinitialization()
{
	{ // Disconnect resource listeners
		const RERenderer::IRenderer& renderer = getRendererSafe();
		const RERenderer::TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
		textureResourceManager.setInvalidResourceId(m_argb_nxaTextureResourceId, *this);
		textureResourceManager.setInvalidResourceId(m_hr_rg_mb_nyaTextureResourceId, *this);
		textureResourceManager.setInvalidResourceId(mEmissiveTextureResourceId, *this);
		renderer.getMeshResourceManager().setInvalidResourceId(mMeshResourceId, *this);
	}

	// Release the used RHI resources
	mObjectSpaceToViewSpaceMatrixUniformHandle = NULL_HANDLE;
	mObjectSpaceToClipSpaceMatrixUniformHandle = NULL_HANDLE;
	mSamplerStateGroup = nullptr;
	mSamplerStatePtr = nullptr;
	mResourceGroup = nullptr;
	mGraphicsProgram = nullptr;
	mGraphicsPipelineState = nullptr;
	mUniformBuffer = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
}

void Mesh::onUpdate()
{
	// Stop the stopwatch
	mStopwatch.stop();

	// Update the global timer (FPS independent movement)
	mGlobalTimer += mStopwatch.getMilliseconds() * 0.0005f;

	// Start the stopwatch
	mStopwatch.start();
}

void Mesh::onDraw(RERHI::RHICommandBuffer& commandBuffer)
{
	{ // Set uniform
		// Get the aspect ratio
		float aspectRatio = 4.0f / 3.0f;
		{
			// Get the render target with and height
			const RERHI::RHIRenderTarget* renderTarget = getMainRenderTarget();
			if (nullptr != renderTarget)
			{
				RECore::uint32 width  = 1;
				RECore::uint32 height = 1;
				renderTarget->getWidthAndHeight(width, height);

				// Get the aspect ratio
				aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			}
		}

		// Calculate the object space to clip space matrix
		const glm::mat4 viewSpaceToClipSpace	= glm::perspective(45.0f, aspectRatio, 100.0f, 0.1f);	// Near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
		const glm::mat4 viewTranslate			= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -7.0f, 25.0f));
		const glm::mat4 worldSpaceToViewSpace	= glm::rotate(viewTranslate, mGlobalTimer, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 objectSpaceToWorldSpace	= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
			  glm::mat4 objectSpaceToViewSpace	= worldSpaceToViewSpace * objectSpaceToWorldSpace;
		const glm::mat4 objectSpaceToClipSpace	= viewSpaceToClipSpace * objectSpaceToViewSpace;

		// Upload the uniform data
		// -> Two versions: One using an uniform buffer and one setting an individual uniform
		if (nullptr != mUniformBuffer)
		{
			// Using "RERHI::Command::CopyUniformBufferData::create()" in here would be more compact to write, but would also result in copying around things inside memory, hence we're using the command buffer auxiliary memory directly in here
			struct UniformBlockDynamicVs final
			{
				float objectSpaceToClipSpaceMatrix[4 * 4];	// Object space to clip space matrix
				float objectSpaceToViewSpaceMatrix[4 * 4];	// Object space to view space matrix
			};
			RERHI::Command::CopyUniformBufferData* copyUniformBufferData = commandBuffer.addCommand<RERHI::Command::CopyUniformBufferData>(sizeof(UniformBlockDynamicVs));
			copyUniformBufferData->uniformBuffer = mUniformBuffer;
			copyUniformBufferData->numberOfBytes = sizeof(UniformBlockDynamicVs);
			UniformBlockDynamicVs* uniformBlockDynamicVS = reinterpret_cast<UniformBlockDynamicVs*>(RERHI::CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData));
			memcpy(uniformBlockDynamicVS->objectSpaceToClipSpaceMatrix, glm::value_ptr(objectSpaceToClipSpace), sizeof(float) * 4 * 4);

			// TODO(naetherm) float3x3 (currently there are alignment issues when using Direct3D, have a look into possible solutions)
			glm::mat3 objectSpaceToViewSpace3x3 = glm::mat3(objectSpaceToViewSpace);
			objectSpaceToViewSpace = glm::mat4(objectSpaceToViewSpace3x3);
			memcpy(uniformBlockDynamicVS->objectSpaceToViewSpaceMatrix, glm::value_ptr(objectSpaceToViewSpace), sizeof(float) * 4 * 4);
		}
		else
		{
			// Set legacy uniforms
			RERHI::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, glm::value_ptr(objectSpaceToClipSpace));
			RERHI::Command::SetUniform::createMatrix3fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToViewSpaceMatrixUniformHandle, glm::value_ptr(glm::mat3(objectSpaceToViewSpace)));
		}
	}

	// Dispatch pre-recorded command buffer, in case the referenced assets have already been loaded and the command buffer has been filled as a consequence
	if (!mCommandBuffer.isEmpty())
	{
		RERHI::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
	}
}

void Mesh::fillCommandBuffer()
{
	const RERenderer::IRenderer& renderer = getRendererSafe();
	const RERenderer::MeshResource* meshResource = renderer.getMeshResourceManager().tryGetById(mMeshResourceId);
	if (nullptr != meshResource && nullptr != meshResource->getVertexArrayPtr())
	{
		// Due to background texture loading, some textures might not be ready yet resulting in fallback texture usage
		// -> "Mesh::onLoadingStateChange()" will invalidate the resource group as soon as a texture resource finishes loading 
		if (nullptr == mResourceGroup)
		{
			const RERenderer::TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
			const RERenderer::TextureResource* _argb_nxaTextureResource = textureResourceManager.tryGetById(m_argb_nxaTextureResourceId);
			const RERenderer::TextureResource* _hr_rg_mb_nyaTextureResource = textureResourceManager.tryGetById(m_hr_rg_mb_nyaTextureResourceId);
			const RERenderer::TextureResource* emissiveTextureResource = textureResourceManager.tryGetById(mEmissiveTextureResourceId);
			if (nullptr == _argb_nxaTextureResource || nullptr == _argb_nxaTextureResource->getTexturePtr() ||
				nullptr == _hr_rg_mb_nyaTextureResource || nullptr == _hr_rg_mb_nyaTextureResource->getTexturePtr() ||
				nullptr == emissiveTextureResource || nullptr == emissiveTextureResource->getTexturePtr())
			{
				return;
			}

			// Create resource group
			RERHI::RHIResource* resources[4] = { mUniformBuffer, _argb_nxaTextureResource->getTexturePtr(), _hr_rg_mb_nyaTextureResource->getTexturePtr(), emissiveTextureResource->getTexturePtr() };
			RERHI::RHISamplerState* samplerStates[4] = { nullptr, mSamplerStatePtr, mSamplerStatePtr, mSamplerStatePtr };
			mResourceGroup = mRootSignature->createResourceGroup(0, static_cast<RECore::uint32>(GLM_COUNTOF(resources)), resources, samplerStates);
		}

		// Render frame
		if (nullptr != mGraphicsPipelineState)
		{
			// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
			RENDERER_SCOPED_PROFILER_EVENT(renderer.getContext(), mCommandBuffer, "Mesh")

			// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
			RERHI::Command::ClearGraphics::create(mCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);

			// Set the used graphics root signature
			RERHI::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

			// Set the used graphics pipeline state object (PSO)
			RERHI::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

			// Set graphics resource groups
			RERHI::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mResourceGroup);
			RERHI::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mSamplerStateGroup);

			{ // Draw mesh instance
				// Input assembly (IA): Set the used vertex array
				RERHI::Command::SetGraphicsVertexArray::create(mCommandBuffer, meshResource->getVertexArrayPtr());

				// Render the specified geometric primitive, based on indexing into an array of vertices
				RERHI::Command::DrawIndexedGraphics::create(mCommandBuffer, meshResource->getNumberOfIndices());
			}
		}
	}
}
