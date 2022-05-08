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
#include "RERenderer/Resource/Material/MaterialTechnique.h"
#include "RERenderer/Resource/Material/MaterialResource.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/MaterialBufferManager.h"
#include "RERenderer/Resource/Texture/TextureResourceManager.h"
#include "RERenderer/Resource/Texture/TextureResource.h"
#include "RERenderer/Resource/RendererResourceManager.h"
#include <RECore/Math/Math.h>
#include "RERenderer/IRenderer.h"


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
		#define DEFINE_CONSTANT(name) static constexpr uint32_t name = STRING_ID(#name);
			// Rasterizer state
      DEFINE_CONSTANT(FillMode)
			DEFINE_CONSTANT(CullMode)
      DEFINE_CONSTANT(FrontCounterClockwise)
      DEFINE_CONSTANT(DepthBias)
      DEFINE_CONSTANT(DepthBiasClamp)
      DEFINE_CONSTANT(SlopeScaledDepthBias)
      DEFINE_CONSTANT(DepthClipEnable)
      DEFINE_CONSTANT(MultisampleEnable)
      DEFINE_CONSTANT(AntialiasedLineEnable)
      DEFINE_CONSTANT(ForcedSampleCount)
      DEFINE_CONSTANT(ConservativeRasterizationMode)
      DEFINE_CONSTANT(ScissorEnable)
			// Depth stencil state
			DEFINE_CONSTANT(DepthEnable)
			DEFINE_CONSTANT(DepthWriteMask)
      DEFINE_CONSTANT(DepthFunc)
      DEFINE_CONSTANT(StencilEnable)
      DEFINE_CONSTANT(StencilReadMask)
      DEFINE_CONSTANT(StencilWriteMask)
      DEFINE_CONSTANT(FrontFace)
      DEFINE_CONSTANT(BackFace)
			// Blend state
			DEFINE_CONSTANT(AlphaToCoverageEnable)
      DEFINE_CONSTANT(IndependentBlendEnable)
		#undef DEFINE_CONSTANT


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
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MaterialTechnique::MaterialTechnique(MaterialTechniqueId materialTechniqueId, MaterialResource& materialResource, MaterialBlueprintResourceId materialBlueprintResourceId) :
		MaterialBufferSlot(materialResource),
		mMaterialTechniqueId(materialTechniqueId),
		mMaterialBlueprintResourceId(materialBlueprintResourceId),
		mStructuredBufferRootParameterIndex(~0u),
		mSerializedGraphicsPipelineStateHash(RECore::getInvalid<uint32_t>())
	{
		MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
		if (nullptr != materialBufferManager)
		{
			materialBufferManager->requestSlot(*this);
		}

		// Calculate FNV1a hash of "RERHI::SerializedGraphicsPipelineState"
		calculateSerializedGraphicsPipelineStateHash();
	}

	MaterialTechnique::~MaterialTechnique()
	{
		// Due to hot-reloading it's possible that there's no assigned material slot, so we need to do a check here
		if (RECore::isValid(getAssignedMaterialSlot()))
		{
			MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
			ASSERT(nullptr != materialBufferManager, "Invalid material buffer manager")
			materialBufferManager->releaseSlot(*this);
		}
	}

	void MaterialTechnique::fillGraphicsCommandBuffer(const IRenderer& renderer, RERHI::RHICommandBuffer& commandBuffer, uint32_t& resourceGroupRootParameterIndex, RERHI::RHIResourceGroup** resourceGroup)
	{
		// Sanity check
		RHI_ASSERT(RECore::isValid(mMaterialBlueprintResourceId), "Invalid material blueprint resource ID")
		RHI_ASSERT(nullptr != resourceGroup, "The RHI resource group pointer must be valid")

		{ // Bind the material buffer manager
			MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
			if (nullptr != materialBufferManager)
			{
				materialBufferManager->fillGraphicsCommandBuffer(*this, commandBuffer);
			}
		}

		// Set resource group
		fillCommandBuffer(renderer, resourceGroupRootParameterIndex, resourceGroup);
	}

	void MaterialTechnique::fillComputeCommandBuffer(const IRenderer& renderer, RERHI::RHICommandBuffer& commandBuffer, uint32_t& resourceGroupRootParameterIndex, RERHI::RHIResourceGroup** resourceGroup)
	{
		// Sanity check
		RHI_ASSERT(RECore::isValid(mMaterialBlueprintResourceId), "Invalid material blueprint resource ID")
		RHI_ASSERT(nullptr != resourceGroup, "The RHI resource group pointer must be valid")

		{ // Bind the material buffer manager
			MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
			if (nullptr != materialBufferManager)
			{
				materialBufferManager->fillComputeCommandBuffer(*this, commandBuffer);
			}
		}

		// Set resource group
		fillCommandBuffer(renderer, resourceGroupRootParameterIndex, resourceGroup);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RECore::IResourceListener methods ]
	//[-------------------------------------------------------]
	void MaterialTechnique::onLoadingStateChange(const RECore::IResource&)
	{
		makeResourceGroupDirty();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MaterialBufferManager* MaterialTechnique::getMaterialBufferManager() const
	{
		// It's valid if a material blueprint resource doesn't contain a material uniform buffer (usually the case for compositor material blueprint resources)
		const MaterialBlueprintResource* materialBlueprintResource = getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
		return (nullptr != materialBlueprintResource) ? materialBlueprintResource->getMaterialBufferManager() : nullptr;
	}

	const MaterialTechnique::Textures& MaterialTechnique::getTextures(const IRenderer& renderer)
	{
		// Need for gathering the textures now?
		if (mTextures.empty())
		{
			const MaterialBlueprintResource* materialBlueprintResource = renderer.getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
			if (nullptr != materialBlueprintResource)
			{
				const MaterialResource& materialResource = getMaterialResource();
				TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
				const MaterialBlueprintResource::Textures& textures = materialBlueprintResource->getTextures();
				const size_t numberOfTextures = textures.size();
				mTextures.reserve(numberOfTextures);
				for (size_t i = 0; i < numberOfTextures; ++i)
				{
					const MaterialBlueprintResource::Texture& blueprintTexture = textures[i];

					// Start with the material blueprint textures
					Texture texture;
					texture.rootParameterIndex = blueprintTexture.rootParameterIndex;
					texture.materialProperty   = blueprintTexture.materialProperty;
					texture.textureResourceId  = blueprintTexture.textureResourceId;

					// Apply material specific modifications
					const MaterialPropertyId materialPropertyId = texture.materialProperty.getMaterialPropertyId();
					if (RECore::isValid(materialPropertyId))
					{
						// Figure out the material property value
						const MaterialProperty* materialProperty = materialResource.getPropertyById(materialPropertyId);
						if (nullptr != materialProperty)
						{
							// TODO(naetherm) Error handling: Usage mismatch etc.
							texture.materialProperty = *materialProperty;
							textureResourceManager.loadTextureResourceByAssetId(texture.materialProperty.getTextureAssetIdValue(), blueprintTexture.fallbackTextureAssetId, texture.textureResourceId, this, blueprintTexture.rgbHardwareGammaCorrection);
						}
					}

					// Insert texture
					mTextures.push_back(texture);
				}
			}
		}
		return mTextures;
	}

	void MaterialTechnique::calculateSerializedGraphicsPipelineStateHash()
	{
		const MaterialBlueprintResource* materialBlueprintResource = getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
		if (nullptr != materialBlueprintResource)
		{
			// Start with the graphics pipeline state of the material blueprint resource
			RERHI::SerializedGraphicsPipelineState serializedGraphicsPipelineState = materialBlueprintResource->getGraphicsPipelineState();

			// Apply material properties
			// -> Renderer toolkit counterpart is "RendererToolkit::JsonMaterialBlueprintHelper::readPipelineStateObject()"
			const MaterialProperties& materialBlueprintMaterialProperties = materialBlueprintResource->getMaterialProperties();
			const MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector = getMaterialResource().getSortedPropertyVector();
			const size_t numberOfMaterialProperties = sortedMaterialPropertyVector.size();
			for (size_t i = 0; i < numberOfMaterialProperties; ++i)
			{
				// A material can have multiple material techniques, do only apply material properties which are known to the material blueprint resource
				const MaterialProperty& materialProperty = sortedMaterialPropertyVector[i];
				if (nullptr != materialBlueprintMaterialProperties.getPropertyById(materialProperty.getMaterialPropertyId()))
				{
					switch (materialProperty.getUsage())
					{
						case MaterialProperty::Usage::UNKNOWN:
						case MaterialProperty::Usage::STATIC:
						case MaterialProperty::Usage::SHADER_UNIFORM:
						case MaterialProperty::Usage::SHADER_COMBINATION:
							// Nothing here
							break;

						case MaterialProperty::Usage::RASTERIZER_STATE:
							if (materialProperty.getMaterialPropertyId() == ::detail::CullMode)
							{
								serializedGraphicsPipelineState.rasterizerState.cullMode = materialProperty.getCullModeValue();
							} else if (materialProperty.getMaterialPropertyId() == ::detail::FillMode)
              {
                serializedGraphicsPipelineState.rasterizerState.fillMode = materialProperty.getFillModeValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::FrontCounterClockwise)
              {
                serializedGraphicsPipelineState.rasterizerState.frontCounterClockwise = materialProperty.getIntegerValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::DepthBias)
              {
                serializedGraphicsPipelineState.rasterizerState.depthBias = materialProperty.getIntegerValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::DepthBiasClamp)
              {
                serializedGraphicsPipelineState.rasterizerState.depthBiasClamp = materialProperty.getFloatValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::SlopeScaledDepthBias)
              {
                serializedGraphicsPipelineState.rasterizerState.slopeScaledDepthBias = materialProperty.getFloatValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::DepthClipEnable)
              {
                serializedGraphicsPipelineState.rasterizerState.depthClipEnable = materialProperty.getBooleanValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::MultisampleEnable)
              {
                serializedGraphicsPipelineState.rasterizerState.multisampleEnable = materialProperty.getBooleanValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::AntialiasedLineEnable)
              {
                serializedGraphicsPipelineState.rasterizerState.antialiasedLineEnable = materialProperty.getBooleanValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::ForcedSampleCount)
              {
                serializedGraphicsPipelineState.rasterizerState.forcedSampleCount = materialProperty.getIntegerValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::ConservativeRasterizationMode)
              {
                serializedGraphicsPipelineState.rasterizerState.conservativeRasterizationMode = materialProperty.getConservativeRasterizationModeValue();
              } else if (materialProperty.getMaterialPropertyId() == ::detail::ScissorEnable)
              {
                serializedGraphicsPipelineState.rasterizerState.scissorEnable = materialProperty.getBooleanValue();
              } else {
								RHI_ASSERT(false, "Unknown parameter for RasterizerState!")
							}
							break;

						case MaterialProperty::Usage::DEPTH_STENCIL_STATE:
							// TODO(naetherm) Implement all depth stencil state properties
							switch (materialProperty.getMaterialPropertyId())
							{
								case ::detail::DepthEnable:
									serializedGraphicsPipelineState.depthStencilState.depthEnable = materialProperty.getBooleanValue();
									break;

								case ::detail::DepthWriteMask:
									serializedGraphicsPipelineState.depthStencilState.depthWriteMask = materialProperty.getDepthWriteMaskValue();
									break;

                case ::detail::DepthFunc:
                  serializedGraphicsPipelineState.depthStencilState.depthFunc = materialProperty.getComparisonFuncValue();
                  break;

                case ::detail::StencilEnable:
                  serializedGraphicsPipelineState.depthStencilState.stencilEnable = materialProperty.getBooleanValue();
                  break;

                case ::detail::StencilReadMask:
                  serializedGraphicsPipelineState.depthStencilState.stencilReadMask = materialProperty.getIntegerValue();
                  break;

                case ::detail::StencilWriteMask:
                  serializedGraphicsPipelineState.depthStencilState.stencilWriteMask = materialProperty.getIntegerValue();
                  break;

								default:
									RHI_ASSERT(false, "TODO(naetherm) Depth stencil state not implemented, yet")
									break;
							}
							break;

						case MaterialProperty::Usage::BLEND_STATE:
							// TODO(naetherm) Implement all blend state properties
							if (materialProperty.getMaterialPropertyId() == ::detail::AlphaToCoverageEnable)
							{
								serializedGraphicsPipelineState.blendState.alphaToCoverageEnable = materialProperty.getBooleanValue();
							} else if (materialProperty.getMaterialPropertyId() == ::detail::IndependentBlendEnable)
              {
                serializedGraphicsPipelineState.blendState.independentBlendEnable = materialProperty.getBooleanValue();
              }
							else
							{
								RHI_ASSERT(false, "TODO(naetherm) Blend state not implemented, yet")
							}
							break;

						case MaterialProperty::Usage::SAMPLER_STATE:
						case MaterialProperty::Usage::TEXTURE_REFERENCE:
						case MaterialProperty::Usage::GLOBAL_REFERENCE:
						case MaterialProperty::Usage::UNKNOWN_REFERENCE:
						case MaterialProperty::Usage::PASS_REFERENCE:
						case MaterialProperty::Usage::MATERIAL_REFERENCE:
						case MaterialProperty::Usage::INSTANCE_REFERENCE:
						case MaterialProperty::Usage::GLOBAL_REFERENCE_FALLBACK:
							// Nothing here
							break;
					}
				}
			}

			// Calculate the FNV1a hash of "RERHI::SerializedGraphicsPipelineState"
			mSerializedGraphicsPipelineStateHash = RECore::Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&serializedGraphicsPipelineState), sizeof(RERHI::SerializedGraphicsPipelineState));

			// Register the FNV1a hash of "RERHI::SerializedGraphicsPipelineState" inside the material blueprint resource manager so it's sufficient to pass around the tiny hash instead the over 400 bytes full serialized pipeline state
			getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().addSerializedGraphicsPipelineState(mSerializedGraphicsPipelineStateHash, serializedGraphicsPipelineState);
		}
		else
		{
			RECore::setInvalid(mSerializedGraphicsPipelineStateHash);
		}
	}

	void MaterialTechnique::scheduleForShaderUniformUpdate()
	{
		MaterialBufferManager* materialBufferManager = getMaterialBufferManager();
		if (nullptr != materialBufferManager)
		{
			materialBufferManager->scheduleForUpdate(*this);
		}
	}

	void MaterialTechnique::fillCommandBuffer(const IRenderer& renderer, uint32_t& resourceGroupRootParameterIndex, RERHI::RHIResourceGroup** resourceGroup)
	{
		// Set textures
		const Textures& textures = getTextures(renderer);
		if (textures.empty())
		{
			if (nullptr != mStructuredBufferPtr)
			{
				// Create resource group, if needed
				if (nullptr == mResourceGroup)
				{
					// Get material blueprint resource
					const MaterialBlueprintResource* materialBlueprintResource = getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
					RHI_ASSERT(nullptr != materialBlueprintResource, "Invalid material blueprint resource")

					// Create texture resource group: First entry is the structured buffer
					std::vector<RERHI::RHIResource*> resources;
					std::vector<RERHI::RHISamplerState*> samplerStates;
					resources.resize(1);
					samplerStates.resize(1);
					resources[0] = mStructuredBufferPtr;
					samplerStates[0] = nullptr;
					mResourceGroup = renderer.getRendererResourceManager().createResourceGroup(*materialBlueprintResource->getRootSignaturePtr(), mStructuredBufferRootParameterIndex, static_cast<uint32_t>(resources.size()), resources.data(), samplerStates.data() RHI_RESOURCE_DEBUG_NAME("Material technique"));
				}

				// Tell the caller about the resource group
				resourceGroupRootParameterIndex = mStructuredBufferRootParameterIndex;
				*resourceGroup = mResourceGroup;
			}
			else
			{
				RECore::setInvalid(resourceGroupRootParameterIndex);
				*resourceGroup = nullptr;
			}
		}
		else
		{
			// Create resource group, if needed
			if (nullptr == mResourceGroup)
			{
				// Check texture resources
				const size_t numberOfTextures = textures.size();
				const TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
				for (size_t i = 0; i < numberOfTextures; ++i)
				{
					// Due to background texture loading, some textures might not be ready, yet
					// -> But even in this situation there should be a decent fallback texture in place
					const Texture& texture = textures[i];
					TextureResource* textureResource = textureResourceManager.tryGetById(texture.textureResourceId);
					if (nullptr == textureResource)
					{
						// Maybe it's a dynamically created texture like a shadow map created by "RERenderer::CompositorInstancePassShadowMap"
						// which might not have been ready yet when the material was originally loaded
						textureResource = textureResourceManager.getTextureResourceByAssetId(texture.materialProperty.getTextureAssetIdValue());
						if (nullptr != textureResource)
						{
							mTextures[i].textureResourceId = textureResource->getId();
						}
					}
					if (nullptr != textureResource)
					{
						// We also need to get informed in case e.g. dynamic compositor textures get changed in order to update the texture resource group accordantly
						textureResource->connectResourceListener(*this);
					}
				}

				// Get material blueprint resource
				const MaterialBlueprintResource* materialBlueprintResource = getMaterialResourceManager().getRenderer().getMaterialBlueprintResourceManager().tryGetById(mMaterialBlueprintResourceId);
				RHI_ASSERT(nullptr != materialBlueprintResource, "Invalid material blueprint resource")

				// Create texture resource group
				std::vector<RERHI::RHIResource*> resources;
				std::vector<RERHI::RHISamplerState*> samplerStates;
				uint32_t textureStartIndex = 0;
				if (nullptr != mStructuredBufferPtr)
				{
					// Sanity check
					// TODO(naetherm) All resources need to be inside the same resource group, this needs to be guaranteed by design
					RHI_ASSERT(mStructuredBufferRootParameterIndex == textures[0].rootParameterIndex, "Invalid structured buffer root parameter index")

					// First entry is the structured buffer
					resources.resize(numberOfTextures + 1);
					samplerStates.resize(numberOfTextures + 1);
					resources[0] = mStructuredBufferPtr;
					samplerStates[0] = nullptr;
					textureStartIndex = 1;
				}
				else
				{
					resources.resize(numberOfTextures);
					samplerStates.resize(numberOfTextures);
				}
				const MaterialBlueprintResource::Textures& materialBlueprintResourceTextures = materialBlueprintResource->getTextures();
				const MaterialBlueprintResource::SamplerStates& materialBlueprintResourceSamplerStates = materialBlueprintResource->getSamplerStates();
				for (size_t i = 0; i < numberOfTextures; ++i)
				{
					// Set texture resource
					TextureResource* textureResource = textureResourceManager.tryGetById(textures[i].textureResourceId);
					RHI_ASSERT(nullptr != textureResource, "Invalid texture resource")
					resources[i + textureStartIndex] = textureResource->getTexturePtr();
					RHI_ASSERT(nullptr != resources[i + textureStartIndex], "Invalid resource")

					// Set sampler state, if there's one (e.g. texel fetch instead of sampling might be used)
					if (RECore::isValid(materialBlueprintResourceTextures[i].samplerStateIndex))
					{
						RHI_ASSERT(materialBlueprintResourceTextures[i].samplerStateIndex < materialBlueprintResourceSamplerStates.size(), "Invalid sampler state index")
						samplerStates[i + textureStartIndex] = materialBlueprintResourceSamplerStates[materialBlueprintResourceTextures[i].samplerStateIndex].samplerStatePtr;
					}
					else
					{
						samplerStates[i + textureStartIndex] = nullptr;
					}
				}
				// TODO(naetherm) All resources need to be inside the same resource group, this needs to be guaranteed by design
				mResourceGroup = renderer.getRendererResourceManager().createResourceGroup(*materialBlueprintResource->getRootSignaturePtr(), textures[0].rootParameterIndex, static_cast<uint32_t>(resources.size()), resources.data(), samplerStates.data() RHI_RESOURCE_DEBUG_NAME("Material technique"));
			}

			// Tell the caller about the resource group
			resourceGroupRootParameterIndex = textures[0].rootParameterIndex;
			*resourceGroup = mResourceGroup;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
