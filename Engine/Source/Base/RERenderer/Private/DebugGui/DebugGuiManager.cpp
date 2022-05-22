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
#include "RERenderer/DebugGui/DebugGuiManager.h"
#include "RERenderer/DebugGui/DebugGuiHelper.h"
#include "RERenderer/Resource/Texture/TextureResourceManager.h"
#include <RECore/File/IFileManager.h>
#include "RERenderer/Core/IProfiler.h"
#include "RERenderer/IRenderer.h"
#include "RERenderer/Context.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <RECore/Asset/Asset.h>
#include <RECore/Log/Log.h>
#include <RECore/Memory/Memory.h>
#include <ImGuizmo.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/detail/setup.hpp>	// For "GLM_COUNTOF()"
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static const RECore::AssetId IMGUI_GLYPH_MAP_2D(ASSET_ID("RacoonEngine/Texture/DynamicByCode/ImGuiGlyphMap2D"));

		// Vertex input layout
		static constexpr RERHI::VertexAttribute VertexAttributesLayout[] =
		{
			{ // Attribute 0
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_4,		// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"Position",									// name[32] (char)
				"POSITION",									// semanticName[32] (char)
				0,											// semanticIndex (RECore::uint32)
				// Data source
				0,											// inputSlot (RECore::uint32)
				0,											// alignedByteOffset (RECore::uint32)
				sizeof(float) * 4 + sizeof(RECore::uint8) * 4,	// strideInBytes (RECore::uint32)
				0											// instancesPerElement (RECore::uint32)
			},
			{ // Attribute 1
				// Data destination
				RERHI::VertexAttributeFormat::R8G8B8A8_UNORM,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"Color",									// name[32] (char)
				"COLOR",									// semanticName[32] (char)
				0,											// semanticIndex (RECore::uint32)
				// Data source
				0,											// inputSlot (RECore::uint32)
				sizeof(float) * 4,							// alignedByteOffset (RECore::uint32)
				sizeof(float) * 4 + sizeof(RECore::uint8) * 4,	// strideInBytes (RECore::uint32)
				0											// instancesPerElement (RECore::uint32)
			}
		};
		const RERHI::VertexAttributes VertexAttributes(static_cast<RECore::uint32>(GLM_COUNTOF(VertexAttributesLayout)), VertexAttributesLayout);


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] void* AllocFunc(size_t sz, void* user_data)
		{
			return RECore::Memory::reallocate(nullptr, 0, sz, 1);
		}

		void FreeFunc(void* ptr, void* user_data)
		{
      RECore::Memory::reallocate(ptr, 0, 0, 1);
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void DebugGuiManager::setImGuiAllocatorFunctions()
	{
		ImGui::SetAllocatorFunctions(::detail::AllocFunc, ::detail::FreeFunc, nullptr);
	}

	void DebugGuiManager::getDefaultTextureAssetIds(AssetIds& assetIds)
	{
		assetIds.push_back(::detail::IMGUI_GLYPH_MAP_2D);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void DebugGuiManager::newFrame(RERHI::RHIRenderTarget& renderTarget, CompositorWorkspaceInstance* compositorWorkspaceInstance)
	{
		// Startup the debug GUI manager now?
		if (!mIsRunning)
		{
			startup();
			mIsRunning = true;
		}

		// Call the platform specific implementation
		onNewFrame(renderTarget);

		// Start the frame
		ImGui::NewFrame();
		//ImGuizmo::BeginFrame();
		DebugGuiHelper::beginFrame();
		if (mOpenMetricsWindow)
		{	ImGui::ShowMetricsWindow(&mOpenMetricsWindow);
			DebugGuiHelper::drawMetricsWindow(mOpenMetricsWindow, compositorWorkspaceInstance);
		}
	}

	const RERHI::RHIVertexArrayPtr& DebugGuiManager::getFillVertexArrayPtr(RERHI::RHICommandBuffer* commandBuffer)
	{
		if (GImGui->Initialized)
		{
			// Ask ImGui to render into the internal command buffer and then request the resulting draw data
			ImGui::Render();
			const ImDrawData* imDrawData = ImGui::GetDrawData();
			RERHI::RHIDynamicRHI& rhi = mRenderer.getRhi();
			RERHI::RHIBufferManager& bufferManager = mRenderer.getBufferManager();

			{ // Vertex and index buffers
				// Create and grow vertex/index buffers if needed
				if (nullptr == mVertexBuffer || mNumberOfAllocatedVertices < static_cast<RECore::uint32>(imDrawData->TotalVtxCount))
				{
					mNumberOfAllocatedVertices = static_cast<RECore::uint32>(imDrawData->TotalVtxCount + 5000);	// Add some reserve to reduce reallocations
					mVertexBuffer = bufferManager.createVertexBuffer(mNumberOfAllocatedVertices * sizeof(ImDrawVert), nullptr, 0, RERHI::BufferUsage::DYNAMIC_DRAW RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
					mVertexArray = nullptr;
				}
				if (nullptr == mIndexBuffer || mNumberOfAllocatedIndices < static_cast<RECore::uint32>(imDrawData->TotalIdxCount))
				{
					mNumberOfAllocatedIndices = static_cast<RECore::uint32>(imDrawData->TotalIdxCount + 10000);	// Add some reserve to reduce reallocations
					mIndexBuffer = bufferManager.createIndexBuffer(mNumberOfAllocatedIndices * sizeof(ImDrawIdx), nullptr, 0, RERHI::BufferUsage::DYNAMIC_DRAW, RERHI::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
					mVertexArray = nullptr;
				}
				if (nullptr == mVertexArray)
				{
					RHI_ASSERT(nullptr != mVertexBuffer, "Invalid vertex buffer")
					RHI_ASSERT(nullptr != mIndexBuffer, "Invalid index buffer")

					// Create vertex array object (VAO)
					const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { mVertexBuffer };
					mVertexArray = bufferManager.createVertexArray(::detail::VertexAttributes, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, mIndexBuffer RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
				}

				{ // Copy and convert all vertices and indices into a single contiguous buffer
					RERHI::MappedSubresource vertexBufferMappedSubresource;
					if (rhi.map(*mVertexBuffer, 0, RERHI::MapType::WRITE_DISCARD, 0, vertexBufferMappedSubresource))
					{
						RERHI::MappedSubresource indexBufferMappedSubresource;
						if (rhi.map(*mIndexBuffer, 0, RERHI::MapType::WRITE_DISCARD, 0, indexBufferMappedSubresource))
						{
							ImDrawVert* imDrawVert = static_cast<ImDrawVert*>(vertexBufferMappedSubresource.data);
							ImDrawIdx* imDrawIdx = static_cast<ImDrawIdx*>(indexBufferMappedSubresource.data);
							for (int i = 0; i < imDrawData->CmdListsCount; ++i)
							{
								const ImDrawList* imDrawList = imDrawData->CmdLists[i];
								memcpy(imDrawVert, &imDrawList->VtxBuffer[0], imDrawList->VtxBuffer.size() * sizeof(ImDrawVert));
								memcpy(imDrawIdx, &imDrawList->IdxBuffer[0], imDrawList->IdxBuffer.size() * sizeof(ImDrawIdx));
								imDrawVert += imDrawList->VtxBuffer.size();
								imDrawIdx += imDrawList->IdxBuffer.size();
							}

							// Unmap the index buffer
							rhi.unmap(*mIndexBuffer, 0);
						}

						// Unmap the vertex buffer
						rhi.unmap(*mVertexBuffer, 0);
					}
				}
			}
		}

		// Done
		return mVertexArray;
	}

	void DebugGuiManager::fillGraphicsCommandBuffer(RERHI::RHICommandBuffer& commandBuffer)
	{
		if (GImGui->Initialized)
		{
			// No combined scoped profiler CPU and GPU sample as well as renderer debug event command by intent, this is something the caller has to take care of
			// RENDERER_SCOPED_PROFILER_EVENT(mRenderer.getContext(), commandBuffer, "Debug GUI")

			// Render command lists
			// -> There's no need to try to gather draw calls and batch them into multi-draw-indirect buffers, ImGui does already a pretty good job
			int vertexOffset = 0;
			int indexOffset = 0;
			const ImDrawData* imDrawData = ImGui::GetDrawData();
			for (int commandListIndex = 0; commandListIndex < imDrawData->CmdListsCount; ++commandListIndex)
			{
				const ImDrawList* imDrawList = imDrawData->CmdLists[commandListIndex];
				for (int commandIndex = 0; commandIndex < imDrawList->CmdBuffer.size(); ++commandIndex)
				{
					const ImDrawCmd* pcmd = &imDrawList->CmdBuffer[commandIndex];
					if (nullptr != pcmd->UserCallback)
					{
            RE_LOG(Info, "Callback method")
						pcmd->UserCallback(imDrawList, pcmd);
					}
					else
					{
            if (pcmd->TextureId) {
              { // Setup orthographic projection matrix into our vertex shader uniform buffer
                const ImVec2& displaySize = ImGui::GetIO().DisplaySize;
                ImVec2 texelOffset(0.0f, 0.0f);
                if (mRenderer.getRhi().getNameId() == RERHI::NameId::DIRECT3D9)
                {
                  // Take care of the Direct3D 9 half-pixel/half-texel offset
                  // -> See "Directly Mapping Texels to Pixels (Direct3D 9)" at https://docs.microsoft.com/en-gb/windows/win32/direct3d9/directly-mapping-texels-to-pixels?redirectedfrom=MSDN
                  texelOffset.x += 0.5f;
                  texelOffset.y += 0.5f;
                }
                const float l = texelOffset.x;
                const float r = displaySize.x + texelOffset.x;
                const float t = texelOffset.y;
                const float b = displaySize.y + texelOffset.y;
                const float objectSpaceToClipSpaceMatrix[4][4] =
                  {
                    { 2.0f / (r - l),	 0.0f,        0.0f, 0.0f },
                    { 0.0f,				 2.0f/(t-b),  0.0f, 0.0f },
                    { 0.0f,				 0.0f,        0.5f, 0.0f },
                    { (r + l) / (l - r), (t+b)/(b-t), 0.5f, 1.0f }
                  };
                if (nullptr != mVertexShaderUniformBuffer)
                {
                  // Copy data into the uniform buffer
                  RERHI::Command::CopyUniformBufferData::create(commandBuffer, *mVertexShaderUniformBuffer, objectSpaceToClipSpaceMatrix, sizeof(objectSpaceToClipSpaceMatrix));
                }
                else
                {
                  // Set legacy uniforms
                  RERHI::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, &objectSpaceToClipSpaceMatrix[0][0]);
                }
              }
              RERHI::RHITexture2D *tex = reinterpret_cast<RERHI::RHITexture2D *>(pcmd->TextureId);

              //RERHI::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

              // Set the used graphics pipeline state object (PSO)
              //RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

              // Set graphics resource groups
              RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, this->getResourceGroupByTexture(tex));
              RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
              //RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, tex);
            } else {

              //RERHI::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

              // Set the used graphics pipeline state object (PSO)
              //RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

              // Set graphics resource groups
              RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, this->getResourceGroupByTexture(mTexture2D));
              RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
            }

						// Set graphics scissor rectangle
						RERHI::Command::SetGraphicsScissorRectangles::create(commandBuffer, static_cast<long>(pcmd->ClipRect.x), static_cast<long>(pcmd->ClipRect.y), static_cast<long>(pcmd->ClipRect.z), static_cast<long>(pcmd->ClipRect.w));

						// Draw graphics
						RERHI::Command::DrawIndexedGraphics::create(commandBuffer, static_cast<RECore::uint32>(pcmd->ElemCount), 1, static_cast<RECore::uint32>(indexOffset), static_cast<RECore::int32>(vertexOffset));
					}
					indexOffset += pcmd->ElemCount;
				}
				vertexOffset += imDrawList->VtxBuffer.size();
			}
		}
	}

	void DebugGuiManager::fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(RERHI::RHICommandBuffer& commandBuffer, RERHI::RHITexture2D* tex)
	{
		if (GImGui->Initialized)
		{
			// No combined scoped profiler CPU and GPU sample as well as renderer debug event command by intent, this is something the caller has to take care of
			// RENDERER_SCOPED_PROFILER_EVENT(mRenderer.getContext(), commandBuffer, "Fixed debug GUI")

			// Create fixed build in RHI configuration resources, if required
			if (nullptr == mRootSignature)
			{
				createFixedBuildInRhiConfigurationResources(tex);
			}

			{ // Setup orthographic projection matrix into our vertex shader uniform buffer
				const ImVec2& displaySize = ImGui::GetIO().DisplaySize;
				ImVec2 texelOffset(0.0f, 0.0f);
				if (mRenderer.getRhi().getNameId() == RERHI::NameId::DIRECT3D9)
				{
					// Take care of the Direct3D 9 half-pixel/half-texel offset
					// -> See "Directly Mapping Texels to Pixels (Direct3D 9)" at https://docs.microsoft.com/en-gb/windows/win32/direct3d9/directly-mapping-texels-to-pixels?redirectedfrom=MSDN
					texelOffset.x += 0.5f;
					texelOffset.y += 0.5f;
				}
				const float l = texelOffset.x;
				const float r = displaySize.x + texelOffset.x;
				const float t = texelOffset.y;
				const float b = displaySize.y + texelOffset.y;
				const float objectSpaceToClipSpaceMatrix[4][4] =
				{
					{ 2.0f / (r - l),	 0.0f,        0.0f, 0.0f },
					{ 0.0f,				 2.0f/(t-b),  0.0f, 0.0f },
					{ 0.0f,				 0.0f,        0.5f, 0.0f },
					{ (r + l) / (l - r), (t+b)/(b-t), 0.5f, 1.0f }
				};
				if (nullptr != mVertexShaderUniformBuffer)
				{
					// Copy data into the uniform buffer
					RERHI::Command::CopyUniformBufferData::create(commandBuffer, *mVertexShaderUniformBuffer, objectSpaceToClipSpaceMatrix, sizeof(objectSpaceToClipSpaceMatrix));
				}
				else
				{
					// Set legacy uniforms
					RERHI::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, &objectSpaceToClipSpaceMatrix[0][0]);
				}
			}

			{ // RHI configuration
				// Set the used graphics root signature
        RERHI::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

				// Set the used graphics pipeline state object (PSO)
        RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

				// Set graphics resource groups
        RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, mResourceGroup);
        RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
			}

			// Setup input assembly (IA): Set the used vertex array
			RERHI::Command::SetGraphicsVertexArray::create(commandBuffer, getFillVertexArrayPtr(&commandBuffer));

			// Render command lists
			fillGraphicsCommandBuffer(commandBuffer);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::DebugGuiManager methods   ]
	//[-------------------------------------------------------]
	void DebugGuiManager::startup()
	{
		RHI_ASSERT(!mIsRunning, "The debug GUI manager is already running")

		{ // Create texture instance
			// Build texture atlas
			unsigned char* pixels = nullptr;
			int width = 0;
			int height = 0;
      this->embraceTheDarkness();
			//ImGui::GetIO().Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
      ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

			// Upload texture to RHI
			mTexture2D = mRenderer.getTextureManager().createTexture2D(static_cast<RECore::uint32>(width), static_cast<RECore::uint32>(height), RERHI::TextureFormat::R8G8B8A8, pixels, RERHI::TextureFlag::GENERATE_MIPMAPS | RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::DEFAULT, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Debug 2D GUI glyph texture atlas"));

			// Tell the texture resource manager about our glyph texture so it can be referenced inside e.g. compositor nodes
			mRenderer.getTextureResourceManager().createTextureResourceByAssetId(::detail::IMGUI_GLYPH_MAP_2D, *mTexture2D);
/*
      // Create sampler state instance and wrap it into a resource group instance
      RERHI::RHIResource* samplerStateResource = nullptr;
      {
        RERHI::SamplerState samplerState = RERHI::RHISamplerState::getDefaultSamplerState();
        samplerState.addressU = RERHI::TextureAddressMode::WRAP;
        samplerState.addressV = RERHI::TextureAddressMode::WRAP;
        samplerStateResource = this->mRenderer.getRhi().createSamplerState(samplerState RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
        mSamplerStateGroup = mRootSignature->createResourceGroup(1, 1, &samplerStateResource, nullptr RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
      }
      { // Create resource group
        RERHI::RHIResource *resources[2] = {mVertexShaderUniformBuffer, mTexture2D};
        RERHI::RHISamplerState *samplerStates[2] = {nullptr, static_cast<RERHI::RHISamplerState *>(samplerStateResource)};
        mResourceGroup = mRootSignature->createResourceGroup(0, static_cast<RECore::uint32>(GLM_COUNTOF(resources)), resources, samplerStates RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
        this->mTextureMaps.emplace(mTexture2D, mResourceGroup);
      }*/
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	DebugGuiManager::DebugGuiManager(IRenderer& renderer) :
		mRenderer(renderer),
		mImGuiContext(nullptr),
		mIsRunning(false),
		mObjectSpaceToClipSpaceMatrixUniformHandle(NULL_HANDLE),
		mNumberOfAllocatedVertices(0),
		mNumberOfAllocatedIndices(0),
		mOpenMetricsWindow(false)
	{
		// Set ImGui allocator functions
		setImGuiAllocatorFunctions();

		// Create ImGui context
		mImGuiContext = ImGui::CreateContext();

		// Change ImGui filenames so one is able to guess where those files come from when using RacoonEngine
		const RECore::IFileManager& fileManager = renderer.getFileManager();
		const char* localDataMountPoint = fileManager.getLocalDataMountPoint();
		ImGuiIO& imGuiIo = ImGui::GetIO();
		imGuiIo.IniFilename = nullptr;
		imGuiIo.LogFilename = nullptr;
		if (nullptr != localDataMountPoint && fileManager.createDirectories(localDataMountPoint))
		{
			// ImGui has no file system abstraction and needs absolute filenames
			const std::string virtualDebugGuiDirectoryName = localDataMountPoint;
			mAbsoluteIniFilename = fileManager.mapVirtualToAbsoluteFilename(RECore::IFileManager::FileMode::WRITE, (virtualDebugGuiDirectoryName + "/RacoonEngineImGuiLayout.ini").c_str());
			mAbsoluteLogFilename = fileManager.mapVirtualToAbsoluteFilename(RECore::IFileManager::FileMode::WRITE, (virtualDebugGuiDirectoryName + "/RacoonEngineImGuiLog.txt").c_str());
			imGuiIo.IniFilename = mAbsoluteIniFilename.c_str();
			imGuiIo.LogFilename = mAbsoluteLogFilename.c_str();
		}

		// Setup ImGui style
		ImGui::StyleColorsDark();
	}

	DebugGuiManager::~DebugGuiManager()
	{
		ImGui::DestroyContext(mImGuiContext);
	}

	void DebugGuiManager::createFixedBuildInRhiConfigurationResources(RERHI::RHITexture2D* tex)
	{
		RERHI::RHIDynamicRHI& rhi = mRenderer.getRhi();
		RHI_ASSERT(nullptr == mRootSignature, "The debug GUI manager has already root signature")

		{ // Create the root signature instance
			// Create the root signature
			RERHI::DescriptorRangeBuilder ranges[4];
			ranges[0].initialize(RERHI::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicVs", RERHI::ShaderVisibility::VERTEX);
			ranges[1].initialize(RERHI::ResourceType::TEXTURE_2D,		0, "GlyphMap",				RERHI::ShaderVisibility::FRAGMENT);
			ranges[2].initializeSampler(0, RERHI::ShaderVisibility::FRAGMENT);

			RERHI::RootParameterBuilder rootParameters[2];
			rootParameters[0].initializeAsDescriptorTable(2, &ranges[0]);
			rootParameters[1].initializeAsDescriptorTable(1, &ranges[2]);

			// Setup
			RERHI::RootSignatureBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(static_cast<RECore::uint32>(GLM_COUNTOF(rootParameters)), rootParameters, 0, nullptr, RERHI::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi.createRootSignature(rootSignatureBuilder RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
		}

		{ // Create the graphics pipeline state instance
			{ // Create the graphics program
				// Get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
				#include "RERenderer/DebugGui/Detail/Shader/DebugGui_GLSL_450.h"	// For Vulkan
				#include "RERenderer/DebugGui/Detail/Shader/DebugGui_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
				#include "RERenderer/DebugGui/Detail/Shader/DebugGui_GLSL_ES3.h"
				#include "RERenderer/DebugGui/Detail/Shader/DebugGui_HLSL_D3D9.h"
				#include "RERenderer/DebugGui/Detail/Shader/DebugGui_HLSL_D3D10_D3D11_D3D12.h"
				#include "RERenderer/DebugGui/Detail/Shader/DebugGui_Null.h"

				// Create the graphics program
				RERHI::RHIShaderLanguage& shaderLanguage = rhi.getDefaultShaderLanguage();
				mGraphicsProgram = shaderLanguage.createGraphicsProgram(
					*mRootSignature,
					::detail::VertexAttributes,
					shaderLanguage.createVertexShaderFromSourceCode(::detail::VertexAttributes, vertexShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Debug GUI")),
					shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Debug GUI"))
					RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != mGraphicsProgram)
			{
				// TODO(naetherm) Render pass related update, the render pass in here is currently just a dummy so the debug compositor works
				RERHI::RHIRenderPass* renderPass = rhi.createRenderPass(1, &rhi.getCapabilities().preferredSwapChainColorTextureFormat, rhi.getCapabilities().preferredSwapChainDepthStencilTextureFormat, 1 RHI_RESOURCE_DEBUG_NAME("Debug GUI"));

				RERHI::GraphicsPipelineState graphicsPipelineState = RERHI::GraphicsPipelineStateBuilder(mRootSignature, mGraphicsProgram, ::detail::VertexAttributes, *renderPass);
				graphicsPipelineState.rasterizerState.cullMode				 = RERHI::CullMode::NONE;
				graphicsPipelineState.rasterizerState.scissorEnable			 = 1;
				graphicsPipelineState.depthStencilState.depthEnable			 = false;
				graphicsPipelineState.depthStencilState.depthWriteMask		 = RERHI::DepthWriteMask::ZERO;
				graphicsPipelineState.blendState.renderTarget[0].blendEnable = true;
				graphicsPipelineState.blendState.renderTarget[0].srcBlend	 = RERHI::Blend::SRC_ALPHA;
				graphicsPipelineState.blendState.renderTarget[0].destBlend	 = RERHI::Blend::INV_SRC_ALPHA;
				mGraphicsPipelineState = rhi.createGraphicsPipelineState(graphicsPipelineState RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
			}
		}

		// Create vertex uniform buffer instance
		if (rhi.getCapabilities().maximumUniformBufferSize > 0)
		{
			mVertexShaderUniformBuffer = mRenderer.getBufferManager().createUniformBuffer(sizeof(float) * 4 * 4, nullptr, RERHI::BufferUsage::DYNAMIC_DRAW RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
		}
		else if (nullptr != mGraphicsProgram)
		{
			mObjectSpaceToClipSpaceMatrixUniformHandle = mGraphicsProgram->getUniformHandle("ObjectSpaceToClipSpaceMatrix");
		}

		// Create sampler state instance and wrap it into a resource group instance
		RERHI::RHIResource* samplerStateResource = nullptr;
		{
			RERHI::SamplerState samplerState = RERHI::RHISamplerState::getDefaultSamplerState();
			samplerState.addressU = RERHI::TextureAddressMode::WRAP;
			samplerState.addressV = RERHI::TextureAddressMode::WRAP;
			samplerStateResource = rhi.createSamplerState(samplerState RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
			mSamplerStateGroup = mRootSignature->createResourceGroup(1, 1, &samplerStateResource, nullptr RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
		}

		{ // Create resource group
      RERHI::RHIResource *resources[2] = {mVertexShaderUniformBuffer, mTexture2D};
      RERHI::RHISamplerState *samplerStates[2] = {nullptr, static_cast<RERHI::RHISamplerState *>(samplerStateResource)};
      mResourceGroup = mRootSignature->createResourceGroup(0, static_cast<RECore::uint32>(GLM_COUNTOF(resources)), resources, samplerStates RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
      this->mTextureMaps.emplace(mTexture2D, mResourceGroup);
		}
	}

  void DebugGuiManager::registerTexture(RERHI::RHITexture2D *texture) {

    if (nullptr != mRootSignature)
    {
      //createFixedBuildInRhiConfigurationResources();
    if (this->mTextureMaps.find(texture) == this->mTextureMaps.end()) {
      RE_LOG(Info, "Creating mapping")
      RERHI::SamplerState samplerState = RERHI::RHISamplerState::getDefaultSamplerState();
      samplerState.addressU = RERHI::TextureAddressMode::WRAP;
      samplerState.addressV = RERHI::TextureAddressMode::WRAP;
      RERHI::RHIResource* samplerStateResource = this->mRenderer.getRhi().createSamplerState(samplerState RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
      RERHI::RHIResource *resources[2] = {mVertexShaderUniformBuffer, texture};
      RERHI::RHISamplerState *samplerStates[2] = {nullptr, static_cast<RERHI::RHISamplerState *>(samplerStateResource)};
      RERHI::RHIResourceGroup* resourceGroup = mRootSignature->createResourceGroup(0, static_cast<RECore::uint32>(GLM_COUNTOF(resources)), resources, samplerStates RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
      resourceGroup->AddReference();
      // Add to map
      this->mTextureMaps.emplace(texture, resourceGroup);
    }

    }
  }

void DebugGuiManager::embraceTheDarkness()
{
  ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
  colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
  colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
  colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
  colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
  colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
  colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
  colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
  colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
  colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
  colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
  colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
  colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
  colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
  colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
  colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding                     = ImVec2(8.00f, 8.00f);
  style.FramePadding                      = ImVec2(5.00f, 2.00f);
  style.CellPadding                       = ImVec2(6.00f, 6.00f);
  style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
  style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
  style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
  style.IndentSpacing                     = 25;
  style.ScrollbarSize                     = 15;
  style.GrabMinSize                       = 10;
  style.WindowBorderSize                  = 1;
  style.ChildBorderSize                   = 1;
  style.PopupBorderSize                   = 1;
  style.FrameBorderSize                   = 1;
  style.TabBorderSize                     = 1;
  style.WindowRounding                    = 7;
  style.ChildRounding                     = 4;
  style.FrameRounding                     = 3;
  style.PopupRounding                     = 4;
  style.ScrollbarRounding                 = 9;
  style.GrabRounding                      = 3;
  style.LogSliderDeadzone                 = 4;
  style.TabRounding                       = 4;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
