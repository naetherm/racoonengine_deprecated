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
#include "RERenderer/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateSignature.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
#include <RECore/Math/Math.h>
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	ShaderCombinationId GraphicsPipelineStateSignature::generateShaderCombinationId(const ShaderBlueprintResource& shaderBlueprintResource, const ShaderProperties& shaderProperties)
	{
		ShaderCombinationId shaderCombinationId(RECore::Math::FNV1a_INITIAL_HASH_32);

		{ // Apply shader blueprint resource ID
			const ShaderBlueprintResourceId shaderBlueprintResourceId = shaderBlueprintResource.getId();
			shaderCombinationId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&shaderBlueprintResourceId), sizeof(RECore::uint32), shaderCombinationId);
		}

		// Apply shader properties
		const ShaderProperties& referencedShaderProperties = shaderBlueprintResource.getReferencedShaderProperties();
		for (const ShaderProperties::Property& property : shaderProperties.getSortedPropertyVector())
		{
			// Use the additional information to reduce the shader properties in order to generate fewer combinations
			const ShaderPropertyId shaderPropertyId = property.shaderPropertyId;
			if (referencedShaderProperties.hasPropertyValue(shaderPropertyId))
			{
				// No need to check for zero-value shader properties in here, already optimized out by "RERenderer::MaterialBlueprintResource::optimizeShaderProperties()"

				// Apply shader property ID
				shaderCombinationId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&shaderPropertyId), sizeof(RECore::uint32), shaderCombinationId);

				// Apply shader property value
				shaderCombinationId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&property.value), sizeof(RECore::int32), shaderCombinationId);
			}
		}

		// Done
		return shaderCombinationId;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GraphicsPipelineStateSignature::GraphicsPipelineStateSignature(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature) :
		mMaterialBlueprintResourceId(graphicsPipelineStateSignature.mMaterialBlueprintResourceId),
		mSerializedGraphicsPipelineStateHash(graphicsPipelineStateSignature.mSerializedGraphicsPipelineStateHash),
		mShaderProperties(graphicsPipelineStateSignature.mShaderProperties),
		mGraphicsPipelineStateSignatureId(graphicsPipelineStateSignature.mGraphicsPipelineStateSignatureId),
		mShaderCombinationId{graphicsPipelineStateSignature.mShaderCombinationId[0], graphicsPipelineStateSignature.mShaderCombinationId[1], graphicsPipelineStateSignature.mShaderCombinationId[2], graphicsPipelineStateSignature.mShaderCombinationId[3], graphicsPipelineStateSignature.mShaderCombinationId[4]}
	{
		// Nothing here
	}

	GraphicsPipelineStateSignature& GraphicsPipelineStateSignature::operator=(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature)
	{
		mMaterialBlueprintResourceId = graphicsPipelineStateSignature.mMaterialBlueprintResourceId;
		mSerializedGraphicsPipelineStateHash = graphicsPipelineStateSignature.mSerializedGraphicsPipelineStateHash;
		mShaderProperties = graphicsPipelineStateSignature.mShaderProperties;
		mGraphicsPipelineStateSignatureId = graphicsPipelineStateSignature.mGraphicsPipelineStateSignatureId;
		for (RECore::uint8 i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
		{
			mShaderCombinationId[i] = graphicsPipelineStateSignature.mShaderCombinationId[i];
		}

		// Done
		return *this;
	}

	void GraphicsPipelineStateSignature::set(const MaterialBlueprintResource& materialBlueprintResource, RECore::uint32 serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties)
	{
		mMaterialBlueprintResourceId		 = materialBlueprintResource.getId();
		mSerializedGraphicsPipelineStateHash = serializedGraphicsPipelineStateHash;
		mShaderProperties					 = shaderProperties;
		mGraphicsPipelineStateSignatureId	 = RECore::Math::FNV1a_INITIAL_HASH_32;
		for (RECore::uint8 i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
		{
			mShaderCombinationId[i] = RECore::getInvalid<ShaderCombinationId>();
		}

		// Incorporate primitive hashes
		mGraphicsPipelineStateSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mMaterialBlueprintResourceId), sizeof(RECore::uint32), mGraphicsPipelineStateSignatureId);
		mGraphicsPipelineStateSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mSerializedGraphicsPipelineStateHash), sizeof(RECore::uint32), mGraphicsPipelineStateSignatureId);

		// Incorporate shader related hashes
		const ShaderBlueprintResourceManager& shaderBlueprintResourceManager = materialBlueprintResource.getResourceManager<MaterialBlueprintResourceManager>().getRenderer().getShaderBlueprintResourceManager();
		for (RECore::uint8 i = 0; i < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++i)
		{
			const ShaderBlueprintResource* shaderBlueprintResource = shaderBlueprintResourceManager.tryGetById(materialBlueprintResource.getGraphicsShaderBlueprintResourceId(static_cast<GraphicsShaderType>(i)));
			if (nullptr != shaderBlueprintResource)
			{
				const RECore::uint32 hash = mShaderCombinationId[i] = generateShaderCombinationId(*shaderBlueprintResource, mShaderProperties);
				mGraphicsPipelineStateSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&hash), sizeof(RECore::uint32), mGraphicsPipelineStateSignatureId);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
